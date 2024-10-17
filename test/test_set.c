/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: test/test_set.c
 * @ Author: carl
 * @ Email: naughtygeng@qq.com
 * @ Created Time: 2022-2月-20(星期日) 08:04:36
 * @ All Rights Reserved
 * *****************************************************************************/

#include "naughty_set.h"

#include <stdio.h>

struct s
{
	double a;
	double b;
};

struct node
{
	struct naughty_set_node node;
};

int32_t compare(struct naughty_set_node *node1, struct naughty_set_node *node2)
{
	int32_t res = 0;
	if (naughty_set_data_of(node1, struct s).a < naughty_set_data_of(node2, struct s).a)
	{
		return -1;
	}
	else if (naughty_set_data_of(node1, struct s).a > naughty_set_data_of(node2, struct s).a)
	{
		return 1;
	}
	return 0;
}

int main()
{
	struct naughty_set_header h;
	naughty_set_initialize(&h, compare);
	struct naughty_set_node *node_ptr;
	naughty_set_alloc_node(&h, struct node, node, struct s, &node_ptr);

	naughty_set_data_of(node_ptr, struct s).a = 13;
	naughty_set_data_of(node_ptr, struct s).b = 33;

	naughty_set_insert_node(&h, node_ptr);

	struct naughty_set_node *find_ptr;

	struct node find_compare;
	struct s data_compare;
	find_compare.node.data_ptr = &data_compare;
	naughty_set_data_of(&find_compare.node, struct s).a = 13;

	naughty_set_find_node(&h, &find_compare.node, &find_ptr);

	printf("%f\n", naughty_set_data_of(find_ptr, struct s).b);

	naughty_set_release(&h, struct node, node);

	return 0;
}
