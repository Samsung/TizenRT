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
//#include <arch/board/board.h>
#include <netutils/netlib.h>

#include <sys/socket.h>

#include "tc_internal.h"

/**
   * @testcase		   :tc_net_listen_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :listen()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_listen_p(void)
{
	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fd < 0) {
		printf("Socket creation fail %s:%d\n", __FUNCTION__,  __LINE__);
		return;
	}
	memset(&sa, 0, sizeof sa);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(1100);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	if (ret < 0) {
		close(socket_fd);
		printf("fail %s:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	ret = (listen(socket_fd, 10));
	close(socket_fd);

	TC_ASSERT_NEQ("listen", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_listen_fd_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :listen()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_listen_fd_n(void)
{
	struct sockaddr_in sa;
	int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		printf("fail %s:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	memset(&sa, 0, sizeof sa);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(1101);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(fd, (struct sockaddr *)&sa, sizeof(sa));
	if (ret < 0) {
		close(fd);
		printf("fail %s:%d", __FUNCTION__, __LINE__);
		return;
	}
	ret = (listen(-1, 10));
	close(fd);
	TC_ASSERT_NEQ("listen", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
   * @testcase		   :tc_net_listen_backlog_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :listen()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_listen_backlog_p(void)
{
	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fd < 0) {
		printf("Socket creation fail %s:%d\n", __FUNCTION__,  __LINE__);
		return;
	}
	memset(&sa, 0, sizeof sa);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(1100);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	if (ret < 0) {
		close(socket_fd);
		printf("fail %s:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	ret = (listen(socket_fd, -1));
	close(socket_fd);

	TC_ASSERT_NEQ("listen", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_listen_fd_backlog_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :listen()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_listen_fd_backlog_n(void)
{
	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fd < 0) {
		printf("Socket creation fail %s:%d\n", __FUNCTION__,  __LINE__);
		return;
	}
	memset(&sa, 0, sizeof sa);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(1100);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	if (ret < 0) {
		close(socket_fd);
		printf("fail %s:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	ret = (listen(-1, -1));
	close(socket_fd);
	TC_ASSERT_NEQ("listen", ret, 0);
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
