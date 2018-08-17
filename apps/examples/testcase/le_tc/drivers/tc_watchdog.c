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

/// @file tc_watchdog.c
/// @brief Test Case Example for watchdog driver
#include <tinyara/watchdog.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "tc_internal.h"
#include <errno.h>

/**
 * @brief Closes all open files
 */
static inline void close_fds(int *fds, int count)
{
	for (; count >= 0; count--) {
		close(fds[count]);
	}
}

/**
* @fn                   :tc_driver_watchdog_open_close
* @brief                :Test the watchdog driver open and close
* @scenario             :Test the watchdog driver open and close
* API's covered         :open, close
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_watchdog_open_close(void)
{
	int fd = 0;
	int ret = 0;

	fd = open("/dev/watchdog0", O_RDWR);
	TC_ASSERT_GT("watchdog_open", fd, 0);

	ret = close(fd);
	TC_ASSERT_EQ("watchdog_close", ret, OK);

	/* Negative test cases */
	int count = 0;
	int fds[255] = { 0, };
	for (count = 0; count < 255; count++) {
		fds[count] = open("/dev/watchdog0", O_RDWR);
		if (fds[count] < 0) {
			TC_ASSERT_EQ_CLEANUP("watchdog_open", errno, EMFILE, close_fds(fds, --count));
			break;
		}
	}

	close_fds(fds, --count);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_watchdog_read_write
* @brief                :Test the watchdog driver read and write
* @scenario             :Test the watchdog driver read and write
* API's covered         :read, write
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_watchdog_read_write(void)
{
	int fd = 0;
	int ret = 0;

	fd = open("/dev/watchdog0", O_RDWR);
	TC_ASSERT_GT("watchdog_open", fd, 0);

	ret = read(fd, NULL, 0);
	TC_ASSERT_EQ_CLEANUP("watchdog_read", ret, 0, close(fd));

	ret = write(fd, NULL, 1);
	TC_ASSERT_EQ_CLEANUP("watchdog_write", ret, 0, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_watchdog_ioctl
* @brief                :Test the watchdog driver ioctl
* @scenario             :Test the watchdog driver ioctl
* API's covered         :ioctl
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_watchdog_ioctl(void)
{
	int fd = 0;
	int ret = 0;

	fd = open("/dev/watchdog0", O_RDWR);
	TC_ASSERT_GT("watchdog_open", fd, 0);

	ret = ioctl(fd, WDIOC_SETTIMEOUT, 1000);
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl", ret, OK, close(fd));

	ret = ioctl(fd, WDIOC_START, 0);
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl", ret, OK, close(fd));

	FAR struct watchdog_status_s status;
	ret = ioctl(fd, WDIOC_GETSTATUS, (unsigned long)&status);
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl", ret, OK, close(fd));
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl", status.flags & WDFLAGS_ACTIVE, WDFLAGS_ACTIVE, close(fd));
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl", status.timeout, 1000, close(fd));

	/* Negative testcase for WDIOC_GETSTATUS */
	ret = ioctl(fd, WDIOC_GETSTATUS, 0UL);
	TC_ASSERT_LT_CLEANUP("watchdog_ioctl", ret, 0, close(fd));

	FAR struct watchdog_capture_s cap;
	ret = ioctl(fd, WDIOC_CAPTURE, (unsigned long)&cap);
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl", ret, OK, close(fd));

	/* Negative testcase for WDIOC_CAPTURE */
	ret = ioctl(fd, WDIOC_CAPTURE, 0UL);
	TC_ASSERT_LT_CLEANUP("watchdog_ioctl", ret, 0, close(fd));

	ret = ioctl(fd, WDIOC_KEEPALIVE, 0UL);
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl", ret, OK, close(fd));

	ret = ioctl(fd, WDIOC_STOP, 0UL);
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl", ret, OK, close(fd));

	ret = ioctl(fd, -1, 0UL);
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl", ret, OK, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: watchdog driver test
 ****************************************************************************/
void watchdog_main(void)
{
	tc_driver_watchdog_open_close();
	tc_driver_watchdog_read_write();
	tc_driver_watchdog_ioctl();

	return;
}
