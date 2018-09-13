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
 * ND6 protocol definitions
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
#ifndef LWIP_HDR_PROT_ND6_H
#define LWIP_HDR_PROT_ND6_H

#include <net/lwip/arch.h>
#include <net/lwip/ip6_addr.h>
#include <net/lwip/prot/ip6.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Common Macros for Neighbor Discovery headers **/
#define ND6H_TYPE(hdr) ((hdr)->_type)
#define ND6H_CODE(hdr) ((hdr)->_code)
#define ND6H_CHKSUM(hdr) ((hdr)->_chksum)
#define ND6H_RESERV(hdr) ((hdr)->reserved)

/** Neighbor solicitation message header. */
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct ns_header {
	PACK_STRUCT_FLD_8(u8_t _type);
	PACK_STRUCT_FLD_8(u8_t _code);
	PACK_STRUCT_FIELD(u16_t _chksum);
	PACK_STRUCT_FIELD(u32_t reserved);
	PACK_STRUCT_FLD_S(ip6_addr_p_t _target_address);
	/* Options follow. */
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif
#define ND6H_NS_TARGET_ADDR(hdr) ((hdr)->_target_address)

/** Neighbor advertisement message header. */
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct na_header {
	PACK_STRUCT_FLD_8(u8_t _type);
	PACK_STRUCT_FLD_8(u8_t _code);
	PACK_STRUCT_FIELD(u16_t _chksum);
	PACK_STRUCT_FLD_8(u8_t _flags);
	PACK_STRUCT_FLD_8(u8_t reserved[3]);
	PACK_STRUCT_FLD_S(ip6_addr_p_t _target_address);
	/* Options follow. */
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif
#define ND6H_NA_FLAG(hdr) ((hdr)->_flags)
#define ND6H_NA_TARGET_ADDR(hdr) ((hdr)->_target_address)

#define ND6_FLAG_ROUTER      (0x80)
#define ND6_FLAG_SOLICITED   (0x40)
#define ND6_FLAG_OVERRIDE    (0x20)
/** Router solicitation message header. */
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct rs_header {
	PACK_STRUCT_FLD_8(u8_t _type);
	PACK_STRUCT_FLD_8(u8_t _code);
	PACK_STRUCT_FIELD(u16_t _chksum);
	PACK_STRUCT_FIELD(u32_t reserved);
	/* Options follow. */
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif

/** Router advertisement message header. */
#define ND6_RA_FLAG_MANAGED_ADDR_CONFIG (0x80)
#define ND6_RA_FLAG_OTHER_CONFIG (0x40)
#define ND6_RA_FLAG_HOME_AGENT (0x20)
#define ND6_RA_PREFERENCE_MASK (0x18)
#define ND6_RA_PREFERENCE_HIGH (0x08)
#define ND6_RA_PREFERENCE_MEDIUM (0x00)
#define ND6_RA_PREFERENCE_LOW (0x18)
#define ND6_RA_PREFERENCE_DISABLED (0x10)
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct ra_header {
	PACK_STRUCT_FLD_8(u8_t _type);
	PACK_STRUCT_FLD_8(u8_t _code);
	PACK_STRUCT_FIELD(u16_t _chksum);
	PACK_STRUCT_FLD_8(u8_t _current_hop_limit);
	PACK_STRUCT_FLD_8(u8_t _flags);
	PACK_STRUCT_FIELD(u16_t _router_lifetime);
	PACK_STRUCT_FIELD(u32_t _reachable_time);
	PACK_STRUCT_FIELD(u32_t _retrans_timer);
	/* Options follow. */
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif
#define ND6H_RA_HOPLIM(hdr) ((hdr)->_current_hop_limit)
#define ND6H_RA_FLAG(hdr) ((hdr)->_flags)
#define ND6H_RA_ROUT_LIFE(hdr) ((hdr)->_router_lifetime)
#define ND6H_RA_REACH_TIME(hdr) ((hdr)->_reachable_time)
#define ND6H_RA_RETRANS_TMR(hdr) ((hdr)->_retrans_timer)

/** Redirect message header. */
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct redirect_header {
	PACK_STRUCT_FLD_8(u8_t _type);
	PACK_STRUCT_FLD_8(u8_t _code);
	PACK_STRUCT_FIELD(u16_t _chksum);
	PACK_STRUCT_FIELD(u32_t reserved);
	PACK_STRUCT_FLD_S(ip6_addr_p_t _target_address);
	PACK_STRUCT_FLD_S(ip6_addr_p_t _destination_address);
	/* Options follow. */
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif
#define ND6H_RD_TARGET_ADDR(hdr) ((hdr)->_target_address)
#define ND6H_RD_DEST_ADDR(hdr) ((hdr)->_destination_address)

/** Link-layer address option. */
#define ND6_OPTION_TYPE_SOURCE_LLADDR (0x01)
#define ND6_OPTION_TYPE_TARGET_LLADDR (0x02)
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct lladdr_option {
	PACK_STRUCT_FLD_8(u8_t _type);
	PACK_STRUCT_FLD_8(u8_t _length);
	PACK_STRUCT_FLD_8(u8_t _addr[NETIF_MAX_HWADDR_LEN]);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif
#define ND6H_LLADDR_OPT_TYPE(hdr) ((hdr)->_type)
#define ND6H_LLADDR_OPT_LEN(hdr) ((hdr)->_length)
#define ND6H_LLADDR_OPT_ADDR(hdr) ((hdr)->_addr)

/** Prefix information option. */
#define ND6_OPTION_TYPE_PREFIX_INFO (0x03)
#define ND6_PREFIX_FLAG_ON_LINK (0x80)
#define ND6_PREFIX_FLAG_AUTONOMOUS (0x40)
#define ND6_PREFIX_FLAG_ROUTER_ADDRESS (0x20)
#define ND6_PREFIX_FLAG_SITE_PREFIX (0x10)
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct prefix_option {
	PACK_STRUCT_FLD_8(u8_t _type);
	PACK_STRUCT_FLD_8(u8_t _length);
	PACK_STRUCT_FLD_8(u8_t _prefix_length);
	PACK_STRUCT_FLD_8(u8_t _flags);
	PACK_STRUCT_FIELD(u32_t _valid_lifetime);
	PACK_STRUCT_FIELD(u32_t _preferred_lifetime);
	PACK_STRUCT_FLD_8(u8_t reserved2[3]);
	PACK_STRUCT_FLD_8(u8_t _site_prefix_length);
	PACK_STRUCT_FLD_S(ip6_addr_p_t _prefix);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif
#define ND6H_PF_OPT_TYPE(hdr) ((hdr)->_type)
#define ND6H_PF_OPT_LEN(hdr) ((hdr)->_length)
#define ND6H_PF_OPT_PF_LEN(hdr) ((hdr)->_prefix_length)
#define ND6H_PF_OPT_FLAG(hdr) ((hdr)->_flags)
#define ND6H_PF_OPT_VAL_LIFE(hdr) ((hdr)->_valid_lifetime)
#define ND6H_PF_OPT_PREFER_LIFE(hdr) ((hdr)->_preferred_lifetime)
#define ND6H_PF_OPT_SITE_PF_LEN(hdr) ((hdr)->_site_prefix_length)
#define ND6H_PF_OPT_PF(hdr) ((hdr)->_prefix)

/** Redirected header option. */
#define ND6_OPTION_TYPE_REDIR_HDR (0x04)
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct redirected_header_option {
	PACK_STRUCT_FLD_8(u8_t _type);
	PACK_STRUCT_FLD_8(u8_t _length);
	PACK_STRUCT_FLD_8(u8_t reserved[6]);
	/* Portion of redirected packet follows. */
	/* PACK_STRUCT_FLD_8(u8_t redirected[8]); */
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif
#define ND6H_RD_OPT_TYPE(hdr) ((hdr)->_type)
#define ND6H_RD_OPT_LEN(hdr) ((hdr)->_length)

/** MTU option. */
#define ND6_OPTION_TYPE_MTU (0x05)
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct mtu_option {
	PACK_STRUCT_FLD_8(u8_t _type);
	PACK_STRUCT_FLD_8(u8_t _length);
	PACK_STRUCT_FIELD(u16_t reserved);
	PACK_STRUCT_FIELD(u32_t _mtu);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif
#define ND6H_MTU_OPT_TYPE(hdr) ((hdr)->_type)
#define ND6H_MTU_OPT_LEN(hdr) ((hdr)->_length)
#define ND6H_MTU_OPT_MTU(hdr) ((hdr)->_mtu)

/** Route information option. */
#define ND6_OPTION_TYPE_ROUTE_INFO (24)
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct route_option {
	PACK_STRUCT_FLD_8(u8_t _type);
	PACK_STRUCT_FLD_8(u8_t _length);
	PACK_STRUCT_FLD_8(u8_t _prefix_length);
	PACK_STRUCT_FLD_8(u8_t _preference);
	PACK_STRUCT_FIELD(u32_t _route_lifetime);
	PACK_STRUCT_FLD_S(ip6_addr_p_t _prefix);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif
#define ND6H_ROUT_OPT_TYPE(hdr) ((hdr)->_type)
#define ND6H_ROUT_OPT_LEN(hdr) ((hdr)->_length)
#define ND6H_ROUT_OPT_PF_LEN(hdr) ((hdr)->_prefix_length)
#define ND6H_ROUT_OPT_PREFER(hdr) ((hdr)->_preference)
#define ND6H_ROUT_OPT_ROUT_LIFE(hdr) ((hdr)->_route_lifetime)
#define ND6H_ROUT_OPT_PF(hdr) ((hdr)->_prefix)

/** Recursive DNS Server Option. */
#if LWIP_ND6_RDNSS_MAX_DNS_SERVERS
#define LWIP_RDNSS_OPTION_MAX_SERVERS LWIP_ND6_RDNSS_MAX_DNS_SERVERS
#else
#define LWIP_RDNSS_OPTION_MAX_SERVERS 1
#endif
#define ND6_OPTION_TYPE_RDNSS (25)
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct rdnss_option {
	PACK_STRUCT_FLD_8(u8_t _type);
	PACK_STRUCT_FLD_8(u8_t _length);
	PACK_STRUCT_FIELD(u16_t reserved);
	PACK_STRUCT_FIELD(u32_t _lifetime);
	PACK_STRUCT_FLD_S(ip6_addr_p_t _rdnss_address[LWIP_RDNSS_OPTION_MAX_SERVERS]);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif
#define ND6H_RDNSS_OPT_TYPE(hdr) ((hdr)->_type)
#define ND6H_RDNSS_OPT_LEN(hdr) ((hdr)->_length)
#define ND6H_RDNSS_OPT_LIFE(hdr) ((hdr)->_lifetime)
#define ND6H_RDNSS_OPT_ADDR(hdr) ((hdr)->_rdnss_address)

#ifdef __cplusplus
}
#endif
#endif							/* LWIP_HDR_PROT_ND6_H */
