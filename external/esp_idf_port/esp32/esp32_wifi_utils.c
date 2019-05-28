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
#include <tinyara/config.h>
#include <debug.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <sched.h>
#include <debug.h>
#include <fcntl.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <tinyara/wdog.h>
#include <tinyara/arch.h>
#include <tinyara/cancelpt.h>

#include "wifi_common.h"
#include "wifi_utils.h"

#include "esp_attr.h"
#include "esp_phy_init.h"
#include "esp_wifi_os_adapter.h"
#include "esp_system.h"
#include "rom/ets_sys.h"
#include <sched/sched.h>
#include <semaphore/semaphore.h>
#include <os.h>

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "esp_wifi_internal.h"

#define WIFI_CONN_RETRY              5
#define WIFI_MAX_STA_CONN            4
#define WIFI_DEFAULT_BEACON_INTERVAL 100

#ifndef CONFIG_MAX_STA_CONN
#define CONFIG_MAX_STA_CONN WIFI_MAX_STA_CONN
#endif

#define DEFAULT_SSID    CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC_SSID
#define DEFAULT_PWD     CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC_PASSPHRASE

#define DEFAULT_PS_MODE WIFI_PS_MIN_MODEM

#ifdef CONFIG_WIFI_ALL_CHANNEL_SCAN
#define DEFAULT_SCAN_METHOD WIFI_ALL_CHANNEL_SCAN
#elif CONFIG_WIFI_FAST_SCAN
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#else
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#endif							/*CONFIG_SCAN_METHOD */

#ifdef CONFIG_WIFI_CONNECT_AP_BY_SIGNAL
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#elif CONFIG_WIFI_CONNECT_AP_BY_SECURITY
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SECURITY
#else
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#endif							/*CONFIG_SORT_METHOD */

#ifdef CONFIG_FAST_SCAN_THRESHOLD
#define DEFAULT_RSSI CONFIG_FAST_SCAN_MINIMUM_SIGNAL
#ifdef CONFIG_EXAMPLE_OPEN
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#elif CONFIG_EXAMPLE_WEP
#define DEFAULT_AUTHMODE WIFI_AUTH_WEP
#elif CONFIG_EXAMPLE_WPA
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA_PSK
#elif CONFIG_EXAMPLE_WPA2
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA2_PSK
#else
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif
#else
#define DEFAULT_RSSI (-127)
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif							/*CONFIG_FAST_SCAN_THRESHOLD */

typedef enum WiFi_InterFace_ID {
	WIFI_NONE,					// default
	WIFI_STATION_IF,			// Station mode (turns on wpa_supplicant)
	WIFI_SOFT_AP_IF,			// Soft AP mode (turns on hostapd)
	WIFI_P2P_IF					// P2P mode (turns on wpa_supplicant)
} WiFi_InterFace_ID_t;

static wifi_utils_cb_s g_cbk = { NULL, NULL, NULL, NULL, NULL };

static WiFi_InterFace_ID_t g_mode = WIFI_STATION_IF;
static volatile int isStaConnected = 0;
static volatile int isAPJoined = 0;
static volatile int s_retry_num = 0;
static bool wifi_init = false;
static bool event_init = false;

static const char *TAG = "WIFI_UITLS";

wifi_utils_result_e wifi_utils_stop_sta(void);

void scan_done_handler(void)
{
	uint16_t sta_number = 0;
	uint8_t i;
	wifi_ap_record_t *ap_list_buffer;

	esp_wifi_scan_get_ap_num(&sta_number);
	ESP_LOGI(TAG, "scan_done_handler sta_number is %d!\n", sta_number);
	ap_list_buffer = (wifi_ap_record_t *)malloc(sta_number * sizeof(wifi_ap_record_t));
	if (ap_list_buffer == NULL) {
		ESP_LOGE(TAG, "Failed to malloc buffer to print scan results");
		return;
	}

	wifi_utils_scan_list_s *scan_list = NULL;
	scan_list = (wifi_utils_scan_list_s *)zalloc(sta_number * sizeof(wifi_utils_scan_list_s));
	if (scan_list == NULL) {
		ESP_LOGE(TAG, "Failed to malloc buffer to print scan results");
		return;
	}

	if (esp_wifi_scan_get_ap_records(&sta_number, (wifi_ap_record_t *) ap_list_buffer) == ESP_OK) {
		for (i = 0; i < sta_number; i++) {
			scan_list[i].ap_info.channel = ap_list_buffer[i].primary;
			strncpy(scan_list[i].ap_info.ssid, ap_list_buffer[i].ssid, WIFI_UTILS_SSID_LEN);
			scan_list[i].ap_info.ssid_length = strlen(ap_list_buffer[i].ssid);
			snprintf(scan_list[i].ap_info.bssid, WIFI_UTILS_MACADDR_STR_LEN, "%02x:%02x:%02x:%02x:%02x:%02x", ap_list_buffer[i].bssid[0], ap_list_buffer[i].bssid[1], ap_list_buffer[i].bssid[2], ap_list_buffer[i].bssid[3], ap_list_buffer[i].bssid[4], ap_list_buffer[i].bssid[5]);
			scan_list[i].ap_info.max_rate = 0;
			scan_list[i].ap_info.rssi = ap_list_buffer[i].rssi;
			scan_list[i].ap_info.phy_mode = ap_list_buffer[i].phy_11n;
			switch (ap_list_buffer[i].authmode) {
			case WIFI_AUTH_OPEN:
				scan_list[i].ap_info.ap_auth_type = WIFI_UTILS_AUTH_OPEN;
				break;
			case WIFI_AUTH_WEP:
				scan_list[i].ap_info.ap_auth_type = WIFI_UTILS_AUTH_WEP_SHARED;
				break;
			case WIFI_AUTH_WPA_PSK:
				scan_list[i].ap_info.ap_auth_type = WIFI_UTILS_AUTH_WPA_PSK;
				break;
			case WIFI_AUTH_WPA2_PSK:
				scan_list[i].ap_info.ap_auth_type = WIFI_UTILS_AUTH_WPA2_PSK;
				break;
			case WIFI_AUTH_WPA_WPA2_PSK:
				scan_list[i].ap_info.ap_auth_type = WIFI_UTILS_AUTH_WPA_AND_WPA2_PSK;
				break;
			case WIFI_AUTH_WPA2_ENTERPRISE:
			default:
				scan_list[i].ap_info.ap_auth_type = WIFI_UTILS_AUTH_UNKNOWN;
				break;
			}

			switch (ap_list_buffer[i].pairwise_cipher) {
			case WIFI_CIPHER_TYPE_NONE:
				scan_list[i].ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_NONE;
				break;
			case WIFI_CIPHER_TYPE_WEP40:
				scan_list[i].ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_WEP_64;
				break;
			case WIFI_CIPHER_TYPE_WEP104:
				scan_list[i].ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_WEP_128;
				break;
			case WIFI_CIPHER_TYPE_TKIP:
				scan_list[i].ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_TKIP;
				break;
			case WIFI_CIPHER_TYPE_CCMP:
				scan_list[i].ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_AES;
				break;
			case WIFI_CIPHER_TYPE_TKIP_CCMP:
				scan_list[i].ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_TKIP_AND_AES;
				break;
			default:
				scan_list[i].ap_info.ap_crypto_type = WIFI_UTILS_CRYPTO_UNKNOWN;
				break;
			}

			if (i > 0) {
				scan_list[i - 1].next = &scan_list[i];
			}
		}
		g_cbk.scan_done(WIFI_UTILS_SUCCESS, scan_list, NULL);
	}
	free(ap_list_buffer);
	free(scan_list);
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	switch (event->event_id) {
	case SYSTEM_EVENT_SCAN_DONE: {
		scan_done_handler();
		ESP_LOGI(TAG, "SYSTEM_EVENT_SCAN_DONE ");
		break;
	}
	case SYSTEM_EVENT_STA_START: {
		isStaConnected = 0;
		ESP_LOGI(TAG, "sta start");
		break;
	}
	case SYSTEM_EVENT_STA_STOP: {
		isStaConnected = 0;
		ESP_LOGI(TAG, "sta stop");
		break;
	}
	case SYSTEM_EVENT_STA_GOT_IP: {
		ESP_LOGI(TAG, "got ip:%s", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
		break;
	}
	case SYSTEM_EVENT_STA_LOST_IP: {
		ESP_LOGI(TAG, "[%s] lost ip", __func__);
		break;
	}
	case SYSTEM_EVENT_STA_CONNECTED: {
		s_retry_num = 0;
		isStaConnected = 1;
		g_cbk.sta_connected(WIFI_UTILS_SUCCESS, NULL);
		ESP_LOGI(TAG, "[%s] SYSTEM_EVENT_STA_CONNECTED", __func__);
		break;
	}
	case SYSTEM_EVENT_STA_DISCONNECTED: {
		s_retry_num++;
		int reason = event->event_info.disconnected.reason;
		ESP_LOGI(TAG, "[%s] connect to the AP %s fail %d: %d; %d\n", __func__, event->event_info.disconnected.ssid, s_retry_num, event->event_info.disconnected.reason, isStaConnected);

		/*give chance to other thread to continue*/
		sleep(1);
		if (s_retry_num > 0) {
			ESP_LOGI(TAG, "g_cbk.sta_connected fail\n");
			if (s_retry_num < WIFI_CONN_RETRY) {
				esp_wifi_connect();
			}

			else {
				g_cbk.sta_connected(WIFI_UTILS_FAIL, NULL);
				s_retry_num = 0;
			}

		} else {
			ESP_LOGI(TAG, "g_cbk.sta_disconnected\n");
			g_cbk.sta_disconnected(NULL);
		/*in case receive multiple times SYSTEM_EVENT_STA_DISCONNECTED*/
			s_retry_num = -1;
		}
		isStaConnected = 0;
		break;
	}
	case SYSTEM_EVENT_AP_START: {
		isStaConnected = 0;
		ESP_LOGI(TAG, "ap start");
		break;
	}
	case SYSTEM_EVENT_AP_STOP: {
		ESP_LOGI(TAG, "ap stop");
		break;
	}
	case SYSTEM_EVENT_AP_STACONNECTED: {
		isAPJoined = 1;
		ESP_LOGI(TAG, "station: join, AID=%d", MAC2STR(event->event_info.sta_connected.mac), event->event_info.sta_connected.aid);
		break;
	}
	case SYSTEM_EVENT_AP_STADISCONNECTED: {
		ESP_LOGI(TAG, "station: leave, AID=%d", MAC2STR(event->event_info.sta_disconnected.mac), event->event_info.sta_disconnected.aid);
		g_cbk.softap_sta_left(NULL);
		break;
	}
	case SYSTEM_EVENT_AP_STAIPASSIGNED: {
		ESP_LOGI(TAG, "ap ip assigned");
		g_cbk.softap_sta_joined(NULL);
		break;
	}
	default: {
		ESP_LOGI(TAG, "EVENT %d ", event->event_id);
		break;
	}
	}
	return ESP_OK;
}

//
// Interface API
//
wifi_utils_result_e wifi_utils_init(void)
{
	esp_err_t ret;
	tcpip_adapter_init();

	s_retry_num = 0;
	if (!event_init) {
		ret = esp_event_loop_init(event_handler, NULL);
		if (ret != ESP_OK) {
			ESP_LOGI(TAG, "esp_event_loop_init failed, %d\n", ret);
			return WIFI_UTILS_FAIL;
		}
		event_init = true;

	}
	esp_wifi_internal_set_log_level(WIFI_LOG_VERBOSE);
	esp_wifi_internal_set_log_mod(WIFI_LOG_MODULE_ALL, WIFI_LOG_MODULE_ALL | WIFI_LOG_SUBMODULE_INIT | WIFI_LOG_SUBMODULE_IOCTL | WIFI_LOG_SUBMODULE_CONN | WIFI_LOG_SUBMODULE_SCAN, true);

	if (!wifi_init) {
		wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
		ret = esp_wifi_init(&cfg);
		if (ret != ESP_OK) {
			ESP_LOGI(TAG, "esp_wifi_init failed, %d\n", ret);
			return WIFI_UTILS_FAIL;
		}
		wifi_init = true;
	}

	ret = esp_wifi_set_storage(WIFI_STORAGE_RAM);
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_set_storage failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}

	wifi_config_t wifi_config = {
		.sta = {
			.ssid = DEFAULT_SSID,
			.password = DEFAULT_PWD,
			.scan_method = WIFI_FAST_SCAN,
			.sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
			.threshold.rssi = -127,
			.threshold.authmode = WIFI_AUTH_OPEN,
		}
		,
	};

	ret = esp_wifi_set_mode(WIFI_MODE_STA);
	if (ret) {
		ESP_LOGI(TAG, "esp_wifi_set_mode failed, %d\n", ret);
		return;
	}

	ret = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
	if (ret) {
		ESP_LOGI(TAG, "esp_wifi_set_config failed, %d\n", ret);
		return;
	}

	ret = esp_wifi_start();
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_start failed\n");
		return WIFI_UTILS_FAIL;
	}

	g_mode = WIFI_STATION_IF;
	isStaConnected = 0;
	s_retry_num = 0;

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_deinit(void)
{
	esp_err_t ret;
	ESP_LOGI(TAG, "wifi_utils_deinit\n");
	if (g_mode == WIFI_STATION_IF) {
		ret = wifi_utils_stop_sta();
		if (ret != ESP_OK && ret != ESP_ERR_WIFI_NOT_INIT) {
			ESP_LOGI(TAG, "wifi_utils_stop_sta failed, %d\n", ret);
			return WIFI_UTILS_FAIL;
		}
		usleep(1000);
	} else if (g_mode == WIFI_SOFT_AP_IF) {
		ret = wifi_utils_stop_softap();
		if (ret != ESP_OK && ret != ESP_ERR_WIFI_NOT_INIT) {
			ESP_LOGI(TAG, "wifi_utils_stop_softap failed, %d\n", ret);
			return WIFI_UTILS_FAIL;
		}
		usleep(1000);
	}

	ret = esp_wifi_deinit();
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_deinit failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}
	wifi_init = false;

	g_cbk = (wifi_utils_cb_s) {
		NULL, NULL, NULL, NULL, NULL
	};
	g_mode = WIFI_NONE;
	isStaConnected = 0;
	s_retry_num = 0;
	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_scan_ap(void *arg)
{
	esp_err_t ret;
	wifi_scan_config_t scan_config = { 0 };
	scan_config.ssid = (uint8_t *) NULL;

	ret = esp_wifi_set_mode(WIFI_MODE_STA);
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_set_mode failed\n");
		return WIFI_UTILS_FAIL;
	}

	ret = esp_wifi_scan_start(&scan_config, false);
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_scan_start failed\n");
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
	esp_err_t ret;
	wifi_utils_ap_auth_type_e auth = ap_connect_config->ap_auth_type;
	wifi_utils_ap_crypto_type_e crypto = ap_connect_config->ap_crypto_type;
	/* WIFI_MANAGER_CRYPTO_AES: 3 */
	if (crypto == 3 && auth != WIFI_UTILS_AUTH_WPA2_PSK) {
		ESP_LOGI(TAG, "Invalid crypto/auth match\n");
		return WIFI_UTILS_FAIL;
	}
	wifi_config_t wifi_config = {
		.sta = {
			.ssid = DEFAULT_SSID,
			.password = DEFAULT_PWD,
			.scan_method = DEFAULT_SCAN_METHOD,
			.sort_method = DEFAULT_SORT_METHOD,
			.threshold.rssi = DEFAULT_RSSI,
			.threshold.authmode = DEFAULT_AUTHMODE,
		}
		,
	};
	strncpy((char *)wifi_config.sta.ssid, ap_connect_config->ssid, ap_connect_config->ssid_length);
	wifi_config.sta.ssid[ap_connect_config->ssid_length] = 0;
	strncpy((char *)wifi_config.sta.password, ap_connect_config->passphrase, ap_connect_config->passphrase_length);
	wifi_config.sta.password[ap_connect_config->passphrase_length] = 0;

	if (g_mode == WIFI_SOFT_AP_IF) {
		ret = wifi_utils_stop_softap();
		if (ret != ESP_OK) {
			ESP_LOGI(TAG, "wifi_utils_stop_softap failed, %d\n", ret);
			return WIFI_UTILS_FAIL;
		}
		usleep(1000);
	}

	ret = esp_wifi_set_mode(WIFI_MODE_STA);
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_set_mode failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}

	ret = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_set_config failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}

	ret = esp_wifi_start();
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_start failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}

	isStaConnected = 0;
	s_retry_num = 0;
	/* Join AP */
	esp_wifi_connect();
	g_mode = WIFI_STATION_IF;
	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_disconnect_ap(void *arg)
{
	int ret = WIFI_UTILS_SUCCESS;
	s_retry_num = -1;

	if (g_mode != WIFI_STATION_IF) {
		ESP_LOGI(TAG, "[%s] current mode is not station!\n", __func__);
		return WIFI_UTILS_FAIL;
	}

	if (isStaConnected > 0) {
		ret = esp_wifi_disconnect();
		if (ret != ESP_OK) {
			ESP_LOGI(TAG, "[%s] esp_wifi_disconnect failed, %d\n", __func__, ret);
			return WIFI_UTILS_FAIL;
		}
	}
	/* Kill WLAN Network */
	isStaConnected = 0;
	return ret;
}

wifi_utils_result_e wifi_utils_get_info(wifi_utils_info_s *wifi_info)
{
	esp_err_t ret;
	int32_t rssi = 0;
	uint32_t ip = 0;

	if (!wifi_info) {
		ESP_LOGI(TAG, "Invalid Arguments!\n");
		return WIFI_UTILS_INVALID_ARGS;
	}

	/* Get MAC address */
	if (g_mode == WIFI_SOFT_AP_IF) {
		wifi_info->wifi_status = WIFI_UTILS_SOFTAP_MODE;
		esp_read_mac(wifi_info->mac_address, ESP_MAC_WIFI_SOFTAP);
	} else if (g_mode == WIFI_STATION_IF) {
		esp_read_mac(wifi_info->mac_address, ESP_MAC_WIFI_STA);
		wifi_ap_record_t ap_info;
		ret = esp_wifi_sta_get_ap_info(&ap_info);
		if (ret == ESP_OK) {
			wifi_info->wifi_status = WIFI_UTILS_CONNECTED;
			wifi_info->rssi = ap_info.rssi;
		} else {
			wifi_info->wifi_status = WIFI_UTILS_DISCONNECTED;
			wifi_info->rssi = 0;
		}

		extern esp_err_t esp_wifi_get_ip(uint32_t * ip);
		esp_wifi_get_ip(&ip);
		wifi_info->ip4_address = ip;
	}

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_start_softap(wifi_utils_softap_config_s *softap_config)
{
	int ret = WIFI_UTILS_SUCCESS;
	wifi_auth_mode_t mode;

	if (g_mode == WIFI_STATION_IF) {
		ret = wifi_utils_stop_sta();
		if (ret != ESP_OK && ret != ESP_ERR_WIFI_NOT_INIT) {
			ESP_LOGI(TAG, "wifi_utils_stop_sta failed, %d\n", ret);
			return WIFI_UTILS_FAIL;
		}
		usleep(1000);
	}

	switch (softap_config->ap_auth_type) {
	case WIFI_UTILS_AUTH_OPEN:
		mode = WIFI_AUTH_OPEN;
		break;
	case WIFI_UTILS_AUTH_WEP_SHARED:
		mode = WIFI_AUTH_WEP;
		break;
	case WIFI_UTILS_AUTH_WPA_PSK:
		mode = WIFI_AUTH_WPA_PSK;
		break;
	case WIFI_UTILS_AUTH_WPA2_PSK:
		mode = WIFI_AUTH_WPA2_PSK;
		break;
	case WIFI_UTILS_AUTH_WPA_AND_WPA2_PSK:
		mode = WIFI_AUTH_WPA_WPA2_PSK;
		break;
	case WIFI_UTILS_AUTH_UNKNOWN:
	default:
		ESP_LOGI(TAG, "AP AUTH type is unknown %d;\n", softap_config->ap_auth_type);
		return WIFI_UTILS_INVALID_ARGS;
	}

	if (softap_config->channel > 14 || softap_config->channel < 1) {
		ESP_LOGI(TAG, "AP channel is wrong: %d;\n", softap_config->channel);
		return WIFI_UTILS_INVALID_ARGS;
	}

	if (softap_config->ssid_length < 1) {
		ESP_LOGI(TAG, "AP ssid_length is wrong: %s,  %d;\n", softap_config->ssid, softap_config->ssid_length);
		return WIFI_UTILS_INVALID_ARGS;
	}

	if (mode != WIFI_AUTH_OPEN && softap_config->passphrase_length < 1) {
		ESP_LOGI(TAG, "AP passphrase_length is wrong: %s,  %d;\n", softap_config->passphrase, softap_config->passphrase_length);
		return WIFI_UTILS_INVALID_ARGS;
	}

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

	if (!wifi_init) {
		ret = esp_wifi_init(&cfg);
		if (ret != ESP_OK) {
			ESP_LOGI(TAG, "esp_wifi_init failed, %d\n", ret);
			return WIFI_UTILS_FAIL;
		}
		wifi_init = true;
	}
	wifi_config_t wifi_config = {
		.ap = {
			.ssid_len = softap_config->ssid_length,
			.channel = softap_config->channel,
			.max_connection = CONFIG_MAX_STA_CONN,
			.authmode = mode,
			.beacon_interval = WIFI_DEFAULT_BEACON_INTERVAL,
		}
		,
	};
	strncpy(wifi_config.ap.ssid, softap_config->ssid, softap_config->ssid_length);
	strncpy(wifi_config.ap.password, softap_config->passphrase, softap_config->passphrase_length);

	ret = esp_wifi_set_mode(WIFI_MODE_AP);
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_set_mode failed, %d\n", ret);
		return;
	}

	ret = esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config);
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_set_config failed, %d\n", ret);
		return;
	}

	ret = esp_wifi_start();
	if (ret != ESP_OK) {
		return;
	}

	s_retry_num = -1;
	g_mode = WIFI_SOFT_AP_IF;

	return ret;
}

wifi_utils_result_e wifi_utils_start_sta(void)
{
	esp_err_t ret;
	wifi_config_t wifi_config = {
		.sta = {
			.ssid = DEFAULT_SSID,
			.password = DEFAULT_PWD,
			.scan_method = DEFAULT_SCAN_METHOD,
			.sort_method = DEFAULT_SORT_METHOD,
			.threshold.rssi = DEFAULT_RSSI,
			.threshold.authmode = DEFAULT_AUTHMODE,
		},
	};

	if (g_mode == WIFI_SOFT_AP_IF) {
		ret = wifi_utils_stop_softap();
		if (ret != ESP_OK) {
			ESP_LOGI(TAG, "wifi_utils_stop_softap failed, %d\n", ret);
			return WIFI_UTILS_FAIL;
		}
		usleep(100000);
	}
	ret = esp_wifi_set_mode(WIFI_MODE_STA);
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_set_mode failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}
	ret = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_set_config failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}

	ret = esp_wifi_start();
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_start failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}
	g_mode = WIFI_STATION_IF;
	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_stop_sta(void)
{
	esp_err_t ret = WIFI_UTILS_SUCCESS;

	ret = esp_wifi_disconnect();
	if (ret != ESP_OK && ret != ESP_ERR_WIFI_NOT_INIT && ret != ESP_ERR_WIFI_NOT_STARTED) {
		ESP_LOGI(TAG, "esp_wifi_disconnect failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}

	ret = esp_wifi_stop();
	if (ret != ESP_OK && ret != ESP_ERR_WIFI_NOT_INIT) {
		ESP_LOGI(TAG, "esp_wifi_stop failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}
	g_mode = WIFI_NONE;
	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_stop_softap(void)
{
	esp_err_t ret = WIFI_UTILS_SUCCESS;

	if (isAPJoined) {
		ret = esp_wifi_deauth_sta(0);
		if (ret != ESP_OK) {
			ESP_LOGI(TAG, "esp_wifi_deauth_sta failed, %d\n", ret);
			return WIFI_UTILS_FAIL;
		}
		isAPJoined = 0;
	}
	ret = esp_wifi_stop();
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_stop failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}
	g_mode = WIFI_NONE;
	return ret;
}

wifi_utils_result_e wifi_utils_set_autoconnect(uint8_t check)
{
	esp_err_t ret = WIFI_UTILS_SUCCESS;
	ret = esp_wifi_set_auto_connect(check);
	if (ret != ESP_OK) {
		ESP_LOGI(TAG, "esp_wifi_set_auto_connect failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}
	return ret;
}
