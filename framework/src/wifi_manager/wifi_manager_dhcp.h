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

#include <tinyara/wifi/wifi_utils.h>
#include <protocols/dhcpc.h>
#include <protocols/dhcpd.h>
#include <arpa/inet.h>
#include <wifi_manager/wifi_manager.h>

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

/**
 * Enum, definitions and variables
 */
typedef enum {
	DHCP_ERR = -1,
	DHCP_OK,
	DHCP_EXIST,
} dhcp_status_e;

#define WIFIMGR_IP4_ZERO 0
#define WIFIMGR_MAC_ZERO {0, 0, 0, 0, 0, 0}

/*  dhcp server */
typedef void (*dhcp_sta_joined_cb)(dhcp_evt_type_e type, void *data);

void dhcps_inc_num(void);
void dhcps_dec_num(void);
void dhcps_reset_num(void);
uint8_t dhcps_get_num(void);

#ifndef CONFIG_WIFIMGR_DISABLE_DHCPS
wifi_manager_result_e wm_dhcps_start(dhcp_sta_joined_cb cb);
wifi_manager_result_e wm_dhcps_stop(void);

/* TODO: Currently, wifi manager stores only a single mac address of the associated node
 * while it is running as a softap mode. This might be modified later,
 * when a chipset needs to support multiple connections simultaneously.
 */
dhcp_status_e dhcps_add_node(void *node);
void dhcps_del_node(void);
#endif

#ifndef CONFIG_WIFIMGR_DISABLE_DHCPC
wifi_manager_result_e dhcpc_get_ipaddr(void);
#endif

void dhcpc_close_ipaddr(void);

wifi_manager_result_e dhcpc_fetch_ipaddr(struct in_addr *ip);

