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
#include <tinyara/config.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <task_manager/task_manager.h>
#ifdef CONFIG_TASK_MANAGER_USER_SPECIFIC_BROADCAST
#include <task_manager/task_manager_broadcast_list.h>
#endif
#include "task_manager_internal.h"

/****************************************************************************
 * task_manager_dealloc_broadcast_msg
 ****************************************************************************/
int task_manager_dealloc_broadcast_msg(int msg, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (msg < TM_BROADCAST_MSG_MAX || timeout < TM_RESPONSE_WAIT_INF) {
		return TM_INVALID_PARAM;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	request_msg.cmd = TASKMGRCMD_DEALLOC_BROADCAST_MSG;
	request_msg.caller_pid = getpid();
	request_msg.timeout = timeout;
	request_msg.data = (void *)TM_ALLOC(sizeof(int));
	if (request_msg.data == NULL) {
		return TM_OUT_OF_MEMORY;
	}
	*((int *)request_msg.data) = msg;
	
	if (timeout != TM_NO_RESPONSE) {
		TM_ASPRINTF(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
		if (request_msg.q_name == NULL) {
			TM_FREE(request_msg.data);
			return TM_OUT_OF_MEMORY;
		}
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		TM_FREE(request_msg.data);
		if (request_msg.q_name != NULL) {
			TM_FREE(request_msg.q_name);
		}
		return status;
	}
	if (timeout != TM_NO_RESPONSE) {
		status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
		TM_FREE(request_msg.q_name);
	}
	return status;
}
