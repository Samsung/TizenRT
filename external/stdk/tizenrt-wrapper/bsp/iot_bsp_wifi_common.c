/* ***************************************************************************
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
#include <string.h>
#include <tinyara/time.h>
#include <wifi_manager/wifi_manager.h>
#include "iot_debug.h"
#include "iot_bsp_wifi.h"
#include "iot_os_util.h"
#include "iot_util.h"
#include "event_groups.h"
#ifdef CONFIG_ARCH_BOARD_ESP32_FAMILY
#include <tinyara/wifi/wifi_utils.h>
#endif
#ifdef CONFIG_NETUTILS_NTPCLIENT
#include <protocols/ntpclient.h>
#endif

#define BIT2     0x00000004
#define BIT1     0x00000002
#define BIT0     0x00000001
#define portMAX_DELAY (0xffffffff)

const int WIFI_CONNECTED_BIT = BIT0;
const int WIFI_SCAN_DONE_BIT = BIT1;
const int WIFI_ON_BIT = BIT2;
static int WIFI_INITIALIZED = false;
static event_group_handle_t wifi_event_group;
static struct iot_mac bsp_mac;

typedef struct {
	uint16_t ap_num;
	wifi_manager_scan_info_s wifi_scan_result[IOT_WIFI_MAX_SCAN_RESULT];
}t_wifi_manager_scan_result;

static t_wifi_manager_scan_result wifi_manager_scan_result;

#ifdef CONFIG_NETUTILS_NTPCLIENT
static struct ntpc_server_conn_s server_conn[] = {
	{CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER, CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER_PORT},
	{"pool.ntp.org", CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER_PORT},
	{"1.kr.pool.ntp.org", CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER_PORT},
	{"1.asia.pool.ntp.org", CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER_PORT},
	{"us.pool.ntp.org", CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER_PORT},
};

static void ntp_link_error(void)
{
	IOT_ERROR("ntp_link_error() callback is called\n");
}
#endif

static bool _initialize_ntp(void)
{
	IOT_INFO("Initializing NTP");

#ifdef CONFIG_NETUTILS_NTPCLIENT
	if (ntpc_get_status() == NTP_RUNNING) {
		IOT_WARN("ntpc has been already running\n");
		return true;
	}

	struct timespec init_tp;
	init_tp.tv_sec = 0;
	init_tp.tv_nsec = 0;
	clock_settime(CLOCK_REALTIME, &init_tp);

	if (ntpc_start(server_conn, sizeof(server_conn) / sizeof(server_conn[0]), 0, (void *)ntp_link_error) < 0) {
		IOT_ERROR("ntpc_start() failed\n");
		return false;
	}

	IOT_INFO("ntpc_start() succeed\n");
	return true;
#else
#error "CONFIG_NETUTILS_NTPCLIENT not set"
#endif
}

static void _obtain_time(void)
{
	time_t now = 0;
	struct tm timeinfo = { 0 };
	int retry = 0;
	const int retry_count = 10;

	event_group_wait_bits(wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);

	if (!_initialize_ntp()) {
		IOT_ERROR("NTP initialize failed, can not obtain time!");
		return;
	}

	// wait for time to be set, system time initialized in _initialize_ntp
	while (timeinfo.tm_year <= (EPOCH_YEAR - TM_YEAR_BASE) && ++retry < retry_count) {
		IOT_INFO("Waiting for system time to be set... (%d/%d)", retry, retry_count);
		IOT_DELAY(3000);
		time(&now);
		localtime_r(&now, &timeinfo);
	}

	if (retry < retry_count) {
		IOT_INFO("[WIFI] system time updated by %ld", now);
	} else {
		IOT_ERROR("[WIFI] system time has not been synced yet!");
	}
}

void bsp_wm_sta_connected(wifi_manager_result_e res)
{
	IOT_INFO(" T%d --> %s res(%d)\n", getpid(), __FUNCTION__, res);
	event_group_set_bits(wifi_event_group, WIFI_CONNECTED_BIT);
}

void bsp_wm_sta_disconnected(wifi_manager_disconnect_e disconn)
{
	sleep(2);
	IOT_INFO(" T%d --> %s\n", getpid(), __FUNCTION__);
}

void bsp_wm_softap_sta_join(void)
{
	IOT_INFO(" T%d --> %s\n", getpid(), __FUNCTION__);
}

void bsp_wm_softap_sta_leave(void)
{
	IOT_INFO(" T%d --> %s\n", getpid(), __FUNCTION__);
}

void bsp_wm_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res)
{
	IOT_INFO(" T%d --> %s\n", getpid(), __FUNCTION__);
	/* Make sure you copy the scan results onto a local data structure.
	* It will be deleted soon eventually as you exit this function.
	*/
	if (scan_result == NULL) {
		event_group_set_bits(wifi_event_group, WIFI_SCAN_DONE_BIT);
		return;
	}

	memset(&wifi_manager_scan_result, 0 ,sizeof(wifi_manager_scan_result));

	wifi_manager_scan_info_s *wifi_scan_iter = *scan_result;
	while (wifi_scan_iter != NULL) {
		IOT_DEBUG("WiFi AP SSID: %-25s, BSSID: %-20s, Rssi: %d, Auth: %d, Crypto: %d",
			   wifi_scan_iter->ssid, wifi_scan_iter->bssid, wifi_scan_iter->rssi,
			   wifi_scan_iter->ap_auth_type, wifi_scan_iter->ap_crypto_type);

		memcpy(&wifi_manager_scan_result.wifi_scan_result[wifi_manager_scan_result.ap_num], wifi_scan_iter, sizeof(wifi_manager_scan_info_s));
		wifi_manager_scan_result.ap_num++;
		if (wifi_manager_scan_result.ap_num >= IOT_WIFI_MAX_SCAN_RESULT) {
			break;
		}
		wifi_scan_iter = wifi_scan_iter->next;
	}
	event_group_set_bits(wifi_event_group, WIFI_SCAN_DONE_BIT);
}

/*
 * Global
 */
static wifi_manager_cb_s _wifi_callbacks = {
	bsp_wm_sta_connected,
	bsp_wm_sta_disconnected,
	bsp_wm_softap_sta_join,
	bsp_wm_softap_sta_leave,
	bsp_wm_scan_done,
};

iot_error_t iot_bsp_wifi_init()
{
	if (WIFI_INITIALIZED) {
		return IOT_ERROR_NONE;
	}

	wifi_event_group = event_group_create();

	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	res = wifi_manager_init(&_wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		IOT_INFO(" wifi_manager_init fail\n");
		return IOT_ERROR_INIT_FAIL;
	}

	/* get mac once after wm init, avoid getting mac during wifi provisioning,
	* which would cause a dead lock issue */
	struct iot_mac init_mac;
	iot_bsp_wifi_get_mac(&init_mac);

	WIFI_INITIALIZED = true;

	return IOT_ERROR_NONE;
}

int _iot_bsp_sta_mode(void)
{
#ifdef CONFIG_ARCH_BOARD_ESP32_FAMILY
	wifi_utils_info_s info;
	wifi_utils_get_info(&info);
	return (info.wifi_status == WIFI_UTILS_SOFTAP_MODE) ? 0 : 1;
#else
	wifi_manager_info_s info;
	wifi_manager_get_info(&info);
	return (info.mode == STA_MODE) ? 1 : 0;
#endif
}

iot_error_t iot_bsp_wifi_set_mode(iot_wifi_conf *conf)
{
	int str_len = 0;
	wifi_manager_result_e res;
	wifi_manager_softap_config_s sap_config;
	wifi_manager_ap_config_s apconfig;

	switch(conf->mode) {
		case IOT_WIFI_MODE_OFF:
			//esp_wifi_set_mode(WIFI_MODE_NULL);
		break;
		case IOT_WIFI_MODE_SCAN:
			if (_iot_bsp_sta_mode() == 0) {
				res = wifi_manager_set_mode(STA_MODE, NULL);
				if (res != WIFI_MANAGER_SUCCESS) {
					IOT_INFO(" Set STA mode Fail\n");
					return IOT_ERROR_INIT_FAIL;
				}
			}

			res = wifi_manager_scan_ap();
			if (res != WIFI_MANAGER_SUCCESS) {
				IOT_INFO(" scan Fail\n");
				return IOT_ERROR_READ_FAIL;
			}
		break;

		case IOT_WIFI_MODE_STATION:
			if (_iot_bsp_sta_mode() == 0) {
				res = wifi_manager_set_mode(STA_MODE, NULL);
				if (res != WIFI_MANAGER_SUCCESS) {
					IOT_INFO(" Set STA mode Fail\n");
					return IOT_ERROR_INIT_FAIL;
				}
			}

			IOT_INFO("Start STA mode\n");

			// Info the task STA is on.
			event_group_set_bits(wifi_event_group, WIFI_ON_BIT);

			//Connect to AP
			memset(&apconfig, 0 , sizeof(wifi_manager_ap_config_s));
			apconfig.ssid_length = strlen(conf->ssid);
			strncpy(apconfig.ssid, conf->ssid, apconfig.ssid_length);
			apconfig.ssid[apconfig.ssid_length + 1] = '\0';

			apconfig.passphrase_length = strlen(conf->pass);
			if (apconfig.passphrase_length) {
				strncpy(apconfig.passphrase, conf->pass, apconfig.passphrase_length);
				apconfig.passphrase[apconfig.passphrase_length + 1] = '\0';
				apconfig.ap_auth_type = WIFI_MANAGER_AUTH_UNKNOWN;
				apconfig.ap_crypto_type = WIFI_MANAGER_CRYPTO_UNKNOWN;
			} else {
				apconfig.passphrase[0] = '\0';
				apconfig.passphrase_length = 0;
				apconfig.ap_auth_type = WIFI_MANAGER_AUTH_OPEN;
				apconfig.ap_crypto_type = WIFI_MANAGER_CRYPTO_NONE;
			}

			res = wifi_manager_connect_ap(&apconfig);
			if (res != WIFI_MANAGER_SUCCESS) {
				IOT_INFO(" AP connect failed\n");
				return IOT_ERROR_INIT_FAIL;
			}

			/* Wait for DHCP connection */
			event_group_wait_bits(wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);

			IOT_DEBUG("connect to ap SSID:%s password:%s", apconfig.ssid, apconfig.passphrase);

			IOT_INFO("Time is not set yet. Connecting to WiFi and getting time over NTP.");
			_obtain_time();
		break;
		case IOT_WIFI_MODE_SOFTAP:
			memset(&sap_config, 0, sizeof(wifi_manager_softap_config_s));
			str_len = strlen(conf->ssid);
			strncpy(sap_config.ssid, conf->ssid, str_len);
			sap_config.ssid[str_len + 1] = '\0';

			str_len =  strlen(conf->pass);
			strncpy(sap_config.passphrase, conf->pass, str_len);
			sap_config.passphrase[str_len + 1] = '\0';
			sap_config.channel = 1;

			res = wifi_manager_set_mode(SOFTAP_MODE, &sap_config);
			if (res != WIFI_MANAGER_SUCCESS) {
				IOT_INFO(" Run SoftAP Fail\n");
			}

			IOT_DEBUG("wifi_init_softap finished.SSID:%s password:%s",
						sap_config.ssid, sap_config.passphrase);
		break;
		default:
			IOT_ERROR("iot bsp wifi can't support this mode = %d", conf->mode);
			return IOT_ERROR_INIT_FAIL;
	}

	return IOT_ERROR_NONE;
}

uint16_t iot_bsp_wifi_get_scan_result(iot_wifi_scan_result_t *scan_result)
{
	uint16_t ap_num;
	uint16_t i;

	//Wait scan done
	event_group_wait_bits(wifi_event_group, WIFI_SCAN_DONE_BIT, false, true, portMAX_DELAY);
	ap_num = wifi_manager_scan_result.ap_num;

	for(i = 0; i < ap_num; i++)	{
		memcpy(scan_result[i].ssid, wifi_manager_scan_result.wifi_scan_result[i].ssid, strlen((char *)wifi_manager_scan_result.wifi_scan_result[i].ssid));
		sscanf(wifi_manager_scan_result.wifi_scan_result[i].bssid, "%x:%x:%x:%x:%x:%x",
			&scan_result[i].bssid[0], &scan_result[i].bssid[1], &scan_result[i].bssid[2],
			&scan_result[i].bssid[3], &scan_result[i].bssid[4], &scan_result[i].bssid[5]);

		scan_result[i].rssi = wifi_manager_scan_result.wifi_scan_result[i].rssi;
		scan_result[i].freq = iot_util_convert_channel_freq(wifi_manager_scan_result.wifi_scan_result[i].channel);
		scan_result[i].authmode = wifi_manager_scan_result.wifi_scan_result[i].ap_auth_type;
	}

	return ap_num;
}

iot_error_t iot_bsp_wifi_get_mac(struct iot_mac *wifi_mac)
{
	int ret = -1;
	if(*((int *)bsp_mac.addr) == 0) {
#ifdef CONFIG_ARCH_BOARD_ESP32_FAMILY
		wifi_utils_info_s info;
		wifi_utils_result_e wret = wifi_utils_get_info(&info);
		ret = (wret == WIFI_UTILS_SUCCESS) ? 0 : -1;
#else
		wifi_manager_info_s info;
		wifi_manager_result_e res = wifi_manager_get_info(&info);
		ret = (res == WIFI_MANAGER_SUCCESS) ? 0 : -1;
#endif
		if (ret != 0) {
			IOT_INFO("Get info failed\n");
			return IOT_ERROR_READ_FAIL;
		}

		strncpy((char *)wifi_mac->addr, (const char *)info.mac_address, 6);
		bsp_mac = *wifi_mac;
	} else {
		*wifi_mac = bsp_mac;
	}
	
	return IOT_ERROR_NONE;
}

iot_wifi_freq_t iot_bsp_wifi_get_freq(void)
{
	return IOT_WIFI_FREQ_2_4G_ONLY;
}
