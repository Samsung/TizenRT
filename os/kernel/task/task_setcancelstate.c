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
 * kernel/task/task_setcancelstate.c
 *
 *   Copyright (C) 2007, 2008, 2016 Gregory Nutt. All rights reserved.
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
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>

#include "sched/sched.h"
#include "task/task.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: task_setcancelstate
 *
 * Description:
 *   The task_setcancelstate() function atomically both sets the calling
 *   task's cancelability state to the indicated state and returns the
 *   previous cancelability state at the location referenced by oldstate.
 *   Legal values for state are TASK_CANCEL_ENABLE and TASK_CANCEL_DISABLE.
 *
 *   The cancelability state and type of any newly created tasks are
 *   TASK_CANCEL_ENABLE and TASK_CANCEL_DEFERRED respectively.
 *
 * Input Parameters:
 *   state    - the new cancellability state, either TASK_CANCEL_ENABLE or
 *              TASK_CANCEL_DISABLE
 *   oldstate - The location to return the old cancellability state.
 *
 * Returned Value:
 *   Zero (OK) on success; ERROR is returned on any failure with the
 *   errno value set appropriately.
 *
 ****************************************************************************/

int task_setcancelstate(int state, FAR int *oldstate)
{
	FAR struct tcb_s *tcb = this_task();
	irqstate_t flags;
	bool do_exit = false;
	int ret = OK;

	/* Suppress context changes for a bit so that the flags are stable. (the
	 * flags should not change in interrupt handling).
	 */

	flags = enter_critical_section();
	sched_lock();

	/* Return the current state if so requrested */

	if (oldstate != NULL) {
		if ((tcb->flags & TCB_FLAG_NONCANCELABLE) != 0) {
			*oldstate = TASK_CANCEL_DISABLE;
		} else {
			*oldstate = TASK_CANCEL_ENABLE;
		}
	}

	/* Set the new cancellation state */

	if (state == TASK_CANCEL_ENABLE) {
		/* Clear the non-cancelable flag */

		tcb->flags &= ~TCB_FLAG_NONCANCELABLE;

		/* Check if a cancellation was pending */

		if ((tcb->flags & TCB_FLAG_CANCEL_PENDING) != 0) {
#ifdef CONFIG_CANCELLATION_POINTS
			/* If we are using deferred cancellation? */

			if ((tcb->flags & TCB_FLAG_CANCEL_DEFERRED) != 0) {
			/* Yes.. If we are within a cancellation point, then
			 * notify of the cancellation.
			 */

				if (tcb->cpcount > 0) {
					notify_cancellation(tcb);
				}
			} else
#endif
			{
				/* No.. We are using asynchronous cancellation.  If the
				 * cancellation was pending in this case, then just exit.
				 *
				 * Acting on it has to wait until the locks taken above
				 * have been released: the exit calls never return, and
				 * the termination sequence may block, which is not
				 * allowed inside the critical section.
				 */

				tcb->flags &= ~TCB_FLAG_CANCEL_PENDING;
				do_exit = true;
			}
		}
	} else if (state == TASK_CANCEL_DISABLE) {
		/* A cancellation may already be committed for this thread:
		 * pthread_cancel() or task_delete() observed it as cancelable and
		 * is in the middle of terminating it.  Becoming non-cancelable now
		 * would let the thread enter a critical region (e.g. take a
		 * filesystem lock) that the imminent termination would strand, so
		 * act on the cancellation right here instead, once the locks taken
		 * above have been released.
		 */

		if ((tcb->flags & TCB_FLAG_CANCEL_DOOMED) != 0) {
			/* Clear the doomed mark and make the thread non-cancelable
			 * before exiting: the exit sequence itself re-enters this
			 * function (e.g. task_flushstreams() -> write() -> the
			 * filesystem semtake helpers), and the mark must not trigger
			 * a second exit from within the first one.
			 */

			tcb->flags &= ~TCB_FLAG_CANCEL_DOOMED;
			tcb->flags |= TCB_FLAG_NONCANCELABLE;
			do_exit = true;
		} else {
			/* Set the non-cancelable state */

			tcb->flags |= TCB_FLAG_NONCANCELABLE;
		}
	} else {
		set_errno(EINVAL);
		ret = ERROR;
	}

	sched_unlock();
	leave_critical_section(flags);

	if (do_exit) {
		/* Act on the cancellation now that every lock taken above has been
		 * released.  Neither call returns.
		 */

#ifndef CONFIG_DISABLE_PTHREAD
		if ((tcb->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD) {
			pthread_exit(PTHREAD_CANCELED);
		} else
#endif
		{
			exit(EXIT_FAILURE);
		}
	}

	return ret;
}
