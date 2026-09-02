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
 * @file test_bss_pointer.c
 * @brief Test A2: BSS pointer reference
 *
 * A static (BSS) global variable points to an allocated block.
 * The block is reachable from BSS, so no leak should be reported.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* BSS global — uninitialized static goes to BSS */
static void *g_bss_ptr;

void test_bss_pointer(void)
{
	printf("\n");
	printf("========================================================\n");
	printf("  TEST A2: BSS Pointer Reference\n");
	printf("========================================================\n");

	printf("\n");
	printf("  Description:\n");
	printf("    Allocate 256 bytes, store pointer in BSS global.\n");
	printf("    Block is reachable from BSS root → NO LEAK.\n");
	printf("\n");

	g_bss_ptr = malloc(256);
	if (!g_bss_ptr) {
		printf("  ERROR: malloc failed\n");
		return;
	}

	memset(g_bss_ptr, 0xBB, 256);
	printf("  Allocated: %p (256 bytes, pattern 0xBB)\n", g_bss_ptr);
	printf("  Stored in: g_bss_ptr (BSS at %p)\n", &g_bss_ptr);
	printf("\n");

	printf("  >>> Now run leak checker tool from TASH: <<<\n");
	printf("  TASH> mem_leak\n");
}
