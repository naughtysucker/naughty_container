/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_list.h
 * @ Author: carl
 * @ Email: naughtygeng@qq.com
 * @ Created Time: 2021-Aug-03(Tuesday) 10:50:43
 * @ All Rights Reserved
 * *****************************************************************************/

#ifndef NAUGHTY_LIST_H
#define NAUGHTY_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "naughty_exception.h"
#include "naughty_common.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**_Description
 *  @Alloc a list node container with list header's memory_alloc function.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @type: Type of the Container which includes the list node.
 * _Return
 *  @Pointer of alloced list node container.
 *  @if 0:
 *		alloc failed
 *  @if not 0:
 *		alloc success
 */
#define naughty_list_alloc_node_container(list_header_ptr, type) ((type *)((struct naughty_list_header *)list_header_ptr)->memory_alloc(sizeof(type)))

/**_Description
 *  @Get the address of the container's list node member.
 * _Parameters
 *  @container_ptr: Pointer of the container.
 *  @container_type: Type of the container.
 *  @list_node_member: The name of container's list node.
 * _Return
 *  @Pointer of container's list node.
 */
#define naughty_list_node_ptr_of(container_ptr, container_type, list_node_member) ((struct naughty_list_node *)&((container_type*)container_ptr)->list_node_member)

/**_Description
 *  @Alloc list node.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @container_type: Type of containre.
 *  @list_node_member: Name of list node member.
 *  @list_node_ptr_ptr: Pointer of list node's pointer.
 * _Return
 *  @Exceptions.
 */
#define naughty_list_alloc_node(list_header_ptr, container_type, list_node_member, list_node_ptr_ptr) naughty_list_alloc_node_by_size(list_header_ptr, sizeof(container_type), (ssize_t)&((container_type *)0)->list_node_member, list_node_ptr_ptr)

/**_Description
 *  @Release all of the list's nodes.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @container_type: Type of container.
 *  @list_node_member: Name of container's list node.
 * _Return
 *  @Exceptions
 */
#define naughty_list_release(list_header_ptr, container_type, list_node_member) naughty_list_release_all_nodes(list_header_ptr, -(ssize_t)&((container_type *)0)->list_node_member)

/**_Description
 *  @Rmove a node from list and release it's memory.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @removed_list_node_ptr: Pointer of the list node to be removed.
 *  @container_type: Type of list node container.
 *  @list_node_member: Name of container's list node member.
 * _Return
 *  @Exceptions
 */
#define naughty_list_remove_node(list_header_ptr, removed_list_node_ptr, container_type, list_node_member) naughty_list_remove_node_by_offset(list_header_ptr, removed_list_node_ptr, -(ssize_t)&((container_type *)0)->list_node_member)

/**_Description
 *  @Release a Stand Alone list node.
 *  @The Released node SHOULD NOT be inserted into a list.
 * _Parameters
 *  @
 * _Return
 *  @
 */
#define naughty_list_release_node(list_header_ptr, released_list_node_ptr, container_type, list_node_member) naughty_list_release_node_by_offset(list_header_ptr, released_list_node_ptr, -(ssize_t)&((container_type *)0)->list_node_member)

struct naughty_list_node
{
	struct naughty_list_node *previous;
	struct naughty_list_node *next;
};

struct naughty_list_node_container
{
	struct naughty_list_node node;
};

struct naughty_list_header
{
	struct naughty_list_node *first_node;
	struct naughty_list_node *last_node;
	size_t list_size;
	void* (*memory_alloc)(size_t);
	void (*memory_free)(void *);
	int32_t (*compare_function)(struct naughty_list_node *, struct naughty_list_node *);
};

/**_Description
 *  @Remove one node from list and modify header.
 * _Parameters
 *  @list_header_ptr: The header's pointer of list.
 *  @removed_list_node_ptr: Pointer of the node which will be removed from list.
 * _Return
 *  @Exceptions
 */
extern naughty_exception naughty_list_remove_node_by_offset(struct naughty_list_header *list_header_ptr, struct naughty_list_node *removed_list_node_ptr, ssize_t offset);

/**_Description
 *  @Insert one node into list and modify the list's header.
 * _Parameters
 *  @list_header_ptr: Pointer of list's header.
 *  @origin_list_ptr: Pointer of one node in list already.
 *  @inserted_list_node_ptr: Pointer of the node which will be inserted before the origin_list_node.
 * _Return
 *  @Exceptions
 */
extern naughty_exception naughty_list_insert_node(struct naughty_list_header *list_header_ptr, void *origin_list_ptr, struct naughty_list_node *inserted_list_node_ptr);

/**_Description
 *  @Insert one node into list and modify the list's header.
 * _Parameters
 *  @list_header_ptr: Pointer of list's header.
 *  @origin_list_ptr: Pointer of one node in list already.
 *  @inserted_list_node_ptr: Pointer of the node which will be inserted after the origin_list_node.
 * _Return
 *  @Exceptions
 */
extern naughty_exception naughty_list_insert_after(struct naughty_list_header *list_header_ptr, void *origin_list_ptr, struct naughty_list_node *inserted_list_node_ptr);

/**_Description
 *  @Initialize list header.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 * _Return
 *  @Exceptions
 */
extern naughty_exception naughty_list_initialize(struct naughty_list_header *list_header_ptr, int32_t (*compare_function)(struct naughty_list_node *, struct naughty_list_node *));

/**_Description
 *  @Get list's size.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @size_ptr: Pointer of list's size.
 * _Return
 *  @Exceptions
 */
extern naughty_exception naughty_list_get_size(struct naughty_list_header *list_header_ptr, size_t *size_ptr);

/**_Description
 *  @Get the list node pointer by index.
 * _Parameters
 *  @list_header_ptr: Pointer of list's header.
 *  @output_list_node_ptr_ptr: Output.
 * _Return
 *  @Exceptions.
 */
extern naughty_exception naughty_list_at(struct naughty_list_header *list_header_ptr, size_t index, struct naughty_list_node **output_list_node_ptr_ptr);

/**_Description
 *  @Alloc a list node.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @container_memory_size: Size of memory will be alloced for container.
 *  @data_memory_size: Size of memory will be alloced for data.
 *  @offset: Address of list node minus container_of(list node).
 *  @list_node_ptr_ptr: Pointer of container's pointer.
 * _Return
 *  @Exceptions
 */
extern naughty_exception naughty_list_alloc_node_by_size(struct naughty_list_header *list_header_ptr, size_t container_memory_size, size_t offset, struct naughty_list_node **list_node_ptr_ptr);

/**_Description
 *  @ Release a list node.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @list_node_ptr: Pointer of list node to be released.
 *  @offset: Address of container_of(list_node) minus address of list_node.
 * _Return
 *  @Exceptions
 */
extern naughty_exception naughty_list_release_node_by_offset(struct naughty_list_header *list_header_ptr, struct naughty_list_node *list_node_ptr, ssize_t offset);

/**_Description
 *  @Release all of the list's nodes.
 * _Parameters
 *  @list_header_ptr: Pointer of the list header.
 *  @offset: Address of container_of(list_node) minus address of list_node.
 * _Return
 *  @Exceptions
 */
extern naughty_exception naughty_list_release_all_nodes(struct naughty_list_header *list_header_ptr, ssize_t offset);

/**_Description
 *  @Traversal list and execute callback function.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @callback_func: Callback function to be executed when iterates each list node.
 *  callback_func_param: Parameter of callback function.
 * _Return
 *  @Exceptions
 */
extern naughty_exception naughty_list_traversal(struct naughty_list_header *list_header_ptr, void (*callback_func)(struct naughty_list_node *, void *), void *callback_func_param);

/**_Description
 *  @Set the list header's memory_alloc and memory_free function.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @mem_alloc: Memory alloc function.
 *  @mem_free: Memory free function.
 * _Return
 *  @Exceptions
 */
extern naughty_exception naughty_list_set_memory_function(struct naughty_list_header *list_header_ptr, void *(*mem_alloc)(size_t), void (*mem_free)(void *));

/**_Description
 *  @Merge Two Lists.
 * _Parameters
 *  @list_header_ptr01: (Input & Output). It should be Sorted.
 *  @list_header_ptr02: (Input). This parameter will be cleared after merge. It should be Sorted.
 * _Return
 *  @Exception
 */
extern naughty_exception naughty_list_merge(struct naughty_list_header *list_header_ptr01, struct naughty_list_header *list_header_ptr02, int32_t (*compare_function)(struct naughty_list_node *, struct naughty_list_node *));

/**_Description
 *  @Remove the node from the list, but not release the node's memory.
 * _Parameters
 *  @list_header_ptr: Pointer of the list.
 *  @list_node_ptr: Pointer of the node which to be removed from the list.
 * _Return
 *  @Exception
 */
extern naughty_exception naughty_list_unlink_node(struct naughty_list_header *list_header_ptr, struct naughty_list_node *list_node_ptr);

/**_Description
 *  @Sort the list.
 * _Parameters
 *  @list_header_ptr: Pointer of the list.
 * _Return
 *  @Exception.
 */
extern naughty_exception naughty_list_sort(struct naughty_list_header *list_header_ptr);

#ifdef __cplusplus
}
#endif

#endif
