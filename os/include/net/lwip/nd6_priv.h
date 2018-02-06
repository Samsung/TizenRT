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
#ifndef LWIP_HDR_ND6_PRIV_H
#define LWIP_HDR_ND6_PRIV_H

#include <net/lwip/opt.h>

#if LWIP_IPV6  /* don't build if not configured for use in lwipopts.h */

#include <net/lwip/pbuf.h>
#include <net/lwip/ip6_addr.h>
#include <net/lwip/netif.h>


#ifdef __cplusplus
extern "C" {
#endif

#if LWIP_ND6_QUEUEING
/** struct for queueing outgoing packets for unknown address
  * defined here to be accessed by memp.h
  */
struct nd6_q_entry {
	struct nd6_q_entry *next;
	struct pbuf *p;
};
#endif	/* LWIP_ND6_QUEUEING */

/** Struct for tables. */
struct nd6_neighbor_cache_entry {
	ip6_addr_t next_hop_address;
	struct netif *netif;
	u8_t lladdr[NETIF_MAX_HWADDR_LEN];
	/*u32_t pmtu; */
#if LWIP_ND6_QUEUEING
	/** Pointer to queue of pending outgoing packets on this entry. */
	struct nd6_q_entry *q;
#else	/* LWIP_ND6_QUEUEING */
	/** Pointer to a single pending outgoing packet on this entry. */
	struct pbuf *q;
#endif	/* LWIP_ND6_QUEUEING */
	u8_t state;
	u8_t isrouter;
	union {
		u32_t reachable_time;	/* in ms since value may originate from network packet */
		u32_t delay_time;	/* ticks (ND6_TMR_INTERVAL) */
		u32_t probes_sent;
		u32_t stale_time;	/* ticks (ND6_TMR_INTERVAL) */
	} counter;
};

struct nd6_destination_cache_entry {
	ip6_addr_t destination_addr;
	ip6_addr_t next_hop_addr;
	u16_t pmtu;
	u32_t age;
};

struct nd6_prefix_list_entry {
	ip6_addr_t prefix;
	struct netif *netif;
	u32_t invalidation_timer;	/* in ms since value may originate from network packet */
#if LWIP_IPV6_AUTOCONFIG
	u8_t flags;
#define ND6_PREFIX_AUTOCONFIG_AUTONOMOUS 0x01
#define ND6_PREFIX_AUTOCONFIG_ADDRESS_GENERATED 0x02
#define ND6_PREFIX_AUTOCONFIG_ADDRESS_DUPLICATE 0x04
#endif	/* LWIP_IPV6_AUTOCONFIG */
};


struct nd6_router_list_entry {
	struct nd6_neighbor_cache_entry *neighbor_entry;
	u32_t invalidation_timer;	/* in ms since value may originate from network packet */
	u8_t flags;
};


enum nd6_neighbor_cache_entry_state {
	ND6_NO_ENTRY = 0,
	ND6_INCOMPLETE,
	ND6_REACHABLE,
	ND6_STALE,
	ND6_DELAY,
	ND6_PROBE
};


/* Router tables. */
/* @todo make these static? and entries accessible through API? */
extern struct nd6_neighbor_cache_entry neighbor_cache[];
extern struct nd6_destination_cache_entry destination_cache[];
extern struct nd6_prefix_list_entry prefix_list[];
extern struct nd6_router_list_entry default_router_list[];

/* Default values, can be updated by a RA message. */
extern u32_t reachable_time;
extern u32_t retrans_timer;

#ifdef __cplusplus
}
#endif

#endif	/* LWIP_IPV6 */

#endif	/* LWIP_HDR_ND6_PRIV_H */
