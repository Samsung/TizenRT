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
 * @file test_triple_cycle.c
 * @brief Test B5: Triple reference cycle (A→B→C→A)
 *
 * Three blocks form a reference cycle: A→B→C→A.
 * No root points to any of them. v1 misses (false negative), v2 detects all 3.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>

struct tc_node_s {
	struct tc_node_s *next;
	int data[16];
};

static int triple_cycle_creator_task(int argc, char *argv[])
{
	struct tc_node_s *a;
	struct tc_node_s *b;
	struct tc_node_s *c;

	printf("  [triple_cycle_creator PID %d] Creating 3-node cycle...\n", getpid());

	a = (struct tc_node_s *)malloc(sizeof(struct tc_node_s));
	b = (struct tc_node_s *)malloc(sizeof(struct tc_node_s));
	c = (struct tc_node_s *)malloc(sizeof(struct tc_node_s));

	if (!a || !b || !c) {
		printf("  ERROR: malloc failed\n");
		return -1;
	}

	memset(a, 0xA5, sizeof(struct tc_node_s));
	memset(b, 0xB5, sizeof(struct tc_node_s));
	memset(c, 0xC5, sizeof(struct tc_node_s));

	/* Create cycle: A→B→C→A */
	a->next = b;
	b->next = c;
	c->next = a;

	printf("  Allocated A: %p (next=%p)\n", a, b);
	printf("  Allocated B: %p (next=%p)\n", b, c);
	printf("  Allocated C: %p (next=%p)\n", c, a);
	printf("  Cycle: A→B→C→A (no root reference)\n");

	/* Clear locals so no stack reference remains */
	a = NULL;
	b = NULL;
	c = NULL;
	printf("  [triple_cycle_creator] Cleared locals, exiting.\n");
	return 0;
}

void test_triple_cycle(void)
{
	pid_t pid;

	printf("\n");
	printf("========================================================\n");
	printf("  TEST B5: Triple Reference Cycle (A→B→C→A)\n");
	printf("========================================================\n");

	printf("\n");
	printf("  Description:\n");
	printf("    Three heap blocks form a cycle: A→B→C→A.\n");
	printf("    No BSS, DATA, or stack reference to any block.\n");
	printf("    v1 misses (all appear reachable via cycle).\n");
	printf("    v2 detects all 3 as UNREACHABLE.\n");
	printf("\n");

	printf("  Spawning triple_cycle_creator task...\n");
	pid = task_create("tc_creator", 100, 2048, triple_cycle_creator_task, NULL);
	if (pid < 0) {
		printf("  ERROR: Failed to create tc_creator task\n");
		return;
	}

	sleep(1);
	printf("  Cycle creator task has exited.\n");
	printf("\n");

	printf("  >>> Now run mem_leak tool from TASH: <<<\n");
	printf("  TASH> mem_leak\n");
	printf("\n");
	printf("  NOTE: Look for 3 blocks with patterns 0xA5, 0xB5, 0xC5.\n");
	printf("\n");
}
