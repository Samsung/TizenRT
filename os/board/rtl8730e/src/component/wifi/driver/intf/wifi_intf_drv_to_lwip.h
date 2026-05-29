/**
  ******************************************************************************
  * @file    wifi_intf_drv_to_lwip.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __LWIP_INTF_H__
#define __LWIP_INTF_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include <rtw_skbuff.h>
struct netif;

//----- ------------------------------------------------------------------
// Ethernet Buffer
//----- ------------------------------------------------------------------
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
#include "ethernetif.h"  // moved to ethernetif.h by jimmy 12/2/2015
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
void wifi_if_netif_rx(struct sk_buff *skb, void *preorder_ctrl);
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
int rltk_wlan_send(int idx, struct eth_drv_sg *sg_list, int sg_len, int total_len, u8 is_special_pkt);
void rltk_wlan_recv(int idx, struct eth_drv_sg *sg_list, int sg_len);
#endif

//----- ------------------------------------------------------------------
// Network Interface provided
//----- ------------------------------------------------------------------

int netif_is_valid_IP(int idx, unsigned char *ip_dest);
unsigned char *netif_get_hwaddr(int idx_wlan);
void netif_rx(int idx, unsigned int len);
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
extern void ethernetif_recv(struct netif *netif, int total_len);
#endif //CONFIG_LWIP_LAYER == 1

extern unsigned char *rltk_wlan_get_ip(int idx);
extern unsigned char *rltk_wlan_get_gw(int idx);
extern unsigned char *rltk_wlan_get_gwmask(int idx);

#ifdef	__cplusplus
}
#endif

#endif //#ifndef __LWIP_INTF_H__
