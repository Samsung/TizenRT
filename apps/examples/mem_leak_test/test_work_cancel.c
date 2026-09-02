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
 * @file test_work_cancel.c
 * @brief Test: Work cancel without free (true positive detection)
 *
 * This test allocates a block, queues work with it as arg, then cancels
 * the work WITHOUT freeing the arg. The work_s is removed from the work
 * queue, so the block is no longer reachable from any root. This is a
 * TRUE leak.
 *
 * Expected behavior:
 *   - Current tool: MAY MISS the leak (false negative)
 *     Reason: The work_s structure is static (in BSS). After work_cancel(),
 *     the work_s is removed from the work queue but the structure itself
 *     still exists in BSS with the arg pointer still set. Since BSS is
 *     scanned as a root, the tool may find the arg pointer in the stale
 *     work_s and mark the block as reachable.
 *
 *   - New tool: Behavior depends on implementation
 *     - If work_s.arg is NOT cleared by work_cancel(): BSS scan finds
 *       the stale pointer → block appears REACHABLE (false negative)
 *     - If work_s.arg IS cleared by work_cancel(): No pointer found
 *       → block correctly reported as UNREACHABLE
 *
 *     Note: This is a limitation of conservative scanning. The BSS-rooted
 *     work_s struct still contains the stale pointer. Both tools may
 *     miss this unless the work_s fields are explicitly cleared.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tinyara/wqueue.h>
#include <tinyara/clock.h>
#include <unistd.h>

#define TEST_BLOCK_SIZE 128
#define WORK_DELAY_TICKS (50 * 100)

/* Static work structure - lives in BSS (root source) */
static struct work_s g_cancel_work;
static void *g_cancelled_arg = NULL;

/* Dummy work callback - should NOT be called (we cancel before it runs) */
static void cancel_test_callback(FAR void *arg)
{
	printf("  [cancel_test_callback] ERROR: should not be called!\n");
}

void test_work_cancel_without_free(void)
{
	int ret;

	printf("\n");
	printf("========================================================\n");
	printf("  TEST 5: Work Cancel Without Free (True Positive)\n");
	printf("========================================================\n");
	printf("\n");
	printf("  Description:\n");
	printf("    Allocate a block, queue work with it as arg,\n");
	printf("    then cancel the work WITHOUT freeing the arg.\n");
	printf("    The work_s is removed from the queue, but the\n");
	printf("    arg block is orphaned. This is a TRUE leak.\n");
	printf("\n");

	/* Clear the work structure */
	memset(&g_cancel_work, 0, sizeof(struct work_s));

	/* Allocate the block that will be leaked */
	g_cancelled_arg = malloc(TEST_BLOCK_SIZE);
	if (!g_cancelled_arg) {
		printf("  ERROR: Failed to allocate test buffer\n");
		return;
	}

	/* Initialize with recognizable pattern */
	memset(g_cancelled_arg, 0xFF, TEST_BLOCK_SIZE);

	printf("  Allocated buffer at: %p (size: %d bytes)\n",
		g_cancelled_arg, TEST_BLOCK_SIZE);
	printf("\n");

	/* Queue the work with a large delay */
	ret = work_queue(HPWORK, &g_cancel_work, cancel_test_callback,
			 g_cancelled_arg, WORK_DELAY_TICKS);
	if (ret < 0) {
		printf("  ERROR: work_queue failed\n");
		free(g_cancelled_arg);
		return;
	}

	printf("  Queued work with arg=%p, delay=%d ticks\n",
		g_cancelled_arg, WORK_DELAY_TICKS);
	printf("  At this point, buffer is REACHABLE via work queue.\n");
	printf("\n");

	/* Cancel the work - removes it from the work queue.
	 * After cancel, g_wqueues[] no longer references g_cancel_work.
	 *
	 * HOWEVER: g_cancel_work is static (in BSS). Its fields (including
	 * arg) may still contain the old values. Since BSS is scanned as a
	 * root, the stale arg pointer may still be found.
	 */
	ret = work_cancel(HPWORK, &g_cancel_work);
	if (ret < 0) {
		printf("  WARNING: work_cancel returned %d (may already be done)\n", ret);
	}

	/* Give the system a moment to process the cancellation */
	usleep(10000);

	printf("  Cancelled work. Work removed from queue.\n");
	printf("  BUT: work_s (in BSS) may still contain stale arg pointer.\n");
	printf("\n");

	/* Clear local reference */
	g_cancelled_arg = NULL;
	printf("  Cleared local pointer (g_cancelled_arg = NULL)\n");
	printf("\n");

	printf("  >>> Now run: mem_leak <<<\n");
	printf("\n");
	printf("  NOTE: Look for a 128-byte block with pattern 0xFF.\n");
	printf("  This test demonstrates the need for snapshot diff mode,\n");
	printf("  as both tools may miss this due to stale BSS pointers.\n");
	printf("\n");

	/* Intentionally NOT freeing - this IS a leak */
}
