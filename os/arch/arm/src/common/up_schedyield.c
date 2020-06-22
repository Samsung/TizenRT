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
 * Included Files
 ****************************************************************************/

#include "sched/sched.h"
#include "up_internal.h"

#ifdef CONFIG_ARMV8M_TRUSTZONE
#include <tinyara/tz_context.h>
#endif

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
		 * rtcb is the tcb of task which is yeilding the resource
		 */
		up_savestate(rtcb->xcp.regs);
#ifdef CONFIG_ARMV8M_TRUSTZONE
		/* Store the secure context and PSPLIM of OLD rtcb */
		tz_store_context(tcb->xcp.regs);
#endif

		/* Restore the exception context of the new changed rtcb from the head
		 * of the g_readytorun task list.
		 */

		rtcb = this_task();
		/* Then switch contexts.  Any necessary address environment
		 * changes will be made when the interrupt returns.
		 */

		up_restorestate(rtcb->xcp.regs);
#ifdef CONFIG_ARMV8M_TRUSTZONE
		/* Load the secure context and PSPLIM of OLD rtcb */
		tz_load_context(rtcb->xcp.regs);
#endif

	}
	/* Copy the exception context into the TCB at the (old) head of the
	 * g_readytorun Task list. if up_saveusercontext returns a non-zero
	 * value, then this is really the previously running task restarting!
	 * here, rtcb is the tcb of task which is yeilding the resource
	 */

	else if (!up_saveusercontext(rtcb->xcp.regs)) {
		/* Restore the exception context of the (new) changed rtcb from the head
		 * of the g_readytorun task list.
		 */

		rtcb = this_task();
		/* Then switch contexts */

		up_fullcontextrestore(rtcb->xcp.regs);
	}
}
