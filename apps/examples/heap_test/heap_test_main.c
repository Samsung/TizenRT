/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <semaphore.h>

/****************************************************************************
 * heap_test_main: Test application to verify Heap protection feature
 ****************************************************************************/

/* Global pointer variable common to both 'heap_test' and 'heap_test1' user tasks */

char *ptr = NULL;

int heap_test1(int argc, char *argv[])
{
	printf("%s task: Trying to Access heap allocated to task"
		" 'heap_test_main' at ptr: %p\n\n", __func__,  ptr);

	sleep(1);

	/* Access heap allocated to 'heap_test' task */

	printf("%x\n",  *ptr);

	printf("User Heap protection feature Failed!!\n"
		"'heap_test1' task successfully accessed heap"
		" allocated to 'heap_test_main' task\n");

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int heap_test_main(int argc, char *argv[])
#endif
{
	uint16_t pid;

	printf("\n************************************************\n"
	"* Test to verify User Heap protetion feature   *\n"
	"* One user task should not be allowed to       *\n"
	"* access heap allocated to other user task.    *\n"
	"* MPU shall raise exception.                   *\n"
	"************************************************\n");
	ptr = (char *)malloc(512);
	if (!ptr) {
		printf("\nError: Memory allocation failed!\n");
		return 0;
	}
	*ptr = 0x7;
	printf("\n%s task: ptr: %x\n", __func__,  *ptr);
	printf("%s task: ptr: %p\n", __func__,  ptr);

	pid = task_create("heap_test1", 100, 4096, heap_test1, NULL);
	if (pid < 0) {
		printf("%s: Failed to create 'heap_test1' task!\n", __func__);
		return -1;
	}

	sleep(5);
	free(ptr);

	return 0;
}
