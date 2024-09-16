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
 * kernel/task/task_vfork
 *
 *   Copyright (C) 2013-2014 Gregory Nutt. All rights reserved.
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

#include <sys/wait.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <queue.h>
#include <debug.h>

#include <tinyara/sched.h>

#include "sched/sched.h"
#include "group/group.h"
#include "task/task.h"

#include <tinyara/common_logs/common_logs.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* vfork() requires architecture-specific support as well as waipid(). */

#if defined(CONFIG_ARCH_HAVE_VFORK) && defined(CONFIG_SCHED_WAITPID)

/* This is an artificial limit to detect error conditions where an argv[]
 * list is not properly terminated.
 */

#define MAX_VFORK_ARGS 256

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vfork_namesetup
 *
 * Description:
 *   Copy the task name.
 *
 * Input Parameters:
 *   tcb        - Address of the new task's TCB
 *   name       - Name of the new task
 *
 * Return Value:
 *  None
 *
 ****************************************************************************/

#if CONFIG_TASK_NAME_SIZE > 0
static inline void vfork_namesetup(FAR struct tcb_s *parent, FAR struct task_tcb_s *child)
{
	/* Copy the name from the parent into the child TCB */

	strncpy(child->cmn.name, parent->name, CONFIG_TASK_NAME_SIZE);
}
#else
#define vfork_namesetup(p, c)
#endif							/* CONFIG_TASK_NAME_SIZE */

/****************************************************************************
 * Name: vfork_stackargsetup
 *
 * Description:
 *   Clone the task arguments in the same relative positions on the child's
 *   stack.
 *
 * Input Parameters:
 *   parent - Address of the parent task's TCB
 *   child  - Address of the child task's TCB
 *
 * Return Value:
 *   Zero (OK) on success; a negated errno on failure.
 *
 ****************************************************************************/

static inline int vfork_stackargsetup(FAR struct tcb_s *parent, FAR struct task_tcb_s *child)
{
	/* Is the parent a task? or a pthread?  Only tasks (and kernel threads)
	 * have command line arguments.
	 */

	child->argv = NULL;
	if ((parent->flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_PTHREAD) {
		FAR struct task_tcb_s *ptcb = (FAR struct task_tcb_s *)parent;
		uintptr_t offset;
		int argc;

		/* Get the address correction */

		offset = child->cmn.xcp.regs[REG_SP] - parent->xcp.regs[REG_SP];

		/* Change the child argv[] to point into its stack (instead of its
		 * parent's stack).
		 */

		child->argv = (FAR char **)((uintptr_t)ptcb->argv + offset);

		/* Copy the adjusted address for each argument */

		argc = 0;
		while (ptcb->argv[argc]) {
			uintptr_t newaddr = (uintptr_t)ptcb->argv[argc] + offset;
			child->argv[argc] = (FAR char *)newaddr;

			/* Increment the number of args.  Here is a sanity check to
			 * prevent running away with an unterminated argv[] list.
			 * MAX_VFORK_ARGS should be sufficiently large that this never
			 * happens in normal usage.
			 */

			if (++argc > MAX_VFORK_ARGS) {
				return -E2BIG;
			}
		}

		/* Put a terminator entry at the end of the child argv[] array. */

		child->argv[argc] = NULL;
	}

	return OK;
}

/****************************************************************************
 * Name: vfork_argsetup
 *
 * Description:
 *   Clone the argument list from the parent to the child.
 *
 * Input Parameters:
 *   parent - Address of the parent task's TCB
 *   child  - Address of the child task's TCB
 *
 * Return Value:
 *   Zero (OK) on success; a negated errno on failure.
 *
 ****************************************************************************/

static inline int vfork_argsetup(FAR struct tcb_s *parent, FAR struct task_tcb_s *child)
{
	/* Clone the task name */

	vfork_namesetup(parent, child);

	/* Adjust and copy the argv[] array. */

	return vfork_stackargsetup(parent, child);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: task_vforksetup
 *
 * Description:
 *   The vfork() function has the same effect as fork(), except that the
 *   behavior is undefined if the process created by vfork() either modifies
 *   any data other than a variable of type pid_t used to store the return
 *   value from vfork(), or returns from the function in which vfork() was
 *   called, or calls any other function before successfully calling _exit()
 *   or one of the exec family of functions.
 *
 *   This function provides one step in the overall vfork() sequence:  It
 *   Allocates and initializes the child task's TCB.  The overall sequence is:
 *
 *   1) User code calls vfork().  vfork() is provided in architecture-specific
 *      code.
 *   2) vfork()and calls task_vforksetup().
 *   3) task_vforksetup() allocates and configures the child task's TCB.  This
 *      consists of:
 *      - Allocation of the child task's TCB.
 *      - Initialization of file descriptors and streams
 *      - Configuration of environment variables
 *      - Setup the input parameters for the task.
 *      - Initialization of the TCB (including call to up_initial_state()
 *   4) up_vfork() provides any additional operating context. up_vfork must:
 *      - Allocate and initialize the stack
 *      - Initialize special values in any CPU registers that were not
 *        already configured by up_initial_state()
 *   5) up_vfork() then calls task_vforkstart()
 *   6) task_vforkstart() then executes the child thread.
 *
 * Input Parameters:
 *   parent - Address of the parent task's TCB
 *   child  - Address of the child task's TCB
 *
 * Returned Value:
 *   Upon successful completion, task_vforksetup() returns a pointer to
 *   newly allocated and initialized child task's TCB.  NULL is returned
 *   on any failure and the errno is set appropriately.
 *
 ****************************************************************************/

FAR struct task_tcb_s *task_vforksetup(start_t retaddr)
{
	struct tcb_s *parent = this_task();
	struct task_tcb_s *child;
	uint8_t ttype;
	int priority;
	int ret;

	DEBUGASSERT(retaddr);

	/* Get the type of the fork'ed task (kernel or user) */

	if ((parent->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_KERNEL) {
		/* Fork'ed from a kernel thread */

		ttype = TCB_FLAG_TTYPE_KERNEL;
	} else {
		/* Fork'ed from a user task or pthread */

		ttype = TCB_FLAG_TTYPE_TASK;
	}

	/* Allocate a TCB for the child task. */

	child = (FAR struct task_tcb_s *)kmm_zalloc(sizeof(struct task_tcb_s));
	if (!child) {
		sdbg("%s TCB\n", clog_message_str[CMN_LOG_ALLOC_FAIL]);
		set_errno(ENOMEM);
		return NULL;
	}

	/* Allocate a new task group with the same privileges as the parent */

#ifdef HAVE_TASK_GROUP
	ret = group_allocate(child, parent->flags);
	if (ret < 0) {
		goto errout_with_tcb;
	}
#endif

	/* Associate file descriptors with the new task */

#if CONFIG_NFILE_DESCRIPTORS > 0 || CONFIG_NSOCKET_DESCRIPTORS > 0
	ret = group_setuptaskfiles(child);
	if (ret < OK) {
		goto errout_with_tcb;
	}
#endif

	/* Get the priority of the parent task */

#ifdef CONFIG_PRIORITY_INHERITANCE
	priority = parent->base_priority;	/* "Normal," unboosted priority */
#else
	priority = parent->sched_priority;	/* Current priority */
#endif

	/* Initialize the task control block.  This calls up_initial_state() */

	svdbg("Child priority=%d start=%p\n", priority, retaddr);
	ret = task_schedsetup(child, priority, retaddr, parent->entry.main, ttype);
	if (ret < OK) {
		ret = -get_errno();
		goto errout_with_tcb;
	}

	svdbg("parent=%p, returning child=%p\n", parent, child);
	return child;

errout_with_tcb:
	sched_releasetcb((FAR struct tcb_s *)child, ttype);
	set_errno(-ret);
	return NULL;
}

/****************************************************************************
 * Name: task_vforkstart
 *
 * Description:
 *   The vfork() function has the same effect as fork(), except that the
 *   behavior is undefined if the process created by vfork() either modifies
 *   any data other than a variable of type pid_t used to store the return
 *   value from vfork(), or returns from the function in which vfork() was
 *   called, or calls any other function before successfully calling _exit()
 *   or one of the exec family of functions.
 *
 *   This function provides one step in the overall vfork() sequence:  It
 *   starts execution of the previously initialized TCB.  The overall
 *   sequence is:
 *
 *   1) User code calls vfork()
 *   2) Architecture-specific code provides vfork()and calls task_vforksetup().
 *   3) task_vforksetup() allocates and configures the child task's TCB.  This
 *      consists of:
 *      - Allocation of the child task's TCB.
 *      - Initialization of file descriptors and streams
 *      - Configuration of environment variables
 *      - Setup the input parameters for the task.
 *      - Initialization of the TCB (including call to up_initial_state()
 *   4) vfork() provides any additional operating context. vfork must:
 *      - Allocate and initialize the stack
 *      - Initialize special values in any CPU registers that were not
 *        already configured by up_initial_state()
 *   5) vfork() then calls task_vforkstart()
 *   6) task_vforkstart() then executes the child thread.
 *
 * Input Parameters:
 *   retaddr - The return address from vfork() where the child task
 *     will be started.
 *
 * Returned Value:
 *   Upon successful completion, vfork() returns 0 to the child process and
 *   returns the process ID of the child process to the parent process.
 *   Otherwise, -1 is returned to the parent, no child process is created,
 *   and errno is set to indicate the error.
 *
 ****************************************************************************/

pid_t task_vforkstart(FAR struct task_tcb_s *child)
{
	struct tcb_s *parent = this_task();
	pid_t pid;
	int rc;
	int ret;

	svdbg("Starting Child TCB=%p, parent=%p\n", child, this_task());
	DEBUGASSERT(child);

	/* Duplicate the original argument list in the forked child TCB */

	ret = vfork_argsetup(parent, child);
	if (ret < 0) {
		task_vforkabort(child, -ret);
		return ERROR;
	}

	/* Now we have enough in place that we can join the group */

#ifdef HAVE_TASK_GROUP
	ret = group_initialize(child);
	if (ret < 0) {
		task_vforkabort(child, -ret);
		return ERROR;
	}
#endif

	/* Get the assigned pid before we start the task */

	pid = (int)child->cmn.pid;

	/* Activate the task */

	(void)task_activate((FAR struct tcb_s *)child);

	/* Since the child task has the same priority as the parent task, it is
	 * now ready to run, but has not yet ran.  It is a requirement that
	 * the parent environment be stable while vfork runs; the child thread
	 * is still dependent on things in the parent thread... like the pointers
	 * into parent thread's stack which will still appear in the child's
	 * registers and environment.
	 *
	 * We do not have SIG_CHILD, so we have to do some silly things here.
	 * The simplest way to make sure that the child thread runs to completion
	 * is simply to yield here.  Since the child can only do exit() or
	 * execv/l(), that should be all that is needed.
	 *
	 * Hmmm.. this is probably not sufficient.  What if we are running
	 * SCHED_RR?  What if the child thread is suspended and rescheduled
	 * after the parent thread again?
	 */

	/* We can also exploit a bug in the execv() implementation:  The PID
	 * of the task exec'ed by the child will not be the same as the PID of
	 * the child task.  Therefore, waitpid() on the child task's PID will
	 * accomplish what we need to do.
	 */

	rc = 0;

#ifdef CONFIG_DEBUG
	ret = waitpid(pid, &rc, 0);
	if (ret < 0) {
		sdbg("%s waitpid %d\n", clog_message_str[CMN_LOG_FAILED_OP], errno);
	}
#else
	(void)waitpid(pid, &rc, 0);
#endif

	return pid;
}

/****************************************************************************
 * Name: task_vforkabort
 *
 * Description:
 *   Recover from any errors after task_vforksetup() was called.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void task_vforkabort(FAR struct task_tcb_s *child, int errcode)
{
	/* The TCB was added to the active task list by task_schedsetup() */

	dq_rem((FAR dq_entry_t *)child, (dq_queue_t *)&g_inactivetasks);

	/* Release the TCB */

	sched_releasetcb((FAR struct tcb_s *)child, child->cmn.flags & TCB_FLAG_TTYPE_MASK);
	set_errno(errcode);
}

#endif							/* CONFIG_ARCH_HAVE_VFORK && CONFIG_SCHED_WAITPID */
