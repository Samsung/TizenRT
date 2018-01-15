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

/// @file itc_semaphore.c
/// @brief Test Case Example for semaphore API
#include <tinyara/config.h>
#include <semaphore.h>
#include <pthread.h>
#include "tc_internal.h"
#include <stdio.h>
#include <sys/types.h>

#define LOOP_SIZE CONFIG_ITC_KERN_SEM_LOOP_SIZE
#define PSHARED 0
#define SEM_COUNT 3
#define SEM_DEST 1

/**
* @fn                   :itc_semaphore_sem_init_destroy_p_multitime
* @brief                :sem_destroy destroys semaphore
* @scenario             :sem init destroy multitime
* API's covered         :sem_init,sem_destroy
* Preconditions         :none
* Postconditions        :none
*/
static void itc_semaphore_sem_init_destroy_p_multitime(void)
{
	sem_t sem;
	int ret_chk;
	int exec_index;

	for (exec_index = 0; exec_index < LOOP_SIZE; exec_index++) {
		ret_chk = sem_init(&sem, PSHARED, SEM_COUNT);
		TC_ASSERT_EQ("sem_init", ret_chk, OK);

		ret_chk = sem_destroy(&sem);
		TC_ASSERT_EQ("sem_destroy", ret_chk, OK);
		TC_ASSERT_EQ("sem_destroy", sem.semcount, SEM_DEST);
	}

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: semaphore
 ****************************************************************************/

int itc_semaphore_main(void)
{
	itc_semaphore_sem_init_destroy_p_multitime();

	return 0;
}
