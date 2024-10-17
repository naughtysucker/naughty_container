/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: test_list.cc
 * @ Author: carl
 * @ Email: naughtygeng@qq.com
 * @ Created Time: 2022-6月-28(星期二) 23:17:10
 * @ All Rights Reserved
 * *****************************************************************************/

#include "naughty_list.h"

#include <iostream>
#include <list>
#include <cstdint>
#include <random>
#include <map>
#include <set>
#include <ctime>
#include <chrono>

struct test_container
{
	struct naughty_list_node node;
	double data;
};

int32_t test_compare_func(struct naughty_list_node *node_ptr01, struct naughty_list_node *node_ptr02)
{
	int32_t result = 0;
	if (naughty_container_of(node_ptr01, struct test_container, node)->data < naughty_container_of(node_ptr02, struct test_container, node)->data)
	{
		result = -1;
	}
	else if (naughty_container_of(node_ptr01, struct test_container, node)->data == naughty_container_of(node_ptr02, struct test_container, node)->data)
	{
		result = 0;
	}
	else if (naughty_container_of(node_ptr01, struct test_container, node)->data > naughty_container_of(node_ptr02, struct test_container, node)->data)
	{
		result = 1;
	}

	return result;
}

std::random_device rd;
std::default_random_engine re(rd());
std::uniform_real_distribution<double> urd(0, 1000000);

int32_t test(int32_t test_number_count)
{
	struct naughty_list_header na_list;

	naughty_list_initialize(&na_list, test_compare_func);

	std::list<double> std_list;
	
	for (int32_t i = 0; i < test_number_count; i++)
	{
		double ran_double = urd(re);

		std_list.push_back(ran_double);

		struct naughty_list_node *na_list_node_ptr;
		naughty_list_alloc_node(&na_list, struct naughty_list_node_container, node, &na_list_node_ptr);
		naughty_container_of(na_list_node_ptr, struct test_container, node)->data = ran_double;
		naughty_list_insert_after(&na_list, na_list.last_node, na_list_node_ptr);
	}

	clock_t std_sort_begin_time = clock();
	std_list.sort();
	clock_t std_sort_end_time = clock();

	clock_t std_sort_duration = std_sort_end_time - std_sort_begin_time;

	std::cout << "STD LIST SORT SPENDS: " << std_sort_duration << std::endl;

	clock_t na_sort_begin_time = clock();
	naughty_list_sort(&na_list);
	clock_t na_sort_end_time = clock();

	clock_t na_sort_duration = na_sort_end_time - na_sort_begin_time;

	std::cout << "NAUGHTY LIST SORT SPENDS: " << na_sort_duration << std::endl;

	// Verify
	if (std_list.size() != na_list.list_size)
	{
		std::cout << "Error!!! Size Error " << std_list.size() << " " << na_list.list_size << std::endl;
	}
	else
	{
		std::cout << "Size Ok " << na_list.list_size << std::endl;
	}

	struct naughty_list_node *na_list_iter = na_list.first_node;
	std::list<double>::iterator std_list_iter = std_list.begin();

	int32_t count = 0;
	for (; na_list_iter; na_list_iter = na_list_iter->next, std_list_iter++)
	{
		if (*std_list_iter != naughty_container_of(na_list_iter, struct test_container, node)->data)
		{
			std::cout << "Error!!! Value Error " << count << std::endl;
			break;
		}
		count++;
	}

	naughty_list_release(&na_list, struct naughty_list_node_container, node);

	std::cout << "iteration: " << count << std::endl;

	std::cout << "Speed Ratio: " << test_number_count << "   |   " << (double)std_sort_duration / (double)na_sort_duration << std::endl;

	return 0;
}

int main()
{
	test(1000);
	test(10000);
	test(100000);
	test(1000000);
	test(10000000);

	return 0;
}
