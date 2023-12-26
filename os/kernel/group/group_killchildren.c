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
 *  kernel/group/group_killchildren.c
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
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

#include <sched.h>

#include "group/group.h"

#ifdef HAVE_GROUP_MEMBERS

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/*****************************************************************************
 * Name: group_killchildren_handler
 *
 * Description:
 *   Callback from group_foreachchild that handles one member of the group.
 *
 * Parameters:
 *   pid - The ID of the group member that may be signalled.
 *   arg - The PID of the thread to be retained.
 *
 * Return Value:
 *   0 (OK) on success; a negated errno value on failure.
 *
 *****************************************************************************/

static int group_killchildren_handler(pid_t pid, FAR void *arg)
{
	int ret = OK;

	/* Is this the pthread that we are looking for? */

	if (pid != (pid_t)((uintptr_t)arg)) {
		/* Yes.. cancel it */

		ret = pthread_cancel(pid);
	}

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: group_killchildren
 *
 * Description:
 *   Delete all children of a task except for the specified task.  This is
 *   used by the task restart logic.  When the main task is restarted,
 *   all of its child pthreads must be terminated.
 *
 * Parameters:
 *   tcb - TCB of the task to be retained.
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

int group_killchildren(FAR struct task_tcb_s *tcb)
{
	int ret;

#ifdef CONFIG_SMP
	/* NOTE: sched_lock() is not enough for SMP
	 * because tcb->group will be accessed from the child tasks
	 */

	irqstate_t flags = enter_critical_section();
#else
	/* Lock the scheduler so that there this thread will not lose priority
	 * until all of its children are suspended.
	 */

	sched_lock();
#endif

	ret = group_foreachchild(tcb->cmn.group, group_killchildren_handler, (FAR void *)((uintptr_t)tcb->cmn.pid));

#ifdef CONFIG_SMP
	leave_critical_section(flags);
#else
	sched_unlock();
#endif
	return ret;

}

#endif							/* HAVE_GROUP_MEMBERS */
