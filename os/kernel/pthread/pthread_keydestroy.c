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

#include <tinyara/sched.h>

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
	pthread_key_t key_index;
	int destr_count = 0;

	for (key_index = 0; key_index < PTHREAD_KEYS_MAX; key_index++) {
		if (tcb->key_data[key_index] != NULL && group->tg_destructor[key_index] != NULL) {
			/* Execute destructor with data if they are valid */

			group->tg_destructor[key_index](tcb->key_data[key_index]);

			/* Execute destructor until exceeding limitation */

			destr_count++;
			if (destr_count >= PTHREAD_DESTRUCTOR_ITERATIONS) {
				return;
			}
		}
	}
}
