#pragma once

#include "naughty_list.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct naughty_heap_list_container_t
{
    uint64_t verify_number;
    struct naughty_list_node heap_list_node;
    size_t is_under_using;
};

struct naughty_heap_t
{
    uint64_t verify_number;

    struct naughty_list_header heap_list_header;
    void *begin_addr;
    void *end_addr;
};

#define naughty_heap_get_block_data_address(heap_list_container_ptr) ((byte_t*)heap_list_container_ptr + sizeof(struct naughty_heap_list_container_t))

naughty_exception naughty_heap_initialize(struct naughty_heap_t *heap_ptr, void *begin_addr, void *end_addr, uint64_t verify_number);

naughty_exception naughty_heap_get_node_block_size(struct naughty_heap_list_container_t *heap_node_ptr, size_t *output_size_ptr);

naughty_exception naughty_heap_take_block(struct naughty_heap_t *heap_ptr, struct naughty_heap_list_container_t *heap_node_ptr, size_t take_size);

naughty_exception naughty_heap_all_blocks_merge(struct naughty_heap_t *heap_ptr);

naughty_exception naughty_heap_alloc(struct naughty_heap_t *heap_ptr, size_t size, void **output_addr_ptr);

naughty_exception naughty_heap_free(struct naughty_heap_t *heap_ptr, void *addr);

#ifdef __cplusplus
}
#endif
