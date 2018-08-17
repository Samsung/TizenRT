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
 * kernel/task/task_create.c
 *
 *   Copyright (C) 2007-2010, 2013-2014 Gregory Nutt. All rights reserved.
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
#include <errno.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/kmalloc.h>
#include <tinyara/kthread.h>
#include <tinyara/ttrace.h>
#ifdef CONFIG_DEBUG_MM_HEAPINFO
#include <tinyara/mm/mm.h>
#ifdef CONFIG_HEAPINFO_USER_GROUP
#include <tinyara/sched.h>
#include <string.h>
#endif
#endif

#include "sched/sched.h"
#include "group/group.h"
#include "task/task.h"

/****************************************************************************
 * Preprocessor Definitions
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
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: thread_create
 *
 * Description:
 *   This function creates and activates a new thread of the specified type
 *   with a specified priority and returns its system-assigned ID.  It is the
 *   internal, common implementation of task_create() and kernel_thread().
 *   See comments with task_create() for further information.
 *
 * Input Parameters:
 *   name       - Name of the new task
 *   ttype      - Type of the new task
 *   priority   - Priority of the new task
 *   stack_size - size (in bytes) of the stack needed
 *   entry      - Entry point of a new task
 *   arg        - A pointer to an array of input parameters. The array
 *                should be terminated with a NULL argv[] value. If no
 *                parameters are required, argv may be NULL.
 *
 * Return Value:
 *   Returns the non-zero process ID of the new task on success
 *   ERROR on failure. The errno will be set to indicate the nature of the error.
 *
 *   This function can fail for three reasons.
 *   1) If the memory is insufficient, errno = ENOMEM
 *   2) If requested priority is beyond the allowed range, errno = EINVAL
 *   3) If it is unable to assign a new, unique task ID to the TCB. errno = EBUSY
 *
 ****************************************************************************/

static int thread_create(FAR const char *name, uint8_t ttype, int priority, int stack_size, main_t entry, FAR char *const argv[])
{
	FAR struct task_tcb_s *tcb;
	pid_t pid;
	int errcode;
	int ret;

	trace_begin(TTRACE_TAG_TASK, "thread_create");

	/* Check whether we are allowed to create new task ? */
	if (g_alive_taskcount == CONFIG_MAX_TASKS) {
		sdbg("ERROR: CONFIG_MAX_TASKS(%d) count reached\n", CONFIG_MAX_TASKS);
		errcode = EBUSY;
		goto errout;
	}

	/* Allocate a TCB for the new task. */

	tcb = (FAR struct task_tcb_s *)kmm_zalloc(sizeof(struct task_tcb_s));
	if (!tcb) {
		sdbg("ERROR: Failed to allocate TCB\n");
		errcode = ENOMEM;
		goto errout;
	}

	/* Allocate a new task group with privileges appropriate for the parent
	 * thread type.
	 */

#ifdef HAVE_TASK_GROUP
	ret = group_allocate(tcb, ttype);
	if (ret < 0) {
		errcode = -ret;
		goto errout_with_tcb;
	}
#endif

	/* Associate file descriptors with the new task */

#if CONFIG_NFILE_DESCRIPTORS > 0 || CONFIG_NSOCKET_DESCRIPTORS > 0
	ret = group_setuptaskfiles(tcb);
	if (ret < OK) {
		errcode = -ret;
		goto errout_with_tcb;
	}
#endif

	/* Allocate the stack for the TCB */

	ret = up_create_stack((FAR struct tcb_s *)tcb, stack_size, ttype);
	if (ret < OK) {
		errcode = -ret;
		goto errout_with_tcb;
	}

	/* Initialize the task control block */

	ret = task_schedsetup(tcb, priority, task_start, entry, ttype);
	if (ret < OK) {
		errcode = get_errno();
		goto errout_with_tcb;
	}
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	/* Update the pid information in stack node */
	struct mm_allocnode_s *node;

	node = (struct mm_allocnode_s *)(tcb->cmn.stack_alloc_ptr - SIZEOF_MM_ALLOCNODE);
	node->pid = (-1) * (tcb->cmn.pid);
#endif

	/* Setup to pass parameters to the new task */

	(void)task_argsetup(tcb, name, argv);

	/* Now we have enough in place that we can join the group */

#ifdef HAVE_TASK_GROUP
	ret = group_initialize(tcb);
	if (ret < 0) {
		errcode = -ret;
		goto errout_with_tcb;
	}
#endif

	/* Get the assigned pid before we start the task */

	pid = (int)tcb->cmn.pid;

#ifdef CONFIG_HEAPINFO_USER_GROUP
	heapinfo_check_group_list(pid, tcb->cmn.name);
#endif

	/* Activate the task */

	(void)task_activate((FAR struct tcb_s *)tcb);

	trace_end(TTRACE_TAG_TASK);
	return pid;

errout_with_tcb:
	sched_releasetcb((FAR struct tcb_s *)tcb, ttype);

errout:
	set_errno(errcode);
	trace_end(TTRACE_TAG_TASK);
	return ERROR;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: task_create
 *
 * Description:
 *   This function creates and activates a new task with a specified
 *   priority and returns its system-assigned ID.
 *
 *   The entry address entry is the address of the "main" function of the
 *   task.  This function will be called once the C environment has been
 *   set up.  The specified function will be called with four arguments.
 *   Should the specified routine return, a call to exit() will
 *   automatically be made.
 *
 *   Note that four (and only four) arguments must be passed for the spawned
 *   functions.
 *
 * Input Parameters:
 *   name       - Name of the new task
 *   priority   - Priority of the new task
 *   stack_size - size (in bytes) of the stack needed
 *   entry      - Entry point of a new task
 *   arg        - A pointer to an array of input parameters. The array
 *                should be terminated with a NULL argv[] value. If no
 *                parameters are required, argv may be NULL.
 *
 * Return Value:
 *   Returns the non-zero process ID of the new task on success
 *   ERROR on failure. The errno will be set to indicate the nature of the error.
 *
 *   This function can fail for three reasons.
 *   1) If the memory is insufficient, errno = ENOMEM
 *   2) If requested priority is beyond the allowed range, errno = EINVAL
 *   3) If it is unable to assign a new, unique task ID to the TCB. errno = EBUSY
 *
 ****************************************************************************/

#ifndef CONFIG_BUILD_KERNEL
int task_create(FAR const char *name, int priority, int stack_size, main_t entry, FAR char *const argv[])
{
	return thread_create(name, TCB_FLAG_TTYPE_TASK, priority, stack_size, entry, argv);
}
#endif

/****************************************************************************
 * Name: kernel_thread
 *
 * Description:
 *   This function creates and activates a kernel thread task with kernel-
 *   mode privileges.  It is identical to task_create() except that it
 *   configures the newly started thread to run in kernel model.
 *
 * Input Parameters:
 *   (same as task_create())
 *
 * Return Value:
 *   (same as task_create())
 *
 ****************************************************************************/

int kernel_thread(FAR const char *name, int priority, int stack_size, main_t entry, FAR char *const argv[])
{
	return thread_create(name, TCB_FLAG_TTYPE_KERNEL, priority, stack_size, entry, argv);
}
