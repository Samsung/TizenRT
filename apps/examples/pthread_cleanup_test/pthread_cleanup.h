/****************************************************************************
 * apps/examples/pthread_cleanup_test/pthread_cleanup.h
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
 *    distribution. 3. Neither the name Samsung Electronics nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
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

#ifndef __APPS_EXAMPLES_PTHREAD_CLEANUP_TEST_H
#define __APPS_EXAMPLES_PTHREAD_CLEANUP_TEST_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Test result macros */

#define TEST_PASS(fmt, ...) \
	printf("[PASS] " fmt "\n", ##__VA_ARGS__)

#define TEST_FAIL(fmt, ...) \
	printf("[FAIL] " fmt "\n", ##__VA_ARGS__)

#define TEST_INFO(fmt, ...) \
	printf("[INFO] " fmt "\n", ##__VA_ARGS__)

#define TEST_START(name) \
	printf("\n[TEST] %s\n", name); \
	printf("[INFO] Starting test...\n")

#define TEST_DONE(name, result) \
	if (result) { \
		TEST_PASS("%s completed successfully", name); \
	} else { \
		TEST_FAIL("%s failed", name); \
	}

/* Configuration defaults */

#ifndef CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_STACKSIZE
#define DEFAULT_STACKSIZE 8192
#else
#define DEFAULT_STACKSIZE CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_STACKSIZE
#endif

#ifndef CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_THREAD_COUNT
#define DEFAULT_THREAD_COUNT 10
#else
#define DEFAULT_THREAD_COUNT CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_THREAD_COUNT
#endif

#ifndef CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_NESTING_DEPTH
#define DEFAULT_NESTING_DEPTH 50
#else
#define DEFAULT_NESTING_DEPTH CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_NESTING_DEPTH
#endif

#ifndef CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_ITERATIONS
#define DEFAULT_ITERATIONS 1000
#else
#define DEFAULT_ITERATIONS CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_ITERATIONS
#endif

/* Test IDs */

typedef enum {
	/* Basic functionality tests */
	TC01_SINGLE_CLEANUP_WITH_EXIT = 1,
	TC02_MULTIPLE_CLEANUP_WITH_EXIT,
	TC03_CLEANUP_WITH_POP_EXECUTE,
	TC04_CLEANUP_WITH_POP_NO_EXECUTE,
	TC05_NESTED_PUSH_POP,
	
	/* Cancellation tests */
	TC06_DEFERRED_CANCELLATION,
	TC07_ASYNCHRONOUS_CANCELLATION,
	TC08_CANCELLATION_WITH_CLEANUP,
	TC09_CANCEL_DISABLED,
	TC10_CANCEL_STATE_TRANSITIONS,
	TC11_DETACHED_THREAD_CANCELLATION,
	
	/* Cancellation point tests */
	TC12_CANCEL_AT_COND_WAIT,
	TC13_CANCEL_AT_PTHREAD_JOIN,
	TC13B_CANCEL_AFTER_THREAD_EXIT,
	TC14_CANCEL_AT_SEM_WAIT,
	TC15_CANCEL_AT_SLEEP,
	TC16_CANCEL_AT_MULTIPLE_POINTS,
	
	/* Stress tests */
	TC17_HIGH_FREQUENCY_PUSH_POP,
	TC18_MULTI_THREAD_CLEANUP,
	TC19_DEEP_NESTING,
	TC20_RAPID_CREATE_CANCEL,
	TC21_LONG_RUNNING_CLEANUP,
	
	/* Resource cleanup tests */
	TC22_MEMORY_CLEANUP,
	TC23_MUTEX_CLEANUP,
	TC24_SEMAPHORE_CLEANUP,
	TC25_FILE_DESCRIPTOR_CLEANUP,
	TC26_MULTIPLE_RESOURCES,
	TC27_CLEANUP_ORDERING,
	
	/* Edge case tests */
	TC28_NULL_ARGUMENT,
	TC29_CLEANUP_CALLS_EXIT,
	TC30_CLEANUP_DURING_CANCEL,
	TC31_POP_WITHOUT_PUSH,
	TC32_ASYNCHRONOUS_TYPE_CLEANUP,
	TC33_MIXED_CANCELLATION_TYPES
} test_id_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* Test functions */

int test_basic_cleanup_with_exit(void);
int test_multiple_cleanup_handlers(void);
int test_cleanup_pop_execute(void);
int test_cleanup_pop_no_execute(void);
int test_nested_push_pop(void);

int test_deferred_cancellation(void);
int test_asynchronous_cancellation(void);
int test_cancellation_with_cleanup(void);
int test_cancel_disabled_thread(void);
int test_cancel_state_transitions(void);
int test_detached_thread_cancellation(void);

int test_cancel_at_cond_wait(void);
int test_cancel_at_pthread_join(void);
int test_cancel_after_thread_exit(void);
int test_cancel_at_sem_wait(void);
int test_cancel_at_sleep(void);
int test_cancel_at_multiple_points(void);

int test_high_frequency_push_pop(void);
int test_multi_thread_cleanup(void);
int test_deep_nesting(void);
int test_rapid_create_cancel(void);
int test_long_running_cleanup(void);

int test_memory_cleanup(void);
int test_mutex_cleanup(void);
int test_semaphore_cleanup(void);
int test_file_descriptor_cleanup(void);
int test_multiple_resources(void);
int test_cleanup_ordering(void);

int test_null_argument_cleanup(void);
int test_cleanup_calls_exit(void);
int test_cleanup_during_cancel(void);
int test_pop_without_push(void);
int test_asynchronous_type_cleanup(void);
int test_mixed_cancellation_types(void);

/* Test runner */

void run_test(test_id_t id);
void run_all_tests(void);
void run_test_category(int category);
void show_test_menu(void);

/* Main entry point */

int pthread_cleanup_main(int argc, char *argv[]);

#endif /* __APPS_EXAMPLES_PTHREAD_CLEANUP_TEST_H */
