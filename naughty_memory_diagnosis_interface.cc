/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_memory_diagnosis_interface.cc
 * @ Author: --global
 * @ Email: --global
 * @ Created Time: 2022-Feb-05(Saturday) 17:46:24
 * @ All Rights Reserved
 * *****************************************************************************/

#include "naughty_memory_diagnosis.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern naughty_memory_diagnosis_struct naughty_global_memory_diagnosis_struct;

#ifdef __cplusplus
}
#endif

void *operator new(size_t size)
{
	return naughty_memory_diagnosis_memory_alloc(&naughty_global_memory_diagnosis_struct, size, __FILE__, __LINE__);
}

void *operator new[](size_t size)
{
	return naughty_memory_diagnosis_memory_alloc(&naughty_global_memory_diagnosis_struct, size, __FILE__, __LINE__);
}

void operator delete(void *addr)
{
	naughty_memory_diagnosis_memory_free(&naughty_global_memory_diagnosis_struct, addr);
}

void operator delete[](void *addr)
{
	naughty_memory_diagnosis_memory_free(&naughty_global_memory_diagnosis_struct, addr);
}

void *operator new(size_t size, const char *filename, int32_t line)
{
	return naughty_memory_diagnosis_memory_alloc(&naughty_global_memory_diagnosis_struct, size, filename, line);
}

void *operator new[](size_t size, const char *filename, int32_t line)
{
	return naughty_memory_diagnosis_memory_alloc(&naughty_global_memory_diagnosis_struct, size, filename, line);
}



