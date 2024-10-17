/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_log.h
 * @ Author: carl
 * @ Created Time: 2022-6月-06(星期一) 07:31:46
 * @ All Rights Reserved
 * *****************************************************************************/

#ifndef NAUGHTY_LOG_H
#define NAUGHTY_LOG_H

#include "naughty_fifo.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

enum naughty_log_fifo_type
{
	naughty_log_fifo_type_abandon_old_data,
	naughty_log_fifo_type_abandon_new_data,
};

struct naughty_log_struct
{
	struct naughty_fifo log_fifo;
	enum naughty_log_fifo_type fifo_type;
	size_t string_max_length;
	void *(*mem_alloc)(size_t);
	void (*mem_free)(void *);
};

/**_Description
 *  @ Initialize the log struct.
 * _Parameters
 *  @ log_struct_ptr: Pointer of log struct.
 *  @ fifo_size: How many logs can be stored in the fifo.
 *  @ fifo_type: How to do when the fifo has no space.
 *  @ string_max_length: Max string length of each log.
 * _Return
 *  @ Exceptions
 */
naughty_exception naughty_log_initialize(struct naughty_log_struct *log_struct_ptr, size_t fifo_size, enum naughty_log_fifo_type fifo_type, size_t string_max_length);

/**_Description
 *  @ Push back one log into the log struct.
 * _Parameters
 *  @ log_struct_ptr: Pointer of log struct.
 *  @ format: String of format. Same as snprintf.
 *  @ ...: Variable parameters. Same as snprintf.
 * _Return
 *  @ Exceptions
 */
naughty_exception naughty_log_format_and_push_back(struct naughty_log_struct *log_struct_ptr, const char *format, ...);

/**_Description
 *  @ Push back one log.
 *  @ This function will allocate memory for the string, so you just need keep the parameter 'stirng' valid during this function running.
 * _Parameters
 *  @ log_struct_ptr: Pointer of log struct.
 *  @ string: Log's content.
 * _Return
 *  @ Exceptions
 */
naughty_exception naughty_log_push_back(struct naughty_log_struct *log_struct_ptr, const char *string);

/**_Description
 *  @ Pop the first log from the log struct.
 * _Parameters
 *  @ log_struct_ptr: Pointer of log struct.
 * _Return
 *  @ Exceptions
 */
naughty_exception naughty_log_pop_front(struct naughty_log_struct *log_struct_ptr);

/**_Description
 *  @ Get the first log of log struct.
 * _Parameters
 *  @ log_struct_ptr: Pointer of log struct.
 *  @ string_ptr: (Output) | String of the first log.
 * _Return
 *  @ Exceptions
 */
naughty_exception naughty_log_get_front(struct naughty_log_struct *log_struct_ptr, const char **string_ptr);

/**_Description
 *  @ Relese the log struct.
 * _Parameters
 *  @ log_struct_ptr: Pointer of log struct.
 * _Return
 *  @ Exceptions
 */
naughty_exception naughty_log_release(struct naughty_log_struct *log_struct_ptr);

#ifdef __cplusplus
}
#endif

#endif
