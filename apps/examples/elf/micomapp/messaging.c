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
#include <messaging/messaging.h>

#define SYNC_BLOCK_PORT "sync_block_port"
#define SYNC_BLOCK_DATA "sync_block_msg"
#define SYNC_BLOCK_REPLY "reply_msg"

#define NOREPLY_NONBLOCK_PORT "noreply_nonblock"
#define NOREPLY_NONBLOCK_DATA "noreply_data"

#define MULTICAST_PORT "multicast_port"

#define CHECK_PORT "check_port"
#define CHECK_MSG  "do_next"

#define BUFFER_SIZE 20
#define MSG_PRIO 10
#define TASK_PRIO 130
#define STACKSIZE 2048

static sem_t nonblock_sem;
static sem_t multi_sem;

static int multicast_fail_cnt;
static int block_fail_cnt;
static int nonblock_fail_cnt;
/****************************************************************************
 * Public Functions
 ****************************************************************************/
static int block_recv(int argc, FAR char *argv[])
{
	int ret;
	msg_recv_buf_t recv_data;
	msg_send_data_t reply_data;

	recv_data.buf = (char *)malloc(BUFFER_SIZE);
	if (recv_data.buf == NULL) {
		printf("[MICOM] Fail to receive, because out of memory.\n");
		return ERROR;
	}
	recv_data.buflen = BUFFER_SIZE;

	ret = messaging_recv_block(SYNC_BLOCK_PORT, &recv_data);
	if (ret < 0) {
		free(recv_data.buf);
		printf("[MICOM] Fail to receive with block mode.\n");
		return ERROR;
	}

	printf("[MICOM] Success to receive with block mode in receiver. msg : [%s]\n", (char *)recv_data.buf);
	printf("[MICOM] - Reply to sender [%s] data.\n", SYNC_BLOCK_REPLY);

	reply_data.msg = SYNC_BLOCK_REPLY;
	reply_data.msglen = sizeof(SYNC_BLOCK_REPLY);
	if (ret == MSG_REPLY_REQUIRED) {
		ret = messaging_reply(SYNC_BLOCK_PORT, recv_data.sender_pid, &reply_data);
		if (ret != OK) {
			free(recv_data.buf);
			printf("[MICOM] Fail to reply.\n");
			return ERROR;
		}
	} else {
		printf("[MICOM] We will not reply, because received msg is not sync type. %d\n", ret);
	}

	free(recv_data.buf);
	return OK;
}

void recv_callback(msg_reply_type_t msg_type, msg_recv_buf_t *recv_data, void *cb_data)
{
	if (recv_data == NULL) {
		printf("[MICOM] Fail to receive unicast message with non-block mode.\n");
		nonblock_fail_cnt++;
		return;
	}
	printf("[MICOM] Success to receive with non-block mode in recv callback from PID(%d). msg : [%s]\n", recv_data->sender_pid, (char *)recv_data->buf);
	sem_post(&nonblock_sem);
}

static int nonblock_recv(int argc, FAR char *argv[])
{
	int ret;
	msg_callback_info_t cb_info;
	msg_recv_buf_t data;

	sem_init(&nonblock_sem, 0, 0);

	cb_info.cb_func = recv_callback;
	cb_info.cb_data = NULL;

	data.buf = (char *)malloc(BUFFER_SIZE);
	if (data.buf == NULL) {
		printf("[MICOM] Fail to recv nonblock message : out of memory.\n");
		return ERROR;
	}
	data.buflen = BUFFER_SIZE;

	ret = messaging_recv_nonblock(NOREPLY_NONBLOCK_PORT, &data, &cb_info);
	if (ret != OK) {
		free(data.buf);
		printf("[MICOM] Fail to receive with non-block mode.\n");
		return ERROR;
	}

	/* Wait not to finish this task, because of receiving data through the callback. */
	sem_wait(&nonblock_sem);
	sem_destroy(&nonblock_sem);

	ret = messaging_cleanup(NOREPLY_NONBLOCK_PORT);
	if (ret != OK) {
		free(data.buf);
		printf("[MICOM] Fail to cleanup NOREPLY_NONBLOCK_PORT.\n");
		return ERROR;
	}

	free(data.buf);
	return OK;
}

static void multi_recv_callback(msg_reply_type_t msg_type, msg_recv_buf_t *recv_data, void *cb_data)
{
	if (recv_data == NULL) {
		printf("[MICOM] Fail to receive multicast message with non-block mode.\n");
		multicast_fail_cnt++;
		return;
	}
	printf("[MICOM] Success to receive multicast message in recv callback. [%s]\n", (char *)recv_data->buf);
}

static int multi_recv_nonblock(int argc, FAR char *argv[])
{
	int ret;
	msg_callback_info_t cb_info;
	msg_recv_buf_t data;

	cb_info.cb_func = multi_recv_callback;
	cb_info.cb_data = NULL;

	sem_init(&multi_sem, 0, 0);

	data.buf = (char *)zalloc(BUFFER_SIZE);
	if (data.buf == NULL) {
		printf("[MICOM] Fail to recv nonblock multicast message : out of memory.\n");
		multicast_fail_cnt++;
		sem_destroy(&multi_sem);
		return ERROR;
	}
	data.buflen = BUFFER_SIZE;

	ret = messaging_recv_nonblock(MULTICAST_PORT, &data, &cb_info);
	if (ret != OK) {
		multicast_fail_cnt++;
		printf("[MICOM] Fail to receive multicast with non-block mode.\n");
		free(data.buf);
		sem_destroy(&multi_sem);
		return ERROR;
	}

	/* Wait not to finish this task, because of receiving data through the callback. */
	sem_wait(&multi_sem);
	sem_destroy(&multi_sem);

	free(data.buf);

	ret = messaging_cleanup(MULTICAST_PORT);
	if (ret != OK) {
		multicast_fail_cnt++;
		printf("[MICOM] Fail to cleanup MULTICAST_PORT.\n");
		return ERROR;
	}
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
		printf("[MICOM] Fail to malloc for multi_recv buffer.\n");
		return ERROR;
	}

	ret = messaging_recv_block(MULTICAST_PORT, &msg);
	if (ret < 0) {
		multicast_fail_cnt++;
		printf("[MICOM] Fail to receive multicast message in multi_recv.\n");
		return ERROR;
	}

	printf("[MICOM] Success to receive multicast message [%s], receiver pid : %d\n", msg.buf, getpid());
	free(msg.buf);
	
	return OK;
}

static int block_recv_test(void)
{
	int pid;

	pid = task_create("block_recv", 120, STACKSIZE, block_recv, NULL);
	if (pid < 0) {
		printf("[MICOM] Fail to create block_recv task.\n");
		return ERROR;
	}
	return 0;
}

static int nonblock_recv_test(void)
{
	int pid;

	pid = task_create("nonblock_recv", TASK_PRIO, STACKSIZE, nonblock_recv, NULL);
	if (pid < 0) {
		printf("[MICOM] Fail to create nonblock_recv task.\n");
		return ERROR;
	}
	return 0;
}

static int multicast_recv_test(void)
{
	int recv1_pid;
	int recv2_pid;
	int recv3_pid;

	recv1_pid = task_create("multi_recv_nonblock", TASK_PRIO, STACKSIZE, multi_recv_nonblock, NULL);
	if (recv1_pid < 0) {
		printf("[MICOM] Fail to create multi_recv_nonblock task.\n");
		return ERROR;
	}

	recv2_pid = task_create("multi_recv_block1", TASK_PRIO, STACKSIZE, multi_recv_block, NULL);
	if (recv2_pid < 0) {
		printf("[MICOM] Fail to create multi_recv_block1 task.\n");
		return ERROR;
	}

	recv3_pid = task_create("multi_recv_block2", TASK_PRIO, STACKSIZE, multi_recv_block, NULL);
	if (recv3_pid < 0) {
		printf("[MICOM] Fail to create multi_recv_block2 task.\n");
		return ERROR;
	}

	return OK;
}

static void check_can_do_next(void)
{
	msg_recv_buf_t recv_data;

	recv_data.buf = (char *)malloc(BUFFER_SIZE);
	if (recv_data.buf == NULL) {
		printf("[MICOM] Fail to receive, because out of memory.\n");
		return;
	}
	recv_data.buflen = BUFFER_SIZE;

	while (strncmp(recv_data.buf, CHECK_MSG, strlen(CHECK_MSG) + 1) != 0) {
		memset(recv_data.buf, 0, BUFFER_SIZE);
		(void)messaging_recv_block(CHECK_PORT, &recv_data);
	}

	free(recv_data.buf);
}

void messaging_test(void)
{
	int ret;
	int rep_cnt;
	int repetition_num = CONFIG_MESSAGING_TEST_REPETITION_NUM;

	if (repetition_num < 0) {
		repetition_num = 10;
	}

	for (rep_cnt = 1; rep_cnt <= repetition_num; rep_cnt++) {
		ret = multicast_recv_test();
		if (ret != OK) {
			multicast_fail_cnt++;
		}

		ret = block_recv_test();
		if (ret != OK) {
			block_fail_cnt++;
		}

		ret = nonblock_recv_test();
		if (ret != OK) {
			nonblock_fail_cnt++;
		}

		check_can_do_next();
		if (CONFIG_MESSAGING_TEST_REPETITION_NUM < 0) {
			/* Test will run infinitely. */
			rep_cnt = 1;
		}
	}

	printf("<<Receiver>> Summary\n");
	printf(" - Total : %d iteration\n", CONFIG_MESSAGING_TEST_REPETITION_NUM);
	printf(" - Block mode : %d fails\n", block_fail_cnt);
	printf(" - Non-block mode : %d fails\n", nonblock_fail_cnt);
	printf(" - Multicast mode : %d fails\n", multicast_fail_cnt);

}
