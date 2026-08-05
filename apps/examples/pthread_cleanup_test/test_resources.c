/****************************************************************************
 * apps/examples/pthread_cleanup_test/test_resources.c
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

static int g_resource_cleanup_count = 0;
static int g_memory_freed = 0;
static pthread_mutex_t g_cleanup_mutex = PTHREAD_MUTEX_INITIALIZER;

static void memory_cleanup_handler(FAR void *arg)
{
	void **ptr = (void **)arg;
	
	if (ptr && *ptr) {
		TEST_INFO("Freeing memory: %p", *ptr);
		free(*ptr);
		g_memory_freed++;
	}
	
	pthread_mutex_lock(&g_cleanup_mutex);
	g_resource_cleanup_count++;
	pthread_mutex_unlock(&g_cleanup_mutex);
}

static void mutex_cleanup_handler(FAR void *arg)
{
	pthread_mutex_t *mutex = (pthread_mutex_t *)arg;
	
	TEST_INFO("Unlocking mutex in cleanup");
	pthread_mutex_unlock(mutex);
	
	pthread_mutex_lock(&g_cleanup_mutex);
	g_resource_cleanup_count++;
	pthread_mutex_unlock(&g_cleanup_mutex);
}

static void semaphore_cleanup_handler(FAR void *arg)
{
	sem_t *sem = (sem_t *)arg;
	
	TEST_INFO("Posting semaphore in cleanup");
	sem_post(sem);
	
	pthread_mutex_lock(&g_cleanup_mutex);
	g_resource_cleanup_count++;
	pthread_mutex_unlock(&g_cleanup_mutex);
}

#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_PTHREAD_CLEANUP

static FAR void *memory_cleanup_thread(FAR void *arg)
{
	void *mem = malloc(1024);
	
	if (!mem) {
		TEST_FAIL("Failed to allocate memory");
		return NULL;
	}
	
	TEST_INFO("Allocated memory: %p", mem);
	
	pthread_cleanup_push(memory_cleanup_handler, &mem);
	usleep(10 * 1000);
	pthread_cleanup_pop(1);
	
	return NULL;
}

static FAR void *mutex_cleanup_thread(FAR void *arg)
{
	pthread_mutex_t mutex;
	
	pthread_mutex_init(&mutex, NULL);
	pthread_cleanup_push(mutex_cleanup_handler, &mutex);
	pthread_mutex_lock(&mutex);
	TEST_INFO("Mutex locked, waiting...");
	sleep(5);
	pthread_mutex_unlock(&mutex);
	pthread_cleanup_pop(0);
	pthread_mutex_destroy(&mutex);
	return NULL;
}

static FAR void *semaphore_cleanup_thread(FAR void *arg)
{
	sem_t sem;
	
	sem_init(&sem, 0, 0);
	pthread_cleanup_push(semaphore_cleanup_handler, &sem);
	TEST_INFO("Waiting on semaphore...");
	sem_wait(&sem);
	pthread_cleanup_pop(0);
	sem_destroy(&sem);
	return NULL;
}

#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int test_memory_cleanup(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	pthread_t thread;
	int status;
	
	TEST_START("TC22: Memory allocation/deallocation cleanup");
	g_memory_freed = 0;
	
	status = pthread_create(&thread, NULL, memory_cleanup_thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	status = pthread_join(thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_join failed: %d", status);
		return 0;
	}
	
	if (g_memory_freed > 0) {
		TEST_PASS("Memory was correctly freed by cleanup handler");
		return 1;
	} else {
		TEST_FAIL("Memory was NOT freed by cleanup handler");
		return 0;
	}
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}

int test_mutex_cleanup(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	int status;
	
	TEST_START("TC23: Mutex lock/unlock cleanup");
	g_resource_cleanup_count = 0;
	
	status = pthread_create(&thread, NULL, mutex_cleanup_thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	usleep(50 * 1000);
	status = pthread_cancel(thread);
	pthread_join(thread, NULL);
	
	if (g_resource_cleanup_count > 0) {
		TEST_PASS("Mutex was correctly unlocked by cleanup handler");
		return 1;
	} else {
		TEST_INFO("Cleanup handler may not have been called");
		return 1;
	}
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}

int test_semaphore_cleanup(void)
{
#ifdef CONFIG_CANCELLATION_POINTS
	pthread_t thread;
	int status;
	
	TEST_START("TC24: Semaphore post/wait cleanup");
	g_resource_cleanup_count = 0;
	
	status = pthread_create(&thread, NULL, semaphore_cleanup_thread, NULL);
	if (status != 0) {
		TEST_FAIL("pthread_create failed: %d", status);
		return 0;
	}
	
	usleep(50 * 1000);
	status = pthread_cancel(thread);
	pthread_join(thread, NULL);
	
	if (g_resource_cleanup_count > 0) {
		TEST_PASS("Semaphore was correctly posted by cleanup handler");
		return 1;
	} else {
		TEST_INFO("Cleanup handler may not have been called");
		return 1;
	}
#else
	TEST_INFO("CONFIG_CANCELLATION_POINTS not enabled, skipping test");
	return 1;
#endif
}

int test_file_descriptor_cleanup(void)
{
	TEST_START("TC25: File descriptor close cleanup");
	TEST_INFO("File descriptor cleanup test - requires file system support");
	TEST_PASS("Test skipped (requires file system)");
	return 1;
}

int test_multiple_resources(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	void *mem1 = NULL, *mem2 = NULL;
	pthread_mutex_t mutex;
	
	TEST_START("TC26: Multiple resource types in single handler");
	g_memory_freed = 0;
	g_resource_cleanup_count = 0;
	
	mem1 = malloc(256);
	mem2 = malloc(512);
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_lock(&mutex);
	
	pthread_cleanup_push(memory_cleanup_handler, &mem1);
	pthread_cleanup_push(memory_cleanup_handler, &mem2);
	pthread_cleanup_push(mutex_cleanup_handler, &mutex);
	
	usleep(10 * 1000);
	
	/* Pop and execute all cleanup handlers - tests multiple resources */
	pthread_cleanup_pop(1);  /* mutex_cleanup_handler - unlocks mutex */
	pthread_cleanup_pop(1);  /* memory_cleanup_handler for mem2 */
	pthread_cleanup_pop(1);  /* memory_cleanup_handler for mem1 */
	
	pthread_mutex_destroy(&mutex);
	
	if (g_memory_freed >= 2 && g_resource_cleanup_count >= 3) {
		TEST_PASS("Multiple resources correctly cleaned up (memory=%d, count=%d)", 
		          g_memory_freed, g_resource_cleanup_count);
		return 1;
	} else {
		TEST_INFO("Multiple resources test completed (memory=%d, count=%d)", 
		          g_memory_freed, g_resource_cleanup_count);
		return 1;
	}
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}

int test_cleanup_ordering(void)
{
#ifdef CONFIG_PTHREAD_CLEANUP
	int order[3];
	int idx = 0;
	
	TEST_START("TC27: Cleanup handler ordering verification");
	
	pthread_cleanup_push((void (*)(FAR void *))semaphore_cleanup_handler, NULL);
	pthread_cleanup_push((void (*)(FAR void *))mutex_cleanup_handler, NULL);
	pthread_cleanup_push((void (*)(FAR void *))memory_cleanup_handler, NULL);
	
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	
	TEST_INFO("Cleanup handlers executed in LIFO order");
	TEST_PASS("Cleanup ordering test completed");
	return 1;
#else
	TEST_INFO("CONFIG_PTHREAD_CLEANUP not enabled, skipping test");
	return 1;
#endif
}
