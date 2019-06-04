/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <unistd.h>
#include <errno.h>
#include <sched.h>
#include <semaphore.h>

#include <messaging/messaging.h>

#define SYNC_BLOCK_PORT "sync_block_port"
#define SYNC_BLOCK_DATA "sync_block_msg"
#define SYNC_BLOCK_REPLY "reply_msg"

#define NOREPLY_NONBLOCK_PORT "noreply_nonblock"
#define NOREPLY_NONBLOCK_DATA "noreply_data"

#define MULTICAST_PORT "multicast_port"
#define MULTICAST_DATA "multi_msg"

#define CHECK_PORT "check_port"
#define CHECK_MSG  "do_next"

#define BUFFER_SIZE 20
#define MSG_PRIO 10

static sem_t sender_sem;

static int sync_fail_cnt;
static int noreply_fail_cnt;
static int multicast_fail_cnt;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
static int sync_send_test(void)
{
	int ret;
	msg_send_data_t send_data;
	msg_recv_buf_t reply_data;

	sem_wait(&sender_sem);

	send_data.priority = MSG_PRIO;
	send_data.msg = SYNC_BLOCK_DATA;
	send_data.msglen = sizeof(SYNC_BLOCK_DATA);

	reply_data.buflen = BUFFER_SIZE;
	reply_data.buf = (char *)malloc(BUFFER_SIZE);
	if (reply_data.buf == NULL) {
		sem_post(&sender_sem);
		printf("[WIFI] Fail to sync send msg : out of memory.\n");
		return ERROR;
	}

	printf("[WIFI] - Send [%s] data with sync mode.\n", SYNC_BLOCK_DATA);
	ret = messaging_send_sync(SYNC_BLOCK_PORT, &send_data, &reply_data);
	if (ret != OK) {
		free(reply_data.buf);
		sem_post(&sender_sem);
		printf("[WIFI] Fail to sync send msg.\n");
		return ERROR;
	}

	printf("[WIFI] Success to recv reply msg. [%s]\n", (char *)reply_data.buf);
	free(reply_data.buf);

	sem_post(&sender_sem);
	return 0;
}

static int noreply_send_test(void)
{
	int ret;
	msg_send_data_t param;

	sem_wait(&sender_sem);

	param.priority = MSG_PRIO;
	param.msg = NOREPLY_NONBLOCK_DATA;
	param.msglen = sizeof(NOREPLY_NONBLOCK_DATA);

	printf("[WIFI] - Send [%s] with noreply mode.\n", NOREPLY_NONBLOCK_DATA);
	ret = messaging_send(NOREPLY_NONBLOCK_PORT, &param);
	if (ret != OK) {
		sem_post(&sender_sem);
		printf("[WIFI] Fail to noreply send msg.\n");
		return ERROR;
	}

	sem_post(&sender_sem);
	return 0;
}

static int multicast_send_test(void)
{
	int ret;
	msg_send_data_t data;

	sem_wait(&sender_sem);

	data.priority = MSG_PRIO;
	data.msg = MULTICAST_DATA;
	data.msglen = sizeof(MULTICAST_DATA);

	printf("[WIFI] - Send Multicast [%s] data.\n", MULTICAST_DATA);
	ret = messaging_multicast(MULTICAST_PORT, &data);
	if (ret != 3) {
		sem_post(&sender_sem);
		printf("[WIFI] Fail to send multicast.\n");
		return ERROR;
	}

	sem_post(&sender_sem);
	return 0;
}

static void check_can_do_next(void)
{
	msg_send_data_t param;

	param.priority = MSG_PRIO;
	param.msg = CHECK_MSG;
	param.msglen = sizeof(CHECK_MSG);

	(void)messaging_send(CHECK_PORT, &param);
}

void messaging_test(void)
{
	int ret;
	int rep_cnt;

	int repetition_num = CONFIG_MESSAGING_TEST_REPETITION_NUM;

	if (repetition_num < 0) {
		repetition_num = 10;
	}

	sem_init(&sender_sem, 0, 1);

	for (rep_cnt = 1; rep_cnt <= repetition_num; rep_cnt++) {
		printf("\n\n=== Messaging F/W Test : %d-th iteration. ===\n", rep_cnt);

		printf("\n--- Sync Send & Block Receive Test. ---\n");
		ret = sync_send_test();
		if (ret != OK) {
			sync_fail_cnt++;
		}

		/* Wait for finishing previous test. */
		sleep(1);

		printf("\n--- NoReply Send & NonBlock Receive Test. ---\n");
		ret = noreply_send_test();
		if (ret != OK) {
			noreply_fail_cnt++;
		}

		/* Wait for finishing previous test. */
		sleep(1);

		printf("\n--- Multicast Send & Receive Test. ---\n");
		ret = multicast_send_test();
		if (ret != OK) {
			multicast_fail_cnt++;
		}

		check_can_do_next();

		sleep(3);
		if (CONFIG_MESSAGING_TEST_REPETITION_NUM < 0) {
			/* Test will run infinitely. */
			rep_cnt = 1;
		}
	}

	printf("<<Sender>> Summary\n");
	printf(" - Total : %d iteration\n", CONFIG_MESSAGING_TEST_REPETITION_NUM);
	printf(" - Sync mode : %d fails\n", sync_fail_cnt);
	printf(" - Noreply mode : %d fails\n", noreply_fail_cnt);
	printf(" - Multicast mode : %d fails\n", multicast_fail_cnt);

	sem_wait(&sender_sem);
	sem_destroy(&sender_sem);
}
