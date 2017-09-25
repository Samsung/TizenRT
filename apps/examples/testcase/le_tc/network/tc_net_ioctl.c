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
* @testcase            :tc_net_ioctl_p
* @brief               :manipulates the set/clear non-blocking i/o for socket files.
* @scenario            :ioctlsocket api control files with request code.
* @apicovered          :socket, ioctlsocket, close
* @precondition        :fd must be an open file descriptor.
* @postcondition       :none
* @return              :void
*/
static void tc_net_ioctl_p(void)
{
	long val = 0;
	int fd;

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	fd = ioctlsocket(sock, FIONBIO, &val);
	TC_ASSERT_NEQ_CLEANUP("ioctlsocket", fd, NEG_VAL, close(sock));
	close(sock);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_ioctl_fionread_n
* @brief               :manipulates the set/clear non-blocking i/o for socket files.
* @scenario            :ioctlsocket api control files with request code.
* @apicovered          :socket,ioctlsocket, close
* @precondition        :fd must be an open file descriptor.
* @postcondition       :none
* @return              :void
*/
static void tc_net_ioctl_fionread_p(void)
{
	long val = 10;
	int fd;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	fd = ioctlsocket(sock, FIONREAD, &val);
	TC_ASSERT_NEQ_CLEANUP("ioctlsocket", fd, NEG_VAL, close(sock));
	close(sock);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_ioctl_n
* @brief               :manipulates the set/clear non-blocking i/o for socket files.
* @scenario            :ioctlsocket api control files with request code.
* @apicovered          :ioctlsocket
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_ioctl_n(void)
{
	int ret = ioctlsocket(-1, FIONBIO, 0);

	TC_ASSERT_EQ("ioctlsocket", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name:ioctl()
 ****************************************************************************/
int net_ioctl_main(int s)
{
	tc_net_ioctl_p();
	tc_net_ioctl_fionread_p();
	tc_net_ioctl_n();
	return 0;
}
