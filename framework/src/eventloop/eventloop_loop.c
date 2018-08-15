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
#include <libtuv/uv.h>
#include <eventloop/eventloop.h>

#include "eventloop_internal.h"

el_loop_t *g_loop_list[CONFIG_MAX_TASKS];

#define MAX_PID_MASK   (CONFIG_MAX_TASKS - 1)
#define PID_HASH(pid)  ((pid) & MAX_PID_MASK)

el_loop_t *get_app_loop(void)
{
	int index;

	/* Get a loop assigned to own task */
	index = PID_HASH(getpid());
	if (index < 0 || index >= CONFIG_MAX_TASKS) {
		return NULL;
	}

	if (g_loop_list[index] == NULL) {
		g_loop_list[index] = (el_loop_t *)EL_ALLOC(sizeof(el_loop_t));
		if (g_loop_list[index] != NULL) {
			uv_loop_init(g_loop_list[index]);
		}
	}

	return g_loop_list[index];
}

static void release_app_loop(void)
{
	int index;

	index = PID_HASH(getpid());
	if (index < 0 || index >= CONFIG_MAX_TASKS) {
		return;
	}

	if (g_loop_list[index] != NULL) {
		EL_FREE(g_loop_list[index]);
	}

	g_loop_list[index] = NULL;
}

/* It runs until there is no event to process or eventloop_loop_stop is called. */
int eventloop_loop_run(void)
{
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
	release_app_loop();

	return OK;
}

int eventloop_loop_stop(void)
{
	el_loop_t *loop;

	loop = get_app_loop();
	if (loop == NULL) {
		return EVENTLOOP_LOOP_FAIL;
	}

	if (uv_loop_close(loop) < 0) {
		eldbg("Failed to cloase loop\n");
		return EVENTLOOP_BUSY;
	}
	release_app_loop();

	return OK;
}
