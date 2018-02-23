/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <debug.h>
#include <stdio.h>
#include <errno.h>
#include <slsi_wifi/slsi_wifi_api.h>
#include "wifi_utils.h"

#define MACADDR_LENGTH			6
#define SSID_LENGTH_MAX			32
#define PASSPHRASE_LENGTH_MAX	64
#define DHCP_RETRY_COUNT		1
#define WIFI_UTILS_DEBUG        0

static WiFi_InterFace_ID_t g_mode;
static wifi_utils_cb_s g_cbk = {NULL, NULL, NULL, NULL, NULL};

/*
 * Utils
 */

static void
get_security_type(slsi_security_config_t *sec_modes, uint8_t num_sec_modes,
				   wifi_utils_ap_auth_type_e *auth,
				   wifi_utils_ap_crypto_type_e *crypto)
{
	if (!sec_modes) {
		*auth = WIFI_UTILS_AUTH_OPEN;
		*crypto = WIFI_UTILS_CRYPTO_NONE;
	} else {
		if (sec_modes->secmode == SLSI_SEC_MODE_WEP ||
			sec_modes->secmode == SLSI_SEC_MODE_WEP_SHARED ||
			sec_modes->secmode == (SLSI_SEC_MODE_WEP | SLSI_SEC_MODE_WEP_SHARED)) {
			*auth = WIFI_UTILS_AUTH_WEP_SHARED;
			*crypto = WIFI_UTILS_CRYPTO_WEP_64;
		} else if (num_sec_modes == 2 &&
				   sec_modes[0].secmode == SLSI_SEC_MODE_WPA_MIXED &&
				   sec_modes[1].secmode == SLSI_SEC_MODE_WPA2_MIXED) {
			*auth = WIFI_UTILS_AUTH_WPA_AND_WPA2_PSK;
			*crypto = WIFI_UTILS_CRYPTO_TKIP_AND_AES;
		} else if (sec_modes->secmode == SLSI_SEC_MODE_WPA2_MIXED) {
			*auth = WIFI_UTILS_AUTH_WPA2_PSK;
			*crypto = WIFI_UTILS_CRYPTO_TKIP_AND_AES;
		} else if (sec_modes->secmode == SLSI_SEC_MODE_WPA2_CCMP) {
			*auth = WIFI_UTILS_AUTH_WPA2_PSK;
			*crypto = WIFI_UTILS_CRYPTO_AES;
		} else if (sec_modes->secmode == SLSI_SEC_MODE_WPA2_TKIP) {
			*auth = WIFI_UTILS_AUTH_WPA2_PSK;
			*crypto = WIFI_UTILS_CRYPTO_TKIP;
		} else if (sec_modes->secmode == SLSI_SEC_MODE_WPA_MIXED) {
			*auth = WIFI_UTILS_AUTH_WPA_PSK;
			*crypto = WIFI_UTILS_CRYPTO_TKIP_AND_AES;
		} else if (sec_modes->secmode == SLSI_SEC_MODE_WPA_CCMP) {
			*auth = WIFI_UTILS_AUTH_WPA_PSK;
			*crypto = WIFI_UTILS_CRYPTO_AES;
		} else if (sec_modes->secmode == SLSI_SEC_MODE_WPA_TKIP) {
			*auth = WIFI_UTILS_AUTH_WPA_PSK;
			*crypto = WIFI_UTILS_CRYPTO_TKIP;
		} else {
			*auth = WIFI_UTILS_AUTH_UNKNOWN;
			*crypto = WIFI_UTILS_CRYPTO_UNKNOWN;
		}
	}
}

static void
free_scan_results(wifi_utils_scan_list_s *scan_list)
{
	wifi_utils_scan_list_s *cur = scan_list, *prev = NULL;
	while (cur) {
		prev = cur;
		cur = cur->next;
		free(prev);
	}
	scan_list = NULL;
}

static wifi_utils_result_e
fetch_scan_results(wifi_utils_scan_list_s **scan_list, slsi_scan_info_t **slsi_scan_info)
{
	wifi_utils_scan_list_s *cur = NULL, *prev = NULL;
	slsi_scan_info_t *wifi_scan_iter = NULL;
	int cnt = 0;
	if (*slsi_scan_info == NULL) {
		return WIFI_UTILS_FAIL;
	}
	/* Initialize pointer */
	wifi_scan_iter = *slsi_scan_info;
	while (wifi_scan_iter) {
#if WIFI_UTILS_DEBUG
		/* Debug */
		ndbg("SSID (");
		int i = 0;
		for (; i < wifi_scan_iter->ssid_len; i++) {
			ndbg("%c", wifi_scan_iter->ssid[i]);
		}
		ndbg(")\n");
		ndbg("rssi(%d)\n", wifi_scan_iter->rssi);
		ndbg("beacon_period(%d)\n", wifi_scan_iter->beacon_period);
		ndbg("channel(%d)\n", wifi_scan_iter->channel);
		ndbg("physical mode(%d)\n", wifi_scan_iter->phy_mode);
		ndbg("bss type(%d)\n", wifi_scan_iter->bss_type);
		ndbg("wps support(%d)\n", wifi_scan_iter->wps_support);
		ndbg("num_sec modes(%d)\n", wifi_scan_iter->num_sec_modes);
		ndbg("-----------------------------------------------\n");
#endif
		cur = (wifi_utils_scan_list_s *)malloc(sizeof(wifi_utils_scan_list_s));
		if (!cur) {
			free_scan_results(*scan_list);
			return WIFI_UTILS_FAIL;
		}
		cur->next = NULL;

		memset(&cur->ap_info, 0x0, sizeof(wifi_utils_ap_scan_info_s));
		cur->ap_info.rssi = wifi_scan_iter->rssi;
		cur->ap_info.channel = wifi_scan_iter->channel;
		cur->ap_info.phy_mode = wifi_scan_iter->phy_mode;
		get_security_type(wifi_scan_iter->sec_modes, wifi_scan_iter->num_sec_modes,
						  &cur->ap_info.ap_auth_type, &cur->ap_info.ap_crypto_type);
		strncpy(cur->ap_info.ssid, (char *)wifi_scan_iter->ssid, wifi_scan_iter->ssid_len);
		cur->ap_info.ssid_length = wifi_scan_iter->ssid_len;
		strncpy(cur->ap_info.bssid, (char *)wifi_scan_iter->bssid, SLSI_MACADDR_STR_LEN);

		if (!prev) {
			*scan_list = cur;
		} else {
			prev->next = cur;
		}
		prev = cur;
		wifi_scan_iter = wifi_scan_iter->next;
		cnt++;
	}
	ndbg("%d records scanned\n", cnt);

	return WIFI_UTILS_SUCCESS;
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

static void linkup_handler(slsi_reason_t *reason)
{
	wifi_utils_result_e res = WIFI_UTILS_FAIL;
	if (reason->reason_code == SLSI_STATUS_SUCCESS) {
		res = WIFI_UTILS_SUCCESS;
	}
	int *type = (int *)malloc(sizeof(int));
	if (type == NULL) {
		ndbg("[WU] malloc error\n");
		return;
	}

	if (g_mode == SLSI_WIFI_STATION_IF) {
		if (res == WIFI_UTILS_SUCCESS) {
			*type = 1;
		} else {
			*type = 2;
		}
	} else if (g_mode == SLSI_WIFI_SOFT_AP_IF) {
		*type = 3;
	}
	pthread_t tid;
	int ret = pthread_create(&tid, NULL, (pthread_startroutine_t)callback_handler, (void *)type);
	if (ret != 0) {
		ndbg("[WU] pthread create fail(%d)\n", errno);
		free(type);
		return;
	}
	pthread_setname_np(tid, "wifi_utils_cbk_handler");
	pthread_detach(tid);
}

static void linkdown_handler(slsi_reason_t *reason)
{
	int *type = (int *)malloc(sizeof(int));
	if (type == NULL) {
		ndbg("[WU] malloc error linkdown\n");
		return;
	}
	*type = 4;
	if (g_mode == SLSI_WIFI_STATION_IF) {
		*type = 4;
	} else if (g_mode == SLSI_WIFI_SOFT_AP_IF) {
		*type = 5;
	}
	pthread_t tid;
	int ret = pthread_create(&tid, NULL, (pthread_startroutine_t)callback_handler, (void *)type);
	if (ret != 0) {
		ndbg("[WU] pthread create fail(%d)\n", errno);
		free(type);
		return;
	}
	pthread_setname_np(tid, "wifi_utils_cbk_handler");
	pthread_detach(tid);
}

static int8_t wifi_scan_result_callback(slsi_reason_t *reason)
{
	wifi_utils_scan_list_s *scan_list = NULL;
	if (reason->reason_code != SLSI_STATUS_SUCCESS) {
		ndbg("[WU] Scan failed %d\n");
		// todo: arg need to be passed, we didn't implement passing arg yet.
		g_cbk.scan_done(WIFI_UTILS_FAIL, NULL, NULL);
		return SLSI_STATUS_ERROR;
	}
	slsi_scan_info_t *wifi_scan_result;
	int8_t res = WiFiGetScanResults(&wifi_scan_result);
	if (res != SLSI_STATUS_SUCCESS) {
		return SLSI_STATUS_ERROR;
	}
	if (g_cbk.scan_done) {
		if (fetch_scan_results(&scan_list, &wifi_scan_result) == WIFI_UTILS_SUCCESS) {
			g_cbk.scan_done(WIFI_UTILS_SUCCESS, scan_list, NULL);
			free_scan_results(scan_list);
		} else {
			g_cbk.scan_done(WIFI_UTILS_FAIL, NULL, NULL);
		}
	}
	WiFiFreeScanResults(&wifi_scan_result);

	return SLSI_STATUS_SUCCESS;
}


//
// Interface API
//
wifi_utils_result_e wifi_utils_init(void)
{
	if (g_mode != SLSI_WIFI_NONE) {
		return WIFI_UTILS_FAIL;
	}

	int ret = SLSI_STATUS_SUCCESS;
	g_cbk = (wifi_utils_cb_s){NULL, NULL, NULL, NULL, NULL};
	ret = WiFiStart(SLSI_WIFI_STATION_IF, NULL);
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("[WU] Failed to start STA mode\n");
		return WIFI_UTILS_FAIL;
	}
	g_mode = SLSI_WIFI_STATION_IF;

	ret = WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler);
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("[WU] Link callback handles: register failed !\n");
		return WIFI_UTILS_FAIL;
	} else {
		nvdbg("[WU] Link callback handles: registered\n");
	}

	ret = WiFiRegisterScanCallback(&wifi_scan_result_callback);
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("[WU] [ERR] Register Scan Callback(%d)\n", ret);
		return WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_deinit(void)
{
	int ret = WiFiStop();
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("[WU] Failed to stop STA mode\n");
		return WIFI_UTILS_FAIL;
	}
	g_mode = SLSI_WIFI_NONE;
	g_cbk = (wifi_utils_cb_s){NULL, NULL, NULL, NULL, NULL};

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_scan_ap(void *arg)
{
	int8_t ret = WiFiScanNetwork();
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("[WU] WiFI scan fail(%d).", ret);
		return WIFI_UTILS_FAIL;
	}
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_register_callback(wifi_utils_cb_s *cbk)
{
	if (!cbk) {
		return WIFI_UTILS_INVALID_ARGS;
	}
	g_cbk = *cbk;
	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_connect_ap(wifi_utils_ap_config_s *ap_connect_config, void *arg)
{
	if (!ap_connect_config) {
		return WIFI_UTILS_INVALID_ARGS;
	}

	int ret;
	wifi_utils_result_e result = WIFI_UTILS_FAIL;
	slsi_security_config_t *config = NULL;

	if (ap_connect_config->passphrase_length > 0) {
		config = (slsi_security_config_t *)zalloc(sizeof(slsi_security_config_t));
		if (!config) {
			ndbg("[WU] Memory allocation failed!\n");
			goto connect_ap_fail;
		}

		if ((ap_connect_config->ap_auth_type == WIFI_UTILS_AUTH_WEP_SHARED) &&
			(ap_connect_config->passphrase_length == 5 || ap_connect_config->passphrase_length == 13)) {
			config->passphrase[0] = '"';
			memcpy(&config->passphrase[1], ap_connect_config->passphrase,
				   ap_connect_config->passphrase_length);
			config->passphrase[ap_connect_config->passphrase_length + 1] = '"';
			config->passphrase[ap_connect_config->passphrase_length + 2] = '\0';
		} else {
			memcpy(config->passphrase, ap_connect_config->passphrase, ap_connect_config->passphrase_length);
		}

		if (ap_connect_config->ap_auth_type == WIFI_UTILS_AUTH_WEP_SHARED) {
			config->secmode = SLSI_SEC_MODE_WEP_SHARED;
		} else if (ap_connect_config->ap_auth_type == WIFI_UTILS_AUTH_WPA_PSK) {
			if (ap_connect_config->ap_crypto_type == WIFI_UTILS_CRYPTO_AES) {
				config->secmode = SLSI_SEC_MODE_WPA_CCMP;
			} else if (ap_connect_config->ap_crypto_type == WIFI_UTILS_CRYPTO_TKIP) {
				config->secmode = SLSI_SEC_MODE_WPA_TKIP;
			} else if (ap_connect_config->ap_crypto_type == WIFI_UTILS_CRYPTO_TKIP_AND_AES) {
				config->secmode = SLSI_SEC_MODE_WPA_MIXED;
			}
		} else if (ap_connect_config->ap_auth_type == WIFI_UTILS_AUTH_WPA2_PSK) {
			if (ap_connect_config->ap_crypto_type == WIFI_UTILS_CRYPTO_AES) {
				config->secmode = SLSI_SEC_MODE_WPA2_CCMP;
			} else if (ap_connect_config->ap_crypto_type == WIFI_UTILS_CRYPTO_TKIP) {
				config->secmode = SLSI_SEC_MODE_WPA2_TKIP;
			} else if (ap_connect_config->ap_crypto_type == WIFI_UTILS_CRYPTO_TKIP_AND_AES) {
				config->secmode = SLSI_SEC_MODE_WPA2_MIXED;
			}
		} else if (ap_connect_config->ap_auth_type == WIFI_UTILS_AUTH_WPA_AND_WPA2_PSK) {
			if (ap_connect_config->ap_crypto_type == WIFI_UTILS_CRYPTO_AES) {
				config->secmode = (SLSI_SEC_MODE_WPA_CCMP | SLSI_SEC_MODE_WPA2_CCMP);
			} else if (ap_connect_config->ap_crypto_type == WIFI_UTILS_CRYPTO_TKIP) {
				config->secmode = (SLSI_SEC_MODE_WPA_TKIP | SLSI_SEC_MODE_WPA2_TKIP);
			} else if (ap_connect_config->ap_crypto_type == WIFI_UTILS_CRYPTO_TKIP_AND_AES) {
				config->secmode = (SLSI_SEC_MODE_WPA_MIXED | SLSI_SEC_MODE_WPA2_MIXED);
			}
		} else {
			/* wrong security type */
			ndbg("[WU] Wrong security type\n");
			goto connect_ap_fail;
		}
	} else {
		ndbg("[WU] No passphrase!\n");
		goto connect_ap_fail;
	}

	ret = WiFiNetworkJoin((uint8_t *)ap_connect_config->ssid, ap_connect_config->ssid_length, NULL, config);
	if (ret != SLSI_STATUS_SUCCESS) {
		if (ret == SLSI_STATUS_ALREADY_CONNECTED) {
			nvdbg("[WU] WiFiNetworkJoin already connected\n");
			result = WIFI_UTILS_ALREADY_CONNECTED;
		} else {
			ndbg("[WU] WiFiNetworkJoin failed: %d, %s\n", ret, ap_connect_config->ssid);
			goto connect_ap_fail;
		}
	} else {
		result = WIFI_UTILS_SUCCESS;
		nvdbg("[WU] Successfully joined the network: %s(%d)\n", ap_connect_config->ssid,
			  ap_connect_config->ssid_length);
	}

connect_ap_fail:
	if (config) {
		free(config);
		config = NULL;
	}

	return result;
}

wifi_utils_result_e wifi_utils_disconnect_ap(void *arg)
{
	int ret = WiFiNetworkLeave();
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("[WU] WiFiNetworkLeave fail because of %d\n", ret);
		return WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_get_info(wifi_utils_info *wifi_info)
{
	if (!wifi_info) {
		return WIFI_UTILS_INVALID_ARGS;
	}
	if (g_mode == SLSI_WIFI_NONE) {
		return WIFI_UTILS_FAIL;
	}

	int ret = WiFiGetMac(wifi_info->mac_address);
	if (ret != SLSI_STATUS_SUCCESS) {
		return WIFI_UTILS_FAIL;
	}

	wifi_info->rssi = (int)0;

	if (g_mode == SLSI_WIFI_SOFT_AP_IF) {
		wifi_info->wifi_status = WIFI_UTILS_SOFTAP_MODE;
	} else if (g_mode == SLSI_WIFI_STATION_IF) {
		uint8_t isConnected;
		if (WiFiIsConnected(&isConnected, NULL) == SLSI_STATUS_SUCCESS) {
			int8_t rssi;
			wifi_info->wifi_status = WIFI_UTILS_CONNECTED;
			if (WiFiGetRssi(&rssi) == SLSI_STATUS_SUCCESS) {
				wifi_info->rssi = (int)rssi;
			}
		} else {
			wifi_info->wifi_status = WIFI_UTILS_DISCONNECTED;
		}
	}

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_start_softap(wifi_utils_softap_config_s *softap_config)
{
	if (!softap_config) {
		return WIFI_UTILS_INVALID_ARGS;
	}

	wifi_utils_result_e ret = WIFI_UTILS_FAIL;
	slsi_ap_config_t *ap_config = NULL;
	slsi_security_config_t *security_config = NULL;

	ap_config = (slsi_ap_config_t *)zalloc(sizeof(slsi_ap_config_t));
	if (!ap_config) {
		ndbg("[WU] Memory allocation failed!\n");
		return WIFI_UTILS_FAIL;
	}

	/* add initialization code as slsi_app */
	ap_config->beacon_period = 100;
	ap_config->DTIM = 1;
	ap_config->phy_mode = 1;

	if (softap_config->channel > 14 || softap_config->channel < 1) {
		ndbg("[WU] Channel needs to be between 1 and 14" " (highest channel depends on regulatory of countries)\n");
		goto start_soft_ap_fail;
	} else {
		ap_config->channel = softap_config->channel;
	}

	if (softap_config->ssid_length < 1) {
		goto start_soft_ap_fail;
	} else {
		memcpy(&ap_config->ssid, softap_config->ssid, softap_config->ssid_length);
		ap_config->ssid_len = softap_config->ssid_length;
	}

	if (softap_config->passphrase_length < 1) {
		goto start_soft_ap_fail;
	} else {
		security_config = (slsi_security_config_t *)zalloc(sizeof(slsi_security_config_t));
		if (!security_config) {
			ndbg("[WU] Memory allocation failed!\n");
			goto start_soft_ap_fail;
		}
		memcpy(security_config->passphrase, softap_config->passphrase, softap_config->passphrase_length);
	}

	if ((softap_config->ap_auth_type == WIFI_UTILS_AUTH_WPA_PSK) &&
		(softap_config->ap_crypto_type == WIFI_UTILS_CRYPTO_TKIP)) {
		security_config->secmode = SLSI_SEC_MODE_WPA_TKIP;
	} else if ((softap_config->ap_auth_type == WIFI_UTILS_AUTH_WPA2_PSK) &&
			   (softap_config->ap_crypto_type == WIFI_UTILS_CRYPTO_AES)) {
		security_config->secmode = SLSI_SEC_MODE_WPA2_CCMP;
	} else if ((softap_config->ap_auth_type == WIFI_UTILS_AUTH_WPA_AND_WPA2_PSK) &&
			   (softap_config->ap_crypto_type == WIFI_UTILS_CRYPTO_TKIP_AND_AES)) {
		security_config->secmode = (SLSI_SEC_MODE_WPA_MIXED | SLSI_SEC_MODE_WPA2_MIXED);
	} else {
		// if not WPA-TKIP, WPA2-AES, WPA/WPA2 TKIP/AES/MIXED, return fail.
		ndbg("[WU] Wrong security config. Match proper auth and crypto.\n");
		goto start_soft_ap_fail;
	}
	ap_config->security = security_config;

	if (WiFiStart(SLSI_WIFI_SOFT_AP_IF, ap_config) != SLSI_STATUS_SUCCESS) {
		ndbg("[WU] Failed to start AP mode\n");
		goto start_soft_ap_fail;
	}
	g_mode = SLSI_WIFI_SOFT_AP_IF;
	nvdbg("[WU] SoftAP with SSID: %s has successfully started!\n", softap_config->ssid);

	ret = WIFI_UTILS_SUCCESS;

start_soft_ap_fail:
	if (ap_config) {
		free(ap_config);
		ap_config = NULL;
	}
	if (security_config) {
		free(security_config);
		security_config = NULL;
	}
	return ret;
}

wifi_utils_result_e wifi_utils_start_sta(void)
{
	int ret = SLSI_STATUS_SUCCESS;
	ret = WiFiStart(SLSI_WIFI_STATION_IF, NULL);
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("[WU] Failed to start STA mode\n");
		return WIFI_UTILS_FAIL;
	}
	g_mode = SLSI_WIFI_STATION_IF;

	ret = WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler);
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("[WU] Link callback handles: register failed !\n");
		return WIFI_UTILS_FAIL;
	} else {
		nvdbg("[WU] Link callback handles: registered\n");
	}

	ret = WiFiRegisterScanCallback(&wifi_scan_result_callback);
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("[WU] [ERR] Register Scan Callback(%d)\n", ret);
		return WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_stop_softap(void)
{
	int ret;

	if (g_mode != SLSI_WIFI_SOFT_AP_IF) {
		return WIFI_UTILS_FAIL;
	}

	ret = WiFiStop();
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("[WU] Failed to stop AP mode\n");
		return WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}
