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

/// @file clock.c

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

#define SEC_PER_MIN ((time_t)60)
#define SEC_PER_HOUR ((time_t)60 * SEC_PER_MIN)
#define SEC_PER_DAY ((time_t)24 * SEC_PER_HOUR)
#define SEC_2   2
#define SEC_10  10
#define NSEC_20 20

/**
* @fn                   :tc_clock_clock_initialize
* @brief                :Initialize the time value to match the RTC
* @scenario             :Initialize the time value to match the RTC
* API's covered         :clock_initialize
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_clock_clock_initialize(void)
{
	g_basetime.tv_nsec = NSEC_20;
	time_t jdn = 0;

	jdn = clock_calendar2utc(CONFIG_START_YEAR, CONFIG_START_MONTH, CONFIG_START_DAY);

	clock_initialize();
	/*
	   g_basetime.tv_sec base time is getting as seconds into this julian day. hence we need to compare it that same
	 */
	if (g_basetime.tv_nsec != 0 || g_basetime.tv_sec != (jdn * SEC_PER_DAY)) {
		printf("tc_clock_clock_initialize FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_clock_clock_initialize PASS\n");
	total_pass++;
}

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
	ret_chk = clock_getres(CLOCK_REALTIME, &st_res);	/* It only work for realtime. */
	if (ret_chk != OK) {
		printf("tc_clock_clock_getres CLOCK_REALTIME FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	if ((st_res.tv_sec != 0) || (st_res.tv_nsec != NSEC_PER_TICK)) {
		printf("tc_clock_clock_getres CLOCK_REALTIME FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_clock_clock_getres PASS\n");
	total_pass++;
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
	struct timespec stime;
	struct timespec gtime;
	if (clock_gettime(CLOCK_REALTIME, &stime) == ERROR) {
		printf("tc_clock_clock_set_get_time: initial gettime FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	stime.tv_sec += l_day;		/* Add one day */
	stime.tv_nsec = 0;
	if (clock_settime(CLOCK_REALTIME, &stime) == ERROR) {
		printf("tc_clock_clock_set_get_time FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	sleep(SEC_2);

	if (clock_gettime(CLOCK_REALTIME, &gtime) == ERROR) {
		printf("tc_clock_clock_set_get_time: gettime FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	if (!(stime.tv_sec + SEC_2 <= gtime.tv_sec && stime.tv_nsec <= gtime.tv_nsec)) {
		printf("tc_clock_clock_set_get_time FAIL: get value should be greater than set value\n");
		total_fail++;
		RETURN_ERR;
	}

	stime.tv_sec -= l_day;		/* Setting original time to system */

	if (clock_settime(CLOCK_REALTIME, &stime) == ERROR) {
		printf("tc_clock_clock_set_get_time FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_clock_clock_set_get_time PASS\n");
	total_pass++;
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
	int ret_chk = 0;

	struct timeval *st_tvptr_1 = (struct timeval *)malloc(sizeof(struct timeval));
	struct timeval *st_tvptr_2 = (struct timeval *)malloc(sizeof(struct timeval));

	ret_chk = gettimeofday(st_tvptr_1, NULL);
	if (ret_chk != OK) {
		printf("tc_clock_clock_gettimeofday FAIL, Error No: %d\n", errno);
		total_fail++;
		free(st_tvptr_1);
		free(st_tvptr_2);
		RETURN_ERR;
	}
	if (st_tvptr_1 == NULL) {	/* check fail condition */
		printf("tc_clock_clock_gettimeofday FAIL: Time Value is not filled\n");
		total_fail++;
		free(st_tvptr_1);
		free(st_tvptr_2);
		RETURN_ERR;
	}

	sleep(SEC_2);

	ret_chk = gettimeofday(st_tvptr_2, NULL);
	if (ret_chk != OK) {
		printf("tc_clock_clock_gettimeofday FAIL, Error No: %d\n", errno);
		total_fail++;
		free(st_tvptr_1);
		free(st_tvptr_2);
		RETURN_ERR;
	}
	if (st_tvptr_2 == NULL) {	/* check total_fail condition */
		printf("tc_clock_clock_gettimeofday FAIL: Time Value is not filled\n");
		total_fail++;
		free(st_tvptr_1);
		RETURN_ERR;
	}

	if (st_tvptr_2->tv_sec < st_tvptr_1->tv_sec || st_tvptr_2->tv_usec < st_tvptr_1->tv_usec) {
		printf("tc_clock_clock_gettimeofday FAIL: Time Value error\n");
		total_fail++;
		free(st_tvptr_1);
		free(st_tvptr_2);
		RETURN_ERR;
	}

	free(st_tvptr_1);
	free(st_tvptr_2);
	printf("tc_clock_clock_gettimeofday PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_clock_clock_timer
* @brief                :This function must be called once every time the real time clock interrupt occurs
*                        The interval of this clock interrupt must be USER_PER_TICK
* @scenario             :called once every time the real time clock interrupt occurs
* API's covered         :clock_timer
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_clock_clock_timer(void)
{
	systime_t itime = g_system_timer;
	clock_timer();
	itime++;
	if (itime != g_system_timer) {
		printf("tc_clock_clock_timer FAIL.\n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_clock_clock_timer PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_clock_clock_systimer
* @brief                :Return the current value of the 32 bit system timer counter
* @scenario             :Return the current value of the 32 bit system timer counter
* API's covered         :clock_systimer
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_clock_clock_systimer(void)
{
	systime_t itime = ERROR;
	itime = clock_systimer();
	if (itime < 0) {
		printf("tc_clock_clock_systimer FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_clock_clock_systimer PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_clock_clock_systimer64
* @brief                :Returns the current value of 64 bit system timer counter
* @scenario             :Returns the current value of 64 bit system timer counter
* API's covered         :clock_systimer64
* Preconditions         :none
* Postconditions        :none
* @return               :void
 */
static void tc_clock_clock_systimer64(void)
{
	systime_t itime = ERROR;
	itime = clock_systimer();
	if (itime < 0) {
		printf("tc_clock_clock_systimer64, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_clock_clock_systimer64 PASS\n");
	total_pass++;
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
	struct timespec base_time;
	struct timespec comparison_time;
	int base_tick;
	int comparison_tick;
	int ret = ERROR;

	clock_gettime(CLOCK_REALTIME, &base_time);
	base_time.tv_sec *= 2;
	comparison_time.tv_sec = base_time.tv_sec * 2;

	ret = clock_abstime2ticks(CLOCK_REALTIME, &base_time, &base_tick);
	if (ret != OK) {
		total_fail++;
		printf("tc_clock_clock_abstime2ticks FAIL %d\n", base_tick);
		RETURN_ERR;
	}
	ret = clock_abstime2ticks(CLOCK_REALTIME, &comparison_time, &comparison_tick);
	if (ret != OK) {
		total_fail++;
		printf("tc_clock_clock_abstime2ticks FAIL %d %d\n", base_tick, comparison_tick);
		RETURN_ERR;
	}

	/* the difference can be 0 or 1, but should be smaller than 2 */
	if (comparison_tick - (base_tick * 2) < 2) {
		total_fail++;
		printf("tc_clock_clock_abstime2ticks FAIL %d %d\n", base_tick, comparison_tick);
		RETURN_ERR;
	}
	total_pass++;
	printf("tc_clock_clock_abstime2ticks PASS\n");
}

/****************************************************************************
 * Name: clock
 ****************************************************************************/

int clock_main(void)
{
	tc_clock_clock_initialize();
	tc_clock_clock_timer();
	tc_clock_clock_systimer();
	tc_clock_clock_systimer64();
	tc_clock_clock_gettimeofday();
	tc_clock_clock_set_get_time();
	tc_clock_clock_getres();
	tc_clock_clock_abstime2ticks();

	return 0;
}
