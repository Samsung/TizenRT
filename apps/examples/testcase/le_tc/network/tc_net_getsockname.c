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

/// @file tc_net_getsockname.c
/// @brief Test Case Example for getsockname() API
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
   * @testcase		   :tc_net_getsockname_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :getsockname()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_getsockname_p(void)
{
	int sock;
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("socket creation error (%s) line:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	int ret = getsockname(sock, &foo, (socklen_t *)&len);
	close(sock);
	TC_ASSERT_NEQ("getsockname", ret, -1);
	TC_SUCCESS_RESULT();
}
#ifdef AF_UNIX
/**
   * @testcase		   :tc_net_getsockname_unix_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :getsockname()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_getsockname_unix_p(void)
{
	int sock;
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	int ret = getsockname(sock, &foo, (socklen_t *)&len);
	close(sock);
	TC_ASSERT_NEQ("getsockname", ret, -1);
	TC_SUCCESS_RESULT();
}
#endif
/**
   * @testcase		   :tc_net_getsockname_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :getsockname()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_getsockname_n(void)
{
	int sock;
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	sock = -1;

	int ret = getsockname(sock, &foo, (socklen_t *)&len);

	TC_ASSERT_NEQ("getsockname", ret, 0);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_getsockname_close_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :getsockname()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_getsockname_close_n(void)
{
	int sock;
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		printf("socket creation error (%s) line:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	close(sock);
	int ret = getsockname(sock, &foo, (socklen_t *)&len);

	TC_ASSERT_NEQ("getsockname", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
   * @testcase		   :tc_net_getsockname_udp_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :getsockname()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_getsockname_udp_p(void)
{
	int sock;
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (sock < 0) {
		printf("socket creation error (%s) line:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	int ret = getsockname(sock, &foo, (socklen_t *)&len);
	close(sock);
	TC_ASSERT_NEQ("getsockname", ret, -1);
	TC_SUCCESS_RESULT();
}

/**
   * @testcase		   :tc_net_getsockname_icmp_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :getsockname()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_getsockname_icmp_p(void)
{
	int sock;
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock < 0) {
		printf("socket creation error (%s) line:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	int ret = getsockname(sock, &foo, (socklen_t *)&len);
	close(sock);
	TC_ASSERT_NEQ("getsockname", ret, -1);
	TC_SUCCESS_RESULT();
}

/**
   * @testcase		   :tc_net_getsockname_len_sock_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :getsockname()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_getsockname_len_sock_n(void)
{
	int sock;
	int len = -1;
	struct sockaddr foo;

	sock = -1;
	int ret = getsockname(sock, &foo, (socklen_t *)&len);

	TC_ASSERT_NEQ("getsockname", ret, 0);
	TC_SUCCESS_RESULT();

}

/****************************************************************************
 * Name: getsockname()
 ****************************************************************************/

int net_getsockname_main(void)
{
	tc_net_getsockname_p();
	tc_net_getsockname_n();
	tc_net_getsockname_len_sock_n();
	tc_net_getsockname_udp_p();
	tc_net_getsockname_icmp_p();
	tc_net_getsockname_close_n();
	return 0;
}
