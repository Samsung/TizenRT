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

#include "../../../../../external/WICED/bcm4390x/include/wwd_structures.h"

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

#define SOFT_AP_IPADDR  "192.168.0.1\0"
#define SOFT_AP_NETMASK "255.255.255.0\0"
#define SOFT_AP_GW      "192.168.0.1\0"

#ifdef CONFIG_EXAMPLES_WICEDWIFI
extern int wicedwifi_main(int argc, char *argv[]);
#endif

#ifdef CONFIG_EXAMPLES_WICEDWIFI
static wiced_security_t str_to_authtype(const char *arg)
{
	if (strcmp(arg, "open") == 0) {
		return WICED_SECURITY_OPEN;
	} else if (strcmp(arg, "wep") == 0) {
		return WICED_SECURITY_WEP_PSK;
	} else if (strcmp(arg, "wep_shared") == 0) {
		return WICED_SECURITY_WEP_SHARED;
	} else if (strcmp(arg, "wpa2_tkip") == 0) {
		return WICED_SECURITY_WPA2_TKIP_PSK;
	} else if (strcmp(arg, "wpa2_aes") == 0) {
		return WICED_SECURITY_WPA2_AES_PSK;
	} else if (strcmp(arg, "wpa2_mixed") == 0) {
		return WICED_SECURITY_WPA2_MIXED_PSK;
	} else if (strcmp(arg, "wpa_aes") == 0) {
		return WICED_SECURITY_WPA_AES_PSK;
	} else if (strcmp(arg, "wpa_tkip") == 0) {
		return WICED_SECURITY_WPA_TKIP_PSK;
	} else if (strcmp(arg, "wpa_mixed") == 0) {
		return WICED_SECURITY_WPA_MIXED_PSK;
	} else {
		printf("Bad auth type: '%s'\r\n", arg);
		return WICED_SECURITY_UNKNOWN;
	}
}
#endif

int net_dhcps_main(void)
{
	struct netif *netif;
	ip_addr_t ipaddr, netmask, gateway;
#if CONFIG_EXAMPLES_WICEDWIFI
	wiced_ssid_t ssid;
	wiced_security_t security;
	wiced_mac_t mac;
#endif
	uint8_t key_length;
	int channel;
	int result;

	netif = netif_find(NET_DEVNAME);
#if CONFIG_EXAMPLES_WICEDWIFI
	ssid.length = strlen("DHCPS");
	strcpy((char *)ssid.value, "DHCPS");
	security = str_to_authtype("wpa2_aes");
	key_length = strlen("qwer1234");

	result = wwd_wifi_start_ap(&ssid, security, (const uint8_t *)"qwer1234", key_length, channel);
	result = wiced_ip_up(WWD_AP_INTERFACE, 2, NULL);
#endif
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
#if CONFIG_EXAMPLES_WICEDWIFI
		result = wwd_wifi_get_mac_address(&mac, WWD_AP_INTERFACE);
		if (result != WWD_SUCCESS) {
			printf("failed to get mac address from driver, ret=%d\n", result);
		}

		printf("mac address %x:%x:%x:%x:%x:%x\n", netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2], netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);
		printf("IP address set %u.%u.%u.%u\n", (unsigned char)((htonl(netif->ip_addr.addr) >> 24) & 0xff), (unsigned char)((htonl(netif->ip_addr.addr) >> 16) & 0xff), (unsigned char)((htonl(netif->ip_addr.addr) >> 8) & 0xff), (unsigned char)((htonl(netif->ip_addr.addr) >> 0) & 0xff));
#endif
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
