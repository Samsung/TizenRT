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
 *   Copyright (C) 2009, 2011-2014 Gregory Nutt. All rights reserved.
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

#ifndef __INCLUDE_WQUEUE_H
#define __INCLUDE_WQUEUE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/clock.h>

#include <sys/types.h>
#include <stdint.h>
#include <semaphore.h>
#include <queue.h>
#include <stdbool.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* CONFIG_SCHED_WORKQUEUE.  Not selectable.  Set by the configuration system
 *   if either CONFIG_SCHED_HPWORK or CONFIG_SCHED_LPWORK are selected.
 * CONFIG_SCHED_HPWORK.  Create a dedicated "worker" thread to
 *   handle delayed processing from interrupt handlers.  This feature
 *   is required for some drivers but, if there are not complaints,
 *   can be safely disabled.  The worker thread also performs
 *   garbage collection -- completing any delayed memory deallocations
 *   from interrupt handlers.  If the worker thread is disabled,
 *   then that clean will be performed by the IDLE thread instead
 *   (which runs at the lowest of priority and may not be appropriate
 *   if memory reclamation is of high priority).  If CONFIG_SCHED_HPWORK
 *   is enabled, then the following options can also be used:
 * CONFIG_SCHED_HPWORKPRIORITY - The execution priority of the high-
 *   priority worker thread.  Default: 201
 * CONFIG_SCHED_HPWORKSTACKSIZE - The stack size allocated for the worker
 *   thread.  Default: 2048.
 * CONFIG_SIG_SIGWORK - The signal number that will be used to wake-up
 *   the worker thread.  Default: 17
 *
 * CONFIG_SCHED_LPWORK. If CONFIG_SCHED_LPWORK is selected then a lower-
 *   priority work queue will be created.  This lower priority work queue
 *   is better suited for more extended processing (such as file system
 *   clean-up operations)
 * CONFIG_SCHED_LPNTHREADS - The number of thread in the low-priority queue's
 *   thread pool.  Default: 1
 * CONFIG_SCHED_LPWORKPRIORITY - The minimum execution priority of the lower
 *   priority worker thread.  Default: 50
 * CONFIG_SCHED_LPWORKPRIOMAX - The maximum execution priority of the lower
 *   priority worker thread.  Default: 176
 *
 * The user-mode work queue is only available in the protected or kernel
 * builds.  This those configurations, the user-mode work queue provides the
 * same (non-standard) facility for use by applications.
 *
 * CONFIG_SCHED_USRWORK. If CONFIG_SCHED_USRWORK is also defined then the
 *   user-mode work queue will be created.
 * CONFIG_SCHED_USRWORKPRIORITY - The minimum execution priority of the lower
 *   priority worker thread.  Default: 100
 * CONFIG_SCHED_USRWORKSTACKSIZE - The stack size allocated for the lower
 *   priority worker thread.  Default: 2048.
 */

/* Is this a protected build (CONFIG_BUILD_PROTECTED=y) */

#if defined(CONFIG_BUILD_PROTECTED)

/* Yes.. kernel worker threads are not built in a kernel build when we are
 * building the user-space libraries.
 */

#ifdef __KERNEL__

#undef CONFIG_SCHED_USRWORK

#else

#undef CONFIG_SCHED_HPWORK
#undef CONFIG_SCHED_LPWORK

/* User-space worker threads are not built in a kernel build when we are
 * building the kernel-space libraries (but we still need to know that it
 * is configured).
 */

#endif

#elif defined(CONFIG_BUILD_KERNEL)
/* The kernel only build is equivalent to the kernel part of the protected
 * build.
 */

#else
/* User-space worker threads are not built in a flat build
 * (CONFIG_BUILD_PROTECTED=n && CONFIG_BUILD_KERNEL=n)
 */

#undef CONFIG_SCHED_USRWORK
#endif

#ifdef CONFIG_SCHED_WORKQUEUE

/* High priority, kernel work queue configuration ***************************/

#ifdef CONFIG_SCHED_HPWORK
#define HPWORKNAME "hpwork"
#ifndef CONFIG_SCHED_HPWORKPRIORITY
#define CONFIG_SCHED_HPWORKPRIORITY 201
#endif

#ifndef CONFIG_SCHED_HPWORKSTACKSIZE
#define CONFIG_SCHED_HPWORKSTACKSIZE CONFIG_IDLETHREAD_STACKSIZE
#endif

#endif							/* CONFIG_SCHED_HPWORK */

/* Low priority kernel work queue configuration *****************************/

#ifdef CONFIG_SCHED_LPWORK
#define LPWORKNAME "lpwork"

#ifndef CONFIG_SCHED_LPNTHREADS
#define CONFIG_SCHED_LPNTHREADS 1
#endif

#ifndef CONFIG_SCHED_LPWORKPRIORITY
#define CONFIG_SCHED_LPWORKPRIORITY 50
#endif

#ifndef CONFIG_SCHED_LPWORKPRIOMAX
#ifdef CONFIG_SCHED_HPWORK
#define CONFIG_SCHED_LPWORKPRIOMAX (CONFIG_SCHED_HPWORKPRIORITY-16)
#else
#define CONFIG_SCHED_LPWORKPRIOMAX 176
#endif
#endif

#ifdef CONFIG_SCHED_HPWORK

#if CONFIG_SCHED_LPWORKPRIORITY >= CONFIG_SCHED_HPWORKPRIORITY
#error CONFIG_SCHED_LPWORKPRIORITY >= CONFIG_SCHED_HPWORKPRIORITY
#endif
#if CONFIG_SCHED_LPWORKPRIOMAX >= CONFIG_SCHED_HPWORKPRIORITY
#error CONFIG_SCHED_LPWORKPRIOMAX >= CONFIG_SCHED_HPWORKPRIORITY
#endif
#endif

#if CONFIG_SCHED_LPWORKPRIORITY > CONFIG_SCHED_LPWORKPRIOMAX
#error CONFIG_SCHED_LPWORKPRIORITY > CONFIG_SCHED_LPWORKPRIOMAX
#endif

#ifndef CONFIG_SCHED_LPWORKSTACKSIZE
#define CONFIG_SCHED_LPWORKSTACKSIZE CONFIG_IDLETHREAD_STACKSIZE
#endif

#ifdef CONFIG_WORK_HPWORK
/* The high priority worker thread should be higher priority than the low
 * priority worker thread.
 */

#if CONFIG_SCHED_LPWORKPRIORITY > CONFIG_SCHED_HPWORKPRIORITY
#warning "The Lower priority worker thread has the higher priority"
#endif
#endif

#endif							/* CONFIG_SCHED_LPWORK */

/* User space work queue configuration **************************************/

#ifdef CONFIG_SCHED_USRWORK

#ifndef CONFIG_SCHED_USRWORKPRIORITY
#define CONFIG_SCHED_USRWORKPRIORITY 100
#endif

#ifndef CONFIG_SCHED_USRWORKSTACKSIZE
#define CONFIG_SCHED_USRWORKSTACKSIZE CONFIG_IDLETHREAD_STACKSIZE
#endif

#endif							/* CONFIG_SCHED_USRWORK */

/* Work queue IDs:
 *
 * Kernel Work Queues:
 *   HPWORK: This ID of the high priority work queue that should only be
 *     used for hi-priority, time-critical, driver bottom-half functions.
 *
 *   LPWORK: This is the ID of the low priority work queue that can be
 *     used for any purpose.  if CONFIG_SCHED_LPWORK is not defined, then
 *     there is only one kernel work queue and LPWORK == HPWORK.
 *
 * User Work Queue:
 *   USRWORK:  In the kernel phase a a kernel build, there should be no
 *     references to user-space work queues.  That would be an error.
 *     Otherwise, in a flat build, user applications will use the lower
 *     priority work queue (if there is one).
 */

#if defined(CONFIG_SCHED_USRWORK) && !defined(__KERNEL__)
/* User mode */

#define USRWORK  2				/* User mode work queue */
#define HPWORK   USRWORK		/* Redirect kernel-mode references */
#define LPWORK   USRWORK

#else
/* Kernel mode */

#define HPWORK   0				/* High priority, kernel-mode work queue */
#ifdef CONFIG_SCHED_LPWORK
#define LPWORK (HPWORK+1)		/* Low priority, kernel-mode work queue */
#else
#define LPWORK HPWORK			/* Redirect low-priority references */
#endif
#define USRWORK  LPWORK			/* Redirect user-mode references */

#endif							/* CONFIG_SCHED_USRWORK && !__KERNEL__ */

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifndef __ASSEMBLY__

/* Defines the work callback */

typedef void (*worker_t)(FAR void *arg);

/* Defines one entry in the work queue.  The user only needs this structure
 * in order to declare instances of the work structure.  Handling of all
 * fields is performed by the work APIs
 */

struct work_s {
	struct dq_entry_s dq;		/* Implements a doubly linked list */
	worker_t worker;			/* Work callback */
	FAR void *arg;				/* Callback argument */
	clock_t qtime;			/* Time work queued */
	clock_t delay;			/* Delay until work performed */
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: work_usrstart
 *
 * Description:
 *   Start the user mode work queue.
 *
 * Input parameters:
 *   None
 *
 * Returned Value:
 *   The task ID of the worker thread is returned on success.  A negated
 *   errno value is returned on failure.
 *
 ****************************************************************************/

#if defined(CONFIG_SCHED_USRWORK) && !defined(__KERNEL__)
int work_usrstart(void);
#endif

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

#ifdef CONFIG_SCHED_HPWORK
int work_hpstart(void);
#endif

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

#ifdef CONFIG_SCHED_LPWORK
int work_lpstart(void);
#endif

/****************************************************************************
 * Name: work_queue
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
 *   qid    - The work queue ID
 *   work   - The work structure to queue
 *   worker - The worker callback to be invoked.  The callback will invoked
 *            on the worker thread of execution.
 *   arg    - The argument that will be passed to the worker callback when
 *            it is invoked.
 *   delay  - Delay (in clock ticks) from the time queue until the worker
 *            is invoked. Zero means to perform the work immediately.
 *
 * Returned Value:
 *   Zero on success, a negated errno on failure
 *
 ****************************************************************************/

int work_queue(int qid, FAR struct work_s *work, worker_t worker, FAR void *arg, clock_t delay);

/****************************************************************************
 * Name: work_cancel
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
 *   Zero on success, a negated errno on failure
 *
 *   -ENOENT - There is no such work queued.
 *   -EINVAL - An invalid work queue was specified
 *
 ****************************************************************************/

int work_cancel(int qid, FAR struct work_s *work);

/****************************************************************************
 * Name: work_signal
 *
 * Description:
 *   Signal the worker thread to process the work queue now.  This function
 *   is used internally by the work logic but could also be used by the
 *   user to force an immediate re-assessment of pending work.
 *
 * Input parameters:
 *   qid    - The work queue ID
 *
 * Returned Value:
 *   Zero on success, a negated errno on failure
 *
 ****************************************************************************/

int work_signal(int qid);

/****************************************************************************
 * Name: work_available
 *
 * Description:
 *   Check if the work structure is available.
 *
 * Input parameters:
 *   work - The work queue structure to check.
 *   None
 *
 * Returned Value:
 *   true if available; false if busy (i.e., there is still pending work).
 *
 ****************************************************************************/

#define work_available(work) ((work)->worker == NULL)

/****************************************************************************
 * Name: lpwork_boostpriority
 *
 * Description:
 *   Called by the work queue client to assure that the priority of the low-
 *   priority worker thread is at least at the requested level, reqprio. This
 *   function would normally be called just before calling work_queue().
 *
 * Parameters:
 *   reqprio - Requested minimum worker thread priority
 *
 * Return Value:
 *   None
 *
 ****************************************************************************/

#if defined(CONFIG_SCHED_LPWORK) && defined(CONFIG_PRIORITY_INHERITANCE)
void lpwork_boostpriority(uint8_t reqprio);
#endif

/****************************************************************************
 * Name: lpwork_restorepriority
 *
 * Description:
 *   This function is called to restore the priority after it was previously
 *   boosted.  This is often done by client logic on the worker thread when
 *   the scheduled work completes.  It will check if we need to drop the
 *   priority of the worker thread.
 *
 * Parameters:
 *   reqprio - Previously requested minimum worker thread priority to be
 *     "unboosted"
 *
 * Return Value:
 *   None
 *
 ****************************************************************************/

#if defined(CONFIG_SCHED_LPWORK) && defined(CONFIG_PRIORITY_INHERITANCE)
void lpwork_restorepriority(uint8_t reqprio);
#endif

/****************************************************************************
 * Name: work_get_current
 *
 * Description:
 *   Get the current running worker
 ****************************************************************************/
#if defined(CONFIG_DEBUG_WORKQUEUE)
#if defined(CONFIG_BUILD_FLAT) || (defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__))
worker_t work_get_current(void);

#ifdef CONFIG_SCHED_HPWORK
#define IS_HPWORK (strncmp(this_task()->name, HPWORKNAME, sizeof(HPWORKNAME)) == 0)
#else
#define IS_HPWORK (false)
#endif

#ifdef CONFIG_SCHED_LPWORK
#define IS_LPWORK (strncmp(this_task()->name, LPWORKNAME, sizeof(HPWORKNAME)) == 0)
#else
#define IS_LPWORK (false)
#endif
#endif
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __ASSEMBLY__ */
#endif							/* CONFIG_SCHED_WORKQUEUE */
#endif							/* __INCLUDE_WQUEUE_H */
