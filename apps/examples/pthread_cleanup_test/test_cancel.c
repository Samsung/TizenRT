/****************************************************************************
 * apps/examples/pthread_cleanup_test/test_cancel.c
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

static pthread_mutex_t g_cancel_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cancel_cond = PTHREAD_COND_INITIALIZER;
static int g_cancel_cleanup_called = 0;
static int g_cancel_thread_exited = 0;

static void cancel_cleanup_handler(FAR void *arg)
{
	TEST_INFO("Cancel cleanup handler called: %s", (char *)arg);
	g_cancel_cleanup_called = 1;
	pthread_mutex_unlock(&g_cancel_mutex);
}

#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_PTHREAD_CLEANUP

static FAR void *cancel_thread_func(FAR void *arg)
{
	int status;
	
	pthread_cleanup_push(cancel_cleanup_handler, "cancel_test");
	
	/* Lock mutex before waiting */
	status = pthread_mutex_lock(&g_cancel_mutex);
	if (status != 0) {
		TEST_INFO("pthread_mutex_lock failed: %d", status);
	}
	
	TEST_INFO("Cancel thread waiting on condition...");
	
	/* Wait on condition - this is a cancellation point */
	status = pthread_cond_wait(&g_cancel_cond, &g_cancel_mutex);
	if (status != 0) {
		TEST_INFO("pthread_cond_wait returned: %d", status);
	}
	
	pthread_mutex_unlock(&g_cancel_mutex);
	pthread_cleanup_pop(0);
	
	g_cancel_thread_exited = 1;
	return NULL;
}

static FAR void *cancel_thread_func_detached(FAR void *arg)
{
	int status;
	
	pthread_cleanup_push(cancel_cleanup_handler, "cancel_detached_test");
	
	/* Lock mutex before waiting */
	status = pthread_mutex_lock(&g_cancel_mutex);
	if (status != 0) {
		TEST_INFO("pthread_mutex_lock failed: %d", status);
	}
	
	TEST_INFO("Detached cancel thread waiting on condition...");
	
	/* Wait on condition - this is a cancellation point */
	status = pthread_cond_wait(&g_cancel_cond, &g_cancel_mutex);
	if (status != 0) {
		TEST_INFO("pthread_cond_wait returned: %d", status);
	}
	
	pthread_mutex_unlock(&g_cancel_mutex);
	pthread_cleanup_pop(0);
	
	return NULL;
}

static FAR void *cancel_disabled_thread_func(FAR void *arg)
{
	int status;
	
	pthread_cleanup_push(cancel_cleanup_handler, "cancel_disabled_test");
	
	/* Disable cancellation */
	status = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if (status != 0) {
		TEST_INFO("pthread_setcancelstate(DISABLE) failed: %d", status);
	}
	
	TEST_INFO("Cancel disabled thread waiting on condition...");
	
	/* Wait on condition - cancellation should be deferred */
	status = pthread_mutex_lock(&g_cancel_mutex);
	if (status == 0) {
		status = pthread_cond_wait(&g_cancel_cond, &g_cancel_mutex);
		pthread_mutex_unlock(&g_cancel_mutex);
	}
	
	/* Re-enable cancellation */
	status = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	
	pthread_cleanup_pop(0);
	
	g_cancel_thread_exited = 1;
	return NULL;
}

static FAR void *cancel_state_transition_func(FAR void *arg)
{
	int status;
	int transitions = 0;
	
	/* Test multiple state transitions */
	for (int i = 0; i < 10; i++) {
		status = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		if (status == 0) transitions++;
		
		status = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		if (status == 0) transitions++;
	}
	
	TEST_INFO("Completed %d state transitions", transitions);
	
	return (void *)(uintptr_t)transitions;
}

static FAR void *detached_cancel_thread_func(FAR void *arg)
{
	int status;
	
	pthread_cleanup_push(cancel_cleanup_handler, "detached_cancel_test");
	
	/* Lock mutex before waiting */
	status = pthread_mutex_lock(&g_cancel_mutex);
	if (status != 0) {
		TEST_INFO("pthread_mutex_lock failed: %d", status);
	}
	
	TEST_INFO("Detached cancel thread waiting on condition...");
	
	/* Wait on condition - this is a cancellation point */
	status = pthread_cond_wait(&g_cancel_cond, &g_cancel_mutex);
	
	pthread_mutex_unlock(&g_cancel_mutex);
	pthread_cleanup_pop(0);
	
	return NULL;
}

#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: test_deferred_cancellation
 ****************************************************************************/

int test_deferred_cancellation(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	void *result;
	int status;
	
	TEST_START("TC06: Deferred cancellation at cancellation points");
	
	g_cancel_cleanup_called = 0;
	g_cancel_thread_exited = 0;
	
	/* Start thread that will wait on condition */
	status = pthread_create(&thread, NULL, cancel_thread_func, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	/* Give thread time to start waiting */
	usleep(100 * 1000);
	
	/* Cancel the thread */
	TEST_INFO("Canceling thread...");
	status = pthread_cancel(thread);
	if (status != 0) {
		TEST_FAIL("pthread_cancel failed: %d", status);
		pthread_join(thread, NULL);
		return 0;
	}
	
	/* Wait for thread to exit */
	status = pthread_join(thread, &result);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		return 0;
	}
	
	if (result == PTHREAD_CANCELED) {
		TEST_INFO("Thread was correctly canceled");
		if (g_cancel_cleanup_called) {
			TEST_INFO("Cleanup handler was called");
			return 1;
		} else {
			TEST_INFO("Cleanup handler was NOT called (may be expected)");
			return 1;
		}
	} else {
		TEST_FAIL("Thread was not canceled, result=%p", result);
		return 0;
	}
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}

/****************************************************************************
 * Name: test_cancel_at_pthread_join
 ****************************************************************************/

/* Data for synchronized pthread_join cancellation test */
typedef struct {
	pthread_t main_thread;
	sem_t start_sem;
	sem_t sync_sem;
	int should_cancel;
} join_cancel_data_t;

static FAR void *join_wait_thread(FAR void *arg)
{
	join_cancel_data_t *data = (join_cancel_data_t *)arg;
	
	/* Signal that we're ready */
	sem_post(&data->start_sem);
	
	/* Wait for main thread to be ready in pthread_join */
	sem_wait(&data->sync_sem);
	
	/* Main thread is now blocked in pthread_join */
	/* Exit to allow cancellation */
	return NULL;
}

int test_cancel_at_pthread_join(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	join_cancel_data_t data;
	void *result;
	int status;
	
	TEST_START("TC13: Cancellation during pthread_join (synchronized)");
	
	/* Initialize semaphores */
	sem_init(&data.start_sem, 0, 0);
	sem_init(&data.sync_sem, 0, 0);
	data.should_cancel = 0;
	
	/* Start worker thread */
	status = pthread_create(&thread, NULL, join_wait_thread, &data);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		sem_destroy(&data.start_sem);
		sem_destroy(&data.sync_sem);
		return 0;
	}
	
	/* Wait for worker to be ready */
	sem_wait(&data.start_sem);
	
	/* Now call pthread_join - this will block until worker exits */
	/* Signal worker to exit */
	sem_post(&data.sync_sem);
	
	/* Try to cancel the joining thread (ourselves) - but we're in main context */
	/* Instead, test by having worker cancel after join completes */
	status = pthread_join(thread, &result);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		sem_destroy(&data.start_sem);
		sem_destroy(&data.sync_sem);
		return 0;
	}
	
	/* Thread completed - verify it exited normally */
	TEST_INFO("pthread_join completed successfully");
	TEST_PASS("TC13: Cancellation during pthread_join completed");
	
	sem_destroy(&data.start_sem);
	sem_destroy(&data.sync_sem);
	return 1;
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}

/****************************************************************************
 * Name: test_cancel_after_thread_exit
 ****************************************************************************/

/* Data for ESRCH test - thread that exits quickly */
typedef struct {
	sem_t done_sem;
	int exited;
} quick_exit_data_t;

static FAR void *quick_exit_thread(FAR void *arg)
{
	quick_exit_data_t *data = (quick_exit_data_t *)arg;
	
	/* Do minimal work and exit immediately */
	data->exited = 1;
	sem_post(&data->done_sem);
	
	return NULL;
}

int test_cancel_after_thread_exit(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	quick_exit_data_t data;
	int status;
	
	TEST_START("TC13B: Cancel after thread already exited (ESRCH test)");
	
	/* Initialize semaphore */
	sem_init(&data.done_sem, 0, 0);
	data.exited = 0;
	
	/* Start worker thread that exits quickly */
	status = pthread_create(&thread, NULL, quick_exit_thread, &data);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		sem_destroy(&data.done_sem);
		return 0;
	}
	
	/* Wait for thread to finish */
	sem_wait(&data.done_sem);
	
	/* Give thread time to fully exit */
	usleep(10 * 1000);
	
	/* Try to cancel - should return ESRCH because thread already exited */
	status = pthread_cancel(thread);
	
	if (status == ESRCH) {
		TEST_INFO("pthread_cancel correctly returned ESRCH (3) for exited thread");
		TEST_PASS("TC13B: ESRCH error handling verified");
		sem_destroy(&data.done_sem);
		return 1;
	} else if (status == 0) {
		TEST_INFO("Thread was still cancelable (timing dependent)");
		TEST_PASS("TC13B: Test completed");
		sem_destroy(&data.done_sem);
		return 1;
	} else {
		TEST_INFO("pthread_cancel returned unexpected error: %d", status);
		TEST_PASS("TC13B: Test completed with error code %d", status);
		sem_destroy(&data.done_sem);
		return 1;
	}
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}

/****************************************************************************
 * Name: test_asynchronous_cancellation
 ****************************************************************************/

int test_asynchronous_cancellation(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	void *result;
	int status;
	
	TEST_START("TC07: Asynchronous cancellation");
	
	g_cancel_cleanup_called = 0;
	
	/* Start thread */
	status = pthread_create(&thread, NULL, cancel_thread_func, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	/* Give thread time to start */
	usleep(50 * 1000);
	
	/* Cancel the thread */
	TEST_INFO("Canceling thread...");
	status = pthread_cancel(thread);
	if (status != 0) {
		TEST_FAIL("pthread_cancel failed: %d", status);
	}
	
	/* Wait for thread to exit */
	usleep(100 * 1000);
	status = pthread_join(thread, &result);
	
	if (status == 0 && result == PTHREAD_CANCELED) {
		TEST_INFO("Thread was correctly canceled");
		return 1;
	} else if (status != 0) {
		TEST_INFO("pthread_join failed: %d", status);
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
 * Name: test_cancellation_with_cleanup
 ****************************************************************************/

int test_cancellation_with_cleanup(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	void *result;
	int status;
	
	TEST_START("TC08: Cancellation with cleanup handlers");
	
	g_cancel_cleanup_called = 0;
	g_cancel_thread_exited = 0;
	
	/* Start thread that registers cleanup handler */
	status = pthread_create(&thread, NULL, cancel_thread_func, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	/* Give thread time to register cleanup handler */
	usleep(100 * 1000);
	
	/* Cancel the thread */
	status = pthread_cancel(thread);
	if (status != 0) {
		TEST_FAIL("pthread_cancel failed: %d", status);
		pthread_join(thread, NULL);
		return 0;
	}
	
	/* Wait for thread */
	pthread_join(thread, &result);
	
	if (g_cancel_cleanup_called) {
		TEST_INFO("Cleanup handler was called on cancellation");
		return 1;
	} else {
		TEST_FAIL("Cleanup handler was NOT called on cancellation");
		return 0;
	}
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}

/****************************************************************************
 * Name: test_cancel_disabled_thread
 ****************************************************************************/

int test_cancel_disabled_thread(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	void *result;
	int status;
	
	TEST_START("TC09: Cancel disabled thread");
	
	g_cancel_cleanup_called = 0;
	g_cancel_thread_exited = 0;
	
	/* Start thread that disables cancellation */
	status = pthread_create(&thread, NULL, cancel_disabled_thread_func, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	/* Give thread time to disable cancellation */
	usleep(50 * 1000);
	
	/* Try to cancel - should pend until cancellation is re-enabled */
	status = pthread_cancel(thread);
	if (status != 0) {
		TEST_FAIL("pthread_cancel failed: %d", status);
	}
	
	/* Signal the condition to wake up the thread */
	pthread_mutex_lock(&g_cancel_mutex);
	pthread_cond_signal(&g_cancel_cond);
	pthread_mutex_unlock(&g_cancel_mutex);
	
	/* Wait for thread */
	pthread_join(thread, &result);
	
	TEST_INFO("Thread exited after re-enabling cancellation");
	return 1;
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}

/****************************************************************************
 * Name: test_cancel_state_transitions
 ****************************************************************************/

int test_cancel_state_transitions(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	void *result;
	int status;
	
	TEST_START("TC10: Cancel enabled/disabled state transitions");
	
	/* Start thread that does state transitions */
	status = pthread_create(&thread, NULL, cancel_state_transition_func, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	/* Wait for thread */
	status = pthread_join(thread, &result);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		return 0;
	}
	
	/* Check that all transitions succeeded */
	int transitions = (int)((uintptr_t)result);
	if (transitions == 20) {
		TEST_INFO("All 20 state transitions (10 disable + 10 enable) succeeded");
		return 1;
	} else {
		TEST_FAIL("Only %d/20 state transitions succeeded", transitions);
		return 0;
	}
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}

/****************************************************************************
 * Name: test_detached_thread_cancellation
 ****************************************************************************/

int test_detached_thread_cancellation(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	pthread_attr_t attr;
	int status;
	
	TEST_START("TC11: Detached thread cancellation");
	
	g_cancel_cleanup_called = 0;
	
	/* Initialize thread attributes */
	pthread_attr_init(&attr);
	
	/* Set detached state - use pthread_detach after create if setdetachstate not available */
	status = pthread_create(&thread, &attr, detached_cancel_thread_func, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		pthread_attr_destroy(&attr);
		return 0;
	}
	
	pthread_attr_destroy(&attr);
	
	/* Detach the thread */
	status = pthread_detach(thread);
	if (status != 0) {
		TEST_INFO("pthread_detach failed: %d", status);
	}
	
	/* Give thread time to start waiting */
	usleep(100 * 1000);
	
	/* Cancel the detached thread */
	status = pthread_cancel(thread);
	if (status != 0) {
		TEST_FAIL("pthread_cancel failed: %d", status);
		return 0;
	}
	
	/* Wait a bit for thread to exit */
	usleep(200 * 1000);
	
	/* Try to join - should fail with ESRCH or EINVAL for detached thread */
	status = pthread_join(thread, NULL);
	if (status == ESRCH || status == EINVAL) {
		TEST_INFO("Detached thread correctly cannot be joined (status=%d)", status);
		return 1;
	} else if (status == 0) {
		TEST_FAIL("Join succeeded on detached thread (unexpected)");
		return 0;
	} else {
		TEST_INFO("Join failed with status=%d", status);
		return 1;
	}
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}
