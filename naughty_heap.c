#include "naughty_heap.h"

#ifdef __cplusplus
extern "C"
{
#endif

naughty_exception naughty_heap_initialize(struct naughty_heap_t *heap_ptr, void *begin_addr, void *end_addr, uint64_t verify_number)
{
    naughty_exception func_res = naughty_exception_no;

    heap_ptr->verify_number = verify_number;

    func_res = naughty_list_initialize(&heap_ptr->heap_list_header, NULL);
    if (func_res != naughty_exception_no)
    {
        goto func_end;
    }

    void *aligned_begin_addr = naughty_heap_get_next_aligned_address(begin_addr);
    void *aligned_end_addr = (byte_t*)end_addr - (((size_t)end_addr - sizeof(struct naughty_heap_list_container_t)) % NAUGHTY_HEAP_ALIGN_BYTE_COUNT);

    heap_ptr->begin_addr = aligned_begin_addr;
    heap_ptr->end_addr = aligned_end_addr;
    
    if ((byte_t*)aligned_end_addr - (byte_t*)aligned_begin_addr < sizeof(struct naughty_heap_list_container_t)*2)
    {
        func_res = naughty_exception_runout;
        goto func_end;
    }

    ((struct naughty_heap_list_container_t *)aligned_begin_addr)->is_under_using = 0;
    ((struct naughty_heap_list_container_t *)aligned_begin_addr)->verify_number = heap_ptr->verify_number;
    ((struct naughty_heap_list_container_t *)aligned_end_addr - 1)->is_under_using = 1;
    ((struct naughty_heap_list_container_t *)aligned_end_addr - 1)->verify_number = heap_ptr->verify_number;

    naughty_list_insert_after(&heap_ptr->heap_list_header, &heap_ptr->heap_list_header, &((struct naughty_heap_list_container_t *)aligned_begin_addr)->heap_list_node);
    naughty_list_insert_after(&heap_ptr->heap_list_header, heap_ptr->heap_list_header.first_node, &((struct naughty_heap_list_container_t *)aligned_end_addr - 1)->heap_list_node);

func_end:
    return func_res;
}

naughty_exception naughty_heap_get_node_block_size(struct naughty_heap_list_container_t *heap_node_ptr, size_t *output_size_ptr)
{
    naughty_exception func_res = naughty_exception_no;

    if (heap_node_ptr->heap_list_node.next)
    {
        byte_t *unaligned_block_start_addr = naughty_heap_get_unaligned_block_data_address(heap_node_ptr);
        byte_t *aligned_block_start_addr = naughty_heap_get_next_aligned_address(unaligned_block_start_addr);

        *output_size_ptr = (byte_t*)naughty_container_of(heap_node_ptr->heap_list_node.next, struct naughty_heap_list_container_t, heap_list_node) - aligned_block_start_addr;
    }
    else
    {
        *output_size_ptr = 0;
    }

func_end:
    return func_res;
}

naughty_exception naughty_heap_take_block(struct naughty_heap_t *heap_ptr, struct naughty_heap_list_container_t *heap_node_ptr, size_t take_size)
{
    naughty_exception func_res = naughty_exception_no;

    assert(heap_node_ptr->verify_number == heap_ptr->verify_number);

    size_t block_size = 0;
    func_res = naughty_heap_get_node_block_size(heap_node_ptr, &block_size);
    if (func_res != naughty_exception_no)
    {
        goto func_end;
    }

    if (block_size < take_size)
    {
        func_res = naughty_exception_outofrange;
        goto func_end;
    }

    byte_t *unaligned_block_start_addr = naughty_heap_get_unaligned_block_data_address(heap_node_ptr);
    byte_t *aligned_block_start_addr = naughty_heap_get_next_aligned_address(unaligned_block_start_addr);
    void *unaligned_new_block_address = aligned_block_start_addr + take_size;
    void *aligned_new_block_address = naughty_heap_get_next_aligned_address(unaligned_new_block_address);

    if (heap_node_ptr->heap_list_node.next && aligned_new_block_address < (void *)(naughty_container_of(heap_node_ptr->heap_list_node.next, struct naughty_heap_list_container_t, heap_list_node) - 1))
    {
        struct naughty_heap_list_container_t *new_block_node = (struct naughty_heap_list_container_t*)aligned_new_block_address;
        new_block_node->is_under_using = 0;
        new_block_node->verify_number = heap_ptr->verify_number;
        func_res = naughty_list_insert_after(&heap_ptr->heap_list_header, &heap_node_ptr->heap_list_node, &new_block_node->heap_list_node);
        if (func_res != naughty_exception_no)
        {
            goto func_end;
        }
    }

    heap_node_ptr->is_under_using = 1;

func_end:
    return func_res;
}

naughty_exception naughty_heap_all_blocks_merge(struct naughty_heap_t *heap_ptr)
{
    naughty_exception func_res = naughty_exception_no;

    struct naughty_heap_list_container_t *previous_node_ptr = naughty_container_of(heap_ptr->heap_list_header.first_node, struct naughty_heap_list_container_t, heap_list_node);
    struct naughty_heap_list_container_t *current_node_ptr = naughty_container_of(heap_ptr->heap_list_header.first_node->next, struct naughty_heap_list_container_t, heap_list_node);

    while (current_node_ptr != naughty_container_of(heap_ptr->heap_list_header.last_node, struct naughty_heap_list_container_t, heap_list_node))
    {
        if (!previous_node_ptr->is_under_using && !current_node_ptr->is_under_using)
        {
            func_res = naughty_list_unlink_node(&heap_ptr->heap_list_header, &current_node_ptr->heap_list_node);
            if (func_res != naughty_exception_no)
            {
                goto func_end;
            }
        }
        else
        {
            previous_node_ptr = current_node_ptr;
        }
        current_node_ptr = naughty_container_of(previous_node_ptr->heap_list_node.next, struct naughty_heap_list_container_t, heap_list_node);
    }

func_end:
    return func_res;
}

naughty_exception naughty_heap_alloc(struct naughty_heap_t *heap_ptr, size_t size, void **output_addr_ptr)
{
    naughty_exception func_res = naughty_exception_runout;

    struct naughty_heap_list_container_t *node_ptr = naughty_container_of(heap_ptr->heap_list_header.first_node, struct naughty_heap_list_container_t, heap_list_node);

    while (node_ptr != naughty_container_of(heap_ptr->heap_list_header.last_node, struct naughty_heap_list_container_t, heap_list_node))
    {
        if (!node_ptr->is_under_using)
        {
            size_t current_node_size = 0;
            naughty_exception res = naughty_heap_get_node_block_size(node_ptr, &current_node_size);
            if (res != naughty_exception_no)
            {
                func_res = res;
                goto func_end;
            }
            if (size <= current_node_size)
            {
                func_res = naughty_heap_take_block(heap_ptr, node_ptr, size);
                byte_t *unaligned_block_start_addr = naughty_heap_get_unaligned_block_data_address(node_ptr);
                byte_t *aligned_block_start_addr = naughty_heap_get_next_aligned_address(unaligned_block_start_addr);
                *output_addr_ptr = aligned_block_start_addr;
                break;
            }
        }
        node_ptr = naughty_container_of(node_ptr->heap_list_node.next, struct naughty_heap_list_container_t, heap_list_node);
    }

func_end:
    return func_res;
}

naughty_exception naughty_heap_free(struct naughty_heap_t *heap_ptr, void *addr)
{
    naughty_exception func_res = naughty_exception_wrongparameter;

    struct naughty_heap_list_container_t *node_ptr = naughty_container_of(heap_ptr->heap_list_header.first_node, struct naughty_heap_list_container_t, heap_list_node);

    while (node_ptr != naughty_container_of(heap_ptr->heap_list_header.last_node, struct naughty_heap_list_container_t, heap_list_node))
    {
        byte_t *unaligned_block_start_addr = naughty_heap_get_unaligned_block_data_address(node_ptr);
        byte_t *aligned_block_start_addr = naughty_heap_get_next_aligned_address(unaligned_block_start_addr);
        if (aligned_block_start_addr == addr)
        {
            if (node_ptr->is_under_using)
            {
                if (node_ptr->heap_list_node.next && !naughty_container_of(node_ptr->heap_list_node.next, struct naughty_heap_list_container_t, heap_list_node)->is_under_using)
                {
                    func_res = naughty_list_unlink_node(&heap_ptr->heap_list_header, node_ptr->heap_list_node.next);
                    if (func_res != naughty_exception_no)
                    {
                        goto func_end;
                    }
                }
                if (node_ptr == naughty_container_of(heap_ptr->heap_list_header.first_node, struct naughty_heap_list_container_t, heap_list_node) || naughty_container_of(node_ptr->heap_list_node.previous, struct naughty_heap_list_container_t, heap_list_node)->is_under_using)
                {
                    node_ptr->is_under_using = 0;
                    func_res = naughty_exception_no;
                }
                else
                {
                    func_res = naughty_list_unlink_node(&heap_ptr->heap_list_header, &node_ptr->heap_list_node);
                    if (func_res != naughty_exception_no)
                    {
                        goto func_end;
                    }
                }
            }
            else
            {
                func_res = naughty_exception_double_free;
            }
            break;
        }
        node_ptr = naughty_container_of(node_ptr->heap_list_node.next, struct naughty_heap_list_container_t, heap_list_node);
    }

func_end:
    return func_res;
}


#ifdef __cplusplus
}
#endif
