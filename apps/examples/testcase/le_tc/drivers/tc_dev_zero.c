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

/// @file tc_dev_zero.c
/// @brief Test Case Example for dev_zero driver
#include <tinyara/config.h>
#include <stdio.h>
#include <fcntl.h>
#include "tc_internal.h"
#ifndef CONFIG_DISABLE_POLL
#include <poll.h>
#endif

#define cleanup(fd, buf) \
do {\
	close(fd);\
	free(buf);\
} while (0)

/**
* @fn                   :tc_driver_zero_test
* @brief                :Test the dev_zero driver api's
* @scenario             :Since the driver is extremely small, all tests are done here
* API's covered         :open, read, write, poll
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_zero_test(void)
{
	int fd = 0;
	int ret = 0;
	char *buf = malloc(100);

	TC_ASSERT_NEQ("dev_zero_main", buf, NULL);

	fd = open("/dev/zero", O_RDWR);
	TC_ASSERT_GT_CLEANUP("dev_zero_tc open", fd, 0, free(buf));

	ret = read(fd, buf, 100);
	TC_ASSERT_EQ_CLEANUP("devzero_read", ret, 100, cleanup(fd, buf));

	ret = write(fd, buf, 100);
	TC_ASSERT_EQ_CLEANUP("devzero_write", ret, 100, cleanup(fd, buf));

#ifndef CONFIG_DISABLE_POLL
	/* Test positive cases for poll */
	struct pollfd fds[1];
	fds[0].fd = fd;
	fds[0].events = POLLIN;
	ret = poll(fds, 1, 1000);
	TC_ASSERT_GEQ_CLEANUP("devzero_poll", ret, 0, cleanup(fd, buf));

	/* Test negative cases for poll */
	fds[0].events = 0;
	ret = poll(fds, 1, 1000);
	TC_ASSERT_GEQ_CLEANUP("devzero_poll", ret, 0, cleanup(fd, buf));
#endif
	free(buf);
	ret = close(fd);
	TC_ASSERT_GEQ("dev_zero_tc close", ret, 0);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: dev_zero driver test
 ****************************************************************************/
void dev_zero_main(void)
{
	tc_driver_zero_test();
	return;
}
