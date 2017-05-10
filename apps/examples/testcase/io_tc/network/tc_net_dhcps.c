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

/// @file tc_net_dhcps.c
/// @brief Test Case Example for dhcps() API

#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>

#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//#include <arch/board/board.h>
#include <apps/netutils/netlib.h>

#include <sys/socket.h>

#include "tc_internal.h"

#include <net/lwip/dhcp.h>

#if defined(CONFIG_ARCH_BOARD_SIDK_S5JT200)
#if LWIP_HAVE_LOOPIF
#define NET_DEVNAME "wl1"
#else
#define NET_DEVNAME "wl0"
#endif
#else
#error "undefined CONFIG_NET_<type>, check your .config"
#endif

#define SOFT_AP_IPADDR  "192.168.0.1\0"
#define SOFT_AP_NETMASK "255.255.255.0\0"
#define SOFT_AP_GW      "192.168.0.1\0"

int net_dhcps_main(void)
{
	struct netif *netif;
	ip_addr_t ipaddr, netmask, gateway;
	uint8_t key_length;
	int channel;
	int result;

	netif = netif_find(NET_DEVNAME);
	if (netif != NULL) {
		ipaddr.addr = inet_addr(SOFT_AP_IPADDR);
		netmask.addr = inet_addr(SOFT_AP_NETMASK);
		gateway.addr = inet_addr(SOFT_AP_GW);
		netif_set_addr(netif, &ipaddr, &netmask, &gateway);
		if (netif->ip_addr.addr == inet_addr(SOFT_AP_IPADDR)) {
			printf("tc_net_netif_set_addr: PASS\n");
			total_pass++;
		} else {
			printf("tc_net_netif_set_addr: FAIL\n");
			total_fail++;
		}
		netif_set_up(netif);
		if (netif_is_up(netif)) {
			printf("tc_net_dhcps_set_up: PASS\n");
			total_pass++;
		} else {
			printf("tc_net_dhcps_set_up: FAIL\n");
			total_fail++;
		}
	} else {
		printf("cannot find netif name %s\n", NET_DEVNAME);
	}

	if (dhcps_start(netif) == 0) {
		printf("tc_net_dhcps_start: PASS\n");
		total_pass++;
	} else {
		printf("tc_net_dhcps_start: FAIL\n");
		total_fail++;
	}

	netif_set_down(netif);
	if (netif_is_up(netif)) {
		printf("tc_net_netif_set_down: FAIL\n");
		total_fail++;

	} else {
		printf("tc_net_netif_set_down: PASS\n");
		total_pass++;
	}

	dhcps_stop(netif);
	if (netif->dhcps_pcb != NULL) {
		printf("tc_net_dhcps_stop: FAIL\n");
		total_fail++;
	} else {
		printf("tc_net_dhcps_stop: PASS\n");
		total_pass++;
	}

	return 0;
}
