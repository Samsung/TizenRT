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
 * kernel/pthread/pthread_mutexinit.c
 *
 *   Copyright (C) 2007-2009, 2011, 2016 Gregory Nutt. All rights reserved.
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
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <debug.h>

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
 * Name: pthread_mutex_init
 *
 * Description:
 *   Create a mutex
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

int pthread_mutex_init(FAR pthread_mutex_t *mutex,
		       FAR const pthread_mutexattr_t *attr)
{
	int pshared = 0;
#ifdef CONFIG_PTHREAD_MUTEX_TYPES
	uint8_t type = PTHREAD_MUTEX_DEFAULT;
#endif
#ifdef CONFIG_PRIORITY_INHERITANCE
	uint8_t proto = PTHREAD_PRIO_INHERIT;
#endif
#ifndef CONFIG_PTHREAD_MUTEX_UNSAFE
#ifdef CONFIG_PTHREAD_MUTEX_DEFAULT_UNSAFE
	uint8_t robust = PTHREAD_MUTEX_STALLED;
#else
	uint8_t robust = PTHREAD_MUTEX_ROBUST;
#endif
#endif
	int ret = OK;
	int status;

	svdbg("mutex=0x%p attr=0x%p\n", mutex, attr);

	if (!mutex) {
		ret = EINVAL;
	} else {
		/* Were attributes specified?  If so, use them */

		if (attr) {
			pshared = attr->pshared;
#ifdef CONFIG_PRIORITY_INHERITANCE
			proto = attr->proto;
#endif
#ifdef CONFIG_PTHREAD_MUTEX_TYPES
			type = attr->type;
#endif
#ifdef CONFIG_PTHREAD_MUTEX_BOTH
			robust  = attr->robust;
#endif
		}

		/* Indicate that the semaphore is not held by any thread. */

		mutex->pid = -1;

		/* Initialize the mutex like a semaphore with initial count = 1 */

		status = sem_init((sem_t *)&mutex->sem, pshared, 1);
		if (status != OK) {
			ret = get_errno();
		}
		mutex->sem.flags |= FLAGS_SEM_MUTEX;

#ifdef CONFIG_PRIORITY_INHERITANCE
		/* Initialize the semaphore protocol */

		status = sem_setprotocol((FAR sem_t *)&mutex->sem, proto);
		if (status != OK) {
			ret = get_errno();
		}
#endif

#ifndef CONFIG_PTHREAD_MUTEX_UNSAFE
		/* Initial internal fields of the mutex */
		mutex->flink  = NULL;
		mutex->flags  = (robust == PTHREAD_MUTEX_ROBUST ? _PTHREAD_MFLAGS_ROBUST : 0);
#endif

#ifdef CONFIG_PTHREAD_MUTEX_TYPES
		/* Set up attributes unique to the mutex type */

		mutex->type = type;
		mutex->nlocks = 0;
#endif
	}

	svdbg("Returning %d\n", ret);
	return ret;
}
