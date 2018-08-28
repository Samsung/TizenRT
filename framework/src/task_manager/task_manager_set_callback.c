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
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <task_manager/task_manager.h>
#ifdef CONFIG_TASK_MANAGER_USER_SPECIFIC_BROADCAST
#include <task_manager/task_manager_broadcast_list.h>
#endif
#include "task_manager_internal.h"

void taskmgr_msg_cb(int signo, siginfo_t *data)
{
	int handle;
	void *broadcast_param = NULL;
	void *user_data;
	void *cb_data;
	int user_data_size;
	tm_msg_t unicast_param;

	handle = taskmgr_get_handle_by_pid(getpid());
	if (handle == TM_UNREGISTERED_APP) {
		tmdbg("Fail to get handle by pid\n");
		return;
	}
	if (signo == CONFIG_SIG_SIGTM_UNICAST) {
		unicast_param.msg_size = ((tm_msg_t *)data->si_value.sival_ptr)->msg_size;
		unicast_param.msg = TM_ALLOC(unicast_param.msg_size);
		if (unicast_param.msg == NULL) {
			tmdbg("Fail to alloc user data\n");
			return;
		}
		memcpy(unicast_param.msg, ((tm_msg_t *)data->si_value.sival_ptr)->msg, unicast_param.msg_size);
		TM_FREE(((tm_msg_t *)data->si_value.sival_ptr)->msg);

		(*TM_UNICAST_CB(handle))(&unicast_param);
		TM_FREE(unicast_param.msg);
	} else {
		user_data = ((tm_broadcast_internal_msg_t *)data->si_value.sival_ptr)->user_data;
		user_data_size = ((tm_broadcast_internal_msg_t *)data->si_value.sival_ptr)->size;
		cb_data = ((tm_broadcast_info_t *)((tm_broadcast_internal_msg_t *)data->si_value.sival_ptr)->info)->cb_data;

		if (user_data_size != 0) {
			broadcast_param = TM_ALLOC(((tm_broadcast_internal_msg_t *)data->si_value.sival_ptr)->size);
			if (broadcast_param == NULL) {
				tmdbg("Fail to alloc user data\n");
				return;
			}
			memcpy(broadcast_param, user_data, user_data_size);
		}

		(*((tm_broadcast_info_t *)((tm_broadcast_internal_msg_t *)data->si_value.sival_ptr)->info)->cb)(broadcast_param, cb_data);

		TM_FREE(broadcast_param);
		TM_FREE(user_data);
		broadcast_param = NULL;
	}
	TM_FREE(data->si_value.sival_ptr);
}
/****************************************************************************
 * task_manager_set_unicast_cb
 ****************************************************************************/
int task_manager_set_unicast_cb(void (*func)(tm_msg_t *data))
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
int task_manager_set_broadcast_cb(int msg, void (*func)(void *user_data, void *data), void *cb_data)
{
	int status;
	tm_request_t request_msg;
	tm_response_t response_msg;
	struct sigaction act;

	if (msg < 0 || func == NULL) {
		return TM_INVALID_PARAM;
	}
	act.sa_sigaction = (_sa_sigaction_t)taskmgr_msg_cb;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);
	status = sigaddset(&act.sa_mask, SIGTM_BROADCAST);
	if (status < 0) {
		tmdbg("Failed to add signal set\n");
		return TM_OPERATION_FAIL;
	}
	status = sigaction(SIGTM_BROADCAST, &act, NULL);
	if (status == (int)SIG_ERR) {
		tmdbg("sigaction Failed\n");
		return TM_OPERATION_FAIL;
	}
	memset(&request_msg, 0, sizeof(tm_request_t));
	request_msg.cmd = TASKMGRCMD_SET_BROADCAST_CB;
	request_msg.caller_pid = getpid();
	request_msg.timeout = TM_RESPONSE_WAIT_INF;
	request_msg.data = (void *)TM_ALLOC(sizeof(tm_broadcast_info_t));
	if (request_msg.data == NULL) {
		return TM_OUT_OF_MEMORY;
	}
	((tm_broadcast_info_t *)request_msg.data)->msg = msg;
	((tm_broadcast_info_t *)request_msg.data)->cb = (_tm_broadcast_t)func;
	((tm_broadcast_info_t *)request_msg.data)->cb_data = cb_data;
	TM_ASPRINTF(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
	if (request_msg.q_name == NULL) {
		TM_FREE(request_msg.data);
		return TM_OUT_OF_MEMORY;
	}
	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		TM_FREE(request_msg.data);
		if (request_msg.q_name != NULL) {
			TM_FREE(request_msg.q_name);
		}
		return status;
	}
	status = taskmgr_receive_response(request_msg.q_name, &response_msg, TM_RESPONSE_WAIT_INF);
	TM_FREE(request_msg.q_name);
	
	return status;
}

/****************************************************************************
 * task_manager_set_exit_cb
 ****************************************************************************/
int task_manager_set_exit_cb(void (*func)(void *data), void *cb_data)
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
	request_msg.timeout = TM_NO_RESPONSE;
	request_msg.data = (void *)TM_ALLOC(sizeof(tm_termination_info_t));
	if (request_msg.data == NULL) {
		return TM_OUT_OF_MEMORY;
	}
	((tm_termination_info_t *)request_msg.data)->cb = (_tm_termination_t)func;
	((tm_termination_info_t *)request_msg.data)->cb_data = cb_data;
	ret = taskmgr_send_request(&request_msg);
	if (ret < 0) {
		TM_FREE(request_msg.data);
		return ret;
	}

	return ret;
}

/****************************************************************************
 * task_manager_set_stop_cb
 ****************************************************************************/
int task_manager_set_stop_cb(void (*func)(void *data), void *cb_data)
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
	request_msg.timeout = TM_NO_RESPONSE;
	request_msg.data = (void *)TM_ALLOC(sizeof(tm_termination_info_t));
	if (request_msg.data == NULL) {
		return TM_OUT_OF_MEMORY;
	}
	((tm_termination_info_t *)request_msg.data)->cb = (_tm_termination_t)func;
	((tm_termination_info_t *)request_msg.data)->cb_data = cb_data;
	ret = taskmgr_send_request(&request_msg);
	if (ret < 0) {
		TM_FREE(request_msg.data);
		return ret;
	}

	return ret;
}
