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

#define CALLBACK_DATA "EVENTLOOP_TIMER_CALLBACK"

el_timer_t *timer_5s;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static bool timer_callback_2s(void *data)
{
	static int cnt_2s;
	struct timeval now;

	(void)gettimeofday(&now, NULL);
	printf("%s timeout %d %d\n", __func__, now.tv_sec, now.tv_usec);

	if (cnt_2s++ >= 10) {
		printf("Stop timer with interval 2s by itself \n");
		cnt_2s = 0;
		free(data);
		return EVENTLOOP_CALLBACK_STOP;
	}

	return EVENTLOOP_CALLBACK_CONTINUE;
}

static bool timer_callback_5s(void *data)
{
	struct timeval now;

	(void)gettimeofday(&now, NULL);
	printf("%s timeout %d %d\n", __func__, now.tv_sec, now.tv_usec);

	if (data != NULL) {
		printf("Callback data : %s\n", (char *)data);
	}

	return EVENTLOOP_CALLBACK_CONTINUE;
}

static void timer_callback_20s(void *data)
{
	struct timeval now;

	(void)gettimeofday(&now, NULL);
	printf("%s timeout %d %d\n", __func__, now.tv_sec, now.tv_usec);

	if (data != NULL) {
		printf("Callback data : %s\n", (char *)data);
	}
}

static void timer_callback_30s(void *data)
{
	int ret;
	struct timeval now;

	(void)gettimeofday(&now, NULL);
	printf("%s timeout %d %d\n", __func__, now.tv_sec, now.tv_usec);

	ret = eventloop_delete_timer(timer_5s);
	if (ret == OK) {
		printf("Stop timer with interval 5s\n");
	} else {
		printf("Failed to stop timer with interval 5s\n");
	}
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
	el_timer_t *timer_30s;
	char *user_data;

	/* Add timer to be called every 2 seconds */
	user_data = (char *)strdup(CALLBACK_DATA);
	timer_2s = eventloop_add_timer_async(2000, true, timer_callback_2s, user_data);

	/* Add timer to be called every 5 seconds */
	timer_5s = eventloop_add_timer_async(5000, true, timer_callback_5s, NULL);

	/* Add timer to be called once after 20s.  */
	timer_20s = eventloop_add_timer(20000, false, (timeout_callback)timer_callback_20s, CALLBACK_DATA);

	/* Add timer to be called once after 30s. It will delete created timer with 5s interval. */
	timer_30s = eventloop_add_timer(30000, false, (timeout_callback)timer_callback_30s, NULL);

	/* Run loop */
	eventloop_loop_run();

	return 0;
}
