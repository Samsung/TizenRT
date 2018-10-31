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
#include <debug.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <queue.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <tinyara/wdog.h>
#include <eventloop/eventloop.h>

#include "eventloop_internal.h"

sq_queue_t g_timer_list;  // list node type : el_handle_t

static void eventloop_timer_callback(int argc, el_handle_t *handle);

static int eventloop_get_timeout_abstime_ticks(uint32_t timeout)
{
	time_t sec;
	long nsec;
	int ticks;
	struct timespec abstime;

	sec = timeout / MSEC_PER_SEC;
	nsec = (timeout - MSEC_PER_SEC * sec) * NSEC_PER_MSEC;
	ticks = -1;

	(void)clock_gettime(CLOCK_REALTIME, &abstime);

	abstime.tv_sec += sec;
	abstime.tv_nsec += nsec;
	if (abstime.tv_nsec >= NSEC_PER_SEC) {
		abstime.tv_sec++;
		abstime.tv_nsec -= NSEC_PER_SEC;
	}

	clock_abstime2ticks(CLOCK_REALTIME, &abstime, &ticks);

	elvdbg("timeout ticks %d\n", ticks);

	return ticks;
}

static int eventloop_timer_start(el_handle_t *handle)
{
	int ret;
	int ticks;
	el_timer_t *timer;

	if (handle == NULL) {
		return ERROR;
	}

	timer = &handle->data.timer;

	if (timer->wdog == NULL) {
		/* Create a new watchdog */
		timer->wdog = wd_create();
		if (timer->wdog == NULL) {
			eldbg("Failed to create watchdog\n");
			return ERROR;
		}
	}

	ticks = eventloop_get_timeout_abstime_ticks(timer->timeout);
	if (ticks < 0) {
		goto errout;
	}

	ret = wd_start(timer->wdog, ticks, (wdentry_t)eventloop_timer_callback, 1, handle);
	if (ret != OK) {
		eldbg("Failed to start watchdog\n");
		goto errout;
	}
	elvdbg("[%d] watchdog start! %p ticks %d\n", getpid(), timer->wdog, ticks);
	sq_addlast((FAR sq_entry_t *)handle, &g_timer_list);

	return OK;
errout:
	wd_delete(timer->wdog);

	return ERROR;
}

/* Callback function called by watchdog when time is expired */
static void eventloop_timer_callback(int argc, el_handle_t *handle)
{
	int ret;
	bool is_empty;

	if (handle == NULL) {
		return;
	}

	elvdbg("[%d] watchdog callback %p timer %p\n", getpid(), handle, &handle->data.timer);

	/* Remove timer handle from g_timer_list and add it to a jobs list of app */
	sq_rem((FAR sq_entry_t *)handle, &g_timer_list);

	ret = eventloop_job_empty(handle, &is_empty);
	if (ret != OK) {
		return;
	}

	/* Add handle to a queue of jobs */
	ret = eventloop_push_job(handle);
	if (ret == OK && is_empty) {
		/* Send signal to notify event */
		ret = kill(handle->pid, SIGEL_EVENT);
		elvdbg("[%d] watchdog kill %d\n", getpid(), handle->pid);
		if (ret == OK) {
		} else {
			eldbg("Fail to send signal, pid %d errno %d\n", handle->pid, errno);
		}
	}

	/* Wake up task */
	if (eventloop_get_state(handle->pid) == EL_STATE_WAIT_WAKEUP) {
		elvdbg("[%d] SEND WAKEUP [%d]\n", getpid(), handle->pid);
		ret = kill(handle->pid, SIGEL_WAKEUP);
		if (ret != OK) {
			eldbg("Fail to send signal, errno %d\n", errno);
			return;
		}
	}
}

void eventloop_clean_timer_handle(el_handle_t *handle)
{
	int ret;
	el_timer_t *timer;

	if (handle == NULL) {
		return;
	}

	timer = (el_timer_t *)&handle->data.timer;

	/* Clean timer handle */
	ret = wd_delete(timer->wdog);
	if (ret != OK) {
		eldbg("Failed to delete watchdog\n");
		return;
	}
	timer->wdog = NULL;

	eventloop_decrease_app_handle_cnt(handle->pid);

	EL_FREE(handle);
}

/* Remove all timers that task/pthread 'pid' registered */
void eventloop_clean_app_timers(void)
{
	el_handle_t *rem_handle;
	el_handle_t *handle;

	handle = (el_handle_t *)sq_peek(&g_timer_list);
	while (handle != NULL) {
		if (handle->mode == EL_MODE_DEFAULT && handle->pid == getpid()) {
			sq_rem((sq_entry_t *)handle, &g_timer_list);
			rem_handle = handle;
			handle = (el_handle_t *)sq_next(handle);
			eventloop_clean_timer_handle(rem_handle);
			continue;
		}
		handle = (el_handle_t *)sq_next(handle);
	}
}

/* Timer callback function called in user side */
void eventloop_timer_user_callback(el_handle_t *handle)
{
	int ret;
	el_timer_t *timer;

	if (handle == NULL) {
		return;
	}

	elvdbg("[%d] Timer Callback!\n", getpid());

	timer = (el_timer_t *)&handle->data.timer;
	if (timer->cb_func) {
		/* Execute timer callback */
		ret = timer->cb_func(timer->cb_data);
		if (ret == EVENTLOOP_CALLBACK_CONTINUE) {
			ret = eventloop_timer_start(handle);
			if (ret == OK) {
				elvdbg("watchdog restart\n");
				return;
			}
		}
	}

	/* Clean timer handle */
	eventloop_clean_timer_handle(handle);
}

el_timer_t *eventloop_add_timer(uint32_t timeout, bool repeat, timeout_callback func, void *data)
{
	int ret;
	el_timer_t *timer;
	el_handle_t *handle;

	if (func == NULL) {
		eldbg("Invalid parameter\n");
		return NULL;
	}

	/* Set timer handle */
	handle = (el_handle_t *)EL_ALLOC(sizeof(el_handle_t));
	if (handle == NULL) {
		return NULL;
	}
	handle->pid = getpid();
	handle->type = EL_HANDLE_TYPE_TIMER;
	handle->mode = EL_MODE_DEFAULT;

	timer = (el_timer_t *)&handle->data.timer;
	timer->timeout = timeout;
	timer->repeat = repeat;
	timer->cb_func = func;
	timer->cb_data = data;
	timer->wdog = NULL;

	/* Set a handler to call user callback */
	ret = eventloop_set_user_cb_handler();
	if (ret != OK) {
		EL_FREE(handle);
		return NULL;
	}

	/* Start timer */
	ret = eventloop_timer_start(handle);
	if (ret != OK) {
		EL_FREE(handle);
		return NULL;
	}
	eventloop_increase_app_handle_cnt(handle->pid);

	return timer;
}

el_timer_t *eventloop_add_timer_async(unsigned int timeout, bool repeat, timeout_callback func, void *cb_data)
{
	int ret;
	int async_pid;
	el_timer_t *timer;
	el_handle_t *handle;

	if (func == NULL) {
		eldbg("Invalid parameter\n");
		return NULL;
	}

	async_pid = get_async_task();

	/* Validate that event loop task is alive. */
	if (async_pid < 0) {
		eldbg("Failed to add timer event to event loop task. \n");
		return NULL;
	}

	/* Set timer handle */
	handle = (el_handle_t *)EL_ALLOC(sizeof(el_handle_t));
	if (handle == NULL) {
		return NULL;
	}
	handle->pid = async_pid;
	handle->type = EL_HANDLE_TYPE_TIMER;
	handle->mode = EL_MODE_DEFAULT;

	timer = (el_timer_t *)&handle->data.timer;
	timer->timeout = timeout;
	timer->repeat = repeat;
	timer->cb_func = func;
	timer->cb_data = cb_data;
	timer->wdog = NULL;

	/* Start timer */
	ret = eventloop_timer_start(handle);
	if (ret != OK) {
		EL_FREE(handle);
		return NULL;
	}
	eventloop_increase_app_handle_cnt(handle->pid);

	return timer;
}

int eventloop_delete_timer(el_timer_t *timer)
{
	el_handle_t *handle;

	if (timer == NULL) {
		eldbg("Invalid parameter\n");
		return EVENTLOOP_INVALID_PARAM;
	}

	handle = (el_handle_t *)sq_peek(&g_timer_list);
	while (handle != NULL) {
		if (&handle->data.timer == timer) {
			sq_rem((sq_entry_t *)handle, &g_timer_list);
			eventloop_clean_timer_handle(handle);
			return OK;
		}
		handle = (el_handle_t *)sq_next(handle);
	}
	return EVENTLOOP_INVALID_HANDLE;

}

int eventloop_set_timer_mode(el_timer_t *timer, int mode)
{
	el_handle_t *handle;

	if (timer == NULL || mode < 0 || mode >= EL_MODE_MAX) {
		eldbg("Invalid Parameter\n");
		return EVENTLOOP_INVALID_PARAM;
	}

	handle = (el_handle_t *)sq_peek(&g_timer_list);
	while (handle != NULL) {
		if (&handle->data.timer == timer) {
			handle->mode = mode;
			return OK;
		}
		handle = (el_handle_t *)sq_next(handle);
	}

	return EVENTLOOP_INVALID_HANDLE;
}
