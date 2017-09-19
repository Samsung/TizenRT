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

/// @file tc_net_listen.c
/// @brief Test Case Example for listen() API
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

#define PORTNUM		1100
#define PORTNUM1	1101

/**
* @testcase            : tc_net_listen_p
* @brief               : maintain a connection request queue
* @scenario            :
* @apicovered          : listen(), bind()
* @precondition        : none
* @postcondition       : none
*/
static void tc_net_listen_p(void)
{
	int ret;
	struct sockaddr_in sa;

	int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	ret = bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_NEQ("bind", ret, NEG_VAL);
	TC_SUCCESS_RESULT();

	ret = (listen(sock, 10));
	close(sock);
	TC_ASSERT_NEQ("listen", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_listen_fd_n
* @brief               : maintain a connection request queue
* @scenario            :
* @apicovered          : listen(), bind()
* @precondition        : none
* @postcondition       : none
*/
static void tc_net_listen_fd_n(void)
{
	int ret;
	struct sockaddr_in sa;

	int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1101);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(fd, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_NEQ("bind", ret, NEG_VAL);

	ret = listen(NEG_VAL, 10);
	close(fd);
	TC_ASSERT_NEQ("listen", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_listen_backlog_p
* @brief               : maintain a connection request queue
* @scenario            :
* @apicovered          : listen(), bind()
* @precondition        : none
* @postcondition       : none
*/
static void tc_net_listen_backlog_p(void)
{
	int ret;
	struct sockaddr_in sa;

	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", SocketFD, NEG_VAL);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1100);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(SocketFD, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_EQ("bind", ret, ZERO);

	ret = listen(SocketFD, NEG_VAL);
	close(SocketFD);

	TC_ASSERT_NEQ("listen", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_listen_fd_backlog_n
* @brief               : maintain a connection request queue
* @scenario            :
* @apicovered          : listen(), bind()
* @precondition        : none
* @postcondition       : none
*/
static void tc_net_listen_fd_backlog_n(void)
{
	int ret;
	struct sockaddr_in sa;

	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", SocketFD, NEG_VAL);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1100);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(SocketFD, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_EQ("bind", ret, ZERO);

	ret = listen(NEG_VAL, NEG_VAL);
	close(SocketFD);
	TC_ASSERT_NEQ("listen", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: listen()
 ****************************************************************************/

int net_listen_main(void)
{
	tc_net_listen_p();
	tc_net_listen_fd_n();
	tc_net_listen_backlog_p();
	tc_net_listen_fd_backlog_n();
	return 0;
}
