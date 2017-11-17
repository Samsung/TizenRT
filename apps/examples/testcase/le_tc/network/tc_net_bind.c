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

/// @file tc_net_bind.c
/// @brief Test Case Example for bind() API
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

#define PORTNO        5001
#define PORTNO1       5002
#define PORTNO2       5003
#define PORTNO3       5004
#define PORTNO4       5005
#define PORTNO5       5123

/**
* @testcase            : tc_net_bind_p
* @brief               : This bind API bind a name to a socket.
* @scenario            : bind() assigns the address specified by addr to the socket.
* @apicovered          : bind()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_bind_p(void)
{
	struct sockaddr_in sa;
	int ret;

	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", SocketFD, NEG_VAL);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORTNO);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	ret = bind(SocketFD, (struct sockaddr *)&sa, sizeof(sa));
	close(SocketFD);

	TC_ASSERT_NEQ("bind", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_bind_udp_p
* @brief               : This bind API bind a name to a socket.
* @scenario            : bind() assigns the address specified by addr to the socket.
* @apicovered          : bind()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_bind_udp_p(void)
{
	struct sockaddr_in sa;
	int ret;

	int SocketFD = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	TC_ASSERT_NEQ("socket", SocketFD, NEG_VAL);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORTNO);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	ret = bind(SocketFD, (struct sockaddr *)&sa, sizeof(sa));
	close(SocketFD);

	TC_ASSERT_NEQ("bind", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_bind_broadcast_p
* @brief               : This bind API bind a name to a socket.
* @scenario            : bind() assigns the address specified by addr to the socket.
* @apicovered          : bind()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_bind_broadcast_p(void)
{
	struct sockaddr_in sa;
	int ret;

	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", SocketFD, NEG_VAL);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORTNO1);
	sa.sin_addr.s_addr = INADDR_BROADCAST;

	ret = bind(SocketFD, (struct sockaddr *)&sa, sizeof(sa));
	close(SocketFD);

	TC_ASSERT_NEQ("bind", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_bind_fd_n
* @brief               : This bind API bind a name to a socket.
* @scenario            : bind() assigns the address specified by addr to the socket.
* @apicovered          : bind()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_bind_fd_n(void)
{
	struct sockaddr_in sa;
	int ret;

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORTNO4);
	sa.sin_addr.s_addr = INADDR_ANY;

	ret = bind(NEG_VAL, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_EQ("bind", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_bind_addrfamily_n
* @brief               : This bind API bind a name to a socket.
* @scenario            : bind() assigns the address specified by addr to the socket.
* @apicovered          : bind()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_bind_addrfamily_n(void)
{
	struct sockaddr_in sa;
	int ret;

	int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_UNIX;
	sa.sin_port = htons(PORTNO5);
	sa.sin_addr.s_addr = INADDR_ANY;

	ret = bind(fd, (struct sockaddr *)&sa, sizeof(sa));
	close(fd);
	TC_ASSERT_EQ("bind", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_bind_size_n
* @brief               : This bind API bind a name to a socket.
* @scenario            : bind() assigns the address specified by addr to the socket.
* @apicovered          : bind()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_bind_size_n(void)
{
	struct sockaddr_in sa;
	int ret;

	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", SocketFD, NEG_VAL);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORTNO3);
	sa.sin_addr.s_addr = INADDR_ANY;

	ret = bind(SocketFD, (struct sockaddr *)&sa, NEG_VAL);
	close(SocketFD);

	TC_ASSERT_EQ("bind", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_bind_fd_size_n
* @brief               : This bind API bind a name to a socket.
* @scenario            : bind() assigns the address specified by addr to the socket.
* @apicovered          : bind()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_bind_fd_size_n(void)
{
	struct sockaddr_in sa;
	int ret;

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORTNO2);
	sa.sin_addr.s_addr = INADDR_ANY;

	ret = bind(NEG_VAL, (struct sockaddr *)&sa, 0);

	TC_ASSERT_EQ("bind", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: bind()
 ****************************************************************************/

int net_bind_main(void)
{
	tc_net_bind_p();
	tc_net_bind_fd_n();
	tc_net_bind_fd_size_n();
	tc_net_bind_udp_p();
	tc_net_bind_broadcast_p();
	tc_net_bind_size_n();
	tc_net_bind_addrfamily_n();
	return 0;
}
