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
#include <string.h>
#include <errno.h>
#include <debug.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <tinyara/binary_manager.h>

binmgr_result_type_e binary_manager_set_request(binmgr_request_t *request_msg, int cmd, void *arg)
{
	if (request_msg == NULL) {
		bmdbg("Invalid param\n");
		return BINMGR_INVALID_PARAM;
	}

	switch (cmd) {
	case BINMGR_GET_STATE:
	case BINMGR_GET_INFO:
	case BINMGR_NOTIFY_STARTED:
	case BINMGR_UPDATE:
		if (arg == NULL) {
			bmdbg("Invalid param, cmd : %d\n", cmd);
			return BINMGR_INVALID_PARAM;
		}
		snprintf(request_msg->data.bin_name, BIN_NAME_MAX, "%s", (char *)arg);
		break;
	case BINMGR_CREATE_BIN:
		if (arg == NULL) {
			bmdbg("Invalid param, cmd : %d\n", cmd);
			return BINMGR_INVALID_PARAM;
		}
		binmgr_update_bin_t *data = (binmgr_update_bin_t *)arg;
		strncpy(request_msg->data.update_bin.bin_name, data->bin_name, BIN_NAME_MAX);
		request_msg->data.update_bin.version = data->version;
		break;
	case BINMGR_REGISTER_STATECB:
		if (arg == NULL) {
			bmdbg("Invalid param, cmd : %d\n", cmd);
			return BINMGR_INVALID_PARAM;
		}
		request_msg->data.cb_info = (binmgr_cb_t *)arg;
		break;
	default:
		break;
	}

	request_msg->cmd = cmd;
	request_msg->requester_pid = getpid();

	return BINMGR_OK;
}

binmgr_result_type_e binary_manager_send_request(binmgr_request_t *request_msg)
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
		return BINMGR_COMMUNICATION_FAIL;
	}

	ret = mq_send(binmgr_mq, (const char *)request_msg, sizeof(binmgr_request_t), BINMGR_NORMAL_PRIO);
	if (ret < 0) {
		bmdbg("send ERROR %d, errno %d\n", errno);
		mq_close(binmgr_mq);
		return BINMGR_COMMUNICATION_FAIL;
	}

	mq_close(binmgr_mq);

	return BINMGR_OK;
}

binmgr_result_type_e binary_manager_receive_response(void *response_msg, int msg_size)
{
	int nbytes;
	struct mq_attr attr;
	mqd_t private_mq;
	char q_name[BIN_PRIVMQ_LEN];

	if (response_msg == NULL || msg_size < 0) {
		bmdbg("Invalid param\n");
		return BINMGR_INVALID_PARAM;
	}

	attr.mq_maxmsg = BINMGR_MAX_MSG;
	attr.mq_msgsize = msg_size;
	attr.mq_flags = 0;

	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, getpid());

	private_mq = mq_open(q_name, O_RDONLY | O_CREAT, 0666, &attr);
	if (private_mq == (mqd_t)ERROR) {
		bmdbg("mq open ERROR failed, errno %d\n", errno);
		return BINMGR_COMMUNICATION_FAIL;
	}

	nbytes = mq_receive(private_mq, (char *)response_msg, msg_size, NULL);
	if (nbytes <= 0) {
		bmdbg("receive ERROR %d, errno %d\n", nbytes, errno);
		mq_close(private_mq);
		mq_unlink(q_name);
		return BINMGR_COMMUNICATION_FAIL;
	}

	mq_close(private_mq);
	mq_unlink(q_name);

	return BINMGR_OK;
}
