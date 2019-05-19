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
/****************************************************************************
 * apps/system/utils/utils_stackmonitor.c
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <sys/types.h>
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
#include <sys/mount.h>
#endif
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sched.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>

#ifdef CONFIG_DEBUG_MM_HEAPINFO
#include <tinyara/mm/mm.h>
#endif
#include <tinyara/arch.h>
#include <tinyara/sched.h>
#include <tinyara/fs/fs.h>

#include "utils_proc.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define STKMON_PREFIX "Stack Monitor: "

#define STKMON_BUFLEN 64
/* Configuration ************************************************************/

#ifndef CONFIG_STACKMONITOR_STACKSIZE
#define CONFIG_STACKMONITOR_STACKSIZE 2048
#endif

#ifndef CONFIG_STACKMONITOR_PRIORITY
#define CONFIG_STACKMONITOR_PRIORITY 100
#endif

#ifndef CONFIG_STACKMONITOR_INTERVAL
#define CONFIG_STACKMONITOR_INTERVAL 5
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
static volatile bool stkmon_started;
struct stkmon_save_s stkmon_arr[CONFIG_MAX_TASKS * 2];
static int stkmon_chk_idx;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void stkmon_print_title(void)
{
	printf("%5s | %8s | %8s", "PID", "STATUS", "SIZE");

#ifdef CONFIG_STACK_COLORATION
	printf(" | %10s", "PEAK_STACK");
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	printf(" | %10s", "PEAK_HEAP");
#endif
	printf(" | %7s", "TIME");
#if (CONFIG_TASK_NAME_SIZE > 0)
	printf(" | THREAD NAME");
#endif
	printf("\n");
}
static void stkmon_print_inactive_list(void)
{
	int inactive_idx;
	for (inactive_idx = 0; inactive_idx < CONFIG_MAX_TASKS * 2; inactive_idx++) {
		if (stkmon_arr[inactive_idx].timestamp != 0) {
			printf("%5d | %8s | %8d", stkmon_arr[inactive_idx].chk_pid, "INACTIVE", stkmon_arr[inactive_idx].chk_stksize);
#ifdef CONFIG_STACK_COLORATION
			printf(" | %10d", stkmon_arr[inactive_idx].chk_peaksize);
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO
			printf(" | %10d", stkmon_arr[inactive_idx].chk_peakheap);
#endif
			printf(" | %7lld", (uint64_t)((clock_t)stkmon_arr[inactive_idx].timestamp));
#if (CONFIG_TASK_NAME_SIZE > 0)
			printf(" | %s", stkmon_arr[inactive_idx].chk_name);
#endif
			printf("\n");
			stkmon_arr[inactive_idx].timestamp = 0;
		}
	}
}

static void stkmon_print_active_values(char *buf)
{
	int i;
	stat_data stat_info[PROC_STAT_MAX];

	stat_info[0] = buf;

	for (i = 0; i < PROC_STAT_MAX - 1; i++) {
		stat_info[i] = strtok_r(stat_info[i], " ", &stat_info[i + 1]);
	}

	printf("%5s | %8s | %8s", stat_info[PROC_STAT_PID], "ACTIVE", stat_info[PROC_STAT_TOTALSTACK]);
#ifdef CONFIG_STACK_COLORATION
	printf(" | %10s", stat_info[PROC_STAT_PEAKSTACK]);
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	printf(" | %10s", stat_info[PROC_STAT_PEAKHEAP]);
#endif
	printf(" | %7lld", (uint64_t)((clock_t)clock()));
#if (CONFIG_TASK_NAME_SIZE > 0)
	printf(" | %s", stat_info[PROC_STAT_NAME]);
#endif
	printf("\n");
}

static int stkmon_read_proc(FAR struct dirent *entryp, FAR void *arg)
{
	int ret;
	char *filepath;
	char buf[STKMON_BUFLEN];

	asprintf(&filepath, "%s/%s/%s", PROCFS_MOUNT_POINT, entryp->d_name, "stat");
	ret = utils_readfile(filepath, buf, STKMON_BUFLEN, stkmon_print_active_values);
	free(filepath);
	if (ret < 0) {
		printf("Failed to read %s\n", filepath);
		return ERROR;
	}

	return OK;
}

static void *stackmonitor_daemon(void *arg)
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
			return NULL;
		}
	}
#endif

#ifndef CONFIG_DISABLE_SIGNALS
	printf(STKMON_PREFIX "Running\n");

	/* Loop until we detect that there is a request to stop. */
	while (stkmon_started) {
#endif
		printf("\n\n");
		stkmon_print_title();
		printf("------|----------|----------");
#ifdef CONFIG_STACK_COLORATION
		printf("|------------");
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		printf("|------------");
#endif
		printf("|---------");
#if (CONFIG_TASK_NAME_SIZE > 0)
		printf("|------------");
#endif
		printf("\n");
		stkmon_print_inactive_list();
		printf("------|----------|----------");
#ifdef CONFIG_STACK_COLORATION
		printf("|------------");
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		printf("|------------");
#endif
		printf("|---------");
#if (CONFIG_TASK_NAME_SIZE > 0)
		printf("|------------");
#endif
		printf("\n");
		utils_proc_pid_foreach(stkmon_read_proc);
#ifndef CONFIG_DISABLE_SIGNALS
		sleep(CONFIG_STACKMONITOR_INTERVAL);
	}

#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	if (!is_mounted) {
		/* Detach mounted Procfs */
		(void)umount(PROCFS_MOUNT_POINT);
	}
#endif

	/* Stopped */
	printf(STKMON_PREFIX "Stopped well\n");
#endif
	return OK;
}

static void stackmonitor_stop(void)
{
	/* Has the monitor already started? */

	if (stkmon_started) {
		/* Stop the stack monitor.  The next time the monitor wakes up,
		 * it will see the the stop indication and will exist.
		 */
		printf(STKMON_PREFIX "Stopping, not stopped yet\n");
		stkmon_started = FALSE;
	} else {
		printf(STKMON_PREFIX "There is no active StackMonitor\n");
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
void stkmon_logging(struct tcb_s *tcb)
{
#if (CONFIG_TASK_NAME_SIZE > 0)
	int name_idx;
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	struct mm_heap_s *heap;
#endif
	stkmon_arr[stkmon_chk_idx % (CONFIG_MAX_TASKS * 2)].timestamp = clock();
	stkmon_arr[stkmon_chk_idx % (CONFIG_MAX_TASKS * 2)].chk_pid = tcb->pid;
	stkmon_arr[stkmon_chk_idx % (CONFIG_MAX_TASKS * 2)].chk_stksize = tcb->adj_stack_size;
#ifdef CONFIG_STACK_COLORATION
	stkmon_arr[stkmon_chk_idx % (CONFIG_MAX_TASKS * 2)].chk_peaksize = up_check_tcbstack(tcb);
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	heap = mm_get_heap(tcb->stack_alloc_ptr);
	if (heap == NULL) {
		return;
	}
	stkmon_arr[stkmon_chk_idx % (CONFIG_MAX_TASKS * 2)].chk_peakheap = heap->alloc_list[PIDHASH(tcb->pid)].peak_alloc_size;
#endif
#if (CONFIG_TASK_NAME_SIZE > 0)
	for (name_idx = 0; name_idx < CONFIG_TASK_NAME_SIZE + 1; name_idx++) {
		stkmon_arr[stkmon_chk_idx % (CONFIG_MAX_TASKS * 2)].chk_name[name_idx] = tcb->name[name_idx];
	}
#endif
	stkmon_chk_idx %= (CONFIG_MAX_TASKS * 2);
	stkmon_chk_idx++;
}

int utils_stackmonitor(int argc, char **args)
{
#ifndef CONFIG_DISABLE_SIGNALS
	pthread_t stkmon;
	pthread_attr_t stkmon_attr;

	if (argc > 1) {
		if (!strncmp(args[1], "stop", strlen("stop") + 1)) {
			/* stop the stackmonitor */
			stackmonitor_stop();
			return OK;
		} else {
			printf("\nUsage: stkmon\n");
			printf("   or: stkmon stop\n");
			printf("Start, or Stop stack monitor daemon\n");
			return ERROR;
		}
	}

	/* Has the monitor already started? */

	if (!stkmon_started) {
		int ret;

		/* No.. start it now */

		/* Then start the stack monitoring daemon */

		stkmon_started = TRUE;

		pthread_attr_init(&stkmon_attr);
		stkmon_attr.stacksize = CONFIG_STACKMONITOR_STACKSIZE;
		stkmon_attr.priority = CONFIG_STACKMONITOR_PRIORITY;
		stkmon_attr.inheritsched = PTHREAD_EXPLICIT_SCHED;

		ret = pthread_create(&stkmon, &stkmon_attr, stackmonitor_daemon, NULL);
		if (ret != OK) {
			printf(STKMON_PREFIX "ERROR: Failed to start the stack monitor: %d\n", errno);
			stkmon_started = FALSE;
			return ERROR;
		}
		pthread_setname_np(stkmon, "StackMonitor");

		ret = pthread_detach(stkmon);
		if (ret != OK) {
			printf(STKMON_PREFIX "ERROR: Failed to detach the stack monitor: %d\n", errno);
			pthread_cancel(stkmon);
			stkmon_started = FALSE;
			return ERROR;
		}
	} else {
		printf(STKMON_PREFIX "already started\n");
	}
#else
	stackmonitor_daemon(NULL);
#endif

	return OK;
}
