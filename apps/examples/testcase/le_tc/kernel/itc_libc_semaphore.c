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

/// @file itc_libc_semaphore.c
/// @brief ITC Test Case Example for Libc Semaphore API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <semaphore.h>
#include "tc_internal.h"

#define ITC_PSHARED 0
#define ITC_SEM_VALUE SEM_VALUE_MAX

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * @fn                   :itc_libc_semaphore_getprotocol_n_no_sem_init
 * @brief                :sem_getprotocol function without initializing sem_t
 * @Scenario             :Return the value of the semaphore protocol attribute.
 * @API's covered        :sem_init, sem_getprotocol
 * @Preconditions        :NA
 * @Postconditions       :NA
 */
static void itc_libc_semaphore_getprotocol_n_no_sem_init(void)
{
	sem_t sem;
	int protocol;
	int ret_chk;

	// sem is not initialized
	ret_chk = sem_getprotocol(&sem, &protocol);
	TC_ASSERT_EQ("sem_getprotocol", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_getprotocol", get_errno(), EINVAL);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :itc_libc_semaphore_sem_wait_getvalue_p
 * @brief                :this tc test sem_init function
 * @Scenario             :getting value before and after sem_wait
 * API's covered         :sem_getvalue
 * Preconditions         :NA
 * Postconditions        :NA
 */
static void itc_libc_semaphore_sem_wait_getvalue_p(void)
{
	sem_t sem;
	unsigned int value = ITC_SEM_VALUE;
	int sval;
	int ret_chk;

	ret_chk = sem_init(&sem, ITC_PSHARED, value);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	ret_chk = sem_getvalue(&sem, &sval);
	TC_ASSERT_EQ("sem_getvalue", ret_chk, OK);
	TC_ASSERT_EQ("sem_getvalue", sval, value);

	ret_chk = sem_wait(&sem);
	TC_ASSERT_EQ("sem_wait", ret_chk, OK);

	ret_chk = sem_getvalue(&sem, &sval);
	TC_ASSERT_EQ("sem_getvalue", ret_chk, OK);
	TC_ASSERT_EQ("sem_getvalue", sval, value - 1);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :itc_libc_semaphore_sem_wait_post_getvalue_p
 * @brief                :this tc test sem_init function
 * @Scenario             :getting value before and after sem_wait and sem_post
 * API's covered         :sem_getvalue
 * Preconditions         :NA
 * Postconditions        :NA
 */
static void itc_libc_semaphore_sem_wait_post_getvalue_p(void)
{
	sem_t sem;
	unsigned int value = ITC_SEM_VALUE;
	int sval;
	int ret_chk;

	ret_chk = sem_init(&sem, ITC_PSHARED, value);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	ret_chk = sem_getvalue(&sem, &sval);
	TC_ASSERT_EQ("sem_getvalue", ret_chk, OK);
	TC_ASSERT_EQ("sem_getvalue", sval, value);

	ret_chk = sem_wait(&sem);
	TC_ASSERT_EQ("sem_wait", ret_chk, OK);

	ret_chk = sem_getvalue(&sem, &sval);
	TC_ASSERT_EQ("sem_getvalue", ret_chk, OK);
	TC_ASSERT_EQ("sem_getvalue", sval, value - 1);

	ret_chk = sem_post(&sem);
	TC_ASSERT_EQ("sem_post", ret_chk, OK);

	ret_chk = sem_getvalue(&sem, &sval);
	TC_ASSERT_EQ("sem_getvalue", ret_chk, OK);
	TC_ASSERT_EQ("sem_getvalue", sval, value);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_semaphore
 ****************************************************************************/

int itc_libc_semaphore_main(void)
{
	itc_libc_semaphore_getprotocol_n_no_sem_init();
	itc_libc_semaphore_sem_wait_getvalue_p();
	itc_libc_semaphore_sem_wait_post_getvalue_p();
	return 0;
}
