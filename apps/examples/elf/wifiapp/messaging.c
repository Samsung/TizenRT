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

#include <messaging/messaging.h>

#define SYNC_BLOCK_PORT "sync_block_port"
#define SYNC_BLOCK_DATA "sync_block_msg"
#define SYNC_BLOCK_REPLY "reply_msg"

#define BUFFER_SIZE 20
#define MSG_PRIO 10

/****************************************************************************
 * Public Functions
 ****************************************************************************/
static int sync_send_test(void)
{
	int ret;
	msg_send_data_t send_data;
	msg_recv_buf_t reply_data;

	send_data.priority = MSG_PRIO;
	send_data.msg = SYNC_BLOCK_DATA;
	send_data.msglen = sizeof(SYNC_BLOCK_DATA);

	reply_data.buflen = BUFFER_SIZE;
	reply_data.buf = (char *)malloc(BUFFER_SIZE);
	if (reply_data.buf == NULL) {
		printf("[WIFI] Fail to sync send message : out of memory.\n");
		return ERROR;
	}

	printf("[WIFI] - Send [%s] data with sync mode.\n", SYNC_BLOCK_DATA);
	ret = messaging_send_sync(SYNC_BLOCK_PORT, &send_data, &reply_data);
	if (ret != OK) {
		printf("[WIFI] Fail to sync send message.\n");
		free(reply_data.buf);
		return ERROR;
	}

	printf("[WIFI] Success to receive reply from receiver. msg : [%s]\n", (char *)reply_data.buf);
	free(reply_data.buf);

	return 0;
}
void messaging_test(void)
{
	int ret;
	ret = sync_send_test();
}
