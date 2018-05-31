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
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

/****************************************************************************
 * task_manager_set_broadcast_handler
 ****************************************************************************/
int task_manager_set_broadcast_handler(int msg_filter, void (*func)(int signo, tm_msg_t *data))
{
	int ret;
	tm_request_t request_msg;
	struct sigaction act;

	if (msg_filter < 0 || func == NULL) {
		return TM_INVALID_PARAM;
	}

	act.sa_sigaction = (_sa_sigaction_t)func;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);

	ret = sigaddset(&act.sa_mask, SIGTM_BROADCAST);
	if (ret < 0) {
		return TM_FAIL_SET_HANDLER;
	}

	ret = sigaction(SIGTM_BROADCAST, &act, NULL);
	if (ret == (int)SIG_ERR) {
		return TM_FAIL_SET_HANDLER;
	}

	memset(&request_msg, 0, sizeof(tm_request_t));
	request_msg.cmd = TASKMGT_SET_BROADCAST_HANDLER;
	request_msg.caller_pid = getpid();
	request_msg.data = (void *)TM_ALLOC(sizeof(int));
	if (request_msg.data != NULL) {
		*((int *)request_msg.data) = msg_filter;
	} else {
		return TM_OUT_OF_MEMORY;
	}
	ret = taskmgr_send_request(&request_msg);
	if (ret < 0) {
		return TM_FAIL_REQ_TO_MGR;
	}
	return OK;
}
