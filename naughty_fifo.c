/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_fifo.c
 * @ Author: carl
 * @ Created Time: 2021-8月-04(星期三) 07:35:27
 * @ All Rights Reserved
 * *****************************************************************************/

#include "naughty_fifo.h"
#include <string.h>
#include <stdlib.h>

/**_Description
 *  @Construct fifo struct.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo struct.
 *  @unit_size: The size of each unit.
 *  @buffer_size: The capacity of buffer for unit.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_initialize(struct naughty_fifo *fifo_ptr, size_t unit_size, size_t buffer_size, void *buffer)
{
	naughty_exception func_res = naughty_exception_no;

	if (!fifo_ptr)
	{
		func_res = naughty_exception_nullptr;
		goto func_end;
	}
	fifo_ptr->buffer = buffer;
	fifo_ptr->unit_size = unit_size;
	fifo_ptr->buffer_size = buffer_size;
	fifo_ptr->begin_cursor = 0;
	fifo_ptr->fulled_size = 0;

func_end:
	return func_res;
}

/**_Description
 *  @Deconstruct fifo struct.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo struct.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_release(struct naughty_fifo *fifo_ptr)
{
	naughty_exception func_res = naughty_exception_no;
	if (!fifo_ptr)
	{
		func_res = naughty_exception_nullptr;
		goto func_end;
	}
	fifo_ptr->buffer = NULL;
	fifo_ptr->unit_size = 0;
	fifo_ptr->buffer_size = 0;
	fifo_ptr->begin_cursor = 0;

func_end:
	return func_res;
}

/**_Description
 *  @Push back a unit into fifo.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit: Pointer of unit's first byte.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_push_back(struct naughty_fifo *fifo_ptr, void *unit)
{
	naughty_exception func_res = naughty_exception_no;

	if (!(fifo_ptr && unit))
	{
		func_res = naughty_exception_nullptr;
		goto func_end;
	}
	if (fifo_ptr->fulled_size >= fifo_ptr->buffer_size)
	{
		func_res = naughty_exception_runout;
		goto func_end;
	}
	memcpy((byte_t*)fifo_ptr->buffer + ((fifo_ptr->begin_cursor + fifo_ptr->fulled_size) % fifo_ptr->buffer_size) * fifo_ptr->unit_size, unit, fifo_ptr->unit_size);
	fifo_ptr->fulled_size++;

func_end:
	return func_res;
}

/**_Description
 *  @Pop a unit from fifo.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_pop_front(struct naughty_fifo *fifo_ptr)
{
	naughty_exception func_res = naughty_exception_no;
	if (!fifo_ptr)
	{
		func_res = naughty_exception_nullptr;
		goto func_end;
	}
	if (fifo_ptr->fulled_size < 1)
	{
		func_res = naughty_exception_runout;
		goto func_end;
	}

	fifo_ptr->fulled_size -= 1;
	fifo_ptr->begin_cursor += 1;
	fifo_ptr->begin_cursor %= fifo_ptr->buffer_size;

func_end:
	return func_res;
}

/**_Description
 *  @Get the First Unit's ptr
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_get_front(struct naughty_fifo *fifo_ptr, void **unit_ptr)
{
	naughty_exception func_res = naughty_exception_no;
	if (!(fifo_ptr && unit_ptr))
	{
		func_res = naughty_exception_nullptr;
		goto func_end;
	}

	if (fifo_ptr->fulled_size < 1)
	{
		func_res = naughty_exception_runout;
		goto func_end;
	}

	*unit_ptr = (byte_t*)fifo_ptr->buffer + fifo_ptr->begin_cursor * fifo_ptr->unit_size;

func_end:
	return func_res;
}

/**_Description
 *  @Get the Unit's ptr
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_get_data(struct naughty_fifo *fifo_ptr, size_t index, void **unit_ptr)
{
	naughty_exception func_res = naughty_exception_no;

	if (!(fifo_ptr && unit_ptr))
	{
		func_res = naughty_exception_nullptr;
		goto func_end;
	}

	if (index >= fifo_ptr->fulled_size)
	{
		func_res = naughty_exception_outofrange;
		goto func_end;
	}

	*unit_ptr = (byte_t*)fifo_ptr->buffer + fifo_ptr->unit_size * ((index + fifo_ptr->begin_cursor) % fifo_ptr->buffer_size);

func_end:
	return func_res;
}

/**_Description
 *  @Set the Unit's ptr
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit_ptr: Input
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_set_data(struct naughty_fifo *fifo_ptr, size_t index, void *unit_ptr)
{
	naughty_exception func_res = naughty_exception_no;

	void *data_buffer = NULL;

	if (!(fifo_ptr && unit_ptr))
	{
		func_res = naughty_exception_nullptr;
		goto func_end;
	}

	if (index >= fifo_ptr->fulled_size)
	{
		func_res = naughty_exception_outofrange;
		goto func_end;
	}

	data_buffer = (byte_t*)fifo_ptr->buffer + fifo_ptr->unit_size * ((index + fifo_ptr->begin_cursor) % fifo_ptr->buffer_size);

	memcpy(data_buffer, unit_ptr, fifo_ptr->unit_size);

func_end:
	return func_res;
}

/**_Description
 *  @Get fifo's buffer_size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @size_ptr: Pointer of size.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_get_buffer_size(struct naughty_fifo *fifo_ptr, size_t *size_ptr)
{
	naughty_exception func_res = naughty_exception_no;
	if (!(fifo_ptr && size_ptr))
	{
		func_res = naughty_exception_nullptr;
		goto func_end;
	}
	*size_ptr = fifo_ptr->buffer_size;

func_end:
	return func_res;
}

/**_Description
 *  @Get fifo's buffer_size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 * _Return
 *  @Fifo's buffer size
 */
size_t naughty_fifo_buffer_size(struct naughty_fifo *fifo_ptr)
{
	NAUGHTY_ASSERT(fifo_ptr);

func_end:
	return fifo_ptr->buffer_size;
}

/**_Description
 *  @Get fifo's buffer_size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 * _Return
 *  @Fifo's rest size
 */
size_t naughty_fifo_rest_size(struct naughty_fifo *fifo_ptr)
{
	NAUGHTY_ASSERT(fifo_ptr);

func_end:
	return fifo_ptr->buffer_size - fifo_ptr->fulled_size;
}

/**_Description
 *  @Get fifo's fulled_size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @size_ptr: Pointer of size.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_get_fulled_size(struct naughty_fifo *fifo_ptr, size_t *size_ptr)
{
	naughty_exception func_res = naughty_exception_no;
	if (!(fifo_ptr && size_ptr))
	{
		func_res = naughty_exception_nullptr;
		goto func_end;
	}
	*size_ptr = fifo_ptr->fulled_size;

func_end:
	return func_res;
}

/**_Description
 *  @Get fifo's fulled_size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 * _Return
 *  @Fifo's Size
 */
size_t naughty_fifo_size(struct naughty_fifo *fifo_ptr)
{
	NAUGHTY_ASSERT(fifo_ptr);

func_end:
	return fifo_ptr->fulled_size;
}

/**_Description
 *  @Get fifo's rest size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @size_ptr: Pointer of size.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_get_rest_size(struct naughty_fifo *fifo_ptr, size_t *size_ptr)
{
	naughty_exception func_res = naughty_exception_no;
	if (!(fifo_ptr && size_ptr))
	{
		func_res = naughty_exception_nullptr;
		goto func_end;
	}
	*size_ptr = fifo_ptr->buffer_size - fifo_ptr->fulled_size;

func_end:
	return func_res;
}
