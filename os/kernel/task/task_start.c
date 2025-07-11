/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * kernel/task/task_start.c
 *
 *   Copyright (C) 2007-2010, 2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdlib.h>
#include <sched.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/sched.h>

#ifdef CONFIG_TASK_MANAGER
#include <tinyara/task_manager_drv.h>
#include <task_manager/task_manager.h>
#endif

#include "sched/sched.h"
#include "task/task.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* This is an artificial limit to detect error conditions where an argv[]
 * list is not properly terminated.
 */

#define MAX_START_ARGS 256

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

#ifdef CONFIG_TASK_MANAGER
static tm_exit_cb_t g_exit_cb;

void tm_set_exit_cb(tm_exit_cb_t cb)
{
	g_exit_cb = cb;
}
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: task_start
 *
 * Description:
 *   This function is the low level entry point into the main thread of
 *   execution of a task.  It receives initial control when the task is
 *   started and calls main entry point of the newly started task.
 *
 * Inputs:
 *   None
 *
 * Return:
 *   None
 *
 ****************************************************************************/

void task_start(void)
{
	FAR struct task_tcb_s *tcb = (FAR struct task_tcb_s *)this_task();
	int exitcode;
	int argc;

	DEBUGASSERT((tcb->cmn.flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_PTHREAD);

	/* Execute the start hook if one has been registered */

#ifdef CONFIG_SCHED_STARTHOOK
	if (tcb->starthook) {
		tcb->starthook(tcb->starthookarg);
	}
#endif

	/* Count how many non-null arguments we are passing. The first non-null
	 * argument terminates the list .
	 */

	argc = 1;
	while (tcb->argv[argc]) {
		/* Increment the number of args.  Here is a sanity check to
		 * prevent running away with an unterminated argv[] list.
		 * MAX_START_ARGS should be sufficiently large that this never
		 * happens in normal usage.
		 */

		if (++argc > MAX_START_ARGS) {
			exit(EXIT_FAILURE);
		}
	}

	/* Call the 'main' entry point passing argc and argv.  In the kernel build
	 * this has to be handled differently if we are starting a user-space task;
	 * we have to switch to user-mode before calling the task.
	 */

#if defined(CONFIG_BUILD_PROTECTED) || defined(CONFIG_BUILD_KERNEL)
	if ((tcb->cmn.flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_KERNEL) {
		up_task_start(tcb->cmn.entry.main, argc, tcb->argv);
		exitcode = EXIT_FAILURE;	/* Should not get here */
	} else
#endif
	{
		exitcode = tcb->cmn.entry.main(argc, tcb->argv);
#ifdef CONFIG_TASK_MANAGER
		if (g_exit_cb != NULL) {
			(*g_exit_cb)(tcb->cmn.pid);
		}
#endif
	}

	/* Call exit() if/when the task returns */

	exit(exitcode);
}
