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

/// @file tc_clock.c

/// @brief Test Case Example for Clock API

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <tinyara/time.h>
#include <sys/time.h>
#include "../../../../../os/kernel/clock/clock.h"
#include "tc_internal.h"

const long double l_day = 86400;
extern struct timespec g_basetime;

#define CLOCK_ERR 2
#define SEC_PER_MIN ((time_t)60)
#define SEC_PER_HOUR ((time_t)60 * SEC_PER_MIN)
#define SEC_PER_DAY ((time_t)24 * SEC_PER_HOUR)
#define SEC_2   2
#define SEC_10  10
#define NSEC_20 20


/**
* @fn                   :tc_clock_clock_getres
* @brief                :finds the resolution(precision) of the specified clock clockid
* @scenario             :Test the resolution of Clock for Clock Ids CLOCK_MONOTONIC and CLOCK_MONOTONIC_COARSE
* API's covered         :clock_getres
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_clock_clock_getres(void)
{
	int ret_chk;
	struct timespec st_res;

	st_res.tv_sec = SEC_10;

	/* It only work for realtime. */

	ret_chk = clock_getres(CLOCK_REALTIME, &st_res);
	TC_ASSERT_EQ("clock_getres", ret_chk, OK);
	TC_ASSERT_EQ("clock_getres", st_res.tv_sec, 0);
	TC_ASSERT_EQ("clock_getres", st_res.tv_nsec, NSEC_PER_TICK);

	ret_chk = clock_getres(33 , &st_res);
	TC_ASSERT_EQ("clock_getres", ret_chk, ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_clock_clock_set_get_time
* @brief                :sets and gets current time of the clock
* @scenario             :Set  the current value time for the specified clock, clock_id =CLOCK_REALTIME
* API's covered         :clock_gettime, clock_settime
* Preconditions         :only supported clock ID is CLOCK_REALTIME
* Postconditions        :none
* @return               :void
*/
static void tc_clock_clock_set_get_time(void)
{
	int ret_chk;
	struct timespec stime;
	struct timespec gtime;

	ret_chk = clock_gettime(CLOCK_ERR, &stime);
	TC_ASSERT_EQ("clock_gettime", ret_chk, ERROR);
	TC_ASSERT_EQ("clock_gettime", errno, EINVAL);

	ret_chk = clock_gettime(CLOCK_REALTIME, &stime);
	TC_ASSERT_EQ("clock_gettime", ret_chk, OK);

	stime.tv_sec += l_day;		/* Add one day */
	stime.tv_nsec = 0;
	ret_chk = clock_settime(CLOCK_REALTIME, &stime);
	TC_ASSERT_EQ("clock_settime", ret_chk, OK);

	sleep(SEC_2);

	ret_chk = clock_gettime(CLOCK_REALTIME, &gtime);
	TC_ASSERT_EQ("clock_gettime", ret_chk, OK);

	TC_ASSERT_GEQ("clock_gettime", gtime.tv_sec, stime.tv_sec + SEC_2);
	if (gtime.tv_sec == stime.tv_sec + SEC_2) {
		TC_ASSERT_GEQ("clock_gettime", gtime.tv_nsec, stime.tv_nsec);
	}

	stime.tv_sec -= l_day;		/* Setting original time to system */

	ret_chk = clock_settime(CLOCK_ERR, &stime);
	TC_ASSERT_EQ("clock_settime", ret_chk, ERROR);
	TC_ASSERT_EQ("clock_settime", errno, EINVAL);

	ret_chk = clock_settime(CLOCK_REALTIME, &stime);
	TC_ASSERT_EQ("clock_setime", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_clock_clock_gettimeofday
* @brief                :get the time as well as a timezone
* @scenario             :Get  time of the day with timezone set to NULL
* API's covered         :gettimeofday
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_clock_clock_gettimeofday(void)
{
	int ret_chk;
	struct timeval tv1;
	struct timeval tv2;

	ret_chk = gettimeofday(&tv1, NULL);
	TC_ASSERT_EQ("gettimeofday", ret_chk, OK);

	sleep(SEC_2);

	ret_chk = gettimeofday(&tv2, NULL);
	TC_ASSERT_EQ("gettimeofday", ret_chk, OK);

	TC_ASSERT_GEQ("gettimeofday", tv2.tv_sec, tv1.tv_sec + SEC_2);
	if (tv2.tv_sec == tv1.tv_sec + SEC_2) {
		TC_ASSERT_GEQ("gettimeofday", tv2.tv_usec, tv1.tv_usec);
	}
#ifdef CONFIG_DEBUG
	ret_chk = gettimeofday(NULL, NULL);
	TC_ASSERT_EQ("gettimeofday", ret_chk, ERROR);
#endif

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_clock_clock_abstime2ticks
* @brief                :Convert an absolute timespec to ticks
* @scenario             :Compare two conversions to ticks with some calculation
* API's covered         :clock_abstime2ticks
* Preconditions         :none
* Postconditions        :none
* @return               :void
 */
static void tc_clock_clock_abstime2ticks(void)
{
	int ret_chk;
	int base_tick;
	int comparison_tick;
	struct timespec base_time;
	struct timespec comparison_time;

	clock_gettime(CLOCK_REALTIME, &base_time);
	comparison_time.tv_sec = base_time.tv_sec + 1;

	ret_chk = clock_abstime2ticks(CLOCK_REALTIME, &base_time, &base_tick);
	TC_ASSERT_EQ("clock_abstime2ticks", ret_chk, OK);

	ret_chk = clock_abstime2ticks(CLOCK_REALTIME, &comparison_time, &comparison_tick);
	TC_ASSERT_EQ("clock_abstime2ticks", ret_chk, OK);

	/* the difference can be 0 or 1, but should be smaller than 2 */

	TC_ASSERT_GEQ("clock_abstime2ticks", comparison_tick - (base_tick * 2), 2);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: clock
 ****************************************************************************/

int clock_main(void)
{
	tc_clock_clock_gettimeofday();
	tc_clock_clock_set_get_time();
	tc_clock_clock_getres();
	tc_clock_clock_abstime2ticks();

	return 0;
}
