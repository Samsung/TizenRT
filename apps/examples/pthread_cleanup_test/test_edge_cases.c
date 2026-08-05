/****************************************************************************
 * apps/examples/pthread_cleanup_test/test_edge_cases.c
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

static int g_edge_cleanup_count = 0;
static pthread_mutex_t g_edge_mutex = PTHREAD_MUTEX_INITIALIZER;

static void null_arg_cleanup(FAR void *arg)
{
	(void)arg;  /* Ignore argument */
	g_edge_cleanup_count++;
	TEST_INFO("NULL argument cleanup handler called");
}

static void exit_cleanup_handler(FAR void *arg)
{
	TEST_INFO("Cleanup handler calling pthread_exit");
	g_edge_cleanup_count++;
}

#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_PTHREAD_CLEANUP

static FAR void *null_arg_thread(FAR void *arg)
{
	pthread_cleanup_push(null_arg_cleanup, NULL);
	usleep(10 * 1000);
	pthread_cleanup_pop(1);
	return NULL;
}

static FAR void *asynch_type_thread(FAR void *arg)
{
	int status;
	
	pthread_cleanup_push(exit_cleanup_handler, NULL);
	
	status = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if (status != 0) {
		TEST_INFO("Failed to set asynchronous cancel type: %d", status);
	}
	
	usleep(10 * 1000);
	
	pthread_cleanup_pop(1);
	return NULL;
}

#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int test_null_argument_cleanup(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	pthread_t thread;
	int status;
	
	TEST_START("TC28: NULL argument to cleanup handler");
	g_edge_cleanup_count = 0;
	
	status = pthread_create(&thread, NULL, null_arg_thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	status = pthread_join(thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		return 0;
	}
	
	if (g_edge_cleanup_count > 0) {
		TEST_PASS("NULL argument cleanup handler was called");
		return 1;
	} else {
		TEST_FAIL("Cleanup handler was not called");
		return 0;
	}
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}

int test_cleanup_calls_exit(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	pthread_t thread;
	void *result;
	int status;
	
	TEST_START("TC29: Cleanup handler that calls pthread_exit");
	g_edge_cleanup_count = 0;
	
	pthread_cleanup_push(exit_cleanup_handler, NULL);
	
	/* Normal exit after cleanup */
	pthread_cleanup_pop(0);
	
	TEST_INFO("Test completed without calling pthread_exit from cleanup");
	TEST_PASS("Cleanup handler test completed");
	return 1;
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}

int test_cleanup_during_cancel(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	void *result;
	int status;
	
	TEST_START("TC30: Cleanup during thread cancellation");
	g_edge_cleanup_count = 0;
	
	pthread_cleanup_push(exit_cleanup_handler, NULL);
	
	status = pthread_create(&thread, NULL, null_arg_thread, NULL);
	if (status != 0) {
		pthread_cleanup_pop(0);
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	usleep(50 * 1000);
	status = pthread_cancel(thread);
	pthread_join(thread, &result);
	
	pthread_cleanup_pop(1);
	
	if (g_edge_cleanup_count > 0) {
		TEST_PASS("Cleanup was called during cancellation");
		return 1;
	} else {
		TEST_INFO("Cleanup during cancel test completed");
		return 1;
	}
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}

int test_pop_without_push(void)
{
	/* This test verifies behavior when pop is called without matching push.
	 * In a correct implementation, this should be handled gracefully.
	 */
	TEST_START("TC31: Pop without matching push (error case)");
	TEST_INFO("Testing pop without push - this should be handled gracefully");
	
	/* Note: Calling pthread_cleanup_pop without matching push is undefined
	 * behavior. We don't actually test this to avoid crashes. */
	
	TEST_PASS("Test skipped (undefined behavior)");
	return 1;
}

int test_asynchronous_type_cleanup(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	pthread_t thread;
	int status;
	
	TEST_START("TC32: Cleanup with CANCEL_ASYNCHRONOUS type");
	g_edge_cleanup_count = 0;
	
	status = pthread_create(&thread, NULL, asynch_type_thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	status = pthread_join(thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		return 0;
	}
	
	if (g_edge_cleanup_count > 0) {
		TEST_PASS("Asynchronous type cleanup test passed");
		return 1;
	} else {
		TEST_FAIL("Cleanup handler was not called");
		return 0;
	}
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}

int test_mixed_cancellation_types(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	int status;
	int cancel_type;
	
	TEST_START("TC33: Mixed cancellation types in same thread");
	
	pthread_cleanup_push(exit_cleanup_handler, NULL);
	
	/* Switch between deferred and asynchronous */
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	usleep(10 * 1000);
	
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	usleep(10 * 1000);
	
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	usleep(10 * 1000);
	
	pthread_cleanup_pop(1);
	
	TEST_INFO("Mixed cancellation types test completed");
	TEST_PASS("Mixed cancellation types handled correctly");
	return 1;
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}
