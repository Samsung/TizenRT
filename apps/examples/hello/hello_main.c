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
 * examples/hello/hello_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>

#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define HOG_MSEC       1000
#define YIELD_MSEC     100
#define IMPOSSIBLE_CPU -1
#define CPU_ZERO(s) do { *(s) = 0; } while (0)
#define CPU_SET(c,s) do { *(s) |= (1 << (c)); } while (0)

#define SMP_NTHREADS 2

/****************************************************************************
 * Private Data
 ****************************************************************************/

static pthread_barrier_t g_smp_barrier;
static volatile int g_thread_cpu[SMP_NTHREADS + 1];
static int g_pid_start = 0;
static uint8_t affinity = 0;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: show_cpu / show_cpu_conditional
 *
 * Description:
 *   This depends on internal OS interfaces that are not generally available
 *   but can be accessed (albeit inappropriately) in a FLAT build
 *
 ****************************************************************************/

static void show_cpu(FAR const char *caller, int threadno)
{
      	g_thread_cpu[threadno] = sched_getcpu();
	printf("%s[%d]: Running on CPU%d\n", caller, threadno, g_thread_cpu[threadno]);
}

static void show_cpu_conditional(FAR const char *caller, int threadno)
{
	int cpu = sched_getcpu();

	if (cpu != g_thread_cpu[threadno]) {
		g_thread_cpu[threadno] = cpu;
		printf("%s[%d]: Now running on CPU%d\n", caller, threadno, cpu);
	}
}

/****************************************************************************
 * Name: hog_milliseconds
 *
 * Description:
 *   Delay inline for the specified number of milliseconds.
 *
 ****************************************************************************/

static void hog_milliseconds(unsigned int milliseconds)
{
	volatile unsigned int i;
	volatile unsigned int j;

	for (i = 0; i < milliseconds; i++) {
		for (j = 0; j < CONFIG_BOARD_LOOPSPERMSEC; j++) {
		}
	}
}

/****************************************************************************
 * Name: hog_time
 *
 * Description:
 *   Delay for awhile, calling pthread_yield() now and then to allow other
 *   pthreads to get CPU time.
 *
 ****************************************************************************/

static void hog_time(FAR const char *caller, int threadno)
{
	unsigned int remaining = HOG_MSEC;
	unsigned int hogmsec;

	while (remaining > 0) {
		/* Hog some CPU */

		hogmsec = YIELD_MSEC;
		if (hogmsec > remaining) {
			hogmsec = remaining;
		}

		hog_milliseconds(hogmsec);
		remaining -= hogmsec;

		/* Let other threads run */
		pthread_yield();
		show_cpu_conditional(caller, threadno);
	}
}

/****************************************************************************
 * Name: barrier_thread
 ****************************************************************************/

static pthread_addr_t barrier_thread(pthread_addr_t parameter)
{
	int threadno  = (int)((intptr_t)parameter);
	int ret;
	u32 counter = 0;

	printf("Thread[%d]: Started\n",  threadno);
	show_cpu("Thread", threadno);

	/* Hog some CPU time */
	while(1) {
		hog_time("Thread", threadno);
		printf("Counter value[%d] = %d\n", threadno, counter++);
	}
	

#if 0
	/* Wait at the barrier until all threads are synchronized. */

	printf("Thread[%d]: Calling pthread_barrier_wait()\n", threadno);
	fflush(stdout);
	show_cpu_conditional("Thread", threadno);

	ret = pthread_barrier_wait(&g_smp_barrier);
	if (ret == 0) {
		printf("Thread[%d]: Back with ret=0 (I am not special)\n", threadno);
	} else if (ret == PTHREAD_BARRIER_SERIAL_THREAD) {
		printf("Thread[%d]: Back with ret=PTHREAD_BARRIER_SERIAL_THREAD (I AM SPECIAL)\n" 
				, threadno);
	} else {
		printf("Thread[%d]: ERROR could not get semaphore value\n", threadno);
	}

	fflush(stdout);
	show_cpu_conditional("Thread", threadno);

	/* Hog some more CPU time */

	hog_time("Thread", threadno);

	/* Then exit */

	printf("Thread[%d]: Done\n",  threadno);
	fflush(stdout);
	show_cpu_conditional("Thread", threadno);
#endif
	return NULL;
}

static void set_affinity(void)
{
	cpu_set_t cpu_set;

	CPU_ZERO(&cpu_set);
	CPU_SET(affinity, &cpu_set);
	if (sched_setaffinity(0, sizeof(cpu_set_t), &cpu_set) != 0) {
		return;
	}
	affinity++;
	if (affinity == CONFIG_SMP_NCPUS) {
		affinity = 0;
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * smp_main
 ****************************************************************************/

int smp_main_pthread(int argc, FAR char *argv[])
{
	pthread_t threadid[SMP_NTHREADS];
	pthread_addr_t result;
	pthread_attr_t attr;
	pthread_barrierattr_t barrierattr;
	int errcode = EXIT_SUCCESS;
	int ret;
	int i;


	/* Initialize data */

	memset(threadid, 0, sizeof(pthread_t) * SMP_NTHREADS);
	for (i = 0; i <= SMP_NTHREADS; i++) {
		g_thread_cpu[i] = IMPOSSIBLE_CPU;
		if (i < SMP_NTHREADS) {
			threadid[i] = 0;
		}
	}

  
	show_cpu("  Main", 0);
	printf("  Main[0]: Initializing barrier\n");

	/* Create the barrier */

	ret = pthread_barrierattr_init(&barrierattr);
	if (ret != OK) {
		printf("  Main[0]: pthread_barrierattr_init failed, ret=%d\n", ret);

		errcode = EXIT_FAILURE;
		goto errout;
	}

	ret = pthread_barrier_init(&g_smp_barrier, &barrierattr, 
			SMP_NTHREADS);
	if (ret != OK) {
		printf("  Main[0]: pthread_barrier_init failed, ret=%d\n", ret);

		errcode = EXIT_FAILURE;
		goto errout_with_attr;
	}

	/* Start SMP_NTHREADS thread instances */

	ret = pthread_attr_init(&attr);
	if (ret != OK) {
		printf("  Main[0]: pthread_attr_init failed, ret=%d\n", ret);

		errcode = EXIT_FAILURE;
		goto errout_with_barrier;
	}


#ifdef CONFIG_AMP
	/* If AMP is enabled, then we will set the affinity of each thread / task
	 * to run on a different cpu.
	 */
	printf("Running SMP test with AMP configuration\n");
	uint32_t core_id = 1;
#endif

	for (i = 0; i < SMP_NTHREADS; i++) {

#ifdef CONFIG_AMP
		CPU_ZERO(&attr.affinity);
		CPU_SET(core_id, &attr.affinity);
		core_id++;
		if (core_id == CONFIG_SMP_NCPUS) {
			core_id = 0;
		}
#endif

		ret = pthread_create(&threadid[i], &attr, barrier_thread, 
				(pthread_addr_t)((uintptr_t)i + 1));
		if (ret != 0) {
			printf("  Main[0]: Error in thread %d create, ret=%d\n", 
					i + 1, ret);
			printf("  Main[0]: Test aborted with waiting threads\n");

			errcode = EXIT_FAILURE;
			break;
		} else {
			printf("  Main[0]: Thread %d created\n", i + 1);
		}

		show_cpu_conditional("  Main", 0);
	}

	fflush(stdout);
	show_cpu_conditional("  Main", 0);

 
	/* Wait for all thread instances to complete */

	for (i = 0; i < SMP_NTHREADS; i++) {
		if (threadid[i] != 0) {
			ret = pthread_join(threadid[i], &result);
			show_cpu_conditional("  Main", 0);

			if (ret != 0) {
				printf("  Main[0]: Error in thread %d join, ret=%d\n",
					i + 1, ret);
				errcode = EXIT_FAILURE;
           		} else {
				printf("  Main[0]: Thread %d completed with result=%p\n",
					i + 1, result);
			}
		}
	}

  /* Destroy the barrier */

errout_with_barrier:
	ret = pthread_barrier_destroy(&g_smp_barrier);
	if (ret != OK) {
		printf("  Main[0]: pthread_barrier_destroy failed, ret=%d\n", ret);
	}

errout_with_attr:
	ret = pthread_barrierattr_destroy(&barrierattr);
	if (ret != OK) {
		printf("  Main[0]: pthread_barrierattr_destroy failed, ret=%d\n",
             ret);
	}


errout:
	fflush(stdout);
	show_cpu_conditional("  Main", 0);
	return errcode;
}
/****************************************************************************
 * hello_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	printf("Hello, World!!\n");

	if (argc > 1) {
		printf("Run SMP PThread to block core1 and test\n");
		smp_main_pthread(argc, argv);
	}

	return 0;
}
