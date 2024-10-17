#include "naughty_heap.h"
#include <cstdio>
#include <vector>
#include <random>

struct naughty_heap_t g_heap_ins;

int main()
{
    void *buf_all = malloc(1024*1024*1024);

    naughty_exception res = naughty_heap_initialize(&g_heap_ins, buf_all, (byte_t*)buf_all + 1024*1024*1024, 0xABCD1234);
    assert(res == naughty_exception_no);
    printf("Total Buffer: 0x%p     Size: %d \r\n", buf_all ,1024*1024*1024);

    std::random_device r_dev;
    std::mt19937 r_eng(r_dev());
    std::uniform_int_distribution<size_t> r_uni(1, 1024*1024*5);

    std::vector<void*> bufs(100);
    for (int i = 0; i < 100; i++)
    {
        size_t r_siz = r_uni(r_eng);

        res = naughty_heap_alloc(&g_heap_ins, r_siz, &bufs[i]);
        assert(res == naughty_exception_no);
        printf("Alloc Buffer: 0x%p     Size: %d \r\n", bufs[i], r_siz);
    }

    for (int i = 0; i < 100; i++)
    {
        res = naughty_heap_free(&g_heap_ins, bufs[i]);
        assert(res == naughty_exception_no);
        printf("Free ptr: 0x%p \r\n", bufs[i]);
    }
#if 0
    res = naughty_heap_free(&g_heap_ins, bufs[0]);
    printf("res: %d\r\n", res);
    assert(res == naughty_exception_no);
#endif

    res = naughty_heap_alloc(&g_heap_ins, 1024*1024*1024 - sizeof(struct naughty_heap_list_container_t)*2 + 1, &bufs[0]);
    printf("res: %d\r\n", res);
    assert(res == naughty_exception_no);

    return 0;
}