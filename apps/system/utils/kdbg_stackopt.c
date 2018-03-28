/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <apps/shell/tash.h>
#include "kdbg_utils.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifndef CONFIG_STACK_ALIGNMENT

/* The symbol  __ARM_EABI__ is defined by GCC if EABI is being used.  If you
 * are not using GCC, make sure that CONFIG_STACK_ALIGNMENT is set correctly!
 */

#ifdef __ARM_EABI__
#define CONFIG_STACK_ALIGNMENT 8
#else
#define CONFIG_STACK_ALIGNMENT 4
#endif
#endif

#define STACK_ALIGN_MASK    (CONFIG_STACK_ALIGNMENT - 1)
#define STACK_ALIGN_UP(a)   (((a) + STACK_ALIGN_MASK) & ~STACK_ALIGN_MASK)

/****************************************************************************
 * Extern Data
 ****************************************************************************/
extern struct stkmon_save_s stkmon_arr[CONFIG_MAX_TASKS * 2];
extern const tash_taskinfo_t tash_taskinfo_list[];

static bool find_app(TASH_CMD_CALLBACK *cb, char *name)
{
	int cmd_idx;
	bool flag = false;
	char str[CONFIG_TASK_NAME_SIZE];
	int count = tash_get_cmdscount();
	
	for (cmd_idx = 0; cmd_idx < count; cmd_idx++) {
		if (tash_get_cmdpair(str, cb, cmd_idx) == OK) {
			if (!strncmp(name, str, strlen(name) + 1)) {
				flag = true;
				break;
			}
		}
	}
	return flag;
}

static int execute_app(char **args)
{
	int app_idx;
	TASH_CMD_CALLBACK cb;
	int pid = 0;
	int pri = CONFIG_TASH_CMDTASK_PRIORITY;
	long stack_size = CONFIG_TASH_CMDTASK_STACKSIZE;

	if (find_app(&cb, args[0])) {
#if defined(CONFIG_BUILTIN_APPS)
		for (app_idx = 0; tash_taskinfo_list[app_idx].str != NULL; app_idx++) {
			if (!strncmp(args[0], tash_taskinfo_list[app_idx].str, CONFIG_TASK_NAME_SIZE)) {
				pri = tash_taskinfo_list[app_idx].task_prio;
				stack_size = tash_taskinfo_list[app_idx].task_stacksize;
				break;
			}
		}
#endif
		pid = task_create(args[0], pri, stack_size, cb , &args[1]);
	}
	return pid;
}

static int get_frame_size(int argc, char **args)
{
	int args_idx = 0;
	size_t strtablen = 0;
	size_t argvlen = (argc + 1) * sizeof(FAR char *);
	size_t frame_size = 0;
	
	while (args[args_idx]) {
		strtablen += (strlen(args[args_idx]) + 1);
		args_idx++;
	}
	
	frame_size = STACK_ALIGN_UP(argvlen + strtablen);
	return frame_size;
}

static void print_measure_result(pid_t pid, int argc, char **args)
{
	int stkmon_idx;
	for (stkmon_idx = 0; stkmon_idx < CONFIG_MAX_TASKS * 2; stkmon_idx++) {
		if (stkmon_arr[stkmon_idx].chk_pid == pid) {
			printf("---------------------------------------------------------------\n");
			printf(">> Stack Size Measure Finish\n");
			printf("APP Name : %s\n", stkmon_arr[stkmon_idx].chk_name);
			printf("Allocate %d stack size for measure\n",  stkmon_arr[stkmon_idx].chk_stksize);
			printf("Optimal Stack Size is %d\n", stkmon_arr[stkmon_idx].chk_peaksize + get_frame_size(argc, args));
			printf("If the arguments are changed, optimal stack size can be changed\n");
			printf("---------------------------------------------------------------\n");
			return;
		}
	}
	printf("[ERROR] Cannot find the app\n");
	return;
}

int kdbg_stackopt(int argc, char **args)
{
	int pid;
	int status;

	if (argc < 2) {
		printf("Invalid argument\n");
		goto usage;
	}
	
	if (!strncmp(args[1], "--help", strlen("--help") + 1)) {
		goto usage;
	}
	
	if ((pid = execute_app(&args[1])) < 0) {
		printf("Cannot create task\n");
		goto usage;
	} else if (pid == 0) {
		printf("%s : Cannot find APP\n", args[1]);
		goto usage;
	} else {
		waitpid((pid_t)pid, &status, 0);
		print_measure_result((pid_t)pid, argc - 1, &args[1]);
	}
	return OK;

usage:
	printf("\nUsage: %s NAME [ARGS]\n", args[0]);
	printf("Measure optimal stack size of APP\n");
	printf("NAME is the name of the APP you want to measure the optimal stack size\n");
	printf("ARGS is the arguments of the APP named NAME\n");
	printf("If the arguments are changed, optimal stack size can be changed\n");
	return ERROR;
}
