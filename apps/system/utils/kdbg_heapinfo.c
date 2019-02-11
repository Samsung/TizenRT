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
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
#include <sys/mount.h>
#endif
#include <tinyara/sched.h>
#include <tinyara/config.h>
#include <tinyara/mm/mm.h>
#include <tinyara/fs/fs.h>
#ifdef CONFIG_MM_KERNEL_HEAP
#include <tinyara/fs/ioctl.h>
#endif
#include <tinyara/heapinfo_drv.h>
#include "kdbg_utils.h"
#ifdef CONFIG_HEAPINFO_USER_GROUP
#include <tinyara/mm/heapinfo_internal.h>
extern int max_group;
extern struct heapinfo_group_s heapinfo_group[HEAPINFO_USER_GROUP_NUM];
extern struct heapinfo_group_info_s group_info[HEAPINFO_THREAD_NUM];
static char *ptr = CONFIG_HEAPINFO_USER_GROUP_LIST;
const static char *end_list = CONFIG_HEAPINFO_USER_GROUP_LIST + sizeof(CONFIG_HEAPINFO_USER_GROUP_LIST) - 1;
#endif

#define HEAPINFO_BUFLEN 64
#define HEAPINFO_DISPLAY_ALL            0
#define HEAPINFO_DISPLAY_SPECIFIC_HEAP  1
#define HEAPINFO_DISPLAY_GROUP          2
#define HEAPINFO_DISPLAY_SUMMARY        3
#if CONFIG_MM_REGIONS > 1
extern void *regionx_start[CONFIG_MM_REGIONS];
extern size_t regionx_size[CONFIG_MM_REGIONS];
extern int regionx_heap_idx[CONFIG_MM_REGIONS];
#endif

#ifdef CONFIG_MM_KERNEL_HEAP
static int heapinfo_fd;
#endif
static int heapinfo_display_flag = HEAPINFO_DISPLAY_ALL;
static int heapinfo_mode = HEAPINFO_SIMPLE;
static int heapinfo_pid = HEAPINFO_PID_ALL;
#if CONFIG_MM_NHEAPS > 1
extern heapinfo_total_info_t total_info;
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
#if CONFIG_MM_REGIONS > 1
static void heapinfo_print_regions(void)
{
	int region_idx;
	printf("****************************************************************\n");
	printf("RAM Region Information (Size in Bytes)\n");
	printf("****************************************************************\n");
	printf(" Region # | Start Addr |  End Addr  |  SIZE   | Heap Idx \n");
	printf("----------------------------------------------------------\n");
	for (region_idx = 0; region_idx < CONFIG_MM_REGIONS; region_idx++) {
		printf("    %2d    | 0x%8x | 0x%8x |%8d |    %2d    \n", region_idx, regionx_start[region_idx], regionx_start[region_idx] + regionx_size[region_idx], regionx_size[region_idx], regionx_heap_idx[region_idx]);
	}
}
static void heapinfo_print_nheaps(void)
{
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
static void heapinfo_print_values(char *buf)
{
	int i;
	stat_data stat_info[PROC_STAT_MAX];

	stat_info[0] = buf;

	for (i = 0; i < PROC_STAT_MAX - 1; i++) {
		stat_info[i] = strtok_r(stat_info[i], " ", &stat_info[i + 1]);
	}
	printf("%3s", stat_info[PROC_STAT_PID]);
#if defined(CONFIG_SCHED_HAVE_PARENT) && !defined(HAVE_GROUP_MEMBERS)
	printf(" | %5s", stat_info[PROC_STAT_PPID]);
#endif
	printf(" | %5s | %9s | %9s", stat_info[PROC_STAT_TOTALSTACK], stat_info[PROC_STAT_CURRHEAP], stat_info[PROC_STAT_PEAKHEAP]);
#if CONFIG_TASK_NAME_SIZE > 0
	printf(" | %s(", stat_info[PROC_STAT_NAME]);
#else
	printf("<noname>(");
#endif
	printf(")\n");
}

static int heapinfo_read_proc(pid_t pid)
{
	int ret;
	char *filepath;
	char buf[HEAPINFO_BUFLEN];

	asprintf(&filepath, "%s/%d/%s", PROCFS_MOUNT_POINT, pid, "stat");
	ret = kdbg_readfile(filepath, buf, HEAPINFO_BUFLEN, heapinfo_print_values);
	free(filepath);
	if (ret < 0) {
		printf("Failed to read %s\n", filepath);
		return ERROR;
	}

	return OK;
}

#ifdef CONFIG_HEAPINFO_USER_GROUP
static void heapinfo_print_group_threadlist(void)
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

static void heapinfo_show_group(void)
{
	int group_idx;
	printf("****************************************************************\n");
	printf("Heap Allocation Information per User defined Group\n");
	printf("****************************************************************\n");
	printf(" PEAK | HEAP_ON_PEAK | STACK_ON_PEAK | THREADS_IN_GROUP \n");
	printf("----------------------------------------------------------------\n");

	for (group_idx = 0; group_idx <= max_group; group_idx++) {
		printf(" %4d | %12d | %13d | ", heapinfo_group[group_idx].peak_size, heapinfo_group[group_idx].heap_size, heapinfo_group[group_idx].stack_size);
		heapinfo_print_group_threadlist();
	}
}
#endif

static void heapinfo_show_taskinfo(struct mm_heap_s *heap)
{
	int tcb_idx;
	int heap_idx;
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	int ret;
	bool is_mounted;

	is_mounted = false;

	/* Mount Procfs to use */
	ret = mount(NULL, PROCFS_MOUNT_POINT, PROCFS_FSTYPE, 0, NULL);
	if (ret == ERROR) {
		if (errno == EEXIST) {
			is_mounted = true;
		} else {
			printf("Failed to mount procfs : %d\n", errno);
			return;
		}
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

	for (heap_idx = 0; heap_idx < CONFIG_MM_NHEAPS; heap_idx++) {
		for (tcb_idx = 0; tcb_idx < CONFIG_MAX_TASKS; tcb_idx++) {
			if (heap[heap_idx].alloc_list[tcb_idx].pid != HEAPINFO_INIT_INFO) {
				heapinfo_read_proc(heap[heap_idx].alloc_list[tcb_idx].pid);
			}
		}
	}

#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	if (!is_mounted) {
		/* Detach mounted Procfs */
		(void)umount(PROCFS_MOUNT_POINT);
	}
#endif
}
#endif

#if CONFIG_MM_NHEAPS > 1
static void heapinfo_init_totalinfo(void)
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
#ifdef CONFIG_MM_KERNEL_HEAP
	bool showing_kheap = false;
	heapinfo_option_t kernel_option;
#endif
#if CONFIG_MM_NHEAPS > 1
	bool summary_option = true;
	int heap_idx = -1;
	int region_num;
#endif
	if (argc >= 2 && !strncmp(args[1], "--help", strlen("--help") + 1)) {
		goto usage;
	}

	heapinfo_display_flag = HEAPINFO_DISPLAY_ALL;
	heapinfo_mode = HEAPINFO_SIMPLE;
	heapinfo_pid = HEAPINFO_PID_ALL;

#if CONFIG_MM_NHEAPS > 1
	heapinfo_init_totalinfo();
#endif
	struct mm_heap_s *heap = g_mmheap;
	while ((option = getopt(argc, args, "iap:fge:rk")) != ERROR) {
#if CONFIG_MM_NHEAPS > 1
		summary_option = false;
#endif
		switch (option) {
		case 'i':
			heapinfo_peak_init(heap);
			printf("Peak allocated memory size is cleared\n");
			return OK;
		case 'a':
			heapinfo_mode = HEAPINFO_DETAIL_ALL;
			heapinfo_display_flag = HEAPINFO_DISPLAY_ALL;
#if CONFIG_MM_NHEAPS > 1
			summary_option = true;
#endif
			break;
		case 'p':
			if (strncmp(optarg, "0", strlen("0") + 1) == 0 || atoi(optarg)) {
				heapinfo_mode = HEAPINFO_DETAIL_PID;
				heapinfo_pid = atoi(optarg);
				heapinfo_display_flag = HEAPINFO_DISPLAY_ALL;
			} else {
				printf("Invalid PID.\n");
				goto usage;
			}
			break;
		case 'f':
			heapinfo_mode = HEAPINFO_DETAIL_FREE;
			heapinfo_display_flag = HEAPINFO_DISPLAY_ALL;
			break;
		case 'g':
			heapinfo_mode = HEAPINFO_SIMPLE;
			heapinfo_display_flag = HEAPINFO_DISPLAY_GROUP;
			break;
		case 'e':
#if CONFIG_MM_NHEAPS > 1
			heapinfo_mode = HEAPINFO_DETAIL_SPECIFIC_HEAP;
			heapinfo_display_flag = HEAPINFO_DISPLAY_SPECIFIC_HEAP;
			heap_idx = atoi(optarg);
#else
			goto usage;
#endif
			break;
		case 'r':
#if CONFIG_MM_REGIONS > 1
			heapinfo_print_regions();
#else
			goto usage;
#endif
			break;
		case 'k':
#ifdef CONFIG_MM_KERNEL_HEAP
			heapinfo_fd = open(HEAPINFO_DRVPATH, O_RDWR);
			if (heapinfo_fd < 0) {
				printf("Fail to open heapinfo driver %d.\n", errno);
				return ERROR;
			}
			showing_kheap = true;
#else
			printf("Kernel Heap is not supported.\n");
			goto usage;
#endif
			break;
		case '?':
		default:
			printf("Invalid option\n");
			goto usage;
		}
	}

#ifdef CONFIG_MM_KERNEL_HEAP
	if (showing_kheap == true) {
		kernel_option.mode = heapinfo_mode; 
		kernel_option.pid = heapinfo_pid;
	}
#endif

#if CONFIG_MM_NHEAPS > 1
	if (heapinfo_display_flag == HEAPINFO_DISPLAY_ALL) {
		for (heap_idx = 0; heap_idx < CONFIG_MM_NHEAPS; heap_idx++) {
			if (summary_option == false) {
				printf("\n\n****************************************************************\n");
				printf("%c[1;33m", 27);
#ifdef CONFIG_MM_KERNEL_HEAP
				if (showing_kheap == true) {
					region_num = ioctl(heapinfo_fd, HEAPINFOIOC_NREGION, heap_idx);
				} else
#endif
				{
					region_num = heap[heap_idx].mm_nregions;
				}
				printf("HEAP[%d] has %d Region(s).\n", heap_idx, region_num);
				printf("%c[0m", 27);
			}
#ifdef CONFIG_MM_KERNEL_HEAP
			if (showing_kheap == true) {
				(void)ioctl(heapinfo_fd, HEAPINFOIOC_PARSE, (int)&kernel_option);
			} else
#endif
			{
				heapinfo_parse(&heap[heap_idx], heapinfo_mode, heapinfo_pid);
			}
		}
	} else if (heapinfo_display_flag == HEAPINFO_DISPLAY_SPECIFIC_HEAP) {
		if (heap_idx < 0 || heap_idx >= CONFIG_MM_NHEAPS) {
			goto usage;
		}
		printf("****************************************************************\n");
		printf("HEAP[%d] Allocation Info- (Size in Bytes)\n", heap_idx);
#ifdef CONFIG_MM_KERNEL_HEAP
		if (showing_kheap == true) {
			(void)ioctl(heapinfo_fd, HEAPINFOIOC_PARSE, (int)&kernel_option);
		} else
#endif
		{
			heapinfo_parse(&heap[heap_idx], heapinfo_mode, HEAPINFO_PID_ALL);
		}
	} else
#endif
#ifdef CONFIG_MM_KERNEL_HEAP
	if (showing_kheap == true) {
		(void)ioctl(heapinfo_fd, HEAPINFOIOC_PARSE, (int)&kernel_option);
	} else
#endif
	{
		heapinfo_parse(heap, heapinfo_mode, heapinfo_pid);
	}

#if CONFIG_MM_NHEAPS > 1
	if (summary_option == true) {
		heapinfo_print_nheaps();
	}
#endif
	heapinfo_show_taskinfo(heap);


	if (heapinfo_display_flag == HEAPINFO_DISPLAY_GROUP) {
#ifdef CONFIG_HEAPINFO_USER_GROUP
		heapinfo_show_group();
#else
		printf("NOT supported!! Please enable CONFIG_HEAPINFO_USER_GROUP\n");
#endif
	}

#ifdef CONFIG_MM_KERNEL_HEAP
	if (showing_kheap == true) {
		close(heapinfo_fd);
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
#ifdef CONFIG_MM_KERNEL_HEAP
	printf(" -k OPTION    Show the kernel heap memory allocation details based on above options.\n");
	if (showing_kheap == true) {
		close(heapinfo_fd);
	}
#endif
#endif
	return ERROR;
}
