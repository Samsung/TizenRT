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

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

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
#ifdef CONFIG_HEAPINFO_USER_GROUP
#define HEAPINFO_ALLOC 0
#define HEAPINFO_FREE 1
#define HEAPINFO_STACK_ALLOC 2
#define HEAPINFO_STACK_FREE 3
/* group_x_info[0] means current heap alloc size of group_x
 * group_x_info[1] means peak heap size of group_x
 * group_x_info[2] means total current alloc size of grou_x
 * group_x_info[3] means total current alloc size of grou_x
 */
static int group_a_info[4];
static int group_b_info[4];
static int group_c_info[4];
#endif

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
#ifdef CONFIG_HEAPINFO_USER_GROUP
static void calc_group_size(int *mem, int info[4], int type)
{
	struct mm_allocnode_s *node;
	node = (struct mm_allocnode_s *)((char *)mem - SIZEOF_MM_ALLOCNODE);
	if (type == HEAPINFO_ALLOC) {
		info[0] += node->size;
		if (info[0] > info[1]) {
			info[1] = info[0];
		}
		info[2] += node->size;
		if (info[2] > info[3]) {
			info[3] = info[2];
		}
	} else {
		info[0] -= node->size;
		info[2] -= node->size;
	}
}
static void *heapinfo_malloc(size_t size, int info[4])
{
	int *mem;
	mem = (int *)malloc(size);
	calc_group_size(mem, info, HEAPINFO_ALLOC);
	return mem;
}
static void *heapinfo_free(void *mem, int info[4])
{
	calc_group_size(mem, info, HEAPINFO_FREE);
	free(mem);
	return NULL;
}
static void heapinfo_calc_stack(pid_t pid, int info[4], int type)
{
	struct tcb_s *tcb;
	tcb = sched_gettcb(pid);

	if (type == HEAPINFO_STACK_ALLOC) {
		info[2] += tcb->adj_stack_size;
		if (info[2] > info[3]) {
			info[3] = info[2];
		}
	} else {
		info[2] -= tcb->adj_stack_size;
	}
}
static int heapinfo_task(int argc, char *argv[])
{
	int *mem_ptr;
	heapinfo_calc_stack(getpid(), group_a_info, HEAPINFO_STACK_ALLOC);
	
	mem_ptr = (int *)heapinfo_malloc(1000, group_a_info);
	heapinfo_free(mem_ptr, group_a_info);

	mem_ptr = (int *)heapinfo_malloc(500, group_a_info);
	heapinfo_free(mem_ptr, group_a_info);

	heapinfo_calc_stack(getpid(), group_a_info, HEAPINFO_STACK_FREE);
	return OK;
}

static int heapinfo_task2(int argc, char *argv[])
{
	int *mem_ptr;
	heapinfo_calc_stack(getpid(), group_a_info, HEAPINFO_STACK_ALLOC);

	mem_ptr = (int *)heapinfo_malloc(500, group_a_info);
	heapinfo_free(mem_ptr, group_a_info);
	mem_ptr = (int *)heapinfo_malloc(2000, group_a_info);
	heapinfo_free(mem_ptr, group_a_info);

	heapinfo_calc_stack(getpid(), group_a_info, HEAPINFO_STACK_FREE);
	return OK;
}

static void *heapinfo_pthread(void *param)
{
	int *mem_ptr;
	heapinfo_calc_stack(getpid(), group_c_info, HEAPINFO_STACK_ALLOC);

	mem_ptr = (int *)heapinfo_malloc(300, group_c_info);
	heapinfo_free(mem_ptr, group_c_info);
	mem_ptr = (int *)heapinfo_malloc(500, group_c_info);
	heapinfo_free(mem_ptr, group_c_info);

	heapinfo_calc_stack(getpid(), group_c_info, HEAPINFO_STACK_FREE);
	return OK;
}

static int heapinfo_task3(int argc, char *argv[])
{
	int *mem_ptr;
	heapinfo_calc_stack(getpid(), group_b_info, HEAPINFO_STACK_ALLOC);

	mem_ptr = (int *)heapinfo_malloc(1000, group_b_info);
	heapinfo_free(mem_ptr, group_b_info);
	mem_ptr = (int *)heapinfo_malloc(300, group_b_info);
	heapinfo_free(mem_ptr, group_b_info);

	heapinfo_calc_stack(getpid(), group_b_info, HEAPINFO_STACK_FREE);
	return OK;
}
#endif

static int tc_tash_heapinfo(int argc, char **args)
{
	int ret = ERROR;
	pthread_t tc_thread;
	pthread_attr_t tc_thread_attr;
#ifdef CONFIG_HEAPINFO_USER_GROUP
	pthread_t pid;
#endif

	pthread_attr_init(&tc_thread_attr);

	if (argc < 2) {
		printf("No param - Test with default value(stacksize:1024, malloc-num:5)\n");
		tc_thread_attr.stacksize = DEFAULT_STKSIZE;
		malloc_num = DEFAULT_MALLOC_NUM;
	}
#ifdef CONFIG_HEAPINFO_USER_GROUP
	else if (argc == 2 && strcmp(args[1], "-g") == 0) {
		/* Test heapinfo group functionality,
		 * Group A : heapinfo_task, heapinfo_task2
		 * Group B : heapinfo_task3
		 * Group C : heapinfo_pthread */
		task_create("heapinfo_task", TC_HEAPINFO_PRIO, 1024, heapinfo_task, (char * const *)NULL);
		task_create("heapinfo_task2", TC_HEAPINFO_PRIO, 1024, heapinfo_task2, (char * const *)NULL);
		task_create("heapinfo_task3", TC_HEAPINFO_PRIO, 1024, heapinfo_task3, (char * const *)NULL);
		pthread_create(&pid, NULL, heapinfo_pthread, (void *)NULL);
		pthread_setname_np(pid, "heapinfo_pthread");

		sleep(5);
		printf("MEASURED PEAK ALLOC SIZE AND STACK SIZE PER GROUP\n");
		printf("-------------------------------------------------\n");
		printf("PEAK | HEAP_ON_PEAK | STACK_ON_PEAK\n");
		printf("------------------------------------\n");
		printf("%3d | %12d | %13d\n", group_a_info[3], group_a_info[1], group_a_info[3] - group_a_info[1]);
		printf("%3d | %12d | %13d\n", group_b_info[3], group_b_info[1], group_b_info[3] - group_b_info[1]);
		printf("%3d | %12d | %13d\n", group_c_info[3], group_c_info[1], group_c_info[3] - group_c_info[1]);
		TC_SUCCESS_RESULT();
		return OK;
	}
#endif
	else {
		/* Test heapinfo normal functionality */
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
