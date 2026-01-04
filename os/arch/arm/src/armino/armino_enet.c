/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
#include "armino_enet.h"

#include <tinyara/kmalloc.h>
#include <tinyara/netmgr/netdev_mgr.h>

extern void bk_wifi_sta_get_mac(uint8_t *mac);
extern void bk_wifi_ap_get_mac(uint8_t *mac);


struct netdev *armino_dev_wlan0 = NULL;
struct netdev *armino_dev_wlan1 = NULL;
struct bledev *armino_bm_dev_ble0 = NULL;

extern struct trble_ops g_trble_drv_ops; 

extern err_t low_level_output(struct netdev *dev, uint8_t *data, uint16_t dlen);
extern struct trwifi_ops g_trwifi_bk_ops;  

int bk_set_multicast_list(struct netdev *dev, const struct in_addr *group, netdev_mac_filter_action action)
{	
	//ToDO
	return 0;
}

struct netdev* armino_register_dev(int sizeof_priv)
{
	struct nic_io_ops nops = { (int(*)(struct netdev *dev, void *data, uint16_t len))low_level_output, (int(*)(struct netdev *netif, const struct in_addr *group, netdev_mac_filter_action action))bk_set_multicast_list};
	struct netdev_config config;
	config.ops = &nops;
	config.flag = NM_FLAG_ETHARP | NM_FLAG_ETHERNET | NM_FLAG_BROADCAST | NM_FLAG_IGMP;
	config.mtu = CONFIG_NET_ETH_MTU; // is it right that vendor decides MTU size??
	config.hwaddr_len = IFHWADDRLEN;

	config.is_default = 1;

	config.type = NM_WIFI;
	config.t_ops.wl = &g_trwifi_bk_ops;

	void *priv = kmm_zalloc(sizeof_priv);
	if (priv == NULL) {
		return NULL;
	}
	config.priv = priv;

	return netdev_register(&config);
}	

void up_netinitialize(void)
{
	int alloc_size;
	//struct netdev *dev = NULL;

	alloc_size = sizeof(struct netdev);

	armino_dev_wlan0 = armino_register_dev(alloc_size);
	if (armino_dev_wlan0 == NULL) {
		ndbg("Failed to register armino netdev wlan0\n");
	} else {
		uint8_t macptr[6] = {0};
		bk_wifi_sta_get_mac(macptr);
		netdev_set_hwaddr(armino_dev_wlan0, macptr, 6);
	}

	armino_dev_wlan1 = armino_register_dev(alloc_size);
	if (armino_dev_wlan1 == NULL) {
		ndbg("Failed to register armino netdev wlan1\n");
	} else {
		uint8_t macptr[6] = {0};
		bk_wifi_ap_get_mac(macptr);
		netdev_set_hwaddr(armino_dev_wlan1, macptr, 6);
	}

#ifdef CONFIG_VIRTUAL_BLE
	vble_register();
#else
#if defined(CONFIG_ARMINO_BLE) && defined(CONFIG_DRIVERS_BLE)
	armino_bm_dev_ble0 = (struct bledev *)bledev_register(&g_trble_drv_ops);
	if (armino_bm_dev_ble0 == NULL) {
		// rtw_printf("Failed to register armino netdev\n");
	}
#endif
#endif
}
