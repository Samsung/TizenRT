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
 * kernel/wqueue/kwork_cancel.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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

#include <queue.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/wqueue.h>

#include "wqueue/wqueue.h"

#ifdef CONFIG_SCHED_WORKQUEUE

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#if defined(CONFIG_SCHED_HPWORK) || defined(CONFIG_SCHED_LPWORK)
/****************************************************************************
 * Name: work_qcancel
 *
 * Description:
 *   Cancel previously queued work.  This removes work from the work queue.
 *   After work has been cancelled, it may be re-queue by calling work_queue()
 *   again.
 *
 * Input parameters:
 *   qid    - The work queue ID
 *   work   - The previously queue work structure to cancel
 *
 * Returned Value:
 *   Zero (OK) on success, a negated errno on failure.  This error may be
 *   reported:
 *
 *   -ENOENT - There is no such work queued.
 *   -EINVAL - An invalid work queue was specified
 *
 ****************************************************************************/

static int work_qcancel(FAR struct kwork_wqueue_s *wqueue, FAR struct work_s *work)
{
	struct work_s *cur_work;
	irqstate_t flags;
	int ret = -ENOENT;

	DEBUGASSERT(work != NULL);

	/* Cancelling the work is simply a matter of removing the work structure
	 * from the work queue.  This must be done with interrupts disabled because
	 * new work is typically added to the work queue from interrupt handlers.
	 */

	flags = irqsave();
	if (work->worker != NULL) {
		/* A little test of the integrity of the work queue */

		DEBUGASSERT(work->dq.flink || (FAR dq_entry_t *)work == wqueue->q.tail);
		DEBUGASSERT(work->dq.blink || (FAR dq_entry_t *)work == wqueue->q.head);

		/* check whether requested work is in queue list or not */
		cur_work = (struct work_s *)wqueue->q.head;
		do {
			if (cur_work == NULL) {
				irqrestore(flags);
				return -ENOENT;
			} else if (cur_work == work) {
				break;
			}

			cur_work = (struct work_s *)cur_work->dq.flink;
		} while (1);

		/* Remove the entry from the work queue and make sure that it is
		 * mark as available (i.e., the worker field is nullified).
		 */

		dq_rem((FAR dq_entry_t *)work, &wqueue->q);
		work->worker = NULL;
		ret = OK;
	}

	irqrestore(flags);
	return ret;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: work_cancel
 *
 * Description:
 *   Cancel previously queued user-mode work.  This removes work from the
 *   user mode work queue.  After work has been cancelled, it may be re-queue
 *   by calling work_queue() again.
 *
 * Input parameters:
 *   qid    - The work queue ID (must be HPWORK or LPWORK)
 *   work   - The previously queue work structure to cancel
 *
 * Returned Value:
 *   Zero (OK) on success, a negated errno on failure.  This error may be
 *   reported:
 *
 *   -ENOENT - There is no such work queued.
 *   -EINVAL - An invalid work queue was specified
 *
 ****************************************************************************/

int work_cancel(int qid, FAR struct work_s *work)
{
#ifdef CONFIG_SCHED_HPWORK
	if (qid == HPWORK) {
		/* Cancel high priority work */

		return work_qcancel((FAR struct kwork_wqueue_s *)&g_hpwork, work);
	} else
#endif
#ifdef CONFIG_SCHED_LPWORK
		if (qid == LPWORK) {
			/* Cancel low priority work */

			return work_qcancel((FAR struct kwork_wqueue_s *)&g_lpwork, work);
		} else
#endif
		{
			return -EINVAL;
		}
}

#endif							/* CONFIG_SCHED_WORKQUEUE */
