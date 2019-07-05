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
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>
#include <semaphore.h>

#include <messaging/messaging.h>

#define SYNC_BLOCK_PORT "sync_block_port"
#define SYNC_BLOCK_DATA "sync_block"
#define SYNC_BLOCK_REPLY "reply"

#define NOREPLY_NONBLOCK_PORT "noreply_nonblock"
#define NOREPLY_NONBLOCK_DATA "noreply"

#define MULTICAST_PORT "multicast_port"
#define MULTICAST_DATA "multi"

#define CHECK_PORT "check_port"

#define BUFFER_SIZE 20
#define MSG_PRIO 10
#define STACKSIZE 2048
#define TASK_PRIO 221

#define MULTICAST_RECEIVER_NUM 3

static sem_t test_sem;
static int sync_fail_cnt;
static int noreply_fail_cnt;
static int multicast_fail_cnt;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
static void multicast_send_test(void)
{
	int ret;
	msg_send_data_t data;

	data.priority = MSG_PRIO;
	data.msg = MULTICAST_DATA;
	data.msglen = strlen(MULTICAST_DATA) + 1;

	printf("\n--Multicast Test--\n");
	printf("[W] -Send Multicast [%s].\n", MULTICAST_DATA);
	ret = messaging_multicast(MULTICAST_PORT, &data);
	if (ret != MULTICAST_RECEIVER_NUM) {
		multicast_fail_cnt++;
		printf("[W] Fail to send multicast.\n");
		return;
	}
}

static void sync_send_test(void)
{
	int ret;
	msg_send_data_t send_data;
	msg_recv_buf_t reply_data;

	send_data.priority = MSG_PRIO;
	send_data.msg = SYNC_BLOCK_DATA;
	send_data.msglen = strlen(SYNC_BLOCK_DATA) + 1;

	reply_data.buflen = BUFFER_SIZE;
	reply_data.buf = (char *)malloc(BUFFER_SIZE);
	if (reply_data.buf == NULL) {
		sync_fail_cnt++;
		printf("[W] Fail to sync send msg : out of memory.\n");
		return;
	}

	printf("\n--SyncSend & BlockRecv--\n");
	printf("[W] -Send sync [%s].\n", SYNC_BLOCK_DATA);
	ret = messaging_send_sync(SYNC_BLOCK_PORT, &send_data, &reply_data);
	if (ret != OK) {
		sync_fail_cnt++;
		free(reply_data.buf);
		printf("[W] Fail to sync send msg.\n");
		return;
	}

	printf("[W] OK : recv reply [%s].\n", (char *)reply_data.buf);
	free(reply_data.buf);
}

static void noreply_send_test(void)
{
	int ret;
	msg_send_data_t send_data;

	send_data.priority = MSG_PRIO;
	send_data.msg = NOREPLY_NONBLOCK_DATA;
	send_data.msglen = strlen(NOREPLY_NONBLOCK_DATA) + 1;

	printf("\n--NoReplySend & NonBlockRecv--\n");
	printf("[W] -Send noreply [%s].\n", NOREPLY_NONBLOCK_DATA);
	ret = messaging_send(NOREPLY_NONBLOCK_PORT, &send_data);
	if (ret != OK) {
		noreply_fail_cnt++;
		printf("[W] Fail to noreply send msg.\n");
		return;
	}
}

static int check_do_next(char *control_msg)
{
	int ret;
	msg_recv_buf_t control_data;

	control_data.buf = (char *)zalloc(BUFFER_SIZE);
	if (control_data.buf == NULL) {
		printf("[W] Fail to test : out of memory.\n");
		return ERROR;
	}
	control_data.buflen = BUFFER_SIZE;

	ret = messaging_recv_block(CHECK_PORT, &control_data);
	if (ret < 0) {
		free(control_data.buf);
		printf("[W] Fail to recv test control msg.\n");
		return ERROR;
	}

	if (strncmp(control_data.buf, control_msg, strlen(control_msg) + 1) != 0) {
		free(control_data.buf);
		printf("[W] Fail : test control msg error.\n");
		return ERROR;
	}

	free(control_data.buf);
	return OK;
}

static int msg_sender(int argc, FAR char *argv[])
{
	int ret;

	/* 1st test : messaging_multicast */
	multicast_send_test();

	ret = check_do_next("1st_done");
	if (ret != OK) {
		return ERROR;
	}

	/* 2nd test : messaging_send (noreply mode) */
	noreply_send_test();

	ret = check_do_next("2nd_done");
	if (ret != OK) {
		return ERROR;
	}

	/* 3rd test : messaging_send_sync */
	sync_send_test();

	sem_post(&test_sem);
	return OK;
}

static void show_test_result(void)
{
	sleep(1);
	printf("-------------------------\n");
	printf("  MESSAGING SEND RESULT  \n");
	printf("-------------------------\n");
	printf("multicast     fail : %d\n", multicast_fail_cnt);
	printf("send_sync     fail : %d\n", sync_fail_cnt);
	printf("send noreply  fail : %d\n", noreply_fail_cnt);
	printf("-------------------------\n");
}

void messaging_test(void)
{
	int ret;
	int rep_cnt = 1;

	sem_init(&test_sem, 0, 0);

#if CONFIG_MESSAGING_TEST_REPETITION_NUM > 0
	for (rep_cnt = 1; rep_cnt <= CONFIG_MESSAGING_TEST_REPETITION_NUM;) {
#else
	while (1) {
#endif
		sleep(1);
		printf("\n\n=== Messaging F/W Test : %d-th iteration. ===\n", rep_cnt++);
		ret = task_create("msg_sender", TASK_PRIO, STACKSIZE, msg_sender, NULL);
		if (ret < 0) {
			sem_destroy(&test_sem);
			printf("[W] Fail to create task.\n");
			return;
		}

		sem_wait(&test_sem);
	}
	sem_destroy(&test_sem);
	show_test_result();
}
