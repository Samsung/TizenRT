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

/// @file utc_dm_main.c

/// @brief Main Function for Device Management TestCase Example
#include <tinyara/config.h>
#include <stdio.h>
#include <semaphore.h>

#include <dm/dm_error.h>
#include <dm/dm_connectivity.h>
#include <protocols/dhcpc.h>
#include <slsi_wifi/slsi_wifi_api.h>

#include "tc_common.h"
#include "utc_internal.h"

#ifdef CONFIG_WIFI_MANAGER
#define NET_DEVNAME	CONFIG_WIFIMGR_STA_IFNAME
#else
#define NET_DEVNAME "wl1"
#endif

static int isConnected = 0;

static int app_dhcp_main(void)
{
	int ret;
	struct in_addr ip_check;

	ret = dhcp_client_start(NET_DEVNAME);
	if (ret != OK) {
		return -1;
	}

	ret = netlib_get_ipv4addr(NET_DEVNAME, &ip_check)
	if (ret != OK) {
		return -1;
	}
	printf("IP address get %s ----\n", inet_ntoa(ip_check));
	return 1;
}

static void linkUpEvent(void)
{
	isConnected = 1;
}

int wifiAutoConnectInit(void)
{
	int8_t ret;
	uint8_t result;

	isConnected = 0;
	dm_conn_register_linkup_cb(linkUpEvent);

	if (WiFiIsConnected(&result, NULL) != SLSI_STATUS_SUCCESS) {
		printf("Wifi Is not Connected\n");
	}

	if (result > 0) {
		printf("Wi-Fi status - Connected : %d\n", result);
		isConnected = 1;
		return 1;
	}

	ret = WiFiStart(SLSI_WIFI_STATION_IF, NULL);
	if (ret == SLSI_STATUS_SUCCESS) {
		printf("[AutoConnect]STA mode started\n");
		ret = WiFiNetworkJoin((uint8_t *)CONFIG_DM_AP_SSID, strlen(CONFIG_DM_AP_SSID), NULL, (slsi_security_config_t *)getSecurityConfig(CONFIG_DM_AP_SECURITY, CONFIG_DM_AP_PASS));
		sleep(1);
		if (ret == SLSI_STATUS_SUCCESS) {
			printf("[AutoConnect]Start doJoin with SSID %s\n", CONFIG_DM_AP_SSID);
			return 1;
		} else {
			printf("[AutoConnect]Failed to join the network.[%d]\n", ret);
		}
		return -1;
	} else {
		printf("[AutoConnect]Failed to start STA mode\n");
	}
	return -1;
}

static void wifiAutoConnectDeInit(void)
{
	isConnected = 0;
	dm_conn_unregister_linkup_cb(linkUpEvent);
}

static int wifiAutoConnect(void)
{
	int ret;
	ret = -1;
	if (wifiAutoConnectInit() == 1) {
		int waitTime = 10;
		while (waitTime--) {
			if (isConnected == 1) {
				printf("[AutoConnect]WiFi Connected!\n");
				ret = app_dhcp_main();
				if (ret == -1) {
					printf("Timeout fail to get ip address\n");
					return ret;
				} else if (ret == 0) {
					printf("No network interface\n");
					return ret;
				}
				printf("DHCP done\n");
				break;
			}
			sleep(1);
		}
		if (waitTime <= 0) {
			printf("[AutoConnect]WiFi is not working. Test Canceled\n");
			return 0;
		}
		return 1;
	}
	return ret;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_dm_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "DeviceManagement UTC") == ERROR) {
		return ERROR;
	}

#ifndef CONFIG_DM_WIFI
	printf("=== Please Setup WiFi Info ===\n");
	return 0;
#endif
	if (wifiAutoConnect() == 1) {
		dm_lwm2m_testcase_main();
#ifdef CONFIG_UTC_DM_CONN_GET_RSSI
		dm_conn_get_rssi_main();
#endif
#ifdef CONFIG_UTC_DM_CONN_GET_ADDRESS
		dm_conn_get_address_main();
#endif
#ifdef CONFIG_UTC_DM_CONN_GET_INTERFACE
		dm_conn_get_interface_main();
#endif
#ifdef CONFIG_UTC_DM_CONN_GET_CHANNEL
		dm_conn_get_channel_main();
#endif
#ifdef CONFIG_UTC_DM_CONN_GET_TX_POWER
		dm_conn_get_tx_power_main();
#endif
#ifdef CONFIG_UTC_DM_CONN_SET_TX_POWER
		dm_conn_set_tx_power_main();
#endif
#ifdef CONFIG_UTC_DM_CONN_REGI_LINKUP
		dm_conn_regi_linkup_main();
#endif
#ifdef CONFIG_UTC_DM_CONN_REGI_LINKDOWN
		dm_conn_regi_linkdown_main();
#endif
#ifdef CONFIG_UTC_DM_CONN_UNREGI_LINKUP
		dm_conn_unregi_linkup_main();
#endif
#ifdef CONFIG_UTC_DM_CONN_UNREGI_LINKDOWN
		dm_conn_unregi_linkdown_main();
#endif
		wifiAutoConnectDeInit();
	}

	(void)tc_handler(TC_END, "DeviceManagement UTC");

	return 0;
}
