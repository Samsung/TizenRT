/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * pthread_join.c
 *
 *   Copyright (C) 2007, 2008, 2011, 2013 Gregory Nutt. All rights reserved.
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
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/cancelpt.h>
#include <tinyara/ttrace.h>

#include "sched/sched.h"
#include "group/group.h"
#include "pthread/pthread.h"

/****************************************************************************
 * Pre-processor Definitions
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
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pthread_join_internal
 *
 * Description:
 *    A thread can await termination of another thread and retrieve the
 *    return value of the thread.
 *
 *    The caller's task/thread must belong to the same "task group" as the
 *    pthread is (or was) a member of.  The thread may or may not still
 *    be running.
 *
 * Parameters:
 *   thread
 *   pexit_value
 *   blocking
 *
 * Return Value:
 *   0 if successful.  Otherwise, one of the following error codes:
 *
 *   EINVAL  The value specified by thread does not refer to joinable
 *           thread.
 *   ESRCH   No thread could be found corresponding to that specified by the
 *           given thread ID.
 *   EDEADLK A deadlock was detected or the value of thread specifies the
 *           calling thread.
 *   EBUSY   Thread is still alive even non-blocking is given.
 *
 * Assumptions:
 *
 ****************************************************************************/

int pthread_join_internal(pthread_t thread, FAR pthread_addr_t *pexit_value, bool blocking)
{
	FAR struct tcb_s *rtcb = this_task();
	FAR struct task_group_s *group = rtcb->group;
	FAR struct join_s *pjoin;
	int ret;

	svdbg("thread=%d group=%p\n", thread, group);
	DEBUGASSERT(group);

	/* First make sure that this is not an attempt to join to
	 * ourself.
	 */

	if ((pid_t)thread == rtcb->pid) {
		return EDEADLK;
	}

	/* Make sure no other task is mucking with the data structures
	 * while we are performing the following operations.  NOTE:
	 * we can be also sure that pthread_exit() will not execute
	 * because it will also attempt to get this semaphore.
	 */

	(void)pthread_sem_take(&group->tg_joinsem, false);

	/* Find the join information associated with this thread.
	 * This can fail for one of three reasons:  (1) There is no
	 * thread associated with 'thread,' (2) the thread is a task
	 * and does not have join information, or (3) the thread
	 * was detached and has exited.
	 */

	pjoin = pthread_findjoininfo(group, (pid_t)thread);
	if (!pjoin) {
		/* Determine what kind of error to return */

		FAR struct tcb_s *tcb = sched_gettcb((pthread_t)thread);

		sdbg("Could not find thread data\n");

		/* Case (1) or (3) -- we can't tell which.  Assume (3) */

		if (!tcb) {
			ret = ESRCH;
		}

		/* The thread is still active but has no join info.  In that
		 * case, it must be a task and not a pthread.
		 */

		else {
			ret = EINVAL;
		}

		(void)pthread_sem_give(&group->tg_joinsem);
	} else {
		/* NOTE: sched_lock() is not enough for SMP because
		 * another CPU would continue the pthread and exit
		 * sequences so need to protect it with a critical section
		 */

#ifdef CONFIG_SMP
		irqstate_t flags = enter_critical_section();
#endif
		/* We found the join info structure.  Increment for the reference
		 * to the join structure that we have.  This will keep things
		 * stable for we have to do
		 */

		sched_lock();
		pjoin->crefs++;

		/* Check if the thread is still running.  If not, then things are
		 * simpler.  There are still race conditions to be concerned with.
		 * For example, there could be multiple threads executing in the
		 * 'else' block below when we enter!
		 */

		if (pjoin->terminated) {
			svdbg("Thread has terminated\n");

			/* Get the thread exit value from the terminated thread. */

			if (pexit_value) {
				svdbg("exit_value=0x%p\n", pjoin->exit_value);
				*pexit_value = pjoin->exit_value;
			}
		} else {
			svdbg("Thread is still running\n");

			if (blocking == true) {
				/* Relinquish the data set semaphore.  Since pre-emption is
				 * disabled, we can be certain that no task has the
				 * opportunity to run between the time we relinquish the
				 * join semaphore and the time that we wait on the thread exit
				 * semaphore.
				 */

				(void)pthread_sem_give(&group->tg_joinsem);

				/* Take the thread's thread exit semaphore.  We will sleep here
				 * until the thread exits.  We need to exercise caution because
				 * there could be multiple threads waiting here for the same
				 * pthread to exit.
				 */

				(void)pthread_sem_take(&pjoin->exit_sem, false);

				/* The thread has exited! Get the thread exit value */

				if (pexit_value) {
					*pexit_value = pjoin->exit_value;
					svdbg("exit_value=0x%p\n", pjoin->exit_value);
				}

				/* Post the thread's data semaphore so that the exiting thread
				 * will know that we have received the data.
				 */

				(void)pthread_sem_give(&pjoin->data_sem);

				/* Retake the join semaphore, we need to hold this when
				 * pthread_destroyjoin is called.
				 */

				(void)pthread_sem_take(&group->tg_joinsem, false);
			} else {
				sdbg("fail to get exit value\n");

				sched_unlock();
				pjoin->crefs--;
				(void)pthread_sem_give(&group->tg_joinsem);
				return EBUSY;
			}
		}

		/* Pre-emption is okay now. The logic still cannot be re-entered
		 * because we hold the join semaphore
		 */

		sched_unlock();

#ifdef CONFIG_SMP
		leave_critical_section(flags);
#endif

		/* Release our reference to the join structure and, if the reference
		 * count decrements to zero, deallocate the join structure.
		 */

		if (--pjoin->crefs <= 0) {
			(void)pthread_destroyjoin(group, pjoin);
		}

		(void)pthread_sem_give(&group->tg_joinsem);
		ret = OK;
	}

	return ret;
}
