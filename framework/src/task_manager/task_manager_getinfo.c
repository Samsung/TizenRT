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
#include <sys/types.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

/****************************************************************************
 * task_manager_getinfo_with_name
 ****************************************************************************/
tm_appinfo_list_t *task_manager_getinfo_with_name(char *name, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (name == NULL || timeout < TM_RESPONSE_WAIT_INF || timeout == TM_NO_RESPONSE) {
		return NULL;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));

	/* Set the request msg */
	request_msg.cmd = TASKMGRCMD_SCAN_NAME;
	request_msg.data = (void *)TM_ALLOC(strlen(name) + 1);
	if (request_msg.data == NULL) {
		return NULL;
	}
	strncpy((char *)request_msg.data, name, strlen(name) + 1);
	request_msg.timeout = timeout;

	TM_ASPRINTF(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, getpid());
	if (request_msg.q_name == NULL) {
		TM_FREE(request_msg.data);
		return NULL;
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		TM_FREE(request_msg.data);
		TM_FREE(request_msg.q_name);
		return NULL;
	}

	status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
	TM_FREE(request_msg.q_name);

	if (status < 0) {
		return NULL;
	}

	return response_msg.data;
}

/****************************************************************************
 * task_manager_getinfo_with_handle
 ****************************************************************************/
tm_appinfo_t *task_manager_getinfo_with_handle(int handle, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (IS_INVALID_HANDLE(handle) || timeout < TM_RESPONSE_WAIT_INF || timeout == TM_NO_RESPONSE) {
		return NULL;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));

	/* Set the request msg */
	request_msg.cmd = TASKMGRCMD_SCAN_HANDLE;
	request_msg.handle = handle;
	request_msg.timeout = timeout;

	TM_ASPRINTF(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, getpid());
	if (request_msg.q_name == NULL) {
		return NULL;
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		TM_FREE(request_msg.q_name);
		return NULL;
	}

	status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
	TM_FREE(request_msg.q_name);

	if (status < 0) {
		return NULL;
	}

	return &((tm_appinfo_list_t *)response_msg.data)->task;
}

/****************************************************************************
 * task_manager_getinfo_with_group
 ****************************************************************************/
tm_appinfo_list_t *task_manager_getinfo_with_group(int group, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (group < 0 || timeout < TM_RESPONSE_WAIT_INF || timeout == TM_NO_RESPONSE) {
		return NULL;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));

	/* Set the request msg */
	request_msg.cmd = TASKMGRCMD_SCAN_GROUP;
	request_msg.handle = group;
	request_msg.timeout = timeout;

	TM_ASPRINTF(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, getpid());
	if (request_msg.q_name == NULL) {
		return NULL;
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		TM_FREE(request_msg.q_name);
		return NULL;
	}

	status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
	TM_FREE(request_msg.q_name);

	if (status < 0) {
		return NULL;
	}

	return response_msg.data;
}

/****************************************************************************
 * task_manager_getinfo_with_pid
 ****************************************************************************/
tm_appinfo_t *task_manager_getinfo_with_pid(int pid, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (pid < 0 || timeout < TM_RESPONSE_WAIT_INF || timeout == TM_NO_RESPONSE) {
		return NULL;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	/* Set the request msg */
	request_msg.cmd = TASKMGRCMD_SCAN_PID;
	request_msg.caller_pid = pid;
	request_msg.timeout = timeout;

	TM_ASPRINTF(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, getpid());
	if (request_msg.q_name == NULL) {
		return NULL;
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		TM_FREE(request_msg.q_name);
		return NULL;
	}

	status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
	TM_FREE(request_msg.q_name);

	if (status < 0) {
		return NULL;
	}

	return &((tm_appinfo_list_t *)response_msg.data)->task;
}
