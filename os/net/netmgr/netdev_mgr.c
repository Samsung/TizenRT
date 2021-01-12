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
#include <debug.h>
#include <semaphore.h>
#include <netinet/in.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/net/if/ethernet.h>
#include <tinyara/netmgr/netdev_mgr.h>

#include "netdev_mgr_internal.h"


struct netdev *netdev_register(struct netdev_config *config)
{
	return nm_register(config);
}


int netdev_start(struct netdev *dev)
{
	// linkup??
	return 0;
}


int netdev_set_hwaddr(struct netdev *dev, uint8_t *hwaddr, uint8_t hwaddr_len)
{
	struct sockaddr addr;
	memcpy(addr.sa_data, hwaddr, hwaddr_len);
	int res = ND_NETOPS(dev, set_hwaddr)(dev, &addr);
	return res;
}


int netdev_get_hwaddr(struct netdev *dev, uint8_t *hwaddr, uint8_t *hwaddr_len)
{
	struct sockaddr addr;
	int res = ND_NETOPS(dev, get_hwaddr)(dev, &addr);
	if (res < 0) {
		return -1;
	}
	if (hwaddr_len) {
		*hwaddr_len = IFHWADDRLEN;
	}
	memcpy(hwaddr, addr.sa_data, IFHWADDRLEN);

	return res;
}


int netdev_input(struct netdev *dev, void *data, uint16_t len)
{
	return ND_NETOPS(dev, input)(dev, data, len);
}


int netdev_get_mtu(struct netdev *dev, int *mtu)
{
	return ND_NETOPS(dev, get_mtu)(dev, mtu);
}


int netdev_mgr_start(void)
{
	nm_init();
#ifdef CONFIG_NET_LOOPBACK_INTERFACE
	/*	register loopback */
	struct netdev_config config;
	config.type = NM_LOOPBACK;
	config.flag = NM_FLAG_IGMP;
	nm_register(&config);
#endif

	return 0;
}


#ifdef CONFIG_SCSC_WLAN
#include "lwip/netif.h"
#endif
uint8_t *netdev_get_hwaddr_ptr(struct netdev *dev)
{
#ifdef CONFIG_SCSC_WLAN
	struct netdev_ops *ops = (struct netdev_ops *)dev->ops;
	struct netif *ni = (struct netif *)ops->nic;
	return ni->hwaddr;
#else
	return NULL;
#endif
}
