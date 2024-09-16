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
 * kernel/pthread/pthread_create.c
 *
 *   Copyright (C) 2007-2009, 2011, 2013-2016 Gregory Nutt. All rights reserved.
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
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>
#include <queue.h>

#ifdef CONFIG_DEBUG_MM_HEAPINFO
#include <tinyara/mm/mm.h>
#endif

#include <tinyara/arch.h>
#include <tinyara/semaphore.h>
#include <tinyara/kmalloc.h>
#include <tinyara/pthread.h>
#include <tinyara/ttrace.h>

#include "sched/sched.h"
#include "group/group.h"
#include "clock/clock.h"
#include "pthread/pthread.h"
#if defined(CONFIG_BINARY_MANAGER) && defined(CONFIG_APP_BINARY_SEPARATION)
#include "binary_manager/binary_manager.h"
#endif

#include <tinyara/common_logs/common_logs.h>

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Default pthread attributes (see include/nuttx/pthread.h).  When configured
 * to build separate kernel- and user-address spaces, this global is
 * duplicated in each address spaced.  This copy can only be shared within
 * the kernel address space.
 */

const pthread_attr_t g_default_pthread_attr = PTHREAD_ATTR_INITIALIZER;

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* This is the name for name-less pthreads */

static const char g_pthreadname[] = "<pthread>";

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pthread_argsetup
 *
 * Description:
 *   This functions sets up parameters in the Task Control Block (TCB) in
 *   preparation for starting a new thread.
 *
 *   pthread_argsetup() is called from task_init() and task_start() to create
 *   a new task (with arguments cloned via strdup) or pthread_create() which
 *   has one argument passed by value (distinguished by the pthread boolean
 *   argument).
 *
 * Input Parameters:
 *   tcb        - Address of the new task's TCB
 *   arg        - The argument to provide to the pthread on startup.
 *
 * Return Value:
 *  None
 *
 ****************************************************************************/

static inline void pthread_argsetup(FAR struct pthread_tcb_s *tcb, pthread_addr_t arg)
{
#if CONFIG_TASK_NAME_SIZE > 0
	/* Copy the pthread name into the TCB */

	strncpy(tcb->cmn.name, g_pthreadname, CONFIG_TASK_NAME_SIZE);
	tcb->cmn.name[CONFIG_TASK_NAME_SIZE] = '\0';
#endif							/* CONFIG_TASK_NAME_SIZE */

	/* For pthreads, args are strictly pass-by-value; that actual
	 * type wrapped by pthread_addr_t is unknown.
	 */

	tcb->arg = arg;
}

/****************************************************************************
 * Name: pthread_addjoininfo
 *
 * Description:
 *   Add a join structure to the local data set.
 *
 * Parameters:
 *   pjoin
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   The caller has provided protection from re-entrancy.
 *
 ****************************************************************************/

static inline void pthread_addjoininfo(FAR struct task_group_s *group, FAR struct join_s *pjoin)
{
	pjoin->next = NULL;
	if (!group->tg_jointail) {
		group->tg_joinhead = pjoin;
	} else {
		group->tg_jointail->next = pjoin;
	}

	group->tg_jointail = pjoin;
}

/****************************************************************************
 * Name:  pthread_start
 *
 * Description:
 *   This function is the low level entry point into the pthread
 *
 * Parameters:
 * None
 *
 ****************************************************************************/

static void pthread_start(void)
{
	FAR struct pthread_tcb_s *ptcb = (FAR struct pthread_tcb_s *)this_task();
	FAR struct task_group_s *group = ptcb->cmn.group;
	FAR struct join_s *pjoin = (FAR struct join_s *)ptcb->joininfo;
	pthread_addr_t exit_status;

	DEBUGASSERT(group && pjoin);

	/* Sucessfully spawned, add the pjoin to our data set. */

	(void)pthread_sem_take(&group->tg_joinsem, false);
	pthread_addjoininfo(group, pjoin);
	(void)pthread_sem_give(&group->tg_joinsem);

	/* Report to the spawner that we successfully started. */

	pjoin->started = true;
	(void)pthread_sem_give(&pjoin->data_sem);

	/* Pass control to the thread entry point. In the kernel build this has to
	 * be handled differently if we are starting a user-space pthread; we have
	 * to switch to user-mode before calling into the pthread.
	 */

#if defined(CONFIG_BUILD_PROTECTED) || defined(CONFIG_BUILD_KERNEL)
	up_pthread_start(ptcb->cmn.entry.pthread, ptcb->arg);
	exit_status = NULL;
#else
	exit_status = (*ptcb->cmn.entry.pthread)(ptcb->arg);
#endif

	/* The thread has returned (should never happen in the kernel mode case) */

	pthread_exit(exit_status);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name:  pthread_create
 *
 * Description:
 *   This function creates and activates a new thread with a specified
 *   attributes.
 *
 * Input Parameters:
 *    thread
 *    attr
 *    start_routine
 *    arg
 *
 * Returned value:
 *   OK (0) on success; a (non-negated) errno value on failure. The errno
 *   variable is not set.
 *
 ****************************************************************************/

int pthread_create(FAR pthread_t *thread, FAR const pthread_attr_t *attr, pthread_startroutine_t start_routine, pthread_addr_t arg)
{
	FAR struct pthread_tcb_s *ptcb;
	FAR struct join_s *pjoin;
	struct sched_param param;
	int policy;
	int errcode;
	pid_t pid;
	int ret;
#ifdef HAVE_TASK_GROUP
	bool group_joined = false;
#endif

#ifdef CONFIG_APP_BINARY_SEPARATION
	ASSERT((sched_self()->flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_KERNEL);
#endif

	trace_begin(TTRACE_TAG_TASK, "pthread_create");

	/* Check whether we are allowed to create new pthread ? */

	if (g_alive_taskcount == CONFIG_MAX_TASKS) {
		sdbg("ERROR: CONFIG_MAX_TASKS(%d) count reached\n", CONFIG_MAX_TASKS);
		trace_end(TTRACE_TAG_TASK);
		return EBUSY;
	}

	/* If attributes were not supplied, use the default attributes */

	if (!attr) {
		attr = &g_default_pthread_attr;
	}

#if defined(CONFIG_BINARY_MANAGER) && defined(CONFIG_APP_BINARY_SEPARATION)
	if (BM_PRIORITY_MIN - 1 < attr->priority && attr->priority < BM_PRIORITY_MAX + 1) {
		sdbg("%s priority %d, should be > %d, < %d\n", clog_message_str[CMN_LOG_INVALID_VAL], attr->priority, BM_PRIORITY_MIN, BM_PRIORITY_MAX);
		return EPERM;
	}
#endif

	/* Allocate a TCB for the new task. */

	ptcb = (FAR struct pthread_tcb_s *)kmm_zalloc(sizeof(struct pthread_tcb_s));
	if (!ptcb) {
		sdbg("%s TCB\n", clog_message_str[CMN_LOG_ALLOC_FAIL]);
		return ENOMEM;
	}
#ifdef HAVE_TASK_GROUP
	/* Bind the parent's group to the new TCB (we have not yet joined the
	 * group).
	 */

	ret = group_bind(ptcb);
	if (ret < 0) {
		errcode = -ret;
		goto errout_with_tcb;
	}
#endif

#ifdef CONFIG_ARCH_ADDRENV
	/* Share the address environment of the parent task group. */

	ret = up_addrenv_attach(ptcb->cmn.group, this_task());
	if (ret < 0) {
		errcode = -ret;
		goto errout_with_tcb;
	}
#endif

	/* Allocate a detachable structure to support pthread_join logic */

	pjoin = (FAR struct join_s *)kmm_zalloc(sizeof(struct join_s));
	if (!pjoin) {
		sdbg("%s join\n", clog_message_str[CMN_LOG_ALLOC_FAIL]);
		errcode = ENOMEM;
		goto errout_with_tcb;
	}

	/* Allocate the stack for the TCB */

	ret = up_create_stack((FAR struct tcb_s *)ptcb, attr->stacksize, TCB_FLAG_TTYPE_PTHREAD);
	if (ret != OK) {
		errcode = -ret;
		goto errout_with_join;
	}
#if defined(CONFIG_BUILD_PROTECTED)
	ptcb->region = (struct pthread_region_s *)attr->region;
#endif

	/* Should we use the priority and scheduler specified in the pthread
	 * attributes?  Or should we use the current thread's priority and
	 * scheduler?
	 */

	if (attr->inheritsched == PTHREAD_INHERIT_SCHED) {
		/* Get the priority (and any other scheduling parameters) for this
		 * thread.
		 */

		ret = sched_getparam(0, &param);
		if (ret == ERROR) {
			errcode = get_errno();
			goto errout_with_join;
		}

		/* Get the scheduler policy for this thread */

		policy = sched_getscheduler(0);
		if (policy == ERROR) {
			errcode = get_errno();
			goto errout_with_join;
		}
	} else {
		/* Use the scheduler policy and policy the attributes */

		policy = attr->policy;
		param.sched_priority = attr->priority;

	}

	/* Initialize the task control block */

	ret = pthread_schedsetup(ptcb, param.sched_priority, pthread_start, start_routine);
	if (ret != OK) {
		errcode = get_errno();
		goto errout_with_join;
	}

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	/* Exclude a stack node from heap usages of current thread.
	 * This will be shown separately as stack usages.
	 */
	heapinfo_exclude_stacksize(ptcb->cmn.stack_alloc_ptr);
	/* Update the pid information to set a stack node */
	heapinfo_set_stack_node(ptcb->cmn.stack_alloc_ptr, ptcb->cmn.pid);
#endif

#ifdef CONFIG_SMP
	/* pthread_schedsetup() will set the affinity mask by inheriting the
	 * setting from the parent task. We need to override this setting
	 * with the value from the pthread attributes unless that value is
	 * zero: Zero is the default value and simply means yo inherit the
	 * parent thread's affinity mask
	 */

	if (attr->affinity != 0) {
		ptcb->cmn.affinity = attr->affinity;
	}
#endif

	/* Configure the TCB for a pthread receiving on parameter
	 * passed by value
	 */

	pthread_argsetup(ptcb, arg);

#ifdef HAVE_TASK_GROUP
	/* Join the parent's task group */

	ret = group_join(ptcb);
	if (ret < 0) {
		errcode = -ret;
		goto errout_with_join;
	}

	group_joined = true;
#endif

	/* Attach the join info to the TCB. */

	ptcb->joininfo = (FAR void *)pjoin;

	/* Set the appropriate scheduling policy in the TCB */

	switch (policy) {
	case SCHED_FIFO:
		ptcb->cmn.flags &= ~TCB_FLAG_ROUND_ROBIN;
		break;

#if CONFIG_RR_INTERVAL > 0
	case SCHED_RR:
		ptcb->cmn.flags |= TCB_FLAG_ROUND_ROBIN;
		ptcb->cmn.timeslice = MSEC2TICK(CONFIG_RR_INTERVAL);
		break;
#endif
	default:
		DEBUGPANIC();
	}

#ifdef CONFIG_CANCELLATION_POINTS
	/* Set the deferred cancellation types */
	ptcb->cmn.flags |= TCB_FLAG_CANCEL_DEFERRED;
#endif
	/* Get the assigned pid before we start the task (who knows what
	 * could happen to ptcb after this!).  Copy this ID into the join structure
	 * as well.
	 */

	pid = (int)ptcb->cmn.pid;
	pjoin->thread = (pthread_t)pid;

	/* Initialize the semaphores in the join structure to zero. */

	ret = sem_init(&pjoin->data_sem, 0, 0);
	if (ret == OK) {
		ret = sem_init(&pjoin->exit_sem, 0, 0);
	}

	/*
	 * These semapohres are used for signaling and, hence, should not have
	 * priority inheritance enabled.
	 */
	if (ret == OK) {
		ret = sem_setprotocol(&pjoin->data_sem, SEM_PRIO_NONE);
	}

	if (ret == OK) {
		ret = sem_setprotocol(&pjoin->exit_sem, SEM_PRIO_NONE);
	}

	/* Activate the task */

	sched_lock();
	if (ret == OK) {
#if defined(CONFIG_BINARY_MANAGER) && defined(CONFIG_APP_BINARY_SEPARATION)
		/* Add tcb to binary thread list */

		binary_manager_add_binlist(&ptcb->cmn);
#endif
		ret = task_activate((FAR struct tcb_s *)ptcb);
	}

	if (ret == OK) {
		/* Wait for the task to actually get running and to register
		 * its join structure.
		 */

		(void)pthread_sem_take(&pjoin->data_sem, false);

		/* Return the thread information to the caller */

		if (thread) {
			*thread = (pthread_t)pid;
		}

		if (!pjoin->started) {
			ret = EINVAL;
		}
		sched_unlock();
		(void)sem_destroy(&pjoin->data_sem);
	} else {
		sched_unlock();
		dq_rem((FAR dq_entry_t *)ptcb, (FAR dq_queue_t *)&g_inactivetasks);
		(void)sem_destroy(&pjoin->data_sem);
		(void)sem_destroy(&pjoin->exit_sem);

		errcode = EIO;
		goto errout_with_join;
	}

	trace_end(TTRACE_TAG_TASK);
	return ret;

errout_with_join:
	sched_kfree(pjoin);
	ptcb->joininfo = NULL;

errout_with_tcb:
#ifdef HAVE_TASK_GROUP
	/* Clear group binding */

	if (ptcb && !group_joined) {
		ptcb->cmn.group = NULL;
	}
#endif

	sched_releasetcb((FAR struct tcb_s *)ptcb, TCB_FLAG_TTYPE_PTHREAD);
	trace_end(TTRACE_TAG_TASK);
	return errcode;
}
