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
#include <wifi_manager/wifi_manager.h>
#include <stress_tool/st_perf.h>
#include "wm_test.h"
#include "wm_test_utils.h"
#include "wm_test_network.h"
#include "wm_test_mock.h"
#include "wm_test_log.h"

#define WM_TEST_TRIAL 2
#define TAG "[WT]"

//if semaphore operation failed then it'll try it again 10ms later
static char *WM_AP_SSID;
static char *WM_AP_PASSWORD;
static wifi_manager_ap_auth_type_e WM_AP_AUTH;
static wifi_manager_ap_crypto_type_e WM_AP_CRYPTO;
static char *WM_SOFTAP_SSID;
static char *WM_SOFTAP_PASSWORD;
static int WM_SOFTAP_CHANNEL;

/*
 * callbacks
 */
static void wm_cb_sta_connected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_softap_sta_join(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_softap_sta_leave(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_scan_done(wifi_manager_cb_msg_s msg, void *arg);

static wifi_manager_cb_s g_wifi_callbacks = {
	wm_cb_sta_connected,
	wm_cb_sta_disconnected,
	wm_cb_softap_sta_join,
	wm_cb_softap_sta_leave,
	wm_cb_scan_done,
};

static struct wo_queue *g_wo_queue = NULL;

void wm_cb_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d)", msg.res);
	WT_LOG(TAG, "bssid %02x:%02x:%02x:%02x:%02x:%02x",
		   msg.bssid[0], msg.bssid[1],
		   msg.bssid[2], msg.bssid[3],
		   msg.bssid[4], msg.bssid[5]);
	int conn = 0;
	if (WIFI_MANAGER_SUCCESS == msg.res) {
		conn = WO_CONN_SUCCESS;
	} else {
		conn = WO_CONN_FAIL;
	}
	WO_TEST_SIGNAL(conn, g_wo_queue);
}

void wm_cb_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d) reason %d", msg.res, msg.reason);
	WO_TEST_SIGNAL(WO_CONN_SUCCESS, g_wo_queue);
}

void wm_cb_softap_sta_join(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d)", msg.res);
	WT_LOG(TAG, "bssid %02x:%02x:%02x:%02x:%02x:%02x",
		   msg.bssid[0], msg.bssid[1],
		   msg.bssid[2], msg.bssid[3],
		   msg.bssid[4], msg.bssid[5]);
	WO_TEST_SIGNAL(WO_CONN_SUCCESS, g_wo_queue);
}

void wm_cb_softap_sta_leave(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d) reason %d", msg.res, msg.reason);
	WO_TEST_SIGNAL(WO_CONN_SUCCESS, g_wo_queue);
}

void wm_cb_scan_done(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d)", msg.res);
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (msg.res != WIFI_MANAGER_SUCCESS || msg.scanlist == NULL) {
		WO_TEST_SIGNAL(WO_CONN_FAIL, g_wo_queue);
		return;
	}
	wt_print_scanlist(msg.scanlist);
	WO_TEST_SIGNAL(WO_CONN_SUCCESS, g_wo_queue);
}

/*
 * Following API are not covered.
 * 1) wifi_manager_result_e wifi_manager_scan_specific_ap(wifi_manager_ap_config_s *config);
 * 2) wifi_manager_result_e wifi_manager_save_config(wifi_manager_ap_config_s *config);
 * 3) wifi_manager_result_e wifi_manager_get_config(wifi_manager_ap_config_s *config);
 * 4) wifi_manager_result_e wifi_manager_remove_config(void);
 * 5) wifi_manager_result_e wifi_manager_get_connected_config(wifi_manager_ap_config_s *config);
 */
static int _run_procedure(void)
{
	int conn = 0;
	wifi_manager_ap_config_s apconfig;
	wifi_manager_info_s wminfo;
	wifi_manager_stats_s stats;
	wm_get_apinfo(&apconfig, WM_AP_SSID, WM_AP_PASSWORD, WM_AP_AUTH, WM_AP_CRYPTO);

	CONTROL_VDRIVER(VWIFI_CMD_SET, VWIFI_KEY_RESULT, (int)TRWIFI_SUCCESS, 0);

	wifi_manager_result_e wres = WIFI_MANAGER_SUCCESS;
	WT_LOG(TAG, "init wi-fi");
	wres = wifi_manager_init(&g_wifi_callbacks);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "fail to init %d\n", wres);
		return -1;
	}

	wres = wifi_manager_get_info(&wminfo);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "get info fail %d\n", wres);
		return -1;
	}
	wt_print_conninfo(&wminfo);

	/*  Start softAP */
	WT_LOG(TAG, "start softAP");
	wifi_manager_softap_config_s softap_config;
	wm_get_softapinfo(&softap_config, WM_SOFTAP_SSID, WM_SOFTAP_PASSWORD, WM_SOFTAP_CHANNEL);
	wres = wifi_manager_set_mode(SOFTAP_MODE, &softap_config);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "fail to start softap %d\n", wres);
		return -1;
	}

	wres = wifi_manager_get_info(&wminfo);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "get info fail %d\n", wres);
		return -1;
	}
	wt_print_conninfo(&wminfo);

	/*  wait join event */
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SOFTAP_STA_JOINED, 0, 3000);
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, VWIFI_PKT_DHCPS_EVT, 0, 3000);
	WT_LOG(TAG, "wait join event");
	WO_TEST_WAIT(conn, g_wo_queue);

	wres = wifi_manager_get_info(&wminfo);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "get info fail %d\n", wres);
		return -1;
	}
	wt_print_conninfo(&wminfo);

	/*  scan in softAP mode */
	WT_LOG(TAG, "scan in softAP mode");
	wres = wifi_manager_scan_ap(NULL);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "fail to scan %d\n", wres);
		return -1;
	}
	wres = wifi_manager_get_info(&wminfo);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "get info fail %d\n", wres);
		return -1;
	}
	wt_print_conninfo(&wminfo);

	/*  wait scan event */
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SCAN_DONE, 0, 1000);
	WT_LOG(TAG, "wait scan done event");
	WO_TEST_WAIT(conn, g_wo_queue);

	/*  scan in softAP mode */
	WT_LOG(TAG, "scan in softAP mode");
	wres = wifi_manager_scan_ap(NULL);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "fail to scan %d\n", wres);
		return -1;
	}

	/*  wait scan event */
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SCAN_DONE, 0, 1000);
	WT_LOG(TAG, "wait scan done event");
	WO_TEST_WAIT(conn, g_wo_queue);

	wres = wifi_manager_get_info(&wminfo);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "get info fail %d\n", wres);
		return -1;
	}
	wt_print_conninfo(&wminfo);

	/*  set STA */
	WT_LOG(TAG, "start STA mode");

	wres = wifi_manager_set_mode(STA_MODE, NULL);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "start STA fail %d\n", wres);
		return -1;
	}

	wres = wifi_manager_get_info(&wminfo);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "get info fail %d\n", wres);
		return -1;
	}
	wt_print_conninfo(&wminfo);

	/*  scan in STA mode */
	WT_LOG(TAG, "scan in STA mode");
	wres = wifi_manager_scan_ap(NULL);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "fail to scan %d\n", wres);
		return -1;
	}

	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SCAN_DONE, 0, 1000);
	WT_LOG(TAG, "wait scan done event in STA mode");
	WO_TEST_WAIT(conn, g_wo_queue); /*  wait scan event */

	/*  connect to AP */
	WT_LOG(TAG, "connect AP");
	wres = wifi_manager_connect_ap(&apconfig);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "connect AP fail %d\n", wres);
		return -1;
	}
	wres = wifi_manager_get_info(&wminfo);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "get info fail %d\n", wres);
		return -1;
	}
	wt_print_conninfo(&wminfo);

	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_CONNECTED, 0, 1000);
	WT_LOG(TAG, "wait connect success event");
	WO_TEST_WAIT(conn, g_wo_queue);

	wres = wifi_manager_get_info(&wminfo);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "get info fail %d\n", wres);
		return -1;
	}
	wt_print_conninfo(&wminfo);

	/*  disconnect to AP */
	wres = wifi_manager_disconnect_ap();
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "disconnect AP fail %d\n", wres);
		return -1;
	}
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_DISCONNECTED, 0, 1000);
	WT_LOG(TAG, "wait disconnect success event");
	WO_TEST_WAIT(conn, g_wo_queue);

	/*  connect to AP */
	WT_LOG(TAG, "connect AP");
	wres = wifi_manager_connect_ap(&apconfig);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "connect AP fail %d\n", wres);
		return -1;
	}
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_CONNECTED, 0, 1000);
	WT_LOG(TAG, "wait connect success event");
	WO_TEST_WAIT(conn, g_wo_queue);

	/*  scan in connected state */
	WT_LOG(TAG, "scan in connected state of STA mode");
	wres = wifi_manager_scan_ap(NULL);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "fail to scan %d\n", wres);
		return -1;
	}
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SCAN_DONE, 0, 1000);
	WT_LOG(TAG, "wait scan done event in connected state of STA mode");
	WO_TEST_WAIT(conn, g_wo_queue); /*  wait scan event */

	wres = wifi_manager_get_stats(&stats);
	if (wres != WIFI_MANAGER_SUCCESS && wres != WIFI_MANAGER_NOT_AVAILABLE) {
		// some drivers doesn't supports stats feature.
		WT_LOGE(TAG, "get stats fail %d\n", wres);
		return -1;
	}
	wt_print_stats(&stats);

	WT_LOG(TAG, "deinit wi-fi");
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT_FUNC, LWNL_EVT_STA_DISCONNECTED, 3, 0);
	wres = wifi_manager_deinit();
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "fail to deinit %d\n", wres);
		return -1;
	}
	return 0;
}

TEST_F(mode_change)
{
	ST_START_TEST;
	ST_EXPECT_EQ(0, _run_procedure());
	ST_END_TEST;
}

TEST_F(init_n)
{
	ST_START_TEST;
	CONTROL_VDRIVER(VWIFI_CMD_SET, VWIFI_KEY_RESULT, TRWIFI_FAIL, 0);
	ST_EXPECT_EQ(WIFI_MANAGER_FAIL, wifi_manager_init(&g_wifi_callbacks));
	ST_END_TEST;
}

TEST_SETUP(init_p)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_TEARDOWN(init_p)
{
	ST_START_TEST;
	wifi_manager_deinit();
	ST_END_TEST;
}

TEST_F(init_p)
{
	ST_START_TEST;
	CONTROL_VDRIVER(VWIFI_CMD_SET, VWIFI_KEY_RESULT, TRWIFI_SUCCESS, 0);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	ST_END_TEST;
}

TEST_F(set_power_p)
{
	ST_START_TEST;
	CONTROL_VDRIVER(VWIFI_CMD_SET, VWIFI_KEY_RESULT, TRWIFI_SUCCESS, 0);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_set_powermode(WIFI_MANAGER_POWERMODE_DISABLE));
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(scan_p)
{
	ST_START_TEST;
	int conn = 0;
	CONTROL_VDRIVER(VWIFI_CMD_SET, VWIFI_KEY_RESULT, TRWIFI_SUCCESS, 0);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_info_s wminfo;

	/*  scan in softAP mode */
	WT_LOG(TAG, "scan in softAP mode");
	wifi_manager_result_e wres = wifi_manager_scan_ap(NULL);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "fail to scan %d\n", wres);
		return -1;
	}

	/*  wait scan event */
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SCAN_DONE, 0, 1000);
	WT_LOG(TAG, "wait scan done event");
	WO_TEST_WAIT(conn, g_wo_queue);

	/*  scan in softAP mode */
	WT_LOG(TAG, "scan in softAP mode");
	wres = wifi_manager_scan_ap(NULL);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "fail to scan %d\n", wres);
		return -1;
	}

	/*  wait scan event */
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SCAN_DONE, 0, 1000);
	WT_LOG(TAG, "wait scan done event");
	WO_TEST_WAIT(conn, g_wo_queue);

	wres = wifi_manager_get_info(&wminfo);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "get info fail %d\n", wres);
		return -1;
	}
	wt_print_conninfo(&wminfo);

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(scan_n)
{
	ST_START_TEST;
	int conn = 0;
	CONTROL_VDRIVER(VWIFI_CMD_SET, VWIFI_KEY_RESULT, TRWIFI_SUCCESS, 0);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_info_s wminfo;

	/*  scan in softAP mode */
	WT_LOG(TAG, "scan in softAP mode");
	wifi_manager_result_e wres = wifi_manager_scan_ap(NULL);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "fail to scan %d\n", wres);
		return -1;
	}

	/*  wait scan event */
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SCAN_DONE, 0, 1000);
	WT_LOG(TAG, "wait scan done event");
	WO_TEST_WAIT(conn, g_wo_queue);

	/*  scan in softAP mode */
	WT_LOG(TAG, "scan in softAP mode");
	wres = wifi_manager_scan_ap(NULL);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "fail to scan %d\n", wres);
		return -1;
	}

	/*  wait scan event */
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SCAN_FAILED, 0, 1000);
	WT_LOG(TAG, "wait scan done event");
	WO_TEST_WAIT(conn, g_wo_queue);

	wres = wifi_manager_get_info(&wminfo);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "get info fail %d\n", wres);
		return -1;
	}
	wt_print_conninfo(&wminfo);

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

/* desc: check reason code is valid when it is disconnected unexpectedly */
TEST_F(disconn_evt)
{
	ST_START_TEST;
	int conn = 0;
	CONTROL_VDRIVER(VWIFI_CMD_SET, VWIFI_KEY_RESULT, TRWIFI_SUCCESS, 0);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_info_s wminfo;

	/*  connect to AP */
	WT_LOG(TAG, "connect AP");
	wifi_manager_ap_config_s apconfig;
	wm_get_apinfo(&apconfig, WM_AP_SSID, WM_AP_PASSWORD, WM_AP_AUTH, WM_AP_CRYPTO);
	wifi_manager_result_e wres = wifi_manager_connect_ap(&apconfig);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "connect AP fail %d\n", wres);
		return -1;
	}

	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_CONNECTED, 0, 1000);
	WT_LOG(TAG, "wait connect success event");
	WO_TEST_WAIT(conn, g_wo_queue);

	// set reason code 3
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_DISCONNECTED, 3, 1000);
	WT_LOG(TAG, "wait disconnect success event");
	WO_TEST_WAIT(conn, g_wo_queue);

	wres = wifi_manager_get_info(&wminfo);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "get info fail %d\n", wres);
		return -1;
	}
	wt_print_conninfo(&wminfo);

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

/*
 * description: global configuration
 */
TESTCASE_SETUP(global_config)
{
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
}
END_TESTCASE

TESTCASE_TEARDOWN(global_config)
{
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
}
END_TESTCASE

START_TEST_F(connect_stress)
{
	int conn = 0;
	wifi_manager_ap_config_s apconfig;
	wifi_manager_info_s wminfo;
	wm_get_apinfo(&apconfig, WM_AP_SSID, WM_AP_PASSWORD, WM_AP_AUTH, WM_AP_CRYPTO);

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_scan_ap(NULL));
	/*  wait scan event */
	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_SCAN_FAILED, 0, 1000);
	WO_TEST_WAIT(conn, g_wo_queue);

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_connect_ap(&apconfig));

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_get_info(&wminfo));
	wt_print_conninfo(&wminfo);

	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_CONNECTED, 0, 1000);
	WT_LOG(TAG, "wait connect success event");
	WO_TEST_WAIT(conn, g_wo_queue);
	ST_EXPECT_EQ(WO_CONN_SUCCESS, conn);

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_get_info(&wminfo));
	wt_print_conninfo(&wminfo);

	/*  disconnect to AP */
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_disconnect_ap());

	CONTROL_VDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_DISCONNECTED, 0, 1000);
	WT_LOG(TAG, "wait disconnect success event");
	WO_TEST_WAIT(conn, g_wo_queue);
}
END_TEST_F

void wm_run_stress_test4(struct wt_options *opt)
{
	WM_AP_SSID = opt->ssid;
	WM_AP_PASSWORD = opt->password;
	WM_AP_AUTH = opt->auth_type;
	WM_AP_CRYPTO = opt->crypto_type;
	WM_SOFTAP_SSID = opt->softap_ssid;
	WM_SOFTAP_PASSWORD = opt->softap_password;
	WM_SOFTAP_CHANNEL = opt->softap_channel;

	g_wo_queue = wo_create_queue();
	if (!g_wo_queue) {
		WT_LOGE(TAG, "create queue fail");
		return;
	}

	ST_SET_PACK(wifi);
	ST_SET_SMOKE1(wifi, opt->repeat, 0, "use case test", mode_change);
	ST_SET_SMOKE1(wifi, opt->repeat, 0, "init negative case", init_n);
	ST_SET_SMOKE(wifi, opt->repeat, 0, "init positive case", init_p);
	ST_SET_SMOKE1(wifi, opt->repeat, 0, "set power", set_power_p);
	ST_SET_SMOKE1(wifi, opt->repeat, 0, "scan positive case", scan_p);
	ST_SET_SMOKE1(wifi, opt->repeat, 0, "scan negative case", scan_n);
	ST_SET_SMOKE1(wifi, opt->repeat, 0, "scan negative case", disconn_evt);
	ST_RUN_TEST(wifi);

	ST_SET_PACK(wifi_connect);
	ST_TC_SET_GLOBAL(wifi_connect, global_config);
	ST_SET_SMOKE1(wifi_connect, opt->repeat, 0, "", connect_stress);
	ST_SET_KEEP_RUNNING;
	ST_RUN_TEST(wifi_connect);
	ST_RESET_KEEP_RUNNING;

	WT_LOG(TAG, "deinit sem");
	wo_destroy_queue(g_wo_queue);
}
