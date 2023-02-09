/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 *
 *   Copyright (C) 2020 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdbool.h>
#include <net/if.h>
#include <debug.h>
#include <amebalite_enet.h>

#include <tinyara/kmalloc.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include <tinyara/ble/ble_manager.h>
#include "wifi_structures.h"

//for concurrent mode, not supported now
//#define RTK_CONCURRENT_MODE
#ifdef CONFIG_AMEBALITE_WIFI

struct netdev *ameba_nm_dev_wlan0 = NULL;
struct netdev *ameba_nm_dev_wlan1 = NULL;
struct bledev *ameba_bm_dev_ble0 = NULL;

extern err_t low_level_output(struct netdev *dev, uint8_t *data, uint16_t dlen);
extern struct trwifi_ops g_trwifi_drv_ops;
extern struct trble_ops g_trble_drv_ops;

extern void wifi_init_packet_filter(void);
extern int wifi_add_packet_filter(unsigned char filter_id, rtw_packet_filter_pattern_t *patt, rtw_packet_filter_rule_t rule);
extern int wifi_enable_packet_filter(unsigned char filter_id);
extern int wifi_disable_packet_filter(unsigned char filter_id);
extern int wifi_remove_packet_filter(unsigned char filter_id);

#define MULTICAST_IP_TO_MAC(ip) { (u8)0x01, \
				  (u8)0x00, \
				  (u8)0x5e, \
				  (u8)((ip)[1] & 0x7F), \
				  (u8)(ip)[2], \
				  (u8)(ip)[3] \
}

static inline void ether_addr_copy(u8 *dst, const u8 *src)
{
	u16 *a = (u16 *) dst;
	const u16 *b = (const u16 *)src;

	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
}

#define RTK_ETHER_COPY(dst, src)       ether_addr_copy((dst), (src))
#define MASK_SIZE 6

int rtk_set_multicast_packet_filters(int dev_index, const u8 *addr){
	u8 mask[MASK_SIZE]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	int ret = 0;

	rtw_packet_filter_pattern_t packet_filter;
	rtw_packet_filter_rule_t rule;

	RTK_ETHER_COPY(packet_filter.pattern, addr);

	packet_filter.offset = 0;
	packet_filter.mask_size = MASK_SIZE;
	packet_filter.mask = mask;

	rule = RTW_POSITIVE_MATCHING;

	wifi_init_packet_filter();
	ret = wifi_add_packet_filter(dev_index, &packet_filter, rule);
	if (ret != 0){
		vddbg("wifi_add_packet_filter fail, ret = %d\n",ret);
		return ret;
	}
	ret = wifi_enable_packet_filter(dev_index);
	if (ret != 0){
		vddbg("wifi_enable_packet_filter fail, ret = %d\n",ret);
		return ret;
	}
	return ret;
}

int rtk_clear_packet_filters(int dev_index){
	int ret = 0;

	ret = wifi_disable_packet_filter(dev_index);
	if (ret != 0){
		vddbg("wifi_disable_packet_filter fail, ret = %d\n",ret);
		return ret;
	}
	ret = wifi_remove_packet_filter(dev_index);
	if (ret != 0){
		vddbg("wifi_remove_packet_filter fail, ret = %d\n",ret);
		return ret;
	}

	return ret;
}

int rtk_set_multicast_list(struct netdev *dev, const struct in_addr *group, netdev_mac_filter_action action)
{
	u8 addr[6] = MULTICAST_IP_TO_MAC((u8 *)group);
	int dev_index = get_idx_from_dev(dev);

	if (action == NM_ADD_MAC_FILTER) {
		vddbg("Add filter for mac address = %p, dev_index =%d\n", addr, dev_index);
		return rtk_set_multicast_packet_filters(dev_index, addr);
	} else if (action == NM_DEL_MAC_FILTER){
		vddbg("Clear filter for mac address = %p, dev_index =%d\n", addr, dev_index);
		return rtk_clear_packet_filters(dev_index);
	}
	else{
		vddbg("Invalid action: %d\n", action);
		return -EINVAL;
	}

}


struct netdev* amebalite_register_dev(int sizeof_priv)
{
	struct nic_io_ops nops = { (int(*)(struct netdev *dev, void *data, uint16_t len))low_level_output, (int(*)(struct netdev *netif, const struct in_addr *group, netdev_mac_filter_action action))rtk_set_multicast_list};
	struct netdev_config config;
	config.ops = &nops;
	config.flag = NM_FLAG_ETHARP | NM_FLAG_ETHERNET | NM_FLAG_BROADCAST | NM_FLAG_IGMP;
	config.mtu = CONFIG_NET_ETH_MTU; // is it right that vendor decides MTU size??
	config.hwaddr_len = IFHWADDRLEN;

	config.is_default = 1;

	config.type = NM_WIFI;
	config.t_ops.wl = &g_trwifi_drv_ops;

	void *priv = kmm_zalloc(sizeof_priv);
	if (priv == NULL) {
		return NULL;
	}
	config.priv = priv;

	return netdev_register(&config);
}
#endif

void up_netinitialize(void)
{
#ifdef CONFIG_AMEBALITE_WIFI
	int alloc_size;
	//struct netdev *dev = NULL;

	alloc_size = sizeof(struct netdev);

	ameba_nm_dev_wlan0 = amebalite_register_dev(alloc_size);
	if (ameba_nm_dev_wlan0 == NULL) {
		rtw_printf("Failed to register amebalite netdev\n");
	}
#ifdef RTK_CONCURRENT_MODE
	ameba_nm_dev_wlan1 = amebalite_register_dev(alloc_size);
	if (ameba_nm_dev_wlan1 == NULL) {
		rtw_printf("Failed to register amebalite netdev\n");
	}
#endif
#endif
#if defined(CONFIG_AMEBALITE_BLE) && defined(CONFIG_DRIVERS_BLE)
	ameba_bm_dev_ble0 = bledev_register(&g_trble_drv_ops);
	if (ameba_bm_dev_ble0 == NULL) {
		rtw_printf("Failed to register amebad netdev\n");
	}
#endif
}
