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
 * libc/sem/sem_init.c
 *
 *   Copyright (C) 2007-2009, 2011-2012, 2016 Gregory Nutt. All rights reserved.
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
#include <limits.h>
#include <semaphore.h>
#include <errno.h>

#ifdef CONFIG_SEMAPHORE_HISTORY
#include <tinyara/debug/sysdbg.h>
#endif
#if defined(CONFIG_BINMGR_RECOVERY) && defined(__KERNEL__)
#include <tinyara/semaphore.h>
#include <tinyara/arch.h>

#define is_kernel_sem(a) is_kernel_space((void *)a)
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function: sem_init
 *
 * Description:
 *   This function initializes the UNAMED semaphore sem. Following a
 *   successful call to sem_init(), the semaphore may be used in subsequent
 *   calls to sem_wait(), sem_post(), and sem_trywait().  The semaphore
 *   remains usable until it is destroyed.
 *
 *   Only sem itself may be used for performing synchronization. The result
 *   of referring to copies of sem in calls to sem_wait(), sem_trywait(),
 *   sem_post(), and sem_destroy() is undefined.
 *
 * Parameters:
 *   sem - Semaphore to be initialized
 *   pshared - Process sharing (not used)
 *   value - Semaphore initialization value
 *
 * Return Value:
 *   0 (OK), or -1 (ERROR) if unsuccessful.
 *
 * Assumptions:
 *
 ****************************************************************************/

int sem_init(FAR sem_t *sem, int pshared, unsigned int value)
{
	/* Verify that a semaphore was provided and the count is within the valid
	 * range.
	 */

	if (sem && value <= SEM_VALUE_MAX) {
		/* Initialize the semaphore count */

		sem->semcount = (int16_t)value;
#if defined(CONFIG_SEMAPHORE_HISTORY) && defined(__KERNEL__)
		save_semaphore_history(sem, (void *)NULL, SEM_INIT);
#endif

		sem->flags = FLAGS_INITIALIZED;

		/* Initialize to support priority inheritance */

#ifdef CONFIG_PRIORITY_INHERITANCE
		sem->flags &= ~(PRIOINHERIT_FLAGS_DISABLE);
#endif

#ifdef SAVE_SEM_HOLDER
#if CONFIG_SEM_PREALLOCHOLDERS > 0
		sem->hhead = NULL;
#else
		sem->holder.htcb = NULL;
		sem->holder.counts = 0;
#endif
		if (sem->semcount == 0) {
			/* The semaphore with zero value is used for signaling */
			sem->flags |= FLAGS_SIGSEM;
		}
#endif

#if defined(CONFIG_BINMGR_RECOVERY) && defined(__KERNEL__)
		/* Register semaphore in kernel region for kernel resource management */
		
		if (sem->semcount != 0 && is_kernel_sem(sem)) {
			sem_register(sem);
		}
#endif
		return OK;
	} else {
		set_errno(EINVAL);
		return ERROR;
	}
}
