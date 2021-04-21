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
#include <sys/ioctl.h>
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
#include <sys/mount.h>
#endif
#include <tinyara/sched.h>
#include <tinyara/config.h>
#include <tinyara/mm/mm.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/mminfo.h>

#ifdef CONFIG_APP_BINARY_SEPARATION
#include <tinyara/binary_manager.h>
#endif

#include "utils_proc.h"
#ifdef CONFIG_HEAPINFO_USER_GROUP
#include <tinyara/mm/heapinfo_internal.h>
extern int heapinfo_max_group;
extern struct heapinfo_group_s heapinfo_group[HEAPINFO_USER_GROUP_NUM];
extern struct heapinfo_group_info_s group_info[HEAPINFO_THREAD_NUM];
static char *ptr = CONFIG_HEAPINFO_USER_GROUP_LIST;
const static char *end_list = CONFIG_HEAPINFO_USER_GROUP_LIST + sizeof(CONFIG_HEAPINFO_USER_GROUP_LIST) - 1;
#endif

#define HEAPINFO_BUFLEN 128
#define HEAPINFO_DISPLAY_ALL            0
#define HEAPINFO_DISPLAY_SPECIFIC_HEAP  1
#define HEAPINFO_DISPLAY_GROUP          2
#define HEAPINFO_DISPLAY_SUMMARY        3

#if CONFIG_KMM_REGIONS > 1
extern void *kregionx_start[CONFIG_KMM_REGIONS];
extern size_t kregionx_size[CONFIG_KMM_REGIONS];
extern int kregionx_heap_idx[CONFIG_KMM_REGIONS];
#endif

#if CONFIG_KMM_REGIONS > 1
static void heapinfo_print_regions(void)
{
	int region_idx;
	printf("****************************************************************\n");
	printf("RAM Region Information (Size in Bytes)\n");
	printf("****************************************************************\n");
	printf(" Region # | Start Addr |  End Addr  |  SIZE   | Heap Idx \n");
	printf("----------------------------------------------------------\n");
	for (region_idx = 0; region_idx < CONFIG_KMM_REGIONS; region_idx++) {
		printf("    %2d    | 0x%8x | 0x%8x |%8d |    %2d    \n", region_idx, kregionx_start[region_idx], kregionx_start[region_idx] + kregionx_size[region_idx], kregionx_size[region_idx], kregionx_heap_idx[region_idx]);
	}
}
#endif
static void heapinfo_print_values(char *buf, void *arg)
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
#ifdef CONFIG_APP_BINARY_SEPARATION
	printf(" | %8s", stat_info[PROC_STAT_HEAP_NAME]);
#endif
#if CONFIG_TASK_NAME_SIZE > 0
	printf(" | %s(", stat_info[PROC_STAT_NAME]);
#else
	printf("<noname>(");
#endif
	printf(")\n");
}

static int heapinfo_read_proc(FAR struct dirent *entryp, FAR void *arg)
{
	int ret;
	char *filepath;
	char buf[HEAPINFO_BUFLEN];

	asprintf(&filepath, "%s/%s/%s", PROCFS_MOUNT_POINT, entryp->d_name, "stat");
	ret = utils_readfile(filepath, buf, HEAPINFO_BUFLEN, heapinfo_print_values, NULL);
	if (ret < 0) {
		printf("Failed to read %s\n", filepath);
		free(filepath);
		return ERROR;
	}
	free(filepath);
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

	for (group_idx = 0; group_idx <= heapinfo_max_group; group_idx++) {
		printf(" %4d | %12d | %13d | ", heapinfo_group[group_idx].peak_size, heapinfo_group[group_idx].heap_size, heapinfo_group[group_idx].stack_size);
		heapinfo_print_group_threadlist();
	}
}
#endif

static void heapinfo_show_taskinfo(void)
{
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
	printf("%5s | %9s | %9s |", "STACK", "CURR_HEAP", "PEAK_HEAP");
#ifdef CONFIG_APP_BINARY_SEPARATION
	printf(" %s |", "   BIN  ");
#endif
	printf(" %s\n----|", "NAME");
#if defined(CONFIG_SCHED_HAVE_PARENT) && !defined(HAVE_GROUP_MEMBERS)
	printf("-------|");
#endif
	printf("-------|-----------|-----------|");
#ifdef CONFIG_APP_BINARY_SEPARATION
	printf("----------|");
#endif
	printf("----------\n");

	utils_proc_pid_foreach(heapinfo_read_proc, NULL);

#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	if (!is_mounted) {
		/* Detach mounted Procfs */
		(void)umount(PROCFS_MOUNT_POINT);
	}
#endif
}

int utils_heapinfo(int argc, char **args)
{
	int ret;
	int opt;
	int heapinfo_fd;
	int heapinfo_display_flag = HEAPINFO_DISPLAY_ALL;
	bool init_flag = false;
	heapinfo_option_t options;
	options.heap_type = HEAPINFO_HEAP_TYPE_KERNEL;
	options.mode = HEAPINFO_SIMPLE;
	options.pid = HEAPINFO_PID_ALL;
#ifdef CONFIG_BUILD_PROTECTED
	char *heap_name = "KERNEL";
#endif

	if (argc >= 2 && !strncmp(args[1], "--help", strlen("--help") + 1)) {
		goto usage;
	}

	while ((opt = getopt(argc, args, "ikb:ap:fgr")) != ERROR) {
		switch (opt) {
		/* i : initialize the peak allocated memory size. */
		case 'i':
			options.mode = HEAPINFO_INIT_PEAK;
			init_flag = true;
			break;
		/* k, b : select the heap type about kernel, binary. */
		case 'k':
			options.heap_type = HEAPINFO_HEAP_TYPE_KERNEL;
			break;
#ifdef CONFIG_APP_BINARY_SEPARATION
		case 'b':
			options.heap_type = HEAPINFO_HEAP_TYPE_BINARY;
			strncpy(options.app_name, optarg, BIN_NAME_MAX - 1);
			options.app_name[BIN_NAME_MAX - 1] = '\0';
			heap_name = options.app_name;
			break;
#endif
		/* a, p, f, g, e : select heapinfo display options */
		case 'a':
			options.mode = HEAPINFO_DETAIL_ALL;
			heapinfo_display_flag = HEAPINFO_DISPLAY_ALL;
			break;
		case 'p':
			if (strncmp(optarg, "0", strlen("0") + 1) == 0 || atoi(optarg)) {
				options.mode = HEAPINFO_DETAIL_PID;
				options.pid = atoi(optarg);
				heapinfo_display_flag = HEAPINFO_DISPLAY_ALL;
			} else {
				printf("Invalid PID.\n");
				goto usage;
			}
			break;
		case 'f':
			options.mode = HEAPINFO_DETAIL_FREE;
			heapinfo_display_flag = HEAPINFO_DISPLAY_ALL;
			break;
		case 'g':
			options.mode = HEAPINFO_SIMPLE;
			heapinfo_display_flag = HEAPINFO_DISPLAY_GROUP;
			break;
		case 'r':
#if CONFIG_KMM_REGIONS > 1
			heapinfo_print_regions();
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

#ifdef CONFIG_BUILD_PROTECTED
	if (init_flag != true) {
		printf("\n****************************************************************\n");
		printf("     %s HEAP INFORMATION\n", heap_name);
		printf("****************************************************************\n");
	}
#endif
	heapinfo_fd = open(MMINFO_DRVPATH, O_RDWR);
	if (heapinfo_fd < 0) {
		printf("Heapinfo Fail, %d.\n", get_errno());
		return ERROR;
	}
	ret = ioctl(heapinfo_fd, MMINFOIOC_PARSE, (int)&options);
	if (ret == ERROR) {
		printf("Heapinfo Fail, %d.\n", get_errno());
		close(heapinfo_fd);
		return ERROR;		
	}
	close(heapinfo_fd);

	if (init_flag == true) {
#ifdef CONFIG_BUILD_PROTECTED
		printf("[%s]", heap_name);
#endif
		printf("Peak allocated memory size is cleared\n");
		return OK;
	}

	heapinfo_show_taskinfo();


	if (heapinfo_display_flag == HEAPINFO_DISPLAY_GROUP) {
#ifdef CONFIG_HEAPINFO_USER_GROUP
		heapinfo_show_group();
#else
		printf("NOT supported!! Please enable CONFIG_HEAPINFO_USER_GROUP\n");
#endif
	}

	return OK;

usage:
	printf("\nUsage: heapinfo [-TARGET] [-OPTION]\n");
	printf("Display information of heap memory\n");
#ifdef CONFIG_BUILD_PROTECTED
	printf("\nTargets:\n");
	printf(" -k             Kernel Heap\n");
#ifdef CONFIG_APP_BINARY_SEPARATION
	printf(" -b BIN_NAME    Heap for BIN_NAME binary\n");
#endif
#endif
	printf("\nOptions:\n");
	printf(" -a             Show the all allocation details\n");
	printf(" -p PID         Show the specific PID allocation details \n");
	printf(" -f             Show the free list \n");
#ifdef CONFIG_HEAPINFO_USER_GROUP
	printf(" -g             Show the User defined group allocation details \n");
#endif
#if CONFIG_KMM_REGIONS > 1
	printf(" -r             Show the all region information\n");
#endif
	printf(" -i             Initialize the peak allocated size\n");
	return ERROR;
}
