/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <errno.h>
#include <wifi_manager/wifi_manager.h>
#include "wm_test.h"
#include "wm_test_utils.h"
#include "wm_test_log.h"

/* Macro*/
#define WIFIMGR_SSID ""
#define WIFIMGR_PWD ""
#define WIFIMGR_AUTH WIFI_MANAGER_AUTH_WPA2_PSK
#define WIFIMGR_CRYPTO WIFI_MANAGER_CRYPTO_AES

#define WO_INTERVAL 10
#define WO_GOAL_CNT 1000
#define TAG "[WTON]"

/* callbacks*/
static void wm_sta_connected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg);

/* State */
static int run_init(void *arg);
static int run_connecting(wifi_manager_ap_config_s *ap_config);
static int run_connected(void);

/* Global*/
static wifi_manager_cb_s g_wifi_callbacks = {
	wm_sta_connected,
	wm_sta_disconnected,
	NULL, NULL, NULL,
};
static struct wo_queue *g_wo_queue = NULL;

void wm_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	int conn = 0;
	WT_LOG(TAG, "-->res(%d)", msg.res);
	if (WIFI_MANAGER_SUCCESS == msg.res) {
		conn = WO_CONN_SUCCESS;
	} else {
		conn = WO_CONN_FAIL;
	}
	WO_TEST_SIGNAL(conn, g_wo_queue);
}

void wm_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> %d", msg.res);
	WO_TEST_SIGNAL(WO_CONN_FAIL, g_wo_queue);
}

static void print_wifi_ap_profile(wifi_manager_ap_config_s *config, char *title)
{
	WT_LOG(TAG, "====================================");
	if (title) {
		WT_LOG(TAG, "%s", title);
	}
	WT_LOG(TAG, "------------------------------------");
	WT_LOG(TAG, "SSID: %s", config->ssid);
	WT_LOG(TAG, "security type (%d) (%d)", config->ap_auth_type, config->ap_crypto_type);
	WT_LOG(TAG, "====================================");
}

static int run_init(void *arg)
{
	wifi_manager_result_e res = wifi_manager_init(&g_wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "res %d", res);
		return -1;
	}

	int cnt_auto_connect = 0;

	/* Set AP Configuration */
	struct wt_options *ap_info = (struct wt_options *)arg;
	wifi_manager_ap_config_s apconfig;
  wm_get_apinfo(&apconfig, ap_info->ssid, ap_info->password, ap_info->auth_type, ap_info->crypto_type);
  print_wifi_ap_profile(&apconfig, "Connecting AP Info");

	/*  Run Auto Test */
	int state = 1;
	while (1) {
		if (state == 1) {
			state = run_connecting(&apconfig);
		} else if (state == 2) {
			cnt_auto_connect++;
			WT_LOG(TAG, "\n\n\n[WO] connection count %d\n\n\n", cnt_auto_connect);
			if (cnt_auto_connect > WO_GOAL_CNT) {
				break;
			}
			state = run_connected();
		} else {
			WT_LOGE(TAG, "Unknown state (%d)", state);
			assert(0);
		}
	}

	WT_LOG(TAG, "terminate program total (%d)", cnt_auto_connect);

	return 0;
}

static int run_connecting(wifi_manager_ap_config_s *ap_config)
{
	WT_LOG(TAG, "-->");

	wifi_manager_result_e res = wifi_manager_connect_ap(ap_config);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "res %d", res);
		goto connect_fail;
	}

	int conn = WO_CONN_FAIL;
	WO_TEST_WAIT(conn, g_wo_queue);

	if (conn == WO_CONN_FAIL) {
		// does it need to get info from wi-fi wm_get_info(ap_config);
		goto connect_fail;
	} else if (conn == WO_CONN_SUCCESS) {
		return 2; // connected, wait disconnect message
	} else {
		WT_LOGE(TAG, "program is corrupted");
		assert(0);
	}
	return 0;

connect_fail:
	WT_LOG(TAG, "wait %d second", WO_INTERVAL);
	sleep(WO_INTERVAL);
	return 1;
}

static int run_connected(void)
{
	WT_LOG(TAG, "-->");
	int conn = WO_CONN_FAIL;
	WO_TEST_WAIT(conn, g_wo_queue);
	if (conn == WO_CONN_FAIL) {
		return 1;
	} else {
		WT_LOGE(TAG, "program is corrupted");
		assert(0);
	}
	return 0;
}

void wm_test_on_off(void *arg)
{
	g_wo_queue = wo_create_queue();
	if (!g_wo_queue) {
		WT_LOGE(TAG, "create queue fail");
		return;
	}

	run_init(arg);

	wo_destroy_queue(g_wo_queue);
}
