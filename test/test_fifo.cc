/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: test_fifo.cc
 * @ Author: carl
 * @ Created Time: 2022-6月-08(星期三) 12:34:18
 * @ All Rights Reserved
 * *****************************************************************************/

#include <iostream>
#include <deque>

#include "naughty_fifo.h"

struct test_t
{
	uint32_t a;
	uint32_t b;
};

test_t g_buffer[1000];

int test()
{
	naughty_fifo fifo_ins;

	naughty_fifo_initialize(&fifo_ins, sizeof(test_t), 1000, g_buffer);

	for (int i = 0; i < 10; i++)
	{
		test_t t;
		t.a = 3 * i;
		t.b = 4 * i;
		naughty_fifo_push_back(&fifo_ins, &t);
	}

	test_t *t;
	naughty_fifo_get_data(&fifo_ins, 3, (void **)&t);

	std::cout << t->a << std::endl;
	std::cout << t->b << std::endl;

	while (1)
	{
		size_t fifo_size = 0;
		// naughty_fifo_get_fulled_size(&fifo_ins, &fifo_size);
		fifo_size = naughty_fifo_size(&fifo_ins);
		if (fifo_size == 0)
		{
			break;
		}
		test_t *num_ptr = 0;
		naughty_fifo_get_front(&fifo_ins, (void **)&num_ptr);
		std::cout << num_ptr->a << std::endl;
		std::cout << num_ptr->b << std::endl;
		naughty_fifo_pop_front(&fifo_ins);
	}

	naughty_fifo_release(&fifo_ins);

	return 0;
}

int main()
{
	test();
}
