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
 * @file test_cross_task.c
 * @brief Test D3: Cross-task sharing
 *
 * Task A allocates a block and stores the pointer in a shared BSS global.
 * Task B reads the global and uses the block.
 * The block is reachable via BSS → NO LEAK.
 * Then clear the global → block becomes unreachable → LEAK.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>

/* Shared BSS global — accessible by all tasks */
static void *g_shared_cross_task;

static int cross_task_allocator(int argc, char *argv[])
{
	g_shared_cross_task = malloc(256);
	if (!g_shared_cross_task) {
		printf("  ERROR: malloc failed\n");
		return -1;
	}

	memset(g_shared_cross_task, 0xD3, 256);
	printf("  [allocator PID %d] Allocated: %p (256 bytes, pattern 0xD3)\n",
		getpid(), g_shared_cross_task);
	printf("  Stored in shared BSS global: g_shared_cross_task\n");
	return 0;
}

static int cross_task_consumer(int argc, char *argv[])
{
	printf("  [consumer PID %d] Reading g_shared_cross_task = %p\n",
		getpid(), g_shared_cross_task);
	if (g_shared_cross_task) {
		printf("  [consumer] Block is accessible, data[0]=0x%02x\n",
			((unsigned char *)g_shared_cross_task)[0]);
	}
	return 0;
}

void test_cross_task(void)
{
	pid_t pid;

	printf("\n");
	printf("========================================================\n");
	printf("  TEST D3: Cross-Task Sharing\n");
	printf("========================================================\n");

	printf("\n");
	printf("  Description:\n");
	printf("    Task A allocates a block, stores in BSS global.\n");
	printf("    Task B reads the global and accesses the block.\n");
	printf("    Block is reachable via BSS → NO LEAK.\n");
	printf("    Then clear BSS → block becomes unreachable → LEAK.\n");
	printf("\n");

	/* Task A allocates */
	pid = task_create("ct_alloc", 100, 2048, cross_task_allocator, NULL);
	if (pid < 0) {
		printf("  ERROR: Failed to create allocator task\n");
		return;
	}
	sleep(1);

	/* Task B consumes */
	pid = task_create("ct_consume", 100, 2048, cross_task_consumer, NULL);
	if (pid < 0) {
		printf("  ERROR: Failed to create consumer task\n");
		return;
	}
	sleep(1);

	/* Now create the leak */
	g_shared_cross_task = NULL;
	printf("  >>> Run mem_leak tool from TASH: <<<\n");
	printf("  TASH> mem_leak\n");
	printf("\n");
	printf("  NOTE: Look for a 256-byte block with pattern 0xD3.\n");
	printf("\n");
}
