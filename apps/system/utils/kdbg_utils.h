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

#ifndef __APPS_SYSTEM_UTILS_KDBG_UTILS_H
#define __APPS_SYSTEM_UTILS_KDBG_UTILS_H

#include <tinyara/config.h>
#ifdef CONFIG_ENABLE_STACKMONITOR_CMD
#include <tinyara/clock.h>
#include <sys/types.h>

struct stkmon_save_s {
	clock_t timestamp;
	pid_t chk_pid;
	size_t chk_stksize;
	size_t chk_peaksize;
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	int chk_peakheap;
#endif
#if (CONFIG_TASK_NAME_SIZE > 0)
	char chk_name[CONFIG_TASK_NAME_SIZE + 1];
#endif
};
#endif

#endif							/* __APPS_SYSTEM_UTILS_KDBG_UTILS_H */
