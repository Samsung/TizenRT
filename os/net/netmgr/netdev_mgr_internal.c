/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <tinyara/kmalloc.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/net/if/ethernet.h>
#include <tinyara/netmgr/netdev_mgr.h>

#include "netdev_mgr_internal.h"

#ifndef CONFIG_NETDEV_NUM
#define CONFIG_NETDEV_NUM 3
#endif
#define NETDEV_MGR_LOOP_NAME "lo"
#define NETDEV_MGR_WIFI_NAME "wlan"
#define NETDEV_MGR_ETHERNET_NAME "eth"

static struct netdev g_netdev[CONFIG_NETDEV_NUM];
static int g_netdev_idx = 0;
static int g_wlan_idx = 0;
static int g_eth_idx = 0;

/* protect access of g_netdev and g_netdev_idx
 * netdev is set while system is resetting.
 * and application doesn't require write operation to netdev
 * so if there is no write operation then protection wouldn't be required
 */
static sem_t g_netdev_lock;

#define NETDEV_LOCK								\
	do {										\
		sem_wait(&g_netdev_lock);				\
	} while (0)

#define NETDEV_UNLOCK							\
	do {										\
		sem_post(&g_netdev_lock);				\
	} while (0)


/*
 * external function
 */
#ifdef CONFIG_NET_LWIP
// the way to get lwip stack need to be changed.
extern void *get_netdev_ops_lwip(void);
#endif


struct netdev *nm_get_netdev(uint8_t *ifname)
{
	for (int i = 0; i < g_netdev_idx; i++) {
		if (!strcmp((const char *)ifname, (const char *)g_netdev[i].ifname)) {
			return &g_netdev[i];
		}
	}
	return NULL;
}

int nm_foreach(tr_netdev_callback_t callback, void *arg)
{
	if (!callback) {
		return -1;
	}

	struct netdev *dev = NULL;
	int ret = 0;

	for (int i = 0; i < g_netdev_idx; i++) {
		dev = &g_netdev[i];
		if (callback(dev, arg) != 0) {
			ret = -1;
			break;
		}
	}
	return ret;
}


int _nm_register_loop(struct netdev *dev, struct netdev_config *config)
{
	struct nic_config nconfig;
	nconfig.loopback = 1;

	int res = 0;
#ifdef CONFIG_NET_LWIP
	struct netdev_ops *ops = get_netdev_ops_lwip();
	dev->ops = (void *)ops;
	res = ops->init_nic(dev, &nconfig);
#endif

	return res;
}


struct netdev *nm_register(struct netdev_config *config)
{
	//NETDEV_LOCK;
	if (g_netdev_idx == CONFIG_NETDEV_NUM) {
		return NULL;
	}

	struct netdev *dev = &g_netdev[g_netdev_idx++];

	char name[IFNAMSIZ] = {0,};
	if (config->type == NM_LOOPBACK) {
		snprintf(name, IFNAMSIZ, "%s", NETDEV_MGR_LOOP_NAME);
		strncpy(dev->ifname, name, IFNAMSIZ);
		dev->type = NM_LOOPBACK;
		int res = _nm_register_loop(dev, config);
		if (res < 0) {
			ndbg("initialize loopback fail\n");
			return NULL;
		}
		return dev;
	} else if (config->type == NM_WIFI) {
		snprintf(name, IFNAMSIZ, "%s%d", NETDEV_MGR_WIFI_NAME, g_wlan_idx++);
		strncpy(dev->ifname, name, IFNAMSIZ);
		dev->type = NM_WIFI;
		dev->t_ops.wl = config->t_ops.wl;
	} else if (config->type == NM_ETHERNET) {
		snprintf(name, IFNAMSIZ, "%s%d", NETDEV_MGR_ETHERNET_NAME, g_eth_idx++);
		strncpy(dev->ifname, name, IFNAMSIZ);
		dev->type = NM_ETHERNET;
		dev->t_ops.eth = config->t_ops.eth;
	} else {
		ndbg("unknown type\n");
		return NULL;
	}

	// to do calculate exact size of tx_buf
	dev->tx_buf = (uint8_t *)kmm_malloc(config->mtu + 12); // 12 is padding.
	if (!dev->tx_buf) {
		ndbg("create txbuf fail(%d)\n", config->mtu + 12);
		return NULL;
	}
	struct nic_config nconfig;
	nconfig.flag = config->flag;
	/*  Hardware address */
	nconfig.mtu = config->mtu;
	nconfig.hwaddr_len = config->hwaddr_len;
	/*  IP4 address */
	struct sockaddr_in *addr = (struct sockaddr_in *)&nconfig.addr;
	addr->sin_addr.s_addr = 0;
	addr = (struct sockaddr_in *)&nconfig.netmask;
	addr->sin_addr.s_addr = 0;
	addr = (struct sockaddr_in *)&nconfig.gw;
	addr->sin_addr.s_addr = inet_addr("255.255.255.0");

	nconfig.is_default = config->is_default;
	nconfig.loopback = 0;

#ifdef CONFIG_NET_LWIP
	struct netdev_ops *ops = get_netdev_ops_lwip();

	ops->linkoutput = config->ops->linkoutput;
	ops->igmp_mac_filter = config->ops->igmp_mac_filter;

	dev->ops = (void *)ops;
	ops->init_nic(dev, &nconfig);
#endif

	dev->priv = config->priv;

	//NETDEV_UNLOCK;
	return dev;
}


int nm_count(void)
{
	return g_netdev_idx;
}


int nm_ifup(struct netdev *dev)
{
	int ret = ((struct netdev_ops *)(dev->ops))->ifup(dev);
	if (ret < 0) {
		ndbg("fail to up network interface\n");
		return -1;
	}

	if (dev->type == NM_WIFI) {
		ret = dev->t_ops.wl->init(dev);
		if (ret < 0) {
			ndbg("fail to up wi-fi driver interface\n");
			return -2;
		}
	} else if (dev->type == NM_ETHERNET) {
		ret = dev->t_ops.eth->init(dev);
		if (ret < 0) {
			ndbg("fail to intialize ethernet driver interface\n");
			return -1;
		}
		ret = dev->t_ops.eth->enable(dev);
		if (ret < 0) {
			ndbg("fail to up ethernet driver interface\n");
			return -1;
		}
	}

	return 0;
}


int nm_ifdown(struct netdev *dev)
{
	int ret = ((struct netdev_ops *)(dev->ops))->ifdown(dev);
	if (ret < 0) {
		ndbg("fail to down network interface\n");
		return -1;
	}

	if (dev->type == NM_WIFI) {
		ret = dev->t_ops.wl->deinit(dev);
		if (ret < 0) {
			ndbg("fail to down driver interface\n");
			return -2;
		}
	} else if (dev->type == NM_ETHERNET) {
		ret = dev->t_ops.eth->deinit(dev);
		if (ret < 0) {
			ndbg("fail to deinit etherent driver\n");
			return -1;
		}
	}

	return 0;
}


void nm_init(void)
{
	for (int i = 0; i < CONFIG_NETDEV_NUM; i++) {
		g_netdev[i].tx_buf = NULL;
		g_netdev[i].priv = NULL;
		g_netdev[i].ops = NULL;
		g_netdev[i].type = NM_UNKNOWN;
	}

	sem_init(&g_netdev_lock, 0, 1);
}
