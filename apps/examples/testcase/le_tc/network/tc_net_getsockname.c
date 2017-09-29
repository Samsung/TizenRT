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
* @testcase            :tc_net_getsockname_p
* @brief               :get the socket name.
* @scenario            :getsockname gives the current address to which the socket is bound.
* @apicovered          :getsockname
* @precondition        :socket file descriptor.
* @postcondition       :none
* @return              :void
*/
static void tc_net_getsockname_p(int sock)
{
	int ret;
	struct sockaddr foo;
	int len = sizeof(struct sockaddr);

	ret = getsockname(sock, &foo, (socklen_t *)&len);

	TC_ASSERT_NEQ("getsockname", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_getsockname_unix_p
* @brief               :get the socket name.
* @scenario            :getsockname gives the current address to which the socket is bound.
* @apicovered          :getsockname
* @precondition        :socket file descriptor.
* @postcondition       :none
* @return              :void
*/
static void tc_net_getsockname_unix_p(int sock)
{
	struct sockaddr foo;
	int len = sizeof(struct sockaddr);

	int ret = getsockname(sock, &foo, (socklen_t *)&len);

	TC_ASSERT_NEQ("getsockname", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_getsockname_n
* @brief               :get the socket name.
* @scenario            :getsockname gives the current address to which the socket is bound,
*                       with invalid socket fd.
* @apicovered          :getsockname
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_getsockname_n(void)
{
	struct sockaddr foo;
	int len = sizeof(struct sockaddr);

	int ret = getsockname(NEG_VAL, &foo, (socklen_t *)&len);

	TC_ASSERT_EQ("getsockname", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_getsockname_close_n
* @brief               :get the socket name.
* @scenario            :getsockname gives the current address to which the socket is bound.
* @apicovered          :getsockname
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_getsockname_close_n(void)
{
	struct sockaddr foo;
	int len = sizeof(struct sockaddr);

	int ret = getsockname(NEG_VAL, &foo, (socklen_t *)&len);

	TC_ASSERT_EQ("getsockname", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_getsockname_udp_p
* @brief               :get the socket name.
* @scenario            :getsockname gives the current address to which the socket is bound.
* @apicovered          :getsockname
* @precondition        :socket file descriptor.
* @postcondition       :none
* @return              :void
*/
static void tc_net_getsockname_udp_p(int sock)
{
	struct sockaddr foo;
	int len = sizeof(struct sockaddr);

	int ret = getsockname(sock, &foo, (socklen_t *)&len);

	TC_ASSERT_NEQ("getsockname", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_getsockname_icmp_p
* @brief               :get the socket name.
* @scenario            :getsockname gives the current address to which the socket is bound.
* @apicovered          :getsockname
* @precondition        :socket file descriptor.
* @postcondition       :none
* @return              :void
*/
static void tc_net_getsockname_icmp_p(int sock)
{
	struct sockaddr foo;
	int len = sizeof(struct sockaddr);

	int ret = getsockname(sock, &foo, (socklen_t *)&len);

	TC_ASSERT_NEQ("getsockname", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_getsockname_len_sock_n
* @brief               :get the socket name.
* @scenario            :getsockname gives the current address to which the socket is bound,
*                       with invalid length argument.
* @apicovered          :getsockname
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_getsockname_len_sock_n(void)
{
	struct sockaddr foo;
	int fd = -1;

	int ret = getsockname(fd, &foo, NULL);

	TC_ASSERT_EQ("getsockname", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name:net_getsockname_main
 ****************************************************************************/
int net_getsockname_main(void)
{
	int tcp_sock = socket(AF_INET, SOCK_STREAM, ZERO);
	int udp_sock = socket(AF_INET, SOCK_DGRAM, ZERO);
	int unix_sock = socket(AF_UNIX, SOCK_STREAM, ZERO);

	tc_net_getsockname_p(tcp_sock);
	tc_net_getsockname_n();
	tc_net_getsockname_len_sock_n();
	tc_net_getsockname_udp_p(udp_sock);
	tc_net_getsockname_icmp_p(tcp_sock);
	tc_net_getsockname_close_n();
	tc_net_getsockname_unix_p(unix_sock);

	close(tcp_sock);
	close(udp_sock);
	close(unix_sock);
	return 0;
}
