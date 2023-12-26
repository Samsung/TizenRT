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
/*****************************************************************************
 *  kernel/group/group_find.c
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
 *****************************************************************************/

/*****************************************************************************
 * Included Files
 *****************************************************************************/

#include <tinyara/config.h>

#include <sched.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>

#include "group/group.h"
#include "environ/environ.h"

#ifdef HAVE_TASK_GROUP

/*****************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

/*****************************************************************************
 * Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Data
 *****************************************************************************/

/*****************************************************************************
 * Public Data
 *****************************************************************************/

/*****************************************************************************
 * Private Functions
 *****************************************************************************/

/*****************************************************************************
 * Public Functions
 *****************************************************************************/

/*****************************************************************************
 * Name: group_findbygid
 *
 * Description:
 *   Given a group ID, find the group task structure with that ID.  IDs are
 *   used instead of pointers to group structures.  This is done because a
 *   group can disappear at any time leaving a stale pointer; an ID is cleaner
 *   because if the group disappears, this function will fail gracefully.
 *
 * Parameters:
 *   gid - The group ID to find.
 *
 * Return Value:
 *   On success, a pointer to the group task structure is returned.  This
 *   function can fail only if there is no group that corresponds to the
 *   group ID.
 *
 * Assumptions:
 *   Called during when signally tasks in a safe context.  No special
 *   precautions should be required here.  However, extra care is taken when
 *   accessing the global g_grouphead list.
 *
 *****************************************************************************/

#if defined(HAVE_GROUP_MEMBERS) || defined(CONFIG_ARCH_ADDRENV)
FAR struct task_group_s *group_findbygid(gid_t gid)
{
	FAR struct task_group_s *group;
	irqstate_t flags;

	/* Find the status structure with the matching GID  */

	flags = enter_critical_section();
	for (group = g_grouphead; group; group = group->flink) {
		if (group->tg_gid == gid) {
			leave_critical_section(flags);
			return group;
		}
	}

	leave_critical_section(flags);
	return NULL;
}
#endif

/*****************************************************************************
 * Name: group_findbypid
 *
 * Description:
 *   Given a task ID, find the group task structure with was started by that
 *   task ID.  That task's ID is retained in the group as tg_task and will
 *   be remember even if the main task thread leaves the group.
 *
 * Parameters:
 *   pid - The task ID of the main task thread.
 *
 * Return Value:
 *   On success, a pointer to the group task structure is returned.  This
 *   function can fail only if there is no group that corresponds to the
 *   task ID.
 *
 * Assumptions:
 *   Called during when signally tasks in a safe context.  No special
 *   precautions should be required here.  However, extra care is taken when
 *   accessing the global g_grouphead list.
 *
 *****************************************************************************/

#ifdef HAVE_GROUP_MEMBERS
FAR struct task_group_s *group_findbypid(pid_t pid)
{
	FAR struct task_group_s *group;
	irqstate_t flags;

	/* Find the status structure with the matching PID  */

	flags = enter_critical_section();
	for (group = g_grouphead; group; group = group->flink) {
		if (group->tg_task == pid) {
			leave_critical_section(flags);
			return group;
		}
	}

	leave_critical_section(flags);
	return NULL;
}
#endif

#endif							/* HAVE_TASK_GROUP */
