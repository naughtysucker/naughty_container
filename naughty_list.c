/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_list.c
 * @ Author: carl
 * @ Email: naughtygeng@qq.com
 * @ Created Time: 2021-Aug-03(Tuesday) 10:57:40
 * @ All Rights Reserved
 * *****************************************************************************/

#include "naughty_list.h"
#include <stdint.h>
#include <stdio.h>

#ifdef __cpluscplus
extern "C"
{
#endif

/**_Description
 *  @Remove one node from list and modify header.
 * _Parameters
 *  @list_header_ptr: The header's pointer of list.
 *  @removed_list_node_ptr: Pointer of the node which will be removed from list.
 * _Return
 *  @Exceptions
 */
extern naughty_exception naughty_list_remove_node(struct naughty_list_header *list_header_ptr, struct naughty_list_node *removed_list_node_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	if (!(removed_list_node_ptr && list_header_ptr))
	{
		func_exception = naughty_exception_nullptr;
		goto func_end;
	}
	if (removed_list_node_ptr->previous)
	{
		removed_list_node_ptr->previous->next = removed_list_node_ptr->next;
	}
	else if (list_header_ptr->first_node == removed_list_node_ptr)
	{
		list_header_ptr->first_node = removed_list_node_ptr->next;
	}
	else
	{
		func_exception = naughty_exception_unknown;
		goto func_end;
	}
	if (removed_list_node_ptr->next)
	{
		removed_list_node_ptr->next->previous = removed_list_node_ptr->previous;
	}
	else if (list_header_ptr->last_node == removed_list_node_ptr)
	{
		list_header_ptr->last_node = removed_list_node_ptr->previous;
	}
	else
	{
		func_exception = naughty_exception_unknown;
		goto func_end;
	}

func_end:
	return func_exception;
}

/**_Description
 *  @Remove one node from list and modify header.
 * _Parameters
 *  @list_header_ptr: The header's pointer of list.
 *  @removed_list_node_ptr: Pointer of the node which will be removed from list.
 *  @offset: Address of container_of(list node) minus address of list node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_list_remove_release_node_by_offset(struct naughty_list_header *list_header_ptr, struct naughty_list_node *removed_list_node_ptr, ssize_t offset)
{
	naughty_exception func_exception = naughty_exception_no;

	func_exception = naughty_list_remove_node(list_header_ptr, removed_list_node_ptr);

	if (func_exception != naughty_exception_no)
	{
		goto func_end;
	}

	list_header_ptr->list_size -= 1;

	func_exception = naughty_list_release_node_by_offset(list_header_ptr, removed_list_node_ptr, offset);

func_end:
	return func_exception;
}

/**_Description
 *  @Insert one node into list and modify the list's header.
 * _Parameters
 *  @list_header_ptr: Pointer of list's header.
 *  @origin_list_ptr: Pointer of one node in list already.
 *  @inserted_list_node_ptr: Pointer of the node which will be inserted after the origin_list_node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_list_insert_after(struct naughty_list_header *list_header_ptr, void *origin_list_ptr, struct naughty_list_node *inserted_list_node_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	if (!(origin_list_ptr && inserted_list_node_ptr && list_header_ptr))
	{
		func_exception = naughty_exception_nullptr;
		goto func_end;
	}

	if (origin_list_ptr == list_header_ptr)
	{
		// insert after header
		struct naughty_list_node *swap_list_node_ptr;
		swap_list_node_ptr = list_header_ptr->first_node;
		list_header_ptr->first_node = (struct naughty_list_node *)inserted_list_node_ptr;
		list_header_ptr->first_node->previous = NULL;
		list_header_ptr->first_node->next = swap_list_node_ptr;
		if (!swap_list_node_ptr)
		{
			list_header_ptr->last_node = list_header_ptr->first_node;
		}
		else
		{
			swap_list_node_ptr->previous = list_header_ptr->first_node;
		}
	}
	else
	{
		struct naughty_list_node *origin_list_node_ptr = (struct naughty_list_node *)origin_list_ptr;
		struct naughty_list_node *swap_list_node_ptr;
		swap_list_node_ptr = origin_list_node_ptr->next;
		origin_list_node_ptr->next = inserted_list_node_ptr;
		inserted_list_node_ptr->previous = origin_list_node_ptr;
		inserted_list_node_ptr->next = swap_list_node_ptr;
		if (!swap_list_node_ptr)
		{
			list_header_ptr->last_node = inserted_list_node_ptr;
		}
		else
		{
			swap_list_node_ptr->previous = inserted_list_node_ptr;
		}
	}
	list_header_ptr->list_size += 1;

func_end:
	return func_exception;
}

/**_Description
 *  @Insert one node into list and modify the list's header.
 * _Parameters
 *  @list_header_ptr: Pointer of list's header.
 *  @origin_list_ptr: Pointer of one node in list already.
 *  @inserted_list_node_ptr: Pointer of the node which will be inserted after the origin_list_node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_list_insert_node(struct naughty_list_header *list_header_ptr, void *origin_list_ptr, struct naughty_list_node *inserted_list_node_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	if (!(origin_list_ptr && inserted_list_node_ptr && list_header_ptr))
	{
		func_exception = naughty_exception_nullptr;
		goto func_end;
	}

	if (origin_list_ptr == list_header_ptr)
	{
		// insert after header
		struct naughty_list_node *swap_list_node_ptr;
		swap_list_node_ptr = list_header_ptr->first_node;
		list_header_ptr->first_node = (struct naughty_list_node *)inserted_list_node_ptr;
		list_header_ptr->first_node->previous = NULL;
		list_header_ptr->first_node->next = swap_list_node_ptr;
		if (!swap_list_node_ptr)
		{
			list_header_ptr->last_node = list_header_ptr->first_node;
		}
		else
		{
			swap_list_node_ptr->previous = list_header_ptr->first_node;
		}
	}
	else
	{
		struct naughty_list_node *origin_list_node_ptr = (struct naughty_list_node *)origin_list_ptr;
		struct naughty_list_node *swap_list_node_ptr;
		swap_list_node_ptr = origin_list_node_ptr->previous;
		origin_list_node_ptr->previous = inserted_list_node_ptr;
		inserted_list_node_ptr->next = origin_list_node_ptr;
		inserted_list_node_ptr->previous = swap_list_node_ptr;
		if (!swap_list_node_ptr)
		{
			list_header_ptr->first_node = inserted_list_node_ptr;
		}
		else
		{
			swap_list_node_ptr->next = inserted_list_node_ptr;
		}
	}
	list_header_ptr->list_size += 1;

func_end:
	return naughty_exception_no;
}

/**_Description
 *  @Initial list header.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_list_initial(struct naughty_list_header *list_header_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	if (!list_header_ptr)
	{
		func_exception = naughty_exception_nullptr;
		goto func_end;
	}
	list_header_ptr->first_node = NULL;
	list_header_ptr->list_size = 0;
	// not neccessary
	list_header_ptr->last_node = NULL;
	// Default memory function
	list_header_ptr->memory_alloc = malloc;
	list_header_ptr->memory_free = free;

func_end:
	return func_exception;
}

/**_Description
 *  @Get list's size.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @size_ptr: Pointer of list's size.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_list_get_size(struct naughty_list_header *list_header_ptr, size_t *size_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	if (!(list_header_ptr && size_ptr))
	{
		func_exception = naughty_exception_nullptr;
		goto func_end;
	}
	*size_ptr = list_header_ptr->list_size;

func_end:
	return func_exception;
}

/**_Description
 *  @Get the list node pointer by index.
 * _Parameters
 *  @list_header_ptr: Pointer of list's header.
 *  @output_list_node_ptr_ptr: Output.
 * _Return
 *  @Exceptions.
 */
naughty_exception naughty_list_at(struct naughty_list_header *list_header_ptr, size_t index, struct naughty_list_node **output_list_node_ptr_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	if (!(list_header_ptr && output_list_node_ptr_ptr))
	{
		func_exception = naughty_exception_nullptr;
		goto func_end;
	}

	if (index >= list_header_ptr->list_size)
	{
		func_exception = naughty_exception_outofrange;
		goto func_end;
	}

	struct naughty_list_node *node_ptr = list_header_ptr->first_node;
	for (size_t i = 0; node_ptr; node_ptr = node_ptr->next, i++)
	{
		if (i == index)
		{
			break;
		}
	}

	if (!node_ptr)
	{
		func_exception = naughty_exception_unknown;
		goto func_end;
	}

	*output_list_node_ptr_ptr = node_ptr;

func_end:
	return func_exception;
}

/**_Description
 *  @Alloc a list node.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @container_memory_size: Size of memory will be alloced for container.
 *  @data_memory_size: Size of memory will be alloced for data.
 *  @offset: Address of list node minus container_of(list node).
 *  @list_node_ptr_ptr: Pointer of list node's pointer.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_list_alloc_node_by_size(struct naughty_list_header *list_header_ptr, size_t container_memory_size, size_t data_memory_size, size_t offset, struct naughty_list_node **list_node_ptr_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	if (!(list_header_ptr && list_node_ptr_ptr))
	{
		func_exception = naughty_exception_nullptr;
		goto func_end;
	}
	void *container_memory_ptr = list_header_ptr->memory_alloc(container_memory_size);
	void *data_memory_ptr = NULL;
	if (data_memory_size)
	{
		data_memory_ptr = list_header_ptr->memory_alloc(data_memory_size);
	}
	if (!container_memory_ptr || (data_memory_size && !data_memory_ptr))
	{
		func_exception = naughty_exception_alloc;
		goto func_alloc_exception;
	}
	*list_node_ptr_ptr = (struct naughty_list_node *)((uint8_t *)container_memory_ptr + offset);
	((struct naughty_list_node *)*list_node_ptr_ptr)->data_ptr = data_memory_ptr;

	goto func_end;
	
func_alloc_exception:
	if (list_header_ptr->memory_free)
	{
		if (container_memory_ptr)
		{
			list_header_ptr->memory_free(container_memory_ptr);
		}
		if (data_memory_ptr)
		{
			list_header_ptr->memory_free(data_memory_ptr);
		}
	}
func_end:
	return func_exception;
}

/**_Description
 *  @ Release a list node.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @list_node_ptr: Pointer of list node to be released.
 *  @offset: Address of container_of(list_node) minus address of list_node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_list_release_node_by_offset(struct naughty_list_header *list_header_ptr, struct naughty_list_node *list_node_ptr, ssize_t offset)
{
	naughty_exception func_exception = naughty_exception_no;

	if (!(list_header_ptr && list_node_ptr))
	{
		func_exception = naughty_exception_nullptr;
		goto func_end;
	}

	if (list_header_ptr->memory_free)
	{
		if (list_node_ptr->data_ptr)
		{
			list_header_ptr->memory_free(list_node_ptr->data_ptr);
		}
		list_header_ptr->memory_free((uint8_t *)list_node_ptr + offset);
	}

func_end:
	return naughty_exception_no;
}

/**_Description
 *  @Release all of the list's nodes.
 * _Parameters
 *  @list_header_ptr: Pointer of the list header.
 *  @offset: Address of container_of(list_node) minus address of list_node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_list_release_all_nodes(struct naughty_list_header *list_header_ptr, ssize_t offset)
{
	naughty_exception func_exception = naughty_exception_no;

	if (!list_header_ptr)
	{
		func_exception = naughty_exception_nullptr;
		goto func_end;
	}

	for (struct naughty_list_node *list_node_ptr = list_header_ptr->first_node; list_node_ptr;)
	{
		struct naughty_list_node *next_list_node_ptr = list_node_ptr->next;
		func_exception = naughty_list_release_node_by_offset(list_header_ptr, list_node_ptr, offset);
		if (func_exception != naughty_exception_no)
		{
			goto func_end;
		}
		list_node_ptr = next_list_node_ptr;
	}

	func_exception = naughty_list_initial(list_header_ptr);

func_end:
	return func_exception;
}

/**_Description
 *  @Traversal list and execute callback function.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @callback_func: Callback function to be executed when iterates each list node.
 *  callback_func_param: Parameter of callback function.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_list_traversal(struct naughty_list_header *list_header_ptr, void (*callback_func)(struct naughty_list_node *, void *), void *callback_func_param)
{
	naughty_exception func_exception = naughty_exception_no;

	if (!(list_header_ptr && callback_func))
	{
		func_exception = naughty_exception_nullptr;
		goto func_end;
	}
	for (struct naughty_list_node *list_node_ptr = list_header_ptr->first_node; list_node_ptr; list_node_ptr = list_node_ptr->next)
	{
		callback_func(list_node_ptr, callback_func_param);
	}

func_end:
	return func_exception;
}

/**_Description
 *  @Set the list header's memory_alloc and memory_free function.
 * _Parameters
 *  @list_header_ptr: Pointer of list header.
 *  @mem_alloc: Memory alloc function.
 *  @mem_free: Memory free function.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_list_set_memory_function(struct naughty_list_header *list_header_ptr, void *(*mem_alloc)(size_t), void (*mem_free)(void *))
{
	naughty_exception func_exception = naughty_exception_no;

	if (!(list_header_ptr && mem_alloc && mem_free))
	{
		func_exception = naughty_exception_nullptr;
		goto func_end;
	}
	list_header_ptr->memory_alloc = mem_alloc;
	list_header_ptr->memory_free = mem_free;

func_end:
	return func_exception;
}

#ifdef __cpluscplus
}
#endif
