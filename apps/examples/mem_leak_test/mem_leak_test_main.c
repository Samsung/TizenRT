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
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/**
 * @file mem_leak_test_main.c
 * @brief Test suite for memory leak checker tool validation
 *
 * This test suite creates specific memory leak scenarios to validate the
 * behavior of the memory leak checker tool. Each test sets up a known
 * leak condition, then the user runs 'mem_leak_checker' from the TASH
 * prompt to observe whether the tool detects the leak correctly.
 *
 * Usage:
 *   mem_leak_test           - Show menu/usage
 *   mem_leak_test 1         - Run test 1 (Reference cycle)
 *   mem_leak_test 2         - Run test 2 (MQ shared block)
 *   mem_leak_test 3         - Run test 3 (Work queue shared block)
 *   mem_leak_test 4         - Run test 4 (MQ leak after close)
 *   mem_leak_test 5         - Run test 5 (Work cancel without free)
 *   mem_leak_test 6         - Run ALL original tests sequentially
 *
 *   mem_leak_test 11        - Test A1: Normal alloc/free
 *   mem_leak_test 12        - Test A2: BSS pointer reference
 *   mem_leak_test 13        - Test A3: Stack pointer reference
 *   mem_leak_test 14        - Test A4: DATA segment reference
 *   mem_leak_test 17        - Test A7: Multi-level reference (A→B→C)
 *   mem_leak_test 22        - Test B2: Simple orphan
 *   mem_leak_test 25        - Test B5: Triple cycle (A→B→C→A)
 *   mem_leak_test 31        - Test D1: Large allocation (4KB)
 *   mem_leak_test 32        - Test D2: Many small allocations (100+)
 *   mem_leak_test 33        - Test D3: Cross-task sharing
 *   mem_leak_test 34        - Test D4: Kernel heap pointer to app
 *   mem_leak_test 99        - Run ALL new tests sequentially
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Forward declarations for all test functions */

/* Original tests (B1, A5, A6, B3, B4) */
void test_reference_cycle(void);
void test_mq_shared_block(void);
void test_workqueue_shared_block(void);
void test_mq_leak_after_close(void);
void test_work_cancel_without_free(void);

/* Category A: No False Positives */
void test_normal_alloc_free(void);
void test_bss_pointer(void);
void test_stack_pointer(void);
void test_data_pointer(void);
void test_multilevel_ref(void);

/* Category B: True Positives */
void test_simple_orphan(void);
void test_triple_cycle(void);

/* Category D: Edge Cases */
void test_large_alloc(void);
void test_many_allocs(void);
void test_cross_task(void);
void test_kernel_to_app(void);

/* Helper: print the usage menu */
static void print_usage(void)
{
	printf("\n");
	printf("========================================================\n");
	printf("     Memory Leak Checker - Test Suite\n");
	printf("========================================================\n");
	printf("\n");
	printf("  This suite creates known leak scenarios so you can\n");
	printf("  verify the leak checker's detection capability.\n");
	printf("\n");
	printf("  After running a test, use 'mem_leak' and 'mem_leak_v2'\n");
	printf("  from TASH to check if the leak was detected.\n");
	printf("\n");
	printf("  Usage:\n");
	printf("    mem_leak_test <test_number>\n");
	printf("\n");
	printf("  Original Tests:\n");
	printf("    1  Reference cycle (A<->B, no root)         [B1]\n");
	printf("    2  MQ shared block (buffer in kernel MQ)     [A5]\n");
	printf("    3  Work queue shared block (arg in work_q)   [A6]\n");
	printf("    4  MQ leak after close (mq_close w/o drain)  [B3]\n");
	printf("    5  Work cancel without free                   [B4]\n");
	printf("    6  Run ALL original tests sequentially\n");
	printf("\n");
	printf("  Category A: No False Positives:\n");
	printf("    11 Normal alloc/free (baseline)              [A1]\n");
	printf("    12 BSS pointer reference                     [A2]\n");
	printf("    13 Stack pointer reference (separate task)   [A3]\n");
	printf("    14 DATA segment reference                    [A4]\n");
	printf("    17 Multi-level reference (A->B->C)            [A7]\n");
	printf("\n");
	printf("  Category B: True Positives:\n");
	printf("    22 Simple orphan (lose all references)        [B2]\n");
	printf("    25 Triple cycle (A->B->C->A)                 [B5]\n");
	printf("\n");
	printf("  Category D: Edge Cases:\n");
	printf("    31 Large allocation (4096 bytes)              [D1]\n");
	printf("    32 Many small allocations (100 blocks)        [D2]\n");
	printf("    33 Cross-task sharing                        [D3]\n");
	printf("    34 Kernel heap pointer to app block          [D4]\n");
	printf("\n");
	printf("    99 Run ALL new tests sequentially\n");
	printf("\n");
	printf("  Example:\n");
	printf("    TASH> mem_leak_test 1\n");
	printf("\n");
	printf("========================================================\n");
	printf("\n");
}

/****************************************************************************
 * Main entry point
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int mem_leak_test_main(int argc, char *argv[])
#endif
{
	int choice;

	if (argc < 2) {
		printf("\n  Error: No test number specified.\n");
		print_usage();
		return -1;
	}

	choice = atoi(argv[1]);

	printf("\n");

	switch (choice) {
	/* Original tests */
	case 1:
		test_reference_cycle();
		break;
	case 2:
		test_mq_shared_block();
		break;
	case 3:
		test_workqueue_shared_block();
		break;
	case 4:
		test_mq_leak_after_close();
		break;
	case 5:
		test_work_cancel_without_free();
		break;
	case 6:
		printf(">>> Running ALL original tests sequentially <<<\n\n");
		test_reference_cycle();
		sleep(1);
		test_mq_shared_block();
		sleep(1);
		test_workqueue_shared_block();
		sleep(1);
		test_mq_leak_after_close();
		sleep(1);
		test_work_cancel_without_free();
		break;

	/* Category A: No False Positives */
	case 11:
		test_normal_alloc_free();
		break;
	case 12:
		test_bss_pointer();
		break;
	case 13:
		test_stack_pointer();
		break;
	case 14:
		test_data_pointer();
		break;
	case 17:
		test_multilevel_ref();
		break;

	/* Category B: True Positives */
	case 22:
		test_simple_orphan();
		break;
	case 25:
		test_triple_cycle();
		break;

	/* Category D: Edge Cases */
	case 31:
		test_large_alloc();
		break;
	case 32:
		test_many_allocs();
		break;
	case 33:
		test_cross_task();
		break;
	case 34:
		test_kernel_to_app();
		break;

	/* Run all new tests */
	case 99:
		printf(">>> Running ALL new tests sequentially <<<\n\n");
		test_normal_alloc_free();
		sleep(1);
		test_bss_pointer();
		sleep(1);
		test_stack_pointer();
		sleep(1);
		test_data_pointer();
		sleep(1);
		test_multilevel_ref();
		sleep(1);
		test_simple_orphan();
		sleep(1);
		test_triple_cycle();
		sleep(1);
		test_large_alloc();
		sleep(1);
		test_many_allocs();
		sleep(1);
		test_cross_task();
		sleep(1);
		test_kernel_to_app();
		break;

	default:
		printf("  Error: Invalid test number '%s'.\n", argv[1]);
		print_usage();
		return -1;
	}

	return 0;
}
