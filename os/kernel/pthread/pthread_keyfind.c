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
 * Name: pthread_key_find
 *
 * Description:
 *   This function returns pthread key pointer which has key value
 *   as key index from tcb's key list.
 *
 * Parameters:
 *   tcb = pthread_tcb_s structure address
 *   key = pthread key value
 *
 * Return Value:
 *   If successful, return key address. Return NULL on failure.
 *
 ************************************************************************/

struct pthread_key_s *pthread_key_find(struct pthread_tcb_s *tcb, pthread_key_t key)
{
	struct pthread_key_s *cur_key;

	if (tcb && (cur_key = (struct pthread_key_s *)sq_peek(&tcb->key_list)) != NULL) {
		do {
			if (cur_key->key == key) {
				return cur_key;
			}
		} while ((cur_key = sq_next(cur_key)) != NULL);
	}

	return NULL;
}
