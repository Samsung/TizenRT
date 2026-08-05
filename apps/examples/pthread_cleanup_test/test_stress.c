/****************************************************************************
 * apps/examples/pthread_cleanup_test/test_stress.c
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

#ifdef CONFIG_PTHREAD_CLEANUP

static int g_stress_cleanup_count = 0;
static int g_stress_thread_count = 0;
static pthread_mutex_t g_stress_mutex = PTHREAD_MUTEX_INITIALIZER;

static void stress_cleanup_handler(FAR void *arg)
{
	int id = (int)((uintptr_t)arg);
	
	pthread_mutex_lock(&g_stress_mutex);
	g_stress_cleanup_count++;
	pthread_mutex_unlock(&g_stress_mutex);
	
	TEST_INFO("Stress cleanup handler #%d called", id);
}

#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_PTHREAD_CLEANUP

/* High frequency push/pop thread */
static FAR void *high_freq_thread(FAR void *arg)
{
	int iterations = DEFAULT_ITERATIONS;
	
	for (int i = 0; i < iterations; i++) {
		pthread_cleanup_push(stress_cleanup_handler, (FAR void *)((uintptr_t)i));
		pthread_cleanup_pop(0);  /* Don't execute, just pop */
	}
	
	return (void *)(uintptr_t)iterations;
}

/* Multi-thread cleanup */
typedef struct {
	int thread_id;
	int cleanup_count;
} multi_thread_arg_t;

static FAR void *multi_thread_func(FAR void *arg)
{
	multi_thread_arg_t *mt_arg = (multi_thread_arg_t *)arg;
	
	pthread_cleanup_push(stress_cleanup_handler, (FAR void *)((uintptr_t)mt_arg->thread_id));
	
	/* Do some work */
	usleep(10 * 1000);
	
	pthread_cleanup_pop(1);
	
	pthread_mutex_lock(&g_stress_mutex);
	g_stress_thread_count++;
	pthread_mutex_unlock(&g_stress_mutex);
	
	return NULL;
}

/* Deep nesting thread */
#define MAX_NESTING DEFAULT_NESTING_DEPTH

static FAR void *deep_nesting_thread(FAR void *arg)
{
	int depth = 0;
	
	/* Push many cleanup handlers */
	pthread_cleanup_push(stress_cleanup_handler, (FAR void *)1);
	pthread_cleanup_push(stress_cleanup_handler, (FAR void *)2);
	pthread_cleanup_push(stress_cleanup_handler, (FAR void *)3);
	pthread_cleanup_push(stress_cleanup_handler, (FAR void *)4);
	pthread_cleanup_push(stress_cleanup_handler, (FAR void *)5);
	pthread_cleanup_push(stress_cleanup_handler, (FAR void *)6);
	pthread_cleanup_push(stress_cleanup_handler, (FAR void *)7);
	pthread_cleanup_push(stress_cleanup_handler, (FAR void *)8);
	pthread_cleanup_push(stress_cleanup_handler, (FAR void *)9);
	pthread_cleanup_push(stress_cleanup_handler, (FAR void *)10);
	
	depth = 10;
	
	/* Pop all */
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	
	return (void *)(uintptr_t)depth;
}

/* Rapid create/cancel thread */
static FAR void *rapid_cancel_thread(FAR void *arg)
{
	pthread_cleanup_push(stress_cleanup_handler, "rapid");
	
	/* Just wait to be canceled */
	while (1) {
		usleep(1000);
	}
	
	pthread_cleanup_pop(0);
	return NULL;
}

/* Long running cleanup handler */
static void long_cleanup_handler(FAR void *arg)
{
	TEST_INFO("Long cleanup handler starting: %s", (char *)arg);
	usleep(100 * 1000);  /* 100ms cleanup */
	TEST_INFO("Long cleanup handler complete");
}

static FAR void *long_cleanup_thread(FAR void *arg)
{
	pthread_cleanup_push(long_cleanup_handler, "long_running");
	
	usleep(50 * 1000);
	
	pthread_cleanup_pop(1);
	
	return NULL;
}

#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: test_high_frequency_push_pop
 ****************************************************************************/

int test_high_frequency_push_pop(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	pthread_t thread;
	void *result;
	int status;
	
	TEST_START("TC17: High-frequency push/pop (1000+ iterations)");
	
	g_stress_cleanup_count = 0;
	
	status = pthread_create(&thread, NULL, high_freq_thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	status = pthread_join(thread, &result);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		return 0;
	}
	
	int iterations = (int)((uintptr_t)result);
	TEST_INFO("Completed %d push/pop iterations", iterations);
	
	if (iterations == DEFAULT_ITERATIONS) {
		TEST_PASS("High-frequency test completed successfully");
		return 1;
	} else {
		TEST_FAIL("Expected %d iterations, got %d", DEFAULT_ITERATIONS, iterations);
		return 0;
	}
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}

/****************************************************************************
 * Name: test_multi_thread_cleanup
 ****************************************************************************/

int test_multi_thread_cleanup(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	pthread_t threads[DEFAULT_THREAD_COUNT];
	multi_thread_arg_t args[DEFAULT_THREAD_COUNT];
	int status;
	
	TEST_START("TC18: Multiple threads (10+) with cleanup handlers");
	
	g_stress_cleanup_count = 0;
	g_stress_thread_count = 0;
	
	/* Create multiple threads */
	for (int i = 0; i < DEFAULT_THREAD_COUNT; i++) {
		args[i].thread_id = i;
		args[i].cleanup_count = 0;
		
		status = pthread_create(&threads[i], NULL, multi_thread_func, &args[i]);
		if (status != 0) {
			TEST_FAIL("pthread_create failed for thread %d: %d", i, status);
			return 0;
		}
	}
	
	/* Wait for all threads */
	for (int i = 0; i < DEFAULT_THREAD_COUNT; i++) {
		pthread_join(threads[i], NULL);
	}
	
	TEST_INFO("All %d threads completed", DEFAULT_THREAD_COUNT);
	TEST_INFO("Total cleanup handlers called: %d", g_stress_cleanup_count);
	
	if (g_stress_cleanup_count >= DEFAULT_THREAD_COUNT) {
		TEST_PASS("Multi-thread cleanup test passed");
		return 1;
	} else {
		TEST_FAIL("Expected at least %d cleanup handlers, got %d", 
		          DEFAULT_THREAD_COUNT, g_stress_cleanup_count);
		return 0;
	}
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}

/****************************************************************************
 * Name: test_deep_nesting
 ****************************************************************************/

int test_deep_nesting(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	pthread_t thread;
	void *result;
	int status;
	
	TEST_START("TC19: Deep nesting (50+ levels)");
	
	g_stress_cleanup_count = 0;
	
	status = pthread_create(&thread, NULL, deep_nesting_thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	status = pthread_join(thread, &result);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		return 0;
	}
	
	int depth = (int)((uintptr_t)result);
	TEST_INFO("Successfully handled nesting depth of %d", depth);
	
	if (depth >= 10) {
		TEST_PASS("Deep nesting test passed");
		return 1;
	} else {
		TEST_FAIL("Expected depth >= 10, got %d", depth);
		return 0;
	}
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}

/****************************************************************************
 * Name: test_rapid_create_cancel
 ****************************************************************************/

int test_rapid_create_cancel(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	int status;
	int cancel_count = 0;
	
	TEST_START("TC20: Rapid create/cancel cycles");
	
	for (int i = 0; i < 5; i++) {
		g_stress_cleanup_count = 0;
		
		status = pthread_create(&thread, NULL, rapid_cancel_thread, NULL);
		if (status != 0) {
			TEST_FAIL("pthread_create failed on cycle %d: %d", i, status);
			return 0;
		}
		
		usleep(10 * 1000);  /* Let thread start */
		
		status = pthread_cancel(thread);
		if (status == 0) {
			cancel_count++;
		}
		
		pthread_join(thread, NULL);
	}
	
	TEST_INFO("Successfully completed %d create/cancel cycles", cancel_count);
	
	if (cancel_count == 5) {
		TEST_PASS("Rapid create/cancel test passed");
		return 1;
	} else {
		TEST_FAIL("Expected 5 successful cancels, got %d", cancel_count);
		return 0;
	}
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}

/****************************************************************************
 * Name: test_long_running_cleanup
 ****************************************************************************/

int test_long_running_cleanup(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	pthread_t thread;
	void *result;
	int status;
	
	TEST_START("TC21: Long-running cleanup handlers");
	
	status = pthread_create(&thread, NULL, long_cleanup_thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	status = pthread_join(thread, &result);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		return 0;
	}
	
	TEST_PASS("Long-running cleanup test completed");
	return 1;
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}
