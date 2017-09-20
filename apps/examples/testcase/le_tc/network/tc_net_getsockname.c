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

/// @file tc_net_getsockname.c
/// @brief Test Case Example for getsockname() API
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
* @testcase            : tc_net_getsockname_p
* @brief               : The getsockname API get socket name.
* @scenario            : getsockname gives the current address to which the socket is bound.
* @apicovered          : getsockname()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockname_p(int sock)
{
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	int ret = getsockname(sock, &foo, (socklen_t *)&len);

	TC_ASSERT_NEQ("getsockname", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getsockname_unix_p
* @brief               : The getsockname API get socket name.
* @scenario            : getsockname gives the current address to which the socket is bound.
* @apicovered          : getsockname()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockname_unix_p(int sock)
{
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	int ret = getsockname(sock, &foo, (socklen_t *)&len);

	TC_ASSERT_NEQ("getsockname", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getsockname_n
* @brief               : The getsockname API get socket name.
* @scenario            : getsockname gives the current address to which the socket is bound,
                         with zero addrlen.
* @apicovered          : getsockname()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockname_n(void)
{
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	int ret = getsockname(NEG_VAL, &foo, (socklen_t *)&len);

	TC_ASSERT_NEQ("getsockname", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getsockname_close_n
* @brief               : The getsockname API get socket name.
* @scenario            : getsockname gives the current address to which the socket is bound,
                         with invalid addrlen.
* @apicovered          : getsockname()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockname_close_n(void)
{
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	int ret = getsockname(NEG_VAL, &foo, (socklen_t *)&len);

	TC_ASSERT_EQ("getsockname", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getsockname_udp_p
* @brief               : The getsockname API get socket name.
* @scenario            : getsockname gives the current address to which the socket is bound.
* @apicovered          : getsockname()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockname_udp_p(int sock)
{
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	int ret = getsockname(sock, &foo, (socklen_t *)&len);

	TC_ASSERT_NEQ("getsockname", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getsockname_icmp_p
* @brief               : The getsockname API get socket name.
* @scenario            : getsockname gives the current address to which the socket is bound.
* @apicovered          : getsockname()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockname_icmp_p(int sock)
{
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	int ret = getsockname(sock, &foo, (socklen_t *)&len);

	TC_ASSERT_NEQ("getsockname", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getsockname_len_sock_n
* @brief               : The getsockname API get socket name.
* @scenario            : getsockname gives the current address to which the socket is bound.
* @apicovered          : getsockname()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockname_len_sock_n(void)
{
	int sock;
	int len = NEG_VAL;
	struct sockaddr foo;

	sock = NEG_VAL;
	int ret = getsockname(sock, &foo, (socklen_t *)&len);

	TC_ASSERT_NEQ("getsockname", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: getsockname()
 ****************************************************************************/
int net_getsockname_main(void)
{
	int s = socket(AF_INET, SOCK_STREAM, ZERO);
	tc_net_getsockname_p(s);
	tc_net_getsockname_n();
	tc_net_getsockname_len_sock_n();
	tc_net_getsockname_udp_p(s);
	tc_net_getsockname_icmp_p(s);
	tc_net_getsockname_close_n();
	tc_net_getsockname_unix_p(s);
	close(s);
	return 0;
}
