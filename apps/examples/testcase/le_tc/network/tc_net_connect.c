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

/// @file tc_net_connect.c
/// @brief Test Case Example for connect() API
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
   * @testcase		   :tc_net_connect_fd_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :connect()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_connect_fd_n(struct sockaddr_in *sa)
{
	inet_pton(AF_INET, "192.168.1.3", &(sa->sin_addr));

	int ret = connect(0, (struct sockaddr *)sa, sizeof(struct sockaddr_in));

	TC_ASSERT_EQ("connect", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_connect_broadcastaddr_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :connect()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_connect_broadcastaddr_n(struct sockaddr_in *sa)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		printf("socket creation error line:%d\n", __LINE__);
		return;
	}
	struct in_addr ad;

	ad.s_addr = INADDR_BROADCAST;
	sa->sin_addr = ad;
	int ret = connect(fd, (struct sockaddr *)sa, sizeof(struct sockaddr_in));
	close(fd);

	TC_ASSERT_EQ("connect", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_connect_loopbackaddr_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :connect()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_connect_loopbackaddr_n(struct sockaddr_in *sa)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		printf("socket creation error line:%d\n", __LINE__);
		return;
	}

	struct in_addr ad;
	ad.s_addr = INADDR_LOOPBACK;
	sa->sin_addr = ad;
	int ret = connect(fd, (struct sockaddr *)sa, sizeof(struct sockaddr_in));
	close(fd);

	TC_ASSERT_EQ("connect", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_connect_socklen_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :connect()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_connect_socklen_n(struct sockaddr_in *sa)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		printf("socket creation error line:%d\n", __LINE__);
		return;
	}
	struct in_addr ad;
	ad.s_addr = INADDR_LOOPBACK;
	sa->sin_addr = ad;
	int ret = connect(fd, (struct sockaddr *)sa, -1);
	close(fd);

	TC_ASSERT_EQ("connect", ret, -1);
	TC_SUCCESS_RESULT();

}

/****************************************************************************
 * Name: connect()
 ****************************************************************************/

int net_connect_main(void)
{
	struct sockaddr_in sa;

	memset(&sa, 0, sizeof sa);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(1100);

	tc_net_connect_fd_n(&sa);
	tc_net_connect_broadcastaddr_n(&sa);
	tc_net_connect_loopbackaddr_n(&sa);
	tc_net_connect_socklen_n(&sa);
	return 0;
}
