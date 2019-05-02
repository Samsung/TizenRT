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

/***************************************************************************
 * Included Files
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <debug.h>
#include <sched.h>
#include <fcntl.h>
#include <string.h>
#include <mqueue.h>
#include <tinyara/binary_manager.h>

#include <apps/system/binary_update.h>

static int binary_update_send_request(binmgr_request_t *request_msg)
{
	int ret;
	mqd_t binmgr_mq;
	struct mq_attr attr;

	attr.mq_maxmsg = BINMGR_MAX_MSG;
	attr.mq_msgsize = sizeof(binmgr_request_t);
	attr.mq_flags = 0;

	binmgr_mq = mq_open(BINMGR_REQUEST_MQ, O_WRONLY, 0666, 0);
	if (binmgr_mq == (mqd_t)ERROR) {
		bmdbg("mq open ERROR failed, errno %d\n", errno);
		return ERROR;
	}

	ret = mq_send(binmgr_mq, (const char *)request_msg, sizeof(binmgr_request_t), BINMGR_NORMAL_PRIO);
	if (ret < 0) {
		bmdbg("send ERROR %d, errno %d\n", errno);
		mq_close(binmgr_mq);
		return ERROR;
	}

	mq_close(binmgr_mq);

	return OK;
}

static int binary_update_receive_response(void *response_msg, int msg_size)
{
	int nbytes;
	struct mq_attr attr;
	mqd_t private_mq;
	char q_name[BIN_PRIVMQ_LEN];

	if (response_msg == NULL || msg_size < 0) {
		bmdbg("Invalid param\n");
		return ERROR;
	}

	attr.mq_maxmsg = BINMGR_MAX_MSG;
	attr.mq_msgsize = msg_size;
	attr.mq_flags = 0;

	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, getpid());

	private_mq = mq_open(q_name, O_RDONLY | O_CREAT, 0666, &attr);
	if (private_mq == (mqd_t)ERROR) {
		bmdbg("mq open ERROR failed, errno %d\n", errno);
		return ERROR;
	}

	nbytes = mq_receive(private_mq, (char *)response_msg, msg_size, NULL);
	if (nbytes <= 0) {
		bmdbg("receive ERROR %d, errno %d\n", nbytes, errno);
		mq_close(private_mq);
		mq_unlink(q_name);
		return ERROR;
	}

	mq_close(private_mq);
	mq_unlink(q_name);

	return OK;
}

int binary_update_reload_binary(char *binary_name)
{
	int ret;
	binmgr_request_t request_msg;
	binmgr_reload_response_t response_msg;

	if (binary_name == NULL || strlen(binary_name) > BIN_NAME_MAX - 1) {
		bmdbg("load_new_binary failed : invalid binary name\n");
		return BINMGR_INVALID_PARAM;
	}

	request_msg.cmd = BINMGR_RELOAD;
	request_msg.requester_pid = getpid();
	snprintf(request_msg.bin_name, BIN_NAME_MAX, "%s", binary_name);

	ret = binary_update_send_request(&request_msg);
	if (ret < 0) {
		bmdbg("Failed to send request msg %d\n", ret);
		return BINMGR_COMMUNICATION_FAIL;
	}

	ret = binary_update_receive_response(&response_msg, sizeof(binmgr_reload_response_t));
	if (ret < 0) {
		bmdbg("Failed to receive response msg %d\n", ret);
		return BINMGR_COMMUNICATION_FAIL;
	}

	return response_msg.result;
}

int binary_update_get_binary_info(char *binary_name, binary_info_t *binary_info)
{
	int ret;
	binmgr_request_t request_msg;
	binmgr_getinfo_response_t response_msg;

	if (binary_name == NULL || binary_info == NULL || strlen(binary_name) > BIN_NAME_MAX - 1) {
		bmdbg("get_binary_info failed : invalid param.\n");
		return BINMGR_INVALID_PARAM;
	}

	request_msg.cmd = BINMGR_GET_INFO;
	request_msg.requester_pid = getpid();
	snprintf(request_msg.bin_name, BIN_NAME_MAX, "%s", binary_name);

	ret = binary_update_send_request(&request_msg);
	if (ret < 0) {
		bmdbg("Failed to send request msg %d\n", ret);
		return BINMGR_COMMUNICATION_FAIL;
	}

	ret = binary_update_receive_response(&response_msg, sizeof(binmgr_getinfo_response_t));
	if (ret < 0) {
		bmdbg("Failed to receive response msg %d\n", ret);
		return BINMGR_COMMUNICATION_FAIL;
	}

	if (response_msg.result == BINMGR_OK) {
		/* Copy binary info data */
		memset(binary_info, 0, sizeof(binary_info_t));
		memcpy(binary_info, &response_msg.data, sizeof(binary_info_t));
	} else {
		bmdbg("Binary manager getinfo FAIL %d\n", response_msg.result);
	}

	return response_msg.result;
}

int binary_update_get_binary_info_all(binary_info_list_t *binary_info_list)
{
	int ret;
	int bin_idx;
	binmgr_request_t request_msg;
	binmgr_getinfo_all_response_t response_msg;

	request_msg.cmd = BINMGR_GET_INFO_ALL;
	request_msg.requester_pid = getpid();

	ret = binary_update_send_request(&request_msg);
	if (ret < 0) {
		bmdbg("Failed to send request msg %d\n", ret);
		return BINMGR_COMMUNICATION_FAIL;
	}

	ret = binary_update_receive_response(&response_msg, sizeof(binmgr_getinfo_all_response_t));
	if (ret < 0) {
		bmdbg("Failed to receive response msg %d\n", ret);
		return BINMGR_COMMUNICATION_FAIL;
	}

	if (response_msg.result == BINMGR_OK) {
		/* Copy binary info list data */
		memset(binary_info_list, 0, sizeof(binary_info_list_t));
		memcpy(binary_info_list, &response_msg.data, sizeof(binary_info_list_t));
	} else {
		bmdbg("Binary manager getinfo_all FAIL %d\n", response_msg.result);
	}

	return response_msg.result;
}
