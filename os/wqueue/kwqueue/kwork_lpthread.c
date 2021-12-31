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
 * wqueue/kwqueue/kwork_lpthread.c
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

#include <unistd.h>
#include <sched.h>
#include <string.h>
#include <errno.h>
#include <queue.h>
#include <debug.h>

#include <tinyara/wqueue.h>
#include <tinyara/kthread.h>
#include <tinyara/kmalloc.h>
#include <tinyara/clock.h>

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

/* The state of the kernel mode, low priority work queue(s). */

static struct lp_wqueue_s g_lpwork;

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: work_lpthread
 *
 * Description:
 *   These are the worker thread(s) that performs the actions placed on the
 *   low priority work queue.
 *
 *   These, along with the higher priority worker thread are the kernel mode
 *   work queues (also build in the flat build).  One of these threads also
 *   performs periodic garbage collection (that would otherwise be performed
 *   by the idle thread if CONFIG_SCHED_WORKQUEUE is not defined).  That will
 *   be the lower priority worker thread if it is available.
 *
 *   All kernel mode worker threads are started by the OS during normal
 *   bring up.  This entry point is referenced by OS internally and should
 *   not be accessed by application logic.
 *
 * Input parameters:
 *   argc, argv (not used)
 *
 * Returned Value:
 *   Does not return
 *
 ****************************************************************************/

static int work_lpthread(int argc, char *argv[])
{
#if CONFIG_SCHED_LPNTHREADS > 0
	int wndx;
	pid_t me = getpid();
	int i;

	/* Find out thread index by search the workers in g_lpwork */

	struct lp_wqueue_s *lwq = get_lpwork();
	for (wndx = 0, i = 0; i < CONFIG_SCHED_LPNTHREADS; i++) {
		if (lwq->worker[i].pid == me) {
			wndx = i;
			break;
		}
	}

	DEBUGASSERT(i < CONFIG_SCHED_LPNTHREADS);
#endif

	/* Loop forever */

	for (;;) {
#if CONFIG_SCHED_LPNTHREADS > 0
		/* Thread 0 is special.  Only thread 0 performs period garbage collection */

		if (wndx > 0) {
			/* The other threads will perform work, waiting indefinitely until
			 * signalled for the next work availability.
			 *
			 * The special value of zero for the poll period instructs work_process
			 * to wait indefinitely until a signal is received.
			 */

			work_process((FAR struct wqueue_s *)lwq, wndx);
		} else
#endif
		{
			/* Perform garbage collection.  This cleans-up memory de-allocations
			 * that were queued because they could not be freed in that execution
			 * context (for example, if the memory was freed from an interrupt handler).
			 * NOTE: If the work thread is disabled, this clean-up is performed by
			 * the IDLE thread (at a very, very low priority).
			 *
			 * In the event of multiple low priority threads, on index == 0 will do
			 * the garbage collection.
			 */

			sched_garbagecollection();

			/* Then process queued work.  work_process will not return until:
			 * (1) there is no further work in the work queue, and (2) the polling
			 * period provided by g_lpwork.delay expires.
			 */

			work_process((FAR struct wqueue_s *)lwq, 0);
		}
	}

	return OK;					/* To keep some compilers happy */
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

struct lp_wqueue_s *get_lpwork(void)
{
	return &g_lpwork;
}

/****************************************************************************
 * Name: work_lpstart
 *
 * Description:
 *   Start the low-priority, kernel-mode worker thread(s)
 *
 * Input parameters:
 *   None
 *
 * Returned Value:
 *   The task ID of the worker thread is returned on success.  A negated
 *   errno value is returned on failure.
 *
 ****************************************************************************/

int work_lpstart(void)
{
	int pid;
	int wndx;

	/* Initialize work queue data structures */

	struct lp_wqueue_s *lwq = get_lpwork();
	memset(lwq, 0, sizeof(struct wqueue_s));

	dq_init(&lwq->q);

	/* Don't permit any of the threads to run until we have fully initialized
	 * g_lpwork.
	 */

	sched_lock();

	/* Start the low-priority, kernel mode worker thread(s) */

	svdbg("Starting low-priority kernel worker thread(s)\n");

	for (wndx = 0; wndx < CONFIG_SCHED_LPNTHREADS; wndx++) {
		pid = kernel_thread(LPWORKNAME, CONFIG_SCHED_LPWORKPRIORITY, CONFIG_SCHED_LPWORKSTACKSIZE, (main_t)work_lpthread, (FAR char *const *)NULL);

		DEBUGASSERT(pid > 0);
		if (pid < 0) {
			int errcode = errno;
			DEBUGASSERT(errcode > 0);

			slldbg("kernel_thread %d failed: %d\n", wndx, errcode);
			sched_unlock();
			return -errcode;
		}

		lwq->worker[wndx].pid = (pid_t)pid;
		lwq->worker[wndx].busy = true;
	}

	sched_unlock();
	return lwq->worker[0].pid;
}
