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
 * @file test_data_pointer.c
 * @brief Test A4: DATA segment reference
 *
 * An initialized global (DATA segment) variable points to an allocated block.
 * The block is reachable from DATA → NO LEAK.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DATA global — initialized static goes to DATA segment */
static void *g_data_ptr = NULL;

void test_data_pointer(void)
{
	printf("\n");
	printf("========================================================\n");
	printf("  TEST A4: DATA Segment Reference\n");
	printf("========================================================\n");

	printf("\n");
	printf("  Description:\n");
	printf("    Allocate 512 bytes, store pointer in DATA global.\n");
	printf("    Block is reachable from DATA root → NO LEAK.\n");
	printf("\n");

	g_data_ptr = malloc(512);
	if (!g_data_ptr) {
		printf("  ERROR: malloc failed\n");
		return;
	}

	memset(g_data_ptr, 0xDD, 512);
	printf("  Allocated: %p (512 bytes, pattern 0xDD)\n", g_data_ptr);
	printf("  Stored in: g_data_ptr (DATA at %p)\n", &g_data_ptr);
	printf("\n");

	printf("  >>> Now run mem_leak tool from TASH: <<<\n");
	printf("  TASH> mem_leak\n");
	printf("\n");
}
