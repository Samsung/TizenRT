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

#define INVAL_DOMAIN        30
#define INVAL_PROTO         6
#define INVAL_TYPE          256

/**
* @testcase            : tc_net_socket_pf_inet_sock_raw_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the pf_inet family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_pf_inet_sock_raw_p(void)
{
	int fd = socket(PF_INET, SOCK_RAW, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_inet family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_dgram_p(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_pf_inet_sock_dgram_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the pf_inet family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_pf_inet_sock_dgram_p(void)
{
	int fd = socket(PF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_pf_inet_sock_stream_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the pf_inet family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_pf_inet_sock_stream_p(void)
{
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_tcp_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_inet family and Transmission Control Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_dgram_tcp_n(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_udp_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_inet family and User Datagram Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_dgram_udp_p(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_soc_dgram_icmp_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_inet family and Internet Control Message Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_soc_dgram_icmp_p(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_igmp_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_inet family and Internet Group Management Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_dgram_igmp_p(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IGMP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_ipv6_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_inet family and IPv6-in-IPv4 tunnelling.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_dgram_ipv6_n(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IPV6);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_routing_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_inet family and protocol argument value is IPv6 routing header.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_dgram_routing_p(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ROUTING);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_fragment_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_inet family and protocol argument value is IPv6 fragmentation header.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_dgram_fragment_p(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_FRAGMENT);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_dgram_icmpv6_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_inet family and protocol argument value is ICMPV6.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_dgram_icmpv6_n(void)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMPV6);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_tcp_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_unix family and tcp protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_dgram_tcp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_TCP);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_udp_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_unix family and udp protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_dgram_udp_p(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_UDP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_icmp_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_unix family and Internet Control Message Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_dgram_icmp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_ICMP);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_igmp_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_unix family and Internet Group Management Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_dgram_igmp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_IGMP);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_x25_sock_stream_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_x25 family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_x25_sock_stream_n(void)
{
	int fd = socket(AF_X25, SOCK_STREAM, 0);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_ax25_sock_stream_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_ax25 family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_ax25_sock_stream_n(void)
{
	int fd = socket(AF_AX25, SOCK_STREAM, 0);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_packet_sock_stream_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_packet family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_packet_sock_stream_n(void)
{
	int fd = socket(AF_PACKET, SOCK_STREAM, 0);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_unix family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_stream_p(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_tcp_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_unix family and Transmission Control Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_stream_tcp_p(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_udp_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_unix family and User Datagram Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_stream_udp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_UDP);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_icmp_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_unix family and Internet Control Message Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_stream_icmp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_ICMP);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_igmp_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_unix family and Internet Group Management Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_stream_igmp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_IGMP);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_ipv6_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_unix family and IPv6-in-IPv4 tunnelling.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_stream_ipv6_n(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_IPV6);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unixsock_stream_routing_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_unix family and protocol argument value is IPv6 routing header.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unixsock_stream_routing_n(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_ROUTING);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_stream_icmpv6_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_unix family and protocol argument value is ICMPV6.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_stream_icmpv6_n(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_ICMPV6);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_netlink_sock_stream_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_netlink family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_netlink_sock_stream_n(void)
{
	int fd = socket(AF_NETLINK, SOCK_STREAM, 0);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unspec_sock_stream_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_unspec family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unspec_sock_stream_p(void)
{
	int fd = socket(AF_UNSPEC, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_tcp_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_inet family and Transmission Control Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_tcp_p(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_protocol_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_inet family and protocol argument value is invalid.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_protocol_n(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, INVAL_TYPE);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_domain_sock_stream_protocol_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket with invalid value of domain and protocol argument.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_domain_sock_stream_protocol_n(void)
{
	int fd = socket(INVAL_DOMAIN, SOCK_STREAM, INVAL_TYPE);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_type_protocol_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the socket with af_inet family and invalid values for type and protocol argument.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_type_protocol_n(void)
{
	int fd = socket(AF_INET, INVAL_PROTO, INVAL_TYPE);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_domain_type_protocol_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the socket with invalid values of domain, type and protocol argument.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_domain_type_protocol_n(void)
{
	int fd = socket(INVAL_DOMAIN, INVAL_PROTO, INVAL_TYPE);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_domain_sock_stream_tcp_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for Transmission Control Protocol with invalid domain value.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_domain_sock_stream_tcp_n(void)
{
	int fd = socket(INVAL_DOMAIN, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_udp_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_inet family and User Datagram Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_udp_n(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_icmp_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_inet family and Internet Control Message Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_icmp_p(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_ICMP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_igmp_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_inet family and Internet Group Management Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_igmp_p(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IGMP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_protoipv6_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_inet family and IPv6-in-IPv4 tunnelling.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_protoipv6_n(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IPV6);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_routing_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_inet family and protocol argument value is IPv6 routing header.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_routing_p(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_ROUTING);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_fragment_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_inet family and protocol argument value is IPv6 fragmentation header.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_fragment_p(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_FRAGMENT);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_icmpv6_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_inet family and protocol argument value is ICMPV6.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_icmpv6_n(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_ICMPV6);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet6_sock_stream_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_inet6 family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet6_sock_stream_p(void)
{
	int fd = socket(AF_INET6, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_type_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_inet family with invalid type.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_type_n(void)
{
	int fd = socket(AF_INET, INVAL_PROTO, 0);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_domain_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket with invalid domain value.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_domain_n(void)
{
	int fd = socket(INVAL_DOMAIN, SOCK_STREAM, 0);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_domain_protocol_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket with invalid value of domain and protocol argument.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_domain_protocol_n(void)
{
	int fd = socket(INVAL_DOMAIN, SOCK_STREAM, INVAL_TYPE);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_type_protocol_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_inet family with invalid value of type and protocol argument.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_type_protocol_n(void)
{
	int fd = socket(AF_INET, INVAL_PROTO, INVAL_TYPE);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_stream_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket for the af_inet family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_stream_p(void)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_invalid_domain_sock_stream_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the tcp socket with invalid value of domain.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_invalid_domain_sock_stream_n(void)
{
	int fd = socket(INVAL_DOMAIN, SOCK_STREAM, 0);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_ipv6_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_unix family and IPv6-in-IPv4 tunnelling.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_dgram_ipv6_n(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_IPV6);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_routing_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_unix family and protocol argument value is IPv6 routing header.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_dgram_routing_n(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_ROUTING);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_fragment_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_unix family and protocol argument value is IPv6 fragmentation header.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_dgram_fragment_p(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_FRAGMENT);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_dgram_icmpv6_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_unix family and protocol argument value is ICMPV6.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_dgram_icmpv6_n(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_ICMPV6);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_netlink_sock_dgram_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_netlink family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_netlink_sock_dgram_p(void)
{
	int fd = socket(AF_NETLINK, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unspec_sock_dgram_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_unspec family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unspec_sock_dgram_p(void)
{
	int fd = socket(AF_UNSPEC, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_local_sock_dgram_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_loval family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_local_sock_dgram_p(void)
{
	int fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_x25_sock_dgram_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_x25 family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_x25_sock_dgram_n(void)
{
	int fd = socket(AF_X25, SOCK_DGRAM, 0);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_ax25_sock_dgram_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_ax25 family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_ax25_sock_dgram_n(void)
{
	int fd = socket(AF_AX25, SOCK_DGRAM, 0);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_packet_sock_dgram_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the udp socket for the af_packet family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_packet_sock_dgram_p(void)
{
	int fd = socket(AF_PACKET, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_inet family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_raw_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_tcp_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_inet family and Transmission Control Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_raw_tcp_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_udp_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_inet family and User Datagram Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_raw_udp_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_icmp_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_inet family and Internet Control Message Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_raw_icmp_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_igmp_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_inet family and Internet Group Management Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_raw_igmp_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_IGMP);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_ipv6_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_inet family and IPv6-in-IPv4 tunnelling.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_raw_ipv6_n(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_IPV6);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_routing_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_inet family and protocol argument value is IPv6 routing header.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_raw_routing_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ROUTING);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_fragment_p
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_inet family and protocol argument value is IPv6 fragmentation header.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_raw_fragment_p(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_FRAGMENT);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_inet_sock_raw_icmpv6_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_inet family and protocol argument value is ICMPV6.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_inet_sock_raw_icmpv6_n(void)
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMPV6);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_unix family.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_raw_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, 0);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_tcp_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_unix family and Transmission Control Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_raw_tcp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_TCP);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_udp_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_unix family and User Datagram Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_raw_udp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_UDP);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_icmp_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_unix family and Internet Control Message Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_raw_icmp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_ICMP);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_igmp_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_unix family and Internet Group Management Protocol.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_raw_igmp_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_IGMP);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_ipv6_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_unix family and IPv6-in-IPv4 tunnelling.
                         on error, -1 is returned.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_raw_ipv6_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_IPV6);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_routing_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_unix family and protocol argument value is IPv6 routing header.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_raw_routing_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_ROUTING);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_fragment_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_unix family and protocol argument value is IPv6 fragmentation header.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_raw_fragment_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_FRAGMENT);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_socket_af_unix_sock_raw_icmpv6_n
* @brief               : this socket API creates an endpoint for communication and returns a descriptor.
* @scenario            : create the raw socket for the af_unix family and protocol argument value is ICMPV6.
* @apicovered          : socket()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_socket_af_unix_sock_raw_icmpv6_n(void)
{
	int fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_ICMPV6);
	TC_ASSERT_EQ("socket", fd, NEG_VAL);
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_get_socket_struct_p
* @brief               : map a externally used socket index to the internal socket representation.
* @scenario            : map a externally used socket index to the internal socket representation.
* @apicovered          : socket(), get_socket_struct()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_get_socket_struct_p(void)
{
	struct socket *sock = NULL;

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", fd, ZERO);

	sock = get_socket_struct(fd);

	close(fd);
	TC_ASSERT_NEQ("get_socket_struct", sock, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_get_socket_struct_n
* @brief               : map a externally used socket index to the internal socket representation
* @scenario            : map a externally used socket index to the internal socket representation with invalid socket fd.
* @apicovered          : socket(), get_socket_struct()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_get_socket_struct_n(void)
{
	struct socket *sock = NULL;

	sock = get_socket_struct(NEG_VAL);

	TC_ASSERT_EQ("get_socket_struct", sock, NULL);
	TC_SUCCESS_RESULT();
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
