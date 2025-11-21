/**
 * @file
 * This is the IPv4 layer implementation for incoming and outgoing IP traffic.
 *
 * @see ip_frag.c
 *
 */

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

#include "lwip/opt.h"

/* TODO : To Check Copyright related inputs from HQ/RTK */
#if LWIP_IPV4

#include "lwip/ip.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/ip4_frag.h"
#include "lwip/inet_chksum.h"
#include "lwip/netif.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/icmp.h"
#include "lwip/igmp.h"
#include "lwip/raw.h"
#include "lwip/udp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/autoip.h"
#include "lwip/stats.h"
#include "lwip/sys.h"
#include "lwip/timeouts.h"
#include "lwip/lwip_ipnat.h"
#include "lwip/prot/dhcp.h"

#include <string.h>
#include "debug.h"

#define configTICK_RATE_HZ								( 1000 )

#if defined(IP_FORWARD) && (IP_FORWARD == 1)
#if defined(CONFIG_ENABLE_HOMELYNK) && (CONFIG_ENABLE_HOMELYNK == 1)
void nat_debug_print(void);
static u16_t ip_nat_add_entry(u8_t proto, u32_t src, u16_t sport, u32_t dest, u16_t dport, u32_t app_use);
static struct nat_table *ip_nat_entry_search(u8_t proto, u32_t addr, u16_t port, u16_t portmap, u8_t dest, u8_t frag, u32_t daddr, u16_t dportmap);

#define NON_INDEX (0xFFFF)

struct nat_table {
	u32_t src;
	u32_t dest;
	u32_t ts;
	u16_t sport;
	u16_t dport;
	u16_t portmap;
	u8_t proto;
	u8_t fin_wait1: 1;
	u8_t fin_wait2: 1;
	u8_t fin_ack1: 1;
	u8_t fin_ack2: 1;
	u8_t syn_acked: 1;
	u8_t rst: 1;
	u16_t next, prev;
	u32_t app_use;
	u32_t pkt_count;
};

u16_t nat_entry_list = NON_INDEX;
u16_t nat_entry_list_last = NON_INDEX;
u16_t nat_entry_idle = 0;
sys_mutex_t nat_entry_lock;
static struct nat_table *ip_nat_table;

uint32_t filter_drop_threshold = 0;
uint32_t tcp_entry_count = 0;
uint32_t udp_entry_count = 0;
uint32_t icmp_entry_count = 0;
uint16_t ip_nat_max_entry = 0;

uint32_t ip_nat_tcp_max_timeout = IP_NAT_MAX_TIMEOUT_MS_TCP;
uint32_t ip_nat_udp_max_timeout = IP_NAT_MAX_TIMEOUT_MS_UDP;

static inline uint32_t GET_NAT_ENTRY_TIME_ELAPSED(uint32_t now_ts, uint32_t e_ts)
{
	if (now_ts >= e_ts) {
		return ((now_ts) - (e_ts));
	} else {
		return (((now_ts) + ((e_ts) ^ (0xFFFFFFFF / (configTICK_RATE_HZ / 1000))) + 1));
	}
}

static inline struct nat_table *GET_NAT_ENTRY(u16_t en_idx)
{
	if (en_idx == NON_INDEX) {
		return NULL;
	} else {
		return &ip_nat_table[en_idx];
	}
}

void ip_nat_reinitialize(void)
{
	int i;
	sys_mutex_lock(&nat_entry_lock);
	memset(ip_nat_table, 0x00, sizeof(struct nat_table) * IP_NAT_MAX);
	for (i = 0; i < IP_NAT_MAX - 1; i++) {
		ip_nat_table[i].next = i + 1;
	}
	ip_nat_table[i].next = NON_INDEX;
	nat_entry_idle = 0;
	nat_entry_list = NON_INDEX;
	nat_entry_list_last = NON_INDEX;
	tcp_entry_count = 0;
	udp_entry_count = 0;
	icmp_entry_count = 0;
	sys_mutex_unlock(&nat_entry_lock);
}

#if 0
static void ipnat_ageing_tmr(void *arg)
{
	int total_session = 0;
	sys_mutex_lock(&nat_entry_lock);
	total_session = tcp_entry_count + udp_entry_count + icmp_entry_count;

	if (total_session > 0 && total_session < 50) {
		//nat_debug_print();
	} else {
		//printf("\n\r total :%d %d %d", tcp_entry_count, udp_entry_count, icmp_entry_count);
	}
	sys_mutex_unlock(&nat_entry_lock);
	sys_timeout((30 * 1000), ipnat_ageing_tmr, arg);
}
#endif

/* TODO : Need to handle malloc failure? Pass info to upper layer */
void ip_nat_initialize(void)
{
	int i;
	sys_mutex_new(&nat_entry_lock);
	filter_drop_threshold = (IP_NAT_MAX * 80) / 100;
	int size = sizeof(struct nat_table) * IP_NAT_MAX;
	ip_nat_table = (struct nat_table *)kmm_malloc(size);
	if (!ip_nat_table) {
		LWIP_DEBUGF(IPNAT_DEBUG | LWIP_DBG_LEVEL_SERIOUS, ("%s:%d Memory allocation failed for size=%u Bytes\n", __FUNCTION__, __LINE__, size));
		return;
	}
	memset(ip_nat_table, 0x00, size);
	for (i = 0; i < IP_NAT_MAX - 1; i++) {
		ip_nat_table[i].next = i + 1;
	}
	ip_nat_table[i].next = NON_INDEX;
	nat_entry_idle = 0;
	tcp_entry_count = 0;
	udp_entry_count = 0;
	icmp_entry_count = 0;
//sys_timeout((15*1000), ipnat_ageing_tmr, NULL);
}

void ip_nat_deinitialize(void)
{
	sys_mutex_lock(&nat_entry_lock);
	if (ip_nat_table) {
		kmm_free(ip_nat_table);
		ip_nat_table = NULL;
	}
	sys_mutex_unlock(&nat_entry_lock);
	sys_mutex_free(&nat_entry_lock);
}

static void ip_nat_insert_new_rule(struct nat_table *rule_entry)
{
	u16_t ti = rule_entry - ip_nat_table;

	if (ti != nat_entry_idle) {
		LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d %d node is not idle\n", __FUNCTION__, __LINE__,ti));
	}
	nat_entry_idle = rule_entry->next;
	rule_entry->prev = NON_INDEX;
	rule_entry->next = nat_entry_list;

	if (nat_entry_list != NON_INDEX) {
		GET_NAT_ENTRY(nat_entry_list)->prev = ti;
	}
	nat_entry_list = ti;
	if (nat_entry_list_last == NON_INDEX) {
		nat_entry_list_last = ti;
	}

#if LWIP_TCP
	if (rule_entry->proto == IP_PROTO_TCP) {
		tcp_entry_count++;
	}
#endif
#if LWIP_UDP
	if (rule_entry->proto == IP_PROTO_UDP) {
		udp_entry_count++;
	}
#endif
#if LWIP_ICMP
	if (rule_entry->proto == IP_PROTO_ICMP) {
		icmp_entry_count++;
	}
#endif
//	LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d INSERT proto=%d TCP=%d UDP=%d ICMP=%d\n", __FUNCTION__, __LINE__,rule_entry->proto, tcp_entry_count, udp_entry_count, icmp_entry_count));
}

static void ip_nat_set_entry_idle(struct nat_table *t)
{
	u16_t ti = t - ip_nat_table;
	if (ti == nat_entry_list) {
		nat_entry_list = t->next;
	}
	if (ti == nat_entry_list_last) {
		nat_entry_list_last = t->prev;
	}
	if (t->next != NON_INDEX) {
		GET_NAT_ENTRY(t->next)->prev = t->prev;
	}
	if (t->prev != NON_INDEX) {
		GET_NAT_ENTRY(t->prev)->next = t->next;
	}
	t->prev = NON_INDEX;
	t->next = nat_entry_idle;
	nat_entry_idle = ti;

#if LWIP_TCP
	if (t->proto == IP_PROTO_TCP) {
		tcp_entry_count--;
	}
#endif

#if LWIP_UDP
	if (t->proto == IP_PROTO_UDP) {
		udp_entry_count--;
	}
#endif

#if LWIP_ICMP
	if (t->proto == IP_PROTO_ICMP) {
		icmp_entry_count--;
	}
#endif

}

static struct nat_table *ip_nat_entry_search(u8_t proto, u32_t addr, u16_t port, u16_t portmap, u8_t dest, u8_t frag, u32_t daddr, u16_t dportmap)
{
	u16_t i, next;
	struct nat_table *NEntry;
//	LWIP_DEBUGF(IPNAT_DEBUG, ("searching in table %s: %" U16_F ".%" U16_F ".%" U16_F ".%" U16_F ", port: %d, mport: %d\n", (dest ? "dest" : "src"), ip4_addr1_16((ip4_addr_t *)&addr), ip4_addr2_16((ip4_addr_t *)&addr), ip4_addr3_16((ip4_addr_t *)&addr), ip4_addr4_16((ip4_addr_t *)&addr), PP_HTONS(port), PP_HTONS(portmap)));

	u32_t now = sys_now();

	for (i = nat_entry_list; i != NON_INDEX; i = next) {
		NEntry = GET_NAT_ENTRY(i);
		next = NEntry->next;

#if LWIP_TCP
		if (NEntry->proto == IP_PROTO_TCP && ((((NEntry->fin_ack1 && NEntry->fin_ack2) || !NEntry->syn_acked) && GET_NAT_ENTRY_TIME_ELAPSED(now, NEntry->ts) > IP_NAT_MAX_TIMEOUT_MS_TCP_DISCON) || GET_NAT_ENTRY_TIME_ELAPSED(now, NEntry->ts) > ip_nat_tcp_max_timeout)) {
			if (!NEntry->syn_acked && (GET_NAT_ENTRY_TIME_ELAPSED(now, NEntry->ts) > IP_NAT_MAX_TIMEOUT_MS_TCP_DISCON)) {
				ip_nat_set_entry_idle(NEntry);
				continue;
			}
			if (NEntry->fin_ack1 && NEntry->fin_ack2 && GET_NAT_ENTRY_TIME_ELAPSED(now, NEntry->ts) > IP_NAT_MAX_TIMEOUT_MS_TCP_FIN_WAIT) {
				ip_nat_set_entry_idle(NEntry);
				continue;
			}
			if (GET_NAT_ENTRY_TIME_ELAPSED(now, NEntry->ts) > ip_nat_tcp_max_timeout) {
				ip_nat_set_entry_idle(NEntry);
				continue;
			}

		}
		if (NEntry->proto == IP_PROTO_TCP && NEntry->rst && (GET_NAT_ENTRY_TIME_ELAPSED(now, NEntry->ts) > IP_NAT_MAX_TIMEOUT_MS_TCP_RST_DISCON)) {
			ip_nat_set_entry_idle(NEntry);
			continue;
		}
#endif

#if LWIP_UDP
		if (NEntry->proto == IP_PROTO_UDP && GET_NAT_ENTRY_TIME_ELAPSED(now, NEntry->ts) > ip_nat_udp_max_timeout) {

			if (NEntry->app_use == 0) {
				ip_nat_set_entry_idle(NEntry);
			} else if (NEntry->app_use == 1) {
				if (NEntry->proto == IP_PROTO_UDP && GET_NAT_ENTRY_TIME_ELAPSED(now, NEntry->ts) > IP_NAT_MAX_TIMEOUT_MS_UDP_ALG) {
					ip_nat_set_entry_idle(NEntry);
				}
			}
			continue;
		}
#endif

#if LWIP_ICMP
		if (NEntry->proto == IP_PROTO_ICMP) {
			if (GET_NAT_ENTRY_TIME_ELAPSED(now, NEntry->ts) > IP_NAT_MAX_TIMEOUT_MS_ICMP) {
				ip_nat_set_entry_idle(NEntry);
				continue;
			}
		}
#endif

		if (frag == 0 && dest == 0 && NEntry->proto == proto && NEntry->src == addr && NEntry->sport == port) {
			NEntry->ts = now;
			NEntry->pkt_count++;
			return NEntry;
		}
		if (frag == 0 && dest == 0 && NEntry->proto == proto && NEntry->src == addr) {
			if (daddr != 0x00 && NEntry->dest == daddr && dportmap != 0 && NEntry->dport == dportmap) {
				NEntry->ts = now;
				NEntry->pkt_count++;
				return NEntry;
			}
		}

		if (frag == 0 && dest == 1 && NEntry->proto == proto && NEntry->dest == addr) {
			if (daddr != 0x00 && NEntry->dest == daddr && dportmap != 0 && NEntry->dport == port) {
				NEntry->ts = now;
				NEntry->pkt_count++;
				return NEntry;
			}
		}
		if (frag == 0 && dest == 1 && NEntry->proto == proto && NEntry->dest == addr && NEntry->dport == port && NEntry->portmap == portmap) {
			NEntry->ts = now;
			NEntry->pkt_count++;
			return NEntry;
		}
		if (frag == 0 && dest == 1 && NEntry->proto == proto && NEntry->dest == addr && NEntry->dport == port) {
			if (portmap > 0 && NEntry->portmap == portmap) {
				NEntry->ts = now;
				NEntry->pkt_count++;
				return NEntry;
			} else if (portmap == 0) {
				NEntry->ts = now;
				NEntry->pkt_count++;
				return NEntry;
			} else {
				continue;
			}
		}
		if (frag == 1 && dest == 0 && NEntry->proto == proto && NEntry->src == addr && NEntry->dest == daddr) {
			NEntry->ts = now;
			NEntry->pkt_count++;
			return NEntry;
		}
		if (frag == 1 && dest == 1 && NEntry->proto == proto && NEntry->dest == addr) {
			NEntry->ts = now;
			NEntry->pkt_count++;
			return NEntry;
		}
	}
	return NULL;
}

void filter_old_nat_entry(unsigned int threshold_value)
{
	u16_t i, next;
	int filter_count = 0;
	struct nat_table *NEntry;
	u32_t now = sys_now();

	for (i = nat_entry_list; i != NON_INDEX; i = next) {
		NEntry = GET_NAT_ENTRY(i);
		next = NEntry->next;
		if (GET_NAT_ENTRY_TIME_ELAPSED(now, NEntry->ts) > threshold_value) {
			if (NEntry->proto == IP_PROTO_TCP) {
				if ((NEntry->fin_ack1 == 1 && NEntry->fin_ack2) || NEntry->rst == 1) {
					ip_nat_set_entry_idle(NEntry);
				}
			} else {
				ip_nat_set_entry_idle(NEntry);
			}
			filter_count++;
		}
	}
}

static u16_t ip_nat_add_entry(u8_t proto, u32_t src, u16_t sport, u32_t dest, u16_t dport, u32_t app_use)
{

	struct nat_table *NEntry;

	if ((tcp_entry_count + udp_entry_count + icmp_entry_count) > filter_drop_threshold) {
		filter_old_nat_entry(IP_NAT_MAX_TIMEOUT_MS_FILTER_DROP);
	}

	NEntry = ip_nat_entry_search(proto, src, sport, 0, 0, 0, 0, 0);
	if (NEntry) {
		NEntry->ts = sys_now();
		NEntry->dest = dest;
		NEntry->dport = dport;
		NEntry->app_use = app_use;

		ip_nat_set_entry_idle(NEntry);
		ip_nat_insert_new_rule(NEntry);

		return NEntry->portmap;
	}

	NEntry = GET_NAT_ENTRY(nat_entry_idle);
	if (NEntry) {
		u16_t portmap = sport;

		NEntry->ts = sys_now();
		NEntry->src = src;
		NEntry->dest = dest;
		NEntry->sport = sport;
		NEntry->dport = dport;
		NEntry->portmap = portmap;
		NEntry->proto = proto;
		NEntry->fin_wait1 = 0;
		NEntry->fin_wait2 = 0;
		NEntry->fin_ack1 = 0;
		NEntry->fin_ack2 = 0;
		NEntry->syn_acked = 0;
		NEntry->rst = 0;
		NEntry->app_use = app_use;
		ip_nat_insert_new_rule(NEntry);
		return portmap;
	}
	filter_old_nat_entry((IP_NAT_MAX_TIMEOUT_MS_FILTER_DROP - 20000));
	return 0;
}

#if LWIP_TCP
void ip_nat_set_tcp_timeout(u32_t secs)
{
	ip_nat_tcp_max_timeout = secs * 1000;
}

void ip_nat_manipulate_address_tcp(struct tcp_hdr *tcphdr, ip4_addr_p_t *field, u32_t newval)
{
	u16_t local_csum = tcphdr->chksum;
	u32_t local_target = field->addr;
	u32_t new_target = newval;
	unsigned char *optr = (unsigned char *)&local_target;
	unsigned char *nptr = (unsigned char *)&new_target;
	int olen = 4;
	int nlen = 4;

	unsigned char *chksum = (unsigned char *)&local_csum;
	long x, old, new;
	x = chksum[0] * 256 + chksum[1];
	x = ~x & 0xFFFF;
	while (olen) {
		old = optr[0] * 256 + optr[1];
		optr += 2;
		x -= old & 0xffff;
		if (x <= 0) {
			x--;
			x &= 0xffff;
		}
		olen -= 2;
	}
	while (nlen) {
		new = nptr[0] * 256 + nptr[1];
		nptr += 2;
		x += new & 0xffff;
		if (x & 0x10000) {
			x++;
			x &= 0xffff;
		}
		nlen -= 2;
	}
	x = ~x & 0xFFFF;
	chksum[0] = x / 256;
	chksum[1] = x & 0xff;
	tcphdr->chksum = local_csum;

}
#endif							// LWIP_TCP

#if LWIP_UDP
void ip_napt_set_udp_timeout(u32_t secs)
{
	ip_nat_udp_max_timeout = secs * 1000;
}

void ip_nat_manipulate_address_udp(struct udp_hdr *udphdr, ip4_addr_p_t *field, u32_t newval)
{

	u16_t local_csum = udphdr->chksum;
	u32_t local_target = field->addr;
	u32_t new_target = newval;
	unsigned char *optr = (unsigned char *)&local_target;
	unsigned char *nptr = (unsigned char *)&new_target;
	int olen = 4;
	int nlen = 4;

	unsigned char *chksum = (unsigned char *)&local_csum;
	long x, old, new;
	x = chksum[0] * 256 + chksum[1];
	x = ~x & 0xFFFF;
	while (olen) {
		old = optr[0] * 256 + optr[1];
		optr += 2;
		x -= old & 0xffff;
		if (x <= 0) {
			x--;
			x &= 0xffff;
		}
		olen -= 2;
	}
	while (nlen) {
		new = nptr[0] * 256 + nptr[1];
		nptr += 2;
		x += new & 0xffff;
		if (x & 0x10000) {
			x++;
			x &= 0xffff;
		}
		nlen -= 2;
	}
	x = ~x & 0xFFFF;
	chksum[0] = x / 256;
	chksum[1] = x & 0xff;
	udphdr->chksum = local_csum;

}
#endif							// LWIP_UDP

void ip_nat_manipulate_address(struct ip_hdr *iphdr, ip4_addr_p_t *field, u32_t newval)
{
	u16_t local_csum = IPH_CHKSUM(iphdr);
	u32_t local_target = field->addr;
	u32_t new_target = newval;
	unsigned char *optr = (unsigned char *)&local_target;
	unsigned char *nptr = (unsigned char *)&new_target;
	int olen = 4;
	int nlen = 4;

	//LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d Original target IP: %s\n",__FUNCTION__,__LINE__,inet_ntoa(*field)));
	//LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d New target IP: %s\n",__FUNCTION__,__LINE__,inet_ntoa(*((struct in_addr *)&newval))));

	if (1) {
		unsigned char *chksum = (unsigned char *)&local_csum;
		long x, old, new;
		x = chksum[0] * 256 + chksum[1];
		x = ~x & 0xFFFF;
		while (olen) {
			old = optr[0] * 256 + optr[1];
			optr += 2;
			x -= old & 0xffff;
			if (x <= 0) {
				x--;
				x &= 0xffff;
			}
			olen -= 2;
		}
		while (nlen) {
			new = nptr[0] * 256 + nptr[1];
			nptr += 2;
			x += new & 0xffff;
			if (x & 0x10000) {
				x++;
				x &= 0xffff;
			}
			nlen -= 2;
		}
		x = ~x & 0xFFFF;
		chksum[0] = x / 256;
		chksum[1] = x & 0xff;

		IPH_CHKSUM(iphdr) = local_csum;
		field->addr = newval;
	}
}

static void ip_nat_rx_packet(struct pbuf *p, struct ip_hdr *iphdr)
{
	struct nat_table *NEntry;
	u16_t frag_offset;
#if LWIP_ICMP
	if (IPH_PROTO(iphdr) == IP_PROTO_ICMP) {
		frag_offset = lwip_ntohs(iphdr->_offset) & IP_OFFMASK;
		struct icmp_echo_hdr *iecho = (struct icmp_echo_hdr *)((u8_t *) p->payload + IPH_HL(iphdr) * 4);
		if (frag_offset == 0 && iecho->type == ICMP_ER) {
			sys_mutex_lock(&nat_entry_lock);
			NEntry = ip_nat_entry_search(IP_PROTO_ICMP, iphdr->src.addr, iecho->id, iecho->id, 1, 0, 0, 0);
			if (!NEntry) {
				sys_mutex_unlock(&nat_entry_lock);
				return;
			}
			ip_nat_manipulate_address(iphdr, &iphdr->dest, NEntry->src);
			sys_mutex_unlock(&nat_entry_lock);
			return;
		} else if ((IPH_OFFSET(iphdr) & PP_HTONS(IP_OFFMASK | IP_MF)) != 0) {
			sys_mutex_lock(&nat_entry_lock);
			NEntry = ip_nat_entry_search(IP_PROTO_ICMP, iphdr->src.addr, 0, 0, 1, 1, 0, 0);

			if (NEntry) {
				ip_nat_manipulate_address(iphdr, &iphdr->dest, NEntry->src);
			} else {
				LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d ICMP ip_nat_rx_packet NOT found\n",__FUNCTION__,__LINE__));
			}
			sys_mutex_unlock(&nat_entry_lock);
		}

		return;
	}
#endif /* LWIP_ICMP */

#if LWIP_TCP
	if (IPH_PROTO(iphdr) == IP_PROTO_TCP) {
		struct tcp_hdr *tcphdr = (struct tcp_hdr *)((u8_t *) p->payload + IPH_HL(iphdr) * 4);
		sys_mutex_lock(&nat_entry_lock);
		NEntry = ip_nat_entry_search(IP_PROTO_TCP, iphdr->src.addr, tcphdr->src, tcphdr->dest, 1, 0, 0, 0);
		if (!NEntry) {
			sys_mutex_unlock(&nat_entry_lock);
			return;
		}

		if (NEntry->rst == 1) {
			//LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d CLEAN TCP RST!!!!\n",__FUNCTION__,__LINE__));
			NEntry->rst = 0;
		}
		ip_nat_manipulate_address_tcp(tcphdr, &iphdr->dest, NEntry->src);
		ip_nat_manipulate_address(iphdr, &iphdr->dest, NEntry->src);

		if ((TCPH_FLAGS(tcphdr) & (TCP_SYN | TCP_ACK)) == (TCP_SYN | TCP_ACK)) {
			NEntry->syn_acked = 1;
		}

		if ((TCPH_FLAGS(tcphdr) & TCP_FIN)) {
			NEntry->fin_wait1 = 1;
		}

		if (NEntry->fin_wait2 && (TCPH_FLAGS(tcphdr) & TCP_ACK)) {
			NEntry->fin_ack2 = 1;
		}

		if (TCPH_FLAGS(tcphdr) & TCP_RST) {
			NEntry->rst = 1;
		} else {
			NEntry->rst = 0;
		}

		sys_mutex_unlock(&nat_entry_lock);
		return;
	}
#endif /* LWIP_TCP */

#if LWIP_UDP
	if (IPH_PROTO(iphdr) == IP_PROTO_UDP) {
		struct udp_hdr *udphdr = (struct udp_hdr *)((u8_t *) p->payload + IPH_HL(iphdr) * 4);
		frag_offset = lwip_ntohs(iphdr->_offset) & IP_OFFMASK;

		if (frag_offset == 0) {

			sys_mutex_lock(&nat_entry_lock);

			NEntry = ip_nat_entry_search(IP_PROTO_UDP, iphdr->src.addr, udphdr->src, udphdr->dest, 1, 0, 0, 0);

			if (!NEntry) {
				sys_mutex_unlock(&nat_entry_lock);
				return;
			}

			ip_nat_manipulate_address_udp(udphdr, &iphdr->dest, NEntry->src);
			ip_nat_manipulate_address(iphdr, &iphdr->dest, NEntry->src);
			sys_mutex_unlock(&nat_entry_lock);
			return;
		} else if ((IPH_OFFSET(iphdr) & PP_HTONS(IP_OFFMASK | IP_MF)) != 0) {

			sys_mutex_lock(&nat_entry_lock);
			NEntry = ip_nat_entry_search(IP_PROTO_UDP, iphdr->src.addr, 0, 0, 1, 1, 0, 0);
			if (NEntry) {
				ip_nat_manipulate_address(iphdr, &iphdr->dest, NEntry->src);
			} else {
				//LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d UDP ip_nat_rx_packet NOT found!!!\n",__FUNCTION__,__LINE__));
			}
			sys_mutex_unlock(&nat_entry_lock);
		}
	}
#endif							// LWIP_UDP

}

err_t ip_nat_forward_packet(struct pbuf *p, struct ip_hdr *iphdr, struct netif *input_iface, struct netif *output_iface)
{
	(void)input_iface;
	struct nat_table *NEntry;
	struct nat_table *NEntry1;

	u16_t frag_offset;
#if LWIP_ICMP

	if (IPH_PROTO(iphdr) == IP_PROTO_ICMP) {
		frag_offset = lwip_ntohs(iphdr->_offset) & IP_OFFMASK;
		struct icmp_echo_hdr *iecho = (struct icmp_echo_hdr *)((u8_t *) p->payload + IPH_HL(iphdr) * 4);

		if (frag_offset == 0 && iecho->type == ICMP_ECHO) {
			sys_mutex_lock(&nat_entry_lock);
			ip_nat_add_entry(IP_PROTO_ICMP, iphdr->src.addr, iecho->id, iphdr->dest.addr, iecho->id, 0);
			sys_mutex_unlock(&nat_entry_lock);
			ip_nat_manipulate_address(iphdr, &iphdr->src, output_iface->ip_addr.u_addr.ip4.addr);
		} else if ((IPH_OFFSET(iphdr) & PP_HTONS(IP_OFFMASK | IP_MF)) != 0) {
			sys_mutex_lock(&nat_entry_lock);
			NEntry = ip_nat_entry_search(IP_PROTO_ICMP, iphdr->src.addr, 0, 0, 0, 1, iphdr->dest.addr, 0);

			if (NEntry) {
				ip_nat_manipulate_address(iphdr, &iphdr->src, output_iface->ip_addr.u_addr.ip4.addr);
			} else {
				//LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d ICMP ip_nat_forward_packet NOT found!!!\n",__FUNCTION__,__LINE__));
			}
			sys_mutex_unlock(&nat_entry_lock);

		} else {
			//LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d ICMP type =%d ip_nat_forward_packet DO NOTthing!!\n",__FUNCTION__,__LINE__,iecho->type));
		}

		//ip4_debug_print(p);
		return ERR_OK;
	}
#endif

#if LWIP_TCP
	if (IPH_PROTO(iphdr) == IP_PROTO_TCP) {
		struct tcp_hdr *tcphdr = (struct tcp_hdr *)((u8_t *) p->payload + IPH_HL(iphdr) * 4);
		if ((TCPH_FLAGS(tcphdr) & (TCP_SYN | TCP_ACK)) == TCP_SYN) {
			sys_mutex_lock(&nat_entry_lock);
			ip_nat_add_entry(IP_PROTO_TCP, iphdr->src.addr, tcphdr->src, iphdr->dest.addr, tcphdr->dest, 0);
			sys_mutex_unlock(&nat_entry_lock);

			ip_nat_manipulate_address_tcp(tcphdr, &iphdr->src, output_iface->ip_addr.u_addr.ip4.addr);
			ip_nat_manipulate_address(iphdr, &iphdr->src, output_iface->ip_addr.u_addr.ip4.addr);
		} else {

			sys_mutex_lock(&nat_entry_lock);
			NEntry = ip_nat_entry_search(IP_PROTO_TCP, iphdr->src.addr, tcphdr->src, 0, 0, 0, 0, 0);

			if (!NEntry) {
				NEntry = ip_nat_entry_search(IP_PROTO_TCP, iphdr->src.addr, tcphdr->src, tcphdr->dest, 1, 0, 0, 0);
				if (NEntry) {
					//printf("\n\r %s %d TCP NAT RX has update DNAT --Do Nothing %08X sport=%d ~~~%08X dport=%d",__FUNCTION__, __LINE__, iphdr->src.addr, lwip_ntohs(tcphdr->src), iphdr->dest.addr, lwip_ntohs(tcphdr->dest));
					sys_mutex_unlock(&nat_entry_lock);
				} else {
#if LWIP_ICMP
					NEntry1 = ip_nat_entry_search(IP_PROTO_TCP, iphdr->src.addr, tcphdr->src, 0, 0, 0, iphdr->dest.addr, tcphdr->dest);
					if (NEntry1) {
						ip_nat_set_entry_idle(NEntry1);
						NEntry1->src = iphdr->src.addr;
						NEntry1->sport = tcphdr->src;
						NEntry1->portmap = tcphdr->src;
						NEntry1->ts = sys_now();
						NEntry1->dest = iphdr->dest.addr;
						NEntry1->dport = tcphdr->dest;
						NEntry1->app_use = 0;

						ip_nat_insert_new_rule(NEntry1);
						sys_mutex_unlock(&nat_entry_lock);
						return ERR_OK;
					} else {
						icmp_dest_unreach(p, ICMP_DUR_PORT);
					}
#endif
					sys_mutex_unlock(&nat_entry_lock);
					LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d return ERR_RTE\n", __FUNCTION__, __LINE__));
					return ERR_RTE;	/* Drop unknown TCP session */
				}

			} else {
				if (NEntry->dest != iphdr->dest.addr || NEntry->dport != tcphdr->dest) {

#if LWIP_ICMP
					icmp_dest_unreach(p, ICMP_DUR_PORT);
#endif
					sys_mutex_unlock(&nat_entry_lock);
					LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d return ERR_RTE\n", __FUNCTION__, __LINE__));
					return ERR_RTE;
				}

				if ((TCPH_FLAGS(tcphdr) & TCP_FIN)) {
					NEntry->fin_wait2 = 1;
				}

				if (NEntry->fin_wait1 && (TCPH_FLAGS(tcphdr) & TCP_ACK)) {
					NEntry->fin_ack1 = 1;
				}

				if (TCPH_FLAGS(tcphdr) & TCP_RST) {
					NEntry->rst = 1;
				} else {
					NEntry->rst = 0;
				}

				sys_mutex_unlock(&nat_entry_lock);

				ip_nat_manipulate_address_tcp(tcphdr, &iphdr->src, output_iface->ip_addr.u_addr.ip4.addr);
				ip_nat_manipulate_address(iphdr, &iphdr->src, output_iface->ip_addr.u_addr.ip4.addr);
			}
			return ERR_OK;
		}
	}
#endif

#if LWIP_UDP
	if (IPH_PROTO(iphdr) == IP_PROTO_UDP) {
		struct udp_hdr *udphdr = (struct udp_hdr *)((u8_t *) p->payload + IPH_HL(iphdr) * 4);

//    struct nat_table *NEntry1;
		frag_offset = lwip_ntohs(iphdr->_offset) & IP_OFFMASK;
		if (frag_offset == 0) {

			sys_mutex_lock(&nat_entry_lock);

			NEntry = ip_nat_entry_search(IP_PROTO_UDP, iphdr->src.addr, udphdr->src, 0, 0, 0, 0, 0);
			if (!NEntry) {

				NEntry1 = ip_nat_entry_search(IP_PROTO_UDP, iphdr->src.addr, udphdr->src, udphdr->dest, 1, 0, 0, 0);
				if (NEntry1) {

					if (NEntry1->src == iphdr->dest.addr) {
						//   printf("\n\r %s %d NAT RX has update DNAT --Do Nothing ",__FUNCTION__, __LINE__);
					} else {
						//printf("\n\r %s %d NAT RX has NOT update forward update %X",__FUNCTION__, __LINE__,NEntry1->src);
					}
					sys_mutex_unlock(&nat_entry_lock);
				} else {
					ip_nat_add_entry(IP_PROTO_UDP, iphdr->src.addr, udphdr->src, iphdr->dest.addr, udphdr->dest, 0);
					sys_mutex_unlock(&nat_entry_lock);

					ip_nat_manipulate_address_udp(udphdr, &iphdr->src, output_iface->ip_addr.u_addr.ip4.addr);
					ip_nat_manipulate_address(iphdr, &iphdr->src, output_iface->ip_addr.u_addr.ip4.addr);
				}
			} else {
				if (NEntry->dest != iphdr->dest.addr || NEntry->dport != udphdr->dest) {

					if (NEntry->src == iphdr->src.addr && NEntry->sport == udphdr->src) {

						ip_nat_set_entry_idle(NEntry);
						NEntry->src = iphdr->src.addr;
						NEntry->sport = udphdr->src;
						NEntry->portmap = udphdr->src;
						NEntry->ts = sys_now();
						NEntry->dest = iphdr->dest.addr;
						NEntry->dport = udphdr->dest;
						NEntry->app_use = 0;
						//LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d GO INSERT NEntry prev=%d next=%d\n", __FUNCTION__, __LINE__,NEntry->prev, NEntry->next));
						ip_nat_insert_new_rule(NEntry);

					} else {

#if LWIP_ICMP
						//LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d icmp_dest_unreach\n", __FUNCTION__, __LINE__));
						icmp_dest_unreach(p, ICMP_DUR_PORT);
#endif

						sys_mutex_unlock(&nat_entry_lock);
						LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d return ERR_RTE\n", __FUNCTION__, __LINE__));
						return ERR_RTE;
					}
				}
				sys_mutex_unlock(&nat_entry_lock);

				ip_nat_manipulate_address_udp(udphdr, &iphdr->src, output_iface->ip_addr.u_addr.ip4.addr);
				ip_nat_manipulate_address(iphdr, &iphdr->src, output_iface->ip_addr.u_addr.ip4.addr);
			}
		} else if ((IPH_OFFSET(iphdr) & PP_HTONS(IP_OFFMASK | IP_MF)) != 0) {

			sys_mutex_lock(&nat_entry_lock);

			NEntry = ip_nat_entry_search(IP_PROTO_UDP, iphdr->src.addr, 0, 0, 0, 1, iphdr->dest.addr, 0);
			if (NEntry) {
				ip_nat_manipulate_address(iphdr, &iphdr->src, output_iface->ip_addr.u_addr.ip4.addr);
			} else {
				LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d UDP frag ip_nat_forward_packet NOT found!!!\n", __FUNCTION__, __LINE__));
			}
			sys_mutex_unlock(&nat_entry_lock);
		} else {
		}
		return ERR_OK;
	}
#endif

	return ERR_OK;
}

err_t ip_nat_transfer(struct pbuf *p, struct netif *src, struct netif *target)
{
	struct ip_hdr *iphdr;
	u16_t iphdr_hlen;

	err_t result = ERR_OK;
	iphdr = (struct ip_hdr *)p->payload;

	iphdr_hlen = IPH_HL(iphdr);
	iphdr_hlen *= 4;

	if ((iphdr->dest.addr & PP_HTONL(0xf0000000UL)) == PP_HTONL(0xe0000000UL)) {
		return ERR_OK;
	}
	result = ip_nat_forward_packet(p, iphdr, src, target);

	return result;
}

err_t ip_nat_enqueue(struct pbuf *p, struct netif *inp)
{
	struct ip_hdr *iphdr;
	u16_t iphdr_hlen;
	iphdr = (struct ip_hdr *)p->payload;
	iphdr_hlen = IPH_HL(iphdr);
	iphdr_hlen *= 4;

	if ((iphdr->dest.addr & PP_HTONL(0xf0000000UL)) == PP_HTONL(0xe0000000UL)) {
		return ERR_OK;
	}

	if (ip4_addr_isbroadcast_u32(iphdr->dest.addr, inp)) {
		return ERR_OK;
	}

	//LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d dest:=0x%x inp=0x%x\n", __FUNCTION__, __LINE__, (iphdr->dest).addr, (inp->ip_addr).u_addr.ip4.addr));
	if (ip4_addr_cmp(&iphdr->dest, &((inp->ip_addr).u_addr.ip4))) {
		ip_nat_rx_packet(p, iphdr);
	}
	return ERR_OK;

}

void ipnat_dump(void)
{
	int total_session = 0;
	sys_mutex_lock(&nat_entry_lock);
	total_session = tcp_entry_count + udp_entry_count + icmp_entry_count;
	if (total_session > 0) {
		nat_debug_print();
	}
	sys_mutex_unlock(&nat_entry_lock);
}

void nat_debug_print(void)
{

	int i, next;
	u32_t now = sys_now();
	LWIP_DEBUGF(IPNAT_DEBUG, ("%s:%d NAT session table:\n", __FUNCTION__, __LINE__));	
	LWIP_DEBUGF(IPNAT_DEBUG, (" sa                      da                      sport   dport   proto    pkts    ts\n"));	
	LWIP_DEBUGF(IPNAT_DEBUG, ("+-----------------------+-----------------------+-------+-------+-----+-------+---------+\n"));
	for (i = nat_entry_list; i != NON_INDEX; i = next) {
		struct nat_table *Entry = &ip_nat_table[i];
		next = Entry->next;
		LWIP_DEBUGF(IPNAT_DEBUG, ("| %3" U16_F " | %3" U16_F " | %3" U16_F " | %3" U16_F " |", ip4_addr1_16((ip4_addr_t *)&Entry->src), ip4_addr2_16((ip4_addr_t *)&Entry->src), ip4_addr3_16((ip4_addr_t *)&Entry->src), ip4_addr4_16((ip4_addr_t *)&Entry->src)));
		LWIP_DEBUGF(IPNAT_DEBUG, (" %3" U16_F " | %3" U16_F " | %3" U16_F " | %3" U16_F " |", ip4_addr1_16((ip4_addr_t *)&Entry->dest), ip4_addr2_16((ip4_addr_t *)&Entry->dest), ip4_addr3_16((ip4_addr_t *)&Entry->dest), ip4_addr4_16((ip4_addr_t *)&Entry->dest)));
		//printf(" %5u | %5u | %3u | %5u | %5lu\n", PP_HTONS(Entry->sport), PP_HTONS(Entry->dport), Entry->proto, Entry->pkt_count, GET_NAT_ENTRY_TIME_ELAPSED(now, Entry->ts));
	}
}
#endif							/* CONFIG_ENABLE_HOMELYNK */
#endif

#endif							/* LWIP_IPV4 */
