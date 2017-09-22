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

/// @file tc_net_ioctl.c
/// @brief Test Case Example for ioctl() API
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

extern int ioctlsocket(int s, long cmd, void *argp);

/**
* @testcase            : tc_net_ioctl_p
* @brief               : This function manipulates the underlying device parameters of special files.
* @scenario            : ioctlsocket api control files with device-dependent request code.
* @apicovered          : ioctl()
* @precondition        : fd must be an open file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_ioctl_p(void)
{
	long val = 0;
	int fd;

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	fd = ioctlsocket(sock, FIONBIO, &val);
	close(sock);
	TC_ASSERT_NEQ("tc_net_ioctl_p", fd, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_ioctl_fionread_n
* @brief               : This function manipulates the underlying device parameters of special files.
* @scenario            : ioctlsocket api control files with device-dependent request code.
* @apicovered          : ioctl()
* @precondition        : fd must be an open file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_ioctl_fionread_n(void)
{
	long val = 10;
	int fd;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	fd = ioctlsocket(sock, FIONREAD, &val);
	close(sock);
	TC_ASSERT_NEQ("tc_net_ioctl_fionread_n", fd, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_ioctl_n
* @brief               : This function manipulates the underlying device parameters of special files
* @scenario            : ioctlsocket api control files with device-dependent request code.
* @apicovered          : ioctl()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_ioctl_n(void)
{
	int ret = ioctlsocket(NEG_VAL, FIONBIO, ZERO);

	TC_ASSERT_EQ("tc_net_ioctl_n", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: ioctl()
 ****************************************************************************/

int net_ioctl_main(int s)
{
	tc_net_ioctl_p();
	tc_net_ioctl_fionread_n();
	tc_net_ioctl_n();
	return 0;
}
