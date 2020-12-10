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
#include <errno.h>
#include <debug.h>
#include <time.h>
#include <semaphore.h>

#include <tinyara/sched.h>
#include <tinyara/kernel_test_drv.h>

#include "clock/clock.h"

/****************************************************************************
 * Private Function
 ****************************************************************************/

static int test_sem_tick_wait(unsigned long arg)
{
	int ret_chk;
	sem_t sem;
	struct timespec cur_time;
	struct timespec base_time;

	/* init sem count to 1 */

	ret_chk = sem_init(&sem, 0, 1);
	if (ret_chk != OK) {
		dbg("sem_init failed.");
		return ERROR;
	}

	/* success to get sem case test */

	ret_chk = clock_gettime(CLOCK_REALTIME, &base_time);
	if (ret_chk != OK) {
		dbg("clock_gettime failed.");
		goto errout_with_sem_init;
	}

	ret_chk = sem_tickwait(&sem, clock(), 2);
	if (ret_chk != OK) {
		dbg("sem_tickwait failed.");
		goto errout_with_sem_init;
	}

	ret_chk = clock_gettime(CLOCK_REALTIME, &cur_time);
	if (ret_chk != OK) {
		dbg("clock_gettime failed.");
		goto errout_with_sem_init;
	}
	if (base_time.tv_sec + 2 == cur_time.tv_sec) {
		dbg("clock_gettime failed.");
		goto errout_with_sem_init;
	}

	ret_chk = sem_post(&sem);
	if (ret_chk != OK) {
		dbg("sem_post failed.");
		goto errout_with_sem_init;
	}

	ret_chk = sem_destroy(&sem);
	if (ret_chk != OK) {
		dbg("sem_destroy failed.");
		goto errout_with_sem_init;
	}

	/* init sem count to 0 */

	ret_chk = sem_init(&sem, 0, 0);
	if (ret_chk != OK) {
		dbg("sem_init failed.");
		return ERROR;
	}

	/* expired time test */

	ret_chk = sem_tickwait(&sem, clock() - 2, 0);
	if (ret_chk != ERROR) {
		dbg("sem_tickwait failed.");
		goto errout_with_sem_init;
	}

	ret_chk = sem_tickwait(&sem, clock() - 2, 1);
	if (ret_chk != ERROR) {
		dbg("sem_tickwait failed.");
		goto errout_with_sem_init;
	}

	ret_chk = sem_tickwait(&sem, clock() - 2, 3);
	if (ret_chk != ERROR) {
		dbg("sem_tickwait failed.");
		goto errout_with_sem_init;
	}

	ret_chk = sem_destroy(&sem);
	if (ret_chk != OK) {
		dbg("sem_destroy failed.");
		goto errout_with_sem_init;
	}

	return OK;

errout_with_sem_init:
	sem_destroy(&sem);
	return ERROR;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int test_sem(int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	switch (cmd) {
	case TESTIOC_SEM_TICK_WAIT_TEST:
		ret = test_sem_tick_wait(arg);
		break;
	}
	return ret;
}
