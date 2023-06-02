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
 * kernel/task/task_restart.c
 *
 *   Copyright (C) 2007, 2009, 2012-2013 Gregory Nutt. All rights reserved.
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

#include <sys/types.h>
#include <sched.h>
#if defined(CONFIG_SCHED_ATEXIT) || defined(CONFIG_SCHED_ONEXIT)
#include <stdlib.h>
#endif
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/ttrace.h>

#include "sched/sched.h"
#include "group/group.h"
#include "signal/signal.h"
#include "task/task.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

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

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: task_restart
 *
 * Description:
 *   This function "restarts" a task.  The task is first terminated and then
 *   reinitialized with same ID, priority, original entry point, stack size,
 *   and parameters it had when it was first started. This function supports 
 *   to register a function through atexit or on_exit to run at process termination.
 *
 * Inputs:
 *   pid - The task ID of the task to delete.  An ID of zero signifies the
 *         calling task.
 *
 * Return Value:
 *   OK on sucess; ERROR on failure.
 *
 *   This function can fail if:
 *   (1) A pid of zero or the pid of the calling task is provided
 *      (functionality not implemented)
 *   (2) The pid is not associated with any task known to the system.
 *
 ****************************************************************************/

int task_restart(pid_t pid)
{
	FAR struct tcb_s *rtcb;
	FAR struct task_tcb_s *tcb;
	irqstate_t state;
#ifdef CONFIG_SMP
	int cpu;
	int ret = 0;
#endif

	trace_begin(TTRACE_TAG_TASK, "task_restart");

	/* Make sure this task does not become ready-to-run while
	 * we are futzing with its TCB
	 */

//PORTNOTE: in case of SMP, enter_critical_section is used instead of sched_lock
//Nuttx has this locking inside group_kill_children function but TizenRT does not do this there
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
	//PORTNOTE: is it not necessary to unlock the scheduler in case of
	//exiting suddenly due to error?
#endif

	/* Check if the task to restart is the calling task */

	rtcb = this_task();
	if ((pid == 0) || (pid == rtcb->pid)) {
		/* Not implemented */

		set_errno(ENOSYS);
		trace_end(TTRACE_TAG_TASK);
		return ERROR;
	}

	/* We are restarting some other task than ourselves */

	else {
		/* Find for the TCB associated with matching pid  */

		tcb = (FAR struct task_tcb_s *)sched_gettcb(pid);
#ifndef CONFIG_DISABLE_PTHREAD
		if (!tcb || (tcb->cmn.flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD)
#else
		if (!tcb)
#endif
		{
			/* There is no TCB with this pid or, if there is, it is not a
			 * task.
			 */

			set_errno(ESRCH);
			trace_end(TTRACE_TAG_TASK);
			return ERROR;
		}

#if defined(CONFIG_SCHED_ATEXIT) || defined(CONFIG_SCHED_ONEXIT)
		/* If exit function(s) were registered, call them now before we do any un-
		 * initialization.
		 */

		task_atexit((FAR struct tcb_s *)tcb);

		/* Call any registered on_exit function(s) */

		task_onexit((FAR struct tcb_s *)tcb, EXIT_SUCCESS);
#endif

#ifdef CONFIG_SMP
		/* If the task is running on another CPU, then pause that CPU.
		 * We can then manipulate the TCB of the restarted task and when
		 * we resume that CPU, the restart can take effect.
		 */

		cpu = sched_pause_cpu(&tcb->cmn);
#endif

		/* Try to recover from any bad states */

		task_recover((FAR struct tcb_s *)tcb);

		/* Kill any children of this thread */

#ifdef HAVE_GROUP_MEMBERS
		(void)group_killchildren(tcb);
#endif

		/* Remove the TCB from whatever list it is in.  At this point, the
		 * TCB should no longer be accessible to the system
		 */

		state = enter_critical_section();
#ifdef CONFIG_SMP
		FAR dq_queue_t *tasklist = TLIST_HEAD(tcb->cmn.task_state, tcb->cmn.cpu);
		dq_rem((FAR dq_entry_t *)tcb, tasklist);
#else
		dq_rem((FAR dq_entry_t *)tcb, (dq_queue_t *)g_tasklisttable[tcb->cmn.task_state].list);
#endif
		tcb->cmn.task_state = TSTATE_TASK_INVALID;
		leave_critical_section(state);

#ifndef CONFIG_DISABLE_SIGNALS
		/* Deallocate anything left in the TCB's queues */

		sig_cleanup((FAR struct tcb_s *)tcb);	/* Deallocate Signal lists */
		tcb->cmn.sigprocmask = NULL_SIGNAL_SET;
#endif

		/* Reset the current task priority  */

		tcb->cmn.sched_priority = tcb->init_priority;

		/* Reset the base task priority and the number of pending reprioritizations */

#ifdef CONFIG_SMP
//PORTNOTE: The counterpart of this line was not present in TizenRT at all
//Has been added while porting SMP only
		tcb->cmn.irqcount = 0;
#endif

#ifdef CONFIG_PRIORITY_INHERITANCE
		tcb->cmn.base_priority = tcb->init_priority;
#if CONFIG_SEM_NNESTPRIO > 0
		tcb->cmn.npend_reprio = 0;
#endif
#endif

		/* Re-initialize the processor-specific portion of the TCB
		 * This will reset the entry point and the start-up parameters
		 */

		up_initial_state((FAR struct tcb_s *)tcb);

		/* Add the task to the inactive task list */

		dq_addfirst((FAR dq_entry_t *)tcb, (dq_queue_t *)&g_inactivetasks);
		tcb->cmn.task_state = TSTATE_TASK_INACTIVE;

#ifdef CONFIG_SMP
		/* Resume the paused CPU (if any) */
		if (cpu >= 0) {
			ret = up_cpu_resume(cpu);
			if (ret < 0) {
				//PORTNOTE: not needed to unlock scheduler?
				//In another case above it is not unlocked
				return ret;
			}
		}
#endif

		/* Activate the task */

		(void)task_activate((FAR struct tcb_s *)tcb);
	}

#ifdef CONFIG_SMP
	leave_critical_section(flags);
#else
	sched_unlock();
#endif
	trace_end(TTRACE_TAG_TASK);
	return OK;
}
