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
 *  kernel/group/group_addrenv.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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

#include <debug.h>

#include <tinyara/sched.h>
#include <tinyara/common_logs/common_logs.h>

#include "sched/sched.h"
#include "group/group.h"

#ifdef CONFIG_ARCH_ADDRENV

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/
/* This variable holds the group ID of the current task group.  This ID is
 * zero if the current task is a kernel thread that has no address
 * environment (other than the kernel context).
 *
 * This must only be accessed with interrupts disabled.
 */

gid_t g_gid_current;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: group_addrenv
 *
 * Description:
 *   Instantiate the group address environment for the current thread at the
 *   the head of the ready to run list.
 *
 *   This function is called from platform-specific code after any context
 *   switch (i.e., after any change in the thread at the head of the ready-to-
 *   run list).  This function will change the address environment if the
 *   new thread is part of a different task group.
 *
 * Parameters:
 *   tcb - The TCB of thread that needs an address environment.  This should
 *         be the TCB at the head of the ready-to-run list, but that is not
 *         enough.
 *
 * Return Value:
 *   Zero (OK) is returned on success.  A negated errno value is returned on
 *   any failure.
 *
 * Assumptions:
 *   This function should only be called within critical OS sections with
 *   interrupts disabled.  Interrupts are disabled internally just to be
 *   certain, however.
 *
 ****************************************************************************/

int group_addrenv(FAR struct tcb_s *tcb)
{
	FAR struct task_group_s *group;
	FAR struct task_group_s *oldgroup;
	irqstate_t flags;
	gid_t gid;
	int ret;

	/* NULL for the tcb means to use the TCB of the task at the head of the
	 * ready to run list.
	 */

	if (!tcb) {
		tcb = this_task();
	}

	DEBUGASSERT(tcb && tcb->group);
	group = tcb->group;

	/* Does the group have an address environment? */

	if ((group->tg_flags & GROUP_FLAG_ADDRENV) == 0) {
		/* No... just return perhaps leaving a different address environment
		 * intact.
		 */

		return OK;
	}

	/* Get the ID of the group that needs the address environment */

	gid = group->tg_gid;
	DEBUGASSERT(gid != 0);

	/* Are we going to change address environments? */

	flags = enter_critical_section();
	if (gid != g_gid_current) {
		/* Yes.. Is there a current address environment in place? */

		if (g_gid_current != 0) {
			/* Find the old group with this ID. */

			oldgroup = group_findbygid(g_gid_current);
			DEBUGASSERT(oldgroup && (oldgroup->tg_flags & GROUP_FLAG_ADDRENV) != 0);

			if (oldgroup) {
				/* We need to flush the D-Cache and Invalidate the I-Cache for
				 * the group whose environment is disappearing.
				 */

				up_addrenv_coherent(&oldgroup->tg_addrenv);
			}
		}

		/* Instantiate the new address environment (removing the old
		 * environment in the process).  For the case of kernel threads,
		 * the old mappings will be removed and no new mappings will be
		 * instantiated.
		 */

		ret = up_addrenv_select(&group->tg_addrenv, NULL);
		if (ret < 0) {
			bdbg("%s %d\n", clog_message_str[CMN_LOG_FAILED_OP], ret);
		}

		/* Save the new, current group */

		g_gid_current = gid;
	}

	leave_critical_section(flags);
	return OK;
}

#endif							/* CONFIG_ARCH_ADDRENV */
