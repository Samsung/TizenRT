/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <tinyara/sched.h>
#include <tinyara/config.h>
#include <tinyara/mm/mm.h>
#ifdef CONFIG_HEAPINFO_USER_GROUP
#include <stdbool.h>
#include <tinyara/mm/heapinfo_internal.h>
extern int max_group;
extern struct heapinfo_group_s heapinfo_group[HEAPINFO_USER_GROUP_NUM];
extern struct heapinfo_group_info_s group_info[HEAPINFO_THREAD_NUM];
static char *ptr = CONFIG_HEAPINFO_USER_GROUP_LIST;
const static char *end_list = CONFIG_HEAPINFO_USER_GROUP_LIST + sizeof(CONFIG_HEAPINFO_USER_GROUP_LIST) - 1;
#endif

#define HEAPINFO_DISPLAY_ALL            0
#define HEAPINFO_DISPLAY_SPECIFIC_HEAP  1
#define HEAPINFO_DISPLAY_GROUP          2
#define HEAPINFO_DISPLAY_SUMMARY        3
#if CONFIG_MM_REGIONS > 1
#define HEAPINFO_DISPLAY_REGION         4
extern void *regionx_start[CONFIG_MM_REGIONS];
extern size_t regionx_size[CONFIG_MM_REGIONS];
extern int regionx_heap_idx[CONFIG_MM_REGIONS];
#endif

#if CONFIG_MM_NHEAPS > 1
extern heapinfo_total_info_t total_info;
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
static void kdbg_heapinfo_init(FAR struct tcb_s *tcb, FAR void *arg)
{

	tcb->peak_alloc_size = 0;

	printf("PID %d, peak allocated heap information is cleared\n", tcb->pid);
}

static void kdbg_heapinfo_task(FAR struct tcb_s *tcb, FAR void *arg)
{
	struct mm_allocnode_s *node;
	node = (struct mm_allocnode_s *)(tcb->stack_alloc_ptr - SIZEOF_MM_ALLOCNODE);

	printf("%3d", tcb->pid);
#if defined(CONFIG_SCHED_HAVE_PARENT) && !defined(HAVE_GROUP_MEMBERS)
	printf(" | %5d", tcb->ppid);
#endif
	if (tcb->pid == 0) {
		printf(" | %5d", CONFIG_IDLETHREAD_STACKSIZE);
	} else {
		printf(" | %5d", node->size);
	}
	printf(" | %9d | %9d", tcb->curr_alloc_size, tcb->peak_alloc_size);

	/* Show task name and arguments */
#if CONFIG_TASK_NAME_SIZE > 0
	printf(" | %s(", tcb->name);
#else
	printf("<noname>(");
#endif
	printf(")\n");
}
#endif

#ifdef CONFIG_HEAPINFO_USER_GROUP
static void kdbg_heapinfo_group_threadlist(void)
{
	while (*ptr != '/') {
		printf("%c", *ptr++);
		if (ptr == end_list) {
			ptr = CONFIG_HEAPINFO_USER_GROUP_LIST;
			printf("\n");
			return;
		}
	}
	printf("\n");
	ptr++;
}
#endif

#if CONFIG_MM_NHEAPS > 1
static void kdbg_init_total_info(void)
{
	total_info.total_heap_size = 0;
	total_info.cur_free = 0;
	total_info.largest_free_size = 0;
	total_info.cur_dead_thread = 0;
	total_info.sum_of_stacks = 0;
	total_info.sum_of_heaps = 0;
}
#endif

int kdbg_heapinfo(int argc, char **args)
{
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	int option;
	int display_flag = HEAPINFO_DISPLAY_ALL;
	int mode = HEAPINFO_SIMPLE;
	int pid = HEAPINFO_PID_ALL;
#if CONFIG_MM_REGIONS > 1
	int region_idx;
#endif
#if CONFIG_MM_NHEAPS > 1
	bool summary_option = true;
	int heap_idx = -1;
#endif

	if (argc >= 2 && !strncmp(args[1], "--help", strlen("--help") + 1)) {
		goto usage;
	}

#if CONFIG_MM_NHEAPS > 1
	kdbg_init_total_info();
#endif
	struct mm_heap_s *user_heap = g_mmheap;
	while ((option = getopt(argc, args, "iap:fge:r")) != ERROR) {
#if CONFIG_MM_NHEAPS > 1
		summary_option = false;
#endif
		switch (option) {
		case 'i':
			sched_foreach(kdbg_heapinfo_init, NULL);
			printf("Peak allocated memory size is cleared\n");
			return OK;
		case 'a':
			mode = HEAPINFO_DETAIL_ALL;
			pid = HEAPINFO_PID_ALL;
			display_flag = HEAPINFO_DISPLAY_ALL;
#if CONFIG_MM_NHEAPS > 1
			summary_option = true;
#endif
			break;
		case 'p':
			mode = HEAPINFO_DETAIL_PID;
			pid = atoi(optarg);
			display_flag = HEAPINFO_DISPLAY_ALL;
			break;
		case 'f':
			mode = HEAPINFO_DETAIL_FREE;
			pid = HEAPINFO_PID_ALL;
			display_flag = HEAPINFO_DISPLAY_ALL;
			break;
		case 'g':
			display_flag = HEAPINFO_DISPLAY_GROUP;
			break;
		case 'e':
#if CONFIG_MM_NHEAPS > 1
			mode = HEAPINFO_DETAIL_SPECIFIC_HEAP;
			heap_idx = atoi(optarg);
			display_flag = HEAPINFO_DISPLAY_SPECIFIC_HEAP;
#else
			goto usage;
#endif
			break;
		case 'r':
#if CONFIG_MM_REGIONS > 1
			display_flag = HEAPINFO_DISPLAY_REGION;
#else
			goto usage;
#endif
			break;
		case '?':
		default:
			printf("Invalid option\n");
			goto usage;
		}
	}
#if CONFIG_MM_REGIONS > 1
	if (display_flag == HEAPINFO_DISPLAY_REGION) {
		printf("****************************************************************\n");
		printf("RAM Region Information (Size in Bytes)\n");
		printf("****************************************************************\n");
		printf(" Region # | Start Addr |  End Addr  |  SIZE   | Heap Idx \n");
		printf("----------------------------------------------------------\n");
		for (region_idx = 0; region_idx < CONFIG_MM_REGIONS; region_idx++) {
			printf("    %2d    | 0x%8x | 0x%8x |%8d |    %2d    \n", region_idx, regionx_start[region_idx], regionx_start[region_idx] + regionx_size[region_idx], regionx_size[region_idx], regionx_heap_idx[region_idx]);
		}
		return OK;
	}
#endif

#if CONFIG_MM_NHEAPS > 1
	if (display_flag == HEAPINFO_DISPLAY_ALL) {
		for (heap_idx = 0; heap_idx < CONFIG_MM_NHEAPS; heap_idx++) {
			if (summary_option == false) {
				printf("\n\n****************************************************************\n");
				printf("%c[1;33m", 27);
				printf("HEAP[%d] has %d Region(s).\n", heap_idx, user_heap[heap_idx].mm_nregions);
				printf("%c[0m", 27);
			}
			heapinfo_parse(&user_heap[heap_idx], mode, pid);
		}
	} else if (display_flag == HEAPINFO_DISPLAY_SPECIFIC_HEAP) {
		if (heap_idx < 0 || heap_idx >= CONFIG_MM_NHEAPS) {
			goto usage;
		}
		printf("****************************************************************\n");
		printf("HEAP[%d] Allocation Info- (Size in Bytes)\n", heap_idx);
		heapinfo_parse(&user_heap[heap_idx], mode, HEAPINFO_PID_ALL);
	} else
#endif
	heapinfo_parse(user_heap, mode, pid);

#if CONFIG_MM_NHEAPS > 1
	if (summary_option == true) {
		printf("\n****************************************************************\n");
		printf("%c[1;33m", 27);
		printf("     Summary of Total Heap Usages (Size in Bytes)\n");
		printf("%c[0m", 27);
		printf("****************************************************************\n");
		/* Print the total heap details. */

		printf("Total                           : %u (100%%)\n", total_info.total_heap_size);
		printf("  - Allocated (Current / Peak)  : %u (%d%%) / %u (%d%%)\n",\
			total_info.cur_alloc_size, (size_t)((uint64_t)(total_info.cur_alloc_size) * 100 / total_info.total_heap_size),\
			total_info.peak_alloc_size,  (size_t)((uint64_t)(total_info.peak_alloc_size) * 100 / total_info.total_heap_size));
		printf("  - Free (Current)              : %u (%d%%)\n", total_info.cur_free, (size_t)((uint64_t)total_info.cur_free * 100 / total_info.total_heap_size));
		printf("  - Reserved                    : %u\n", CONFIG_MM_NHEAPS * SIZEOF_MM_ALLOCNODE * 2);

		printf("\n****************************************************************\n");
		printf("     Details of Total Heap Usages (Size in Bytes)\n");
		printf("****************************************************************\n");
		printf("< Free >\n");
		printf("  - Largest Free Node Size            : %u\n", total_info.largest_free_size);
		printf("\n< Allocation >\n");
		printf("  - Current Size (Alive Allocation) = (1) + (2) + (3)\n");
		printf("     . by Dead Threads (*) (1)        : %u\n", total_info.cur_dead_thread);
		printf("     . by Alive Threads\n");
		printf("        - Sum of \"STACK\"(**) (2)      : %u\n", total_info.sum_of_stacks);
		printf("        - Sum of \"CURR_HEAP\" (3)      : %u\n", total_info.sum_of_heaps);

		printf("** NOTE **\n");
		printf("(*)  Alive allocation by dead threads might be used by others or might be a leakage.\n");
		printf("(**) Only Idle task has a separate stack region,\n");
		printf("  rest are all allocated on the heap region.\n");

		printf("\n** For details, \"heapinfo -a\" or \"heapinfo -e [HEAP_IDX]\"\n");
	}
#endif
	printf("\n< by Alive Threads >\n");
	printf("%3s | ", "PID");
#if defined(CONFIG_SCHED_HAVE_PARENT) && !defined(HAVE_GROUP_MEMBERS)
	printf("%5s | ", "PPID");
#endif
	printf("%5s | %9s | %9s | %s\n", "STACK", "CURR_HEAP", "PEAK_HEAP", "NAME");
	printf("----|");
#if defined(CONFIG_SCHED_HAVE_PARENT) && !defined(HAVE_GROUP_MEMBERS)
	printf("-------|");
#endif
	printf("-------|-----------|-----------|----------\n");
	sched_foreach(kdbg_heapinfo_task, NULL);

#ifdef CONFIG_HEAPINFO_USER_GROUP
	if (display_flag == HEAPINFO_DISPLAY_GROUP) {
		int group_idx;
		printf("****************************************************************\n");
		printf("Heap Allocation Information per User defined Group\n");
		printf("****************************************************************\n");
		printf(" PEAK | HEAP_ON_PEAK | STACK_ON_PEAK | THREADS_IN_GROUP \n");
		printf("----------------------------------------------------------------\n");

		for (group_idx = 0; group_idx <= max_group; group_idx++) {
			printf(" %4d | %12d | %13d | ", heapinfo_group[group_idx].peak_size, heapinfo_group[group_idx].heap_size, heapinfo_group[group_idx].stack_size);
			kdbg_heapinfo_group_threadlist();
		}
	}
#else
	if (display_flag == HEAPINFO_DISPLAY_GROUP) {
		printf("NOT supported!! Please enable CONFIG_HEAPINFO_USER_GROUP\n");
	}
#endif
	return OK;

usage:
	printf("\nUsage: heapinfo [OPTIONS]\n");
	printf("Display information of heap memory\n");
	printf("\nOptions:\n");
	printf(" -i           Initialize the heapinfo\n");
	printf(" -a           Show the all allocation details\n");
	printf(" -p PID       Show the specific PID allocation details \n");
	printf(" -f           Show the free list \n");
#ifdef CONFIG_HEAPINFO_USER_GROUP
	printf(" -g           Show the User defined group allocation details \n");
#endif
#if CONFIG_MM_NHEAPS > 1
	printf(" -e HEAP_IDX  Show the heap[HEAP_IDX] allocation details(HEAP_IDX range : 0 ~ %d)\n", CONFIG_MM_NHEAPS - 1);
#endif
#if CONFIG_MM_REGIONS > 1
	printf(" -r           Show the all region information\n");
#endif
#endif
	return ERROR;
}
