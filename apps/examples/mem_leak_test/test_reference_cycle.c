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
 * @file test_reference_cycle.c
 * @brief Test: Reference cycle detection
 *
 * This test creates a reference cycle between two heap-allocated objects.
 * Neither object is reachable from any true root (BSS, DATA, stack).
 *
 * Expected behavior:
 *   - Current tool: NO LEAK reported (false negative)
 *     Reason: heap_check() treats all heap blocks as roots, so the
 *     cycle A→B→A makes both appear reachable.
 *
 *   - New tool (mark-and-sweep): BOTH blocks reported as UNREACHABLE
 *     Reason: heap blocks are NOT roots; only true roots are scanned.
 *     Since neither A nor B is reachable from BSS/DATA/stack, both
 *     are correctly identified as leaks.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>

/* Test node structure - mimics shared_ptr cycle pattern */
struct cycle_node_s {
	struct cycle_node_s *other;    /* Pointer to the other node */
	int data;                      /* Some payload data */
	char padding[16];              /* Padding to make block size realistic */
};

/* Statically allocated pointer used to set up the cycle, then cleared.
 * These are in BSS. We set them to the allocated blocks, then clear them
 * to remove the BSS root reference.
 */
static struct cycle_node_s *g_temp_a = NULL;
static struct cycle_node_s *g_temp_b = NULL;

/* Addresses of the allocated blocks, stored for printing.
 * These are NOT pointers to the blocks — just raw addresses for display.
 * We store them as integers so they don't act as roots.
 */
static unsigned long g_block_a_addr = 0;
static unsigned long g_block_b_addr = 0;

/* The worker task that creates the cycle and exits.
 * By running in a separate task that exits, the stack of this task
 * is no longer scanned by the leak checker (the TCB is gone).
 * This ensures no stale stack pointers keep the blocks "reachable".
 */
static int cycle_creator_task(int argc, char *argv[])
{
    {
	struct cycle_node_s *a;
	struct cycle_node_s *b;

	printf("  [DEBUG] cycle_creator_task running at PID %d\n", getpid());

	/* Allocate two nodes on the heap */
	a = (struct cycle_node_s *)malloc(sizeof(struct cycle_node_s));
	b = (struct cycle_node_s *)malloc(sizeof(struct cycle_node_s));


	if (!a || !b) {
		printf("  ERROR: Failed to allocate test nodes\n");
		return -1;
	}

	/* Initialize data */
	memset(a, 0xAA, sizeof(struct cycle_node_s));
	memset(b, 0xBB, sizeof(struct cycle_node_s));

	/* Create the reference cycle: A → B → A */
	a->other = b;
	b->other = a;
	a->data = 0x1111;
	b->data = 0x2222;

	/* Store addresses for display (as integers, not pointers) */
	g_block_a_addr = (unsigned long)a;
	g_block_b_addr = (unsigned long)b;

	printf("  Allocated block A at: 0x%lx (size: %d bytes)\n",
		g_block_a_addr, (int)sizeof(struct cycle_node_s));
	printf("  Allocated block B at: 0x%lx (size: %d bytes)\n",
		g_block_b_addr, (int)sizeof(struct cycle_node_s));
	printf("  A->other = 0x%lx (points to B)\n", (unsigned long)a->other);
	printf("  B->other = 0x%lx (points to A)\n", (unsigned long)b->other);
	printf("\n");

	/* CRITICAL: Zero out local stack variables before returning.
	 * This ensures no stale pointers remain on the stack when
	 * the task exits. Even though the task exits (TCB freed),
	 * zeroing locals is good practice for leak testing.
	 */
	a = NULL;
	b = NULL;
	g_block_a_addr = NULL;
	g_block_b_addr = NULL;
    }

	/* Task exits — its stack is no longer scanned by the leak checker */
	return 0;
}

void test_reference_cycle(void)
{
	pid_t pid;

	printf("\n");
	printf("========================================================\n");
	printf("  TEST 1: Reference Cycle Detection\n");
	printf("========================================================\n");
	printf("  [DEBUG] test_reference_cycle running at PID %d\n", getpid());

	printf("\n");
	printf("  Description:\n");
	printf("    Two heap blocks (A and B) reference each other.\n");
	printf("    Neither is reachable from BSS, DATA, or any stack.\n");
	printf("    This simulates a shared_ptr reference cycle, which\n");
	printf("    is the most common C++ memory leak pattern.\n");
	printf("\n");

	/* Spawn a separate task to create the cycle.
	 * When this task exits, its stack is no longer scanned,
	 * ensuring no stale stack pointers keep the blocks reachable.
	 */
	printf("  Spawning separate task to create the cycle...\n");
	pid = task_create("cycle_creator", 100, 2048, cycle_creator_task, NULL);
	if (pid < 0) {
		printf("  ERROR: Failed to create cycle_creator task\n");
		return;
	}

	/* Wait for the cycle creator task to complete */
	sleep(1);

	/* Clear BSS root pointers (already NULL, but explicit for clarity) */
	g_temp_a = NULL;
	g_temp_b = NULL;

	printf("  Cycle creator task has exited.\n");
	printf("  BSS root pointers cleared (g_temp_a = NULL, g_temp_b = NULL)\n");
	printf("  Now A and B are ONLY reachable from each other (cycle).\n");
	printf("  No BSS, DATA, or stack reference exists to either block.\n");
	printf("\n");

	printf("  >>> Now run mem_leak tool from TASH: <<<\n");
	printf("  TASH> mem_leak\n");
	printf("\n");

	/* Intentionally NOT freeing - this IS a leak.
	 * The blocks remain allocated with only each other as references.
	 */
}

