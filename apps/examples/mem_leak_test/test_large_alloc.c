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
 * @file test_large_alloc.c
 * @brief Test D1: Large allocation (>1KB)
 *
 * Allocates a 4KB block, keeps it reachable via BSS.
 * Then loses the reference to create a leak.
 * Verifies size handling in the scanner.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void *g_large_ptr;

void test_large_alloc(void)
{
	printf("\n");
	printf("========================================================\n");
	printf("  TEST D1: Large Allocation (4096 bytes)\n");
	printf("========================================================\n");

	printf("\n");
	printf("  Description:\n");
	printf("    Allocate 4096 bytes, keep in BSS root.\n");
	printf("    Step 1: Block reachable → NO LEAK expected.\n");
	printf("    Step 2: Clear BSS pointer → LEAK expected.\n");
	printf("\n");

	g_large_ptr = malloc(4096);
	if (!g_large_ptr) {
		printf("  ERROR: malloc failed\n");
		return;
	}

	memset(g_large_ptr, 0xD1, 4096);
	printf("  Allocated: %p (4096 bytes, pattern 0xD1)\n", g_large_ptr);
	printf("  Stored in: g_large_ptr (BSS at %p)\n", &g_large_ptr);
	printf("\n");

	/* Now create the leak */
	g_large_ptr = NULL;
	printf("  >>> Run mem_leak tool again from TASH: <<<\n");
	printf("  TASH> mem_leak\n");
	printf("\n");
	printf("  NOTE: Look for a 4096-byte block with pattern 0xD1.\n");
	printf("\n");
}
