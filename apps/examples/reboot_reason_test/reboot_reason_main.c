/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
 * examples/reboot_reason_test/reboot_reason_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef CONFIG_WATCHDOG
#include <fcntl.h>
#include <sys/ioctl.h>
#include <tinyara/watchdog.h>
#endif
#ifdef CONFIG_LIB_BOARDCTL
#include <sys/boardctl.h>
#endif
#include <sys/prctl.h>
#include <tinyara/reboot_reason.h>

#define REBOOT_REASON_TEST_VAL 123

static void display_reboot_reason_option(void)
{
	printf("\nSelect Reboot Reason Test Option.\n");
	printf("\t-Press H or h : HW Reset Test\n");
#ifdef CONFIG_MM_ASSERT_ON_FAIL
	printf("\t-Press M or m : Memory Allocation Fail Test\n");
#endif
#ifdef CONFIG_WATCHDOG
	printf("\t-Press W or w : Watchdog Reset Test\n");
#endif
	printf("\t-Press P or p : Prefetch Abort Test\n");
	printf("\t-Press V or v : Random Value Write-Read Test\n");
	printf("\t-Press C or c : Clear reboot reason\n");
	printf("\t-Press R or r : Check previous reboot reason\n");
	printf("\t-Press X or x : Terminate Tests.\n");
}
#ifdef CONFIG_WATCHDOG
static void watchdog_test(void)
{
	int fd;
	int ret;

	fd = open(CONFIG_WATCHDOG_DEVPATH, O_RDWR);
	if (fd < 0) {
		printf("ERROR: Fail to open %s\n", CONFIG_WATCHDOG_DEVPATH);
		return;
	}

	ret = ioctl(fd, WDIOC_SETTIMEOUT, 1000);
	if (ret != OK) {
		printf("ERROR: Fail to set watchdog timeout.\n");
		close(fd);
		return;
	}

	ret = ioctl(fd, WDIOC_START, 0);
	if (ret != OK) {
		printf("ERROR: Fail to start watchdog.\n");
		close(fd);
		return;
	}

	/* Wait until watchdog timeout */
	while (1);
}
#endif

#ifdef CONFIG_MM_ASSERT_ON_FAIL
static void memory_alloc_fail_test(void)
{
	struct mallinfo data;
	int largest_size;
	int *alloc_ptr;

#ifdef CONFIG_CAN_PASS_STRUCTS
	data = mallinfo();
#else
	mallinfo(&data);
#endif
	largest_size = data.mxordblk;
	/* Try to allocate not available size. */
	alloc_ptr = (int *)malloc(largest_size + 1);

	/* This line cannot be reached,
	 * because there will be assert on memory allocation failure from malloc.
	 */
	free(alloc_ptr);
}
#endif

static void prefetch_abort_test(void)
{
	/* The address allocated from heap can't be code. This will lead prefetch abort. */
	int *ptr = (int *)malloc(1024);
	int (*fp)(int, int) = ptr;
	fp(3, 4);
}

static void reboot_board(void)
{
#ifdef CONFIG_LIB_BOARDCTL
	printf("Board will be reset automatically.\n");
	boardctl(BOARDIOC_RESET, 0);
#else
	printf("Please reboot the board manually.\n");
#endif
}

/****************************************************************************
 * reboot_reason_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int reboot_reason_main(int argc, char *argv[])
#endif
{
	int ch;
	bool is_testing = true;

	printf("Reboot Reason Test Start!\n");

	while (is_testing) {
		display_reboot_reason_option();
		ch = getchar();
		switch (ch) {
		case 'H':
		case 'h':
			printf("Expected reboot reason after hw reset is : %d\n", REBOOT_SYSTEM_HW_RESET);
			printf("Press HW Reset Button.\n");
			/* Wait for pressing the hw reset button. */
			while (1);
			break;
#ifdef CONFIG_MM_ASSERT_ON_FAIL
		case 'M':
		case 'm':
			/* Test for Memory Allocation Fail */
			printf("After Memory Allocation Fail, Expected Reboot reason is %d\n", REBOOT_SYSTEM_MEMORYALLOCFAIL);
			memory_alloc_fail_test();
			break;
#endif
#ifdef CONFIG_WATCHDOG
		case 'W':
		case 'w':
			/* Test for Watchdog Reset */
			printf("After watchdog reset, Expected Reboot reason is %d\n", REBOOT_SYSTEM_WATCHDOG);
			watchdog_test();
			break;
#endif
		case 'P':
		case 'p':
			/* Test for Prefetch Abort */
			printf("After Prefetch Abort, Expected Reboot reason is %d\n", REBOOT_SYSTEM_PREFETCHABORT);
			prefetch_abort_test();
			break;
		case 'V':
		case 'v':
			/* Test for Temp value write-read */
			printf("Write Test Reboot Reason : %d\n", REBOOT_REASON_TEST_VAL);
			prctl(PR_REBOOT_REASON_WRITE, REBOOT_REASON_TEST_VAL);
			reboot_board();
			break;
		case 'C':
		case 'c':
			/* Clear the previous reboot reason with REBOOT_REASON_INITIALIZED(0) */
			printf("Will write %d and then will clear.\n", REBOOT_REASON_TEST_VAL);
			printf("Expected reboot reason after reset is %d, otherwise wrong operation.\n", REBOOT_UNKNOWN);
			prctl(PR_REBOOT_REASON_WRITE, REBOOT_REASON_TEST_VAL);
			prctl(PR_REBOOT_REASON_CLEAR);
			reboot_board();
			break;
		case 'R':
		case 'r':
			/* Read the previous reboot reason */
			printf("\nPrevious Reboot Reason is : %d\n", prctl(PR_REBOOT_REASON_READ));
			break;
		case 'X':
		case 'x':
			printf("Test will be finished.\n");
			is_testing = false;
			break;
		default:
			printf("Invalid Scenario.\n");
			break;
		}
	}

	return 0;
}
