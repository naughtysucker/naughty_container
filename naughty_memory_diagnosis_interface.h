/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_memory_diagnosis_interface.h
 * @ Author: carl
 * @ Created Time: 2022-Jan-31(Monday) 02:37:52
 * @ All Rights Reserved
 * *****************************************************************************/

#ifndef NAUGHTY_MEMORY_DIAGNOSIS_INTERFACE_H
#define NAUGHTY_MEMORY_DIAGNOSIS_INTERFACE_H

#include "naughty_memory_diagnosis.h"

#define malloc(size) naughty_memory_diagnosis_memory_alloc(&naughty_global_memory_diagnosis_struct, size, __FILE__, __LINE__)
#define free(addr)   naughty_memory_diagnosis_memory_free(&naughty_global_memory_diagnosis_struct, addr)
#define realloc(mem_ptr, size) naughty_memory_diagnosis_memory_realloc(&naughty_global_memory_diagnosis_struct, mem_ptr, size, __FILE__, __LINE__)
#define calloc(nmemb, size) naughty_memory_diagnosis_memory_calloc(&naughty_global_memory_diagnosis_struct, nmemb, size, __FILE__, __LINE__)

#ifdef __cplusplus

extern void *operator new(size_t size);
extern void *operator new[](size_t size);
extern void operator delete(void *addr);
extern void operator delete[](void *addr);
extern void *operator new(size_t size, const char *filename, int32_t line);
extern void *operator new[](size_t size, const char *filename, int32_t line);

#define new new(__FILE__, __LINE__)

#endif

#endif
