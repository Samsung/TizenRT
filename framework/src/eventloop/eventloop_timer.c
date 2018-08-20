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
#include <stdbool.h>
#include <libtuv/uv.h>
#include <eventloop/eventloop.h>

#include "eventloop_internal.h"

struct timer_cb_s {
	timeout_callback func;
	void *cb_data;
};
typedef struct timer_cb_s timer_cb_t;

/* Eventloop calls this function when timeout.
 * It calls callback function registered by user, and frees the timer resource allocated for timeout once.
 */
static void timeout_callback_func(el_timer_t *timer)
{
	timer_cb_t *callback = NULL;

	if (timer == NULL || timer->data == NULL) {
		eldbg("Invalid callback timer\n");
		return;
	}

	callback = (timer_cb_t *)timer->data;

	elvdbg("[%d] timeout callback!!\n", getpid());
	if (callback->func) {
		callback->func(callback->cb_data);
	}
}

static el_timer_t *add_timer(el_loop_t *loop, unsigned int timeout, bool repeat, timeout_callback func, void *data)
{
	el_timer_t *timer;
	timer_cb_t *callback;

	if (loop == NULL || func == NULL) {
		return NULL;
	}

	timer = (el_timer_t *)EL_ALLOC(sizeof(el_timer_t));
	if (timer == NULL) {
		eldbg("Failed to allocate timer\n");
		return NULL;
	}

	callback = (timer_cb_t *)EL_ALLOC(sizeof(timer_cb_t));
	if (callback == NULL) {
		eldbg("Failed to allocate callback\n");
		EL_FREE(timer);
		return NULL;
	}
	callback->func = func;
	callback->cb_data = data;
	timer->data = (void *)callback;

	uv_update_time(loop);
	uv_timer_init(loop, timer);

	if (!repeat) {
		/* Add timer to be called once after timeout */
		uv_timer_start(timer, (uv_timer_cb)timeout_callback_func, timeout, 0);
	} else {
		/* Add timer to be called every timeout repeatly */
		uv_timer_start(timer, (uv_timer_cb)timeout_callback_func, timeout, timeout);
	}

	return timer;
}

el_timer_t *eventloop_add_timer(unsigned int timeout, bool repeat, timeout_callback func, void *data)
{
	el_loop_t *loop;

	loop = get_app_loop();
	if (loop == NULL) {
		eldbg("Failed to get loop\n");
		return NULL;
	}

	return (el_timer_t *)add_timer(loop, timeout, repeat, func, data);
}

int eventloop_delete_timer(el_timer_t *timer)
{
	if (timer == NULL) {
		eldbg("Invalid parameter\n");
		return EVENTLOOP_INVALID_PARAM;
	}

	/* Stop the timer */
	uv_timer_stop(timer);

	EL_FREE(timer->data);
	timer->data = NULL;
	EL_FREE(timer);
	timer = NULL;

	return OK;
}

el_timer_t *eventloop_add_timer_async(unsigned int timeout, bool repeat, timeout_callback func, void *data)
{
	int ret;
	int async_pid;
	el_loop_t *loop;
	el_timer_t *timer;

	async_pid = get_async_task();

	/* Validate that event loop task is alive. */
	if (async_pid < 0) {
		eldbg("Failed to add timer event to event loop task. \n");
		return NULL;
	}

	loop = uv_default_loop();
	if (loop == NULL) {
		eldbg("Failed to get loop\n");
		return NULL;
	}

	timer = add_timer(loop, timeout, repeat, func, data);
	if (timer == NULL) {
		eldbg("Failed to add timer\n");
		return NULL;
	}

	/* Wake up event loop task */
	if (uv_loop_alive(loop) != ASYNCLOOP_RUNNING) {
		ret = kill(async_pid, SIGEL_WAKEUP);
		if (ret != OK) {
			eldbg("Failed to send signal to wake up event loop task, err %d.\n", errno);
			eventloop_delete_timer(timer);
			return NULL;
		}
	}

	return timer;
}
