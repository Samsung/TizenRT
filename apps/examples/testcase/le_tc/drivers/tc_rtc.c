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
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "tc_internal.h"

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

	rtctime_s.tm_hour = 1;
	rtctime_s.tm_min = 2;
	rtctime_s.tm_sec = 3;

	ret = ioctl(fd, RTC_SET_TIME, (unsigned long)&rtctime_s);
	TC_ASSERT_GEQ_CLEANUP("rtc_ioctl", ret, 0, close(fd));

	ret = ioctl(fd, RTC_RD_TIME, &rtctime_r);
	TC_ASSERT_GEQ_CLEANUP("rtc_ioctl", ret, 0, close(fd));
	TC_ASSERT_EQ_CLEANUP("rtc_ioctl", rtctime_r.tm_hour, rtctime_s.tm_hour, close(fd));
	TC_ASSERT_EQ_CLEANUP("rtc_ioctl", rtctime_r.tm_min, rtctime_s.tm_min, close(fd));
	TC_ASSERT_EQ_CLEANUP("rtc_ioctl", rtctime_r.tm_sec, rtctime_s.tm_sec, close(fd));

	/* Negative test cases */
	ret = ioctl(fd, -1, 0);
	TC_ASSERT_LT_CLEANUP("rtc_ioctl", ret, 0, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("rtc_close", ret, OK);

	ret = unlink("/dev/rtc0");
	TC_ASSERT_EQ("rtc_unlink", ret, OK);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: rtc driver test
 ****************************************************************************/
void rtc_main(void)
{
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
	tc_driver_rtc_open_close();
#endif
	tc_driver_rtc_read_write();
	tc_driver_rtc_ioctl();

	return;
}
