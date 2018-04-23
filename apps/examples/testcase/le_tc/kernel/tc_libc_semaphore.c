/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file tc_libc_semaphore.c
/// @brief Test Case Example for Libc Semaphore API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/types.h>
#include "tc_internal.h"

#define SEM_VALUE SEM_VALUE_MAX
#define PSHARED 0

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * @fn                   :tc_libc_semaphore_sem_init
 * @brief                :this tc test sem_init function
 * @Scenario             :If sem is NULL or sem value is bigger then SEM_VALUE_MAX, it return ERROR and errno EINVAL is set.
 *                        Else sem is intialized to the default value.
 * API's covered         :sem_init
 * Preconditions         :NA
 * Postconditions        :NA
 * @return               :total_pass on success.
 */
static void tc_libc_semaphore_sem_init(void)
{
	sem_t sem;
	unsigned int value = SEM_VALUE;
	int ret_chk;
#ifdef CONFIG_PRIORITY_INHERITANCE
	uint8_t sem_flag;
#endif

	ret_chk = sem_init(NULL, PSHARED, 0);
	TC_ASSERT_EQ("sem_init", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_init", get_errno(), EINVAL);

	ret_chk = sem_init(&sem, PSHARED, SEM_VALUE_MAX + 1);
	TC_ASSERT_EQ("sem_init", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_init", get_errno(), EINVAL);

	ret_chk = sem_init(&sem, PSHARED, value);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);
	TC_ASSERT_EQ("sem_init", sem.semcount, value);
#ifdef CONFIG_PRIORITY_INHERITANCE
	sem_flag = FLAGS_INITIALIZED;
	sem_flag &= ~(PRIOINHERIT_FLAGS_DISABLE);
	TC_ASSERT_EQ("sem_init", sem.flags, sem_flag);
#if CONFIG_SEM_PREALLOCHOLDERS > 0
	TC_ASSERT_EQ("sem_init", sem.hhead, NULL);
#else
	TC_ASSERT_EQ("sem_init", sem.holder.htcb, NULL);
	TC_ASSERT_EQ("sem_init", sem.holder.counts, 0);
#endif
#endif

	TC_SUCCESS_RESULT();
	return;
}


/**
 * @fn                   :tc_libc_semaphore_sem_getvalue
 * @brief                :this tc test sem_init function
 * @Scenario             :If sem or sval is NULL, it return ERROR and errno EINVAL is set.
 *                        Else sval get sem->semcount.
 * API's covered         :sem_getvalue
 * Preconditions         :NA
 * Postconditions        :NA
 * @return               :total_pass on success.
 */
static void tc_libc_semaphore_sem_getvalue(void)
{
	sem_t sem;
	unsigned int value = SEM_VALUE;
	int sval;
	int ret_chk;

	ret_chk = sem_init(&sem, PSHARED, value);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	ret_chk = sem_getvalue(NULL, &sval);
	TC_ASSERT_EQ("sem_getvalue", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_getvalue", get_errno(), EINVAL);

	ret_chk = sem_getvalue(&sem, NULL);
	TC_ASSERT_EQ("sem_getvalue", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_getvalue", get_errno(), EINVAL);

	ret_chk = sem_getvalue(&sem, &sval);
	TC_ASSERT_EQ("sem_getvalue", ret_chk, OK);
	TC_ASSERT_EQ("sem_getvalue", sval, value);

	TC_SUCCESS_RESULT();
	return;
}

/**
 * @fn                   :tc_libc_semaphore_sem_getprotocol
 * @brief                :this tc test sem_getprotocol function
 * @Scenario             :Return the value of the semaphore protocol attribute.
 * API's covered         :sem_init, sem_getprotocol
 * Preconditions         :NA
 * Postconditions        :NA
 * @return               :total_pass on success.
 */
static void tc_libc_semaphore_sem_getprotocol(void)
{
	sem_t sem;
	unsigned int value = SEM_VALUE;
	int protocol;
	int ret_chk;

	ret_chk = sem_init(&sem, PSHARED, value);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	ret_chk = sem_getprotocol(&sem, &protocol);
	TC_ASSERT_EQ("sem_getprotocol", ret_chk, OK);

	TC_SUCCESS_RESULT();
	return;
}

/****************************************************************************
 * Name: libc_semaphore
 ****************************************************************************/

int libc_semaphore_main(void)
{
	tc_libc_semaphore_sem_getvalue();
	tc_libc_semaphore_sem_getprotocol();
	tc_libc_semaphore_sem_init();

	return 0;
}
