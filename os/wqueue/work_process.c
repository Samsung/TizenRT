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
 *
 *   Copyright (C) 2009-2014 Gregory Nutt. All rights reserved.
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

#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <queue.h>

#include <tinyara/clock.h>
#include <tinyara/wqueue.h>

#include <arch/irq.h>

#include "wqueue.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
#if defined(CONFIG_DEBUG_WORKQUEUE)
#if defined(CONFIG_BUILD_FLAT) || (defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__))
static worker_t cur_worker;
#endif
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#if defined(CONFIG_DEBUG_WORKQUEUE)
#if defined(CONFIG_BUILD_FLAT) || (defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__))
worker_t work_get_current(void)
{
	return cur_worker;
}
#endif
#endif

/****************************************************************************
 * Name: work_process
 *
 * Description:
 *   This is the logic that performs actions placed on any work list.  This
 *   logic is the common underlying logic to all work queues.  This logic is
 *   part of the internal implementation of each work queue; it should not
 *   be called from application level logic.
 *
 * Input parameters:
 *   wqueue - Describes the work queue to be processed
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void work_process(FAR struct wqueue_s *wqueue, int wndx)
{
	volatile FAR struct work_s *work;
	worker_t worker;
	FAR void *arg;
	clock_t elapsed;
	clock_t ctick;
	clock_t next;

	/* Then process queued work.  We need to keep interrupts disabled while
	 * we process items in the work list.
	 */

	next = 0;

#if defined(CONFIG_SCHED_USRWORK) && !defined(__KERNEL__)
	while (work_lock() < 0);
#else
	irqstate_t flags;
	flags = enter_critical_section();
#endif


	/* And check each entry in the work queue.  Since we have disabled
	 * interrupts we know:  (1) we will not be suspended unless we do
	 * so ourselves, and (2) there will be no changes to the work queue
	 */

	work = (FAR struct work_s *)wqueue->q.head;
	
	while (work) {
		
		/* Is this work ready?  It is ready if there is no delay or if
		 * the delay has elapsed. qtime is the time that the work was added
		 * to the work queue.  It will always be greater than or equal to
		 * zero.  Therefore a delay of zero will always execute immediately.
		 */

		ctick = clock();
		elapsed = ctick - work->qtime;

		if (elapsed >= work->delay) {
			/* Remove the ready-to-execute work from the list */

			(void)dq_rem((struct dq_entry_s *)work, &wqueue->q);

			/* Extract the work description from the entry (in case the work
			 * instance by the re-used after it has been de-queued).
			 */

			worker = work->worker;

			/* Check for a race condition where the work may be nullified
			 * before it is removed from the queue.
			 */

			if (worker != NULL) {
				/* Extract the work argument (before re-enabling interrupts) */

				arg = work->arg;

				/* Mark the work as no longer being queued */

				work->worker = NULL;

				/* Do the work.  Re-enable interrupts while the work is being
				 * performed... we don't have any idea how long this will take!
				 */
				
#if defined(CONFIG_SCHED_USRWORK) && !defined(__KERNEL__)
				work_unlock();
#else
				leave_critical_section(flags);
#endif
#if defined(CONFIG_DEBUG_WORKQUEUE)
#if defined(CONFIG_BUILD_FLAT) || (defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__))
				cur_worker = worker;
#endif
#endif
				worker(arg);

				/* Now, unfortunately, since we re-enabled interrupts we don't
				 * know the state of the work list and we will have to start
				 * back at the head of the list.
				 */

#if defined(CONFIG_SCHED_USRWORK) && !defined(__KERNEL__)
				while (work_lock() < 0);
#else
				flags = enter_critical_section();
#endif
				work = (FAR struct work_s *)wqueue->q.head;
			} else {
				/* Cancelled.. Just move to the next work in the list with
				 * interrupts still disabled.
				 */

				work = (FAR struct work_s *)work->dq.flink;
			}
		} else {				/* elapsed < work->delay */

			next = work->delay - elapsed;

			/* Then break at while loop due to sorted list */
			break;
		}
	}

	if (wqueue->q.head == NULL) {
#if defined(CONFIG_SCHED_USRWORK) && !defined(__KERNEL__)
		work_unlock();
#endif
		sigset_t set;
		sigemptyset(&set);
		sigaddset(&set, SIGWORK);

		/* Wait indefinitely until signalled with SIGWORK */
		wqueue->worker[wndx].busy = false;
		DEBUGVERIFY(sigwaitinfo(&set, NULL));
		wqueue->worker[wndx].busy = true;
	} else if (next > 0) {
#if defined(CONFIG_SCHED_USRWORK) && !defined(__KERNEL__)
		work_unlock();
#endif
		/* Wait awhile to check the work list.  We will wait here until
		 * either the time elapses or until we are awakened by a signal.
		 * Interrupts will be re-enabled while we wait.
		 */
		wqueue->worker[wndx].busy = false;
		usleep(next * USEC_PER_TICK);
		wqueue->worker[wndx].busy = true;
	}
#if defined(CONFIG_SCHED_USRWORK) && !defined(__KERNEL__)
	else {
		work_unlock();
	}
#else
	leave_critical_section(flags);
#endif

}
