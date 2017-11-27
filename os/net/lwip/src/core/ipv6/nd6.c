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

#include <net/lwip/opt.h>

#if LWIP_IPV6					/* don't build if not configured for use in lwipopts.h */

#include <net/lwip/nd6.h>
#include <net/lwip/priv/nd6_priv.h>
#include <net/lwip/prot/nd6.h>
#include <net/lwip/prot/icmp6.h>
#include <net/lwip/pbuf.h>
#include <net/lwip/mem.h>
#include <net/lwip/memp.h>
#include <net/lwip/ip6.h>
#include <net/lwip/ip6_addr.h>
#include <net/lwip/inet_chksum.h>
#include <net/lwip/netif.h>
#include <net/lwip/icmp6.h>
#include <net/lwip/mld6.h>
#include <net/lwip/ip.h>
#include <net/lwip/stats.h>
#include <net/lwip/dns.h>

#include <string.h>

#ifdef LWIP_HOOK_FILENAME
#include LWIP_HOOK_FILENAME
#endif

#if LWIP_IPV6_DUP_DETECT_ATTEMPTS > IP6_ADDR_TENTATIVE_COUNT_MASK
#error LWIP_IPV6_DUP_DETECT_ATTEMPTS > IP6_ADDR_TENTATIVE_COUNT_MASK
#endif

#define TIME_HOUR_TO_MS(hour)    (hour * 3600 * 1000)
#define TIME_SEC_TO_MS(sec)      (sec * 1000)

/* Router tables. */
struct nd6_neighbor_cache_entry neighbor_cache[LWIP_ND6_NUM_NEIGHBORS];
struct nd6_destination_cache_entry destination_cache[LWIP_ND6_NUM_DESTINATIONS];
struct nd6_prefix_list_entry prefix_list[LWIP_ND6_NUM_PREFIXES];
struct nd6_router_list_entry default_router_list[LWIP_ND6_NUM_ROUTERS];

/* Default values, can be updated by a RA message. */
u8_t current_hop_limit = LWIP_ICMP6_HL;
u32_t reachable_time = LWIP_ND6_REACHABLE_TIME;
u32_t retrans_timer = LWIP_ND6_RETRANS_TIMER;	/* @todo implement this value in timer */

/* Index for cache entries. */
static u8_t nd6_cached_neighbor_index;
static u8_t nd6_cached_destination_index;

/* Multicast address holder. */
static ip6_addr_t multicast_address;

/* Static buffer to parse RA packet options (size of a prefix option, biggest option) */
static u8_t nd6_ra_buffer[sizeof(struct prefix_option)];

/* Forward declarations. */
static s8_t nd6_find_neighbor_cache_entry(const ip6_addr_t *ip6addr);
static s8_t nd6_find_lladdr_neighbor_cache_entry(const u8_t *lladdr, const struct netif *inp);
static s8_t nd6_new_neighbor_cache_entry(void);
static void nd6_free_neighbor_cache_entry(s8_t i);
static s8_t nd6_find_destination_cache_entry(const ip6_addr_t *ip6addr);
static s8_t nd6_new_destination_cache_entry(void);
static void nd6_free_expired_router_in_destination_cache(const ip6_addr_t *ip6addr);
static s8_t nd6_is_prefix_in_netif(const ip6_addr_t *ip6addr, struct netif *netif);
static s8_t nd6_select_router(const ip6_addr_t *ip6addr, struct netif *netif);
static s8_t nd6_select_unreachable_default_router(void);
static s8_t nd6_get_router(const ip6_addr_t *router_addr, struct netif *netif);
static s8_t nd6_new_router(const ip6_addr_t *router_addr, struct netif *netif);
static s8_t nd6_get_onlink_prefix(ip6_addr_t *prefix, struct netif *netif);
static s8_t nd6_new_onlink_prefix(ip6_addr_t *prefix, struct netif *netif);
static s8_t nd6_get_next_hop_entry(const ip6_addr_t *ip6addr, struct netif *netif);
static err_t nd6_queue_packet(s8_t neighbor_index, struct pbuf *q);
#define ND6_SEND_FLAG_MULTICAST_DEST 0x01
#define ND6_SEND_FLAG_ALLNODES_DEST 0x02
#define ND6_SEND_FLAG_ADDRANY_SRC 0x04
static void nd6_send_ns(struct netif *netif, const ip6_addr_t *target_addr, u8_t flags);
static void nd6_send_na(struct netif *netif, const ip6_addr_t *target_addr, u8_t flags);
static void nd6_send_neighbor_cache_probe(struct nd6_neighbor_cache_entry *entry, u8_t flags);
#if LWIP_IPV6_SEND_ROUTER_SOLICIT
static err_t nd6_send_rs(struct netif *netif);
#endif							/* LWIP_IPV6_SEND_ROUTER_SOLICIT */

#if LWIP_ND6_QUEUEING
static void nd6_free_q(struct nd6_q_entry *q);
#else							/* LWIP_ND6_QUEUEING */
#define nd6_free_q(q) pbuf_free(q)
#endif							/* LWIP_ND6_QUEUEING */
static void nd6_send_q(s8_t i);
static void nd6_cleanup_all_caches(ip6_addr_t *invalid_ip6addr);
static u8_t nd6_prefix_lifetime_isvalid(const struct nd6_prefix_list_entry *prefix_entry, u32_t lifetime_ms);
#if ND6_DEBUG
void nd6_cache_debug_print(void);
#endif

/**
 * Process an incoming neighbor discovery message
 *
 * @param p the nd packet, p->payload pointing to the icmpv6 header
 * @param inp the netif on which this packet was received
 */
void nd6_input(struct pbuf *p, struct netif *inp)
{
	u8_t msg_type;
	s8_t i;

	ND6_STATS_INC(nd6.recv);

	msg_type = *((u8_t *) p->payload);

	LWIP_DEBUGF(ND6_DEBUG, ("Received msg_type : %u\n", msg_type));

	switch (msg_type) {
	case ICMP6_TYPE_NA: {		/* Neighbor Advertisement. */
		s32_t nd6_oflag;
		s32_t nd6_sflag;
		s32_t nd6_rflag;
		s32_t lladdr_diff;
		struct na_header *na_hdr;
		struct lladdr_option *lladdr_opt;
		ip6_addr_t target_address;

		/* Check that na header fits in packet. */
		if (p->len < (sizeof(struct na_header))) {
			/* @todo debug message */
			pbuf_free(p);
			ND6_STATS_INC(nd6.lenerr);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		na_hdr = (struct na_header *)p->payload;

		if ((IP6H_HOPLIM(ip6_current_header()) != 255) ||
		    (ip6_addr_ismulticast(&target_address)) ||
		    (ND6H_CODE(na_hdr) != 0)) {
			/* RFC 4861 clause 7.1.2.
			 * Validation of NA message */
			pbuf_free(p);
			ND6_STATS_INC(nd6.proterr);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		nd6_oflag = ND6H_NA_FLAG(na_hdr) & ND6_FLAG_OVERRIDE;
		nd6_sflag = ND6H_NA_FLAG(na_hdr) & ND6_FLAG_SOLICITED;
		nd6_rflag = ND6H_NA_FLAG(na_hdr) & ND6_FLAG_ROUTER;
		ip6_addr_set(&target_address, &(ND6H_NA_TARGET_ADDR(na_hdr)));

		lladdr_opt = NULL;
		if (p->len >= (sizeof(struct na_header) + 8)) {
			u32_t offset = sizeof(struct na_header);

			while (p->len >= offset + 8) {
				lladdr_opt = (struct lladdr_option *)((u8_t *) p->payload + offset);
				offset += (ND6H_LLADDR_OPT_LEN(lladdr_opt) << 3);

				if (ND6H_LLADDR_OPT_LEN(lladdr_opt) == 0) {
					pbuf_free(p);
					ND6_STATS_INC(nd6.lenerr);
					ND6_STATS_INC(nd6.drop);
					return;
				}

				if (ND6H_LLADDR_OPT_TYPE(lladdr_opt) != ND6_OPTION_TYPE_TARGET_LLADDR) {
					/* RFC 4861 clause 4.4.
					 * only target lladdr option is possible option for NA message */
					lladdr_opt = NULL;
				}
			}
		}

		/* Detect the duplicated NA message */
		if (ip6_addr_ismulticast(ip6_current_dest_addr())) {
			if (nd6_sflag) {
				/* RFC 7.1.2.
				 * S flag must be clear */
				pbuf_free(p);
				return;
			}

#if LWIP_IPV6_DUP_DETECT_ATTEMPTS
			/* If the target address matches this netif, it is a DAD response. */
			for (i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
				if (!ip6_addr_isinvalid(netif_ip6_addr_state(inp, i)) && ip6_addr_cmp(&target_address, netif_ip6_addr(inp, i))) {
					/* We are using a duplicate address. */
					netif_ip6_addr_set_state(inp, i, IP6_ADDR_INVALID);

#if LWIP_IPV6_AUTOCONFIG
					/* Check to see if this address was autoconfigured. */
					if (!ip6_addr_islinklocal(&target_address)) {
						i = nd6_get_onlink_prefix(&target_address, inp);
						if (i >= 0) {
							/* Mark this prefix as duplicate, so that we don't use it
							 * to generate this address again. */
							prefix_list[i].flags |= ND6_PREFIX_AUTOCONFIG_ADDRESS_DUPLICATE;
						}
					}
#endif							/* LWIP_IPV6_AUTOCONFIG */

					pbuf_free(p);

#ifdef CONFIG_NET_IPv6_AUTOCONFIG
					/* disable IPv6 address stateless autoconfiguration */
					netif_set_ip6_autoconfig_enabled(inp, 0);
#endif
					/* RFC 4862. 5.4.5
					 * If DAD fails, IP operation SHOULD be disabled when the address is derived from hardware address.
					 */
					netif_set_down(inp);

					return;
				}
			}
#endif							/* LWIP_IPV6_DUP_DETECT_ATTEMPTS */
		}

		i = nd6_find_neighbor_cache_entry(&target_address);
		if (i < 0) {
			/* RFC 7.2.5.
			 * When a valid Neighbor Advertisement is received
			 * (either solicited or unsolicited), the Neighbor Cache is
			 * searched for the target's entry. If no entry exists,
			 * the advertisement SHOULD be silently discarded. */
			pbuf_free(p);
			return;
		}

		neighbor_cache[i].netif = inp;

		lladdr_diff = 0;
		if (lladdr_opt) {
			lladdr_diff = MEMCMP(neighbor_cache[i].lladdr, ND6H_LLADDR_OPT_ADDR(lladdr_opt), inp->hwaddr_len);
		}

		if ((neighbor_cache[i].state == ND6_INCOMPLETE)) {
			/* Check that link-layer address option also fits in packet. */
			if (lladdr_opt == NULL) {
				pbuf_free(p);
				ND6_STATS_INC(nd6.proterr);
				ND6_STATS_INC(nd6.drop);
				return;
			}

			MEMCPY(neighbor_cache[i].lladdr, ND6H_LLADDR_OPT_ADDR(lladdr_opt), inp->hwaddr_len);

			if (nd6_sflag) {
				neighbor_cache[i].state = ND6_REACHABLE;
				neighbor_cache[i].counter.reachable_time = reachable_time;
			} else {
				neighbor_cache[i].state = ND6_STALE;
			}

			if (nd6_rflag) {
				s8_t j;

				j = nd6_get_router(&target_address, inp);

				if (j < 0) {
					/* This new neighbor should be added as router */
					j = nd6_new_router(&target_address, inp);
					if (j >= 0) {
						default_router_list[j].neighbor_entry = &(neighbor_cache[i]);
						default_router_list[j].invalidation_timer = LWIP_ND6_DEFAULT_ROUTER_VALID_LIFETIME;
						default_router_list[j].flags = 0;
						ip6_addr_copy(default_router_list[j].router_ip6addr, neighbor_cache[i].next_hop_address);
					} else {
						pbuf_free(p);
						ND6_STATS_INC(nd6.memerr);
						ND6_STATS_INC(nd6.drop);
						return;
					}
				} else {
					/* @todo need to update neighbor_entry on default router list */
				}
				neighbor_cache[i].isrouter = 1;
			} else {
				/* Router flag has been disabled */
				neighbor_cache[i].isrouter = 0;
			}

			if (neighbor_cache[i].q != NULL) {
				nd6_send_q(i);
			}
		} else {
			if (nd6_oflag == 0 && lladdr_diff != 0) {
				/* O flag is clear and lladdr is different between TLLA and cache entry */
				if (neighbor_cache[i].state == ND6_REACHABLE) {
					/* State is REACHABLE */
					neighbor_cache[i].state = ND6_STALE;
				}
			} else {
				if (lladdr_diff != 0) {
					MEMCPY(neighbor_cache[i].lladdr, ND6H_LLADDR_OPT_ADDR(lladdr_opt), inp->hwaddr_len);
					neighbor_cache[i].state = ND6_STALE;
				}

				if (nd6_sflag) {
					neighbor_cache[i].state = ND6_REACHABLE;
					neighbor_cache[i].counter.reachable_time = reachable_time;
				}

				if (nd6_rflag) {
					neighbor_cache[i].isrouter = 1;
				} else {
					/* RFC 7.2.5
					 * Node MUST remove that router from the Default Router List
					 * and update Destination Cache entries for all destinations using that
					 * neighbor router as specified in Section 7.3.3 */
					if (neighbor_cache[i].isrouter) {
						s8_t tmp;

						neighbor_cache[i].isrouter = 0;
						tmp = nd6_get_router(&neighbor_cache[i].next_hop_address, inp);

						if (tmp >= 0) {
							/* RFC 4861, 6.3.5.  Timing out Prefixes and Default Routers  */
							if (default_router_list[tmp].neighbor_entry) {
								nd6_free_expired_router_in_destination_cache(&(default_router_list[tmp].neighbor_entry->next_hop_address));
							}
							default_router_list[tmp].neighbor_entry = NULL;
							default_router_list[tmp].invalidation_timer = 0;
							default_router_list[tmp].flags = 0;
							ip6_addr_set_any(&default_router_list[tmp].router_ip6addr);
						}
					}
				}
			}
		}

		break;				/* ICMP6_TYPE_NA */
	}
	case ICMP6_TYPE_NS: {		/* Neighbor solicitation. */
		struct ns_header *ns_hdr;
		struct lladdr_option *lladdr_opt = NULL;
		u8_t accepted;

		/* Check that ns header fits in packet. */
		if (p->len < sizeof(struct ns_header)) {
			/* @todo debug message */
			pbuf_free(p);
			ND6_STATS_INC(nd6.lenerr);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		ns_hdr = (struct ns_header *)p->payload;

		if ((IP6H_HOPLIM(ip6_current_header()) != 255) ||
		    (ND6H_CODE(ns_hdr) != 0) ||
		    ip6_addr_ismulticast(&ND6H_NS_TARGET_ADDR(ns_hdr))) {
			/* RFC 4861 clause 7.1.1.
			 * Validation of Ns message */
			pbuf_free(p);
			ND6_STATS_INC(nd6.proterr);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		/* Check if there is a link-layer address provided. Only point to it if in this buffer. */
		if (p->len >= (sizeof(struct ns_header) + 8)) {
			u32_t offset = sizeof(struct ns_header);

			while (p->len >= offset + 8) {
				lladdr_opt = (struct lladdr_option *)((u8_t *) p->payload + offset);
				offset += (ND6H_LLADDR_OPT_LEN(lladdr_opt) << 3);

				if (ND6H_LLADDR_OPT_LEN(lladdr_opt) == 0) {
					pbuf_free(p);
					ND6_STATS_INC(nd6.lenerr);
					ND6_STATS_INC(nd6.drop);
					return;
				}

				if (ND6H_LLADDR_OPT_TYPE(lladdr_opt) != ND6_OPTION_TYPE_SOURCE_LLADDR) {
					/* RFC 4861 clause 4.3
					 * only source lladdr option is possible option for NS message */
					lladdr_opt = NULL;
				}
			}
		}

		/* Check if the target address is configured on the receiving netif. */
		accepted = 0;
		for (i = 0; i < LWIP_IPV6_NUM_ADDRESSES; ++i) {
			if ((ip6_addr_isvalid(netif_ip6_addr_state(inp, i)) || (ip6_addr_istentative(netif_ip6_addr_state(inp, i)) && ip6_addr_isany(ip6_current_src_addr()))) && ip6_addr_cmp(&(ND6H_NS_TARGET_ADDR(ns_hdr)), netif_ip6_addr(inp, i))) {
				accepted = 1;
				break;
			}
		}

		/* NS not for us? */
		if (!accepted) {
			pbuf_free(p);
			return;
		}

		/* Check for ANY address in src (DAD algorithm). */
		if (ip6_addr_isany(ip6_current_src_addr())) {
			if (lladdr_opt != NULL) {
				/* RFC 7.1.1.
				 * If the IP source address is the unspecified address, there is no
				 * source link-layer address option in the message. */
				pbuf_free(p);
				ND6_STATS_INC(nd6.proterr);
				ND6_STATS_INC(nd6.drop);
				return;
			}

			if (ip6_addr_issolicitednode(ip6_current_dest_addr())) {
				/* Sender is validating this address. */
				for (i = 0; i < LWIP_IPV6_NUM_ADDRESSES; ++i) {
					if (!ip6_addr_isinvalid(netif_ip6_addr_state(inp, i)) && ip6_addr_cmp(&(ND6H_NS_TARGET_ADDR(ns_hdr)), netif_ip6_addr(inp, i))) {
						/* Send a NA back so that the sender does not use this address. */
						nd6_send_na(inp, netif_ip6_addr(inp, i), ND6_FLAG_OVERRIDE | ND6_SEND_FLAG_ALLNODES_DEST);
						if (ip6_addr_istentative(netif_ip6_addr_state(inp, i))) {
							/* We shouldn't use this address either. */
							netif_ip6_addr_set_state(inp, i, IP6_ADDR_INVALID);
#ifdef CONFIG_NET_IPv6_AUTOCONFIG
							/* disable IPv6 address stateless autoconfiguration */
							netif_set_ip6_autoconfig_enabled(inp, 0);
#endif
							/* RFC 4862. 5.4.5
							 * If DAD fails, IP operation SHOULD be disabled when the address is derived from hardware address.
							 */
							netif_set_down(inp);
						}
					}
				}
			} else {
				/* RFC 7.1.1.
				 * If the IP source address is the unspecified address, the IP
				 * destination address is a solicited-node multicast address. */
				pbuf_free(p);
				ND6_STATS_INC(nd6.proterr);
				ND6_STATS_INC(nd6.drop);
				return;
			}
		} else {
			ip6_addr_t target_address;

			/* RFC 7.2.4.
			 * Unicast NSs are not required to include a source lladdr option */
			if (ip6_addr_ismulticast(ip6_current_dest_addr()) && (lladdr_opt == NULL)) {
				pbuf_free(p);
				ND6_STATS_INC(nd6.lenerr);
				ND6_STATS_INC(nd6.drop);
				return;
			}

			i = nd6_find_neighbor_cache_entry(ip6_current_src_addr());
			if (i >= 0) {
				/* RFC 7.2.3.
				 * If an entry already exists, and the cached link-layer address
				 * differs from the one in the received Source Link-Layer option,
				 * the cached address should be replaced by the received address,
				 * and the entry's reachability state MUST be set to STALE. */
				if (lladdr_opt) {
					if (MEMCMP(neighbor_cache[i].lladdr, ND6H_LLADDR_OPT_ADDR(lladdr_opt), inp->hwaddr_len) != 0) {
						MEMCPY(neighbor_cache[i].lladdr, ND6H_LLADDR_OPT_ADDR(lladdr_opt), inp->hwaddr_len);

					/* RFC 4861 page 91, appendix c */
						if ((neighbor_cache[i].state == ND6_INCOMPLETE) && neighbor_cache[i].q != NULL) {
							neighbor_cache[i].state = ND6_STALE;
							nd6_send_q(i);
						} else {
							neighbor_cache[i].state = ND6_STALE;
						}
					}
				}
			} else {
				/* Add their IPv6 address and link-layer address to neighbor cache.
				 * We will need it at least to send a unicast NA message, but most
				 * likely we will also be communicating with this node soon. */
				i = nd6_new_neighbor_cache_entry();
				if (i < 0) {
					/* We couldn't assign a cache entry for this neighbor.
					 * we won't be able to reply. drop it. */
					pbuf_free(p);
					ND6_STATS_INC(nd6.memerr);
					return;
				}

				neighbor_cache[i].netif = inp;

				if (lladdr_opt) {
					MEMCPY(neighbor_cache[i].lladdr, ND6H_LLADDR_OPT_ADDR(lladdr_opt), inp->hwaddr_len);
					ip6_addr_set(&(neighbor_cache[i].next_hop_address), ip6_current_src_addr());
					neighbor_cache[i].state = ND6_STALE;
				} else {
					ip6_addr_set(&(neighbor_cache[i].next_hop_address), ip6_current_src_addr());
					neighbor_cache[i].state = ND6_INCOMPLETE;
				}
			}

			/* Create an aligned copy. */
			ip6_addr_set(&target_address, &(ND6H_NS_TARGET_ADDR(ns_hdr)));

			/* Send back a NA for us. Allocate the reply pbuf. */
			nd6_send_na(inp, &target_address, ND6_FLAG_SOLICITED | ND6_FLAG_OVERRIDE);
		}

		break;				/* ICMP6_TYPE_NS */
	}
	case ICMP6_TYPE_RA: {		/* Router Advertisement. */
		struct ra_header *ra_hdr;
		struct lladdr_option *lladdr_opt = NULL;
		u8_t nce_flag = 0;

		u8_t *buffer;		/* Used to copy options. */
		u16_t offset;
#if LWIP_ND6_RDNSS_MAX_DNS_SERVERS
		/* There can by multiple RDNSS options per RA */
		u8_t rdnss_server_idx = 0;
#endif							/* LWIP_ND6_RDNSS_MAX_DNS_SERVERS */

		/* Check that RA header fits in packet. */
		if (p->len < sizeof(struct ra_header)) {
			/* @todo debug message */
			pbuf_free(p);
			ND6_STATS_INC(nd6.lenerr);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		if (!ip6_addr_islinklocal(ip6_current_src_addr())) {
			pbuf_free(p);
			ND6_STATS_INC(nd6.proterr);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		if (IP6H_HOPLIM(ip6_current_header()) != 255) {
			pbuf_free(p);
			ND6_STATS_INC(nd6.proterr);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		ra_hdr = (struct ra_header *)p->payload;
		if (ND6H_CODE(ra_hdr) != 0) {
			pbuf_free(p);
			ND6_STATS_INC(nd6.proterr);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		/* If we are sending RS messages, stop. */
#if LWIP_IPV6_SEND_ROUTER_SOLICIT
		/* ensure at least one solicitation is sent */
		if (inp->rs_count == LWIP_ND6_MAX_MULTICAST_SOLICIT) {
			nd6_send_rs(inp);
		}
		inp->rs_count = 0;
#endif							/* LWIP_IPV6_SEND_ROUTER_SOLICIT */

		/* Offset to options. */
		offset = sizeof(struct ra_header);

		/* Process each option. */
		while ((p->tot_len - offset) > 0) {
			if (p->len == p->tot_len) {
				/* no need to copy from contiguous pbuf */
				buffer = &((u8_t *) p->payload)[offset];
			} else {
				buffer = nd6_ra_buffer;
				if (pbuf_copy_partial(p, buffer, sizeof(struct prefix_option), offset) != sizeof(struct prefix_option)) {
					pbuf_free(p);
					ND6_STATS_INC(nd6.lenerr);
					ND6_STATS_INC(nd6.drop);
					return;
				}
			}
			if (buffer[1] == 0) {
				/* zero-length extension. drop packet */
				pbuf_free(p);
				ND6_STATS_INC(nd6.lenerr);
				ND6_STATS_INC(nd6.drop);
				return;
			}
			switch (buffer[0]) {
			case ND6_OPTION_TYPE_SOURCE_LLADDR: {
				if (nce_flag) {
					/* TODO: We should consider when we received SADDR option two times */
					LWIP_DEBUGF(ND6_DEBUG, ("Duplicated SLLADDR OPTION\n"));
					break;
				}
				lladdr_opt = (struct lladdr_option *)buffer;
				nce_flag = 1;
				break;
			}
			case ND6_OPTION_TYPE_MTU: {
				struct mtu_option *mtu_opt;
				mtu_opt = (struct mtu_option *)buffer;
				if (lwip_htonl(ND6H_MTU_OPT_MTU(mtu_opt)) >= 1280) {
#if LWIP_ND6_ALLOW_RA_UPDATES
					inp->mtu = (u16_t) lwip_htonl(ND6H_MTU_OPT_MTU(mtu_opt));
					for (i = 0; i < LWIP_ND6_NUM_DESTINATIONS; i++) {
						if (! ip6_addr_isany(&(destination_cache[i].destination_addr))) {
							if ((inp->mtu <= destination_cache[i].pmtu) || (destination_cache[i].pmtu_timer <= 0)) {
								destination_cache[i].pmtu = inp->mtu;
								destination_cache[i].pmtu_timer = 10 * 60 * 1000; /* 10 minutes (= 600000 ms) */
							}
						}
					}
#endif							/* LWIP_ND6_ALLOW_RA_UPDATES */
				}
				break;
			}
			case ND6_OPTION_TYPE_PREFIX_INFO: {
				struct prefix_option *prefix_opt;
				prefix_opt = (struct prefix_option *)buffer;

				/* RFC 4861. 4.6.2
				 * Preferred Lifetime:
				 *   The value MUST NOT exceed the Valid Lifetime field to avoid preferring address that are no longer valid.
				 */
				if (lwip_htonl(ND6H_PF_OPT_VAL_LIFE(prefix_opt)) < lwip_htonl(ND6H_PF_OPT_PREFER_LIFE(prefix_opt))) {
					pbuf_free(p);
					ND6_STATS_INC(nd6.proterr);
					return;
				}

				if ((ND6H_PF_OPT_FLAG(prefix_opt) & ND6_PREFIX_FLAG_ON_LINK) && (ND6H_PF_OPT_PF_LEN(prefix_opt) <= 128) && !ip6_addr_islinklocal(&(ND6H_PF_OPT_PF(prefix_opt)))) {
					/* Add to on-link prefix list. */
					s8_t prefix;
					ip6_addr_t prefix_addr;
					u32_t lifetime_ms = TIME_SEC_TO_MS(lwip_htonl(ND6H_PF_OPT_VAL_LIFE(prefix_opt)));

					/* Get a memory-aligned copy of the prefix. */
					ip6_addr_set(&prefix_addr, &(ND6H_PF_OPT_PF(prefix_opt)));

					/* find cache entry for this prefix. */
					prefix = nd6_get_onlink_prefix(&prefix_addr, inp);
					if (prefix < 0) {
						/* Create a new cache entry. */
						prefix = nd6_new_onlink_prefix(&prefix_addr, inp);
						prefix_list[prefix].invalidation_timer = lifetime_ms;
					}
					if (prefix >= 0) {
						/* Found matched prefix entry or Create a new prefix entry */
						if (nd6_prefix_lifetime_isvalid(&prefix_list[prefix], lifetime_ms)) {
							prefix_list[prefix].invalidation_timer = lifetime_ms;
						} else {
							if (prefix_list[prefix].invalidation_timer > TIME_HOUR_TO_MS(2)) {
								/* Otherwise, reset the valid lifetime of the corresponding address to 2 hours. */
								LWIP_DEBUGF(ND6_DEBUG, ("prefix lifetime reset to 2 hours (current invalidation_timer %lu ms, lifetime %lu ms)\n", prefix_list[prefix].invalidation_timer, lifetime_ms));
								prefix_list[prefix].invalidation_timer = TIME_HOUR_TO_MS(2);
							}
						}

#if LWIP_IPV6_AUTOCONFIG
						if (ND6H_PF_OPT_PF_LEN(prefix_opt) == 64) {
							if (ND6H_PF_OPT_FLAG(prefix_opt) & ND6_PREFIX_FLAG_AUTONOMOUS) {
								/* Mark prefix as autonomous, so that address autoconfiguration can take place.
								 * Only OR flag, so that we don't over-write other flags (such as ADDRESS_DUPLICATE)*/
								LWIP_DEBUGF(ND6_DEBUG, ("created on-link prefix (for stateless-autoconfig)\n"));
								prefix_list[prefix].flags |= ND6_PREFIX_AUTOCONFIG_AUTONOMOUS;
							}
						} else
#endif	/* LWIP_IPV6_AUTOCONFIG */
						{
							/* @todo just add onlink prefix? */
							LWIP_DEBUGF(ND6_DEBUG, ("created on-link prefix (not for stateless-autoconfig)\n"));
							prefix_list[prefix].flags = 0;

						}
					}
				}
				break;
			}
			case ND6_OPTION_TYPE_ROUTE_INFO:
				/* @todo implement preferred routes.
				   struct route_option * route_opt;
				   route_opt = (struct route_option *)buffer; */

				break;
#if LWIP_ND6_RDNSS_MAX_DNS_SERVERS
			case ND6_OPTION_TYPE_RDNSS: {
				u8_t num, n;
				struct rdnss_option *rdnss_opt;

				rdnss_opt = (struct rdnss_option *)buffer;
				num = (rdnss_opt->length - 1) / 2;
				for (n = 0; (rdnss_server_idx < DNS_MAX_SERVERS) && (n < num); n++) {
					ip_addr_t rdnss_address;

					/* Get a memory-aligned copy of the prefix. */
					ip_addr_copy_from_ip6(rdnss_address, ND6H_RDNSS_OPT_ADDR(rdnss_opt)[n]);

					if (htonl(rdnss_opt->lifetime) > 0) {
						/* TODO implement Lifetime > 0 */
						dns_setserver(rdnss_server_idx++, &rdnss_address);
					} else {
						/* TODO implement DNS removal in dns.c */
						u8_t s;
						for (s = 0; s < DNS_MAX_SERVERS; s++) {
							const ip_addr_t *addr = dns_getserver(s);
							if (ip_addr_cmp(addr, &rdnss_address)) {
								dns_setserver(s, NULL);
							}
						}
					}
				}
				break;
			}
#endif							/* LWIP_ND6_RDNSS_MAX_DNS_SERVERS */
			default:
				/* Unrecognized option, abort. */
				ND6_STATS_INC(nd6.proterr);
				break;
			}
			/* option length is checked earlier to be non-zero to make sure loop ends */
			offset += (((u16_t) buffer[1]) << 3);
		}

		struct nd6_neighbor_cache_entry *nce_update = NULL;

		s8_t j;
		j = nd6_find_neighbor_cache_entry(ip6_current_src_addr());

		if (nce_flag && j < 0) {
			j = nd6_new_neighbor_cache_entry();
			if (j < 0) {
				pbuf_free(p);
				ND6_STATS_INC(nd6.memerr);
				ND6_STATS_INC(nd6.drop);
				return;
			}
			ip6_addr_set(&(neighbor_cache[j].next_hop_address), ip6_current_src_addr());
			SMEMCPY(neighbor_cache[j].lladdr, ND6H_LLADDR_OPT_ADDR(lladdr_opt), inp->hwaddr_len);
			neighbor_cache[j].netif = inp;
			neighbor_cache[j].q = NULL;
			neighbor_cache[j].state = ND6_STALE;
			neighbor_cache[j].isrouter = 1;
			nce_update = &(neighbor_cache[j]);
		} else if (nce_flag && j >= 0) {
			if (neighbor_cache[j].state == ND6_INCOMPLETE) {
				neighbor_cache[j].state = ND6_STALE;
			}
			/* Neighbor cache address isn't same with lladdr */
			if (MEMCMP(neighbor_cache[j].lladdr, ND6H_LLADDR_OPT_ADDR(lladdr_opt), inp->hwaddr_len)) {
				SMEMCPY(neighbor_cache[j].lladdr, ND6H_LLADDR_OPT_ADDR(lladdr_opt), inp->hwaddr_len);
				neighbor_cache[j].state = ND6_STALE;
			}
			neighbor_cache[j].isrouter = 1;
			nce_update = &(neighbor_cache[j]);
		} else if (j >= 0) {
			neighbor_cache[j].isrouter = 1;
			/* TODO: Should change neighbor_cache state? */
		}

		/* Re-set default timer values. */
#if LWIP_ND6_ALLOW_RA_UPDATES
		if (ND6H_RA_RETRANS_TMR(ra_hdr) > 0) {
			retrans_timer = lwip_htonl(ND6H_RA_RETRANS_TMR(ra_hdr));
		}
		if (ND6H_RA_REACH_TIME(ra_hdr) > 0) {
			reachable_time = lwip_htonl(ND6H_RA_REACH_TIME(ra_hdr));
		}
#endif							/* LWIP_ND6_ALLOW_RA_UPDATES */

		/* RFC 6.3.4.  Processing Received Router Advertisements
		 * If the received Cur Hop Limit value is non-zero, the host SHOULD set
		 * its CurHopLimit variable to the received value.
		 */
		if (ND6H_RA_HOPLIM(ra_hdr) > 0) {
			current_hop_limit = ND6H_RA_HOPLIM(ra_hdr);
		}

		u16_t lifetime = lwip_htons(ND6H_RA_ROUT_LIFE(ra_hdr));

		/* Get the matching default router entry. */
		i = nd6_get_router(ip6_current_src_addr(), inp);

		/* RFC 6.3.4. Processing Received Router Advertisements.
		 * If the address is not already present in the host's Default
		 * Router List, and the advertisement's Router Lifetime is non-
		 * zero, create a new entry in the list, and initialize its
		 * invalidation timer value from the advertisement's Router
		 * Lifetime field.
		 */
		if (i < 0 && lifetime != 0) {
			/* Create a new router entry. */
			i = nd6_new_router(ip6_current_src_addr(), inp);

			/* Failed to create new router */
			if (i < 0) {
				/* Could not create a new router entry. */
				LWIP_DEBUGF(ND6_DEBUG, ("Failed to create new router, ret %d\n", i));
				pbuf_free(p);
				ND6_STATS_INC(nd6.memerr);
				return;
			}
			LWIP_DEBUGF(ND6_DEBUG, ("Created new router on default_router_list %d\n", i));
			default_router_list[i].invalidation_timer = lifetime * 1000;

			if (nce_update) {
				default_router_list[i].neighbor_entry = nce_update;
				default_router_list[i].neighbor_entry->counter.reachable_time = reachable_time;
				ip6_addr_copy(default_router_list[i].router_ip6addr, default_router_list[i].neighbor_entry->next_hop_address);
			} else {
				ip6_addr_copy(default_router_list[i].router_ip6addr, *ip6_current_src_addr());
			}

		} else if (i >= 0 && lifetime != 0) {
			default_router_list[i].invalidation_timer = lifetime * 1000;
			if (nce_update) {
				if (default_router_list[i].neighbor_entry != nce_update) {
					default_router_list[i].neighbor_entry = nce_update;
					ip6_addr_copy(default_router_list[i].router_ip6addr, default_router_list[i].neighbor_entry->next_hop_address);
				}
			}
			if (default_router_list[i].neighbor_entry) {
				default_router_list[i].neighbor_entry->counter.reachable_time = reachable_time;
			}
		} else if (i >= 0 && lifetime == 0) {
			/* RFC 4861, 6.3.5.  Timing out Prefixes and Default Routers  */
			if (default_router_list[i].neighbor_entry) {
				default_router_list[i].neighbor_entry->isrouter = 0;
				nd6_free_expired_router_in_destination_cache(&(default_router_list[i].neighbor_entry->next_hop_address));
				default_router_list[i].neighbor_entry = NULL;
			}
			default_router_list[i].invalidation_timer = 0;
			default_router_list[i].flags = 0;
			ip6_addr_set_any(&default_router_list[i].router_ip6addr);
		} else if (i < 0 && lifetime == 0) {
			if (nce_update) {
				nce_update->counter.reachable_time = reachable_time;
			}
		} else {
			LWIP_DEBUGF(ND6_DEBUG, ("Never happen\n"));
		}
		break;				/* ICMP6_TYPE_RA */
	}
	case ICMP6_TYPE_RD: {		/* Redirect */
		struct redirect_header *redir_hdr;
		struct lladdr_option *lladdr_opt = NULL;
		struct redirected_header_option *redirected_opt = NULL;

		ip6_addr_t tmp;

		/* Check that Redir header fits in packet. */
		if (p->len < sizeof(struct redirect_header)) {
			/* @todo debug message */
			pbuf_free(p);
			ND6_STATS_INC(nd6.lenerr);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		redir_hdr = (struct redirect_header *)p->payload;

		/* Processing validition check as per RFC 4861, 8.1 */

		if (IP6H_HOPLIM(ip6_current_header()) != 255 ) {
			pbuf_free(p);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		if (ND6H_CODE(redir_hdr) != 0) {
			pbuf_free(p);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		if (ip6_addr_ismulticast(&ND6H_RD_DEST_ADDR(redir_hdr))) {
			pbuf_free(p);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		if (!ip6_addr_islinklocal(ip6_current_src_addr())) {
			pbuf_free(p);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		if (!ip6_addr_islinklocal(&ND6H_RD_TARGET_ADDR(redir_hdr))) {
			if (!ip6_addr_cmp(&ND6H_RD_DEST_ADDR(redir_hdr), &ND6H_RD_TARGET_ADDR(redir_hdr))) {
				pbuf_free(p);
				ND6_STATS_INC(nd6.drop);
				return;
			}
		}

		/* Copy original destination address to current source address, to have an aligned copy. */
		ip6_addr_set(&tmp, &(ND6H_RD_DEST_ADDR(redir_hdr)));

		/* Find dest address in cache */
		i = nd6_find_destination_cache_entry(&tmp);

		if (i >= 0) {
			if ((nd6_get_router(&destination_cache[i].next_hop_addr, inp) >= 0)
					&& (!ip6_addr_cmp(&destination_cache[i].next_hop_addr, ip6_current_src_addr()))) {
				LWIP_DEBUGF(ND6_DEBUG, ("Redirect source address is not the current first-hop router, dropped\n"));
				pbuf_free(p);
				ND6_STATS_INC(nd6.drop);
				return;
			}
		} else {
			/* Destination not in cache, create new destination cache entry. */
			/* As per RFC 4861, 8.3 Host specification
			 * If no Destination Cache entry exists for the destination, an implementation SHOULD create such an entry.
			 */
			i = nd6_new_destination_cache_entry();
			if (i >= 0) {
				destination_cache[i].pmtu = inp->mtu;
				destination_cache[i].pmtu_timer = 0;
				destination_cache[i].age = 0;
				ip6_addr_set(&(destination_cache[i].next_hop_addr), &ND6H_RD_TARGET_ADDR(redir_hdr));
				ip6_addr_set(&(destination_cache[i].destination_addr), &ND6H_RD_DEST_ADDR(redir_hdr));
			} else {
				pbuf_free(p);
				ND6_STATS_INC(nd6.drop);
				ND6_STATS_INC(nd6.memerr);
				return;
			}
		}

		/* End of validation check for RD message */

		/* Processing redirect header's option field
		 * RFC 4861, 4.5.  Redirect Message Format
		 * Possible options : Target link-layer address and Redirected Header */

		u8_t *buffer;
		size_t redir_optlen;
		size_t offset;

		redir_optlen = (p->len - sizeof(struct redirect_header));

		if (redir_optlen >= 2) {
			buffer = ((u8_t *)p->payload + sizeof(struct redirect_header));
			while (!redirected_opt && redir_optlen >= 2) {
				if (buffer[1] == 0) { /* Invalid length */
					pbuf_free(p);
					ND6_STATS_INC(nd6.drop);
					return;
				}
				switch (buffer[0]) {
				case ND6_OPTION_TYPE_TARGET_LLADDR:
					if (buffer[1] == 1) { /* IEEE 802 Link Layer Address */
						lladdr_opt = (struct lladdr_option *)buffer;
						offset = sizeof(struct lladdr_option);
					} else {
						lladdr_opt = NULL;
						offset = buffer[1] << 3;
					}
					break;
				case ND6_OPTION_TYPE_REDIR_HDR:
					redirected_opt = (struct redirected_header_option *)buffer;
					offset = sizeof(struct redirected_header_option);
					break;
				default:
					offset = (buffer[1] << 3);
					break;
				}
				redir_optlen -= offset;
				buffer = buffer + offset;
			}
		}

		/* Set the new target address. */
		ip6_addr_set(&(destination_cache[i].next_hop_addr), &(ND6H_RD_TARGET_ADDR(redir_hdr)));

		/* If Link-layer address of other router is given, try to add to neighbor cache. */
		if (lladdr_opt != NULL) {
			/* Copy target address to current source address, to have an aligned copy. */
			ip6_addr_set(&tmp, &(ND6H_RD_TARGET_ADDR(redir_hdr)));

			i = nd6_find_neighbor_cache_entry(&tmp);

			if (i < 0) {
				/* Creates the Neighbor cache entry for the target */
				i = nd6_new_neighbor_cache_entry();
				if (i < 0) {
					LWIP_DEBUGF(ND6_DEBUG, ("Failed to create a new neighbor cache entry\n"));
					ND6_STATS_INC(nd6.memerr);
					pbuf_free(p);
					return;
				}
			}

			neighbor_cache[i].netif = inp;

			/* If the link-layer address is the same as that already in the cache,
			 * the cache entry's state remains unchanged. */
			if (nd6_find_lladdr_neighbor_cache_entry(ND6H_LLADDR_OPT_ADDR(lladdr_opt), inp) < 0) {
				neighbor_cache[i].state = ND6_STALE;
			}

			MEMCPY(neighbor_cache[i].lladdr, ND6H_LLADDR_OPT_ADDR(lladdr_opt), inp->hwaddr_len);
			ip6_addr_set(&(neighbor_cache[i].next_hop_address), &tmp);
		}
		break;				/* ICMP6_TYPE_RD */
	}
	case ICMP6_TYPE_PTB: {		/* Packet too big */
		struct icmp6_hdr *icmp6hdr;	/* Packet too big message */
		struct ip6_hdr *ip6hdr;	/* IPv6 header of the packet which caused the error */
		u32_t pmtu;
		ip6_addr_t tmp;

		/* Check that ICMPv6 header + IPv6 header fit in payload */
		if (p->len < (sizeof(struct icmp6_hdr) + IP6_HLEN)) {
			/* drop short packets */
			pbuf_free(p);
			ND6_STATS_INC(nd6.lenerr);
			ND6_STATS_INC(nd6.drop);
			return;
		}

		icmp6hdr = (struct icmp6_hdr *)p->payload;
		ip6hdr = (struct ip6_hdr *)((u8_t *) p->payload + sizeof(struct icmp6_hdr));

		/* Copy original destination address to current source address, to have an aligned copy. */
		ip6_addr_set(&tmp, &(ip6hdr->dest));

		/* Look for entry in destination cache. */
		i = nd6_find_destination_cache_entry(&tmp);
		if (i < 0) {
#if LWIP_IPV6_PMTU_FOR_MULTICAST
			if (ip6_addr_ismulticast(&tmp)) {
				/* create new entry in destination cache */
				i = nd6_new_destination_cache_entry();
				if (i >= 0) {
					s8_t router_idx;

					/* set next_hop_address to default router. */
					router_idx = nd6_select_router(&tmp, inp);
					if (router_idx < 0) {
						pbuf_free(p);
						return;
					}
					ip6_addr_copy(destination_cache[i].next_hop_addr, default_router_list[router_idx].neighbor_entry->next_hop_address);

					/* copy multicast address to destination cache. */
					ip6_addr_set(&(destination_cache[i].destination_addr), &tmp);

					/* initialize pmtu, pmtu_timer and age */
					destination_cache[i].pmtu = inp->mtu;
					destination_cache[i].pmtu_timer = 0;
					destination_cache[i].age = 0;
				} else {
					pbuf_free(p);
					return;
				}
			} else
#endif
			{
				/* Destination not in cache, drop packet. */
				pbuf_free(p);
				return;
			}
		}

		/* Change the Path MTU. */
		pmtu = lwip_htonl(icmp6hdr->data);
		if ((pmtu <= destination_cache[i].pmtu) || (destination_cache[i].pmtu_timer <= 0)) {
			/* PMTU will be updated when one of the following conditions is true.
				1. pmtu is changed for the 1st time
				2. pmtu size is equal or decreased
				3. pmtu size is increased and 10 minutes or more passed since last pmtu update. (RFC 1981)
			*/
			destination_cache[i].pmtu = (u16_t) LWIP_MIN(pmtu, 0xFFFF);
			destination_cache[i].pmtu_timer = 10 * 60 * 1000; /* 10 minutes (= 600000 ms) */
		}

		break;				/* ICMP6_TYPE_PTB */
	}

	default:
		ND6_STATS_INC(nd6.proterr);
		ND6_STATS_INC(nd6.drop);
		break;					/* default */
	}

	pbuf_free(p);
}

/**
 * Periodic timer for Neighbor discovery functions:
 *
 * - Update neighbor reachability states
 * - Update destination cache entries age
 * - Update invalidation timers of default routers and on-link prefixes
 * - Perform duplicate address detection (DAD) for our addresses
 * - Send router solicitations
 */
void nd6_tmr(void)
{
	s8_t i;
	struct netif *netif;

	/* Process neighbor entries. */
	for (i = 0; i < LWIP_ND6_NUM_NEIGHBORS; i++) {
		switch (neighbor_cache[i].state) {
		case ND6_INCOMPLETE:
			if (neighbor_cache[i].probes_sent >= LWIP_ND6_MAX_MULTICAST_SOLICIT) {
				/* Retries exceeded. */
				nd6_cleanup_all_caches(&neighbor_cache[i].next_hop_address);
			} else {
				neighbor_cache[i].counter.incomplete_time += LWIP_ND6_RETRANS_TIMER;
				if (retrans_timer <= LWIP_ND6_RETRANS_TIMER ||
					neighbor_cache[i].counter.incomplete_time >= retrans_timer) {
					/* Send a NS for this entry */
					neighbor_cache[i].counter.incomplete_time = 0;
					neighbor_cache[i].probes_sent++;
					nd6_send_neighbor_cache_probe(&neighbor_cache[i], ND6_SEND_FLAG_MULTICAST_DEST);
				}
			}
			break;
		case ND6_REACHABLE:
			/* Send queued packets, if any are left. Should have been sent already. */
			if (neighbor_cache[i].q != NULL) {
				nd6_send_q(i);
			}
			if (neighbor_cache[i].counter.reachable_time <= ND6_TMR_INTERVAL) {
				/* Change to stale state. */
				neighbor_cache[i].state = ND6_STALE;
			} else {
				neighbor_cache[i].counter.reachable_time -= ND6_TMR_INTERVAL;
			}
			break;
		case ND6_STALE:
			/* RFC 7.3.3. */
			if (neighbor_cache[i].q != NULL) {
				nd6_send_q(i);
			}
			break;
		case ND6_DELAY:
			if (neighbor_cache[i].counter.delay_time <= ND6_TMR_INTERVAL) {
				/* Change to PROBE state. */
				neighbor_cache[i].state = ND6_PROBE;
				neighbor_cache[i].counter.probe_time = 0;
				neighbor_cache[i].probes_sent = 0;
			} else {
				neighbor_cache[i].counter.delay_time -= ND6_TMR_INTERVAL;
			}
			break;
		case ND6_PROBE:
			if (neighbor_cache[i].probes_sent >= LWIP_ND6_MAX_UNICAST_SOLICIT) {
				nd6_cleanup_all_caches(&neighbor_cache[i].next_hop_address);
			} else {
				neighbor_cache[i].counter.probe_time += LWIP_ND6_RETRANS_TIMER;
				if (retrans_timer <= LWIP_ND6_RETRANS_TIMER ||
					neighbor_cache[i].counter.probe_time >= retrans_timer) {
					/* Send a NS for this entry. */
					neighbor_cache[i].counter.probe_time = 0;
					neighbor_cache[i].probes_sent++;
					nd6_send_neighbor_cache_probe(&neighbor_cache[i], 0);
				}
			}
			break;
		case ND6_NO_ENTRY:
		default:
			/* Do nothing. */
			break;
		}
	}

	/* Process destination entries. */
	for (i = 0; i < LWIP_ND6_NUM_DESTINATIONS; i++) {
		destination_cache[i].age++;
		if (destination_cache[i].pmtu_timer > 0) {
			destination_cache[i].pmtu_timer -= ND6_TMR_INTERVAL;
		}
	}

	/* Process router entries. */
	for (i = 0; i < LWIP_ND6_NUM_ROUTERS; i++) {
		if (default_router_list[i].neighbor_entry != NULL) {
			/* Active entry. */
			if (default_router_list[i].invalidation_timer >= ND6_TMR_INTERVAL) {
				default_router_list[i].invalidation_timer -= ND6_TMR_INTERVAL;
			}
			if (default_router_list[i].invalidation_timer < ND6_TMR_INTERVAL) {
				/* Less than 1 second remaining. Clear this entry. */
				default_router_list[i].neighbor_entry->isrouter = 0;

				/* RFC 4861, 6.3.5.  Timing out Prefixes and Default Routers  */
				nd6_free_expired_router_in_destination_cache(&(default_router_list[i].neighbor_entry->next_hop_address));
				default_router_list[i].neighbor_entry = NULL;
				default_router_list[i].invalidation_timer = 0;
				default_router_list[i].flags = 0;
				ip6_addr_set_any(&default_router_list[i].router_ip6addr);
			}
		}
	}

	/* Process prefix entries. */
	for (i = 0; i < LWIP_ND6_NUM_PREFIXES; i++) {
		if (prefix_list[i].netif != NULL) {
			if (prefix_list[i].invalidation_timer < ND6_TMR_INTERVAL) {
				/* Entry timed out, remove it */
				prefix_list[i].invalidation_timer = 0;

#if LWIP_IPV6_AUTOCONFIG
				/* If any addresses were configured with this prefix, remove them */
				if (prefix_list[i].flags & ND6_PREFIX_AUTOCONFIG_ADDRESS_GENERATED) {
					s8_t j;

					for (j = 1; j < LWIP_IPV6_NUM_ADDRESSES; j++) {
						if ((netif_ip6_addr_state(prefix_list[i].netif, j) != IP6_ADDR_INVALID) && ip6_addr_netcmp(&prefix_list[i].prefix, netif_ip6_addr(prefix_list[i].netif, j))) {
							netif_ip6_addr_set_state(prefix_list[i].netif, j, IP6_ADDR_INVALID);
							prefix_list[i].flags = 0;

							/* Exit loop. */
							break;
						}
					}
				}
#endif							/* LWIP_IPV6_AUTOCONFIG */

				prefix_list[i].netif = NULL;
				prefix_list[i].flags = 0;
			} else {
				prefix_list[i].invalidation_timer -= ND6_TMR_INTERVAL;

#if LWIP_IPV6_AUTOCONFIG
				/* Initiate address autoconfiguration for this prefix, if conditions are met. */
				if (prefix_list[i].netif->ip6_autoconfig_enabled && (prefix_list[i].flags & ND6_PREFIX_AUTOCONFIG_AUTONOMOUS) && !(prefix_list[i].flags & ND6_PREFIX_AUTOCONFIG_ADDRESS_GENERATED)) {
					s8_t j;
					/* Try to get an address on this netif that is invalid.
					 * Skip 0 index (link-local address) */
					for (j = 1; j < LWIP_IPV6_NUM_ADDRESSES; j++) {
						if (netif_ip6_addr_state(prefix_list[i].netif, j) == IP6_ADDR_INVALID) {
							/* Generate an address using this prefix and interface ID from link-local address. */
							netif_ip6_addr_set_parts(prefix_list[i].netif, j, prefix_list[i].prefix.addr[0], prefix_list[i].prefix.addr[1], netif_ip6_addr(prefix_list[i].netif, 0)->addr[2], netif_ip6_addr(prefix_list[i].netif, 0)->addr[3]);

							/* Mark it as tentative (DAD will be performed if configured). */
							netif_ip6_addr_set_state(prefix_list[i].netif, j, IP6_ADDR_TENTATIVE);

							/* Mark this prefix with ADDRESS_GENERATED, so that we don't try again. */
							prefix_list[i].flags |= ND6_PREFIX_AUTOCONFIG_ADDRESS_GENERATED;

							/* Exit loop. */
							break;
						}
					}
				}
#endif							/* LWIP_IPV6_AUTOCONFIG */
			}
		}
	}

	/* Process our own addresses, if DAD configured. */
	for (netif = netif_list; netif != NULL; netif = netif->next) {
		for (i = 0; i < LWIP_IPV6_NUM_ADDRESSES; ++i) {
			u8_t addr_state = netif_ip6_addr_state(netif, i);
			if (ip6_addr_istentative(addr_state)) {
				if ((addr_state & IP6_ADDR_TENTATIVE_COUNT_MASK) >= LWIP_IPV6_DUP_DETECT_ATTEMPTS) {
					/* No NA received in response. Mark address as valid. */
					netif_ip6_addr_set_state(netif, i, IP6_ADDR_PREFERRED);
					/* @todo implement preferred and valid lifetimes. */
				} else if (netif->flags & NETIF_FLAG_UP) {
					/* Send a NS for this address. */
					nd6_send_ns(netif, netif_ip6_addr(netif, i), ND6_SEND_FLAG_MULTICAST_DEST | ND6_SEND_FLAG_ADDRANY_SRC);
					/* tentative: set next state by increasing by one */
					netif_ip6_addr_set_state(netif, i, addr_state + 1);
					/* @todo send max 1 NS per tmr call? enable return */
					/*return; */
				}
			}
		}
	}

#if LWIP_IPV6_SEND_ROUTER_SOLICIT
	/* Send router solicitation messages, if necessary. */
	for (netif = netif_list; netif != NULL; netif = netif->next) {
		if ((netif->flags & NETIF_FLAG_UP) && (!ip6_addr_isinvalid(netif_ip6_addr_state(netif, 0)))) {
			if (netif->rs_interval < LWIP_ND6_MAX_SOLICIT_INTERVAL) {
				netif->rs_interval += ND6_TMR_INTERVAL;
			}
			if ((netif->rs_count > 0) && (netif->rs_interval >= LWIP_ND6_MAX_SOLICIT_INTERVAL)) {
				nd6_send_rs(netif);
				netif->rs_count--;
				netif->rs_interval = 0;
			}
		}
	}
#endif							/* LWIP_IPV6_SEND_ROUTER_SOLICIT */
}

/** Send a neighbor solicitation message for a specific neighbor cache entry
 *
 * @param entry the neightbor cache entry for wich to send the message
 * @param flags one of ND6_SEND_FLAG_*
 */
static void nd6_send_neighbor_cache_probe(struct nd6_neighbor_cache_entry *entry, u8_t flags)
{
	nd6_send_ns(entry->netif, &entry->next_hop_address, flags);
}

/**
 * Send a neighbor solicitation message
 *
 * @param netif the netif on which to send the message
 * @param target_addr the IPv6 target address for the ND message
 * @param flags one of ND6_SEND_FLAG_*
 */
static void nd6_send_ns(struct netif *netif, const ip6_addr_t *target_addr, u8_t flags)
{
	struct ns_header *ns_hdr;
	struct pbuf *p;
	const ip6_addr_t *src_addr;
	u16_t lladdr_opt_len;

	u8_t i;

	if ((flags & ND6_SEND_FLAG_ADDRANY_SRC) || !ip6_addr_isvalid(netif_ip6_addr_state(netif, 0))) {
		src_addr = IP6_ADDR_ANY6;
		/* Option "MUST NOT be included when the source IP address is the unspecified address." */
		lladdr_opt_len = 0;
	} else if (!ip6_addr_islinklocal(target_addr)) {
		/* @todo should we do for site-local or unique-local */
		for (i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
			src_addr = netif_ip6_addr(netif, i);
			if (!ip6_addr_islinklocal(src_addr)
					&& ip6_addr_isvalid(netif_ip6_addr_state(netif, i))) {
				break;
			}
		}

		if (i >= LWIP_IPV6_NUM_ADDRESSES) {
			/* no matched global or site address */
			src_addr = IP6_ADDR_ANY6;
			lladdr_opt_len = 0;
		} else {
			lladdr_opt_len = ((netif->hwaddr_len + 2) + 7) >> 3;
		}
	} else {
		/* @todo should we do for site-local or unique-local */
		/* Use link-local address as source address. */
		src_addr = netif_ip6_addr(netif, 0);
		/* calculate option length (in 8-byte-blocks) */
		lladdr_opt_len = ((netif->hwaddr_len + 2) + 7) >> 3;
	}

	/* Allocate a packet. */
	p = pbuf_alloc(PBUF_IP, sizeof(struct ns_header) + (lladdr_opt_len << 3), PBUF_RAM);
	if (p == NULL) {
		ND6_STATS_INC(nd6.memerr);
		return;
	}

	/* Set fields. */
	ns_hdr = (struct ns_header *)p->payload;

	ND6H_TYPE(ns_hdr) = ICMP6_TYPE_NS;
	ND6H_CODE(ns_hdr) = 0;
	ND6H_CHKSUM(ns_hdr) = 0;
	ND6H_RESERV(ns_hdr) = 0;
	ip6_addr_set(&(ND6H_NS_TARGET_ADDR(ns_hdr)), target_addr);

	if (lladdr_opt_len != 0) {
		struct lladdr_option *lladdr_opt = (struct lladdr_option *)((u8_t *) p->payload + sizeof(struct ns_header));
		ND6H_LLADDR_OPT_TYPE(lladdr_opt) = ND6_OPTION_TYPE_SOURCE_LLADDR;
		ND6H_LLADDR_OPT_LEN(lladdr_opt) = (u8_t) lladdr_opt_len;
		SMEMCPY(ND6H_LLADDR_OPT_ADDR(lladdr_opt), netif->hwaddr, netif->hwaddr_len);
	}

	/* Generate the solicited node address for the target address. */
	if (flags & ND6_SEND_FLAG_MULTICAST_DEST) {
		ip6_addr_set_solicitednode(&multicast_address, target_addr->addr[3]);
		target_addr = &multicast_address;
	}
#if CHECKSUM_GEN_ICMP6
	IF__NETIF_CHECKSUM_ENABLED(netif, NETIF_CHECKSUM_GEN_ICMP6) {
		ND6H_CHKSUM(ns_hdr) = ip6_chksum_pseudo(p, IP6_NEXTH_ICMP6, p->len, src_addr, target_addr);
	}
#endif							/* CHECKSUM_GEN_ICMP6 */

	/* Send the packet out. */
	ND6_STATS_INC(nd6.xmit);
	ip6_output_if(p, (src_addr == IP6_ADDR_ANY6) ? NULL : src_addr, target_addr, LWIP_ICMP6_HL, 0, IP6_NEXTH_ICMP6, netif);
	pbuf_free(p);
}

/**
 * Send a neighbor advertisement message
 *
 * @param netif the netif on which to send the message
 * @param target_addr the IPv6 target address for the ND message
 * @param flags one of ND6_SEND_FLAG_*
 */
static void nd6_send_na(struct netif *netif, const ip6_addr_t *target_addr, u8_t flags)
{
	struct na_header *na_hdr;
	struct lladdr_option *lladdr_opt;
	struct pbuf *p;
	const ip6_addr_t *src_addr;
	const ip6_addr_t *dest_addr;
	u16_t lladdr_opt_len;

	/* Use link-local address as source address. */
	/* src_addr = netif_ip6_addr(netif, 0); */
	/* Use target address as source address. */
	src_addr = target_addr;

	/* Allocate a packet. */
	lladdr_opt_len = ((netif->hwaddr_len + 2) >> 3) + (((netif->hwaddr_len + 2) & 0x07) ? 1 : 0);
	p = pbuf_alloc(PBUF_IP, sizeof(struct na_header) + (lladdr_opt_len << 3), PBUF_RAM);
	if (p == NULL) {
		ND6_STATS_INC(nd6.memerr);
		return;
	}

	/* Set fields. */
	na_hdr = (struct na_header *)p->payload;
	lladdr_opt = (struct lladdr_option *)((u8_t *) p->payload + sizeof(struct na_header));

	ND6H_TYPE(na_hdr) = ICMP6_TYPE_NA;
	ND6H_CODE(na_hdr) = 0;
	ND6H_CHKSUM(na_hdr) = 0;
	ND6H_NA_FLAG(na_hdr) = flags & 0xf0;
	ND6H_RESERV(na_hdr)[0] = 0;
	ND6H_RESERV(na_hdr)[1] = 0;
	ND6H_RESERV(na_hdr)[2] = 0;
	ip6_addr_set(&(ND6H_NA_TARGET_ADDR(na_hdr)), target_addr);

	ND6H_LLADDR_OPT_TYPE(lladdr_opt) = ND6_OPTION_TYPE_TARGET_LLADDR;
	ND6H_LLADDR_OPT_LEN(lladdr_opt) = (u8_t) lladdr_opt_len;
	SMEMCPY(ND6H_LLADDR_OPT_ADDR(lladdr_opt), netif->hwaddr, netif->hwaddr_len);

	/* Generate the solicited node address for the target address. */
	if (flags & ND6_SEND_FLAG_MULTICAST_DEST) {
		ip6_addr_set_solicitednode(&multicast_address, target_addr->addr[3]);
		dest_addr = &multicast_address;
	} else if (flags & ND6_SEND_FLAG_ALLNODES_DEST) {
		ip6_addr_set_allnodes_linklocal(&multicast_address);
		dest_addr = &multicast_address;
	} else {
		dest_addr = ip6_current_src_addr();
	}

#if CHECKSUM_GEN_ICMP6
	IF__NETIF_CHECKSUM_ENABLED(netif, NETIF_CHECKSUM_GEN_ICMP6) {
		ND6H_CHKSUM(na_hdr) = ip6_chksum_pseudo(p, IP6_NEXTH_ICMP6, p->len, src_addr, dest_addr);
	}
#endif							/* CHECKSUM_GEN_ICMP6 */

	/* Send the packet out. */
	ND6_STATS_INC(nd6.xmit);
	ip6_output_if(p, src_addr, dest_addr, LWIP_ICMP6_HL, 0, IP6_NEXTH_ICMP6, netif);
	pbuf_free(p);
}

#if LWIP_IPV6_SEND_ROUTER_SOLICIT
/**
 * Send a router solicitation message
 *
 * @param netif the netif on which to send the message
 */
static err_t nd6_send_rs(struct netif *netif)
{
	struct rs_header *rs_hdr;
	struct lladdr_option *lladdr_opt;
	struct pbuf *p;
	const ip6_addr_t *src_addr;
	err_t err;
	u16_t lladdr_opt_len = 0;

	/* Link-local source address, or unspecified address? */
	if (ip6_addr_isvalid(netif_ip6_addr_state(netif, 0))) {
		src_addr = netif_ip6_addr(netif, 0);
	} else {
		src_addr = IP6_ADDR_ANY6;
	}

	/* Generate the all routers target address. */
	ip6_addr_set_allrouters_linklocal(&multicast_address);

	/* Allocate a packet. */
	if (src_addr != IP6_ADDR_ANY6) {
		lladdr_opt_len = ((netif->hwaddr_len + 2) >> 3) + (((netif->hwaddr_len + 2) & 0x07) ? 1 : 0);
	}
	p = pbuf_alloc(PBUF_IP, sizeof(struct rs_header) + (lladdr_opt_len << 3), PBUF_RAM);
	if (p == NULL) {
		ND6_STATS_INC(nd6.memerr);
		return ERR_BUF;
	}

	/* Set fields. */
	rs_hdr = (struct rs_header *)p->payload;

	ND6H_TYPE(rs_hdr) = ICMP6_TYPE_RS;
	ND6H_CODE(rs_hdr) = 0;
	ND6H_CHKSUM(rs_hdr) = 0;
	ND6H_RESERV(rs_hdr) = 0;

	if (src_addr != IP6_ADDR_ANY6) {
		/* Include our hw address. */
		lladdr_opt = (struct lladdr_option *)((u8_t *) p->payload + sizeof(struct rs_header));
		ND6H_LLADDR_OPT_TYPE(lladdr_opt) = ND6_OPTION_TYPE_SOURCE_LLADDR;
		ND6H_LLADDR_OPT_LEN(lladdr_opt) = (u8_t) lladdr_opt_len;
		SMEMCPY(ND6H_LLADDR_OPT_ADDR(lladdr_opt), netif->hwaddr, netif->hwaddr_len);
	}
#if CHECKSUM_GEN_ICMP6
	IF__NETIF_CHECKSUM_ENABLED(netif, NETIF_CHECKSUM_GEN_ICMP6) {
		ND6H_CHKSUM(rs_hdr) = ip6_chksum_pseudo(p, IP6_NEXTH_ICMP6, p->len, src_addr, &multicast_address);
	}
#endif							/* CHECKSUM_GEN_ICMP6 */

	/* Send the packet out. */
	ND6_STATS_INC(nd6.xmit);

	err = ip6_output_if(p, (src_addr == IP6_ADDR_ANY6) ? NULL : src_addr, &multicast_address, LWIP_ICMP6_HL, 0, IP6_NEXTH_ICMP6, netif);
	pbuf_free(p);

	return err;
}
#endif							/* LWIP_IPV6_SEND_ROUTER_SOLICIT */

/**
 * Search for a neighbor cache entry
 *
 * @param ip6addr the IPv6 address of the neighbor
 * @return The neighbor cache entry index that matched, -1 if no
 * entry is found
 */
static s8_t nd6_find_neighbor_cache_entry(const ip6_addr_t *ip6addr)
{
	s8_t i;
	for (i = 0; i < LWIP_ND6_NUM_NEIGHBORS; i++) {
		if (ip6_addr_cmp(ip6addr, &(neighbor_cache[i].next_hop_address))) {
			return i;
		}
	}
	return -1;
}

/**
 * Search for a neighbor cache entry by using link-layer address (lladdr)
 *
 * @param lladdr the Link-layer address
 * @param netif points to a network interface
 * @return The neighbor cache entry index that matched, -1 if no
 * entry is found
 */
static s8_t nd6_find_lladdr_neighbor_cache_entry(const u8_t *lladdr, const struct netif *inp)
{
	s8_t i;

	if (inp == NULL || lladdr == NULL) {
		return -1;
	}

	for (i = 0; i < LWIP_ND6_NUM_NEIGHBORS; i++) {
		if (!MEMCMP(neighbor_cache[i].lladdr, lladdr, inp->hwaddr_len)) {
			return i;
		}
	}

	return -1;
}

/**
 * Create a new neighbor cache entry.
 *
 * If no unused entry is found, will try to recycle an old entry
 * according to ad-hoc "age" heuristic.
 *
 * @return The neighbor cache entry index that was created, -1 if no
 * entry could be created
 */
static s8_t nd6_new_neighbor_cache_entry(void)
{
	s8_t i;
	s8_t j;
	u32_t time;

	/* First, try to find an empty entry. */
	for (i = 0; i < LWIP_ND6_NUM_NEIGHBORS; i++) {
		if (neighbor_cache[i].state == ND6_NO_ENTRY) {
			return i;
		}
	}

	/* We need to recycle an entry. in general, do not recycle if it is a router. */

	/* Next, try to find a Stale entry. */
	for (i = 0; i < LWIP_ND6_NUM_NEIGHBORS; i++) {
		if ((neighbor_cache[i].state == ND6_STALE) && (!neighbor_cache[i].isrouter)) {
			nd6_free_neighbor_cache_entry(i);
			return i;
		}
	}

	/* Next, try to find a Probe entry. */
	for (i = 0; i < LWIP_ND6_NUM_NEIGHBORS; i++) {
		if ((neighbor_cache[i].state == ND6_PROBE) && (!neighbor_cache[i].isrouter)) {
			nd6_free_neighbor_cache_entry(i);
			return i;
		}
	}

	/* Next, try to find a Delayed entry. */
	for (i = 0; i < LWIP_ND6_NUM_NEIGHBORS; i++) {
		if ((neighbor_cache[i].state == ND6_DELAY) && (!neighbor_cache[i].isrouter)) {
			nd6_free_neighbor_cache_entry(i);
			return i;
		}
	}

	/* Next, try to find the oldest reachable entry. */
	time = 0xfffffffful;
	j = -1;
	for (i = 0; i < LWIP_ND6_NUM_NEIGHBORS; i++) {
		if ((neighbor_cache[i].state == ND6_REACHABLE) && (!neighbor_cache[i].isrouter)) {
			if (neighbor_cache[i].counter.reachable_time < time) {
				j = i;
				time = neighbor_cache[i].counter.reachable_time;
			}
		}
	}
	if (j >= 0) {
		nd6_free_neighbor_cache_entry(j);
		return j;
	}

	/* Next, find oldest incomplete entry without queued packets. */
	time = 0;
	j = -1;
	for (i = 0; i < LWIP_ND6_NUM_NEIGHBORS; i++) {
		if ((neighbor_cache[i].q == NULL) && (neighbor_cache[i].state == ND6_INCOMPLETE) && (!neighbor_cache[i].isrouter)) {
			if (neighbor_cache[i].probes_sent >= time) {
				j = i;
				time = neighbor_cache[i].probes_sent;
			}
		}
	}
	if (j >= 0) {
		nd6_free_neighbor_cache_entry(j);
		return j;
	}

	/* Next, find oldest incomplete entry with queued packets. */
	time = 0;
	j = -1;
	for (i = 0; i < LWIP_ND6_NUM_NEIGHBORS; i++) {
		if ((neighbor_cache[i].state == ND6_INCOMPLETE) && (!neighbor_cache[i].isrouter)) {
			if (neighbor_cache[i].probes_sent >= time) {
				j = i;
				time = neighbor_cache[i].probes_sent;
			}
		}
	}
	if (j >= 0) {
		nd6_free_neighbor_cache_entry(j);
		return j;
	}

	/* No more entries to try. */
	return -1;
}

/**
 * Will free any resources associated with a neighbor cache
 * entry, and will mark it as unused.
 *
 * @param i the neighbor cache entry index to free
 */
static void nd6_free_neighbor_cache_entry(s8_t i)
{
	if ((i < 0) || (i >= LWIP_ND6_NUM_NEIGHBORS)) {
		return;
	}

	/* Free any queued packets. */
	if (neighbor_cache[i].q != NULL) {
		nd6_free_q(neighbor_cache[i].q);
		neighbor_cache[i].q = NULL;
	}

	memset(neighbor_cache[i].lladdr, 0, NETIF_MAX_HWADDR_LEN);
	neighbor_cache[i].state = ND6_NO_ENTRY;
	neighbor_cache[i].isrouter = 0;
	neighbor_cache[i].netif = NULL;
	neighbor_cache[i].counter.reachable_time = 0;
	neighbor_cache[i].probes_sent = 0;
	ip6_addr_set_zero(&(neighbor_cache[i].next_hop_address));
}

/**
 * Search for a destination cache entry
 *
 * @param ip6addr the IPv6 address of the destination
 * @return The destination cache entry index that matched, -1 if no
 * entry is found
 */
static s8_t nd6_find_destination_cache_entry(const ip6_addr_t *ip6addr)
{
	s8_t i;
	for (i = 0; i < LWIP_ND6_NUM_DESTINATIONS; i++) {
		if (ip6_addr_cmp(ip6addr, &(destination_cache[i].destination_addr))) {
			return i;
		}
	}
	return -1;
}

/**
 * Create a new destination cache entry. If no unused entry is found,
 * will recycle oldest entry.
 *
 * @return The destination cache entry index that was created, -1 if no
 * entry was created
 */
static s8_t nd6_new_destination_cache_entry(void)
{
	s8_t i, j;
	u32_t age;

	/* Find an empty entry. */
	for (i = 0; i < LWIP_ND6_NUM_DESTINATIONS; i++) {
		if (ip6_addr_isany(&(destination_cache[i].destination_addr))) {
			return i;
		}
	}

	/* Find oldest entry. */
	age = 0;
	j = LWIP_ND6_NUM_DESTINATIONS - 1;
	for (i = 0; i < LWIP_ND6_NUM_DESTINATIONS; i++) {
		if (destination_cache[i].age > age) {
			j = i;
		}
	}

	return j;
}

/**
 * Clear the destination cache.
 *
 * This operation may be necessary for consistency in the light of changing
 * local addresses and/or use of the gateway hook.
 */
void nd6_clear_destination_cache(void)
{
	int i;

	for (i = 0; i < LWIP_ND6_NUM_DESTINATIONS; i++) {
		ip6_addr_set_any(&destination_cache[i].destination_addr);
	}
}

/**
 * Free the destination entity of which Lifetime has been expired.
 *
 * When the Lifetime of default router is expired, the entiry should be discarded
 * to prevent sending traffic to the (deleted) router
 * Accordingly RFC 4861 cluase 6.3.5, the host MUST update the Destination cache ..
 */
void nd6_free_expired_router_in_destination_cache(const ip6_addr_t *ip6addr)
{
	int i;

	for (i = 0; i < LWIP_ND6_NUM_DESTINATIONS; i++) {
		if (ip6_addr_cmp(ip6addr, &(destination_cache[i].next_hop_addr))) {
			ip6_addr_set_any(&destination_cache[i].destination_addr);
		}
	}
}

/**
 * Determine whether an address matches an on-link prefix.
 *
 * @param ip6addr the IPv6 address to match
 * @return 1 if the address is on-link, 0 otherwise
 */
static s8_t nd6_is_prefix_in_netif(const ip6_addr_t *ip6addr, struct netif *netif)
{
	s8_t i;
	for (i = 0; i < LWIP_ND6_NUM_PREFIXES; i++) {
		if ((prefix_list[i].netif == netif) && (prefix_list[i].invalidation_timer > 0) && ip6_addr_netcmp(ip6addr, &(prefix_list[i].prefix))) {
			return 1;
		}
	}
	/* Check to see if address prefix matches a (manually?) configured address. */
	for (i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
		if (ip6_addr_isvalid(netif_ip6_addr_state(netif, i)) && ip6_addr_netcmp(ip6addr, netif_ip6_addr(netif, i))) {
			return 1;
		}
	}
	return 0;
}

/**
 * Select a un-reachabled default router for a destination.
 * When only RA is received at the first time without lladdr option
 * default router has created but there are no information on neighbor entry
 * For future use of the router, add address of router on default_router_list
 *
 * @return the default router entry index, or -1 if no suitable
 *         router is found
 */

static s8_t nd6_select_unreachable_default_router(void)
{
	s8_t i;

	for (i = 0; i < LWIP_ND6_NUM_ROUTERS; i++) {
		if (default_router_list[i].neighbor_entry == NULL
					&& !ip6_addr_isany(&default_router_list[i].router_ip6addr)) {
			return i;
		}
	}
	return -1;
}

/**
 * Select a default router for a destination.
 *
 * @param ip6addr the destination address
 * @param netif the netif for the outgoing packet, if known
 * @return the default router entry index, or -1 if no suitable
 *         router is found
 */
static s8_t nd6_select_router(const ip6_addr_t *ip6addr, struct netif *netif)
{
	s8_t i;
	/* last_router is used for round-robin router selection (as recommended
	 * in RFC). This is more robust in case one router is not reachable,
	 * we are not stuck trying to resolve it. */
	static s8_t last_router;
	(void)ip6addr;				/* @todo match preferred routes!! (must implement ND6_OPTION_TYPE_ROUTE_INFO) */

	/* @todo: implement default router preference */

	/* Look for reachable routers. */
	for (i = 0; i < LWIP_ND6_NUM_ROUTERS; i++) {
		if (++last_router >= LWIP_ND6_NUM_ROUTERS) {
			last_router = 0;
		}
		if ((default_router_list[i].neighbor_entry != NULL) && (netif != NULL ? netif == default_router_list[i].neighbor_entry->netif : 1) && (default_router_list[i].invalidation_timer > 0) && (default_router_list[i].neighbor_entry->state == ND6_REACHABLE)) {
			return i;
		}
	}

	/* Look for router in other reachability states, but still valid according to timer. */
	for (i = 0; i < LWIP_ND6_NUM_ROUTERS; i++) {
		if (++last_router >= LWIP_ND6_NUM_ROUTERS) {
			last_router = 0;
		}
		if ((default_router_list[i].neighbor_entry != NULL) && (netif != NULL ? netif == default_router_list[i].neighbor_entry->netif : 1) && (default_router_list[i].invalidation_timer > 0)) {
			return i;
		}
	}

	/* Look for any router for which we have any information at all. */
	for (i = 0; i < LWIP_ND6_NUM_ROUTERS; i++) {
		if (++last_router >= LWIP_ND6_NUM_ROUTERS) {
			last_router = 0;
		}
		if (default_router_list[i].neighbor_entry != NULL && (netif != NULL ? netif == default_router_list[i].neighbor_entry->netif : 1)) {
			return i;
		}
	}

	/* no suitable router found. */
	return -1;
}

/**
 * Find a router-announced route to the given destination.
 *
 * The caller is responsible for checking whether the returned netif, if any,
 * is in a suitable state (up, link up) to be used for packet transmission.
 *
 * @param ip6addr the destination IPv6 address
 * @return the netif to use for the destination, or NULL if none found
 */
struct netif *nd6_find_route(const ip6_addr_t *ip6addr)
{
	s8_t i;

	i = nd6_select_router(ip6addr, NULL);
	if (i >= 0) {
		if (default_router_list[i].neighbor_entry != NULL) {
			return default_router_list[i].neighbor_entry->netif;	/* may be NULL */
		}
	}

	return NULL;
}

/**
 * Find an entry for a default router.
 *
 * @param router_addr the IPv6 address of the router
 * @param netif the netif on which the router is found, if known
 * @return the index of the router entry, or -1 if not found
 */
static s8_t nd6_get_router(const ip6_addr_t *router_addr, struct netif *netif)
{
	s8_t i;

	/* Look for router. */
	for (i = 0; i < LWIP_ND6_NUM_ROUTERS; i++) {
		if ((default_router_list[i].neighbor_entry != NULL) && ((netif != NULL) ? netif == default_router_list[i].neighbor_entry->netif : 1) && ip6_addr_cmp(router_addr, &(default_router_list[i].neighbor_entry->next_hop_address))) {
			return i;
		}
	}

	/* router not found. */
	return -1;
}

/**
 * Create a new entry for a default router.
 *
 * @param router_addr the IPv6 address of the router
 * @param netif the netif on which the router is connected, if known
 * @return the index on the router table, or -1 if could not be created
 */
static s8_t nd6_new_router(const ip6_addr_t *router_addr, struct netif *netif)
{
	s8_t router_index;
	s8_t free_router_index = -1;

	/* Create new default router in empty slot */
	for (router_index = (LWIP_ND6_NUM_ROUTERS - 1); router_index >= 0; router_index--) {
		if (default_router_list[router_index].neighbor_entry == NULL) {
			free_router_index = router_index;
		}
	}

	return free_router_index;
}

/**
 * Find the cached entry for an on-link prefix.
 *
 * @param prefix the IPv6 prefix that is on-link
 * @param netif the netif on which the prefix is on-link
 * @return the index on the prefix table, or -1 if not found
 */
static s8_t nd6_get_onlink_prefix(ip6_addr_t *prefix, struct netif *netif)
{
	s8_t i;

	/* Look for prefix in list. */
	for (i = 0; i < LWIP_ND6_NUM_PREFIXES; ++i) {
		if ((ip6_addr_netcmp(&(prefix_list[i].prefix), prefix)) && (prefix_list[i].netif == netif)) {
			return i;
		}
	}

	/* Entry not available. */
	return -1;
}

/**
 * Creates a new entry for an on-link prefix.
 *
 * @param prefix the IPv6 prefix that is on-link
 * @param netif the netif on which the prefix is on-link
 * @return the index on the prefix table, or -1 if not created
 */
static s8_t nd6_new_onlink_prefix(ip6_addr_t *prefix, struct netif *netif)
{
	s8_t i;

	/* Create new entry. */
	for (i = 0; i < LWIP_ND6_NUM_PREFIXES; ++i) {
		if ((prefix_list[i].netif == NULL) || (prefix_list[i].invalidation_timer == 0)) {
			/* Found empty prefix entry. */
			prefix_list[i].netif = netif;
			ip6_addr_set(&(prefix_list[i].prefix), prefix);
#if LWIP_IPV6_AUTOCONFIG
			prefix_list[i].flags = 0;
#endif							/* LWIP_IPV6_AUTOCONFIG */
			return i;
		}
	}

	/* Entry not available. */
	return -1;
}

/**
 * Determine the next hop for a destination. Will determine if the
 * destination is on-link, else a suitable on-link router is selected.
 *
 * The last entry index is cached for fast entry search.
 *
 * @param ip6addr the destination address
 * @param netif the netif on which the packet will be sent
 * @return the neighbor cache entry for the next hop, ERR_RTE if no
 *         suitable next hop was found, ERR_MEM if no cache entry
 *         could be created
 */
static s8_t nd6_get_next_hop_entry(const ip6_addr_t *ip6addr, struct netif *netif)
{
#ifdef LWIP_HOOK_ND6_GET_GW
	const ip6_addr_t *next_hop_addr;
#endif							/* LWIP_HOOK_ND6_GET_GW */
	s8_t i;
	s8_t unreachable_router_index = -1;

#if LWIP_NETIF_HWADDRHINT
	if (netif->addr_hint != NULL) {
		/* per-pcb cached entry was given */
		u8_t addr_hint = *(netif->addr_hint);
		if (addr_hint < LWIP_ND6_NUM_DESTINATIONS) {
			nd6_cached_destination_index = addr_hint;
		}
	}
#endif							/* LWIP_NETIF_HWADDRHINT */

	/* Look for ip6addr in destination cache. */
	if (ip6_addr_cmp(ip6addr, &(destination_cache[nd6_cached_destination_index].destination_addr))) {
		/* the cached entry index is the right one! */
		/* do nothing. */
		ND6_STATS_INC(nd6.cachehit);
	} else {
		/* Search destination cache. */
		i = nd6_find_destination_cache_entry(ip6addr);
		if (i >= 0) {
			/* found destination entry. make it our new cached index. */
			nd6_cached_destination_index = i;
			/* destination address in destination cache can be off-link address */
			if (!ip6_addr_islinklocal(ip6addr) && !nd6_is_prefix_in_netif(ip6addr, netif)) {
				i = nd6_get_router(&destination_cache[nd6_cached_destination_index].next_hop_addr, netif);
				if (i > 0) {
					LWIP_DEBUGF(ND6_DEBUG, ("Found router, send through default router index %d\n", i));
				} else {
					LWIP_DEBUGF(ND6_DEBUG, ("Failed to find router, need to NUD\n"));
				}
			}
		} else {
			/* Not found. Create a new destination entry. */
			i = nd6_new_destination_cache_entry();
			if (i >= 0) {
				/* got new destination entry. make it our new cached index. */
				nd6_cached_destination_index = i;
			} else {
				/* Could not create a destination cache entry. */
				return ERR_MEM;
			}

			/* Copy dest address to destination cache. */
			ip6_addr_set(&(destination_cache[nd6_cached_destination_index].destination_addr), ip6addr);

			/* Now find the next hop. is it a neighbor? */
			if (ip6_addr_islinklocal(ip6addr) || nd6_is_prefix_in_netif(ip6addr, netif)) {
				/* Destination in local link. */
				destination_cache[nd6_cached_destination_index].pmtu = netif->mtu;
				destination_cache[nd6_cached_destination_index].pmtu_timer = 0;
				ip6_addr_copy(destination_cache[nd6_cached_destination_index].next_hop_addr, destination_cache[nd6_cached_destination_index].destination_addr);
#ifdef LWIP_HOOK_ND6_GET_GW
			} else if ((next_hop_addr = LWIP_HOOK_ND6_GET_GW(netif, ip6addr)) != NULL) {
				/* Next hop for destination provided by hook function. */
				destination_cache[nd6_cached_destination_index].pmtu = netif->mtu;
				destination_cache[nd6_cached_destination_index].pmtu_timer = 0;
				ip6_addr_set(&destination_cache[nd6_cached_destination_index].next_hop_addr, next_hop_addr);
#endif							/* LWIP_HOOK_ND6_GET_GW */
			} else {
				/* We need to select a router. */
				i = nd6_select_router(ip6addr, netif);
				if (i < 0) {
					i = nd6_select_unreachable_default_router();
					if (i < 0) {
						/* No router found. */
						LWIP_DEBUGF(ND6_DEBUG, ("Failed to find default router, ERR_RTE"));
						ip6_addr_set_any(&(destination_cache[nd6_cached_destination_index].destination_addr));
						return ERR_RTE;
					} else {
						LWIP_DEBUGF(ND6_DEBUG, ("Found suspicious router in default router list, index %d", i));
						destination_cache[nd6_cached_destination_index].pmtu = netif->mtu;	/* Start with netif mtu, correct through ICMPv6 if necessary */
						destination_cache[nd6_cached_destination_index].pmtu_timer = 0;
						ip6_addr_copy(destination_cache[nd6_cached_destination_index].next_hop_addr, default_router_list[i].router_ip6addr);
						unreachable_router_index = i;
					}
				} else {
					LWIP_DEBUGF(ND6_DEBUG, ("Set next-hop address as default router address, index %d", i));
					destination_cache[nd6_cached_destination_index].pmtu = netif->mtu;	/* Start with netif mtu, correct through ICMPv6 if necessary */
					destination_cache[nd6_cached_destination_index].pmtu_timer = 0;
					ip6_addr_copy(destination_cache[nd6_cached_destination_index].next_hop_addr, default_router_list[i].neighbor_entry->next_hop_address);
				}
			}
		}
	}

#if LWIP_NETIF_HWADDRHINT
	if (netif->addr_hint != NULL) {
		/* per-pcb cached entry was given */
		*(netif->addr_hint) = nd6_cached_destination_index;
	}
#endif							/* LWIP_NETIF_HWADDRHINT */

	/* Look in neighbor cache for the next-hop address. */
	if (ip6_addr_cmp(&(destination_cache[nd6_cached_destination_index].next_hop_addr), &(neighbor_cache[nd6_cached_neighbor_index].next_hop_address))) {
		/* Cache hit. */
		/* Do nothing. */
		ND6_STATS_INC(nd6.cachehit);
	} else {
		i = nd6_find_neighbor_cache_entry(&(destination_cache[nd6_cached_destination_index].next_hop_addr));
		if (i >= 0) {
			/* Found a matching record, make it new cached entry. */
			nd6_cached_neighbor_index = i;
		} else {
			/* Neighbor not in cache. Make a new entry. */
			i = nd6_new_neighbor_cache_entry();
			if (i >= 0) {
				/* got new neighbor entry. make it our new cached index. */
				nd6_cached_neighbor_index = i;
			} else {
				/* Could not create a neighbor cache entry. */
				return ERR_MEM;
			}

			/* Initialize fields. */
			ip6_addr_copy(neighbor_cache[i].next_hop_address, destination_cache[nd6_cached_destination_index].next_hop_addr);
			neighbor_cache[i].isrouter = 0;
			neighbor_cache[i].netif = netif;
			neighbor_cache[i].state = ND6_INCOMPLETE;
			neighbor_cache[i].counter.incomplete_time = 0;
			neighbor_cache[i].probes_sent = 0;
		}
	}

	/* Reset this destination's age. */
	destination_cache[nd6_cached_destination_index].age = 0;

	if (unreachable_router_index >= 0
			&& default_router_list[unreachable_router_index].neighbor_entry == NULL) {
		default_router_list[unreachable_router_index].neighbor_entry = &neighbor_cache[nd6_cached_neighbor_index];
	}

	return nd6_cached_neighbor_index;
}

/**
 * Queue a packet for a neighbor.
 *
 * @param neighbor_index the index in the neighbor cache table
 * @param q packet to be queued
 * @return ERR_OK if succeeded, ERR_MEM if out of memory
 */
static err_t nd6_queue_packet(s8_t neighbor_index, struct pbuf *q)
{
	err_t result = ERR_MEM;
	struct pbuf *p;
	int copy_needed = 0;
#if LWIP_ND6_QUEUEING
	struct nd6_q_entry *new_entry, *r;
#endif							/* LWIP_ND6_QUEUEING */

	if ((neighbor_index < 0) || (neighbor_index >= LWIP_ND6_NUM_NEIGHBORS)) {
		return ERR_ARG;
	}

	/* IF q includes a PBUF_REF, PBUF_POOL or PBUF_RAM, we have no choice but
	 * to copy the whole queue into a new PBUF_RAM (see bug #11400)
	 * PBUF_ROMs can be left as they are, since ROM must not get changed. */
	p = q;
	while (p) {
		if (p->type != PBUF_ROM) {
			copy_needed = 1;
			break;
		}
		p = p->next;
	}
	if (copy_needed) {
		/* copy the whole packet into new pbufs */
		p = pbuf_alloc(PBUF_LINK, q->tot_len, PBUF_RAM);
		while ((p == NULL) && (neighbor_cache[neighbor_index].q != NULL)) {
			/* Free oldest packet (as per RFC recommendation) */
#if LWIP_ND6_QUEUEING
			r = neighbor_cache[neighbor_index].q;
			neighbor_cache[neighbor_index].q = r->next;
			r->next = NULL;
			nd6_free_q(r);
#else							/* LWIP_ND6_QUEUEING */
			pbuf_free(neighbor_cache[neighbor_index].q);
			neighbor_cache[neighbor_index].q = NULL;
#endif							/* LWIP_ND6_QUEUEING */
			p = pbuf_alloc(PBUF_LINK, q->tot_len, PBUF_RAM);
		}
		if (p != NULL) {
			if (pbuf_copy(p, q) != ERR_OK) {
				pbuf_free(p);
				p = NULL;
			}
		}
	} else {
		/* referencing the old pbuf is enough */
		p = q;
		pbuf_ref(p);
	}
	/* packet was copied/ref'd? */
	if (p != NULL) {
		/* queue packet ... */
#if LWIP_ND6_QUEUEING
		/* allocate a new nd6 queue entry */
		new_entry = (struct nd6_q_entry *)memp_malloc(MEMP_ND6_QUEUE);
		if ((new_entry == NULL) && (neighbor_cache[neighbor_index].q != NULL)) {
			/* Free oldest packet (as per RFC recommendation) */
			r = neighbor_cache[neighbor_index].q;
			neighbor_cache[neighbor_index].q = r->next;
			r->next = NULL;
			nd6_free_q(r);
			new_entry = (struct nd6_q_entry *)memp_malloc(MEMP_ND6_QUEUE);
		}
		if (new_entry != NULL) {
			new_entry->next = NULL;
			new_entry->p = p;
			if (neighbor_cache[neighbor_index].q != NULL) {
				/* queue was already existent, append the new entry to the end */
				r = neighbor_cache[neighbor_index].q;
				while (r->next != NULL) {
					r = r->next;
				}
				r->next = new_entry;
			} else {
				/* queue did not exist, first item in queue */
				neighbor_cache[neighbor_index].q = new_entry;
			}
			LWIP_DEBUGF(LWIP_DBG_TRACE, ("ipv6: queued packet %p on neighbor entry %" S16_F "\n", (void *)p, (s16_t) neighbor_index));
			result = ERR_OK;
		} else {
			/* the pool MEMP_ND6_QUEUE is empty */
			pbuf_free(p);
			LWIP_DEBUGF(LWIP_DBG_TRACE, ("ipv6: could not queue a copy of packet %p (out of memory)\n", (void *)p));
			/* { result == ERR_MEM } through initialization */
		}
#else							/* LWIP_ND6_QUEUEING */
		/* Queue a single packet. If an older packet is already queued, free it as per RFC. */
		if (neighbor_cache[neighbor_index].q != NULL) {
			pbuf_free(neighbor_cache[neighbor_index].q);
		}
		neighbor_cache[neighbor_index].q = p;
		LWIP_DEBUGF(LWIP_DBG_TRACE, ("ipv6: queued packet %p on neighbor entry %" S16_F "\n", (void *)p, (s16_t) neighbor_index));
		result = ERR_OK;
#endif							/* LWIP_ND6_QUEUEING */
	} else {
		LWIP_DEBUGF(LWIP_DBG_TRACE, ("ipv6: could not queue a copy of packet %p (out of memory)\n", (void *)q));
		/* { result == ERR_MEM } through initialization */
	}

	return result;
}

#if LWIP_ND6_QUEUEING
/**
 * Free a complete queue of nd6 q entries
 *
 * @param q a queue of nd6_q_entry to free
 */
static void nd6_free_q(struct nd6_q_entry *q)
{
	struct nd6_q_entry *r;
	LWIP_ASSERT("q != NULL", q != NULL);
	LWIP_ASSERT("q->p != NULL", q->p != NULL);
	while (q) {
		r = q;
		q = q->next;
		LWIP_ASSERT("r->p != NULL", (r->p != NULL));
		pbuf_free(r->p);
		memp_free(MEMP_ND6_QUEUE, r);
	}
}
#endif							/* LWIP_ND6_QUEUEING */

/**
 * Send queued packets for a neighbor
 *
 * @param i the neighbor to send packets to
 */
static void nd6_send_q(s8_t i)
{
	struct ip6_hdr *ip6hdr;
	ip6_addr_t dest;
#if LWIP_ND6_QUEUEING
	struct nd6_q_entry *q;
#endif							/* LWIP_ND6_QUEUEING */

	if ((i < 0) || (i >= LWIP_ND6_NUM_NEIGHBORS)) {
		return;
	}
#if LWIP_ND6_QUEUEING
	while (neighbor_cache[i].q != NULL) {
		/* remember first in queue */
		q = neighbor_cache[i].q;
		/* pop first item off the queue */
		neighbor_cache[i].q = q->next;
		/* Get ipv6 header. */
		ip6hdr = (struct ip6_hdr *)(q->p->payload);
		/* Create an aligned copy. */
		ip6_addr_set(&dest, &(ip6hdr->dest));
		/* send the queued IPv6 packet */
		(neighbor_cache[i].netif)->output_ip6(neighbor_cache[i].netif, q->p, &dest);
		/* free the queued IP packet */
		pbuf_free(q->p);
		/* now queue entry can be freed */
		memp_free(MEMP_ND6_QUEUE, q);
	}
#else							/* LWIP_ND6_QUEUEING */
	if (neighbor_cache[i].q != NULL) {
		/* Get ipv6 header. */
		ip6hdr = (struct ip6_hdr *)(neighbor_cache[i].q->payload);
		/* Create an aligned copy. */
		ip6_addr_set(&dest, &(ip6hdr->dest));
		/* send the queued IPv6 packet */
		(neighbor_cache[i].netif)->output_ip6(neighbor_cache[i].netif, neighbor_cache[i].q, &dest);
		/* free the queued IP packet */
		pbuf_free(neighbor_cache[i].q);
		neighbor_cache[i].q = NULL;
	}
#endif							/* LWIP_ND6_QUEUEING */
}

/**
 * Check validation of received Prefix lifetime value
 *
 * @param prefix_entry Pointer of the entry in prefix_list for comparing current value of invalidation timer
 * @param lifetime_ms Received option value of lifetime for validation check (time unit MUST be ms)
 *
 * @return 1 if the lifetime is valid, 0  if the lifetime is invalid
 *
 */

static u8_t nd6_prefix_lifetime_isvalid(const struct nd6_prefix_list_entry *prefix_entry, u32_t lifetime_ms)
{
	if (prefix_entry == NULL)
		return 0;

	if ((lifetime_ms > prefix_entry->invalidation_timer) ||
	    (lifetime_ms > TIME_HOUR_TO_MS(2)) ||
	    (lifetime_ms == 0)) {
		return 1;
	}

	return 0;
}

/**
 * A packet is to be transmitted to a specific IPv6 destination on a specific
 * interface. Check if we can find the hardware address of the next hop to use
 * for the packet. If so, give the hardware address to the caller, which should
 * use it to send the packet right away. Otherwise, enqueue the packet for
 * later transmission while looking up the hardware address, if possible.
 *
 * As such, this function returns one of three different possible results:
 *
 * - ERR_OK with a non-NULL 'hwaddrp': the caller should send the packet now.
 * - ERR_OK with a NULL 'hwaddrp': the packet has been enqueued for later.
 * - not ERR_OK: something went wrong; forward the error upward in the stack.
 *
 * @param netif The lwIP network interface on which the IP packet will be sent.
 * @param q The pbuf(s) containing the IP packet to be sent.
 * @param ip6addr The destination IPv6 address of the packet.
 * @param hwaddrp On success, filled with a pointer to a HW address or NULL (meaning
 *        the packet has been queued).
 * @return
 * - ERR_OK on success, ERR_RTE if no route was found for the packet,
 * or ERR_MEM if low memory conditions prohibit sending the packet at all.
 */
err_t nd6_get_next_hop_addr_or_queue(struct netif *netif, struct pbuf *q, const ip6_addr_t *ip6addr, const u8_t **hwaddrp)
{
	s8_t i;

	/* Get next hop record. */
	i = nd6_get_next_hop_entry(ip6addr, netif);
	if (i < 0) {
		/* failed to get a next hop neighbor record. */
		return i;
	}

	/* Now that we have a destination record, send or queue the packet. */
	if (neighbor_cache[i].state == ND6_STALE) {
		/* RFC 7.3.3.
		 * The first time a node sends a packet to a neighbor whose entry is
		 * STALE, the sender changes the state to DELAY and sets a timer to
		 * expire in DELAY_FIRST_PROBE_TIME seconds. */
		*hwaddrp = neighbor_cache[i].lladdr;
		neighbor_cache[i].state = ND6_DELAY;
		neighbor_cache[i].counter.delay_time = LWIP_ND6_DELAY_FIRST_PROBE_TIME;
		return ERR_OK;
	}
	/* @todo should we send or queue if PROBE? send for now, to let unicast NS pass. */
	if ((neighbor_cache[i].state == ND6_REACHABLE) || (neighbor_cache[i].state == ND6_DELAY) || (neighbor_cache[i].state == ND6_PROBE)) {

		/* Tell the caller to send out the packet now. */
		*hwaddrp = neighbor_cache[i].lladdr;
		return ERR_OK;
	}

	/* We should queue packet on this interface. */
	*hwaddrp = NULL;
	return nd6_queue_packet(i, q);
}

/**
 * Get the Path MTU for a destination.
 *
 * @param ip6addr the destination address
 * @param netif the netif on which the packet will be sent
 * @return the Path MTU, if known, or the netif default MTU
 */
u16_t nd6_get_destination_mtu(const ip6_addr_t *ip6addr, struct netif *netif)
{
	s8_t i;

	i = nd6_find_destination_cache_entry(ip6addr);
	if (i >= 0) {
		if (destination_cache[i].pmtu > 0) {
			return destination_cache[i].pmtu;
		}
	}

	if (netif != NULL) {
		return netif->mtu;
	}

	return 1280;				/* Minimum MTU */
}

#if LWIP_ND6_TCP_REACHABILITY_HINTS
/**
 * Provide the Neighbor discovery process with a hint that a
 * destination is reachable. Called by tcp_receive when ACKs are
 * received or sent (as per RFC). This is useful to avoid sending
 * NS messages every 30 seconds.
 *
 * @param ip6addr the destination address which is know to be reachable
 *                by an upper layer protocol (TCP)
 */
void nd6_reachability_hint(const ip6_addr_t *ip6addr)
{
	s8_t i;

	/* Find destination in cache. */
	if (ip6_addr_cmp(ip6addr, &(destination_cache[nd6_cached_destination_index].destination_addr))) {
		i = nd6_cached_destination_index;
		ND6_STATS_INC(nd6.cachehit);
	} else {
		i = nd6_find_destination_cache_entry(ip6addr);
	}
	if (i < 0) {
		return;
	}

	/* Find next hop neighbor in cache. */
	if (ip6_addr_cmp(&(destination_cache[i].next_hop_addr), &(neighbor_cache[nd6_cached_neighbor_index].next_hop_address))) {
		i = nd6_cached_neighbor_index;
		ND6_STATS_INC(nd6.cachehit);
	} else {
		i = nd6_find_neighbor_cache_entry(&(destination_cache[i].next_hop_addr));
	}
	if (i < 0) {
		return;
	}

	/* For safety: don't set as reachable if we don't have a LL address yet. Misuse protection. */
	if (neighbor_cache[i].state == ND6_INCOMPLETE || neighbor_cache[i].state == ND6_NO_ENTRY) {
		return;
	}

	/* Set reachability state. */
	neighbor_cache[i].state = ND6_REACHABLE;
	neighbor_cache[i].counter.reachable_time = reachable_time;
}
#endif							/* LWIP_ND6_TCP_REACHABILITY_HINTS */

/**
 * Remove all prefix, neighbor_cache and router entries of the specified netif.
 *
 * @param netif points to a network interface
 */
void nd6_cleanup_netif(struct netif *netif)
{
	u8_t i;
	s8_t router_index;
	for (i = 0; i < LWIP_ND6_NUM_PREFIXES; i++) {
		if (prefix_list[i].netif == netif) {
			prefix_list[i].netif = NULL;
			prefix_list[i].flags = 0;
		}
	}
	for (i = 0; i < LWIP_ND6_NUM_NEIGHBORS; i++) {
		if (neighbor_cache[i].netif == netif) {
			for (router_index = 0; router_index < LWIP_ND6_NUM_ROUTERS; router_index++) {
				if (default_router_list[router_index].neighbor_entry == &neighbor_cache[i]) {
					default_router_list[router_index].neighbor_entry = NULL;
					default_router_list[router_index].flags = 0;
					ip6_addr_set_any(&default_router_list[router_index].router_ip6addr);
				}
			}
			neighbor_cache[i].isrouter = 0;
			nd6_free_neighbor_cache_entry(i);
		}
	}
}

/**
 * Remove all destination, neighbor_cache and router entries of the specified invalid address.
 *
 * @param invalid_ip6addr points to an invalid next-hop address (e.g., expired, un-reached address)
 */
static void nd6_cleanup_all_caches(ip6_addr_t *invalid_ip6addr)
{
	s8_t i;
	struct nd6_neighbor_cache_entry *nentry;

	if (ip6_addr_isany(invalid_ip6addr)) {
		return;
	}

	LWIP_DEBUGF(ND6_DEBUG, ("Clean-up caches including %4x:%4x:%4x:%4x:%4x:%4x:%4x:%4x (invalid addr)\n", IP6_ADDR_BLOCK1(invalid_ip6addr), IP6_ADDR_BLOCK2(invalid_ip6addr), IP6_ADDR_BLOCK3(invalid_ip6addr), IP6_ADDR_BLOCK4(invalid_ip6addr), IP6_ADDR_BLOCK5(invalid_ip6addr), IP6_ADDR_BLOCK6(invalid_ip6addr), IP6_ADDR_BLOCK7(invalid_ip6addr), IP6_ADDR_BLOCK8(invalid_ip6addr)));

	/* Clean-up destination cache */
	nd6_free_expired_router_in_destination_cache(invalid_ip6addr);

	/* Clean-up default router list */
	for (i = 0; i < LWIP_ND6_NUM_ROUTERS; i++) {
		if (default_router_list[i].neighbor_entry != NULL) {
			nentry = default_router_list[i].neighbor_entry;
			if (ip6_addr_cmp(invalid_ip6addr, &(nentry->next_hop_address))) {
				default_router_list[i].neighbor_entry = NULL;
				default_router_list[i].flags = 0;
				ip6_addr_set_any(&default_router_list[i].router_ip6addr);
			}
		}
	}

	/* Clean-up neighbor cache list */
	for (i = 0; i < LWIP_ND6_NUM_NEIGHBORS; i++) {
		if (ip6_addr_cmp(invalid_ip6addr, &(neighbor_cache[i].next_hop_address))) {
			nd6_free_neighbor_cache_entry(i);
		}
	}
}
#if LWIP_IPV6_MLD
/**
 * The state of a local IPv6 address entry is about to change. If needed, join
 * or leave the solicited-node multicast group for the address.
 *
 * @param netif The netif that owns the address.
 * @param addr_idx The index of the address.
 * @param new_state The new (IP6_ADDR_) state for the address.
 */
void nd6_adjust_mld_membership(struct netif *netif, s8_t addr_idx, u8_t new_state)
{
	u8_t old_state, old_member, new_member;

	old_state = netif_ip6_addr_state(netif, addr_idx);

	/* Determine whether we were, and should be, a member of the solicited-node
	 * multicast group for this address. For tentative addresses, the group is
	 * not joined until the address enters the TENTATIVE_1 (or VALID) state. */
	old_member = (old_state != IP6_ADDR_INVALID && old_state != IP6_ADDR_TENTATIVE);
	new_member = (new_state != IP6_ADDR_INVALID && new_state != IP6_ADDR_TENTATIVE);

	if (old_member != new_member) {
		ip6_addr_set_solicitednode(&multicast_address, netif_ip6_addr(netif, addr_idx)->addr[3]);

		if (new_member) {
			mld6_joingroup_netif(netif, &multicast_address);
		} else {
			mld6_leavegroup_netif(netif, &multicast_address);
		}
	}
}
#endif							/* LWIP_IPV6_MLD */

#if ND6_DEBUG
/**
 * Print all neighbor cache entities and destination cache entities
 */
void nd6_cache_debug_print(void)
{
	int i;

	LWIP_DEBUGF(ND6_DEBUG, ("+--------------------------------------------------------+\n"));
	LWIP_DEBUGF(ND6_DEBUG, ("+ neighbor_cache\n"));
	LWIP_DEBUGF(ND6_DEBUG, ("+--------------------------------------------------------+\n"));

	for (i = 0; i < LWIP_ND6_NUM_NEIGHBORS; i++) {
		LWIP_DEBUGF(ND6_DEBUG, ("next-hop addr   %4x %4x %4x %4x %4x %4x %4x %4x\n", IP6_ADDR_BLOCK1(&neighbor_cache[i].next_hop_address), IP6_ADDR_BLOCK2(&neighbor_cache[i].next_hop_address), IP6_ADDR_BLOCK3(&neighbor_cache[i].next_hop_address), IP6_ADDR_BLOCK4(&neighbor_cache[i].next_hop_address), IP6_ADDR_BLOCK5(&neighbor_cache[i].next_hop_address), IP6_ADDR_BLOCK6(&neighbor_cache[i].next_hop_address), IP6_ADDR_BLOCK7(&neighbor_cache[i].next_hop_address), IP6_ADDR_BLOCK8(&neighbor_cache[i].next_hop_address)));
		LWIP_DEBUGF(ND6_DEBUG, ("lladdr %2x:%2x:%2x:%2x:%2x:%2x | state %2u | isrouter %2u\n",  neighbor_cache[i].lladdr[0], neighbor_cache[i].lladdr[1], neighbor_cache[i].lladdr[2], neighbor_cache[i].lladdr[3], neighbor_cache[i].lladdr[4], neighbor_cache[i].lladdr[5], neighbor_cache[i].state, neighbor_cache[i].isrouter));
		LWIP_DEBUGF(ND6_DEBUG, ("incomplete time %lu | reachable time %lu | delay time %lu | probe time %lu\n", neighbor_cache[i].counter.incomplete_time, neighbor_cache[i].counter.reachable_time, neighbor_cache[i].counter.delay_time, neighbor_cache[i].counter.probe_time));
		LWIP_DEBUGF(ND6_DEBUG, ("+--------------------------------------------------------+\n"));
	}

	LWIP_DEBUGF(ND6_DEBUG, ("+--------------------------------------------------------+\n"));
	LWIP_DEBUGF(ND6_DEBUG, ("+ destination_cache\n"));
	LWIP_DEBUGF(ND6_DEBUG, ("+--------------------------------------------------------+\n"));

	for (i = 0; i < LWIP_ND6_NUM_DESTINATIONS; i++) {
		LWIP_DEBUGF(ND6_DEBUG, ("desination_addr %4x %4x %4x %4x %4x %4x %4x %4x\n", IP6_ADDR_BLOCK1(&destination_cache[i].destination_addr), IP6_ADDR_BLOCK2(&destination_cache[i].destination_addr), IP6_ADDR_BLOCK3(&destination_cache[i].destination_addr), IP6_ADDR_BLOCK4(&destination_cache[i].destination_addr), IP6_ADDR_BLOCK5(&destination_cache[i].destination_addr), IP6_ADDR_BLOCK6(&destination_cache[i].destination_addr), IP6_ADDR_BLOCK7(&destination_cache[i].destination_addr), IP6_ADDR_BLOCK8(&destination_cache[i].destination_addr)));
		LWIP_DEBUGF(ND6_DEBUG, ("next-hop addr   %4x %4x %4x %4x %4x %4x %4x %4x\n", IP6_ADDR_BLOCK1(&destination_cache[i].next_hop_addr), IP6_ADDR_BLOCK2(&destination_cache[i].next_hop_addr), IP6_ADDR_BLOCK3(&destination_cache[i].next_hop_addr), IP6_ADDR_BLOCK4(&destination_cache[i].next_hop_addr), IP6_ADDR_BLOCK5(&destination_cache[i].next_hop_addr), IP6_ADDR_BLOCK6(&destination_cache[i].next_hop_addr), IP6_ADDR_BLOCK7(&destination_cache[i].next_hop_addr), IP6_ADDR_BLOCK8(&destination_cache[i].next_hop_addr)));
		LWIP_DEBUGF(ND6_DEBUG, ("pmtu %2u | pmtu_timer %u | age %4lu\n", destination_cache[i].pmtu, destination_cache[i].pmtu_timer, destination_cache[i].age));
		LWIP_DEBUGF(ND6_DEBUG, ("+--------------------------------------------------------+\n"));
	}
}
#endif /* ND6_DEBUG */

#endif							/* LWIP_IPV6 */
