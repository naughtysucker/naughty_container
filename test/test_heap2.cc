#include <cstdint>
#include <random>
#include <map>
#include <utility>
#include <vector>
#include <cstdio>
#include <iostream>
#include <thread>
#include <mutex>
#include <cassert>
#include <cstring>

#include "naughty_heap.h"

uint8_t g_heap_buf[(size_t)1024*1024*1024];

struct naughty_heap_t g_heap_ins;


std::random_device r_dev;
std::mt19937 r_eng(r_dev());
std::mutex mtx_r_eng;

std::pair<size_t, void*> alloc_random_memory(std::pair<size_t, size_t> rand)
{
	std::lock_guard<std::mutex> lckgrd(mtx_r_eng);

	size_t r_index = 0;

	std::uniform_int_distribution<size_t> r_uni(rand.first, rand.second);

	size_t r_siz = r_uni(r_eng);

	void* ptr = nullptr;

    naughty_exception res = naughty_heap_alloc(&g_heap_ins, r_siz, &ptr);
    assert(res == naughty_exception_no);
    assert(!((size_t)ptr % NAUGHTY_HEAP_ALIGN_BYTE_COUNT));

    memset(ptr, 0xCC, r_siz);

	assert(ptr);

	if (!ptr)
	{
		std::cout << "Memory allocate Failed" << std::endl;
		// while (1);
		exit(114514);
	}

	return { r_siz, ptr };
}

std::mutex mtx_cout;

std::mutex mtx_task;
void test_task(std::pair<size_t, size_t> rand, size_t max_siz)
{
	size_t total_siz = 0;

	std::vector<std::pair<size_t, void*>> mem_map;
	uint64_t loop = 0;
	while (1)
	{
		{
			std::lock_guard<std::mutex> lck_grd_task(mtx_task);

			loop++;
			{
				std::lock_guard<std::mutex> lckgrd(mtx_cout);
				std::cout << rand.first << "-" << rand.second << "  loop: " << loop << "       objs count: " << mem_map.size() << std::endl;
			}
			if (total_siz > max_siz)
			{
				std::uniform_int_distribution<size_t> rdist(0, 1);

				size_t if_remove;
				{
					std::lock_guard<std::mutex> lckgrd(mtx_r_eng);
					if_remove = rdist(r_eng);
				}

				if (if_remove)
				{
					if (mem_map.size())
					{
						std::uniform_int_distribution<size_t> rdist(0, mem_map.size() - 1);

						size_t remove_item_index;
						{
							std::lock_guard<std::mutex> lckgrd(mtx_r_eng);
							remove_item_index = rdist(r_eng);
						}

                        naughty_exception res = naughty_heap_free(&g_heap_ins, mem_map[remove_item_index].second);
                        assert(res == naughty_exception_no);

						total_siz -= mem_map[remove_item_index].first;

						{
							std::lock_guard<std::mutex> lckgrd(mtx_cout);
							printf("%ld-%ld dealloced %ld \t\t\t {} \t\t\t total_siz %ld \r\n", rand.first, rand.second, mem_map[remove_item_index].first, mem_map[remove_item_index].second, total_siz);
						}
						mem_map.erase(mem_map.begin() + remove_item_index);
					}
					else
					{
						assert(0);
					}
				}
			}
			else
			{
				std::uniform_int_distribution<size_t> rdist(0, 1);

				size_t if_alloc;
				{
					std::lock_guard<std::mutex> lckgrd(mtx_r_eng);
					if_alloc = rdist(r_eng);
				}

				if (if_alloc)
				{
					std::pair<size_t, void*> mem_obj = alloc_random_memory(rand);
					total_siz += mem_obj.first;
					mem_map.push_back(mem_obj);
					{
						std::lock_guard<std::mutex> lckgrd(mtx_cout);
						printf("%ld-%ld alloced %ld \t\t\t %ld \t\t\t total_siz %ld", rand.first, rand.second, mem_obj.first, mem_obj.second, total_siz);
					}
				}
			}
		}
		std::this_thread::yield();
	}
}

void test_max_malloc()
{
	size_t total_siz = 0;

	while (1)
	{
		size_t siz = 1024 * 1024 * 10;
		void *ptr = malloc(siz);

		if (!ptr)
		{
			std::cout << "Max Malloc Size: " << total_siz << std::endl;
			exit(0);
		}

		total_siz += siz;
	}
}

int main()
{
	// test();

	// test_max_malloc();

    naughty_exception res = naughty_heap_initialize(&g_heap_ins, g_heap_buf + 1, g_heap_buf - 1 + (size_t)1024*1024*1024, 0xCCBBAADD);
    assert(res == naughty_exception_no);

	std::vector<std::thread> threads;

	// threads.push_back(std::thread(test_task, std::pair<size_t, size_t>{ 1024 * 1024, 1024 * 1024 * 128 }, 1024 * 1024 * 512)); // 1M-128M, 512

	threads.push_back(std::thread(test_task, std::pair<size_t, size_t>{ 65536, 1024 * 1024 * 1 }, 1024 * 1024 * 256)); // 64K-1M, 256M
	// threads.push_back(std::thread(test_task, std::pair<size_t, size_t>{ 65536, 1024 * 1024 * 1 }, 1024 * 1024 * 512)); // 64K-1M, 512M

	threads.push_back(std::thread(test_task, std::pair<size_t, size_t>{ 1, 65536 }, 1024 * 1024 * 128)); // 1-64K, 128M

	for (std::thread& th : threads)
	{
		th.join();
	}

	return 0;
}
