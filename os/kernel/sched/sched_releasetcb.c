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
 * kernel/sched/sched_releasetcb.c
 *
 *   Copyright (C) 2007, 2009, 2012-2014 Gregory Nutt. All rights reserved.
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

#include <sys/types.h>
#include <sched.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/sched.h>

#include "sched/sched.h"
#include "group/group.h"
#include "timer/timer.h"
#ifdef CONFIG_BINARY_MANAGER
#include "binary_manager/binary_manager.h"
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO
#include <tinyara/mm/mm.h>
#endif

/************************************************************************
 * Name:  sched_releasepid
 *
 * Description:  When a task is destroyed, this function must
 * be called to make its process ID available for re-use.
 ************************************************************************/

static void sched_releasepid(pid_t pid)
{
	irqstate_t flags = enter_critical_section();
	int hash_ndx = PIDHASH(pid);

	/* Make any pid associated with this hash available.  Note:
	 * no special precautions need be taken here because the
	 * following action is atomic
	 */
	g_pidhash[hash_ndx].tcb = NULL;
	g_pidhash[hash_ndx].pid = INVALID_PROCESS_ID;

#ifdef CONFIG_SCHED_CPULOAD
	/* Decrement the total CPU load count held by this thread from total
	 * for all threads and reset the load count on this defunct thread
	 */
	sched_clear_cpuload(pid);
#endif
	/* Decrement the alive task count as task is exiting */
	g_alive_taskcount--;
	leave_critical_section(flags);
}

/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name: sched_releasetcb
 *
 * Description:
 *   Free all resources contained in a TCB
 *
 * Parameters:
 *   tcb - The TCB to be released
 *   ttype - The type of the TCB to be released
 *
 *   This thread type is normally available in the flags field of the TCB,
 *   however, there are certain error recovery contexts where the TCB my
 *   not be fully initialized when sched_releasetcb is called.
 *
 * Return Value:
 *   OK on success; ERROR on failure
 *
 * Assumptions:
 *   Interrupts are disabled.
 *
 ************************************************************************/
int sched_releasetcb(FAR struct tcb_s *tcb, uint8_t ttype)
{
	int ret = OK;

	if (tcb) {
		/* Release the some of task's resources if PID was assigned.
		 * PID zero is reserved for the IDLE task.  The TCB of the IDLE
		 * task is never release so a value of zero simply means that
		 * the process ID was never allocated to this TCB.
		 */
		if (tcb->pid) {
#ifndef CONFIG_DISABLE_POSIX_TIMERS
			/* Release any timers that the task might hold.  We do this
			 * before release the PID because it may still be trying to
			 * deliver signals (although interrupts are should be
			 * disabled here).
			 */

#ifdef CONFIG_HAVE_WEAKFUNCTIONS
			if (timer_deleteall != NULL)
#endif
			{
				timer_deleteall(tcb->pid);
			}
#endif

			sched_releasepid(tcb->pid);
		}

		/* Delete the thread's stack if one has been allocated */

		if (tcb->stack_alloc_ptr) {
#ifdef CONFIG_BUILD_KERNEL
			/* If the exiting thread is not a kernel thread, then it has an
			 * address environment.  Don't bother to release the stack memory
			 * in this case... There is no point since the memory lies in the
			 * user memory region that will be destroyed anyway (and the
			 * address environment has probably already been destroyed at
			 * this point.. so we would crash if we even tried it).  But if
			 * this is a privileged group, when we still have to release the
			 * memory using the kernel allocator.
			 */

			if ((tcb->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_KERNEL)
#endif
			{
				up_release_stack(tcb, ttype);
			}
		}
#ifdef CONFIG_PIC
		/* Delete the task's allocated DSpace region (external modules only) */

		if (tcb->dspace) {
			if (tcb->dspace->crefs <= 1) {
				sched_kfree(tcb->dspace);
			} else {
				tcb->dspace->crefs--;
			}
		}
#endif

#if defined(CONFIG_ARCH_ADDRENV) && defined(CONFIG_ARCH_KERNEL_STACK)
		/* Release the kernel stack */

		(void)up_addrenv_kstackfree(tcb);
#endif

#ifdef CONFIG_ARCH_ADDRENV
		/* Release this thread's reference to the address environment */

		ret = up_addrenv_detach(tcb->group, tcb);
#endif

#ifdef HAVE_TASK_GROUP
		/* Leave the group (if we did not already leave in task_exithook.c) */

		group_leave(tcb);
#endif

		/* And, finally, release the TCB itself */

		sched_kfree(tcb);
	}

	return ret;
}
