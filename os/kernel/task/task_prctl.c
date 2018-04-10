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
/************************************************************************
 * kernel/task/task_prctl.c
 *
 *   Copyright (C) 2012-2013 Gregory Nutt. All rights reserved.
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
 ************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>

#include <sys/prctl.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/sched.h>
#include <tinyara/ttrace.h>

#include "sched/sched.h"
#include "task/task.h"

/************************************************************************
 * Private Functions
 ************************************************************************/

/************************************************************************
 * Public Functions
 ************************************************************************/

/****************************************************************************
 * Name: prctl
 *
 * Description:
 *   prctl() is called with a first argument describing what to do (with
 *   values PR_* defined above) and with additional arguments depending on
 *   the specific command.
 *
 * Returned Value:
 *   The returned value may depend on the specific commnand.  For PR_SET_NAME
 *   and PR_GET_NAME, the returned value of 0 indicates successful operation.
 *   On any failure, -1 is retruend and the errno value is set appropriately.
 *
 *     EINVAL The value of 'option' is not recognized.
 *     EFAULT optional arg1 is not a valid address.
 *     ESRCH  No task/thread can be found corresponding to that specified
 *       by optional arg1.
 *
 ****************************************************************************/

int prctl(int option, ...)
{
	va_list ap;
	int err;

	trace_begin(TTRACE_TAG_TASK, "prctl");
	va_start(ap, option);
	switch (option) {
	case PR_SET_NAME:
	case PR_GET_NAME:
#if CONFIG_TASK_NAME_SIZE > 0
	{
		/* Get the prctl arguments */

		FAR char *name = va_arg(ap, FAR char *);
		int pid = va_arg(ap, int);
		FAR struct tcb_s *tcb;

		/* Get the TCB associated with the PID (handling the special case of
		 * pid==0 meaning "this thread")
		 */

		if (!pid) {
			tcb = this_task();
		} else {
			tcb = sched_gettcb(pid);
		}

		/* An invalid pid will be indicated by a NULL TCB returned from
		 * sched_gettcb()
		 */

		if (!tcb) {
			sdbg("Pid does not correspond to a task: %d\n", pid);
			err = ESRCH;
			goto errout;
		}

		/* A pointer to the task name storage must also be provided */

		if (!name) {
			sdbg("No name provide\n");
			err = EFAULT;
			goto errout;
		}

		/* Now get or set the task name */

		if (option == PR_SET_NAME) {
			/* Ensure that tcb->name will be null-terminated, truncating if necessary */

			strncpy(tcb->name, name, CONFIG_TASK_NAME_SIZE);
			tcb->name[CONFIG_TASK_NAME_SIZE] = '\0';
#ifdef CONFIG_HEAPINFO_USER_GROUP
			heapinfo_check_group_list(tcb->pid, tcb->name);
#endif
		} else {
			/* The returned value will be null-terminated, truncating if necessary */

			strncpy(name, tcb->name, CONFIG_TASK_NAME_SIZE - 1);
			name[CONFIG_TASK_NAME_SIZE - 1] = '\0';
		}
	}
	break;
#else
	sdbg("Option not enabled: %d\n", option);
	err = ENOSYS;
	goto errout;
#endif

	default:
		sdbg("Unrecognized option: %d\n", option);
		err = EINVAL;
		goto errout;
	}

	/* Not reachable unless CONFIG_TASK_NAME_SIZE is > 0.  NOTE: This might
	 * change if additional commands are supported.
	 */

#if CONFIG_TASK_NAME_SIZE > 0
	va_end(ap);
	trace_end(TTRACE_TAG_TASK);
	return OK;
#endif

errout:
	va_end(ap);
	set_errno(err);
	trace_end(TTRACE_TAG_TASK);
	return ERROR;
}
