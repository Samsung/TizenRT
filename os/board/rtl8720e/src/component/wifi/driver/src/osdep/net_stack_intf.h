/* mbed Microcontroller Library
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
 */
#ifndef __NET_STACK_INTF_H__
#define __NET_STACK_INTF_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include <wireless.h>
#include <skbuff.h>

struct netif;

//----- ------------------------------------------------------------------
// Ethernet Buffer
//----- ------------------------------------------------------------------

struct eth_drv_sg {
	unsigned int	buf;
	unsigned int 	len;
};

//----- ------------------------------------------------------------------
// Wlan Interface Provided
//----- ------------------------------------------------------------------
unsigned char rltk_wlan_check_isup(int idx);
void rltk_wlan_tx_inc(int idx);
void rltk_wlan_tx_dec(int idx);
struct sk_buff *rltk_wlan_get_recv_skb(int idx);
struct sk_buff *rltk_wlan_alloc_skb(unsigned int total_len);
void rltk_wlan_set_netif_info(int idx_wlan, void *dev, unsigned char *dev_addr);
void rltk_wlan_send_skb(int idx, struct sk_buff *skb);	//struct sk_buff as defined above comment line
int rltk_wlan_send(int idx, struct eth_drv_sg *sg_list, int sg_len, int total_len);
void rltk_wlan_recv(int idx, struct eth_drv_sg *sg_list, int sg_len);
unsigned char rltk_wlan_running(unsigned char idx);		// interface is up. 0: interface is down

//----- ------------------------------------------------------------------
// Network Interface provided
//----- ------------------------------------------------------------------

int netif_is_valid_IP(int idx, unsigned char *ip_dest);
int netif_get_idx(struct netif *pnetif);
unsigned char *netif_get_hwaddr(int idx_wlan);
void netif_rx(int idx, unsigned int len);
#if (CONFIG_LWIP_LAYER == 1)
extern void ethernetif_recv(struct netif *netif, int total_len);
#endif //CONFIG_LWIP_LAYER == 1

#ifdef CONFIG_WOWLAN
extern unsigned char *rltk_wlan_get_ip(int idx);
#endif

#ifdef	__cplusplus
}
#endif

#endif //#ifndef __NET_STACK_INTF_H__
