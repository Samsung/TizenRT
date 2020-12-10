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
#include <signal.h>

#include <tinyara/kernel_test_drv.h>

#include "signal/signal.h"
#include "timer/timer.h"

/****************************************************************************
 * Private Function
 ****************************************************************************/

static int test_timer_initialize(unsigned long arg)
{
	int ret_chk;
	timer_t timer_id;
	struct sigevent st_sigevent;
	FAR struct posix_timer_s *timer;
	FAR struct posix_timer_s *next;

	int initalloc_cnt = 0;
	int initfree_cnt = 0;
	int createalloc_cnt = 0;
	int createfree_cnt = 0;
	int finalalloc_cnt = 0;
	int finalfree_cnt = 0;

	/* Set and enable alarm */
	st_sigevent.sigev_notify = SIGEV_SIGNAL;
	st_sigevent.sigev_signo = SIGRTMIN;
	st_sigevent.sigev_value.sival_ptr = &timer_id;

	/* check the count for g_alloctimers and g_freetimers after timer_initialize */
	timer_initialize();

	for (timer = (FAR struct posix_timer_s *)g_alloctimers.head; timer; timer = next) {
		next = timer->flink;
		initalloc_cnt++;
	}

	for (timer = (FAR struct posix_timer_s *)g_freetimers.head; timer; timer = next) {
		next = timer->flink;
		initfree_cnt++;
	}

	/* check the count for g_alloctimers and g_freetimers after create now they change */
	ret_chk = timer_create(CLOCK_REALTIME, &st_sigevent, &timer_id);
	if (ret_chk == ERROR) {
		dbg("timer_create failed.");
		return ERROR;
	}

	if (timer_id == NULL) {
		dbg("timer_create failed.");
		return ERROR;
	}

	for (timer = (FAR struct posix_timer_s *)g_alloctimers.head; timer; timer = next) {
		next = timer->flink;
		createalloc_cnt++;
	}

	for (timer = (FAR struct posix_timer_s *)g_freetimers.head; timer; timer = next) {
		next = timer->flink;
		createfree_cnt++;
	}

	/* check the count for g_alloctimers and g_freetimers after timer_initialize now they change to original value */
	timer_initialize();

	for (timer = (FAR struct posix_timer_s *)g_alloctimers.head; timer; timer = next) {
		next = timer->flink;
		finalalloc_cnt++;
	}

	for (timer = (FAR struct posix_timer_s *)g_freetimers.head; timer; timer = next) {
		next = timer->flink;
		finalfree_cnt++;
	}

	ret_chk = timer_delete(timer_id);

	if (ret_chk == ERROR) {
		dbg("timer_delete failed.");
		return ERROR;
	}

	if (initalloc_cnt != finalalloc_cnt) {
		dbg("timer_initialize failed.");
		return ERROR;
	}

	if (initfree_cnt != finalfree_cnt) {
		dbg("timer_initialize failed.");
		return ERROR;
	}

	if (createalloc_cnt == finalalloc_cnt) {
		dbg("timer_initialize failed.");
		return ERROR;
	}

	if (createfree_cnt == finalfree_cnt) {
		dbg("timer_initialize failed.");
		return ERROR;
	}

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int test_timer(int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	switch (cmd) {
	case TESTIOC_TIMER_INITIALIZE_TEST:
		ret = test_timer_initialize(arg);
		break;
	}
	return ret;
}
