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
 * kernel/pthread/pthread_mutexdestroy.c
 *
 *   Copyright (C) 2007-2009 Gregory Nutt. All rights reserved.
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
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sched.h>
#include <errno.h>
#include <debug.h>
#include <stdbool.h>

#include <tinyara/semaphore.h>

#include "pthread/pthread.h"

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

/****************************************************************************
 * Name: pthread_mutex_destroy
 *
 * Description:
 *   Destroy a mutex.
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

int pthread_mutex_destroy(FAR pthread_mutex_t *mutex)
{
	struct tcb_s *ptcb;
	struct join_s *pjoin = NULL;
	int ret = EINVAL;
	int status;

	svdbg("mutex=0x%p\n", mutex);
	DEBUGASSERT(mutex != NULL);

	if (mutex != NULL) {

		/* Make sure the semaphore is stable while we make the following checks */

		sched_lock();

		/* Is the semaphore available? */

		if (mutex->pid >= 0) {
			DEBUGASSERT(mutex->pid != 0);	/* < 0: available, >0 owned, ==0 error */
		

			/* No.. Verify that the PID still exists.  We may be destroying
			 * the mutex after cancelling a pthread and the mutex may have
			 * been in a bad state owned by the dead pthread.  NOTE: The
			 * following behavior is unspecified for pthread_mutex_destroy()
			 * (see pthread_mutex_consistent()).
			 *
			 * If the holding thread is still valid, then we should be able to
			 * map its PID to the underlying TCB. That is what sched_gettcb()
			 * does.
			 */

			ptcb = sched_gettcb(mutex->pid);
			if (ptcb && ((ptcb->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD)) {
				pjoin = pthread_findjoininfo(ptcb->group, ptcb->pid);
			}

			/* In some cases, pthread_mutex_destroy can be executed on middle of
			 * exiting pthread which holds mutex. The sched_releasepid() updates
			 * g_pidhash list to NULL by calling _exit() from pthread_exit().
			 * But, before calling it, pthread_exit() calls pthread_completejoin().
			 * It gives semaphore of pthread_join before executing _exit().
			 * It might cause user call pthread_mutex_destroy before updating scheduler.
			 * Checking pjoin structure and terminated element helps to check
			 * whether pthread which holds mutex is exiting or not.
			 */

			if (ptcb == NULL || \
			   (((ptcb->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD) && (pjoin == NULL || pjoin->terminated == true))) {
				/* The thread associated with the PID no longer exists */

				mutex->pid = -1;

				/* Reset the semaphore.  If threads are were on this
				 * semaphore, then this will awakened them and make
				 * destruction of the semaphore impossible here.
				 */

				status = sem_reset((FAR sem_t *)&mutex->sem, 1);
				if (status < 0) {
					ret = -status;
				}

				/* Check if the reset caused some other thread to lock the
				 * mutex.
				 */
				else if (mutex->pid != -1) {
					/* Yes.. then we cannot destroy the mutex now. */
					ret = EBUSY;
				}

				/* Destroy the underlying semaphore */

				else {
					status = sem_destroy((FAR sem_t *)&mutex->sem);
					ret = (status != OK) ? get_errno() : OK;
				}
			} else {
				ret = EBUSY;
			}
		} else {
			/* Destroy the semaphore
			 *
			 * REVISIT:  What if there are threads waiting on the semaphore?
			 * Perhaps this logic should all sem_reset() first?
			 */

			status = sem_destroy((sem_t *)&mutex->sem);
			ret = ((status != OK) ? get_errno() : OK);
		}

		sched_unlock();
	}

	svdbg("Returning %d\n", ret);
	return ret;
}
