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
 * Name: pthread_setspecific
 *
 * Description:
 *   The pthread_setspecific() function associates a thread-
 *   specific value with a key obtained via a previous call
 *   to pthread_key_create().  Different threads may bind
 *   different values to the same key.  These values are
 *   typically pointers to blocks of dynamically allocated
 *   memory that have been reserved for use by the calling
 *   thread.
 *
 *   The effect of calling pthread_setspecific() with
 *   with a key value not obtained from pthread_create() or
 *   after a key has been deleted with pthread_key_delete()
 *   is undefined.
 *
 * Parameters:
 *   key = The data key to get or set
 *   value = The value to bind to the key.
 *
 * Return Value:
 *   If successful, pthread_setspecific() will return zero (OK).
 *   Otherwise, an error number will be returned:
 *
 *      ENOMEM - Insufficient memory exists to associate
 *         the value with the key.
 *      EINVAL - The key value is invalid.
 *
 * Assumptions:
 *
 * POSIX Compatibility:
 *   int pthread_setspecific(pthread_key_t key, void *value)
 *   void *pthread_getspecific(pthread_key_t key)
 *
 *   - Both calling pthread_setspecific() and pthread_getspecific()
 *     may be called from a thread-specific data destructor
 *     function.
 *
 ****************************************************************************/

int pthread_setspecific(pthread_key_t key, FAR const void *value)
{
	struct pthread_tcb_s *rtcb = (FAR struct pthread_tcb_s *)this_task();
	struct task_group_s *group = rtcb->cmn.group;

	DEBUGASSERT(group && (rtcb->cmn.flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD);

	/* Check key validation */

	if (key < PTHREAD_KEYS_MAX && group->tg_key[key] == KEY_INUSE) {
		/* Store the data in the TCB. */

		rtcb->key_data[key] = (void *)value;

		/* Return success. */

		return OK;
	}

	return EINVAL;
}
