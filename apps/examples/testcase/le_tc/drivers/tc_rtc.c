/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/// @file tc_rtc.c
/// @brief Test Case Example for rtc driver
#include <tinyara/config.h>
#include <tinyara/rtc.h>
#include <tinyara/time.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "tc_internal.h"
#include <tinyara/sched.h>

#ifdef CONFIG_RTC_ALARM
#define SIG 1
#define PRIORITY 100
#define STACKSIZE 1024
#define RTC_ALARM_ID 0
static bool alarm_received[CONFIG_RTC_NALARMS];
#endif

/**
* @fn                   :tc_driver_rtc_open_close
* @brief                :Test the rtc driver open and close
* @scenario             :Test the rtc driver open and close
* API's covered         :open, close
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
static void tc_driver_rtc_open_close(void)
{
	int fd = 0;
	int ret = 0;

	fd = open("/dev/rtc0", O_RDWR);
	TC_ASSERT_GT("rtc_open", fd, 0);

	ret = close(fd);
	TC_ASSERT_GEQ("rtc_close", ret, 0);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @fn                   :tc_driver_rtc_read_write
* @brief                :Test the rtc driver read and write
* @scenario             :Test the rtc driver read and write
* API's covered         :read, write
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_rtc_read_write(void)
{
	int fd = 0;
	int ret = 0;

	fd = open("/dev/rtc0", O_RDWR);
	TC_ASSERT_GT("rtc_open", fd, 0);

	ret = read(fd, NULL, 0);
	TC_ASSERT_EQ_CLEANUP("rtc_read", ret, 0, close(fd));

	ret = write(fd, NULL, 1);
	TC_ASSERT_EQ_CLEANUP("rtc_write", ret, 1, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_rtc_ioctl
* @brief                :Test the rtc driver ioctl
* @scenario             :Test the rtc driver ioctl
* API's covered         :ioctl, unlink
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_rtc_ioctl(void)
{
	int fd = 0;
	int ret = 0;
	FAR struct rtc_time rtctime_s;
	FAR struct rtc_time rtctime_r;

	/* Positive test cases */
	fd = open("/dev/rtc0", O_RDWR);
	TC_ASSERT_GT("rtc_open", fd, 0);

	memset(&rtctime_s, 0, sizeof(struct rtc_time));
	memset(&rtctime_r, 0, sizeof(struct rtc_time));

	rtctime_s.tm_hour = 1;
	rtctime_s.tm_min = 2;
	rtctime_s.tm_sec = 3;
	rtctime_s.tm_mday = 1;
	rtctime_s.tm_mon = 2;
	rtctime_s.tm_year = 71;

	ret = ioctl(fd, RTC_SET_TIME, (unsigned long)&rtctime_s);
	TC_ASSERT_GEQ_CLEANUP("rtc_ioctl", ret, 0, close(fd));

	ret = ioctl(fd, RTC_RD_TIME, (unsigned long)&rtctime_r);
	TC_ASSERT_GEQ_CLEANUP("rtc_ioctl", ret, 0, close(fd));
	TC_ASSERT_EQ_CLEANUP("rtc_ioctl", rtctime_r.tm_hour, rtctime_s.tm_hour, close(fd));
	TC_ASSERT_EQ_CLEANUP("rtc_ioctl", rtctime_r.tm_min, rtctime_s.tm_min, close(fd));
	TC_ASSERT_EQ_CLEANUP("rtc_ioctl", rtctime_r.tm_sec, rtctime_s.tm_sec, close(fd));
	TC_ASSERT_EQ_CLEANUP("rtc_ioctl", rtctime_r.tm_mday, rtctime_s.tm_mday, close(fd));
	TC_ASSERT_EQ_CLEANUP("rtc_ioctl", rtctime_r.tm_mon, rtctime_s.tm_mon, close(fd));

	/* If the year according to the RTC's Epoch is less than 1970,
	 *  it is assumed to be 100 years later, that is,between 2000 and 2069.
	 *  Refer : https://man7.org/linux/man-pages/man4/rtc.4.html */
	if (rtctime_s.tm_year < EPOCH_YEAR - TM_YEAR_BASE) {
		TC_ASSERT_EQ_CLEANUP("rtc_ioctl", rtctime_r.tm_year, rtctime_s.tm_year + 100, close(fd));
	} else {
		TC_ASSERT_EQ_CLEANUP("rtc_ioctl", rtctime_r.tm_year, rtctime_s.tm_year, close(fd));
	}

	/* Negative test cases */
	ret = ioctl(fd, -1, 0);
	TC_ASSERT_LT_CLEANUP("rtc_ioctl", ret, 0, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("rtc_close", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_rtc_alarm_ioctl
* @brief                :Test the rtc alarm driver ioctl
* @scenario             :Test the rtc alarm driver ioctl
* API's covered         :ioctl's set alarm, set relative, cancel alarm
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
#ifdef CONFIG_RTC_ALARM

/****************************************************************************
 * Name: alarm_handler
 ****************************************************************************/

static void alarm_handler(int signo, FAR siginfo_t *info, FAR void *ucontext)
{
	int almndx = info->si_value.sival_int;
	if (almndx >= 0 && almndx < CONFIG_RTC_NALARMS) {
		alarm_received[almndx] = true;
	}
}

static void tc_driver_rtc_alarm_ioctl(void)
{
	int fd = 0;
	int ret = 0;
	int seconds = 2;
	FAR struct sigaction act;
	FAR struct rtc_setalarm_s setalarm;
	FAR	struct rtc_setrelative_s setrel;
	sigset_t set;

	fd = open("/dev/rtc0", O_RDWR);
	TC_ASSERT_GT("rtc_open", fd, 0);

	sigemptyset(&set);
	sigaddset(&set, SIG);

	ret = sigprocmask(SIG_UNBLOCK, &set, NULL);
	TC_ASSERT_EQ("sigprocmask", ret, OK);
	act.sa_sigaction = alarm_handler;
	act.sa_flags = SA_SIGINFO;
	sigfillset(&act.sa_mask);
	sigdelset(&act.sa_mask, SIG);
	ret = sigaction(SIG, &act, NULL);
	TC_ASSERT_EQ("signaction", ret, OK);

	setrel.id = RTC_ALARM_ID;
	setrel.pid = 0;
	setrel.reltime = (time_t)seconds;
	setrel.signo = SIG;
	setrel.sigvalue.sival_int = RTC_ALARM_ID;

	ret = ioctl(fd, RTC_SET_RELATIVE, (unsigned long)((uintptr_t)&setrel));
	TC_ASSERT_GEQ_CLEANUP("rtc_ioctl", ret, 0, close(fd));
	sleep(seconds + 1);
	TC_ASSERT_EQ_CLEANUP("rtc_ioctl", alarm_received[setrel.sigvalue.sival_int], true, close(fd));

	alarm_received[setrel.sigvalue.sival_int] = false;
	ret = ioctl(fd, RTC_SET_RELATIVE, (unsigned long)((uintptr_t)&setrel));
	TC_ASSERT_GEQ_CLEANUP("rtc_ioctl", ret, 0, close(fd));

	ret = ioctl(fd, RTC_CANCEL_ALARM, (unsigned long)RTC_ALARM_ID);
	TC_ASSERT_GEQ_CLEANUP("rtc_ioctl", ret, 0, close(fd));
	sleep(seconds + 1);
	TC_ASSERT_EQ_CLEANUP("rtc_ioctl", alarm_received[setrel.sigvalue.sival_int], false, close(fd));

	setalarm.id = RTC_ALARM_ID;
	setalarm.pid = 0;
	setalarm.signo = SIG;
	setalarm.sigvalue.sival_int = RTC_ALARM_ID;

	ret = ioctl(fd, RTC_RD_TIME, (unsigned long)&setalarm.time);
	TC_ASSERT_GEQ_CLEANUP("rtc_ioctl", ret, 0, close(fd));

	if (setalarm.time.tm_sec + seconds >= 60) {
		setalarm.time.tm_sec = (setalarm.time.tm_sec + seconds) % 60;
		setalarm.time.tm_min += 1;
	} else {
		setalarm.time.tm_sec += (int)seconds;
	}

	ret = ioctl(fd, RTC_SET_ALARM, (unsigned long)((uintptr_t)&setalarm));
	TC_ASSERT_GEQ_CLEANUP("rtc_ioctl", ret, 0, close(fd));
	sleep(seconds + 1);
	TC_ASSERT_EQ_CLEANUP("rtc_ioctl", alarm_received[setalarm.sigvalue.sival_int], true, close(fd));
	alarm_received[setrel.sigvalue.sival_int] = false;

	ret = ioctl(fd, RTC_SET_ALARM, (unsigned long)((uintptr_t)&setalarm));
	TC_ASSERT_GEQ_CLEANUP("rtc_ioctl", ret, 0, close(fd));

	ret = ioctl(fd, RTC_CANCEL_ALARM, (unsigned long)RTC_ALARM_ID);
	TC_ASSERT_GEQ_CLEANUP("rtc_ioctl", ret, 0, close(fd));
	sleep(seconds + 1);
	TC_ASSERT_EQ_CLEANUP("rtc_ioctl", alarm_received[setalarm.sigvalue.sival_int], false, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("rtc_close", ret, OK);

	TC_SUCCESS_RESULT();

}
#endif

/****************************************************************************
 * Name: rtc driver test
 ****************************************************************************/
void rtc_driver_main(void)
{
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
	tc_driver_rtc_open_close();
#endif
	tc_driver_rtc_read_write();
	tc_driver_rtc_ioctl();
#ifdef CONFIG_RTC_ALARM
	tc_driver_rtc_alarm_ioctl();
#endif

	return;
}
