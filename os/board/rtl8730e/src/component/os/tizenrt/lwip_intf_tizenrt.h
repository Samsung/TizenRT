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

#include <rtw_skbuff.h>
#include "rtw_wifi_common.h"
#include <tinyara/netmgr/netdev_mgr.h>

struct netif;
//----- ------------------------------------------------------------------
// Network Interface provided
//----- ------------------------------------------------------------------

#ifdef CONFIG_NET_NETMGR
int get_idx_from_dev(struct netdev *dev);
#endif
void *rltk_pbuf_wrapper(struct sk_buff *skb, uint16_t *len);
void rltk_wlan_indicate_lwip(int idx_wlan, void *p_buf);
extern unsigned char *rltk_wlan_get_ip(int idx);
extern unsigned char *rltk_wlan_get_gw(int idx);
extern unsigned char *rltk_wlan_get_gwmask(int idx);

#ifdef	__cplusplus
}
#endif

#endif //#ifndef __LWIP_INTF_H__
