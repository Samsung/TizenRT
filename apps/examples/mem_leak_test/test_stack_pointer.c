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
 * @file test_stack_pointer.c
 * @brief Test A3: Stack pointer reference
 *
 * A separate task allocates memory and keeps the pointer on its stack.
 * The task sleeps so the checker can run from TASH while the task is alive.
 * The block is reachable from the task's stack → NO LEAK.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>

static int stack_holder_task(int argc, char *argv[])
{
	void *p = malloc(64);
	if (!p) {
		printf("  ERROR: malloc failed\n");
		return -1;
	}

	memset(p, 0xCC, 64);
	printf("  [stack_holder_task PID %d] Allocated: %p (64 bytes, pattern 0xCC)\n",
		getpid(), p);
	printf("  Pointer kept on stack of PID %d\n", getpid());
	printf("  Task sleeping for 30 seconds — run checker now!\n");
	printf("\n");

	printf("  >>> Now run mem_leak tool from TASH: <<<\n");
	printf("  TASH> mem_leak\n");
	printf("\n");

	/* Sleep to keep the task alive while checker runs */
	sleep(30);

	free(p);
	printf("  [stack_holder_task] Freed %p, exiting.\n", p);
	return 0;
}

void test_stack_pointer(void)
{
	pid_t pid;

	printf("\n");
	printf("========================================================\n");
	printf("  TEST A3: Stack Pointer Reference\n");
	printf("========================================================\n");

	printf("\n");
	printf("  Description:\n");
	printf("    A separate task allocates 64 bytes and keeps the\n");
	printf("    pointer on its stack. The task sleeps so the checker\n");
	printf("    can run while the task is alive.\n");
	printf("    Block is reachable from stack → NO LEAK.\n");
	printf("\n");

	pid = task_create("stack_holder", 100, 2048, stack_holder_task, NULL);
	if (pid < 0) {
		printf("  ERROR: Failed to create stack_holder task\n");
		return;
	}
	printf("  Created stack_holder task (PID %d)\n", pid);
	printf("  The task will sleep for 30 seconds.\n");
	printf("  Run the checker within that time window.\n");
}
