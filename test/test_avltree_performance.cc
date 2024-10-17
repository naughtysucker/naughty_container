/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: test_avltree_performance.cc
 * @ Author: cal
 * @ Created Time: 2021-Nov-17(Wednesday) 21:59:29
 * @ All Rights Reserved
 * *****************************************************************************/

#include "naughty_avltree.h"

#include <iostream>
#include <list>
#include <cstdint>
#include <random>
#include <map>
#include <set>
#include <ctime>
#include <chrono>

int32_t compare_test_node_func(struct naughty_avltree_node *node1, struct naughty_avltree_node *node2)
{
	if (naughty_avltree_data_of(node1, double) < naughty_avltree_data_of(node2, double))
	{
		return -1;
	}
	else if (naughty_avltree_data_of(node1, double) > naughty_avltree_data_of(node2, double))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

std::random_device rd;
std::default_random_engine re(rd());
std::uniform_real_distribution<double> urd(0, 1000000);

void test_performance_data_double(size_t data_count)
{
	std::set<double> stl_set;
	struct naughty_avltree_header nah;

	stl_set.clear();
	naughty_avltree_initialize(&nah, compare_test_node_func);
	// nah.compare_function = compare_test_node_func;
	
	std::list<double> double_to_store_list;
	std::list<double> double_to_remove_list;
	for (size_t i = 0; i < data_count; i++)
	{
		double ran_double = urd(re);
		double_to_store_list.push_back(ran_double);
		if (((int32_t)ran_double) % 17 < 7)
		{
			double_to_remove_list.push_back(ran_double);
		}
	}

	// Test STL SET INSERT SPEED
	clock_t stl_insert_begin_time = clock();
	for (double data : double_to_store_list)
	{
		stl_set.insert(data);
	}
	clock_t stl_insert_end_time = clock();
	clock_t stl_insert_duration = stl_insert_end_time - stl_insert_begin_time;
	std::cout << "STL SET INSERT SPENDS: " << stl_insert_duration << std::endl;
	// Test STL SET FIND SPEED
	clock_t stl_find_begin_time = clock();
	for (double data : double_to_remove_list)
	{
		std::set<double>::iterator iter = stl_set.find(data);
	}
	clock_t stl_find_end_time = clock();
	clock_t stl_find_duration = stl_find_end_time - stl_find_begin_time;
	std::cout << "STL SET FIND SPENDS: " << stl_find_duration << std::endl;
	// Test STL SET REMOVE SPEED
	clock_t stl_remove_begin_time = clock();
	for (double data : double_to_remove_list)
	{
		stl_set.erase(data);
	}
	clock_t stl_remove_end_time = clock();
	clock_t stl_remove_duration =  stl_remove_end_time - stl_remove_begin_time;
	std::cout << "STL SET REMOVE SPENDS: " << stl_remove_duration << std::endl;

	// Test naughty_avltree insert speed
	clock_t na_insert_begin_time = clock();
	for (double data : double_to_store_list)
	{
		struct naughty_avltree_node *nanp;
		naughty_avltree_alloc_node(&nah, struct naughty_avltree_node_container, node, double, &nanp);
		naughty_avltree_data_of(nanp, double) = data;
		naughty_avltree_insert_node(&nah, nanp);
	}
	clock_t na_insert_end_time = clock();
	clock_t na_insert_duration = na_insert_end_time - na_insert_begin_time;
	std::cout << "NAUGHTY_AVLTREE INSERT SPENDS: " << na_insert_duration << std::endl;
	// Test naughty_avltree find speed
	clock_t na_find_begin_time = clock();
	struct naughty_avltree_node *nanp_compare;
	naughty_avltree_alloc_node(&nah, struct naughty_avltree_node_container, node, double, &nanp_compare);
	for (double data : double_to_remove_list)
	{
		struct naughty_avltree_node *find_node_ptr;
		naughty_avltree_data_of(nanp_compare, double) = data;
		naughty_avltree_find_node(&nah, nanp_compare, &find_node_ptr);
	}
	clock_t na_find_end_time = clock();
	clock_t na_find_duration = na_find_end_time - na_find_begin_time;
	std::cout << "NAUGHTY_AVLTREE FIND SPENDS: " << na_find_duration << std::endl;
	// Test naughty_avltree remove speed
	clock_t na_remove_begin_time = clock();
	for (double data : double_to_remove_list)
	{
		naughty_avltree_data_of(nanp_compare, double) = data;
		naughty_avltree_remove_node_according_to_compare(&nah, nanp_compare, struct naughty_avltree_node_container, node);
	}
	clock_t na_remove_end_time = clock();
	clock_t na_remove_duration = na_remove_end_time - na_remove_begin_time;
	std::cout << "NAUGHTY_AVLTREE REMOVE SPENDS: " << na_remove_duration << std::endl;

	naughty_avltree_release_node(&nah, nanp_compare, struct naughty_avltree_node_container, node);

	naughty_avltree_release(&nah, struct naughty_avltree_node_container, node);


	std::cout << "INSERT : " << (double)stl_insert_duration / na_insert_duration << std::endl;
	std::cout << "FIND : " << (double)stl_find_duration / na_find_duration << std::endl;
	std::cout << "REMOVE : " << (double)stl_remove_duration / na_remove_duration << std::endl;
}

struct test_avltree_node_container
{
	struct naughty_avltree_node node;
	double data;
};

int32_t compare_test_node_built_in_struct_func(struct naughty_avltree_node *node1, struct naughty_avltree_node *node2)
{
	if (naughty_container_of(node1, struct test_avltree_node_container, node)->data < naughty_container_of(node2, struct test_avltree_node_container, node)->data)
	{
		return -1;
	}
	else if (naughty_container_of(node1, struct test_avltree_node_container, node)->data > naughty_container_of(node2, struct test_avltree_node_container, node)->data)
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

class CCC
{
public:

// bool compare_test_node_built_in_struct_func(const struct naughty_avltree_node &node1, const struct naughty_avltree_node &node2)
bool operator()(const struct test_avltree_node_container &node1, const struct test_avltree_node_container &node2)
{
	if (node1.data < node2.data)
	{
		return true;
	}
	else 
	{
		return false;
	}
}
};

void test_performance_double_in_container(size_t data_count)
{
	std::set<double> stl_set;
	struct naughty_avltree_header nah;

	stl_set.clear();
	naughty_avltree_initialize(&nah, compare_test_node_built_in_struct_func);
	// nah.compare_function = compare_test_node_built_in_struct_func;
	
	std::list<double> double_to_store_list;
	std::list<double> double_to_remove_list;
	for (size_t i = 0; i < data_count; i++)
	{
		double ran_double = urd(re);
		double_to_store_list.push_back(ran_double);
		if (((int32_t)ran_double) % 17 < 7)
		{
			double_to_remove_list.push_back(ran_double);
		}
	}

	// Test STL SET INSERT SPEED
	clock_t stl_insert_begin_time = clock();
	for (double data : double_to_store_list)
	{
		stl_set.insert(data);
	}
	clock_t stl_insert_end_time = clock();
	clock_t stl_insert_duration = stl_insert_end_time - stl_insert_begin_time;
	std::cout << "STL SET INSERT SPENDS: " << stl_insert_duration << std::endl;
	// Test STL SET FIND SPEED
	clock_t stl_find_begin_time = clock();
	for (double data : double_to_remove_list)
	{
		std::set<double>::iterator iter = stl_set.find(data);
	}
	clock_t stl_find_end_time = clock();
	clock_t stl_find_duration = stl_find_end_time - stl_find_begin_time;
	std::cout << "STL SET FIND SPENDS: " << stl_find_duration << std::endl;
	// Test STL SET REMOVE SPEED
	clock_t stl_remove_begin_time = clock();
	for (double data : double_to_remove_list)
	{
		stl_set.erase(data);
	}
	clock_t stl_remove_end_time = clock();
	clock_t stl_remove_duration =  stl_remove_end_time - stl_remove_begin_time;
	std::cout << "STL SET REMOVE SPENDS: " << stl_remove_duration << std::endl;

	// Test naughty_avltree insert speed
	clock_t na_insert_begin_time = clock();
	for (double data : double_to_store_list)
	{
		struct naughty_avltree_node *nanp;
		naughty_avltree_alloc_node_without_data(&nah, struct test_avltree_node_container, node, &nanp);
		naughty_container_of(nanp, struct test_avltree_node_container, node)->data = data;
		naughty_avltree_insert_node(&nah, nanp);
	}
	clock_t na_insert_end_time = clock();
	clock_t na_insert_duration = na_insert_end_time - na_insert_begin_time;
	std::cout << "NAUGHTY_AVLTREE INSERT SPENDS: " << na_insert_duration << std::endl;
	// Test naughty_avltree find speed
	clock_t na_find_begin_time = clock();
	struct naughty_avltree_node *nanp_compare;
	naughty_avltree_alloc_node_without_data(&nah, struct test_avltree_node_container, node, &nanp_compare);
	for (double data : double_to_remove_list)
	{
		struct naughty_avltree_node *find_node_ptr;
		naughty_container_of(nanp_compare, struct test_avltree_node_container, node)->data = data;
		naughty_avltree_find_node(&nah, nanp_compare, &find_node_ptr);
#ifdef _DEBUG // Remove By The Way
		if (find_node_ptr)
		{
			naughty_avltree_remove_node_according_to_node_pointer(&nah, find_node_ptr, struct test_avltree_node_container, node);
		}
#endif
	}
	clock_t na_find_end_time = clock();
	clock_t na_find_duration = na_find_end_time - na_find_begin_time;
	std::cout << "NAUGHTY_AVLTREE FIND SPENDS: " << na_find_duration << std::endl;
	// Test naughty_avltree remove speed
	clock_t na_remove_begin_time = clock();
	for (double data : double_to_remove_list)
	{
		naughty_container_of(nanp_compare, struct test_avltree_node_container, node)->data = data;
		naughty_avltree_remove_node_according_to_compare(&nah, nanp_compare, struct test_avltree_node_container, node);
	}
	clock_t na_remove_end_time = clock();
	clock_t na_remove_duration = na_remove_end_time - na_remove_begin_time;
	std::cout << "NAUGHTY_AVLTREE REMOVE SPENDS: " << na_remove_duration << std::endl;

	naughty_avltree_release_node(&nah, nanp_compare, struct test_avltree_node_container, node);

	naughty_avltree_release(&nah, struct test_avltree_node_container, node);


	std::cout << "INSERT : " << (double)stl_insert_duration / na_insert_duration << std::endl;
	std::cout << "FIND : " << (double)stl_find_duration / na_find_duration << std::endl;
	std::cout << "REMOVE : " << (double)stl_remove_duration / na_remove_duration << std::endl;
}

void test_performance_double_in_container_stl_also_in_container(size_t data_count)
{
	std::set<struct test_avltree_node_container, CCC> stl_set;
	struct naughty_avltree_header nah;

	stl_set.clear();
	naughty_avltree_initialize(&nah, compare_test_node_built_in_struct_func);
	// nah.compare_function = compare_test_node_built_in_struct_func;
	
	std::list<double> double_to_store_list;
	std::list<double> double_to_remove_list;
	for (size_t i = 0; i < data_count; i++)
	{
		double ran_double = urd(re);
		double_to_store_list.push_back(ran_double);
		if (((int32_t)ran_double) % 17 < 7)
		{
			double_to_remove_list.push_back(ran_double);
		}
	}

	// Test STL SET INSERT SPEED
	clock_t stl_insert_begin_time = clock();
	for (double data : double_to_store_list)
	{
		struct test_avltree_node_container tmp_obj;
		tmp_obj.data = data;
		stl_set.insert(tmp_obj);
	}
	clock_t stl_insert_end_time = clock();
	clock_t stl_insert_duration = stl_insert_end_time - stl_insert_begin_time;
	std::cout << "STL SET INSERT SPENDS: " << stl_insert_duration << std::endl;
	// Test STL SET FIND SPEED
	clock_t stl_find_begin_time = clock();
	for (double data : double_to_remove_list)
	{
		struct test_avltree_node_container tmp_obj;
		tmp_obj.data = data;
		std::set<struct test_avltree_node_container>::iterator iter = stl_set.find(tmp_obj);
	}
	clock_t stl_find_end_time = clock();
	clock_t stl_find_duration = stl_find_end_time - stl_find_begin_time;
	std::cout << "STL SET FIND SPENDS: " << stl_find_duration << std::endl;
	// Test STL SET REMOVE SPEED
	clock_t stl_remove_begin_time = clock();
	for (double data : double_to_remove_list)
	{
		struct test_avltree_node_container tmp_obj;
		tmp_obj.data = data;
		stl_set.erase(tmp_obj);
	}
	clock_t stl_remove_end_time = clock();
	clock_t stl_remove_duration =  stl_remove_end_time - stl_remove_begin_time;
	std::cout << "STL SET REMOVE SPENDS: " << stl_remove_duration << std::endl;

	// Test naughty_avltree insert speed
	clock_t na_insert_begin_time = clock();
	for (double data : double_to_store_list)
	{
		struct naughty_avltree_node *nanp;
		naughty_avltree_alloc_node_without_data(&nah, struct test_avltree_node_container, node, &nanp);
		naughty_container_of(nanp, struct test_avltree_node_container, node)->data = data;
		naughty_avltree_insert_node(&nah, nanp);
	}
	clock_t na_insert_end_time = clock();
	clock_t na_insert_duration = na_insert_end_time - na_insert_begin_time;
	std::cout << "NAUGHTY_AVLTREE INSERT SPENDS: " << na_insert_duration << std::endl;
	// Test naughty_avltree find speed
	clock_t na_find_begin_time = clock();
	struct naughty_avltree_node *nanp_compare;
	naughty_avltree_alloc_node_without_data(&nah, struct test_avltree_node_container, node, &nanp_compare);
	for (double data : double_to_remove_list)
	{
		struct naughty_avltree_node *find_node_ptr;
		naughty_container_of(nanp_compare, struct test_avltree_node_container, node)->data = data;
		naughty_avltree_find_node(&nah, nanp_compare, &find_node_ptr);
#ifdef _DEBUG // Remove By The Way
		if (find_node_ptr)
		{
			naughty_avltree_remove_node_according_to_node_pointer(&nah, find_node_ptr, struct test_avltree_node_container, node);
		}
#endif
	}
	clock_t na_find_end_time = clock();
	clock_t na_find_duration = na_find_end_time - na_find_begin_time;
	std::cout << "NAUGHTY_AVLTREE FIND SPENDS: " << na_find_duration << std::endl;
	// Test naughty_avltree remove speed
	clock_t na_remove_begin_time = clock();
	for (double data : double_to_remove_list)
	{
		naughty_container_of(nanp_compare, struct test_avltree_node_container, node)->data = data;
		naughty_avltree_remove_node_according_to_compare(&nah, nanp_compare, struct test_avltree_node_container, node);
	}
	clock_t na_remove_end_time = clock();
	clock_t na_remove_duration = na_remove_end_time - na_remove_begin_time;
	std::cout << "NAUGHTY_AVLTREE REMOVE SPENDS: " << na_remove_duration << std::endl;

	naughty_avltree_release_node(&nah, nanp_compare, struct test_avltree_node_container, node);

	naughty_avltree_release(&nah, struct test_avltree_node_container, node);


	std::cout << "INSERT : " << (double)stl_insert_duration / na_insert_duration << std::endl;
	std::cout << "FIND : " << (double)stl_find_duration / na_find_duration << std::endl;
	std::cout << "REMOVE : " << (double)stl_remove_duration / na_remove_duration << std::endl;
}

int main(int argc, char **argv)
{
	double test_count = 1000000;
	if (argc > 1)
	{
		test_count *= std::atof(argv[1]);
	}
	std::cout << "TEST COUNT: " << test_count << std::endl;
	test_performance_data_double(test_count);
	std::cout << std::endl;
	test_performance_double_in_container(test_count);
	std::cout << std::endl;
	test_performance_double_in_container_stl_also_in_container(test_count);

	return 0;
}
