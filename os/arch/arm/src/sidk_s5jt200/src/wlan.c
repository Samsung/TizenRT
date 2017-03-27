/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
#include <net/lwip/opt.h>

#include <tinyara/config.h>
#include <stdio.h>
#include <debug.h>

#if defined(CONFIG_NET_LWIP)
#include <net/lwip/opt.h>
#include <net/lwip/mem.h>
#include <net/lwip/memp.h>
#include <net/lwip/dhcp.h>
#include <net/lwip/tcpip.h>
#include <net/lwip/init.h>
#endif

#include <stdlib.h>

#include <net/lwip/def.h>
#include <net/lwip/mem.h>
#include <net/lwip/pbuf.h>
#include <net/lwip/stats.h>
#include <net/lwip/snmp.h>
#include <net/lwip/netif/etharp.h>
#include <net/lwip/netif/ppp_oe.h>

#include <net/lwip/sys.h>

#include <net/if.h>

struct wlanif {
	struct eth_addr *ethaddr;
	/* Add whatever per-interface state that is needed here. */
};

/* Define those to better describe your network interface. */
#define IFNAME0 'w'
#define IFNAME1 'l'

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t wlan_init(struct netif *netif)
{
#if 0
	struct wlanif *wlanif;

	LWIP_ASSERT("netif != NULL", (netif != NULL));

	wlanif = mem_malloc(sizeof(struct wlanif));
	if (wlanif == NULL) {
		LWIP_DEBUGF(NETIF_DEBUG, ("wlanif_init: out of memory\n"));
		return ERR_MEM;
	}
#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	netif->hostname = "wlip";
#endif							/* LWIP_NETIF_HOSTNAME */

	/*
	 * Initialize the snmp variables and counters inside the struct netif.
	 * The last argument should be replaced with your link speed, in units
	 * of bits per second.
	 */
	NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

	netif->state = wlanif;
	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;

	snprintf(netif->d_ifname, IFNAMSIZ, "%c%c%d", netif->name[0], netif->name[1], netif->num);

	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...) */
	//netif->output = etharp_output;
	//netif->linkoutput = low_level_output;

	wlanif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

	/* initialize the hardware */
	// low_level_init(netif);
#endif
	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;

	snprintf(netif->d_ifname, IFNAMSIZ, "%c%c%d", netif->name[0], netif->name[1], netif->num);

	netif->flags = NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_BROADCAST | NETIF_FLAG_IGMP;

	return ERR_OK;
}
