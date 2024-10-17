/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_set.h
 * @ Author: carl
 * @ Email: naughtygeng@qq.com
 * @ Created Time: 2022-2月-20(星期日) 07:37:38
 * @ All Rights Reserved
 * *****************************************************************************/

#ifndef NAUGHTY_SET_H
#define NAUGHTY_SET_H

#include "naughty_avltree.h"

#define naughty_set_node naughty_avltree_node
#define naughty_set_header naughty_avltree_header
#define naughty_set_node_container naughty_avltree_node_container
#define naughty_set_data_of naughty_avltree_data_of
#define naughty_set_alloc_node_container naughty_avltree_alloc_node_container
#define naughty_set_node_ptr_of naughty_avltree_node_ptr_of
#define naughty_set_alloc_node_data naughty_avltree_alloc_node_data
#define naughty_set_alloc_node naughty_avltree_alloc_node
#define naughty_set_alloc_node_without_data naughty_avltree_alloc_node_without_data
#define naughty_set_release naughty_avltree_release
#define naughty_set_release_node naughty_avltree_release_node
#define naughty_set_remove_node_according_to_compare naughty_avltree_remove_node_according_to_compare
#define naughty_set_remove_node_according_to_pointer naughty_avltree_remove_node_according_to_node_pointer
#define naughty_set_initialize naughty_avltree_initialize
#define naughty_set_set_memory_function naughty_avltree_set_memory_function
#define naughty_set_insert_node naughty_avltree_insert_node
#define naughty_set_find_node naughty_avltree_find_node

#define naughty_set_get_first_node naughty_avltree_get_first_node
#define naughty_set_get_next_node naughty_avltree_get_next_node
#define naughty_set_get_previous_node naughty_avltree_get_previous_node

#define naughty_set_get_last_node naughty_avltree_get_last_node

#endif
