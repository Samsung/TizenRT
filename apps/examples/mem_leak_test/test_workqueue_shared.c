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
 * @file test_workqueue_shared.c
 * @brief Test: Work queue shared block (false negative prevention)
 *
 * This test allocates a block, queues a work item with the block as
 * the argument, and does NOT let the work execute yet (delay > 0).
 * The block should appear REACHABLE because the work queue holds a
 * reference to it via the work_s structure.
 *
 * Expected behavior:
 *   - Current tool: Block NOT reported as leak (REACHABLE)
 *     Reason: heap_check() scans kernel heap, finds the work_s structure
 *     containing the arg pointer, marks the app block as reachable.
 *     Also, g_wqueues[] in kernel BSS points to the work_s.
 *
 *   - New tool: Block NOT reported as leak (REACHABLE)
 *     Reason: Kernel BSS (g_wqueues[]) scanned as root → work_s found
 *     → arg pointer found → app block marked reachable.
 *
 * This test verifies that work queue pointer tracking works correctly
 * in BOTH old and new implementations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tinyara/wqueue.h>
#include <tinyara/clock.h>

#define TEST_BLOCK_SIZE 128
/* Large delay so work doesn't execute before we run the leak checker */
#define WORK_DELAY_TICKS (100 * 100)

/* Static work structure - lives in BSS (root source) */
static struct work_s g_work;
static void *g_work_arg = NULL;

/* Dummy work callback - just prints (should not be called during test) */
static void work_callback(FAR void *arg)
{
	printf("  [work_callback] called with arg=%p\n", arg);
}

void test_workqueue_shared_block(void)
{
	int ret;

	printf("\n");
	printf("========================================================\n");
	printf("  TEST 3: Work Queue Shared Block\n");
	printf("========================================================\n");
	printf("\n");
	printf("  Description:\n");
	printf("    Allocate a block, queue work with it as arg.\n");
	printf("    Use a large delay so work doesn't execute.\n");
	printf("    The block should be REACHABLE because the work\n");
	printf("    queue holds a reference to it via work_s.arg.\n");
	printf("\n");

	/* Clear the work structure */
	memset(&g_work, 0, sizeof(struct work_s));

	/* Allocate the block that will be passed as work arg */
	g_work_arg = malloc(TEST_BLOCK_SIZE);
	if (!g_work_arg) {
		printf("  ERROR: Failed to allocate test buffer\n");
		return;
	}

	/* Initialize with recognizable pattern */
	memset(g_work_arg, 0xDD, TEST_BLOCK_SIZE);

	printf("  Allocated work arg buffer at: %p (size: %d bytes)\n",
		g_work_arg, TEST_BLOCK_SIZE);
	printf("\n");

	/* Queue the work with a large delay.
	 *
	 * Reachability chain after work_queue():
	 *   Kernel BSS (g_wqueues[HPWORK].q) → work_s (stack/BSS) → arg (app heap)
	 *
	 * The work_s is in BSS (static g_work), which is scanned as a root.
	 * The arg pointer inside work_s points to the app heap block.
	 */
	ret = work_queue(HPWORK, &g_work, work_callback, g_work_arg, WORK_DELAY_TICKS);
	if (ret < 0) {
		printf("  ERROR: work_queue failed\n");
		free(g_work_arg);
		return;
	}

	printf("  Queued work with arg=%p, delay=%d ticks\n",
		g_work_arg, WORK_DELAY_TICKS);
	printf("  (Work will NOT execute during this test due to large delay)\n");
	printf("\n");

	/* Clear local reference - the only reference is now in the work_s */
	g_work_arg = NULL;
	printf("  Cleared local pointer (g_work_arg = NULL)\n");
	printf("  The only reference to the buffer is now in work_s.arg\n");
	printf("  (work_s is in BSS, which is a root source)\n");
	printf("\n");

	printf("  >>> Now run: mem_leak <<<\n");
	printf("\n");
	/* Do NOT cancel work here - we want the buffer to remain reachable */
}
