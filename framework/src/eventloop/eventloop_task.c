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
#include <sched.h>
#include <signal.h>
#include <libtuv/uv.h>
#include <eventloop/eventloop.h>
#include <tinyara/eventloop.h>

#include "eventloop_internal.h"

#define EVENTLOOP_STACK_SIZE 4096
#define EVENTLOOP_PRIORITY 201

static int el_task_pid = -1;

int get_async_task(void)
{
	return el_task_pid;
}

int eventloop_task(int argc, char *argv[])
{
	el_loop_t *loop;
	sigset_t sigset;

	loop = uv_default_loop();
	if (loop == NULL) {
		eldbg("Failed to get default loop\n");
		return -1;
	}

	/* Unset all signals except for SIGEL_WAKEUP and SIGKILL */
	sigfillset(&sigset);
	sigdelset(&sigset, SIGEL_WAKEUP);
	sigdelset(&sigset, SIGKILL);
	(void)sigprocmask(SIG_SETMASK, &sigset, NULL);

	while (1) {
		/* Waiting the SIGEL_WAKEUP which is sent by other task for event handling. */
		(void)sigsuspend(&sigset);

		/* Run loop for processing added events. */
		if (uv_run(loop, UV_RUN_DEFAULT) != 0) {
			break;
		}
	}
	el_task_pid = -1;

	return 0;
}

int eventloop_task_start(void)
{
	int pid;

	elvdbg("Starting eventloop default loop\n");

	pid = task_create("eventloop", EVENTLOOP_PRIORITY, EVENTLOOP_STACK_SIZE, eventloop_task, (FAR char *const *)NULL);
	if (pid < 0) {
		return -errno;
	}
	el_task_pid = pid;
	elvdbg("Event loop task is started! pid = %d\n", el_task_pid);

	return pid;
}
