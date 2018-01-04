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
#ifndef LWIP_HDR_PROT_TCP_H
#define LWIP_HDR_PROT_TCP_H

#include <net/lwip/arch.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Length of the TCP header, excluding options. */
#define TCP_HLEN 20

/* Fields are (of course) in network byte order.
 * Some fields are converted to host byte order in tcp_input().
 */
#ifdef PACK_STRUCT_USE_INCLUDES
#include <net/lwip/arch/bpstruct.h>
#endif
PACK_STRUCT_BEGIN
struct tcp_hdr {
	PACK_STRUCT_FIELD(u16_t src);
	PACK_STRUCT_FIELD(u16_t dest);
	PACK_STRUCT_FIELD(u32_t seqno);
	PACK_STRUCT_FIELD(u32_t ackno);
	PACK_STRUCT_FIELD(u16_t _hdrlen_rsvd_flags);
	PACK_STRUCT_FIELD(u16_t wnd);
	PACK_STRUCT_FIELD(u16_t chksum);
	PACK_STRUCT_FIELD(u16_t urgp);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif

/* TCP header flags bits */
#define TCP_FIN 0x01U
#define TCP_SYN 0x02U
#define TCP_RST 0x04U
#define TCP_PSH 0x08U
#define TCP_ACK 0x10U
#define TCP_URG 0x20U
#define TCP_ECE 0x40U
#define TCP_CWR 0x80U
/* Valid TCP header flags */
#define TCP_FLAGS 0x3fU

#define TCPH_HDRLEN(phdr) ((u16_t)(lwip_ntohs((phdr)->_hdrlen_rsvd_flags) >> 12))
#define TCPH_FLAGS(phdr)  ((u16_t)(lwip_ntohs((phdr)->_hdrlen_rsvd_flags) & TCP_FLAGS))

#define TCPH_HDRLEN_SET(phdr, len) (phdr)->_hdrlen_rsvd_flags = lwip_htons(((len) << 12) | TCPH_FLAGS(phdr))
#define TCPH_FLAGS_SET(phdr, flags) (phdr)->_hdrlen_rsvd_flags = (((phdr)->_hdrlen_rsvd_flags & PP_HTONS(~TCP_FLAGS)) | lwip_htons(flags))
#define TCPH_HDRLEN_FLAGS_SET(phdr, len, flags) (phdr)->_hdrlen_rsvd_flags = (u16_t)(lwip_htons((u16_t)((len) << 12) | (flags)))

#define TCPH_SET_FLAG(phdr, flags) (phdr)->_hdrlen_rsvd_flags = ((phdr)->_hdrlen_rsvd_flags | lwip_htons(flags))
#define TCPH_UNSET_FLAG(phdr, flags) (phdr)->_hdrlen_rsvd_flags = ((phdr)->_hdrlen_rsvd_flags & ~lwip_htons(flags))

#ifdef __cplusplus
}
#endif

#endif	/* LWIP_HDR_PROT_TCP_H */
