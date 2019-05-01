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
/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
/**
 * @file
 *
 * lwIP Options Configuration
 */

#ifndef __LWIP_LWIPOPTS_H__
#define __LWIP_LWIPOPTS_H__

#include <tinyara/config.h>

/* --------- PreDefined Configurations -------------*/
/*
 * Note
 * In this platform, DHCP client and server are supported through netutils
 * Instead of using lwIP DHCP, please use netutils dhcpc and dhcpd
 */
#ifdef CONFIG_LWIP_DHCPS
#define LWIP_DHCPS                      CONFIG_LWIP_DHCPS
#else
#define LWIP_DHCPS 0
#endif /* CONFIG_LWIP_DHCPS */

#ifdef CONFIG_LWIP_DHCPC
#define LWIP_DHCP                       CONFIG_LWIP_DHCPC
#else
#define LWIP_DHCP 0
#endif /* CONFIG_LWIP_DHCP */

#define LWIP_NETIF_API                  1

#ifdef CONFIG_NET_ENABLE_LOOPBACK
#define ENABLE_LOOPBACK    CONFIG_NET_ENABLE_LOOPBACK
#endif

#ifdef CONFIG_NET_LWIP_ETHERNET
#define LWIP_ETHERNET    CONFIG_NET_LWIP_ETHERNET
#endif
/* --------- PreDefined Configurations -------------*/

/* ---------- IPv options ---------- */

#ifdef CONFIG_NET_IPv4
#define LWIP_IPV4 CONFIG_NET_IPv4
#else
#define LWIP_IPV4 0
#endif

#ifdef CONFIG_NET_IPv6
#define LWIP_IPV6 CONFIG_NET_IPv6
#else
#define LWIP_IPV6 0
#endif

/* ---------- ARP options ---------- */
#ifdef CONFIG_NET_ARP
#define LWIP_ARP                        CONFIG_NET_ARP
#endif

#if defined(CONFIG_NET_ARP)
#if !defined(CONFIG_NET_ARP_TABLESIZE)
#error "Please define LWIP ARP Table size"
#else
#define ARP_TABLE_SIZE  CONFIG_NET_ARP_TABLESIZE
#endif
#endif

#ifdef CONFIG_NET_ARP_QUEUEING
#define ARP_QUEUEING                    CONFIG_NET_ARP_QUEUEING
#endif

#ifdef CONFIG_NET_ETHARP_TRUST_IP_MAC
#define ETHARP_TRUST_IP_MAC             CONFIG_NET_ETHARP_TRUST_IP_MAC
#endif

#ifdef CONFIG_NET_ETH_PAD_SIZE
#define ETH_PAD_SIZE                    CONFIG_NET_ETH_PAD_SIZE
#endif

#ifdef CONFIG_NET_ARP_STATIC_ENTRIES
#define ETHARP_SUPPORT_STATIC_ENTRIES   CONFIG_NET_ARP_STATIC_ENTRIES
#endif

/* ---------- ARP options ---------- */


/* ---------- ND6 options ---------- */
#ifdef CONFIG_NET_IPv6_ND_QUEUEING
#define LWIP_ND6_QUEUEING	CONFIG_NET_IPv6_ND_QUEUEING
#else
#define LWIP_ND6_QUEUEING	0
#endif


#ifdef CONFIG_NET_IPv6_ND_QUEUE
#define MEMP_NUM_ND6_QUEUE	CONFIG_NET_IPv6_ND_QUEUE
#endif

#ifdef CONFIG_NET_IPv6_ND_NUM_NEIGHBORS
#define LWIP_ND6_NUM_NEIGHBORS	CONFIG_NET_IPv6_ND_NUM_NEIGHBORS
#endif

#ifdef CONFIG_NET_IPv6_ND_NUM_DESTINATIONS
#define LWIP_ND6_NUM_DESTINATIONS	CONFIG_NET_IPv6_ND_NUM_DESTINATIONS
#endif

#ifdef CONFIG_NET_IPv6_ND_NUM_PREFIXES
#define LWIP_ND6_NUM_PREFIXES		CONFIG_NET_IPv6_ND_NUM_PREFIXES
#endif

#ifdef CONFIG_NET_IPv6_ND_NUM_ROUTERS
#define LWIP_ND6_NUM_ROUTERS		CONFIG_NET_IPv6_ND_NUM_ROUTERS
#endif


#ifdef CONFIG_NET_IPv6_ND_MAX_MULTICAST_SOLICIT
#define LWIP_ND6_MAX_MULTICAST_SOLICIT	CONFIG_NET_IPv6_ND_MAX_MULTICAST_SOLICIT
#endif

#ifdef CONFIG_NET_IPv6_ND_MAX_UNICAST_SOLICIT
#define LWIP_ND6_MAX_UNICAST_SOLICIT	CONFIG_NET_IPv6_ND_MAX_UNICAST_SOLICIT
#endif

#ifdef CONFIG_NET_IPv6_ND_MAX_SOLICIT_INTERVAL
#define LWIP_ND6_MAX_SOLICIT_INTERVAL CONFIG_NET_IPv6_ND_MAX_SOLICIT_INTERVAL
#endif

#ifdef CONFIG_NET_IPv6_ND_REACHABLE_TIME
#define LWIP_ND6_MAX_ANYCAST_DELAY_TIME	CONFIG_NET_IPv6_ND_REACHABLE_TIME
#endif

#ifdef CONFIG_NET_IPv6_ND_RETRANS_TIMER
#define LWIP_ND6_RETRANS_TIMER		CONFIG_NET_IPv6_ND_RETRANS_TIMER
#endif


#ifdef CONFIG_NET_IPv6_ND_DELAY_FIRST_PROBE_TIME
#define LWIP_ND6_DELAY_FIRST_PROBE_TIME	CONFIG_NET_IPv6_ND_DELAY_FIRST_PROBE_TIME
#endif

#ifdef CONFIG_NET_IPv6_ND_ALLOW_RA_UPDATES
#define LWIP_ND6_ALLOW_RA_UPDATES	CONFIG_NET_IPv6_ND_ALLOW_RA_UPDATES
#else
#define LWIP_ND6_ALLOW_RA_UPDATES	0
#endif

#ifdef CONFIG_NET_IPv6_ND_TCP_REACHABILITY_HINTS
#define LWIP_ND6_TCP_REACHABILITY_HINTS	CONFIG_NET_IPv6_ND_TCP_REACHABILITY_HINTS
#else
#define LWIP_ND6_TCP_REACHABILITY_HINTS	0
#endif

#ifdef CONFIG_NET_IPv6_ND_RDNSS_MAX_DNS_SERVERS
#define LWIP_ND6_RDNSS_MAX_DNS_SERVERS	CONFIG_NET_IPv6_ND_RDNSS_MAX_DNS_SERVERS
#endif

/* ---------- ND6 options ---------- */


/* ---------- DHCP6 options ---------- */

#ifdef CONFIG_NET_IPv6_DHCP
#define LWIP_IPV6_DHCP6			CONFIG_NET_IPv6_DHCP
#else
#define LWIP_IPV6_DHCP6			0
#endif

/* ---------- VLAN options ---------- */
#ifdef CONFIG_NET_LWIP_VLAN
#define ETHARP_SUPPORT_VLAN             1
#ifdef NET_LWIP_VLAN_CHECK
#define ETHARP_VLAN_CHECK               CONFIG_NET_LWIP_VLAN_CHECK_ID
#endif
#else
#define ETHARP_SUPPORT_VLAN             0
#endif
/* ---------- VLAN options ---------- */

/* ---------- IP options ---------- */

#ifdef CONFIG_NET_IP_FORWARD
#define IP_FORWARD                      1
#else
#define IP_FORWARD                      0
#endif

#ifdef CONFIG_NET_IP_OPTIONS_ALLOWED
#define IP_OPTIONS_ALLOWED              1
#else
#define IP_OPTIONS_ALLOWED              0
#endif

#ifdef CONFIG_NET_IP_FRAG
#define IP_FRAG	                       1
#else
#define IP_FRAG	                       0
#endif

#ifdef CONFIG_NET_IP_REASSEMBLY
#define IP_REASSEMBLY                  1
#else
#define IP_REASSEMBLY                  0
#endif

/* if undefined, the value set in the lwip/opts.h */
#ifdef CONFIG_NET_IPV4_REASS_MAX_PBUFS
#define IP_REASS_MAX_PBUFS	           CONFIG_NET_IPV4_REASS_MAX_PBUFS
#endif

/* if undefined, the value set in the lwip/opts.h */
#ifdef CONFIG_NET_IPV4_REASS_MAXAGE
#define IP_REASS_MAXAGE	               CONFIG_NET_IPV4_REASS_MAXAGE
#endif

/* if undefined, the value set in the lwip/opts.h */
#ifdef CONFIG_NET_IP_DEFAULT_TTL
#define IP_DEFAULT_TTL                 CONFIG_NET_IP_DEFAULT_TTL
#endif

/* ---------- IP options ---------- */

/* ---------- IPv6 options ---------- */
#ifdef CONFIG_NET_IPv6
#define LWIP_IPV6			CONFIG_NET_IPv6
#else
#define LWIP_IPV6			0
#endif

#ifdef CONFIG_NET_IPv6_NUM_ADDRESSES
#define LWIP_IPV6_NUM_ADDRESSES		CONFIG_NET_IPv6_NUM_ADDRESSES
#endif

#ifdef CONFIG_NET_IPv6_FORWARD
#define LWIP_IPV6_FORWARD		CONFIG_NET_IPv6_FORWARD
#else
#define LWIP_IPV6_FORWARD		0
#endif

#ifdef CONFIG_NET_IPv6_FRAG
#define LWIP_IPV6_FRAG			CONFIG_NET_IPv6_FRAG
#else
#define LWIP_IPV6_FRAG			0
#endif

#ifdef CONFIG_NET_IPv6_REASS
#define LWIP_IPV6_REASS			CONFIG_NET_IPv6_REASS
#else
#define LWIP_IPV6_REASS			0
#endif

#ifdef CONFIG_NET_IPV6_REASS_MAXAGE
#define LWIP_IPV6_REASS_MAXAGE	               CONFIG_NET_IPV6_REASS_MAXAGE
#endif

#ifdef CONFIG_NET_IPv6_SEND_ROUTER_SOLICIT
#define LWIP_IPV6_SEND_ROUTER_SOLICIT	CONFIG_NET_IPv6_SEND_ROUTER_SOLICIT
#else
#define LWIP_IPV6_SEND_ROUTER_SOLICIT	0
#endif

#ifdef CONFIG_NET_IPv6_AUTOCONFIG
#define LWIP_IPV6_AUTOCONFIG		CONFIG_NET_IPv6_AUTOCONFIG
#else
#define LWIP_IPV6_AUTOCONFIG		0
#endif

#ifdef CONFIG_NET_IPv6_DUP_DETECT_ATTEMPTS
#define LWIP_IPV6_DUP_DETECT_ATTEMPTS	CONFIG_NET_IPv6_DUP_DETECT_ATTEMPTS
#endif

#ifdef CONFIG_NET_IPv6_PMTU_FOR_MULTICAST
#define LWIP_IPV6_PMTU_FOR_MULTICAST		CONFIG_NET_IPv6_PMTU_FOR_MULTICAST
#else
#define LWIP_IPV6_PMTU_FOR_MULTICAST		0
#endif

/* ---------- IPv6 options ---------- */

/* ---------- ICMP options ---------- */
#ifdef CONFIG_NET_ICMP
#define LWIP_ICMP                       CONFIG_NET_ICMP
#endif

#ifdef CONFIG_NET_BROADCAST_PING
#define LWIP_BROADCAST_PING             CONFIG_NET_BROADCAST_PING
#endif

#ifdef CONFIG_NET_MULTICAST_PING4
#define LWIP_MULTICAST_PING4		CONFIG_NET_MULTICAST_PING4
#endif

#ifdef CONFIG_NET_MULTICAST_PING6
#define LWIP_MULTICAST_PING6		CONFIG_NET_MULTICAST_PING6
#endif
/* ---------- ICMP options ---------- */

/* ---------- IGMP options ---------- */
#ifdef CONFIG_NET_LWIP_IGMP
#define LWIP_IGMP                       CONFIG_NET_LWIP_IGMP
#endif
// pknet
#ifdef CONFIG_NET_LWIP_MEMP_NUM_IGMP_GROUP
#define MEMP_NUM_IGMP_GROUP             CONFIG_NET_LWIP_MEMP_NUM_IGMP_GROUP
#endif

/* ---------- IGMP options ---------- */

/* ---------- TCP options ---------- */
#ifdef CONFIG_NET_TCP
#define LWIP_TCP	CONFIG_NET_TCP
#define LWIP_EVENT_API 0
#define LWIP_CALLBACK_API 1
#endif

#ifdef CONFIG_NET_TCP_TTL
#define TCP_TTL	CONFIG_NET_TCP_TTL
#endif

#ifdef CONFIG_NET_TCP_WND
#define TCP_WND	CONFIG_NET_TCP_WND
#endif

#ifdef CONFIG_NET_TCP_MAXRTX
#define TCP_MAXRTX	CONFIG_NET_TCP_MAXRTX
#endif

#ifdef CONFIG_NET_TCP_SYNMAXRTX
#define TCP_SYNMAXRTX	CONFIG_NET_TCP_SYNMAXRTX
#endif

#ifdef CONFIG_NET_TCP_QUEUE_OOSEQ
#define TCP_QUEUE_OOSEQ	CONFIG_NET_TCP_QUEUE_OOSEQ
#endif

#ifdef CONFIG_NET_TCP_MSS
#define TCP_MSS	CONFIG_NET_TCP_MSS
#endif

#ifdef CONFIG_NET_TCP_CALCULATE_EFF_SEND_MSS
#define TCP_CALCULATE_EFF_SEND_MSS	CONFIG_NET_TCP_CALCULATE_EFF_SEND_MSS
#endif

#ifdef CONFIG_NET_TCP_SND_BUF
#define TCP_SND_BUF	CONFIG_NET_TCP_SND_BUF
#endif

#ifdef CONFIG_NET_TCP_SND_QUEUELEN
#define TCP_SND_QUEUELEN	CONFIG_NET_TCP_SND_QUEUELEN
#endif

#ifdef CONFIG_NET_TCP_OOSEQ_MAX_BYTES
#define TCP_OOSEQ_MAX_BYTES	CONFIG_NET_TCP_OOSEQ_MAX_BYTES
#endif

#ifdef CONFIG_NET_TCP_OOSEQ_MAX_PBUFS
#define TCP_OOSEQ_MAX_PBUFS	CONFIG_NET_TCP_OOSEQ_MAX_PBUFS
#endif

#ifdef CONFIG_NET_TCP_LISTEN_BACKLOG
#define TCP_LISTEN_BACKLOG	CONFIG_NET_TCP_LISTEN_BACKLOG
#endif

#ifdef CONFIG_NET_TCP_DEFAULT_LISTEN_BACKLOG
#define TCP_DEFAULT_LISTEN_BACKLOG	CONFIG_NET_TCP_DEFAULT_LISTEN_BACKLOG
#endif

#ifdef CONFIG_NET_TCP_OVERSIZE
#define TCP_OVERSIZE	CONFIG_NET_TCP_OVERSIZE
#endif

#ifdef CONFIG_NET_TCP_TIMESTAMPS
#define TCP_TIMESTAMPS	CONFIG_NET_TCP_TIMESTAMPS
#endif

#ifdef CONFIG_NET_TCP_KEEPALIVE
#define LWIP_TCP_KEEPALIVE              CONFIG_NET_TCP_KEEPALIVE
#endif

#ifdef CONFIG_NET_TCP_WND_UPDATE_THRESHOLD
#define TCP_WND_UPDATE_THRESHOLD	CONFIG_NET_TCP_WND_UPDATE_THRESHOLD
#endif

/* ---------- TCP options ---------- */

/* ---------- UDP options ---------- */

#ifdef CONFIG_NET_UDP
#define LWIP_UDP	CONFIG_NET_UDP
#define LWIP_UDP_TODO 1
#endif

#ifdef CONFIG_NET_UDP_TTL
#define UDP_TTL	CONFIG_NET_UDP_TTL
#endif

#ifdef CONFIG_NET_UDPLITE
#define LWIP_UDPLITE	CONFIG_NET_UDPLITE
#endif

#ifdef CONFIG_NET_NETBUF_RECVINFO
#define LWIP_NETBUF_RECVINFO	CONFIG_NET_NETBUF_RECVINFO
#endif

/* ---------- UDP options ---------- */

/* ---------- SNMP options ---------- */
#ifdef CONFIG_NET_LWIP_SNMP
#define LWIP_SNMP                       1
#else
#define LWIP_SNMP                       0
#endif

#ifdef CONFIG_NET_LWIP_SNMP_CONCURRENT_REQUESTS
#define SNMP_CONCURRENT_REQUESTS        CONFIG_NET_LWIP_SNMP_CONCURRENT_REQUESTS
#endif

#ifdef CONFIG_NET_LWIP_SNMP_TRAP_DESTINATIONS
#define SNMP_TRAP_DESTINATIONS          CONFIG_NET_LWIP_SNMP_TRAP_DESTINATIONS
#endif

#ifdef CONFIG_NET_LWIP_SNMP_PRIVATE_MIB
#define SNMP_PRIVATE_MIB                1
#else
#define SNMP_PRIVATE_MIB                0
#endif

#ifdef CONFIG_NET_LWIP_SNMP_MAX_TREE_DEPTH
#define SNMP_MAX_TREE_DEPTH             CONFIG_NET_LWIP_SNMP_MAX_TREE_DEPTH
#endif
/* ---------- SNMP options ---------- */

/* ---------- Memory options ---------- */

#ifdef CONFIG_NET_MEM_ALIGNMENT
#define MEM_ALIGNMENT	CONFIG_NET_MEM_ALIGNMENT
#endif

#ifdef CONFIG_NET_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT
#define LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT	CONFIG_NET_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT
#endif

#ifdef CONFIG_NET_MEM_LIBC_MALLOC
#define MEM_LIBC_MALLOC	CONFIG_NET_MEM_LIBC_MALLOC
#endif

#ifdef CONFIG_NET_MEM_USE_POOLS
#define MEM_USE_POOLS	CONFIG_NET_MEM_USE_POOLS
#endif

#ifdef CONFIG_NET_MEM_SIZE
#define MEM_SIZE	CONFIG_NET_MEM_SIZE
#endif

#ifdef CONFIG_NET_MEMP_MEM_MALLOC
#define MEMP_MEM_MALLOC	CONFIG_NET_MEMP_MEM_MALLOC
#endif

#ifdef CONFIG_NET_MEMP_OVERFLOW_CHECK
#define MEMP_OVERFLOW_CHECK	CONFIG_NET_MEMP_OVERFLOW_CHECK
#endif

#ifdef CONFIG_NET_MEMP_SANITY_CHECK
#define MEMP_SANITY_CHECK	CONFIG_NET_MEMP_SANITY_CHECK
#endif

#ifdef CONFIG_NET_MEMP_SEPARATE_POOLS
#define MEMP_SEPARATE_POOLS	CONFIG_NET_MEMP_SEPARATE_POOLS
#endif

/* ---------- Memory options ---------- */

/*---------- Interanl Memory Pool Sizes ----*/

#ifdef CONFIG_NET_MEMP_NUM_PBUF
#define MEMP_NUM_PBUF	CONFIG_NET_MEMP_NUM_PBUF
#endif

#ifndef MEMP_NUM_RAW_PCB
#ifdef CONFIG_NET_MEMP_NUM_RAW_PCB
#define MEMP_NUM_RAW_PCB	CONFIG_NET_MEMP_NUM_RAW_PCB
#endif
#endif

#ifdef CONFIG_NET_MEMP_NUM_UDP_PCB
#define MEMP_NUM_UDP_PCB	CONFIG_NET_MEMP_NUM_UDP_PCB
#endif

#ifdef CONFIG_NET_MEMP_NUM_TCP_PCB
#define MEMP_NUM_TCP_PCB	CONFIG_NET_MEMP_NUM_TCP_PCB
#endif

#ifdef CONFIG_NET_MEMP_NUM_TCP_PCB_LISTEN
#define MEMP_NUM_TCP_PCB_LISTEN	CONFIG_NET_MEMP_NUM_TCP_PCB_LISTEN
#endif

#ifdef CONFIG_NET_MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG	CONFIG_NET_MEMP_NUM_TCP_SEG
#endif

#ifdef CONFIG_NET_MEMP_NUM_REASSDATA
#define MEMP_NUM_REASSDATA	CONFIG_NET_MEMP_NUM_REASSDATA
#endif

#ifdef CONFIG_NET_MEMP_NUM_FRAG_PBUF
#define MEMP_NUM_FRAG_PBUF	CONFIG_NET_MEMP_NUM_FRAG_PBUF
#endif

#ifdef CONFIG_NET_MEMP_NUM_ARP_QUEUE
#define MEMP_NUM_ARP_QUEUE	CONFIG_NET_MEMP_NUM_ARP_QUEUE
#endif

#ifdef CONFIG_NET_MEMP_NUM_IGMP_GROUP
#define MEMP_NUM_IGMP_GROUP	CONFIG_NET_MEMP_NUM_IGMP_GROUP
#endif

#ifdef CONFIG_NET_MEMP_NUM_SYS_TIMEOUT
#define MEMP_NUM_SYS_TIMEOUT	CONFIG_NET_MEMP_NUM_SYS_TIMEOUT
#endif

#ifdef CONFIG_NET_MEMP_NUM_NETBUF
#define MEMP_NUM_NETBUF	CONFIG_NET_MEMP_NUM_NETBUF
#endif
/*
#ifdef CONFIG_NET_MEMP_NUM_NETCONN
#define MEMP_NUM_NETCONN	CONFIG_NET_MEMP_NUM_NETCONN
#endif
*/
#ifdef CONFIG_NET_MEMP_NUM_TCPIP_MSG_API
#define MEMP_NUM_TCPIP_MSG_API		CONFIG_NET_MEMP_NUM_TCPIP_MSG_API
#endif

#ifdef CONFIG_NET_MEMP_NUM_TCPIP_MSG_INPKT
#define MEMP_NUM_TCPIP_MSG_INPKT	CONFIG_NET_MEMP_NUM_TCPIP_MSG_INPKT
#endif

#ifdef CONFIG_NET_MEMP_NUM_SNMP_NODE
#define MEMP_NUM_SNMP_NODE	CONFIG_NET_MEMP_NUM_SNMP_NODE
#endif

#ifdef CONFIG_NET_MEMP_NUM_SNMP_ROOTNODE
#define MEMP_NUM_SNMP_ROOTNODE	CONFIG_NET_MEMP_NUM_SNMP_ROOTNODE
#endif

#ifdef CONFIG_NET_MEMP_NUM_SNMP_VARBIND
#define MEMP_NUM_SNMP_VARBIND	CONFIG_NET_MEMP_NUM_SNMP_VARBIND
#endif

#ifdef CONFIG_NET_MEMP_NUM_SNMP_VALUE
#define MEMP_NUM_SNMP_VALUE	CONFIG_NET_MEMP_NUM_SNMP_VALUE
#endif

#ifdef CONFIG_NET_MEMP_NUM_PPPOE_INTERFACES
#define MEMP_NUM_PPPOE_INTERFACES	CONFIG_NET_MEMP_NUM_PPPOE_INTERFACES
#endif

#ifdef CONFIG_NET_PBUF_POOL_SIZE
#define PBUF_POOL_SIZE	CONFIG_NET_PBUF_POOL_SIZE
#endif

/*---------- Interanl Memory Pool Sizes ----*/

/* ---------- Raw Socket options ---------- */
#ifdef CONFIG_NET_RAW
#define LWIP_RAW	         CONFIG_NET_RAW
#endif

#ifdef CONFIG_NET_RAW_TTL
#define RAW_TTL	             CONFIG_NET_RAW_TTL
#endif

/* ---------- Socket options ---------- */

#define SOCKLEN_T_DEFINED               1
#define LWIP_TIMEVAL_PRIVATE            0
#define LWIP_POSIX_SOCKETS_IO_NAMES     0

#ifdef CONFIG_DISABLE_POLL
#define LWIP_SELECT                     1
#else
#define LWIP_SELECT                     0
#endif

#ifdef CONFIG_NET_SOCKET
#define LWIP_SOCKET	CONFIG_NET_SOCKET
#endif

#ifdef CONFIG_NET_SOCKET_OPTION_BROADCAST
#define IP_SOF_BROADCAST                CONFIG_NET_SOCKET_OPTION_BROADCAST
#endif

#ifdef CONFIG_NET_RANDOMIZE_INITIAL_LOCAL_PORTS
#define LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS      CONFIG_NET_RANDOMIZE_INITIAL_LOCAL_PORTS
#endif

#ifdef CONFIG_NFILE_DESCRIPTORS
#define LWIP_SOCKET_OFFSET CONFIG_NFILE_DESCRIPTORS
#endif

#ifdef CONFIG_NSOCKET_DESCRIPTORS
#define MEMP_NUM_NETCONN CONFIG_NSOCKET_DESCRIPTORS
#define MEMP_NUM_RAW_PCB CONFIG_NSOCKET_DESCRIPTORS
#endif

#ifdef CONFIG_NET_TCP_KEEPALIVE
#define LWIP_TCP_KEEPALIVE	CONFIG_NET_TCP_KEEPALIVE
#endif

#ifdef CONFIG_NET_SO_SNDTIMEO
#define LWIP_SO_SNDTIMEO	CONFIG_NET_SO_SNDTIMEO
#endif

#ifdef CONFIG_NET_SO_RCVTIMEO
#define LWIP_SO_RCVTIMEO	CONFIG_NET_SO_RCVTIMEO
#endif

#ifdef CONFIG_NET_SO_RCVBUF
#define LWIP_SO_RCVBUF	CONFIG_NET_SO_RCVBUF
#endif

#ifdef CONFIG_NET_SO_REUSE
#define SO_REUSE	CONFIG_NET_SO_REUSE
#endif

#ifdef CONFIG_NET_SO_REUSE_RXTOALL
#define SO_REUSE_RXTOALL	CONFIG_NET_SO_REUSE_RXTOALL
#endif

/* ---------- Socket options ---------- */

/* ---------- SLIP options ---------- */
#ifdef CONFIG_NET_LWIP_SLIP_INTERFACE
#define LWIP_HAVE_SLIPIF                1

#ifdef CONFIG_NET_LWIP_SLIPIF_THREAD_NAME
#define SLIPIF_THREAD_NAME              CONFIG_NET_LWIP_SLIPIF_THREAD_NAME
#endif

#ifdef CONFIG_NET_LWIP_SLIPIF_THREAD_STACKSIZE
#define SLIPIF_THREAD_STACKSIZE         CONFIG_NET_LWIP_SLIPIF_THREAD_STACKSIZE
#endif

#ifdef CONFIG_NET_LWIP_SLIPIF_THREAD_PRIO
#define SLIPIF_THREAD_PRIO              CONFIG_NET_LWIP_SLIPIF_THREAD_PRIO
#endif

#else
#define LWIP_HAVE_SLIPIF                0
#endif
/* ---------- SLIP options ---------- */

/* ---------- PPP options ---------- */
#ifdef CONFIG_NET_LWIP_PPP_SUPPORT
#define PPP_SUPPORT                     1

#ifdef CONFIG_NET_LWIP_PPP_SESSIONS
#define NUM_PPP                         CONFIG_NET_LWIP_PPP_SESSIONS
#endif

#ifdef CONFIG_NET_LWIP_PAP_SUPPORT
#define PAP_SUPPORT                     1
#else
#define PAP_SUPPORT                     0
#endif

#ifdef CONFIG_NET_LWIP_CHAP_SUPPORT
#define CHAP_SUPPORT                    1
#else
#define CHAP_SUPPORT                    0
#endif

#ifdef CONFIG_NET_LWIP_VJ_SUPPORT
#define VJ_SUPPORT                      1
#else
#define VJ_SUPPORT                      0
#endif

#ifdef CONFIG_NET_LWIP_MD5_SUPPORT
#define MD5_SUPPORT                     1
#else
#define MD5_SUPPORT                     0
#endif

#ifdef CONFIG_NET_LWIP_MD5_SUPPORT
#define PPP_MTU                         CONFIG_NET_LWIP_MD5_SUPPORT
#endif

#ifdef CONFIG_NET_LWIP_PPP_THREAD_NAME
#define PPP_THREAD_NAME                 CONFIG_NET_LWIP_PPP_THREAD_NAME
#endif

#ifdef CONFIG_NET_LWIP_PPP_THREAD_STACKSIZE
#define PPP_THREAD_STACKSIZE            CONFIG_NET_LWIP_PPP_THREAD_STACKSIZE
#endif

#ifdef CONFIG_NET_LWIP_PPP_THREAD_PRIO
#define PPP_THREAD_PRIO                 CONFIG_NET_LWIP_PPP_THREAD_PRIO
#endif

#else
#define PPP_SUPPORT                     0
#endif
/* ---------- PPP options ---------- */

/* ---------- System options ---------- */

/* NO_SYS==1: Provides VERY minimal functionality. Otherwise,
 * use lwIP facilities.
 * It should be set NO_SYS for TizenRT.
 */
#define NO_SYS 0

#ifdef CONFIG_NET_TCPIP_CORE_LOCKING
#define LWIP_TCPIP_CORE_LOCKING CONFIG_NET_TCPIP_CORE_LOCKING
#endif

#ifdef CONFIG_NET_TCPIP_CORE_LOCKING_INPUT
#define LWIP_TCPIP_CORE_LOCKING_INPUT CONFIG_NET_TCPIP_CORE_LOCKING_INPUT
#endif

#ifdef CONFIG_NET_TCPIP_THREAD_NAME
#define TCPIP_THREAD_NAME	CONFIG_NET_TCPIP_THREAD_NAME
#endif

#ifdef CONFIG_NET_TCPIP_THREAD_PRIO
#define TCPIP_THREAD_PRIO	CONFIG_NET_TCPIP_THREAD_PRIO
#endif

#ifdef CONFIG_NET_TCPIP_THREAD_STACKSIZE
#define TCPIP_THREAD_STACKSIZE	CONFIG_NET_TCPIP_THREAD_STACKSIZE
#endif

#ifdef CONFIG_NET_COMPAT_MUTEX
#define LWIP_COMPAT_MUTEX	CONFIG_NET_COMPAT_MUTEX
#endif

#ifdef CONFIG_NET_SYS_LIGHTWEIGHT_PROT
#define SYS_LIGHTWEIGHT_PROT	CONFIG_NET_SYS_LIGHTWEIGHT_PROT
#endif

#ifdef CONFIG_NET_DEFAULT_THREAD_NAME
#define DEFAULT_THREAD_NAME	CONFIG_NET_DEFAULT_THREAD_NAME
#endif

#ifdef CONFIG_NET_DEFAULT_THREAD_PRIO
#define DEFAULT_THREAD_PRIO	CONFIG_NET_DEFAULT_THREAD_PRIO
#endif

#ifdef CONFIG_NET_DEFAULT_THREAD_STACKSIZE
#define DEFAULT_THREAD_STACKSIZE	CONFIG_NET_DEFAULT_THREAD_STACKSIZE
#endif

/* ---------- Mailbox options ---------- */

#ifdef CONFIG_NET_DEFAULT_ACCEPTMBOX_SIZE
#define DEFAULT_ACCEPTMBOX_SIZE	CONFIG_NET_DEFAULT_ACCEPTMBOX_SIZE
#endif

#ifdef CONFIG_NET_DEFAULT_RAW_RECVMBOX_SIZE
#define DEFAULT_RAW_RECVMBOX_SIZE	CONFIG_NET_DEFAULT_RAW_RECVMBOX_SIZE
#endif

#ifdef CONFIG_NET_DEFAULT_TCP_RECVMBOX_SIZE
#define DEFAULT_TCP_RECVMBOX_SIZE	CONFIG_NET_DEFAULT_TCP_RECVMBOX_SIZE
#endif

#ifdef CONFIG_NET_DEFAULT_UDP_RECVMBOX_SIZE
#define DEFAULT_UDP_RECVMBOX_SIZE	CONFIG_NET_DEFAULT_UDP_RECVMBOX_SIZE
#endif

#ifdef CONFIG_NET_TCPIP_MBOX_SIZE
#define TCPIP_MBOX_SIZE	CONFIG_NET_TCPIP_MBOX_SIZE
#endif

/* ---------- Mailbox options ---------- */

/* ---------- Debug options ---------- */
#ifdef CONFIG_NET_LWIP_ASSERT
#define LWIP_NOASSERT 0
#else
#define LWIP_NOASSERT 1
#endif

#ifdef CONFIG_NET_LWIP_ERROR
#define LWIP_NOERROR 0
#else
#define LWIP_NOERROR 1
#endif

#ifdef CONFIG_NET_LWIP_DEBUG
#define LWIP_DEBUG 1

#ifdef CONFIG_NET_LWIP_DEBUG_LEVEL
#define LWIP_DBG_MIN_LEVEL      CONFIG_NET_LWIP_DEBUG_LEVEL
#endif

#define LWIP_DEBUG_TIMERNAMES 0

#ifdef CONFIG_NET_ETHARP_DEBUG
#define ETHARP_DEBUG	LWIP_DBG_ON
#else
#define ETHARP_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_NETIF_DEBUG
#define NETIF_DEBUG		LWIP_DBG_ON
#else
#define NETIF_DEBUG		LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_PBUF_DEBUG
#define PBUF_DEBUG	LWIP_DBG_ON
#else
#define PBUF_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_API_LIB_DEBUG
#define API_LIB_DEBUG	LWIP_DBG_ON
#else
#define API_LIB_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_API_MSG_DEBUG
#define API_MSG_DEBUG	LWIP_DBG_ON
#else
#define API_MSG_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_SOCKETS_DEBUG
#define SOCKETS_DEBUG	LWIP_DBG_ON
#else
#define SOCKETS_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_ICMP_DEBUG
#define ICMP_DEBUG	LWIP_DBG_ON
#else
#define ICMP_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_IGMP_DEBUG
#define IGMP_DEBUG	LWIP_DBG_ON
#else
#define IGMP_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_IP_DEBUG
#define IP_DEBUG	LWIP_DBG_ON
#else
#define IP_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_IP_REASS_DEBUG
#define IP_REASS_DEBUG	LWIP_DBG_ON
#else
#define IP_REASS_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_RAW_DEBUG
#define RAW_DEBUG	LWIP_DBG_ON
#else
#define RAW_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_MEM_DEBUG
#define MEM_DEBUG	LWIP_DBG_ON
#else
#define MEM_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_MEMP_DEBUG
#define MEMP_DEBUG	LWIP_DBG_ON
#else
#define MEMP_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_SYS_DEBUG
#define SYS_DEBUG	LWIP_DBG_ON
#else
#define SYS_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_TIMERS_DEBUG
#define TIMERS_DEBUG	LWIP_DBG_ON
#else
#define TIMERS_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_TCP_DEBUG
#define TCP_DEBUG	LWIP_DBG_ON
#else
#define TCP_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_TCP_INPUT_DEBUG
#define TCP_INPUT_DEBUG	LWIP_DBG_ON
#else
#define TCP_INPUT_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_TCP_FR_DEBUG
#define TCP_FR_DEBUG	LWIP_DBG_ON
#else
#define TCP_FR_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_TCP_RTO_DEBUG
#define TCP_RTO_DEBUG	LWIP_DBG_ON
#else
#define TCP_RTO_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_TCP_CWND_DEBUG
#define TCP_CWND_DEBUG	LWIP_DBG_ON
#else
#define TCP_CWND_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_TCP_WND_DEBUG
#define TCP_WND_DEBUG	LWIP_DBG_ON
#else
#define TCP_WND_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_TCP_OUTPUT_DEBUG
#define TCP_OUTPUT_DEBUG	LWIP_DBG_ON
#else
#define TCP_OUTPUT_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_TCP_RST_DEBUG
#define TCP_RST_DEBUG	LWIP_DBG_ON
#else
#define TCP_RST_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_TCP_QLEN_DEBUG
#define TCP_QLEN_DEBUG	LWIP_DBG_ON
#else
#define TCP_QLEN_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_UDP_DEBUG
#define UDP_DEBUG	LWIP_DBG_ON
#else
#define UDP_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_TCPIP_DEBUG
#define TCPIP_DEBUG	LWIP_DBG_ON
#else
#define TCPIP_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_AUTOIP_DEBUG
#define AUTOIP_DEBUG	LWIP_DBG_ON
#else
#define AUTOIP_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_POLL_DEBUG
#define POLL_DEBUG	LWIP_DBG_ON
#else
#define POLL_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_IPv6_DEBUG
#define IP6_DEBUG	LWIP_DBG_ON
#else
#define IP6_DEBUG	LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_ND6_DEBUG
#define ND6_DEBUG	LWIP_DBG_ON
#else
#define ND6_DEBUG	LWIP_DBG_OFF
#endif

#endif /* CONFIG_NET_LWIP_DEBUG */
/* ---------- Debug options ---------- */

/* ---------- Stat options ---------- */

#ifdef CONFIG_NET_STATS
#define LWIP_STATS	CONFIG_NET_STATS
#endif

#ifdef CONFIG_NET_STATS_DISPLAY
#define LWIP_STATS_DISPLAY	CONFIG_NET_STATS_DISPLAY
#endif

#ifdef CONFIG_NET_LINK_STATS
#define LINK_STATS	CONFIG_NET_LINK_STATS
#endif

#ifdef CONFIG_NET_ETHARP_STATS
#define ETHARP_STATS	CONFIG_NET_ETHARP_STATS
#endif

#ifdef CONFIG_NET_IP_STATS
#define IP_STATS	CONFIG_NET_IP_STATS
#endif

#ifdef CONFIG_NET_IPFRAG_STATS
#define IPFRAG_STATS	CONFIG_NET_IPFRAG_STATS
#endif

#ifdef CONFIG_NET_ICMP_STATS
#define ICMP_STATS	CONFIG_NET_ICMP_STATS
#endif

#ifdef CONFIG_NET_IGMP_STATS
#define IGMP_STATS	CONFIG_NET_IGMP_STATS
#endif

#ifdef CONFIG_NET_UDP_STATS
#define UDP_STATS	CONFIG_NET_UDP_STATS
#endif

#ifdef CONFIG_NET_TCP_STATS
#define TCP_STATS	CONFIG_NET_TCP_STATS
#endif

#ifdef CONFIG_NET_MEM_STATS
#define MEM_STATS	CONFIG_NET_MEM_STATS
#endif

#ifdef CONFIG_NET_MEMP_STATS
#define MEMP_STATS	CONFIG_NET_MEMP_STATS
#endif

#ifdef CONFIG_NET_SYS_STATS
#define SYS_STATS	CONFIG_NET_SYS_STATS
#endif

#ifdef CONFIG_NET_IPv6_STATS
#define IP6_STATS	CONFIG_NET_IPv6_STATS
#else
#define IP6_STATS	0	
#endif

#ifdef CONFIG_NET_IPv6_ICMP_STATS
#define ICMP6_STATS	CONFIG_NET_IPv6_ICMP_STATS
#else
#define ICMP6_STATS	0	
#endif

#ifdef CONFIG_NET_IPv6_FRAG_STATS
#define IP6_FRAG_STATS	CONFIG_NET_IPv6_FRAG_STATS
#else
#define IP6_FRAG_STATS	0
#endif

#ifdef CONFIG_NET_IPv6_MLD_STATS
#define MLD6_STATS	CONFIG_NET_IPv6_MLD_STATS
#else
#define MLD6_STATS	0
#endif

#ifdef CONFIG_NET_IPv6_ND_STATS
#define ND6_STATS	CONFIG_NET_IPv6_ND_STATS
#else
#define ND6_STATS	0
#endif

/* ---------- DNS options --------- */

/* ---------- Stat options ---------- */
#ifdef CONFIG_NET_LWIP_NETDB
#define LWIP_DNS CONFIG_NET_LWIP_NETDB
#endif

#if LWIP_DNS

#ifdef CONFIG_NET_DNS_TABLE_SIZE
#define DNS_TABLE_SIZE	CONFIG_NET_DNS_TABLE_SIZE
#endif

#ifdef CONFIG_NET_DNS_MAX_NAME_LENGTH
#define DNS_MAX_NAME_LENGTH CONFIG_NET_DNS_MAX_NAME_LENGTH
#endif

#ifdef CONFIG_NET_DNS_MAX_SERVERS
#define DNS_MAX_SERVERS CONFIG_NET_DNS_MAX_SERVERS
#endif

#ifdef CONFIG_NET_DNS_DOES_NAME_CHECK
#define DNS_DOES_NAME_CHECK CONFIG_NET_DNS_DOES_NAME_CHECK
#endif

#ifdef CONFIG_NET_DNS_SECURE
#define LWIP_DNS_SECURE CONFIG_NET_DNS_SECURE
#endif

#ifdef CONFIG_NET_DNS_LOCAL_HOSTLIST
#define DNS_LOCAL_HOSTLIST CONFIG_NET_DNS_LOCAL_HOSTLIST

#ifdef CONFIG_NET_DNS_LOCAL_HOSTLIST_IS_DYNAMIC
#define DNS_LOCAL_HOSTLIST_IS_DYNAMIC CONFIG_NET_DNS_LOCAL_HOSTLIST_IS_DYNAMIC
#endif
#endif /* CONFIG_NET_DNS_LOCAL_HOSTLIST */

#endif /* LWIP_DNS */
/* ---------- End of DNS options ---------*/


/* ----------Else ----------------*/
#ifdef CONFIG_NET_LWIP_LOOPBACK_INTERFACE
#define LWIP_HAVE_LOOPIF                CONFIG_NET_LWIP_LOOPBACK_INTERFACE
#endif

#endif							/* __LWIP_LWIPOPTS_H__ */
