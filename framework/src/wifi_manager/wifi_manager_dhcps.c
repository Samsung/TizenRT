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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <netutils/netlib.h>
#include "wifi_manager_error.h"
#include "wifi_manager_dhcp.h"
#include "wifi_manager_log.h"

/**
 * Enum, definitions and variables
 */
#define WIFIMGR_SOFTAP_IFNAME    CONFIG_WIFIMGR_SOFTAP_IFNAME
#define WIFIMGR_STA_IFNAME       CONFIG_WIFIMGR_STA_IFNAME
#define WIFIMGR_SET_IP4ADDR(intf, ip, netmask, gateway)         \
	do {                                                        \
		int res = -1;                                           \
		res = netlib_set_ipv4addr(intf, &ip);                   \
		if (res == -1) {                                        \
			WM_LOG_ERROR("[WM] set ipv4 addr error\n");                \
		}                                                       \
		res = netlib_set_ipv4netmask(intf, &netmask);           \
		if (res == -1) {                                        \
			WM_LOG_ERROR("[WM] set netmask addr error\n");             \
		}                                                       \
		res = netlib_set_dripv4addr(intf, &gateway);            \
		if (res == -1) {                                        \
			WM_LOG_ERROR("[WM] set route addr error\n");               \
		}                                                       \
	} while (0)

#ifndef CONFIG_WIFIMGR_DISABLE_DHCPS
static dhcp_node_s g_dhcp_list = {WIFIMGR_IP4_ZERO, WIFIMGR_MAC_ZERO};
#endif

static uint8_t num_sta = 0;

/**
 * Internal DHCP server APIs
 */
void dhcps_inc_num(void)
{
	if (num_sta == UINT8_MAX) {
		WM_LOG_ERROR("num_sta overflowed\n");
		return;
	}
	WM_LOG_VERBOSE("increase num_sta by 1\n");
	num_sta++;
}

void dhcps_dec_num(void)
{
	if (num_sta == 0) {
		WM_LOG_ERROR("num_sta underflowed\n");
		return;
	}
	WM_LOG_VERBOSE("decrease num_sta by 1\n");
	num_sta--;
}

void dhcps_reset_num(void)
{
	WM_LOG_VERBOSE("reset num_sta to 0\n");
	num_sta = 0;
}

uint8_t dhcps_get_num(void)
{
	return num_sta;
}


#ifndef CONFIG_WIFIMGR_DISABLE_DHCPS
static void _dhcps_remove_list(void)
{
	//TODO: Currently only one client can be joined and left
	g_dhcp_list.ipaddr = 0;
	for (int i = 0; i < 6; i++) {
		g_dhcp_list.macaddr[i] = 0;
	}
}


dhcp_status_e dhcps_add_node(void *msg)
{
	dhcp_node_s *node = (dhcp_node_s *)msg;
	if (node != NULL) {
		WM_LOG_VERBOSE("[DHCP] IP: %d:%d:%d:%d\n",
			  ((char *)&(node->ipaddr))[0],
			  ((char *)&(node->ipaddr))[1],
			  ((char *)&(node->ipaddr))[2],
			  ((char *)&(node->ipaddr))[3]);
		WM_LOG_VERBOSE("[DHCP] MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
			  node->macaddr[0], node->macaddr[1],
			  node->macaddr[2], node->macaddr[3],
			  node->macaddr[4], node->macaddr[5]);
	} else {
		WM_LOG_ERROR("[DHCP] No DHCP node exists\n");
		return DHCP_ERR;
	}

	if (memcmp(g_dhcp_list.macaddr, node->macaddr, 6) == 0) {
		g_dhcp_list.ipaddr = node->ipaddr;
		return DHCP_EXIST;
	}
	g_dhcp_list.ipaddr = node->ipaddr;
	memcpy(g_dhcp_list.macaddr, node->macaddr, 6);
	return DHCP_OK;
}


void dhcps_del_node(void)
{
	//TODO: Currently only one client can be joined and left
	g_dhcp_list.ipaddr = 0;
	for (int i = 0; i < 6; i++) {
		g_dhcp_list.macaddr[i] = 0;
	}
}

wifi_manager_result_e wm_dhcps_start(dhcp_sta_joined_cb cb)
{
#ifdef CONFIG_NET_DHCP
	struct in_addr ip = {.s_addr = CONFIG_NETUTILS_DHCPD_ROUTERIP};
	struct in_addr gw = {.s_addr = CONFIG_NETUTILS_DHCPD_ROUTERIP};
	struct in_addr netmask = {.s_addr = CONFIG_NETUTILS_DHCPD_NETMASK};
#else
	struct in_addr ip = {.s_addr = 0x012fa8c0};
	struct in_addr gw = {.s_addr = 0x012fa8c0};
	struct in_addr netmask = {.s_addr = 0x00ffffff};
#endif

	_dhcps_remove_list();
	WIFIMGR_SET_IP4ADDR(WIFIMGR_SOFTAP_IFNAME, ip, netmask, gw);

	if (dhcp_server_start(WIFIMGR_SOFTAP_IFNAME, cb) != OK) {
		WM_LOG_ERROR("[DHCP] DHCP Server - started fail\n");
		return WIFI_MANAGER_FAIL;
	}
	WM_LOG_VERBOSE("[DHCP] DHCP Server - started successfully\n");

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e wm_dhcps_stop(void)
{
	struct in_addr in = { .s_addr = INADDR_NONE };
	WIFIMGR_SET_IP4ADDR(WIFIMGR_SOFTAP_IFNAME, in, in, in);

	if (dhcp_server_stop(WIFIMGR_SOFTAP_IFNAME) != OK) {
		WM_LOG_ERROR("[DHCP] DHCP Server - stopped failed\n");
		return WIFI_MANAGER_FAIL;
	}
	WM_LOG_VERBOSE("[DHCP] DHCP Server - stopped successfully\n");
	return WIFI_MANAGER_SUCCESS;
}

#endif //CONFIG_WIFIMGR_DISABLE_DHCPS
