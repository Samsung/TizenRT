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

/**
* @testcase		tc_net_socket_pf_inet_sock_raw_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_pf_inet_sock_raw_n(void)
{
	int fd = -1;
	fd = socket(PF_INET, SOCK_RAW, 0);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_inet_sock_dgram_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_dgram_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}

/**
* @testcase		tc_net_socket_pf_inet_sock_dgram_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_pf_inet_sock_dgram_p(void)
{
	int fd = -1;
	fd = socket(PF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}

/**
* @testcase		tc_net_socket_pf_inet_sock_stream_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_pf_inet_sock_stream_p(void)
{

	int fd = -1;
	fd = socket(PF_INET, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}

/**
* @testcase		tc_net_socket_af_inet_sock_dgram_tcp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_dgram_tcp_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);

	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_inet_sock_dgram_udp_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_dgram_udp_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}

/**
* @testcase		tc_net_socket_af_inet_soc_dgram_icmp_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_dgram_icmp_n(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

#ifdef CONFIG_NET_LWIP_IGMP
/**
* @testcase		tc_net_socket_af_inet_sock_dgram_igmp_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_dgram_igmp_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IGMP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif

#ifdef CONFIG_NET_IPv6
/**
* @testcase		tc_net_socket_af_inet_sock_dgram_ipv6_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_dgram_ipv6_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IPV6);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_inet_sock_dgram_icmpv6_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_dgram_icmpv6_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMPV6);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif

#ifdef AF_UNIX
/**
* @testcase		tc_net_socket_af_unix_sock_dgram_tcp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_dgram_tcp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_TCP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_unix_sock_dgram_udp_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_dgram_udp_p(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_UDP);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}

/**
* @testcase		tc_net_socket_af_unix_sock_dgram_icmp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_dgram_icmp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_ICMP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

#ifdef CONFIG_NET_LWIP_IGMP
/**
* @testcase		tc_net_socket_af_unix_sock_dgram_igmp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_dgram_igmp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_IGMP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif
#endif

#ifdef AF_X25
/**
* @testcase		tc_net_socket_af_x25_sock_stream_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_x25_sock_stream_n(void)
{
	int fd = -1;
	fd = socket(AF_X25, SOCK_STREAM, 0);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif

#ifdef AF_AX25
/**
* @testcase		tc_net_socket_af_ax25_sock_stream_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_ax25_sock_stream_n(void)
{
	int fd = -1;
	fd = socket(AF_AX25, SOCK_STREAM, 0);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif

#ifdef AF_PACKET
/**
* @testcase		tc_net_socket_af_packet_sock_stream_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_packet_sock_stream_n(void)
{
	int fd = -1;
	fd = socket(AF_PACKET, SOCK_STREAM, 0);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif

#ifdef AF_UNIX
/**
* @testcase		tc_net_socket_af_unix_sock_stream_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_stream_p(void)
{
	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}

/**
* @testcase		tc_net_socket_af_unix_sock_stream_tcp_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_stream_tcp_p(void)
{
	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()
	close(fd);
}

/**
* @testcase		tc_net_socket_af_unix_sock_stream_udp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_stream_udp_n(void)
{
	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_UDP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_unix_sock_stream_icmp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_stream_icmp_n(void)
{
	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_ICMP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

#ifdef CONFIG_NET_LWIP_IGMP
/**
* @testcase		tc_net_socket_af_unix_sock_stream_igmp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_stream_igmp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_IGMP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif
#ifdef CONFIG_NET_IPv6
/**
* @testcase		tc_net_socket_af_unix_sock_stream_ipv6_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_stream_ipv6_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_IPV6);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_unix_sock_stream_icmpv6_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_stream_icmpv6_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_ICMPV6);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif
#endif

#ifdef AF_NETLINK
/**
* @testcase		tc_net_socket_af_netlink_sock_stream_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_netlink_sock_stream_n(void)
{
	int fd = -1;
	fd = socket(AF_NETLINK, SOCK_STREAM, 0);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif

/**
* @testcase		tc_net_socket_af_unspec_sock_stream_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unspec_sock_stream_p(void)
{
	int fd = -1;
	fd = socket(AF_UNSPEC, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}

/**
* @testcase		tc_net_socket_af_inet_sock_stream_tcp_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_stream_tcp_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}

/**
* @testcase		tc_net_socket_af_inet_sock_stream_protocol_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_stream_protocol_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 256);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_domain_sock_stream_protocol_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_domain_sock_stream_protocol_n(void)
{
	int fd = -1;
	fd = socket(30, SOCK_STREAM, 256);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_inet_type_protocol_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_type_protocol_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, 7, 256);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_domain_type_protocol_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_domain_type_protocol_n(void)
{
	int fd = -1;
	fd = socket(13, 6, 256);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_domain_sock_stream_tcp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_domain_sock_stream_tcp_n(void)
{
	int fd = -1;
	fd = socket(40, SOCK_STREAM, IPPROTO_TCP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_inet_sock_stream_udp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_stream_udp_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_inet_sock_stream_icmp_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_stream_icmp_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_ICMP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

#ifdef CONFIG_NET_LWIP_IGMP
/**
* @testcase		tc_net_socket_af_inet_sock_stream_igmp_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_stream_igmp_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IGMP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

#endif

#ifdef CONFIG_NET_IPv6
/**
* @testcase		tc_net_socket_af_inet_sock_stream_protoipv6_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_stream_protoipv6_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IPV6);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_inet_sock_stream_icmpv6_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_stream_icmpv6_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_ICMPV6);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif
/**
* @testcase		tc_net_socket_af_inet6_sock_stream_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet6_sock_stream_p(void)
{
	int fd = -1;
	fd = socket(AF_INET6, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}

/**
* @testcase		tc_net_socket_af_inet_sock_stream_type_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_stream_type_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, 5, 0);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_inet_sock_stream_domain_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_stream_domain_n(void)
{
	int fd = -1;
	fd = socket(12, SOCK_STREAM, 0);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_inet_sock_stream_domain_protocol_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_stream_domain_protocol_n(void)
{
	int fd = -1;
	fd = socket(15, SOCK_STREAM, 289);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_inet_sock_stream_type_protocol_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_stream_type_protocol_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, 7, 300);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_inet_sock_stream_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_stream_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}

#ifdef CONFIG_NET_IPv6
/**
* @testcase		tc_net_socket_invalid_domain_sock_stream_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_invalid_domain_sock_stream_n(void)
{
	int fd = -1;
	fd = socket(-1, SOCK_STREAM, 0);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif

#ifdef AF_UNIX
#ifdef CONFIG_NET_IPv6
/**
* @testcase		tc_net_socket_af_unix_sock_dgram_ipv6_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_dgram_ipv6_n(void)
{
	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_IPV6);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
/**
* @testcase		tc_net_socket_af_unix_sock_dgram_icmpv6_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_dgram_icmpv6_n(void)
{
	int fd = -1;
	fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_ICMPV6);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif
#endif

#ifdef AF_NETLINK
/**
* @testcase		tc_net_socket_af_netlink_sock_dgram_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_netlink_sock_dgram_p(void)
{
	int fd = -1;
	fd = socket(AF_NETLINK, SOCK_DGRAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}
#endif
/**
* @testcase		tc_net_socket_af_unspec_sock_dgram_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unspec_sock_dgram_p(void)
{
	int fd = -1;
	fd = socket(AF_UNSPEC, SOCK_DGRAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}

#ifdef AF_LOCAL
/**
* @testcase		tc_net_socket_af_local_sock_dgram_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_local_sock_dgram_p(void)
{
	int fd = -1;
	fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}
#endif

#ifdef AF_X25
/**
* @testcase		tc_net_socket_af_x25_sock_dgram_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_x25_sock_dgram_n(void)
{
	int fd = -1;
	fd = socket(AF_X25, SOCK_DGRAM, 0);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif
#ifdef AF_AX25
/**
* @testcase		tc_net_socket_af_ax25_sock_dgram_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_ax25_sock_dgram_n(void)
{
	int fd = -1;
	fd = socket(AF_AX25, SOCK_DGRAM, 0);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif

#ifdef AF_PACKET
/**
* @testcase		tc_net_socket_af_packet_sock_dgram_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_packet_sock_dgram_p(void)
{
	int fd = -1;
	fd = socket(AF_PACKET, SOCK_DGRAM, 0);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}
#endif

/**
* @testcase		tc_net_socket_af_inet_sock_raw_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_raw_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, 0);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_inet_sock_raw_tcp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_raw_tcp_n(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_inet_sock_raw_udp_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_raw_udp_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}

/**
* @testcase		tc_net_socket_af_inet_sock_raw_icmp_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_raw_icmp_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()

	close(fd);
}

#ifdef CONFIG_NET_LWIP_IGMP
/**
* @testcase		tc_net_socket_af_inet_sock_raw_igmp_p
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_raw_igmp_p(void)
{
	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_IGMP);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()
	close(fd);
}
#endif

#ifdef CONFIG_NET_IPv6
/**
* @testcase		tc_net_socket_af_inet_sock_raw_ipv6_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_raw_ipv6_n(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_IPV6);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
/**
* @testcase		tc_net_socket_af_inet_sock_raw_icmpv6_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_inet_sock_raw_icmpv6_p(void)
{

	int fd = -1;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMPV6);
	TC_ASSERT_GEQ("socket", fd, 0)
	TC_SUCCESS_RESULT()
	close(fd);
}
#endif

#ifdef AF_UNIX
/**
* @testcase		tc_net_socket_af_unix_sock_raw_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_raw_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, 0);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_unix_sock_raw_tcp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_raw_tcp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_TCP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_unix_sock_raw_udp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_raw_udp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_UDP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_unix_sock_raw_icmp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_raw_icmp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_ICMP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

#ifdef CONFIG_NET_LWIP_IGMP
/**
* @testcase		tc_net_socket_af_unix_sock_raw_igmp_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_raw_igmp_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_IGMP);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif

#ifdef CONFIG_NET_IPv6
/**
* @testcase		tc_net_socket_af_unix_sock_raw_ipv6_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_raw_ipv6_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_IPV6);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}

/**
* @testcase		tc_net_socket_af_unix_sock_raw_icmpv6_n
* @brief
* @scenario
* @apicovered		socket()
* @precondition
* @postcondition
*/
static void tc_net_socket_af_unix_sock_raw_icmpv6_n(void)
{

	int fd = -1;
	fd = socket(AF_UNIX, SOCK_RAW, IPPROTO_ICMPV6);
	TC_ASSERT_EQ_CLEANUP("socket", fd, -1, close(fd))
	TC_SUCCESS_RESULT()
}
#endif
#endif
/****************************************************************************
 * Name: socket()
 ****************************************************************************/

int net_socket_main(void)
{
	tc_net_socket_af_inet_sock_stream_p();
	tc_net_socket_pf_inet_sock_stream_p();
	tc_net_socket_af_inet_sock_dgram_p();
	tc_net_socket_pf_inet_sock_dgram_p();
	tc_net_socket_pf_inet_sock_raw_n();
	tc_net_socket_af_inet_sock_dgram_tcp_n();
	tc_net_socket_af_inet_sock_dgram_udp_p();
	tc_net_socket_af_inet_sock_dgram_icmp_n();
#ifdef CONFIG_NET_LWIP_IGMP
	tc_net_socket_af_inet_sock_dgram_igmp_n();
#endif
#ifdef CONFIG_NET_IPv6
	tc_net_socket_af_inet_sock_dgram_ipv6_n();
	tc_net_socket_invalid_domain_sock_stream_n();
	tc_net_socket_af_inet_sock_dgram_icmpv6_n();
#endif
	tc_net_socket_af_unspec_sock_stream_p();
	tc_net_socket_af_inet_sock_stream_protocol_n();
	tc_net_socket_domain_sock_stream_protocol_n();
	tc_net_socket_af_inet_sock_stream_tcp_p();
	tc_net_socket_af_inet_type_protocol_n();
	tc_net_socket_domain_type_protocol_n();
	tc_net_socket_domain_sock_stream_tcp_n();
	tc_net_socket_af_inet_sock_stream_udp_n();
	tc_net_socket_af_inet_sock_stream_icmp_n();
#ifdef CONFIG_NET_LWIP_IGMP
	tc_net_socket_af_inet_sock_stream_igmp_n();
#endif
#ifdef CONFIG_NET_IPv6
	tc_net_socket_af_inet_sock_stream_protoipv6_n();
	tc_net_socket_af_inet_sock_stream_icmpv6_n();
#endif
	tc_net_socket_af_inet6_sock_stream_p();
	tc_net_socket_af_inet_sock_stream_type_n();
	tc_net_socket_af_inet_sock_stream_domain_n();
	tc_net_socket_af_inet_sock_stream_domain_protocol_n();
	tc_net_socket_af_inet_sock_stream_type_protocol_n();
	tc_net_socket_af_unspec_sock_dgram_p();
	tc_net_socket_af_inet_sock_raw_n();
	tc_net_socket_af_inet_sock_raw_tcp_n();
	tc_net_socket_af_inet_sock_raw_udp_p();
	tc_net_socket_af_inet_sock_raw_icmp_p();
#ifdef CONFIG_NET_LWIP_IGMP
	tc_net_socket_af_inet_sock_raw_igmp_p();
#endif
#ifdef CONFIG_NET_IPv6
	tc_net_socket_af_inet_sock_raw_ipv6_n();
	tc_net_socket_af_inet_sock_raw_icmpv6_p();
#endif
	return 0;
}
