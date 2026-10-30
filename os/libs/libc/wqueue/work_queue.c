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

#include <sys/types.h>
#include <stdint.h>
#include <queue.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/clock.h>
#include <tinyara/wqueue.h>

#include "wqueue.h"

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

/****************************************************************************
 * Name: work_qqueue
 *
 * Description:
 *   Queue work to be performed at a later time.  All queued work will be
 *   performed on the worker thread of execution (not the caller's).
 *
 *   The work structure is allocated by caller, but completely managed by
 *   the work queue logic.  The caller should never modify the contents of
 *   the work queue structure; the caller should not call work_queue()
 *   again until either (1) the previous work has been performed and removed
 *   from the queue, or (2) work_cancel() has been called to cancel the work
 *   and remove it from the work queue.
 *
 * Input parameters:
 *   qid    - The work queue ID (index)
 *   work   - The work structure to queue
 *   worker - The worker callback to be invoked.  The callback will invoked
 *            on the worker thread of execution.
 *   arg    - The argument that will be passed to the workder callback when
 *            int is invoked.
 *   delay  - Delay (in clock ticks) from the time queue until the worker
 *            is invoked. Zero means to perform the work immediately.
 *
 * Returned Value:
 *   Zero (OK) on success, a negated errno on failure.
 *
 ****************************************************************************/

int work_qqueue(FAR struct wqueue_s *wqueue, FAR struct work_s *work, worker_t worker, FAR void *arg, clock_t delay)
{
	DEBUGASSERT(work != NULL);

	struct work_s *next_work = NULL;
	struct work_s *cur_work;
	clock_t elapsed;
	clock_t ctick;
	ctick = clock();

#if defined(CONFIG_SCHED_USRWORK) && !defined(__KERNEL__)
	while (work_lock() < 0);
#else
	irqstate_t flags;
	flags = enter_critical_section();
#endif

	/* check whether requested work is in queue list or not */
	cur_work = (struct work_s *)wqueue->q.head;
	while (cur_work != NULL) {
		if (cur_work == work) {
#if defined(CONFIG_SCHED_USRWORK) && !defined(__KERNEL__)
			work_unlock();
#else
			leave_critical_section(flags);
#endif
			return -EALREADY;
		}

		if (next_work == NULL) {
			elapsed = ctick - cur_work->qtime;
			if (cur_work->delay > elapsed && cur_work->delay - elapsed > delay) {
				next_work = cur_work;
			}
		}

		cur_work = (struct work_s *)cur_work->dq.flink;
	}

	work->worker = worker;		/* Work callback */
	work->arg = arg;		/* Callback argument */
	work->delay = delay;		/* Delay until work performed */
	work->qtime = ctick;		/* Time work queued */

	if (next_work) {
		dq_addbefore((FAR dq_entry_t *)next_work, (FAR dq_entry_t *)work, &wqueue->q);
	} else {
		dq_addlast((FAR dq_entry_t *)work, &wqueue->q);
	}
#if defined(CONFIG_SCHED_USRWORK) && !defined(__KERNEL__)
	work_unlock();
#else
	leave_critical_section(flags);
#endif

	return OK;
}
