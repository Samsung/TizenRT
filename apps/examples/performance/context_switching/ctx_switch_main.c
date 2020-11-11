/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <sched.h>
#include <time.h>
#include <sys/types.h>

#define SWITCHING_ITERATIONS 1000000

static int yield_task_1(int a, char *b[])
{
	int cnt = SWITCHING_ITERATIONS;
	struct timespec start;
	struct timespec end;
	double diff_time;

	clock_gettime(CLOCK_MONOTONIC, &start);

	while (cnt--) {
		sched_yield();
	}

	clock_gettime(CLOCK_MONOTONIC, &end);

	diff_time = ((double)end.tv_sec + 1.0e-9 * end.tv_nsec) - ((double)start.tv_sec + 1.0e-9 * start.tv_nsec);

	printf("%d-th Average Context Switching Time is %.10f seconds\n", SWITCHING_ITERATIONS, (double)diff_time / (2 * SWITCHING_ITERATIONS));

	return 0;
}

static int yield_task_2(int a, char *b[])
{
	int cnt = SWITCHING_ITERATIONS;

	while (cnt--) {
		sched_yield();
	}

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int ctx_switch_main(int argc, char *argv[])
#endif
{
	printf("Context Switching Performance Measurement\n");

	/* Do not context switching until making two tasks */
	sched_lock();

	task_create("A_Task", SCHED_PRIORITY_MAX, 1024, yield_task_1, NULL);
	task_create("B_Task", SCHED_PRIORITY_MAX, 1024, yield_task_2, NULL);

	sched_unlock();

	return 0;
}
