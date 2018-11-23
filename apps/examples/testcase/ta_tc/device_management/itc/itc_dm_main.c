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

/// @file itc_dm_main.c

/// @brief Main Function for Device Management TestCase Example
#include <tinyara/config.h>
#include <stdio.h>
#include <semaphore.h>

#include <dm/dm_error.h>
#include <dm/dm_connectivity.h>
#include <protocols/dhcpc.h>
#include <slsi_wifi/slsi_wifi_api.h>

#include "tc_common.h"
#include "itc_internal.h"

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

int wifiAutoConnectInit_itc(void)
{
	int8_t ret;
	uint8_t result;
	uint8_t *ssid = NULL;
	char *ap_security = NULL;
	char *ap_password = NULL;

	slsi_security_config_t *sec_config = NULL;

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
		ssid = (uint8_t *)CONFIG_DM_AP_SSID;
		ap_security = (char *) CONFIG_DM_AP_SECURITY;
		ap_password = (char *) CONFIG_DM_AP_PASS;
		if ((ssid == NULL) || (ap_security == NULL) || (ap_password == NULL)) {
			return -1;
		}

		sec_config = (slsi_security_config_t *)getSecurityConfig(ap_security, ap_password);
		ret = WiFiNetworkJoin(ssid, strlen(CONFIG_DM_AP_SSID), NULL, sec_config);
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

void wifiAutoConnectDeInit_itc(void)
{
	isConnected = 0;
	dm_conn_unregister_linkup_cb(linkUpEvent);
}

static int wifiAutoConnect(void)
{
	int ret;
	ret = -1;
	if ((ret = wifiAutoConnectInit_itc()) == 1) {
		int waitTime = 10;
		while (waitTime--) {
			if (isConnected == 1) {
				printf("[AutoConnect]WiFi Connected!\n");
				int dhcp_ret = app_dhcp_main();
				if (dhcp_ret == -1) {
					printf("Timeout fail to get ip address\n");
					return dhcp_ret;
				} else if (dhcp_ret == 0) {
					printf("No network interface\n");
					return dhcp_ret;
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
	}
	return ret;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_dm_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "DeviceManagement ITC") == ERROR) {
		return ERROR;
	}

#ifndef CONFIG_EXAMPLES_TESTCASE_DM_WIFI
	printf("=== Please Setup WiFi Info ===\n");
	return 0;
#endif
	if (wifiAutoConnect() == 1) {
		itc_dm_lwm2m_testcase_main();
#ifdef CONFIG_ITC_DM_CONN_GET_RSSI
		itc_dm_conn_get_rssi_main();
#endif
#ifdef CONFIG_ITC_DM_CONN_GET_ADDRESS
		itc_dm_conn_get_address_main();
#endif
#ifdef CONFIG_ITC_DM_CONN_GET_INTERFACE
		itc_dm_conn_get_interface_main();
#endif
#ifdef CONFIG_ITC_DM_CONN_GET_CHANNEL
		itc_dm_conn_get_channel_main();
#endif
#ifdef CONFIG_ITC_DM_CONN_GET_TX_POWER
#ifdef CONFIG_ITC_DM_CONN_SET_TX_POWER
		itc_dm_conn_set_get_tx_power_main();
#endif
#endif

#ifdef CONFIG_ITC_DM_CONN_REGI_LINKUP
#ifdef CONFIG_ITC_DM_CONN_UNREGI_LINKUP
		itc_dm_conn_regi_unreg_linkup_main();
#endif
#endif
#ifdef CONFIG_ITC_DM_CONN_REGI_LINKDOWN
#ifdef CONFIG_ITC_DM_CONN_UNREGI_LINKDOWN
		itc_dm_conn_regi_unreg_linkdown_main();
#endif
#endif
		wifiAutoConnectDeInit_itc();
	}

	(void)tc_handler(TC_END, "DeviceManagement ITC");

	return 0;
}
