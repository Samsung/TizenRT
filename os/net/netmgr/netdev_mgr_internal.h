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
#pragma once

#define NETDEV_IP 1
#define NETDEV_GW 2
#define NETDEV_NETMASK 3

#define ND_NETOPS(dev, method) (((struct netdev_ops *)((dev)->ops))->method)
#define ND_NETOPS_RET(res, dev, method, param)							\
	do {																\
		if (((struct netdev_ops *)((dev)->ops))->method) {						\
			res = (((struct netdev_ops *)((dev)->ops))->method)param;	\
		}																\
	} while (0)
#define GET_NETIF_FROM_NETDEV(dev) (struct netif *)(((struct netdev_ops *)(dev)->ops)->nic)

struct nic_config {
	int flag;
	int mtu;
	int hwaddr_len;
	/*	Device address */
	struct sockaddr addr; // ipv6, ipv4
	struct sockaddr netmask;
	struct sockaddr gw;
	struct sockaddr_storage addr6; // ipv6, ipv4
	struct sockaddr_storage netmask6;
	struct sockaddr_storage gw6;
	int is_default;
	int loopback;
	struct nic_io_ops io_ops;
};

struct netdev_ops {
	/*  Initialize NIC */
	int (*init_nic)(struct netdev *dev, struct nic_config *config);
	int (*deinit_nic)(struct netdev *dev);

	/*  Address */
	int (*get_ip4addr)(struct netdev *dev, struct sockaddr *addr, int type);
	int (*set_ip4addr)(struct netdev *dev, struct sockaddr *addr, int type);
	int (*set_ip6addr)(struct netdev *dev, struct sockaddr_storage *addr, int type);
	int (*set_ip6addr_type)(struct netdev *dev, uint8_t type);
	int (*get_ifaddrs)(struct netdev *dev, struct ifaddrs **addrs);
	int (*delete_ipaddr)(struct netdev *dev);

	int (*get_hwaddr)(struct netdev *dev, struct sockaddr *hwaddr);
	int (*set_hwaddr)(struct netdev *dev, struct sockaddr *hwaddr);

	int (*get_mtu)(struct netdev *dev, int *mtu);
	int (*get_flag)(struct netdev *dev, uint8_t *flag);
	/*  Interface control */
	int (*ifup)(struct netdev *dev);
	int (*ifdown)(struct netdev *dev);
	/*  control network stack traffic */
	int (*softup)(struct netdev *dev);
	int (*softdown)(struct netdev *dev);

	/* multicast */
	int (*joingroup)(struct netdev *dev, struct in_addr *addr);
	int (*leavegroup)(struct netdev *dev, struct in_addr *addr);

	int (*input)(struct netdev *dev, void *data, uint16_t len);
	int (*linkoutput)(struct netdev *dev, void *data, uint16_t len);
	int (*igmp_mac_filter)(struct netdev *dev, const struct in_addr *group, netdev_mac_filter_action action);

	/* statistics */
	int (*get_stats)(struct netdev *dev, void *arg);
#ifdef CONFIG_NETDEV_PHY_IOCTL
	int (*d_ioctl)(struct netdev *dev, int cmd, void *data);
#endif
	/*  NIC stack specific */
	void *nic;
};

// integrate it to non-netmgr version, it's duplicated to netdev_callback_t
typedef int (*tr_netdev_callback_t)(struct netdev *dev, void *arg);

void nm_init(void);
struct netdev *nm_register(struct netdev_config *config);
struct netdev *nm_get_netdev(uint8_t *ifname);
int nm_foreach(tr_netdev_callback_t callback, void *arg);
int nm_count(void);
int nm_ifup(struct netdev *dev);
int nm_ifdown(struct netdev *dev);
