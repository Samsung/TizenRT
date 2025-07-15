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

#ifndef _WIFI_MANAGER_INFO_H__
#define _WIFI_MANAGER_INFO_H__

#define WIFIMGR_SSID 0x01
#define WIFIMGR_SOFTAP_SSID 0x02
#define WIFIMGR_RSSI 0x08
#define WIFIMGR_STATE 0X10
#define WIFIMGR_ALL_INFO (WIFIMGR_SSID | WIFIMGR_SOFTAP_SSID | WIFIMGR_RSSI | WIFIMGR_STATE)

struct wifimgr_info_msg {
	char *ssid;
	char *softap_ssid;
	int rssi;
	wifimgr_state_e state;
	char bssid[WIFIMGR_MACADDR_LEN]; /**< bssid is a mac address of AP */
};
typedef struct wifimgr_info_msg wifimgr_info_msg_s;

struct wifimgr_signal_quality {
	unsigned int channel;
	signed char snr;
	int network_bw;
	unsigned int max_rate;
};
typedef struct wifimgr_signal_quality wifimgr_signal_quality_s;

struct wifimgr_wpa_states {
	int wpa_supplicant_state;
	int wpa_supplicant_key_mgmt;
};
typedef struct wifimgr_wpa_states wifimgr_wpa_states_s;

struct wifimgr_driver_info {
	char lib_version[64];
};
typedef struct wifimgr_driver_info wifimgr_driver_info_s;

int wifimgr_get_info(int flag, wifimgr_info_msg_s *info);
int wifimgr_set_info(int flag, wifimgr_info_msg_s *info);
int wifimgr_set_channel_plan(uint8_t channel_plan);
int wifimgr_get_signal_quality(wifimgr_signal_quality_s *signal_quality);
int wifimgr_get_disconnect_reason(int *disconnect_reason);
int wifimgr_get_driver_info(wifimgr_driver_info_s *driver_info);
int wifimgr_get_wpa_supplicant_state(wifimgr_wpa_states_s *wpa_state);

#endif // _WIFI_MANAGER_INFO_H__
