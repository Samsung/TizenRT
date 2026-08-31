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
 *
 * Usage:
 *   ltprun all [-r <count>]              Run all tests (optionally repeated)
 *   ltprun category <name> [-r <count>]  Run all tests in a category
 *   ltprun test <ltp_tN|N> [-r <count>]  Run a single test by name or index
 *   ltprun list [category]               List all tests with categories
 *   ltprun categories                     List all available categories
 *   ltprun stop                           Stop running test execution
 *   ltprun help                           Show this help message

 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
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

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Flag set by 'ltprun stop' command to request graceful shutdown.
 * Checked in the test execution loops to break out of infinite/repeat mode.
 * Can be set from another TASH session while ltprun is running.
 */
static volatile int g_stop_requested = 0;

/****************************************************************************
 * Private Types
 ****************************************************************************/



typedef enum {
	LTP_RESULT_PASS = 0,
	LTP_RESULT_FAIL = 1,
	LTP_RESULT_UNKNOWN = 2,
	LTP_RESULT_TIMEOUT = 3,
} ltp_result_e;

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

	/* Call the actual test function */
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
 ****************************************************************************/

static ltp_result_e ltp_execute_test(int test_idx)
{
	const ltp_test_entry_t *entry = &g_ltp_tests[test_idx];
	pid_t pid;
	int status;
	pid_t ret;

	printf("\n");
	ltp_print_separator();
	printf("Running: %s (category: %s)\n", entry->test_name, g_ltp_categories[entry->category_id]);
	ltp_print_separator();

	/* Spawn the test in a separate task.
	 * Use a static buffer for the index string so it persists until the
	 * new task reads it. (Local stack variables would be destroyed before
	 * the new task starts, causing argv[1] to contain garbage.)
	 */
	static char idx_str[8];
	snprintf(idx_str, sizeof(idx_str), "%d", test_idx);

	pid = task_create("ltp_test", LTP_TEST_PRIORITY, LTP_TEST_STACKSIZE, ltp_test_wrapper, (FAR char * const[])
					  {
					  "ltp_test", idx_str, NULL}
	);


	if (pid < 0) {
		printf("[ERROR] %s: task_create failed: %d\n", entry->test_name, pid);
		return LTP_RESULT_FAIL;
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
 *
 ****************************************************************************/

static int ltp_run_all(int repeat_count)
{
	int i;
	int round;
	int passed = 0;
	int failed = 0;
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
			ltp_result_e result = ltp_execute_test(i);
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

	ltp_print_summary(total_runs, passed, failed, repeat_count);
	return failed > 0 ? 1 : 0;
}

/****************************************************************************
 * Name: ltp_run_category


 *
 * Run all tests in a given category, optionally repeating.
 *
 ****************************************************************************/

static int ltp_run_category(FAR const char *cat_name, int repeat_count)
{
	int cat_id = ltp_find_category_by_name(cat_name);
	int i;
	int round;
	int matched = 0;
	int passed = 0;
	int failed = 0;
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
				ltp_result_e result = ltp_execute_test(i);
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


	ltp_print_summary(total_runs, passed, failed, repeat_count);
	return failed > 0 ? 1 : 0;
}

/****************************************************************************
 * Name: ltp_run_single

 *
 * Run a single test by name (e.g., "ltp_t13"), optionally repeating.
 *
 ****************************************************************************/

static int ltp_run_single(FAR const char *test_name, int repeat_count)
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

	if (repeat_count == LTP_REPEAT_INFINITE) {
		printf("LTP Runner: Running %s (infinite mode)...\n", g_ltp_tests[idx].test_name);

	} else if (repeat_count > 1) {
		printf("LTP Runner: Running %s (%d repeats)...\n", g_ltp_tests[idx].test_name, repeat_count);
	}

	for (round = 0; max_rounds < 0 || round < max_rounds; round++) {
		if (g_stop_requested) {
			printf("\nStop requested, stopping after round %d...\n", round);
			break;
		}
		if (round > 0) {
			printf("\n--- Repeat Round %d ---\n", round + 1);
		}
		ltp_result_e result = ltp_execute_test(idx);
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
 * Name: ltp_print_help
 *
 * Print usage help.
 *
 ****************************************************************************/

static void ltp_print_help(void)
{
	printf("\nLTP Test Runner for TizenRT\n");
	printf("\nUsage:\n");
	printf("  ltprun all [-r <count>]              Run all tests\n");
	printf("  ltprun category <name> [-r <count>]  Run all tests in a category\n");
	printf("  ltprun test <ltp_tN|N> [-r <count>]  Run a single test by name or index\n");
	printf("  ltprun list [category]               List all tests\n");
	printf("  ltprun categories                     List all categories\n");
	printf("  ltprun stop                           Stop running test execution\n");
	printf("  ltprun help                           Show this help\n");
	printf("\nOptions:\n");
	printf("  -r <count>, --repeat <count>   Repeat test execution\n");
	printf("                                  count=0 means infinite\n");
	printf("                                  Default: 1 (run once)\n");
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

	printf("  ltprun category stress              # Run stress tests\n");
	printf("  ltprun category scheduler -r 5      # Run scheduler tests 5 times\n");
	printf("  ltprun test ltp_t13                 # Run test #13\n");
	printf("  ltprun test 13 --repeat 100         # Run test #13 100 times\n");
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

	/* Clear stop flag before starting new test execution */
	g_stop_requested = 0;

	if (strcmp(argv[1], "all") == 0) {
		repeat_count = ltp_parse_repeat_arg(argc, argv, 2);
		if (repeat_count < 0) {
			return -1;
		}
		return ltp_run_all(repeat_count);
	} else if (strcmp(argv[1], "category") == 0) {
		if (argc < 3) {
			printf("Error: category name required.\n");
			printf("Usage: ltprun category <name> [-r <count>]\n");
			return -1;
		}
		repeat_count = ltp_parse_repeat_arg(argc, argv, 3);
		if (repeat_count < 0) {
			return -1;
		}
		return ltp_run_category(argv[2], repeat_count);
	} else if (strcmp(argv[1], "test") == 0) {
		if (argc < 3) {
			printf("Error: test name or index required.\n");
			printf("Usage: ltprun test <ltp_tN|N> [-r <count>]\n");
			return -1;
		}
		repeat_count = ltp_parse_repeat_arg(argc, argv, 3);
		if (repeat_count < 0) {
			return -1;
		}
		return ltp_run_single(argv[2], repeat_count);
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
