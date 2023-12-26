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
 * kernel/task/task_exithook.c
 *
 *   Copyright (C) 2011-2013 Gregory Nutt. All rights reserved.
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

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/sched.h>
#include <tinyara/fs/fs.h>

#include "sched/sched.h"
#include "group/group.h"
#include "signal/signal.h"
#include "task/task.h"
#include "debug/debug.h"
#if defined(CONFIG_BINARY_MANAGER) && defined(CONFIG_APP_BINARY_SEPARATION)
#include "binary_manager/binary_manager.h"
#endif

#ifdef CONFIG_TASK_MANAGER
#include <task_manager/task_manager.h>
#include <tinyara/task_manager_drv.h>
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: task_atexit
 *
 * Description:
 *   Call any registered atexit function(s)
 *
 ****************************************************************************/

#if defined(CONFIG_SCHED_ATEXIT) && !defined(CONFIG_SCHED_ONEXIT)
inline void task_atexit(FAR struct tcb_s *tcb)
{
	FAR struct task_group_s *group = tcb->group;
	struct atexit_s *patexit;

	/* Make sure that we have not already left the group.  Only the final
	 * exiting thread in the task group should trigger the atexit()
	 * callbacks.
	 */

	if (group && group->tg_nmembers == 1) {

		/* Call each atexit function in the reverse order of registration */

		while (!(sq_empty(&(group->tg_atexitfunc)))) {
			patexit = (struct atexit_s *)sq_remfirst(&(group->tg_atexitfunc));

			(*patexit->atexitfunc)();

			sched_kfree(patexit);
		}
	}
}
#else
#define task_atexit(tcb)
#endif

/****************************************************************************
 * Name: task_onexit
 *
 * Description:
 *   Call any registered on_exit function(s)
 *
 ****************************************************************************/

#ifdef CONFIG_SCHED_ONEXIT
inline void task_onexit(FAR struct tcb_s *tcb, int status)
{
	FAR struct task_group_s *group = tcb->group;
	struct onexit_s *ponexit;

	/* Make sure that we have not already left the group.  Only the final
	 * exiting thread in the task group should trigger the atexit()
	 * callbacks.
	 */

	if (group && group->tg_nmembers == 1) {

		/* Call each on_exit function in the reverse order of registration */

		while (!(sq_empty(&(group->tg_onexitfunc)))) {
			ponexit = (struct onexit_s *)sq_remfirst(&(group->tg_onexitfunc));

			(*ponexit->onexitfunc)(status, ponexit->onexitarg);

			sched_kfree(ponexit);
		}
	}
}
#else
#define task_onexit(tcb, status)
#endif

/****************************************************************************
 * Name: task_exitstatus
 *
 * Description:
 *   Report exit status when main task of a task group exits
 *
 ****************************************************************************/

#ifdef CONFIG_SCHED_CHILD_STATUS
static inline void task_exitstatus(FAR struct task_group_s *group, int status)
{
	FAR struct child_status_s *child;

	/* Check if the parent task group has suppressed retention of
	 * child exit status information.
	 */

	if ((group->tg_flags & GROUP_FLAG_NOCLDWAIT) == 0) {
		/* No.. Find the exit status entry for this task in the parent TCB */

		child = group_findchild(group, getpid());
		if (child) {
#ifndef HAVE_GROUP_MEMBERS
			/* No group members? Save the exit status */

			child->ch_status = status;
#endif
			/* Save the exit status..  For the case of HAVE_GROUP_MEMBERS,
			 * the child status will be as exited until the last member
			 * of the task group exits.
			 */

			child->ch_status = status;
		}
	}
}
#else

#define task_exitstatus(group, status)

#endif							/* CONFIG_SCHED_CHILD_STATUS */

/****************************************************************************
 * Name: task_groupexit
 *
 * Description:
 *   Mark that the final thread of a child task group as exited.
 *
 ****************************************************************************/

#ifdef CONFIG_SCHED_CHILD_STATUS
static inline void task_groupexit(FAR struct task_group_s *group)
{
	FAR struct child_status_s *child;

	/* Check if the parent task group has suppressed retention of child exit
	 * status information.
	 */

	if ((group->tg_flags & GROUP_FLAG_NOCLDWAIT) == 0) {
		/* No.. Find the exit status entry for this task in the parent TCB */

		child = group_findchild(group, getpid());
		if (child) {
			/* Mark that all members of the child task group has exited */

			child->ch_flags |= CHILD_FLAG_EXITED;
		}
	}
}

#else

#define task_groupexit(group)

#endif							/* CONFIG_SCHED_CHILD_STATUS */

/****************************************************************************
 * Name: task_sigchild
 *
 * Description:
 *   Send the SIGCHILD signal to the parent thread
 *
 ****************************************************************************/

#if defined(CONFIG_SCHED_HAVE_PARENT) && !defined(CONFIG_DISABLE_SIGNALS)
#ifdef HAVE_GROUP_MEMBERS
static inline void task_sigchild(gid_t pgid, FAR struct tcb_s *ctcb, int status)
{
	FAR struct task_group_s *chgrp = ctcb->group;
	FAR struct task_group_s *pgrp;
	siginfo_t info;

	DEBUGASSERT(chgrp);

	/* Get the parent task group.  It is possible that all of the members of
	 * the parent task group have exited.  This would not be an error.  In
	 * this case, the child task group has been orphaned.
	 */

	pgrp = group_findbygid(pgid);
	if (!pgrp) {
		/* Set the task group ID to an invalid group ID.  The dead parent
		 * task group ID could get reused some time in the future.
		 */

		chgrp->tg_pgid = INVALID_GROUP_ID;
		return;
	}

	/* Save the exit status now if this is the main thread of the task group
	 * that is exiting. Only the exiting main task of a task group carries
	 * interpretable exit  Check if this is the main task that is exiting.
	 */

#ifndef CONFIG_DISABLE_PTHREAD
	if ((ctcb->flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_PTHREAD)
#endif
	{
		task_exitstatus(pgrp, status);
	}

	/* But only the final exiting thread in a task group, whatever it is,
	 * should generate SIGCHLD.
	 */

	if (chgrp->tg_nmembers == 1) {
		/* Mark that all of the threads in the task group have exited */

		task_groupexit(pgrp);

		/* Create the siginfo structure.  We don't actually know the cause.
		 * That is a bug. Let's just say that the child task just exited
		 * for now.
		 */

		info.si_signo = SIGCHLD;
		info.si_code = CLD_EXITED;
		info.si_value.sival_ptr = NULL;
#ifndef CONFIG_DISABLE_PTHREAD
		info.si_pid = chgrp->tg_task;
#else
		info.si_pid = ctcb->pid;
#endif
		info.si_status = status;

		/* Send the signal to one thread in the group */

		(void)group_signal(pgrp, &info);
	}
}

#else							/* HAVE_GROUP_MEMBERS */

static inline void task_sigchild(FAR struct tcb_s *ptcb, FAR struct tcb_s *ctcb, int status)
{
	siginfo_t info;

	/* If task groups are not supported then we will report SIGCHLD when the
	 * task exits.  Unfortunately, there could still be threads in the group
	 * that are still running.
	 */

#ifndef CONFIG_DISABLE_PTHREAD
	if ((ctcb->flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_PTHREAD)
#endif
	{
#ifdef CONFIG_SCHED_CHILD_STATUS
		/* Save the exit status now of the main thread */

		task_exitstatus(ptcb->group, status);

#else							/* CONFIG_SCHED_CHILD_STATUS */
		/* Exit status is not retained.  Just decrement the number of
		 * children from this parent.
		 */

		DEBUGASSERT(ptcb->group != NULL && ptcb->group->tg_nchildren > 0);
		ptcb->group->tg_nchildren--;

#endif							/* CONFIG_SCHED_CHILD_STATUS */

		/* Create the siginfo structure.  We don't actually know the cause.
		 * That is a bug. Let's just say that the child task just exited
		 * for now.
		 */

		info.si_signo = SIGCHLD;
		info.si_code = CLD_EXITED;
		info.si_value.sival_ptr = NULL;
#ifndef CONFIG_DISABLE_PTHREAD
		info.si_pid = ctcb->group->tg_task;
#else
		info.si_pid = ctcb->pid;
#endif
		info.si_status = status;

		/* Send the signal.  We need to use this internal interface so that we
		 * can provide the correct si_code value with the signal.
		 */

		(void)sig_tcbdispatch(ptcb, &info);
	}
}

#endif							/* HAVE_GROUP_MEMBERS */
#else							/* CONFIG_SCHED_HAVE_PARENT && !CONFIG_DISABLE_SIGNALS */

#define task_sigchild(x, ctcb, status)

#endif							/* CONFIG_SCHED_HAVE_PARENT && !CONFIG_DISABLE_SIGNALS */

/****************************************************************************
 * Name: task_signalparent
 *
 * Description:
 *   Send the SIGCHILD signal to the parent task group
 *
 ****************************************************************************/

#ifdef CONFIG_SCHED_HAVE_PARENT
static inline void task_signalparent(FAR struct tcb_s *ctcb, int status)
{
#ifdef HAVE_GROUP_MEMBERS
	DEBUGASSERT(ctcb && ctcb->group);

	/* Keep things stationary throughout the following */

	sched_lock();

	/* Send SIGCHLD to all members of the parent's task group */

	task_sigchild(ctcb->group->tg_pgid, ctcb, status);
	sched_unlock();
#else
	FAR struct tcb_s *ptcb;

	/* Keep things stationary throughout the following */

	sched_lock();

	/* Get the TCB of the receiving, parent task.  We do this early to
	 * handle multiple calls to task_signalparent.  ctcb->group->tg_ppid is
	 * set to an invalid value below and the following call will fail if we
	 * are called again.
	 */

	ptcb = sched_gettcb(ctcb->group->tg_ppid);
	if (ptcb == NULL) {
		/* The parent no longer exists... bail */

		sched_unlock();
		return;
	}

	/* If ptcb is not ctcb, send SIGCHLD to all members of the parent's task group.
	 * If ptcb is same as ctcb, calling task_sigchild is not needed.
	 * Because ctcb is the last exit task.
	 */
	if (ptcb->pid != ctcb->pid) {
		task_sigchild(ptcb, ctcb, status);
	}

	sched_unlock();
#endif
}
#else
#define task_signalparent(ctcb, status)
#endif

/****************************************************************************
 * Name: task_exitwakeup
 *
 * Description:
 *   Wakeup any tasks waiting for this task to exit
 *
 ****************************************************************************/

#if defined(CONFIG_SCHED_WAITPID) && !defined(CONFIG_SCHED_HAVE_PARENT)
static inline void task_exitwakeup(FAR struct tcb_s *tcb, int status)
{
	FAR struct task_group_s *group = tcb->group;

	/* Have we already left the group? */

	if (group) {
		/* Only tasks (and kernel threads) return valid status.  Record the
		 * exit status when the task exists.  The group, however, may still
		 * be executing.
		 */

#ifndef CONFIG_DISABLE_PTHREAD
		if ((tcb->flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_PTHREAD)
#endif
		{
			/* Report the exit status.  We do not nullify tg_statloc here
			 * because we want to prevent other tasks from registering for
			 * the return status.  There is only one task per task group,
			 * there for, this logic should execute exactly once in the
			 * lifetime of the task group.
			 *
			 * "If more than one thread is suspended in waitpid() awaiting
			 *  termination of the same process, exactly one thread will
			 *  return the process status at the time of the target process
			 *  termination."
			 *
			 *  Hmmm.. what do we return to the others?
			 */

			if (group->tg_statloc) {
				*group->tg_statloc = status << 8;
			}
		}

		/* Is this the last thread in the group? */

		if (group->tg_nmembers == 1) {
			/* Yes.. Wakeup any tasks waiting for this task to exit */

			group->tg_statloc = NULL;
			while (group->tg_exitsem.semcount < 0) {
				/* Wake up the thread */

				sem_post(&group->tg_exitsem);
			}
		}
	}
}
#else
#define task_exitwakeup(tcb, status)
#endif

/****************************************************************************
 * Name: task_flushstreams
 *
 * Description:
 *   Flush all streams when the final thread of a group exits.
 *
 ****************************************************************************/

#if CONFIG_NFILE_STREAMS > 0
static inline void task_flushstreams(FAR struct tcb_s *tcb)
{
	FAR struct task_group_s *group = tcb->group;

	/* Have we already left the group?  Are we the last thread in the group? */

	if (group && group->tg_nmembers == 1) {
#if (defined(CONFIG_BUILD_PROTECTED) || defined(CONFIG_BUILD_KERNEL)) && \
	 defined(CONFIG_MM_KERNEL_HEAP)
		(void)lib_flushall(tcb->group->tg_streamlist);
#else
		(void)lib_flushall(&tcb->group->tg_streamlist);
#endif
	}
}
#else
#define task_flushstreams(tcb)
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: task_exithook
 *
 * Description:
 *   This function implements some of the internal logic of exit() and
 *   task_delete().  This function performs some clean-up and other actions
 *   required when a task exits:
 *
 *   - All open streams are flushed and closed.
 *   - All functions registered with atexit() and on_exit() are called, in
 *     the reverse order of their registration.
 *
 *   When called from exit(), the tcb still resides at the head of the ready-
 *   to-run list.  The following logic is safe because we will not be
 *   returning from the exit() call.
 *
 *   When called from task_terminate() we are operating on a different thread;
 *   on the thread that called task_delete().  In this case, task_delete
 *   will have already removed the tcb from the ready-to-run list to prevent
 *   any further action on this task.
 *
 *   nonblocking will be set true only when we are called from task_terminate()
 *   via _exit().  In that case, we must be careful to do nothing that can
 *   cause the cause the thread to block.
 *
 ****************************************************************************/

void task_exithook(FAR struct tcb_s *tcb, int status, bool nonblocking)
{
	/* Under certain conditions, task_exithook() can be called multiple times.
	 * A bit in the TCB was set the first time this function was called.  If
	 * that bit is set, then just exit doing nothing more..
	 */

	if ((tcb->flags & TCB_FLAG_EXIT_PROCESSING) != 0) {
		return;
	}

#ifdef CONFIG_DEBUG
	/* Save the terminated task/pthread's information for stack monitor and heapinfo. */
	dbg_save_termination_info(tcb);
#endif

#ifdef CONFIG_CANCELLATION_POINTS
	/* Mark the task as non-cancelable to avoid additional calls to exit()
	 * due to any cancellation point logic that might get kicked off by
	 * actions taken during exit processing.
	 */
	tcb->flags |= TCB_FLAG_NONCANCELABLE;
	tcb->flags &= ~TCB_FLAG_CANCEL_PENDING;
	tcb->cpcount = 0;
#endif

#if defined(CONFIG_SCHED_ATEXIT) || defined(CONFIG_SCHED_ONEXIT)
	/* If exit function(s) were registered, call them now before we do any un-
	 * initialization.
	 *
	 * NOTES:
	 *
	 * 1. In the case of task_delete(), the exit function will *not* be called
	 *    on the thread execution of the task being deleted!  That is probably
	 *    a bug.
	 * 2. We cannot call the exit functions if nonblocking is requested:  These
	 *    functions might block.
	 * 3. This function will only be called with with non-blocking == true
	 *    only when called through _exit(). _exit() behaviors requires that
	 *    the exit functions *not* be called.
	 */

	if (!nonblocking) {
		task_atexit(tcb);

		/* Call any registered on_exit function(s) */

		task_onexit(tcb, status);
	}
#endif

	/* If the task was terminated by another task, it may be in an unknown
	 * state.  Make some feeble effort to recover the state.
	 */

	task_recover(tcb);

	/* NOTE: signal handling needs to be done in a critical section. */
#ifdef CONFIG_SMP
	irqstate_t flags = enter_critical_section();
#endif

	/* Send the SIGCHILD signal to the parent task group */

	task_signalparent(tcb, status);

	/* Wakeup any tasks waiting for this task to exit */

	task_exitwakeup(tcb, status);

	/* If this is the last thread in the group, then flush all streams (File
	 * descriptors will be closed when the TCB is deallocated).
	 *
	 * NOTES:
	 * 1. We cannot flush the buffered I/O if nonblocking is requested.
	 *    that might cause this logic to block.
	 * 2. This function will only be called with with non-blocking == true
	 *    only when called through _exit(). _exit() behavior does not
	 *    require that the streams be flushed
	 */

	if (!nonblocking) {
		task_flushstreams(tcb);
	}

#if defined(CONFIG_BINARY_MANAGER) && defined(CONFIG_APP_BINARY_SEPARATION)
	/* Remove a tcb from binary list */
	binary_manager_remove_binlist(tcb);
#endif

#ifdef HAVE_TASK_GROUP
	/* Leave the task group.  Perhaps discarding any un-reaped child
	 * status (no zombies here!)
	 */

	group_leave(tcb);
#endif

#ifndef CONFIG_DISABLE_SIGNALS
	/* Deallocate anything left in the TCB's queues */

	sig_cleanup(tcb);			/* Deallocate Signal lists */
#endif

#ifdef CONFIG_SMP
	leave_critical_section(flags);
#endif

	/* This function can be re-entered in certain cases.  Set a flag
	 * bit in the TCB to not that we have already completed this exit
	 * processing.
	 */

	tcb->flags |= TCB_FLAG_EXIT_PROCESSING;
}
