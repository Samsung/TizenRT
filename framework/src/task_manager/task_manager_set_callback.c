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
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

void taskmgr_msg_cb(int signo, siginfo_t *data)
{
	int handle;
	handle = taskmgr_get_handle_by_pid(getpid());
	if (handle == TM_UNREGISTERED_APP) {
		tmdbg("Fail to get handle by pid\n");
		return;
	}
	if (signo == CONFIG_SIG_SIGTM_UNICAST) {
		(*TM_UNICAST_CB(handle))((void *)data->si_value.sival_ptr);
	} else {
		(*TM_BROADCAST_CB(handle))(data->si_value.sival_int);
	}
}
/****************************************************************************
 * task_manager_set_unicast_cb
 ****************************************************************************/
int task_manager_set_unicast_cb(void (*func)(void *data))
{
	int ret = OK;
	struct sigaction act;
	tm_request_t request_msg;

	if (func == NULL) {
		return TM_INVALID_PARAM;
	}

	act.sa_sigaction = (_sa_sigaction_t)taskmgr_msg_cb;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);

	ret = sigaddset(&act.sa_mask, SIGTM_UNICAST);
	if (ret < 0) {
		tmdbg("Failed to add signal set\n");
		return TM_OPERATION_FAIL;
	}

	ret = sigaction(SIGTM_UNICAST, &act, NULL);
	if (ret == (int)SIG_ERR) {
		tmdbg("sigaction Failed\n");
		return TM_OPERATION_FAIL;
	}

	/* send user defined callback function to task manager */

	memset(&request_msg, 0, sizeof(tm_request_t));
	/* Set the request msg */
	request_msg.cmd = TASKMGRCMD_SET_UNICAST_CB;
	request_msg.caller_pid = getpid();
	request_msg.data = (void *)func;
	request_msg.timeout = TM_NO_RESPONSE;
	ret = taskmgr_send_request(&request_msg);
	if (ret < 0) {
		return ret;
	}

	return ret;
}

/****************************************************************************
 * task_manager_set_broadcast_cb
 ****************************************************************************/
int task_manager_set_broadcast_cb(int msg_mask, void (*func)(int data))
{
	int ret;
	tm_request_t request_msg;
	struct sigaction act;

	if (msg_mask < 0 || func == NULL) {
		return TM_INVALID_PARAM;
	}

	act.sa_sigaction = (_sa_sigaction_t)taskmgr_msg_cb;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);

	ret = sigaddset(&act.sa_mask, SIGTM_BROADCAST);
	if (ret < 0) {
		tmdbg("Failed to add signal set\n");
		return TM_OPERATION_FAIL;
	}

	ret = sigaction(SIGTM_BROADCAST, &act, NULL);
	if (ret == (int)SIG_ERR) {
		tmdbg("sigaction Failed\n");
		return TM_OPERATION_FAIL;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	request_msg.cmd = TASKMGRCMD_SET_BROADCAST_CB;
	request_msg.caller_pid = getpid();
	request_msg.timeout = TM_NO_RESPONSE;
	request_msg.data = (void *)TM_ALLOC(sizeof(tm_broadcast_t));
	if (request_msg.data == NULL) {
		return TM_OUT_OF_MEMORY;
	}
	((tm_broadcast_t *)request_msg.data)->msg_mask = msg_mask;
	((tm_broadcast_t *)request_msg.data)->cb = (_tm_broadcast_t)func;

	ret = taskmgr_send_request(&request_msg);
	if (ret < 0) {
		TM_FREE(request_msg.data);
		return ret;
	}
	return OK;
}

/****************************************************************************
 * task_manager_set_exit_cb
 ****************************************************************************/
int task_manager_set_exit_cb(void (*func)(void))
{
	int ret = OK;
	tm_request_t request_msg;

	if (func == NULL) {
		return TM_INVALID_PARAM;
	}

	/* send exit callback function to task manager */
	memset(&request_msg, 0, sizeof(tm_request_t));
	/* Set the request msg */
	request_msg.cmd = TASKMGRCMD_SET_EXIT_CB;
	request_msg.caller_pid = getpid();
	request_msg.data = (void *)func;
	request_msg.timeout = TM_NO_RESPONSE;
	ret = taskmgr_send_request(&request_msg);
	if (ret < 0) {
		return ret;
	}

	return ret;
}

/****************************************************************************
 * task_manager_set_stop_cb
 ****************************************************************************/
int task_manager_set_stop_cb(void (*func)(void))
{
	int ret = OK;
	tm_request_t request_msg;

	if (func == NULL) {
		return TM_INVALID_PARAM;
	}

	/* send stop callback function to task manager */
	memset(&request_msg, 0, sizeof(tm_request_t));
	/* Set the request msg */
	request_msg.cmd = TASKMGRCMD_SET_STOP_CB;
	request_msg.caller_pid = getpid();
	request_msg.data = (void *)func;
	request_msg.timeout = TM_NO_RESPONSE;
	ret = taskmgr_send_request(&request_msg);
	if (ret < 0) {
		return ret;
	}

	return ret;
}
