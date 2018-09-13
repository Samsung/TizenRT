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
#include <assert.h>

#include <tinyara/sched.h>

#include "sched/sched.h"

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
 * Name: pthread_getspecific
 *
 * Description:
 *   The pthread_getspecific() function returns the value currently
 *   bound to the specified key on behalf of the calling thread.
 *
 *   The effect of calling pthread_getspecific() with with a key value
 *   not obtained from pthread_create() or after a key has been deleted
 *   with pthread_key_delete() is undefined.
 *
 * Parameters:
 *   key = The data key to get or set
 *
 * Return Value:
 *   The function pthread_getspecific() returns the thread-specific data
 *   associated with the given key.  If no thread specific data is
 *   associated with the key, then the value NULL is returned.
 *
 * Assumptions:
 *
 * POSIX Compatibility:
 *   - Both calling pthread_setspecific() and pthread_getspecific()
 *     may be called from a thread-specific data destructor
 *     function.
 *
 ************************************************************************/

void *pthread_getspecific(pthread_key_t key)
{
	struct pthread_tcb_s *rtcb = (FAR struct pthread_tcb_s *)this_task();
	struct task_group_s *group = rtcb->cmn.group;

	DEBUGASSERT(group && (rtcb->cmn.flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD);

	/* Check key validation */

	if (key < PTHREAD_KEYS_MAX && group->tg_key[key] == KEY_INUSE) {
		/* Return the stored value. */

		return rtcb->key_data[key];
	}

	return NULL;
}
