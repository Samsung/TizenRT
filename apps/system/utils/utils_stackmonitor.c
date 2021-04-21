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

#include <sys/prctl.h>

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

#define STKMON_BUFLEN 128
/* Configuration ************************************************************/

#ifndef CONFIG_STACKMONITOR_STACKSIZE
#define CONFIG_STACKMONITOR_STACKSIZE 4096
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
	int ret;
	struct stkmon_save_s *terminated_infos;

	terminated_infos = (struct stkmon_save_s *)zalloc(sizeof(struct stkmon_save_s) * STKMON_MAX_LOGS);
	if (terminated_infos == NULL) {
		printf("Failed to allocate the information array.\n");
		return;
	}

	/* Copy the terminated task/pthread information from kernel. */
	ret = prctl((int)PR_GET_STKLOG, terminated_infos);
	if (ret != OK) {
		free(terminated_infos);
		printf("Failed to read terminated threads information.\n");
		return;
	}

	for (inactive_idx = 0; inactive_idx < STKMON_MAX_LOGS; inactive_idx++) {
		if (terminated_infos[inactive_idx].timestamp != 0) {
			printf("%5d | %8s | %8d", terminated_infos[inactive_idx].chk_pid, "INACTIVE", terminated_infos[inactive_idx].chk_stksize);
#ifdef CONFIG_STACK_COLORATION
			printf(" | %10d", terminated_infos[inactive_idx].chk_peaksize);
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO
			printf(" | %10d", terminated_infos[inactive_idx].chk_peakheap);
#endif
#ifdef CONFIG_SYSTEM_TIME64
			printf(" | %7llu", (uint64_t)terminated_infos[inactive_idx].timestamp);
#else
			printf(" | %7lu", (uint32_t)terminated_infos[inactive_idx].timestamp);
#endif
#if (CONFIG_TASK_NAME_SIZE > 0)
			printf(" | %s", terminated_infos[inactive_idx].chk_name);
#endif
			printf("\n");
		}
	}

	free(terminated_infos);
}

static void stkmon_print_active_values(char *buf, void *arg)
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
#ifdef CONFIG_SYSTEM_TIME64
	printf(" | %7llu", (uint64_t)clock());
#else
	printf(" | %7lu", (uint32_t)clock());
#endif
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
	ret = utils_readfile(filepath, buf, STKMON_BUFLEN, stkmon_print_active_values, NULL);
	if (ret < 0) {
		printf("Failed to read %s\n", filepath);
		free(filepath);
		return ERROR;
	}
	free(filepath);
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
		utils_proc_pid_foreach(stkmon_read_proc, NULL);
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
		 * it will see the stop indication and will exist.
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
