/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file tc_tash_heapinfo.c

/// @brief Test Case Example for heapinfo tash cmd

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <tinyara/mm/mm.h>
#include <apps/shell/tash.h>
#include "tc_internal.h"

#define DEFAULT_STKSIZE 1024
#define DEFAULT_MALLOC_NUM 5
#define TC_HEAPINFO_TASKNAME "tc_heapinfo"
#define TC_HEAPINFO_PRIO 100
#define MAX_MALLOC_NUM 10

static char *str = "tc-heapinfo";
static int malloc_num;

static void *heapinfo_thread(void *arg)
{
	int *mem_ptr = NULL;

	mem_ptr = (int *)malloc(sizeof(int) * malloc_num);
	if (mem_ptr == NULL) {
		printf("tc_tash_heapinfo FAIL : malloc\n");
		return NULL;
	}

	/* waiting 10s for testing */
	sleep(10);

	/* after testing heapinfo cmd, all allocated will be deallocated */
	free(mem_ptr);

	return NULL;
}

static int tc_tash_heapinfo(int argc, char **args)
{
	int ret = ERROR;
	pthread_t tc_thread;
	pthread_attr_t tc_thread_attr;

	pthread_attr_init(&tc_thread_attr);

	if (argc < 2) {
		printf("No param - Test with default value(stacksize:1024, malloc-num:5)\n");
		tc_thread_attr.stacksize = DEFAULT_STKSIZE;
		malloc_num = DEFAULT_MALLOC_NUM;
	} else {
		tc_thread_attr.stacksize = atoi(args[1]);
		malloc_num = atoi(args[2]);
		printf("Test with stacksize: %d, malloc-num: %d\n", tc_thread_attr.stacksize, malloc_num);
	}

	printf("Expected Allocated Heap Size for tc_heapinfo thread : %d\n", MM_ALIGN_UP(sizeof(int) * malloc_num + SIZEOF_MM_ALLOCNODE));
	tc_thread_attr.priority = TC_HEAPINFO_PRIO;

	ret = pthread_create(&tc_thread, &tc_thread_attr, heapinfo_thread, NULL);
	TC_ASSERT_EQ_RETURN("pthread_create", ret, OK, ERROR);
	pthread_setname_np(tc_thread, TC_HEAPINFO_TASKNAME);

	pthread_detach(tc_thread);
	TC_SUCCESS_RESULT();
	return OK;
}

int tash_heapinfo_main(void)
{
	TASH_CMD_CALLBACK func = tc_tash_heapinfo;
	tash_cmd_install(str, func, TASH_EXECMD_SYNC);

	return 0;
}
