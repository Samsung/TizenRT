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

#ifndef __APPS_SYSTEM_UTILS_UTILS_PROC_H
#define __APPS_SYSTEM_UTILS_UTILS_PROC_H

#include <tinyara/config.h>
#include <dirent.h>

enum proc_stat_data_e {
	PROC_STAT_PID = 0,
	PROC_STAT_PPID,
	PROC_STAT_PRIORITY,
	PROC_STAT_FLAG,
	PROC_STAT_STATE,
	PROC_STAT_TOTALSTACK,
	PROC_STAT_PEAKSTACK,
	PROC_STAT_CURRHEAP,
	PROC_STAT_PEAKHEAP,
#ifdef CONFIG_SCHED_CPULOAD
#ifdef CONFIG_SCHED_MULTI_CPULOAD
	PROC_STAT_CPULOAD_SHORT,
	PROC_STAT_CPULOAD_MID,
	PROC_STAT_CPULOAD_LONG,
#else
	PROC_STAT_CPULOAD,
#endif
#endif
#ifdef CONFIG_APP_BINARY_SEPARATION
	PROC_STAT_HEAP_NAME,
#endif
#if CONFIG_TASK_NAME_SIZE > 0
	PROC_STAT_NAME,
#endif
	PROC_STAT_MAX,
};

typedef char *stat_data;
typedef int (*procentry_handler_t)(FAR struct dirent *entryp, FAR void *arg);
typedef void (*utils_handler_t)(char *buf, void *arg);

int utils_proc_pid_foreach(procentry_handler_t handler, void *arg);
int utils_readfile(FAR const char *filepath, char *buf, int buflen, utils_handler_t print_func, void *arg);

#endif							/* __APPS_SYSTEM_UTILS_UTILS_PROC_H */
