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
#include <tinyara/config.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>

#include <eventloop/eventloop.h>

el_timer_t *timer_10s;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void timer_callback_2s(void *data)
{
	struct timeval now;
	(void)gettimeofday(&now, NULL);
	printf("%s timeout %d %d\n", __func__, now.tv_sec, now.tv_usec);
}

static void timer_callback_10s(void *data)
{
	struct timeval now;
	(void)gettimeofday(&now, NULL);
	printf("%s timeout %d %d\n", __func__, now.tv_sec, now.tv_usec);

	if (data != NULL) {
		printf("Callback %s\n", (char *)data);
	}
}

static void timer_callback_20s(void *data)
{
	struct timeval now;
	(void)gettimeofday(&now, NULL);
	printf("%s timeout %d %d\n", __func__, now.tv_sec, now.tv_usec);

	if (data != NULL) {
		el_timer_t *timer = (el_timer_t *)data;
		printf("Stop timer with interval 2s\n");
		eventloop_delete_timer(timer);
	}
}

static void timer_callback_40s(void *data)
{
	struct timeval now;
	(void)gettimeofday(&now, NULL);
	printf("%s timeout %d %d\n", __func__, now.tv_sec, now.tv_usec);

	printf("Stop timer with interval 10s\n");
	eventloop_delete_timer(timer_10s);

	eventloop_loop_stop();
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int eventloop_sample_main(int argc, FAR char *argv[])
#endif
{
	el_timer_t *timer_2s;
	el_timer_t *timer_20s;
	el_timer_t *timer_40s;

	/* Add timer to be called every 2 seconds */
	timer_2s = eventloop_add_timer_async(2000, true, timer_callback_2s, NULL);

	/* Add timer to be called every 10 seconds */
	timer_10s = eventloop_add_timer_async(10000, true, timer_callback_10s, "10s Timeout");

	/* Add timer to be called once after 20s. It deletes the timer with 2s interval through passing the pointer */
	timer_20s = eventloop_add_timer(20000, false, timer_callback_20s, timer_2s);

	/* Add timer to be called once after 40s. It deletes the timer with 10s interval declared as global variable */
	timer_40s = eventloop_add_timer(40000, false, timer_callback_40s, NULL);

	/* Run loop */
	eventloop_loop_run();

	/* Delete created timers */
	eventloop_delete_timer(timer_20s);
	eventloop_delete_timer(timer_40s);
	/* Other timers with 2s and 10s interval are already deleted in callback functions, timer_callback_20s and timer_callback_40s */

	return 0;
}
