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
 * wqueue/kwqueue/kwork_hpthread.c
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

/* The state of the kernel mode, high priority work queue. */

static struct hp_wqueue_s g_hpwork;

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: work_hpthread
 *
 * Description:
 *   This is the worker thread that performs the actions placed on the high
 *   priority work queue.
 *
 *   This, along with the lower priority worker thread(s) are the kernel
 *   mode work queues (also build in the flat build).  One of these threads
 *   also performs periodic garbage collection (that would otherwise be
 *   performed by the idle thread if CONFIG_SCHED_WORKQUEUE is not defined).
 *   That will be the higher priority worker thread only if a lower priority
 *   worker thread is available.
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

static int work_hpthread(int argc, char *argv[])
{
	/* Loop forever */

	for (;;) {
#ifndef CONFIG_SCHED_LPWORK
		/* First, perform garbage collection.  This cleans-up memory
		 * de-allocations that were queued because they could not be freed in
		 * that execution context (for example, if the memory was freed from
		 * an interrupt handler).
		 *
		 * NOTE: If the work thread is disabled, this clean-up is performed by
		 * the IDLE thread (at a very, very low priority).  If the low-priority
		 * work thread is enabled, then the garbage collection is done on that
		 * thread instead.
		 */

		sched_garbagecollection();
#endif

		/* Then process queued work.  work_process will not return until: (1)
		 * there is no further work in the work queue, and (2) the polling
		 * period provided by g_hpwork.delay expires.
		 */

		work_process((FAR struct wqueue_s *)&g_hpwork, 0);
	}

	return OK;					/* To keep some compilers happy */
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

struct hp_wqueue_s *get_hpwork(void)
{
	return &g_hpwork;
}

/****************************************************************************
 * Name: work_hpstart
 *
 * Description:
 *   Start the high-priority, kernel-mode work queue.
 *
 * Input parameters:
 *   None
 *
 * Returned Value:
 *   The task ID of the worker thread is returned on success.  A negated
 *   errno value is returned on failure.
 *
 ****************************************************************************/

int work_hpstart(void)
{
	int pid;

	/* Initialize work queue data structures */

	dq_init(&g_hpwork.q);

	/* Start the high-priority, kernel mode worker thread */

	svdbg("Starting high-priority kernel worker thread\n");

	pid = kernel_thread(HPWORKNAME, CONFIG_SCHED_HPWORKPRIORITY, CONFIG_SCHED_HPWORKSTACKSIZE, (main_t)work_hpthread, (FAR char *const *)NULL);

	DEBUGASSERT(pid > 0);
	if (pid < 0) {
		int errcode = errno;
		DEBUGASSERT(errcode > 0);

		slldbg("kernel_thread failed: %d\n", errcode);
		return -errcode;
	}

	g_hpwork.worker[0].pid = pid;
	g_hpwork.worker[0].busy = true;
	return pid;
}
