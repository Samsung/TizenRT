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

/// @file tc_net_getsockopt.c
/// @brief Test Case Example for getsockopt() API
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
* @testcase            : tc_net_getsockopt_multicast_ttl_p
* @brief               : This getsockopt API get options on sockets.
* @scenario            : getsockopt manipulate options for the socket referred to by the sock fd.
* @apicovered          : getsockopt(), setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockopt_multicast_ttl_p(void)
{
	int ret;
	socklen_t optval = ONE;
	socklen_t optlen = sizeof(optval);

	int sock = socket(AF_INET, SOCK_DGRAM, ZERO);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &optval, optlen);
	TC_ASSERT_EQ("setsockopt", ret, ZERO);

	ret = getsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &optval, &optlen);
	close(sock);
	TC_ASSERT_GEQ("getsockopt", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getsockopt_multicast_ttl_loop_own_p
* @brief               : This getsockopt API get options on sockets.
* @scenario            : getsockopt manipulate options for the socket referred to by the sock fd,
                         with ip multicast ttl option.
* @apicovered          : getsockopt(), setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockopt_multicast_ttl_loop_own_p(void)
{
	int ret;
	socklen_t loop = ONE;
	socklen_t looplen = sizeof(loop);

	int sock = socket(AF_INET, SOCK_DGRAM, ZERO);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, looplen);
	TC_ASSERT_EQ("setsockopt", ret, ZERO);
	ret = getsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &loop, &looplen);
	close(sock);
	TC_ASSERT_GEQ("getsockopt", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getsockopt_multicast_ttl_loop_p
* @brief               : This getsockopt API get options on sockets.
* @scenario            : getsockopt manipulate options for the socket referred to by the sock fd,
                         with ip multicase loop option.
* @apicovered          : getsockopt(), setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockopt_multicast_ttl_loop_p(void)
{
	int ret;
	socklen_t loop = 1;
	socklen_t looplen = sizeof(loop);

	int sock = socket(AF_INET, SOCK_DGRAM, ZERO);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, looplen);
	TC_ASSERT_EQ("setsockopt", ret, ZERO);

	ret = getsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &loop, &looplen);
	close(sock);
	TC_ASSERT_GEQ("getsockopt", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getsockopt_invalid_filedesc_n
* @brief               : This getsockopt API get options on sockets.
* @scenario            : getsockopt manipulate options for the socket referred to by the sock fd,
                         with invalid socket fd.
* @apicovered          : getsockopt(), setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockopt_invalid_filedesc_n(void)
{
	int ret;
	socklen_t optval = ONE;
	socklen_t optlen = sizeof(optval);

	ret = setsockopt(NEG_VAL, SOL_SOCKET, ZERO, ZERO, ZERO);
	TC_ASSERT_NEQ("setsockopt", ret, ZERO);

	ret = getsockopt(NEG_VAL, IPPROTO_IP, IP_MULTICAST_TTL, &optval, &optlen);
	TC_ASSERT_EQ("getsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getsockopt_optval_n
* @brief               : This getsockopt API get options on sockets.
* @scenario            : getsockopt manipulate options for the socket referred to by the sock fd,
                         with null optval.
* @apicovered          : getsockopt(), setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockopt_optval_n(void)
{
	int ret;
	socklen_t optval = ONE;
	socklen_t optlen = sizeof(optval);

	int sock = socket(AF_INET, SOCK_STREAM, ZERO);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	ret = setsockopt(sock, SOL_SOCKET, ZERO, ZERO, ZERO);
	TC_ASSERT_NEQ("setsockopt", ret, ZERO);

	ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &optval, optlen);
	TC_ASSERT_EQ("setsockopt", ret, NEG_VAL);
	ret = getsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, NULL, NULL);
	close(sock);
	TC_ASSERT_EQ("getsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getsockopt_sol_socket_so_acceptconn_p
* @brief               : This getsockopt API get options on sockets.
* @scenario            : getsockopt manipulate options for the socket referred to by the sock fd,
                         with acceptconn option.
* @apicovered          : getsockopt(), setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockopt_sol_socket_so_acceptconn_p(void)
{
	int ret;
	socklen_t optval = ONE;
	socklen_t optlen = sizeof(optval);

	int sock = socket(AF_INET, SOCK_STREAM, ONE);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	ret = setsockopt(sock, SOL_SOCKET, SO_ACCEPTCONN, &optval, optlen);
	TC_ASSERT_EQ("setsockopt", ret, ZERO);
	ret = getsockopt(sock, SOL_SOCKET, SO_ACCEPTCONN, &optval, &optlen);
	close(sock);
	TC_ASSERT_EQ("getsockopt", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getsockopt_sol_socket_so_broadcast_p
* @brief               : This getsockopt API get options on sockets.
* @scenario            : getsockopt manipulate options for the socket referred to by the sock fd,
                         with broadcast option.
* @apicovered          : getsockopt(), setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockopt_sol_socket_so_broadcast_p(void)
{
	int ret;
	socklen_t optval = ONE;
	socklen_t optlen = sizeof(optval);

	int sock = socket(AF_INET, SOCK_DGRAM, ZERO);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optval, optlen);
	TC_ASSERT_EQ("getsockopt", ret, ZERO);

	ret = getsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optval, &optlen);
	close(sock);
	TC_ASSERT_GEQ("getsockopt", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getsockopt_sol_socket_so_keepalive_p
* @brief               : This getsockopt API get options on sockets.
* @scenario            : getsockopt manipulate options for the socket referred to by the sock fd,
                         with keepalive option.
* @apicovered          : getsockopt(), setsockopt()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getsockopt_sol_socket_so_keepalive_p(void)
{
	int ret;
	socklen_t optval = ONE;
	socklen_t optlen = sizeof(optval);

	int sock = socket(AF_INET, SOCK_STREAM, ZERO);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	ret = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen);
	TC_ASSERT_EQ("setsockopt", ret, ZERO);

	ret = getsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen);
	close(sock);
	TC_ASSERT_GEQ("getsockopt", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: getsockopt()
 ****************************************************************************/

void net_getsockopt_main()
{
	tc_net_getsockopt_invalid_filedesc_n();
	tc_net_getsockopt_optval_n();
	tc_net_getsockopt_multicast_ttl_loop_p();
	tc_net_getsockopt_multicast_ttl_loop_own_p();
	tc_net_getsockopt_multicast_ttl_p();
#ifndef CONFIG_QEMU_COVERAGE
	tc_net_getsockopt_sol_socket_so_broadcast_p();
	tc_net_getsockopt_sol_socket_so_keepalive_p();
#endif
	tc_net_getsockopt_sol_socket_so_acceptconn_p();
}
