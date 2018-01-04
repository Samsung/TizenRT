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
#ifndef LWIP_HDR_PROT_UDP_H
#define LWIP_HDR_PROT_UDP_H

#include <net/lwip/arch.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UDP_HLEN 8

/* Fields are (of course) in network byte order. */
#ifdef PACK_STRUCT_USE_INCLUDES
#include <net/lwip/arch/bpstruct.h>
#endif
PACK_STRUCT_BEGIN
struct udp_hdr {
	PACK_STRUCT_FIELD(u16_t src);
	PACK_STRUCT_FIELD(u16_t dest);	/* src/dest UDP ports */
	PACK_STRUCT_FIELD(u16_t len);
	PACK_STRUCT_FIELD(u16_t chksum);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include <net/lwip/arch/epstruct.h>
#endif

#ifdef __cplusplus
}
#endif

#endif	/* LWIP_HDR_PROT_UDP_H */
