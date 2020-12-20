/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <debug.h>
#include <errno.h>
#include <time.h>

#include <tinyara/kernel_test_drv.h>

#include "clock/clock.h"

/****************************************************************************
 * Private Function
 ****************************************************************************/

static int test_clock_abstime2ticks(unsigned long arg)
{
	int ret_chk;
	int base_tick;
	int comparison_tick;
	struct timespec cur_time;
	struct timespec base_time;
	struct timespec comparison_time;
	struct timespec result_time;

	ret_chk = clock_gettime(CLOCK_REALTIME, &cur_time);
	if (ret_chk != OK) {
		dbg("clock_gettime failed. errno : %d\n", get_errno());
		return ERROR;
	}

	base_time.tv_sec = cur_time.tv_sec + 101;
	base_time.tv_nsec = cur_time.tv_nsec;

	comparison_time.tv_sec = cur_time.tv_sec + 102;
	comparison_time.tv_nsec = cur_time.tv_nsec;
	ret_chk = clock_abstime2ticks(CLOCK_REALTIME, &base_time, &base_tick);
	if (ret_chk == ERROR) {
		dbg("clock_abstime2ticks failed. ret : %d\n", ret_chk);
		return ERROR;
	}

	ret_chk = clock_abstime2ticks(CLOCK_REALTIME, &comparison_time, &comparison_tick);
	if (ret_chk != OK) {
		dbg("clock_abstime2ticks failed. ret : %d\n", ret_chk);
		return ERROR;
	}

	clock_ticks2time(comparison_tick - base_tick, &result_time);
	if (result_time.tv_sec != 1) {
		dbg("clock_abstime2ticks failed. %d.%ld sec is not 1 sec.\n", result_time.tv_sec, result_time.tv_nsec);
		return ERROR;
	}
	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int test_clock(int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	switch (cmd) {
	case TESTIOC_CLOCK_ABSTIME2TICKS_TEST:
		ret = test_clock_abstime2ticks(arg);
		break;
	}
	return ret;
}
