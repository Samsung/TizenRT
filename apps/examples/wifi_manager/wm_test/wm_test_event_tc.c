/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#include <unistd.h>
#include <string.h>
#include <queue.h>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <tinyara/net/if/wifi.h>
#include <wifi_manager/wifi_manager.h>
#include <stress_tool/st_perf.h>
#include "wm_test.h"
#include "wm_test_utils.h"
#include "wm_test_network.h"
#include "wm_test_mock.h"
#include "wm_test_log.h"

#define TAG "[WE]"
/*
 * callbacks
 */
static void wm_cb_sta_connected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_softap_sta_joined(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_softap_sta_left(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_scan_done(wifi_manager_cb_msg_s msg, void *arg);

static wifi_manager_cb_s g_wifi_callbacks = {
	wm_cb_sta_connected,
	wm_cb_sta_disconnected,
	wm_cb_softap_sta_joined,
	wm_cb_softap_sta_left,
	wm_cb_scan_done};
static wifi_manager_ap_config_s g_apconfig;
static wifi_manager_softap_config_s g_softapconfig;
static struct wo_queue *g_wo_queue = NULL;

void wm_cb_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d)", msg.res);
	WT_LOG(TAG, "bssid %02x:%02x:%02x:%02x:%02x:%02x",
		   msg.bssid[0], msg.bssid[1],
		   msg.bssid[2], msg.bssid[3],
		   msg.bssid[4], msg.bssid[5]);
	WO_TEST_SIGNAL((int)msg.res, g_wo_queue);
}

void wm_cb_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d) reason %d", msg.res, msg.reason);
	WO_TEST_SIGNAL((int)WIFI_MANAGER_SUCCESS, g_wo_queue);
}

void wm_cb_softap_sta_joined(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d)", msg.res);
	WT_LOG(TAG, "bssid %02x:%02x:%02x:%02x:%02x:%02x",
		   msg.bssid[0], msg.bssid[1],
		   msg.bssid[2], msg.bssid[3],
		   msg.bssid[4], msg.bssid[5]);
	WO_TEST_SIGNAL((int)msg.res, g_wo_queue);
}

void wm_cb_softap_sta_left(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d) reason %d", msg.res, msg.reason);
	WO_TEST_SIGNAL((int)msg.res, g_wo_queue);
}

void wm_cb_scan_done(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d)", msg.res);
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (msg.res != WIFI_MANAGER_SUCCESS || msg.scanlist == NULL) {
		WO_TEST_SIGNAL((int)msg.res, g_wo_queue);
		return;
	}
	wt_print_scanlist(msg.scanlist);
	WO_TEST_SIGNAL((int)msg.res, g_wo_queue);
}

START_TEST_F(wifi_evt_tc)
{
	int conn = WIFI_MANAGER_SUCCESS;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_connect_ap(&g_apconfig));
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_CONNECT_FAILED, 0, 1000);
	WO_TEST_WAIT(conn, g_wo_queue);
	ST_EXPECT_EQ(WIFI_MANAGER_FAIL, conn);

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_connect_ap(&g_apconfig));
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_CONNECTED, 0, 1000);
	WO_TEST_WAIT(conn, g_wo_queue);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, conn);

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_scan_ap(NULL));
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SCAN_DONE, 0, 1000);
	WO_TEST_WAIT(conn, g_wo_queue);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, conn);

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_scan_ap(NULL));
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SCAN_FAILED, 0, 1000);
	WO_TEST_WAIT(conn, g_wo_queue);
	ST_EXPECT_EQ(WIFI_MANAGER_FAIL, conn);

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_disconnect_ap());
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_DISCONNECTED, 0, 1000);
	WO_TEST_WAIT(conn, g_wo_queue);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, conn);

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(SOFTAP_MODE, &g_softapconfig));

	/*  join event */
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SOFTAP_STA_JOINED, 0, 1000);
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, VWIFI_PKT_DHCPS_EVT, 0, 3000);
	WO_TEST_WAIT(conn, g_wo_queue);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, conn);

	/*  leave event */
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SOFTAP_STA_LEFT, 0, 1000);
	WO_TEST_WAIT(conn, g_wo_queue);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, conn);

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
}
END_TEST_F

void wm_run_event_tc(struct wt_options *opt)
{
	g_wo_queue = wo_create_queue();
	if (!g_wo_queue) {
		WT_LOGE(TAG, "create queue fail");
		return;
	}
	wm_get_apinfo(&g_apconfig, opt->ssid, opt->password, opt->auth_type, opt->crypto_type);
	wm_get_softapinfo(&g_softapconfig, opt->softap_ssid, opt->softap_password, opt->softap_channel);
	ST_SET_PACK(wifi_event);
	ST_SET_SMOKE1(wifi_event, opt->repeat, 0, "", wifi_evt_tc);
	ST_RUN_TEST(wifi_event);

	wo_destroy_queue(g_wo_queue);
}
