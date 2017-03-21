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

#define ARP_QUEUEING                    1
#define ETHARP_TRUST_IP_MAC             1

/* ---------- IP options ---------- */
#define IP_REASS_MAXAGE                 5
#define IP_REASS_MAX_PBUFS              20


/* ---------- ICMP options ---------- */
#ifdef CONFIG_NET_LWIP_ICMP
#define LWIP_ICMP                       1
#else
#define LWIP_ICMP                       0
#endif

#ifdef CONFIG_NET_LWIP_ICMP_BROADCAST_PING
#define LWIP_BROADCAST_PING             1
#else
#define LWIP_BROADCAST_PING             0
#endif

#ifdef CONFIG_NET_LWIP_ICMP_MULTICAST_PING
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
#ifdef CONFIG_NET_LWIP_TCP
#define LWIP_TCP                1

#ifdef CONFIG_NET_LWIP_TCP_WINDOW_SIZE
#define TCP_WND                 CONFIG_NET_LWIP_TCP_WINDOW_SIZE
#endif

#ifdef CONFIG_NET_LWIP_TCP_MAX_RETRANS
#define TCP_MAXRTX              CONFIG_NET_LWIP_TCP_MAX_RETRANS
#endif

#ifdef CONFIG_NET_LWIP_TCP_SYN_MAX_RETRANS
#define TCP_SYNMAXRTX           CONFIG_NET_LWIP_TCP_SYN_MAX_RETRANS
#endif

#ifdef CONFIG_NET_LWIP_TCP_QUEUE_OOSEQ
#define TCP_QUEUE_OOSEQ         1
#else
#define TCP_QUEUE_OOSEQ         0
#endif

#ifdef CONFIG_NET_LWIP_TCP_MSS
#define TCP_MSS                 CONFIG_NET_LWIP_TCP_MSS
#endif

#ifdef CONFIG_NET_LWIP_TCP_SEND_BUF
#define TCP_SND_BUF             CONFIG_NET_LWIP_TCP_SEND_BUF
#endif

#ifdef CONFIG_NET_LWIP_TCP_TIMESTAMPS
#define LWIP_TCP_TIMESTAMPS             1
#else
#define LWIP_TCP_TIMESTAMPS             0
#endif

#ifdef CONFIG_NET_LWIP_TCP_KEEPALIVE
#define LWIP_TCP_KEEPALIVE              1
#else
#define LWIP_TCP_KEEPALIVE              0
#endif

#ifdef CONFIG_NET_LWIP_TCP_LISTEN_BACKLOG
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
#ifdef CONFIG_NET_LWIP_UDP
#define LWIP_UDP                        1
#else
#define LWIP_UDP                        0
#endif

#ifdef CONFIG_NET_LWIP_UDPLITE
#define LWIP_UDPLITE                    1
#else
#define LWIP_UDPLITE                    0
#endif

#define UDP_TTL                         255
#define LWIP_UDP_TODO                   1
#define LWIP_DHCPS                      0
/* ---------- UDP options ---------- */



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
#if !defined(CONFIG_NET_LWIP_MEM_ALIGNMENT)
#error "CONFIG_NET_LWIP_MEM_ALIGNMENT is undefined"
#else
#define MEM_ALIGNMENT                   CONFIG_NET_LWIP_MEM_ALIGNMENT
#endif

#if !defined(CONFIG_NET_LWIP_MEM_SIZE)
#error "CONFIG_NET_LWIP_MEM_SIZE is undefined"
#else
#define MEM_SIZE                	CONFIG_NET_LWIP_MEM_SIZE
#endif

#ifdef CONFIG_NET_LWIP_MEMP_OVERFLOW_CHECK
#define MEMP_OVERFLOW_CHECK		CONFIG_NET_LWIP_MEMP_OVERFLOW_CHECK
#else
#define MEMP_OVERFLOW_CHECK		0
#endif

#ifdef CONFIG_NET_LWIP_MEMP_SANITY_CHECK
#define MEMP_SANITY_CHECK               1
#else
#define MEMP_SANITY_CHECK               0
#endif

#define MEMP_MEM_MALLOC                 1

#ifdef CONFIG_NET_LWIP_MEMP_NUM_PBUF
#define MEMP_NUM_PBUF                   CONFIG_NET_LWIP_MEMP_NUM_PBUF
#endif

#ifdef CONFIG_NET_LWIP_MEMP_NUM_RAW_PCB
#define MEMP_NUM_RAW_PCB                CONFIG_NET_LWIP_MEMP_NUM_RAW_PCB
#endif

#ifdef CONFIG_NET_LWIP_MEMP_NUM_UDP_PCB
#define MEMP_NUM_UDP_PCB		CONFIG_NET_LWIP_MEMP_NUM_UDP_PCB
#endif


#ifdef CONFIG_NET_LWIP_MEMP_NUM_TCP_PCB
#define MEMP_NUM_TCP_PCB		CONFIG_NET_LWIP_MEMP_NUM_TCP_PCB
#endif

#ifdef CONFIG_NET_LWIP_MEMP_NUM_TCP_PCB_LISTEN
#define MEMP_NUM_TCP_PCB_LISTEN         CONFIG_NET_LWIP_MEMP_NUM_TCP_PCB_LISTEN
#endif

#ifdef CONFIG_NET_LWIP_MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG                CONFIG_NET_LWIP_MEMP_NUM_TCP_SEG
#endif

#ifdef CONFIG_NET_LWIP_MEMP_NUM_REASSDATA
#define MEMP_NUM_REASSDATA              CONFIG_NET_LWIP_MEMP_NUM_REASSDATA
#endif

#ifdef CONFIG_NET_LWIP_MEMP_NUM_FRAG_PBUF
#define MEMP_NUM_FRAG_PBUF              CONFIG_NET_LWIP_MEMP_NUM_FRAG_PBUF
#endif

#ifdef CONFIG_NET_LWIP_MEMP_NUM_ARP_QUEUE
#define MEMP_NUM_ARP_QUEUE		CONFIG_NET_LWIP_MEMP_NUM_ARP_QUEUE
#endif
/* ---------- Memory options ---------- */


/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#define PBUF_POOL_SIZE          50


/* ---------- Socket options ---------- */
#ifdef CONFIG_NET_LWIP_SOCKET
#define LWIP_SOCKET                     1

#ifdef CONFIG_NET_LWIP_RAW
#define LWIP_RAW                        1
#else
#define LWIP_RAW                        0
#endif

#ifdef CONFIG_NET_LWIP_SOCKET_OPTION_BROADCAST
#define IP_SOF_BROADCAST                1
#else
#define IP_SOF_BROADCAST                0
#endif


#ifdef CONFIG_NET_LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS
#define LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS      1
#else
#define LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS      0
#endif

#ifdef CONFIG_NET_LWIP_SOCKET_SENDTIMEOUT
#define LWIP_SO_SNDTIMEO                        1
#else
#define LWIP_SO_SNDTIMEO                        0
#endif

#ifdef CONFIG_NET_LWIP_SOCKET_RECVTIMEOUT
#define LWIP_SO_RCVTIMEO                        1
#else
#define LWIP_SO_RCVTIMEO                        0
#endif

#ifdef CONFIG_NET_LWIP_SOCKET_RCVBUF
#define LWIP_SO_RCVBUF                          1
#else
#define LWIP_SO_RCVBUF                          0
#endif

#ifdef CONFIG_NET_LWIP_SOCKET_REUSE
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

#define LWIP_HAVE_LOOPIF                1


/*
   -----------------------------------
   ---------- DEBUG options ----------
   -----------------------------------
*/

#define LWIP_DEBUG                      1

#define API_LIB_DEBUG               LWIP_DBG_OFF
#define API_MSG_DEBUG               LWIP_DBG_OFF
#define DHCP_DEBUG                  LWIP_DBG_OFF
#define DNS_DEBUG                   LWIP_DBG_OFF
#define ETHARP_DEBUG                LWIP_DBG_OFF
#define ETHIF_DEBUG                 LWIP_DBG_OFF
#define UDP_DEBUG                   LWIP_DBG_OFF
#define UDP_LPC_EMAC                LWIP_DBG_OFF
#define IP_DEBUG                    LWIP_DBG_OFF
#define IP_REASS_DEBUG              LWIP_DBG_OFF
#define ICMP_DEBUG                  LWIP_DBG_OFF
#define IGMP_DEBUG                  LWIP_DBG_OFF
#define MEM_DEBUG                   LWIP_DBG_OFF
#define MEMP_DEBUG                  LWIP_DBG_OFF
#define NETIF_DEBUG                 LWIP_DBG_OFF
#define PBUF_DEBUG                  LWIP_DBG_OFF
#define POLL_DEBUG                  LWIP_DBG_OFF
#define PPP_DEBUG                   LWIP_DBG_OFF
#define SOCKETS_DEBUG               LWIP_DBG_OFF
#define SYS_DEBUG                   LWIP_DBG_OFF
#define SYSARCH_DEBUG               LWIP_DBG_OFF
#define TCPIP_DEBUG                 LWIP_DBG_OFF
#define TCP_DEBUG                   LWIP_DBG_OFF
#define TCP_INPUT_DEBUG             LWIP_DBG_OFF
#define TCP_FR_DEBUG                LWIP_DBG_OFF
#define TCP_RTO_DEBUG               LWIP_DBG_OFF
#define TCP_CWND_DEBUG              LWIP_DBG_OFF
#define TCP_WND_DEBUG               LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG            LWIP_DBG_OFF
#define TCP_RST_DEBUG               LWIP_DBG_OFF
#define TCP_QLEN_DEBUG              LWIP_DBG_OFF
#define TIMERS_DEBUG                LWIP_DBG_OFF

#define LWIP_DEBUG_TIMERNAMES       0

/*
   ---------------------------------
   ---------- OS options ----------
   ---------------------------------
*/

#define TCPIP_THREAD_NAME              "LWIP_TCP/IP"
#define TCPIP_THREAD_STACKSIZE          (1024*4)
#define TCPIP_MBOX_SIZE                 64
#define DEFAULT_RAW_RECVMBOX_SIZE       64
#define DEFAULT_UDP_RECVMBOX_SIZE       64
#define DEFAULT_TCP_RECVMBOX_SIZE       54
#define DEFAULT_ACCEPTMBOX_SIZE         64
#define DEFAULT_THREAD_STACKSIZE        (1024*4)
#define TCPIP_THREAD_PRIO               110

#define LWIP_STATS                      1
#define LWIP_COMPAT_MUTEX               1
#define LWIP_STATS_DISPLAY              1

#endif							/* __LWIP_LWIPOPTS_H__ */
