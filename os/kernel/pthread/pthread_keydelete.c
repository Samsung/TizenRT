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
#include <pthread.h>
#include <queue.h>
#include <errno.h>
#include <assert.h>

#include <tinyara/sched.h>
#include <tinyara/kmalloc.h>

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
	struct pthread_tcb_s *rtcb = (FAR struct pthread_tcb_s *)this_task();
	struct task_group_s *group = rtcb->cmn.group;
	struct pthread_key_s *cur_key;

	DEBUGASSERT(group && (rtcb->cmn.flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD);

	/* Check key validation */

	if (key < PTHREAD_KEYS_MAX) {

		/* Find key data */

		cur_key = (struct pthread_key_s *)pthread_key_find(rtcb, key);
		if (cur_key == NULL) {

			/* No Key in the list */

			return EINVAL;
		}

		/* Remove key from list */

		sq_rem((sq_entry_t *)cur_key, &rtcb->key_list);

		/* Free key data */

		sched_kfree(cur_key);

		/* Update pthread key number and group key number */

		rtcb->nkeys--;
		group->tg_nkeys--;

		return OK;
	}

	return EINVAL;
}
