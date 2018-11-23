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
#include <pthread.h>
#include <sys/types.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

/****************************************************************************
 * task_manager_register_builtin
 ****************************************************************************/
int task_manager_register_builtin(char *name, int permission, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (name == NULL || timeout < TM_RESPONSE_WAIT_INF || permission < TM_APP_PERMISSION_ALL || permission > TM_APP_PERMISSION_DEDICATE) {
		return TM_INVALID_PARAM;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	request_msg.cmd = TASKMGRCMD_REGISTER_BUILTIN;
	request_msg.caller_pid = getpid();
	request_msg.handle = permission;
	request_msg.data = (void *)TM_ALLOC(strlen(name) + 1);
	if (request_msg.data == NULL) {
		return TM_OUT_OF_MEMORY;
	}
	strncpy((char *)request_msg.data, name, strlen(name) + 1);
	request_msg.timeout = timeout;

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

int task_manager_register_task(char *name, int priority, int stack_size, main_t entry, char * argv[], int permission, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (name == NULL || timeout < TM_RESPONSE_WAIT_INF || permission < TM_APP_PERMISSION_ALL || permission > TM_APP_PERMISSION_DEDICATE || priority < SCHED_PRIORITY_MIN || priority > SCHED_PRIORITY_MAX || stack_size < 1 || entry == NULL) {
		return TM_INVALID_PARAM;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	request_msg.cmd = TASKMGRCMD_REGISTER_TASK;
	request_msg.caller_pid = getpid();
	request_msg.handle = permission;
	request_msg.data = (void *)TM_ALLOC(sizeof(tm_task_info_t));
	if (request_msg.data == NULL) {
		return TM_OUT_OF_MEMORY;
	}
	((tm_task_info_t *)request_msg.data)->name = (char *)TM_ALLOC(strlen(name) + 1);
	if (((tm_task_info_t *)request_msg.data)->name == NULL) {
		TM_FREE(request_msg.data);
		return TM_OUT_OF_MEMORY;
	}
	strncpy(((tm_task_info_t *)request_msg.data)->name, name, strlen(name) + 1);
	((tm_task_info_t *)request_msg.data)->priority = priority;
	((tm_task_info_t *)request_msg.data)->stack_size = stack_size;
	((tm_task_info_t *)request_msg.data)->entry = entry;
	((tm_task_info_t *)request_msg.data)->argv = argv;
	request_msg.timeout = timeout;

	if (timeout != TM_NO_RESPONSE) {
		TM_ASPRINTF(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
		if (request_msg.q_name == NULL) {
			TM_FREE(((tm_task_info_t *)request_msg.data)->name);
			TM_FREE(request_msg.data);
			return TM_OUT_OF_MEMORY;
		}
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		TM_FREE(((tm_task_info_t *)request_msg.data)->name);
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

#ifndef CONFIG_DISABLE_PTHREAD
int task_manager_register_pthread(char *name, pthread_attr_t *attr, pthread_startroutine_t start_routine, pthread_addr_t arg, int permission, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (name == NULL || timeout < TM_RESPONSE_WAIT_INF || permission < TM_APP_PERMISSION_ALL || permission > TM_APP_PERMISSION_DEDICATE || attr == NULL || start_routine == NULL) {
		return TM_INVALID_PARAM;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	request_msg.cmd = TASKMGRCMD_REGISTER_PTHREAD;
	request_msg.caller_pid = getpid();
	request_msg.handle = permission;
	request_msg.data = (void *)TM_ALLOC(sizeof(tm_pthread_info_t));
	if (request_msg.data == NULL) {
		return TM_OUT_OF_MEMORY;
	}
	((tm_pthread_info_t *)request_msg.data)->name = (char *)TM_ALLOC(strlen(name) + 1);
	if (((tm_pthread_info_t *)request_msg.data)->name == NULL) {
		TM_FREE(request_msg.data);
		return TM_OUT_OF_MEMORY;
	}
	strncpy(((tm_pthread_info_t *)request_msg.data)->name, name, strlen(name) + 1);
	((tm_pthread_info_t *)request_msg.data)->attr = (pthread_attr_t *)TM_ALLOC(sizeof(pthread_attr_t));
	if (((tm_pthread_info_t *)request_msg.data)->attr == NULL) {
		TM_FREE(((tm_pthread_info_t *)request_msg.data)->name);
		TM_FREE(request_msg.data);
		return TM_OUT_OF_MEMORY;
	}
	memcpy(((tm_pthread_info_t *)request_msg.data)->attr, attr, sizeof(pthread_attr_t));
	((tm_pthread_info_t *)request_msg.data)->entry = start_routine;
	((tm_pthread_info_t *)request_msg.data)->arg = arg;
	request_msg.timeout = timeout;

	if (timeout != TM_NO_RESPONSE) {
		TM_ASPRINTF(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
		if (request_msg.q_name == NULL) {
			TM_FREE(((tm_pthread_info_t *)request_msg.data)->attr);
			TM_FREE(((tm_pthread_info_t *)request_msg.data)->name);
			TM_FREE(request_msg.data);
			return TM_OUT_OF_MEMORY;
		}
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		TM_FREE(((tm_pthread_info_t *)request_msg.data)->attr);
		TM_FREE(((tm_pthread_info_t *)request_msg.data)->name);
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
#endif
