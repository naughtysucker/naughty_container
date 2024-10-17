/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_fifo.h
 * @ Author: carl
 * @ Created Time: 2021-8月-04(星期三) 07:22:31
 * @ All Rights Reserved
 * *****************************************************************************/

#ifndef NAUGHTY_FIFO_H
#define NAUGHTY_FIFO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "naughty_exception.h"
#include "naughty_common.h"
#include <stdint.h>
#include <stdio.h>

struct naughty_fifo
{
	size_t unit_size;
	void *buffer;
	size_t buffer_size;
	size_t begin_cursor;
	size_t fulled_size;
};

/**_Description
 *  @Construct fifo struct.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo struct.
 *  @unit_size: The size of each unit.
 *  @buffer_size: The capacity of buffer for unit.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_initialize(struct naughty_fifo *fifo_ptr, size_t unit_size, size_t buffer_size, void *buffer);

/**_Description
 *  @Deconstruct fifo struct.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo struct.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_release(struct naughty_fifo *fifo_ptr);

/**_Description
 *  @Push back a unit into fifo.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit: Pointer of unit's first byte.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_push_back(struct naughty_fifo *fifo_ptr, void *unit);

/**_Description
 *  @Pop a unit from fifo.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_pop_front(struct naughty_fifo *fifo_ptr);

/**_Description
 *  @Get the First Unit's ptr
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_get_front(struct naughty_fifo *fifo_ptr, void **unit_ptr);

/**_Description
 *  @Get fifo's buffer_size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @size_ptr: Pointer of size.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_get_buffer_size(struct naughty_fifo *fifo_ptr, size_t *size_ptr);

/**_Description
 *  @Get fifo's buffer_size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 * _Return
 *  @Fifo's buffer size
 */
size_t naughty_fifo_buffer_size(struct naughty_fifo *fifo_ptr);

/**_Description
 *  @Get fifo's fulled_size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 * _Return
 *  @Fifo's Size
 */
size_t naughty_fifo_size(struct naughty_fifo *fifo_ptr);

/**_Description
 *  @Get fifo's fulled_size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @size_ptr: Pointer of size.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_get_fulled_size(struct naughty_fifo *fifo_ptr, size_t *size_ptr);

/**_Description
 *  @Get fifo's rest size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @size_ptr: Pointer of size.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_get_rest_size(struct naughty_fifo *fifo_ptr, size_t *size_ptr);

/**_Description
 *  @Get fifo's rest size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 * _Return
 *  @Fifo's rest size
 */
size_t naughty_fifo_rest_size(struct naughty_fifo *fifo_ptr);

/**_Description
 *  @Get the Unit's ptr
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_get_data(struct naughty_fifo *fifo_ptr, size_t index, void **data);

/**_Description
 *  @Set the Unit's ptr
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit_ptr: Input
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_set_data(struct naughty_fifo *fifo_ptr, size_t index, void *unit_ptr);

#ifdef __cplusplus
}
#endif

#endif
