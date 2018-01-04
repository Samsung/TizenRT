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
#ifndef LWIP_HDR_ND6_H
#define LWIP_HDR_ND6_H

#include <net/lwip/opt.h>

#if LWIP_IPV6  /* don't build if not configured for use in lwipopts.h */

#include <net/lwip/ip6_addr.h>
#include <net/lwip/err.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 1 second period */
#define ND6_TMR_INTERVAL 1000

struct pbuf;
struct netif;

void nd6_tmr(void);
void nd6_input(struct pbuf *p, struct netif *inp);
void nd6_clear_destination_cache(void);
struct netif *nd6_find_route(const ip6_addr_t *ip6addr);
err_t nd6_get_next_hop_addr_or_queue(struct netif *netif, struct pbuf *q, const ip6_addr_t *ip6addr, const u8_t **hwaddrp);
u16_t nd6_get_destination_mtu(const ip6_addr_t *ip6addr, struct netif *netif);
#if LWIP_ND6_TCP_REACHABILITY_HINTS
void nd6_reachability_hint(const ip6_addr_t *ip6addr);
#endif	/* LWIP_ND6_TCP_REACHABILITY_HINTS */
void nd6_cleanup_netif(struct netif *netif);
#if LWIP_IPV6_MLD
void nd6_adjust_mld_membership(struct netif *netif, s8_t addr_idx, u8_t new_state);
#endif	/* LWIP_IPV6_MLD */

#ifdef __cplusplus
}
#endif

#endif	/* LWIP_IPV6 */

#endif	/* LWIP_HDR_ND6_H */
