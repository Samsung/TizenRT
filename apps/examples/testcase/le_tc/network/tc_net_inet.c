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
* @testcases           : tc_inet_chksum_pbuf_p
* @brief               : Calculate a checksum over a chain of pbufs.
* @scenario            : pbuf chain over that the checksum should be calculated.
* @apicovered          : inet_chksum_pbuf()
* @precondition        : allocate pbuf and chain pbuf.
* @postcondition       : none
* @return              : void
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
* @testcase            : tc_inet_chksum_pseudo_partial_p
* @brief               : Calculates the pseudo checksum used by TCP and UDP for a pbuf chain.
* @scenario            : Chain the pbuf and check the pseudo checksum with IP address.
* @apicovered          : inet_chksum_pseudo_partial()
* @precondition        : allocate pbuf and chain pbuf.
* @postcondition       : none
* @return              : void
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
	TC_ASSERT_NEQ("malloc", source, NULL);

	dest = (ip_addr_t *)malloc(sizeof(ip_addr_t));
	TC_ASSERT_NEQ_CLEANUP("malloc", dest, NULL, TC_FREE_MEMORY(source));

	ipaddr_aton("198.51.100.4", source);
	ipaddr_aton("198.52.100.5", dest);
	p1 = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf_malloc", p1, NULL, goto cleanup1);

	p2 = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf_malloc", p2, NULL, goto cleanup2);

	p3 = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf_malloc", p3, NULL, goto cleanup3);

	p4 = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf_malloc", p4, NULL, goto cleanup4);

	pbuf_chain(p1, p4);
	checksum = inet_chksum_pseudo_partial(p1, source, dest, 6, PBUF_RAM, 6);
	pbuf_free(p4);
cleanup4:
	pbuf_free(p3);
cleanup3:
	pbuf_free(p2);
cleanup2:
	pbuf_free(p1);
cleanup1:
	TC_FREE_MEMORY(source);
	TC_FREE_MEMORY(dest);
	TC_ASSERT_NEQ("inet_chksum_pseudo_partial", checksum, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_inet_addr_p
* @brief               : Internet address manipulation routines.
* @scenario            : Converts the Internet host address from IPv4 numbers-and-dots notation into binary data in network byte order.
* @apicovered          : inet_addr()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_inet_addr_p(void)
{
	unsigned int ret;

	ret = inet_addr("127.0.0.1");
	TC_ASSERT_NEQ("inet", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_inet_aton_p
* @brief               : Internet address manipulation routines.
* @scenario            : Converts the Internet host address from the IPv4 numbers-and-dots notation into binary form.
* @apicovered          : inet_aton()
* @precondition        : none
* @postcondition       : none
* @return              : void
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
* @testcase            : tc_net_inet_ntoa_p
* @brief               : Internet address manipulation routines.
* @scenario            : Converts the Internet host address in, given in network byte order, to a string in IPv4 dotted-decimal notation.
* @apicovered          : inet_ntoa()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_inet_ntoa_p(void)
{
	struct sockaddr_in addr_inet;
	char *ret;

	addr_inet.sin_addr.s_addr = 0x6601a8c0;
	ret = inet_ntoa(addr_inet.sin_addr);

	TC_ASSERT_NEQ("inet", *ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_inet_ntop
* @brief               : Convert IPv4 and IPv6 addresses from binary to text form.
* @scenario            : converts a numeric address into a text string suitable for presentation.
* @apicovered          : inet_ntop()
* @precondition        : none
* @postcondition       : none
* @return              : void
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
* @testcase            : tc_net_inet_pton
* @brief               : Convert IPv4 and IPv6 addresses from text to binary form.
* @scenario            : Converts an address in its standard text presentation form into its numeric binary form. 
* @apicovered          : inet_pton()
* @precondition        : none
* @postcondition       : none
* @return              : void
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
* @testcase            : tc_net_htons
* @brief               : To convert from host order to network order.
* @scenario            : The htons() converts the unsigned short integer hostshort from host byte order to network byte order.
* @apicovered          : htons()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_htons(void)
{
	uint16_t var = 20;
	uint16_t ret;
	uint16_t ref = 0x1400;

	ret = htons(var);
	TC_ASSERT_EQ("inet", ret, ref);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_ntohs
* @brief               : To convert network to host order.
* @scenario            : The ntohs() converts the unsigned short integer netshort from network byte order to host byte order.
* @apicovered          : ntohs()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_ntohs(void)
{
	uint16_t var = 0x1400;
	uint16_t ret;
	uint16_t ref = 20;

	ret = ntohs(var);
	TC_ASSERT_EQ("inet", ret, ref);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_htonl
* @brief               : To convert from host order to network order.
* @scenario            : The htonl() converts the unsigned integer hostlong from host byte order to network byte order.
* @apicovered          : htonl()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_htonl(void)
{
	uint32_t var = 10;
	uint32_t ret;
	uint32_t ref = 0xa000000;

	ret = htonl(var);
	TC_ASSERT_EQ("inet", ret, ref);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_ntohl
* @brief               : To convert network to host byte order
* @scenario            : The ntohl() converts the unsigned integer netlong from network byte order to host byte order.
* @apicovered          : ntohl()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_ntohl(void)
{
	uint32_t var = 0xa000000;
	uint32_t ret;
	uint32_t ref = 10;

	ret = ntohl(var);
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
