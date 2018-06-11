/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

/************************************************************************
 * Included Files
 ************************************************************************/

#include <sys/types.h>
#include <sched.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>

#include <tinyara/sched.h>

#include "sched/sched.h"
#include "pthread/pthread.h"

/************************************************************************
 * Definitions
 ************************************************************************/

/************************************************************************
 * Private Type Declarations
 ************************************************************************/

/************************************************************************
 * Global Variables
 ************************************************************************/

/************************************************************************
 * Private Variables
 ************************************************************************/

/************************************************************************
 * Private Functions
 ************************************************************************/

/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name: pthread_key_delete
 *
 * Description:
 *   This POSIX function should delete a thread-specific data key
 *   previously returned by pthread_key_create().
 *
 * Parameters:
 *   key = the key to delete
 *
 * Return Value:
 *   If successful, return zero. Return error number on failure.
 *
 * Assumptions:
 *
 * POSIX Compatibility:
 *
 ************************************************************************/

int pthread_key_delete(pthread_key_t key)
{
	struct tcb_s *rtcb = this_task();
	struct task_group_s *group = rtcb->group;
	struct pthread_tcb_s *ptcb;
	struct join_s *pjoin;

	DEBUGASSERT(group);

	/* Check key validation */

	if (key < PTHREAD_KEYS_MAX && group->tg_key[key] == KEY_INUSE) {
		sched_lock();

		/* Delete destructor */

		group->tg_destructor[key] = NULL;

		/* Delete key data from all of thread in this group */

		for (pjoin = group->tg_joinhead; pjoin; pjoin = pjoin->next) {
			ptcb = (struct pthread_tcb_s *)sched_gettcb((pid_t)pjoin->thread);
			if (ptcb) {
				ptcb->key_data[key] = NULL;
			}
		}

		/* Mark this key is free */

		group->tg_key[key] = KEY_NOT_INUSE;

		sched_unlock();
		return OK;
	}

	return EINVAL;
}
