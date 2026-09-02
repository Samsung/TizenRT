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
 * @file test_kernel_to_app.c
 * @brief Test D4: Kernel heap pointer to app block
 *
 * Allocates a block on the app heap, then sends the pointer via message
 * queue so the kernel heap holds a reference to it.
 * The block is reachable via kernel heap → NO LEAK.
 * Then close the MQ without receiving → kernel frees its structures,
 * app block becomes orphaned → LEAK.
 *
 * This is similar to Test 4 (MQ leak) but explicitly tests the
 * kernel→app cross-heap reference scanning.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sched.h>
#include <sys/types.h>


static void *g_k2a_buf;
static mqd_t g_k2a_mq;

void test_kernel_to_app(void)
{
	struct mq_attr attr;
	char msg_buf[sizeof(void *)];
	int prio = 1;

	printf("\n");
	printf("========================================================\n");
	printf("  TEST D4: Kernel Heap Pointer to App Block\n");
	printf("========================================================\n");

	printf("\n");
	printf("  Description:\n");
	printf("    Allocate block on app heap, send pointer via MQ.\n");
	printf("    Kernel MQ holds reference → block reachable → NO LEAK.\n");
	printf("    Then close MQ without receiving → kernel frees MQ,\n");
	printf("    app block orphaned → LEAK.\n");
	printf("\n");

	/* Set up MQ attributes */
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(void *);
	attr.mq_flags = 0;
	attr.mq_curmsgs = 0;

	/* Create MQ */
	g_k2a_mq = mq_open("/k2a_test_mq", O_CREAT | O_WRONLY, 0666, &attr);
	if (g_k2a_mq == (mqd_t)-1) {
		printf("  ERROR: mq_open failed\n");
		return;
	}

	/* Allocate app heap block */
	g_k2a_buf = malloc(256);
	if (!g_k2a_buf) {
		printf("  ERROR: malloc failed\n");
		mq_close(g_k2a_mq);
		mq_unlink("/k2a_test_mq");
		return;
	}

	memset(g_k2a_buf, 0xD4, 256);
	printf("  Allocated app block: %p (256 bytes, pattern 0xD4)\n", g_k2a_buf);

	/* Send pointer via MQ — kernel MQ now holds a copy */
	memcpy(msg_buf, &g_k2a_buf, sizeof(void *));
	if (mq_send(g_k2a_mq, msg_buf, sizeof(void *), prio) < 0) {
		printf("  ERROR: mq_send failed\n");
		free(g_k2a_buf);
		mq_close(g_k2a_mq);
		mq_unlink("/k2a_test_mq");
		return;
	}
	printf("  Sent pointer via MQ (kernel holds reference)\n");

	/* Clear local reference */
	g_k2a_buf = NULL;
	printf("  Cleared local pointer (g_k2a_buf = NULL)\n");
	printf("  Only reference is now in kernel MQ structure.\n");
	printf("\n");

	/* Now close MQ without receiving — creates the leak */
	mq_close(g_k2a_mq);
	mq_unlink("/k2a_test_mq");
	g_k2a_mq = (mqd_t)-1;
	printf("  Closed and unlinked MQ WITHOUT receiving message.\n");
	printf("  Kernel MQ infrastructure torn down.\n");
	printf("  App block is now orphaned (true leak).\n");
	printf("\n");

	printf("  >>> Run mem_leak tool from TASH: <<<\n");
	printf("  TASH> mem_leak\n");
	printf("\n");
	printf("  NOTE: Look for a 256-byte block with pattern 0xD4.\n");
	printf("  NOTE: This test may be missed due to stale pointers\n");
	printf("  in freed kernel MQ memory. See mem_leak_test_results.md\n");
	printf("  for details on the stale pointer limitation.\n");
	printf("\n");
}
