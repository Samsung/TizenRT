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
#include <netutils/netlib.h>

#include <sys/socket.h>

#include "tc_internal.h"

#define INVAL_DOMAIN 30
#define INVAL_PROTO  6
#define INVAL_TYPE   256

/**
* @testcase            : tc_net_socket_pf_inet_sock_raw_p
* @brief               : raw socket
* @scenario            : using for pf_inet address family and ip protocol 
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : int 
*/
static void tc_net_socket_pf_inet_sock_raw_p(void)
{
	int fd = socket(PF_INET, SOCK_RAW, 0);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()

	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_p
* @brief               : udp socket
* @scenario            : using for af_inet address family and ip protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : int 
*/
static void tc_net_socket_af_inet_sock_dgram_p(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, 0);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_pf_inet_sock_dgram_p
* @brief               : udp socket
* @scenario            : using for pf_inet address family and ip prtocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : int 
*/
static void tc_net_socket_pf_inet_sock_dgram_p(void)
{
	int fd = socket(PF_INET, SOCK_DGRAM, 0);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_pf_inet_sock_stream_p
* @brief               : tcp socket
* @scenario            : using for pf_inet address family and ip protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : int 
*/
static void tc_net_socket_pf_inet_sock_stream_p(void)
{
	int fd = socket(PF_INET, SOCK_STREAM, 0);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_tcp_n
* @brief               : udp socket
* @scenario            : using for af_inet address family 
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : int 
*/
static void tc_net_socket_af_inet_sock_dgram_tcp_n(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_udp_p
* @brief               : udp socket
* @scenario            : using for af_inet address family and udp protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : int 
*/
static void tc_net_socket_af_inet_sock_dgram_udp_p(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_soc_dgram_icmp_p
* @brief               : udp socket
* @scenario            : using for af_inet and for icmp protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_soc_dgram_icmp_p(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);

}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_igmp_p
* @brief               : udp socket
* @scenario            : af_inet address family and igmp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_dgram_igmp_p(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IGMP);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_ipv6_n
* @brief               : udp socket
* @scenario            : using for af_inet and ipv6 protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void
*/
static void tc_net_socket_af_inet_sock_dgram_ipv6_n(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IPV6);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_routing_p
* @brief               : udp socket
* @scenario            : using for af_inet address family and routing protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_dgram_routing_p(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ROUTING);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_fragment_p
* @brief               : udp socket
* @scenario            : using for fragment and af_inet address family
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_dgram_fragment_p(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_FRAGMENT);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_icmpv6_n
* @brief               : udp socket
* @scenario            : using for icmpv6 protocol and af_inet address family
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_dgram_icmpv6_n(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMPV6);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_tcp_n
* @brief               : udp socket
* @scenario            : using for tcp protocol and using af_unix address family
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_dgram_tcp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_TCP);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_udp_p
* @brief               : udp socket
* @scenario            : using for udp protocol and af_unix address family
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_dgram_udp_p(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_UDP);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_icmp_n
* @brief               : udp proctol 
* @scenario            : using for icmp
* @apicovered          : socket()
* @precondition        :
* @return              : void 
* @postcondition       :
*/
static void tc_net_socket_af_unix_sock_dgram_icmp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_ICMP);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_igmp_n
* @brief               : udp socket and af_unix address family
* @scenario            : using for igmp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_dgram_igmp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_IGMP);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_x25_sock_stream_n
* @brief               : tcp socket and af_x25 address family
* @scenario            : using for ip 
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_x25_sock_stream_n(void)
{
	int fd = socket(AF_X25, SOCK_STREAM, 0);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_ax25_sock_stream_n
* @brief               : tcp socket and af_ax25 address family
* @scenario            : using for ip
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void
*/
static void tc_net_socket_af_ax25_sock_stream_n(void)
{
	int fd = socket(AF_AX25, SOCK_STREAM, 0);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_packet_sock_stream_n
* @brief               : tcp socket
* @scenario            : using for ip
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_packet_sock_stream_n(void)
{
	int fd = socket(AF_PACKET, SOCK_STREAM, 0);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_p
* @brief               : tcp socket
* @scenario            : using for ip protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_stream_p(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, 0);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_tcp_p
* @brief               : tcp socket 
* @scenario            : using for tcp protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_stream_tcp_p(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_TCP);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_udp_n
* @brief               : tcp socket 
* @scenario            : using for udp protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_stream_udp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_UDP);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_icmp_n
* @brief               : tcp socket
* @scenario            : using for icmp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_stream_icmp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_ICMP);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_igmp_n
* @brief               : tcp socket 
* @scenario            : using for igmp protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void
*/
static void tc_net_socket_af_unix_sock_stream_igmp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_IGMP);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_ipv6_n
* @brief               : tcp socket
* @scenario            : using for ipv6
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_stream_ipv6_n(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_IPV6);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unixsock_stream_routing_n
* @brief               : tcp socket
* @scenario            : using for routing
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unixsock_stream_routing_n(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_ROUTING);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_icmpv6_n
* @brief               : tcp socket
* @scenario            :
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_stream_icmpv6_n(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_ICMPV6);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_netlink_sock_stream_n
* @brief               : tcp socket 
* @scenario            : 
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_netlink_sock_stream_n(void)
{
	int fd = socket(AF_NETLINK, SOCK_STREAM, 0);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unspec_sock_stream_p
* @brief               : tcp socket 
* @scenario            : using for ip
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unspec_sock_stream_p(void)
{
	int fd = socket(AF_UNSPEC, SOCK_STREAM, 0);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_tcp_p
* @brief               : tcp socket 
* @scenario            : using for tcp protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_tcp_p(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_protocol_n
* @brief               : tcp socket af_inet address family 
* @scenario            : inavlid condition for protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_protocol_n(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, INVAL_TYPE);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()

	close(fd);
}

/**
* @testcase            : tc_net_socket_domain_sock_stream_protocol_n
* @brief               : tcp socket
* @scenario            : invalid condition for address family and protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_domain_sock_stream_protocol_n(void)
{
	int fd = socket(INVAL_DOMAIN, SOCK_STREAM, INVAL_TYPE);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_type_protocol_n
* @brief               :  af_inet address family
* @scenario            : invalid condition for type and protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_type_protocol_n(void)
{
	int fd = socket(AF_INET, INVAL_PROTO, INVAL_TYPE);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_domain_type_protocol_n
* @brief               : 
* @scenario            : invalid for domain, type and protocol
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_domain_type_protocol_n(void)
{
	int fd = socket(INVAL_DOMAIN, INVAL_PROTO, INVAL_TYPE);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_domain_sock_stream_tcp_n
* @brief               : tcp socket
* @scenario            : using for tcp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_domain_sock_stream_tcp_n(void)
{
	int fd = socket(INVAL_DOMAIN, SOCK_STREAM, IPPROTO_TCP);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_udp_n
* @brief               : tcp socket
* @scenario            : using for udp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_udp_n(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()

	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_icmp_p
* @brief               : tcp socket
* @scenario            : using for icmp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_icmp_p(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_ICMP);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_igmp_p
* @brief               : tcp socket 
* @scenario            : using for igmp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_igmp_p(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IGMP);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_protoipv6_n
* @brief               : tcp socket
* @scenario            : using for ipv6
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_protoipv6_n(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IPV6);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_routing_p
* @brief               : tcp socket
* @scenario            : using for routing
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_routing_p(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_ROUTING);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_fragment_p
* @brief               : tcp socket 
* @scenario            : using for fragment
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_fragment_p(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_FRAGMENT);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_icmpv6_n
* @brief               : tcp socket 
* @scenario            : using for icmpv6 
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_icmpv6_n(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_ICMPV6);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet6_sock_stream_p
* @brief               : tcp socket 
* @scenario            : using for af_inet6 address family
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet6_sock_stream_p(void)
{
	int fd = socket(AF_INET6, SOCK_STREAM, ZERO);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_type_n
* @brief               : adress family for af_inet
* @scenario            : 
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_type_n(void)
{
	int fd = socket(AF_INET, INVAL_PROTO, ZERO);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_domain_n
* @brief               : tcp socket and ip protocol
* @scenario            : inavlid condition for address family
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_domain_n(void)
{
	int fd = socket(INVAL_DOMAIN, SOCK_STREAM, ZERO);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_domain_protocol_n
* @brief               : tcp socket 
* @scenario            : invalid address faimly
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_domain_protocol_n(void)
{
	int fd = socket(INVAL_DOMAIN, SOCK_STREAM, INVAL_TYPE);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_type_protocol_n
* @brief               : af_inet address family
* @scenario            : invalid stream
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_type_protocol_n(void)
{
	int fd = socket(AF_INET, INVAL_PROTO, INVAL_TYPE);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_p
* @brief               : tcp socket for af_inet family
* @scenario            : using for ip 
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_stream_p(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_invalid_domain_sock_stream_n
* @brief               : tcp socket
* @scenario            : invalid socket family
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_invalid_domain_sock_stream_n(void)
{
	int fd = socket(INVAL_DOMAIN, SOCK_STREAM, ZERO);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_ipv6_n
* @brief               : udp socket for af_unix 
* @scenario            : using for ipv6
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_dgram_ipv6_n(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_IPV6);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_routing_n
* @brief               : udp socket for af_unix
* @scenario            : using for routing
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_dgram_routing_n(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_ROUTING);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_fragment_p
* @brief               : udp socket for af_unix family
* @scenario            : using for fragment
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_dgram_fragment_p(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_FRAGMENT);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_icmpv6_n
* @brief               : udp_socket for af_unix family
* @scenario            : using for icmpv6
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_dgram_icmpv6_n(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_ICMPV6);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_netlink_sock_dgram_p
* @brief               : udp_socket for netlink socket
* @scenario            : using for ip
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_netlink_sock_dgram_p(void)
{
	int fd = socket(AF_NETLINK, SOCK_DGRAM, ZERO);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unspec_sock_dgram_p
* @brief               : udp_socket family AF_UNSPEC
* @scenario            : using for ip
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unspec_sock_dgram_p(void)
{
	int fd = socket(AF_UNSPEC, SOCK_DGRAM, 0);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_local_sock_dgram_p
* @brief               : udp_socket and local family
* @scenario            : using for ip
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_local_sock_dgram_p(void)
{
	int fd = socket(AF_LOCAL, SOCK_DGRAM, 0);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_x25_sock_dgram_n
* @brief               : udp_socket
* @scenario            : using for ip
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void
*/
static void tc_net_socket_af_x25_sock_dgram_n(void)
{
	int fd = socket(AF_X25, SOCK_DGRAM, 0);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_ax25_sock_dgram_n
* @brief               : udp_socket
* @scenario            : using for ip
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_ax25_sock_dgram_n(void)
{
	int fd = socket(AF_AX25, SOCK_DGRAM, 0);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_packet_sock_dgram_p
* @brief               : udp_socket
* @scenario            : using for ip
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_packet_sock_dgram_p(void)
{
	int fd = socket(AF_PACKET, SOCK_DGRAM, 0);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_p
* @brief               : raw_socket
* @scenario            : using for ip
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_raw_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, 0);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_tcp_p
* @brief               : raw_socket
* @scenario            : using for tcp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_raw_tcp_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_udp_p
* @brief               : raw_socket
* @scenario            : using for icmp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_raw_udp_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_icmp_p
* @brief               : raw_socket
* @scenario            : using for icmp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_raw_icmp_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_igmp_p
* @brief               : raw_socket
* @scenario            : using for igmp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_raw_igmp_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_IGMP);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_ipv6_n
* @brief               : raw_socket
* @scenario            : usinfg for ipv6
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_raw_ipv6_n(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_IPV6);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_routing_p
* @brief               : raw_socket
* @scenario            : using for routing
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_raw_routing_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ROUTING);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_fragment_p
* @brief               : raw_socket
* @scenario            : using for fragment
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_raw_fragment_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_FRAGMENT);

	TC_ASSERT_GEQ("socket", fd, ZERO)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_icmpv6_n
* @brief               : raw_socket
* @scenario            : using for icmpv6
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_inet_sock_raw_icmpv6_n(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMPV6);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_n
* @brief               : raw_socket
* @scenario            : using for ip
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_raw_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, ZERO);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_tcp_n
* @brief               : raw_socket
* @scenario            : using for tcp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_raw_tcp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_TCP);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_udp_n
* @brief               : raw_socket
* @scenario            : using for udp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_raw_udp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_UDP);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_icmp_n
* @brief               : raw_socket
* @scenario            : using for icmp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_raw_icmp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_ICMP);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_igmp_n
* @brief               : raw_socket 
* @scenario            : using for igmp
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_raw_igmp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_IGMP);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_ipv6_n
* @brief               : raw_socket 
* @scenario            : using for ipv6
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_raw_ipv6_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_IPV6);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_routing_n
* @brief               : raw_socket
* @scenario            : using for routing
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_raw_routing_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_ROUTING);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_fragment_n
* @brief               : raw_socket
* @scenario            : using for fragment
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_raw_fragment_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_FRAGMENT);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_icmpv6_n
* @brief               : raw socket
* @scenario            : using for icmpv6 protocl
* @apicovered          : socket()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_net_socket_af_unix_sock_raw_icmpv6_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_ICMPV6);

	TC_ASSERT_EQ("socket", fd, NEG_VAL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_get_socket_struct_p
* @brief               : Map a externally used socket index to the internal socket representation
* @scenario            : externally used socket index
* @apicovered          : socket(), get_socket_struct()
* @precondition        :
* @postcondition       :
* @return              : void 
*/
static void tc_get_socket_struct_p(void)
{
	struct socket *sock = NULL;
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("get_socket_struct", fd, ZERO)

	sock = get_socket_struct(fd);

	TC_ASSERT_NEQ("get_socket_struct", sock, NULL)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase            : tc_get_socket_struct_n
* @brief               : Map a externally used socket index to the internal socket representation
* @scenario            : externally used socket index
* @apicovered          : socket(), get_socket_struct()
* @precondition        :
* @postcondition       :
* @return              : void
*/
static void tc_get_socket_struct_n(void)
{
	int fd = NEG_VAL;
	struct socket *sock = NULL;

	sock = get_socket_struct(fd);

	TC_ASSERT_EQ("get_socket_struct", sock, NULL)
	TC_SUCCESS_RESULT()
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
	tc_get_socket_struct_p();
	tc_get_socket_struct_n();

	return 0;
}
