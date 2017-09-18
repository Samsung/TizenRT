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

#define INVAL_AF        33
#define INVAL_SOCKSIZE  7

/**
* @testcases			: tc_inet_chksum_pbuf_p
* @brief				:
* @scenario				:
* @apicovered			: inet_chksum_pbuf
* @precondition			:
* @postcondition		:
*/
static void tc_inet_chksum_pbuf_p(void)
{
	struct pbuf *p1 = NULL;
	struct pbuf *p2 = NULL;
	struct pbuf *p3 = NULL;
	struct pbuf *p4 = NULL;
	int checksum = 0;

	p1 = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	p2 = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	p3 = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	p4 = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	pbuf_chain(p1, p4);
	checksum = inet_chksum_pbuf(p1);
	pbuf_free(p1);
	pbuf_free(p2);
	pbuf_free(p3);
	pbuf_free(p4);
	TC_ASSERT_NEQ("inet_chksum_pbuf", checksum, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_inet_chksum_pseudo_partial_p
* @brief				:
* @scenario				:
* @apicovered			: tc_inet_chksum_pseudo_partial_p
* @precondition			:
* @postcondition		:
*/
static void tc_inet_chksum_pseudo_partial_p(void)
{
	struct pbuf *p1 = NULL;
	struct pbuf *p2 = NULL;
	struct pbuf *p3 = NULL;
	struct pbuf *p4 = NULL;
	ip_addr_t *source = NULL;
	ip_addr_t *dest = NULL;
	int checksum = 0;

	source = (ip_addr_t *)malloc(sizeof(ip_addr_t));
	dest = (ip_addr_t *)malloc(sizeof(ip_addr_t));
	ipaddr_aton("198.51.100.4", source);
	ipaddr_aton("198.52.100.5", dest);
	p1 = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	p2 = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	p3 = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	p4 = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	pbuf_chain(p1, p4);
	checksum = inet_chksum_pseudo_partial(p1, source, dest, 6, PBUF_RAM, 6);
	TC_FREE_MEMORY(source);
	TC_FREE_MEMORY(dest);
	pbuf_free(p1);
	pbuf_free(p2);
	pbuf_free(p3);
	pbuf_free(p4);
	TC_ASSERT_NEQ("inet_chksum_pseudo_partial", checksum, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_inet_addr_p
* @brief				:
* @scenario				:
* @apicovered			: inet_addr()
* @precondition			:
* @postcondition		:
*/
static void tc_net_inet_addr_p(void)
{
	unsigned int ret;

	ret = inet_addr("127.0.0.1");

	TC_ASSERT_NEQ("inet", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_inet_aton_p
* @brief				:
* @scenario				:
* @apicovered			: inet_aton()
* @precondition			:
* @postcondition		:
*/
static void tc_net_inet_aton_p(void)
{
	struct sockaddr_in addr_inet;
	unsigned long ret;

	ret = inet_aton("127.0.0.1", &addr_inet.sin_addr);

	TC_ASSERT_NEQ("inet", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_inet_ntoa_p
* @brief				:
* @scenario				:
* @apicovered			: inet_ntoa()
* @precondition			:
* @postcondition		:
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
	const char *ret;

	in_addr.s_addr = 0x17071994;

#ifdef CONFIG_NET_IPv4
	ret = inet_ntop(AF_INET, &in_addr, dst, INET_ADDRSTRLEN);
	TC_ASSERT_NEQ("inet_ntop", ret, NULL);

	/* Failure case: size of destination buffer less than INET_ADDRSTRLEN bytes */
	ret = inet_ntop(AF_INET, &in_addr, dst, INVAL_SOCKSIZE);
	TC_ASSERT_EQ("inet_ntop", ret, NULL);
#endif
#ifdef CONFIG_NET_IPv6
	ret = inet_ntop(AF_INET6, &in_addr, dst, INET_ADDRSTRLEN);
	TC_ASSERT_NEQ("inet_ntop", ret, NULL);
#endif
	/* Failure case: invalid address family */
	ret = inet_ntop(INVAL_AF, &in_addr, dst, INET_ADDRSTRLEN);
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
	TC_ASSERT_EQ("inet_pton", ret, ONE);

	/* Failure case: invalid network address */
	ret = inet_pton(AF_INET, "30051995", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, ZERO);
#endif
#ifdef CONFIG_NET_IPv6
	ret = inet_pton(AF_INET6, "0:0:0:0:0:0:0:1", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, ONE);
#endif
	/* Failure case: invalid address family */
	ret = inet_pton(INVAL_AF, "107.108.218.83", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, NEG_VAL);

	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_htons
* @brief				:
* @scenario				:
* @apicovered			: htons()
* @precondition			:
* @postcondition		:
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
* @testcase				: tc_net_ntohs
* @brief				:
* @scenario				:
* @apicovered			: ntohs()
* @precondition			:
* @postcondition		:
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
* @testcase				: tc_net_htonl
* @brief				:
* @scenario				:
* @apicovered			: htonl()
* @precondition			:
* @postcondition		:
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
* @testcase				: tc_net_ntohl
* @brief				:
* @scenario				:
* @apicovered			: ntohl()
* @precondition			:
* @postcondition		:
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
	tc_inet_chksum_pseudo_partial_p();
	tc_inet_chksum_pbuf_p();
	return 0;
}
