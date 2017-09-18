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
#include <slsi_wifi/slsi_wifi_api.h>
#include "wifi_utils.h"

#include <net/lwip/netif.h>		// to be fixed

#define MACADDR_LENGTH			6
#define SSID_LENGTH_MAX			32
#define PASSPHRASE_LENGTH_MAX	64
#define DHCP_RETRY_COUNT		1

typedef void (*link_up_handler)(void *params);
typedef void (*link_down_handler)(void *params);

static sem_t g_sem_scan_result;
static WiFi_InterFace_ID_t g_mode;
static uint8_t g_join_result;
static link_up_handler g_linkup = NULL;
static link_down_handler g_linkdown = NULL;

void linkup_handler(slsi_reason_t *reason)
{
	g_join_result = reason->reason_code;
	if ((reason->reason_code == SLSI_STATUS_SUCCESS) && g_linkup) {
		g_linkup(NULL);
	}
}

void linkdown_handler(slsi_reason_t *reason)
{
	if (g_linkdown) {
		g_linkdown(NULL);
	}
}

wifi_utils_result_e wifi_utils_init(void)
{
	if (g_mode != SLSI_WIFI_NONE) {
		return WIFI_UTILS_FAIL;
	}

	int ret;

	ret = WiFiStart(SLSI_WIFI_STATION_IF, NULL);
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("Failed to start STA mode\n");
		return WIFI_UTILS_FAIL;
	}
	g_mode = SLSI_WIFI_STATION_IF;

	ret = WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler);
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("Link callback handles: register failed !\n");
		return WIFI_UTILS_FAIL;
	} else {
		nvdbg("Link callback handles: registered\n");
	}
	sem_init(&g_sem_scan_result, 0, 0);

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_deinit(void)
{
	int ret;

	ret = WiFiStop();
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("Failed to stop STA mode\n");
		return WIFI_UTILS_FAIL;
	}
	g_mode = SLSI_WIFI_NONE;

	g_linkup = NULL;
	g_linkdown = NULL;
	g_join_result = 0;
	sem_destroy(&g_sem_scan_result);

	return WIFI_UTILS_SUCCESS;
}

int8_t scan_result_handler(slsi_reason_t *reason)
{
	if (reason->reason_code == 0) {
		/* Scan succeeded */
	} else {
		ndbg("Scan failed reason: %d, locally_generated: %d\n", reason->reason_code, reason->locally_generated);
	}

	sem_post(&g_sem_scan_result);
	return 0;
}

wifi_utils_result_e wifi_utils_scan_ap(wifi_utils_ap_scan_info_s *ap_list, unsigned int list_size, unsigned int *found_ap_count)
{
	if (!ap_list || (list_size == 0) || !found_ap_count) {
		return WIFI_UTILS_INVALID_ARGS;
	}

	int ret;
	unsigned int bss_count = 0;
	slsi_scan_info_t *results = NULL;
	slsi_scan_info_t *head = NULL;

	// issue a scan request command and wait for the message return
	WiFiRegisterScanCallback(scan_result_handler);
	ret = WiFiScanNetwork();
	if (ret != SLSI_STATUS_SUCCESS) {
		return WIFI_UTILS_FAIL;
	}

	sem_wait(&g_sem_scan_result);

	// get scan results when message arrives
	ret = WiFiGetScanResults(&results);
	if (ret != SLSI_STATUS_SUCCESS || !results) {
		return WIFI_UTILS_FAIL;
	}
	head = results;

	do {
		ap_list[bss_count].channel = results->channel;
		snprintf(ap_list[bss_count].ssid, SLSI_SSID_LEN + 1, "%s", results->ssid);
		ap_list[bss_count].ssid_length = (unsigned int)(results->ssid_len);
		hwaddr_aton(results->bssid, ap_list[bss_count].bssid);
		ap_list[bss_count].rssi = results->rssi;

		if (!results->sec_modes) {
			ap_list[bss_count].ap_auth_type = WIFI_UTILS_AUTH_OPEN;
			ap_list[bss_count].ap_crypto_type = WIFI_UTILS_CRYPTO_NONE;
		} else {
			if (results->sec_modes->secmode == SLSI_SEC_MODE_WEP || results->sec_modes->secmode == SLSI_SEC_MODE_WEP_SHARED || results->sec_modes->secmode == (SLSI_SEC_MODE_WEP | SLSI_SEC_MODE_WEP_SHARED)) {
				ap_list[bss_count].ap_auth_type = WIFI_UTILS_AUTH_WEP_SHARED;
				ap_list[bss_count].ap_crypto_type = WIFI_UTILS_CRYPTO_WEP_64;
			} else if (results->num_sec_modes == 2 && results->sec_modes[0].secmode == SLSI_SEC_MODE_WPA_MIXED && results->sec_modes[1].secmode == SLSI_SEC_MODE_WPA2_MIXED) {
				ap_list[bss_count].ap_auth_type = WIFI_UTILS_AUTH_WPA_AND_WPA2_PSK;
				ap_list[bss_count].ap_crypto_type = WIFI_UTILS_CRYPTO_TKIP_AND_AES;
			} else if (results->sec_modes->secmode == SLSI_SEC_MODE_WPA2_MIXED) {
				ap_list[bss_count].ap_auth_type = WIFI_UTILS_AUTH_WPA2_PSK;
				ap_list[bss_count].ap_crypto_type = WIFI_UTILS_CRYPTO_TKIP_AND_AES;
			} else if (results->sec_modes->secmode == SLSI_SEC_MODE_WPA2_CCMP) {
				ap_list[bss_count].ap_auth_type = WIFI_UTILS_AUTH_WPA2_PSK;
				ap_list[bss_count].ap_crypto_type = WIFI_UTILS_CRYPTO_AES;
			} else if (results->sec_modes->secmode == SLSI_SEC_MODE_WPA2_TKIP) {
				ap_list[bss_count].ap_auth_type = WIFI_UTILS_AUTH_WPA2_PSK;
				ap_list[bss_count].ap_crypto_type = WIFI_UTILS_CRYPTO_TKIP;
			} else if (results->sec_modes->secmode == SLSI_SEC_MODE_WPA_MIXED) {
				ap_list[bss_count].ap_auth_type = WIFI_UTILS_AUTH_WPA_PSK;
				ap_list[bss_count].ap_crypto_type = WIFI_UTILS_CRYPTO_TKIP_AND_AES;
			} else if (results->sec_modes->secmode == SLSI_SEC_MODE_WPA_CCMP) {
				ap_list[bss_count].ap_auth_type = WIFI_UTILS_AUTH_WPA_PSK;
				ap_list[bss_count].ap_crypto_type = WIFI_UTILS_CRYPTO_AES;
			} else if (results->sec_modes->secmode == SLSI_SEC_MODE_WPA_TKIP) {
				ap_list[bss_count].ap_auth_type = WIFI_UTILS_AUTH_WPA_PSK;
				ap_list[bss_count].ap_crypto_type = WIFI_UTILS_CRYPTO_TKIP;
			} else {
				ap_list[bss_count].ap_auth_type = WIFI_UTILS_AUTH_UNKNOWN;
				ap_list[bss_count].ap_crypto_type = WIFI_UTILS_CRYPTO_UNKNOWN;
			}
		}

		results = results->next;
		if (ap_list[bss_count].ap_auth_type != WIFI_UTILS_AUTH_UNKNOWN && ap_list[bss_count].ap_crypto_type != WIFI_UTILS_CRYPTO_UNKNOWN) {
			bss_count++;
		}
	} while (results->next && bss_count < list_size);

	*found_ap_count = bss_count;
	results = head;
	WiFiFreeScanResults(&results);

	int i;
	for (i = 0; i < *found_ap_count && i < list_size; i++) {
		int j;
		int max = i;
		for (j = i + 1; j < *found_ap_count && i < list_size; j++) {
			if (ap_list[max].rssi < ap_list[j].rssi) {
				max = j;
			}
		}

		wifi_utils_ap_scan_info_s tmp;
		tmp = ap_list[i];
		ap_list[i] = ap_list[max];
		ap_list[max] = tmp;
	}

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_register_callback(void *linkup_event_func, void *linkdown_event_func)
{
	g_linkup = linkup_event_func;
	g_linkdown = linkdown_event_func;

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_connect_ap(wifi_utils_ap_config_s *ap_connect_config)
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
			ndbg("Memory allocation failed!\n");
			goto connect_ap_fail;
		}

		if ((ap_connect_config->ap_auth_type == WIFI_UTILS_AUTH_WEP_SHARED) && (ap_connect_config->passphrase_length == 5 || ap_connect_config->passphrase_length == 13)) {
			config->passphrase[0] = '"';
			memcpy(&config->passphrase[1], ap_connect_config->passphrase, ap_connect_config->passphrase_length);
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
			ndbg("Wrong security type\n");
			goto connect_ap_fail;
		}
	} else {
		ndbg("No passphrase!\n");
		goto connect_ap_fail;
	}

	ret = WiFiNetworkJoin((uint8_t *)ap_connect_config->ssid, ap_connect_config->ssid_length, NULL, config);
	if (ret != SLSI_STATUS_SUCCESS) {
		if (ret == SLSI_STATUS_ALREADY_CONNECTED) {
			nvdbg("WiFiNetworkJoin already connected\n");
			result = WIFI_UTILS_SUCCESS;
		} else {
			ndbg("WiFiNetworkJoin failed: %d, %s\n", ret, ap_connect_config->ssid);
			goto connect_ap_fail;
		}
	} else {
		result = WIFI_UTILS_SUCCESS;
		nvdbg("Successfully joined the network: %s(%d)\n", ap_connect_config->ssid, ap_connect_config->ssid_length);
	}

connect_ap_fail:
	if (config) {
		free(config);
		config = NULL;
	}

	return result;
}

wifi_utils_result_e wifi_utils_disconnect_ap(void)
{
	int ret;

	ret = WiFiNetworkLeave();
	if (ret != SLSI_STATUS_SUCCESS) {
		ndbg("WiFiNetworkLeave fail because of %d\n", ret);
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

	struct netif *netif;

	netif = netif_find(CTRL_IFNAME);	/* to be changed pkes */
	wifi_info->ip4_address = netif->ip_addr.addr;

	int ret;

	ret = WiFiGetMac(wifi_info->mac_address);
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
		ndbg("Memory allocation failed!\n");
		return WIFI_UTILS_FAIL;
	}

	/* add initialization code as slsi_app */
	ap_config->beacon_period = 100;
	ap_config->DTIM = 1;
	ap_config->phy_mode = 1;

	if (softap_config->channel > 14 || softap_config->channel < 1) {
		ndbg("Channel needs to be between 1 and 14" " (highest channel depends on regulatory of countries)\n");
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
			ndbg("Memory allocation failed!\n");
			goto start_soft_ap_fail;
		}
		memcpy(security_config->passphrase, softap_config->passphrase, softap_config->passphrase_length);
	}

	if ((softap_config->ap_auth_type == WIFI_UTILS_AUTH_WPA_PSK) && (softap_config->ap_crypto_type == WIFI_UTILS_CRYPTO_TKIP)) {
		security_config->secmode = SLSI_SEC_MODE_WPA_TKIP;
	} else if ((softap_config->ap_auth_type == WIFI_UTILS_AUTH_WPA2_PSK) && (softap_config->ap_crypto_type == WIFI_UTILS_CRYPTO_AES)) {
		security_config->secmode = SLSI_SEC_MODE_WPA2_CCMP;
	} else if ((softap_config->ap_auth_type == WIFI_UTILS_AUTH_WPA_AND_WPA2_PSK) && (softap_config->ap_crypto_type == WIFI_UTILS_CRYPTO_TKIP_AND_AES)) {
		security_config->secmode = (SLSI_SEC_MODE_WPA_MIXED | SLSI_SEC_MODE_WPA2_MIXED);
	} else {
		// if not WPA-TKIP, WPA2-AES, WPA/WPA2 TKIP/AES/MIXED, return fail.
		ndbg("Wrong security config. Match proper auth and crypto.\n");
		goto start_soft_ap_fail;
	}
	ap_config->security = security_config;

	if (WiFiStart(SLSI_WIFI_SOFT_AP_IF, ap_config) != SLSI_STATUS_SUCCESS) {
		ndbg("Failed to start AP mode\n");
		goto start_soft_ap_fail;
	}
	g_mode = SLSI_WIFI_SOFT_AP_IF;
	nvdbg("SoftAP with SSID: %s has successfully started!\n", softap_config->ssid);

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
	g_mode = SLSI_WIFI_NONE;

	g_join_result = 0;

	if (wifi_utils_init() != WIFI_UTILS_SUCCESS) {
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
		ndbg("Failed to stop AP mode\n");
		return WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}
