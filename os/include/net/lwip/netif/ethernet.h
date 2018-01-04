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
#ifndef LWIP_HDR_NETIF_ETHERNET_H
#define LWIP_HDR_NETIF_ETHERNET_H

#include <net/lwip/opt.h>

#include <net/lwip/pbuf.h>
#include <net/lwip/netif.h>
#include <net/lwip/prot/ethernet.h>

#ifdef __cplusplus
extern "C" {
#endif

#if LWIP_ARP || LWIP_ETHERNET

/** Define this to 1 and define LWIP_ARP_FILTER_NETIF_FN(pbuf, netif, type)
 * to a filter function that returns the correct netif when using multiple
 * netifs on one hardware interface where the netif's low-level receive
 * routine cannot decide for the correct netif (e.g. when mapping multiple
 * IP addresses to one hardware interface).
 */
#ifndef LWIP_ARP_FILTER_NETIF
#define LWIP_ARP_FILTER_NETIF 0
#endif

err_t ethernet_input(struct pbuf *p, struct netif *netif);
err_t ethernet_output(struct netif *netif, struct pbuf *p, const struct eth_addr *src, const struct eth_addr *dst, u16_t eth_type);

extern const struct eth_addr ethbroadcast;
extern const struct eth_addr ethzero;

#endif	/* LWIP_ARP || LWIP_ETHERNET */

#ifdef __cplusplus
}
#endif

#endif	/* LWIP_HDR_NETIF_ETHERNET_H */
