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
#define LWIP_ICMP                       1

/* ---------- IGMP options ---------- */

#define LWIP_IGMP                       1
#define LWIP_RAND()                     rand()

/* ---------- TCP options ---------- */
#define LWIP_TCP                        1
#define TCP_TTL                         255
#define LWIP_TCP_KEEPALIVE              1
#define LWIP_TCPIP_CORE_LOCKING_INPUT   0
#define LWIP_TCPIP_CORE_LOCKING         0

/* TCP Maximum segment size. */
#define TCP_MSS                         (1500 - 40)	/* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF                     (20*TCP_MSS)

/*  TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
  as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work. */

//#define TCP_SND_QUEUELEN                (4*TCP_SND_BUF/TCP_MSS)

/* TCP receive window. */
#define TCP_WND                         (40*TCP_MSS)

/* ---------- UDP options ---------- */
#define LWIP_UDP                        1
#define UDP_TTL                         255
#define LWIP_UDP_TODO                   1

#define LWIP_DHCPS                      0

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

/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT                   4

/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. */
#define MEM_SIZE                (150*1024)

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
#define MEMP_NUM_PBUF          50

/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */
#define MEMP_NUM_UDP_PCB        8

/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */
#define MEMP_NUM_TCP_PCB        8

/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */
#define MEMP_NUM_TCP_PCB_LISTEN 8

/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */
#define MEMP_NUM_TCP_SEG        (2 * TCP_SND_QUEUELEN)

/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#define PBUF_POOL_SIZE          50

/*
* MEMP_MEM_MALLOC==1: Use mem_malloc/mem_free instead of the lwip pool allocator.
* Especially useful with MEM_LIBC_MALLOC but handle with care regarding execution
* speed and usage from interrupts!
*/
#define MEMP_MEM_MALLOC                 1

#define MEMP_OVERFLOW_CHECK             1
#define MEMP_SANITY_CHECK               1

/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#ifdef CONFIG_CUSTOM_SOCKETS
#define LWIP_SOCKET                     1
#else
#define LWIP_SOCKET                     0
#endif

#ifdef CONFIG_DISABLE_POLL
#define LWIP_SELECT                     1
#else
#define LWIP_SELECT                     0
#endif

#define LWIP_POSIX_SOCKETS_IO_NAMES     0
#define LWIP_SOCKET_OFFSET              CONFIG_NFILE_DESCRIPTORS
/**
 * LWIP_SO_RCVTIMEO==1: Enable receive timeout for sockets/netconns and
 * SO_RCVTIMEO processing.
 */
#define LWIP_SO_RCVTIMEO                1
/**
 * SO_REUSE==1: Enable SO_REUSEADDR option.
 */
#define SO_REUSE                        1

#define SOCKLEN_T_DEFINED               1

#define LWIP_TIMEVAL_PRIVATE            0

#define LWIP_HAVE_LOOPIF                1

#define LWIP_RAW                        1

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
