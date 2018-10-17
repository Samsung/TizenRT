/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/// @file itc_net_listen.c
/// @brief Test Case Example for listen() API

#include <sys/socket.h>
#include <arpa/inet.h>

#include "tc_internal.h"

#define ADDR_PORT 1100
#define SOCK_PORT 1101
#define LOOP_COUNT 10
#define BACK_LOG 10

/**
* @testcase        :itc_net_listen_p_multiple_time
* @brief           :listen for socket connections and limit the queue of incoming connections
* @scenario        :create socket, bind it and then listen and repeat multiple times
* @apicovered      :listen()
* @precondition    :open socket
* @postcondition   :close socket
*/
static void itc_net_listen_p_multiple_time(void)
{
	int index;
	int socket_fd;
	int ret;
	struct sockaddr_in sa;

	for (index = 0; index < LOOP_COUNT; index++) {
		socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		TC_ASSERT_GEQ("socket", socket_fd, CONFIG_NFILE_DESCRIPTORS);//If ASSERT FAILS, no need to close(socket_fd) as socket is not created

		memset(&sa, 0, sizeof sa);

		sa.sin_family = AF_INET;
		sa.sin_port = htons(ADDR_PORT);
		sa.sin_addr.s_addr = htonl(INADDR_ANY);

		ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
		TC_ASSERT_EQ_CLEANUP("bind", ret, 0, close(socket_fd));

		ret = listen(socket_fd, BACK_LOG);
		TC_ASSERT_EQ_CLEANUP("listen", ret, 0, close(socket_fd));

		ret = close(socket_fd);
		TC_ASSERT_EQ("close", ret, 0);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase        :itc_net_listen_p_different_socket
* @brief           :listen for socket connections and limit the queue of incoming connections
* @scenario        :create socket, bind it and then listen
* @apicovered      :listen()
* @precondition    :open socket
* @postcondition   :close socket
*/
static void itc_net_listen_p_different_socket(void)
{
	struct sockaddr_in sa;
	struct sockaddr_in saddr;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_GEQ("socket", socket_fd, CONFIG_NFILE_DESCRIPTORS);

	int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_GEQ("socket", fd, CONFIG_NFILE_DESCRIPTORS);

	memset(&sa, 0, sizeof sa);
	memset(&saddr, 0, sizeof saddr);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(ADDR_PORT);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(SOCK_PORT);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_EQ_CLEANUP("bind", ret, 0, close(socket_fd); close(fd));

	ret = bind(fd, (struct sockaddr *)&saddr, sizeof(saddr));
	TC_ASSERT_EQ_CLEANUP("bind", ret, 0, close(socket_fd); close(fd));

	ret = listen(socket_fd, BACK_LOG);
	TC_ASSERT_EQ_CLEANUP("listen", ret, 0, close(socket_fd); close(fd));

	ret = listen(fd, BACK_LOG);
	TC_ASSERT_EQ_CLEANUP("listen", ret, 0, close(socket_fd); close(fd));

	ret = close(socket_fd);
	TC_ASSERT_EQ_CLEANUP("close", ret, 0, close(fd));

	ret = close(fd);
	TC_ASSERT_EQ("close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase        :itc_net_listen_n_after_socket_close
* @brief           :listen for socket connections and limit the queue of incoming connections
* @scenario        :create socket, bind, close socket and then listen
* @apicovered      :listen()
* @precondition    :open socket, close socket
* @postcondition   :none
*/
static void itc_net_listen_n_after_socket_close(void)
{
	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_GEQ("socket", socket_fd, CONFIG_NFILE_DESCRIPTORS);

	memset(&sa, 0, sizeof sa);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(ADDR_PORT);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_EQ_CLEANUP("bind", ret, 0, close(socket_fd));

	ret = close(socket_fd);
	TC_ASSERT_EQ("close", ret, 0);

	ret = listen(socket_fd, BACK_LOG);
	TC_ASSERT_NEQ("listen", ret, 0);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: listen()
 ****************************************************************************/

int itc_net_listen_main(void)
{
	itc_net_listen_p_multiple_time();
	itc_net_listen_p_different_socket();
	itc_net_listen_n_after_socket_close();

	return 0;
}
