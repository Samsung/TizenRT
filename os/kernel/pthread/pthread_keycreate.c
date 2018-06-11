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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <sched.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/sched.h>

#include "sched/sched.h"

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
 * Name: pthread_key_create
 *
 * Description:
 *   This function creates a thread-specific data key visible to all threads
 *   in the system.  Although the same key value may be used by different
 *   threads, the values bound to the key by pthread_setspecific() are
 *   maintained on a per-thread basis and persist for the life of the calling
 *   thread.
 *
 *   Upon key creation, the value NULL will be associated with the new key
 *   in all active threads.  Upon thread creation, the value NULL will be
 *   associated with all defined keys in the new thread.
 *
 * Parameters:
 *   key = A pointer to the key to create.
 *   destructor = An optional destructor() function that may be associated
 *      with each key that is invoked when a thread exits.
 *
 * Return Value:
 *   If successful, the pthread_key_create() function will store the newly
 *   created key value at *key and return zero (OK).  Otherwise, an error
 *   number will bereturned to indicate the error:
 *
 *      EAGAIN - The system lacked sufficient resources to create another
 *         thread-specific data key, or the system-imposed limit on the total
 *         number of keys pers process {PTHREAD_KEYS_MAX} has been exceeded
 *      EINVAL - Invalid argument.
 *
 * Assumptions:
 *
 * POSIX Compatibility:
 *
 ****************************************************************************/

int pthread_key_create(FAR pthread_key_t *key, pthread_destructor_t destructor)
{
	struct tcb_s *rtcb = this_task();
	struct task_group_s *group = rtcb->group;
	pthread_key_t new_key_index = 0;

	DEBUGASSERT(group);

	if (!key) {
		return EINVAL;
	}

	/* Find free key */

	sched_lock();
	do {
		if (group->tg_key[new_key_index] == KEY_NOT_INUSE) {
			/* Update destructor address */

			group->tg_destructor[new_key_index] = destructor;

			/* Update key number */

			*key = new_key_index;

			/* Mark this key allocated */

			group->tg_key[new_key_index] = KEY_INUSE;

			sched_unlock();
			return OK;
		}

		new_key_index++;
	} while (new_key_index < PTHREAD_KEYS_MAX);

	sched_unlock();
	return EAGAIN;
}
