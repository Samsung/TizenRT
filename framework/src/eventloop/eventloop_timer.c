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

	if (callback->func) {
		callback->func(callback->cb_data);
	}

	if (timer->repeat == 0) {
		/* Free the timer resource allocated for timeout once */
		EL_FREE(timer->data);
		timer->data = NULL;
		EL_FREE(timer);
		timer = NULL;
		elvdbg("Cleaning timer resources Success\n");
	}
}

el_timer_t *eventloop_add_timer(unsigned int timeout, bool repeat, timeout_callback func, void *data)
{
	el_timer_t *timer = NULL;
	timer_cb_t *callback = NULL;

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

	uv_update_time(uv_default_loop());
	uv_timer_init(uv_default_loop(), timer);

	if (!repeat) {
		/* Add timer to be called once after timeout */
		uv_timer_start(timer, (uv_timer_cb)timeout_callback_func, timeout, 0);
		return NULL;
	}

	/* Add timer to be called every timeout repeatly */
	uv_timer_start(timer, (uv_timer_cb)timeout_callback_func, timeout, timeout);
	return timer;
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
