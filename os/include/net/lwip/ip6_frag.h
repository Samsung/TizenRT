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
#ifndef LWIP_HDR_IP6_FRAG_H
#define LWIP_HDR_IP6_FRAG_H

#include <net/lwip/opt.h>
#include <net/lwip/pbuf.h>
#include <net/lwip/ip6_addr.h>
#include <net/lwip/ipv6/ip6.h>
#include <net/lwip/netif.h>

#ifdef __cplusplus
extern "C" {
#endif

#if LWIP_IPV6 && LWIP_IPV6_REASS  /* don't build if not configured for use in lwipopts.h */

/** IP6_FRAG_COPYHEADER==1: for platforms where sizeof(void*) > 4, this needs to
 * be enabled (to not overwrite part of the data). When enabled, the IPv6 header
 * is copied instead of referencing it, which gives more room for struct ip6_reass_helper */
#ifndef IPV6_FRAG_COPYHEADER
#define IPV6_FRAG_COPYHEADER   0
#endif

/** The IPv6 reassembly timer interval in milliseconds. */
#define IP6_REASS_TMR_INTERVAL 1000

/* Copy the complete header of the first fragment to struct ip6_reassdata
   or just point to its original location in the first pbuf? */
#if IPV6_FRAG_COPYHEADER
#define IPV6_FRAG_HDRPTR
#define IPV6_FRAG_HDRREF(hdr) (&(hdr))
#else	/* IPV6_FRAG_COPYHEADER */
#define IPV6_FRAG_HDRPTR *
#define IPV6_FRAG_HDRREF(hdr) (hdr)
#endif	/* IPV6_FRAG_COPYHEADER */

/** IPv6 reassembly helper struct.
 * This is exported because memp needs to know the size.
 */
struct ip6_reassdata {
	struct ip6_reassdata *next;
	struct pbuf *p;
	struct ip6_hdr IPV6_FRAG_HDRPTR iphdr;
	u32_t identification;
	u16_t datagram_len;
	u8_t nexth;
	u8_t timer;
};

#define ip6_reass_init() /* Compatibility define */
void ip6_reass_tmr(void);
struct pbuf *ip6_reass(struct pbuf *p);

#endif	/* LWIP_IPV6 && LWIP_IPV6_REASS */

#if LWIP_IPV6 && LWIP_IPV6_FRAG  /* don't build if not configured for use in lwipopts.h */

#ifndef LWIP_PBUF_CUSTOM_REF_DEFINED
#define LWIP_PBUF_CUSTOM_REF_DEFINED
/** A custom pbuf that holds a reference to another pbuf, which is freed
 * when this custom pbuf is freed. This is used to create a custom PBUF_REF
 * that points into the original pbuf. */
struct pbuf_custom_ref {
	/** 'base class' */
	struct pbuf_custom pc;
	/** pointer to the original pbuf that is referenced */
	struct pbuf *original;
};
#endif	/* LWIP_PBUF_CUSTOM_REF_DEFINED */

err_t ip6_frag(struct pbuf *p, struct netif *netif, const ip6_addr_t *dest);

#endif	/* LWIP_IPV6 && LWIP_IPV6_FRAG */


#ifdef __cplusplus
}
#endif

#endif	/* LWIP_HDR_IP6_FRAG_H */
