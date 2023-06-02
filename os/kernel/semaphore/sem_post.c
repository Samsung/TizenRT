/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
 * kernel/semaphore/sem_post.c
 *
 *   Copyright (C) 2007-2009, 2012-2013 Gregory Nutt. All rights reserved.
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

#include <limits.h>
#include <semaphore.h>
#include <errno.h>
#include <sched.h>
#include <tinyara/arch.h>
#include <tinyara/sched.h>
#include <tinyara/mm/mm.h>

#include "sched/sched.h"
#include "semaphore/semaphore.h"

#ifdef CONFIG_SEMAPHORE_HISTORY
#include <tinyara/debug/sysdbg.h>
#endif

/****************************************************************************
 * Definitions
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
void sem_unblock_task(sem_t *sem, struct tcb_s *htcb)
{
	struct tcb_s *stcb = NULL;
#ifdef SAVE_SEM_HOLDER
	struct semholder_s *pholder = NULL;
#endif

#ifdef CONFIG_SEMAPHORE_HISTORY
	save_semaphore_history(sem, (void *)this_task(), SEM_RELEASE);
#endif
#ifdef CONFIG_PRIORITY_INHERITANCE
	/* Don't let any unblocked tasks run until we complete any priority
	 * restoration steps.  Interrupts are disabled, but we do not want
	 * the head of the read-to-run list to be modified yet.
	 *
	 * NOTE: If this sched_lock is called from an interrupt handler, it
	 * will do nothing.
	 */

	sched_lock();
#endif
	/* If the result of of semaphore unlock is non-positive, then
	 * there must be some task waiting for the semaphore.
	 */

	if (sem->semcount <= 0) {
		/* Check if there are any tasks in the waiting for semaphore
		 * task list that are waiting for this semaphore. This is a
		 * prioritized list so the first one we encounter is the one
		 * that we want.
		 */

		for (stcb = (FAR struct tcb_s *)g_waitingforsemaphore.head; (stcb && stcb->waitsem != sem); stcb = stcb->flink) ;

		if (stcb) {
			sem_addholder_tcb(stcb, sem);

			/* It is, let the task take the semaphore */

			stcb->waitsem = NULL;

#ifdef CONFIG_SEMAPHORE_HISTORY
			save_semaphore_history(sem, (void *)stcb, SEM_ACQUIRE);
#endif
			/* Restart the waiting task. */

			up_unblock_task(stcb);
		}
	}

#ifdef SAVE_SEM_HOLDER
	/* Check if we need to drop the priority of any threads holding
	 * this semaphore.  The priority could have been boosted while they
	 * held the semaphore.
	 */

#ifdef CONFIG_PRIORITY_INHERITANCE
	if ((sem->flags & PRIOINHERIT_FLAGS_DISABLE) == 0) {
		sem_restorebaseprio(stcb, htcb, sem);
	} else {
#endif
		/* Free a semaphore holder directly. */
		pholder = sem_findholder(sem, htcb);
		if (pholder) {
			sem_freeholder(sem, pholder);
		}
#ifdef CONFIG_PRIORITY_INHERITANCE
	}

	sched_unlock();
#endif
#endif

}

/****************************************************************************
 * Name: sem_post
 *
 * Description:
 *   When a task has finished with a semaphore, it will call sem_post().
 *   This function unlocks the semaphore referenced by sem by performing the
 *   semaphore unlock operation on that semaphore.
 *
 *   If the semaphore value resulting from this operation is positive, then
 *   no tasks were blocked waiting for the semaphore to become unlocked; the
 *   semaphore is simply incremented.
 *
 *   If the value of the semaphore resulting from this operation is zero,
 *   then one of the tasks blocked waiting for the semaphore shall be
 *   allowed to return successfully from its call to sem_wait().
 *
 * Parameters:
 *   sem - Semaphore descriptor
 *
 * Return Value:
 *   0 (OK) or -1 (ERROR) if unsuccessful
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/

int sem_post(FAR sem_t *sem)
{
	irqstate_t saved_state;
	int ret = ERROR;
	size_t caller_retaddr = 0;
	ARCH_GET_RET_ADDRESS(caller_retaddr);

	/* Make sure we were supplied with a valid semaphore. */

	if (sem && ((sem->flags & FLAGS_INITIALIZED) != 0)) {
		/* The following operations must be performed with interrupts
		 * disabled because sem_post() may be called from an interrupt
		 * handler.
		 */

		saved_state = enter_critical_section();

		/* Perform the semaphore unlock operation. */
		ASSERT_INFO(sem->semcount < SEM_VALUE_MAX, "sem = 0x%x, caller address = 0x%x", sem, caller_retaddr);
		sem_releaseholder(sem, this_task());
		sem->semcount++;

		sem_unblock_task(sem, this_task());
		ret = OK;

		/* Interrupts may now be enabled. */

		leave_critical_section(saved_state);
	} else {
		set_errno(EINVAL);
	}

	return ret;
}
