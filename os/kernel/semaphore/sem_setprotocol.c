/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * kernel/semaphore/sem_setprotocol.c
 *
 *   Copyright (C) 2016 Gregory Nutt. All rights reserved.
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

#include <assert.h>
#include <stdbool.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/semaphore.h>

#include "semaphore/semaphore.h"

#ifdef CONFIG_PRIORITY_INHERITANCE

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static bool sem_has_holder(FAR sem_t *sem)
{
#if CONFIG_SEM_PREALLOCHOLDERS > 0
	return sem->hhead != NULL;
#else
	return sem->holder.htcb != NULL;
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function: sem_setprotocol
 *
 * Description:
 *    Set semaphore protocol attribute.
 *
 *    Semaphores initialized by sem_init() have priority inheritance disabled
 *    by default.  SEM_PRIO_INHERIT makes a semaphore an ownership semaphore:
 *    only a task that acquired a count may post that count.  Enabling priority
 *    inheritance does not validate the current semaphore state; callers must
 *    ensure that no task already holds a count before enabling it.
 *
 * Parameters:
 *    sem      - A pointer to the semaphore whose attributes are to be
 *               modified
 *    protocol - The new protocol to use
 *
 * Return Value:
 *   0 if successful.  Otherwise, -1 is returned and the errno value is set
 *   appropriately.
 *
 ****************************************************************************/

int sem_setprotocol(FAR sem_t *sem, int protocol)
{
	int errcode = EINVAL;
	irqstate_t flags;

	if (sem != NULL) {
		/* Serialize the holder check and protocol change with semaphore
		 * acquisition and release.
		 */

		flags = enter_critical_section();

		if ((sem->flags & FLAGS_INITIALIZED) != 0) {
			switch (protocol) {
			case SEM_PRIO_NONE:
				/* A P.I. semaphore cannot lose its ownership tracking while
				 * any task still holds a count.
				 */

				if (sem_has_holder(sem)) {
					errcode = EBUSY;
					break;
				}

				sem->flags &= ~PRIOINHERIT_FLAGS_ENABLE;
				errcode = OK;
				break;

			case SEM_PRIO_INHERIT:
				/* The caller is responsible for enabling P.I. only while no
				 * count is held.  Existing owners cannot be reconstructed.
				 */

				sem->flags |= PRIOINHERIT_FLAGS_ENABLE;
				errcode = OK;
				break;

			case SEM_PRIO_PROTECT:
				/* Not yet supported */

				errcode = ENOSYS;
				break;

			default:
				errcode = EINVAL;
				break;
			}
		}

		leave_critical_section(flags);

		if (errcode == OK) {
			return OK;
		}
	}

	set_errno(errcode);
	return ERROR;
}

#endif /* CONFIG_PRIORITY_INHERITANCE */
