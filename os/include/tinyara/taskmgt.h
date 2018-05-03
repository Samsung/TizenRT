/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_TASKMGT_H
#define __INCLUDE_TINYARA_TASKMGT_H

/* This file will be used to provide definitions to support
 * task manager framework
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <mqueue.h>
#include <stdbool.h>
#include <debug.h>
#include <tinyara/fs/ioctl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define TASKMGT_DRVPATH     "/dev/taskmgr"

#ifdef CONFIG_TASK_MANAGER_DEBUG
#ifdef CONFIG_TASK_MANAGER_DEBUG_ERROR
#define tmdbg(format, ...)      dbg(format, ##__VA_ARGS__)
#else
#define tmdbg(x...)
#endif
#ifdef CONFIG_TASK_MANAGER_DEBUG_INFO
#define tmvdbg(format, ...)     vdbg(format, ##__VA_ARGS__)
#else
#define tmvdbg(x...)
#endif
#else
#define tmdbg(x...)
#define tmvdbg(x...)
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/
typedef int (*TASK_MANAGER_CALLBACK)(int argc, char **args);

struct task_builtin_list_s {
	const char *name;
	const TASK_MANAGER_CALLBACK entry;
	const int priority;
	const int stacksize;
};
typedef struct task_builtin_list_s task_builtin_list_t;

struct tm_request_s {
	int cmd;
	int caller_pid;
	int handle;
	int timeout;
	char *q_name;
	void* data;
};
typedef struct tm_request_s tm_request_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_TINYARA_TEST_FW_H */
