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

/// @file tc_adc.c
/// @brief Test Case Example for adc driver
#include <tinyara/analog/adc.h>
#include <tinyara/analog/ioctl.h>
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
* @fn                   :tc_driver_adc_open_close
* @brief                :Test the adc driver open and close
* @scenario             :Test the adc driver open and close
* API's covered         :open, close
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_adc_open_close(void)
{
	int fd = 0;
	int ret = 0;

	/* Positive test cases */
	fd = open("/dev/adc0", O_RDONLY);
	TC_ASSERT_GT("adc_open", fd, 0);

	ret = close(fd);
	TC_ASSERT_EQ("adc_close", ret, OK);

	/* Negative test cases */
	int count = 0;
	int fds[255] = { 0, };
	for (count = 0; count < 255; count++) {
		fds[count] = open("/dev/adc0", O_RDONLY);
		if (fds[count] < 0) {
			TC_ASSERT_EQ_CLEANUP("adc_open", errno, EMFILE, close_fds(fds, --count));
			break;
		}
	}

	close_fds(fds, --count);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_adc_read
* @brief                :Test the adc driver read
* @scenario             :Test the adc driver read
* API's covered         :read, ioctl
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_adc_read(void)
{
	int fd = 0;
	int ret = 0;
	char buf[5];
	int i;

	fd = open("/dev/adc0", O_RDONLY);
	TC_ASSERT_GT("adc_open", fd, 0);

	ret = ioctl(fd, ANIOC_TRIGGER, 0);
	TC_ASSERT_GEQ_CLEANUP("adc_ioctl", ret, 0, close(fd));

	for (i = 0; i < 6; i++) {
		ret = read(fd, buf, i);
		TC_ASSERT_GEQ_CLEANUP("adc_read", ret, 0, close(fd));
	}

	close(fd);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: adc driver test
 ****************************************************************************/
void adc_main(void)
{
	tc_driver_adc_open_close();
	tc_driver_adc_read();

	return;
}
