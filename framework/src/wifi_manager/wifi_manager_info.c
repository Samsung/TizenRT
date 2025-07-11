/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#include <stdint.h>
#include <string.h>
#include <time.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/net/netlog.h>
#include <wifi_manager/wifi_manager.h>
#include "wifi_manager_state.h"
#include "wifi_manager_info.h"
#include "wifi_manager_lwnl.h"

#define TAG "[WM]"

struct _wifimgr_info {
	char ssid[WIFIMGR_SSID_LEN + 1];		// SSID of Connected AP if mode is a station
	char softap_ssid[WIFIMGR_SSID_LEN + 1]; // SoftAP SSID if mode is a soft ap
	int rssi;								// It is only used for a station mode
	wifimgr_state_e state;
	wifimgr_state_e prev_state;
};
typedef struct _wifimgr_info _wifimgr_info_s;

static _wifimgr_info_s g_wifi_info = {{0}, {0}, 0, WIFIMGR_NONE, WIFIMGR_NONE};

int wifimgr_get_info(int flag, wifimgr_info_msg_s *info)
{
	if ((flag & WIFIMGR_SSID) != 0) {
		strncpy(info->ssid, g_wifi_info.ssid, WIFIMGR_SSID_LEN);
		info->ssid[WIFIMGR_SSID_LEN] = '\0';
	}

	if ((flag & WIFIMGR_SOFTAP_SSID) != 0) {
		strncpy(info->softap_ssid, g_wifi_info.softap_ssid, WIFIMGR_SSID_LEN);
		info->softap_ssid[WIFIMGR_SSID_LEN] = '\0';
	}

	if ((flag & WIFIMGR_RSSI) != 0) {
		trwifi_info info_utils;
		trwifi_result_e wres = wifi_utils_get_info(&info_utils);
		if (wres == TRWIFI_FAIL) {
			return -1;
		}
		info->rssi = info_utils.rssi;
		memcpy(info->bssid, info_utils.bssid, WIFIMGR_MACADDR_LEN);
	}

	if ((flag & WIFIMGR_STATE) != 0) {
		if (g_wifi_info.state == WIFIMGR_SCANNING) {
			info->state = g_wifi_info.prev_state;
		} else {
			info->state = g_wifi_info.state;
		}
	}

	return 0;
}

int wifimgr_set_info(int flag, wifimgr_info_msg_s *info)
{
	if ((flag & WIFIMGR_SSID) != 0) {
		strncpy(g_wifi_info.ssid, info->ssid, WIFIMGR_SSID_LEN);
		g_wifi_info.ssid[WIFIMGR_SSID_LEN] = '\0';
	}

	if ((flag & WIFIMGR_SOFTAP_SSID) != 0) {
		strncpy(g_wifi_info.softap_ssid, info->softap_ssid, WIFIMGR_SSID_LEN);
		g_wifi_info.softap_ssid[WIFIMGR_SSID_LEN] = '\0';
	}

	if ((flag & WIFIMGR_RSSI) != 0) {
		g_wifi_info.rssi = info->rssi;
	}

	if ((flag & WIFIMGR_STATE) != 0) {
		g_wifi_info.prev_state = g_wifi_info.state;
		g_wifi_info.state = info->state;
	}
	return 0;
}

int wifimgr_set_channel_plan(uint8_t channel_plan)
{
	trwifi_result_e wres = wifi_utils_set_channel_plan(channel_plan);
	if (wres == TRWIFI_FAIL) {
		return -1;
	}

	return 0;
}

int wifimgr_get_signal_quality(wifimgr_signal_quality_s *signal_quality)
{
	trwifi_signal_quality signal_quality_utils;

	trwifi_result_e wres = wifi_utils_get_signal_quality(&signal_quality_utils);
	if (wres == TRWIFI_FAIL) {
		return -1;
	}

	signal_quality->channel = signal_quality_utils.channel;
	memcpy(&(signal_quality->snr), &(signal_quality_utils.snr), sizeof(signed char));
	signal_quality->network_bw = signal_quality_utils.network_bw;
	signal_quality->max_rate = signal_quality_utils.max_rate;

	return 0;
}

int wifimgr_get_disconnect_reason(int *disconnect_reason)
{
	trwifi_result_e wres = wifi_utils_get_disconnect_reason(disconnect_reason);
	if (wres == TRWIFI_FAIL) {
		return -1;
	}

	return 0;
}

int wifimgr_get_driver_info(wifimgr_driver_info_s *driver_info)
{
	trwifi_driver_info driver_info_utils;
	trwifi_result_e wres = wifi_utils_get_driver_info(&driver_info_utils);
	if (wres == TRWIFI_FAIL) {
		return -1;
	}
	memcpy(driver_info->lib_version, driver_info_utils.lib_version, sizeof(driver_info_utils.lib_version));

	return 0;
}

int wifimgr_get_wpa_supplicant_state(wifimgr_wpa_states_s *wpa_state)
{
	trwifi_wpa_states wpa_state_utils;

	trwifi_result_e wres = wifi_utils_get_wpa_supplicant_state(&wpa_state_utils);

	wpa_state->wpa_supplicant_state = wpa_state_utils.wpa_supplicant_state;
	if (wres == TRWIFI_FAIL) {
		return -1;
	}

	wpa_state->wpa_supplicant_key_mgmt = wpa_state_utils.wpa_supplicant_key_mgmt;

	return 0;
}
