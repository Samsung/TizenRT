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
 * apps/system/utils/utils_kill.c
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
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <signal.h>
#include <sched.h>
#include <pthread.h>
#ifdef CONFIG_ENABLE_KILLALL
#include "utils_proc.h"
#define KILLALL_BUFLEN 128
#endif

#if defined(CONFIG_ENABLE_KILLALL) && (CONFIG_TASK_NAME_SIZE <= 0)
#error If you want to use killall, CONFIG_TASK_NAME_SIZE should be > 0
#endif

struct utils_sig_s {
	const char *signame;
	int signo;
};

static const struct utils_sig_s utils_sig[] = {
#ifndef CONFIG_DISABLE_SIGNALS
	{"SIGUSR1",         SIGUSR1},
	{"SIGUSR2",         SIGUSR2},
	{"SIGALRM",         SIGALRM},
#ifdef SIGCHLD
	{"SIGCHLD",         SIGCHLD},
#endif
#ifdef SIGPOLL
	{"SIGPOLL",         SIGPOLL},
#endif
#endif /* CONFIG_DISABLE_SIGNALS */
	{"SIGKILL",         SIGKILL},
#ifndef CONFIG_DISABLE_SIGNALS
#ifdef SIGCONDTIMEDOUT
	{"SIGCONDTIMEDOUT", SIGCONDTIMEDOUT},
#endif
#ifdef SIGWORK
	{"SIGWORK",         SIGWORK},
#endif
#endif /* CONFIG_DISABLE_SIGNALS */
	{NULL,              0}
};

#if defined(CONFIG_ENABLE_KILLALL)
struct utils_killall_arg_s {
	int signo;
	char name[CONFIG_TASK_NAME_SIZE + 1];
	int count;
	int *ret;
};
#endif

static int find_signal(char *ptr)
{
	char *endptr;
	int ret = ERROR;
	int signo;
	int sigidx;

	/* Check incoming parameters.  The first parameter should be "-<signal>" */
	if (ptr == NULL || ptr[0] != '-') {
		return ERROR;
	} else if ('0' <= ptr[1] && ptr[1] <= '9') {
		/* Extract the signal number */

		signo = strtol(&ptr[1], &endptr, 0);
		for (sigidx = 0; utils_sig[sigidx].signo != 0; sigidx++) {
			if (utils_sig[sigidx].signo == signo) {
				ret = utils_sig[sigidx].signo;
				break;
			}
		}
	} else {
		/* Find the signal number using signal name */

		for (sigidx = 0; utils_sig[sigidx].signame != NULL; sigidx++) {
			if (strcasecmp(&ptr[1], utils_sig[sigidx].signame) == 0) {
				ret = utils_sig[sigidx].signo;
				break;
			}
		}
	}

	return ret;
}

static int send_signal(pid_t pid, int signo)
{
	int ret = ERROR;

	/* Send the signal.  Kill return values:
	 *
	 *   EINVAL An invalid signal was specified.
	 *   EPERM  The process does not have permission to send the signal to any
	 *          of the target processes.
	 *   ESRCH  The pid or process group does not exist.
	 *   ENOSYS Do not support sending signals to process groups.
	 */

#ifndef CONFIG_DISABLE_SIGNALS
	ret = kill(pid, signo);
#endif

	if (ret != OK) {
		printf("Send_signal failed. errno : %d\n", get_errno());
	}

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#if defined(CONFIG_ENABLE_KILL)
int utils_kill(int argc, char **args)
{
	int signo;
	int sigidx;
	pid_t pid;
	char *ptr;
	char *endptr;

	if (argc < 2 || argc > 3) {
		printf("Invalid argument\n");
		goto usage;
	}

	if (!strncmp(args[1], "--help", strlen("--help") + 1)) {
		goto usage;
	}

	if (argc == 2) {
		/* For a case that no signal is specified or '-l' option */
		/* 'kill PID' or 'kill -l' */
		ptr = args[1];
		if (!strncmp(ptr, "-l", strlen("-l") + 1)) {
			/* List signal numbers and it's name */
			for (sigidx = 0; utils_sig[sigidx].signame != NULL; sigidx++) {
				printf("%2d) %-15s\n", utils_sig[sigidx].signo, utils_sig[sigidx].signame);
			}
			return OK;
		}
		signo = SIGKILL;
	} else {
		/* For a case that signal and pid are specified. */
		/* 'kill -SIGNAME|SIGNUM PID' */
		signo = find_signal(args[1]);
		if (signo == ERROR) {
			printf("%s: invalid signal specification\n", args[1]);
			return ERROR;
		}
		ptr = args[2];
	}

	if (*ptr < '0' || *ptr > '9') {
		printf("Invlid arguments. It must be process ids\n");
		return ERROR;
	}

	/* Extract the pid */

	pid = strtol(ptr, &endptr, 0);

	/* Send the signal */

	if (send_signal(pid, signo) != OK) {
		return ERROR;
	}

	return OK;

usage:
	printf("\nUsage: kill [-SIGNAME|SIGNUM] PID\n");
	printf("   or: kill -l\n");
	printf("Send a signal SIGNAME or SIGNUM to a process PID\n");
	printf("If no signal is specified, SIGKILL is sent because we don't support SIGTERM\n");
	printf("* Caution: SIGKILL terminates task/thread without any operations\n");
	printf("\nOptions:\n");
	printf(" -l		List all signal names\n");

	return ERROR;
}
#endif

#if defined(CONFIG_ENABLE_KILLALL)
static void killall_send_signal(char *buf, void *arg)
{
	int i;
	stat_data stat_info[PROC_STAT_MAX];
	char *name;
	int pid;
	struct utils_killall_arg_s *killall_arg = (struct utils_killall_arg_s *)arg;
	if (killall_arg == NULL) {
		printf("Failed to send signal, because of invalid parameter.\n");
		return;
	}

	stat_info[0] = buf;
	for (i = 0; i < PROC_STAT_MAX - 1; i++) {
		stat_info[i] = strtok_r(stat_info[i], " ", &stat_info[i + 1]);
	}
	name = stat_info[PROC_STAT_NAME];
	pid = atoi(stat_info[PROC_STAT_PID]);

	if (strncmp(name, killall_arg->name, CONFIG_TASK_NAME_SIZE) == 0) {
		killall_arg->count++;
		if (send_signal(pid, killall_arg->signo) != OK) {
			*(killall_arg->ret) = ERROR;
		}
	}
}

static int killall_read_proc(FAR struct dirent *entryp, FAR void *arg)
{
	char *filepath;
	char buf[KILLALL_BUFLEN];
	int ret = OK;

	asprintf(&filepath, "%s/%s/%s", PROCFS_MOUNT_POINT, entryp->d_name, "stat");
	ret = utils_readfile(filepath, buf, KILLALL_BUFLEN, killall_send_signal, arg);
	if (ret < 0) {
		printf("Failed to read %s\n", filepath);
		ret = ERROR;
	}
	free(filepath);
	return ret;
}

int utils_killall(int argc, char **args)
{
	int signo;
	int sigidx;
	char *ptr;
	struct utils_killall_arg_s arg;
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	int ret;
	bool is_mounted = false;
#endif

	if (argc < 2 || argc > 3) {
		printf("Invalid argument\n");
		goto usage;
	}

	if (!strncmp(args[1], "--help", strlen("--help") + 1)) {
		goto usage;
	}

	if (argc == 2) {
		/* For a case that no signal is specified or '-l' option */
		/* 'killall NAME' or 'killall -l' */
		ptr = args[1];
		if (!strncmp(ptr, "-l", strlen("-l") + 1)) {
			/* List signal numbers and it's name */
			for (sigidx = 0; utils_sig[sigidx].signame != NULL; sigidx++) {
				printf("%2d) %-15s\n", utils_sig[sigidx].signo, utils_sig[sigidx].signame);
			}
			return OK;
		} else if (ptr[0] == '-') {
			/* Invalid argument as process name */
			printf("Invlid arguments.\n");
			goto usage;
		} else {
			signo = SIGKILL;
		}
	} else {
		/* For a case that signal and name are specified. */
		/* 'killall -SIGNAME|SIGNUM NAME' */
		signo = find_signal(args[1]);
		if (signo == ERROR) {
			printf("%s: invalid signal specification\n", args[1]);
			return ERROR;
		}
		ptr = args[2];
	}

#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
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

	arg.signo = signo;
	strncpy(arg.name, ptr, CONFIG_TASK_NAME_SIZE);
	arg.name[CONFIG_TASK_NAME_SIZE] = '\0';
	arg.count = 0;
	arg.ret = OK;

	utils_proc_pid_foreach(killall_read_proc, (void *)&arg);

#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	if (!is_mounted) {
		/* Detach mounted Procfs */
		(void)umount(PROCFS_MOUNT_POINT);
	}
#endif

	return OK;

usage:
	printf("\nUsage: killall [-SIGNAME|SIGNUM] NAME\n");
	printf("   or: killall -l\n");
	printf("Send a signal specified by SIGNAME or SIGNUM to a processes named NAME\n");
	printf("If no signal is specified, SIGKILL is sent because we don't support SIGTERM\n");
	printf("* Caution: SIGKILL terminates task/thread without any operations\n");
	printf("\nOptions:\n");
	printf(" -l           List all signal names\n");

#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	if (!is_mounted) {
		/* Detach mounted Procfs */
		(void)umount(PROCFS_MOUNT_POINT);
	}
#endif
	return ERROR;
}
#endif
