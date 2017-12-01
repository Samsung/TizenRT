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

#include <sys/socket.h>
#include "tc_internal.h"

#define ADDR_PORT 1100

/**
* @testcase        :tc_net_listen_p
* @brief           :listen for socket connections and limit the queue of incoming connections
* @scenario        :create socket, bind it and then listen
* @apicovered      :listen()
* @precondition    :open socket
* @postcondition   :close socket
*/
static void tc_net_listen_p(void)
{
	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_GEQ("socket", socket_fd, 0);

	memset(&sa, 0, sizeof sa);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(ADDR_PORT);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_EQ_CLEANUP("bind", ret, 0, close(socket_fd));

	ret = listen(socket_fd, 10);
	TC_ASSERT_EQ_CLEANUP("listen", ret, 0, close(socket_fd));

	ret = close(socket_fd);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase        :tc_net_listen_multiple_time_p
* @brief           :listen for socket connections and limit the queue of incoming connections
* @scenario        :create socket, bind it and then listen and repeat multiple times
* @apicovered      :listen()
* @precondition    :open socket
* @postcondition   :close socket
*/
static void tc_net_listen_multiple_time_p(void)
{
	int index;
	int loop_count = 10;
	int socket_fd;
	int ret;
	struct sockaddr_in sa;

	for (index = 0; index < loop_count; index++) {
		socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		TC_ASSERT_GEQ("socket", socket_fd, 0);

		memset(&sa, 0, sizeof sa);

		sa.sin_family = AF_INET;
		sa.sin_port = htons(ADDR_PORT);
		sa.sin_addr.s_addr = htonl(INADDR_ANY);

		ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
		TC_ASSERT_EQ_CLEANUP("bind", ret, 0, close(socket_fd));

		ret = listen(socket_fd, 10);
		TC_ASSERT_EQ_CLEANUP("listen", ret, 0, close(socket_fd));

		ret = close(socket_fd);
		TC_ASSERT_EQ("close", ret, 0);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase        :tc_net_listen_different_socket_p
* @brief           :listen for socket connections and limit the queue of incoming connections
* @scenario        :create socket, bind it and then listen
* @apicovered      :listen()
* @precondition    :open socket
* @postcondition   :close socket
*/
static void tc_net_listen_different_socket_p(void)
{
	struct sockaddr_in sa;
	struct sockaddr_in saddr;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_GEQ("socket", socket_fd, 0);

	int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_GEQ("socket", socket_fd, 0);

	memset(&sa, 0, sizeof sa);
	memset(&saddr, 0, sizeof saddr);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(ADDR_PORT);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(1101);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_EQ_CLEANUP("bind", ret, 0, close(socket_fd); close(fd));

	ret = bind(fd, (struct sockaddr *)&saddr, sizeof(saddr));
	TC_ASSERT_EQ_CLEANUP("bind", ret, 0, close(socket_fd); close(fd));

	ret = listen(socket_fd, 10);
	TC_ASSERT_EQ_CLEANUP("listen", ret, 0, close(socket_fd); close(fd));

	ret = listen(fd, 10);
	TC_ASSERT_EQ_CLEANUP("listen", ret, 0, close(socket_fd); close(fd));

	ret = close(socket_fd);
	TC_ASSERT_EQ_CLEANUP("close", ret, 0, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase        :tc_net_listen_after_socket_close_n
* @brief           :listen for socket connections and limit the queue of incoming connections
* @scenario        :create socket, bind, close socket and then listen
* @apicovered      :listen()
* @precondition    :open socket, close socket
* @postcondition   :none
*/
static void tc_net_listen_after_socket_close_n(void)
{
	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_GEQ("socket", socket_fd, 0);

	memset(&sa, 0, sizeof sa);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(ADDR_PORT);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_EQ_CLEANUP("bind", ret, 0, close(socket_fd));

	ret = close(socket_fd);
	TC_ASSERT_EQ("close", ret, 0);

	ret = listen(socket_fd, 10);
	TC_ASSERT_NEQ("listen", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase        :tc_net_listen_fd_n
* @brief           :listen for socket connections and limit the queue of incoming connections
* @scenario        :listen with invalid descriptor
* @apicovered      :listen()
* @precondition    :none
* @postcondition   :none
*/
static void tc_net_listen_fd_n(void)
{
	int ret = listen(-1, 10);
	TC_ASSERT_NEQ("listen", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase        :tc_net_listen_backlog_p
* @brief           :listen for socket connections and limit the queue of incoming connections
* @scenario        :create socket, bind and listen with backlog
* @apicovered      :listen()
* @precondition    :open socket
* @postcondition   :close socket
*/
static void tc_net_listen_backlog_p(void)
{
	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_GEQ("socket", socket_fd, 0);

	memset(&sa, 0, sizeof sa);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(ADDR_PORT);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_EQ_CLEANUP("bind", ret, 0, close(socket_fd));

	ret = listen(socket_fd, -1);
	TC_ASSERT_EQ_CLEANUP("listen", ret, 0, close(socket_fd));

	ret = close(socket_fd);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase        :tc_net_listen_fd_backlog_n
* @brief           :listen for socket connections and limit the queue of incoming connections
* @scenario        :create socket, bind and listen with negative backlog and negative descriptor
* @apicovered      :listen()
* @precondition    :open socket
* @postcondition   :cloes socket
*/
static void tc_net_listen_fd_backlog_n(void)
{
	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_GEQ("socket", socket_fd, 0);

	memset(&sa, 0, sizeof sa);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(ADDR_PORT);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_EQ_CLEANUP("bind", ret, 0, close(socket_fd));

	ret = listen(-1, -1);
	TC_ASSERT_NEQ_CLEANUP("listen", ret, 0, close(socket_fd));

	ret = close(socket_fd);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: listen()
 ****************************************************************************/

int net_listen_main(void)
{
	tc_net_listen_p();
	tc_net_listen_multiple_time_p();
	tc_net_listen_different_socket_p();
	tc_net_listen_after_socket_close_n();
	tc_net_listen_fd_n();
	tc_net_listen_backlog_p();
	tc_net_listen_fd_backlog_n();

	return 0;
}
