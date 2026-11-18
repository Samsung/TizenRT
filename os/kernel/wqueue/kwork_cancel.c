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
 * sched/wqueue/kwork_cancel.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <assert.h>
#include <errno.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/list.h>
#include <tinyara/wqueue.h>

#include "wqueue/wqueue.h"

#ifdef CONFIG_SCHED_WORKQUEUE

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int work_qcancel(FAR struct kwork_wqueue_s *wqueue, bool sync,
                        FAR struct work_s *work)
{
	irqstate_t flags;
	FAR sem_t *sync_wait = NULL;
	FAR struct kworker_s *worker;
	int ret = 0;
	int wndx;

	if (wqueue == NULL || work == NULL) {
		return -EINVAL;
	}

	/* Cancelling the work is simply a matter of removing the work structure
	 * from the work queue.  This must be done with interrupts disabled because
	 * new work is typically added to the work queue from interrupt handlers.
	 */

	flags = spin_lock_irqsave(&wqueue->lock);
	worker = wq_get_worker(wqueue);

	if (!work_available(work)) {
		/* If the head of the pending queue has changed, we should reset
		 * the wqueue timer.
		 */

		if (work_remove(wqueue, work)) {
			work_timer_reset(wqueue);
		}
	} else {
		/* The work is not in the ready state: it was either never queued,
		 * has already completed, or it is currently being executed by a
		 * worker thread.  An executing work cannot be cancelled: report
		 * -EBUSY so that the caller (for instance aio_cancel()) knows the
		 * work instance is still owned by its worker and must not be torn
		 * down.  A synchronous cancel simply waits for the completion
		 * below instead.
		 */

		for (wndx = 0; wndx < wqueue->nthreads; wndx++) {
			if (worker[wndx].work == work) {
				if (!sync) {
					ret = -EBUSY;
				}

				break;
			}
		}

		if (ret == 0 && wndx == wqueue->nthreads) {
			/* There is no such work queued. */

			ret = -ENOENT;
		}
	}

	/* Note that cancel_sync can not be called in the interrupt
	 * context and the idletask context.
	 */

	if (sync) {
		pid_t pid = nxsched_gettid();

		/* Wait until the worker thread finished the work. */

		for (wndx = 0; wndx < wqueue->nthreads; wndx++) {
			if (worker[wndx].work == work && worker[wndx].pid != pid) {
				worker[wndx].wait_count++;
				sync_wait = &worker[wndx].wait;
				break;
			}
		}
	}

	spin_unlock_irqrestore(&wqueue->lock, flags);

	if (sync_wait) {
		nxsem_wait_uninterruptible(sync_wait);
	}

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: work_cancel/work_cancel_wq
 *
 * Description:
 *   Cancel previously queued work.  This removes work from the work queue.
 *   After work has been cancelled, it may be requeued by calling
 *   work_queue() again.
 *
 * Input Parameters:
 *   qid    - The work queue ID (must be HPWORK or LPWORK)
 *   wqueue - The work queue handle
 *   work   - The previously queued work structure to cancel
 *
 * Returned Value:
 *   Zero on success, a negated errno on failure
 *
 *   -ENOENT - There is no such work queued.
 *   -EINVAL - An invalid work queue was specified
 *   -EBUSY  - The work is currently executing and cannot be cancelled.
 *
 ****************************************************************************/

int work_cancel(int qid, FAR struct work_s *work)
{
	return work_qcancel(work_qid2wq(qid), false, work);
}

int work_cancel_wq(FAR struct kwork_wqueue_s *wqueue,
                   FAR struct work_s *work)
{
	return work_qcancel(wqueue, false, work);
}

/****************************************************************************
 * Name: work_cancel_sync/work_cancel_sync_wq
 *
 * Description:
 *   Blocked cancel previously queued user-mode work.  This removes work
 *   from the user mode work queue.  After work has been cancelled, it may
 *   be requeued by calling work_queue() again.
 *
 * Input Parameters:
 *   qid    - The work queue ID (must be HPWORK or LPWORK)
 *   wqueue - The work queue handle
 *   work   - The previously queued work structure to cancel
 *
 * Returned Value:
 *   Zero on success: the work was either cancelled before it started
 *   or, if it was already executing, its execution has now completed.
 *   A negated errno value is returned on any failure:
 *
 *   -ENOENT - There is no such work queued.
 *   -EINVAL - An invalid work queue was specified
 *
 ****************************************************************************/

int work_cancel_sync(int qid, FAR struct work_s *work)
{
	return work_qcancel(work_qid2wq(qid), true, work);
}

int work_cancel_sync_wq(FAR struct kwork_wqueue_s *wqueue,
                        FAR struct work_s *work)
{
	return work_qcancel(wqueue, true, work);
}

#endif /* CONFIG_SCHED_WORKQUEUE */
