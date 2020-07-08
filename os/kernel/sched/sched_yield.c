/****************************************************************************
 * kernel/sched/sched_yield.c
 *
 *   Copyright (C) 2007, 2009 Gregory Nutt. All rights reserved.
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

#include <tinyara/arch.h>

#include "sched/sched.h"

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
 * Name: sched_yield
 *
 * Description:
 *   This function forces the calling task to give up the CPU (only to other
 *   tasks at the same priority).
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   0 (OK) or -1 (ERROR) (errno is not set)
 *
 * Assumptions:
 *
 ****************************************************************************/

int sched_yield(void)
{
#ifndef CONFIG_SCHED_YIELD_OPTIMIZATION

	FAR struct tcb_s *rtcb = this_task();

	/* This equivalent to just resetting the task priority to its current value
	 * since this will cause the task to be rescheduled behind any other tasks
	 * at the same priority.
	 */

	return sched_setpriority(rtcb, rtcb->sched_priority);

#else

	struct tcb_s *rtcb = this_task();

	if (rtcb->flink == NULL) {
		sdbg("Fail to yield, there is no next tcb. Current task : %d\n", rtcb->pid);
		return ERROR;
	}

	/* Yielding the CPU resource to other tasks is possible only if other tasks
	 * priority is either equal or greater than currently running task.
	 * It's achieved by rescheduling the current task behind any other tasks at
	 * same priority
	 */

	if (rtcb->sched_priority <= rtcb->flink->sched_priority) {
		irqstate_t saved_state;

		saved_state = irqsave();

		/* Remove the TCB from the ready-to-run list */

		dq_rem((FAR dq_entry_t *)rtcb, (FAR dq_queue_t *)&g_readytorun);

		/* Add the task in the correct location in the prioritized
		 * g_readytorun task list
		 */

		sched_addprioritized(rtcb, (FAR dq_queue_t *)&g_readytorun);

		/* The current tcb was added in the middle of the ready-to-run list */

		rtcb->task_state = TSTATE_TASK_READYTORUN;

		/* we are going to do a context switch, then now it's the right
		 * time to add any pending tasks back into the ready-to-run task
		 * list now
		 */

		if (g_pendingtasks.head) {
			sched_mergepending();
		}

		/* Get new head of the list */

		rtcb = this_task();

#if CONFIG_RR_INTERVAL > 0
		rtcb->timeslice = MSEC2TICK(CONFIG_RR_INTERVAL);
#endif

		/* A context switch will occur. */

		rtcb->task_state = TSTATE_TASK_RUNNING;

		up_schedyield(rtcb);

		irqrestore(saved_state);
	}
	return OK;

#endif							/* End of CONFIG_SCHED_YIELD_OPTIMIZATION */
}
