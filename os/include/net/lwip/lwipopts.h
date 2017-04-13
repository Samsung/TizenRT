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
#include <time.h>
#include <debug.h>
#include <stdlib.h>
#include <net/lwip/debug.h>

/* NO_SYS==1: Provides VERY minimal functionality. Otherwise,
* use lwIP facilities.
*/
#define NO_SYS                          0
#define SYS_LIGHTWEIGHT_PROT            1

/* ---------- ARP options ---------- */
#ifdef CONFIG_NET_ARP
#define LWIP_ARP                        1
#else
#define LWIP_ARP                        0
#endif

#if defined(CONFIG_NET_ARP)
#if !defined(CONFIG_NET_ARP_TABLESIZE)
#error "Please define LWIP ARP Table size"
#else
#define ARP_TABLE_SIZE  CONFIG_NET_ARP_TABLESIZE
#endif
#endif

#ifdef CONFIG_NET_ARP_QUEUEING
#define ARP_QUEUEING                    1
#else
#define ARP_QUEUEING                    0
#endif


#ifdef CONFIG_NET_ARP_TRUST_IP_MAC
#define ETHARP_TRUST_IP_MAC             1
#else
#define ETHARP_TRUST_IP_MAC             0
#endif

#ifdef CONFIG_NET_ETH_PAD_SIZE
#define ETH_PAD_SIZE                    CONFIG_NET_ETH_PAD_SIZE
#endif

#ifdef CONFIG_NET_ARP_STATIC_ENTRIES
#define ETHARP_SUPPORT_STATIC_ENTRIES   1
#endif
/* ---------- ARP options ---------- */


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
#ifdef CONFIG_NET_LWIP_IP_FORWARD
#define IP_FORWARD                      1
#else
#define IP_FORWARD                      0
#endif

#ifdef CONFIG_NET_LWIP_IP_OPTIONS_ALLOWED
#define IP_OPTIONS_ALLOWED              1
#else
#define IP_OPTIONS_ALLOWED              0
#endif


#ifdef CONFIG_NET_LWIP_IP_REASSEMBLY
#define IP_REASSEMBLY                   1
#else
#define IP_REASSEMBLY                   0
#endif

#ifdef CONFIG_NET_LWIP_IP_FRAG
#define IP_FRAG                         1
#else
#define IP_FRAG                         0
#endif

#ifdef CONFIG_NET_LWIP_IP_REASSEMBLY_MAXAGE
#define IP_REASS_MAXAGE                 CONFIG_NET_LWIP_IP_REASSEMBLY_MAXAGE
#endif

#ifdef CONFIG_NET_LWIP_IP_REASS_MAX_PBUFS
#define IP_REASS_MAX_PBUFS              CONFIG_NET_LWIP_IP_REASS_MAX_PBUFS
#endif

#ifdef CONFIG_NET_LWIP_IP_DEFAULT_TTL
#define IP_DEFAULT_TTL                  CONFIG_NET_LWIP_IP_DEFAULT_TTL
#endif
/* ---------- IP options ---------- */


/* ---------- ICMP options ---------- */
#ifdef CONFIG_NET_ICMP
#define LWIP_ICMP                       1
#else
#define LWIP_ICMP                       0
#endif

#ifdef CONFIG_NET_BROADCAST_PING
#define LWIP_BROADCAST_PING             1
#else
#define LWIP_BROADCAST_PING             0
#endif

#ifdef CONFIG_NET_MULTICAST_PING
#define LWIP_MULTICAST_PING             1
#else
#define LWIP_MULTICAST_PING             0
#endif
/* ---------- ICMP options ---------- */

/* ---------- IGMP options ---------- */
#ifdef CONFIG_NET_LWIP_IGMP
#define LWIP_IGMP                       1
#else
#define LWIP_IGMP                       0
#endif

#ifdef CONFIG_NET_LWIP_MEMP_NUM_IGMP_GROUP
#define MEMP_NUM_IGMP_GROUP             CONFIG_NET_LWIP_MEMP_NUM_IGMP_GROUP
#endif

/* ---------- IGMP options ---------- */

#define LWIP_RAND()                     rand()

/* ---------- TCP options ---------- */
#ifdef CONFIG_NET_TCP
#define LWIP_TCP                1

#ifdef CONFIG_NET_TCP_WND
#define TCP_WND                 CONFIG_NET_TCP_WND
#endif

#ifdef CONFIG_NET_TCP_MAXRTX
#define TCP_MAXRTX              CONFIG_NET_TCP_MAXRTX
#endif

#ifdef CONFIG_NET_TCP_SYNMAXRTX
#define TCP_SYNMAXRTX           CONFIG_NET_TCP_SYNMAXRTX
#endif

#ifdef CONFIG_NET_TCP_QUEUE_OOSEQ
#define TCP_QUEUE_OOSEQ         1
#else
#define TCP_QUEUE_OOSEQ         0
#endif

#ifdef CONFIG_NET_TCP_MSS
#define TCP_MSS                 CONFIG_NET_TCP_MSS
#endif

#ifdef CONFIG_NET_TCP_SND_BUF
#define TCP_SND_BUF             CONFIG_NET_TCP_SND_BUF
#endif

#ifdef CONFIG_NET_TCP_TIMESTAMPS
#define LWIP_TCP_TIMESTAMPS             1
#else
#define LWIP_TCP_TIMESTAMPS             0
#endif

#ifdef CONFIG_NET_TCP_KEEPALIVE
#define LWIP_TCP_KEEPALIVE              1
#else
#define LWIP_TCP_KEEPALIVE              0
#endif

#ifdef CONFIG_NET_TCP_LISTEN_BACKLOG
#define TCP_LISTEN_BACKLOG              1
#else
#define TCP_LISTEN_BACKLOG              0
#endif

#else
#define LWIP_TCP                0
#endif

#define TCP_TTL                         255
#define LWIP_TCPIP_CORE_LOCKING_INPUT   0
#define LWIP_TCPIP_CORE_LOCKING         0
/* ---------- TCP options ---------- */


/* ---------- UDP options ---------- */
#ifdef CONFIG_NET_UDP
#define LWIP_UDP                        1
#else
#define LWIP_UDP                        0
#endif

#ifdef CONFIG_NET_UDPLITE
#define LWIP_UDPLITE                    1
#else
#define LWIP_UDPLITE                    0
#endif

#define UDP_TTL                         255
#define LWIP_UDP_TODO                   1
/* ---------- UDP options ---------- */

/* ---------- DHCP options ---------- */

/*
 * Note
 * In this platform, DHCP client and server are supported through netutils
 * Instead of using lwIP DHCP, please use netutils dhcpc and dhcpd
 */
#define LWIP_DHCPS                      0
#define LWIP_DHCP                       0
/* ---------- DHCP options ---------- */

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

/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */

#define LWIP_NETCONN                    1
#define MEMP_NUM_NETCONN                CONFIG_NSOCKET_DESCRIPTORS
#define MEMP_NUM_SYS_TIMEOUT            16
/*
 * MEMP_NUM_NETBUF: the number of struct netbufs.
 * (only needed if you use the sequential API, like api_lib.c)
 */

#define MEMP_NUM_NETBUF                 16
#define MEMP_NUM_TCPIP_MSG_API          16
#define MEMP_NUM_TCPIP_INPKT            16

/* ---------- Memory options ---------- */
#if !defined(CONFIG_NET_MEM_ALIGNMENT)
#error "CONFIG_NET_MEM_ALIGNMENT is undefined"
#else
#define MEM_ALIGNMENT                   CONFIG_NET_MEM_ALIGNMENT
#endif

#if !defined(CONFIG_NET_MEM_SIZE)
#error "CONFIG_NET_MEM_SIZE is undefined"
#else
#define MEM_SIZE                	CONFIG_NET_MEM_SIZE
#endif

#ifdef CONFIG_NET_MEMP_OVERFLOW_CHECK
#define MEMP_OVERFLOW_CHECK		CONFIG_NET_MEMP_OVERFLOW_CHECK
#else
#define MEMP_OVERFLOW_CHECK		0
#endif

#ifdef CONFIG_NET_MEMP_SANITY_CHECK
#define MEMP_SANITY_CHECK               1
#else
#define MEMP_SANITY_CHECK               0
#endif

#define MEMP_MEM_MALLOC                 1

#ifdef CONFIG_NET_MEMP_NUM_PBUF
#define MEMP_NUM_PBUF                   CONFIG_NET_MEMP_NUM_PBUF
#endif

#ifdef CONFIG_NET_MEMP_NUM_RAW_PCB
#define MEMP_NUM_RAW_PCB                CONFIG_NET_MEMP_NUM_RAW_PCB
#endif

#ifdef CONFIG_NET_MEMP_NUM_UDP_PCB
#define MEMP_NUM_UDP_PCB		CONFIG_NET_MEMP_NUM_UDP_PCB
#endif


#ifdef CONFIG_NET_MEMP_NUM_TCP_PCB
#define MEMP_NUM_TCP_PCB		CONFIG_NET_MEMP_NUM_TCP_PCB
#endif

#ifdef CONFIG_NET_MEMP_NUM_TCP_PCB_LISTEN
#define MEMP_NUM_TCP_PCB_LISTEN         CONFIG_NET_MEMP_NUM_TCP_PCB_LISTEN
#endif

#ifdef CONFIG_NET_MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG                CONFIG_NET_MEMP_NUM_TCP_SEG
#endif

#ifdef CONFIG_NET_MEMP_NUM_REASSDATA
#define MEMP_NUM_REASSDATA              CONFIG_NET_MEMP_NUM_REASSDATA
#endif

#ifdef CONFIG_NET_MEMP_NUM_FRAG_PBUF
#define MEMP_NUM_FRAG_PBUF              CONFIG_NET_MEMP_NUM_FRAG_PBUF
#endif

#ifdef CONFIG_NET_MEMP_NUM_ARP_QUEUE
#define MEMP_NUM_ARP_QUEUE		CONFIG_NET_MEMP_NUM_ARP_QUEUE
#endif
/* ---------- Memory options ---------- */


/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#define PBUF_POOL_SIZE          50


/* ---------- Socket options ---------- */
#ifdef CONFIG_NET_SOCKET
#define LWIP_SOCKET                     1

#ifdef CONFIG_NET_RAW
#define LWIP_RAW                        1
#else
#define LWIP_RAW                        0
#endif

#ifdef CONFIG_NET_SOCKET_OPTION_BROADCAST
#define IP_SOF_BROADCAST                1
#else
#define IP_SOF_BROADCAST                0
#endif


#ifdef CONFIG_NET_RANDOMIZE_INITIAL_LOCAL_PORTS
#define LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS      1
#else
#define LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS      0
#endif

#ifdef CONFIG_NET_SO_SNDTIMEO
#define LWIP_SO_SNDTIMEO                        1
#else
#define LWIP_SO_SNDTIMEO                        0
#endif

#ifdef CONFIG_NET_SO_RCVTIMEO
#define LWIP_SO_RCVTIMEO                        1
#else
#define LWIP_SO_RCVTIMEO                        0
#endif

#ifdef CONFIG_NET_SO_RCVBUF
#define LWIP_SO_RCVBUF                          1
#else
#define LWIP_SO_RCVBUF                          0
#endif

#ifdef CONFIG_NET_SO_REUSE
#define SO_REUSE                                1
#else
#define SO_REUSE                                0
#endif

#else
#define LWIP_SOCKET                     0
#endif
/* ---------- Socket options ---------- */

#ifdef CONFIG_DISABLE_POLL
#define LWIP_SELECT                     1
#else
#define LWIP_SELECT                     0
#endif

#define LWIP_POSIX_SOCKETS_IO_NAMES     0
#define LWIP_SOCKET_OFFSET              CONFIG_NFILE_DESCRIPTORS

#define SOCKLEN_T_DEFINED               1

#define LWIP_TIMEVAL_PRIVATE            0

#ifdef CONFIG_NET_LWIP_LOOPBACK_INTERFACE
#define LWIP_HAVE_LOOPIF                1
#else
#define LWIP_HAVE_LOOPIF                0
#endif

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


/* Thread options */

#ifdef CONFIG_NET_LWIP_TCPIP_THREAD_NAME
#define TCPIP_THREAD_NAME               CONFIG_NET_LWIP_TCPIP_THREAD_NAME
#endif

#ifdef CONFIG_NET_LWIP_TCPIP_THREAD_STACKSIZE
#define TCPIP_THREAD_STACKSIZE          CONFIG_NET_LWIP_TCPIP_THREAD_STACKSIZE
#endif

#ifdef CONFIG_NET_LWIP_TCPIP_THREAD_PRIO
#define TCPIP_THREAD_PRIO               CONFIG_NET_LWIP_TCPIP_THREAD_PRIO
#endif

#ifdef CONFIG_NET_LWIP_TCPIP_MBOX_SIZE
#define TCPIP_MBOX_SIZE                 CONFIG_NET_LWIP_TCPIP_MBOX_SIZE
#endif

#ifdef CONFIG_NET_LWIP_DEFAULT_THREAD_NAME
#define DEFAULT_THREAD_NAME             CONFIG_NET_LWIP_DEFAULT_THREAD_NAME
#endif

#ifdef CONFIG_NET_LWIP_DEFAULT_THREAD_STACKSIZE
#define DEFAULT_THREAD_STACKSIZE        CONFIG_NET_LWIP_DEFAULT_THREAD_STACKSIZE
#endif

#ifdef CONFIG_NET_LWIP_DEFAULT_THREAD_PRIO
#define DEFAULT_THREAD_PRIO             CONFIG_NET_LWIP_DEFAULT_THREAD_PRIO
#endif

#define DEFAULT_RAW_RECVMBOX_SIZE       64
#define DEFAULT_UDP_RECVMBOX_SIZE       64
#define DEFAULT_TCP_RECVMBOX_SIZE       54
#define DEFAULT_ACCEPTMBOX_SIZE         64
/* Thread options */

/* ---------- Debug options ---------- */
#ifdef CONFIG_NET_LWIP_DEBUG
#define LWIP_DEBUG                      1


#ifdef CONFIG_NET_LWIP_DEBUG_LEVEL
#define LWIP_DBG_MIN_LEVEL              CONFIG_NET_LWIP_DEBUG_LEVEL
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_ETHARP
#define ETHARP_DEBUG                    LWIP_DBG_ON
#else
#define ETHARP_DEBUG                    LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_NETIF
#define NETIF_DEBUG                     LWIP_DBG_ON
#else
#define NETIF_DEBUG                     LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_PBUF
#define PBUF_DEBUG                      LWIP_DBG_ON
#else
#define PBUF_DEBUG                      LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_API_LIB
#define API_LIB_DEBUG                   LWIP_DBG_ON
#else
#define API_LIB_DEBUG                   LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_API_MSG
#define API_MSG_DEBUG                   LWIP_DBG_ON
#else
#define API_MSG_DEBUG                   LWIP_DBG_OFF
#endif


#ifdef CONFIG_NET_LWIP_DEBUG_SOCKETS
#define SOCKETS_DEBUG                   LWIP_DBG_ON
#else
#define SOCKETS_DEBUG                   LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_ICMP
#define ICMP_DEBUG                      LWIP_DBG_ON
#else
#define ICMP_DEBUG                      LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_IGMP
#define IGMP_DEBUG                      LWIP_DBG_ON
#else
#define IGMP_DEBUG                      LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_INET
#define INET_DEBUG                      LWIP_DBG_ON
#else
#define INET_DEBUG                      LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_IP
#define IP_DEBUG                        LWIP_DBG_ON
#else
#define IP_DEBUG                        LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_IP_REASS
#define IP_REASS_DEBUG                  LWIP_DBG_ON
#else
#define IP_REASS_DEBUG                  LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_RAW
#define RAW_DEBUG                       LWIP_DBG_ON
#else
#define RAW_DEBUG                       LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_MEM
#define MEM_DEBUG                       LWIP_DBG_ON
#else
#define MEM_DEBUG                       LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_SYS
#define SYS_DEBUG                       LWIP_DBG_ON
#else
#define SYS_DEBUG                       LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_TIMER
#define TIMER_DEBUG                     LWIP_DBG_ON
#else
#define TIMER_DEBUG                     LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_TCP
#define TCP_DEBUG                       LWIP_DBG_ON
#else
#define TCP_DEBUG                       LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_TCP_INPUT
#define TCP_INPUT_DEBUG                 LWIP_DBG_ON
#else
#define TCP_INPUT_DEBUG                 LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_TCP_FR
#define TCP_FR_DEBUG                    LWIP_DBG_ON
#else
#define TCP_FR_DEBUG                    LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_TCP_RTO
#define TCP_RTO_DEBUG                   LWIP_DBG_ON
#else
#define TCP_RTO_DEBUG                   LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_TCP_CWND
#define TCP_CWND_DEBUG                  LWIP_DBG_ON
#else
#define TCP_CWND_DEBUG                  LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_TCP_WND
#define TCP_WND_DEBUG                   LWIP_DBG_ON
#else
#define TCP_WND_DEBUG                   LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_TCP_OUTPUT
#define TCP_OUTPUT_DEBUG                LWIP_DBG_ON
#else
#define TCP_OUTPUT_DEBUG                LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_TCP_RST
#define TCP_RST_DEBUG                   LWIP_DBG_ON
#else
#define TCP_RST_DEBUG                   LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_TCP_QLEN
#define TCP_QLEN_DEBUG                  LWIP_DBG_ON
#else
#define TCP_QLEN_DEBUG                  LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_UDP
#define UDP_DEBUG                       LWIP_DBG_ON
#else
#define UDP_DEBUG                       LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_TCPIP
#define TCPIP_DEBUG                     LWIP_DBG_ON
#else
#define TCPIP_DEBUG                     LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_PPP
#define PPP_DEBUG                       LWIP_DBG_ON
#else
#define PPP_DEBUG                       LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_SLIP
#define SLIP_DEBUG                      LWIP_DBG_ON
#else
#define SLIP_DEBUG                      LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_SNMP_MSG
#define SNMP_MSG_DEBUG                  LWIP_DBG_ON
#else
#define SNMP_MSG_DEBUG                  LWIP_DBG_OFF
#endif

#ifdef CONFIG_NET_LWIP_DEBUG_SNMP_MIB
#define SNMP_MIB_DEBUG                  LWIP_DBG_ON
#else
#define SNMP_MIB_DEBUG                  LWIP_DBG_OFF
#endif

#endif

/* ---------- Debug options ---------- */


/* ---------- Stat options ---------- */
#ifdef CONFIG_NET_LWIP_STATS
#define LWIP_STATS                      1

#ifdef CONFIG_NET_LWIP_STATS_DISPLAY
#define  LWIP_STATS_DISPLAY             1
#else
#define  LWIP_STATS_DISPLAY             0
#endif

#ifdef CONFIG_NET_LWIP_STATS_LINK
#define  LINK_STATS                     1
#else
#define  LINK_STATS                     0
#endif

#ifdef CONFIG_NET_LWIP_STATS_ARP
#define  ETHARP_STATS                   1
#else
#define  ETHARP_STATS                   0
#endif

#ifdef CONFIG_NET_LWIP_STATS_IP
#define  IP_STATS                       1
#else
#define  IP_STATS                       0
#endif

#ifdef CONFIG_NET_LWIP_STATS_IPFRAG
#define  IPFRAG_STATS                   1
#else
#define  IPFRAG_STATS                   0
#endif

#ifdef CONFIG_NET_LWIP_STATS_ICMP
#define  ICMP_STATS                     1
#else
#define  ICMP_STATS                     0
#endif


#ifdef CONFIG_NET_LWIP_STATS_IGMP
#define  IGMP_STATS                     1
#else
#define  IGMP_STATS                     0
#endif

#ifdef CONFIG_NET_LWIP_STATS_UDP
#define  UDP_STATS                      1
#else
#define  UDP_STATS                      0
#endif

#ifdef CONFIG_NET_LWIP_STATS_TCP
#define  TCP_STATS                      1
#else
#define  TCP_STATS                      0
#endif

#ifdef CONFIG_NET_LWIP_STATS_MEM
#define  MEM_STATS                      1
#else
#define  MEM_STATS                      0
#endif


#ifdef CONFIG_NET_LWIP_STATS_SYS
#define  SYS_STATS                      1
#else
#define  SYS_STATS                      0
#endif

#else
#define LWIP_STATS                      0
#endif
/* ---------- Stat options ---------- */


#define LWIP_COMPAT_MUTEX               1

#define LWIP_NETIF_API                  1
#endif							/* __LWIP_LWIPOPTS_H__ */
