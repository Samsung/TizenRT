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
#include <sys/ioctl.h>
#include <stdint.h>
#include <netinet/in.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/net/if/ethernet.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include "netdev_mgr_internal.h"

struct ifenum {
	FAR struct ifconf	*ifc;
	unsigned int		pos;
};

static FAR struct netdev *_netdev_ifrdev(FAR struct ifreq *req)
{
	if (req != NULL) {
		return nm_get_netdev((uint8_t *)req->ifr_name);
	}
	return NULL;
}


static int _netdev_getconf(struct netdev *dev, void *arg)
{
	FAR struct ifenum *ifenum = (FAR struct ifenum *)arg;
	FAR struct ifconf *ifc = ifenum->ifc;
	FAR struct ifreq  *ifr = (FAR struct ifreq *)(ifc->ifc_buf + ifenum->pos);

	if (ifenum->pos + sizeof(struct ifreq) > ifc->ifc_len) {
		return -EFAULT;
	}

	strncpy(ifr->ifr_name, dev->ifname, IFNAMSIZ - 1);
	ifr->ifr_name[IFNAMSIZ - 1] = '\0';
	ND_NETOPS(dev, get_ip4addr)(dev, &ifr->ifr_addr, NETDEV_IP);
	ifenum->pos += sizeof(struct ifreq);

	return 0;
}


static int ioctl_siocgifconf(FAR struct ifconf *ifc)
{
	int ret;
	struct ifenum ife = {
		.ifc = ifc,
		.pos = 0,
	};

	ret = nm_foreach(_netdev_getconf, (void *)&ife);
	if (ret == OK) {
		ifc->ifc_len = ife.pos;
	}

	return ret;
}


static int _netdev_getname(struct netdev *dev, void *arg)
{
	uint8_t flag;
	struct ifreq *req = (struct ifreq *)arg;
	ND_NETOPS(dev, get_flag)(dev, &flag);
	if ((flag & IFF_RUNNING) && req->ifr_name[0] == 0) {
		strncpy(req->ifr_name, dev->ifname, IFNAMSIZ);
	}
	return 0;
}


static int ioctl_siocgifname(FAR struct ifreq *req)
{
	int ret = nm_foreach(_netdev_getname, (void *)req);

	return ret;
}

/****************************************************************************
 * Name: netdev_ifrioctl
 *
 * Description:
 *   Perform network device specific operations.
 *
 * Parameters:
 *   psock    Socket structure
 *   dev      Ethernet driver device structure
 *   cmd      The ioctl command
 *   req      The argument of the ioctl cmd
 *
 * Return:
 *   >=0 on success (positive non-zero values are cmd-specific)
 *   Negated errno returned on failure.
 *
 ****************************************************************************/

int netdev_ifrioctl(FAR struct socket *sock, int cmd, FAR struct ifreq *req)
{
	FAR struct netdev *dev;
	int ret = -EINVAL;
	(void)sock;

	nvdbg("cmd: %d\n", cmd);

	/* Execute the command */
	switch (cmd) {

	case SIOCGIFADDR: {			/* Get IP address */
		dev = _netdev_ifrdev(req);
		if (!dev) {
			break;
		}
		ret = ((struct netdev_ops *)(dev->ops))->get_ip4addr(dev, &req->ifr_addr, NETDEV_IP);
	}
		break;
	case SIOCSIFADDR: {			/* Set IP address */
		dev = _netdev_ifrdev(req);
		if (!dev) {
			break;
		}
		ret = ((struct netdev_ops *)(dev->ops))->set_ip4addr(dev, &req->ifr_addr, NETDEV_IP);
	}
		break;

	case SIOCGIFDSTADDR: {		/* Get P-to-P address */
		dev = _netdev_ifrdev(req);
		if (!dev) {
			break;
		}
		ret = ((struct netdev_ops *)(dev->ops))->get_ip4addr(dev, &req->ifr_addr, NETDEV_GW);
	}
		break;

	case SIOCSIFDSTADDR: {		/* Set P-to-P address */
		dev = _netdev_ifrdev(req);
		if (!dev) {
			break;
		}
		ret = ((struct netdev_ops *)(dev->ops))->set_ip4addr(dev, &req->ifr_addr, NETDEV_GW);
	}
		break;

	case SIOCGIFBRDADDR:		/* Get broadcast IP address */
	case SIOCSIFBRDADDR: {		/* Set broadcast IP address */
		ret = -ENOSYS;
	}
		break;

	case SIOCGIFNETMASK: {		/* Get network mask */
		dev = _netdev_ifrdev(req);
		if (!dev) {
			break;
		}
		ret = ((struct netdev_ops *)(dev->ops))->get_ip4addr(dev, &req->ifr_addr, NETDEV_NETMASK);
	}
		break;

	case SIOCSIFNETMASK: {		/* Set network mask */
		dev = _netdev_ifrdev(req);
		if (!dev) {
			break;
		}
		ret = ((struct netdev_ops *)(dev->ops))->set_ip4addr(dev, &req->ifr_addr, NETDEV_NETMASK);
	}
		break;

		/* TODO: Support IPv6 related IOCTL calls once IPv6 is functional */
#ifdef CONFIG_NET_IPv6
	case SIOCSLIFADDR: {		/* Set IP address */
		dev = _netdev_ifrdev(req);
		if (!dev) {
			break;
		}
		ret = ((struct netdev_ops *)(dev->ops))->set_ip6addr(dev, &(((struct lifreq *)req)->lifr_addr), NETDEV_IP);
	}
		break;

#endif
	case SIOCGLIFMTU:			/* Get MTU size */
	case SIOCGIFMTU: {			/* Get MTU size */
		dev = _netdev_ifrdev(req);
		if (!dev) {
			break;
		}
		ret = ((struct netdev_ops *)(dev->ops))->get_mtu(dev, &req->ifr_mtu);
	}
		break;

	case SIOCSIFFLAGS: {		/* Sets the interface flags */
		/* Is this a request to bring the interface up? */
		dev = _netdev_ifrdev(req);
		if (!dev) {
			break;
		}
		if (req->ifr_flags & IFF_UP) {
			ret = nm_ifup(dev);
		} else if (req->ifr_flags & IFF_DOWN) {
			ret = nm_ifdown(dev);
		}
		ret = OK;
	}
		break;

	case SIOCGIFFLAGS: {		/* Gets the interface flags */
		dev = _netdev_ifrdev(req);
		if (!dev) {
			break;
		}
		ret = ((struct netdev_ops *)(dev->ops))->get_flag(dev, &req->ifr_flags);
	}
		break;

		/* MAC address operations only make sense if Ethernet is supported */
	case SIOCGIFHWADDR: {		/* Get hardware address */
		dev = _netdev_ifrdev(req);
		if (!dev) {
			break;
		}
		ret = ((struct netdev_ops *)(dev->ops))->get_hwaddr(dev, &req->ifr_hwaddr);
	}
		break;

	case SIOCSIFHWADDR: {		/* Set hardware address -- will not take effect until ifup */
		dev = _netdev_ifrdev(req);
		if (!dev) {
			break;
		}
		ret = ((struct netdev_ops *)(dev->ops))->set_hwaddr(dev, &req->ifr_hwaddr);
	}
		break;

	case SIOCDIFADDR: {			/* Delete IP address */
		dev = _netdev_ifrdev(req);
		if (!dev) {
			break;
		}
		ret = ((struct netdev_ops *)(dev->ops))->delete_ipaddr(dev);
	}
		break;

	case SIOCGIFCOUNT: {		/* Get number of devices */
		req->ifr_count = nm_count();
		ret = OK;
	}
		break;

#ifdef CONFIG_NET_ARPIOCTLS
	case SIOCSARP:				/* Set a ARP mapping */
	case SIOCDARP:				/* Delete an ARP mapping */
	case SIOCGARP:				/* Get an ARP mapping */
#error "IOCTL Commands not implemented"
#endif							/* CONFIG_NET_ARPIOCTLS */

#ifdef CONFIG_NETDEV_PHY_IOCTL
	case SIOCGMIIPHY:			/* Get address of MII PHY in use */
	case SIOCGMIIREG:			/* Get MII register via MDIO */
	case SIOCSMIIREG: {			/* Set MII register via MDIO */
		dev = _netdev_ifrdev(req);
		if (!dev || !dev->d_ioctl) {
			break;
		}
		struct mii_ioctl_data_s *mii_data = &req->ifr_ifru.ifru_mii_data;
		ret = dev->d_ioctl(dev, cmd, ((long)(uintptr_t)mii_data));
	}
		break;
#endif							/* CONFIG_NETDEV_PHY_IOCTL */
	case SIOCGIFCONF:
		ret = ioctl_siocgifconf((FAR struct ifconf *)req);
		break;
	case SIOCGIFNAME:
		ret = ioctl_siocgifname(req);
		break;
	default: {
		ret = -ENOTTY;
	}
		break;
	}

	return ret;
}
