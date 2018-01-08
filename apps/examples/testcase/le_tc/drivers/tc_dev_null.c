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

/// @file tc_dev_null.c
/// @brief Test Case Example for dev_null driver
#include <tinyara/config.h>
#include <stdio.h>
#include <fcntl.h>
#include "tc_internal.h"
#ifndef CONFIG_DISABLE_POLL
#include <poll.h>
#endif

/**
* @fn                   :tc_driver_null_test
* @brief                :Test the null driver api's
* @scenario             :Since the driver is extremely small, all tests are done here
* API's covered         :open, read, write, poll
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_null_test(void)
{
	int fd = 0;
	int ret = 0;

	fd = open("/dev/null", O_RDWR);
	TC_ASSERT_GT("dev_null_tc", fd, 0);

	ret = read(fd, NULL, 0);
	TC_ASSERT_EQ_CLEANUP("devnull_read", ret, 0, close(fd));

	ret = write(fd, NULL, 1);
	TC_ASSERT_EQ_CLEANUP("devnull_write", ret, 1, close(fd));

#ifndef CONFIG_DISABLE_POLL
	/* Test positive cases for poll */
	struct pollfd fds[1];
	fds[0].fd = fd;
	fds[0].events = POLLIN;
	ret = poll(fds, 1, 1000);
	TC_ASSERT_GEQ_CLEANUP("devnull_poll", ret, 0, close(fd));

	/* Test negative cases for poll */
	fds[0].events = 0;
	ret = poll(fds, 1, 1000);
	TC_ASSERT_GEQ_CLEANUP("devnull_poll", ret, 0, close(fd));
#endif
	ret = close(fd);
	TC_ASSERT_GEQ("dev_null_tc", ret, 0);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: dev_null driver test
 ****************************************************************************/
void dev_null_main(void)
{
	tc_driver_null_test();

	return;
}
