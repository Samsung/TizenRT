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
 * @file test_cpp_array_cookie.cpp
 * @brief Test E1: C++ new T[n] array cookie interior pointer
 *
 * Demonstrates false positive leak detection for C++ array allocations
 * where new T[n] returns a pointer past the array cookie (interior pointer).
 *
 * This reproduces the issue, where:
 *   - new T[n] allocates: [header][cookie][element 0][element 1]...
 *   - The returned pointer points to element 0 (past the cookie)
 *   - The leak checker subtracts only SIZEOF_MM_ALLOCNODE from the
 *     stored pointer, missing the actual allocation node by cookie_size
 *   - Result: false LEAK reported for a reachable allocation
 *
 * Memory layout after new TestModel[1]:
 *   [16-byte header][8-byte cookie][TestModel element]
 *   ^                ^              ^
 *   |                |              └─ g_array_ptr (what app stores)
 *   |                └─ cookie {element_size, count}
 *   └─ allocation node (what checker looks for)
 *
 * The checker computes: g_array_ptr - 16 = node - 8 (WRONG!)
 * Should match: node (the actual allocation start)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Class with non-trivial destructor to trigger array cookie generation.
 * On ARM with the Itanium C++ ABI, new T[n] for types with non-trivial
 * destructors emits an 8-byte cookie {element_size, count} before the
 * first element.
 */
class TestModel {
public:
	int id;
	int data[4];

	TestModel() : id(0)
	{
		data[0] = data[1] = data[2] = data[3] = 0;
	}

	~TestModel()
	{
		/* Non-trivial destructor forces array cookie */
	}
};

/* Global variable simulating mModels from EPDInferenceHandler.
 * This stores the ELEMENT pointer (past the cookie), not the
 * allocation start. The leak checker must recognize this as a
 * valid reference to the allocation.
 */
static TestModel *g_array_ptr = NULL;

/* Second test: multi-element array simulating m_task_queues
 * from AIWorkerPool (new T[2] with 24-byte elements).
 */
class TestQueueItem {
public:
	int items[6];  /* 24 bytes on ARM */

	TestQueueItem()
	{
		for (int i = 0; i < 6; i++) {
			items[i] = 0;
		}
	}

	~TestQueueItem()
	{
		/* Non-trivial destructor */
	}
};

static TestQueueItem *g_queue_array_ptr = NULL;

void test_cpp_array_cookie(void)
{
	printf("\n");
	printf("========================================================\n");
	printf("  TEST E1: C++ Array Cookie Interior Pointer\n");
	printf("========================================================\n");

	printf("\n");
	printf("  Description:\n");
	printf("    Allocate array with new TestModel[1].\n");
	printf("    C++ new T[n] stores an 8-byte array cookie before\n");
	printf("    the first element and returns a pointer to element[0].\n");
	printf("    The leak checker must recognize this interior pointer.\n");
	printf("\n");

	/* This simulates: array cookie for complex class */
	g_array_ptr = new TestModel[1];
	if (!g_array_ptr) {
		printf("  ERROR: new TestModel[1] failed\n");
		return;
	}

	/* Initialize element data */
	g_array_ptr[0].id = 42;
	g_array_ptr[0].data[0] = 0xDEAD;

	printf("  Allocated: new TestModel[1]\n");
	printf("  g_array_ptr (element pointer): %p\n", (void *)g_array_ptr);
	printf("  (This points PAST the 8-byte cookie, into the allocation)\n");
	printf("  element[0].id = %d\n", g_array_ptr[0].id);
	printf("\n");

	printf("  EXPECTED BEHAVIOR:\n");
	printf("    No leak\n");
	printf("\n");

	printf("  >>> Now run mem_leak from TASH to check: <<<\n");
	printf("  TASH> mem_leak [optons]\n");
	printf("\n");

	fflush(stdout);
}

void test_cpp_array_cookie_multi(void)

{
	printf("\n");
	printf("========================================================\n");
	printf("  TEST E2: C++ Array Cookie (Multi-Element)\n");
	printf("========================================================\n");

	printf("\n");
	printf("  Description:\n");
	printf("    Allocate array with new TestQueueItem[2].\n");
	printf("    Cookie: {element_size=24, count=2}\n");
	printf("\n");

	/* This simulates: m_task_queues = new queue<unique_ptr<Task>>[2] */
	g_queue_array_ptr = new TestQueueItem[2];
	if (!g_queue_array_ptr) {
		printf("  ERROR: new TestQueueItem[2] failed\n");
		return;
	}

	g_queue_array_ptr[0].items[0] = 0xBEEF;
	g_queue_array_ptr[1].items[0] = 0xCAFE;

	printf("  Allocated: new TestQueueItem[2]\n");
	printf("  g_queue_array_ptr (element pointer): %p\n", (void *)g_queue_array_ptr);
	printf("  (Points past 8-byte cookie to element[0])\n");
	printf("  element[0].items[0] = 0x%04x\n", g_queue_array_ptr[0].items[0]);
	printf("  element[1].items[0] = 0x%04x\n", g_queue_array_ptr[1].items[0]);
	printf("\n");

	printf("  EXPECTED BEHAVIOR:\n");
	printf("    No leak\n");
	printf("\n");

	printf("  >>> Now run mem_leak from TASH to check: <<<\n");
	printf("  TASH> mem_leak [options]]\n");
	printf("\n");

	fflush(stdout);
}

#ifdef __cplusplus
}
#endif
