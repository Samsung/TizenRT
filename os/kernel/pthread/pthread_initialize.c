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
 * kernel/pthread/pthread_initialize.c
 *
 *   Copyright (C) 2007-2010, 2013 Gregory Nutt. All rights reserved.
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

#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <assert.h>
#include <errno.h>

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
 * Name: pthread_initialize
 *
 * Description:
 *   This is an internal OS function called only at power-up boot time.  It
 *   no longer does anything since all of the pthread data structures have
 *   been moved into the "task group"
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

void pthread_initialize(void)
{
}

/****************************************************************************
 * Name: pthread_sem_take, pthread_sem_trytake, and pthread_sem_give
 *
 * Description:
 *   Support managed access to the private data sets.
 *
 *   REVISIT : These functions really do nothing more than match the return
 *   value of the semaphore functions (0 or -1 with errno set) to the
 *   return value of more pthread functions (0 or errno).  A better solution
 *   would be to use an internal version of the semaphore functions that
 *   return the error value in the correct form.
 *
 * Parameters:
 *  sem  - The semaphore to lock or unlock
 *
 * Return Value:
 *   0 on success or an errno value on failure.
 *
 ****************************************************************************/

int pthread_sem_take(sem_t *sem)
{
	/* Verify input parameters */

	DEBUGASSERT(sem != NULL);
	if (sem != NULL) {
		/* Take the semaphore */

		while (sem_wait(sem) != OK) {
			int errcode = get_errno();

			/* Handle the special case where the semaphore wait was
			 * awakened by the receipt of a signal.
			 */

			if (errcode != EINTR) {
				return errcode;
			}
		}
		return OK;
	} else {
		/* NULL semaphore pointer! */

		return EINVAL;
	}
}

#ifdef CONFIG_PTHREAD_MUTEX_UNSAFE
int pthread_sem_trytake(sem_t *sem)
{
	int ret = EINVAL;

	/* Verify input parameters */

	DEBUGASSERT(sem != NULL);
	if (sem != NULL) {
		/* Try to take the semaphore */

		int status = sem_trywait(sem);
		ret = status < 0 ? get_errno() : OK;
	}

	return ret;
}
#endif

int pthread_sem_give(sem_t *sem)
{
	/* Verify input parameters */

	DEBUGASSERT(sem != NULL);
	if (sem != NULL) {
		/* Give the semaphore */

		if (sem_post(sem) == OK) {
			return OK;
		} else {
			/* sem_post() reported an error */

			return get_errno();
		}
	} else {
		/* NULL semaphore pointer! */

		return EINVAL;
	}
}
