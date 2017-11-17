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

#define PORTNUM1        5000
#define PORTNUM2        5001
#define BACKLOG         10

/**
* @testcase            : tc_net_listen_p
* @brief               : This listen API listen for connections on a socket.
* @scenario            : listen for socket connections and limit the queue of incoming connections.
* @apicovered          : listen(), bind()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_listen_p(void)
{
	int ret;
	struct sockaddr_in sa;

	int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORTNUM1);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	ret = bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_NEQ_CLEANUP("bind", ret, NEG_VAL, close(sock));

	ret = listen(sock, BACKLOG);
	TC_ASSERT_NEQ_CLEANUP("listen", ret, NEG_VAL, close(sock));
	close(sock);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_listen_fd_n
* @brief               : This listen API listen for connections on a socket.
* @scenario            : listen for socket connections and limit the queue of incoming connections,
                         test with invalid socket fd.
* @apicovered          : listen(), bind()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_listen_fd_n(void)
{
	int ret;
	struct sockaddr_in sa;

	int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORTNUM2);
	sa.sin_addr.s_addr = INADDR_ANY;

	ret = bind(fd, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_NEQ_CLEANUP("bind", ret, NEG_VAL, close(fd));

	ret = listen(-1, BACKLOG);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_listen_backlog_p
* @brief               : This listen API listen for connections on a socket.
* @scenario            : listen for socket connections and limit the queue of incoming connections,
                         test with invalid backlog value.
* @apicovered          : listen(), bind()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_listen_backlog_p(void)
{
	int ret;
	struct sockaddr_in sa;

	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", SocketFD, NEG_VAL);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORTNUM1);
	sa.sin_addr.s_addr = INADDR_ANY;

	ret = bind(SocketFD, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_NEQ_CLEANUP("bind", ret, NEG_VAL, close(SocketFD));

	ret = listen(SocketFD, BACKLOG);
	TC_ASSERT_NEQ_CLEANUP("listen", ret, NEG_VAL, close(SocketFD));
	close(SocketFD);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_listen_fd_backlog_n
* @brief               : This listen API listen for connections on a socket.
* @scenario            : listen for socket connections and limit the queue of incoming connections,
                         test with invalid socket fd and backlog value.
* @apicovered          : listen(), bind()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_listen_fd_backlog_n(void)
{
	int ret;
	struct sockaddr_in sa;

	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", SocketFD, NEG_VAL);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORTNUM1);
	sa.sin_addr.s_addr = INADDR_ANY;

	ret = bind(SocketFD, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_NEQ_CLEANUP("bind", ret, NEG_VAL, close(SocketFD));

	ret = listen(-1, -1);
	TC_ASSERT_EQ_CLEANUP("listen", ret, NEG_VAL, close(SocketFD));
	close(SocketFD);
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
