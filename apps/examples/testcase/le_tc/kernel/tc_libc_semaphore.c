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

/// @file libc_semaphore.c
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

#define PSHARED 0

sem_t g_semaphore;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/**
* @fn                   :tc_libc_semaphores_semaphore_sem_init_getvalue
* @brief                :function initialises and retrieves the value of a named or unnamed semaphore.
* @Scenario             :function initialises and retrieves the value of a named or unnamed semaphore.
* API's covered         :sem_init, sem_getvalue
* Preconditions         :NA
* Postconditions        :NA
* @return               :total_pass on success.
*/
static void tc_libc_semaphore_sem_init_getvalue(void)
{
	int ret_chk = ERROR;
	int getvalue_num;
	int sem_value;
	int sem_num = 1;
	/* If pshared has the value 0, then the semaphore is shared between the threads of a process */

	for (sem_num = 1; sem_num <= SEM_VALUE_MAX; sem_num++) {
		sem_value = sem_num;
		ret_chk = sem_init(&g_semaphore, PSHARED, sem_value);
		TC_ASSERT_EQ("sem_init", ret_chk, OK);

		ret_chk = sem_getvalue(&g_semaphore, &getvalue_num);
		TC_ASSERT_EQ("sem_getvalue", ret_chk, OK);
		TC_ASSERT_EQ_CLEANUP("sem_getvalue",
							 getvalue_num, sem_value,
							 get_errno(),
							 sem_destroy(&g_semaphore));
		sem_destroy(&g_semaphore);
	}

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_semaphore
 ****************************************************************************/

int libc_semaphore_main(void)
{
	tc_libc_semaphore_sem_init_getvalue();

	return 0;
}
