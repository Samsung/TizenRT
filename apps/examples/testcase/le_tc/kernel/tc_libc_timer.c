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

/// @file tc_libc_timer.c
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
#include <syslog.h>
#include <time.h>
#include <tinyara/time.h>
#include <sys/types.h>
#include <math.h>
#include <float.h>
#include "tc_internal.h"

#define BUFF_SIZE 64
#define DAY_1 1
#define DAY_2 2
#define SEC_5 5
#define YEAR2000 2000
#define ISLEAPYEAR 2000
#define ISNOTLEAPYEAR 2001

/**
* @fn                   :tc_libc_timer_clock_calendar2utc
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

	TC_ASSERT_EQ("clock_calendar2utc", time2 - time1, 1);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_timer_gmtime_r
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

	/* verifying the structures returned and filled by comparing their parameters */

	st_rettime = gmtime_r(&test_time, &st_time);
	TC_ASSERT_NEQ("gmtime_r", st_rettime, NULL);
	TC_ASSERT_EQ("gmtime_r", st_rettime->tm_year, st_time.tm_year);
	TC_ASSERT_EQ("gmtime_r", st_rettime->tm_mon, st_time.tm_mon);
	TC_ASSERT_EQ("gmtime_r", year, st_time.tm_year);
	TC_ASSERT_EQ("gmtime_r", today, st_time.tm_mday);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_timer_gmtime
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
	TC_ASSERT_NEQ("time", ret_chk, (time_t)ERROR);

	st_rettime = gmtime(&time1);
	TC_ASSERT_NEQ("gmtime", st_rettime, NULL);

	st_localtime = localtime(&time1);
	TC_ASSERT_NEQ("localtime", st_localtime, NULL);
	TC_ASSERT_EQ("localtime", st_rettime->tm_year, st_localtime->tm_year);
	TC_ASSERT_EQ("localtime", st_rettime->tm_mon, st_localtime->tm_mon);

	sleep(SEC_5);

	ret_chk = time(&time2);
	TC_ASSERT_NEQ("time", ret_chk, (time_t)ERROR);

	/* verifying the returned structure's parameter, year should not be negative, month range is 0-11 */

	st_rettime = gmtime(&time2);
	TC_ASSERT_NEQ("gmtime", st_rettime, NULL);
	TC_ASSERT_GEQ("gtime", st_rettime->tm_year, 0);
	TC_ASSERT_GEQ("gtime", st_rettime->tm_mon, 0);
	TC_ASSERT_LT("gtime", st_rettime->tm_mon, 12);

	ret_chk = (long long int)(time2) - (long long int)(time1);
	TC_ASSERT_GEQ("gtime", ret_chk, SEC_5);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_timer_clock_isleapyear
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

	/* Entered year is a leap year */

	year = ISLEAPYEAR;
	ret_chk = clock_isleapyear(year);
	TC_ASSERT_EQ("clock_isleapyear", ret_chk, 1);

	/*Entered year is not a leap year */

	year = ISNOTLEAPYEAR;
	ret_chk = clock_isleapyear(year);
	TC_ASSERT_EQ("clock_isleapyear", ret_chk, 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_timer_localtime
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
	struct tm *st_gmtime = NULL;

	time(&test_time);
	st_gmtime = gmtime(&test_time);
	TC_ASSERT_NEQ("gmtime", st_gmtime, NULL);

	st_rettime = localtime(&test_time);
	TC_ASSERT_NEQ("localtime", st_gmtime, NULL);
	TC_ASSERT_EQ("localtime", st_rettime->tm_year, st_gmtime->tm_year);
	TC_ASSERT_EQ("localtime", st_rettime->tm_mon, st_gmtime->tm_mon);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_timer_localtime_r
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

	/* get time through gmtime_r */

	gmtime_r(&test_time, &st_gettime);

	/* verifying the structures "returned and filled" by comparing their parameters */

	st_rettime = localtime_r(&test_time, &st_time);
	TC_ASSERT_NEQ("localtime_r", st_rettime, NULL);
	TC_ASSERT_EQ("localtime_r", st_rettime->tm_year, st_time.tm_year);
	TC_ASSERT_EQ("localtime_r", st_rettime->tm_mon, st_time.tm_mon);
	TC_ASSERT_EQ("localtime_r", st_gettime.tm_year, st_time.tm_year);
	TC_ASSERT_EQ("localtime_r", st_gettime.tm_mon, st_time.tm_mon);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_timer_mktime
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
	time_t cur_day;
	time_t after_5days;
	struct tm st_time;
	struct timespec ts;
	int ret_chk;

	/* Get the current time */

	ret_chk = clock_gettime(CLOCK_REALTIME, &ts);
	TC_ASSERT_LEQ("clock_gettime", ret_chk, 0);
	(void)gmtime_r((FAR const time_t *)&ts.tv_sec, &st_time);

	/* convert current day to time_t */
	cur_day = mktime(&st_time);
	TC_ASSERT_NEQ("mktime", cur_day, ERROR);

	/* convert current day + 5 to time_t */
	st_time.tm_mday = st_time.tm_mday + 5;
	after_5days = mktime(&st_time);
	TC_ASSERT_NEQ("mktime", after_5days, ERROR);
	/* after_5day should be greater than cur_day */
	TC_ASSERT_GT("mktime", after_5days, cur_day);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_timer_strftime
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
	struct tm st_time;
	char buffer[BUFF_SIZE];

	/* using specific tm struct */
	st_time.tm_sec = 15;
	st_time.tm_min = 21;
	st_time.tm_hour = 18;
	st_time.tm_mday = 19;
	st_time.tm_mon = 5;
	st_time.tm_year = 2017 - TM_YEAR_BASE;

	/* Verifying year and month filled in time structure.
	 * time structure has month in range 0-11,
	 * so tm_mon + 1 represents actual month number */

	/* Check the zero value */
	TC_ASSERT_EQ("strftime", strftime(buffer, 0, "return_0_check", &st_time), 0);

	/* Check the three-letter abbreviation for the day of the week */
	strftime(buffer, BUFF_SIZE, "a", &st_time);
	TC_ASSERT_EQ("strftime", strncmp(buffer, "a", strlen("a") + 1), 0);

	/* Check the full name for the day of the week */
	strftime(buffer, BUFF_SIZE, "%a", &st_time);
	TC_ASSERT_EQ("strftime", strncmp(buffer, "Day", strlen("Day") + 1), 0);

	/* Check the full month name */
	strftime(buffer, BUFF_SIZE, "%B", &st_time);
	TC_ASSERT_EQ("strftime", strncmp(buffer, "June", strlen("June") + 1), 0);

	/* Check the day of month as a decimal number (range 01 to 31) */
	strftime(buffer, BUFF_SIZE, "%d", &st_time);
	TC_ASSERT_EQ("strftime", atoi(buffer), st_time.tm_mday);

	/* Check the day of month as a decimal number but a leading zero
	 * is replaced by a space. (range 1 to 31)
	 */
	strftime(buffer, BUFF_SIZE, "%e", &st_time);
	TC_ASSERT_EQ("strftime", atoi(buffer), st_time.tm_mday);

	/* Check the abbreviated month name */
	strftime(buffer, BUFF_SIZE, "%h", &st_time);
	TC_ASSERT_EQ("strftime", strncmp(buffer, "Jun", strlen("Jun") + 1), 0);

	/* Check the hour as a decimal number using a 24-hour clock (range 00 to 23) */
	strftime(buffer, BUFF_SIZE, "%H", &st_time);
	TC_ASSERT_EQ("strftime", atoi(buffer), st_time.tm_hour);

	/* Check the hour as a decimal number using a 12-hour clock (range 01 to 12) */
	strftime(buffer, BUFF_SIZE, "%I", &st_time);
	TC_ASSERT_EQ("strftime", atoi(buffer), st_time.tm_hour % 12);

	/* Check the day of the year as a decimal number (range 001 to 366) */
	strftime(buffer, BUFF_SIZE, "%j", &st_time);
	TC_ASSERT_EQ("strftime", atoi(buffer), clock_daysbeforemonth(st_time.tm_mon, clock_isleapyear(st_time.tm_year)) + st_time.tm_mday);

	/* Check the hour as a decimal number using a 24-hour clock (range 0 to 23)
	 * single digits are preceded by a blank.
	 */
	strftime(buffer, BUFF_SIZE, "%k", &st_time);
	TC_ASSERT_EQ("strftime", atoi(buffer), st_time.tm_hour);

	/* Check the hour as a decimal number using a 12-hour clock (range 01 to 12)
	 * single digits are preceded by a blank.
	 */
	strftime(buffer, BUFF_SIZE, "%l", &st_time);
	TC_ASSERT_EQ("strftime", atoi(buffer), st_time.tm_hour % 12);

	/* Check the month as a decimal number (range 01 to 12). */
	strftime(buffer, BUFF_SIZE, "%m", &st_time);
	TC_ASSERT_EQ("strftime", atoi(buffer), st_time.tm_mon + 1);

	/* Check the minute as a decimal number (range 00 to 59). */
	strftime(buffer, BUFF_SIZE, "%M", &st_time);
	TC_ASSERT_EQ("strftime", atoi(buffer), st_time.tm_min);

	/* Check the Enter character */
	strftime(buffer, BUFF_SIZE, "%n", &st_time);
	TC_ASSERT_EQ("strftime", strncmp(buffer, "\n", strlen("\n") + 1), 0);

	/* Check either "AM" or "PM" according to the given time value, Noon is treated as "PM" and midnight as "AM" */
	strftime(buffer, BUFF_SIZE, "%p", &st_time);
	TC_ASSERT_EQ("strftime", strncmp(buffer, "PM", strlen("PM") + 1), 0);

	/* Check either "am" or "pm" according to the given time value, Noon is treated as "pm" and midnight as "am" */
	strftime(buffer, BUFF_SIZE, "%P", &st_time);
	TC_ASSERT_EQ("strftime", strncmp(buffer, "pm", strlen("pm") + 1), 0);

	/* Check the century number (year/100) as a 2-digit integer */
	strftime(buffer, BUFF_SIZE, "%y", &st_time);
	TC_ASSERT_EQ("strftime", atoi(buffer), st_time.tm_year % 100);

	st_time.tm_hour = 3;
	/* Check either "AM" or "PM" according to the given time value, Noon is treated as "PM" and midnight as "AM" */
	strftime(buffer, BUFF_SIZE, "%p", &st_time);
	TC_ASSERT_EQ("strftime", strncmp(buffer, "AM", strlen("AM") + 1), 0);

	/* Check either "am" or "pm" according to the given time value, Noon is treated as "pm" and midnight as "am" */
	strftime(buffer, BUFF_SIZE, "%P", &st_time);
	TC_ASSERT_EQ("strftime", strncmp(buffer, "am", strlen("am") + 1), 0);

	/* Check the number of seconds since the Epoch, that is, since 1970-01-01 */
	strftime(buffer, BUFF_SIZE, "%s", &st_time);
	TC_ASSERT_EQ("strftime", atoi(buffer), mktime((FAR struct tm *)&st_time));

	/* Check the second as a decimal number (range 00 to 60) */
	strftime(buffer, BUFF_SIZE, "%S", &st_time);
	TC_ASSERT_EQ("strftime", atoi(buffer), st_time.tm_sec);

	/* Check the tab character */
	strftime(buffer, BUFF_SIZE, "%t", &st_time);
	TC_ASSERT_EQ("strftime", strncmp(buffer, "\t", strlen("\t") + 1), 0);

	/* Check the year as a decimal number including the century */
	strftime(buffer, BUFF_SIZE, "%Y", &st_time);
	TC_ASSERT_EQ("strftime", atoi(buffer), st_time.tm_year + TM_YEAR_BASE);

	/* Check the '%' character */
	strftime(buffer, BUFF_SIZE, "%%", &st_time);
	TC_ASSERT_EQ("strftime", strncmp(buffer, "%", strlen("%") + 1), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn             :tc_libc_timer_strptime
* @brief          :strptime - Format string as time
* @Scenario       :strptime - converts time string as the time described in timeptr
*                  structure as per the format specifiers.
* API's covered   :strptime
* Preconditions   :none
* Postconditions  :none
* @return         :void
*/
static void tc_libc_timer_strptime(void)
{
	struct tm sp_tm;
	struct tm sf_tm;
	char *ret;

	sf_tm.tm_sec  = 13;
	sf_tm.tm_min  = 42;
	sf_tm.tm_hour = 12;
	sf_tm.tm_mday = 5;
	sf_tm.tm_mon  = 5;
	sf_tm.tm_year = 2018 - TM_YEAR_BASE;

	/* %b or %B or %h - The month name according to the current locale, in abbreviated form or the full name. */
	ret = strptime("Jun", "%B", &sp_tm);
	TC_ASSERT_NEQ("strptime", ret, NULL);
	TC_ASSERT_EQ("strptime", sp_tm.tm_mon, sf_tm.tm_mon);

	/* %b or %B or %h - The month name according to the current locale, in abbreviated form or the full name. */
	ret = strptime("Fail", "%B", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %C - The century number (0-99).
	 * %C does not modify tm structure, so just validating the return value..
	 */
	ret = strptime("Fail", "%C", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %D - Equivalent	to %m/%d/%y(month/day/year). */
	ret = strptime("Fail/05/18", "%D", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %n - The Enter Character.
	 * %n does not modify tm structure, so just validating the return value..
	 */
	ret = strptime("\n", "%n", &sp_tm);
	TC_ASSERT_NEQ("strptime", ret, NULL);

	/* %H - Equivalent to Hour
	 * %p - The locale's equivalent of AM or PM.
	 */
	ret = strptime("13 AM", "%H %p", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %H - Equivalent to Hour
	 * %p - The locale's equivalent of AM or PM.
	 */
	ret = strptime("12 AM", "%H %p", &sp_tm);
	TC_ASSERT_EQ("strptime", sp_tm.tm_hour, 0);

	/* %H - Equivalent to Hour
	 * %p - The locale's equivalent of AM or PM.
	 */
	ret = strptime("13 PM", "%H %p", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %H - Equivalent to Hour
	 * %p - The locale's equivalent of AM or PM.
	 */
	ret = strptime("11 PM", "%H %p", &sp_tm);
	TC_ASSERT_EQ("strptime", sp_tm.tm_hour, 23);

	/* %p - The locale's equivalent of AM or PM. */
	ret = strptime("FM", "%p", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %R - Equivalent to %H:%M (hour:minute). */
	ret = strptime("24:42", "%R", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %r - The 12-hour clock time (using the locale's AM or PM). */
	ret = strptime("Fail:42:13 PM", "%r", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %T - Equivalent to %H:%M:%S (hour/minute/second). */
	ret = strptime("12:42:Fail", "%T", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %U - The week number with Sunday the first day of the week (0-53). */
	ret = strptime("Fail", "%U", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %X - The time, using the locale's time format. */
	ret = strptime("10:Fail:13", "%X", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %x - The date, using the locale's date format. */
	ret = strptime("10/Fail/18", "%x", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %Y or %y - The year, including century (2018 = 2000(century) + 18(year)). */
	ret = strptime("Fail", "%Y", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %Y or %y - The year, including century (2018 = 2000(century) + 18(year)). */
	ret = strptime("Fail", "%y", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* %Y or %y - The year, including century (2018 = 2000(century) + 18(year)). */
	ret = strptime("80", "%y", &sp_tm);
	TC_ASSERT_NEQ("strptime", ret, NULL);
	TC_ASSERT_EQ("strptime", sp_tm.tm_year, 80 + 1900 - TM_YEAR_BASE);

	/* %C - The century number (0-99).
	 * %C does not modify tm structure, so just validating the return value..
	 * %Y or %y - The year, including century (2018 = 2000(century) + 18(year)).
	 */
	ret = strptime("10 80", "%C %y", &sp_tm);
	TC_ASSERT_NEQ("strptime", ret, NULL);
	TC_ASSERT_EQ("strptime", sp_tm.tm_year, 80 + 1000 - TM_YEAR_BASE);

	ret = strptime("test", "%%", &sp_tm);
	TC_ASSERT_EQ("strptime", ret, NULL);

	/* strptime() converts a string representation of time to a time tm structure,
	 * using the specified format. descrtiption of example formats are as shown below,
	 * %Y or %y - The year, including century (2018 = 2000(century) + 18(year)).
	 * %m - The month number.
	 * %d - The day of month (1-31).
	 * %k - The hour on a 24-hour clock (0-23).
	 * %M - The minute (0-59).
	 * %S - The second (0-60).
	 * %p - The locale's equivalent of AM or PM.
	 */
	memset(&sp_tm, 0, sizeof(struct tm));
	ret = strptime("2018-06-05 12:42:13 PM", "%Y-%m-%d %k:%M:%S %p", &sp_tm);
	TC_ASSERT_NEQ("strptime", ret, NULL);
	TC_ASSERT_EQ("strptime", sp_tm.tm_year, sf_tm.tm_year);
	TC_ASSERT_EQ("strptime", sp_tm.tm_mon, sf_tm.tm_mon);
	TC_ASSERT_EQ("strptime", sp_tm.tm_mday, sf_tm.tm_mday);
	TC_ASSERT_EQ("strptime", sp_tm.tm_hour, sf_tm.tm_hour);
	TC_ASSERT_EQ("strptime", sp_tm.tm_min, sf_tm.tm_min);
	TC_ASSERT_EQ("strptime", sp_tm.tm_sec, sf_tm.tm_sec);

	/* %U - The week number with Sunday the first day of the week (0-53).
	 *      The first Sunday of January is the first day of week 1.
	 * %b or %B or %h - The month name according to the current locale, in abbreviated form or the full name.
	 * %l or %I - The hour on a 12-hour clock (1-12).
	 */
	memset(&sp_tm, 0, sizeof(struct tm));
	ret = strptime("22 June 10 AM 18", "%U %B %l %p %Ey", &sp_tm);
	TC_ASSERT_NEQ("strptime", ret, NULL);
	TC_ASSERT_EQ("strptime", sp_tm.tm_year, sf_tm.tm_year);
	TC_ASSERT_EQ("strptime", sp_tm.tm_mon, sf_tm.tm_mon);
	TC_ASSERT_EQ("strptime", sp_tm.tm_hour, sf_tm.tm_hour - 2);

	/* %C - The century number (0-99).
	 * %C does not modify tm structure, so just validating the return value..
	 */
	ret = strptime("50", "%C", &sp_tm);
	TC_ASSERT_NEQ("strptime", ret, NULL);

	/* %D - Equivalent to %m/%d/%y(month/day/year).
	 * %X - The time, using the locale's time format.
	 */
	memset(&sp_tm, 0, sizeof(struct tm));
	ret = strptime("06/05/18 12:42:13", "%D %X", &sp_tm);
	TC_ASSERT_NEQ("strptime", ret, NULL);
	TC_ASSERT_EQ("strptime", sp_tm.tm_year, sf_tm.tm_year);
	TC_ASSERT_EQ("strptime", sp_tm.tm_mon, sf_tm.tm_mon);
	TC_ASSERT_EQ("strptime", sp_tm.tm_mday, sf_tm.tm_mday);
	TC_ASSERT_EQ("strptime", sp_tm.tm_hour, sf_tm.tm_hour);
	TC_ASSERT_EQ("strptime", sp_tm.tm_min, sf_tm.tm_min);
	TC_ASSERT_EQ("strptime", sp_tm.tm_sec, sf_tm.tm_sec);

	/* %x - The date, using the locale's date format.
	 * %R - Equivalent to %H:%M (hour:minute).
	 */
	memset(&sp_tm, 0, sizeof(struct tm));
	ret = strptime("06/05/18 12:42", "%x %R", &sp_tm);
	TC_ASSERT_NEQ("strptime", ret, NULL);
	TC_ASSERT_EQ("strptime", sp_tm.tm_year, sf_tm.tm_year);
	TC_ASSERT_EQ("strptime", sp_tm.tm_mon, sf_tm.tm_mon);
	TC_ASSERT_EQ("strptime", sp_tm.tm_mday, sf_tm.tm_mday);
	TC_ASSERT_EQ("strptime", sp_tm.tm_hour, sf_tm.tm_hour);
	TC_ASSERT_EQ("strptime", sp_tm.tm_min, sf_tm.tm_min);

	/* %T - Equivalent to %H:%M:%S (hour/minute/second). */
	memset(&sp_tm, 0, sizeof(struct tm));
	ret = strptime("12:42:13", "%T", &sp_tm);
	TC_ASSERT_NEQ("strptime", ret, NULL);
	TC_ASSERT_EQ("strptime", sp_tm.tm_hour, sf_tm.tm_hour);
	TC_ASSERT_EQ("strptime", sp_tm.tm_min, sf_tm.tm_min);
	TC_ASSERT_EQ("strptime", sp_tm.tm_sec, sf_tm.tm_sec);


	/* %r - The 12-hour clock time (using the locale's AM or PM).
	 * In the POSIX locale equivalent to %I:%M:%S %p.
	 */
	memset(&sp_tm, 0, sizeof(struct tm));
	ret = strptime("12:42:13 PM", "%r", &sp_tm);
	TC_ASSERT_NEQ("strptime", ret, NULL);
	TC_ASSERT_EQ("strptime", sp_tm.tm_hour, sf_tm.tm_hour);
	TC_ASSERT_EQ("strptime", sp_tm.tm_min, sf_tm.tm_min);
	TC_ASSERT_EQ("strptime", sp_tm.tm_sec, sf_tm.tm_sec);


#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	/* Tuesday (0-6: day of the week, week starts on Sunday)*/
	sf_tm.tm_wday = 2;
	memset(&sp_tm, 0, sizeof(struct tm));
	/* %a or %A - The weekday name according to the current locale,
	 * in abbreviated form or the full name.
	 * %j - The day number in the year (1-366).
	 * %w - The weekday number (0-6) with Sunday = 0.
	 */
	ret = strptime("Tue/Tuesday 187 2", "%a/%A %j %w", &sp_tm);
	TC_ASSERT_NEQ("strptime", ret, NULL);
	TC_ASSERT_EQ("strptime", sp_tm.tm_wday, sf_tm.tm_wday);

	/* %c - The date and time representation for the current locale. */
	memset(&sp_tm, 0, sizeof(struct tm));
	ret = strptime("Tue Jun 5 12:42:13 2018", "%c", &sp_tm);
	TC_ASSERT_NEQ("strptime", ret, NULL);
	TC_ASSERT_EQ("strptime", sp_tm.tm_year, sf_tm.tm_year);
	TC_ASSERT_EQ("strptime", sp_tm.tm_mon, sf_tm.tm_mon);
	TC_ASSERT_EQ("strptime", sp_tm.tm_mday, sf_tm.tm_mday);
	TC_ASSERT_EQ("strptime", sp_tm.tm_hour, sf_tm.tm_hour);
	TC_ASSERT_EQ("strptime", sp_tm.tm_min, sf_tm.tm_min);
	TC_ASSERT_EQ("strptime", sp_tm.tm_sec, sf_tm.tm_sec);
#endif

	/* Check with invalid param. it will returns NULl. */
	ret = strptime("", "%f", NULL);
	TC_ASSERT_EQ("strptime", ret, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_timer_time
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
	TC_ASSERT_NEQ("time", ret_time, (time_t)ERROR);

	/* get time through gmtime_r, localtime_r */

	gmtime_r(&get_time, &st_gmtime);
	localtime_r(&get_time, &st_localtime);
	TC_ASSERT_EQ("time", st_gmtime.tm_year, st_localtime.tm_year);
	TC_ASSERT_EQ("time", st_gmtime.tm_mon, st_localtime.tm_mon);

	TC_SUCCESS_RESULT();
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
		prev_month = clock_daysbeforemonth(month_iter - 1, FALSE);
		cur_month = clock_daysbeforemonth(month_iter, FALSE);
		TC_ASSERT_EQ("clock_daysbeforemonth", cur_month-prev_month, notleapyear_days[month_iter]);
	}

	/* test with leapyear */

	for (month_iter = 1; month_iter < 12; month_iter++) {
		prev_month = clock_daysbeforemonth(month_iter - 1, TRUE);
		cur_month = clock_daysbeforemonth(month_iter, TRUE);
		TC_ASSERT_EQ("clock_daysbeforemonth", cur_month-prev_month, leapyear_days[month_iter]);
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_timer_clock
* @brief                :Get an approximation of processor time used by the program
* @Scenario             :Returns the implementation's best approximation to the processor time
*						 used by the process since the beginning of a implementation-defined era
*						 related only to the process invocation.
*						 to get the number of seconds used, divide by CLOCKS_PER_SEC.
* API's covered         :clock
* Preconditions         :none
* Postconditions        :none
* @return               :Returns the CPU time used so far as a clock_t.
*						 If the processor time used is not available or its value cannot
*						 be represented, the function returns the value (clock_t) -1.
*/
static void tc_libc_timer_clock(void)
{
	clock_t ret_time = ERROR;

	ret_time = clock();
	TC_ASSERT_NEQ("clock", ret_time, 0);

	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_LIBM
static void tc_libc_timer_difftime(void)
{
	time_t t0;
	time_t t1;
#ifdef CONFIG_HAVE_DOUBLE
	double ret;
#else
	float ret;
#endif

	t0 = time(NULL);
	/* Time elapse of 1s */
	sleep(1);
	t1 = time(NULL);

	ret = difftime(t1, t0);

#ifdef CONFIG_HAVE_DOUBLE
	TC_ASSERT_LEQ("difftime", fabs(1.0 - ret), DBL_EPSILON);
#else
	TC_ASSERT_LEQ("difftime", fabsf(1.0f - ret), DBL_EPSILON);
#endif

	TC_SUCCESS_RESULT();
}
#endif

/****************************************************************************
 * Name: libc_timer
 ****************************************************************************/

int libc_timer_main(void)
{
	tc_libc_timer_clock();
	tc_libc_timer_clock_calendar2utc();
	tc_libc_timer_clock_daysbeforemonth();
	tc_libc_timer_clock_isleapyear();
#ifdef CONFIG_LIBM
	tc_libc_timer_difftime();
#endif
	tc_libc_timer_gmtime();
	tc_libc_timer_gmtime_r();
	tc_libc_timer_localtime();
	tc_libc_timer_localtime_r();
	tc_libc_timer_mktime();
	tc_libc_timer_strftime();
	tc_libc_timer_strptime();
	tc_libc_timer_time();

	return 0;
}
