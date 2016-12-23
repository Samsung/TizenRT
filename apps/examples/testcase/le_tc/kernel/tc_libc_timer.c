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

/// @file libc_timer.c
/// @brief Test Case Example for Libc Timer API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <spawn.h>
#include <syslog.h>
#include <tinyara/time.h>
#include <sys/types.h>
#include "tc_internal.h"

#define BUFF_SIZE 64
#define YEAR_BASE 1900
#define DAY_1 1
#define DAY_2 2
#define SEC_5 5
#define YEAR2000 2000
#define ISLEAPYEAR 2000
#define ISNOTLEAPYEAR 2001

/**
* @fn                   :tc_clock_calendar2utc
* @brief                :UTC conversion routines. Calendar/UTC conversion based on algorithms from p. 604 of Seidelman, P. K. 1992.
*                        Explanatory Supplement to the Astronomical Almanac.  University Science Books, Mill Valley.
* @Scenario             :calculated time for 2 consecutive days and checked their difference. It should be equal to 1day time
* API's covered         :clock_calendar2utc
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_timer_clock_calendar2utc(void)
{
	int time1;
	int time2;
	int year;
	int month;
	int day1;
	int day2;

	year = YEAR2000;
	month = 1;
	day1 = DAY_1;
	day2 = DAY_2;

	time1 = clock_calendar2utc(year, month, day1);
	time2 = clock_calendar2utc(year, month, day2);
	/* difference in time values should equal to exact number of seconds in 1 day */
	if ((time2 - time1) != 1) {
		printf("tc_libc_timer_clock_calendar2utc FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_timer_clock_calendar2utc PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_gmtime_r
* @brief                :Time conversion (based on the POSIX API)
* @Scenario             :The gmtime_r function converts the calendar time timep to broken-down time representation
* API's covered         :gmtime_r
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_timer_gmtime_r(void)
{
	time_t test_time;
	int today;
	int year;
	struct tm st_time;
	struct tm *st_rettime = NULL;
	struct tm *st_localtime;

	time(&test_time);
	/* calculate current date */
	st_localtime = localtime(&test_time);
	today = st_localtime->tm_mday;
	year = st_localtime->tm_year;
	st_rettime = gmtime_r(&test_time, &st_time);
	if (st_rettime == NULL) {
		printf("tc_libc_timer_gmtime_r FAIL, st_rettime is null\n");
		total_fail++;
		RETURN_ERR;
	}
	/* verifying the structures returned and filled by comparing their parameters */
	if (st_rettime->tm_year != st_time.tm_year || st_rettime->tm_mon != st_time.tm_mon) {
		printf("tc_libc_timer_gmtime_r FAIL, st_rettime and st_time mismatch \n");
		total_fail++;
		RETURN_ERR;
	}
	if (year != st_time.tm_year || today != st_time.tm_mday) {
		printf("tc_libc_timer_gmtime_r FAIL, year and day mismatch \n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_timer_gmtime_r PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_gmtime
* @brief                :Time conversion, Not thread safe
* @Scenario             :The gmtime function converts the calendar time timep to broken-down time representation
* API's covered         :gmtime
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_timer_gmtime(void)
{
	int ret_chk = 0;
	struct tm *st_rettime = NULL;
	struct tm *st_localtime = NULL;
	time_t time1;
	time_t time2;
	ret_chk = time(&time1);
	if (ret_chk == (time_t)ERROR) {
		printf("tc_libc_timer_gmtime FAIL, time(&time1) is error\n");
		total_fail++;
		RETURN_ERR;
	}

	st_rettime = gmtime(&time1);
	if (st_rettime == NULL) {
		printf("tc_libc_timer_gmtime FAIL, st_rettime for time1 is null\n");
		total_fail++;
		RETURN_ERR;
	}
	st_localtime = localtime(&time1);
	if (st_localtime == NULL) {
		printf("tc_libc_timer_localtime FAIL, st_localtime should point to tm structure \n");
		total_fail++;
		RETURN_ERR;
	}
	if (st_rettime->tm_year != st_localtime->tm_year || st_rettime->tm_mon != st_localtime->tm_mon) {
		printf("tc_libc_timer_gmtime FAIL, localtime and gmtime mismatch \n");
		total_fail++;
		RETURN_ERR;
	}
	sleep(SEC_5);

	ret_chk = time(&time2);
	if (ret_chk == (time_t)ERROR) {
		printf("tc_libc_timer_gmtime FAIL, time(&time2) is error\n");
		total_fail++;
		RETURN_ERR;
	}
	st_rettime = gmtime(&time2);
	if (st_rettime == NULL) {
		printf("tc_libc_timer_gmtime FAIL, st_rettime for time2 is null\n");
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = (long long int)(time2) - (long long int)(time1);
	/* verifying the returned structure's parameter, year should not be negative, month range is 0-11 */
	if (st_rettime->tm_year <= 0 || st_rettime->tm_mon < 0 || st_rettime->tm_mon > 11) {
		printf("tc_libc_timer_gmtime FAIL, st_rettime and st_time mismatch \n");
		total_fail++;
		RETURN_ERR;
	}
	if (ret_chk < SEC_5) {
		printf("tc_libc_timer_gmtime FAIL, gmtime not getting correct time \n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_timer_gmtime PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_clock_isleapyear
* @brief                :Return true if the specified year is a leap year
* @Scenario             :Return true if the specified year is a leap year
* API's covered         :clock_isleapyear
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_timer_clock_isleapyear(void)
{
	int ret_chk = ERROR;
	int year;
	year = ISLEAPYEAR;
	/* Entered year is a leap year */
	ret_chk = clock_isleapyear(year);
	if (ret_chk != 1) {
		printf("tc_libc_timer_clock_isleapyear FAIL, should return a leap year \n");
		total_fail++;
		RETURN_ERR;
	}

	year = ISNOTLEAPYEAR;
	/*Entered year is not a leap year */
	ret_chk = clock_isleapyear(year);
	if (ret_chk != OK) {
		printf("tc_libc_timer_clock_isleapyear FAIL, should not return a leap year \n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_timer_clock_isleapyear PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_localtime
* @brief                :take an argument of data type time_t which represents calendar time.
*                        When interpreted as an absolute time value, it represents the number of seconds
*                        elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
* @Scenario             :Uses the value pointed by timerStruct to fill a tm structure with the values
*                        that represent the corresponding time, expressed for the local timezone.
* API's covered         :localtime
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_timer_localtime(void)
{
	time_t test_time;
	struct tm *st_rettime = NULL;
	time(&test_time);

	struct tm *st_gmtime = NULL;
	st_gmtime = gmtime(&test_time);
	if (st_gmtime == NULL) {
		printf("tc_libc_timer_gmtime FAIL, st_rettime for time1 is null\n");
		total_fail++;
		RETURN_ERR;
	}
	st_rettime = localtime(&test_time);
	if (st_rettime == NULL) {
		printf("tc_libc_timer_localtime FAIL, st_rettime should point to tm structure \n");
		total_fail++;
		RETURN_ERR;
	}
	if (st_rettime->tm_year != st_gmtime->tm_year || st_rettime->tm_mon != st_gmtime->tm_mon) {
		printf("tc_libc_timer_localtime FAIL, localtime and gmtime mismatch \n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_timer_localtime PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_localtime_r
* @brief                :take an argument of data type time_t which represents calendar time.
*                        When interpreted as an absolute time value, it represents the number of seconds
*                        elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
* @Scenario             :Uses the value pointed by timerStruct to fill a tm structure with the values
*                        that represent the corresponding time, expressed for the local timezone.
* API's covered         :localtime_r
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_timer_localtime_r(void)
{
	struct tm *st_rettime = NULL;
	struct tm st_time;
	struct tm st_gettime;
	time_t test_time;

	test_time = time(&test_time);

	gmtime_r(&test_time, &st_gettime);	/* get time through gmtime_r */
	st_rettime = localtime_r(&test_time, &st_time);
	if (st_rettime == NULL) {
		printf("tc_libc_timer_localtime_r FAIL, st_rettime should point to tm structure \n");
		total_fail++;
		RETURN_ERR;
	}
	/* verifying the structures "returned and filled" by comparing their parameters */
	if (st_rettime->tm_year != st_time.tm_year || st_rettime->tm_mon != st_time.tm_mon) {
		printf("tc_libc_timer_localtime_r FAIL, st_rettime and st_time mismatch \n");
		total_fail++;
		RETURN_ERR;
	}
	if (st_gettime.tm_year != st_time.tm_year || st_gettime.tm_mon != st_time.tm_mon) {
		printf("tc_libc_timer_localtime_r FAIL, st_rettime and st_gettime mismatch \n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_timer_localtime_r PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_mktime
* @brief                :converts the structure pointed to by timeptr(input parameter) into a
*                        time_t value according to the local time zone.
* @Scenario             :The mktime() function converts a broken-down time structure, expressed
*                        as local time, to calendar time representation.
* API's covered         :mktime
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_timer_mktime(void)
{
	int ret_chk = ERROR;
	int today;
	int after5days;
	struct tm *st_time;
	time_t test_time;
	time(&test_time);
	/* calculate current date */
	st_time = localtime(&test_time);
	today = st_time->tm_mday;
	st_time->tm_mday = st_time->tm_mday + SEC_5;

	ret_chk = mktime(st_time);
	after5days = st_time->tm_mday;
	if (ret_chk == ERROR) {
		printf("tc_libc_timer_mktime FAIL, mktime API fails, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	if (today + SEC_5 != after5days) {
		printf("tc_libc_timer_mktime FAIL, fails to update struct tm st_time, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_timer_mktime PASS \n");
	total_pass++;
}

/**
* @fn                   :tc_strftime
* @brief                :Format time as string
* @Scenario             :Copies into Source pointer the content of format, expanding its format specifiers
*                        into the corresponding values that represent the time described in timeptr structure,
*                        with a limit of maxsize characters.
* API's covered         :strftime
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_timer_strftime(void)
{
	time_t test_time;
	struct tm *st_time;
	char buffer[BUFF_SIZE];
	time(&test_time);
	st_time = localtime(&test_time);

	/* Verifying year and month filled in time structure */
	strftime(buffer, BUFF_SIZE, "%m", st_time);
	/* time structure has month in range 0-11, so tm_mon + 1 represents actual month number */
	if (atoi(buffer) != (st_time->tm_mon + 1)) {
		printf("tc_libc_timer_strftime FAIL, month val mismatch \n");
		total_fail++;
		RETURN_ERR;
	}
	strftime(buffer, BUFF_SIZE, "%Y", st_time);
	/* st_time->tm_year represents year relative to YEAR_BASE */
	if (atoi(buffer) != (st_time->tm_year + YEAR_BASE)) {
		printf("tc_libc_timer_strftime FAIL, year val mismatch \n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_timer_strftime PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_time
* @brief                :Get the current calendar time as a value of type time_t.
* @Scenario             :The function returns this value, and if the argument is not a null
*                        pointer, it also sets this value to the object pointed by timer.
* API's covered         :time
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_timer_time(void)
{
	time_t get_time = 0;
	time_t ret_time = 0;
	struct tm st_localtime;
	struct tm st_gmtime;

	ret_time = time(&get_time);
	gmtime_r(&get_time, &st_gmtime);	/* get time through gmtime_r */
	localtime_r(&get_time, &st_localtime);	/* get time through localtime_r */

	if (ret_time <= 0) {
		printf("tc_libc_timer_time FAIL, ret_time value is negative, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	if (st_gmtime.tm_year != st_localtime.tm_year || st_gmtime.tm_mon != st_localtime.tm_mon) {
		printf("tc_libc_timer_time FAIL, st_localtime and st_gmtime mismatch \n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_timer_time PASS \n");
	total_pass++;
}

/**
* @fn                   :tc_libc_timer_clock_daysbeforemonth
* @brief                :Get the number of days that occurred before the beginning of the month
* @Scenario             :Check the number of days in each month
* API's covered         :clock_daysbeforemonth
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_timer_clock_daysbeforemonth(void)
{
	int prev_month;
	int cur_month;
	int month_iter;
	int leapyear_days[13] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int notleapyear_days[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	/* test with not leapyear */
	for (month_iter = 1; month_iter < 12; month_iter++) {
		prev_month = clock_daysbeforemonth(month_iter, FALSE);
		cur_month = clock_daysbeforemonth(month_iter+1, FALSE);
		if (cur_month-prev_month != notleapyear_days[month_iter]) {
			printf("tc_libc_timer_clock_daysbeforemonth FAIL : not matched \n");
			total_fail++;
			RETURN_ERR;
		}
	}

	/* test with leapyear */
	for (month_iter = 1; month_iter < 12; month_iter++) {
		prev_month = clock_daysbeforemonth(month_iter, TRUE);
		cur_month = clock_daysbeforemonth(month_iter+1, TRUE);
		if (cur_month-prev_month != leapyear_days[month_iter]) {
			printf("!tc_libc_timer_clock_daysbeforemonth FAIL : not matched \n");
			total_fail++;
			RETURN_ERR;
		}
	}

	printf("tc_libc_timer_clock_daysbeforemonth PASS \n");
	total_pass++;
}

/****************************************************************************
 * Name: libc_timer
 ****************************************************************************/

int libc_timer_main(void)
{
	tc_libc_timer_clock_calendar2utc();
	tc_libc_timer_gmtime_r();
	tc_libc_timer_gmtime();
	tc_libc_timer_clock_isleapyear();
	tc_libc_timer_localtime();
	tc_libc_timer_localtime_r();
	tc_libc_timer_mktime();
	tc_libc_timer_strftime();
	tc_libc_timer_time();
	tc_libc_timer_clock_daysbeforemonth();

	return 0;
}
