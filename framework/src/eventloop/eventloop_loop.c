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
#include <tinyara/sched.h>
#include <libtuv/uv.h>
#include <libtuv/uv__types.h>
#include <libtuv/uv__loop.h>
#include <eventloop/eventloop.h>

#include "eventloop_internal.h"

el_loop_t *g_loop_list[CONFIG_MAX_TASKS];

el_loop_t *get_app_loop(void)
{
	int index;

	/* Get a loop assigned to own task */
	index = PIDHASH(getpid());

	if (g_loop_list[index] == NULL) {
		g_loop_list[index] = (el_loop_t *)EL_ALLOC(sizeof(el_loop_t));
		if (g_loop_list[index] != NULL) {
			uv_loop_init(g_loop_list[index]);
		}
	}

	return g_loop_list[index];
}

static int release_app_loop(void)
{
	int index;

	index = PIDHASH(getpid());

	if (g_loop_list[index] != NULL) {
		if (uv_loop_close(g_loop_list[index]) < 0) {
			eldbg("Failed to close loop\n");
			return EVENTLOOP_BUSY;
		}
		EL_FREE(g_loop_list[index]);
		g_loop_list[index] = NULL;
	}

	return OK;
}

/* It runs until there is no event to process or eventloop_loop_stop is called. */
int eventloop_loop_run(void)
{
	int ret;
	el_loop_t *loop;

	loop = get_app_loop();
	if (loop == NULL) {
		return EVENTLOOP_LOOP_FAIL;
	}

	/* It is returned when event processing is done or stop request is called. */
	if (uv_run(loop, UV_RUN_DEFAULT) != 0) {
		/* there are still events to process */
		return EVENTLOOP_NOT_FINISHED;
	}

	ret = release_app_loop();
	if (ret != OK) {
		eldbg("Running loop is done, but failed to close loop\n");
	}

	return ret;
}

/* A function for closing each handle in loop called by iteration in uv_walk. */
static void eventloop_close_handle(uv_handle_t *handle, void *data)
{
	if (handle == NULL || uv__is_closing(handle)) {
		return;
	}

	switch (handle->type) {
	case UV_TIMER:
		uv_close(handle, (uv_close_cb)eventloop_unregister_timer);
		break;
	case UV_SIGNAL:
		uv_close(handle, (uv_close_cb)eventloop_unregister_event_cb);
		break;
	case UV_ASYNC:
		uv_close(handle, (uv_close_cb)eventloop_unregister_thread_safe_cb);
		break;
	default:
		break;
	}
}

/* It is called from callback function registered in loop.
 * So uv_run in eventloop_loop_run will exit and loop is released after executing this function.
 */
int eventloop_loop_stop(void)
{
	int index;

	index = PIDHASH(getpid());

	/* If app loop is already finished or not exist, EVENTLOOP_LOOP_FAIL is returned. */
	if (g_loop_list[index] == NULL) {
		return EVENTLOOP_LOOP_FAIL;
	}

	/* Stop loop and close all handles in loop */
	uv_stop(g_loop_list[index]);
	uv_walk(g_loop_list[index], (uv_walk_cb)eventloop_close_handle, NULL);

	return OK;
}
