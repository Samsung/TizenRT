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
 * @file test_multilevel_ref.c
 * @brief Test A7: Multi-level reference chain (A→B→C)
 *
 * Allocates three blocks linked as A→B→C. Only A is directly rooted in BSS.
 * B and C are reachable only via transitive marking from A.
 * Tests that v2 correctly follows pointer chains transitively.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ml_node_s {
	struct ml_node_s *next;
	int data[16];
};

/* BSS root — only A is directly reachable from here */
static struct ml_node_s *g_ml_root;

void test_multilevel_ref(void)
{
	struct ml_node_s *a;
	struct ml_node_s *b;
	struct ml_node_s *c;

	printf("\n");
	printf("========================================================\n");
	printf("  TEST A7: Multi-Level Reference (A→B→C)\n");
	printf("========================================================\n");

	printf("\n");
	printf("  Description:\n");
	printf("    Allocate 3 blocks linked as A→B→C.\n");
	printf("    Only A is rooted in BSS (g_ml_root).\n");
	printf("    B and C reachable only via transitive marking.\n");
	printf("    All should be NO LEAK.\n");
	printf("\n");

	a = (struct ml_node_s *)malloc(sizeof(struct ml_node_s));
	b = (struct ml_node_s *)malloc(sizeof(struct ml_node_s));
	c = (struct ml_node_s *)malloc(sizeof(struct ml_node_s));

	if (!a || !b || !c) {
		printf("  ERROR: malloc failed\n");
		return;
	}

	memset(a, 0xA7, sizeof(struct ml_node_s));
	memset(b, 0xB7, sizeof(struct ml_node_s));
	memset(c, 0xC7, sizeof(struct ml_node_s));

	a->next = b;
	b->next = c;
	c->next = NULL;

	/* Only A is directly rooted */
	g_ml_root = a;

	printf("  Allocated A: %p (next=%p)\n", a, b);
	printf("  Allocated B: %p (next=%p)\n", b, c);
	printf("  Allocated C: %p (next=NULL)\n", c);
	printf("  BSS root: g_ml_root = %p (points to A)\n", g_ml_root);
	printf("\n");

	printf("  >>> Now run mem_leak tool from TASH: <<<\n");
	printf("  TASH> mem_leak\n");
	printf("\n");
}
