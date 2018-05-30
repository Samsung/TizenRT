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

#include <stdlib.h>
#include <sys/types.h>
#include <queue.h>

#include <tinyara/sched.h>
#include <tinyara/kmalloc.h>

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
 * Name: pthread_key_destroy
 *
 * Description:
 *   When pthread is finished by exit or cancel, this function executes
 *   key destructor and releases key resources.
 *
 * Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ************************************************************************/

void pthread_key_destroy(struct pthread_tcb_s *tcb)
{
	struct task_group_s *group = tcb->cmn.group;
	struct pthread_key_s *cur_key;
	struct pthread_key_s *next_key;
	int destr_count = 0;

	for (cur_key = (struct pthread_key_s *)sq_peek(&tcb->key_list); cur_key; cur_key = next_key) {

		/* Execute destructor until exceeding limitation */

		if (destr_count < PTHREAD_DESTRUCTOR_ITERATIONS) {

			/* Get key data from pthread tcb */

			if (cur_key->destructor != NULL && cur_key->data != NULL) {

				/* Execute destructor with data if they are valid */

				cur_key->destructor(cur_key->data);
				destr_count++;
			}
		}

		next_key = sq_next(cur_key);

		/* Free each key data */

		sched_kfree(cur_key);
	}

	/* Update Group total key number */

	group->tg_nkeys -= tcb->nkeys;
}
