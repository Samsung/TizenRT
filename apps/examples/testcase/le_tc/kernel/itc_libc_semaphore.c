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

/// @file itc_libc_semaphore.c
/// @brief ITC Test Case Example for Libc Semaphore API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <semaphore.h>
#include <tinyara/config.h>
#include "tc_internal.h"

/****************************************************************************
 * Private Functions
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

	sem.flags = 0;
	// sem is not initialized
	ret_chk = sem_getprotocol(&sem, &protocol);
	TC_ASSERT_EQ("sem_getprotocol", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_getprotocol", get_errno(), EINVAL);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_semaphore
 ****************************************************************************/

int itc_libc_semaphore_main(void)
{
	itc_libc_semaphore_getprotocol_n_no_sem_init();
	return 0;
}
