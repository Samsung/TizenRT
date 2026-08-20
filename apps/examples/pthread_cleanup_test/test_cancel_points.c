/****************************************************************************
 * apps/examples/pthread_cleanup_test/test_cancel_points.c
 *
 *   Copyright (C) 2026 Samsung Electronics. All rights reserved.
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
 * 3. Neither the name Samsung Electronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
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
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "pthread_cleanup.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_CANCELLATION_POINTS

static pthread_mutex_t g_cp_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cp_cond = PTHREAD_COND_INITIALIZER;
static sem_t g_cp_sem;
static int g_cp_cleanup_called = 0;

static void cp_cleanup_handler(FAR void *arg)
{
	TEST_INFO("Cancellation point cleanup: %s", (char *)arg);
	g_cp_cleanup_called = 1;
}

#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_CANCELLATION_POINTS

static FAR void *cond_wait_thread(FAR void *arg)
{
	pthread_cleanup_push(cp_cleanup_handler, "cond_wait");
	
	pthread_mutex_lock(&g_cp_mutex);
	pthread_cond_wait(&g_cp_cond, &g_cp_mutex);
	pthread_mutex_unlock(&g_cp_mutex);
	
	pthread_cleanup_pop(0);
	return NULL;
}

static FAR void *sem_wait_thread(FAR void *arg)
{
	pthread_cleanup_push(cp_cleanup_handler, "sem_wait");
	
	sem_wait(&g_cp_sem);
	
	pthread_cleanup_pop(0);
	return NULL;
}

static FAR void *sleep_thread(FAR void *arg)
{
	pthread_cleanup_push(cp_cleanup_handler, "sleep");
	
	sleep(10);  /* Long sleep to allow cancellation */
	
	pthread_cleanup_pop(0);
	return NULL;
}

static FAR void *join_thread_func(FAR void *arg)
{
	/* Just exit immediately */
	return NULL;
}

static FAR void *join_wait_thread(FAR void *arg)
{
	pthread_t target;
	
	pthread_cleanup_push(cp_cleanup_handler, "pthread_join");
	
	/* Create and join a thread */
	pthread_create(&target, NULL, join_thread_func, NULL);
	pthread_join(target, NULL);
	
	pthread_cleanup_pop(0);
	return NULL;
}

#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: test_cancel_at_cond_wait
 ****************************************************************************/

int test_cancel_at_cond_wait(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	void *result;
	int status;
	
	TEST_START("TC12: Cancellation during pthread_cond_wait");
	
	g_cp_cleanup_called = 0;
	
	status = pthread_create(&thread, NULL, cond_wait_thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	usleep(50 * 1000);  /* Let thread start waiting */
	
	status = pthread_cancel(thread);
	if (status != 0) {
		TEST_FAIL("pthread_cancel failed: %d", status);
		pthread_join(thread, NULL);
		return 0;
	}
	
	status = pthread_join(thread, &result);
	
	if (result == PTHREAD_CANCELED) {
		TEST_INFO("Thread canceled at pthread_cond_wait");
		return 1;
	} else {
		TEST_FAIL("Thread was not canceled");
		return 0;
	}
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}

/* Note: test_cancel_at_pthread_join() is now in test_cancel.c with proper synchronization */

/****************************************************************************
 * Name: test_cancel_at_sem_wait
 ****************************************************************************/

int test_cancel_at_sem_wait(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	void *result;
	int status;
	
	TEST_START("TC14: Cancellation during sem_wait");
	
	g_cp_cleanup_called = 0;
	sem_init(&g_cp_sem, 0, 0);  /* Initialize with 0 - will block */
	
	status = pthread_create(&thread, NULL, sem_wait_thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		sem_destroy(&g_cp_sem);
		return 0;
	}
	
	usleep(50 * 1000);
	
	status = pthread_cancel(thread);
	if (status != 0) {
		TEST_FAIL("pthread_cancel failed: %d", status);
	}
	
	status = pthread_join(thread, &result);
	sem_destroy(&g_cp_sem);
	
	if (result == PTHREAD_CANCELED) {
		TEST_INFO("Thread canceled at sem_wait");
		return 1;
	} else {
		TEST_FAIL("Thread was not canceled");
		return 0;
	}
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}

/****************************************************************************
 * Name: test_cancel_at_sleep
 ****************************************************************************/

int test_cancel_at_sleep(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	void *result;
	int status;
	
	TEST_START("TC15: Cancellation during sleep/usleep");
	
	g_cp_cleanup_called = 0;
	
	status = pthread_create(&thread, NULL, sleep_thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	usleep(100 * 1000);  /* Let thread start sleeping */
	
	status = pthread_cancel(thread);
	if (status != 0) {
		TEST_FAIL("pthread_cancel failed: %d", status);
	}
	
	status = pthread_join(thread, &result);
	
	if (result == PTHREAD_CANCELED) {
		TEST_INFO("Thread canceled during sleep");
		return 1;
	} else {
		TEST_FAIL("Thread was not canceled");
		return 0;
	}
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}

/****************************************************************************
 * Name: test_cancel_at_multiple_points
 ****************************************************************************/

int test_cancel_at_multiple_points(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	void *result;
	int status;
	int cancel_points_hit = 0;
	
	TEST_START("TC16: Multiple cancellation points in sequence");
	
	g_cp_cleanup_called = 0;
	
	/* Create thread that hits multiple cancellation points */
	pthread_cleanup_push(cp_cleanup_handler, "multi_point");
	
	status = pthread_create(&thread, NULL, sleep_thread, NULL);
	if (status != 0) {
		pthread_cleanup_pop(0);
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	usleep(50 * 1000);
	
	/* First cancel attempt */
	status = pthread_cancel(thread);
	if (status == 0) {
		cancel_points_hit++;
		TEST_INFO("First cancellation requested");
	}
	
	/* Second cancel attempt (should still work) */
	status = pthread_cancel(thread);
	if (status == 0) {
		cancel_points_hit++;
		TEST_INFO("Second cancellation requested");
	}
	
	status = pthread_join(thread, &result);
	pthread_cleanup_pop(0);
	
	if (result == PTHREAD_CANCELED) {
		TEST_INFO("Thread canceled after %d cancellation requests", cancel_points_hit);
		return 1;
	} else {
		TEST_FAIL("Thread was not canceled");
		return 0;
	}
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}
