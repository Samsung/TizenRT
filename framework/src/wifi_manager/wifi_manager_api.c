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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wifi_manager/wifi_manager.h>
#include <tinyara/net/if/wifi.h>
#include "wifi_manager_error.h"
#include "wifi_manager_utils.h"
#include "wifi_manager_dhcp.h"
#include "wifi_manager_event.h"
#include "wifi_manager_msghandler.h"
#include "wifi_manager_stats.h"
#include "wifi_manager_state.h"
#include "wifi_manager_cb.h"
#include "wifi_manager_info.h"
#include "wifi_manager_profile.h"

/*  Check Result MACRO */
#define WIFIMGR_CHECK_AP_CONFIG(config)                                                                                         \
	do {                                                                                                                        \
		if (config->ssid_length > WIFIMGR_SSID_LEN || strlen(config->ssid) > WIFIMGR_SSID_LEN) {                                \
			NET_LOGE(TAG, "AP configuration fails: too long ssid\n");                                                           \
			NET_LOGE(TAG, "Make sure that length of SSID < 33\n");                                                              \
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);                                                                  \
			return WIFI_MANAGER_INVALID_ARGS;                                                                                   \
		} else if (config->passphrase_length > WIFIMGR_PASSPHRASE_LEN || strlen(config->passphrase) > WIFIMGR_PASSPHRASE_LEN) { \
			NET_LOGE(TAG, "AP configuration fails: too long passphrase\n");                                                     \
			NET_LOGE(TAG, "Make sure that length of passphrase < 65\n");                                                        \
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);                                                                  \
			return WIFI_MANAGER_INVALID_ARGS;                                                                                   \
		}                                                                                                                       \
	} while (0)

#define RETURN_RESULT(res, msg)            \
	do {                                   \
		if (res < 0) {                     \
			return WIFI_MANAGER_POST_FAIL; \
		} else {                           \
			return msg.result;             \
		}                                  \
	} while (0)
#define TAG "[WM]"

static inline void _convert_state(wifimgr_state_e *state, connect_status_e *conn, wifi_manager_mode_e *mode)
{
	*conn = STATUS_UNKNOWN;
	*mode = WIFI_NONE;
	switch (*state) {
	case WIFIMGR_STA_DISCONNECTED:
		*conn = AP_DISCONNECTED;
		*mode = STA_MODE;
		break;
	case WIFIMGR_STA_DISCONNECTING:
		*conn = AP_DISCONNECTED;
		*mode = STA_MODE;
		break;
	case WIFIMGR_STA_CONNECTING:
		*conn = AP_DISCONNECTED;
		*mode = STA_MODE;
		break;
	case WIFIMGR_STA_CONNECTED:
		*conn = AP_CONNECTED;
		*mode = STA_MODE;
		break;
	case WIFIMGR_SOFTAP:
		// Todo: wifi manager doesn't provide join info in softap *mode
		// so set unknown state to *conn;
		*conn = STATUS_UNKNOWN;
		*mode = SOFTAP_MODE;
		break;
	case WIFIMGR_UNINITIALIZED:
		break;
	default:
		NET_LOGE(TAG, "invalid state\n");
		break;
	}
}
/**
 * public APIs
 */
wifi_manager_result_e wifi_manager_init(wifi_manager_cb_s *wmcb)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (!wmcb) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}

	wifimgr_msg_s msg = {WIFIMGR_CMD_INIT, WIFI_MANAGER_FAIL, (void *)wmcb, NULL};
	RETURN_RESULT(wifimgr_post_message(&msg), msg);
}

wifi_manager_result_e wifi_manager_deinit(void)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	sem_t signal;
	sem_init(&signal, 0, 0);
	wifimgr_msg_s msg = {WIFIMGR_CMD_DEINIT, WIFI_MANAGER_FAIL, NULL, &signal};

	int res = wifimgr_post_message(&msg);
	if (res < 0 || msg.result != WIFI_MANAGER_SUCCESS) {
		NET_LOGE(TAG, "post message fail %d %d\n", res, msg.result);
		sem_destroy(msg.signal);
		if (res < 0) {
			return WIFI_MANAGER_FAIL;
		}
		return msg.result;
	}
	// if wifi is connected state. then it should wait until it disconnects to AP
	sem_wait(msg.signal);
	sem_destroy(msg.signal);

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_set_mode(wifi_manager_mode_e mode, wifi_manager_softap_config_s *config)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (mode != STA_MODE && mode != SOFTAP_MODE) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}

	if (mode == SOFTAP_MODE && !config) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}

	sem_t signal;
	sem_init(&signal, 0, 0);
	wifimgr_msg_s msg = {WIFIMGR_CMD_SET_STA, WIFI_MANAGER_FAIL, NULL, &signal};
	if (mode == SOFTAP_MODE) {
		msg.event = WIFIMGR_CMD_SET_SOFTAP;
		msg.param = (void *)config;
	}

	int res = wifimgr_post_message(&msg);
	if (res < 0 || msg.result != WIFI_MANAGER_SUCCESS) {
		sem_destroy(msg.signal);
		if (res < 0) {
			return WIFI_MANAGER_FAIL;
		}
		return msg.result;
	}
	// if wifi is connected state. then it should wait until it disconnects to AP
	sem_wait(msg.signal);
	sem_destroy(msg.signal);

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_connect_ap(wifi_manager_ap_config_s *config)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (!config) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}

	WIFIMGR_CHECK_AP_CONFIG(config);
	wifimgr_msg_s msg = {WIFIMGR_CMD_CONNECT, WIFI_MANAGER_FAIL, (void *)config, NULL};

	RETURN_RESULT(wifimgr_post_message(&msg), msg);
}

wifi_manager_result_e wifi_manager_disconnect_ap(void)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	wifimgr_msg_s msg = {WIFIMGR_CMD_DISCONNECT, WIFI_MANAGER_FAIL, NULL, NULL};
	RETURN_RESULT(wifimgr_post_message(&msg), msg);
}

// Valid channel list is from https://www.wirelesstrainingsolutions.com/new-spectrum/ by realtek engineer
static int valid_ch_list[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 149, 153, 157, 161, 165};
static int valid_ch_list_size = sizeof(valid_ch_list) / sizeof(valid_ch_list[0]);

wifi_manager_result_e wifi_manager_scan_ap(wifi_manager_scan_config_s *config)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (config) {
		int ch_valid = 0;
		for (int i = 0; i < valid_ch_list_size; i++) {
			if (config->channel == valid_ch_list[i]) {
				ch_valid = 1;
				break;
			}
		}
		if (!ch_valid) {
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
			NET_LOGE(TAG, "invalid channel range %d\n", config->channel);
			return WIFI_MANAGER_INVALID_ARGS;
		}
		if (config->ssid_length > WIFIMGR_SSID_LEN) {
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
			return WIFI_MANAGER_INVALID_ARGS;
		}
		if (config->ssid_length == 0 && config->channel == 0) {
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
			return WIFI_MANAGER_INVALID_ARGS;
		}
	}
	wifimgr_msg_s msg = {WIFIMGR_CMD_SCAN, WIFI_MANAGER_FAIL, (void *)config, NULL};
	RETURN_RESULT(wifimgr_post_message(&msg), msg);
}

wifi_manager_result_e wifi_manager_scan_multi_aps(wifi_manager_scan_multi_configs_s *configs)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (configs) {
		for (int i = 0; i < configs->scan_ap_config_count; i++) {
			wifi_manager_scan_config_s *config = &configs->ap_configs[i];
			if (config->ssid_length > WIFIMGR_SSID_LEN) {
				WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
				return WIFI_MANAGER_INVALID_ARGS;
			}
			if (config->ssid_length == 0 && config->channel == 0) {
				WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
				return WIFI_MANAGER_INVALID_ARGS;
			}
		}
	}
	wifimgr_msg_s msg = {WIFIMGR_CMD_SCAN_MULTI_APS, WIFI_MANAGER_FAIL, (void *)configs, NULL};
	RETURN_RESULT(wifimgr_post_message(&msg), msg);
}

wifi_manager_result_e wifi_manager_scan_specific_ap(wifi_manager_ap_config_s *config)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	return WIFI_MANAGER_NO_API;
}

/**
 * Wi-Fi Stats
 */
wifi_manager_result_e wifi_manager_get_info(wifi_manager_info_s *info)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (info == NULL) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}
	char softap_ssid[WIFIMGR_SSID_LEN + 1];
	wifimgr_info_msg_s winfo = {info->ssid, softap_ssid, 0, WIFIMGR_UNINITIALIZED, {
																					   0,
																				   }};
	wifimgr_get_info(WIFIMGR_ALL_INFO, &winfo);
	info->rssi = winfo.rssi;
	memcpy(info->bssid, winfo.bssid, WIFIMGR_MACADDR_LEN);
	_convert_state(&winfo.state, &info->status, &info->mode);

	// wifi manager stores softap ssid and ap ssid connected
	// so it should be converted by wifi mode
	if (info->mode == SOFTAP_MODE) {
		memcpy(info->ssid, softap_ssid, WIFIMGR_SSID_LEN + 1);
	}

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_get_ap_type(wifi_manager_ap_type_e *aptype)
{
	NET_LOGI(TAG, "-->\n");
	if (aptype == NULL) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}

	wifi_manager_info_s info;
	wifi_manager_get_info(&info);
	if (info.status == AP_DISCONNECTED) {
		NET_LOGE(TAG, "AP disconnected, cannot get connected ap type\n");
		return WIFI_MANAGER_FAIL;
	}

	if (dhcpc_get_ap_type(aptype) != WIFI_MANAGER_SUCCESS) {
		return WIFI_MANAGER_FAIL;
	}

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_set_channel_plan(uint8_t channel_plan)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);

	if (wifimgr_set_channel_plan(channel_plan) != 0) {
		return WIFI_MANAGER_FAIL;
	}

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_get_signal_quality(wifi_manager_signal_quality_s *wifi_manager_signal_quality)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (wifi_manager_signal_quality == NULL) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}
	wifimgr_signal_quality_s wifi_signal_quality = {
		0,
	};
	wifimgr_get_signal_quality(&wifi_signal_quality);

	wifi_manager_signal_quality->channel = wifi_signal_quality.channel;
	memcpy(&(wifi_manager_signal_quality->snr), &(wifi_signal_quality.snr), sizeof(signed char));
	wifi_manager_signal_quality->network_bw = wifi_signal_quality.network_bw;
	wifi_manager_signal_quality->max_rate = wifi_signal_quality.max_rate;

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_get_disconnect_reason(int *disconnect_reason)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (disconnect_reason == NULL) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}

	wifimgr_get_disconnect_reason(disconnect_reason);

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_get_driver_info(wifi_manager_driver_info_s *wifi_manager_driver_info)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (wifi_manager_driver_info == NULL) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}
	wifimgr_driver_info_s driver_info = {
		0,
	};
	wifimgr_get_driver_info(&driver_info);

	memcpy(wifi_manager_driver_info->lib_version, driver_info.lib_version, sizeof(driver_info.lib_version));

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_get_wpa_supplicant_state(wifi_manager_wpa_states_s *wifi_manager_wpa_state)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (wifi_manager_wpa_state == NULL) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}
	wifimgr_wpa_states_s wpa_state = {
		0,
	};
	wifimgr_get_wpa_supplicant_state(&wpa_state);

	wifi_manager_wpa_state->wpa_supplicant_state = wpa_state.wpa_supplicant_state;
	wifi_manager_wpa_state->wpa_supplicant_key_mgmt = wpa_state.wpa_supplicant_key_mgmt;

	return WIFI_MANAGER_SUCCESS;
}
wifi_manager_result_e wifi_manager_get_stats(wifi_manager_stats_s *stats)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (!stats) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}
	memset(stats, 0, sizeof(wifi_manager_stats_s));
	wifimgr_get_stats(stats);

	wifimgr_msg_s msg = {WIFIMGR_CMD_GETSTATS, WIFI_MANAGER_FAIL, (void *)stats, NULL};
	RETURN_RESULT(wifimgr_post_message(&msg), msg);
}

wifi_manager_result_e wifi_manager_set_powermode(wifi_manager_powermode_e mode)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	wifimgr_msg_s msg = {WIFIMGR_CMD_SETPOWER, WIFI_MANAGER_FAIL, (void *)&mode, 0};
	RETURN_RESULT(wifimgr_post_message(&msg), msg);
}

/**
 * Wi-Fi callback
 */
wifi_manager_result_e wifi_manager_register_cb(wifi_manager_cb_s *wmcb)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	int res = wifimgr_register_cb(wmcb);
	if (res < 0) {
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_unregister_cb(wifi_manager_cb_s *wmcb)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (!wmcb) {
		return WIFI_MANAGER_INVALID_ARGS;
	}

	int res = wifimgr_unregister_cb(wmcb);
	if (res < 0) {
		return WIFI_MANAGER_FAIL;
	}

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_set_hostname_sta(const char *hostname)
{
	dhcp_client_sethostname(WIFIMGR_STA_IFNAME, hostname);
	return WIFI_MANAGER_SUCCESS;
}

#if defined(CONFIG_ENABLE_HOMELYNK) && (CONFIG_ENABLE_HOMELYNK == 1)
wifi_manager_result_e wifi_manager_control_bridge(uint8_t enable)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	wifimgr_msg_s msg = {WIFIMGR_CMD_SET_BRIDGE, WIFI_MANAGER_FAIL, (void *)&enable, 0};
	RETURN_RESULT(wifimgr_post_message(&msg), msg);
}
#endif

/**
 * Wi-Fi Profile
 */
#ifdef CONFIG_WIFI_MANAGER_SAVE_CONFIG
wifi_manager_result_e wifi_manager_save_config(wifi_manager_ap_config_s *config)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (!config) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
	}

	WIFIMGR_CHECK_AP_CONFIG(config);
	WIFIMGR_CHECK_UTILRESULT(wifi_profile_write(config, 0), TAG, "wifimgr save config fail");

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_get_config(wifi_manager_ap_config_s *config)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (!config) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
	}

	WIFIMGR_CHECK_UTILRESULT(wifi_profile_read(config, 0), TAG, "wifimgr get config fail");

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_remove_config(void)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	WIFIMGR_CHECK_UTILRESULT(wifi_profile_reset(0), TAG, "wifimgr remove config fail");
	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_get_connected_config(wifi_manager_ap_config_s *config)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	if (!config) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
	}
	WIFIMGR_CHECK_UTILRESULT(wifi_profile_read(config, 1), TAG, "wifimgr get config fail");

	return WIFI_MANAGER_SUCCESS;
}
#else
wifi_manager_result_e wifi_manager_save_config(wifi_manager_ap_config_s *config)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	return WIFI_MANAGER_NO_API;
}

wifi_manager_result_e wifi_manager_get_config(wifi_manager_ap_config_s *config)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	return WIFI_MANAGER_NO_API;
}

wifi_manager_result_e wifi_manager_remove_config(void)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	return WIFI_MANAGER_NO_API;
}

wifi_manager_result_e wifi_manager_get_connected_config(wifi_manager_ap_config_s *config)
{
	NET_LOGI(TAG, "--> %s %d\n", __FUNCTION__, __LINE__);
	return WIFI_MANAGER_NO_API;
}
#endif // CONFIG_WIFI_MANAGER_SAVE_CONFIG
