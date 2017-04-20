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
/****************************************************************************
 * Macros and definitions for the Ethernet link layer.
 *
 *   Copyright (C) 2007, 2009-2012, 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Derived from uIP with has a similar BSD-styple license:
 *
 *   Author: Adam Dunkels <adam@dunkels.com>
 *   Copyright (c) 2001-2003, Adam Dunkels.
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __INCLUDE_TINYARA_NET_ETHERNET_H
#define __INCLUDE_TINYARA_NET_ETHERNET_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>
#include <tinyara/config.h>
#include <net/ethernet.h>
#include <net/lwip/netif.h>

#ifdef CONFIG_NET_ETHERNET

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/* Size of the Ethernet header */

#define ETH_HDRLEN      14		/* Minimum size: 2*6 + 2 */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* The Ethernet header -- 14 bytes. The first two fields are type 'struct
 * ether_addr but are represented as a simple byte array here because
 * some compilers refuse to pack 6 byte structures.
 */

struct eth_hdr_s {
	uint8_t dest[6];			/* Ethernet destination address (6 bytes) */
	uint8_t src[6];				/* Ethernet source address (6 bytes) */
	uint16_t type;				/* Type code (2 bytes) */
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

void ethernetif_status_callback(struct netif *netif);
err_t ethernetif_init(struct netif *netif);
int ethernetif_input(struct netif *netif);

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_NET_ETHERNET */
#endif							/* __INCLUDE_TINYARA_NET_ETHERNET_H */
