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

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define NTHREADS	5
#define LOOP		3

#define MSEC_PER_SEC	1000 /* 1s = 1000ms */

#define MSEC_PER_NSEC	(1 / 1000000) /* 1ns = (1/1000000)ms */

#define USEC_PER_MSEC	1000 /* 1ms = 1000us */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <time.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void *thread_func(void *arg)
{
	unsigned int duration = (unsigned int)arg;
	pid_t cur_thread = getpid();

	systime_t t1 = clock_systimer();
	t1 = (1000 * (t1 % CLOCKS_PER_SEC) + (CLOCKS_PER_SEC / 2)) / CLOCKS_PER_SEC;

	printf("thread with pid=%d sleeping for %ums\n", cur_thread, duration);

	/* sleep for 'duration' micro seconds */
	usleep(duration * USEC_PER_MSEC);

	systime_t t2 = clock_systimer();
	t2 = (1000 * (t2 % CLOCKS_PER_SEC) + (CLOCKS_PER_SEC / 2)) / CLOCKS_PER_SEC;

	printf("thread with pid=%d woke up after %ums\n", cur_thread, (t2 - t1));

	return NULL;
}

static void tickless_test_helper(void)
{
	pthread_t thread_id[NTHREADS];
	unsigned int thread;
	unsigned int sleep_time[NTHREADS] = {1, 33, 89, 67, 17}; /* in milli seconds */

	for (thread = 0; thread < NTHREADS; thread++) {
		pthread_create(&thread_id[thread], NULL, (pthread_addr_t)thread_func, (pthread_addr_t *)(sleep_time[thread]));
	}

	for (thread = 0; thread < NTHREADS; thread++) {
		pthread_join(thread_id[thread], NULL);
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/* sleep test for tickless feature */

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tickless_test_main(int argc, char *argv[])
#endif
{
	unsigned int i = 0;
	printf("\n%s: 'Begin' Test\n\n", __func__);

	for (i = 0; i < LOOP; i++) {
		printf("Loop %d Begin\n\n", i + 1);
		tickless_test_helper();
		printf("\nLoop %d End\n", i + 1);
	}

	printf("\n%s: 'End' Test\n", __func__);
	return 0;
}
