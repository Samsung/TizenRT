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

/// @file itc_timer.c
/// @brief Test Case Example for Timer API

#include <tinyara/config.h>
#include <errno.h>
#include "tc_internal.h"
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>


/**
* @fn                   :itc_timer_set_time_n_after_delete
* @brief                :arm/disarm and fetch state of POSIX per-process timer
* @scenario             :arm/disarm and fetch state of POSIX per-process timer after timer delete
* API's covered         :timer_create, timer_settime, timer_gettime
* Preconditions         :Creation of timer_id(timer_create)
* Postconditions        :none
*/
static void itc_timer_set_time_n_after_delete(void)
{
	int ret_chk = ERROR;
	clockid_t clockid = CLOCK_REALTIME;
	struct sigevent st_sigevent;
	struct itimerspec st_timer_spec_set;
	timer_t timer_id;

	st_sigevent.sigev_notify = SIGEV_SIGNAL;
	st_sigevent.sigev_signo = SIGRTMIN;
	st_sigevent.sigev_value.sival_ptr = &timer_id;
	ret_chk = timer_create(clockid, &st_sigevent, &timer_id);
	TC_ASSERT_NEQ("timer_create", ret_chk, ERROR);
	TC_ASSERT_NEQ("timer_create", timer_id, NULL);

	timer_delete(timer_id);
	TC_ASSERT_EQ("timer_delete", ret_chk, OK);

	st_timer_spec_set.it_interval.tv_sec = 1;
	st_timer_spec_set.it_interval.tv_nsec = 0;
	st_timer_spec_set.it_value.tv_sec = 1;
	st_timer_spec_set.it_value.tv_nsec = 0;

	ret_chk = timer_settime(timer_id, 0, &st_timer_spec_set, NULL);
	TC_ASSERT_NEQ("timer_settime", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_timer_get_time_n_after_delete
* @brief                :arm/disarm and fetch state of POSIX per-process timer
* @scenario             :arm/disarm and fetch state of POSIX per-process timer after delete
* API's covered         :timer_create, timer_settime, timer_gettime
* Preconditions         :Creation of timer_id(timer_create)
* Postconditions        :none
*/
static void itc_timer_get_time_n_after_delete(void)
{
	int ret_chk = ERROR;
	clockid_t clockid = CLOCK_REALTIME;
	struct sigevent st_sigevent;
	struct itimerspec st_timer_spec_set;
	struct itimerspec st_timer_spec_get;
	timer_t timer_id;

	st_sigevent.sigev_notify = SIGEV_SIGNAL;
	st_sigevent.sigev_signo = SIGRTMIN;
	st_sigevent.sigev_value.sival_ptr = &timer_id;
	ret_chk = timer_create(clockid, &st_sigevent, &timer_id);
	TC_ASSERT_NEQ("timer_create", ret_chk, ERROR);
	TC_ASSERT_NEQ("timer_create", timer_id, NULL);

	st_timer_spec_set.it_interval.tv_sec = 1;
	st_timer_spec_set.it_interval.tv_nsec = 0;
	st_timer_spec_set.it_value.tv_sec = 1;
	st_timer_spec_set.it_value.tv_nsec = 0;

	ret_chk = timer_settime(timer_id, 0, &st_timer_spec_set, NULL);
	TC_ASSERT_EQ_ERROR_CLEANUP("timer_settime", ret_chk, OK, errno, timer_delete(timer_id));

	ret_chk = timer_delete(timer_id);
	TC_ASSERT_EQ("timer_delete", ret_chk, OK);

	ret_chk = timer_gettime(timer_id, &st_timer_spec_get);
	TC_ASSERT_NEQ("timer_gettime", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: timer
 ****************************************************************************/

int itc_timer_main(void)
{
	itc_timer_set_time_n_after_delete();
	itc_timer_get_time_n_after_delete();

	return 0;
}
