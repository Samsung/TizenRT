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

/// @file tc_net_dhcpc.c
/// @brief Test Case Example for dhcpc() API

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

#ifdef CONFIG_EXAMPLES_WICEDWIFI
#include "../../../../../external/WICED/bcm4390x/include/wwd_structures.h"
#endif

#ifdef CONFIG_NET_LWIP_DHCPC
#include <net/lwip/dhcp.h>

#if defined(CONFIG_NET_ETHERNET) && CONFIG_NET_ETHERNET == 1
#if LWIP_HAVE_LOOPIF
#define NET_DEVNAME "en1"
#else
#define NET_DEVNAME "en0"
#endif
#elif defined(CONFIG_NET_802154) && CONFIG_NET_802154 == 1
#define NET_DEVNAME "wpan0"
#else
#error "undefined CONFIG_NET_<type>, check your .config"
#endif

struct dhcp dhcp_handle;
#endif

int net_dhcpc_main(void)
{
#if CONFIG_NET_LWIP_DHCPC
	err_t ret;
	struct netif *netif;
	uint32_t timeleft = 150000;

	netif = netif_find(NET_DEVNAME);
	if (netif == NULL) {
		printf("tc_net_dhcp_netif_find: FAIL\n");
		total_fail++;
	} else {
		printf("tc_net_dhcp_netif_find: PASS\n");
		total_pass++;
	}

	netlib_ifup(NET_DEVNAME);

#if CONFIG_EXAMPLES_WICEDWIFI
	printf("Please join an AP before starting DHCP: wicedwifi\n");
	printf("command: - wicedwifi join <ssid> <security> <key> <ip address> <netmask> <gateway>\n");
#endif
	if ((ret = dhcp_start(netif)) != ERR_OK) {
		printf("tc_net_dhcp_start: FAIL\n");
		total_fail++;
	}

	while (netif->dhcp->state != DHCP_BOUND) {
		sleep(1);
		timeleft -= 1000;

		if (timeleft <= 0)
			break;
	}

	if (timeleft <= 0) {
		dhcp_stop(netif);
		netif->dhcp = NULL;
	}

	printf("tc_net_dhcp_start: PASS\n");
	total_pass++;

	printf("IP address get %u.%u.%u.%u ----\n", (unsigned char)((htonl(netif->ip_addr.addr) >> 24) & 0xff), (unsigned char)((htonl(netif->ip_addr.addr) >> 16) & 0xff), (unsigned char)((htonl(netif->ip_addr.addr) >> 8) & 0xff), (unsigned char)((htonl(netif->ip_addr.addr) >> 0) & 0xff));

	ret = dhcp_release(netif);
	if (ret == ERR_OK) {
		printf("tc_net_dhcp_release: PASS\n");
		total_pass++;
	} else {
		printf("tc_net_dhcp_release: FAIL\n");
		total_fail++;
	}

	dhcp_stop(netif);
	if (netif->dhcp->state == DHCP_OFF) {
		printf("tc_net_dhcp_stop: PASS\n");
		total_pass++;
	} else {
		printf("tc_net_dhcp_stop: FAIL\n");
		total_fail++;
	}

	dhcp_cleanup(netif);
	if (netif->dhcp == NULL) {
		printf("tc_net_dhcp_cleanup: PASS\n");
		total_pass++;
	} else {
		printf("tc_net_dhcp_cleanup: FAIL\n");
		total_fail++;
	}

	dhcp_set_struct(netif, &dhcp_handle);
	if (netif->dhcp) {
		printf("tc_net_dhcp_set_struct: PASS\n");
		total_pass++;
	} else {
		printf("tc_net_dhcp_set_struct: FAIL\n");
		total_fail++;
	}

#endif
	return 0;
}
