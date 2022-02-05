/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_memory_diagnosis.h
 * @ Author: carl
 * @ Email: naughtygeng@qq.com
 * @ Created Time: 2021-8月-30(星期一) 05:54:44
 * @ All Rights Reserved
 * *****************************************************************************/

#ifndef NAUGHTY_MEMORY_DIAGNOSIS_H
#define NAUGHTY_MEMORY_DIAGNOSIS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "naughty_avltree.h"
#include <pthread.h>
#if USE_BACKTRACE
#include <execinfo.h>
#endif
#include <stdio.h>

struct naughty_memory_diagnosis_struct
{
	void *(*memory_alloc)(size_t size);
	void (*memory_free)(void *ptr);
	void *(*memory_realloc)(void *mem_address, size_t newsize);
	void *(*memory_calloc)(size_t nmemb, size_t size);
	struct naughty_avltree_header remained_node_avltree_header;
	naughty_exception exception;
	void **backtrace_stop_address;
	size_t max_backtrace_depth;
	// lock to make safe in multi-thread process
	pthread_mutex_t mutex;
};

struct naughty_memory_diagnosis_node
{
	void *mem_ptr;
	size_t mem_size;
	char **backframe_info;
	size_t backframe_size;
	const char *file_name;
	int32_t line_number;
	struct naughty_avltree_node node;
	// struct naughty_list_header stack_frame_info_list_header;
};

extern struct naughty_memory_diagnosis_struct naughty_global_memory_diagnosis_struct;

extern void *naughty_memory_diagnosis_memory_alloc(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, size_t size, const char *filename, int32_t line);

extern void naughty_memory_diagnosis_memory_free(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, void *mem_ptr);

extern void *naughty_memory_diagnosis_memory_realloc(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, void *addr, size_t size, const char *filename, int32_t line);

extern void *naughty_memory_diagnosis_memory_calloc(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, size_t nmemb, size_t size, const char *filename, int32_t line);

extern int32_t naughty_memory_diagnosis_avltree_node_compare(struct naughty_avltree_node *node1, struct naughty_avltree_node *node2);

extern naughty_exception naughty_memory_diagnosis_struct_initial(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, size_t backtrace_depth);

extern naughty_exception naughty_memory_diagnosis_output_to_file(struct naughty_memory_diagnosis_struct *mem_dia_struct_ptr, FILE *opened_fp);

extern naughty_exception naughty_global_memory_diagnosis_struct_initial(size_t size);

extern naughty_exception naughty_global_memory_diagnosis_output_to_file(FILE *opened_fp);

#ifdef __cplusplus
}
#endif

#endif
