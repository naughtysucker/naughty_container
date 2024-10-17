/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: test_memory_diagnosis.c
 * @ Author: carl
 * @ Created Time: 2022-Jan-31(Monday) 02:36:31
 * @ All Rights Reserved
 * *****************************************************************************/

#include "naughty_memory_diagnosis_interface.h"
#include <stdio.h>

void* testfunc()
{
	int *ptr1 = (int*)malloc(1234);
	
	return ptr1;
}

int main()
{
	int ex = naughty_global_memory_diagnosis_struct_initialize(8);

	int *ptr1 = (int*)calloc(9, 120);

	// free(ptr1);

	void *pvoid = testfunc();

	pvoid = realloc(pvoid, 10000);

	FILE *fd = fopen("test_output.txt", "wb");

	ex = naughty_global_memory_diagnosis_output_to_file(fd);

	fclose(fd);
}
