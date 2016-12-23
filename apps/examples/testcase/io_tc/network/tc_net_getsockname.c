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
#include <apps/netutils/netlib.h>

#include <sys/socket.h>

#include "tc_internal.h"
/**
   * @fn                   :tc_net_getsockname_p
   * @brief                :
   * @scenario             :
   * API's covered         :getsockname()
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void tc_net_getsockname_p(void)
{
	int sock;
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	int ret = getsockname(sock, &foo, (socklen_t *)&len);
	if (ret == -1) {
		printf("tc_net_getsockname_p: fail\n");

		nw_total_fail++;
		RETURN_ERR;
	}

	printf("tc_net_getsockname_p: PASS\n");
	nw_total_pass++;
}

/**
   * @fn                   :tc_net_getsockname_unix_p
   * @brief                :
   * @scenario             :
   * API's covered         :getsockname()
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void tc_net_getsockname_unix_p(void)
{
	int sock;
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	int ret = getsockname(sock, &foo, (socklen_t *)&len);
	if (ret == -1) {
		printf("tc_net_getsockname_unix_p: fail\n");

		nw_total_fail++;
		RETURN_ERR;
	}

	printf("tc_net_getsockname_unix_p: PASS\n");
	nw_total_pass++;
}

/**
   * @fn                   :tc_net_getsockname_n
   * @brief                :
   * @scenario             :
   * API's covered         :getsockname()
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void tc_net_getsockname_n(void)
{
	int sock;
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	sock = -1;

	int ret = getsockname(sock, &foo, (socklen_t *)&len);
	if (ret == 0) {
		printf("tc_net_getsockname_n: fail\n");

		nw_total_fail++;
		RETURN_ERR;
	}

	printf("tc_net_getsockname_n: PASS\n");
	nw_total_pass++;
}

/**
   * @fn                   :tc_net_getsockname_close_n
   * @brief                :
   * @scenario             :
   * API's covered         :getsockname()
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void tc_net_getsockname_close_n(void)
{
	int sock;
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	close(sock);
	int ret = getsockname(sock, &foo, (socklen_t *)&len);
	if (ret == 0) {
		printf("tc_net_getsockname_close_n: fail\n");

		nw_total_fail++;
		RETURN_ERR;
	}
	printf("tc_net_getsockname_close_n: PASS\n");
	nw_total_pass++;
}

/**
   * @fn                   :tc_net_getsockname_udp_p
   * @brief                :
   * @scenario             :
   * API's covered         :getsockname()
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void tc_net_getsockname_udp_p(void)
{
	int sock;
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int ret = getsockname(sock, &foo, (socklen_t *)&len);
	if (ret == -1) {
		printf("tc_net_getsockname_udp_p: fail\n");

		nw_total_fail++;
		RETURN_ERR;
	}

	printf("tc_net_getsockname_udp_p: PASS\n");
	nw_total_pass++;
}

/**
   * @fn                   :tc_net_getsockname_icmp_p
   * @brief                :
   * @scenario             :
   * API's covered         :getsockname()
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void tc_net_getsockname_icmp_p(void)
{
	int sock;
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	int ret = getsockname(sock, &foo, (socklen_t *)&len);
	if (ret == -1) {
		printf("tc_net_getsockname_icmp_p: fail\n");

		nw_total_fail++;
		RETURN_ERR;
	}

	printf("tc_net_getsockname_icmp_p: PASS\n");
	nw_total_pass++;
}

/**
   * @fn                   :tc_net_getsockname_len_sock_n
   * @brief                :
   * @scenario             :
   * API's covered         :getsockname()
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void tc_net_getsockname_len_sock_n(void)
{
	int sock;
	int len = -1;
	struct sockaddr foo;

	sock = -1;
	int ret = getsockname(sock, &foo, (socklen_t *)&len);
	if (ret == 0) {
		printf("tc_net_getsockname_len_sock_n: fail\n");

		nw_total_fail++;
		RETURN_ERR;
	}

	printf("tc_net_getsockname_len_sock_n: PASS\n");
	nw_total_pass++;
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
	tc_net_getsockname_unix_p();

	return 0;
}
