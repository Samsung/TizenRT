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
//#include <arch/board/board.h>
#include <netutils/netlib.h>

#include <sys/socket.h>

#include "tc_internal.h"

/**
* @testcase				: tc_net_getsockopt_multicast_ttl_p
* @brief				:
* @scenario				:
* @apicovered			: getsockopt()
* @precondition			: socket file descriptor
* @postcondition		:
*/
static void tc_net_getsockopt_multicast_ttl_p(int sock)
{
	int ret;
	socklen_t optval = 1;
	socklen_t optlen = sizeof(optval);
	ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &optval, optlen);
	TC_ASSERT_EQ("setsockopt", ret, ZERO);

	ret = getsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &optval, &optlen);
	TC_ASSERT_GEQ("getsockopt", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_getsockopt_multicast_ttl_loop_own_p
* @brief				:
* @scenario				:
* @apicovered			: getsockopt()
* @precondition			: socket file descriptor
* @postcondition		:
*/
static void tc_net_getsockopt_multicast_ttl_loop_own_p(int sock)
{
	int ret = NEG_VAL;
	socklen_t loop = ONE;
	socklen_t looplen = sizeof(loop);
	ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, looplen);
	TC_ASSERT_EQ("setsockopt", ret, ZERO);
	ret = getsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &loop, &looplen);
	TC_ASSERT_GEQ("getsockopt", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_getsockopt_multicast_ttl_loop_p
* @brief				:
* @scenario				:
* @apicovered			: getsockopt()
* @precondition			: socket file descriptor
* @postcondition		:
*/
static void tc_net_getsockopt_multicast_ttl_loop_p(int sock)
{
	int ret;
	socklen_t loop = 250;
	socklen_t looplen = sizeof(loop);
	ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, looplen);
	TC_ASSERT_EQ("setsockopt", ret, ZERO);

	ret = getsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &loop, &looplen);
	TC_ASSERT_GEQ("getsockopt", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_getsockopt_invalid_filedesc_n
* @brief				:
* @scenario				:
* @apicovered			: getsockopt()
* @precondition			:
* @postcondition		:
*/
static void tc_net_getsockopt_invalid_filedesc_n(void)
{
	int ret;
	socklen_t optval = 1;
	socklen_t optlen = sizeof(optval);

	ret = setsockopt(NEG_VAL, SOL_SOCKET, 0, 0, 0);
	TC_ASSERT_NEQ("setsockopt", ret, ZERO);

	ret = getsockopt(NEG_VAL, IPPROTO_IP, IP_MULTICAST_TTL, &optval, &optlen);
	TC_ASSERT_EQ("getsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_getsockopt_optval_n
* @brief				:
* @scenario				:
* @apicovered			: getsockopt()
* @precondition			:
* @postcondition		:
*/
static void tc_net_getsockopt_optval_n(int sock)
{
	int ret;

	socklen_t optval = 1;
	socklen_t optlen = sizeof(optval);

	ret = setsockopt(sock, SOL_SOCKET, 0, 0, 0);
	TC_ASSERT_NEQ("setsockopt", ret, ZERO);

	ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &optval, optlen);
	TC_ASSERT_EQ("setsockopt", ret, NEG_VAL);
	ret = getsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, NULL, NULL);
	TC_ASSERT_EQ("getsockopt", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_getsockopt_sol_socket_so_acceptconn_p
* @brief				:
* @scenario				:
* @apicovered			: getsockopt()
* @precondition			: socket file descriptor
* @postcondition		:
*/
static void tc_net_getsockopt_sol_socket_so_acceptconn_p(int sock)
{
	int ret;
	socklen_t optval = 1;
	socklen_t optlen = sizeof(optval);
	ret = setsockopt(sock, SOL_SOCKET, SO_ACCEPTCONN, &optval, optlen);
	TC_ASSERT_GEQ("setsockopt", ret, ZERO);
	ret = getsockopt(sock, SOL_SOCKET, SO_ACCEPTCONN, &optval, &optlen);

	TC_ASSERT_GEQ("getsockopt", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_getsockopt_sol_socket_so_broadcast_p
* @brief				:
* @scenario				:
* @apicovered			: getsockopt()
* @precondition			: socket file descriptor
* @postcondition		:
*/
static void tc_net_getsockopt_sol_socket_so_broadcast_p(int sock)
{
	int ret;
	socklen_t optval = 1;
	socklen_t optlen = sizeof(optval);
	ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optval, optlen);
	TC_ASSERT_EQ("getsockopt", ret, ZERO);

	ret = getsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optval, &optlen);
	TC_ASSERT_GEQ("getsockopt", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_getsockopt_sol_socket_so_keepalive_p
* @brief				:
* @scenario				:
* @apicovered			: getsockopt()
* @precondition			: socket file descriptor
* @postcondition		:
*/
static void tc_net_getsockopt_sol_socket_so_keepalive_p(int sock)
{
	int ret;
	socklen_t optval = 1;
	socklen_t optlen = sizeof(optval);
	ret = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen);
	TC_ASSERT_GEQ("setsockopt", ret, ZERO);

	ret = getsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen);
	TC_ASSERT_GEQ("getsockopt", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: getsockopt()
 ****************************************************************************/

void net_getsockopt_main(int s)
{
	tc_net_getsockopt_invalid_filedesc_n();
	tc_net_getsockopt_optval_n(s);
	tc_net_getsockopt_multicast_ttl_loop_p(s);
	tc_net_getsockopt_multicast_ttl_loop_own_p(s);
	tc_net_getsockopt_multicast_ttl_p(s);
	tc_net_getsockopt_sol_socket_so_acceptconn_p(s);
#ifndef CONFIG_QEMU_COVERAGE
	tc_net_getsockopt_sol_socket_so_broadcast_p(s);
	tc_net_getsockopt_sol_socket_so_keepalive_p(s);
#endif
}
