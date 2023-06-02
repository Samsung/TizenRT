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
 * kernel/signal/sig_deliver.c
 *
 *   Copyright (C) 2007, 2008, 2012-2013 Gregory Nutt. All rights reserved.
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
#include <signal.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>
#include <debug.h>
#include <tinyara/arch.h>

#include "semaphore/semaphore.h"
#include "signal/signal.h"

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
 * Name: sig_deliver
 *
 * Description:
 *   This function is called on the thread of execution of the signal
 *   receiving task.  It processes all queued signals then returns.
 *
 ****************************************************************************/

void sig_deliver(FAR struct tcb_s *stcb)
{
	FAR sigq_t *sigq;
	FAR sigq_t *next;
	sigset_t savesigprocmask;
	irqstate_t saved_state;
	int saved_errno;

	sched_lock();

	/* Save the thread errno.  When we finished dispatching the
	 * signal actions and resume the task, the errno value must
	 * be unchanged by the operation of the signal handling.  In
	 * particular, the EINTR indication that says that the task
	 * was reawakened by a signal must be retained.
	 */

	saved_errno = stcb->pterrno;
	for (sigq = (FAR sigq_t *)stcb->sigpendactionq.head; (sigq); sigq = next) {
		next = sigq->flink;
		svdbg("Sending signal sigq=0x%x\n", sigq);

		/* Remove the signal structure from the sigpendactionq and place it
		 * in the sigpostedq.  NOTE:  Since signals are processed one at a
		 * time, there should never be more than one signal in the sigpostedq
		 */

		saved_state = enter_critical_section();
		sq_rem((FAR sq_entry_t *)sigq, &(stcb->sigpendactionq));
		sq_addlast((FAR sq_entry_t *)sigq, &(stcb->sigpostedq));
		leave_critical_section(saved_state);

		/* Call the signal handler (unless the signal was cancelled)
		 *
		 * Save a copy of the old sigprocmask and install the new
		 * (temporary) sigprocmask.  The new sigprocmask is the union
		 * of the current sigprocmask and the sa_mask for the signal being
		 * delivered plus the signal being delivered.
		 */

		savesigprocmask = stcb->sigprocmask;
		stcb->sigprocmask = savesigprocmask | sigq->mask | SIGNO2SET(sigq->info.si_signo);
#ifdef HAVE_GROUP_MEMBERS
		/* Turn on the mask for checking which signo is blocked for handling the signal. */
		stcb->sigrecvmask |= SIGNO2SET(sigq->info.si_signo);
#endif

		/* Deliver the signal.  In the kernel build this has to be handled
		 * differently if we are dispatching to a signal handler in a user-
		 * space task or thread; we have to switch to user-mode before
		 * calling the task.
		 */

		if (sigq->info.si_signo == SIGKILL) {
#ifdef CONFIG_SIGKILL_HANDLER
			if (stcb->sigkillusrhandler != NULL) {
#if defined(CONFIG_BUILD_PROTECTED) || defined(CONFIG_BUILD_KERNEL)
				if ((stcb->flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_KERNEL) {
					siginfo_t info;
					memcpy(&info, &sigq->info, sizeof(siginfo_t));

					up_signal_dispatch(stcb->sigkillusrhandler, sigq->info.si_signo, &info, NULL);
				} else
#endif
				{
					stcb->sigkillusrhandler(sigq->info.si_signo, &sigq->info, NULL);
				}
			}
#endif
			(*sigq->action.sighandler)(0, NULL, NULL);
		} else
#if defined(CONFIG_BUILD_PROTECTED) || defined(CONFIG_BUILD_KERNEL)
		if ((stcb->flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_KERNEL) {
			/* The sigq_t pointed to by sigq resides in kernel space.  So we
			 * cannot pass a reference to sigq->info to the user application.
			 * Instead, we will copy the siginfo_t structure onto the stack.
			 * We are currently executing on the stack of the user thread
			 * (albeit temporarily in kernel mode), so the copy of the
			 * siginfo_t structure will be accessible by the user thread.
			 */

			siginfo_t info;
			memcpy(&info, &sigq->info, sizeof(siginfo_t));

			up_signal_dispatch(sigq->action.sighandler, sigq->info.si_signo, &info, NULL);
		} else
#endif
		{
			/* The kernel thread signal handler is much simpler. */

			(*sigq->action.sighandler)(sigq->info.si_signo, &sigq->info, NULL);
		}

		/* Restore the original sigprocmask */

		stcb->sigprocmask = savesigprocmask;
#ifdef HAVE_GROUP_MEMBERS
		/* Turn off the checking mask. */
		stcb->sigrecvmask &= ~SIGNO2SET(sigq->info.si_signo);
#endif

		/* Now, handle the (rare?) case where (a) a blocked signal was
		 * received while the signal handling executed but (b) restoring the
		 * original sigprocmask will unblock the signal.
		 */

		sig_unmaskpendingsignal();

		/* Remove the signal from the sigpostedq */

		saved_state = enter_critical_section();
		sq_rem((FAR sq_entry_t *)sigq, &(stcb->sigpostedq));
		leave_critical_section(saved_state);

		/* Then deallocate it */

		sig_releasependingsigaction(sigq);
	}

	stcb->pterrno = saved_errno;
	sched_unlock();
}
