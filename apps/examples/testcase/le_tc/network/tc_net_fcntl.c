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

/// @file tc_net_fcntl.c
/// @brief Test Case Example for fcntl() API
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netutils/netlib.h>

#include <sys/socket.h>

#include "tc_internal.h"

/**
* @testcase            : tc_net_fcntl_nonblock_p
* @brief               : This fcntl API manipulate file descriptor.
* @scenario            : fcntl use setfl command to set the file status flags to the value specified by arg.
* @apicovered          : fcntl()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_fcntl_nonblock_p(int fd)
{
	int ret = fcntl(fd, F_SETFL, O_NONBLOCK);

	TC_ASSERT_NEQ("fcntl", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_fcntl_p
* @brief               : This fcntl API manipulate file descriptor.
* @scenario            : fcntl use setfl command to set the file status flags to the value specified by arg as zero.
* @apicovered          : fcntl()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_fcntl_p(int fd)
{
	int ret = fcntl(fd, F_SETFL, ZERO);

	TC_ASSERT_NEQ("fcntl", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_fcntl_n
* @brief               : This fcntl API manipulate file descriptor.
* @scenario            : fcntl use setfl command to set the file status flags with invalid fd.
* @apicovered          : fcntl()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_fcntl_n(void)
{
	int ret = fcntl(NEG_VAL, F_SETFL, O_NONBLOCK);

	TC_ASSERT_NEQ("fcntl", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_fcntl_ndelay_p
* @brief               : This fcntl API manipulate file descriptor.
* @scenario            : fcntl use setfl command to set the file status flags to the value specified by arg as ndelay.
* @apicovered          : fcntl()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_fcntl_ndelay_p(int fd)
{
	int ret = fcntl(fd, F_SETFL, O_NDELAY);

	TC_ASSERT_NEQ("fcntl", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: fcntl()
 ****************************************************************************/

int net_fcntl_main(int fd)
{
	tc_net_fcntl_nonblock_p(fd);
	tc_net_fcntl_p(fd);
	tc_net_fcntl_n();
	tc_net_fcntl_ndelay_p(fd);

	return 0;
}
