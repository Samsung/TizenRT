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

#define REQ_CBFUNC(X)          ((tm_termination_info_t *)X.data)->cb
#define REQ_CBDATA(X)          ((tm_termination_info_t *)X.data)->cb_data
#define REQ_CBDATA_MSG_SIZE(X) ((tm_msg_t *)((tm_termination_info_t *)X.data)->cb_data)->msg_size
#define REQ_CBDATA_MSG(X)      ((tm_msg_t *)((tm_termination_info_t *)X.data)->cb_data)->msg

void taskmgr_msg_cb(int signo, siginfo_t *data)
{
	int handle;
	tm_msg_t *broadcast_param;
	void *broadcast_data = NULL;
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
		cb_data = ((tm_broadcast_info_t *)((tm_broadcast_internal_msg_t *)data->si_value.sival_ptr)->cb_info)->cb_data;

		if (user_data_size >= 0) {
			broadcast_param = (tm_msg_t *)TM_ALLOC(sizeof(tm_msg_t));
			if (broadcast_param == NULL) {
				tmdbg("Fail to alloc broadcast param\n");
				return;
			}
			if (user_data_size != 0) {
				broadcast_data = TM_ALLOC(user_data_size);
				if (broadcast_data == NULL) {
					TM_FREE(broadcast_param);
					tmdbg("Fail to alloc broadcast data\n");
					return;
				}
				broadcast_param->msg = broadcast_data;
				broadcast_param->msg_size = user_data_size;
				memcpy(broadcast_data, user_data, user_data_size);
			} else {
				broadcast_param->msg = NULL;
				broadcast_param->msg_size = 0;
			}
		} else {
			broadcast_param = NULL;
		}

		(*((tm_broadcast_info_t *)((tm_broadcast_internal_msg_t *)data->si_value.sival_ptr)->cb_info)->cb)(broadcast_param, cb_data);

		if (broadcast_param != NULL) {
			TM_FREE(broadcast_param);
			broadcast_param = NULL;
		}
		if (broadcast_data != NULL) {
			TM_FREE(broadcast_data);
			broadcast_data = NULL;
		}
		if (user_data != NULL) {
			TM_FREE(user_data);
			user_data = NULL;
		}
	}
	TM_FREE(data->si_value.sival_ptr);
}

void taskmgr_stop_cb(int signo, siginfo_t *data)
{
	int taskmgr_pid;
	union sigval msg;
	tm_termination_info_t *cb_info;
	cb_info = (tm_termination_info_t *)data->si_value.sival_ptr;

	/* Call callback function with callback data */
	if (cb_info != NULL && cb_info->cb != NULL) {
		if (cb_info->cb_data != NULL) {
			(*cb_info->cb)(cb_info->cb_data->msg);
			TM_FREE(cb_info->cb_data->msg);
			TM_FREE(cb_info->cb_data);
		} else {
			(*cb_info->cb)(NULL);
		}
	} else {
		tmdbg("stop callback information is not correct.\n");
		return;
	}
	TM_FREE(cb_info);

	taskmgr_pid = taskmgr_get_task_manager_pid();
	if (taskmgr_pid == TM_TASK_MGR_NOT_ALIVE) {
		tmdbg("Task Manager is not alive\n");
		return;
	}
	msg.sival_int = getpid();

	(void)sigqueue(taskmgr_pid, SIGTM_TERMINATION, msg);
}

/****************************************************************************
 * task_manager_set_unicast_cb
 ****************************************************************************/
int task_manager_set_unicast_cb(tm_unicast_callback_t func)
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
int task_manager_set_broadcast_cb(int msg, tm_broadcast_callback_t func, tm_msg_t *cb_data)
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
	((tm_broadcast_info_t *)request_msg.data)->cb = (tm_broadcast_callback_t)func;
	if (cb_data != NULL) {
		((tm_broadcast_info_t *)request_msg.data)->cb_data = (tm_msg_t *)TM_ALLOC(sizeof(tm_msg_t));
		if (((tm_broadcast_info_t *)request_msg.data)->cb_data == NULL) {
			TM_FREE(request_msg.data);
			return TM_OUT_OF_MEMORY;
		}
		if (cb_data->msg_size != 0) {
			((tm_msg_t *)((tm_broadcast_info_t *)request_msg.data)->cb_data)->msg_size = cb_data->msg_size;
			((tm_msg_t *)((tm_broadcast_info_t *)request_msg.data)->cb_data)->msg = TM_ALLOC(cb_data->msg_size);
			if (((tm_msg_t *)((tm_broadcast_info_t *)request_msg.data)->cb_data)->msg == NULL) {
				TM_FREE(request_msg.data);
				TM_FREE(((tm_broadcast_info_t *)request_msg.data)->cb_data);
				return TM_OUT_OF_MEMORY;
			}
			memcpy(((tm_msg_t *)((tm_broadcast_info_t *)request_msg.data)->cb_data)->msg, cb_data->msg, cb_data->msg_size);
		} else {
			((tm_msg_t *)((tm_broadcast_info_t *)request_msg.data)->cb_data)->msg_size = 0;
			((tm_msg_t *)((tm_broadcast_info_t *)request_msg.data)->cb_data)->msg = NULL;
		}
	} else {
		((tm_broadcast_info_t *)request_msg.data)->cb_data = NULL;
	}

	TM_ASPRINTF(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);
	if (request_msg.q_name == NULL) {
		TM_FREE(((tm_msg_t *)((tm_broadcast_info_t *)request_msg.data)->cb_data)->msg);
		TM_FREE(((tm_broadcast_info_t *)request_msg.data)->cb_data);
		TM_FREE(request_msg.data);
		return TM_OUT_OF_MEMORY;
	}
	status = taskmgr_send_request(&request_msg);
	if (status < 0) {
		TM_FREE(((tm_msg_t *)((tm_broadcast_info_t *)request_msg.data)->cb_data)->msg);
		TM_FREE(((tm_broadcast_info_t *)request_msg.data)->cb_data);
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
int task_manager_set_exit_cb(tm_termination_callback_t func, tm_msg_t *cb_data)
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
	REQ_CBFUNC(request_msg) = (tm_termination_callback_t)func;
	if (cb_data != NULL) {
		REQ_CBDATA(request_msg) = TM_ALLOC(sizeof(tm_msg_t));
		if (REQ_CBDATA(request_msg) == NULL) {
			TM_FREE(request_msg.data);
			return TM_OUT_OF_MEMORY;
		}
		REQ_CBDATA_MSG_SIZE(request_msg) = cb_data->msg_size;
		REQ_CBDATA_MSG(request_msg) = TM_ALLOC(cb_data->msg_size);
		if (REQ_CBDATA_MSG(request_msg) == NULL) {
			TM_FREE(REQ_CBDATA(request_msg));
			TM_FREE(request_msg.data);
			return TM_OUT_OF_MEMORY;
		}
		memcpy(REQ_CBDATA_MSG(request_msg), cb_data->msg, cb_data->msg_size);
	} else {
		REQ_CBDATA(request_msg) = NULL;
	}

	ret = taskmgr_send_request(&request_msg);
	if (ret < 0) {
		if (REQ_CBDATA(request_msg) != NULL) {
			TM_FREE(REQ_CBDATA_MSG(request_msg));
			TM_FREE(REQ_CBDATA(request_msg));
		}
		TM_FREE(request_msg.data);
		return ret;
	}

	return ret;
}

/****************************************************************************
 * task_manager_set_stop_cb
 ****************************************************************************/
int task_manager_set_stop_cb(tm_termination_callback_t func, tm_msg_t *cb_data)
{
	int ret = OK;
	struct sigaction act;
	tm_request_t request_msg;

	if (func == NULL) {
		return TM_INVALID_PARAM;
	}

	act.sa_sigaction = (_sa_sigaction_t)taskmgr_stop_cb;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);

	ret = sigaddset(&act.sa_mask, SIGTM_TERMINATION);
	if (ret < 0) {
		tmdbg("Failed to add signal set\n");
		return TM_OPERATION_FAIL;
	}

	ret = sigaction(SIGTM_TERMINATION, &act, NULL);
	if (ret == (int)SIG_ERR) {
		tmdbg("sigaction Failed\n");
		return TM_OPERATION_FAIL;
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
	REQ_CBFUNC(request_msg) = (tm_termination_callback_t)func;
	if (cb_data != NULL) {
		REQ_CBDATA(request_msg) = TM_ALLOC(sizeof(tm_msg_t));
		if (REQ_CBDATA(request_msg) == NULL) {
			TM_FREE(request_msg.data);
			return TM_OUT_OF_MEMORY;
		}
		REQ_CBDATA_MSG_SIZE(request_msg) = cb_data->msg_size;
		REQ_CBDATA_MSG(request_msg) = TM_ALLOC(cb_data->msg_size);
		if (REQ_CBDATA_MSG(request_msg) == NULL) {
			TM_FREE(REQ_CBDATA(request_msg));
			TM_FREE(request_msg.data);
			return TM_OUT_OF_MEMORY;
		}
		memcpy(REQ_CBDATA_MSG(request_msg), cb_data->msg, cb_data->msg_size);
	} else {
		REQ_CBDATA(request_msg) = NULL;
	}

	ret = taskmgr_send_request(&request_msg);
	if (ret < 0) {
		if (REQ_CBDATA(request_msg) != NULL) {
			TM_FREE(REQ_CBDATA_MSG(request_msg));
			TM_FREE(REQ_CBDATA(request_msg));
		}
		TM_FREE(request_msg.data);
		return ret;
	}

	return ret;
}
