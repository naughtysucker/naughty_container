/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_avltree.h
 * @ Author: carl
 * @ Email: naughtygeng@qq.com
 * @ Created Time: 2021-9月-06(星期一) 04:15:43
 * @ All Rights Reserved
 * *****************************************************************************/

#ifndef NAUGHTY_AVLTREE_H
#define NAUGHTY_AVLTREE_H

#include "naughty_exception.h"
#include "naughty_common.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct naughty_avltree_node
{
	struct naughty_avltree_node *left_node;
	struct naughty_avltree_node *right_node;
	struct naughty_avltree_node *parent_node;
	void *data_ptr;
	size_t height;
};

struct naughty_avltree_header
{
	struct naughty_avltree_node *root_node;
	size_t size;
	size_t height;
	int32_t (*compare_function)(struct naughty_avltree_node *, struct naughty_avltree_node*);
	void *(*memory_alloc)(size_t);
	void (*memory_free)(void *);
};

struct naughty_avltree_node_container
{
	struct naughty_avltree_node node;
};

#define NAUGHTY_AVLTREE_HEIGHT(node) ((ssize_t)(node)?(node->height):0)
#define NAUGHTY_AVLTREE_MAX(X, Y) ((X) > (Y) ? (X) : (Y))

#define naughty_avltree_data_of(avltree_node_ptr, type) (*(type *)(((struct naughty_avltree_node *)avltree_node_ptr)->data_ptr))

#define naughty_avltree_alloc_node_container(avltree_header_ptr, type) ((type *)((struct naughty_avltree_header *)avltree_header_ptr)->memory_alloc(sizeof(type)))

#define naughty_avltree_node_ptr_of(container_ptr, container_type, avltree_node_member) ((struct naughty_avltree_node *)&((container_type*)container_ptr)->avltree_node_member)

#define naughty_avltree_alloc_node_data(avltree_header_ptr, data_type) ((data_type *)((struct naughty_avltree_header *)avltree_header_ptr)->memory_alloc(sizeof(data_type)))

#define naughty_avltree_alloc_node(avltree_header_ptr, container_type, avltree_node_member, data_type, avltree_node_ptr_ptr) naughty_avltree_alloc_node_by_size(avltree_header_ptr, sizeof(container_type), sizeof(data_type), (ssize_t)&((container_type *)0)->avltree_node_member, avltree_node_ptr_ptr)

#define naughty_avltree_alloc_node_without_data(avltree_header_ptr, container_type, avltree_node_member, avltree_node_ptr_ptr) naughty_avltree_alloc_node_by_size(avltree_header_ptr, sizeof(container_type), 0, (ssize_t)&((container_type *)0)->avltree_node_member, avltree_node_ptr_ptr)

#define naughty_avltree_release(avltree_header_ptr, container_type, avltree_node_member) naughty_avltree_release_all_nodes_by_offset(avltree_header_ptr, -(ssize_t)&((container_type *)0)->avltree_node_member)

#define naughty_avltree_release_node(avltree_header_ptr, released_avltree_node_ptr, container_type, avltree_node_member) naughty_avltree_release_node_by_offset(avltree_header_ptr, released_avltree_node_ptr, -(ssize_t)&((container_type *)0)->avltree_node_member)

#define naughty_avltree_remove_node_according_to_compare(avltree_header_ptr, removed_avltree_node_ptr, container_type, avltree_node_member) naughty_avltree_remove_node_according_to_compare_by_offset(avltree_header_ptr, removed_avltree_node_ptr, -(ssize_t)&((container_type *)0)->avltree_node_member)

#define naughty_avltree_remove_node_according_to_node_pointer(avltree_header_ptr, removed_avltree_node_ptr, container_type, avltree_node_member) naughty_avltree_remove_node_according_to_node_pointer_by_offset(avltree_header_ptr, removed_avltree_node_ptr, -(ssize_t)&((container_type *)0)->avltree_node_member)

/**_Description
 *  @Alloc a naughty avl tree node.
 * _Parameters
 *  @avltree_header_ptr: Pointer of Avl Tree Header.
 *  @container_memory_size: Memory size of Avl Tree Node's Container.
 *  @data_memory_size: Memory size of Avl Tree Node's Data.
 *  @offset: Address of Avl Tree Node's Container minus Address of Avl Tree Node.
 *  @avltree_node_ptr_ptr: Pointer of Avl Tree Node's pointer.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_alloc_node_by_size(struct naughty_avltree_header *avltree_header_ptr, size_t container_memory_size, size_t data_memory_size, ssize_t offset, struct naughty_avltree_node **avltree_node_ptr_ptr);

/**_Description
 *  @Release one Avl Tree Node.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl tree header.
 *  @avltree_node_ptr: Pointer of avl tree node.
 *  @offset: Address of container minus address of avl tree node's pointer.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_release_node_by_offset(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node *avltree_node_ptr, ssize_t offset);

/**_Description
 *  @Initialize avl tree.
 * _Parameters
 *  @header_ptr: Avl tree's header.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_initialize(struct naughty_avltree_header *header_ptr, int32_t (*compare_function)(struct naughty_avltree_node *, struct naughty_avltree_node *));

/**_Description
 *  @Set Avl Tree's Malloc and Free functions.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl tree's header.
 *  @mem_alloc: Function to alloc memory.
 *  @mem_free: Function to free memory.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_set_memory_function(struct naughty_avltree_header *avltree_header_ptr, void *(*mem_alloc)(size_t), void (*mem_free)(void *));

/**_Description
 *  @Insert One Node Into AVL Tree
 * _Parameters
 *  @avltree_header_ptr: Pointer of AVL Tree's header.
 *  @inserted_node_ptr: Pointer of inserted node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_insert_node(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node *inserted_node_ptr);

/**_Description
 *  @Pre traversal avl tree and execute callback_func when traversal each node.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl tree's header.
 *  @callback_function: Call Back Function when traversal each node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_pre_traversal(struct naughty_avltree_header *avltree_header_ptr, void (*callback_function)(struct naughty_avltree_node *));

/**_Description
 *  @Medium traversal avl tree and execute callback_func when traversal each node.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl tree's header.
 *  @callback_function: Call Back Function when traversal each node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_medium_traversal(struct naughty_avltree_header *avltree_header_ptr, void (*callback_function)(struct naughty_avltree_node *));

/**_Description
 *  @Post traversal avl tree and execute callback_func when traversal each node.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl tree's header.
 *  @callback_function: Call Back Function when traversal each node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_post_traversal(struct naughty_avltree_header *avltree_header_ptr, void (*callback_function)(struct naughty_avltree_node *));

/**_Description
 *  @Travel avl tree and release all nodes.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl tree's header.
 *  @offset: Address of container minus address of avl tree node's pointer.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_release_all_nodes_by_offset(struct naughty_avltree_header *avltree_header_ptr, ssize_t offset);

/**_Description
 *  @Remove the node which the pointer points from avl tree.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl_tree's header.
 *  @node_to_remove_ptr: Pointer of the node.
 *  @offset: Address of container subs address of avltree_node.
 * _Return
 *  @Exception
 */
naughty_exception naughty_avltree_remove_node_according_to_node_pointer_by_offset(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node *node_to_remove_ptr, ssize_t offset);

/**_Description
 *  @Remove first node from avl tree where avl_tree_header->compare_function() returns 0.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl_tree's header.
 *  @avltree_node_ptr_for_compare: The node to compare with nodes in avltree. If compare_function returns 0, the node will be removed from avltree and be release.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_remove_node_according_to_compare_by_offset(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node *avltree_node_ptr_for_compare, ssize_t offset);

/**_Description
 *  @Output first avltree_node_ptr where avl_tree_header->compare_function() returns 0.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl_tree's header.
 *  @avltree_node_ptr_for_compare: The node to compare with nodes in avltree.
 *  @output_node_ptr_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_find_node(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node *avltree_node_ptr_for_compare, struct naughty_avltree_node **output_node_ptr_ptr);

/**_Description
 *  @Get the previous node of current node.
 * _Parameters
 *  @current_node_ptr: Pointer of current node.
 *  @previous_node_ptr_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_get_previous_node(struct naughty_avltree_node *current_node_ptr, struct naughty_avltree_node **previous_node_ptr_ptr);

/**_Description
 *  @Get the next node of current node.
 * _Parameters
 *  @current_node_ptr: Pointer of current node.
 *  @next_node_ptr_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_get_next_node(struct naughty_avltree_node *current_node_ptr, struct naughty_avltree_node **next_node_ptr_ptr);

/**_Description
 *  @Get the first node of avltree.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avltree header
 *  @first_node_ptr_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_get_first_node(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node **first_node_ptr_ptr);

/**_Description
 *  @Get the last node of avltree.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avltree header
 *  @last_node_ptr_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_get_last_node(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node **last_node_ptr_ptr);

#ifdef __cplusplus
}
#endif

#endif
