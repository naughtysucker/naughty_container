/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_memory_diagnosis.c
 * @ Author: carl
 * @ Email: naughtygeng@qq.com
 * @ Created Time: 2021-8月-30(星期一) 05:55:19
 * @ All Rights Reserved
 * *****************************************************************************/

#include "naughty_memory_diagnosis.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct naughty_memory_diagnosis_struct naughty_global_memory_diagnosis_struct;

#if 0
void *naughty_global_memory_diagnosis_memory_alloc(size_t size)
{
	return naughty_memory_diagnosis_memory_alloc(&naughty_global_memory_diagnosis_struct, size);
}

void naughty_global_memory_diagnosis_memory_free(void *mem_ptr)
{
	return naughty_memory_diagnosis_memory_free(&naughty_global_memory_diagnosis_struct, mem_ptr);
}
#endif

naughty_exception naughty_global_memory_diagnosis_struct_initial(size_t size)
{
	return naughty_memory_diagnosis_struct_initial(&naughty_global_memory_diagnosis_struct, size);
}

naughty_exception naughty_global_memory_diagnosis_output_to_file(FILE *opened_fp)
{
	return naughty_memory_diagnosis_output_to_file(&naughty_global_memory_diagnosis_struct, opened_fp);
}

naughty_exception naughty_memory_diagnosis_output_to_file(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, FILE *opened_fp)
{
	naughty_exception func_exception = naughty_exception_no;

	pthread_mutex_lock(&mem_dia_struct_ptr->mutex);

	struct naughty_avltree_node *mem_dia_iter;

	func_exception = naughty_avltree_get_first_node(&mem_dia_struct_ptr->remained_node_avltree_header, &mem_dia_iter);
	if (func_exception != naughty_exception_no)
	{
		goto func_end;
	}

	while (mem_dia_iter)
	{

		struct naughty_memory_diagnosis_node *mem_dia_node_ptr = naughty_container_of(mem_dia_iter, struct naughty_memory_diagnosis_node, node);
		// Write current node to file
		fprintf(opened_fp, "Address: %p    |    Size: %zu\n", mem_dia_node_ptr->mem_ptr, mem_dia_node_ptr->mem_size);
		fprintf(opened_fp, "%s, %d\n", mem_dia_node_ptr->file_name, mem_dia_node_ptr->line_number);
		for (size_t i = 0; i < mem_dia_node_ptr->backframe_size; i++)
		{
			char *one_frame_info_str = mem_dia_node_ptr->backframe_info[i];
			fprintf(opened_fp, "%s\n", one_frame_info_str);
		}
		fprintf(opened_fp, "\n\n");

		func_exception = naughty_avltree_get_next_node(mem_dia_iter, &mem_dia_iter);
		if (func_exception != naughty_exception_no)
		{
			goto func_end;
		}
	}

func_end:
	pthread_mutex_unlock(&mem_dia_struct_ptr->mutex);
	return func_exception;
}

int32_t naughty_memory_diagnosis_avltree_node_compare(struct naughty_avltree_node *node1_ptr, struct naughty_avltree_node *node2_ptr)
{
	struct naughty_memory_diagnosis_node *node1_container = naughty_container_of(node1_ptr, struct naughty_memory_diagnosis_node, node); 
	struct naughty_memory_diagnosis_node *node2_container = naughty_container_of(node2_ptr, struct naughty_memory_diagnosis_node, node); 

	if (node1_container->mem_ptr < node2_container->mem_ptr)
	{
		return -1;
	}
	else if (node1_container->mem_ptr > node2_container->mem_ptr)
	{
		return 1;
	}
	return 0;
}

naughty_exception naughty_memory_diagnosis_struct_initial(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, size_t backtrace_depth)
{
	mem_dia_struct_ptr->memory_alloc = malloc;
	mem_dia_struct_ptr->memory_free = free;
	mem_dia_struct_ptr->memory_realloc = realloc;
	mem_dia_struct_ptr->memory_calloc = calloc;
	naughty_avltree_initial(&mem_dia_struct_ptr->remained_node_avltree_header);
	mem_dia_struct_ptr->remained_node_avltree_header.compare_function = naughty_memory_diagnosis_avltree_node_compare;
	mem_dia_struct_ptr->backtrace_stop_address = NULL;
	mem_dia_struct_ptr->max_backtrace_depth = backtrace_depth;
	mem_dia_struct_ptr->exception = naughty_exception_no;

	pthread_mutex_init(&mem_dia_struct_ptr->mutex, NULL);

	return naughty_exception_no;
}

naughty_exception naughty_memory_diagnosis_record_add(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, void *alloced_ptr, size_t size, const char *filename, int32_t line)
{
	naughty_exception func_exception = naughty_exception_no;

	if (alloced_ptr)
	{
		// record 

		struct naughty_avltree_node *avltree_node_ptr;
		func_exception = naughty_avltree_alloc_node_without_data(&mem_dia_struct_ptr->remained_node_avltree_header, struct naughty_memory_diagnosis_node, node, &avltree_node_ptr);
		if (func_exception != naughty_exception_no)
		{
			//
			goto func_end;
		}

		naughty_container_of(avltree_node_ptr, struct naughty_memory_diagnosis_node, node)->mem_ptr = alloced_ptr;
		naughty_container_of(avltree_node_ptr, struct naughty_memory_diagnosis_node, node)->mem_size = size;

#if USE_BACKTRACE
		// backtrace
		void *backtrace_array[mem_dia_struct_ptr->max_backtrace_depth];
		int backtrace_size = backtrace(backtrace_array, mem_dia_struct_ptr->max_backtrace_depth);

		naughty_container_of(avltree_node_ptr, struct naughty_memory_diagnosis_node, node)->backframe_info = backtrace_symbols(backtrace_array, backtrace_size);
		naughty_container_of(avltree_node_ptr, struct naughty_memory_diagnosis_node, node)->backframe_size = backtrace_size;
#else
		naughty_container_of(avltree_node_ptr, struct naughty_memory_diagnosis_node, node)->backframe_info = NULL;
		naughty_container_of(avltree_node_ptr, struct naughty_memory_diagnosis_node, node)->backframe_size = 0;
#endif
		// just record the filename and line number
		naughty_container_of(avltree_node_ptr, struct naughty_memory_diagnosis_node, node)->file_name = filename;
		naughty_container_of(avltree_node_ptr, struct naughty_memory_diagnosis_node, node)->line_number = line;

		pthread_mutex_lock(&mem_dia_struct_ptr->mutex);
		func_exception = naughty_avltree_insert_node(&mem_dia_struct_ptr->remained_node_avltree_header, avltree_node_ptr);
		pthread_mutex_unlock(&mem_dia_struct_ptr->mutex);
		if (func_exception != naughty_exception_no)
		{
			// 
			naughty_avltree_release_node(&mem_dia_struct_ptr->remained_node_avltree_header, avltree_node_ptr, struct naughty_avltree_node_container, node);
			goto func_end;
		}
	}
	else
	{
		func_exception = naughty_exception_nullptr;
	}
func_end:
	return func_exception;
}

naughty_exception naughty_memory_diagnosis_record_remove(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, void *mem_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	struct naughty_avltree_node *find_avltree_node_ptr;

	struct naughty_avltree_node *compared_avltree_node_ptr;

	if (!mem_dia_struct_ptr)
	{
		func_exception = naughty_exception_nullptr;
		goto func_end;
	}

	if (!mem_ptr)
	{
		func_exception = naughty_exception_nullptr;
		goto func_end;
	}

	func_exception = naughty_avltree_alloc_node_without_data(&mem_dia_struct_ptr->remained_node_avltree_header, struct naughty_memory_diagnosis_node, node, &compared_avltree_node_ptr);
	if (func_exception != naughty_exception_no)
	{
		goto func_end;
	}

	// Edit the Compared Node
	naughty_container_of(compared_avltree_node_ptr, struct naughty_memory_diagnosis_node, node)->mem_ptr = mem_ptr;
	pthread_mutex_lock(&mem_dia_struct_ptr->mutex);
	func_exception = naughty_avltree_find_node(&mem_dia_struct_ptr->remained_node_avltree_header, compared_avltree_node_ptr, &find_avltree_node_ptr);
	pthread_mutex_unlock(&mem_dia_struct_ptr->mutex);
	naughty_exception func_exception_temp = naughty_avltree_release_node(&mem_dia_struct_ptr->remained_node_avltree_header, compared_avltree_node_ptr, struct naughty_memory_diagnosis_node, node);
	if (func_exception != naughty_exception_no)
	{
		goto func_end;
	}
	if (func_exception_temp != naughty_exception_no)
	{
		func_exception = func_exception_temp;
		goto func_end;
	}

	// Release Backtrace Infomation.
	if (naughty_container_of(find_avltree_node_ptr, struct naughty_memory_diagnosis_node, node)->backframe_info)
	{
		free(naughty_container_of(find_avltree_node_ptr, struct naughty_memory_diagnosis_node, node)->backframe_info);
		naughty_container_of(find_avltree_node_ptr, struct naughty_memory_diagnosis_node, node)->backframe_info = NULL;
		naughty_container_of(find_avltree_node_ptr, struct naughty_memory_diagnosis_node, node)->backframe_size = 0;
	}
	pthread_mutex_lock(&mem_dia_struct_ptr->mutex);
	func_exception = naughty_avltree_remove_node_according_to_node_pointer(&mem_dia_struct_ptr->remained_node_avltree_header, find_avltree_node_ptr, struct naughty_memory_diagnosis_node, node);
	pthread_mutex_unlock(&mem_dia_struct_ptr->mutex);
	if (func_exception != naughty_exception_no)
	{
		goto func_end;
	}

func_end:
	return func_exception;
}

void *naughty_memory_diagnosis_memory_alloc(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, size_t size, const char *filename, int32_t line)
{
	naughty_exception func_exception = naughty_exception_no;

	void *alloced_ptr = mem_dia_struct_ptr->memory_alloc(size);

	func_exception = naughty_memory_diagnosis_record_add(mem_dia_struct_ptr, alloced_ptr, size, filename, line);

func_end:
	mem_dia_struct_ptr->exception = func_exception;
	return alloced_ptr;
}

void *naughty_memory_diagnosis_memory_calloc(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, size_t nmemb, size_t size, const char *filename, int32_t line)
{
	naughty_exception func_exception = naughty_exception_no;

	void *alloced_ptr = mem_dia_struct_ptr->memory_calloc(nmemb, size);

	func_exception = naughty_memory_diagnosis_record_add(mem_dia_struct_ptr, alloced_ptr, nmemb * size, filename, line);

func_end:
	mem_dia_struct_ptr->exception = func_exception;
	return alloced_ptr;
}

void *naughty_memory_diagnosis_memory_realloc(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, void *addr, size_t size, const char *filename, int32_t line)
{
	naughty_exception func_exception = naughty_exception_no;
	void *realloced_ptr = mem_dia_struct_ptr->memory_realloc(addr, size);
	if (realloced_ptr)
	{
		// modify
		func_exception = naughty_memory_diagnosis_record_remove(mem_dia_struct_ptr, addr);
		if (func_exception != naughty_exception_no)
		{
			goto func_end;
		}
		func_exception = naughty_memory_diagnosis_record_add(mem_dia_struct_ptr, realloced_ptr, size, filename, line);
	}

func_end:
	mem_dia_struct_ptr->exception = func_exception;
	return addr;
}

void naughty_memory_diagnosis_memory_free(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, void *mem_ptr)
{
	naughty_exception func_exception = naughty_exception_no;

	mem_dia_struct_ptr->memory_free(mem_ptr);

	func_exception = naughty_memory_diagnosis_record_remove(mem_dia_struct_ptr, mem_ptr);

func_end:
	mem_dia_struct_ptr->exception = func_exception;
}

#ifdef __cplusplus
}
#endif
