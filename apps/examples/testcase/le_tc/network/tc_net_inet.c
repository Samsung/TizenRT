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

#define SOURCEIP        "198.51.100.4"
#define DESTIP          "198.51.100.5"
#define INVAL_AF        33
#define VAL             6
#define INVAL_SOCKSIZE  7
#define ADDR            11223344
#define ADDR1           55667788
#define VAL1            1122
#define VAL2            33
#define VAL3            123456780
#define VAL4            10
#define LOOPBACK        "127.0.0.1"

/**
* @testcases           :tc_inet_chksum_pbuf_p
* @brief               :calculate a checksum over a chain of pbufs.
* @scenario            :pbuf chain over that the checksum should be calculated.
* @apicovered          :inet_chksum_pbuf()
* @precondition        :allocate pbuf and chain pbuf.
* @postcondition       :none
* @return              :void
*/
static void tc_inet_chksum_pbuf_p(void)
{
	struct pbuf *p1 = NULL;
	struct pbuf *p2 = NULL;
	struct pbuf *p3 = NULL;
	struct pbuf *p4 = NULL;
	int checksum = ZERO;

	p1 = pbuf_alloc(PBUF_TRANSPORT, VAL, PBUF_RAM);
	TC_ASSERT_NEQ("pbuf_alloc", p1, NULL);
	p2 = pbuf_alloc(PBUF_TRANSPORT, VAL, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf_alloc", p2, NULL, goto cleanup1);
	p3 = pbuf_alloc(PBUF_TRANSPORT, VAL, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf_alloc", p3, NULL, goto cleanup2);
	p4 = pbuf_alloc(PBUF_TRANSPORT, VAL, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf_alloc", p4, NULL, goto cleanup3);
	pbuf_chain(p1, p4);
	checksum = inet_chksum_pbuf(p1);
	pbuf_free(p4);
cleanup3:
	pbuf_free(p3);
cleanup2:
	pbuf_free(p2);
cleanup1:
	pbuf_free(p1);
	TC_ASSERT_NEQ("inet_chksum_pbuf", checksum, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_inet_chksum_pseudo_partial_p
* @brief               :calculates the pseudo checksum used by TCP and UDP for a pbuf chain.
* @scenario            :chain the pbuf and check the pseudo checksum with IP address.
* @apicovered          :inet_chksum_pseudo_partial()
* @precondition        :allocate pbuf and chain pbuf.
* @postcondition       :none
* @return              :void
*/
static void tc_inet_chksum_pseudo_partial_p(void)
{
	struct pbuf *p1 = NULL;
	struct pbuf *p2 = NULL;
	struct pbuf *p3 = NULL;
	struct pbuf *p4 = NULL;
	ip_addr_t *source = NULL;
	ip_addr_t *dest = NULL;
	int checksum = ZERO;

	source = (ip_addr_t *)malloc(sizeof(ip_addr_t));
	TC_ASSERT_NEQ("malloc", source, NULL);

	dest = (ip_addr_t *)malloc(sizeof(ip_addr_t));
	TC_ASSERT_NEQ_CLEANUP("malloc", dest, NULL, TC_FREE_MEMORY(source));

	ipaddr_aton(SOURCEIP, source);
	ipaddr_aton(DESTIP, dest);
	p1 = pbuf_alloc(PBUF_TRANSPORT, VAL, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf_alloc", p1, NULL, goto cleanup1);

	p2 = pbuf_alloc(PBUF_TRANSPORT, VAL, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf_alloc", p2, NULL, goto cleanup2);

	p3 = pbuf_alloc(PBUF_TRANSPORT, VAL, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf_alloc", p3, NULL, goto cleanup3);

	p4 = pbuf_alloc(PBUF_TRANSPORT, VAL, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf_alloc", p4, NULL, goto cleanup4);

	pbuf_chain(p1, p4);
	checksum = inet_chksum_pseudo_partial(p1, source, dest, VAL, PBUF_RAM, 6);
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
* @testcase            :tc_net_inet_addr_p
* @brief               :internet address manipulation routines.
* @scenario            :converts the Internet host address from IPv4 numbers-and-dots
*                       notation into binary data in network byte order.
* @apicovered          :inet_addr()
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_inet_addr_p(void)
{
	unsigned int ret;

	ret = inet_addr(LOOPBACK);
	TC_ASSERT_NEQ("inet_addr", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_inet_aton_p
* @brief               :internet address manipulation routines.
* @scenario            :converts the Internet host address from the IPv4
*                       numbers-and-dots notation into binary form.
* @apicovered          :inet_aton
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_inet_aton_p(void)
{
	unsigned long ret;
	struct sockaddr_in addr_inet;

	ret = inet_aton(LOOPBACK, &addr_inet.sin_addr);
	TC_ASSERT_NEQ("inet_aton", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_inet_ntoa_p
* @brief               :internet address manipulation routines.
* @scenario            :converts the Internet host address in, given in network byte order,
*                       to a string in IPv4 dotted-decimal notation.
* @apicovered          :inet_ntoa
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_inet_ntoa_p(void)
{
	char *ret;
	struct sockaddr_in addr_inet;

	addr_inet.sin_addr.s_addr = ADDR;
	ret = inet_ntoa(addr_inet.sin_addr);

	TC_ASSERT_NEQ("inet_ntoa", ret, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_inet_ntop
* @brief               :convert IPv4 and IPv6 addresses from binary to text form.
* @scenario            :converts a numeric address into a text string suitable for presentation.
* @apicovered          :inet_ntop
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_inet_ntop(void)
{
	struct in_addr in_addr;
	char dst[INET_ADDRSTRLEN];
	const char *ret;

	in_addr.s_addr = ADDR1;

#ifdef CONFIG_NET_IPv4
	ret = inet_ntop(AF_INET, &in_addr, dst, INET_ADDRSTRLEN);
	TC_ASSERT_NEQ("inet_ntop", ret, NULL);

	/* Failure case:size of destination buffer less than INET_ADDRSTRLEN bytes */
	ret = inet_ntop(AF_INET, &in_addr, dst, INVAL_SOCKSIZE);
	TC_ASSERT_EQ("inet_ntop", ret, NULL);
#endif
#ifdef CONFIG_NET_IPv6
	ret = inet_ntop(AF_INET6, &in_addr, dst, INET_ADDRSTRLEN);
	TC_ASSERT_NEQ("inet_ntop", ret, NULL);
#endif
	/* Failure case:invalid address family */
	ret = inet_ntop(INVAL_AF, &in_addr, dst, INET_ADDRSTRLEN);
	TC_ASSERT_EQ("inet_ntop", ret, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_inet_pton
* @brief               :convert IPv4 and IPv6 addresses from text to binary form.
* @scenario            :converts an address in its standard text presentation
*                       form into its numeric binary form
* @apicovered          :inet_pton
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_inet_pton(void)
{
	struct sockaddr_in addr_inet;
	int ret;

#ifdef CONFIG_NET_IPv4
	ret = inet_pton(AF_INET, "107.108.218.83", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, ONE);

	/* Failure case:invalid network address */
	ret = inet_pton(AF_INET, "30051995", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, ZERO);
#endif
#ifdef CONFIG_NET_IPv6
	ret = inet_pton(AF_INET6, "0:0:0:0:0:0:0:1", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, ONE);
#endif
	/* Failure case:invalid address family */
	ret = inet_pton(INVAL_AF, "107.108.218.83", &(addr_inet.sin_addr));
	TC_ASSERT_EQ("inet_pton", ret, NEG_VAL);

	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_htons
* @brief               :to convert from host order to network order.
* @scenario            :htons() converts the unsigned short integer hostshort
*                       from host byte order to network byte order.
* @apicovered          :htons
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_htons(void)
{
	uint16_t ret = htons(VAL2);
	TC_ASSERT_EQ("htons", ret, VAL1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_ntohs
* @brief               :to convert network to host order.
* @scenario            :ntohs() converts the unsigned short integer netshort
*                       from network byte order to host byte order.
* @apicovered          :ntohs
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_ntohs(void)
{
	uint16_t ret = ntohs(VAL1);
	TC_ASSERT_EQ("ntohs", ret, VAL2);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_htonl
* @brief               :to convert from host order to network order.
* @scenario            :htonl() converts the unsigned integer hostlong
*                       from host byte order to network byte order.
* @apicovered          :htonl
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_htonl(void)
{
	uint32_t ret = htonl(VAL4);
	TC_ASSERT_EQ("htonl", ret, VAL3);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_ntohl
* @brief               :to convert network to host byte order
* @scenario            :ntohl() converts the unsigned integer netlong from
*                       network byte order to host byte order.
* @apicovered          :ntohl
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_ntohl(void)
{
	uint32_t ret = ntohl(VAL3);
	TC_ASSERT_EQ("ntohl", ret, VAL4);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name:inet()
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
