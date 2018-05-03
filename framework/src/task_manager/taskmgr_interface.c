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
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sched.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <tinyara/taskmgt.h>
#include <tinyara/clock.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

#define TM_PRIVATE_MQ "tm_priv_mq"

mqd_t g_tm_send_mqfd;
#define TBD 0
/****************************************************************************
 * taskmgt_utils
 ****************************************************************************/

static int taskmgr_receive_response(char *q_name, tm_response_t *response_msg, int timeout)
{
	int status;
	mqd_t private_mqfd;
	FAR struct timespec time;
	struct mq_attr attr;

	if (q_name == NULL) {
		return TM_INVALID_PARAM;
	}

	attr.mq_maxmsg = CONFIG_TASK_MANAGER_MAX_MSG;
	attr.mq_msgsize = sizeof(tm_response_t);
	attr.mq_flags = 0;

	private_mqfd = mq_open(q_name, O_RDONLY | O_CREAT, 0666, &attr);
	if (private_mqfd == (mqd_t)ERROR) {
		tmdbg("mq_open failed!\n");
		return TM_FAIL_RESPONSE;
	}

	if (timeout == TM_RESPONSE_WAIT_INF) {
		status = mq_receive(private_mqfd, (char *)response_msg, sizeof(tm_response_t), 0);
	} else {
		time.tv_sec = timeout / MSEC_PER_SEC;
		time.tv_nsec = (timeout - MSEC_PER_SEC * time.tv_sec) * NSEC_PER_MSEC;

		status = mq_timedreceive(private_mqfd, (char *)response_msg, sizeof(tm_response_t), 0, &time);
	}

	mq_close(private_mqfd);
	mq_unlink(q_name);

	if (status <= 0) {
		tmdbg("mq_receive failed! %d\n", errno);
		return TM_FAIL_RESPONSE;
	}

	return OK;
}

static int taskmgr_send_request(tm_request_t *request_msg)
{
	int status;
	struct mq_attr attr;

	attr.mq_maxmsg = CONFIG_TASK_MANAGER_MAX_MSG;
	attr.mq_msgsize = sizeof(tm_request_t);
	attr.mq_flags = 0;

	g_tm_send_mqfd = mq_open(TM_PUBLIC_MQ, O_WRONLY | O_CREAT, 0666, &attr);
	if (g_tm_send_mqfd < 0) {
		tmdbg("mq_open failed!\n");
		return TM_FAIL_REQ_TO_MGR;
	}

	status = mq_send(g_tm_send_mqfd, (const char *)request_msg, sizeof(tm_request_t), TM_MQ_PRIO);
	if (status < 0) {
		tmdbg("mq_send failed! %d\n", errno);
		mq_close(g_tm_send_mqfd);
		return TM_FAIL_REQ_TO_MGR;
	}

	mq_close(g_tm_send_mqfd);

	return OK;
}

int task_manager_register(char *name, int permission, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (name == NULL || timeout < TM_RESPONSE_WAIT_INF || permission < TM_TASK_PERMISSION_ALL || permission > TM_TASK_PERMISSION_DEDICATE) {
		return TM_INVALID_PARAM;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	request_msg.cmd = TASKMGT_REGISTER_TASK;
	request_msg.caller_pid = getpid();
	request_msg.handle = permission;
	request_msg.data = (void *)TM_ALLOC(strlen(name) + 1);
	strncpy((char *)request_msg.data, name, strlen(name) + 1);
	request_msg.timeout = timeout;

	if (timeout != TM_NO_RESPONSE) {
		asprintf(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		return TM_FAIL_REQ_TO_MGR;
	}

	if (timeout != TM_NO_RESPONSE) {
		status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
		TM_FREE(request_msg.q_name);
		if (status == OK) {
			status = response_msg.status;
		}
		return status;
	}

	return OK;
}

int task_manager_unregister(int handle, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (handle < 0 || timeout < TM_RESPONSE_WAIT_INF) {
		return TM_INVALID_PARAM;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	request_msg.cmd = TASKMGT_UNREGISTER_TASK;
	request_msg.handle = handle;
	request_msg.timeout = timeout;

	if (timeout != TM_NO_RESPONSE) {
		asprintf(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		return TM_FAIL_REQ_TO_MGR;
	}

	if (timeout != TM_NO_RESPONSE) {
		status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
		TM_FREE(request_msg.q_name);
		if (status == OK) {
			status = response_msg.status;
		}
		return status;
	}

	return OK;
}

int task_manager_unicast(int handle, void *msg, int msg_size, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (handle < 0 || timeout < TM_RESPONSE_WAIT_INF) {
		return TM_INVALID_PARAM;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	if (msg != NULL) {
		if (msg_size == 0) {
			return TM_INVALID_PARAM;
		}
		request_msg.data = (void *)TM_ALLOC(msg_size);
		strncpy((char *)request_msg.data, msg, msg_size);
	} else {
		request_msg.data = NULL;
	}

	/* Set the request msg */
	request_msg.cmd = TASKMGT_UNICAST;
	request_msg.handle = handle;
	request_msg.caller_pid = getpid();
	request_msg.timeout = timeout;

	if (timeout != TM_NO_RESPONSE) {
		asprintf(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		return TM_FAIL_REQ_TO_MGR;
	}

	if (timeout != TM_NO_RESPONSE) {
		status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
		TM_FREE(request_msg.q_name);
		if (status == OK) {
			status = response_msg.status;
		}
		return status;
	}

	return OK;
}

int task_manager_start(int handle, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (handle < 0 || timeout < TM_RESPONSE_WAIT_INF) {
		return TM_INVALID_PARAM;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	/* Set the request msg */
	request_msg.cmd = TASKMGT_START;
	request_msg.handle = handle;
	request_msg.caller_pid = getpid();
	request_msg.timeout = timeout;

	if (timeout != TM_NO_RESPONSE) {
		asprintf(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		return TM_FAIL_REQ_TO_MGR;
	}

	if (timeout != TM_NO_RESPONSE) {
		status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
		TM_FREE(request_msg.q_name);
		if (status == OK) {
			status = response_msg.status;
		}
		return status;
	}

	return OK;
}

int task_manager_terminate(int handle, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (handle < 0 || timeout < TM_RESPONSE_WAIT_INF) {
		return TM_INVALID_PARAM;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	/* Set the request msg */
	request_msg.cmd = TASKMGT_TERMINATE;
	request_msg.handle = handle;
	request_msg.caller_pid = getpid();
	request_msg.timeout = timeout;

	if (timeout != TM_NO_RESPONSE) {
		asprintf(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		return TM_FAIL_REQ_TO_MGR;
	}

	if (timeout != TM_NO_RESPONSE) {
		status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
		TM_FREE(request_msg.q_name);
		if (status == OK) {
			status = response_msg.status;
		}
		return status;
	}

	return OK;
}

#if TBD
int task_manager_pause(int handle, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (handle < 0 || timeout < TM_RESPONSE_WAIT_INF) {
		return TM_INVALID_PARAM;
	}

	/* Set the request msg */
	request_msg.cmd = TASKMGT_PAUSE;
	request_msg.handle = handle;
	request_msg.caller_pid = getpid();
	request_msg.timeout = timeout;

	if (timeout != TM_NO_RESPONSE) {
		asprintf(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		return TM_FAIL_REQ_TO_MGR;
	}

	if (timeout != TM_NO_RESPONSE) {
		status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
		TM_FREE(request_msg.q_name);
		if (status == OK) {
			status = response_msg.status;
		}
		return status;
	}

	return OK;
}

int task_manager_resume(int handle, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (handle < 0 || timeout < TM_RESPONSE_WAIT_INF) {
		return TM_INVALID_PARAM;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	/* Set the request msg */
	request_msg.cmd = TASKMGT_RESUME;
	request_msg.handle = handle;
	request_msg.caller_pid = getpid();
	request_msg.timeout = timeout;

	if (timeout != TM_NO_RESPONSE) {
		asprintf(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		return TM_FAIL_REQ_TO_MGR;
	}

	if (timeout != TM_NO_RESPONSE) {
		status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
		TM_FREE(request_msg.q_name);
		if (status == OK) {
			status = response_msg.status;
		}
		return status;
	}

	return OK;
}

int task_manager_restart(int handle, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (handle < 0 || timeout < TM_RESPONSE_WAIT_INF) {
		return TM_INVALID_PARAM;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	/* Set the request msg */
	request_msg.cmd = TASKMGT_RESTART;
	request_msg.handle = handle;
	request_msg.caller_pid = getpid();
	request_msg.timeout = timeout;

	if (timeout != TM_NO_RESPONSE) {
		asprintf(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
	}

	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		return TM_FAIL_REQ_TO_MGR;
	}

	if (timeout != TM_NO_RESPONSE) {
		status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);
		TM_FREE(request_msg.q_name);
		if (status == OK) {
			status = response_msg.status;
		}
		return status;
	}

	return OK;
}
#endif

int task_manager_set_handler(void (*func)(int signo, tm_msg_t *data))
{
	int ret = OK;
	struct sigaction act;

	if (func == NULL) {
		return TM_INVALID_PARAM;
	}

	act.sa_sigaction = (_sa_sigaction_t)func;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);

	ret = sigaddset(&act.sa_mask, SIGTM);
	if (ret < 0) {
		return TM_FAIL_SET_HANDLER;
	}

	ret = sigaction(SIGTM, &act, NULL);
	if (ret == (int)SIG_ERR) {
		ret = TM_FAIL_SET_HANDLER;
	}
	return OK;
}

task_info_list_t *task_manager_getinfo_with_name(char *name, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (name == NULL || timeout < TM_RESPONSE_WAIT_INF || timeout == TM_NO_RESPONSE) {
		return NULL;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));

	/* Set the request msg */
	request_msg.cmd = TASKMGT_SCAN_NAME;
	request_msg.data = (void *)TM_ALLOC(strlen(name) + 1);
	strncpy((char *)request_msg.data, name, strlen(name) + 1);
	request_msg.timeout = timeout;

	asprintf(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, getpid());

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

task_info_t *task_manager_getinfo_with_handle(int handle, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (handle < 0 || handle > CONFIG_TASK_MANAGER_MAX_TASKS || timeout < TM_RESPONSE_WAIT_INF || timeout == TM_NO_RESPONSE) {
		return NULL;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));

	/* Set the request msg */
	request_msg.cmd = TASKMGT_SCAN_HANDLE;
	request_msg.handle = handle;
	request_msg.timeout = timeout;

	asprintf(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, getpid());

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

	return &response_msg.data->task;
}

task_info_list_t *task_manager_getinfo_with_group(int group, int timeout)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;

	if (group < 0 || timeout < TM_RESPONSE_WAIT_INF || timeout == TM_NO_RESPONSE) {
		return NULL;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));

	/* Set the request msg */
	request_msg.cmd = TASKMGT_SCAN_GROUP;
	request_msg.handle = group;
	request_msg.timeout = timeout;

	asprintf(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, getpid());

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


void task_manager_clean_info(task_info_t **info)
{
	if (*info == NULL) {
		return;
	}

	if ((*info)->name != NULL) {
		TM_FREE((*info)->name);
		(*info)->name = NULL;
	}
	TM_FREE(*info);

	*info = NULL;
}

void task_manager_clean_infolist(task_info_list_t **info_list)
{
	task_info_list_t *curr_info;

	if (*info_list == NULL) {
		return;
	}

	while (*info_list != NULL) {
		curr_info = *info_list;
		*info_list = (*info_list)->next;
		if ((curr_info)->task.name != NULL) {
			TM_FREE(curr_info->task.name);
			curr_info->task.name = NULL;
		}
		curr_info->next = NULL;
		TM_FREE(curr_info);
	}
}
