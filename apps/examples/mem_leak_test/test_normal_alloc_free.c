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
 * @file test_normal_alloc_free.c
 * @brief Test A1: Normal alloc/free (baseline)
 *
 * Allocates memory, fills it, then properly frees it.
 * No leak should be reported by either tool.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_normal_alloc_free(void)
{
	void *p;

	printf("\n");
	printf("========================================================\n");
	printf("  TEST A1: Normal Alloc/Free (Baseline)\n");
	printf("========================================================\n");

	printf("\n");
	printf("  Description:\n");
	printf("    Allocate 128 bytes, fill with 0xAA, then free.\n");
	printf("    No leak should be reported by either tool.\n");
	printf("\n");

	p = malloc(128);
	if (!p) {
		printf("  ERROR: malloc failed\n");
		return;
	}

	memset(p, 0xAA, 128);
	printf("  Allocated: %p (128 bytes, pattern 0xAA)\n", p);

	free(p);
	printf("  Freed: %p\n", p);
	printf("\n");

	printf("  >>> Now run mem_leak tool from TASH: <<<\n");
	printf("  TASH> mem_leak\n");
	printf("\n");
}
