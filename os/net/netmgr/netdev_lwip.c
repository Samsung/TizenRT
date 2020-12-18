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

#ifndef _NETDEV_LWIP_OPS_H__
#define _NETDEV_LWIP_OPS_H__

#include <tinyara/config.h>

#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <tinyara/kmalloc.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/net/if/ethernet.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/etharp.h"
#include "lwip/ethip6.h"
#include "lwip/netifapi.h"
#include "lwip/snmp.h"
#include "lwip/igmp.h"
#include "netdev_mgr_internal.h"

/* This is really kind of bogus.. When asked for an IP address, this is
 * family that is returned in the ifr structure.  Probably could just skip
 * this since the address family has nothing to do with the Ethernet address.
 */
#ifdef CONFIG_NET_IPv6
#define AF_INETX AF_INET6
#else							/* CONFIG_NET_IPv6 */
#define AF_INETX AF_INET
#endif							/* CONFIG_NET_IPv6 */

#define LW_GETND(nic) (struct netdev *)(*(struct netdev **)(&((char *)nic)[sizeof(struct netif)]))
#define GET_NETIF_FROM_NETDEV(dev) (struct netif *)(((struct netdev_ops *)(dev)->ops)->nic)

// Temporary impl
static int g_num = 0;

/**
 * Private Function
 */
static inline int _netif_down(struct netif *ni)
{
	/* Is the interface already down? */
	err_t res = netifapi_netif_set_link_down(ni);
	if (res != ERR_OK) {
		ndbg("netdev soft link down fail\n");
		return -1;
	}

	res = netifapi_netif_set_down(ni);
	if (res != ERR_OK) {
		ndbg("netdev soft if down fail\n");
		return -2;
	}
	return 0;
}


static inline int _netif_up(struct netif *ni)
{
	err_t lres = netifapi_netif_set_up(ni);
	if (lres != ERR_OK) {
		ndbg("netni soft if up fail\n");
		return -1;
	}

	lres = netifapi_netif_set_link_up(ni);
	if (lres != ERR_OK) {
		ndbg("netdev soft link up fail\n");
		return -2;
	}
	return 0;
}


static void _netif_soft_ifup(FAR struct netif *dev)
{
	err_t res = netifapi_netif_set_up(dev);
	if (res != ERR_OK) {
		ndbg("netdev soft ifup fail\n");
	}
}


static void _netif_soft_ifdown(FAR struct netif *dev)
{
	err_t res = netifapi_netif_set_down(dev);
	if (res != ERR_OK) {
		ndbg("netdev soft ifdown fail\n");
	}
}


#ifdef CONFIG_NET_IPv4
static inline void _convert_ip4addr_lton(struct sockaddr_in *dest_addr, ip_addr_t *src_addr)
{
	dest_addr->sin_family = AF_INET;
	dest_addr->sin_port = 0;
	dest_addr->sin_addr.s_addr = ip4_addr_get_u32(ip_2_ip4(src_addr));
}

static inline void _convert_ip4addr_ntol(ip_addr_t *dest_addr, struct sockaddr_in *src_addr)
{
	ip4_addr_get_u32(ip_2_ip4(dest_addr)) = src_addr->sin_addr.s_addr;
}
#endif


#ifdef CONFIG_NET_IPv6
static inline void _convert_ip6addr_lton(FAR struct sockaddr_in6 *outaddr, ip_addr_t *addr)
{
	ip6_addr_t *tmp = ip_2_ip6(addr);
	ndbg("convert address lwip to netstack: %4x:%4x:%4x:%4x:%4x:%4x:%4x:%4x\n",
			   IP6_ADDR_BLOCK1(tmp),
			   IP6_ADDR_BLOCK2(tmp),
			   IP6_ADDR_BLOCK3(tmp),
			   IP6_ADDR_BLOCK4(tmp),
			   IP6_ADDR_BLOCK5(tmp),
			   IP6_ADDR_BLOCK6(tmp),
			   IP6_ADDR_BLOCK7(tmp),
			   IP6_ADDR_BLOCK8(tmp));

	outaddr->sin6_family = AF_INET6;
	outaddr->sin6_port = 0;
	memcpy(outaddr->sin6_addr.s6_addr, tmp, 16);
}

static inline void _convert_ip6addr_ntol(ip_addr_t *outaddr, FAR const struct sockaddr_in6 *inaddr)
{
	memcpy(ip_2_ip6(outaddr), inaddr->sin6_addr.s6_addr, 16);
}


static void _netif_setip6addr(struct netif *dev, FAR const struct sockaddr_storage *inaddr)
{
	ip6_addr_t temp;
	s8_t idx;

	if (!dev || !inaddr) {
		ndbg("Invalid parameters\n");
		return;
	}

	struct sockaddr_in6 *src = (struct sockaddr_in6 *)inaddr;
	memcpy(&temp, src->sin6_addr.s6_addr, 16);

	idx = netif_get_ip6_addr_match(dev, &temp);
	if (idx != -1) {
#ifdef CONFIG_NET_IPv6_MLD
		ip6_addr_t solicit_addr;

		/* leaving MLD6 group */
		ip6_addr_set_solicitednode(&solicit_addr, ip_2_ip6(&dev->ip6_addr[0])->addr[idx]);
		mld6_leavegroup_netif(dev, &solicit_addr);
		ndbg("MLD6 group left - %X : %X : %X : %X\n",
			 PP_HTONL(solicit_addr.addr[0]), PP_HTONL(solicit_addr.addr[1]),
			 PP_HTONL(solicit_addr.addr[2]), PP_HTONL(solicit_addr.addr[3]));
#endif /* CONFIG_NET_IPv6_MLD */
		/* delete static ipv6 address if the same ip address exists */
		netif_ip6_addr_set_state(dev, idx, IP6_ADDR_INVALID);
		return;
	}

#ifdef CONFIG_NET_IPv6_AUTOCONFIG
	/* enable IPv6 address stateless auto-configuration */
	netif_set_ip6_autoconfig_enabled(dev, 1);
#endif /* CONFIG_NET_IPv6_AUTOCONFIG */

	/* add static ipv6 address */
	(void)netif_add_ip6_address(dev, &temp, &idx);

#ifdef CONFIG_NET_IPv6_MLD
	ip6_addr_t solicit_addr;

	/* set MLD6 group to receive solicit multicast message */
	ip6_addr_set_solicitednode(&solicit_addr, ip_2_ip6(&dev->ip6_addr[0])->addr[idx]);
	mld6_joingroup_netif(dev, &solicit_addr);
	ndbg("MLD6 group added - %X : %X : %X : %X\n",
		 PP_HTONL(solicit_addr.addr[0]), PP_HTONL(solicit_addr.addr[1]),
		 PP_HTONL(solicit_addr.addr[2]), PP_HTONL(solicit_addr.addr[3]));
#endif /* CONFIG_NET_IPv6_MLD */

	return;
}
#endif // CONFIG_NET_IPv6


static err_t _lwip_nic_init(struct netif *nic)
{
	nic->name[0] = 'w';
	nic->name[1] = 'l';

	// To Do: apply flag which is set in netdev
	// nic->flags = NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_BROADCAST | NETIF_FLAG_IGMP;

	return ERR_OK;
}


#if LWIP_HAVE_LOOPIF
#if LWIP_IPV4
static err_t _netif_loop_output_ipv4(struct netif *netif, struct pbuf *p, const ip4_addr_t *addr)
{
	LWIP_UNUSED_ARG(addr);
	return netif_loop_output(netif, p);
}
#endif							/* LWIP_IPV4 */


#if LWIP_IPV6
static err_t _netif_loop_output_ipv6(struct netif *netif, struct pbuf *p, const ip6_addr_t *addr)
{
	LWIP_UNUSED_ARG(addr);
	return netif_loop_output(netif, p);
}
#endif							/* LWIP_IPV6 */
#endif							/* LWIP_HAVE_LOOPIF */


static err_t _netif_loopif_init(struct netif *netif)
{
	/* initialize the snmp variables and counters inside the struct netif
	 * ifSpeed: no assumption can be made!
	 */
	MIB2_INIT_NETIF(netif, snmp_ifType_softwareLoopback, 0);

	netif->mtu = 8092;

	netif->name[0] = 'l';
	netif->name[1] = 'o';
#if LWIP_HAVE_LOOPIF
#if LWIP_IPV4
	netif->output = _netif_loop_output_ipv4;
#endif
#if LWIP_IPV6
	netif->output_ip6 = _netif_loop_output_ipv6;
#endif
#endif
#if LWIP_LOOPIF_MULTICAST
	netif->flags |= NETIF_FLAG_IGMP;
#endif

	return ERR_OK;
}


static void _lwip_init_loop(struct netif *nic)
{
#if LWIP_HAVE_LOOPIF
#if LWIP_IPV4
#define LOOPIF_ADDRINIT &loop_ipaddr, &loop_netmask, &loop_gw,
	ip4_addr_t loop_ipaddr, loop_netmask, loop_gw;
	IP4_ADDR(&loop_gw, 127, 0, 0, 1);
	IP4_ADDR(&loop_ipaddr, 127, 0, 0, 1);
	IP4_ADDR(&loop_netmask, 255, 0, 0, 0);
#else							/* LWIP_IPV4 */
#define LOOPIF_ADDRINIT
#endif							/* LWIP_IPV4 */

#if NO_SYS
	netif_add(nic, LOOPIF_ADDRINIT NULL, _netif_loopif_init, ip_input);
#else							/* NO_SYS */
	netif_add(nic, LOOPIF_ADDRINIT NULL, _netif_loopif_init, tcpip_input);
#endif							/* NO_SYS */

#if LWIP_IPV6
	IP_ADDR6_HOST(nic->ip6_addr, 0, 0, 0, 0x00000001UL);
	nic->ip6_addr_state[0] = IP6_ADDR_VALID;
#endif							/* LWIP_IPV6 */

	netif_set_link_up(nic);
	netif_set_up(nic);

#endif							/* LWIP_HAVE_LOOPIF */
}


static inline void _free_ifaddrs(struct ifaddrs *addrs)
{
	struct ifaddrs *ifa = NULL, *prev = NULL;
	for (ifa = addrs; ifa; ifa = ifa->ifa_next) {
		if (ifa->ifa_name) {
			free(ifa->ifa_name);
		}
		if (ifa->ifa_addr) {
			free(ifa->ifa_addr);
		}
		if (ifa->ifa_netmask) {
			free(ifa->ifa_netmask);
		}
		if (ifa->ifa_dstaddr) {
			free(ifa->ifa_dstaddr);
		}
		prev = ifa;
		ifa = prev->ifa_next;
		free(prev);
	}
}


#ifdef CONFIG_NET_NETMGR_ZEROCOPY
static err_t lwip_linkoutput(struct netif *nic, struct pbuf *buf)
{
	struct netdev *dev = LW_GETND(nic);

	int res = ND_NETOPS(dev, linkoutput)(dev, (void *)buf, 0);
	if (res < 0) {
		return ERR_IF;
	}

	return ERR_OK;
}


static int lwip_input(struct netdev *dev, void *frame_ptr, uint16_t len)
{
	(void)len;
	if (!dev || !frame_ptr) {
		return -1;
	}

	struct pbuf *p = (struct pbuf *)frame_ptr;
	struct eth_hdr *ethhdr = p->payload;

	switch (htons(ethhdr->type)) {
	case ETHTYPE_IP:
#if LWIP_IPV6
	case ETHTYPE_IPV6:
#endif
	case ETHTYPE_ARP:
#if PPPOE_SUPPORT
	case ETHTYPE_PPPOEDISC:
	case ETHTYPE_PPPOE:
#endif
	{
		/* full packet send to tcpip_thread to process */
		if (netif->input(p, netif) != ERR_OK) {
			LWIP_DEBUGF(NETIF_DEBUG, ("input processing error\n"));
			LINK_STATS_INC(link.err);
		} else {
			LINK_STATS_INC(link.recv);
		}
	}
	break;
	default:
		LWIP_DEBUGF(NETIF_DEBUG, ("not supported ethernet type error\n"));
		break;
	}
	return 0;
}
#else /*  CONFIG_NET_NETMGR_ZEROCOPY */
static err_t lwip_linkoutput(struct netif *nic, struct pbuf *buf)
{
	struct netdev *dev = LW_GETND(nic);
	int offset = 0;
	struct pbuf *tbuf = buf;
	while (tbuf) {
		memcpy((void *)&dev->tx_buf[offset], (void *)tbuf->payload, tbuf->len);
		offset += tbuf->len;
		tbuf = tbuf->next;
	}

	int res = ND_NETOPS(dev, linkoutput)(dev, dev->tx_buf, offset);
	if (res < 0) {
		return ERR_IF;
	}

	return ERR_OK;
}


static int lwip_input(struct netdev *dev, void *frame_ptr, uint16_t len)
{
	LWIP_DEBUGF(NETIF_DEBUG, ("passing to LWIP layer, packet len %d \n", len));
	struct pbuf *p, *q;
	/* Receive the complete packet */
	/* Obtain the size of the packet and put it into the "len" variable. */
	if (0 == len) {
		return 0;
	}
	struct netif *netif = GET_NETIF_FROM_NETDEV(dev);
	/* We allocate a pbuf chain of pbufs from the pool. */
	p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

	if (!p) {
		LWIP_DEBUGF(NETIF_DEBUG, ("mem error\n"));
		LINK_STATS_INC(link.memerr);
		LINK_STATS_INC(link.drop);
		return -1;
	}
	LWIP_DEBUGF(NETIF_DEBUG, ("processing pbufs\n"));

	/* We iterate over the pbuf chain until we have read the entire packet into the pbuf. */
	for (q = p; q != NULL; q = q->next) {
		memcpy(q->payload, frame_ptr, q->len);
		frame_ptr += q->len;
	}

	struct eth_hdr *ethhdr = p->payload;
	switch (htons(ethhdr->type)) {
	case ETHTYPE_IP:
#if LWIP_IPV6
	case ETHTYPE_IPV6:
#endif
	case ETHTYPE_ARP:
#if PPPOE_SUPPORT
	case ETHTYPE_PPPOEDISC:
	case ETHTYPE_PPPOE:
#endif
	{
		/* full packet send to tcpip_thread to process */
		if (netif->input(p, netif) != ERR_OK) {
			LWIP_DEBUGF(NETIF_DEBUG, ("input processing error\n"));
			LINK_STATS_INC(link.err);
			pbuf_free(p);
		} else {
			LINK_STATS_INC(link.recv);
		}
	}
	break;
	default:
		pbuf_free(p);
		p = NULL;
		break;
	}

	return 0;
}
#endif /*  CONFIG_NET_NETMGR_ZEROCOPY */


static err_t lwip_set_multicast_list(struct netif *nic, const ip4_addr_t *group, enum netif_mac_filter_action action)
{
	struct netdev *dev = LW_GETND(nic);
	struct in_addr addr;
	addr.s_addr = group->addr;
	int res = ND_NETOPS(dev, igmp_mac_filter)(dev, &addr,
											(action == NETIF_DEL_MAC_FILTER) ? NM_DEL_MAC_FILTER : NM_ADD_MAC_FILTER);
	if (res < 0) {
		return ERR_IF;
	}
	return ERR_OK;
}


#ifdef CONFIG_NET_IPv4
static int lwip_get_ip4addr(struct netdev *dev, struct sockaddr *addr, int type)
{
	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);
	if (type == NETDEV_IP) {
		_convert_ip4addr_lton((struct sockaddr_in *)addr, &ni->ip_addr);
	} else if (type == NETDEV_GW) {
		_convert_ip4addr_lton((struct sockaddr_in *)addr, &ni->gw);
	} else if (type == NETDEV_NETMASK) {
		_convert_ip4addr_lton((struct sockaddr_in *)addr, &ni->netmask);
	} else {
		return -1;
	}

	return 0;
}


static int lwip_set_ip4addr(struct netdev *dev, struct sockaddr *addr, int type)
{
	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);
	uint8_t flags = ni->flags;
	if (flags & NETIF_FLAG_UP) {
		_netif_soft_ifdown(ni);
	}

	ip_addr_t ipaddr = ni->ip_addr;
	ip_addr_t netmask = ni->netmask;
	ip_addr_t gw = ni->gw;

	if (type == NETDEV_IP) {
		_convert_ip4addr_ntol(&ipaddr, (struct sockaddr_in *)addr);
	} else if (type == NETDEV_GW) {
		_convert_ip4addr_ntol(&gw, (struct sockaddr_in *)addr);
	} else if (type == NETDEV_NETMASK) {
		_convert_ip4addr_ntol(&netmask, (struct sockaddr_in *)addr);
	}
	netifapi_netif_set_addr(ni, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));

	if (flags & NETIF_FLAG_UP) {
		_netif_soft_ifup(ni);
	}

	return 0;
}
#else /*  CONFIG_NET_IPv4 */
static int lwip_get_ip4addr(struct netdev *dev, struct sockaddr *addr, int type)
{
	return -ENOTTY;
}

static int lwip_set_ip4addr(struct netdev *dev, struct sockaddr *addr, int type)
{
	return -ENOTTY;
}
#endif /*  CONFIG_NET_IPv4 */


#ifdef CONFIG_NET_IPv6
static int lwip_set_ip6addr(struct netdev *dev, struct sockaddr_storage *addr, int type)
{
	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);

	uint8_t flags = ni->flags;
	if (flags & NETIF_FLAG_UP) {
		_netif_soft_ifdown(ni);
	}

	if (type == NETDEV_IP) {
		_netif_setip6addr(ni, addr);
	} else if (type == NETDEV_GW) {
		/*  ToDo it's not supported to set ipv6 gateway */
		_convert_ip6addr_ntol(&ni->gw, (struct sockaddr_in6 *)addr);
	} else if (type == NETDEV_NETMASK) {
		/*  ToDo it's not supported to set ipv6 netmask */
		_convert_ip6addr_ntol(&ni->netmask, (struct sockaddr_in6 *)addr);
	} else {
		ndbg("unknown type\n");
	}

	if (flags & NETIF_FLAG_UP) {
		_netif_soft_ifup(ni);
	}

	return 0;
}
#else /* CONFIG_NET_IPv6 */
static int lwip_set_ip6addr(struct netdev *dev, struct sockaddr_storage *addr, int type)
{
	return -ENOTTY;
}
#endif /* CONFIG_NET_IPv6 */


static int lwip_get_ifaddrs(struct netdev *dev, struct ifaddrs **addrs)
{
	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);
	if (!ni) {
		return -1;
	}

	struct ifaddrs *cursor = NULL, *root = NULL;

	// get ipv4 address
	struct ifaddrs *ifa4 = (struct ifaddrs *)zalloc(sizeof(struct ifaddrs));
	if (!ifa4) {
		return -1;
	}
	ifa4->ifa_name = (char *)zalloc(IFNAMSIZ);
	if (!ifa4->ifa_name) {
		goto free_list;
	}
	strncpy(ifa4->ifa_name, dev->ifname, IFNAMSIZ - 1);

	FAR struct sockaddr_in *dest = (struct sockaddr_in *)zalloc(sizeof(struct sockaddr_in) * 3);
	if (!dest) {
		goto free_list;
	}

	_convert_ip4addr_lton(&dest[0], &ni->ip_addr);
	ifa4->ifa_addr = (struct sockaddr *)(&dest[0]);
	_convert_ip4addr_lton(&dest[1], &ni->gw);
	ifa4->ifa_netmask = (struct sockaddr *)(&dest[1]);
	_convert_ip4addr_lton(&dest[2], &ni->netmask);
	ifa4->ifa_dstaddr = (struct sockaddr *)&dest[2];

	cursor = root = ifa4;

#ifdef CONFIG_NET_IPv6
	// get ipv6 address
	for (int j = 0; j < CONFIG_NET_IPv6_NUM_ADDRESSES; j++) {
		if (ni->ip6_addr_state[j] == IP6_ADDR_INVALID) {
			continue;
		}
		struct ifaddrs *ifa6 = (struct ifaddrs *)zalloc(sizeof(struct ifaddrs));
		if (!ifa6) {
			goto free_list;
		}
		cursor->ifa_next = ifa6;
		cursor = ifa6;

		ifa6->ifa_name = (char *)zalloc(IFNAMSIZ);
		if (!ifa6->ifa_name) {
			goto free_list;
		}
		strncpy(ifa6->ifa_name, dev->ifname, IFNAMSIZ - 1);

		struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)zalloc(sizeof(struct sockaddr_in6));
		if (!sin6) {
			goto free_list;
		}
		_convert_ip6addr_lton(sin6, &ni->ip6_addr[j]);

		cursor->ifa_addr = (struct sockaddr *)sin6;
	}
#endif /* CONFIG_NET_IPv6 */
	*addrs = root;

	return 0;
free_list:
	_free_ifaddrs(root);
	*addrs = NULL;

	return -1;
}


static int lwip_delete_ipaddr(struct netdev *dev)
{
	// ToDo: check that it is correct to down the interface when delete an IP?
	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);
	int res = _netif_down(ni);
	if (res < 0) {
		return -ENOTTY;
	}
#ifdef CONFIG_NET_IPv6
	memset(ip_2_ip6(&ni->ip_addr), 0, 16);
#endif
	return 0;
}


static int lwip_get_hwaddr(struct netdev *dev, struct sockaddr *hwaddr)
{
	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);
	hwaddr->sa_family = AF_INETX;
	memcpy(hwaddr->sa_data, ni->hwaddr, IFHWADDRLEN);
	return 0;
}


static int lwip_set_hwaddr(struct netdev *dev, struct sockaddr *hwaddr)
{
	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);
	memcpy(ni->hwaddr, hwaddr->sa_data, IFHWADDRLEN);
	return 0;
}


static int lwip_get_mtu(struct netdev *dev, int *mtu)
{
	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);
	*mtu = ni->mtu;
	return 0;
}


static int lwip_get_flag(struct netdev *dev, uint8_t *flag)
{
	// initialize flag.
	// if it has invalid value then it would pass wrong result.
	*flag = 0x0;

	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);
	if (ni->flags & NETIF_FLAG_LINK_UP) {
		IFF_SET_UP(*flag);
	} else {
		IFF_SET_DOWN(*flag);
	}

	if (ni->flags & NETIF_FLAG_UP) {
		IFF_SET_RUNNING(*flag);
	}
	/*	lwIP doesn't check loopback device */
	if (dev->type == NM_LOOPBACK) {
		*flag |= IFF_LOOPBACK;
	}

	return 0;
}


static int lwip_ifup(struct netdev *dev)
{
	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);

	/* Is the interface already up? */
	if (ni->flags & NETIF_FLAG_LINK_UP) {
		ndbg("netif is already up\n");
		return 0;
	}

	if (_netif_up(ni) < 0) {
		return -1;
	}

	/* Below logic is not processed by lwIP thread.
	 * So it can cause conflict to lwIP thread later.
	 * But now there are no APIs that can manage IPv6 auto-config.
	 * this will be handled carefully later.
	 */
#ifdef CONFIG_NET_IPv6
	/* IPV6 auto configuration : Link-Local address */
	nvdbg("IPV6 link local address auto config\n");

#ifdef CONFIG_NET_IPv6_AUTOCONFIG
	/* enable IPv6 address stateless auto-configuration */
	netif_set_ip6_autoconfig_enabled(ni, 1);
#endif /* CONFIG_NET_IPv6_AUTOCONFIG */
	/* To auto-config linklocal address, ni should have mac address already */
	netif_create_ip6_linklocal_address(ni);
	ndbg("generated IPV6 linklocal address - %X : %X : %X : %X\n",
		 PP_HTONL(ip_2_ip6(&ni->ip6_addr[0])->addr[0]),
		 PP_HTONL(ip_2_ip6(&ni->ip6_addr[0])->addr[1]),
		 PP_HTONL(ip_2_ip6(&ni->ip6_addr[0])->addr[2]),
		 PP_HTONL(ip_2_ip6(&ni->ip6_addr[0])->addr[3]));
#ifdef CONFIG_NET_IPv6_MLD
	ip6_addr_t solicit_addr;

	/* set MLD6 group to receive solicit multicast message */
	ip6_addr_set_solicitednode(&solicit_addr, ip_2_ip6(&ni->ip6_addr[0])->addr[3]);
	mld6_joingroup_netif(ni, &solicit_addr);
	ndbg("MLD6 group added - %X : %X : %X : %X\n",
		 PP_HTONL(solicit_addr.addr[0]), PP_HTONL(solicit_addr.addr[1]),
		 PP_HTONL(solicit_addr.addr[2]), PP_HTONL(solicit_addr.addr[3]));
#endif /* CONFIG_NET_IPv6_MLD */
#endif /* CONFIG_NET_IPv6 */
	return 0;
}


static int lwip_ifdown(struct netdev *dev)
{
	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);
	int res = _netif_down(ni);
	if (res < 0) {
		return -ENOTTY;
	}

	return 0;
}


static int lwip_joingroup(struct netdev *dev, struct in_addr *addr)
{
	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);
	struct ip4_addr a4 = {.addr = addr->s_addr};
	return igmp_joingroup(ip_2_ip4(&(ni->ip_addr)), &a4);
}


static int lwip_leavegroup(struct netdev *dev, struct in_addr *addr)
{
	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);
	struct ip4_addr a4 = {.addr = addr->s_addr};
	return igmp_leavegroup(ip_2_ip4(&(ni->ip_addr)), &a4);
}


static int lwip_init_nic(struct netdev *dev, struct nic_config *config)
{
	if (!dev) {
		return -1;
	}

	char *rnetif = (char *)kmm_malloc(sizeof(struct netif) + sizeof(struct netdev *));
	if (!rnetif) {
		return -1;
	}
	struct netif *nic = (struct netif *)rnetif;
	struct netdev **ndev = (struct netdev **)(&rnetif[sizeof(struct netif)]);
	*ndev = dev;

	((struct netdev_ops *)dev->ops)->nic = (void *)nic;

	if (config->loopback) {
		_lwip_init_loop(nic);
		return 0;
	}

	nic->mtu = CONFIG_NET_ETH_MTU;
	nic->hwaddr_len = config->hwaddr_len;
#if LWIP_IPV6
	nic->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */
	nic->output = etharp_output;
	nic->linkoutput = lwip_linkoutput;
#if LWIP_IPV4 && LWIP_IGMP
	nic->igmp_mac_filter = lwip_set_multicast_list;
#endif
	nic->num = g_num++;

	/*
	 * Initialize the snmp variables and counters inside the struct netif.
	 * The last argument should be replaced with your link speed, in units
	 * of bits per second.
	 */
	MIB2_INIT_NETIF(nic, snmp_ifType_ethernet_csmacd, 0);

	struct ip4_addr ipaddr;
	struct ip4_addr netmask;
	struct ip4_addr gw;
	struct sockaddr_in *taddr = (struct sockaddr_in *)&config->addr;
	ipaddr.addr = taddr->sin_addr.s_addr;
	taddr = (struct sockaddr_in *)&config->netmask;
	netmask.addr = taddr->sin_addr.s_addr;
	taddr = (struct sockaddr_in *)&config->gw;
	gw.addr = taddr->sin_addr.s_addr;
	netif_add(nic, &ipaddr, &netmask, &gw, NULL, _lwip_nic_init, tcpip_input);
	if (config->is_default) {
		netif_set_default(nic);
	}

	// Set lwIP flag: lwip flag and netmgr flag should be same.
	nic->flags = config->flag;
#if LWIP_IPV6_MLD
	nic->flags |= NETIF_FLAG_MLD6;
#endif

	return 0;
}


static int lwip_deinit_nic(struct netdev *dev)
{
	if (!dev) {
		return -1;
	}

	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);
	if (ni) {
		kmm_free((void *)ni);
	}
	ND_NETOPS(dev, nic) = NULL;
	//((struct netdev_ops *)(dev->ops))->nic = NULL;

	return 0;
}


#ifdef CONFIG_NET_NETMON
static int lwip_get_stats(struct netdev *dev, struct netmon_netdev_stats *stats)
{
	struct netif *ni = GET_NETIF_FROM_NETDEV(dev);
	stats->devinpkts = ni->mib2_counters.ifinucastpkts +
		ni->mib2_counters.ifinnucastpkts +
		ni->mib2_counters.ifindiscards +
		ni->mib2_counters.ifinerrors +
		ni->mib2_counters.ifinunknownprotos;

	stats->devinoctets = ni->mib2_counters.ifinoctets;
	stats->devoutpkts = ni->mib2_counters.ifoutucastpkts +
		ni->mib2_counters.ifoutnucastpkts +
		ni->mib2_counters.ifoutdiscards +
		ni->mib2_counters.ifouterrors;

	stats->devoutoctets = ni->mib2_counters.ifoutoctets;

	return 0;
}
#endif


struct netdev_ops *get_netdev_ops_lwip(void)
{
	struct netdev_ops *netdev_ops = (struct netdev_ops *)kmm_malloc(sizeof(struct netdev_ops));
	if (!netdev_ops) {
		return NULL;
	}
	netdev_ops->init_nic = lwip_init_nic;
	netdev_ops->deinit_nic = lwip_deinit_nic;

	netdev_ops->get_ip4addr = lwip_get_ip4addr;
	netdev_ops->set_ip4addr = lwip_set_ip4addr;
	netdev_ops->set_ip6addr = lwip_set_ip6addr;
	netdev_ops->get_ifaddrs = lwip_get_ifaddrs;
	netdev_ops->delete_ipaddr = lwip_delete_ipaddr;

	netdev_ops->get_hwaddr = lwip_get_hwaddr;
	netdev_ops->set_hwaddr = lwip_set_hwaddr;

	netdev_ops->get_mtu = lwip_get_mtu;
	netdev_ops->get_flag = lwip_get_flag;

	netdev_ops->ifup = lwip_ifup;
	netdev_ops->ifdown = lwip_ifdown;

	netdev_ops->joingroup = lwip_joingroup;
	netdev_ops->leavegroup = lwip_leavegroup;

	netdev_ops->input = lwip_input;
#ifdef CONFIG_NET_NETMON
	netdev_ops->get_stats = lwip_get_stats;
#endif
	netdev_ops->nic = NULL;

	return netdev_ops;
}
#endif // _NETDEV_LWIP_OPS_H__
