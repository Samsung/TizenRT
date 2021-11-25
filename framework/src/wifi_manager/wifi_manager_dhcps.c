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
#include <tinyara/net/if/wifi.h>
#include <tinyara/net/netlog.h>
#include "wifi_manager_dhcp.h"
#include "wifi_manager_event.h"
#include "wifi_manager_msghandler.h"
#include "wifi_manager_error.h"
#include "wifi_manager_utils.h"
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
			NET_LOGE(TAG, "set ipv4 addr error\n");                \
		}                                                       \
		res = netlib_set_ipv4netmask(intf, &netmask);           \
		if (res == -1) {                                        \
			NET_LOGE(TAG, "set netmask addr error\n");             \
		}                                                       \
		res = netlib_set_dripv4addr(intf, &gateway);            \
		if (res == -1) {                                        \
			NET_LOGE(TAG, "set route addr error\n");               \
		}                                                       \
	} while (0)
#define TAG "[WM]"

#ifndef CONFIG_WIFIMGR_DISABLE_DHCPS
static dhcp_node_s g_dhcp_list = {WIFIMGR_IP4_ZERO, WIFIMGR_MAC_ZERO};
#endif
static uint8_t num_sta = 0;

#ifndef CONFIG_WIFIMGR_DISABLE_DHCPS
void _wifi_dhcps_event(dhcp_evt_type_e type, void *data)
{
	if (type == DHCP_ACK_EVT) {
		wifimgr_msg_s msg = {WIFIMGR_EVT_DHCPS_ASSIGN_IP, WIFI_MANAGER_FAIL, data, NULL};
		WIFIMGR_CHECK_RESULT_NORET(wifimgr_post_message(&msg), (TAG, "handle dhcpd event fail\n"));
	}
	return;
}
#endif

/**
 * Internal DHCP server APIs
 */
void dhcps_inc_num(void)
{
	if (num_sta == UINT8_MAX) {
		NET_LOGE(TAG, "num_sta overflowed\n");
		return;
	}
	NET_LOGV(TAG, "increase num_sta by 1\n");
	num_sta++;
}

void dhcps_dec_num(void)
{
	if (num_sta == 0) {
		NET_LOGE(TAG, "num_sta underflowed\n");
		return;
	}
	NET_LOGV(TAG, "decrease num_sta by 1\n");
	num_sta--;
}

void dhcps_reset_num(void)
{
	NET_LOGV(TAG, "reset num_sta to 0\n");
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
		NET_LOGV(TAG, "IP: %d:%d:%d:%d\n",
			  ((char *)&(node->ipaddr))[0],
			  ((char *)&(node->ipaddr))[1],
			  ((char *)&(node->ipaddr))[2],
			  ((char *)&(node->ipaddr))[3]);
		NET_LOGV(TAG, "MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
			  node->macaddr[0], node->macaddr[1],
			  node->macaddr[2], node->macaddr[3],
			  node->macaddr[4], node->macaddr[5]);
	} else {
		NET_LOGE(TAG, "[DHCP] No DHCP node exists\n");
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

wifi_manager_result_e wm_dhcps_start(void)
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

	if (dhcp_server_start(WIFIMGR_SOFTAP_IFNAME, _wifi_dhcps_event) != 0) {
		NET_LOGE(TAG, "[DHCP] DHCP Server - started fail\n");
		return WIFI_MANAGER_FAIL;
	}
	NET_LOGV(TAG, "DHCP Server - started successfully\n");

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wm_dhcps_stop(void)
{
	struct in_addr in = { .s_addr = INADDR_NONE };
	WIFIMGR_SET_IP4ADDR(WIFIMGR_SOFTAP_IFNAME, in, in, in);

	if (dhcp_server_stop(WIFIMGR_SOFTAP_IFNAME) != 0) {
		NET_LOGE(TAG, "[DHCP] DHCP Server - stopped failed\n");
		return WIFI_MANAGER_FAIL;
	}
	NET_LOGV(TAG, "DHCP Server - stopped successfully\n");
	return WIFI_MANAGER_SUCCESS;
}

#endif //CONFIG_WIFIMGR_DISABLE_DHCPS
