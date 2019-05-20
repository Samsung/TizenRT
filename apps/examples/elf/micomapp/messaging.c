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

#define BUFFER_SIZE 20
#define MSG_PRIO 10

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
		printf("[MICOM] Fail to receive, because out of memory.\n");
		return ERROR;
	}
	recv_data.buflen = BUFFER_SIZE;

	ret = messaging_recv_block(SYNC_BLOCK_PORT, &recv_data);
	if (ret < 0) {
		printf("[MICOM] Fail to receive with block mode.\n");
		free(recv_data.buf);
		return ERROR;
	}

	printf("[MICOM] Success to receive with block mode in receiver. msg : [%s]\n", (char *)recv_data.buf);
	printf("[MICOM] - Reply to sender [%s] data.\n", SYNC_BLOCK_REPLY);

	reply_data.msg = SYNC_BLOCK_REPLY;
	reply_data.msglen = sizeof(SYNC_BLOCK_REPLY);
	if (ret == MSG_REPLY_REQUIRED) {
		ret = messaging_reply(SYNC_BLOCK_PORT, recv_data.sender_pid, &reply_data);
		if (ret != OK) {
			printf("[MICOM] Fail to reply.\n");
			free(recv_data.buf);
			return ERROR;
		}
	} else {
		printf("[MICOM] We will not reply, because received msg is not sync type. %d\n", ret);
	}

	free(recv_data.buf);
	return OK;
}

void messaging_test(void)
{
	int ret;
	ret = block_recv_test();
}
