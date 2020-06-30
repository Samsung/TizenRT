/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
 * Inputs:
 *   tcb: The TCB of the task that has been reprioritized
 *
 ****************************************************************************/

void up_schedyield(struct tcb_s *rtcb)
{
	if (current_regs) {
		/* Yes, then we have to do things differently.
		 * Just copy the current_regs into the OLD rtcb.
		 */

		up_savestate(rtcb->xcp.regs);

		/* Restore the exception context of the rtcb at the (new) head
		 * of the g_readytorun task list.
		 */

		rtcb = this_task();
		/* Then switch contexts */

		up_restorestate(rtcb->xcp.regs);
	}
	/* No, then we will need to perform the user context switch */

	else {
		/* Switch context to the context of the task at the head of the
		 * ready to run list.
		 */

		struct tcb_s *nexttcb = this_task();
		up_switchcontext(rtcb->xcp.regs, nexttcb->xcp.regs);

		/* up_switchcontext forces a context switch to the task at the
		 * head of the ready-to-run list.  It does not 'return' in the
		 * normal sense.  When it does return, it is because the blocked
		 * task is again ready to run and has execution priority.
		 */
	}
}
