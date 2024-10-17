/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: test_log.cc
 * @ Author: carl
 * @ Email: naughtygeng@qq.com
 * @ Created Time: 2022-6月-10(星期五) 22:30:02
 * @ All Rights Reserved
 * *****************************************************************************/

#include "naughty_log.h"
#include <iostream>

int32_t test()
{
	struct naughty_log_struct log_ins;

	naughty_log_initialize(&log_ins, 20, naughty_log_fifo_type_abandon_old_data, 128);

	for (int32_t i = 0; i < 50; i++)
	{
		naughty_log_format_and_push_back(&log_ins, "Hello, LOG: %d \n", i);
	}

	while (1)
	{
		const char *string = NULL;
		naughty_exception res = naughty_log_get_front(&log_ins, &string);
		if (res)
		{
			break;
		}

		std::cout << string;

		naughty_log_pop_front(&log_ins);
	}

	naughty_log_release(&log_ins);

	return 0;
}

int main()
{
	test();

	return 0;
}
