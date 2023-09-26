/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#include <sys/types.h>
#include <debug.h>
#include <queue.h>

#include <tinyara/sched.h>
#if CONFIG_RR_INTERVAL > 0
#include <tinyara/clock.h>
#endif
#ifdef CONFIG_TASK_SCHED_HISTORY
#include <tinyara/debug/sysdbg.h>
#endif

#include "sched/sched.h"
#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_schedyield
 *
 * Description:
 *   Called when current task is willing to
 *   yield cpu resource to other ready-to-run task.
 *   context switch would occur in below case:
 *
 *   1) The priority of the currently running task is either equal or more than other ready to run task
 *
 ****************************************************************************/

void up_schedyield(void)
{
	struct tcb_s *rtcb = this_task();
	struct tcb_s *ntcb = rtcb->flink;

	if (ntcb == NULL) {
		sdbg("Fail to yield, there is no next tcb. Current task : %d\n", rtcb->pid);
		return;
	}

	/* Yielding the CPU resource to other tasks is possible only if other tasks
	 * priority is either equal or greater than currently running task.
	 * It's achieved by rescheduling the current task behind any other tasks at
	 * same priority
	 */

	if (rtcb->sched_priority <= ntcb->sched_priority) {

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

		ntcb = this_task();

#if CONFIG_RR_INTERVAL > 0
		ntcb->timeslice = MSEC2TICK(CONFIG_RR_INTERVAL);
#endif

		/* A context switch will occur. */

		ntcb->task_state = TSTATE_TASK_RUNNING;

		/* Are we in an interrupt handler? */

		if (CURRENT_REGS) {
			/* Yes, then we have to do things differently.
			 * Just copy the current_regs into the OLD rtcb.
			 */

               arm_savestate(rtcb->xcp.regs);

			/* Restore ntcb data for context switching */

			up_restoretask(ntcb);

			/* Then switch contexts */

              //sched_resume_scheduler(rtcb);
			arm_restorestate(ntcb->xcp.regs);
		}
		/* No, then we will need to perform the user context switch */

		else {
#ifdef CONFIG_TASK_SCHED_HISTORY
			/* Save the task name which will be scheduled */

			save_task_scheduling_status(ntcb);
#endif
			arm_switchcontext(rtcb->xcp.regs, ntcb->xcp.regs);

			/* up_switchcontext forces a context switch to the task at the
			 * head of the ready-to-run list.  It does not 'return' in the
			 * normal sense.  When it does return, it is because the blocked
			 * task is again ready to run and has execution priority.
			 */
		}
	}
}
