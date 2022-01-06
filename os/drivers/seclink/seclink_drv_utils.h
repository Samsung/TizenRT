/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#pragma once

#ifndef LINUX
#include <debug.h>
#endif
#ifdef CONFIG_SECURITY_LINK_DRV_PROFILE
#include <unistd.h>
#include <tinyara/timer.h>
#endif

#define SLDRV_LOG printf
#define SLDRV_TAG "[SECLINK_DRV]"

#define SLDRV_ERR(fd)                                       \
	do {                                                    \
		SLDRV_LOG(SL_TAG "%s:%d ret(%d) code(%s))\n",       \
				  __FILE__, __LINE__, fd, strerror(errno)); \
	} while (0)

#define SLDRV_ENTER

#ifdef CONFIG_SECURITY_LINK_DRV_PROFILE

#define SLDRV_CALL(ret, res, method, param)                                         \
	do {                                                                            \
		if (se->ops->method) {                                                      \
			sldrv_timer_handle hnd;																		\
			(void)sldrv_start_time(&hnd);                                           \
			res = (se->ops->method)param;                                           \
			(void)sldrv_get_time(&hnd);                                             \
			SLDRV_LOG(SLDRV_TAG "[PERF] T%d " #method " res %d elapsed %u %u ms\n", \
					  getpid(), res, hnd.elapsed, hnd.elapsed / 1000);              \
		} else {                                                                    \
			ret = -ENOSYS;                                                          \
		}                                                                           \
	} while (0)

typedef struct {
	int fd;
	struct timer_status_s start;
	struct timer_status_s end;
	uint32_t elapsed; /*  micro seconds */
} sldrv_timer_handle;

int sldrv_start_time(sldrv_timer_handle *hnd);
int sldrv_get_time(sldrv_timer_handle *hnd);
#else

#define SLDRV_CALL(ret, res, method, param) \
	do {                                    \
		if (se->ops->method) {              \
			res = (se->ops->method)param;   \
		} else {                            \
			ret = -ENOSYS;                  \
		}                                   \
	} while (0)

#endif
