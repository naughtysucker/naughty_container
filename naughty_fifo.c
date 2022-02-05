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
naughty_exception naughty_fifo_construction(struct naughty_fifo *fifo_ptr, size_t unit_size, size_t buffer_size)
{
	if (!fifo_ptr)
	{
		goto func_nullptr_exception;
	}
	fifo_ptr->buffer = (uint8_t *)malloc(unit_size * buffer_size);
	if (!fifo_ptr->buffer)
	{
		goto func_alloc_exception;
	}
	fifo_ptr->unit_size = unit_size;
	fifo_ptr->buffer_size = buffer_size;
	fifo_ptr->begin_cursor = 0;
	fifo_ptr->fulled_size = 0;
	fifo_ptr->locked_size = 0;

func_end:
	return naughty_exception_no;
func_nullptr_exception:
	return naughty_exception_nullptr;
func_alloc_exception:
	return naughty_exception_alloc;
}

/**_Description
 *  @Deconstruct fifo struct.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo struct.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_deconstruction(struct naughty_fifo *fifo_ptr)
{
	if (!fifo_ptr)
	{
		goto func_nullptr_exception;
	}
	free(fifo_ptr->buffer);
	fifo_ptr->buffer = NULL;
	fifo_ptr->unit_size = 0;
	fifo_ptr->buffer_size = 0;
	fifo_ptr->begin_cursor = 0;

func_end:
	return naughty_exception_no;
func_nullptr_exception:
	return naughty_exception_nullptr;
func_alloc_exception:
	return naughty_exception_alloc;
}

/**_Description
 *  @Push back a unit into fifo.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit: Pointer of unit's first byte.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_push_back(struct naughty_fifo *fifo_ptr, uint8_t *unit)
{
	if (!(fifo_ptr && unit))
	{
		goto func_nullptr_exception;
	}
	if (fifo_ptr->fulled_size + fifo_ptr->locked_size >= fifo_ptr->buffer_size)
	{
		goto func_runout_exception;
	}
	memcpy(fifo_ptr->buffer + ((fifo_ptr->begin_cursor + fifo_ptr->fulled_size) % fifo_ptr->buffer_size) * fifo_ptr->unit_size, unit, fifo_ptr->unit_size);
	fifo_ptr->fulled_size++;

func_end:
	return naughty_exception_no;
func_nullptr_exception:
	return naughty_exception_nullptr;
func_alloc_exception:
	return naughty_exception_alloc;
func_runout_exception:
	return naughty_exception_runout;
}

/**_Description
 *  @Pop a unit from fifo.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit_ptr: Pointer of unit. What's important, this pointer points to addresses in fifo's buffer.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_pop_front(struct naughty_fifo *fifo_ptr, uint8_t **unit_ptr)
{
	if (!(fifo_ptr && unit_ptr))
	{
		goto func_nullptr_exception;
	}
	if (fifo_ptr->fulled_size < 1)
	{
		goto func_runout_exception;
	}
	*unit_ptr = fifo_ptr->buffer + fifo_ptr->begin_cursor * fifo_ptr->unit_size;
	fifo_ptr->fulled_size -= 1;
	fifo_ptr->begin_cursor += 1;
	fifo_ptr->begin_cursor %= fifo_ptr->buffer_size;

func_end:
	return naughty_exception_no;
func_nullptr_exception:
	return naughty_exception_nullptr;
func_alloc_exception:
	return naughty_exception_alloc;
func_runout_exception:
	return naughty_exception_runout;
}

/**_Description
 *  @Pop a unit from fifo and lock it in case new unit data overwrite the poped unit.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit_ptr: Pointer of unit.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_pop_front_and_lock(struct naughty_fifo *fifo_ptr, uint8_t **unit_ptr)
{
	if (!(fifo_ptr && unit_ptr))
	{
		goto func_nullptr_exception;
	}
	if (fifo_ptr->fulled_size < 1)
	{
		goto func_runout_exception;
	}
	*unit_ptr = fifo_ptr->buffer + fifo_ptr->begin_cursor * fifo_ptr->unit_size;
	fifo_ptr->fulled_size -= 1;
	fifo_ptr->begin_cursor += 1;
	fifo_ptr->begin_cursor %= fifo_ptr->buffer_size;

	fifo_ptr->locked_size = 1;

func_end:
	return naughty_exception_no;
func_nullptr_exception:
	return naughty_exception_nullptr;
func_alloc_exception:
	return naughty_exception_alloc;
func_runout_exception:
	return naughty_exception_runout;
}

/**_Description
 *  @Get fifo's buffer_size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @size_ptr: Pointer of size.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_get_size(struct naughty_fifo *fifo_ptr, size_t *size_ptr)
{
	if (!(fifo_ptr && size_ptr))
	{
		goto func_nullptr_exception;
	}
	*size_ptr = fifo_ptr->buffer_size;

func_end:
	return naughty_exception_no;
func_nullptr_exception:
	return naughty_exception_nullptr;
func_alloc_exception:
	return naughty_exception_alloc;
func_runout_exception:
	return naughty_exception_runout;
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
	if (!(fifo_ptr && size_ptr))
	{
		goto func_nullptr_exception;
	}
	*size_ptr = fifo_ptr->fulled_size;

func_end:
	return naughty_exception_no;
func_nullptr_exception:
	return naughty_exception_nullptr;
func_alloc_exception:
	return naughty_exception_alloc;
func_runout_exception:
	return naughty_exception_runout;
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
	if (!(fifo_ptr && size_ptr))
	{
		goto func_nullptr_exception;
	}
	*size_ptr = fifo_ptr->buffer_size - fifo_ptr->fulled_size - fifo_ptr->locked_size;

func_end:
	return naughty_exception_no;
func_nullptr_exception:
	return naughty_exception_nullptr;
func_alloc_exception:
	return naughty_exception_alloc;
func_runout_exception:
	return naughty_exception_runout;
}
