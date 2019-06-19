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
 * apps/examples/messaging_sample/messaging_unicast.c
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
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <messaging/messaging.h>

#define TEST1_PORT "test1_port"
#define TEST1_DATA "TEST1"

#define TEST2_PORT "test2_port"
#define TEST2_DATA "TEST2"
#define TEST2_REPLY "REPLY_MSG"

#define MSG_PRIO 10
#define TASK_PRIO 100
#define STACKSIZE 2048
#define BUFFER_SIZE 10

extern int fail_cnt;
static char *sync_send_alloc;

static void recv_callback(msg_reply_type_t msg_type, msg_recv_buf_t *recv_data, void *cb_data)
{
	if (recv_data == NULL) {
		fail_cnt++;
		printf("Fail to receive unicast message with non-block mode.\n");
		return;
	}
	printf("Success to receive with non-block mode in recv callback from PID(%d). msg : [%s]\n", recv_data->sender_pid, (char *)recv_data->buf);
}

int noreply_send(int argc, FAR char *argv[])
{
	int ret;
	msg_send_data_t param;
	param.priority = MSG_PRIO;
	param.msg = TEST1_DATA;
	param.msglen = sizeof(TEST1_DATA);

	printf("- Send [%s] data with noreply mode.\n", TEST1_DATA);
	ret = messaging_send(TEST1_PORT, &param);
	if (ret != OK) {
		fail_cnt++;
		printf("Fail to noreply send message.\n");
		return ERROR;
	}

	return 0;
}

int sync_send(int argc, FAR char *argv[])
{
	int ret;
	msg_send_data_t send_data;
	msg_recv_buf_t reply_data;

	send_data.priority = MSG_PRIO;
	send_data.msg = TEST2_DATA;
	send_data.msglen = sizeof(TEST2_DATA);

	reply_data.buflen = BUFFER_SIZE;
	reply_data.buf = (char *)malloc(BUFFER_SIZE);
	if (reply_data.buf == NULL) {
		fail_cnt++;
		printf("Fail to sync send message : out of memory.\n");
		return ERROR;
	}
	sync_send_alloc = reply_data.buf;

	printf("- Send [%s] data with sync mode.\n", TEST2_DATA);
	ret = messaging_send_sync(TEST2_PORT, &send_data, &reply_data);
	if (ret != OK) {
		fail_cnt++;
		printf("Fail to sync send message.\n");
		free(reply_data.buf);
		return ERROR;
	}

	printf("Success to receive reply from receiver. msg : [%s]\n", (char *)reply_data.buf);
	free(reply_data.buf);

	return 0;
}

int nonblock_recv(int argc, FAR char *argv[])
{
	int ret;
	msg_callback_info_t cb_info;
	msg_recv_buf_t data;

	cb_info.cb_func = recv_callback;
	cb_info.cb_data = NULL;

	data.buf = (char *)malloc(BUFFER_SIZE);
	if (data.buf == NULL) {
		fail_cnt++;
		printf("Fail to recv nonblock message : out of memory.\n");
		return ERROR;
	}
	data.buflen = BUFFER_SIZE;

	ret = messaging_recv_nonblock(TEST1_PORT, &data, &cb_info);
	if (ret != OK) {
		fail_cnt++;
		printf("Fail to receive with non-block mode.\n");
		free(data.buf);
		return ERROR;
	}

	/* Wait not to finish this task, because of receiving data through the callback. */
	sleep(3);

	free(data.buf);
	return OK;
}

int block_recv(int argc, FAR char *argv[])
{
	int ret;
	msg_recv_buf_t recv_data;
	msg_send_data_t reply_data;

	recv_data.buf = (char *)malloc(BUFFER_SIZE);
	if (recv_data.buf == NULL) {
		fail_cnt++;
		printf("Fail to receive, because out of memory.\n");
		return ERROR;
	}
	recv_data.buflen = BUFFER_SIZE;

	ret = messaging_recv_block(TEST2_PORT, &recv_data);
	if (ret < 0) {
		fail_cnt++;
		printf("Fail to receive with block mode.\n");
		free(recv_data.buf);
		return ERROR;
	}

	printf("Success to receive with block mode in receiver. msg : [%s]\n", (char *)recv_data.buf);
	printf("- Reply to sender [%s] data.\n", TEST2_REPLY);

	reply_data.msg = TEST2_REPLY;
	reply_data.msglen = sizeof(TEST2_REPLY);
	if (ret == MSG_REPLY_REQUIRED) {
		ret = messaging_reply(TEST2_PORT, recv_data.sender_pid, &reply_data);
		if (ret != OK) {
			fail_cnt++;
			printf("Fail to reply.\n");
			free(recv_data.buf);
			return ERROR;
		}
	} else {
		printf("We will not reply, because received msg is not sync type. %d\n", ret);
	}

	free(recv_data.buf);
	return OK;
}

void noreply_nonblock_messaging_sample(void)
{
	int ret;
	int receiver_pid;

	printf("\n--- Start the NoReply Non-block messaging test. ---\n");

	receiver_pid = task_create("nonblock_recv", TASK_PRIO, STACKSIZE, nonblock_recv, NULL);
	if (receiver_pid < 0) {
		fail_cnt++;
		printf("Fail to create nonblock_recv task.\n");
		return;
	}

	ret = task_create("noreply_send", TASK_PRIO, STACKSIZE, noreply_send, NULL);
	if (ret < 0) {
		fail_cnt++;
		task_delete(receiver_pid);
		printf("Fail to create noreply_send task.\n");
		return;
	}

	/* Wait for finishing noreply_send and nonblock_recv tasks. */
	sleep(1);

	ret = messaging_cleanup(TEST1_PORT);
	if (ret != OK) {
		fail_cnt++;
		printf("Fail to cleanup TEST1_PORT.\n");
		return;
	}
}

void sync_block_messaging_sample(void)
{
	int ret;
	int receiver_pid;

	printf("\n--- Start the Sync Block messaging test. ---\n");

	receiver_pid = task_create("block_recv", TASK_PRIO, STACKSIZE, block_recv, NULL);
	if (receiver_pid < 0) {
		fail_cnt++;
		printf("Fail to create block_recv task.\n");
		return;
	}

	ret = task_create("sync_send", TASK_PRIO, STACKSIZE, sync_send, NULL);
	if (ret < 0) {
		fail_cnt++;
		free(sync_send_alloc);
		task_delete(receiver_pid);
		printf("Fail to create sync_send task.\n");
		return;
	}

	/* Wait for finishing snyc_send and block_recv tasks. */
	sleep(1);

	ret = messaging_cleanup(TEST2_PORT);
	if (ret != OK) {
		fail_cnt++;
		printf("Fail to cleanup TEST2_PORT.\n");
		return;
	}
}
