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
/****************************************************************************
 * apps/system/utils/utils_ps.c
 *
 *   Copyright (C) 2007-2009, 2011-2012, 2014 Gregory Nutt. All rights reserved.
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

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
#include <errno.h>
#include <stdbool.h>
#include <sys/mount.h>
#endif
#include <tinyara/sched.h>
#include <tinyara/fs/fs.h>

#include "utils_proc.h"

#define STATENAMES_ARRAY_SIZE (sizeof(utils_statenames) / sizeof(utils_statenames[0]))
#define PS_BUFLEN 128

static const char *utils_statenames[] = {
	"INVALID ",
	"PENDING ",
	"READY   ",
	"RUNNING ",
	"INACTIVE",
	"WAITSEM ",
	"WAITUNBLOCK",
#ifndef CONFIG_DISABLE_SIGNALS
	"WAITSIG ",
#endif
#ifndef CONFIG_DISABLE_MQUEUE
	"MQNEMPTY",
	"MQNFULL "
#endif
};

static const char *utils_ttypenames[4] = {
	"TASK   ",
	"PTHREAD",
	"KTHREAD",
	"--?--  "
};

static void ps_print_values(char *buf, void *arg)
{
	int i;
	int flags;
	int state;
	stat_data stat_info[PROC_STAT_MAX];

	stat_info[0] = buf;

	for (i = 0; i < PROC_STAT_MAX - 1; i++) {
		stat_info[i] = strtok_r(stat_info[i], " ", &stat_info[i + 1]);
	}

	flags = atoi(stat_info[PROC_STAT_FLAG]);
	if (flags <= 0) {
		return;
	}

	state = atoi(stat_info[PROC_STAT_STATE]);
	if (state <= 0 || STATENAMES_ARRAY_SIZE <= state) {
		return;
	}

	printf("%5s | %4s | %4s | %7s | %c%c | %8s", stat_info[PROC_STAT_PID], stat_info[PROC_STAT_PRIORITY], \
		flags & TCB_FLAG_ROUND_ROBIN ? "RR  " : "FIFO", utils_ttypenames[(flags & TCB_FLAG_TTYPE_MASK) >> TCB_FLAG_TTYPE_SHIFT], \
		flags & TCB_FLAG_NONCANCELABLE ? 'N' : ' ', flags & TCB_FLAG_CANCEL_PENDING ? 'P' : ' ', \
		utils_statenames[state]);

#if (CONFIG_TASK_NAME_SIZE > 0)
	printf(" | %s\n", stat_info[PROC_STAT_NAME]);
#else
	printf("\n");
#endif

}

static int ps_read_proc(FAR struct dirent *entryp, FAR void *arg)
{
	int ret;
	char *filepath;
	char buf[PS_BUFLEN];

	asprintf(&filepath, "%s/%s/%s", PROCFS_MOUNT_POINT, entryp->d_name, "stat");
	ret = utils_readfile(filepath, buf, PS_BUFLEN, ps_print_values, NULL);
	if (ret < 0) {
		printf("Failed to read %s\n", filepath);
		free(filepath);
		return ERROR;
	}
	free(filepath);
	return OK;
}

int utils_ps(int argc, char **args)
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
			return ERROR;
		}
	}

#endif

	printf("\n");
#if (CONFIG_TASK_NAME_SIZE > 0)
	printf("  PID | PRIO | FLAG |  TYPE   | NP |  STATUS  | NAME\n");
	printf("------|------|------|---------|----|----------|----------\n");
#else
	printf("  PID | PRIO | FLAG |  TYPE   | NP |  STATUS  \n");
	printf("------|------|------|---------|----|----------\n");
#endif
	/* Print information for each task/thread */
	utils_proc_pid_foreach(ps_read_proc, NULL);

#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	if (!is_mounted) {
		/* Detach mounted Procfs */
		(void)umount(PROCFS_MOUNT_POINT);
	}
#endif

	return OK;
}
