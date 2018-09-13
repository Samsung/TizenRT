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

/// @file tc_wqueue.c

/// @brief Test Case Example for Work Queue API

/**************************************************************************
* Included Files
**************************************************************************/
#include <tinyara/config.h>
#include <tinyara/clock.h>
#include <tinyara/wqueue.h>
#include <stdio.h>
#include <sys/types.h>
#include "tc_internal.h"

/**************************************************************************
* Private Definitions
**************************************************************************/
#if defined(CONFIG_SCHED_WORKQUEUE) || defined(CONFIG_LIB_USRWORK)

/**************************************************************************
* Private Variables
**************************************************************************/

static clock_t start_time;

/**************************************************************************
* Private Functions
**************************************************************************/
static void wq_test1(void *arg)
{
	clock_t cur_time = 0;
	cur_time = clock();
	printf("workqueue_test 1 : test 1 requested delay is (%u) ticks, executed delay is (%llu) ticks.\n", (uint32_t)arg, (uint64_t)cur_time - (uint64_t)start_time);
}

static void wq_test2(void *arg)
{
	clock_t cur_time = 0;
	cur_time = clock();
	printf("workqueue_test 2 : test 2 requested delay is (%u) ticks, executed delay is (%llu) ticks.\n", (uint32_t)arg, (uint64_t)cur_time - (uint64_t)start_time);
}

static void wq_test3(void *arg)
{
	clock_t cur_time = 0;
	cur_time = clock();
	printf("workqueue_test 3 : test 3 requested delay is (%u) ticks, executed delay is (%llu) ticks.\n", (uint32_t)arg, (uint64_t)cur_time - (uint64_t)start_time);
}
/**************************************************************************
* Public Functions
**************************************************************************/
#if defined(CONFIG_SCHED_HPWORK) || defined(CONFIG_SCHED_LPWORK)
static void tc_wqueue_work_queue_cancel(void)
{
	int result;
	struct work_s *test_work1;
	struct work_s *test_work2;
	struct work_s *test_work3;

	test_work1 = (struct work_s *)malloc(sizeof(struct work_s));
	test_work2 = (struct work_s *)malloc(sizeof(struct work_s));
	test_work3 = (struct work_s *)malloc(sizeof(struct work_s));

	start_time = clock();

	printf("workqueue_test: test1 is queued and will excute it after 50 ticks\n");
	result = work_queue(HPWORK, test_work1, wq_test1, (void *)50, 50);
	TC_ASSERT_EQ_CLEANUP("work_queue", result, OK, goto cleanup);

	result = work_queue(LPWORK, test_work2, wq_test2, (void *)60, 60);
	TC_ASSERT_EQ_CLEANUP("work_queue", result, OK, goto cleanup);
	result = work_cancel(LPWORK, test_work2);
	TC_ASSERT_EQ_CLEANUP("work_cancel", result, OK, goto cleanup);

	result = work_queue(HPWORK, test_work3, wq_test3, (void *)30, 30);
	TC_ASSERT_EQ_CLEANUP("work_queue", result, OK, goto cleanup);
	result = work_cancel(HPWORK, test_work3);
	TC_ASSERT_EQ_CLEANUP("work_cancel", result, OK, goto cleanup);

	printf("workqueue_test: test3 is queued and will excute it after 80 ticks\n");
	result = work_queue(LPWORK, test_work3, wq_test3, (void *)80, 80);
	TC_ASSERT_EQ_CLEANUP("work_queue", result, OK, goto cleanup);

	sleep(1);

cleanup:
	free(test_work1);
	free(test_work2);
	free(test_work3);
	TC_SUCCESS_RESULT();
}
#endif
/****************************************************************************
 * Name: mqueue
 ****************************************************************************/
int wqueue_main(void)
{
#if defined(CONFIG_SCHED_HPWORK) || defined(CONFIG_SCHED_LPWORK)
	tc_wqueue_work_queue_cancel();
#endif
	return 0;
}
#endif
