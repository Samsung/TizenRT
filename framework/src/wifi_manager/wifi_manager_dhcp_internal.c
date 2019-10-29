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
#include "wifi_manager_dhcp_internal.h"

#ifdef CONFIG_WIFIMGR_ERROR_REPORT
#include <error_report/error_report.h>
#define WIFIADD_ERR_RECORD(reason_code)	ERR_DATA_CREATE(ERRMOD_WIFI_MANAGER_DHCP, reason_code)
#else
#define WIFIADD_ERR_RECORD(reason_code)
#endif
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
			ndbg("[WM] set ipv4 addr error\n");                \
		}                                                       \
		res = netlib_set_ipv4netmask(intf, &netmask);           \
		if (res == -1) {                                        \
			ndbg("[WM] set netmask addr error\n");             \
		}                                                       \
		res = netlib_set_dripv4addr(intf, &gateway);            \
		if (res == -1) {                                        \
			ndbg("[WM] set route addr error\n");               \
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
	num_sta++;
}

void dhcps_dec_num(void)
{
	if (num_sta > 0) {
		num_sta--;
	}
}

void dhcps_reset_num(void)
{
	num_sta = 0;
}

uint8_t dhcps_get_num(void)
{
	return num_sta;
}

#ifndef CONFIG_WIFIMGR_DISABLE_DHCPS
wifi_manager_result_e dhcps_start(dhcp_sta_joined cb)
{
	//Default IP addr for SoftAP; 192.168.47.1.
	//Please refer to LWIP_DHCPS_SERVER_IP defined in external/dhcpd/Kconfig
	struct in_addr ip = {.s_addr = 0x012fa8c0 };
	struct in_addr netmask = {.s_addr = 0x00ffffff};
	struct in_addr gw = {.s_addr = 0x012fa8c0};

	_dhcps_remove_list();
	WIFIMGR_SET_IP4ADDR(WIFIMGR_SOFTAP_IFNAME, ip, netmask, gw);

	if (dhcp_server_start(WIFIMGR_SOFTAP_IFNAME, cb) != OK) {
		ndbg("[DHCP] DHCP Server - started fail\n");
		return WIFI_MANAGER_FAIL;
	}
	nvdbg("[DHCP] DHCP Server - started successfully\n");

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e dhcps_stop(void)
{
	struct in_addr in = { .s_addr = INADDR_NONE };
	WIFIMGR_SET_IP4ADDR(WIFIMGR_SOFTAP_IFNAME, in, in, in);

	if (dhcp_server_stop(WIFIMGR_SOFTAP_IFNAME) != OK) {
		ndbg("[DHCP] DHCP Server - stopped failed\n");
	}
	nvdbg("[DHCP] DHCP Server - stopped successfully\n");
	return WIFI_MANAGER_SUCCESS;
}

dhcp_status_e dhcps_add_node(void *msg)
{
	dhcp_node_s *node = (dhcp_node_s *)msg;
	if (node != NULL) {
		nvdbg("[DHCP] IP: %d:%d:%d:%d\n",
			  ((char *)&(node->ipaddr))[0],
			  ((char *)&(node->ipaddr))[1],
			  ((char *)&(node->ipaddr))[2],
			  ((char *)&(node->ipaddr))[3]);
		nvdbg("[DHCP] MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
			  node->macaddr[0], node->macaddr[1],
			  node->macaddr[2], node->macaddr[3],
			  node->macaddr[4], node->macaddr[5]);
	} else {
		ndbg("[DHCP] No DHCP node exists\n");
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

static void _dhcps_remove_list(void)
{
	//TODO: Currently only one client can be joined and left
	g_dhcp_list.ipaddr = 0;
	for (int i = 0; i < 6; i++) {
		g_dhcp_list.macaddr[i] = 0;
	}
}
#endif //CONFIG_WIFIMGR_DISABLE_DHCPS


/**
 * Internal DHCP client APIs
 */
#ifndef CONFIG_WIFIMGR_DISABLE_DHCPC
wifi_manager_result_e dhcpc_get_ipaddr(void)
{
	int ret;
	struct in_addr ip;
	wifi_manager_result_e wret = WIFI_MANAGER_FAIL;

	ret = dhcp_client_start(WIFIMGR_STA_IFNAME);
	if (ret != OK) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_CONNECT_DHCPC_FAIL);
		ndbg("[DHCPC] get IP address fail\n");
		return wret;
	}

	ret = netlib_get_ipv4addr(WIFIMGR_STA_IFNAME, &ip);
	if (ret != OK) {
		ndbg("[DHCPC] get IP address fail\n");
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_CONNECT_DHCPC_FAIL);
		return wret;
	}
	nvdbg("[DHCPC] get IP address %s\n", inet_ntoa(ip));

	return WIFI_MANAGER_SUCCESS;
}
#endif //CONFIG_WIFIMGR_DISABLE_DHCPC

//Set IP to zero, regardless of DHCPC status
void dhcpc_close_ipaddr(void)
{
#ifndef CONFIG_WIFIMGR_DISABLE_DHCPC
	dhcp_client_stop(WIFIMGR_STA_IFNAME);
#else
	struct in_addr in = { .s_addr = INADDR_NONE };
	WIFIMGR_SET_IP4ADDR(WIFIMGR_SOFTAP_IFNAME, in, in, in);
#endif
	nvdbg("[DHCPC] %s - release IP address\n", __FUNCTION__);
	return;
}

wifi_manager_result_e dhcpc_fetch_ipaddr(struct in_addr *ip)
{
	if (netlib_get_ipv4addr(WIFIMGR_STA_IFNAME, ip) != OK) {
		ndbg("[DHCPC] get IP address fail\n");
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_CONNECT_DHCPC_FAIL);
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}

