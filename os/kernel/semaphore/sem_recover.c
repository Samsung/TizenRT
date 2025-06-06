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
 * kernel/semaphore/sem_recover.c
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

#include <tinyara/arch.h>
#include <tinyara/sched.h>

#include "semaphore/semaphore.h"

#ifdef CONFIG_SEMAPHORE_HISTORY
#include <tinyara/debug/sysdbg.h>
#endif

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
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sem_recover
 *
 * Description:
 *   This function is called from task_recover() when a task is deleted via
 *   task_delete() or via pthread_cancel().  It current only checks on the
 *   case where a task is waiting for semaphore at the time that is was
 *   killed.
 *
 *   REVISIT:  A more complete implementation would release counts on all
 *   semaphores held by the thread.  That would, however, require some
 *   significant extension to the semaphore data structures because given
 *   only the task, there is not mechanism to traverse all of the semaphores
 *   with counts held by the task.
 *
 * Inputs:
 *   tcb - The TCB of the terminated task or thread
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   This function is called from task deletion logic in a safe context.
 *
 ****************************************************************************/

void sem_recover(FAR struct tcb_s *tcb)
{
	irqstate_t flags;

	/* The task is being deleted.  If it is waiting for a semphore, then
	 * increment the count on the semaphores.  This logic is almost identical
	 * to what you see in sem_waitirq() except that no attempt is made to
	 * restart the exiting task.
	 *
	 * NOTE:  In the case that the task is waiting we can assume: (1) That the
	 * task state is TSTATE_WAIT_SEM and (2) that the 'waitsem' in the TCB is
	 * non-null.  If we get here via pthread_cancel() or via task_delete(),
	 * then the task state should be preserved; it will be altered in other
	 * cases but in those cases waitsem should be NULL anyway (but we do not
	 * enforce that here).
	 */

	flags = enter_critical_section();
	if (tcb->task_state == TSTATE_WAIT_SEM) {
		sem_t *sem = tcb->waitsem;
		DEBUGASSERT(sem != NULL && sem->semcount < 0);

		/* Restore the correct priority of all threads that hold references
		 * to this semaphore.
		 */

		sem_canceled(tcb, sem);

		/* And increment the count on the semaphore.  This releases the count
		 * that was taken by sem_post().  This count decremented the semaphore
		 * count to negative and caused the thread to be blocked in the first
		 * place.
		 */

		sem->semcount++;

		if ((sem->flags & FLAGS_SEM_MUTEX) != 0) {
			DEBUGASSERT(sem->semcount < 2);
		}

		/* Clear the semaphore to assure that it is not reused.  But leave the
		 * state as TSTATE_WAIT_SEM.  This is necessary because this is a
		 * necessary indication that the TCB still resides in the waiting-for-
		 * semaphore list.
		 */

		tcb->waitsem = NULL;

#ifdef CONFIG_SEMAPHORE_HISTORY
		save_semaphore_history(sem, (void *)tcb, SEM_RELEASE);
#endif

	}

	leave_critical_section(flags);
}
