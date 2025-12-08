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

#ifndef __LWIP_INTF_H__
#define __LWIP_INTF_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include "platform_opts.h"
#include <rtw_skbuff.h>

struct netif;

//----- ------------------------------------------------------------------
// Ethernet Buffer
//----- ------------------------------------------------------------------
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
#include "ethernetif_tizenrt.h"  // moved to ethernetif.h by jimmy 12/2/2015
#endif
//----- ------------------------------------------------------------------
// Wlan Interface Provided
//----- ------------------------------------------------------------------
void wifi_if_tx_inc(int idx);
void wifi_if_tx_dec(int idx);
struct sk_buff *wifi_if_get_recv_skb(int idx);
struct sk_buff *wifi_if_alloc_skb(unsigned int total_len);
void rltk_wlan_set_netif_info(int idx_wlan, void *dev, unsigned char *dev_addr);
void wifi_if_send_skb(int idx, struct sk_buff *skb);	//struct sk_buff as defined above comment line
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
int rltk_wlan_send(int idx, struct eth_drv_sg *sg_list, int sg_len, int total_len);
void rltk_wlan_recv(int idx, struct eth_drv_sg *sg_list, int sg_len);
#endif

//----- ------------------------------------------------------------------
// Network Interface provided
//----- ------------------------------------------------------------------

int netif_is_valid_IP(int idx, unsigned char *ip_dest);
int netif_get_idx(struct netif *pnetif);
int netif_get_hwaddr(int idx_wlan, uint8_t *dev_addr);
void netif_rx(int idx, unsigned int len);
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
extern void ethernetif_recv(struct netif *netif, int total_len);
#endif //CONFIG_LWIP_LAYER == 1
void *rltk_pbuf_wrapper(struct sk_buff *skb, u16_t *len);
void rltk_wlan_indicate_lwip(int idx_wlan, void *p_buf);
#ifdef CONFIG_WOWLAN
extern unsigned char *rltk_wlan_get_ip(int idx);
extern unsigned char *rltk_wlan_get_gw(int idx);
extern unsigned char *rltk_wlan_get_gwmask(int idx);
#endif

#ifdef	__cplusplus
}
#endif

#endif //#ifndef __LWIP_INTF_H__
