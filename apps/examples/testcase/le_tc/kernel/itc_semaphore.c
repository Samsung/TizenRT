/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include "tc_internal.h"

#define PSHARED     0
#define LOOP_CNT    10

/**
* @fn                   :itc_semaphore_sem_wait_n_after_sem_destroy
* @brief                :sem_post, sem_wait
* @scenario             :init semaphore, then delete semaphore. After that call sem_wait
* API's covered         :sem_post, sem_wait
* Preconditions         :Semaphore should be destroyed.
* Postconditions        :NA
*/
static void itc_semaphore_sem_wait_n_after_sem_destroy(void)
{
	sem_t sem;
	int ret_chk;

	ret_chk = sem_init(&sem, PSHARED, 1);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	ret_chk = sem_destroy(&sem);
	TC_ASSERT_EQ("sem_destroy", ret_chk, OK);

	ret_chk = sem_wait(&sem);
	TC_ASSERT_NEQ("sem_wait", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_semaphore_sem_wait_post_n_after_sem_destroy
* @brief                :sem_post, sem_wait
* @scenario             :init semaphore, then delete semaphore. After that call sem_post
* API's covered         :sem_post, sem_wait
* Preconditions         :Semaphore should be destroyed.
* Postconditions        :NA
*/
static void itc_semaphore_sem_wait_post_n_after_sem_destroy(void)
{
	sem_t sem;
	int ret_chk;

	ret_chk = sem_init(&sem, PSHARED, 1);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	ret_chk = sem_wait(&sem);
	TC_ASSERT_EQ("sem_wait", ret_chk, OK);

	ret_chk = sem_destroy(&sem);
	TC_ASSERT_EQ("sem_destroy", ret_chk, OK);

	ret_chk = sem_post(&sem);
	TC_ASSERT_NEQ("sem_wait", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_semaphore_trywait_n_after_destroy
* @brief                :this tc tests sem_trywait
* @scenario             :calls sem_init, sem_destroy. Then call sem_trywait
* API's covered         :sem_trywait
* Preconditions         :none
* Postconditions        :none
*/
static void itc_semaphore_trywait_n_after_destroy(void)
{
	sem_t sem;
	int ret_chk;

	ret_chk = sem_init(&sem, PSHARED, 1);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	ret_chk = sem_destroy(&sem);
	TC_ASSERT_EQ("sem_destroy", ret_chk, OK);

	ret_chk = sem_trywait(&sem);
	TC_ASSERT_NEQ("sem_trywait", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_semaphore_sem_destroy_n_redestroy
* @brief                :sem_destroy destroys semaphore
* @scenario             :destroy sem 2 times
* API's covered         :sem_destroy
* Preconditions         :none
* Postconditions        :none
*/
static void itc_semaphore_sem_destroy_n_redestroy(void)
{
	sem_t sem;
	int ret_chk;

	ret_chk = sem_init(&sem, PSHARED, 1);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	ret_chk = sem_destroy(&sem);
	TC_ASSERT_EQ("sem_destroy", ret_chk, OK);
	TC_ASSERT_EQ("sem_destroy", sem.semcount, 1);

	ret_chk = sem_destroy(&sem);
	TC_ASSERT_EQ("sem_destroy", ret_chk, ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_semaphore_sem_init_destroy_p_multitime
* @brief                :sem_destroy destroys semaphore
* @scenario             :sem init destroy multitime
* API's covered         :sem_destroy
* Preconditions         :none
* Postconditions        :none
*/
static void itc_semaphore_sem_init_destroy_p_multitime(void)
{
	sem_t sem;
	int ret_chk;
	int index;

	for (index = 0; index <= LOOP_CNT; index++) {
		ret_chk = sem_init(&sem, PSHARED, 1);
		TC_ASSERT_EQ("sem_init", ret_chk, OK);

		ret_chk = sem_destroy(&sem);
		TC_ASSERT_EQ("sem_destroy", ret_chk, OK);
		TC_ASSERT_EQ("sem_destroy", sem.semcount, 1);
	}

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: semaphore
 ****************************************************************************/

int itc_semaphore_main(void)
{
	itc_semaphore_sem_wait_n_after_sem_destroy();
	itc_semaphore_sem_wait_post_n_after_sem_destroy();
	itc_semaphore_trywait_n_after_destroy();
	itc_semaphore_sem_destroy_n_redestroy();
	itc_semaphore_sem_init_destroy_p_multitime();

	return 0;
}
