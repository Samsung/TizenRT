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
 * kernel/semaphore/sem_holder.c
 *
 *   Copyright (C) 2009-2011, 2013 Gregory Nutt. All rights reserved.
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

#include <semaphore.h>
#include <sched.h>
#include <assert.h>
#include <debug.h>
#include <tinyara/arch.h>

#include "sched/sched.h"
#include "semaphore/semaphore.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

#ifndef CONFIG_SEM_PREALLOCHOLDERS
#define CONFIG_SEM_PREALLOCHOLDERS 0
#endif

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

typedef int (*holderhandler_t)(FAR struct semholder_s *pholder, FAR sem_t *sem, FAR void *arg);

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/* Preallocated holder structures */

#if CONFIG_SEM_PREALLOCHOLDERS > 0
static struct semholder_s g_holderalloc[CONFIG_SEM_PREALLOCHOLDERS];
static FAR struct semholder_s *g_freeholders;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sem_allocholder
 ****************************************************************************/

static inline FAR struct semholder_s *sem_allocholder(sem_t *sem, FAR struct tcb_s *htcb)
{
	FAR struct semholder_s *pholder;

	/* Check if the "built-in" holder is being used.  We have this built-in
	 * holder to optimize for the simplest case where semaphores are only
	 * used to implement mutexes.
	 */

#if CONFIG_SEM_PREALLOCHOLDERS > 0
	pholder = g_freeholders;
	if (pholder) {
		/* Remove the holder from the free list and put it into the
		 * semaphore's holder list
		 */

		g_freeholders = pholder->flink;
		pholder->flink = sem->hhead;
		sem->hhead = pholder;
	}
#else
	if (!sem->holder.htcb) {
		pholder = &sem->holder;
	}
#endif
	else {
		sdbg("Insufficient pre-allocated holders\n");
		pholder = NULL;
	}

	if (pholder != NULL) {
#ifdef CONFIG_PRIORITY_INHERITANCE
		pholder->sem = sem;
#endif
		pholder->htcb = htcb;
		pholder->counts = 0;

#ifdef CONFIG_PRIORITY_INHERITANCE
		/* Put it into the task's list of held semaphores */

		pholder->tlink = htcb->holdsem;
		htcb->holdsem = pholder;
#endif
	}

	return pholder;
}

/****************************************************************************
 * Name: sem_findorallocateholder
 ****************************************************************************/

static inline FAR struct semholder_s *sem_findorallocateholder(sem_t *sem, FAR struct tcb_s *htcb)
{
	FAR struct semholder_s *pholder = sem_findholder(sem, htcb);
	if (!pholder) {
		pholder = sem_allocholder(sem, htcb);
	}

	return pholder;
}

/****************************************************************************
 * Name: sem_freeholder
 ****************************************************************************/

void sem_freeholder(sem_t *sem, FAR struct semholder_s *pholder)
{
#if CONFIG_SEM_PREALLOCHOLDERS > 0
	FAR struct semholder_s *curr;
	FAR struct semholder_s *prev;
#endif
#ifdef CONFIG_PRIORITY_INHERITANCE
	FAR struct semholder_s **htlink;

	/* Remove the holder from the task's list of held semaphores */

	for (htlink = &pholder->htcb->holdsem; *htlink != NULL; htlink = &(*htlink)->tlink) {
		if (*htlink == pholder) {
			*htlink = pholder->tlink;
			break;
		}
	}

	pholder->tlink = NULL;
	pholder->sem = NULL;
#endif

	/* Release the holder and counts */

	pholder->htcb = NULL;
	pholder->counts = 0;

#if CONFIG_SEM_PREALLOCHOLDERS > 0
	/* Search the list for the matching holder */

	for (prev = NULL, curr = sem->hhead; curr && curr != pholder; prev = curr, curr = curr->flink) ;

	if (curr) {
		/* Remove the holder from the list */

		if (prev) {
			prev->flink = pholder->flink;
		} else {
			sem->hhead = pholder->flink;
		}

		/* And put it in the free list */

		pholder->flink = g_freeholders;
		g_freeholders = pholder;
	}
#endif
}

/****************************************************************************
 * Name: sem_foreachholder
 ****************************************************************************/

static int sem_foreachholder(FAR sem_t *sem, holderhandler_t handler, FAR void *arg)
{
	FAR struct semholder_s *pholder;
#if CONFIG_SEM_PREALLOCHOLDERS > 0
	FAR struct semholder_s *next;
#endif
	int ret = 0;

#if CONFIG_SEM_PREALLOCHOLDERS > 0
	for (pholder = sem->hhead; pholder && ret == 0; pholder = next)
#else
	pholder = &sem->holder;
#endif
	{
#if CONFIG_SEM_PREALLOCHOLDERS > 0
		/* In case this holder gets deleted */

		next = pholder->flink;
#endif
		/* The initial "built-in" container may hold a NULL holder */

		if (pholder->htcb) {
			/* Call the handler */

			ret = handler(pholder, sem, arg);
		}
	}

	return ret;
}

/****************************************************************************
 * Name: sem_recoverholders
 ****************************************************************************/

static int sem_recoverholders(FAR struct semholder_s *pholder, FAR sem_t *sem, FAR void *arg)
{
	sem_freeholder(sem, pholder);
	return 0;
}

#ifdef CONFIG_PRIORITY_INHERITANCE

/****************************************************************************
 * Name: sem_boostholderprio
 ****************************************************************************/

static int sem_boostholderprio(FAR struct semholder_s *pholder, FAR sem_t *sem, FAR void *arg)
{
	FAR struct tcb_s *htcb = (FAR struct tcb_s *)pholder->htcb;
	FAR struct tcb_s *rtcb = (FAR struct tcb_s *)arg;

	/* If the priority of the thread that is waiting for a count is less than
	 * or equal to the priority of the thread holding a count, then do nothing
	 * because the thread is already running at a sufficient priority.
	 */

	if (rtcb->sched_priority > htcb->sched_priority) {
		/* Raise the priority of the holder of the semaphore.  This
		 * cannot cause a context switch because we have preemption
		 * disabled.  The task will be marked "pending" and the switch
		 * will occur during up_block_task() processing.
		 */

		(void)sched_setpriority(htcb, rtcb->sched_priority);
	}

	return 0;
}
#endif
/****************************************************************************
 * Name: sem_verifyholder
 ****************************************************************************/

#ifdef CONFIG_DEBUG
static int sem_verifyholder(FAR struct semholder_s *pholder, FAR sem_t *sem, FAR void *arg)
{
#if 0							// Need to revisit this, but these assumptions seem to be untrue -- OR there is a bug???
	FAR struct tcb_s *htcb = (FAR struct tcb_s *)pholder->htcb;

	/* Called after a semaphore has been released (incremented), the semaphore
	 * could is non-negative, and there is no thread waiting for the count.
	 * In this case, the priority of the holder should not be boosted.
	 */

	DEBUGASSERT(htcb->sched_priority == htcb->base_priority);
#endif
	return 0;
}
#endif

/****************************************************************************
 * Name: sem_dumpholder
 ****************************************************************************/

#if defined(CONFIG_DEBUG) && defined(CONFIG_SEM_PHDEBUG)
static int sem_dumpholder(FAR struct semholder_s *pholder, FAR sem_t *sem, FAR void *arg)
{
#if CONFIG_SEM_PREALLOCHOLDERS > 0
	vdbg("  %08x: %08x %08x %04x\n", pholder, pholder->flink, pholder->htcb, pholder->counts);
#else
	vdbg("  %08x: %08x %04x\n", pholder, pholder->htcb, pholder->counts);
#endif
	return 0;
}
#endif

#ifdef CONFIG_PRIORITY_INHERITANCE

/****************************************************************************
 * Name: sem_findhighestwaiter
 *
 * Description:
 *   Return the highest priority task that is waiting for the given
 *   semaphore.  The list of all tasks waiting for a semaphore
 *   (g_waitingforsemaphore) is prioritized, so the first task whose
 *   'waitsem' matches the semaphore is the highest priority waiter.
 *
 * Parameters:
 *   sem     - The semaphore whose waiters are to be examined
 *   exclude - A task to ignore while scanning, or NULL.  This is used to
 *             skip the task that is leaving the wait (it received the count
 *             or its wait was canceled) but may still reside on the waiting
 *             list when the holder priorities are recomputed.
 *
 * Return Value:
 *   The TCB of the highest priority waiter, or NULL if there is none.
 *
 ****************************************************************************/

FAR struct tcb_s *sem_findhighestwaiter(FAR sem_t *sem, FAR struct tcb_s *exclude)
{
	FAR struct tcb_s *stcb;

	/* A semaphore with priority inheritance disabled never boosts its
	 * holders, so its waiters must not contribute to the holder priority
	 * recomputation.  Holders of such a semaphore can still be tracked (and
	 * hence appear on a task's holdsem list) when CONFIG_BINMGR_RECOVERY is
	 * enabled, so this must be checked explicitly here.
	 */

	if ((sem->flags & PRIOINHERIT_FLAGS_DISABLE) != 0) {
		return NULL;
	}

	for (stcb = (FAR struct tcb_s *)g_waitingforsemaphore.head; stcb != NULL; stcb = stcb->flink) {
		if (stcb->waitsem == sem && stcb != exclude) {
			break;
		}
	}

	return stcb;
}

/****************************************************************************
 * Name: sem_restoreholderprio
 ****************************************************************************/

static int sem_restoreholderprio(FAR struct semholder_s *pholder, FAR sem_t *sem, FAR void *arg)
{
	FAR struct tcb_s *htcb = (FAR struct tcb_s *)pholder->htcb;
	FAR struct semholder_s *hpholder;
	FAR struct tcb_s *stcb;
	int hpriority;

	/* We will attempt to restore the holder's priority to its base priority.
	 * If there is any thread with a higher priority waiting for a semaphore
	 * held by htcb, then this value will be overwritten.
	 */

	hpriority = htcb->boost_priority > htcb->base_priority ? htcb->boost_priority : htcb->base_priority;

	/* Was the priority of the holder thread boosted? If so, then drop its
	 * priority back to the correct level.  What is the correct level?
	 */

	if (htcb->sched_priority != hpriority) {
		/* Try to find the highest priority across all the threads that are
		 * waiting for any semaphore held by htcb.
		 */

		for (hpholder = htcb->holdsem; hpholder != NULL; hpholder = hpholder->tlink) {
			/* Skip a holder entry whose count has been given up: the holder
			 * no longer holds this semaphore, so its waiters must not boost
			 * the holder.  sem_releaseholder() is the only thing that
			 * decrements a holder's count, and the zero-count entry is freed
			 * shortly after in sem_restorebaseprio() -- by both the task path
			 * (sem_restorebaseprio_task) and the interrupt path
			 * (sem_restorebaseprio_irq).  A held semaphore can therefore reach
			 * a zero count only while it is the one being released here; assert
			 * that, both to document the invariant and to catch any holder left
			 * stranded on the list with no counts.
			 */

			if (hpholder->counts <= 0) {
				DEBUGASSERT(hpholder->sem == sem);
				continue;
			}

			/* Find the highest priority waiter of this semaphore, ignoring
			 * the task that is leaving the wait (arg); it may still reside
			 * on the waiting list at this point.
			 */

			stcb = sem_findhighestwaiter(hpholder->sem, (FAR struct tcb_s *)arg);
			if (stcb != NULL && stcb->sched_priority > hpriority) {
				hpriority = stcb->sched_priority;
			}
		}

		/* Apply the selected priority to the holder thread (hopefully back
		 * to the thread's base priority).
		 */

		sched_setpriority(htcb, hpriority);
	}

	return 0;
}

/****************************************************************************
 * Name: sem_restoreholderprioA
 *
 * Description:
 *   Reprioritize all holders except the currently executing task
 *
 ****************************************************************************/

static int sem_restoreholderprioA(FAR struct semholder_s *pholder, FAR sem_t *sem, FAR void *arg)
{
	FAR struct tcb_s *rtcb = this_task();
	if (pholder->htcb != rtcb) {
		return sem_restoreholderprio(pholder, sem, arg);
	}

	return 0;
}

/****************************************************************************
 * Name: sem_restoreholderprioB
 *
 * Description:
 *   Reprioritize only the currently executing task
 *
 ****************************************************************************/

static int sem_restoreholderprioB(FAR struct semholder_s *pholder, FAR sem_t *sem, FAR void *arg)
{
	FAR struct tcb_s *rtcb = this_task();
	if (pholder->htcb == rtcb) {
		(void)sem_restoreholderprio(pholder, sem, arg);
		return 1;
	}

	return 0;
}

/****************************************************************************
 * Name: sem_restorebaseprio_irq
 *
 * Description:
 *   This function is called after the an interrupt handler posts a count on
 *   the semaphore.  It will check if we need to drop the priority of any
 *   threads holding a count on the semaphore.  Their priority could have
 *   been boosted while they held the count.
 *
 * Parameters:
 *   stcb - The TCB of the task that was just started (if any).  If the
 *     post action caused a count to be given to another thread, then stcb
 *     is the TCB that received the count.  Note, just because stcb received
 *     the count, it does not mean that it it is higher priority than other
 *     threads.
 *   sem - A reference to the semaphore being posted.
 *     - If the semaphore count is <0 then there are still threads waiting
 *       for a count.  stcb should be non-null and will be higher priority
 *       than all of the other threads still waiting.
 *     - If it is ==0 then stcb refers to the thread that got the last count;
 *       no other threads are waiting.
 *     - If it is >0 then there should be no threads waiting for counts and
 *       stcb should be null.
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   The scheduler is locked.
 *
 ****************************************************************************/

static inline void sem_restorebaseprio_irq(FAR struct tcb_s *stcb, FAR sem_t *sem)
{
	/* Perform the following actions only if a new thread was given a count.
	 * The thread that received the count should be the highest priority
	 * of all threads waiting for a count from the semaphore.  So in that
	 * case, the priority of all holder threads should be dropped to the
	 * next highest pending priority.
	 */

	/* Drop the priority of all holder threads */

	(void)sem_foreachholder(sem, sem_restoreholderprio, stcb);
}

/****************************************************************************
 * Name: sem_restorebaseprio_task
 *
 * Description:
 *   This function is called after the current running task releases a
 *   count on the semaphore.  It will check if we need to drop the priority
 *   of any threads holding a count on the semaphore.  Their priority could
 *   have been boosted while they held the count.
 *
 * Parameters:
 *   stcb - The TCB of the task that was just started (if any).  If the
 *     post action caused a count to be given to another thread, then stcb
 *     is the TCB that received the count.  Note, just because stcb received
 *     the count, it does not mean that it it is higher priority than other
 *     threads.
 *   sem - A reference to the semaphore being posted.
 *     - If the semaphore count is <0 then there are still threads waiting
 *       for a count.  stcb should be non-null and will be higher priority
 *       than all of the other threads still waiting.
 *     - If it is ==0 then stcb refers to the thread that got the last count;
 *       no other threads are waiting.
 *     - If it is >0 then there should be no threads waiting for counts and
 *       stcb should be null.
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   The scheduler is locked.
 *
 ****************************************************************************/

static inline void sem_restorebaseprio_task(FAR struct tcb_s *stcb, FAR sem_t *sem)
{
	/* Perform the following actions only if a new thread was given a count.
	 * The thread that received the count should be the highest priority
	 * of all threads waiting for a count from the semaphore.  So in that
	 * case, the priority of all holder threads should be dropped to the
	 * next highest pending priority.
	 */

	/* The currently executed thread should be the lower priority
		* thread that just posted the count and caused this action.
		* However, we cannot drop the priority of the currently running
		* thread -- because that will cause it to be suspended.
		*
		* So, do this in two passes.  First, reprioritizing all holders
		* except for the running thread.
		*/

	(void)sem_foreachholder(sem, sem_restoreholderprioA, stcb);

	/* Now, find an reprioritize only the ready to run task */

	(void)sem_foreachholder(sem, sem_restoreholderprioB, stcb);
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sem_initholders
 *
 * Description:
 *   Called from sem_initialize() to set up semaphore holder information.
 *
 * Parameters:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

void sem_initholders(void)
{
#if CONFIG_SEM_PREALLOCHOLDERS > 0
	int i;

	/* Put all of the pre-allocated holder structures into the free list */

	g_freeholders = g_holderalloc;
	for (i = 0; i < (CONFIG_SEM_PREALLOCHOLDERS - 1); i++) {
		g_holderalloc[i].flink = &g_holderalloc[i + 1];
	}

	g_holderalloc[CONFIG_SEM_PREALLOCHOLDERS - 1].flink = NULL;
#endif
}

/****************************************************************************
 * Name: sem_destroyholder
 *
 * Description:
 *   Called from sem_destroyholder() to handle any holders of a semaphore
 *   when it is destroyed.
 *
 * Parameters:
 *   sem - A reference to the semaphore being destroyed
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

void sem_destroyholder(FAR sem_t *sem)
{
	irqstate_t flags = enter_critical_section();
	/* It is an error if a semaphore is destroyed while there are any holders
	 * (except perhaps the thread release the semaphore itself).  Hmmm.. but
	 * we actually have to assume that the caller knows what it is doing because
	 * could have killed another thread that is the actual holder of the semaphore.
	 * We cannot make any assumptions about the state of the semaphore or the
	 * state of any of the holder threads.
	 *
	 * So just recover any stranded holders and hope the task knows what it is
	 * doing.
	 */

#if CONFIG_SEM_PREALLOCHOLDERS > 0
	if (sem->hhead) {
		sdbg("Semaphore destroyed with holders\n");
	}
#else
	if (sem->holder.htcb) {
		sdbg("Semaphore destroyed with holder\n");
	}
#endif

	(void)sem_foreachholder(sem, sem_recoverholders, NULL);
	leave_critical_section(flags);
}

/****************************************************************************
 * Name: sem_findholder
 ****************************************************************************/

struct semholder_s *sem_findholder(sem_t *sem, FAR struct tcb_s *htcb)
{
	FAR struct semholder_s *pholder;

	/* Try to find the holder in the list of holders associated with this
	 * semaphore
	 */

#if CONFIG_SEM_PREALLOCHOLDERS > 0
	for (pholder = sem->hhead; pholder; pholder = pholder->flink)
#else
	pholder = &sem->holder;
#endif
	{
		if (pholder->htcb == htcb) {
			/* Got it! */

			return pholder;
		}
	}

	/* The holder does not appear in the list */

	return NULL;
}

/****************************************************************************
 * Name: sem_addholder_tcb
 *
 * Description:
 *   Called from sem_post() when the waiting thread obtains the semaphore.
 *
 * Parameters:
 *   htcb - TCB of the thread that just obtained the semaphore
 *   sem  - A reference to the incremented semaphore
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   Interrupts are disabled.
 *
 ****************************************************************************/

void sem_addholder_tcb(FAR struct tcb_s *htcb, FAR sem_t *sem)
{
	FAR struct semholder_s *pholder;

	/* No needs to save holder for semaphore used for signaling */
	if ((sem->flags & FLAGS_SIGSEM) != 0) {
		return;
	}

#if defined(CONFIG_PRIORITY_INHERITANCE) && !defined(CONFIG_BINMGR_RECOVERY)
	/*
	 * If priority inheritance is disabled for this thread, then do not
	 * add the holder. If there are never holders of the semaphore,
	 * the priority inheritance is effectively disabled.
	 */
	if ((sem->flags & PRIOINHERIT_FLAGS_DISABLE) == 0) {
#endif
		/* Find or allocate a container for this new holder */
		pholder = sem_findorallocateholder(sem, htcb);
		if (pholder != NULL) {
			/* Reaching the maximum means holder accounting has diverged from
			 * the semaphore state.  Catch it at the acquisition that exposes
			 * the problem, while retaining the limit check to prevent a signed
			 * 16-bit wrap in non-debug builds.
			 */

			DEBUGASSERT(pholder->counts < SEM_VALUE_MAX);
			if (pholder->counts < SEM_VALUE_MAX) {
				/* Increment the number of counts held by this holder */

				pholder->counts++;
			}
		}
#if defined(CONFIG_PRIORITY_INHERITANCE) && !defined(CONFIG_BINMGR_RECOVERY)
	}
#endif
}

/****************************************************************************
 * Name: sem_addholder
 *
 * Description:
 *   Called from sem_wait() or sem_trywait() when the calling thread
 *   obtains the semaphore
 *
 * Parameters:
 *   sem - A reference to the incremented semaphore
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   Interrupts are disabled.
 *
 ****************************************************************************/
void sem_addholder(FAR sem_t *sem)
{
	FAR struct tcb_s *rtcb = this_task();
	sem_addholder_tcb(rtcb, sem);
}

/****************************************************************************
 * Name: sem_releaseholder
 *
 * Description:
 *   Called from sem_post() after a thread releases one count on the
 *   semaphore.
 *
 * Parameters:
 *   sem - A reference to the semaphore being posted
 *   rtcb - TCB of the task posting the semaphore
 *
 * Return Value:
 *   TCB of the holder whose count was released.  This can differ from
 *   rtcb when a counting semaphore is posted by a non-holder.
 *
 * Assumptions:
 *
 ****************************************************************************/

FAR struct tcb_s *sem_releaseholder(FAR sem_t *sem, FAR struct tcb_s *rtcb)
{
	FAR struct semholder_s *pholder;
	FAR struct semholder_s *candidate = NULL;
	unsigned int total = 0;

	if ((sem->flags & FLAGS_SIGSEM) != 0) {
		/* No saved holder for semaphore used for signaling */
		return rtcb;
	}

	/* Find the container for this holder.  POSIX counting semaphores do not
	 * require the posting task to be the task that obtained the count.  If
	 * there is only one recorded holder, it is therefore also the only
	 * possible holder whose count can be released.
	 */

#if CONFIG_SEM_PREALLOCHOLDERS > 0
	for (pholder = sem->hhead; pholder != NULL; pholder = pholder->flink)
#else
	pholder = &sem->holder;
	if (pholder->htcb != NULL)
#endif
	{
		DEBUGASSERT(pholder->counts > 0);

		if (pholder->htcb == rtcb) {
			/* Decrement the counts on this holder -- the holder will be freed
			 * later in sem_restorebaseprio.
			 */

			pholder->counts--;
			return rtcb;
		}

		total++;
		candidate = pholder;
	}

	if (total == 1) {
		/* The posting task is not a holder, but the sole holder is
		 * unambiguous.  Return its TCB so sem_restorebaseprio() removes the
		 * correct zero-count entry after priority restoration.
		 */

		candidate->counts--;
		return candidate->htcb;
	}

	/* With multiple holders there is not enough information to select the
	 * released holder.  Keep the existing records rather than attributing
	 * the release to the wrong task.
	 */

	return rtcb;
}

#ifdef CONFIG_PRIORITY_INHERITANCE
/****************************************************************************
 * Name: void sem_boostpriority(sem_t *sem)
 *
 * Description:
 *
 *
 * Parameters:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

void sem_boostpriority(FAR sem_t *sem)
{
	FAR struct tcb_s *rtcb = this_task();

	/* Boost the priority of every thread holding counts on this semaphore
	 * that are lower in priority than the new thread that is waiting for a
	 * count.
	 */

	(void)sem_foreachholder(sem, sem_boostholderprio, rtcb);
}

/****************************************************************************
 * Name: sem_restorebaseprio
 *
 * Description:
 *   This function is called after the current running task releases a
 *   count on the semaphore or an interrupt handler posts a new count.  It
 *   will check if we need to drop the priority of any threads holding a
 *   count on the semaphore.  Their priority could have been boosted while
 *   they held the count.
 *
 * Parameters:
 *   stcb - The TCB of the task that was just started (if any).  If the
 *     post action caused a count to be given to another thread, then stcb
 *     is the TCB that received the count.  Note, just because stcb received
 *     the count, it does not mean that it it is higher priority than other
 *     threads.
 *   htcb - The TCB of the task which already holds the semaphore.
 *   sem - A reference to the semaphore being posted.
 *     - If the semaphore count is <0 then there are still threads waiting
 *       for a count.  stcb should be non-null and will be higher priority
 *       than all of the other threads still waiting.
 *     - If it is ==0 then stcb refers to the thread that got the last count;
 *       no other threads are waiting.
 *     - If it is >0 then there should be no threads waiting for counts and
 *       stcb should be null.
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   The scheduler is locked.
 *
 ****************************************************************************/

void sem_restorebaseprio(FAR struct tcb_s *stcb, FAR struct tcb_s *htcb, FAR sem_t *sem)
{
	FAR struct semholder_s *pholder;

	/* Check our assumptions */

	DEBUGASSERT((sem->semcount > 0 && stcb == NULL) || (sem->semcount <= 0 && stcb != NULL));

	/* Handler semaphore counts posed from an interrupt handler differently
	 * from interrupts posted from threads.  The primary difference is that
	 * if the semaphore is posted from a thread, then the poster thread is
	 * a player in the priority inheritance scheme.  The interrupt handler
	 * externally injects the new count without otherwise participating
	 * itself.
	 */

	if (stcb) {
		if (up_interrupt_context()) {
			sem_restorebaseprio_irq(stcb, sem);
		} else {
			sem_restorebaseprio_task(stcb, sem);
		}
	}

	/* If there are no tasks waiting for available counts, then all holders
	 * should be at their base priority.
	 */

#ifdef CONFIG_DEBUG
	else {
		(void)sem_foreachholder(sem, sem_verifyholder, NULL);
	}
#endif

	/* In any case, the currently executing task should have an entry in the
	 * list.  Its counts were previously decremented; if it now holds no
	 * counts, then we need to remove it from the list of holders.
	 */

	pholder = sem_findholder(sem, htcb);
	if (pholder) {
		/* When no more counts are held, remove the holder from the list.  The
		 * count was decremented in sem_releaseholder.
		 */

		if (pholder->counts <= 0) {
			sem_freeholder(sem, pholder);
		}
	}
}

/****************************************************************************
 * Name: sem_release_all
 *
 * Description:
 *   Release all semaphore holders for the task.
 *
 * Parameters:
 *   htcb - TCB of the task
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

void sem_release_all(FAR struct tcb_s *htcb)
{
	FAR struct semholder_s *pholder;

	while ((pholder = htcb->holdsem) != NULL) {
		FAR sem_t *sem = pholder->sem;

		sem_freeholder(sem, pholder);

		/* Increment the count on the semaphore to release the count that
		 * was taken by sem_wait() or sem_post().
		 */

		sem->semcount++;
	}
}
#endif
/****************************************************************************
 * Name: sem_canceled
 *
 * Description:
 *   Called from sem_waitirq() after a thread that was waiting for a semaphore
 *   count was awakened because of a signal and the semaphore wait has been
 *   cancelled.  This function restores the correct thread priority of each
 *   holder of the semaphore.
 *
 * Parameters:
 *   sem - A reference to the semaphore no longer being waited for
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifndef CONFIG_DISABLE_SIGNALS
void sem_canceled(FAR struct tcb_s *stcb, FAR sem_t *sem)
{
	/* Check our assumptions */

	DEBUGASSERT(sem->semcount <= 0);

	/* Adjust the priority of every holder as necessary */
#ifdef CONFIG_PRIORITY_INHERITANCE
	if ((sem->flags & PRIOINHERIT_FLAGS_DISABLE) == 0) {
		(void)sem_foreachholder(sem, sem_restoreholderprio, stcb);
	}
#endif
}
#endif

/****************************************************************************
 * Name: sem_enumholders
 *
 * Description:
 *   Show information about threads currently waiting on this semaphore
 *
 * Parameters:
 *   sem - A reference to the semaphore
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

#if defined(CONFIG_DEBUG) && defined(CONFIG_SEM_PHDEBUG)
void sem_enumholders(FAR sem_t *sem)
{
	(void)sem_foreachholder(sem, sem_dumpholder, NULL);
}
#endif

/****************************************************************************
 * Name: sem_nfreeholders
 *
 * Description:
 *   Return the number of available holder containers.  This is a good way
 *   to find out which threads are not calling sem_destroy.
 *
 * Parameters:
 *   sem - A reference to the semaphore
 *
 * Return Value:
 *   The number of available holder containers
 *
 * Assumptions:
 *
 ****************************************************************************/

#if defined(CONFIG_DEBUG) && defined(CONFIG_SEM_PHDEBUG)
int sem_nfreeholders(void)
{
#if CONFIG_SEM_PREALLOCHOLDERS > 0
	FAR struct semholder_s *pholder;
	int n;

	for (pholder = g_freeholders, n = 0; pholder; pholder = pholder->flink) {
		n++;
	}
	return n;
#else
	return 0;
#endif
}
#endif
