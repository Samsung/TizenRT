/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <tinyara/clock.h>
#include <tinyara/wqueue.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int wq_test1(FAR void *arg)
{
	systime_t cur_time = 0;
	cur_time = clock_systimer();

	printf("test1 is excuted at (%d) ticks\n", cur_time);

	return 0;
}

static int wq_test2(FAR void *arg)
{
	systime_t cur_time = 0;
	cur_time = clock_systimer();

	printf("test2 is excuted at (%d) ticks\n", cur_time);

	return 0;
}

static int wq_test3(FAR void *arg)
{
	systime_t cur_time = 0;
	cur_time = clock_systimer();

	printf("test3 is excuted at (%d) ticks\n", cur_time);

	return 0;
}

static int wq_test4(FAR void *arg)
{
	systime_t cur_time = 0;
	cur_time = clock_systimer();

	printf("test4 is excuted at (%d) ticks\n", cur_time);

	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * workqueue_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int workqueue_main(int argc, char *argv[])
#endif
{
	systime_t cur_time;
	struct work_s *test_wq1;
	struct work_s *test_wq2;
	struct work_s *test_wq3;
	struct work_s *test_wq4;

	test_wq1 = (struct work_s *)malloc(sizeof(struct work_s));
	test_wq2 = (struct work_s *)malloc(sizeof(struct work_s));
	test_wq3 = (struct work_s *)malloc(sizeof(struct work_s));
	test_wq4 = (struct work_s *)malloc(sizeof(struct work_s));

	cur_time = clock_systimer();

	printf("current time is (%d)\n", cur_time);
	printf("test1 is queued and will excute it after 50 ticks\n");
	work_queue(HPWORK, test_wq1, wq_test1, NULL, 50);

	printf("test2 is queued and will excute it after 30 ticks\n");
	work_queue(HPWORK, test_wq2, wq_test2, NULL, 30);

	printf("test3 is queued and will excute it after 70 ticks\n");
	work_queue(HPWORK, test_wq3, wq_test3, NULL, 70);

	printf("test4 is queued and will excute it after 90 ticks\n");
	work_queue(HPWORK, test_wq4, wq_test4, NULL, 90);
	return 0;
}
