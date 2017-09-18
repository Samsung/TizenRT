/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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

/// @file tc_libc_sched.c

/// @brief Test Case Example for Libc Sched API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include "tc_internal.h"

#define SCHED_ERROR 0

/**
* @fn                   :tc_libc_sched_sched_get_priority_max
* @brief                :returns the maximum priority value
* @Scenario             :Function returns the maximum priority value that can be used with the scheduling algorithm identified by policy.
* API's covered         :sched_get_priority_max
* Preconditions         :NA
* Postconditions        :NA
* @return               :void
*/
static void tc_libc_sched_sched_get_priority_max(void)
{
	int ret_chk;

	ret_chk = sched_get_priority_max(SCHED_ERROR);
	TC_ASSERT_EQ("sched_get_priority_max", ret_chk, ERROR);

	ret_chk = sched_get_priority_max(SCHED_FIFO);
	TC_ASSERT_EQ("sched_get_priority_max", ret_chk, SCHED_PRIORITY_MAX);

	ret_chk = sched_get_priority_max(SCHED_RR);
	TC_ASSERT_EQ("sched_get_priority_max", ret_chk, SCHED_PRIORITY_MAX);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_sched_sched_get_priority_min
* @brief                :returns the minimum priority value
* @Scenario             :Function returns the minimum priority value that can be used with the scheduling algorithm identified by policy.
* API's covered         :sched_get_priority_max
* Preconditions         :NA
* Postconditions        :NA
* @return               :void
*/
static void tc_libc_sched_sched_get_priority_min(void)
{
	int ret_chk;

	ret_chk = sched_get_priority_min(SCHED_ERROR);
	TC_ASSERT_EQ("sched_get_priority_min", ret_chk, ERROR);

	ret_chk = sched_get_priority_min(SCHED_FIFO);
	TC_ASSERT_EQ("sched_get_priority_min", ret_chk, SCHED_PRIORITY_MIN);

	ret_chk = sched_get_priority_min(SCHED_RR);
	TC_ASSERT_EQ("sched_get_priority_min", ret_chk, SCHED_PRIORITY_MIN);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_libc_sched_task_setcanceltype
 * @brief                :This tc tests tc_libc_sched_task_setcanceltype()
 * @Scenario             :The task_setcanceltype() function atomically both sets the calling
 *                        task's cancelability type to the indicated type and returns the
 *                        previous cancelability type at the location referenced by oldtype
 *                        If successful pthread_setcanceltype() function shall return zero;
 *                        otherwise, an error number shall be returned to indicate the error.
 * @API'scovered         :task_setcanceltype
 * @Preconditions        :none
 * @Postconditions       :none
 * @return               :void
 */
#ifndef CONFIG_CANCELLATION_POINTS
static void tc_libc_sched_task_setcanceltype(void)
{
	int type;
	int oldtype;
	int ret_chk;

	type = TASK_CANCEL_ASYNCHRONOUS;
	ret_chk = task_setcanceltype(type, &oldtype);
	TC_ASSERT_EQ("task_setcanceltype", ret_chk, OK);

	TC_SUCCESS_RESULT();
}
#endif

/****************************************************************************
 * Name: libc_sched
 ****************************************************************************/

int libc_sched_main(void)
{
	tc_libc_sched_sched_get_priority_max();
	tc_libc_sched_sched_get_priority_min();
#ifndef CONFIG_CANCELLATION_POINTS
	tc_libc_sched_task_setcanceltype();
#endif
	return 0;
}
