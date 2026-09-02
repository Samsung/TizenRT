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
 * @file test_many_allocs.c
 * @brief Test D2: Many small allocations (100+)
 *
 * Allocates 100 small blocks, keeps references in a global array.
 * Then frees all but one, creating a single leak.
 * Verifies hash table capacity and scalability.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define D2_COUNT 100

static void *g_ptr_array[D2_COUNT];

void test_many_allocs(void)
{
	int i;

	printf("\n");
	printf("========================================================\n");
	printf("  TEST D2: Many Small Allocations (%d blocks)\n", D2_COUNT);
	printf("========================================================\n");

	printf("\n");
	printf("  Description:\n");
	printf("    Allocate %d small (32-byte) blocks, keep in global array.\n", D2_COUNT);
	printf("    Step 1: All reachable via g_ptr_array → NO LEAK.\n", D2_COUNT);
	printf("    Step 2: Free all but one → 1 LEAK expected.\n");
	printf("\n");

	/* Allocate 100 blocks */
	for (i = 0; i < D2_COUNT; i++) {
		g_ptr_array[i] = malloc(32);
		if (!g_ptr_array[i]) {
			printf("  ERROR: malloc failed at index %d\n", i);
			return;
		}
		memset(g_ptr_array[i], 0xD2, 32);
	}
	printf("  Allocated %d blocks (32 bytes each, pattern 0xD2)\n", D2_COUNT);
	printf("  Stored in: g_ptr_array (BSS, %d entries)\n", D2_COUNT);
	printf("\n");

	/* Free all but one (index 50), then clear the array entry too */
	for (i = 0; i < D2_COUNT; i++) {
		if (i != 50) {
			free(g_ptr_array[i]);
			g_ptr_array[i] = NULL;
		}
	}
	printf("  Freed 99 blocks, keeping index 50 (%p)\n", g_ptr_array[50]);

	/* Now clear the last reference to create a true orphan */
	g_ptr_array[50] = NULL;
	printf("  >>> Run mem_leak tool again from TASH: <<<\n");
	printf("  TASH> mem_leak\n");
	printf("\n");
	printf("  NOTE: Look for 1 block (32 bytes, pattern 0xD2).\n");
	printf("\n");

}
