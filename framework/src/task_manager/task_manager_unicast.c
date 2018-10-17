/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/types.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

/****************************************************************************
 * task_manager_unicast
 ****************************************************************************/
int task_manager_unicast(int handle, tm_msg_t *send_msg, tm_msg_t *reply_msg, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (IS_INVALID_HANDLE(handle) || timeout < TM_RESPONSE_WAIT_INF || (reply_msg != NULL && timeout == TM_NO_RESPONSE) || send_msg == NULL) {
		return TM_INVALID_PARAM;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	if (send_msg->msg != NULL) {
		if (send_msg->msg_size == 0) {
			return TM_INVALID_PARAM;
		}
		request_msg.data = (void *)TM_ALLOC(sizeof(tm_internal_msg_t));
		if (request_msg.data == NULL) {
			return TM_OUT_OF_MEMORY;
		}
		((tm_internal_msg_t *)request_msg.data)->msg = (void *)TM_ALLOC(send_msg->msg_size);
		if (((tm_internal_msg_t *)request_msg.data)->msg == NULL) {
			TM_FREE(request_msg.data);
			return TM_OUT_OF_MEMORY;
		}
		memcpy(((tm_internal_msg_t *)request_msg.data)->msg, send_msg->msg, send_msg->msg_size);
		((tm_internal_msg_t *)request_msg.data)->msg_size = send_msg->msg_size;

		if (reply_msg == NULL) {
			((tm_internal_msg_t *)request_msg.data)->type = TM_UNICAST_ASYNC;
		} else {
			((tm_internal_msg_t *)request_msg.data)->type = TM_UNICAST_SYNC;
		}

	}

	/* Set the request msg */
	request_msg.cmd = TASKMGRCMD_UNICAST;
	request_msg.handle = handle;
	request_msg.caller_pid = getpid();
	request_msg.timeout = timeout;

	if (timeout != TM_NO_RESPONSE) {
		TM_ASPRINTF(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
		if (request_msg.q_name == NULL) {
			TM_FREE(((tm_internal_msg_t *)request_msg.data)->msg);
			TM_FREE(request_msg.data);
			return TM_OUT_OF_MEMORY;
		}
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		TM_FREE(((tm_internal_msg_t *)request_msg.data)->msg);
		TM_FREE(request_msg.data);
		if (request_msg.q_name != NULL) {
			TM_FREE(request_msg.q_name);
		}
		return status;
	}

	if (timeout != TM_NO_RESPONSE) {
		status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
		TM_FREE(request_msg.q_name);
		if (reply_msg != NULL) {
			reply_msg->msg_size = ((tm_msg_t *)response_msg.data)->msg_size;
			reply_msg->msg = TM_ALLOC(reply_msg->msg_size);
			if (reply_msg->msg == NULL) {
				TM_FREE(((tm_internal_msg_t *)request_msg.data)->msg);
				TM_FREE(request_msg.data);
				return TM_OUT_OF_MEMORY;
			}
			memcpy(reply_msg->msg, ((tm_msg_t *)response_msg.data)->msg, reply_msg->msg_size);
			TM_FREE(((tm_msg_t *)response_msg.data)->msg);
			TM_FREE(response_msg.data);
		}
	}

	return status;
}

/****************************************************************************
 * task_manager_reply_unicast
 ****************************************************************************/
int task_manager_reply_unicast(tm_msg_t *reply_msg)
{
	int ret = ERROR;
	mqd_t reply_mqfd;
	struct mq_attr attr;
	tm_msg_t *data;

	if (reply_msg == NULL) {
		return TM_INVALID_PARAM;
	}
	data = (tm_msg_t *)TM_ALLOC(sizeof(tm_msg_t));
	if (data == NULL) {
		return TM_OUT_OF_MEMORY;
	}
	data->msg = TM_ALLOC(reply_msg->msg_size);
	if (data->msg == NULL) {
		TM_FREE(data);
		return TM_OUT_OF_MEMORY;
	}
	data->msg_size = reply_msg->msg_size;
	memcpy(data->msg, reply_msg->msg, reply_msg->msg_size);

	attr.mq_maxmsg = CONFIG_TASK_MANAGER_MAX_MSG;
	attr.mq_msgsize = sizeof(tm_msg_t);
	attr.mq_flags = 0;

	reply_mqfd = mq_open(TM_UNICAST_MQ, O_WRONLY | O_CREAT, 0666, &attr);
	if (reply_mqfd == (mqd_t)ERROR) {
		TM_FREE(data->msg);
		TM_FREE(data);
		tmdbg("mq_open failed!\n");
		return TM_COMMUCATION_FAIL;
	}

	ret = mq_send(reply_mqfd, (char *)data, sizeof(tm_msg_t), TM_MQ_PRIO);
	if (ret != 0) {
		mq_close(reply_mqfd);
		TM_FREE(data->msg);
		TM_FREE(data);
		tmdbg("mq_send failed! %d\n", errno);
		return TM_COMMUCATION_FAIL;
	}

	ret = mq_close(reply_mqfd);
	if (ret != OK) {
		tmdbg("mq_close failed! ret %d, errno %d\n", ret, errno);
	}

	TM_FREE(data->msg);
	TM_FREE(data);

	return ret;
}
