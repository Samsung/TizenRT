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
 * kernel/pthread/pthread_mutex.c
 *
 *   Copyright (C) 2007-2009 Gregory Nutt. All rights reserved.
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

#include <stdbool.h>
#include <sched.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/irq.h>
#include <tinyara/sched.h>

#include "sched/sched.h"
#include "pthread/pthread.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pthread_mutex_add
 *
 * Description:
 *   Add the mutex to the list of mutexes held by this thread.
 *
 * Parameters:
 *  mutex - The mux to be locked
 *
 * Return Value:
 *   None
 *
 ****************************************************************************/

static void pthread_mutex_add(FAR struct pthread_mutex_s *mutex)
{
	FAR struct pthread_tcb_s *rtcb = (FAR struct pthread_tcb_s *)this_task();
	irqstate_t flags;

	DEBUGASSERT(mutex->flink == NULL);

	/* Add the mutex to the list of mutexes held by this task */

	flags = enter_critical_section();
	mutex->flink = rtcb->mhead;
	rtcb->mhead = mutex;
	leave_critical_section(flags);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pthread_mutex_take
 *
 * Description:
 *   Take the pthread_mutex, waiting if necessary.  If successful, add the
 *   mutex to the list of mutexes held by this thread.
 *
 * Parameters:
 *  mutex - The mutex to be locked
 *  intr  - false: ignore EINTR errors when locking; true treat EINTR as
 *          other errors by returning the errno value
 *
 * Return Value:
 *   0 on success or an errno value on failure.
 *
 ****************************************************************************/

int pthread_mutex_take(FAR struct pthread_mutex_s *mutex, bool intr)
{
	int ret = EINVAL;

	/* Verify input parameters */

	DEBUGASSERT(mutex != NULL);
	if (mutex != NULL) {
		/* Make sure that no unexpected context switches occur */

		sched_lock();

		/* Error out if the mutex is already in an inconsistent state. */

		if ((mutex->flags & _PTHREAD_MFLAGS_INCONSISTENT) != 0) {
			ret = EOWNERDEAD;
		} else {
			/* Take semaphore underlying the mutex.  pthread_sem_take
			 * returns zero on success and a positive errno value on failure.
			 */

			ret = pthread_sem_take(&mutex->sem, intr);
			if (ret == OK) {
				/* Check if the holder of the mutex has terminated without
				 * releasing.  In that case, the state of the mutex is
				 * inconsistent and we return EOWNERDEAD.
				 */

				if ((mutex->flags & _PTHREAD_MFLAGS_INCONSISTENT) != 0) {
					ret = EOWNERDEAD;
				}

				/* Add the mutex to the list of mutexes held by this task */

				else {
					pthread_mutex_add(mutex);
				}
			}
		}

		sched_unlock();
	}

	return ret;
}

/****************************************************************************
 * Name: pthread_mutex_trytake
 *
 * Description:
 *   Try to take the pthread_mutex without waiting.  If successful, add the
 *   mutex to the list of mutexes held by this thread.
 *
 * Parameters:
 *  mutex - The mutex to be locked
 *  intr  - false: ignore EINTR errors when locking; true treat EINTR as
 *          other errors by returning the errno value
 *
 * Return Value:
 *   0 on success or an errno value on failure.
 *
 ****************************************************************************/

int pthread_mutex_trytake(FAR struct pthread_mutex_s *mutex)
{
	int ret = EINVAL;

	/* Verify input parameters */

	DEBUGASSERT(mutex != NULL);
	if (mutex != NULL) {
		/* Make sure that no unexpected context switches occur */

		sched_lock();

		/* Error out if the mutex is already in an inconsistent state. */

		if ((mutex->flags & _PTHREAD_MFLAGS_INCONSISTENT) != 0) {
			ret = EOWNERDEAD;
		} else {
			/* Try to take the semaphore underlying the mutex */

			ret = sem_trywait(&mutex->sem);
			if (ret < OK) {
				ret = get_errno();
			} else {
				/* Add the mutex to the list of mutexes held by this task */

				pthread_mutex_add(mutex);
			}
		}

		sched_unlock();
	}

	return ret;
}

/****************************************************************************
 * Name: pthread_mutex_give
 *
 * Description:
 *   Take the pthread_mutex and, if successful, add the mutex to the ist of
 *   mutexes held by this thread.
 *
 * Parameters:
 *  mutex - The mux to be unlocked
 *
 * Return Value:
 *   0 on success or an errno value on failure.
 *
 ****************************************************************************/

int pthread_mutex_give(FAR struct pthread_mutex_s *mutex)
{
	FAR struct pthread_mutex_s *curr;
	FAR struct pthread_mutex_s *prev;
	int ret = EINVAL;

	/* Verify input parameters */

	DEBUGASSERT(mutex != NULL);
	if (mutex != NULL) {
		FAR struct pthread_tcb_s *rtcb = (FAR struct pthread_tcb_s *)this_task();
		irqstate_t flags;

		flags = enter_critical_section();

		/* Remove the mutex from the list of mutexes held by this task */

		for (prev = NULL, curr = rtcb->mhead; curr != NULL && curr != mutex; prev = curr, curr = curr->flink) ;

		DEBUGASSERT(curr == mutex);

		/* Remove the mutex from the list.  prev == NULL means that the mutex
		 * to be removed is at the head of the list.
		 */

		if (prev == NULL) {
			rtcb->mhead = mutex->flink;
		} else {
			prev->flink = mutex->flink;
		}

		mutex->flink = NULL;
		leave_critical_section(flags);

		/* Now release the underlying semaphore */

		ret = pthread_sem_give(&mutex->sem);
	}

	return ret;
}

/****************************************************************************
 * Name: pthread_disable_cancel() and pthread_enable_cancel()
 *
 * Description:
 *   Temporarily disable cancellation and return old cancel state, which
 *   can later be restored.  This is useful when a cancellation point
 *   function is called from within the OS by a non-cancellation point:
 *   In certain such cases, we need to defer the cancellation to prevent
 *   bad things from happening.
 *
 * Parameters:
 *   saved cancel flags for pthread_enable_cancel()
 *
 * Return Value:
 *   old cancel flags for pthread_disable_cancel()
 *
 ****************************************************************************/

#ifdef CONFIG_CANCELLATION_POINTS
uint16_t pthread_disable_cancel(void)
{
	FAR struct pthread_tcb_s *tcb = (FAR struct pthread_tcb_s *)this_task();
	irqstate_t flags;
	uint16_t old;

	/* We need perform the following operations from within a critical section
	 * because it can compete with interrupt level activity.
	 */

	flags = enter_critical_section();
	old = tcb->cmn.flags & (TCB_FLAG_CANCEL_PENDING | TCB_FLAG_NONCANCELABLE);
	tcb->cmn.flags &= ~(TCB_FLAG_CANCEL_PENDING | TCB_FLAG_NONCANCELABLE);
	leave_critical_section(flags);
	return old;
}

void pthread_enable_cancel(uint16_t cancelflags)
{
	FAR struct pthread_tcb_s *tcb = (FAR struct pthread_tcb_s *)this_task();
	irqstate_t flags;

	/* We need perform the following operations from within a critical section
	 * because it can compete with interrupt level activity.
	 */

	flags = enter_critical_section();
	tcb->cmn.flags |= cancelflags;

	/* What should we do if there is a pending cancellation?
	 *
	 * If the thread is executing with deferred cancellation, we need do
	 * nothing more; the cancellation cannot occur until the next
	 * cancellation point.
	 *
	 * However, if the thread is executing in asynchronous cancellation mode,
	 * then we need to terminate now by simply calling pthread_exit().
	 */

	if ((tcb->cmn.flags & TCB_FLAG_CANCEL_DEFERRED) == 0 && (tcb->cmn.flags & TCB_FLAG_CANCEL_PENDING) != 0) {
		pthread_exit(NULL);
	}

	leave_critical_section(flags);
}
#endif							/* CONFIG_CANCELLATION_POINTS */
