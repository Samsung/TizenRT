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
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <eventloop/eventloop.h>

#include "eventloop_internal.h"

/* Clean all registered handles excluding EL_MODE_WAIT_DONE */
static void eventloop_clean_app_handles(void)
{
	el_handle_t *handle;
	el_app_handle_t *app_handle;

	app_handle = (el_app_handle_t *)eventloop_get_app_handle();

	handle = (el_handle_t *)sq_peek(&app_handle->jobs);
	while (handle) {
		if (handle->mode != EL_MODE_WAIT_DONE) {
			switch (handle->type) {
			case EL_HANDLE_TYPE_TIMER:
				eventloop_clean_timer_handle(handle);
				break;
			case EL_HANDLE_TYPE_EVENT:
				eventloop_clean_event_handle(handle);
				break;
			default:
				eldbg("Invalid type.\n");
			}
		}
		handle = (el_handle_t *)sq_next(handle);
	}

	/* Clean all registered events */
	eventloop_clean_app_timers();
	eventloop_clean_app_event_handles();
}

static void eventloop_deinit_app_handles(void)
{
	el_app_handle_t *app_handle;

	app_handle = (el_app_handle_t *)eventloop_get_app_handle();
	memset(app_handle, 0, sizeof(el_app_handle_t));
}

void eventloop_wait_process_event(void)
{
	bool do_quit;
	sigset_t sigset;
	el_handle_t *handle;
	el_app_handle_t *app_handle;

	sigfillset(&sigset);
	sigdelset(&sigset, SIGEL_WAKEUP);
	(void)sigprocmask(SIG_SETMASK, &sigset, NULL);

	app_handle = (el_app_handle_t *)eventloop_get_app_handle();

	elvdbg("WAIT MODE START\n");
	do_quit = false;
	while (!do_quit) {
		EL_APP_STATE(app_handle) = EL_STATE_WAIT_WAKEUP;
		/* Wait for event incoming */
		(void)sigsuspend(&sigset);
		EL_APP_STATE(app_handle) = EL_STATE_NORMAL;
		/* Process events */
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
			if (EL_APP_JOB_COUNT(app_handle) <= 0) {
				do_quit = true;
				break;
			}
		}
	}
}

/* A function for cleaning used resources in eventloop when task/pthread exits */
void eventloop_exit_process(int pid)
{
	/* Clean the used resources */
	eventloop_clean_app_handles();

	/* Handle events that should be processed */
	if (eventloop_get_app_handle_cnt() > 0) {
		eventloop_wait_process_event();
	}

	/* Deinit values of app handle */
	eventloop_deinit_app_handles();
}
