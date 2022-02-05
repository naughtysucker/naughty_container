/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_avltree.c
 * @ Author: carl
 * @ Email: naughtygeng@qq.com
 * @ Created Time: 2021-10月-10(星期日) 22:41:32
 * @ All Rights Reserved
 * *****************************************************************************/

#include "naughty_avltree.h"

#ifdef __cplusplus
extern "C"
{
#endif

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
naughty_exception naughty_avltree_alloc_node_by_size(struct naughty_avltree_header *avltree_header_ptr, size_t container_memory_size, size_t data_memory_size, ssize_t offset, struct naughty_avltree_node **avltree_node_ptr_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	assert(avltree_header_ptr && avltree_node_ptr_ptr);

	void *container_memory_ptr = avltree_header_ptr->memory_alloc(container_memory_size);
	void *data_memory_ptr = NULL;
	if (data_memory_size)
	{
		data_memory_ptr = avltree_header_ptr->memory_alloc(data_memory_size);
	}
	if (!container_memory_ptr || (data_memory_size && !data_memory_ptr))
	{
		func_exception = naughty_exception_alloc;

		if (container_memory_ptr)
		{
			avltree_header_ptr->memory_free(container_memory_ptr);
		}
		if (data_memory_ptr)
		{
			avltree_header_ptr->memory_free(data_memory_ptr);
		}
		goto func_alloc_exception;
	}
	*avltree_node_ptr_ptr = (struct naughty_avltree_node *)((uint8_t *)container_memory_ptr + offset);
	((struct naughty_avltree_node *)*avltree_node_ptr_ptr)->data_ptr = data_memory_ptr;

func_end:
func_alloc_exception:
	return func_exception;
}

/**_Description
 *  @Release one Avl Tree Node.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl tree header.
 *  @avltree_node_ptr: Pointer of avl tree node.
 *  @offset: Address of container minus address of avl tree node's pointer.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_release_node_by_offset(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node *avltree_node_ptr, ssize_t offset)
{
	naughty_exception func_exception = naughty_exception_no;

	assert(avltree_header_ptr && avltree_node_ptr);

	if (avltree_node_ptr->data_ptr)
	{
		avltree_header_ptr->memory_free(avltree_node_ptr->data_ptr);
	}
	avltree_header_ptr->memory_free((uint8_t *)avltree_node_ptr + offset);

func_end:
	return func_exception;
}

/**_Description
 *  @Initial avl tree.
 * _Parameters
 *  @header_ptr: Avl tree's header.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_initial(struct naughty_avltree_header *header_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	assert(header_ptr);

	header_ptr->root_node = NULL;
	header_ptr->size = 0;
	header_ptr->compare_function = NULL;
	header_ptr->memory_alloc = malloc;
	header_ptr->memory_free = free;

	return func_exception;
}

/**_Description
 *  @Set Avl Tree's Malloc and Free functions.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl tree's header.
 *  @mem_alloc: Function to alloc memory.
 *  @mem_free: Function to free memory.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_set_memory_function(struct naughty_avltree_header *avltree_header_ptr, void *(*mem_alloc)(size_t), void (*mem_free)(void *))
{
	naughty_exception func_exception = naughty_exception_no;

	assert(avltree_header_ptr && mem_alloc && mem_free);

	avltree_header_ptr->memory_alloc = mem_alloc;
	avltree_header_ptr->memory_free = mem_free;

func_end:
	return func_exception;
}

/**_Description
 *  @Avl Tree Node Right Rotation.
 * _Parameters
 *  @node_ptr: Avl tree node's pointer.
 *  @output_root_node_ptr_ptr: Pointer of rotated tree's root node.
 * _Return
 *  @Exceptions
 */
static naughty_exception naughty_avltree_right_rotate(struct naughty_avltree_node *node_ptr, struct naughty_avltree_node **output_root_node_ptr_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	struct naughty_avltree_node *swap_node_ptr;

	assert(node_ptr);

	swap_node_ptr = node_ptr->left_node;
	if (swap_node_ptr)
	{
		node_ptr->left_node = swap_node_ptr->right_node;
		if (swap_node_ptr->right_node)
		{
			swap_node_ptr->right_node->parent_node = node_ptr;
		}
		swap_node_ptr->right_node = node_ptr;
		swap_node_ptr->parent_node = node_ptr->parent_node;
		node_ptr->parent_node = swap_node_ptr;
	}
	node_ptr->height = NAUGHTY_AVLTREE_MAX(NAUGHTY_AVLTREE_HEIGHT(node_ptr->left_node), NAUGHTY_AVLTREE_HEIGHT(node_ptr->right_node)) + 1;
	if (swap_node_ptr)
	{
		swap_node_ptr->height = NAUGHTY_AVLTREE_MAX(NAUGHTY_AVLTREE_HEIGHT(swap_node_ptr->left_node), NAUGHTY_AVLTREE_HEIGHT(swap_node_ptr->right_node)) + 1;
	}

	// swap_node_ptr->parent_node = NULL;

	*output_root_node_ptr_ptr = swap_node_ptr;

func_end:
	return func_exception;
}

/**_Description
 *  @Avl Tree Node Left Rotation.
 * _Parameters
 *  @node_ptr: Avl tree node's pointer.
 *  @output_root_node_ptr_ptr: Pointer of rotated tree's root node's pointer.
 * _Return
 *  @Exceptions
 */
static naughty_exception naughty_avltree_left_rotate(struct naughty_avltree_node *node_ptr, struct naughty_avltree_node **output_root_node_ptr_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	struct naughty_avltree_node *swap_node_ptr;

	assert(node_ptr);

	swap_node_ptr = node_ptr->right_node;
	if (swap_node_ptr)
	{
		node_ptr->right_node = swap_node_ptr->left_node;
		if (swap_node_ptr->left_node)
		{
			swap_node_ptr->left_node->parent_node = node_ptr;
		}
		swap_node_ptr->left_node = node_ptr;
		swap_node_ptr->parent_node = node_ptr->parent_node;
		node_ptr->parent_node = swap_node_ptr;
	}
	node_ptr->height = NAUGHTY_AVLTREE_MAX(NAUGHTY_AVLTREE_HEIGHT(node_ptr->left_node), NAUGHTY_AVLTREE_HEIGHT(node_ptr->right_node)) + 1;
	if (swap_node_ptr)
	{
		swap_node_ptr->height = NAUGHTY_AVLTREE_MAX(NAUGHTY_AVLTREE_HEIGHT(swap_node_ptr->left_node), NAUGHTY_AVLTREE_HEIGHT(swap_node_ptr->right_node)) + 1;
	}

	// swap_node_ptr->parent_node = NULL;

	*output_root_node_ptr_ptr = swap_node_ptr;

func_end:
	return func_exception;
}

/**_Description
 *  @Left rotate Node's left-child and then right rotate Node.
 * _Parameters
 *  @node_ptr: Pointer of avl tree node.
 *  @output_root_node_ptr_ptr: Pointer of rotated avl tree's root node's pointer.
 * _Return
 *  @Exceptions
 */
static naughty_exception naughty_avltree_left_right_rotate(struct naughty_avltree_node *node_ptr, struct naughty_avltree_node **output_root_node_ptr_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	assert(node_ptr);

	func_exception = naughty_avltree_left_rotate(node_ptr->left_node, &node_ptr->left_node);
	assert(func_exception == naughty_exception_no);

	func_exception = naughty_avltree_right_rotate(node_ptr, output_root_node_ptr_ptr);
	assert(func_exception == naughty_exception_no);

func_end:
	return func_exception;
}

/**_Description
 *  @Right rotate Node's right-child and then left rotate Node.
 * _Parameters
 *  @node_ptr: Pointer of avl tree node.
 *  @output_root_node_ptr_ptr: Pointer of rotated avl tree's root node's pointer.
 * _Return
 *  @Exceptions
 */
static naughty_exception naughty_avltree_right_left_rotate(struct naughty_avltree_node *node_ptr, struct naughty_avltree_node **output_root_node_ptr_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	assert(node_ptr);

	func_exception = naughty_avltree_right_rotate(node_ptr->right_node, &node_ptr->right_node);
	assert(func_exception == naughty_exception_no);

	func_exception = naughty_avltree_left_rotate(node_ptr, output_root_node_ptr_ptr);
	assert(func_exception == naughty_exception_no);

func_end:
	return func_exception;
}

/**_Description
 *  @Reform the avl tree.
 * _Parameters
 *  @leaf_node_ptr: Pointer of the leaf node.
 *  @output_avltree_root_node_ptr_ptr: Pointer of avl tree root node's pointer.
 * _Return
 *  @Exceptions
 */
static naughty_exception naughty_avltree_reform(struct naughty_avltree_node *leaf_node_ptr, struct naughty_avltree_node **output_avltree_root_node_ptr_ptr)
{
    naughty_exception func_exception = naughty_exception_no;
    struct naughty_avltree_node *current_root_node_ptr = NULL;
    for (struct naughty_avltree_node *node_ptr = leaf_node_ptr; node_ptr; node_ptr = node_ptr->parent_node)
    {
		current_root_node_ptr = 0;
        if ((ssize_t)NAUGHTY_AVLTREE_HEIGHT(node_ptr->left_node) - (ssize_t)NAUGHTY_AVLTREE_HEIGHT(node_ptr->right_node) > 1)
        {
			// Right Rotate
            if (NAUGHTY_AVLTREE_HEIGHT(node_ptr->left_node->left_node) > NAUGHTY_AVLTREE_HEIGHT(node_ptr->left_node->right_node))
			{
				// Rotate Directly
				func_exception = naughty_avltree_right_rotate(node_ptr, &current_root_node_ptr);
			}
			else
			{
				// Rotate Left Child Node First
				func_exception = naughty_avltree_left_right_rotate(node_ptr, &current_root_node_ptr);
			}
			assert(func_exception == naughty_exception_no);
        }
		else if ((ssize_t)NAUGHTY_AVLTREE_HEIGHT(node_ptr->right_node) - (ssize_t)NAUGHTY_AVLTREE_HEIGHT(node_ptr->left_node) > 1)
		{
			// Left Rotate
			if (NAUGHTY_AVLTREE_HEIGHT(node_ptr->right_node->right_node) > NAUGHTY_AVLTREE_HEIGHT(node_ptr->right_node->left_node))
			{
				// Rotate Directly
				func_exception = naughty_avltree_left_rotate(node_ptr, &current_root_node_ptr);
			}
			else
			{
				// Rotate Right Child Node First
				func_exception = naughty_avltree_right_left_rotate(node_ptr, &current_root_node_ptr);
			}
			assert(func_exception == naughty_exception_no);
		}
		else
		{
			// No Rotation
			node_ptr->height = NAUGHTY_AVLTREE_MAX(NAUGHTY_AVLTREE_HEIGHT(node_ptr->left_node), NAUGHTY_AVLTREE_HEIGHT(node_ptr->right_node)) + 1;
		}

		struct naughty_avltree_node *temp_node_ptr = node_ptr;
		if (current_root_node_ptr)
		{
			if (current_root_node_ptr->parent_node)
			{
				if (current_root_node_ptr->parent_node->left_node == node_ptr)
				{
					current_root_node_ptr->parent_node->left_node = current_root_node_ptr;
				}
				else
				{
					current_root_node_ptr->parent_node->right_node = current_root_node_ptr;
				}
			}
			node_ptr = current_root_node_ptr;
		}
		else
		{
			current_root_node_ptr = node_ptr;
		}
    }

	*output_avltree_root_node_ptr_ptr = current_root_node_ptr;
func_normal_end:
	return func_exception;
}

/**_Description
 *  @Insert One Node Into AVL Tree
 * _Parameters
 *  @avltree_header_ptr: Pointer of AVL Tree's header.
 *  @inserted_node_ptr: Pointer of inserted node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_insert_node(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node *inserted_node_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	assert(avltree_header_ptr && inserted_node_ptr);

	inserted_node_ptr->left_node = NULL;
	inserted_node_ptr->right_node = NULL;
	inserted_node_ptr->height = 1;
	if (!avltree_header_ptr->root_node)
	{
		avltree_header_ptr->root_node = inserted_node_ptr;
		inserted_node_ptr->parent_node = NULL;
		goto func_insert_as_root_end;
	}

	struct naughty_avltree_node *parent_of_inserted_node_ptr = avltree_header_ptr->root_node;
	for (struct naughty_avltree_node *node_ptr = parent_of_inserted_node_ptr; node_ptr; )
	{
		if (avltree_header_ptr->compare_function(inserted_node_ptr, node_ptr) < 0)
		{
			node_ptr = node_ptr->left_node;
		}
		else
		{
			node_ptr = node_ptr->right_node;
		}
		if (node_ptr)
		{
			parent_of_inserted_node_ptr = node_ptr;
		}
	}

	if (avltree_header_ptr->compare_function(inserted_node_ptr, parent_of_inserted_node_ptr) < 0)
	{
		parent_of_inserted_node_ptr->left_node = inserted_node_ptr;
	}
	else
	{
		parent_of_inserted_node_ptr->right_node = inserted_node_ptr;
	}
	inserted_node_ptr->parent_node = parent_of_inserted_node_ptr;

	// Reform the avl tree.
	struct naughty_avltree_node *after_reform_root_ptr = NULL;
	func_exception = naughty_avltree_reform(parent_of_inserted_node_ptr, &after_reform_root_ptr);
	avltree_header_ptr->root_node = after_reform_root_ptr;

func_end:
func_insert_as_root_end:
	avltree_header_ptr->size++;
func_exception_occurred:
	return func_exception;
}

/**_Description
 *  @Get parent of node and find the node is it's parent's left child or right child.
 * _Parameters
 *  @node_ptr_ptr: Pointer of node's pointer.
 *  @status_ptr: Pointer of status variable.
 * _Return
 *  @Exceptions
 */
static naughty_exception naughty_avltree_traversal_return_to_parent_and_set_status(struct naughty_avltree_node **node_ptr_ptr, int32_t *status_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	assert(node_ptr_ptr && status_ptr);

	if ((*node_ptr_ptr)->parent_node)
	{
		if (*node_ptr_ptr == (*node_ptr_ptr)->parent_node->left_node)
		{
			*status_ptr = -1;
		}
		else // if (node_ptr == node_ptr->parent_node->right_node)
		{
			*status_ptr = 1;
		}
	}
	*node_ptr_ptr = (*node_ptr_ptr)->parent_node;

func_normal_end:
	return func_exception;
}

/**_Description
 *  @Pre traversal avl tree and execute callback_func when traversal each node.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl tree's header.
 *  @callback_function: Call Back Function when traversal each node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_pre_traversal(struct naughty_avltree_header *avltree_header_ptr, void (*callback_function)(struct naughty_avltree_node *))
{
	assert(avltree_header_ptr);

	naughty_exception func_exception = naughty_exception_no;

	// traversal status
	// 0: from parent node
	// -1: from left child node
	// 1: from right child node
	int32_t traversal_status = 0;

	for (struct naughty_avltree_node *node_ptr = avltree_header_ptr->root_node; node_ptr; )
	{
		if (traversal_status == 0)
		{
			// 
			callback_function(node_ptr);
			if (node_ptr->left_node)
			{
				node_ptr = node_ptr->left_node;
			}
			else
			{
				traversal_status = -1;
			}
		}
		else if (traversal_status == -1)
		{
			if (node_ptr->right_node)
			{
				node_ptr = node_ptr->right_node;
				traversal_status = 0;
			}
			else
			{
				traversal_status = 1;
			}
		}
		else if (traversal_status == 1)
		{
			func_exception = naughty_avltree_traversal_return_to_parent_and_set_status(&node_ptr, &traversal_status);
		}

		assert(func_exception == naughty_exception_no);
	}

func_normal_end:
	return func_exception;
}

/**_Description
 *  @Get the previous node of current node.
 * _Parameters
 *  @current_node_ptr: Pointer of current node.
 *  @previous_node_ptr_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_get_previous_node(struct naughty_avltree_node *current_node_ptr, struct naughty_avltree_node **previous_node_ptr_ptr)
{
	assert(current_node_ptr && previous_node_ptr_ptr);
	naughty_exception func_exception = naughty_exception_no;

	struct naughty_avltree_node *max_left_side_node_ptr = NULL;
	struct naughty_avltree_node *previous_node_ptr = NULL;
	for (struct naughty_avltree_node *left_side_node_ptr = current_node_ptr->left_node; left_side_node_ptr; )
	{
		max_left_side_node_ptr = left_side_node_ptr;
		left_side_node_ptr = left_side_node_ptr->right_node;
	}
	if (max_left_side_node_ptr)
	{
		previous_node_ptr = max_left_side_node_ptr;
	}
	else
	{
		for ( ; current_node_ptr->parent_node; )
		{
			if (current_node_ptr == current_node_ptr->parent_node->right_node)
			{
				previous_node_ptr = current_node_ptr->parent_node;
				break;
			}
			current_node_ptr = current_node_ptr->parent_node;
		}
	}

	*previous_node_ptr_ptr = previous_node_ptr;

func_end:
	return func_exception;
}

/**_Description
 *  @Get the next node of current node.
 * _Parameters
 *  @current_node_ptr: Pointer of current node.
 *  @next_node_ptr_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_get_next_node(struct naughty_avltree_node *current_node_ptr, struct naughty_avltree_node **next_node_ptr_ptr)
{
	assert(current_node_ptr && next_node_ptr_ptr);
	naughty_exception func_exception = naughty_exception_no;

	struct naughty_avltree_node *least_right_side_node_ptr = NULL;
	struct naughty_avltree_node *next_node_ptr = NULL;
	for (struct naughty_avltree_node *right_side_node_ptr = current_node_ptr->right_node; right_side_node_ptr; )
	{
		least_right_side_node_ptr = right_side_node_ptr;
		right_side_node_ptr = right_side_node_ptr->left_node;
	}
	if (least_right_side_node_ptr)
	{
		next_node_ptr = least_right_side_node_ptr;
	}
	else
	{
		for ( ; current_node_ptr->parent_node; )
		{
			if (current_node_ptr == current_node_ptr->parent_node->left_node)
			{
				next_node_ptr = current_node_ptr->parent_node;
				break;
			}
			current_node_ptr = current_node_ptr->parent_node;
		}
	}

	*next_node_ptr_ptr = next_node_ptr;

func_end:
	return func_exception;
}

/**_Description
 *  @Medium traversal avl tree and execute callback_func when traversal each node.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl tree's header.
 *  @callback_function: Call Back Function when traversal each node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_medium_traversal(struct naughty_avltree_header *avltree_header_ptr, void (*callback_function)(struct naughty_avltree_node *))
{
	assert(avltree_header_ptr);

	naughty_exception func_exception = naughty_exception_no;

	// traversal status
	// 0: from parent node
	// -1: from left child node
	// 1: from right child node
	int32_t traversal_status = 0;

	for (struct naughty_avltree_node *node_ptr = avltree_header_ptr->root_node; node_ptr; )
	{
		if (traversal_status == 0)
		{
			if (node_ptr->left_node)
			{
				node_ptr = node_ptr->left_node;
			}
			else
			{
				traversal_status = -1;
			}
		}
		else if (traversal_status == -1)
		{
			// 
			callback_function(node_ptr);
			if (node_ptr->right_node)
			{
				node_ptr = node_ptr->right_node;
				traversal_status = 0;
			}
			else
			{
				traversal_status = 1;
			}
		}
		else if (traversal_status == 1)
		{
			func_exception = naughty_avltree_traversal_return_to_parent_and_set_status(&node_ptr, &traversal_status);
		}

		assert(func_exception == naughty_exception_no);
	}

func_normal_end:
	return func_exception;
}

/**_Description
 *  @Post traversal avl tree and execute callback_func when traversal each node.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl tree's header.
 *  @callback_function: Call Back Function when traversal each node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_post_traversal(struct naughty_avltree_header *avltree_header_ptr, void (*callback_function)(struct naughty_avltree_node *))
{
	assert(avltree_header_ptr);

	naughty_exception func_exception = naughty_exception_no;

	// traversal status
	// 0: from parent node
	// -1: from left child node
	// 1: from right child node
	int32_t traversal_status = 0;

	for (struct naughty_avltree_node *node_ptr = avltree_header_ptr->root_node; node_ptr; )
	{
		if (traversal_status == 0)
		{
			if (node_ptr->left_node)
			{
				node_ptr = node_ptr->left_node;
			}
			else
			{
				traversal_status = -1;
			}
		}
		else if (traversal_status == -1)
		{
			if (node_ptr->right_node)
			{
				node_ptr = node_ptr->right_node;
				traversal_status = 0;
			}
			else
			{
				traversal_status = 1;
			}
		}
		else if (traversal_status == 1)
		{
			// 
			callback_function(node_ptr);
			func_exception = naughty_avltree_traversal_return_to_parent_and_set_status(&node_ptr, &traversal_status);
		}

		assert(func_exception == naughty_exception_no);
	}

func_normal_end:
	return func_exception;
}

/**_Description
 *  @Travel avl tree and release all nodes.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl tree's header.
 *  @offset: Address of container minus address of avl tree node's pointer.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_release_all_nodes_by_offset(struct naughty_avltree_header *avltree_header_ptr, ssize_t offset)
{
	naughty_exception func_exception = naughty_exception_no;

	assert(avltree_header_ptr);

	// traversal status
	// 0: from parent node
	// -1: from left child node
	// 1: from right child node
	int32_t traversal_status = 0;

	for (struct naughty_avltree_node *node_ptr = avltree_header_ptr->root_node; node_ptr; )
	{
		if (traversal_status == 0)
		{
			if (node_ptr->left_node)
			{
				node_ptr = node_ptr->left_node;
			}
			else
			{
				traversal_status = -1;
			}
		}
		else if (traversal_status == -1)
		{
			if (node_ptr->right_node)
			{
				node_ptr = node_ptr->right_node;
				traversal_status = 0;
			}
			else
			{
				traversal_status = 1;
			}
		}
		else if (traversal_status == 1)
		{
			// 
			struct naughty_avltree_node *release_node_ptr = node_ptr;
			func_exception = naughty_avltree_traversal_return_to_parent_and_set_status(&node_ptr, &traversal_status);
			assert(func_exception == naughty_exception_no);
			func_exception = naughty_avltree_release_node_by_offset(avltree_header_ptr, release_node_ptr, offset);
			assert(func_exception == naughty_exception_no);
		}
	}

func_normal_end:
	return func_exception;
}

/**_Description
 *  @Output first avltree_node_ptr where avl_tree_header->compare_function() returns 0.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl_tree's header.
 *  @avltree_node_ptr_for_compare: The node to compare with nodes in avltree.
 *  @output_node_ptr_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_find_node(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node *avltree_node_ptr_for_compare, struct naughty_avltree_node **output_node_ptr_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	*output_node_ptr_ptr = NULL;
	for (struct naughty_avltree_node *avltree_node_ptr = avltree_header_ptr->root_node; avltree_node_ptr; )
	{
		if (avltree_header_ptr->compare_function(avltree_node_ptr_for_compare, avltree_node_ptr) < 0)
		{
			avltree_node_ptr = avltree_node_ptr->left_node;
		}
		else if (avltree_header_ptr->compare_function(avltree_node_ptr_for_compare, avltree_node_ptr) > 0)
		{
			avltree_node_ptr = avltree_node_ptr->right_node;
		}
		else
		{
			*output_node_ptr_ptr = avltree_node_ptr;
			break;
		}
	}

func_end:
	return func_exception;
}

/**_Description
 *  @Remove the node which the pointer points from avl tree.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl_tree's header.
 *  @node_to_remove_ptr: Pointer of the node.
 *  @offset: Address of container subs address of avltree_node.
 * _Return
 *  @Exception
 */
naughty_exception naughty_avltree_remove_node_according_to_node_pointer_by_offset(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node *node_to_remove_ptr, ssize_t offset)
{
	naughty_exception func_exception = naughty_exception_no;

	struct naughty_avltree_node *reform_leaf_node_ptr = node_to_remove_ptr->parent_node;
	struct naughty_avltree_node *node_to_move_ptr = NULL;

	if (NAUGHTY_AVLTREE_HEIGHT(node_to_remove_ptr->left_node) < NAUGHTY_AVLTREE_HEIGHT(node_to_remove_ptr->right_node))
	{
		// Remove the Least node in Right
		struct naughty_avltree_node *least_right_side_node_ptr = NULL;
		for (struct naughty_avltree_node *right_side_node_ptr = node_to_remove_ptr->right_node; right_side_node_ptr; )
		{
			least_right_side_node_ptr = right_side_node_ptr;
			right_side_node_ptr = right_side_node_ptr->left_node;
		}
		//least_right_side_node_ptr Must not be NULL
		if (least_right_side_node_ptr->parent_node != node_to_remove_ptr)
		{
			// Modify Moved Node's Parent
			least_right_side_node_ptr->parent_node->left_node = least_right_side_node_ptr->right_node;
			if (least_right_side_node_ptr->right_node)
			{
				least_right_side_node_ptr->right_node->parent_node = least_right_side_node_ptr->parent_node;
			}
			// Modify Moved Node
			least_right_side_node_ptr->right_node = node_to_remove_ptr->right_node;
			least_right_side_node_ptr->left_node = node_to_remove_ptr->left_node;
			reform_leaf_node_ptr = least_right_side_node_ptr->parent_node;

		}
		else
		{
			// Modify Moved Node
			least_right_side_node_ptr->left_node = node_to_remove_ptr->left_node;
			reform_leaf_node_ptr = least_right_side_node_ptr;
		}
		node_to_move_ptr = least_right_side_node_ptr;
	}
	else
	{
		// Remove the Max node in Left
		struct naughty_avltree_node *max_left_side_node_ptr = NULL;
		for (struct naughty_avltree_node *left_side_node_ptr = node_to_remove_ptr->left_node; left_side_node_ptr; )
		{
			max_left_side_node_ptr = left_side_node_ptr;
			left_side_node_ptr = left_side_node_ptr->right_node;
		}
		//max_left_side_node_ptr Could be Null
		if (max_left_side_node_ptr)
		{
			if (max_left_side_node_ptr->parent_node != node_to_remove_ptr)
			{
				// Modify Moved Node's Parent
				max_left_side_node_ptr->parent_node->right_node = max_left_side_node_ptr->left_node;
				if (max_left_side_node_ptr->left_node)
				{
					max_left_side_node_ptr->left_node->parent_node = max_left_side_node_ptr->parent_node;
				}
				// Modify Moved Node
				max_left_side_node_ptr->right_node = node_to_remove_ptr->right_node;
				max_left_side_node_ptr->left_node = node_to_remove_ptr->left_node;
				reform_leaf_node_ptr = max_left_side_node_ptr->parent_node;
			}
			else
			{
				// Modify Moved Node
				max_left_side_node_ptr->right_node = node_to_remove_ptr->right_node;
				reform_leaf_node_ptr = max_left_side_node_ptr;
			}
			node_to_move_ptr = max_left_side_node_ptr;
		}
		else
		{
			// node_to_remove is the leaf node
			// Can be removed directly
		}
	}
	// Modify Removed Node's Parent
	if (node_to_remove_ptr->parent_node)
	{
		if (node_to_remove_ptr == node_to_remove_ptr->parent_node->left_node)
		{
			node_to_remove_ptr->parent_node->left_node = node_to_move_ptr;
		}
		else
		{
			node_to_remove_ptr->parent_node->right_node = node_to_move_ptr;
		}
	}
	else
	{
		avltree_header_ptr->root_node = node_to_move_ptr;
	}
	if (node_to_move_ptr)
	{
		node_to_move_ptr->parent_node = node_to_remove_ptr->parent_node;
		// Modify Child of Moved Node
		if (node_to_move_ptr->right_node)
		{
			node_to_move_ptr->right_node->parent_node = node_to_move_ptr;
		}
		if (node_to_move_ptr->left_node)
		{
			node_to_move_ptr->left_node->parent_node = node_to_move_ptr;
		}
	}
	struct naughty_avltree_node *after_reform_root_ptr = NULL;
	func_exception = naughty_avltree_reform(reform_leaf_node_ptr, &after_reform_root_ptr);
	if (func_exception != naughty_exception_no)
	{
		goto func_end;
	}
	avltree_header_ptr->root_node = after_reform_root_ptr;
	func_exception = naughty_avltree_release_node_by_offset(avltree_header_ptr, node_to_remove_ptr, offset);

func_end:
	return func_exception;
}

/**_Description
 *  @Remove first node from avl tree where avl_tree_header->compare_function() returns 0.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avl_tree's header.
 *  @avltree_node_ptr_for_compare: The node to compare with nodes in avltree. If compare_function returns 0, the node will be removed from avltree and be release.
 *  @offset: Address of container subs address of avltree_node.
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_remove_node_according_to_compare_by_offset(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node *avltree_node_ptr_for_compare, ssize_t offset)
{
	naughty_exception func_exception = naughty_exception_no;

	for (struct naughty_avltree_node *avltree_node_ptr = avltree_header_ptr->root_node; avltree_node_ptr; )
	{
		if (avltree_header_ptr->compare_function(avltree_node_ptr_for_compare, avltree_node_ptr) < 0)
		{
			avltree_node_ptr = avltree_node_ptr->left_node;
		}
		else if (avltree_header_ptr->compare_function(avltree_node_ptr_for_compare, avltree_node_ptr) > 0)
		{
			avltree_node_ptr = avltree_node_ptr->right_node;
		}
		else
		{
			func_exception = naughty_avltree_remove_node_according_to_node_pointer_by_offset(avltree_header_ptr, avltree_node_ptr, offset);
			break;
		}
	}

func_end:
	return func_exception;
}

/**_Description
 *  @Get the first node of avltree.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avltree header
 *  @first_node_ptr_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_get_first_node(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node **first_node_ptr_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	struct naughty_avltree_node *least_node_ptr = NULL;
	for (struct naughty_avltree_node *node_ptr = avltree_header_ptr->root_node; node_ptr; node_ptr = node_ptr->left_node)
	{
		least_node_ptr = node_ptr;
	}

	*first_node_ptr_ptr = least_node_ptr;

func_end:
	return func_exception;
}

/**_Description
 *  @Get the last node of avltree.
 * _Parameters
 *  @avltree_header_ptr: Pointer of avltree header
 *  @last_node_ptr_ptr: Output
 * _Return
 *  @Exceptions
 */
naughty_exception naughty_avltree_get_last_node(struct naughty_avltree_header *avltree_header_ptr, struct naughty_avltree_node **last_node_ptr_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	struct naughty_avltree_node *max_node_ptr = NULL;
	for (struct naughty_avltree_node *node_ptr = avltree_header_ptr->root_node; node_ptr; node_ptr = node_ptr->right_node)
	{
		max_node_ptr = node_ptr;
	}

	*last_node_ptr_ptr = max_node_ptr;

func_end:
	return func_exception;
}

#ifdef __cplusplus
}
#endif
