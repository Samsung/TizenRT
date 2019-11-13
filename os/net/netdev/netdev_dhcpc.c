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
/****************************************************************************
 * net/netdev/netdev_dhcpc.c
 *
 *   Copyright (C) 2007, 2009, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Based heavily on portions of uIP:
 *
 *   Author: Adam Dunkels <adam@dunkels.com>
 *   Copyright (c) 2005, Swedish Institute of Computer Science
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>		/* TinyAra configuration */

#include <sys/types.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <debug.h>
#include <net/if.h>

#include <protocols/dhcpc.h>
#include <netutils/netlib.h>
#include "lwip/netif.h"
#include "lwip/netifapi.h"
#include "netdev/netdev.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define DHCPC_SET_IP4ADDR(intf, ip, netmask, gateway)	        \
	do {							\
		int res = -1;					\
		res = _netlib_set_ipv4addr(intf, &ip);		\
		if (res == -1) {				\
			nvdbg("set ipv4 addr error\n");	\
		}						\
		res = _netlib_set_ipv4netmask(intf, &netmask);	\
		if (res == -1) {				\
			nvdbg("set netmask addr error\n");	\
		}						\
		res = _netlib_set_dripv4addr(intf, &gateway);	\
		if (res == -1) {				\
			nvdbg("set route addr error\n");	\
		}						\
	} while (0)

/****************************************************************************
 * Private Types
 ****************************************************************************/
/****************************************************************************
 * Private Data
 ****************************************************************************/
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void _setipv4addr(FAR in_addr_t *outaddr, FAR const struct sockaddr *inaddr)
{
	FAR const struct sockaddr_in *src = (FAR const struct sockaddr_in *)inaddr;
	*outaddr = src->sin_addr.s_addr;
}

static int _netlib_set_ipv4addr(FAR const char *ifname, FAR const struct in_addr *addr)
{
	struct netif *dev = NULL;
	dev = netdev_findbyname(ifname);
	if (!dev) {
		ndbg("Failed to netdev_findbyname\n");
		return ERROR;
	}
	netifapi_netif_set_down(dev);

	struct sockaddr_in inaddr;
	inaddr.sin_family = AF_INET;
	inaddr.sin_port = 0;
	memcpy(&inaddr.sin_addr, addr, sizeof(struct in_addr));

	ip_addr_t ipaddr, netmask, gw;
	_setipv4addr(&ip4_addr_get_u32(ip_2_ip4(&ipaddr)), (struct sockaddr *)&inaddr);
	netmask = dev->netmask;
	gw = dev->gw;
	netifapi_netif_set_addr(dev, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));

	netifapi_netif_set_up(dev);

	return OK;
}

static int _netlib_set_ipv4netmask(FAR const char *ifname, FAR const struct in_addr *addr)
{
	struct netif *dev = netdev_findbyname(ifname);
	if (!dev) {
		ndbg("Failed to netdev_findbyname\n");
		return ERROR;
	}
	netifapi_netif_set_down(dev);

	ip_addr_t ipaddr, netmask, gw;
	struct sockaddr_in inaddr;

	inaddr.sin_family = AF_INET;
	inaddr.sin_port = 0;
	memcpy(&inaddr.sin_addr, addr, sizeof(struct in_addr));

	_setipv4addr(&ip4_addr_get_u32(ip_2_ip4(&netmask)), (struct sockaddr *)&inaddr);
	ipaddr = dev->ip_addr;
	gw = dev->gw;
	netifapi_netif_set_addr(dev, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));

	netifapi_netif_set_up(dev);

	return OK;
}

static int _netlib_set_dripv4addr(FAR const char *ifname, FAR const struct in_addr *addr)
{
	struct netif *dev = netdev_findbyname(ifname);
	if (!dev) {
		ndbg("Failed to netdev_findbyname\n");
		return ERROR;
	}
	netifapi_netif_set_down(dev);

	ip_addr_t ipaddr, netmask, gw;
	struct sockaddr_in inaddr;

	inaddr.sin_family = AF_INET;
	inaddr.sin_port = 0;
	memcpy(&inaddr.sin_addr, addr, sizeof(struct in_addr));

	_setipv4addr(&ip4_addr_get_u32(ip_2_ip4(&gw)), (struct sockaddr *)&inaddr);
	ipaddr = dev->ip_addr;
	netmask = dev->netmask;
	netifapi_netif_set_addr(dev, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));

	netifapi_netif_set_up(dev);
	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: dhcp_client_start
 ****************************************************************************/
int netdev_dhcp_client_start(const char *intf)
{
	nvdbg("LWIP DHCPC started\n");
	struct netif *cur_netif;
	cur_netif = netif_find(intf);
	if (cur_netif == NULL) {
		ndbg("No network interface for dhcpc\n");
		return ERROR;
	}
	int32_t timeleft = CONFIG_LWIP_DHCPC_TIMEOUT;
	struct in_addr local_ipaddr;
	struct in_addr local_netmask;
	struct in_addr local_gateway;

	/* Initialize dhcp structure if exists */
	if (netif_dhcp_data(cur_netif)) {
		netifapi_dhcp_stop(cur_netif);
	}

	local_ipaddr.s_addr = IPADDR_ANY;
	local_netmask.s_addr = IPADDR_BROADCAST;
	local_gateway.s_addr = IPADDR_ANY;
	DHCPC_SET_IP4ADDR(intf, local_ipaddr, local_netmask, local_gateway);
	err_t res = netifapi_dhcp_start(cur_netif);
	if (res) {
		ndbg("dhcpc_start failure %d\n", res);
		return ERROR;
	}
	nvdbg("dhcpc_start success, waiting IP address (timeout %d secs)\n", timeleft);
	while (netifapi_dhcp_address_valid(cur_netif) != 0) {
		usleep(100000);
		timeleft -= 100;
		if (timeleft <= 0) {
			ndbg("dhcpc_start timeout\n");
			netifapi_dhcp_stop(cur_netif);
			return ERROR;
		}
	}

	return OK;
}

/****************************************************************************
 * Name: dhcp_client_stop
 ****************************************************************************/
void netdev_dhcp_client_stop(const char *intf)
{
	struct in_addr in = { .s_addr = INADDR_NONE };
	DHCPC_SET_IP4ADDR(intf, in, in, in);
	struct netif *cur_netif;
	cur_netif = netif_find(intf);
	if (cur_netif == NULL) {
		ndbg("No network interface for dhcpc\n");
		return;
	}
	netifapi_dhcp_stop(cur_netif);
	nvdbg("dhcpc_stop -release IP address (lwip)\n");
}
