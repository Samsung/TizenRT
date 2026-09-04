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
 * @file test_mq_leak.c
 * @brief Test: MQ leak after close (true positive detection)
 *
 * This test allocates a block, sends its pointer via MQ, then closes the
 * MQ WITHOUT receiving the message. The MQ infrastructure is torn down,
 * but the app-allocated buffer is NOT freed. This is a TRUE leak.
 *
 * Expected behavior:
 *   - Current tool: MAY MISS the leak (false negative)
 *     Reason: After mq_close(), the kernel frees the MQ message buffer.
 *     However, the freed memory may still contain stale pointer data.
 *     heap_check() scans ALL heap blocks as roots, so if the freed MQ
 *     buffer still contains the pointer value, it may mark the app
 *     block as reachable even though the MQ is gone.
 *
 *   - New tool: DETECTS the leak (true positive)
 *     Reason: After mq_close(), the MQ message buffer is freed and no
 *     longer allocated. It won't appear in the heap walk. The app block
 *     has no path from any true root → correctly reported as UNREACHABLE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>

#define TEST_MQ_NAME    "/mem_leak_test_mq_leak"
#define TEST_MQ_PRIO    1
#define TEST_BLOCK_SIZE 128

/* Keep a reference to print the address after MQ close */
static void *g_leaked_buf = NULL;

void test_mq_leak_after_close(void)
{
	mqd_t mqd;
	struct mq_attr attr;
	int ret;

	printf("\n");
	printf("========================================================\n");
	printf("  TEST 4: MQ Leak After Close (True Positive)\n");
	printf("========================================================\n");
	printf("\n");
	printf("  Description:\n");
	printf("    Allocate a block, send pointer via MQ, then close MQ\n");
	printf("    WITHOUT receiving the message. The MQ is torn down\n");
	printf("    but the app buffer is NOT freed. This is a TRUE leak.\n");
	printf("\n");

	/* Configure MQ attributes */
	attr.mq_maxmsg = 4;
	attr.mq_msgsize = sizeof(void *);
	attr.mq_flags = 0;
	attr.mq_curmsgs = 0;

	/* Open (create) the message queue */
	mqd = mq_open(TEST_MQ_NAME, O_CREAT | O_RDWR, 0666, &attr);
	if (mqd == (mqd_t)-1) {
		printf("  ERROR: Failed to create message queue\n");
		return;
	}

	/* Allocate the block that will be leaked */
	g_leaked_buf = malloc(TEST_BLOCK_SIZE);
	if (!g_leaked_buf) {
		printf("  ERROR: Failed to allocate test buffer\n");
		mq_close(mqd);
		return;
	}

	/* Initialize with recognizable pattern */
	memset(g_leaked_buf, 0xEE, TEST_BLOCK_SIZE);

	printf("  Allocated buffer at: %p (size: %d bytes)\n",
		g_leaked_buf, TEST_BLOCK_SIZE);
	printf("  Message queue name: %s\n", TEST_MQ_NAME);
	printf("\n");

	/* Send the pointer via MQ */
	ret = mq_send(mqd, (const char *)&g_leaked_buf, sizeof(void *), TEST_MQ_PRIO);
	if (ret < 0) {
		printf("  ERROR: mq_send failed\n");
		free(g_leaked_buf);
		mq_close(mqd);
		return;
	}

	printf("  Sent buffer pointer via mq_send().\n");
	printf("  At this point, buffer is REACHABLE via kernel MQ.\n");
	printf("\n");

	/* Close the MQ WITHOUT receiving the message.
	 * This tears down the MQ infrastructure:
	 *   - mq_des is freed
	 *   - mqueue_inode_s is freed (if last reference)
	 *   - Message data buffers are freed
	 *
	 * BUT: The app-allocated buffer (g_leaked_buf) is NOT freed.
	 * The kernel doesn't know it's an app heap pointer - it was just
	 * a payload value copied into the MQ message.
	 *
	 * After mq_close():
	 *   - No kernel structure holds a reference to g_leaked_buf
	 *   - The app buffer is a TRUE LEAK
	 */
	ret = mq_close(mqd);
	if (ret < 0) {
		printf("  WARNING: mq_close failed (continuing anyway)\n");
	}

	/* Also unlink to fully remove the MQ */
	mq_unlink(TEST_MQ_NAME);

	printf("  Closed and unlinked MQ WITHOUT receiving message.\n");
	printf("  Kernel MQ infrastructure torn down.\n");
	printf("  App buffer at %p is now ORPHANED (true leak).\n", g_leaked_buf);
	printf("\n");

	/* Clear local reference */
	g_leaked_buf = NULL;
	printf("  Cleared local pointer (g_leaked_buf = NULL)\n");
	printf("  No root or kernel structure holds a reference to the buffer.\n");
	printf("\n");

	printf("  >>> Now run: mem_leak <<<\n");
	printf("\n");
	printf("  NOTE: Look for a 128-byte block with pattern 0xEE.\n");
	printf("  If found as LEAK, the tool correctly detected it.\n");
	printf("  If NOT found, the tool missed it (stale pointer issue).\n");
	printf("\n");

	/* Intentionally NOT freeing - this IS a leak */
}
