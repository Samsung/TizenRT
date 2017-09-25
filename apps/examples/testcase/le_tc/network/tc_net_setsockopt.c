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

/// @file tc_net_setsockopt.c
/// @brief Test Case Example for setsockopt() API
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
* @testcase            : tc_net_setsockopt_multicast_tcp_nodelay_p
* @brief               : set the socket options with option name as tcp nodelay.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_multicast_tcp_nodelay_p(int s)
{
	int ret;
	int option = 1;

	ret = setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &option, sizeof(option));
	TC_ASSERT_NEQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_setsockopt_multicast_tcp_keepalive_p
* @brief               : set the socket options with option name as tcp keepalive.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_multicast_tcp_keepalive_p(int s)
{
	int ret;
	int option = 1;

	ret = setsockopt(s, IPPROTO_TCP, TCP_KEEPALIVE, &option, sizeof(option));
	TC_ASSERT_NEQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_setsockopt_multicast_tcp_keepidle_p
* @brief               : set the socket options with option name as tcp keepidle.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_multicast_tcp_keepidle_p(int s)
{
	int ret;
	int keepidle = 30;

	ret = setsockopt(s, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle));
	TC_ASSERT_NEQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_setsockopt_multicast_tcp_keepintvl_p
* @brief               : set socket options with option name as tcp keepintvl.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_multicast_tcp_keepintvl_p(int s)
{
	int ret;
	int keepintvl = 120;

	ret = setsockopt(s, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl));
	TC_ASSERT_NEQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_setsockopt_multicast_tcp_keepcnt_p
* @brief               : set socket options with option name as tcp keepcnt.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_multicast_tcp_keepcnt_p(int s)
{
	int ret;
	int keepcnt = 5;

	ret = setsockopt(s, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt));

	TC_ASSERT_NEQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_setsockopt_ip_tos_p
* @brief               : set socket options with option name as ip tos.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_ip_tos_p(int s)
{
	int ret;
	int optval = 1;

	ret = setsockopt(s, IPPROTO_IP, IP_TOS, &optval, sizeof(optval));

	TC_ASSERT_NEQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_setsockopt_broadcast_p
* @brief               : set socket options with option name as broadcast.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_broadcast_p(int s)
{
	int ret;
	int optval = 1;				//broadcast value

	ret = setsockopt(s, SOL_SOCKET, SO_BROADCAST, &optval, sizeof optval);
	TC_ASSERT_NEQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_setsockopt_rcvtimo_p
* @brief               : set socket options with option name as rcvtimeo.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_rcvtimo_p(int s)
{
	int ret;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	ret = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));
	TC_ASSERT_NEQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_setsockopt_reuseaddr_p
* @brief               : set socket options with option name as reuseaddr.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_reuseaddr_p(int s)
{
	int ret;
	int option = 1;

	ret = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof option);
	TC_ASSERT_NEQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_setsockopt_reuseport_p
* @brief               : set socket options with option name as reuseport.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_reuseport_p(int s)
{
	int ret;
	int option = 1;

	ret = setsockopt(s, SOL_SOCKET, SO_REUSEPORT, (char *)&option, sizeof option);
	TC_ASSERT_NEQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_setsockopt_keepalive_p
* @brief               : set socket options with option name as keepalive.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_keepalive_p(int s)
{
	int ret;
	int optval = 1;

	ret = setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof optval);
	TC_ASSERT_NEQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_setsockopt_bad_filedesc_n
* @brief               : set the socket options.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_bad_filedesc_n(void)
{
	int ret;

	ret = setsockopt(NEG_VAL, SOL_SOCKET, 0, 0, 0);
	TC_ASSERT_EQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_setsockopt_invalid_filedesc_n
* @brief               : set the socket options.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_invalid_filedesc_n(void)
{
	int ret;

	ret = setsockopt(0, SOL_SOCKET, 0, 0, 0);
	TC_ASSERT_EQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_setsockopt_invalid_level_n
* @brief               : set the socket options.
* @scenario            : setsockopt() function set the option specified by the option name argument, at the protocol
                         level specified by the level argument, to the value pointed to by the option value argument.
* @apicovered          : setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_setsockopt_invalid_level_n(int s)
{
	int ret;

	ret = setsockopt(s, NEG_VAL, 0, 0, 0);
	TC_ASSERT_EQ("setsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: setsockopt()
 ****************************************************************************/

int net_setsockopt_main(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	tc_net_setsockopt_invalid_level_n(fd);
	tc_net_setsockopt_invalid_filedesc_n();
	tc_net_setsockopt_bad_filedesc_n();
	tc_net_setsockopt_keepalive_p(fd);
	tc_net_setsockopt_reuseport_p(fd);
	tc_net_setsockopt_reuseaddr_p(fd);
	tc_net_setsockopt_rcvtimo_p(fd);
	tc_net_setsockopt_broadcast_p(fd);
	tc_net_setsockopt_ip_tos_p(fd);
	tc_net_setsockopt_multicast_tcp_keepcnt_p(fd);
	tc_net_setsockopt_multicast_tcp_keepintvl_p(fd);
	tc_net_setsockopt_multicast_tcp_keepidle_p(fd);
	tc_net_setsockopt_multicast_tcp_keepalive_p(fd);
	tc_net_setsockopt_multicast_tcp_nodelay_p(fd);
	close(fd);
	return 0;
}
