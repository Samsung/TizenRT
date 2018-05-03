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

/// @file tc_net_inet.c
/// @brief Test Case Example for inet() API
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netutils/netlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include "tc_internal.h"


/**
* @testcase		tc_net_inet_addr_p
* @brief
* @scenario
* @apicovered		inet_addr()
* @precondition
* @postcondition
*/
static void tc_net_inet_addr_p(void)
{
	unsigned int ret;

	ret = inet_addr("127.0.0.1");

	TC_ASSERT_NEQ("inet", ret, -1);
	TC_SUCCESS_RESULT();
}


/**
* @testcase		tc_net_inet_aton_p
* @brief
* @scenario
* @apicovered		inet_aton()
* @precondition
* @postcondition
*/
static void tc_net_inet_aton_p(void)
{
	struct sockaddr_in addr_inet;
	unsigned long ret;

	ret = inet_aton("127.0.0.1", &addr_inet.sin_addr);

	TC_ASSERT_NEQ("inet", ret, 0);
	TC_SUCCESS_RESULT();
}


/**
* @testcase		tc_net_inet_ntoa_p
* @brief
* @scenario
* @apicovered		inet_ntoa()
* @precondition
* @postcondition
*/
static void tc_net_inet_ntoa_p(void)
{
	struct sockaddr_in addr_inet;
	char *ret;

	addr_inet.sin_addr.s_addr = 0x6601a8c0;
	ret = inet_ntoa(addr_inet.sin_addr);

	TC_ASSERT_NEQ("inet", *ret, -1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase     tc_net_inet_ntop
* @brief
* @scenario
* @apicovered       inet_ntop()
* @precondition
* @postcondition
*/
static void tc_net_inet_ntop(void)
{
	struct in_addr in_addr;
	char dst[INET_ADDRSTRLEN];
#ifdef CONFIG_NET_IPv6
	char dst6[INET6_ADDRSTRLEN];
#endif
	const char *ret;

	in_addr.s_addr = 0x17071994;

#ifdef CONFIG_NET_IPv4
	ret = inet_ntop(AF_INET, &in_addr, dst, INET_ADDRSTRLEN);
	TC_ASSERT_NEQ("inet_ntop", ret, NULL);

	/* Failure case: size of destination buffer less than INET_ADDRSTRLEN bytes */
	ret = inet_ntop(AF_INET, &in_addr, dst, 7);
	TC_ASSERT_EQ("inet_ntop", ret, NULL);
#endif
#ifdef CONFIG_NET_IPv6
	ret = inet_ntop(AF_INET6, &in_addr, dst6, INET6_ADDRSTRLEN);
	TC_ASSERT_NEQ("inet_ntop", ret, NULL);
#endif
	/* Failure case: invalid address family */
	ret = inet_ntop(33, &in_addr, dst, INET_ADDRSTRLEN);
	TC_ASSERT_EQ("inet_ntop", ret, NULL);


	TC_SUCCESS_RESULT();
}

/**
* @testcase		tc_net_inet_pton
* @brief
* @scenario
* @apicovered		inet_pton()
* @precondition
* @postcondition
*/
static void tc_net_inet_pton(void)
{
	struct sockaddr_in addr_inet;
	int ret;

#ifdef CONFIG_NET_IPv4
	ret = inet_pton(AF_INET, "107.108.218.83", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, 1);

	/* Failure case: invalid network address */
	ret = inet_pton(AF_INET, "30051995", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, 0);
#endif
#ifdef CONFIG_NET_IPv6
	ret = inet_pton(AF_INET6, "0:0:0:0:0:0:0:1", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, 1);
#endif
	/* Failure case: invalid address family */
	ret = inet_pton(33, "107.108.218.83", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, -1);

	TC_SUCCESS_RESULT();
}

/**
* @testcase		tc_net_htons
* @brief
* @scenario
* @apicovered		htons()
* @precondition
* @postcondition
*/
static void tc_net_htons(void)
{
	uint16_t var = 20;
	uint16_t ret;
	uint16_t ref;

	ret = htons(var);
	ref = 0x1400;

	TC_ASSERT_EQ("inet", ret, ref);
	TC_SUCCESS_RESULT();
}

/**
* @testcase		tc_net_ntohs
* @brief
* @scenario
* @apicovered		ntohs()
* @precondition
* @postcondition
*/
static void tc_net_ntohs(void)
{
	uint16_t var = 0x1400;
	uint16_t ret;
	uint16_t ref;

	ret = ntohs(var);
	ref = 20;

	TC_ASSERT_EQ("inet", ret, ref);
	TC_SUCCESS_RESULT();
}

/**
* @testcase		tc_net_htonl
* @brief
* @scenario
* @apicovered		htonl()
* @precondition
* @postcondition
*/
static void tc_net_htonl(void)
{
	uint32_t var = 10;
	uint32_t ret;
	uint32_t ref;

	ret = htonl(var);
	ref = 0xa000000;

	TC_ASSERT_EQ("inet", ret, ref);
	TC_SUCCESS_RESULT();
}

/**
* @testcase		tc_net_ntohl
* @brief
* @scenario
* @apicovered		ntohl()
* @precondition
* @postcondition
*/
static void tc_net_ntohl(void)
{
	uint32_t var = 0xa000000;
	uint32_t ret;
	uint32_t ref;

	ret = ntohl(var);
	ref = 10;

	TC_ASSERT_EQ("inet", ret, ref);
	TC_SUCCESS_RESULT();
}


/****************************************************************************
 * Name: inet()
 ****************************************************************************/

int net_inet_main(void)
{
	tc_net_htonl();
	tc_net_htons();
	tc_net_inet_addr_p();
	tc_net_inet_aton_p();
#ifdef CONFIG_NET_IPv4
	tc_net_inet_ntoa_p();
#endif
	tc_net_inet_ntop();
	tc_net_inet_pton();
	tc_net_ntohl();
	tc_net_ntohs();

	return 0;
}
