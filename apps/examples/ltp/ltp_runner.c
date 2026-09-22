/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * apps/examples/ltp/ltp_runner.c
 *
 * LTP Test Runner for TizenRT
 *
 * Provides a TASH command "ltprun" to execute LTP tests in three modes:
 *   1. Run all tests sequentially
 *   2. Run tests grouped by category (scheduler, pthread, signal, etc.)
 *   3. Run a single test by name or index
 *
 * Supports repeat execution via -r <count> (0 = infinite).
 * Supports per-test timeout via -t <sec> (0 = no timeout).
 * Supports passing command-line arguments to a single test via -a "<args>".
 *
 * Usage:
 *   ltprun all [-r <count>] [-t <sec>]              Run all tests
 *   ltprun category <name> [-r <count>] [-t <sec>]  Run all tests in a category
 *   ltprun test <ltp_tN|N> [-r <count>] [-t <sec>] [-a "<args>"]  Run a single test
 *   ltprun list [category]                          List all tests with categories
 *   ltprun categories                               List all available categories
 *   ltprun stop                                     Stop running test execution
 *   ltprun help                                     Show this help message
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include <sched.h>

#include "ltp_test_registry.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define LTP_TEST_PRIORITY   100   /* Same priority as TASH commands */
#define LTP_TEST_STACKSIZE  8192  /* Same stack size as individual TASH tests */
#define LTP_RUNNER_PRIORITY 90    /* Lower priority than test tasks */

/* LTP test result codes (from posixtest.h) */
#define PTS_PASS        0
#define PTS_FAIL        1
#define PTS_UNRESOLVED  2

#define LTP_REPEAT_DEFAULT  1
#define LTP_REPEAT_INFINITE 0

/* Skip list: allows skipping specific tests by number (1-based).
 * Uses a boolean mask indexed by test number for O(1) lookup.
 * Populated by -S/--skip command-line option.
 */
#define LTP_SKIP_DELIMS ",;"

/* Default per-test timeout in seconds, used when -t is not specified.
 * Configurable via Kconfig (CONFIG_EXAMPLES_LTP_DEFAULT_TIMEOUT).
 * Stress tests get a longer default since they are designed to run
 * continuously until signaled.
 */
#define LTP_DEFAULT_TIMEOUT_STRESS  CONFIG_EXAMPLES_LTP_DEFAULT_TIMEOUT
#define LTP_DEFAULT_TIMEOUT_NORMAL  (CONFIG_EXAMPLES_LTP_DEFAULT_TIMEOUT / 2)
#define LTP_TIMEOUT_NONE             0   /* No timeout - wait forever */
#define LTP_TIMEOUT_UNSPECIFIED     -1   /* Use category-based default */

/* Signal sent to test tasks to request graceful shutdown.
 * All LTP stress tests register a handler for SIGUSR1 that sets
 * their do_it flag to 0, causing them to exit their main loop.
 */
#define LTP_TIMEOUT_SIGNAL  SIGUSR1

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* Context for the timeout thread */
typedef struct {
	pid_t target_pid;
	int delay_sec;
} timeout_info_t;

typedef enum {
	LTP_RESULT_PASS = 0,
	LTP_RESULT_FAIL = 1,
	LTP_RESULT_UNKNOWN = 2,
	LTP_RESULT_TIMEOUT = 3,
} ltp_result_e;

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Flag set by 'ltprun stop' command to request graceful shutdown.
 * Checked in the test execution loops to break out of infinite/repeat mode.
 * Can be set from another TASH session while ltprun is running.
 */
static volatile int g_stop_requested = 0;

/* Skip list mask: indexed by test number (1-based).
 * Set to true for tests that should be skipped via -S/--skip option.
 * Size is LTP_TEST_COUNT + 1 so we can index by 1-based test number.
 */
static bool g_skip_mask[LTP_TEST_COUNT + 1];
static int g_skip_count;	/* Number of tests marked for skipping */

/****************************************************************************
 * Private Functions
 ****************************************************************************/


/****************************************************************************
 * Name: ltp_print_separator
 ****************************************************************************/

static void ltp_print_separator(void)
{
	printf("========================================"
	       "========================================\n");
}

/****************************************************************************
 * Name: ltp_find_category_by_name
 *
 * Find category index by name. Returns -1 if not found.
 *
 ****************************************************************************/

static int ltp_find_category_by_name(FAR const char *name)
{
	int i;
	for (i = 0; i < LTP_CATEGORY_COUNT; i++) {
		if (g_ltp_categories[i] && strcmp(g_ltp_categories[i], name) == 0) {
			return i;
		}
	}
	return -1;
}

/****************************************************************************
 * Name: ltp_find_test_by_name
 *
 * Find test index by TASH command name (e.g., "ltp_t13").
 * Returns -1 if not found.
 *
 ****************************************************************************/

static int ltp_find_test_by_name(FAR const char *name)
{
	int i;
	for (i = 0; i < LTP_TEST_COUNT; i++) {
		if (strcmp(g_ltp_tests[i].test_name, name) == 0) {
			return i;
		}
	}
	return -1;
}

/****************************************************************************
 * Name: ltp_find_test_by_index
 *
 * Find test by numeric index (extracted from "ltp_tN" name).
 * Returns -1 if not found.
 *
 ****************************************************************************/

static int ltp_find_test_by_index(int index)
{
	if (index < 1 || index > LTP_TEST_COUNT) {
		return -1;
	}
	return index - 1;	/* Tests are 0-indexed in array, 1-indexed in names */
}

/****************************************************************************
 * Name: ltp_test_is_skipped
 *
 * Check if a test (by 0-based array index) should be skipped.
 * Returns true if the test is in the skip list, false otherwise.
 *
 ****************************************************************************/

static bool ltp_test_is_skipped(int test_idx)
{
	/* g_skip_mask is indexed by 1-based test number */
	return g_skip_mask[test_idx + 1];
}

/****************************************************************************
 * Name: ltp_timeout_thread
 *
 * Thread that sleeps for a specified duration and then sends SIGUSR1
 * to the target test task to request graceful shutdown.
 *
 * If the test task has already exited before the timeout expires,
 * kill(pid, 0) will return ERROR (ESRCH), and we skip sending the signal.
 * This prevents signaling a recycled PID.
 *
 ****************************************************************************/

static FAR void *ltp_timeout_thread(FAR void *arg)
{
	timeout_info_t *info = (timeout_info_t *)arg;

	sleep(info->delay_sec);

	/* Check if the target process still exists before sending the signal.
	 * kill(pid, 0) performs existence check without actually sending a signal.
	 * This prevents signaling a recycled PID if the original test task
	 * already exited before the timeout expired.
	 */
	if (kill(info->target_pid, 0) == 0) {
		/* Process still exists - send SIGUSR1 to request graceful shutdown */
		kill(info->target_pid, LTP_TIMEOUT_SIGNAL);
	}

	free(info);
	return NULL;
}

/****************************************************************************
 * Name: ltp_start_timeout
 *
 * Start a detached timeout thread that will send SIGUSR1 to the test task
 * after the specified number of seconds.  Returns 0 on failure.
 *
 ****************************************************************************/

static void ltp_start_timeout(pid_t test_pid, int timeout_sec)
{
	pthread_t tid;
	timeout_info_t *info;
	int ret;

	info = (timeout_info_t *)malloc(sizeof(timeout_info_t));
	if (info == NULL) {
		printf("[WARNING] Failed to allocate timeout info, no timeout\n");
		return;
	}

	info->target_pid = test_pid;
	info->delay_sec = timeout_sec;

	ret = pthread_create(&tid, NULL, ltp_timeout_thread, (void *)info);
	if (ret != 0) {
		printf("[WARNING] Failed to create timeout thread: %d\n", ret);
		free(info);
		return;
	}

	/* Detach so the thread cleans up automatically when it exits */
	pthread_detach(tid);
}

/****************************************************************************
 * Name: ltp_get_default_timeout
 *
 * Return the default timeout for a test based on its category.
 * Stress tests get a longer default since they run indefinitely.
 *
 ****************************************************************************/

static int ltp_get_default_timeout(int category_id)
{
	if (strcmp(g_ltp_categories[category_id], "stress") == 0) {
		return LTP_DEFAULT_TIMEOUT_STRESS;
	}
	return LTP_DEFAULT_TIMEOUT_NORMAL;
}

/****************************************************************************
 * Name: ltp_test_wrapper
 *
 * Wrapper entry point for task_create().  Receives the test index as a
 * string argument, looks up the test function, and calls it.
 * LTP tests frequently call exit(), so each test must run in its own task
 * to avoid killing the runner.
 *
 ****************************************************************************/

static int ltp_test_wrapper(int argc, FAR char *argv[])
{
	int test_idx;

	if (argc < 2) {
		return PTS_UNRESOLVED;
	}

	test_idx = atoi(argv[2]);
	if (test_idx < 0 || test_idx >= LTP_TEST_COUNT) {
		return PTS_UNRESOLVED;
	}

	/* If extra arguments were passed (argv[3]), forward them to the test.
	 * argv layout: {"ltp_test", idx_str, [test_args...], NULL}
	 * We pass {test_name, test_args..., NULL} to the test function.
	 */
	if (argc >= 3 && argv[3] != NULL) {
		/* Build a new argv for the test: {test_name, arg1, arg2, ..., NULL} */
		char *test_argv[8];
		int test_argc = 0;
		int i;

		test_argv[test_argc++] = (char *)g_ltp_tests[test_idx].test_name;
		for (i = 3; i < argc && argv[i] != NULL && test_argc < 7; i++) {
			test_argv[test_argc++] = argv[i];
		}
		test_argv[test_argc] = NULL;

		return g_ltp_tests[test_idx].test_func(test_argc, test_argv);
	}

	/* No extra arguments - call with just the test name */
	return g_ltp_tests[test_idx].test_func(1, (char *[])
										   {
										   (char *)g_ltp_tests[test_idx].
										   test_name, NULL}
	);
}


/****************************************************************************
 * Name: ltp_execute_test
 *
 * Execute a single test in a separate task so that exit() calls in the
 * test only kill the test task, not the runner.
 * Uses task_create() + waitpid() for isolation.
 *
 * If timeout_sec > 0, a detached timeout thread is started that will send
 * SIGUSR1 to the test task after timeout_sec seconds. This allows stress
 * tests (which run indefinitely) to be terminated gracefully.
 *
 ****************************************************************************/

static ltp_result_e ltp_execute_test(int test_idx, int timeout_sec, FAR const char *test_args)
{
	const ltp_test_entry_t *entry = &g_ltp_tests[test_idx];
	pid_t pid;
	int status;
	pid_t ret;

	printf("\n");
	ltp_print_separator();
	if (timeout_sec > 0) {
		printf("Running: %s (category: %s, timeout: %ds)\n",
		       entry->test_name, g_ltp_categories[entry->category_id], timeout_sec);
	} else {
		printf("Running: %s (category: %s, no timeout)\n",
		       entry->test_name, g_ltp_categories[entry->category_id]);
	}
	ltp_print_separator();

	/* Spawn the test in a separate task.
	 * Use static buffers for the index string and test args so they
	 * persist until the new task reads them. (Local stack variables
	 * would be destroyed before the new task starts, causing argv to
	 * contain garbage.)
	 */
	static char idx_str[8];
	static char args_buf[64];
	snprintf(idx_str, sizeof(idx_str), "%d", test_idx);

	/* Use provided test_args, or fall back to default_args from registry */
	const char *final_args = (test_args != NULL && test_args[0] != '\0')
	                         ? test_args
	                         : entry->default_args;

	if (final_args != NULL && final_args[0] != '\0') {
		/* Copy final_args to static buffer so it persists for the child task */
		strncpy(args_buf, final_args, sizeof(args_buf) - 1);
		args_buf[sizeof(args_buf) - 1] = '\0';
		pid = task_create("ltp_test", LTP_TEST_PRIORITY, LTP_TEST_STACKSIZE,
		                  ltp_test_wrapper, (FAR char * const[])
		                  {"ltp_test", idx_str, args_buf, NULL});
	} else {
		pid = task_create("ltp_test", LTP_TEST_PRIORITY, LTP_TEST_STACKSIZE,
		                  ltp_test_wrapper, (FAR char * const[])
		                  {"ltp_test", idx_str, NULL});
	}

	if (pid < 0) {
		printf("[ERROR] %s: task_create failed: %d\n", entry->test_name, pid);
		return LTP_RESULT_FAIL;
	}

	/* Start timeout thread if a timeout is specified AND the test uses
	 * SIGUSR1/SIGALRM for termination. Tests that don't register signal
	 * handlers should not receive SIGUSR1 as it may cause assertion
	 * failures or undefined behavior.
	 *
	 * The thread will send SIGUSR1 to the test task after timeout_sec
	 * seconds to request graceful shutdown. If the test exits before
	 * the timeout, the thread checks process existence via kill(pid, 0)
	 * and skips sending the signal.
	 */
	if (timeout_sec > 0 && entry->needs_signal) {
		ltp_start_timeout(pid, timeout_sec);
	}

	/* Wait for the test task to complete */
	ret = waitpid(pid, &status, 0);
	if (ret < 0) {
		printf("[ERROR] %s: waitpid failed\n", entry->test_name);
		return LTP_RESULT_FAIL;
	}

	/* Check exit status */
	if (WIFEXITED(status)) {
		int exit_code = WEXITSTATUS(status);
		if (exit_code == 0) {
			printf("[PASS] %s\n", entry->test_name);
			return LTP_RESULT_PASS;
		} else {
			printf("[FAIL] %s (exit code: %d)\n", entry->test_name, exit_code);
			return LTP_RESULT_FAIL;
		}
	}

	printf("[FAIL] %s (abnormal termination)\n", entry->test_name);
	return LTP_RESULT_FAIL;
}

/****************************************************************************
 * Name: ltp_print_summary
 *
 * Print summary of test results.
 *
 ****************************************************************************/

static void ltp_print_summary(int total_runs, int passed, int failed, int repeat_count)
{
	printf("\n");
	ltp_print_separator();
	printf("LTP Test Summary\n");
	ltp_print_separator();
	if (repeat_count == LTP_REPEAT_INFINITE) {
		printf("  Total Runs:  %d  (infinite mode)\n", total_runs);
	} else if (repeat_count > 1) {
		int unique_tests = total_runs / repeat_count;
		printf("  Total Runs:  %d  (%d tests x %d repeats)\n", total_runs, unique_tests, repeat_count);
	} else {
		printf("  Total:      %d\n", total_runs);
	}
	printf("  Passed:     %d\n", passed);
	printf("  Failed:     %d\n", failed);
	printf("  Skipped:    %d\n", total_runs - passed - failed);
	if (total_runs > 0) {
		printf("  Pass Rate:  %.2f%%\n", (double)passed * 100.0 / (double)total_runs);
	}
	ltp_print_separator();
}

/****************************************************************************
 * Name: ltp_run_all
 *
 * Run all tests in the registry, optionally repeating.
 * timeout_sec: LTP_TIMEOUT_UNSPECIFIED = use per-category default,
 *              0 = no timeout, >0 = timeout in seconds
 *
 ****************************************************************************/

static int ltp_run_all(int repeat_count, int timeout_sec)
{
	int i;
	int round;
	int passed = 0;
	int failed = 0;
	int skipped = 0;
	int total_runs = 0;
	int max_rounds = (repeat_count == LTP_REPEAT_INFINITE) ? -1 : repeat_count;


	if (repeat_count == LTP_REPEAT_INFINITE) {
		printf("LTP Runner: Running all %d tests (infinite mode)...\n", LTP_TEST_COUNT);
	} else if (repeat_count > 1) {
		printf("LTP Runner: Running all %d tests (%d repeats)...\n", LTP_TEST_COUNT, repeat_count);
	} else {
		printf("LTP Runner: Running all %d tests...\n", LTP_TEST_COUNT);
	}

	for (round = 0; max_rounds < 0 || round < max_rounds; round++) {
		if (g_stop_requested) {
			printf("\nStop requested, stopping after round %d...\n", round);
			break;
		}
		if (round > 0) {
			printf("\n--- Repeat Round %d ---\n", round + 1);
		}
		for (i = 0; i < LTP_TEST_COUNT; i++) {
			if (g_stop_requested) {
				printf("\nStop requested, stopping...\n");
				break;
			}
			if (ltp_test_is_skipped(i)) {
				printf("[SKIP] %s\n", g_ltp_tests[i].test_name);
				skipped++;
				continue;
			}
			int t = (timeout_sec == LTP_TIMEOUT_UNSPECIFIED) ?
				ltp_get_default_timeout(g_ltp_tests[i].category_id) : timeout_sec;
			ltp_result_e result = ltp_execute_test(i, t, NULL);
			total_runs++;
			if (result == LTP_RESULT_PASS) {
				passed++;
			} else {
				failed++;
			}
		}
		if (g_stop_requested) {
			break;
		}
	}

	total_runs += skipped;
	ltp_print_summary(total_runs, passed, failed, repeat_count);
	return failed > 0 ? 1 : 0;
}

/****************************************************************************
 * Name: ltp_run_category
 *
 * Run all tests in a given category, optionally repeating.
 * timeout_sec: LTP_TIMEOUT_UNSPECIFIED = use per-category default,
 *              0 = no timeout, >0 = timeout in seconds
 *
 ****************************************************************************/

static int ltp_run_category(FAR const char *cat_name, int repeat_count, int timeout_sec)
{
	int cat_id = ltp_find_category_by_name(cat_name);
	int i;
	int round;
	int matched = 0;
	int passed = 0;
	int failed = 0;
	int skipped = 0;
	int total_runs = 0;
	int max_rounds = (repeat_count == LTP_REPEAT_INFINITE) ? -1 : repeat_count;


	if (cat_id < 0) {
		printf("Error: category '%s' not found.\n", cat_name);
		printf("Use 'ltprun categories' to see available categories.\n");
		return -1;
	}

	/* Count matching tests */
	for (i = 0; i < LTP_TEST_COUNT; i++) {
		if (g_ltp_tests[i].category_id == cat_id) {
			matched++;
		}
	}

	if (matched == 0) {
		printf("No tests found in category '%s'.\n", cat_name);
		return 0;
	}

	if (repeat_count == LTP_REPEAT_INFINITE) {
		printf("LTP Runner: Running %d tests in category '%s' (infinite mode)...\n", matched, cat_name);
	} else if (repeat_count > 1) {
		printf("LTP Runner: Running %d tests in category '%s' (%d repeats)...\n", matched, cat_name, repeat_count);
	} else {
		printf("LTP Runner: Running %d tests in category '%s'...\n", matched, cat_name);
	}

	for (round = 0; max_rounds < 0 || round < max_rounds; round++) {
		if (g_stop_requested) {
			printf("\nStop requested, stopping after round %d...\n", round);
			break;
		}
		if (round > 0) {
			printf("\n--- Repeat Round %d ---\n", round + 1);
		}
		for (i = 0; i < LTP_TEST_COUNT; i++) {
			if (g_ltp_tests[i].category_id == cat_id) {
				if (g_stop_requested) {
					printf("\nStop requested, stopping...\n");
					break;
				}
				if (ltp_test_is_skipped(i)) {
					printf("[SKIP] %s\n", g_ltp_tests[i].test_name);
					skipped++;
					continue;
				}
				int t = (timeout_sec == LTP_TIMEOUT_UNSPECIFIED) ?
					ltp_get_default_timeout(cat_id) : timeout_sec;
				ltp_result_e result = ltp_execute_test(i, t, NULL);
				total_runs++;
				if (result == LTP_RESULT_PASS) {
					passed++;
				} else {
					failed++;
				}
			}
		}
		if (g_stop_requested) {
			break;
		}
	}

	total_runs += skipped;
	ltp_print_summary(total_runs, passed, failed, repeat_count);
	return failed > 0 ? 1 : 0;
}

/****************************************************************************
 * Name: ltp_run_single
 *
 * Run a single test by name (e.g., "ltp_t13"), optionally repeating.
 * timeout_sec: LTP_TIMEOUT_UNSPECIFIED = use per-category default,
 *              0 = no timeout, >0 = timeout in seconds
 *
 ****************************************************************************/

static int ltp_run_single(FAR const char *test_name, int repeat_count, int timeout_sec, FAR const char *test_args)
{
	int idx;
	int round;
	int passed = 0;
	int failed = 0;
	int total_runs = 0;
	int max_rounds = (repeat_count == LTP_REPEAT_INFINITE) ? -1 : repeat_count;

	/* Check if argument is numeric (index) or a name */
	if (test_name[0] >= '0' && test_name[0] <= '9') {
		idx = ltp_find_test_by_index(atoi(test_name));
	} else {
		idx = ltp_find_test_by_name(test_name);
	}

	if (idx < 0) {
		printf("Error: test '%s' not found.\n", test_name);
		printf("Use 'ltprun list' to see available tests.\n");
		return -1;
	}

	/* Check if the test is in the skip list */
/*	if (ltp_test_is_skipped(idx)) {
		printf("[SKIP] %s\n", g_ltp_tests[idx].test_name);
		return 0;
	}*/

	if (repeat_count == LTP_REPEAT_INFINITE) {

		printf("LTP Runner: Running %s (infinite mode)...\n", g_ltp_tests[idx].test_name);
	} else if (repeat_count > 1) {
		printf("LTP Runner: Running %s (%d repeats)...\n", g_ltp_tests[idx].test_name, repeat_count);
	}

	int t = (timeout_sec == LTP_TIMEOUT_UNSPECIFIED) ?
		ltp_get_default_timeout(g_ltp_tests[idx].category_id) : timeout_sec;

	for (round = 0; max_rounds < 0 || round < max_rounds; round++) {
		if (g_stop_requested) {
			printf("\nStop requested, stopping after round %d...\n", round);
			break;
		}
		if (round > 0) {
			printf("\n--- Repeat Round %d ---\n", round + 1);
		}
		ltp_result_e result = ltp_execute_test(idx, t, test_args);
		total_runs++;
		if (result == LTP_RESULT_PASS) {
			passed++;
		} else {
			failed++;
		}
		if (g_stop_requested) {
			break;
		}
	}

	ltp_print_summary(total_runs, passed, failed, repeat_count);
	return failed > 0 ? 1 : 0;
}

/****************************************************************************
 * Name: ltp_list_tests
 *
 * List all tests, optionally filtered by category.
 *
 ****************************************************************************/

static int ltp_list_tests(FAR const char *cat_name)
{
	int i;
	int cat_id = -1;
	int count = 0;

	if (cat_name) {
		cat_id = ltp_find_category_by_name(cat_name);
		if (cat_id < 0) {
			printf("Error: category '%s' not found.\n", cat_name);
			return -1;
		}
	}

	ltp_print_separator();
	if (cat_id >= 0) {
		printf("Tests in category '%s':\n", cat_name);
	} else {
		printf("All LTP Tests (%d total):\n", LTP_TEST_COUNT);
	}
	ltp_print_separator();

	for (i = 0; i < LTP_TEST_COUNT; i++) {
		if (cat_id >= 0 && g_ltp_tests[i].category_id != cat_id) {
			continue;
		}
		printf("  [%3d] %-12s  category: %s\n", i + 1, g_ltp_tests[i].test_name, g_ltp_categories[g_ltp_tests[i].category_id]);
		count++;
	}

	ltp_print_separator();
	printf("Total: %d test(s)\n", count);
	return 0;
}

/****************************************************************************
 * Name: ltp_list_categories
 *
 * List all available test categories with test counts.
 *
 ****************************************************************************/

static int ltp_list_categories(void)
{
	int i;
	int counts[LTP_CATEGORY_COUNT];

	memset(counts, 0, sizeof(counts));

	for (i = 0; i < LTP_TEST_COUNT; i++) {
		counts[g_ltp_tests[i].category_id]++;
	}

	ltp_print_separator();
	printf("LTP Test Categories:\n");
	ltp_print_separator();

	for (i = 0; i < LTP_CATEGORY_COUNT; i++) {
		printf("  %-30s  %d test(s)\n", g_ltp_categories[i], counts[i]);
	}

	ltp_print_separator();
	printf("Total: %d categories, %d tests\n", LTP_CATEGORY_COUNT, LTP_TEST_COUNT);
	return 0;
}

/****************************************************************************
 * Name: ltp_parse_repeat_arg
 *
 * Parse -r or --repeat argument from argv. Returns repeat count or -1 on error.
 *
 ****************************************************************************/

static int ltp_parse_repeat_arg(int argc, FAR char *argv[], int start_idx)
{
	int i;

	for (i = start_idx; i < argc; i++) {
		if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--repeat") == 0) {
			if (i + 1 >= argc) {
				printf("Error: %s requires a count argument\n", argv[i]);
				return -1;
			}
			int repeat = atoi(argv[i + 1]);
			if (repeat < 0) {
				printf("Error: repeat count must be >= 0 (0 = infinite)\n");
				return -1;
			}
			return repeat;
		}
	}

	return LTP_REPEAT_DEFAULT;
}

/****************************************************************************
 * Name: ltp_parse_timeout_arg
 *
 * Parse -t or --timeout argument from argv.
 * Returns: LTP_TIMEOUT_UNSPECIFIED if not found,
 *          LTP_TIMEOUT_NONE (0) if -t 0,
 *          positive value for timeout in seconds,
 *          -2 on error.
 *
 ****************************************************************************/

static int ltp_parse_timeout_arg(int argc, FAR char *argv[], int start_idx)
{
	int i;

	for (i = start_idx; i < argc; i++) {
		if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--timeout") == 0) {
			if (i + 1 >= argc) {
				printf("Error: %s requires a timeout value\n", argv[i]);
				return -2;
			}
			int timeout = atoi(argv[i + 1]);
			if (timeout < 0) {
				printf("Error: timeout must be >= 0 (0 = no timeout)\n");
				return -2;
			}
			return timeout;
		}
	}

	return LTP_TIMEOUT_UNSPECIFIED;
}

/****************************************************************************
 * Name: ltp_parse_args_arg
 *
 * Parse -a or --args argument from argv.
 * Copies the argument string into args_buf.
 * Returns 0 on success, -1 on error, 1 if not found (args_buf left empty).
 *
 ****************************************************************************/

static int ltp_parse_args_arg(int argc, FAR char *argv[], int start_idx,
                              FAR char *args_buf, int buf_size)
{
	int i;

	for (i = start_idx; i < argc; i++) {
		if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--args") == 0) {
			if (i + 1 >= argc) {
				printf("Error: %s requires an argument string\n", argv[i]);
				return -1;
			}
			strncpy(args_buf, argv[i + 1], buf_size - 1);
			args_buf[buf_size - 1] = '\0';
			return 0;
		}
	}

	args_buf[0] = '\0';
	return 1;  /* Not found */
}

/****************************************************************************
 * Name: ltp_parse_skip_arg
 *
 * Parse -S or --skip argument from argv.
 * Populates g_skip_mask[] with test numbers to skip.
 * Accepts comma and/or semicolon-separated test numbers (1-based).
 * Example: "1,5,10" or "1;5;10" or "1,5;10"
 *
 * Returns 0 on success, -1 on error, 1 if not found.
 *
 ****************************************************************************/

static int ltp_parse_skip_arg(int argc, FAR char *argv[], int start_idx)
{
	int i;
	char skip_buf[128];
	char *token;
	char *saveptr;

	for (i = start_idx; i < argc; i++) {
		if (strcmp(argv[i], "-S") == 0 || strcmp(argv[i], "--skip") == 0) {
			if (i + 1 >= argc) {
				printf("Error: %s requires a test number list\n", argv[i]);
				return -1;
			}

			/* Clear the skip mask before populating */
			memset(g_skip_mask, 0, sizeof(g_skip_mask));
			g_skip_count = 0;

			/* Copy to local buffer for strtok_r processing */
			strncpy(skip_buf, argv[i + 1], sizeof(skip_buf) - 1);
			skip_buf[sizeof(skip_buf) - 1] = '\0';

			/* Parse comma/semicolon-separated test numbers */
			token = strtok_r(skip_buf, LTP_SKIP_DELIMS, &saveptr);
			while (token != NULL) {
				int num = atoi(token);
				if (num < 1 || num > LTP_TEST_COUNT) {
					printf("Error: skip test number %d is out of range (1-%d)\n",
					       num, LTP_TEST_COUNT);
					return -1;
				}
				if (!g_skip_mask[num]) {
					g_skip_mask[num] = true;
					g_skip_count++;
				}
				token = strtok_r(NULL, LTP_SKIP_DELIMS, &saveptr);
			}

			if (g_skip_count > 0) {
				printf("Skip list: %d test(s) will be skipped\n", g_skip_count);
			}
			return 0;
		}
	}

	return 1;  /* Not found */
}

/****************************************************************************
 * Name: ltp_print_help
 *
 * Print usage help.
 *
 ****************************************************************************/


static void ltp_print_help(void)
{
	printf("\nLTP Test Runner for TizenRT\n");
	printf("\nUsage:\n");
	printf("  ltprun all [-r <count>] [-t <sec>]              Run all tests\n");
	printf("  ltprun category <name> [-r <count>] [-t <sec>]  Run all tests in a category\n");
	printf("  ltprun test <ltp_tN|N> [-r <count>] [-t <sec>] [-a <args>]  Run a single test\n");
	printf("  ltprun list [category]                          List all tests\n");
	printf("  ltprun categories                               List all categories\n");
	printf("  ltprun stop                                     Stop running test execution\n");
	printf("  ltprun help                                     Show this help\n");
	printf("\nOptions:\n");
	printf("  -r <count>, --repeat <count>   Repeat test execution\n");
	printf("                                  count=0 means infinite\n");
	printf("                                  Default: 1 (run once)\n");
	printf("  -t <sec>, --timeout <sec>      Per-test timeout in seconds\n");
	printf("                                  0 = no timeout (wait forever)\n");
	printf("                                  Default: %ds for stress, %ds for others\n",
	       LTP_DEFAULT_TIMEOUT_STRESS, LTP_DEFAULT_TIMEOUT_NORMAL);
	printf("                                  (configurable via Kconfig)\n");
	printf("  -a <args>, --args <args>       Pass command-line arguments to test\n");
	printf("                                  (only for 'test' command)\n");
	printf("                                  Default args for stress tests: %d threads\n",
	       CONFIG_EXAMPLES_LTP_DEFAULT_THREADS);
	printf("                                  (configurable via Kconfig)\n");
	printf("  -S <list>, --skip <list>       Skip specific tests by number\n");
	printf("                                  Comma/semicolon-separated (e.g., \"1,5,10\")\n");
	printf("                                  Works with 'all' and 'category' commands\n");
	printf("\n");
	printf("Use 'ltprun stop' to gracefully stop test execution.\n");

	printf("The current test will complete and a summary will be printed.\n");
	printf("\nCategories:\n");
	printf("  scheduler   - Scheduler API tests (sched_*, pthread_*sched*)\n");
	printf("  pthreads    - Pthread API tests (except sched)\n");
	printf("  signals     - Signal API tests (sig*, signal)\n");
	printf("  condvar     - Condition variable tests\n");
	printf("  stress      - Stress and concurrency tests\n");
	printf("  timers      - Timer/clock API tests (timer_*, clock_*, nanosleep)\n");
	printf("  mqueues     - Message queue API tests (mq_open, mq_send, mq_receive, etc.)\n");
	printf("  semaphores  - Semaphore API tests (sem_init, sem_post, sem_wait, etc.)\n");
	printf("\nExamples:\n");
	printf("  ltprun all                          # Run everything once\n");
	printf("  ltprun all -r 10                    # Run everything 10 times\n");
	printf("  ltprun all -r 0                     # Run continuously (infinite)\n");
	printf("  ltprun category stress              # Run stress tests (default 10s timeout)\n");
	printf("  ltprun category stress -t 30        # Run stress tests with 30s timeout\n");
	printf("  ltprun category stress -t 0         # Run stress tests with no timeout\n");
	printf("  ltprun category scheduler -r 5      # Run scheduler tests 5 times\n");
	printf("  ltprun test ltp_t13                 # Run test #13\n");
	printf("  ltprun test 13 --repeat 100         # Run test #13 100 times\n");
	printf("  ltprun test ltp_t702 -t 60          # Run pthread_cancel stress, 60s timeout\n");
	printf("  ltprun test ltp_t757 -a \"5\"          # Run multi_send_rev_1 with 5 threads\n");
	printf("  ltprun all -S \"5,10,15\"             # Run all tests, skip 5, 10, 15\n");
	printf("  ltprun all -S \"1;2;3\"              # Run all tests, skip 1, 2, 3\n");
	printf("  ltprun category stress -S \"701,702\" # Run stress tests, skip 701, 702\n");
	printf("  ltprun all -r 5 -S \"1,2,3\"         # Run all 5 times, skip 1, 2, 3\n");
	printf("\n");
}


/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: ltp_runner_main
 *
 * Entry point for the ltprun TASH command.
 *
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int ltp_runner_main(int argc, FAR char *argv[])
#endif
{
	int repeat_count;
	int timeout_sec;

	if (argc < 2) {
		ltp_print_help();
		return 0;
	}

	if (strcmp(argv[1], "stop") == 0) {
		if (g_stop_requested) {
			printf("ltprun: stop already requested\n");
		} else {
			g_stop_requested = 1;
			printf("ltprun: stop requested, will stop after current test\n");
		}
		return 0;
	}

	/* Clear stop flag and skip mask before starting new test execution */
	g_stop_requested = 0;
	memset(g_skip_mask, 0, sizeof(g_skip_mask));
	g_skip_count = 0;

	if (strcmp(argv[1], "all") == 0) {
		timeout_sec = ltp_parse_timeout_arg(argc, argv, 2);
		if (timeout_sec == -2) {
			return -1;
		}
		repeat_count = ltp_parse_repeat_arg(argc, argv, 2);
		if (repeat_count < 0) {
			return -1;
		}
		if (ltp_parse_skip_arg(argc, argv, 2) < 0) {
			return -1;
		}
		return ltp_run_all(repeat_count, timeout_sec);

	} else if (strcmp(argv[1], "category") == 0) {
		if (argc < 3) {
			printf("Error: category name required.\n");
			printf("Usage: ltprun category <name> [-r <count>] [-t <sec>]\n");
			return -1;
		}
		timeout_sec = ltp_parse_timeout_arg(argc, argv, 3);
		if (timeout_sec == -2) {
			return -1;
		}
		repeat_count = ltp_parse_repeat_arg(argc, argv, 3);
		if (repeat_count < 0) {
			return -1;
		}
		if (ltp_parse_skip_arg(argc, argv, 3) < 0) {
			return -1;
		}
		return ltp_run_category(argv[2], repeat_count, timeout_sec);

	} else if (strcmp(argv[1], "test") == 0) {
		char test_args[64] = "";
		if (argc < 3) {
			printf("Error: test name or index required.\n");
			printf("Usage: ltprun test <ltp_tN|N> [-r <count>] [-t <sec>] [-a <args>]\n");
			return -1;
		}
		timeout_sec = ltp_parse_timeout_arg(argc, argv, 3);
		if (timeout_sec == -2) {
			return -1;
		}
		repeat_count = ltp_parse_repeat_arg(argc, argv, 3);
		if (repeat_count < 0) {
			return -1;
		}
		if (ltp_parse_args_arg(argc, argv, 3, test_args, sizeof(test_args)) < 0) {
			return -1;
		}
		if (ltp_parse_skip_arg(argc, argv, 3) < 0) {
			return -1;
		}
		return ltp_run_single(argv[2], repeat_count, timeout_sec, test_args);

	} else if (strcmp(argv[1], "list") == 0) {
		if (argc >= 3) {
			return ltp_list_tests(argv[2]);
		}
		return ltp_list_tests(NULL);
	} else if (strcmp(argv[1], "categories") == 0) {
		return ltp_list_categories();
	} else if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-h") == 0) {
		ltp_print_help();
		return 0;
	} else {
		printf("Error: unknown command '%s'\n", argv[1]);
		ltp_print_help();
		return -1;
	}
}
