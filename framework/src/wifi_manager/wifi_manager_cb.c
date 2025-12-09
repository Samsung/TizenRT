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
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <tinyara/net/netlog.h>
#include <tinyara/net/if/wifi.h>
#include <wifi_manager/wifi_manager.h>
#include "wifi_manager_cb.h"
#include "wifi_manager_utils.h"
#include "wifi_manager_error.h"
#include "wifi_manager_stats.h"

#define WIFIMGR_NUM_CALLBACKS 10
#define LOCK_WIFICB pthread_mutex_lock(&g_cb_handler.lock)
#define UNLOCK_WIFICB pthread_mutex_unlock(&g_cb_handler.lock)
#define TAG "[WM]"

/*  Handler */
struct wifimgr_cb_handler {
	wifi_manager_cb_s *cb[WIFIMGR_NUM_CALLBACKS];
	int8_t idx;
	pthread_mutex_t lock;
};
typedef struct wifimgr_cb_handler wifimgr_cb_handler_s;

/*
 * Private functions
 */
static void _handle_user_cb(_wifimgr_usr_cb_type_e evt, void *arg);
static void _free_scan_info(wifi_manager_scan_info_s *scan_list);
static wifi_manager_result_e _convert_scan_info(wifi_manager_scan_info_s **wm_scan_list, trwifi_scan_list_s *wu_scan_list);
/*
 * Global variable
 */
static wifimgr_cb_handler_s g_cb_handler;

/*
 * Private function
 */
static void _free_scan_info(wifi_manager_scan_info_s *scan_list)
{
	wifi_manager_scan_info_s *iter = scan_list, *prev = NULL;
	while (iter) {
		prev = iter;
		iter = iter->next;
		free(prev);
	}
}

static wifi_manager_result_e _convert_scan_info(wifi_manager_scan_info_s **wm_scan_list,
												trwifi_scan_list_s *wu_scan_list)
{
	wifi_manager_scan_info_s *cur = NULL, *prev = NULL;
	trwifi_scan_list_s *iter = wu_scan_list;
	while (iter) {
		cur = (wifi_manager_scan_info_s *)calloc(1, sizeof(wifi_manager_scan_info_s));
		if (!cur) {
			_free_scan_info(*wm_scan_list);
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_INTERNAL_FAIL);
			return WIFI_MANAGER_FAIL;
		}
		cur->rssi = iter->ap_info.rssi;
		cur->channel = iter->ap_info.channel;
		cur->phy_mode = iter->ap_info.phy_mode;
		cur->ap_auth_type = wifimgr_convert2wifimgr_auth(iter->ap_info.ap_auth_type);
		cur->ap_crypto_type = wifimgr_convert2wifimgr_crypto(iter->ap_info.ap_crypto_type);
		strncpy(cur->ssid, (char *)iter->ap_info.ssid, WIFIMGR_SSID_LEN);
		cur->ssid[WIFIMGR_SSID_LEN] = '\0';
		strncpy(cur->bssid, (char *)iter->ap_info.bssid, WIFIMGR_MACADDR_STR_LEN);
		cur->bssid[WIFIMGR_MACADDR_STR_LEN] = '\0';

		if (!prev) {
			*wm_scan_list = cur;
		} else {
			prev->next = cur;
		}
		prev = cur;
		iter = iter->next;
	}
	return WIFI_MANAGER_SUCCESS;
}

static void _create_cb_msg(wifi_manager_cb_msg_s *msg,
						  _wifimgr_usr_cb_type_e evt,
						  void *arg)
{
	if (evt == CB_SCAN_DONE) {
		trwifi_scan_list_s *list = (trwifi_scan_list_s *)arg;
		wifi_manager_scan_info_s *info = NULL;
		if (!list || WIFI_MANAGER_SUCCESS != _convert_scan_info(&info, list)) {
			NET_LOGE(TAG, "convert scan error\n");
			msg->res = WIFI_MANAGER_FAIL;
		} else {
			msg->scanlist = info;
		}
	} else if (evt == CB_STA_CONNECT_FAILED) {
		msg->res = WIFI_MANAGER_FAIL;
	} else {
		trwifi_cbk_msg_s *tmsg = (trwifi_cbk_msg_s *)arg;
		msg->reason = tmsg->reason;
		memcpy(msg->bssid, tmsg->bssid, sizeof(msg->bssid));
		if (evt == CB_STA_DISCONNECTED) {
			msg->res = WIFI_MANAGER_DISCONNECT;
		} else if (evt == CB_STA_RECONNECTED) {
			msg->res = WIFI_MANAGER_RECONNECT;
		}
	}
}

static void _delete_cb_msg(wifi_manager_cb_msg_s *msg)
{
	if (msg->scanlist) {
		_free_scan_info(msg->scanlist);
	}
}

static void _post_user_cb(_wifimgr_usr_cb_type_e evt,
						  wifi_manager_cb_msg_s msg)
{
	for (int i = 0; i < WIFIMGR_NUM_CALLBACKS; i++) {
		wifi_manager_cb_s *cbk = g_cb_handler.cb[i];
		if (!cbk) {
			continue;
		}
		switch (evt) {
		case CB_STA_CONNECTED:
			NET_LOGV(TAG, "call sta connect success event\n");
			if (cbk->sta_connected) {
				cbk->sta_connected(msg, NULL);
			}
			break;
		case CB_STA_CONNECT_FAILED:
			NET_LOGV(TAG, "call sta connect fail event\n");
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_CONNECT_FAIL);
			if (cbk->sta_connected) {
				cbk->sta_connected(msg, NULL);
			}
			break;
		case CB_STA_DISCONNECTED:
			NET_LOGV(TAG, "call sta disconnect event\n");
			if (cbk->sta_disconnected) {
				cbk->sta_disconnected(msg, NULL);
			}
			break;
		case CB_STA_RECONNECTED:
			NET_LOGV(TAG, "call sta reconnect event\n");
			if (cbk->sta_disconnected) {
				cbk->sta_disconnected(msg, NULL);
			}
			break;
		case CB_STA_JOINED:
			NET_LOGV(TAG, "call sta join event\n");
			if (cbk->softap_sta_joined) {
				cbk->softap_sta_joined(msg, NULL);
			}
			break;
		case CB_STA_LEFT:
			NET_LOGV(TAG, "call sta leave event\n");
			if (cbk->softap_sta_left) {
				cbk->softap_sta_left(msg, NULL);
			}
			break;
		case CB_SCAN_DONE:
			NET_LOGV(TAG, "call sta scan event\n");
			/* convert scan data.*/
			if (cbk->scan_ap_done) {
				cbk->scan_ap_done(msg, NULL);
			}
			break;
		default:
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
			NET_LOGE(TAG, "Invalid State\n");
		}
	}
}

void _handle_user_cb(_wifimgr_usr_cb_type_e evt, void *arg)
{
	wifi_manager_cb_msg_s msg = {WIFI_MANAGER_SUCCESS, 0, {0,}, NULL};

	_create_cb_msg(&msg, evt, arg);
	_post_user_cb(evt, msg);
	_delete_cb_msg(&msg);

	WIFIMGR_STATS_INC(evt);
}

/*
 * Public function
 */
int wifimgr_register_cb(wifi_manager_cb_s *wmcb)
{
	LOCK_WIFICB;
	if (g_cb_handler.idx == WIFIMGR_NUM_CALLBACKS) {
		UNLOCK_WIFICB;
		return -1;
	}

	for (int i = 1; i < WIFIMGR_NUM_CALLBACKS; i++) {
		if (g_cb_handler.cb[i] == NULL) {
			g_cb_handler.cb[i] = wmcb;
			g_cb_handler.idx++;
			NET_LOGI(TAG, "wifimgr cb is registered(%d)\n", i);
			UNLOCK_WIFICB;
			return 0;
		}
	}
	UNLOCK_WIFICB;

	return 0;
}

int wifimgr_unregister_cb(wifi_manager_cb_s *wmcb)
{
	LOCK_WIFICB;
	for (int i = 1; i < WIFIMGR_NUM_CALLBACKS; i++) {
		if (g_cb_handler.cb[i] == wmcb) {
			g_cb_handler.cb[i] = NULL;
			g_cb_handler.idx--;
			UNLOCK_WIFICB;
			return 0;
		}
	}
	UNLOCK_WIFICB;
	return -1;
}

void wifimgr_call_cb(_wifimgr_usr_cb_type_e evt, void *arg)
{
	_handle_user_cb(evt, arg);
}

void wifimgr_unregister_all(void)
{
	LOCK_WIFICB;
	for (int i = 0; i < WIFIMGR_NUM_CALLBACKS; i++) {
		if (g_cb_handler.cb[i] != NULL) {
			g_cb_handler.cb[i] = NULL;
		}
	}
	g_cb_handler.idx = 0;
	UNLOCK_WIFICB;
}
