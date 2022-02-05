/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: test_memory_diagnosis.cc
 * @ Author: --global
 * @ Email: --global
 * @ Created Time: 2022-Feb-06(Sunday) 00:16:52
 * @ All Rights Reserved
 * *****************************************************************************/

#include "naughty_memory_diagnosis_interface.h"

void *testfunc_cc()
{
	return new int[20];
}

int main()
{
	int ex = naughty_global_memory_diagnosis_struct_initial(8);

	testfunc_cc();
	int *intp = new int[30];

	delete[] intp;

	FILE *fd = fopen("test_output.txt", "wb");

	ex = naughty_global_memory_diagnosis_output_to_file(fd);

	fclose(fd);

	return 0;
}
