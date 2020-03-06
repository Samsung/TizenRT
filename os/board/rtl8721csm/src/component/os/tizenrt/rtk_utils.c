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

#include <tinyara/wifi/wifi_utils.h>
#include "wifi_conf.h"
#include "rtk_wifi_utils.h"
#include <wifi_manager/wifi_manager.h>
#include "../include/debug.h"

/* WLAN CONFIG ---------------------------------------------------------------*/
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

#define vTaskDelay(t)   usleep(t*1000)

/* Private define ------------------------------------------------------------*/

extern struct netif xnetif[NET_IF_NUM];

wifi_utils_cb_s g_cbk = { NULL, NULL, NULL, NULL, NULL };

//typedef unsigned char    bool;

static WiFi_InterFace_ID_t g_mode = RTK_WIFI_NONE;

wifi_utils_scan_list_s *g_scan_list;
int g_scan_num;
static void _free_scanlist(void)
{
	while (g_scan_list) {
		wifi_utils_scan_list_s *cur = g_scan_list;
		g_scan_list = g_scan_list->next;
		rtw_mfree(cur, sizeof(wifi_utils_scan_list_s));
	}
	g_scan_num = 0;
}
rtw_result_t app_scan_result_handler(rtw_scan_handler_result_t *malloced_scan_result)
{
	wifi_utils_scan_list_s *scan_list;
	scan_list = (wifi_utils_scan_list_s *)rtw_zmalloc(sizeof(wifi_utils_scan_list_s));
	if (scan_list == NULL) {
		ndbg("\r\n[app_scan_result_handler]:Fail to malloc scan_list\r\n");
		return RTW_ERROR;
	}

	if (malloced_scan_result->scan_complete != RTW_TRUE) {
		rtw_scan_result_t *record = &malloced_scan_result->ap_details;
		record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */
		scan_list->ap_info.channel = record->channel;
		strncpy(scan_list->ap_info.ssid, record->SSID.val, record->SSID.len);
		scan_list->ap_info.ssid_length = record->SSID.len;
		snprintf(scan_list->ap_info.bssid, WIFI_UTILS_MACADDR_STR_LEN, "%02x:%02x:%02x:%02x:%02x:%02x", record->BSSID.octet[0], record->BSSID.octet[1], record->BSSID.octet[2], record->BSSID.octet[3], record->BSSID.octet[4], record->BSSID.octet[5]);
		scan_list->ap_info.max_rate = 0;
		scan_list->ap_info.rssi = record->signal_strength;
		scan_list->ap_info.phy_mode = 0x00000004; //bit2 is set to 1 if support 11n
		switch (record->security) {
		case RTW_SECURITY_OPEN:
			scan_list->ap_info.ap_auth_type = WIFI_UTILS_AUTH_OPEN;
			break;
		case RTW_SECURITY_WEP_SHARED:
			scan_list->ap_info.ap_auth_type = WIFI_UTILS_AUTH_WEP_SHARED;
			break;
		case RTW_SECURITY_WEP_PSK:
			scan_list->ap_info.ap_auth_type = WIFI_UTILS_AUTH_WPA_PSK;
			break;
		case RTW_SECURITY_WPA2_AES_PSK:
			scan_list->ap_info.ap_auth_type = WIFI_UTILS_AUTH_WPA2_PSK;
			break;
		case RTW_SECURITY_WPA_WPA2_MIXED:
			scan_list->ap_info.ap_auth_type = WIFI_UTILS_AUTH_WPA_AND_WPA2_PSK;
			break;
		default:
			scan_list->ap_info.ap_auth_type = WIFI_UTILS_AUTH_UNKNOWN;
			break;
		}

		switch (record->security) {
		case RTW_SECURITY_OPEN:
			scan_list->ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_NONE;
			break;
		case RTW_SECURITY_WEP_SHARED:
			scan_list->ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_WEP_64;
			break;
		case RTW_SECURITY_WEP_PSK:
			scan_list->ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_WEP_128;
			break;
		case RTW_SECURITY_WPA_TKIP_PSK:
			scan_list->ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_TKIP;
			break;
		case RTW_SECURITY_WPA_AES_PSK:
		case RTW_SECURITY_WPA2_AES_PSK:
			scan_list->ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_AES;
			break;
		case RTW_SECURITY_WPA2_MIXED_PSK:
			scan_list->ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_TKIP_AND_AES;
			break;
		default:
			scan_list->ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_UNKNOWN;
			break;
		}
		if (g_scan_list == NULL) {
			g_scan_list = scan_list;
			g_scan_num = 1;
		} else {
			wifi_utils_scan_list_s *cur = g_scan_list;
			int idx;
			for (idx = 1; idx < g_scan_num; idx++) {
				cur = cur->next;
			}
			cur->next = scan_list;
			scan_list->next = NULL;
			g_scan_num++;
		}
	} else {
		nvdbg("SCAN DONE: Calling wifi_scan_result_callback\r\n");
		g_cbk.scan_done(WIFI_UTILS_SUCCESS, g_scan_list, NULL);
		_free_scanlist();
		if (g_scan_list) {
			ndbg("SCAN list is not initialized\n");
			return RTW_ERROR;
		}
	}
	return RTW_SUCCESS;
}

/*
 * Callback
 */
static int callback_handler(void *arg)
{
	int *type = (int*)(arg);

	if (*type == 1 && g_cbk.sta_connected) {
		g_cbk.sta_connected(WIFI_UTILS_SUCCESS, NULL);
	} else if (*type == 2 && g_cbk.sta_connected) {
		g_cbk.sta_connected(WIFI_UTILS_FAIL, NULL);
	} else if (*type == 3 && g_cbk.softap_sta_joined) {
		g_cbk.softap_sta_joined(NULL);
	} else if (*type == 4 && g_cbk.sta_disconnected) {
		g_cbk.sta_disconnected(NULL);
	} else if (*type == 5 && g_cbk.softap_sta_left) {
		g_cbk.softap_sta_left(NULL);
	}
	free(type);
	return 0;
}

static void linkup_handler(rtk_reason_t *reason)
{
	int *type = (int *)malloc(sizeof(int));
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
		free(type);
		return;
	}
	pthread_setname_np(tid, "wifi_utils_cbk_handler");
	pthread_detach(tid);
}

static void linkdown_handler(rtk_reason_t *reason)
{
	int *type = (int *)malloc(sizeof(int));
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
		free(type);
		return;
	}
	pthread_setname_np(tid, "wifi_utils_cbk_handler");
	pthread_detach(tid);
}

//
// Interface API
//
wifi_utils_result_e wifi_utils_init(void)
{
	wifi_utils_result_e wuret = WIFI_UTILS_FAIL;
	if (g_mode == RTK_WIFI_NONE) {
		int ret = RTK_STATUS_SUCCESS;
		g_cbk = (wifi_utils_cb_s){NULL, NULL, NULL, NULL, NULL};
#if CONFIG_LWIP_LAYER
		/* Initilaize the LwIP stack */
		if (LwIP_Is_Init() < 0) {
			LwIP_Init_If();
		}
#endif
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
		wuret = WIFI_UTILS_SUCCESS;
	} else {
		ndbg("Already %d\n", g_mode);
	}
	return wuret;
	//return WIFI_UTILS_FAIL;
}

wifi_utils_result_e wifi_utils_deinit(void)
{
	wifi_utils_result_e wuret = WIFI_UTILS_FAIL;
	int ret = cmd_wifi_off();
	if (ret == RTK_STATUS_SUCCESS) {
		g_mode = RTK_WIFI_NONE;
		g_cbk = (wifi_utils_cb_s){NULL, NULL, NULL, NULL, NULL};
		wuret = WIFI_UTILS_SUCCESS;
	} else {
		ndbg("[RTK] Failed to stop STA mode\n");
	}
	return wuret;
}

wifi_utils_result_e wifi_utils_scan_ap(void *arg)
{
	wifi_utils_result_e wuret = WIFI_UTILS_FAIL;
	if(wifi_scan_networks(app_scan_result_handler, NULL ) != RTW_SUCCESS){
		//ndbg("[RTK] [ERR] WiFi scan fail(%d)\n", ret);
		return wuret;
	}
	wuret = WIFI_UTILS_SUCCESS;
	ndbg("[RTK] WIFi Scan success\n");
	return wuret;
}

wifi_utils_result_e wifi_utils_register_callback(wifi_utils_cb_s *cbk)
{
	wifi_utils_result_e wuret = WIFI_UTILS_INVALID_ARGS;
	if (cbk) {
		g_cbk = *cbk;
		wuret = WIFI_UTILS_SUCCESS;
	} else {
		ndbg("[RTK] WiFi callback register failure (no callback)\n");
	}
	return wuret;
}

wifi_utils_result_e wifi_utils_connect_ap(wifi_utils_ap_config_s *ap_connect_config, void *arg)
{
	wifi_utils_result_e wuret = WIFI_UTILS_INVALID_ARGS;
	if (!ap_connect_config) {
		return wuret;
	}

	int ret;
	wuret = WIFI_UTILS_FAIL;

	if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		if(wifi_utils_deinit()){
			ndbg("[RTK] Failed to stop AP mode\n");
			return WIFI_UTILS_FAIL;
		}
		vTaskDelay(20);
		if (wifi_utils_init() < 0){
			ndbg("\n\rERROR: Wifi on failed!");
			return WIFI_UTILS_FAIL;
		}
	}

	ret = cmd_wifi_connect(ap_connect_config,arg);
	if (ret != RTK_STATUS_SUCCESS) {
		ndbg("[RTK] WiFiNetworkJoin failed: %d, %s\n", ret, ap_connect_config->ssid);
		return wuret;
	} else {
		wuret = WIFI_UTILS_SUCCESS;
		nvdbg("[RTK] Successfully joined the network: %s(%d)\n", ap_connect_config->ssid,
			  ap_connect_config->ssid_length);
	}

	return wuret;
}

wifi_utils_result_e wifi_utils_disconnect_ap(void *arg)
{
	wifi_utils_result_e wuret = WIFI_UTILS_FAIL;
	int ret = cmd_wifi_disconnect();
	if (ret == RTK_STATUS_SUCCESS) {
		ndbg("[RTK] WiFiNetworkLeave success\n");
		wuret = WIFI_UTILS_SUCCESS;
	} else {
		ndbg("[RTK] WiFiNetworkLeave fail because of %d\n", ret);
	}

	return wuret;
}

wifi_utils_result_e wifi_utils_get_info(wifi_utils_info_s *wifi_info)
{
	wifi_utils_result_e wuret = WIFI_UTILS_INVALID_ARGS;
	if (wifi_info) {
		wuret = WIFI_UTILS_FAIL;
		if (g_mode != RTK_WIFI_NONE) {
			char mac_str[18] = {0 ,};
			(void)wifi_get_mac_address((char *)mac_str);
			int ret = sscanf(mac_str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%*c", &wifi_info->mac_address[0], &wifi_info->mac_address[1], &wifi_info->mac_address[2], &wifi_info->mac_address[3], &wifi_info->mac_address[4], &wifi_info->mac_address[5]);
			if (ret == WIFIMGR_MACADDR_LEN) {
				wifi_info->rssi = (int)0;
				if (g_mode == RTK_WIFI_SOFT_AP_IF) {
					wifi_info->wifi_status = WIFI_UTILS_SOFTAP_MODE;
				} else if (g_mode == RTK_WIFI_STATION_IF) {
					uint8_t isConnected;
					if (wifi_is_connected_to_ap() == RTK_STATUS_SUCCESS) {
						int8_t rssi;
						wifi_info->wifi_status = WIFI_UTILS_CONNECTED;
						if (wifi_get_rssi(&rssi) == RTK_STATUS_SUCCESS) {
							wifi_info->rssi = (int)rssi;
						}
					} else {
						wifi_info->wifi_status = WIFI_UTILS_DISCONNECTED;
					}
				}
				wuret = WIFI_UTILS_SUCCESS;
			} else {
				ndbg("[RTK] no MAC exists\n");
			}
		} else {
			ndbg("[RTK] need to init... get info fail\n");
		}
	}
	return wuret;
}

wifi_utils_result_e wifi_utils_start_softap(wifi_utils_softap_config_s *softap_config)
{
	if (!softap_config) {
		return WIFI_UTILS_INVALID_ARGS;
	}

	if(g_mode == RTK_WIFI_SOFT_AP_IF)
		ndbg("[RTK] softap is already running!\n");

	wifi_utils_result_e ret = WIFI_UTILS_FAIL;

	ret = WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler);
	if (ret != RTK_STATUS_SUCCESS) {
		ndbg("[RTK] Link callback handles: register failed !\n");
		return WIFI_UTILS_FAIL;
	} else {
		nvdbg("[RTK] Link callback handles: registered\n");
	}

	if (cmd_wifi_ap(softap_config) != RTK_STATUS_SUCCESS) {
		ndbg("[RTK] Failed to start AP mode\n");
		return ret;
	}
	g_mode = RTK_WIFI_SOFT_AP_IF;
	nvdbg("[RTK] SoftAP with SSID: %s has successfully started!\n", softap_config->ssid);

	ret = WIFI_UTILS_SUCCESS;

	return ret;
}

wifi_utils_result_e wifi_utils_start_sta(void)
{
	wifi_utils_result_e wuret = WIFI_UTILS_FAIL;
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
		wuret = WIFI_UTILS_SUCCESS;
	} else {
		ndbg("[RTK] Failed to start STA mode\n");
	}
	return wuret;
}

wifi_utils_result_e wifi_utils_stop_softap(void)
{
	wifi_utils_result_e wuret = WIFI_UTILS_FAIL;
	int ret;
	if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		ret = cmd_wifi_off();
		if (ret == RTK_STATUS_SUCCESS) {
			g_mode = RTK_WIFI_NONE;
			wuret = WIFI_UTILS_SUCCESS;
			ndbg("[RTK] Stop AP mode successfully\n");
		} else {
			ndbg("[RTK] Stop AP mode fail\n");
		}
	} else {
		ndbg("[RTK] Mode is not AP mode\n");
	}
	return wuret;
}

wifi_utils_result_e wifi_utils_set_autoconnect(uint8_t check)
{
	wifi_utils_result_e wuret = WIFI_UTILS_FAIL;
	int ret = RTK_FAIL;
	ret = wifi_set_autoreconnect(check);
	if (ret == RTK_STATUS_SUCCESS) {
		wuret = WIFI_UTILS_SUCCESS;
		ndbg("[RTK] External Autoconnect set to %d\n", check);
	} else {
		ndbg("[RTK] External Autoconnect failed to set %d", check);
	}
	return wuret;
}

