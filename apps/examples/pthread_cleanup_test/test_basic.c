/****************************************************************************
 * apps/examples/pthread_cleanup_test/test_basic.c
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

#include "pthread_cleanup.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_PTHREAD_CLEANUP

static int g_cleanup_called = 0;
static int g_cleanup_count = 0;
static int g_cleanup_order[10];
static int g_order_index = 0;

static void cleanup_handler(FAR void *arg)
{
	int id = (int)((uintptr_t)arg);
	
	g_cleanup_called = 1;
	g_cleanup_order[g_order_index++] = id;
	TEST_INFO("Cleanup handler called with arg=%d", id);
}

static void multi_cleanup_handler(FAR void *arg)
{
	int id = (int)((uintptr_t)arg);
	
	g_cleanup_called = 1;
	g_cleanup_count++;
	g_cleanup_order[g_order_index++] = id;
	TEST_INFO("Multi cleanup handler #%d called", id);
}

#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: test_basic_cleanup_with_exit
 ****************************************************************************/

typedef struct {
	int cleanup_arg;
	int *called_flag;
} basic_test_arg_t;

static void basic_cleanup_handler(FAR void *arg)
{
	basic_test_arg_t *targ = (basic_test_arg_t *)arg;
	if (targ && targ->called_flag) {
		*(targ->called_flag) = 1;
		TEST_INFO("Cleanup handler called with arg=%d", targ->cleanup_arg);
	}
}

static FAR void *basic_exit_thread(FAR void *arg)
{
	basic_test_arg_t *targ = (basic_test_arg_t *)arg;
	
	pthread_cleanup_push(basic_cleanup_handler, targ);
	
	/* Exit via pthread_exit - cleanup should be called */
	pthread_exit((pthread_addr_t)(uintptr_t)42);
	
	pthread_cleanup_pop(0);  /* Never reached */
	return NULL;
}

int test_basic_cleanup_with_exit(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	pthread_t thread;
	basic_test_arg_t targ;
	int status;
	
	TEST_START("TC01: Single cleanup handler with pthread_exit");
	
	targ.cleanup_arg = 1;
	targ.called_flag = &g_cleanup_called;
	g_cleanup_called = 0;
	g_order_index = 0;
	
	/* Create a thread that registers a cleanup handler and exits */
	status = pthread_create(&thread, NULL, basic_exit_thread, &targ);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	status = pthread_join(thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		return 0;
	}
	
	if (g_cleanup_called) {
		TEST_INFO("Cleanup handler was called on pthread_exit");
		return 1;
	} else {
		TEST_FAIL("Cleanup handler was NOT called on pthread_exit");
		return 0;
	}
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}

typedef struct {
	int expected_count;
	int *count_ptr;
} multi_test_arg_t;

static FAR void *multi_cleanup_thread(FAR void *arg)
{
	multi_test_arg_t *targ = (multi_test_arg_t *)arg;
	int i;
	
	/* Register multiple cleanup handlers */
	for (i = 1; i <= targ->expected_count; i++) {
		pthread_cleanup_push(multi_cleanup_handler, (FAR void *)((uintptr_t)i));
	}
	
	/* Pop and execute all handlers */
	for (i = targ->expected_count; i >= 1; i--) {
		pthread_cleanup_pop(1);
	}
	
	return NULL;
}

int test_multiple_cleanup_handlers(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	pthread_t thread;
	multi_test_arg_t targ;
	int status;
	
	TEST_START("TC02: Multiple cleanup handlers with pthread_exit");
	
	targ.expected_count = 5;
	targ.count_ptr = &g_cleanup_count;
	g_cleanup_count = 0;
	g_order_index = 0;
	
	/* Create thread that registers and pops cleanup handlers */
	status = pthread_create(&thread, NULL, multi_cleanup_thread, &targ);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	status = pthread_join(thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		return 0;
	}
	
	if (g_cleanup_count == 5) {
		TEST_INFO("All 5 cleanup handlers were called");
		return 1;
	} else {
		TEST_FAIL("Expected 5 cleanup handlers, got %d", g_cleanup_count);
		return 0;
	}
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}

typedef struct {
	int execute_flag;
	int *called_ptr;
} pop_test_arg_t;

static FAR void *pop_execute_thread(FAR void *arg)
{
	pop_test_arg_t *targ = (pop_test_arg_t *)arg;
	
	pthread_cleanup_push(multi_cleanup_handler, (FAR void *)1);
	pthread_cleanup_pop(targ->execute_flag);
	
	return NULL;
}

int test_cleanup_pop_execute(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	pthread_t thread;
	pop_test_arg_t targ;
	int status;
	
	TEST_START("TC03: Cleanup handler with pthread_cleanup_pop(execute=1)");
	
	targ.execute_flag = 1;
	targ.called_ptr = &g_cleanup_called;
	g_cleanup_called = 0;
	g_order_index = 0;
	
	/* Create thread that pops with execute=1 */
	status = pthread_create(&thread, NULL, pop_execute_thread, &targ);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	status = pthread_join(thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		return 0;
	}
	
	if (g_cleanup_called) {
		TEST_INFO("Cleanup handler was executed with pop(1)");
		return 1;
	} else {
		TEST_FAIL("Cleanup handler was NOT executed with pop(1)");
		return 0;
	}
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}

int test_cleanup_pop_no_execute(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	pthread_t thread;
	pop_test_arg_t targ;
	int status;
	
	TEST_START("TC04: Cleanup handler with pthread_cleanup_pop(execute=0)");
	
	targ.execute_flag = 0;
	targ.called_ptr = &g_cleanup_called;
	g_cleanup_called = 0;
	g_order_index = 0;
	
	/* Create thread that pops with execute=0 */
	status = pthread_create(&thread, NULL, pop_execute_thread, &targ);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	status = pthread_join(thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		return 0;
	}
	
	if (!g_cleanup_called) {
		TEST_INFO("Cleanup handler was correctly NOT executed with pop(0)");
		return 1;
	} else {
		TEST_FAIL("Cleanup handler was executed with pop(0), expected no execution");
		return 0;
	}
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}

typedef struct {
	int depth;
	int *count_ptr;
	int *order_ptr;
	int *order_index_ptr;
} nested_test_arg_t;

static FAR void *nested_cleanup_thread(FAR void *arg)
{
	nested_test_arg_t *targ = (nested_test_arg_t *)arg;
	
	/* Test nested push/pop pairs - proper pairing */
	pthread_cleanup_push(multi_cleanup_handler, (FAR void *)1);
	{
		pthread_cleanup_push(multi_cleanup_handler, (FAR void *)2);
		{
			pthread_cleanup_push(multi_cleanup_handler, (FAR void *)3);
			
			/* Pop in reverse order */
			pthread_cleanup_pop(1);  /* Execute handler 3 */
		}
		pthread_cleanup_pop(1);  /* Execute handler 2 */
	}
	pthread_cleanup_pop(1);  /* Execute handler 1 */
	
	return NULL;
}

int test_nested_push_pop(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	pthread_t thread;
	nested_test_arg_t targ;
	int status;
	
	TEST_START("TC05: Nested push/pop pairs");
	
	g_cleanup_count = 0;
	g_order_index = 0;
	
	/* Create thread that does nested push/pop */
	status = pthread_create(&thread, NULL, nested_cleanup_thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	status = pthread_join(thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		return 0;
	}
	
	if (g_cleanup_count == 3) {
		TEST_INFO("All 3 nested cleanup handlers were called in correct order");
		
		/* Verify LIFO order: 3, 2, 1 */
		if (g_cleanup_order[0] == 3 && g_cleanup_order[1] == 2 && g_cleanup_order[2] == 1) {
			TEST_INFO("Cleanup handlers executed in correct LIFO order: 3 -> 2 -> 1");
		} else {
			TEST_FAIL("Cleanup handlers NOT in LIFO order");
			return 0;
		}
		return 1;
	} else {
		TEST_FAIL("Expected 3 cleanup handlers, got %d", g_cleanup_count);
		return 0;
	}
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}
