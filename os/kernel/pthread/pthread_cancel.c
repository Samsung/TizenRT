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
/**************************************************************************
 * kernel/pthread/pthread_cancel.c
 *
 *   Copyright (C) 2007, 2009, 2013 Gregory Nutt. All rights reserved.
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
 **************************************************************************/

/**************************************************************************
 * Included Files
 **************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "sched/sched.h"
#include "task/task.h"
#include "pthread/pthread.h"

/**************************************************************************
 * Private Definitions
 **************************************************************************/

/**************************************************************************
 * Private Types
 **************************************************************************/

/**************************************************************************
 * Private Function Prototypes
 **************************************************************************/

/**************************************************************************
 * Global Variables
 **************************************************************************/

/**************************************************************************
 * Private Variables
 **************************************************************************/

/**************************************************************************
 * Private Functions
 **************************************************************************/

/**************************************************************************
 * Public Functions
 **************************************************************************/

int pthread_cancel(pthread_t thread)
{
	FAR struct pthread_tcb_s *tcb;


	/* First, make sure that the handle references a valid thread */

	if (thread == 0) {
		/* pid == 0 is the IDLE task.  Callers cannot cancel the
		 * IDLE task.
		 */

		return ESRCH;
	}

	tcb = (FAR struct pthread_tcb_s *)sched_gettcb((pid_t)thread);
	if (!tcb) {
	
		/* The pid does not correspond to any known thread.  The thread
		 * has probably already exited.
		 */

		return ESRCH;
	}

	DEBUGASSERT((tcb->cmn.flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD);

	/* Check to see if this thread has the non-cancelable bit set in its
	 * flags. Suppress context changes for a bit so that the flags are stable.
	 * (the flags should not change in interrupt handling.
	 */

	sched_lock();
	if ((tcb->cmn.flags & TCB_FLAG_NONCANCELABLE) != 0) {
		/* Then we cannot cancel the thread now.  Here is how this is
		 * supposed to work:
		 *
		 * "When cancelability is disabled, all cancels are held pending
		 *  in the target thread until the thread changes the cancelability.
		 *  When cancelability is deferred, all cancels are held pending in
		 *  the target thread until the thread changes the cancelability, calls
		 *  a function which is a cancellation point or calls pthread_testcancel(),
		 *  thus creating a cancellation point. When cancelability is asynchronous,
		 *  all cancels are acted upon immediately, interrupting the thread with its
		 *  processing."
		 */

		tcb->cmn.flags |= TCB_FLAG_CANCEL_PENDING;
		sched_unlock();
		return OK;
	}

#ifdef CONFIG_CANCELLATION_POINTS
	/* Check if this thread supports deferred cancellation */

	if ((tcb->cmn.flags & TCB_FLAG_CANCEL_DEFERRED) != 0) {
		/* If the thread is waiting at a cancellation point, then notify of the
		 * cancellation thereby waking the task up with an ECANCELED error.
		 *
		 * REVISIT: is locking the scheduler sufficent in SMP mode?
		 */

		tcb->cmn.flags |= TCB_FLAG_CANCEL_PENDING;

		if (tcb->cmn.cpcount > 0) {
			notify_cancellation(&tcb->cmn);
		}

		sched_unlock();
		return OK;
	}
#endif

	sched_unlock();

	/* Check to see if the ID refers to ourselves.. this would be the
	 * same as pthread_exit(PTHREAD_CANCELED).
	 */

	if (tcb == (FAR struct pthread_tcb_s *)this_task()) {
		pthread_exit(PTHREAD_CANCELED);
	}

#ifdef CONFIG_PTHREAD_CLEANUP
	/* Perform any stack pthread clean-up callbacks.
	 *
	 * REVISIT: In this case, the clean-up callback will execute on the
	 * thread of the caller of pthread cancel, not on the thread of
	 * the thread-to-be-canceled.  Is that an issue?  Presumably they
	 * are both within the same group and within the same process address
	 * space.
	 */

	pthread_cleanup_popall(tcb);
#endif

#if CONFIG_NPTHREAD_KEYS > 0
	pthread_key_destroy(tcb);
#endif

	/* Complete pending join operations */

	(void)pthread_completejoin((pid_t)thread, PTHREAD_CANCELED);

#ifndef CONFIG_PTHREAD_MUTEX_UNSAFE
	/* Recover any mutexes still held by the canceled thread */
	pthread_mutex_inconsistent(tcb);
#endif

	/* Then let task_terminate do the real work */

	return task_terminate((pid_t)thread, false);
}
