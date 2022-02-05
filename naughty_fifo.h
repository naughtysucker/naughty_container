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
#include <stdint.h>
#include <stdio.h>

struct naughty_fifo
{
	size_t unit_size;
	uint8_t *buffer;
	size_t buffer_size;
	size_t begin_cursor;
	size_t fulled_size;
	size_t locked_size;
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
naughty_exception naughty_fifo_construction(struct naughty_fifo *fifo_ptr, size_t unit_size, size_t buffer_size);

/**_Description
 *  @Deconstruct fifo struct.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo struct.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_deconstruction(struct naughty_fifo *fifo_ptr);

/**_Description
 *  @Push back a unit into fifo.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit: Pointer of unit's first byte.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_push_back(struct naughty_fifo *fifo_ptr, uint8_t *unit);

/**_Description
 *  @Pop a unit from fifo.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit_ptr: Pointer of unit. What's important, this pointer points to addresses in fifo's buffer.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_pop_front(struct naughty_fifo *fifo_ptr, uint8_t **unit_ptr);

/**_Description
 *  @Pop a unit from fifo and lock it in case new unit data overwrite the poped unit.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @unit_ptr: Pointer of unit.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_pop_front_and_lock(struct naughty_fifo *fifo_ptr, uint8_t **unit_ptr);

/**_Description
 *  @Get fifo's buffer_size.
 * _Parameters
 *  @fifo_ptr: Pointer of fifo.
 *  @size_ptr: Pointer of size.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_fifo_get_size(struct naughty_fifo *fifo_ptr, size_t *size_ptr);

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

#ifdef __cplusplus
}
#endif

#endif
