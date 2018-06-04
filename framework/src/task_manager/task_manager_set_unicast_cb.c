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
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

void taskmgr_unicast_cb(int signo, tm_msg_t *data)
{
	int handle;
	handle = taskmgr_get_handle_by_pid(getpid());
	if (handle == TM_FAIL_UNREGISTERED_TASK) {
		tmdbg("Fail to start the unicast callback\n");
		return;
	}
	(*TASK_UNICAST_CB(handle))((void *)data->si_value.sival_ptr);
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

	act.sa_sigaction = (_sa_sigaction_t)taskmgr_unicast_cb;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);

	ret = sigaddset(&act.sa_mask, SIGTM_UNICAST);
	if (ret < 0) {
		return TM_FAIL_SET_HANDLER;
	}

	ret = sigaction(SIGTM_UNICAST, &act, NULL);
	if (ret == (int)SIG_ERR) {
		return TM_FAIL_SET_HANDLER;
	}

	/* send user defined callback function to task manager */

	memset(&request_msg, 0, sizeof(tm_request_t));
	/* Set the request msg */
	request_msg.cmd = TASKMGT_SET_UNICAST_CB;
	request_msg.caller_pid = getpid();
	request_msg.data = (void *)func;


	ret = taskmgr_send_request(&request_msg);
	if (ret < 0) {
		return TM_FAIL_REQ_TO_MGR;
	}

	return ret;
}

/****************************************************************************
 * task_manager_set_termination_cb
 ****************************************************************************/
int task_manager_set_termination_cb(void (*func)(void))
{
	if (func == NULL) {
		return TM_INVALID_PARAM;
	}

	if (atexit((void *)func) != OK) {
		return TM_FAIL_SET_TERMINATION_CB;
	}

	return OK;
}
