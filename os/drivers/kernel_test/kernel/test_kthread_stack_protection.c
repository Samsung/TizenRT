/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <tinyara/kernel_test_drv.h>
#include <debug.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <tinyara/kthread.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define	KTHREAD_STACK_NORMAL_SIZE		300
#define	KTHREAD_STACK_OVERFLOW_SIZE		4096

/****************************************************************************
 * Private Declarations
 ****************************************************************************/

static sem_t test_kthread_stack_sem;

/****************************************************************************
 * Private Function
 ****************************************************************************/

#if defined(CONFIG_REG_STACK_OVERFLOW_PROTECTION)
static void kthread_stack_overflow_array(void)
{
	volatile char array_normal[KTHREAD_STACK_OVERFLOW_SIZE];

	array_normal[0] = 'a';
}

static void kthread_stack_prot_inlimit(void)
{
	char array_normal[KTHREAD_STACK_NORMAL_SIZE];

	array_normal[0] = 'a';
}

static void kthread_stack_overflow_func(int *val)
{
	char *ptr;
	int i = 0;
	volatile char array_normal[KTHREAD_STACK_NORMAL_SIZE];

	ptr = &array_normal[KTHREAD_STACK_NORMAL_SIZE];
	for (i = 0; i < KTHREAD_STACK_NORMAL_SIZE; i++) {
		*ptr = 'a';
		ptr = ptr - 1;
	}

	*val = *val + 1;
	if (*val == (KTHREAD_STACK_OVERFLOW_SIZE / KTHREAD_STACK_NORMAL_SIZE)) {
		return;
	}
	/* recursive call to create stack overflow condition */
	kthread_stack_overflow_func(val);
}
#endif

static int kernel_thread_stack_protection_test(int argc, FAR char *argv[])
{
	char ch = argv[1][0];
	char *ptr;
	int i = 0;
	char array_normal[KTHREAD_STACK_NORMAL_SIZE];

#if defined(CONFIG_MPU_STACK_OVERFLOW_PROTECTION)
	if (ch == 'N' || ch == 'n') {
		dbg("Test Stack access within range\n");
		ptr = &array_normal[KTHREAD_STACK_NORMAL_SIZE];
		for (i = 0; i < KTHREAD_STACK_NORMAL_SIZE; i++) {
			*ptr = 'a';
			ptr = ptr - 1;
		}
		dbg(" Test stack access within range - Passed !!\n");
	} else if (ch == 'O' || ch == 'o') {
		dbg("Test Stack access in overflow\n");
		ptr = &array_normal[KTHREAD_STACK_NORMAL_SIZE];
		for (i = 0; i < KTHREAD_STACK_OVERFLOW_SIZE; i++) {
			*ptr = 'b';
			ptr = ptr - 1;
		}
		dbg(" Test stack access in overflow - Failed !!\n");
	}
#elif defined(CONFIG_REG_STACK_OVERFLOW_PROTECTION)

	if (ch == 'N' || ch == 'n') {
		dbg("Test Stack access within range\n");
		kthread_stack_prot_inlimit();
		dbg(" Test stack access within range using array size - Passed !!\n");
	} else if (ch == 'A' || ch == 'a') {
		dbg("Test Overflow using array declaration\n");
		kthread_stack_overflow_array();
		dbg(" Test stack access in overflow - Failed !!\n");
	} else if (ch == 'F' || ch == 'f') {
		dbg("Test Overflow using recursive function\n");
		kthread_stack_overflow_func(&i);
		dbg(" Test stack access in overflow - Failed !!\n");
	}
#endif

	(void)sem_post(&test_kthread_stack_sem);

	return 0;
}

static int create_kernel_thread_for_stack_protection_test(unsigned long arg)
{
	int pid;
	char *argv[2];
	argv[1] = NULL;
	argv[0] = (char *)arg;

	sem_init(&test_kthread_stack_sem, 0, 0);

	pid = kernel_thread("kthread_stack_prot_test", SCHED_PRIORITY_DEFAULT, 1024, kernel_thread_stack_protection_test, argv);
	if (pid < 0) {
		dbg("Failed to start kernel_thread_stack_protection_test thread\n");
		sem_destroy(&test_kthread_stack_sem);
		return -1;
	}

	sem_wait(&test_kthread_stack_sem);
	sem_destroy(&test_kthread_stack_sem);

	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int test_kthread_stack_overflow_protection(int cmd, unsigned long arg)
{
	int ret = -EINVAL;

	switch (cmd) {
	case TESTIOC_KTHREAD_STACK_PROTECTION_TEST:
		ret = create_kernel_thread_for_stack_protection_test(arg);
		break;
	}
	return ret;
}
