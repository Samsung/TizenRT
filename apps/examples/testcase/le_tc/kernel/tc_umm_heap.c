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

/// @file umm_heap.c
/// @brief Test Case Example for Umm Heap API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sched.h>
#include <tinyara/mm/mm.h>
#include "tc_internal.h"

#define ALLOC_SIZE_VAL 10
#define ALLOC_FREE_TIMES 5
#define TEST_TIMES 100
#define ALL_FREE 0
#define TOTAL_ALLOC_SIZE (MM_ALIGN_UP((sizeof(int)*ALLOC_SIZE_VAL) + SIZEOF_MM_ALLOCNODE))*ALLOC_FREE_TIMES
/****************************************************************************
 * Definitions
 ****************************************************************************/

/**
* @fn                   :tc_umm_heap_malloc_free
* @brief                :Allocate memory through malloc and free it.
* @scenario             :Allocate memory through malloc\n
*                        free allocated memory
* @API's covered        :malloc, free
* @passcase             :When malloc function returns non null memory and memory is null after free.
* @failcase             :When malloc function returns null memory or memory is not null after free.
* @Preconditions        :NA
*/
#ifdef CONFIG_DEBUG_MM_HEAPINFO
static void mem_deallocate_func(int *mem_arr[], int dealloc_size)
{
	int dealloc_cnt;
	for (dealloc_cnt = 0; dealloc_cnt < dealloc_size; dealloc_cnt++) {
		free(mem_arr[dealloc_cnt]);
		mem_arr[dealloc_cnt] = NULL;
	}
}

static void tc_umm_heap_malloc_free(struct tcb_s *st_tcb)
{
	int *mem_ptr[ALLOC_FREE_TIMES] = { NULL };
	int alloc_cnt;
	int alloc_tc_cnt;

	for (alloc_tc_cnt = 0; alloc_tc_cnt < TEST_TIMES; alloc_tc_cnt++) {
		for (alloc_cnt = 0; alloc_cnt < ALLOC_FREE_TIMES; alloc_cnt++) {
			mem_ptr[alloc_cnt] = (int *)malloc(ALLOC_SIZE_VAL * sizeof(int));
			if (mem_ptr[alloc_cnt] == NULL) {
				mem_deallocate_func(mem_ptr, alloc_cnt);
				printf("tc_umm_heap_malloc_free FAIL : errno %d\n", errno);
				total_fail++;
				RETURN_ERR;
			}
		}
		if (st_tcb->curr_alloc_size != TOTAL_ALLOC_SIZE) {
			printf("tc_umm_heap_malloc_free malloc FAIL : errno %d size %d definedSize %d\n", errno, st_tcb->curr_alloc_size, TOTAL_ALLOC_SIZE);
			mem_deallocate_func(mem_ptr, ALLOC_FREE_TIMES);
			total_fail++;
			RETURN_ERR;
		}
		mem_deallocate_func(mem_ptr, ALLOC_FREE_TIMES);
		if (st_tcb->curr_alloc_size != ALL_FREE) {
			printf("tc_umm_heap_malloc_free free FAIL : errno %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}
	}
	total_pass++;
	printf("tc_umm_heap_malloc_free PASS\n");
}

/**
* @fn                   :tc_umm_heap_calloc
* @brief                :Allocate memory through calloc and free it.
* @scenario             :Allocate memory through calloc\n
*                        free allocated memory
* @API's covered        :calloc, free
* @passcase             :When calloc function returns non null memory.
* @failcase             :When calloc function returns null memory.
* @Preconditions        :NA
*/
static void tc_umm_heap_calloc(struct tcb_s *st_tcb)
{
	int *mem_ptr[ALLOC_FREE_TIMES] = { NULL };
	int alloc_cnt;
	int alloc_tc_cnt;

	for (alloc_tc_cnt = 0; alloc_tc_cnt < TEST_TIMES; alloc_tc_cnt++) {
		for (alloc_cnt = 0; alloc_cnt < ALLOC_FREE_TIMES; alloc_cnt++) {
			mem_ptr[alloc_cnt] = (int *)calloc(ALLOC_SIZE_VAL, sizeof(int));
			if (mem_ptr[alloc_cnt] == NULL) {
				mem_deallocate_func(mem_ptr, alloc_cnt);
				printf("tc_umm_heap_calloc FAIL : errno %d\n", errno);
				total_fail++;
				RETURN_ERR;
			}
		}
		if (st_tcb->curr_alloc_size != TOTAL_ALLOC_SIZE) {
			printf("tc_umm_heap_calloc calloc FAIL : errno %d\n", errno);
			mem_deallocate_func(mem_ptr, ALLOC_FREE_TIMES);
			total_fail++;
			RETURN_ERR;
		}
		mem_deallocate_func(mem_ptr, ALLOC_FREE_TIMES);
		if (st_tcb->curr_alloc_size != ALL_FREE) {
			printf("tc_umm_heap_calloc free FAIL : errno %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}
	}
	total_pass++;
	printf("tc_umm_heap_calloc PASS \n");
}

/**
* @fn                   :tc_umm_heap_realloc
* @brief                :Allocate memory through malloc and free it.
* @scenario             :Allocate memory through malloc\n
*                        Reallocate memory through malloc\n
*                        free allocated memory
* @API's covered        :malloc, realloc, free
* @passcase             :When realloc function returns non null memory.
* @failcase             :When realloc function returns null memory.
* @Preconditions        :malloc
*/
static void tc_umm_heap_realloc(struct tcb_s *st_tcb)
{
	int *mem_ptr[ALLOC_FREE_TIMES] = { NULL };
	int *val = NULL;
	int alloc_cnt;
	int alloc_tc_cnt;

	for (alloc_tc_cnt = 0; alloc_tc_cnt < TEST_TIMES; alloc_tc_cnt++) {
		for (alloc_cnt = 0; alloc_cnt < ALLOC_FREE_TIMES; alloc_cnt++) {
			mem_ptr[alloc_cnt] = (int *)realloc(val, ALLOC_SIZE_VAL * sizeof(int));
			if (mem_ptr[alloc_cnt] == NULL) {
				mem_deallocate_func(mem_ptr, alloc_cnt);
				printf("tc_umm_heap_realloc FAIL : errno %d\n", errno);
				total_fail++;
				RETURN_ERR;
			}
		}
		if (st_tcb->curr_alloc_size != TOTAL_ALLOC_SIZE) {
			printf("tc_umm_heap_realloc realloc FAIL : errno %d\n", errno);
			mem_deallocate_func(mem_ptr, ALLOC_FREE_TIMES);
			total_fail++;
			RETURN_ERR;
		}
		mem_deallocate_func(mem_ptr, ALLOC_FREE_TIMES);
		if (st_tcb->curr_alloc_size != ALL_FREE) {
			printf("tc_umm_heap_realloc free FAIL : errno %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}
	}
	total_pass++;
	printf("tc_umm_heap_realloc PASS \n");
}

/**
* @fn                   :tc_umm_heap_memalign
* @brief                :Allocate memory through memalign and free it.
* @scenario             :Allocate memory through memalign\n
*                        free allocated memory
* @API's covered        :memalign, free
* @passcase             :When memalign function returns non null memory.
* @failcase             :When memalign function returns null memory.
* @Preconditions        :NA
*/
static void tc_umm_heap_memalign(struct tcb_s *st_tcb)
{
	int *mem_ptr[ALLOC_FREE_TIMES] = { NULL };
	int alloc_cnt;
	int alloc_tc_cnt;

	for (alloc_tc_cnt = 0; alloc_tc_cnt < TEST_TIMES; alloc_tc_cnt++) {
		for (alloc_cnt = 0; alloc_cnt < ALLOC_FREE_TIMES; alloc_cnt++) {
			mem_ptr[alloc_cnt] = (int *)memalign(sizeof(int), ALLOC_SIZE_VAL * sizeof(int));
			if (mem_ptr[alloc_cnt] == NULL) {
				mem_deallocate_func(mem_ptr, alloc_cnt);
				printf("tc_umm_heap_memalign FAIL : errno %d\n", errno);
				total_fail++;
				RETURN_ERR;
			}
		}
		if (st_tcb->curr_alloc_size != TOTAL_ALLOC_SIZE) {
			printf("tc_umm_heap_memalign realloc FAIL : errno %d\n", errno);
			mem_deallocate_func(mem_ptr, ALLOC_FREE_TIMES);
			total_fail++;
			RETURN_ERR;
		}
		mem_deallocate_func(mem_ptr, ALLOC_FREE_TIMES);
		if (st_tcb->curr_alloc_size != ALL_FREE) {
			printf("tc_umm_heap_memalign free FAIL : errno %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}
	}
	total_pass++;
	printf("tc_umm_heap_memalign PASS \n");
}

/**
* @fn                   :tc_umm_heap_random_malloc
* @brief                :Allocate memory through malloc with random size
* @scenario             :Allocate memory through malloc\n
*                        with random size
* @API's covered        :malloc, free
* @passcase             :When malloc function returns non null memory and memory is null after free.
* @failcase             :When malloc function returns null memory or memory is not null after free.
* @Preconditions        :NA
*/
static void tc_umm_heap_random_malloc(struct tcb_s *st_tcb)
{
	int *mem_ptr[ALLOC_FREE_TIMES] = { NULL };
	int allocated[ALLOC_FREE_TIMES] = { 0 };
	int alloc_cnt;
	int alloc_tc_cnt;
	int allocated_size = 0;

	srand(time(NULL));

	for (alloc_tc_cnt = 0; alloc_tc_cnt < TEST_TIMES; alloc_tc_cnt++) {
		allocated_size = 0;
		for (alloc_cnt = 0; alloc_cnt < ALLOC_FREE_TIMES; alloc_cnt++) {
			allocated[alloc_cnt] = rand();
			mem_ptr[alloc_cnt] = (int *)malloc(allocated[alloc_cnt]);
			if (mem_ptr[alloc_cnt] == NULL) {
				mem_deallocate_func(mem_ptr, alloc_cnt);
				printf("tc_umm_heap_random_malloc FAIL : errno %d\n", errno);
				total_fail++;
				RETURN_ERR;
			}
		}

		for (alloc_cnt = 0; alloc_cnt < ALLOC_FREE_TIMES; alloc_cnt++) {
			/* do alloc 'allocated[alloc_cnt]',
			   but allocated MM_ALIGN_UP'(allocated[alloc_cnt] + SIZEOF_MM_ALLOCNODE)',
			   because of the chunk size */
			allocated_size += MM_ALIGN_UP(allocated[alloc_cnt] + SIZEOF_MM_ALLOCNODE);
		}
		if (st_tcb->curr_alloc_size != allocated_size) {
			printf("tc_umm_heap_random_malloc FAIL : errno %d\n", errno);
			mem_deallocate_func(mem_ptr, ALLOC_FREE_TIMES);
			total_fail++;
			RETURN_ERR;
		}
		mem_deallocate_func(mem_ptr, ALLOC_FREE_TIMES);
		if (st_tcb->curr_alloc_size != ALL_FREE) {
			printf("tc_umm_heap_random_malloc free FAIL : errno %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}
	}
	total_pass++;
	printf("tc_umm_heap_random_malloc PASS\n");

}
#endif

/**
* @fn                   :tc_umm_heap_mallinfo
* @brief                :Gives current heap information for the user heap.
* @scenario             :Allocate memory through malloc\n
*                        Gives current heap information for the user heap
*                        Free allocated memory
* @API's covered        :malloc, mallinfo, free
* @passcase             :When malloc function returns non null memory and mallinfo gives non null information.
* @failcase             :When malloc function returns null memory or mallinfo gives null information.
* @Preconditions        :malloc
*/
static void tc_umm_heap_mallinfo(void)
{
	int *mem_ptr = NULL;
	int alloc_tc_cnt;
	struct mallinfo st_mallinfo;

	for (alloc_tc_cnt = 0; alloc_tc_cnt < TEST_TIMES; alloc_tc_cnt++) {
		mem_ptr = (int *)malloc(sizeof(int));
		if (mem_ptr == NULL) {
			printf("tc_umm_heap_mallinfo malloc FAIL : errno %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}
		st_mallinfo = mallinfo();
		if (st_mallinfo.fordblks <= 0) {
			printf("tc_umm_heap_mallinfo FAIL : errno %d\n", errno);
			free(mem_ptr);
			total_fail++;
			RETURN_ERR;
		}
		free(mem_ptr);
		mem_ptr = NULL;
	}
	total_pass++;
	printf("tc_umm_heap_mallinfo PASS \n");
}

static void tc_umm_heap_zalloc(void)
{
	int mem_idx;
	int *mem_ptr = NULL;
	mem_ptr = zalloc(sizeof(int) * 5);

	if (mem_ptr == NULL) {
		printf("tc_umm_heap_zalloc FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	for (mem_idx = 0; mem_idx < 5; mem_idx++) {
		if (mem_ptr[mem_idx] != 0) {
			printf("tc_umm_heap_zalloc FAIL\n");
			free(mem_ptr);
			total_fail++;
			RETURN_ERR;
		}
	}

	free(mem_ptr);
	total_pass++;
	printf("tc_umm_heap_zalloc PASS\n");
}

static int umm_task(int argc, char *argv[])
{
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	struct tcb_s *st_tcb = sched_self();
	tc_umm_heap_malloc_free(st_tcb);
	tc_umm_heap_calloc(st_tcb);
	tc_umm_heap_realloc(st_tcb);
	tc_umm_heap_memalign(st_tcb);
	tc_umm_heap_random_malloc(st_tcb);
#endif
	tc_umm_heap_mallinfo();
	tc_umm_heap_zalloc();

	task_delete(0);
	return 0;
}

/****************************************************************************
 * Name: umm_heap
 ****************************************************************************/

int umm_heap_main(void)
{
	task_create("umm_task", 150, 2048, umm_task, (char *const *)NULL);
	sleep(1);

	return 0;
}
