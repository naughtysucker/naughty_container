/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_log.c
 * @ Author: carl
 * @ Created Time: 2022-6月-06(星期一) 07:32:32
 * @ All Rights Reserved
 * *****************************************************************************/

#include "naughty_log.h"

#ifdef __cplusplus
extern "C"
{
#endif

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
naughty_exception naughty_log_initialize(struct naughty_log_struct *log_struct_ptr, size_t fifo_size, enum naughty_log_fifo_type fifo_type, size_t string_max_length)
{
	naughty_exception func_exception = naughty_exception_no;

	NAUGHTY_ASSERT(log_struct_ptr);

	log_struct_ptr->fifo_type = fifo_type;
	log_struct_ptr->string_max_length = string_max_length;
	log_struct_ptr->mem_alloc = malloc;
	log_struct_ptr->mem_free = free;

	void *buf = log_struct_ptr->mem_alloc(sizeof(void *) * fifo_size);

	naughty_fifo_initialize(&log_struct_ptr->log_fifo, sizeof(void *), fifo_size, buf);

func_end:
	return func_exception;
}

/**_Description
 *  @ Push back one log into the log struct.
 * _Parameters
 *  @ log_struct_ptr: Pointer of log struct.
 *  @ format: String of format. Same as snprintf.
 *  @ ...: Variable parameters. Same as snprintf.
 * _Return
 *  @ Exceptions
 */
naughty_exception naughty_log_format_and_push_back(struct naughty_log_struct *log_struct_ptr, const char *format, ...)
{
	naughty_exception func_exception = naughty_exception_no;

	NAUGHTY_ASSERT(log_struct_ptr);

	char buffer[log_struct_ptr->string_max_length];

	va_list args_list;
	va_start(args_list, format);

	vsnprintf(buffer, log_struct_ptr->string_max_length, format, args_list);

	va_end(args_list);

	func_exception = naughty_log_push_back(log_struct_ptr, buffer);

func_end:
	return func_exception;
}

/**_Description
 *  @ Push back one log.
 *  @ This function will allocate memory for the string, so you just need keep the parameter 'stirng' valid during this function running.
 * _Parameters
 *  @ log_struct_ptr: Pointer of log struct.
 *  @ string: Log's content.
 * _Return
 *  @ Exceptions
 */
naughty_exception naughty_log_push_back(struct naughty_log_struct *log_struct_ptr, const char *string)
{
	naughty_exception func_exception = naughty_exception_no;

	NAUGHTY_ASSERT(log_struct_ptr);

	char *buffer = (char *)log_struct_ptr->mem_alloc(strlen(string));
	if (!buffer)
	{
		func_exception = naughty_exception_alloc;
		goto func_end;
	}

	strcpy(buffer, string);


	if (log_struct_ptr->fifo_type == naughty_log_fifo_type_abandon_old_data)
	{
		if (!naughty_fifo_rest_size(&log_struct_ptr->log_fifo))
		{
			func_exception = naughty_log_pop_front(log_struct_ptr);
			if (func_exception != naughty_exception_no)
			{
				goto func_end;
			}
		}
	}

	func_exception = naughty_fifo_push_back(&log_struct_ptr->log_fifo, (byte_t *)&buffer);
	if (func_exception != naughty_exception_no)
	{
		log_struct_ptr->mem_free(buffer);
	}

func_end:
	return func_exception;
}

/**_Description
 *  @ Pop the first log from the log struct.
 * _Parameters
 *  @ log_struct_ptr: Pointer of log struct.
 * _Return
 *  @ Exceptions
 */
naughty_exception naughty_log_pop_front(struct naughty_log_struct *log_struct_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	NAUGHTY_ASSERT(log_struct_ptr);

	char **string_ptr = NULL;

	func_exception = naughty_fifo_get_front(&log_struct_ptr->log_fifo, (void **)&string_ptr);
	if (func_exception != naughty_exception_no)
	{
		goto func_end;
	}

	log_struct_ptr->mem_free(*string_ptr);

	func_exception = naughty_fifo_pop_front(&log_struct_ptr->log_fifo);

func_end:
	return func_exception;
}

/**_Description
 *  @ Get the first log of log struct.
 * _Parameters
 *  @ log_struct_ptr: Pointer of log struct.
 *  @ string_ptr: (Output) | String of the first log.
 * _Return
 *  @ Exceptions
 */
naughty_exception naughty_log_get_front(struct naughty_log_struct *log_struct_ptr, const char **string_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	NAUGHTY_ASSERT(log_struct_ptr);

	char **char_ptr_ptr;
	func_exception = naughty_fifo_get_front(&log_struct_ptr->log_fifo, (void **)&char_ptr_ptr);
	if (func_exception != naughty_exception_no)
	{
		goto func_end;
	}

	*string_ptr = *char_ptr_ptr;

func_end:
	return func_exception;
}

/**_Description
 *  @ Relese the log struct.
 * _Parameters
 *  @ log_struct_ptr: Pointer of log struct.
 * _Return
 *  @ Exceptions
 */
naughty_exception naughty_log_release(struct naughty_log_struct *log_struct_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	do
	{
		func_exception = naughty_log_pop_front(log_struct_ptr);
	} while (func_exception == naughty_exception_no);

	func_exception = naughty_fifo_release(&log_struct_ptr->log_fifo);

func_end:
	return func_exception;
}

#ifdef __cplusplus
}
#endif

