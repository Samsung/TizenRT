/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/
#include "pthread.h"
#include "rtk_lwip_netconf.h"

#include "wifi_conf.h"

//RTK
#define CONFIG_INIT_NET                 1
/* WLAN CONFIG ---------------------------------------------------------------*/
#define CONFIG_WLAN	1
#define RTK_OK          0		/*!< RTK_err_t value indicating success (no error) */
#define RTK_FAIL        -1		/*!< Generic RTK_err_t code indicating failure */

#define RTK_ERR_NO_MEM          0x101	/*!< Out of memory */
#define RTK_ERR_INVALID_ARG     0x102	/*!< Invalid argument */
#define RTK_ERR_INVALID_STATE   0x103	/*!< Invalid state */
#define RTK_ERR_INVALID_SIZE    0x104	/*!< Invalid size */
#define RTK_ERR_NOT_FOUND       0x105	/*!< Requested resource not found */
#define RTK_ERR_NOT_SUPPORTED   0x106	/*!< Operation or feature not supported */
#define RTK_ERR_TIMEOUT         0x107	/*!< Operation timed out */
#define RTK_ERR_INVALID_RRTKONSE    0x108	/*!< Received rRTKonse was invalid */
#define RTK_ERR_INVALID_CRC     0x109	/*!< CRC or checksum was invalid */
#define RTK_ERR_INVALID_VERSION     0x10A	/*!< Version was invalid */
#define RTK_ERR_INVALID_MAC     0x10B	/*!< MAC address was invalid */

#define RTK_ERR_WIFI_BASE       0x3000	/*!< Starting number of WiFi error codes */
#define RTK_ERR_MESH_BASE       0x4000	/*!< Starting number of MESH error codes */

#define vTaskDelay(t)   usleep(t)

/* Private define ------------------------------------------------------------*/

extern struct netif xnetif[NET_IF_NUM];

wifi_utils_cb_s g_cbk = { NULL, NULL, NULL, NULL, NULL };

//typedef unsigned char    bool;

static WiFi_InterFace_ID_t g_mode = RTK_WIFI_NONE;

/*
 * Callback
 */
static int callback_handler(void *arg)
{
	int *type = (int*)(arg);

	if (*type == 1 && g_cbk.sta_connected) {
		g_cbk.sta_connected(TRWIFI_SUCCESS, NULL);
	} else if (*type == 2 && g_cbk.sta_connected) {
		g_cbk.sta_connected(TRWIFI_FAIL, NULL);
	} else if (*type == 3 && g_cbk.softap_sta_joined) {
		g_cbk.softap_sta_joined(NULL);
	} else if (*type == 4 && g_cbk.sta_disconnected) {
		g_cbk.sta_disconnected(NULL);
	} else if (*type == 5 && g_cbk.softap_sta_left) {
		g_cbk.softap_sta_left(NULL);
	}
	kmm_free(type);
	return 0;
}

static void linkup_handler(rtk_reason_t *reason)
{
	int *type = (int *)kmm_malloc(sizeof(int));
	if (type == NULL) {
		ndbg("[RTK] malloc error\n");
		return;
	}

	if (g_mode == RTK_WIFI_STATION_IF) {
		if (reason->reason_code == RTK_STATUS_SUCCESS) {
			*type = 1;
		} else {
			*type = 2;
		}
	} else if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		*type = 3;
	}
	pthread_t tid;
	int ret = pthread_create(&tid, NULL, (pthread_startroutine_t)callback_handler, (void *)type);
	if (ret != 0) {
		ndbg("[RTK] pthread create fail(%d)\n", errno);
		kmm_free(type);
		return;
	}
	pthread_setname_np(tid, "wifi_utils_cbk_handler");
	pthread_detach(tid);
}

static void linkdown_handler(rtk_reason_t *reason)
{
	int *type = (int *)kmm_malloc(sizeof(int));
	if (type == NULL) {
		ndbg("[RTK] malloc error linkdown\n");
		return;
	}
	*type = 4;
	if (g_mode == RTK_WIFI_STATION_IF) {
		*type = 4;
	} else if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		*type = 5;
	}
	pthread_t tid;
	int ret = pthread_create(&tid, NULL, (pthread_startroutine_t)callback_handler, (void *)type);
	if (ret != 0) {
		ndbg("[RTK] pthread create fail(%d)\n", errno);
		kmm_free(type);
		return;
	}
	pthread_setname_np(tid, "wifi_utils_cbk_handler");
	pthread_detach(tid);
}

//
// Interface API
//
trwifi_result_e wifi_utils_init(void)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	if (g_mode == RTK_WIFI_NONE) {
		int ret = RTK_STATUS_SUCCESS;
		g_cbk = (wifi_utils_cb_s){NULL, NULL, NULL, NULL, NULL};

		ret = WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler);
		if (ret != RTK_STATUS_SUCCESS) {
			ndbg("[RTK] Link callback handles: register failed !\n");
			return wuret;
		} else {
			nvdbg("[RTK] Link callback handles: registered\n");
		}

		ret = cmd_wifi_on(RTK_WIFI_STATION_IF);
		if (ret != RTK_STATUS_SUCCESS) {
			ndbg("[RTK] Failed to start STA mode\n");
			return wuret;
		}
		g_mode = RTK_WIFI_STATION_IF;
		wuret = TRWIFI_SUCCESS;
	} else {
		ndbg("Already %d\n", g_mode);
	}
	return wuret;
	//return TRWIFI_FAIL;
}

trwifi_result_e trwifi_deinit(void)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int ret = cmd_wifi_off();
	if (ret == RTK_STATUS_SUCCESS) {
		g_mode = RTK_WIFI_NONE;
		g_cbk = (trwifi_cb_s){NULL, NULL, NULL, NULL, NULL};
		wuret = TRWIFI_SUCCESS;
	} else {
		ndbg("[RTK] Failed to stop STA mode\n");
	}
	return wuret;
}

trwifi_result_e trwifi_scan_ap(void *arg)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	if(wifi_scan_networks(app_scan_result_handler, NULL ) != RTW_SUCCESS){
		//ndbg("[RTK] [ERR] WiFi scan fail(%d)\n", ret);
		return wuret;
	}
	wuret = TRWIFI_SUCCESS;
	ndbg("[RTK] WIFi Scan success\n");
	return wuret;
}

trwifi_result_e trwifi_register_callback(trwifi_cb_s *cbk)
{
	trwifi_result_e wuret = TRWIFI_INVALID_ARGS;
	if (cbk) {
		g_cbk = *cbk;
		wuret = TRWIFI_SUCCESS;
	} else {
		ndbg("[RTK] WiFi callback register failure (no callback)\n");
	}
	return wuret;
}

trwifi_result_e wifi_utils_connect_ap(trwifi_ap_config_s *ap_connect_config, void *arg)
{
	trwifi_result_e wuret = TRWIFI_INVALID_ARGS;
	if (!ap_connect_config) {
		return wuret;
	}

	int ret;
	wuret = TRWIFI_FAIL;

	if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		if(trwifi_deinit()){
			ndbg("[RTK] Failed to stop AP mode\n");
			return TRWIFI_FAIL;
		}
		vTaskDelay(20);
		if (trwifi_init() < 0){
			ndbg("\n\rERROR: Wifi on failed!");
			return TRWIFI_FAIL;
		}
	}

	ret = cmd_wifi_connect(ap_connect_config,arg);
	if (ret != RTK_STATUS_SUCCESS) {
		ndbg("[RTK] WiFiNetworkJoin failed: %d, %s\n", ret, ap_connect_config->ssid);
		return wuret;
	} else {
		wuret = TRWIFI_SUCCESS;
		nvdbg("[RTK] Successfully joined the network: %s(%d)\n", ap_connect_config->ssid,
			  ap_connect_config->ssid_length);
	}

	return wuret;
}

trwifi_result_e wifi_utils_disconnect_ap(void *arg)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int ret = cmd_wifi_disconnect();
	if (ret == RTK_STATUS_SUCCESS) {
		ndbg("[RTK] WiFiNetworkLeave success\n");
		wuret = TRWIFI_SUCCESS;
	} else {
		ndbg("[RTK] WiFiNetworkLeave fail because of %d\n", ret);
	}

	return wuret;
}

trwifi_result_e wifi_utils_get_info(trwifi_info_s *wifi_info)
{
	trwifi_result_e wuret = TRWIFI_INVALID_ARGS;
	if (wifi_info) {
		wuret = TRWIFI_FAIL;
		if (g_mode != RTK_WIFI_NONE) {
			char mac_str[18] = {0 ,};
			(void)wifi_get_mac_address((char *)mac_str);
			int ret = sscanf(mac_str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%*c", &wifi_info->mac_address[0], &wifi_info->mac_address[1], &wifi_info->mac_address[2], &wifi_info->mac_address[3], &wifi_info->mac_address[4], &wifi_info->mac_address[5]);
			if (ret == WIFIMGR_MACADDR_LEN) {
				wifi_info->rssi = (int)0;
				if (g_mode == RTK_WIFI_SOFT_AP_IF) {
					wifi_info->wifi_status = TRWIFI_SOFTAP_MODE;
				} else if (g_mode == RTK_WIFI_STATION_IF) {
					uint8_t isConnected;
					if (wifi_is_connected_to_ap() == RTK_STATUS_SUCCESS) {
						int8_t rssi;
						wifi_info->wifi_status = TRWIFI_CONNECTED;
						if (wifi_get_rssi(&rssi) == RTK_STATUS_SUCCESS) {
							wifi_info->rssi = (int)rssi;
						}
					} else {
						wifi_info->wifi_status = TRWIFI_DISCONNECTED;
					}
				}
				wuret = TRWIFI_SUCCESS;
			} else {
				ndbg("[RTK] no MAC exists\n");
			}
		} else {
			ndbg("[RTK] need to init... get info fail\n");
		}
	}
	return wuret;
}

trwifi_result_e wifi_utils_start_softap(trwifi_softap_config_s *softap_config)
{
	if (!softap_config) {
		return TRWIFI_INVALID_ARGS;
	}

	if(g_mode == RTK_WIFI_SOFT_AP_IF)
		ndbg("[RTK] softap is already running!\n");

	trwifi_result_e ret = TRWIFI_FAIL;

	ret = WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler);
	if (ret != RTK_STATUS_SUCCESS) {
		ndbg("[RTK] Link callback handles: register failed !\n");
		return TRWIFI_FAIL;
	} else {
		nvdbg("[RTK] Link callback handles: registered\n");
	}

	if (cmd_wifi_ap(softap_config) != RTK_STATUS_SUCCESS) {
		ndbg("[RTK] Failed to start AP mode\n");
		return ret;
	}
	g_mode = RTK_WIFI_SOFT_AP_IF;
	nvdbg("[RTK] SoftAP with SSID: %s has successfully started!\n", softap_config->ssid);

	ret = TRWIFI_SUCCESS;

	return ret;
}

trwifi_result_e wifi_utils_start_sta(void)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int ret = RTK_STATUS_SUCCESS;

	if(g_mode == RTK_WIFI_STATION_IF)
		ndbg("[RTK] station is already running!\n");

	ret = WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler);
	if (ret != RTK_STATUS_SUCCESS) {
		ndbg("[RTK] Link callback handles: register failed !\n");
		return wuret;
	} else {
		nvdbg("[RTK] Link callback handles: registered\n");
	}

	cmd_wifi_off();
	vTaskDelay(20);
	ret = cmd_wifi_on(RTK_WIFI_STATION_IF);
	if (ret == RTK_STATUS_SUCCESS) {
		g_mode = RTK_WIFI_STATION_IF;
		wuret = TRWIFI_SUCCESS;
	} else {
		ndbg("[RTK] Failed to start STA mode\n");
	}
	return wuret;
}

trwifi_result_e wifi_utils_stop_softap(void)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int ret;
	if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		ret = cmd_wifi_off();
		if (ret == RTK_STATUS_SUCCESS) {
			g_mode = RTK_WIFI_NONE;
			wuret = TRWIFI_SUCCESS;
			ndbg("[RTK] Stop AP mode successfully\n");
		} else {
			ndbg("[RTK] Stop AP mode fail\n");
		}
	} else {
		ndbg("[RTK] Mode is not AP mode\n");
	}
	return wuret;
}

trwifi_result_e wifi_utils_set_autoconnect(uint8_t check)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int ret = RTK_FAIL;
	ret = wifi_set_autoreconnect(check);
	if (ret == RTK_STATUS_SUCCESS) {
		wuret = TRWIFI_SUCCESS;
		ndbg("[RTK] External Autoconnect set to %d\n", check);
	} else {
		ndbg("[RTK] External Autoconnect failed to set %d", check);
	}
	return wuret;
}

