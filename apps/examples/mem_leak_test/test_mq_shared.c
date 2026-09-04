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
 * @file test_mq_shared.c
 * @brief Test: Message queue shared block (false negative prevention)
 *
 * This test allocates a block, sends a pointer to it via a POSIX message
 * queue, and does NOT receive it. The block should appear REACHABLE because
 * the kernel holds a reference to it via the MQ infrastructure.
 *
 * Expected behavior:
 *   - Current tool: Block NOT reported as leak (REACHABLE)
 *     Reason: heap_check() scans kernel heap, finds the pointer in the
 *     MQ message buffer, marks the app block as reachable.
 *
 *   - New tool: Block NOT reported as leak (REACHABLE)
 *     Reason: Kernel heap is scanned as a root source. The MQ message
 *     buffer in kernel heap contains a pointer to the app block.
 *     Transitive marking reaches the app block.
 *
 * This test verifies that the kernel heap scanning mechanism works
 * correctly in BOTH old and new implementations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>

#define TEST_MQ_NAME    "/mem_leak_test_mq"
#define TEST_MQ_PRIO    1
#define TEST_BLOCK_SIZE 128

/* Pointer that will be sent via MQ, then cleared locally */
static void *g_shared_buf = NULL;

void test_mq_shared_block(void)
{
	mqd_t mqd;
	struct mq_attr attr;
	int ret;

	printf("\n");
	printf("========================================================\n");
	printf("  TEST 2: Message Queue Shared Block\n");
	printf("========================================================\n");
	printf("\n");
	printf("  Description:\n");
	printf("    Allocate a block, send its pointer via message queue.\n");
	printf("    Do NOT receive the message. The block should be\n");
	printf("    REACHABLE because the kernel MQ holds a reference.\n");
	printf("\n");

	/* Configure MQ attributes */
	attr.mq_maxmsg = 4;
	attr.mq_msgsize = sizeof(void *);
	attr.mq_flags = 0;
	attr.mq_curmsgs = 0;

	/* Open (create) the message queue */
	mqd = mq_open(TEST_MQ_NAME, O_CREAT | O_WRONLY, 0666, &attr);
	if (mqd == (mqd_t)-1) {
		printf("  ERROR: Failed to create message queue\n");
		return;
	}

	/* Allocate the block that will be shared */
	g_shared_buf = malloc(TEST_BLOCK_SIZE);
	if (!g_shared_buf) {
		printf("  ERROR: Failed to allocate test buffer\n");
		mq_close(mqd);
		return;
	}

	/* Initialize with a recognizable pattern */
	memset(g_shared_buf, 0xCC, TEST_BLOCK_SIZE);

	printf("  Allocated shared buffer at: %p (size: %d bytes)\n",
		g_shared_buf, TEST_BLOCK_SIZE);
	printf("  Message queue name: %s\n", TEST_MQ_NAME);
	printf("\n");

	/* Send the pointer via MQ - kernel copies the pointer value
	 * into a kernel-heap-allocated message buffer.
	 *
	 * Reachability chain after mq_send():
	 *   task_group->tg_msgdesq → mq_des → mqueue_inode_s → msglist
	 *   → message buffer (kernel heap) → contains &g_shared_buf value
	 *   → points to app heap block
	 */
	ret = mq_send(mqd, (const char *)&g_shared_buf, sizeof(void *), TEST_MQ_PRIO);
	if (ret < 0) {
		printf("  ERROR: mq_send failed\n");
		free(g_shared_buf);
		mq_close(mqd);
		return;
	}

	printf("  Sent buffer pointer via mq_send().\n");
	printf("  The pointer is now stored in kernel heap (MQ message buffer).\n");
	printf("\n");

	/* Clear local reference - the only reference is now in the kernel MQ */
	g_shared_buf = NULL;
	printf("  Cleared local pointer (g_shared_buf = NULL)\n");
	printf("  The only reference to the buffer is now in the kernel MQ.\n");
	printf("\n");

	printf("  >>> Now run: mem_leak <<<\n");
	printf("\n");
	printf("  NOTE: After running the checker, you can clean up by:\n");
	printf("    - Receiving the message: write a small receiver task\n");
	printf("    - Or closing the MQ (see Test 4 for that scenario)\n");
	printf("\n");

	/* Do NOT close MQ here - we want the buffer to remain reachable.
	 * The MQ remains open, keeping the buffer in kernel MQ. */
}
