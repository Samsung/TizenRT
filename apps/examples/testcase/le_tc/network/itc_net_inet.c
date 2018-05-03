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

/// @file itc_net_inet.c
/// @brief ITC Test Case Example for inet() API

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <tinyara/config.h>
#include "tc_internal.h"

/**
* @testcase          :itc_net_inet_addr_n
* @brief             :function converts the Internet host address cp from IPv4
*                     numbers-and-dots notation into binary data in network byte order.
* @scenario          :passing invalid address
* @apicovered        :inet_addr()
* @precondition      :None
* @postcondition     :None
*/
static void itc_net_inet_addr_n(void)
{
	int ret;

	ret = inet_addr("q");
	TC_ASSERT_EQ("inet", ret, -1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase          :itc_net_inet_aton_n
* @brief             :converts the Internet host address cp from the IPv4
*                     numbers-and-dots notation into binary form
* @scenario          :Passing invalid args
* @apicovered        :inet_aton()
* @precondition      :None
* @postcondition     :None
*/
static void itc_net_inet_aton_n(void)
{
	unsigned long ret;

	ret = inet_aton(NULL, NULL);

	TC_ASSERT_EQ("inet", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase          :itc_net_inet_ntop_p
* @brief             :function  converts  the network address structure src 
*                     in the af address family into a character string*
* @scenario          :passing hex value in inet_ntop
* @apicovered        :inet_ntop()
* @precondition      :None
* @postcondition     :None
*/
static void itc_net_inet_ntop_p(void)
{
#ifdef CONFIG_NET_IPv4
	struct in_addr in_addr;
	char dst[INET_ADDRSTRLEN];
	in_addr.s_addr = 0x17071994;
#endif

#ifdef CONFIG_NET_IPv6
	struct in6_addr in6_addr;
	char dst6[INET6_ADDRSTRLEN];
	in6_addr.s6_addr32[0] = 0x17071994;
	in6_addr.s6_addr32[1] = 0x17071994;
	in6_addr.s6_addr32[2] = 0x17071994;
	in6_addr.s6_addr32[3] = 0x17071994;
#endif
	const char *ret;

#ifdef CONFIG_NET_IPv4
	ret = inet_ntop(AF_INET, &in_addr, dst, INET_ADDRSTRLEN);
	TC_ASSERT_NEQ("inet_ntop", ret, NULL);
#endif

#ifdef CONFIG_NET_IPv6
	ret = inet_ntop(AF_INET6, &in6_addr, dst6, INET6_ADDRSTRLEN);
	TC_ASSERT_NEQ("inet_ntop", ret, NULL);
#endif

	TC_SUCCESS_RESULT();
}

/**
* @testcase          :itc_net_inet_ntop_n
* @brief             :function  converts  the network address structure src 
*                     in the af address family into a character string*
* @scenario          :passing invalid args to inet_ntop
* @apicovered        :inet_ntop()
* @precondition      :None
* @postcondition     :None
*/
static void itc_net_inet_ntop_n(void)
{
	struct in_addr in_addr;
	char dst[INET_ADDRSTRLEN];
	const char *ret;

	in_addr.s_addr = 0x17071994;

#ifdef CONFIG_NET_IPv4
	ret = inet_ntop(AF_INET, &in_addr, dst, 7);
	TC_ASSERT_EQ("inet_ntop", ret, NULL);
#endif
#ifdef CONFIG_NET_IPv6
	ret = inet_ntop(AF_INET6, &in_addr, dst, 7);
	TC_ASSERT_EQ("inet_ntop", ret, NULL);
#endif
	ret = inet_ntop(33, &in_addr, dst, INET_ADDRSTRLEN);
	TC_ASSERT_EQ("inet_ntop", ret, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         :itc_net_inet_pton_p
* @brief            :function converts the character string src into a 
*                    network address structure in the af address family
* @scenario         :passing IPv4 addr in dot format and IPv6 in colon format
* @apicovered       :inet_pton()
* @precondition     :None
* @postcondition    :None
*/
static void itc_net_inet_pton_p(void)
{
	struct sockaddr_in addr_inet;
	int ret;

#ifdef CONFIG_NET_IPv4
	ret = inet_pton(AF_INET, "107.108.218.83", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, 1);
#endif
#ifdef CONFIG_NET_IPv6
	ret = inet_pton(AF_INET6, "0:0:0:0:0:0:0:1", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, 1);
#endif
	TC_SUCCESS_RESULT();
}

/**
* @testcase         :itc_net_inet_pton_n
* @brief            :function converts the character string src into a 
*                    network address structure in the af address family
* @scenario         :passing invalid args to inet_pton
* @apicovered       :inet_pton()
* @precondition     :None
* @postcondition    :None
*/
static void itc_net_inet_pton_n(void)
{
	struct sockaddr_in addr_inet;
	int ret;

#ifdef CONFIG_NET_IPv4
	ret = inet_pton(AF_INET, "30051995", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, 0);
#endif
#ifdef CONFIG_NET_IPv6
	ret = inet_pton(AF_INET6, "0:0:0:0:0:0:0:1", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, 1);
#endif
	ret = inet_pton(33, "107.108.218.83", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, -1);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         :itc_net_ntohl_p
* @brief            :function converts the unsigned integer
*                    netlong from network byte order to host byte order
* @scenario         :passing uint32_t hostlong and comparing value
* @apicovered       :ntohl()
* @precondition     :None
* @postcondition    :None
*/
static void itc_net_ntohl_p(void)
{
	uint32_t var = 0xa000000;
	uint32_t ret;
	uint32_t ref;

	ret = ntohl(var);
	ref = 10;

	TC_ASSERT_EQ("inet", ret, ref);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         :itc_net_ntohs_P
* @brief            :function converts the unsigned short integer
*                    hostshort from host byte order to network byte order. 
* @scenario         :passin guint16_t hostshort and reconvering to uint16_t netshort
*                    comparing their values
* @apicovered       :ntohs()
* @precondition     :None
* @postcondition    :None
*/
static void itc_net_ntohs_p(void)
{
	uint16_t var = 20;
	uint16_t ret;

	ret = ntohs(htons(var));
	TC_ASSERT_EQ("inet", ret, var);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
* Name: inet()
****************************************************************************/

int itc_net_inet_main(void)
{
	itc_net_inet_addr_n();
	itc_net_inet_aton_n();
	itc_net_inet_ntop_p();
	itc_net_inet_ntop_n();
	itc_net_inet_pton_p();
	itc_net_inet_pton_n();
	itc_net_ntohl_p();
	itc_net_ntohs_p();

	return 0;
}
