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
#include <tinyara/wifi/wifi_common.h>
#include <wifi_manager/wifi_manager.h>
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

#ifdef CONFIG_WIFIMGR_INTERNAL_AUTOCONNECT
#define WIFIMGR_DEFAULT_CONN_CONFIG {WIFI_RECONN_INTERVAL, 77, 128}
#else
#define WIFIMGR_DEFAULT_CONN_CONFIG {WIFI_RECONN_NONE, -1, -1}
#endif

/*  Check Result MACRO */
#define WIFIMGR_CHECK_AP_CONFIG(config)									\
	do {																\
		if (config->ssid_length > WIFIMGR_SSID_LEN ||					\
			config->passphrase_length > WIFIMGR_PASSPHRASE_LEN ||		\
			strlen(config->ssid) > WIFIMGR_SSID_LEN ||					\
			strlen(config->passphrase) > WIFIMGR_PASSPHRASE_LEN) {		\
			WM_LOG_ERROR("[WM] AP configuration fails: too long ssid or passphrase\n");	\
			WM_LOG_ERROR("[WM] Make sure that length of SSID < 33 and length of passphrase < 65\n"); \
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);			\
			return WIFI_MANAGER_INVALID_ARGS;						    \
		}																\
	} while (0)

static inline void _convert_state(wifimgr_state_e *state, connect_status_e *conn, wifi_manager_mode_e *mode)
{
	switch (*state) {
	case WIFIMGR_UNINITIALIZED:
		*conn = STATUS_UNKNOWN;
		*mode = WIFI_NONE;
		break;
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
	case WIFIMGR_STA_RECONNECT:
		*conn = AP_RECONNECTING;
		*mode = STA_MODE;
		break;
	case WIFIMGR_SOFTAP:
		// Todo: wifi manager doesn't provide join info in softap *mode
		// so set unknown state to *conn;
		*conn = STATUS_UNKNOWN;
		*mode = SOFTAP_MODE;
		break;
	default:
		WM_LOG_ERROR("[WM][err] invalid state\n");
		break;
	}
}

#define RETURN_RESULT(res, msg)					\
	do {										\
		if (res < 0) {							\
			return WIFI_MANAGER_FAIL;			\
		} else {								\
			return msg.result;					\
		}										\
	} while (0)

/**
 * public APIs
 */
wifi_manager_result_e wifi_manager_init(wifi_manager_cb_s *wmcb)
{
	if (!wmcb) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}

	wifimgr_msg_s msg = {EVT_INIT_CMD, WIFI_MANAGER_FAIL, (void *)wmcb, NULL};
	int res = wifimgr_post_message(&msg);

	RETURN_RESULT(res, msg);
}


wifi_manager_result_e wifi_manager_deinit(void)
{
	sem_t signal;
	sem_init(&signal, 0, 0);
	wifimgr_msg_s msg = {EVT_DEINIT_CMD, WIFI_MANAGER_FAIL, NULL, &signal};

	int res = wifimgr_post_message(&msg);
	if (res < 0 || msg.result != WIFI_MANAGER_SUCCESS) {
		WM_LOG_ERROR("[WM] post message fail %d %d%s:%d\n", res, msg.result, __FUNCTION__, __LINE__);
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
	wifimgr_msg_s msg = {EVT_SET_STA_CMD, WIFI_MANAGER_FAIL, NULL, &signal};
	if (mode == SOFTAP_MODE) {
		msg.event = EVT_SET_SOFTAP_CMD;
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


wifi_manager_result_e wifi_manager_connect_ap_config(wifi_manager_ap_config_s *config,
													 wifi_manager_reconnect_config_s *conn_config)
{
	if (!config || !conn_config) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}

	WIFIMGR_CHECK_AP_CONFIG(config);
	_wifimgr_conn_info_msg_s conninfo = {config, conn_config};
	wifimgr_msg_s msg = {EVT_CONNECT_CMD, WIFI_MANAGER_FAIL, &conninfo, NULL};

	int res = wifimgr_post_message(&msg);

	RETURN_RESULT(res, msg);
}


wifi_manager_result_e wifi_manager_connect_ap(wifi_manager_ap_config_s *config)
{
	if (!config) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}

	wifi_manager_reconnect_config_s conn_config = WIFIMGR_DEFAULT_CONN_CONFIG;
	return wifi_manager_connect_ap_config(config, &conn_config);
}


wifi_manager_result_e wifi_manager_disconnect_ap(void)
{
	wifimgr_msg_s msg = {EVT_DISCONNECT_CMD, WIFI_MANAGER_FAIL, NULL, NULL};
	int res = wifimgr_post_message(&msg);

	RETURN_RESULT(res, msg);
}


wifi_manager_result_e wifi_manager_scan_ap(void)
{
	wifimgr_msg_s msg = {EVT_SCAN_CMD, WIFI_MANAGER_FAIL, NULL, NULL};
	int res = wifimgr_post_message(&msg);

	RETURN_RESULT(res, msg);
}

wifi_manager_result_e wifi_manager_scan_specific_ap(wifi_manager_ap_config_s *config)
{
	wifimgr_msg_s msg = {EVT_SCAN_CMD, WIFI_MANAGER_FAIL, config, NULL};
	int res = wifimgr_post_message(&msg);

	RETURN_RESULT(res, msg);
}

/**
 * Wi-Fi Stats
 */
wifi_manager_result_e wifi_manager_get_info(wifi_manager_info_s *info)
{
	if (info == NULL) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}

	// get ip address
	struct in_addr ip_ref;
	wifi_manager_result_e res = dhcpc_fetch_ipaddr(&ip_ref);
	if (res != WIFI_MANAGER_SUCCESS) {
		WM_LOG_ERROR("[WM] T%d Failed to fetch ip4 address\n", getpid());
		return WIFI_MANAGER_FAIL;
	}

	uint8_t *ip = (uint8_t *)&(ip_ref.s_addr);
	snprintf(info->ip4_address, 18, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

	char softap_ssid[WIFIMGR_SSID_LEN + 1];
	wifimgr_info_msg_s winfo = {info->ssid, softap_ssid,
								info->mac_address, 0, WIFIMGR_UNINITIALIZED};

	wifimgr_get_info(WIFIMGR_ALL_INFO, &winfo);
	info->rssi = winfo.rssi;
	_convert_state(&winfo.state, &info->status, &info->mode);

	// wifi manager stores softap ssid and ap ssid connected
	// so it should be converted by wifi mode
	if (info->mode == SOFTAP_MODE) {
		memcpy(info->ssid, softap_ssid, WIFIMGR_SSID_LEN + 1);
	}

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e wifi_manager_get_stats(wifi_manager_stats_s *stats)
{
	if (!stats) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}

	wifimgr_get_stats(stats);

	return WIFI_MANAGER_SUCCESS;
}


/**
 * Wi-Fi callback
 */
wifi_manager_result_e wifi_manager_register_cb(wifi_manager_cb_s *wmcb)
{
	int res = wifimgr_register_cb(wmcb);
	if (res < 0) {
		return WIFI_MANAGER_FAIL;
	}

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e wifi_manager_unregister_cb(wifi_manager_cb_s *wmcb)
{
	if (!wmcb) {
		return WIFI_MANAGER_INVALID_ARGS;
	}

	int res = wifimgr_unregister_cb(wmcb);
	if (res < 0) {
		return WIFI_MANAGER_FAIL;
	}

	return WIFI_MANAGER_SUCCESS;
}


/**
 * Wi-Fi Profile
 */
#ifdef CONFIG_WIFI_MANAGER_SAVE_CONFIG
wifi_manager_result_e wifi_manager_save_config(wifi_manager_ap_config_s *config)
{
	if (!config) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
	}

	WIFIMGR_CHECK_AP_CONFIG(config);
	WIFIMGR_CHECK_UTILRESULT(wifi_profile_write(config, 0), "wifimgr save config fail\n", WIFI_MANAGER_FAIL);

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e wifi_manager_get_config(wifi_manager_ap_config_s *config)
{
	if (!config) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
	}

	WIFIMGR_CHECK_UTILRESULT(wifi_profile_read(config, 0), "wifimgr get config fail\n", WIFI_MANAGER_FAIL);

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e wifi_manager_remove_config(void)
{
	WIFIMGR_CHECK_UTILRESULT(wifi_profile_reset(0), "wifimgr remove config fail\n", WIFI_MANAGER_FAIL);
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e wifi_manager_get_connected_config(wifi_manager_ap_config_s *config)
{
	if (!config) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
	}
	WIFIMGR_CHECK_UTILRESULT(wifi_profile_read(config, 1), "wifimgr get config fail\n", WIFI_MANAGER_FAIL);

	return WIFI_MANAGER_SUCCESS;
}
#else
wifi_manager_result_e wifi_manager_save_config(wifi_manager_ap_config_s *config)
{
	return WIFI_MANAGER_NO_API;
}


wifi_manager_result_e wifi_manager_get_config(wifi_manager_ap_config_s *config)
{
	return WIFI_MANAGER_NO_API;
}


wifi_manager_result_e wifi_manager_remove_config(void)
{
	return WIFI_MANAGER_NO_API;
}


wifi_manager_result_e wifi_manager_get_connected_config(wifi_manager_ap_config_s *config)
{
	return WIFI_MANAGER_NO_API;
}
#endif // CONFIG_WIFI_MANAGER_SAVE_CONFIG
