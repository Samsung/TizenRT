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
#include <pthread.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/sched.h>
#include <tinyara/kmalloc.h>

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
	struct pthread_tcb_s *rtcb = (struct pthread_tcb_s *)this_task();
	struct task_group_s *group = rtcb->cmn.group;
	struct pthread_key_s *new_key;
	struct pthread_key_s *cur_key;
	pthread_key_t new_key_inex = 1;

	DEBUGASSERT(group && (rtcb->cmn.flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD);

	if (!key) {
		return EINVAL;
	}

	/* Check if we have exceeded the system-defined number of keys. */

	if (group->tg_nkeys == PTHREAD_KEYS_MAX) {
		return EAGAIN;
	}

	/* Allocate new key structure */

	new_key = (struct pthread_key_s *)kmm_malloc(sizeof(struct pthread_key_s));
	if (!new_key) {
		return EAGAIN;
	}

	/* Initialize data and Save desctructor */

	new_key->data = NULL;
	new_key->destructor = destructor;

	if ((cur_key = (struct pthread_key_s *)sq_peek(&rtcb->key_list)) == NULL || cur_key->key != 0) {
		new_key->key = 0;
		sq_addfirst((sq_entry_t *)new_key, &rtcb->key_list);
	} else {
		while (sq_next(cur_key) && sq_next(cur_key)->key == new_key_inex) {
			new_key_inex++;
			cur_key = sq_next(cur_key);
		}

		new_key->key = new_key_inex;

		if (sq_next(cur_key) == NULL) {
			sq_addlast((sq_entry_t *)new_key, &rtcb->key_list);
		} else {
			sq_addafter((sq_entry_t *)cur_key, (sq_entry_t *)new_key, &rtcb->key_list);
		}
	}

	/* Return the key value and update key number */

	*key = new_key->key;

	/* Update this pthread key number */

	rtcb->nkeys++;;

	/* Update total key number of this group */

	group->tg_nkeys++;

	return OK;
}
