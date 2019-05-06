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
 * apps/examples/messaging_sample/messaging_multicast.c
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

#define TEST3_PORT "test3_port"
#define TEST3_DATA "MULTI"

#define MSG_PRIO 10
#define TASK_PRIO 100
#define STACKSIZE 2048
#define BUFFER_SIZE 10

extern int fail_cnt;

static void multi_recv_callback(msg_reply_type_t msg_type, msg_recv_buf_t *recv_data, void *cb_data)
{
	if (recv_data == NULL) {
		fail_cnt++;
		printf("Fail to receive multicast message with non-block mode.\n");
		return;
	}
	printf("Success to receive multicast message in recv callback. [%s]\n", (char *)recv_data->buf);
}

static int multi_recv_nonblock(int argc, FAR char *argv[])
{
	int ret;
	msg_callback_info_t cb_info;
	msg_recv_buf_t data;

	cb_info.cb_func = multi_recv_callback;
	cb_info.cb_data = NULL;

	data.buf = (char *)malloc(BUFFER_SIZE);
	if (data.buf == NULL) {
		fail_cnt++;
		printf("Fail to recv nonblock multicast message : out of memory.\n");
		return ERROR;
	}
	data.buflen = BUFFER_SIZE;

	ret = messaging_recv_nonblock(TEST3_PORT, &data, &cb_info);
	if (ret != OK) {
		fail_cnt++;
		printf("Fail to receive multicast with non-block mode.\n");
		free(data.buf);
		return ERROR;
	}

	/* Wait not to finish this task, because of receiving data through the callback. */
	sleep(5);

	free(data.buf);
	return OK;
}

static int multi_recv_block(int argc, FAR char *argv[])
{
	int ret;
	msg_recv_buf_t msg;

	msg.buflen = BUFFER_SIZE;
	msg.buf = (char *)malloc(BUFFER_SIZE);
	if (msg.buf == NULL) {
		fail_cnt++;
		printf("Fail to malloc for multi_recv1 buffer.\n");
		return ERROR;
	}

	ret = messaging_recv_block(TEST3_PORT, &msg);
	if (ret < 0) {
		fail_cnt++;
		printf("Fail to receive multicast message in multi_recv1.\n");
		return ERROR;
	}

	printf("Success to receive multicast message [%s], receiver pid : %d\n", msg.buf, getpid());
	free(msg.buf);
	
	return OK;
}

static int multi_sender(int argc, FAR char *argv[])
{
	int ret;
	msg_send_data_t data;
	data.priority = MSG_PRIO;
	data.msg = TEST3_DATA;
	data.msglen = sizeof(TEST3_DATA);

	printf("- Send Multicast [%s] data.\n", TEST3_DATA);
	ret = messaging_multicast(TEST3_PORT, &data);
	if (ret != 3) {
		fail_cnt++;
		printf("Fail to send multicast.\n");
		return ERROR;
	}

	return 0;
}

void multicast_messaging_sample(void)
{
	int ret;
	int recv1_pid;
	int recv2_pid;
	int recv3_pid;
	int sender_pid;

	printf("\n--- Start the Multicast messaging test. ---\n");

	recv1_pid = task_create("multi_recv3", TASK_PRIO, STACKSIZE, multi_recv_nonblock, NULL);
	if (recv1_pid < 0) {
		fail_cnt++;
		printf("Fail to create multi_recv3 task.\n");
		return;
	}

	recv2_pid = task_create("multi_recv1", TASK_PRIO, STACKSIZE, multi_recv_block, NULL);
	if (recv2_pid < 0) {
		fail_cnt++;
		printf("Fail to create multi_recv1 task.\n");
		return;
	}

	recv3_pid = task_create("multi_recv2", TASK_PRIO, STACKSIZE, multi_recv_block, NULL);
	if (recv3_pid < 0) {
		fail_cnt++;
		printf("Fail to create multi_recv2 task.\n");
		return;
	}

	sender_pid = task_create("multi_sender", TASK_PRIO, STACKSIZE, multi_sender, NULL);
	if (sender_pid < 0) {
		fail_cnt++;
		printf("Fail to create multi_recv3 task.\n");
		return;
	}

	/* Wait for finishing multicast test. */
	sleep(5);

	ret = messaging_cleanup(TEST3_PORT);
	if (ret != OK) {
		fail_cnt++;
		printf("Fail to cleanup TEST3_PORT.\n");
		return;
	}
}
