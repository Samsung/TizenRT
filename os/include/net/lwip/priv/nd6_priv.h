/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
/**
 * @file
 *
 * Neighbor discovery and stateless address autoconfiguration for IPv6.
 * Aims to be compliant with RFC 4861 (Neighbor discovery) and RFC 4862
 * (Address autoconfiguration).
 */

/*
 * Copyright (c) 2010 Inico Technologies Ltd.
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
 * Author: Ivan Delamer <delamer@inicotech.com>
 *
 *
 * Please coordinate changes and requests with Ivan Delamer
 * <delamer@inicotech.com>
 */

#ifndef LWIP_HDR_ND6_PRIV_H
#define LWIP_HDR_ND6_PRIV_H

#include <net/lwip/opt.h>

#if LWIP_IPV6					/* don't build if not configured for use in lwipopts.h */

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
#endif							/* LWIP_ND6_QUEUEING */

/** Struct for tables. */
struct nd6_neighbor_cache_entry {
	ip6_addr_t next_hop_address;
	struct netif *netif;
	u8_t lladdr[NETIF_MAX_HWADDR_LEN];
	/*u32_t pmtu; */
#if LWIP_ND6_QUEUEING
	/** Pointer to queue of pending outgoing packets on this entry. */
	struct nd6_q_entry *q;
#else							/* LWIP_ND6_QUEUEING */
	/** Pointer to a single pending outgoing packet on this entry. */
	struct pbuf *q;
#endif							/* LWIP_ND6_QUEUEING */
	u8_t state;
	u8_t isrouter;
	union {
		u32_t incomplete_time;	/* in ms */
		u32_t reachable_time;	/* in ms */
		u32_t delay_time;		/* in ms */
		u32_t probe_time;		/* in ms */
	} counter;
	u16_t probes_sent;			/* probes had been sent */
};

struct nd6_destination_cache_entry {
	ip6_addr_t destination_addr;
	ip6_addr_t next_hop_addr;
	u16_t pmtu;
	s32_t pmtu_timer;
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
#endif							/* LWIP_IPV6_AUTOCONFIG */
};

struct nd6_router_list_entry {
	ip6_addr_t router_ip6addr;
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
#endif							/* LWIP_IPV6 */
#endif							/* LWIP_HDR_ND6_PRIV_H */
