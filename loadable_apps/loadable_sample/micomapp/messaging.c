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
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <messaging/messaging.h>

#define SYNC_BLOCK_PORT "sync_block_port"
#define SYNC_BLOCK_DATA "sync_block"
#define SYNC_BLOCK_REPLY "reply"

#define NOREPLY_NONBLOCK_PORT "noreply_nonblock"
#define NOREPLY_NONBLOCK_DATA "noreply"

#define MULTICAST_PORT "multicast_port"

#define CHECK_PORT "check_port"

#define BUFFER_SIZE 20
#define MSG_PRIO 10
#define TASK_PRIO 221
#define STACKSIZE 4096

static sem_t recv_sem;
static sem_t test_sem;

static int block_fail_cnt;
static int nonblock_fail_cnt;
static int multicast_fail_cnt;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
static int block_recv_test(void)
{
	int ret;
	msg_recv_buf_t recv_data;
	msg_send_data_t reply_data;

	recv_data.buf = (char *)malloc(BUFFER_SIZE);
	if (recv_data.buf == NULL) {
		block_fail_cnt++;
		printf("[M] Fail to receive, because out of memory.\n");
		return ERROR;
	}
	recv_data.buflen = BUFFER_SIZE;

	ret = messaging_recv_block(SYNC_BLOCK_PORT, &recv_data);
	if (ret < 0) {
		free(recv_data.buf);
		block_fail_cnt++;
		printf("[M] Fail to receive with block mode.\n");
		return ERROR;
	}

	printf("[M] OK: BlockRecv. [%s]\n", (char *)recv_data.buf);
	printf("[M] - Reply [%s].\n", SYNC_BLOCK_REPLY);

	reply_data.msg = SYNC_BLOCK_REPLY;
	reply_data.msglen = strlen(SYNC_BLOCK_REPLY) + 1;
	if (ret == MSG_REPLY_REQUIRED) {
		ret = messaging_reply(SYNC_BLOCK_PORT, recv_data.sender_pid, &reply_data);
		if (ret != OK) {
			free(recv_data.buf);
			block_fail_cnt++;
			printf("[M] Fail to reply.\n");
			return ERROR;
		}
	} else {
		printf("[M] Will not reply, because received msg is not sync type. %d\n", ret);
	}

	free(recv_data.buf);
	return OK;
}

static void recv_callback(msg_reply_type_t msg_type, msg_recv_buf_t *recv_data, void *cb_data)
{
	if (recv_data == NULL) {
		printf("[M] Fail to receive unicast msg with nonblock mode.\n");
		return;
	}
	printf("[M] OK: NonblockRecv from PID(%d) - [%s]\n", recv_data->sender_pid, (char *)recv_data->buf);
}

static void multi_recv_callback(msg_reply_type_t msg_type, msg_recv_buf_t *recv_data, void *cb_data)
{
	if (recv_data == NULL) {
		multicast_fail_cnt++;
		printf("[M] Fail to receive multicast msg(nonblock).\n");
		return;
	}
	printf("[M] OK: Multicast(nonblock)Recv [%s].\n", (char *)recv_data->buf);
}

static void set_messaging_signal(sigset_t *sigset)
{

	sigfillset(sigset);
	sigdelset(sigset, SIGMSG_MESSAGING);
	sigdelset(sigset, SIGUSR1);
	(void)sigprocmask(SIG_SETMASK, sigset, NULL);
}

static int multi_recv_nonblock(int argc, FAR char *argv[])
{
	int ret;
	msg_callback_info_t cb_info;
	msg_recv_buf_t data;
	sigset_t sigset;

	set_messaging_signal(&sigset);

	cb_info.cb_func = multi_recv_callback;
	cb_info.cb_data = NULL;

	data.buf = (char *)malloc(BUFFER_SIZE);
	if (data.buf == NULL) {
		multicast_fail_cnt++;
		printf("[M] Fail to recv nonblock multicast msg : out of memory.\n");
		return ERROR;
	}
	data.buflen = BUFFER_SIZE;

	ret = messaging_recv_nonblock(MULTICAST_PORT, &data, &cb_info);
	if (ret != OK) {
		free(data.buf);
		multicast_fail_cnt++;
		printf("[M] Fail to receive multicast with non-block mode.\n");
		return ERROR;
	}

	/* Wait not to finish this task, because of receiving data through the callback. */
	(void)sigsuspend(&sigset);

	free(data.buf);

	ret = messaging_cleanup(MULTICAST_PORT);
	if (ret != OK) {
		multicast_fail_cnt++;
		printf("[M] Fail to cleanup MULTICAST_PORT.\n");
		return ERROR;
	}

	sem_post(&recv_sem);
	return OK;
}

static int multi_recv_block(int argc, FAR char *argv[])
{
	int ret;
	msg_recv_buf_t msg;

	msg.buflen = BUFFER_SIZE;
	msg.buf = (char *)zalloc(BUFFER_SIZE);
	if (msg.buf == NULL) {
		multicast_fail_cnt++;
		printf("[M] Fail to malloc for multi_recv buffer.\n");
		return ERROR;
	}

	ret = messaging_recv_block(MULTICAST_PORT, &msg);
	if (ret < 0) {
		free(msg.buf);
		multicast_fail_cnt++;
		printf("[M] Fail to receive multicast message.\n");
		return ERROR;
	}

	printf("[M] OK: Multicast(block)Recv [%s].\n", msg.buf);
	free(msg.buf);
	
	return OK;
}

static int multicast_recv_test(void)
{
	int recv_pid;

	recv_pid = task_create("multi_recv_nonblock", TASK_PRIO, STACKSIZE, multi_recv_nonblock, NULL);
	if (recv_pid < 0) {
		multicast_fail_cnt++;
		printf("[M] Fail to create multi_recv_nonblock task. %d\n", errno);
		return ERROR;
	}

	recv_pid = task_create("multi_recv_block1", TASK_PRIO, STACKSIZE, multi_recv_block, NULL);
	if (recv_pid < 0) {
		multicast_fail_cnt++;
		printf("[M] Fail to create multi_recv_block1 task. %d\n", errno);
		return ERROR;
	}

	recv_pid = task_create("multi_recv_block2", TASK_PRIO, STACKSIZE, multi_recv_block, NULL);
	if (recv_pid < 0) {
		multicast_fail_cnt++;
		printf("[M] Fail to create multi_recv_block2 task. %d\n", errno);
		return ERROR;
	}

	return OK;
}

static int nonblock_recv_test(void)
{
	int ret;
	msg_callback_info_t cb_info;
	msg_recv_buf_t data;
	sigset_t sigset;

	set_messaging_signal(&sigset);

	cb_info.cb_func = recv_callback;
	cb_info.cb_data = NULL;

	data.buf = (char *)malloc(BUFFER_SIZE);
	if (data.buf == NULL) {
		nonblock_fail_cnt++;
		printf("[M] Fail to recv nonblock msg : out of memory.\n");
		return ERROR;
	}
	data.buflen = BUFFER_SIZE;

	ret = messaging_recv_nonblock(NOREPLY_NONBLOCK_PORT, &data, &cb_info);
	if (ret != OK) {
		free(data.buf);
		nonblock_fail_cnt++;
		printf("[M] Fail to receive with nonblock mode.\n");
		return ERROR;
	}

	/* Wait not to finish this task, because of receiving data through the callback. */
	(void)sigsuspend(&sigset);

	free(data.buf);

	ret = messaging_cleanup(NOREPLY_NONBLOCK_PORT);
	if (ret != OK) {
		nonblock_fail_cnt++;
		printf("[M] Fail to cleanup NOREPLY_NONBLOCK_PORT.\n");
		return ERROR;
	}

	return OK;
}

static int send_do_next(char *control_msg)
{
	int ret;
	msg_send_data_t send_data;

	send_data.priority = MSG_PRIO;
	send_data.msglen = strlen(control_msg) + 1;
	send_data.msg = control_msg;

	ret = messaging_send(CHECK_PORT, &send_data);
	if (ret != OK) {
		printf("[M] Fail to send control msg. : %s\n", control_msg);
		return ERROR;
	}

	return OK;
}

static int msg_receiver(int argc, FAR char *argv[])
{
	int ret;

	sem_init(&recv_sem, 0, 0);

	/* 1st test : multicast recv
	 *	    - 2 receivers of messaging_recv_block, 1 receiver of messaging_recv_nonblock
	 */
	multicast_recv_test();

	/* Wait until multicast test will be finished. */
	sem_wait(&recv_sem);
	sem_destroy(&recv_sem);

	ret = send_do_next("1st_done");
	if (ret != OK) {
		return ERROR;
	}

	/* 2nd test : messaging_recv_nonblock */
	nonblock_recv_test();

	ret = send_do_next("2nd_done");
	if (ret != OK) {
		return ERROR;
	}

	/* 3rd test : messaging_recv_block */
	block_recv_test();

	sem_post(&test_sem);
	return OK;
}

static void show_test_result(void)
{
	printf("-------------------------\n");
	printf("  MESSAGING RECV RESULT  \n");
	printf("-------------------------\n");
	printf("multicast     fail : %d\n", multicast_fail_cnt);
	printf("recv_block    fail : %d\n", block_fail_cnt);
	printf("recv_nonblock fail : %d\n", nonblock_fail_cnt);
	printf("-------------------------\n");
}

void messaging_test(void)
{
	int ret;
#if CONFIG_MESSAGING_TEST_REPETITION_NUM > 0
	int rep_cnt;
#endif

	sem_init(&test_sem, 0, 0);

#if CONFIG_MESSAGING_TEST_REPETITION_NUM > 0
	for (rep_cnt = 1; rep_cnt <= CONFIG_MESSAGING_TEST_REPETITION_NUM; rep_cnt++) {
#else
	while (1) {
#endif
		ret = task_create("msg_receiver", TASK_PRIO, STACKSIZE, msg_receiver, NULL);
		if (ret < 0) {
			sem_destroy(&test_sem);
			printf("[M] Fail to create task.\n");
			return;
		}
		sem_wait(&test_sem);
	}
	sem_destroy(&test_sem);
	show_test_result();
}
