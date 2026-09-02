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
 * @file test_simple_orphan.c
 * @brief Test B2: Simple orphan (lose all references)
 *
 * Allocates a block in a separate task, then the task exits without freeing.
 * No BSS, DATA, or stack reference remains. Both tools should detect this.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>

static int orphan_creator_task(int argc, char *argv[])
{
	void *p = malloc(128);
	if (!p) {
		printf("  ERROR: malloc failed\n");
		return -1;
	}

	memset(p, 0x22, 128);
	printf("  [orphan_creator PID %d] Allocated: %p (128 bytes, pattern 0x22)\n",
		getpid(), p);

	/* Overwrite local variable so no stack reference remains */
	p = NULL;
	printf("  [orphan_creator] Cleared local pointer, exiting.\n");
	return 0;
}

void test_simple_orphan(void)
{
	pid_t pid;

	printf("\n");
	printf("========================================================\n");
	printf("  TEST B2: Simple Orphan (Lose All References)\n");
	printf("========================================================\n");

	printf("\n");
	printf("  Description:\n");
	printf("    A separate task allocates 128 bytes, then clears\n");
	printf("    its local pointer and exits. No BSS, DATA, or stack\n");
	printf("    reference remains. This is a TRUE leak.\n");
	printf("\n");

	printf("  Spawning orphan_creator task...\n");
	pid = task_create("orphan_creator", 100, 2048, orphan_creator_task, NULL);
	if (pid < 0) {
		printf("  ERROR: Failed to create orphan_creator task\n");
		return;
	}

	sleep(1);
	printf("  Orphan creator task has exited.\n");
	printf("  Block is now orphaned (no references).\n");
	printf("\n");

	printf("  >>> Now run mem_leak tool from TASH: <<<\n");
	printf("  TASH> mem_leak\n");
	printf("\n");
	printf("  NOTE: Look for a 128-byte block with pattern 0x22.\n");
	printf("\n");
}
