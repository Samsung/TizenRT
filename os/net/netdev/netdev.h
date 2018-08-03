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

/****************************************************************************
 * net/netdev/netdev.h
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __NET_NETDEV_NETDEV_H
#define __NET_NETDEV_NETDEV_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>

#include <tinyara/net/ip.h>

#include <net/lwip/netif.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* netdev_register.c *********************************************************/

#if CONFIG_NSOCKET_DESCRIPTORS > 0
void netdev_seminit(void);
void netdev_semtake(void);
void netdev_semgive(void);
#endif

/* netdev_ioctl.c ************************************************************/

void netdev_ifup(FAR struct netif *dev);
void netdev_ifdown(FAR struct netif *dev);

/* netdev_findbyname.c *******************************************************/

#if CONFIG_NSOCKET_DESCRIPTORS > 0

FAR struct netif *netdev_findbyname(FAR const char *ifname);

#endif

/* netdev_default.c ***********************************************************/

#if CONFIG_NSOCKET_DESCRIPTORS > 0
FAR struct netif *netdev_default(void);
#endif

/* netdev_txnotify.c *********************************************************/

#if CONFIG_NSOCKET_DESCRIPTORS > 0
#ifdef CONFIG_NET_IPv4
#ifdef CONFIG_NETDEV_MULTINIC
void netdev_ipv4_txnotify(in_addr_t lipaddr, in_addr_t ripaddr);
#else
void netdev_ipv4_txnotify(in_addr_t ripaddr);
#endif
#endif							/* CONFIG_NET_IPv4 */

#ifdef CONFIG_NET_IPv6
#ifdef CONFIG_NETDEV_MULTINIC
void netdev_ipv6_txnotify(FAR const net_ipv6addr_t lipaddr, FAR const net_ipv6addr_t ripaddr);
#else
void netdev_ipv6_txnotify(FAR const net_ipv6addr_t ripaddr);
#endif
#endif							/* CONFIG_NET_IPv6 */
#endif							/* CONFIG_NSOCKET_DESCRIPTORS > 0 */

/* netdev_rxnotify.c *********************************************************/

#if CONFIG_NSOCKET_DESCRIPTORS > 0 && defined(CONFIG_NET_RXAVAIL)

#ifdef CONFIG_NET_IPv4
#ifdef CONFIG_NETDEV_MULTINIC
void netdev_ipv4_rxnotify(in_addr_t lipaddr, in_addr_t ripaddr);
#else
void netdev_ipv4_rxnotify(in_addr_t ripaddr);
#endif
#endif							/* CONFIG_NET_IPv4 */

#ifdef CONFIG_NET_IPv6
#ifdef CONFIG_NETDEV_MULTINIC
void netdev_ipv6_rxnotify(FAR const net_ipv6addr_t lipaddr, FAR const net_ipv6addr_t ripaddr);
#else
void netdev_ipv6_rxnotify(FAR const net_ipv6addr_t ripaddr);
#endif
#endif							/* CONFIG_NET_IPv6 */

#else
#ifdef CONFIG_NET_IPv4
#ifdef CONFIG_NETDEV_MULTINIC
#define netdev_ipv4_rxnotify(lipaddr, ripaddr)
#else
#define netdev_ipv4_rxnotify(ripaddr)
#endif
#endif							/* CONFIG_NET_IPv4 */

#ifdef CONFIG_NET_IPv6
#ifdef CONFIG_NETDEV_MULTINIC
#define netdev_ipv6_rxnotify(lipaddr, ripaddr)
#else
#define netdev_ipv6_rxnotify(ripaddr)
#endif
#endif							/* CONFIG_NET_IPv6 */
#endif

/* netdev_count.c ************************************************************/

#if CONFIG_NSOCKET_DESCRIPTORS > 0
int netdev_count(void);
#endif

/* netdev_getstats.c *******************************************************/

#if CONFIG_NSOCKET_DESCRIPTORS > 0 && defined(CONFIG_NET_STATS)
int netdev_getstats(void *arg);
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __NET_NETDEV_NETDEV_H */
