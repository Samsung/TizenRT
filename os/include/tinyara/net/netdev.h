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
/****************************************************************************
 * Defines architecture-specific device driver interfaces to the uIP network.
 *
 *   Copyright (C) 2007, 2009, 2011-2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Derived largely from portions of uIP with has a similar BSD-styple license:
 *
 *   Copyright (c) 2001-2003, Adam Dunkels.
 *   All rights reserved.
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

#ifndef __INCLUDE_NET_NETDEV_H
#define __INCLUDE_NET_NETDEV_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/ioctl.h>
#include <stdint.h>
#include <net/if.h>

#include <net/ethernet.h>
#include <arpa/inet.h>

#include <tinyara/net/ip.h>

#ifndef CONFIG_NET_MULTIBUFFER
#include <net/lwip/netif.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* This structure collects information that is specific to a specific network
 * interface driver.  If the hardware platform supports only a single instance
 * of this structure.
 */

struct net_driver_s {
	/* This link is used to maintain a single-linked list of ethernet drivers.
	 * Must be the first field in the structure due to blink type casting.
	 */

#if CONFIG_NSOCKET_DESCRIPTORS > 0
	FAR struct net_driver_s *flink;

	/* This is the name of network device assigned when netdev_register was called.
	 * This name is only used to support socket ioctl lookups by device name
	 * Examples: "eth0"
	 */

	char d_ifname[IFNAMSIZ];
#endif

	/* Drivers interface flags.  See IFF_* definitions in include/net/if.h */

	uint8_t d_flags;

#ifdef CONFIG_NET_MULTILINK
	/* Multi network devices using multiple data links protocols are selected */

	uint8_t d_lltype;			/* See enum net_lltype_e */
	uint8_t d_llhdrlen;			/* Link layer header size */
	uint16_t d_mtu;				/* Maximum packet size */
#endif

#ifdef CONFIG_NET_ETHERNET
	/* Ethernet device identity */

	struct ether_addr d_mac;	/* Device MAC address */
#endif

	/* Network identity */

#ifdef CONFIG_NET_IPv4
	in_addr_t d_ipaddr;			/* Host IPv4 address assigned to the network interface */
	in_addr_t d_draddr;			/* Default router IP address */
	in_addr_t d_netmask;		/* Network subnet mask */
#endif

#ifdef CONFIG_NET_IPv6
	net_ipv6addr_t d_ipv6addr;	/* Host IPv6 address assigned to the network interface */
	net_ipv6addr_t d_ipv6draddr;	/* Default router IPv6 address */
	net_ipv6addr_t d_ipv6netmask;	/* Network IPv6 subnet mask */
#endif

	/* The d_buf array is used to hold incoming and outgoing packets. The device
	 * driver should place incoming data into this buffer. When sending data,
	 * the device driver should read the link level headers and the TCP/IP
	 * headers from this buffer. The size of the link level headers is
	 * configured by the NET_LL_HDRLEN(dev) define.
	 *
	 * uIP will handle only a single buffer for both incoming and outgoing
	 * packets.  However, the drive design may be concurrently send and
	 * filling separate, break-off buffers if CONFIG_NET_MULTIBUFFER is
	 * defined.  That buffer management must be controlled by the driver.
	 */

#ifdef CONFIG_NET_MULTIBUFFER
	uint8_t *d_buf;
#else
	uint8_t d_buf[MAX_NET_DEV_MTU + CONFIG_NET_GUARDSIZE];
#endif

	/* d_appdata points to the location where application data can be read from
	 * or written to in the the packet buffer.
	 */

	uint8_t *d_appdata;

	/* The length of the packet in the d_buf buffer.
	 *
	 * Holds the length of the packet in the d_buf buffer.
	 *
	 * When the network device driver calls the uIP input function,
	 * d_len should be set to the length of the packet in the d_buf
	 * buffer.
	 *
	 * When sending packets, the device driver should use the contents of
	 * the d_len variable to determine the length of the outgoing
	 * packet.
	 */

	uint16_t d_len;

	/* When d_buf contains outgoing xmit data, d_sndlen is non-zero and represents
	 * the amount of application data after d_appdata
	 */

	uint16_t d_sndlen;

	/* Driver callbacks */

	int (*d_ifup)(FAR struct net_driver_s *dev);
	int (*d_ifdown)(FAR struct net_driver_s *dev);
	int (*d_ifstate)(FAR struct net_driver_s *dev);	// request TX to driver
	int (*d_txavail)(FAR struct net_driver_s *dev);
	int (*d_txpoll)(FAR struct net_driver_s *dev);	// request TX to driver
	int (*d_macout)(FAR struct net_driver_s *dev);	// generate mac header
#ifdef CONFIG_NET_RXAVAIL
	int (*d_rxavail)(FAR struct net_driver_s *dev);
#endif

#ifdef CONFIG_NETDEV_PHY_IOCTL
	int (*d_ioctl)(FAR struct net_driver_s *dev, int cmd, long arg);
#endif

	/* Drivers may attached device-specific, private information */

	void *d_private;
};

typedef int (*netif_poll_callback_t)(FAR struct net_driver_s *dev);

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
/****************************************************************************
 * Polling of connections
 *
 * These functions will traverse each active uIP connection structure and
 * perform appropriate operations:  netif_timer() will perform TCP timer
 * operations (and UDP polling operations); netif_poll() will perform TCP
 * and UDP polling operations. The CAN driver MUST implement logic to
 * periodically call netif_timer(); netif_poll() may be called asynchronously
 * from the network driver can accept another outgoing packet.
 *
 * In both cases, these functions will call the provided callback function
 * for every active connection. Polling will continue until all connections
 * have been polled or until the user-supplied function returns a non-zero
 * value (which it should do only if it cannot accept further write data).
 *
 * When the callback function is called, there may be an outbound packet
 * waiting for service in the uIP packet buffer, and if so the d_len field
 * is set to a value larger than zero. The device driver should then send
 * out the packet.
 *
 * Example:
 *   int driver_callback(FAR struct net_driver_s *dev)
 *   {
 *     if (dev->d_len > 0)
 *       {
 *         devicedriver_send();
 *         return 1; <-- Terminates polling if necessary
 *       }
 *     return 0;
 *   }
 *
 *   ...
 *   netif_poll(dev, driver_callback);
 *
 * Note: If you are writing a uIP device driver that needs ARP (Address
 * Resolution Protocol), e.g., when running uIP over Ethernet, you will
 * need to call the arp_out() function in the callback function
 * before sending the packet:
 *
 *   int driver_callback(FAR struct net_driver_s *dev)
 *   {
 *     if (dev->d_len > 0)
 *       {
 *         arp_out();
 *         devicedriver_send();
 *         return 1; <-- Terminates polling if necessary
 *       }
 *
 *     return 0;
 *   }
 *
 ****************************************************************************/
#ifndef CONFIG_NET_LWIP
int netif_poll(FAR struct net_driver_s *dev, netif_poll_callback_t callback);
#endif
int netif_timer(FAR struct net_driver_s *dev, netif_poll_callback_t callback, int hsec);

/****************************************************************************
 * Name: neighbor_out
 *
 * Description:
 *   This function should be called before sending out an IPv6 packet. The
 *   function checks the destination IPv6 address of the IPv6 packet to see
 *   what Ethernet MAC address that should be used as a destination MAC
 *   address on the Ethernet.
 *
 *   If the destination IPv6 address is in the local network (determined
 *   by logical ANDing of netmask and our IPv6 address), the function
 *   checks the Neighbor Table to see if an entry for the destination IPv6
 *   address is found.  If so, an Ethernet header is pre-pended at the
 *   beginning of the packet and the function returns.
 *
 *   If no Neighbor Table entry is found for the destination IPv6 address,
 *   the packet in the d_buf[] is replaced by an ICMPv6 Neighbor Solict
 *   request packet for the IPv6 address. The IPv6 packet is dropped and
 *   it is assumed that the higher level protocols (e.g., TCP) eventually
 *   will retransmit the dropped packet.
 *
 *   Upon return in either the case, a packet to be sent is present in the
 *   d_buf[] buffer and the d_len field holds the length of the Ethernet
 *   frame that should be transmitted.
 *
 ****************************************************************************/

#ifdef CONFIG_NET_IPv6
void neighbor_out(FAR struct net_driver_s *dev);
#endif							/* CONFIG_NET_IPv6 */

/****************************************************************************
 * Carrier detection
 *
 * Call netdev_carrier_on when the carrier has become available and the device
 * is ready to receive/transmit packets.
 *
 * Call detdev_carrier_off when the carrier disappeared and the device has
 * moved into non operational state.
 *
 ****************************************************************************/

int netdev_carrier_on(FAR struct net_driver_s *dev);
int netdev_carrier_off(FAR struct net_driver_s *dev);

/****************************************************************************
 * Name: ipv6_chksum
 *
 * Description:
 *   Calculate the IPv6 header checksum of the packet header in d_buf.
 *
 *   The IPv6 header checksum is the Internet checksum of the 40 bytes of
 *   the IPv6 header.
 *
 *   If CONFIG_NET_ARCH_CHKSUM is defined, then this function must be
 *   provided by architecture-specific logic.
 *
 * Returned Value:
 *   The IPv6 header checksum of the IPv6 header in the d_buf buffer.
 *
 ****************************************************************************/

#ifdef CONFIG_NET_IPv6
uint16_t ipv6_chksum(FAR struct net_driver_s *dev);
#endif
#endif							/* __INCLUDE_NET_NETDEV_H */
