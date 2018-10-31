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
#include <debug.h>
#include <unistd.h>
#include <queue.h>
#include <eventloop/eventloop.h>

#include "eventloop_internal.h"

el_app_handle_t g_el_app_list[CONFIG_MAX_TASKS];

#define MAX_PID_MASK   (CONFIG_MAX_TASKS - 1)
#define APP_IDX_HASH(pid)  ((pid) & MAX_PID_MASK)

el_app_handle_t *eventloop_get_app_handle(void)
{
	return &g_el_app_list[APP_IDX_HASH(getpid())];
}

el_handle_t *eventloop_pop_job(void)
{
	return (el_handle_t *)sq_remfirst(&g_el_app_list[APP_IDX_HASH(getpid())].jobs);
}

int eventloop_push_job(el_handle_t *handle)
{
	if (handle == NULL || handle->pid < 0) {
		eldbg("Invalid handle\n");
		return ERROR;
	}

	sq_addlast((FAR sq_entry_t *)handle, &g_el_app_list[APP_IDX_HASH(handle->pid)].jobs);

	return OK;
}

int eventloop_job_empty(el_handle_t *handle, bool *result)
{
	if (handle == NULL || handle->pid < 0) {
		eldbg("Invalid handle\n");
		return ERROR;
	}

	*result = sq_empty(&g_el_app_list[APP_IDX_HASH(handle->pid)].jobs);

	return OK;
}

void eventloop_increase_app_handle_cnt(int pid)
{
	if (pid >= 0) {
		g_el_app_list[APP_IDX_HASH(pid)].count++;
		eldbg("App %d handles increased %d\n", pid, g_el_app_list[APP_IDX_HASH(pid)].count);
	}
}

void eventloop_decrease_app_handle_cnt(int pid)
{
	if (pid >= 0) {
		g_el_app_list[APP_IDX_HASH(pid)].count--;
		eldbg("App %d handles decreased %d\n", pid, g_el_app_list[APP_IDX_HASH(pid)].count);
	}
}

int eventloop_get_app_handle_cnt(void)
{
	return g_el_app_list[APP_IDX_HASH(getpid())].count;
}

int eventloop_get_state(int pid)
{
	if (pid < 0) {
		eldbg("Invalid parameter, pid\n");
		return ERROR;
	}
	return g_el_app_list[APP_IDX_HASH(pid)].state;
}

/* User callback for processing incomming events */
static void eventloop_user_cb(int signum)
{
	el_handle_t *handle;

	elvdbg("User Callback\n");

	while ((handle = (el_handle_t *)eventloop_pop_job())) {
		elvdbg("[%d] handle %p pid = %d type = %d\n", getpid(), handle, handle->pid, handle->type);
		switch (handle->type) {
		case EL_HANDLE_TYPE_TIMER:
			eventloop_timer_user_callback(handle);
			break;
		case EL_HANDLE_TYPE_EVENT:
			eventloop_event_user_callback(handle);
			break;
		default:
			eldbg("Invalid type.\n");
		}
	}
}

/* Register callback which is called when event occurs */
int eventloop_set_user_cb_handler(void)
{
	int ret;
	struct sigaction act;

	act.sa_handler = (_sa_handler_t)eventloop_user_cb;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);

	ret = sigaddset(&act.sa_mask, SIGEL_EVENT);
	if (ret < 0) {
		eldbg("Failed to add signal set\n");
		return ERROR;
	}

	ret = sigaction(SIGEL_EVENT, &act, NULL);
	if (ret == (int)SIG_ERR) {
		eldbg("sigaction Failed\n");
		return ERROR;
	}

	return OK;
}
