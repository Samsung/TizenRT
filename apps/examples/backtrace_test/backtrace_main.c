/****************************************************************************
 * apps/examples/backtrace_test/backtrace_main.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: user_level_3
 *
 * Description:
 *   Deepest level - triggers ASSERT to capture backtrace automatically.
 *   In protected build, ASSERT goes through syscall (SYS_up_assert)
 *   which triggers kernel backtrace.
 *
 ****************************************************************************/

static void user_level_3(void)
{
	printf("[USER BT TEST] === Level 3 (deepest) - triggering NULL pointer deref ===\n");
	printf("[USER BT TEST] Expect automatic backtrace from kernel:\n");
	printf("[USER BT TEST]   user_level_3 -> user_level_2 -> user_level_1\n");
	printf("[USER BT TEST]   -> backtrace_test_main -> tash_entry\n");
	printf("\n");
	/* Trigger a real Data Abort exception via null pointer dereference.
	 * This goes through the exception handler (not syscall), so
	 * CURRENT_REGS is properly saved with user context.
	 */
	volatile int *null_ptr = NULL;
	*null_ptr = 0xDEAD;
}


/****************************************************************************
 * Name: user_level_2
 ****************************************************************************/

static void user_level_2(void)
{
	printf("[USER BT TEST] === Level 2 ===\n");
	user_level_3();
}

/****************************************************************************
 * Name: user_level_1
 ****************************************************************************/

static void user_level_1(void)
{
	printf("[USER BT TEST] === Level 1 ===\n");
	user_level_2();
}

/****************************************************************************
 * Name: user_deep_recursion
 *
 * Description:
 *   Deep recursion to stress-test the unwinder with many frames.
 *
 ****************************************************************************/

/* Deep nested call chain - each function has a unique PC address.
 * This tests the unwinder with many distinct frames.
 * Using __attribute__((noinline)) to prevent compiler from inlining.
 */

static void user_deep_10(void) __attribute__((noinline));
static void user_deep_9(void) __attribute__((noinline));
static void user_deep_8(void) __attribute__((noinline));
static void user_deep_7(void) __attribute__((noinline));
static void user_deep_6(void) __attribute__((noinline));
static void user_deep_5(void) __attribute__((noinline));
static void user_deep_4(void) __attribute__((noinline));
static void user_deep_3(void) __attribute__((noinline));
static void user_deep_2(void) __attribute__((noinline));
static void user_deep_1(void) __attribute__((noinline));

static void user_deep_10(void)
{
	printf("[USER BT TEST] === Deep nested level 10 (deepest) ===\n");
	printf("[USER BT TEST] Triggering NULL pointer deref for backtrace...\n");
	volatile int *null_ptr = NULL;
	*null_ptr = 0xDEAD;
}

static void user_deep_9(void)
{
	user_deep_10();
}

static void user_deep_8(void)
{
	user_deep_9();
}

static void user_deep_7(void)
{
	user_deep_8();
}

static void user_deep_6(void)
{
	user_deep_7();
}

static void user_deep_5(void)
{
	user_deep_6();
}

static void user_deep_4(void)
{
	user_deep_5();
}

static void user_deep_3(void)
{
	user_deep_4();
}

static void user_deep_2(void)
{
	user_deep_3();
}

static void user_deep_1(void)
{
	user_deep_2();
}



/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int backtrace_test_main(int argc, char *argv[])
#endif
{
	int choice = 0;

	printf("\n");
	printf("=============================================================\n");
	printf("ARM EHABI Backtrace Test - User Mode\n");
	printf("=============================================================\n");
	printf("\n");

	if (argc > 1) {
		choice = atoi(argv[1]);
	}

	switch (choice) {
	case 0:
		/* Basic 3-level nested backtrace via ASSERT */
		printf("[USER BT TEST] Test 0: Basic nested backtrace (3 levels)\n");
		printf("[USER BT TEST] Expect to see user_level_3, user_level_2,\n");
		printf("[USER BT TEST]   user_level_1, and task entry in backtrace\n");
		printf("\n");
		user_level_1();
		break;

	case 1:
		/* Deep nested call chain (10 levels) */
		printf("[USER BT TEST] Test 1: Deep nested backtrace (10 levels)\n");
		printf("[USER BT TEST] Tests unwinder with many distinct frames\n");
		printf("[USER BT TEST] Expect: user_deep_10 -> user_deep_9 -> ... -> user_deep_1\n");
		printf("\n");
		user_deep_1();
		break;


	case 2:
		/* Direct ASSERT trigger */
		printf("[USER BT TEST] Test 2: Direct ASSERT trigger\n");
		printf("[USER BT TEST] This will capture automatic backtrace and reboot\n");
		printf("\n");
		DEBUGASSERT(0);
		break;

	default:
		printf("[USER BT TEST] Usage: backtrace_test [test_number]\n");
		printf("[USER BT TEST]   0 - Basic nested backtrace (3 levels)\n");
		printf("[USER BT TEST]   1 - Deep recursion (10 levels)\n");
		printf("[USER BT TEST]   2 - Direct ASSERT trigger\n");
		printf("\n");
		printf("[USER BT TEST] Note: All tests trigger ASSERT which captures\n");
		printf("[USER BT TEST] backtrace automatically via kernel unwinder.\n");
		break;
	}

	printf("\n");
	printf("[USER BT TEST] Test complete.\n");
	printf("=============================================================\n");
	printf("\n");

	return 0;
}
