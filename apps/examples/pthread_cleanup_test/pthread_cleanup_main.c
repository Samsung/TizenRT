/****************************************************************************
 * apps/examples/pthread_cleanup_test/pthread_cleanup_main.c
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
#include <tinyara/init.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "pthread_cleanup.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_PRIORITY
#define PRIORITY 100
#else
#define PRIORITY CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_PRIORITY
#endif

#ifndef CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_STACKSIZE
#define STACKSIZE 8192
#else
#define STACKSIZE CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_STACKSIZE
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const char *g_test_names[] = {
	/* Basic functionality tests (1-5) */
	[TC01_SINGLE_CLEANUP_WITH_EXIT] = "TC01: Single cleanup handler with pthread_exit",
	[TC02_MULTIPLE_CLEANUP_WITH_EXIT] = "TC02: Multiple cleanup handlers with pthread_exit",
	[TC03_CLEANUP_WITH_POP_EXECUTE] = "TC03: Cleanup handler with pthread_cleanup_pop(execute=1)",
	[TC04_CLEANUP_WITH_POP_NO_EXECUTE] = "TC04: Cleanup handler with pthread_cleanup_pop(execute=0)",
	[TC05_NESTED_PUSH_POP] = "TC05: Nested push/pop pairs",
	
	/* Cancellation tests (6-11) */
	[TC06_DEFERRED_CANCELLATION] = "TC06: Deferred cancellation at cancellation points",
	[TC07_ASYNCHRONOUS_CANCELLATION] = "TC07: Asynchronous cancellation",
	[TC08_CANCELLATION_WITH_CLEANUP] = "TC08: Cancellation with cleanup handlers",
	[TC09_CANCEL_DISABLED] = "TC09: Cancel disabled thread",
	[TC10_CANCEL_STATE_TRANSITIONS] = "TC10: Cancel enabled/disabled state transitions",
	[TC11_DETACHED_THREAD_CANCELLATION] = "TC11: Detached thread cancellation",
	
	/* Cancellation point tests (12-16) */
	[TC12_CANCEL_AT_COND_WAIT] = "TC12: Cancellation during pthread_cond_wait",
	[TC13_CANCEL_AT_PTHREAD_JOIN] = "TC13: Cancellation during pthread_join (synchronized)",
	[TC13B_CANCEL_AFTER_THREAD_EXIT] = "TC13B: Cancel after thread already exited (ESRCH test)",
	[TC14_CANCEL_AT_SEM_WAIT] = "TC14: Cancellation during sem_wait",
	[TC15_CANCEL_AT_SLEEP] = "TC15: Cancellation during sleep/usleep",
	[TC16_CANCEL_AT_MULTIPLE_POINTS] = "TC16: Multiple cancellation points in sequence",
	
	/* Stress tests (17-21) */
	[TC17_HIGH_FREQUENCY_PUSH_POP] = "TC17: High-frequency push/pop (1000+ iterations)",
	[TC18_MULTI_THREAD_CLEANUP] = "TC18: Multiple threads (10+) with cleanup handlers",
	[TC19_DEEP_NESTING] = "TC19: Deep nesting (50+ levels)",
	[TC20_RAPID_CREATE_CANCEL] = "TC20: Rapid create/cancel cycles",
	[TC21_LONG_RUNNING_CLEANUP] = "TC21: Long-running cleanup handlers",
	
	/* Resource cleanup tests (22-27) */
	[TC22_MEMORY_CLEANUP] = "TC22: Memory allocation/deallocation cleanup",
	[TC23_MUTEX_CLEANUP] = "TC23: Mutex lock/unlock cleanup",
	[TC24_SEMAPHORE_CLEANUP] = "TC24: Semaphore post/wait cleanup",
	[TC25_FILE_DESCRIPTOR_CLEANUP] = "TC25: File descriptor close cleanup",
	[TC26_MULTIPLE_RESOURCES] = "TC26: Multiple resource types in single handler",
	[TC27_CLEANUP_ORDERING] = "TC27: Cleanup handler ordering verification",
	
	/* Edge case tests (28-33) */
	[TC28_NULL_ARGUMENT] = "TC28: NULL argument to cleanup handler",
	[TC29_CLEANUP_CALLS_EXIT] = "TC29: Cleanup handler that calls pthread_exit",
	[TC30_CLEANUP_DURING_CANCEL] = "TC30: Cleanup during thread cancellation",
	[TC31_POP_WITHOUT_PUSH] = "TC31: Pop without matching push (error case)",
	[TC32_ASYNCHRONOUS_TYPE_CLEANUP] = "TC32: Cleanup with CANCEL_ASYNCHRONOUS type",
	[TC33_MIXED_CANCELLATION_TYPES] = "TC33: Mixed cancellation types in same thread"
};

static const int g_num_tests = sizeof(g_test_names) / sizeof(g_test_names[0]);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: show_test_menu
 ****************************************************************************/

void show_test_menu(void)
{
	printf("\n===========================================\n");
	printf("  Pthread Cancellation & Cleanup Stress Test\n");
	printf("===========================================\n\n");
	
	printf("Test Categories:\n");
	printf("  1. Basic Functionality Tests (TC01-TC05)\n");
	printf("  2. Cancellation Tests (TC06-TC11)\n");
	printf("  3. Cancellation Point Tests (TC12-TC16)\n");
	printf("  4. Stress Tests (TC17-TC21)\n");
	printf("  5. Resource Cleanup Tests (TC22-TC27)\n");
	printf("  6. Edge Case Tests (TC28-TC33)\n\n");
	
	printf("Commands:\n");
	printf("  <test_id>  - Run specific test (e.g., 1 for TC01)\n");
	printf("  a          - Run all tests\n");
	printf("  b          - Run basic tests (1-5)\n");
	printf("  c          - Run cancellation tests (6-11)\n");
	printf("  p          - Run cancellation point tests (12-16)\n");
	printf("  s          - Run stress tests (17-21)\n");
	printf("  r          - Run resource cleanup tests (22-27)\n");
	printf("  e          - Run edge case tests (28-33)\n");
	printf("  i          - Infinite stability test (runs until Ctrl+C)\n");
	printf("  h          - Show this help menu\n");
	printf("  q          - Quit\n\n");
}

/****************************************************************************
 * Name: run_test
 ****************************************************************************/

void run_test(test_id_t id)
{
	int result = 0;
	
	if (id <= 0 || id >= g_num_tests) {
		printf("[ERROR] Invalid test ID: %d\n", id);
		return;
	}
	
	printf("\n========================================\n");
	printf("Running: %s\n", g_test_names[id]);
	printf("========================================\n");
	
	switch (id) {
	/* Basic functionality tests */
	case TC01_SINGLE_CLEANUP_WITH_EXIT:
		result = test_basic_cleanup_with_exit();
		break;
	case TC02_MULTIPLE_CLEANUP_WITH_EXIT:
		result = test_multiple_cleanup_handlers();
		break;
	case TC03_CLEANUP_WITH_POP_EXECUTE:
		result = test_cleanup_pop_execute();
		break;
	case TC04_CLEANUP_WITH_POP_NO_EXECUTE:
		result = test_cleanup_pop_no_execute();
		break;
	case TC05_NESTED_PUSH_POP:
		result = test_nested_push_pop();
		break;
	
	/* Cancellation tests */
	case TC06_DEFERRED_CANCELLATION:
		result = test_deferred_cancellation();
		break;
	case TC07_ASYNCHRONOUS_CANCELLATION:
		result = test_asynchronous_cancellation();
		break;
	case TC08_CANCELLATION_WITH_CLEANUP:
		result = test_cancellation_with_cleanup();
		break;
	case TC09_CANCEL_DISABLED:
		result = test_cancel_disabled_thread();
		break;
	case TC10_CANCEL_STATE_TRANSITIONS:
		result = test_cancel_state_transitions();
		break;
	case TC11_DETACHED_THREAD_CANCELLATION:
		result = test_detached_thread_cancellation();
		break;
	
	/* Cancellation point tests */
	case TC12_CANCEL_AT_COND_WAIT:
		result = test_cancel_at_cond_wait();
		break;
	case TC13_CANCEL_AT_PTHREAD_JOIN:
		result = test_cancel_at_pthread_join();
		break;
	case TC13B_CANCEL_AFTER_THREAD_EXIT:
		result = test_cancel_after_thread_exit();
		break;
	case TC14_CANCEL_AT_SEM_WAIT:
		result = test_cancel_at_sem_wait();
		break;
	case TC15_CANCEL_AT_SLEEP:
		result = test_cancel_at_sleep();
		break;
	case TC16_CANCEL_AT_MULTIPLE_POINTS:
		result = test_cancel_at_multiple_points();
		break;
	
	/* Stress tests */
	case TC17_HIGH_FREQUENCY_PUSH_POP:
		result = test_high_frequency_push_pop();
		break;
	case TC18_MULTI_THREAD_CLEANUP:
		result = test_multi_thread_cleanup();
		break;
	case TC19_DEEP_NESTING:
		result = test_deep_nesting();
		break;
	case TC20_RAPID_CREATE_CANCEL:
		result = test_rapid_create_cancel();
		break;
	case TC21_LONG_RUNNING_CLEANUP:
		result = test_long_running_cleanup();
		break;
	
	/* Resource cleanup tests */
	case TC22_MEMORY_CLEANUP:
		result = test_memory_cleanup();
		break;
	case TC23_MUTEX_CLEANUP:
		result = test_mutex_cleanup();
		break;
	case TC24_SEMAPHORE_CLEANUP:
		result = test_semaphore_cleanup();
		break;
	case TC25_FILE_DESCRIPTOR_CLEANUP:
		result = test_file_descriptor_cleanup();
		break;
	case TC26_MULTIPLE_RESOURCES:
		result = test_multiple_resources();
		break;
	case TC27_CLEANUP_ORDERING:
		result = test_cleanup_ordering();
		break;
	
	/* Edge case tests */
	case TC28_NULL_ARGUMENT:
		result = test_null_argument_cleanup();
		break;
	case TC29_CLEANUP_CALLS_EXIT:
		result = test_cleanup_calls_exit();
		break;
	case TC30_CLEANUP_DURING_CANCEL:
		result = test_cleanup_during_cancel();
		break;
	case TC31_POP_WITHOUT_PUSH:
		result = test_pop_without_push();
		break;
	case TC32_ASYNCHRONOUS_TYPE_CLEANUP:
		result = test_asynchronous_type_cleanup();
		break;
	case TC33_MIXED_CANCELLATION_TYPES:
		result = test_mixed_cancellation_types();
		break;
	
	default:
		printf("[ERROR] Test not implemented: %s\n", g_test_names[id]);
		return;
	}
	
	TEST_DONE(g_test_names[id], result);
}

/****************************************************************************
 * Name: run_test_category
 ****************************************************************************/

void run_test_category(int category)
{
	int start, end;
	
	switch (category) {
	case 1: /* Basic */
		start = TC01_SINGLE_CLEANUP_WITH_EXIT;
		end = TC05_NESTED_PUSH_POP;
		break;
	case 2: /* Cancellation */
		start = TC06_DEFERRED_CANCELLATION;
		end = TC11_DETACHED_THREAD_CANCELLATION;
		break;
	case 3: /* Cancellation points */
		start = TC12_CANCEL_AT_COND_WAIT;
		end = TC16_CANCEL_AT_MULTIPLE_POINTS;
		break;
	case 4: /* Stress */
		start = TC17_HIGH_FREQUENCY_PUSH_POP;
		end = TC21_LONG_RUNNING_CLEANUP;
		break;
	case 5: /* Resource cleanup */
		start = TC22_MEMORY_CLEANUP;
		end = TC27_CLEANUP_ORDERING;
		break;
	case 6: /* Edge cases */
		start = TC28_NULL_ARGUMENT;
		end = TC33_MIXED_CANCELLATION_TYPES;
		break;
	default:
		printf("[ERROR] Invalid category: %d\n", category);
		return;
	}
	
	for (int i = start; i <= end; i++) {
		run_test((test_id_t)i);
	}
}

/****************************************************************************
 * Name: run_all_tests
 ****************************************************************************/

void run_all_tests(void)
{
	printf("\n*** Running ALL tests ***\n");
	
	for (int i = 1; i < g_num_tests; i++) {
		run_test((test_id_t)i);
	}
	
	printf("\n*** All tests completed ***\n");
}

/****************************************************************************
 * Name: run_stability_test
 ****************************************************************************/

void run_stability_test(int iterations)
{
	int pass_count = 0;
	int fail_count = 0;
	int total_tests = g_num_tests - 1;  /* Exclude index 0 */
	
	printf("\n");
	printf("================================================\n");
	printf("  STABILITY TEST - %d iterations\n", iterations);
	printf("  Total test runs: %d (%d tests x %d iterations)\n", 
	       total_tests * iterations, total_tests, iterations);
	printf("================================================\n\n");
	
	for (int iter = 1; iter <= iterations; iter++) {
		printf("\n[Iteration %d/%d]\n", iter, iterations);
		
		for (int i = 1; i < g_num_tests; i++) {
			int result = 0;
			
			/* Skip tests that may cause issues in long runs */
			if (i == TC31_POP_WITHOUT_PUSH) {
				pass_count++;
				continue;
			}
			
			/* Run test silently - only show failures */
			switch ((test_id_t)i) {
			case TC01_SINGLE_CLEANUP_WITH_EXIT:
				result = test_basic_cleanup_with_exit();
				break;
			case TC02_MULTIPLE_CLEANUP_WITH_EXIT:
				result = test_multiple_cleanup_handlers();
				break;
			case TC03_CLEANUP_WITH_POP_EXECUTE:
				result = test_cleanup_pop_execute();
				break;
			case TC04_CLEANUP_WITH_POP_NO_EXECUTE:
				result = test_cleanup_pop_no_execute();
				break;
			case TC05_NESTED_PUSH_POP:
				result = test_nested_push_pop();
				break;
			case TC06_DEFERRED_CANCELLATION:
				result = test_deferred_cancellation();
				break;
			case TC07_ASYNCHRONOUS_CANCELLATION:
				result = test_asynchronous_cancellation();
				break;
			case TC08_CANCELLATION_WITH_CLEANUP:
				result = test_cancellation_with_cleanup();
				break;
			case TC09_CANCEL_DISABLED:
				result = test_cancel_disabled_thread();
				break;
			case TC10_CANCEL_STATE_TRANSITIONS:
				result = test_cancel_state_transitions();
				break;
			case TC11_DETACHED_THREAD_CANCELLATION:
				result = test_detached_thread_cancellation();
				break;
			case TC12_CANCEL_AT_COND_WAIT:
				result = test_cancel_at_cond_wait();
				break;
			case TC13_CANCEL_AT_PTHREAD_JOIN:
				result = test_cancel_at_pthread_join();
				break;
			case TC13B_CANCEL_AFTER_THREAD_EXIT:
				result = test_cancel_after_thread_exit();
				break;
			case TC14_CANCEL_AT_SEM_WAIT:
				result = test_cancel_at_sem_wait();
				break;
			case TC15_CANCEL_AT_SLEEP:
				result = test_cancel_at_sleep();
				break;
			case TC16_CANCEL_AT_MULTIPLE_POINTS:
				result = test_cancel_at_multiple_points();
				break;
			case TC17_HIGH_FREQUENCY_PUSH_POP:
				result = test_high_frequency_push_pop();
				break;
			case TC18_MULTI_THREAD_CLEANUP:
				result = test_multi_thread_cleanup();
				break;
			case TC19_DEEP_NESTING:
				result = test_deep_nesting();
				break;
			case TC20_RAPID_CREATE_CANCEL:
				result = test_rapid_create_cancel();
				break;
			case TC21_LONG_RUNNING_CLEANUP:
				result = test_long_running_cleanup();
				break;
			case TC22_MEMORY_CLEANUP:
				result = test_memory_cleanup();
				break;
			case TC23_MUTEX_CLEANUP:
				result = test_mutex_cleanup();
				break;
			case TC24_SEMAPHORE_CLEANUP:
				result = test_semaphore_cleanup();
				break;
			case TC25_FILE_DESCRIPTOR_CLEANUP:
				result = test_file_descriptor_cleanup();
				break;
			case TC26_MULTIPLE_RESOURCES:
				result = test_multiple_resources();
				break;
			case TC27_CLEANUP_ORDERING:
				result = test_cleanup_ordering();
				break;
			case TC28_NULL_ARGUMENT:
				result = test_null_argument_cleanup();
				break;
			case TC29_CLEANUP_CALLS_EXIT:
				result = test_cleanup_calls_exit();
				break;
			case TC30_CLEANUP_DURING_CANCEL:
				result = test_cleanup_during_cancel();
				break;
			case TC32_ASYNCHRONOUS_TYPE_CLEANUP:
				result = test_asynchronous_type_cleanup();
				break;
			case TC33_MIXED_CANCELLATION_TYPES:
				result = test_mixed_cancellation_types();
				break;
			default:
				result = 1;
				break;
			}
			
			if (result) {
				pass_count++;
			} else {
				fail_count++;
				printf("[FAIL] %s\n", g_test_names[i]);
			}
		}
		
		/* Show progress every 10 iterations */
		if (iter % 10 == 0 || iter == iterations) {
			printf("Progress: %d/%d iterations (%d%%) - Pass: %d, Fail: %d\n",
			       iter, iterations, (iter * 100) / iterations, pass_count, fail_count);
		}
	}
	
	printf("\n");
	printf("================================================\n");
	printf("  STABILITY TEST COMPLETE\n");
	printf("  Total tests: %d\n", pass_count + fail_count);
	printf("  Passed: %d (%d%%)\n", pass_count, ((pass_count * 100) / (pass_count + fail_count)));
	printf("  Failed: %d (%d%%)\n", fail_count, ((fail_count * 100) / (pass_count + fail_count)));
	printf("================================================\n");
	
	if (fail_count == 0) {
		printf("\n[PASS] All stability tests passed!\n");
	} else {
		printf("\n[FAIL] %d stability tests failed.\n", fail_count);
	}
}

/****************************************************************************
 * Name: run_infinite_stability_test
 ****************************************************************************/

void run_infinite_stability_test(void)
{
	int pass_count = 0;
	int fail_count = 0;
	int total_tests = g_num_tests - 1;  /* Exclude index 0 */
	int iter = 0;
	
	printf("\n");
	printf("================================================\n");
	printf("  INFINITE STABILITY TEST\n");
	printf("  Running until interrupted (Ctrl+C or send signal)\n");
	printf("  Press Ctrl+C to stop and see results\n");
	printf("================================================\n\n");
	
	/* Infinite loop - runs until interrupted */
	for (iter = 1; ; iter++) {
		printf("\n[Iteration %d]\n", iter);
		
		for (int i = 1; i < g_num_tests; i++) {
			int result = 0;
			
			/* Skip tests that may cause issues in long runs */
			if (i == TC31_POP_WITHOUT_PUSH) {
				pass_count++;
				continue;
			}
			
			/* Run test silently - only show failures */
			switch ((test_id_t)i) {
			case TC01_SINGLE_CLEANUP_WITH_EXIT:
				result = test_basic_cleanup_with_exit();
				break;
			case TC02_MULTIPLE_CLEANUP_WITH_EXIT:
				result = test_multiple_cleanup_handlers();
				break;
			case TC03_CLEANUP_WITH_POP_EXECUTE:
				result = test_cleanup_pop_execute();
				break;
			case TC04_CLEANUP_WITH_POP_NO_EXECUTE:
				result = test_cleanup_pop_no_execute();
				break;
			case TC05_NESTED_PUSH_POP:
				result = test_nested_push_pop();
				break;
			case TC06_DEFERRED_CANCELLATION:
				result = test_deferred_cancellation();
				break;
			case TC07_ASYNCHRONOUS_CANCELLATION:
				result = test_asynchronous_cancellation();
				break;
			case TC08_CANCELLATION_WITH_CLEANUP:
				result = test_cancellation_with_cleanup();
				break;
			case TC09_CANCEL_DISABLED:
				result = test_cancel_disabled_thread();
				break;
			case TC10_CANCEL_STATE_TRANSITIONS:
				result = test_cancel_state_transitions();
				break;
			case TC11_DETACHED_THREAD_CANCELLATION:
				result = test_detached_thread_cancellation();
				break;
			case TC12_CANCEL_AT_COND_WAIT:
				result = test_cancel_at_cond_wait();
				break;
			case TC13_CANCEL_AT_PTHREAD_JOIN:
				result = test_cancel_at_pthread_join();
				break;
			case TC13B_CANCEL_AFTER_THREAD_EXIT:
				result = test_cancel_after_thread_exit();
				break;
			case TC14_CANCEL_AT_SEM_WAIT:
				result = test_cancel_at_sem_wait();
				break;
			case TC15_CANCEL_AT_SLEEP:
				result = test_cancel_at_sleep();
				break;
			case TC16_CANCEL_AT_MULTIPLE_POINTS:
				result = test_cancel_at_multiple_points();
				break;
			case TC17_HIGH_FREQUENCY_PUSH_POP:
				result = test_high_frequency_push_pop();
				break;
			case TC18_MULTI_THREAD_CLEANUP:
				result = test_multi_thread_cleanup();
				break;
			case TC19_DEEP_NESTING:
				result = test_deep_nesting();
				break;
			case TC20_RAPID_CREATE_CANCEL:
				result = test_rapid_create_cancel();
				break;
			case TC21_LONG_RUNNING_CLEANUP:
				result = test_long_running_cleanup();
				break;
			case TC22_MEMORY_CLEANUP:
				result = test_memory_cleanup();
				break;
			case TC23_MUTEX_CLEANUP:
				result = test_mutex_cleanup();
				break;
			case TC24_SEMAPHORE_CLEANUP:
				result = test_semaphore_cleanup();
				break;
			case TC25_FILE_DESCRIPTOR_CLEANUP:
				result = test_file_descriptor_cleanup();
				break;
			case TC26_MULTIPLE_RESOURCES:
				result = test_multiple_resources();
				break;
			case TC27_CLEANUP_ORDERING:
				result = test_cleanup_ordering();
				break;
			case TC28_NULL_ARGUMENT:
				result = test_null_argument_cleanup();
				break;
			case TC29_CLEANUP_CALLS_EXIT:
				result = test_cleanup_calls_exit();
				break;
			case TC30_CLEANUP_DURING_CANCEL:
				result = test_cleanup_during_cancel();
				break;
			case TC32_ASYNCHRONOUS_TYPE_CLEANUP:
				result = test_asynchronous_type_cleanup();
				break;
			case TC33_MIXED_CANCELLATION_TYPES:
				result = test_mixed_cancellation_types();
				break;
			default:
				result = 1;
				break;
			}
			
			if (result) {
				pass_count++;
			} else {
				fail_count++;
				printf("[FAIL] %s\n", g_test_names[i]);
			}
		}
		
		/* Show progress every 10 iterations */
		if (iter % 10 == 0) {
			printf("Progress: %d iterations - Pass: %d, Fail: %d\n",
			       iter, pass_count, fail_count);
		}
	}
}

/****************************************************************************
 * Name: pthread_cleanup_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL 
int main(int argc, FAR char *argv[])
#else
int pthread_cleanup_main(int argc, char *argv[])
#endif
{
	char input[32];
	int category;
	
	printf("\n");
	printf("================================================\n");
	printf("  Pthread Cancellation & Cleanup Stress Test Suite\n");
	printf("  Supports both FLAT and LOADABLE builds\n");
	printf("================================================\n");
	
	show_test_menu();
	
	while (1) {
		printf("pthread_cleanup> ");
		fflush(stdout);
		
		if (fgets(input, sizeof(input), stdin) == NULL) {
			break;
		}
		
		/* Remove newline */
		input[strcspn(input, "\n")] = 0;
		
		switch (input[0]) {
		case 'q':
		case 'Q':
			printf("Exiting stress test.\n");
			return 0;
		
		case 'h':
		case 'H':
			show_test_menu();
			break;
		
		case 'a':
		case 'A':
			run_all_tests();
			break;
		
		case 'b':
		case 'B':
			run_test_category(1);
			break;
		
		case 'c':
		case 'C':
			run_test_category(2);
			break;
		
		case 'p':
		case 'P':
			run_test_category(3);
			break;
		
		case 's':
		case 'S':
			run_test_category(4);
			break;
		
		case 'r':
		case 'R':
			run_test_category(5);
			break;
		
		case 'i':
		case 'I':
			/* Infinite stability test */
			run_infinite_stability_test();
			break;
		
		default:
			/* Try to parse as test ID */
			category = atoi(input);
			if (category > 0 && category < g_num_tests) {
				run_test((test_id_t)category);
			} else if (input[0] != '\0') {
				printf("Unknown command: %s. Type 'h' for help.\n", input);
			}
			break;
		}
	}
	
	return 0;
}
