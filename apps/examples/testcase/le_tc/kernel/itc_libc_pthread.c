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

/// @file itc_libc_pthread.c

/// @brief ITC Test Case Example for Libc Pthread API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <tinyara/pthread.h>
#include <tinyara/config.h>
#include "tc_internal.h"

#define LOOP_SIZE CONFIG_ITC_LIBC_PTHREAD_LOOP_SIZE
#define NUM_READER 10
#define NUM_WRITER 10

/****************************************************************************
  * Private Functions
 ****************************************************************************/

/**
* @fn                   :itc_libc_pthread_pthread_attr_init_destroy_p_multitime
* @brief                :pthread_attr_init and pthread_attr_destroy call multiple time
* @Scenario             :If pthread_attr is NULL, ENOMEM is returned.
*                        Else, it return OK and pthread_attr is set to default value
* @API'scovered         :pthread_attr_init
* @Preconditions        :none
* @Postconditions       :none
*/
static void itc_libc_pthread_pthread_attr_init_destroy_p_multitime(void)
{
	pthread_attr_t attr;
	pthread_attr_t destroyed_attr;
	int ret_chk;
	int exec_index;

	memset(&destroyed_attr, 0, sizeof(pthread_attr_t));

	for (exec_index = 0; exec_index < LOOP_SIZE; exec_index++) {
		ret_chk = pthread_attr_init(&attr);
		TC_ASSERT_EQ("pthread_attr_init", ret_chk, OK);
		TC_ASSERT_EQ("pthread_attr_init", attr.stacksize, PTHREAD_STACK_DEFAULT);
		TC_ASSERT_EQ("pthread_attr_init", attr.priority, PTHREAD_DEFAULT_PRIORITY);
		TC_ASSERT_EQ("pthread_attr_init", attr.policy, PTHREAD_DEFAULT_POLICY);
		TC_ASSERT_EQ("pthread_attr_init", attr.inheritsched, PTHREAD_EXPLICIT_SCHED);

		ret_chk = pthread_attr_destroy(&attr);
		TC_ASSERT_EQ("pthread_attr_destroy", ret_chk, OK);
		TC_ASSERT_EQ("pthread_attr_destroy", memcmp(&attr, &destroyed_attr, sizeof(pthread_attr_t)), 0);
	}
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_libc_pthread_pthread_rwlock_init_destroy_p_multitime
* @brief                :This tc tests pthread_rwlock_init and pthread_rwlock_destroy
* @Scenario             :If status is return OK,with default lock value.
*                        Else return error with errno set.
* @API's covered        :pthread_rwlock_init,pthread_rwlock_destroy
* @Preconditions        :none
* @Postconditions       :none
*/
static void itc_libc_pthread_pthread_rwlock_init_destroy_p_multitime(void)
{
	int status;
	int exec_index;
	pthread_rwlock_t rw_lock;
	rw_lock.num_readers = NUM_READER;
	rw_lock.num_writers = NUM_WRITER;
	rw_lock.write_in_progress = true;

	for (exec_index = 0; exec_index < LOOP_SIZE; exec_index++) {
		status = pthread_rwlock_init(&rw_lock, NULL);
		TC_ASSERT_EQ("pthread_rwlock_init", status, OK);
		TC_ASSERT_EQ("pthread_rwlock_init", rw_lock.num_readers, 0);
		TC_ASSERT_EQ("pthread_rwlock_init", rw_lock.num_writers, 0);
		TC_ASSERT_EQ("pthread_rwlock_init", rw_lock.write_in_progress, false);

		status = pthread_rwlock_destroy(&rw_lock);
		TC_ASSERT_EQ("pthread_rwlock_destroy", status, OK);
	}

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: ITC libc_pthread
 ****************************************************************************/

int itc_libc_pthread_main(void)
{
	itc_libc_pthread_pthread_attr_init_destroy_p_multitime();
	itc_libc_pthread_pthread_rwlock_init_destroy_p_multitime();
	return 0;
}
