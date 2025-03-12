/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
/**
 * @file
 * Ethernet Interface Skeleton
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

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "lwip/tcpip.h"
#include "lwip/icmp.h"
#include "lwip/netif/etharp.h"
#include "lwip/err.h"
#include "platform_opts.h"
#include "ethernetif_tizenrt.h"

#include "lwip/ethip6.h" //Add for ipv6

#if CONFIG_WLAN
#include <lwip_intf_tizenrt.h>
#endif

#if defined(CONFIG_INIC_HOST) && CONFIG_INIC_HOST
#include "freertos/inic_intf.h"
#endif

#if defined(CONFIG_AS_INIC_AP)
#include "inic_ipc_host_trx.h"
#endif

#ifndef CONFIG_ENABLE_HOMELYNK
/* Check if current mode is softap */
extern int softap_flag;
#endif //#ifndef CONFIG_ENABLE_HOMELYNK

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param dev the network device
 * @param data the data to send
 * @param dlen the length of data
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
err_t low_level_output(struct netdev *dev, uint8_t *data, uint16_t dlen)
{
	struct eth_drv_sg sg_list[MAX_ETH_DRV_SG];
	int sg_len = 0;
#if defined(CONFIG_AS_INIC_AP)
	int ret = 0;
	int idx = 0;
	struct eth_hdr *ethhdr = NULL;
	u8 is_special_pkt = 0;
	u8 *addr = data;
#endif

#if CONFIG_WLAN
#ifndef CONFIG_AS_INIC_AP
	if (!wifi_is_running(netif_get_idx(netif))) {
		return ERR_IF;
	}
#endif
#endif

#if defined(CONFIG_AS_INIC_AP)
	if (dlen >= ETH_HLEN + 24) {
		ethhdr = (struct eth_hdr *)data;
		if (ETH_P_IP == _htons(ethhdr->type)) {
			addr += ETH_HLEN;
			if (((addr[21] == 68) && (addr[23] == 67)) ||
				((addr[21] == 67) && (addr[23] == 68))) {
				// DHCP packet, 68 : UDP BOOTP client, 67 : UDP BOOTP server
				is_special_pkt = 1;
			}
		}
	}
#endif
	if (data != NULL && sg_len < MAX_ETH_DRV_SG) {
		sg_list[sg_len].buf = (unsigned int)data;
		sg_list[sg_len++].len = dlen;
	}
	if (sg_len) {
#if CONFIG_WLAN
#if defined(CONFIG_AS_INIC_AP)
#ifdef CONFIG_ENABLE_HOMELYNK
		/* If concurrent is enabled, get idx based on the two netif */
		idx = get_idx_from_dev(dev);
#else
		/* If concurrent is disabled, TizenRT only uses idx 0 on application layer, but driver splits STA and SOFTAP into idx 0 and idx 1, need to check which idx to send to */
		if (softap_flag == 1) {
			idx = 1;
		}
#endif //#ifdef CONFIG_ENABLE_HOMELYNK
		ret = inic_host_send(idx, sg_list, sg_len, dlen, NULL, is_special_pkt);
		if (ret == ERR_IF) {
			return ret;
		}
		if (ret == 0)
#else
		if (rltk_wlan_send(netif_get_idx(netif), sg_list, sg_len, p->tot_len) == 0)
#endif
#elif CONFIG_INIC_HOST
		if (rltk_inic_send(sg_list, sg_len, p->tot_len) == 0)
#else
		if (1)
#endif
		{
			return ERR_OK;
		} else {
			return ERR_BUF;    // return a non-fatal error
		}
	}
	return ERR_OK;
}
