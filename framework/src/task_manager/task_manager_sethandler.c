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
#include <sys/types.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

/****************************************************************************
 * task_manager_set_handler
 ****************************************************************************/
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

	ret = sigaddset(&act.sa_mask, SIGTM_UNICAST);
	if (ret < 0) {
		return TM_FAIL_SET_HANDLER;
	}

	ret = sigaction(SIGTM_UNICAST, &act, NULL);
	if (ret == (int)SIG_ERR) {
		ret = TM_FAIL_SET_HANDLER;
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
