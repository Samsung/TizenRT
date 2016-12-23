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

/// @file tc_net_socket.c
/// @brief Test Case Example for socket() API
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
* @fn                   :tc_net_socket_pf_inet_sock_raw_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_pf_inet_sock_raw_p(void)
{
	int fd = -1;
	fd = socket(PF_INET, SOCK_RAW, 0);
	if (fd < 0) {
		printf("tc_net_socket_pf_inet_sock_raw_p FAIL: TCP socket create failure %d\n", errno);
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_pf_inet_sock_raw_p PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_dgram_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_dgram_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_dgram_p FAIL: TCP socket create failure %d\n", errno);
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_dgram_p PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_pf_inet_sock_dgram_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_pf_inet_sock_dgram_p(void)
{
	int fd = -1;
	fd = socket(PF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		printf("tc_net_socket_pf_inet_sock_dgram_p FAIL: TCP socket create failure %d\n", errno);
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_pf_inet_sock_dgram_p PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_pf_inet_sock_stream_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_pf_inet_sock_stream_p(void)
{

	int fd = -1;
	fd = socket(PF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		printf("tc_net_socket_pf_inet_sock_stream_p FAIL: TCP socket create failure %d\n", errno);
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_pf_inet_sock_stream_p PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_dgram_tcp_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_dgram_tcp_n(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_sock_dgram_tcp_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_dgram_tcp_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_dgram_udp_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_dgram_udp_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_dgram_udp_p FAIL: UDP socket create failure %d\n", errno);
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_dgram_udp_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_soc_dgram_icmp_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_soc_dgram_icmp_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_soc_dgram_icmp_p FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_soc_dgram_icmp_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_dgram_igmp_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_dgram_igmp_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IGMP);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_dgram_igmp_p: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_dgram_igmp_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_dgram_ipv6_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_dgram_ipv6_n(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IPV6);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_sock_dgram_ipv6_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_dgram_ipv6_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_dgram_routing_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_dgram_routing_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ROUTING);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_dgram_routing_p FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_dgram_routing_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_dgram_fragment_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_dgram_fragment_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_FRAGMENT);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_dgram_fragment_p FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_dgram_fragment_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_dgram_icmpv6_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_dgram_icmpv6_n(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMPV6);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_sock_dgram_icmpv6_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_dgram_icmpv6_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_dgram_tcp_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_dgram_tcp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_TCP);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_dgram_tcp_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_dgram_tcp_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_dgram_udp_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_dgram_udp_p(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_UDP);
	if (fd < 0) {
		printf("tc_net_socket_af_unix_sock_dgram_udp_p FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_dgram_udp_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_dgram_icmp_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_dgram_icmp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_ICMP);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_dgram_icmp_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_dgram_icmp_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_dgram_igmp_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_dgram_igmp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_IGMP);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_dgram_igmp_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_dgram_igmp_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_x25_sock_stream_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_x25_sock_stream_n(void)
{
	int fd = -1;
	fd = socket(AF_X25, SOCK_STREAM, 0);
	if (fd != -1) {
		printf("tc_net_socket_af_x25_sock_stream_n: socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_x25_sock_stream_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_ax25_sock_stream_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_ax25_sock_stream_n(void)
{
	int fd = -1;
	fd = socket(AF_AX25, SOCK_STREAM, 0);
	if (fd != -1) {
		printf("tc_net_socket_af_ax25_sock_stream_n: socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_ax25_sock_stream_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_packet_sock_stream_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_packet_sock_stream_n(void)
{
	int fd = -1;
	fd = socket(AF_PACKET, SOCK_STREAM, 0);
	if (fd != -1) {
		printf("tc_net_socket_af_packet_sock_stream_n: socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_packet_sock_stream_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_stream_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_stream_p(void)
{
	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0) {
		printf("tc_net_socket_af_unix_sock_stream_p: TCP socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_stream_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_stream_tcp_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_stream_tcp_p(void)
{
	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		printf("tc_net_socket_af_unix_sock_stream_tcp_p: TCP socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_stream_tcp_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_stream_udp_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_stream_udp_n(void)
{
	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_UDP);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_stream_udp_n: socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_stream_udp_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_stream_icmp_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_stream_icmp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_ICMP);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_stream_icmp_n: socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_stream_icmp_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_stream_igmp_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_stream_igmp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_IGMP);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_stream_igmp_n: socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_stream_igmp_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_stream_ipv6_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_stream_ipv6_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_IPV6);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_stream_ipv6_n: socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_stream_ipv6_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unixsock_stream_routing_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unixsock_stream_routing_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_ROUTING);
	if (fd != -1) {
		printf("tc_net_socket_af_unixsock_stream_routing_n: socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unixsock_stream_routing_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_stream_icmpv6_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_stream_icmpv6_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_ICMPV6);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_stream_icmpv6_n: \n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_stream_icmpv6_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_netlink_sock_stream_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_netlink_sock_stream_n(void)
{
	int fd = -1;
	fd = socket(AF_NETLINK, SOCK_STREAM, 0);
	if (fd != -1) {
		printf("tc_net_socket_af_netlink_sock_stream_n: failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_netlink_sock_stream_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unspec_sock_stream_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unspec_sock_stream_p(void)
{
	int fd = -1;
	fd = socket(AF_UNSPEC, SOCK_STREAM, 0);
	if (fd < 0) {
		printf("tc_net_socket_af_unspec_sock_stream_p: socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unspec_sock_stream_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_tcp_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_tcp_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_stream_tcp_p: TCP IPPROTO_TCP failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_tcp_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_protocol_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_protocol_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 256);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_sock_stream_protocol_n: TCP socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_protocol_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_domain_sock_stream_protocol_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_domain_sock_stream_protocol_n(void)
{

	int fd = -1;
	fd = socket(30, SOCK_STREAM, 256);
	if (fd != -1) {
		printf("tc_net_socket_domain_sock_stream_protocol_n: TCP socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_domain_sock_stream_protocol_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_type_protocol_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_type_protocol_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, 7, 256);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_type_protocol_n: TCP socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_type_protocol_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_domain_type_protocol_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_domain_type_protocol_n(void)
{
	int fd = -1;
	fd = socket(13, 6, 256);
	if (fd != -1) {
		printf("tc_net_socket_domain_type_protocol_n: TCP socket failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_domain_type_protocol_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_domain_sock_stream_tcp_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_domain_sock_stream_tcp_n(void)
{
	int fd = -1;
	fd = socket(40, SOCK_STREAM, IPPROTO_TCP);
	if (fd != -1) {
		printf("tc_net_socket_domain_sock_stream_tcp_n: IPPROTO_TCP failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_domain_sock_stream_tcp_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_udp_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_udp_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_sock_stream_udp_n: IPPROTO_UDP failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_udp_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_icmp_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_icmp_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_ICMP);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_stream_icmp_p: TCP IPPROTO_ICMP failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_icmp_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_igmp_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_igmp_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IGMP);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_stream_igmp_p: TCP IPPROTO_IGMP failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_igmp_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_protoipv6_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_protoipv6_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IPV6);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_sock_stream_protoipv6_n: TCP IPPROTO_IPV6 failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_protoipv6_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_routing_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_routing_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_ROUTING);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_stream_routing_p: TCP IPPROTO_ROUTING failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_routing_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_fragment_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_fragment_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_FRAGMENT);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_stream_fragment_p: TCP IPPROTO_FRAGMENT failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_fragment_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_icmpv6_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_icmpv6_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_ICMPV6);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_sock_stream_icmpv6_n: TCP IPPROTO_ICMPV6 failure fd = %d err=%d\n", fd, errno);
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_icmpv6_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet6_sock_stream_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet6_sock_stream_p(void)
{
	int fd = -1;
	fd = socket(AF_INET6, SOCK_STREAM, 0);
	if (fd < 0) {
		printf("tc_net_socket_af_inet6_sock_stream_p: TCP AF_INET6\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet6_sock_stream_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_type_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_type_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, 5, 0);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_sock_stream_type_n: TCP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_type_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_domain_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_domain_n(void)
{
	int fd = -1;
	fd = socket(12, SOCK_STREAM, 0);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_sock_stream_domain_n:  TCP socket create failure %d\n", errno);
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_domain_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_domain_protocol_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_domain_protocol_n(void)
{
	int fd = -1;
	fd = socket(15, SOCK_STREAM, 289);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_sock_stream_domain_protocol_n: TCP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_domain_protocol_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_type_protocol_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_type_protocol_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, 7, 300);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_sock_stream_type_protocol_n: TCP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_type_protocol_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_stream_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_stream_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_stream_p: TCP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_stream_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_invalid_domain_sock_stream_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_invalid_domain_sock_stream_n(void)
{
	int fd = -1;
	fd = socket(-1, SOCK_STREAM, 0);
	if (fd != -1) {
		printf("tc_net_socket_invalid_domain_sock_stream_n: TCP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_invalid_domain_sock_stream_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_dgram_ipv6_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_dgram_ipv6_n(void)
{
	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_IPV6);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_dgram_ipv6_n: UDP socket create failure \n");
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_unix_sock_dgram_ipv6_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_dgram_routing_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_dgram_routing_n(void)
{
	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_ROUTING);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_dgram_routing_n FAIL: UDP socket create failure \n");
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_unix_sock_dgram_routing_n: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_dgram_fragment_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_dgram_fragment_p(void)
{
	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_FRAGMENT);
	if (fd < 0) {
		printf("tc_net_socket_af_unix_sock_dgram_fragment_p FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_unix_sock_dgram_fragment_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_dgram_icmpv6
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_dgram_icmpv6_n(void)
{
	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_ICMPV6);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_dgram_icmpv6_n FAIL: UDP socket create failure %d\n", errno);
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_unix_sock_dgram_icmpv6_n : PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_netlink_sock_dgram_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_netlink_sock_dgram_p(void)
{
	int fd = -1;
	fd = socket(AF_NETLINK, SOCK_DGRAM, 0);
	if (fd < 0) {
		printf("tc_net_socket_af_netlink_sock_dgram_p FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_netlink_sock_dgram_p:PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unspec_sock_dgram_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unspec_sock_dgram_p(void)
{
	int fd = -1;
	fd = socket(AF_UNSPEC, SOCK_DGRAM, 0);
	if (fd < 0) {
		printf("tc_net_socket_af_unspec_sock_dgram_p FAIL: UDP socket create failure \n");
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_unspec_sock_dgram_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_local_sock_dgram_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_local_sock_dgram_p(void)
{
	int fd = -1;
	fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
	if (fd < 0) {
		printf("tc_net_socket_af_local_sock_dgram_p: UDP socket create failure %d\n", errno);
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_local_sock_dgram_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_x25_sock_dgram_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_x25_sock_dgram_n(void)
{
	int fd = -1;
	fd = socket(AF_X25, SOCK_DGRAM, 0);
	if (fd != -1) {
		printf("tc_net_socket_af_x25_sock_dgram_n FAIL: UDP socket create failure \n");
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_x25_sock_dgram_n:PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_ax25_sock_dgram_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_ax25_sock_dgram_n(void)
{
	int fd = -1;
	fd = socket(AF_AX25, SOCK_DGRAM, 0);
	if (fd != -1) {
		printf("tc_net_socket_af_ax25_sock_dgram_n FAIL: UDP socket create failure \n");
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_ax25_sock_dgram_n:PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_packet_sock_dgram_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_packet_sock_dgram_p(void)
{
	int fd = -1;
	fd = socket(AF_PACKET, SOCK_DGRAM, 0);
	if (fd < 0) {
		printf("tc_net_socket_af_packet_sock_dgram_p FAIL: UDP socket create failure \n");
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_packet_sock_dgram_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_raw_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_raw_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, 0);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_raw_p FAIL: UDP socket create failure \n");
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_inet_sock_raw_p:PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_raw_tcp_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_raw_tcp_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_raw_tcp_p FAIL: UDP socket create failure %d\n", errno);
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_inet_sock_raw_tcp_p PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_raw_udp_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/

static void tc_net_socket_af_inet_sock_raw_udp_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_raw_udp_p FAIL: UDP socket create failure \n");
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_inet_sock_raw_udp_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_raw_icmp_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_raw_icmp_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_raw_icmp_p FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_inet_sock_raw_icmp_p PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_raw_igmp_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_raw_igmp_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_IGMP);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_raw_igmp_p FAIL: UDP socket create failure \n");
		nw_total_fail++;
		RETURN_ERR;
	}
	close(fd);
	printf("tc_net_socket_af_inet_sock_raw_igmp_p: PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_raw_ipv6_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_raw_ipv6_n(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_IPV6);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_sock_raw_ipv6_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_raw_ipv6_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_raw_routing_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_raw_routing_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_ROUTING);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_raw_routing_p FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_raw_routing_p PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_raw_fragment_p
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_raw_fragment_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_FRAGMENT);
	if (fd < 0) {
		printf("tc_net_socket_af_inet_sock_raw_fragment_p FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_raw_fragment_p PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_inet_sock_raw_icmpv6_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_inet_sock_raw_icmpv6_n(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMPV6);
	if (fd != -1) {
		printf("tc_net_socket_af_inet_sock_raw_icmpv6_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_inet_sock_raw_icmpv6_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_raw_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_raw_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, 0);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_raw_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_raw_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_raw_tcp_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_raw_tcp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_TCP);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_raw_tcp_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_raw_tcp_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_raw_udp_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_raw_udp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_UDP);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_raw_udp_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_raw_udp_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_raw_icmp_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_raw_icmp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_ICMP);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_raw_icmp_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_raw_icmp_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_raw_igmp_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_raw_igmp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_IGMP);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_raw_igmp_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_raw_igmp_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_raw_ipv6_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_raw_ipv6_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_IPV6);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_raw_ipv6_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_raw_ipv6_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_raw_routing_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_raw_routing_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_ROUTING);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_raw_routing_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_raw_routing_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_raw_frag_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_raw_fragment_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_FRAGMENT);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_raw_frag_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_raw_frag_n PASS\n");
	nw_total_pass++;
}

/**
* @fn                   :tc_net_socket_af_unix_sock_raw_icmpv6_n
* @brief                :
* @scenario             :
* API's covered         :socket()
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void tc_net_socket_af_unix_sock_raw_icmpv6_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_ICMPV6);
	if (fd != -1) {
		printf("tc_net_socket_af_unix_sock_raw_icmpv6_n FAIL: UDP socket create failure\n");
		nw_total_fail++;
		RETURN_ERR;
	}

	close(fd);
	printf("tc_net_socket_af_unix_sock_raw_icmpv6_n PASS\n");
	nw_total_pass++;
}

/****************************************************************************
 * Name: socket()
 ****************************************************************************/

int net_socket_main(void)
{
	tc_net_socket_af_inet_sock_stream_p();
	tc_net_socket_pf_inet_sock_stream_p();
	tc_net_socket_af_inet_sock_dgram_p();
	tc_net_socket_pf_inet_sock_dgram_p();
	tc_net_socket_pf_inet_sock_raw_p();
	tc_net_socket_af_inet_sock_dgram_tcp_n();
	tc_net_socket_af_inet_sock_dgram_udp_p();
	tc_net_socket_af_inet_soc_dgram_icmp_p();
	tc_net_socket_af_inet_sock_dgram_igmp_p();
	tc_net_socket_af_inet_sock_dgram_ipv6_n();
	tc_net_socket_af_inet_sock_dgram_routing_p();
	tc_net_socket_af_inet_sock_dgram_fragment_p();
	tc_net_socket_invalid_domain_sock_stream_n();
	tc_net_socket_af_inet_sock_dgram_icmpv6_n();
	tc_net_socket_af_unix_sock_dgram_tcp_n();
	tc_net_socket_af_unix_sock_dgram_udp_p();
	tc_net_socket_af_unix_sock_dgram_icmp_n();
	tc_net_socket_af_unix_sock_dgram_igmp_n();
	tc_net_socket_af_x25_sock_stream_n();
	tc_net_socket_af_ax25_sock_stream_n();
	tc_net_socket_af_packet_sock_stream_n();
	tc_net_socket_af_unix_sock_stream_p();
	tc_net_socket_af_unix_sock_stream_udp_n();
	tc_net_socket_af_unix_sock_stream_icmp_n();
	tc_net_socket_af_unix_sock_stream_igmp_n();
	tc_net_socket_af_unix_sock_stream_ipv6_n();
	tc_net_socket_af_unixsock_stream_routing_n();
	tc_net_socket_af_unix_sock_stream_icmpv6_n();
	tc_net_socket_af_netlink_sock_stream_n();
	tc_net_socket_af_unspec_sock_stream_p();
	tc_net_socket_af_unix_sock_stream_tcp_p();
	tc_net_socket_af_inet_sock_stream_protocol_n();
	tc_net_socket_domain_sock_stream_protocol_n();
	tc_net_socket_af_inet_sock_stream_tcp_p();
	tc_net_socket_af_inet_type_protocol_n();
	tc_net_socket_domain_type_protocol_n();
	tc_net_socket_domain_sock_stream_tcp_n();
	tc_net_socket_af_inet_sock_stream_udp_n();
	tc_net_socket_af_inet_sock_stream_icmp_p();
	tc_net_socket_af_inet_sock_stream_igmp_p();
	tc_net_socket_af_inet_sock_stream_protoipv6_n();
	tc_net_socket_af_inet_sock_stream_routing_p();
	tc_net_socket_af_inet_sock_stream_fragment_p();
	tc_net_socket_af_inet_sock_stream_icmpv6_n();
	tc_net_socket_af_inet6_sock_stream_p();
	tc_net_socket_af_inet_sock_stream_type_n();
	tc_net_socket_af_inet_sock_stream_domain_n();
	tc_net_socket_af_inet_sock_stream_domain_protocol_n();
	tc_net_socket_af_inet_sock_stream_type_protocol_n();
	tc_net_socket_af_unix_sock_dgram_ipv6_n();
	tc_net_socket_af_unix_sock_dgram_routing_n();
	tc_net_socket_af_unix_sock_dgram_fragment_p();
	tc_net_socket_af_unix_sock_dgram_icmpv6_n();
	tc_net_socket_af_netlink_sock_dgram_p();
	tc_net_socket_af_unspec_sock_dgram_p();
	tc_net_socket_af_local_sock_dgram_p();
	tc_net_socket_af_x25_sock_dgram_n();
	tc_net_socket_af_ax25_sock_dgram_n();
	tc_net_socket_af_packet_sock_dgram_p();
	tc_net_socket_af_inet_sock_raw_p();
	tc_net_socket_af_inet_sock_raw_tcp_p();
	tc_net_socket_af_inet_sock_raw_udp_p();
	tc_net_socket_af_inet_sock_raw_icmp_p();
	tc_net_socket_af_inet_sock_raw_igmp_p();
	tc_net_socket_af_inet_sock_raw_ipv6_n();
	tc_net_socket_af_inet_sock_raw_routing_p();
	tc_net_socket_af_inet_sock_raw_fragment_p();
	tc_net_socket_af_inet_sock_raw_icmpv6_n();
	tc_net_socket_af_unix_sock_raw_n();
	tc_net_socket_af_unix_sock_raw_tcp_n();
	tc_net_socket_af_unix_sock_raw_udp_n();
	tc_net_socket_af_unix_sock_raw_icmp_n();
	tc_net_socket_af_unix_sock_raw_igmp_n();
	tc_net_socket_af_unix_sock_raw_ipv6_n();
	tc_net_socket_af_unix_sock_raw_routing_n();
	tc_net_socket_af_unix_sock_raw_fragment_n();
	tc_net_socket_af_unix_sock_raw_icmpv6_n();

	return 0;
}
