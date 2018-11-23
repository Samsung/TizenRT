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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>
#include <assert.h>
#include <net/if.h>
#include <tinyara/net/ethernet.h>

#ifdef CONFIG_NET_LWIP

#include <net/lwip/netif.h>
#include <net/lwip/opt.h>
#include <net/lwip/debug.h>
#include <net/lwip/def.h>
#include <net/lwip/mem.h>
#include <net/lwip/pbuf.h>
#include <net/lwip/stats.h>
#include <net/lwip/ip_addr.h>
#include <net/lwip/snmp.h>

#include <net/lwip/netif/etharp.h>
#include <net/lwip/ethip6.h>

#define ETHERNET_MTU 1500

/********************************************************************
 * Private Functions
 ********************************************************************/

/* List of registered Ethernet device drivers */

struct netif *g_netdevices = NULL;

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

#define ETH_HWADDR_0 0x00
#define ETH_HWADDR_1 0x01
#define ETH_HWADDR_2 0x01
#define ETH_HWADDR_3 0x66
#define ETH_HWADDR_4 0x73
#define ETH_HWADDR_5 0x38

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
	struct ether_addr *ethaddr;
	/* Add whatever per-interface state that is needed here. */
};

#ifdef LWIP_NETIF_STATUS_CALLBACK
void ethernetif_status_callback(struct netif *netif)
{
#if 0
	if (netif->flags & NETIF_FLAG_UP) {
		printf("IP: %d.%d.%d.%d\n", netif->ip_addr.addr, ip4_addr2_16(&netif->ip_addr), ip4_addr3_16(&netif->ip_addr), ip4_addr4_16(&netif->ip_addr));
	}
#endif
}
#endif

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t ethernetif_output(struct netif *netif, struct pbuf *p)
{
	struct pbuf *q;

	netif->d_len = 0;
	q = p;
	while (q) {
		/* Copy the data from the pbuf to the interface buf, one pbuf at a
		   time. The size of the data in each pbuf is kept in the ->len
		   variable. */
		memcpy((void *)(&(netif->d_buf[netif->d_len])), q->payload, q->len);
		netif->d_len += q->len;

		/* Check if this is the last pbuf of the packet. If yes, then break */
		if (q->len == q->tot_len) {
			break;
		} else {
			q = q->next;
		}
	}
	/* signal that packet should be sent */
	netif->d_txavail(netif);

	LINK_STATS_INC(link.xmit);

	return ERR_OK;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function enc_pktif() that
 * handles the actual reception of bytes from the network
 * interface. Then the received pkt is sent to LWIP layer for processing.
 *
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
int ethernetif_input(struct netif *netif)
{

	LWIP_DEBUGF(NETIF_DEBUG, ("passing to LWIP layer, packet len %d \n", netif->d_len));
	struct pbuf *p, *q;
	u16_t len = 0;
	u8_t *frame_ptr;
	/* Receive the complete packet */
	frame_ptr = &(netif->d_buf[0]);
	/* Obtain the size of the packet and put it into the "len" variable. */
	len = netif->d_len;
	if (0 == len) {
		return 0;
	}
	/* We allocate a pbuf chain of pbufs from the pool. */
	p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

	LWIP_DEBUGF(NETIF_DEBUG, ("processing pbufs\n"));
	/* We iterate over the pbuf chain until we have read the entire packet into the pbuf. */
	if (p != NULL) {
		for (q = p; q != NULL; q = q->next) {
			memcpy(q->payload, frame_ptr, q->len);
			frame_ptr += q->len;
		}
		/* full packet send to tcpip_thread to process */
		if (netif->input(p, netif) != ERR_OK) {
			LWIP_DEBUGF(NETIF_DEBUG, ("input processing error\n"));
			LINK_STATS_INC(link.err);
			pbuf_free(p);
			/* Don't reference the packet any more! */
			p = NULL;
		} else {
			LINK_STATS_INC(link.recv);
		}
	} else {
		LWIP_DEBUGF(NETIF_DEBUG, ("mem error\n"));
		LINK_STATS_INC(link.memerr);
		LINK_STATS_INC(link.drop);
		return -1;
	}
	return 0;
}

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
err_t ethernetif_init(struct netif *netif)
{
	LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	netif->hostname = "lwip";
#endif							/* LWIP_NETIF_HOSTNAME */

	/*
	 * Initialize the snmp variables and counters inside the struct netif.
	 * The last argument should be replaced with your link speed, in units
	 * of bits per second.
	 */
	//NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;

	/* Initialize the interface d_ifname which is used for netdev API's and if APi's  */
	snprintf(netif->d_ifname, IFNAMSIZ, "%c%c%d", netif->name[0], netif->name[1], netif->num);

	netif->hwaddr_len = IFHWADDRLEN;
	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...) */
#if LWIP_IPV4 && LWIP_IPV6
	netif->output = etharp_output;
	netif->output_ip6 = ethip6_output;
#elif LWIP_IPV4
	netif->output = etharp_output;
#else
	netif->output_ip6 = ethip6_output;
#endif
	/* netif->linkoutput is set in enc_initialize function */
	netif->linkoutput = ethernetif_output;
	netif->mtu = ETHERNET_MTU;
	/* device capabilities */
	/* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_IGMP;
#if LWIP_NETIF_STATUS_CALLBACK
	netif->status_callback = enc28j60_status_callback;
#endif

#ifdef CONFIG_ARCH_HAVE_MAC
	/* set MAC hardware address */
	netif->hwaddr[0] = (CONFIG_TASH_MACADDR >> (8 * 5)) & 0xff;
	netif->hwaddr[1] = (CONFIG_TASH_MACADDR >> (8 * 4)) & 0xff;
	netif->hwaddr[2] = (CONFIG_TASH_MACADDR >> (8 * 3)) & 0xff;
	netif->hwaddr[3] = (CONFIG_TASH_MACADDR >> (8 * 2)) & 0xff;
	netif->hwaddr[4] = (CONFIG_TASH_MACADDR >> (8 * 1)) & 0xff;
	netif->hwaddr[5] = (CONFIG_TASH_MACADDR >> (8 * 0)) & 0xff;

	memcpy(netif->d_mac.ether_addr_octet, netif->hwaddr, IFHWADDRLEN);
#else
	/* set MAC hardware address */
	netif->hwaddr[0] = ETH_HWADDR_0;
	netif->hwaddr[1] = ETH_HWADDR_1;
	netif->hwaddr[2] = ETH_HWADDR_2;
	netif->hwaddr[3] = ETH_HWADDR_3;
	netif->hwaddr[4] = ETH_HWADDR_4;
	netif->hwaddr[5] = ETH_HWADDR_5;

	memcpy(netif->d_mac.ether_addr_octet, netif->hwaddr, IFHWADDRLEN);
#endif
#if CONFIG_NET_LWIP
	netif->d_ipaddr = ip4_addr_get_u32(ip_2_ip4(&netif->ip_addr));
	netif->d_draddr = ip4_addr_get_u32(ip_2_ip4(&netif->gw));
	netif->d_netmask = ip4_addr_get_u32(ip_2_ip4(&netif->netmask));
#else
	netif->d_ipaddr = netif->ip_addr.s_addr.addr;
	netif->d_draddr = netif->gw.s_addr.addr;
	netif->d_netmask = netif->netmask.s_addr.addr;
#endif
	//memcpy(netif->d_mac.ether_addr_octet,netif->hwaddr, IFHWADDRLEN);
	/* Do whatever else is needed to initialize interface. */
	return ERR_OK;
}

#endif
