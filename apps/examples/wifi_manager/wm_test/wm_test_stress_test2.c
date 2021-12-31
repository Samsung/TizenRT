/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <sys/types.h>
#include <stdio.h>
#include <queue.h>
#include <semaphore.h>
#include <assert.h>
#include <string.h>
#include <wifi_manager/wifi_manager.h>
#include <stress_tool/st_perf.h>
#include "wm_test.h"
#include "wm_test_log.h"
#include "wm_test_utils.h"

/* Macro*/
#define WM_TEST_TRIAL	   CONFIG_WIFIMANAGER_TEST_TRIAL
#define WM_NSOFTAP_SSID "no_sta_0101" // for auto test
#define TAG "[WTS2]"

static interop_ap_config_list_s g_apconfig_list;
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
static void wm_sta_connected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_softap_sta_join(wifi_manager_cb_msg_s msg, void *arg);
static void wm_softap_sta_leave(wifi_manager_cb_msg_s msg, void *arg);
static void wm_scan_done(wifi_manager_cb_msg_s msg, void *arg);

extern int wifi_interop_read_file(interop_ap_config_list_s *ap_config_list, char *file_path);
/*
 * Global
 */
static wifi_manager_cb_s g_wifi_callbacks = {
	wm_sta_connected,
	wm_sta_disconnected,
	wm_softap_sta_join,
	wm_softap_sta_leave,
	wm_scan_done,
};
static struct wo_queue *g_wo_queue = NULL;
static int g_conn = 0;
/*
 * Callback
 */
void wm_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->  res(%d)", msg.res);
	WO_TEST_SIGNAL(msg.res, g_wo_queue);
}

void wm_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->");
	WO_TEST_SIGNAL(msg.res, g_wo_queue);
}

void wm_softap_sta_join(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->");
	WO_TEST_SIGNAL(-1, g_wo_queue);
}

void wm_softap_sta_leave(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->");
	WO_TEST_SIGNAL(-1, g_wo_queue);
}

void wm_scan_done(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->");
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (msg.res != WIFI_MANAGER_SUCCESS || msg.scanlist == NULL) {
		WO_TEST_SIGNAL(msg.res, g_wo_queue);
		return;
	}
	wt_print_scanlist(msg.scanlist);
	WO_TEST_SIGNAL(msg.res, g_wo_queue);
}

static void wm_get_nosoftapinfo(wifi_manager_softap_config_s *ap_config)
{
	strncpy(ap_config->ssid, WM_NSOFTAP_SSID, strlen(WM_NSOFTAP_SSID) + 1);
	strncpy(ap_config->passphrase, WM_SOFTAP_PASSWORD, strlen(WM_SOFTAP_PASSWORD) + 1);
	ap_config->channel = WM_SOFTAP_CHANNEL;
}

/*
 * station join test
 */
TEST_SETUP(sta_join)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	ST_END_TEST;
}

TEST_TEARDOWN(sta_join)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_disconnect_ap());
	WO_TEST_WAIT(g_conn, g_wo_queue);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(sta_join)
{
	ST_START_TEST;
	wifi_manager_ap_config_s apconfig;
	wm_get_apinfo(&apconfig, WM_AP_SSID, WM_AP_PASSWORD, WM_AP_AUTH, WM_AP_CRYPTO);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_connect_ap(&apconfig));
	WO_TEST_WAIT(g_conn, g_wo_queue);

	ST_END_TEST;
}

/*
 * station scan test
 */
TEST_SETUP(sta_scan)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	ST_END_TEST;
}

TEST_TEARDOWN(sta_scan)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(sta_scan)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_scan_ap(NULL));
	WO_TEST_WAIT(g_conn, g_wo_queue);
	ST_END_TEST;
}

/*
 * station leave test
 */
TEST_SETUP(sta_leave)
{
	ST_START_TEST;

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_ap_config_s apconfig;
	wm_get_apinfo(&apconfig, WM_AP_SSID, WM_AP_PASSWORD, WM_AP_AUTH, WM_AP_CRYPTO);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_connect_ap(&apconfig));
	WO_TEST_WAIT(g_conn, g_wo_queue);

	ST_END_TEST;
}

TEST_TEARDOWN(sta_leave)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(sta_leave)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_disconnect_ap());
	WO_TEST_WAIT(g_conn, g_wo_queue);

	ST_END_TEST;
}

/*
 * SoftAP Start
 */
TEST_SETUP(softap_start)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	ST_END_TEST;
}

TEST_TEARDOWN(softap_start)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(softap_start)
{
	ST_START_TEST;
	wifi_manager_softap_config_s ap_config;
	wm_get_nosoftapinfo(&ap_config);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(SOFTAP_MODE, &ap_config));
	ST_END_TEST;
}

/*
 * SoftAP Scan
 */
TEST_SETUP(softap_scan)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_softap_config_s ap_config;
	wm_get_nosoftapinfo(&ap_config);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(SOFTAP_MODE, &ap_config));
	ST_END_TEST;
}

TEST_TEARDOWN(softap_scan)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(softap_scan)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_scan_ap(NULL));
	WO_TEST_WAIT(g_conn, g_wo_queue);
	ST_END_TEST;
}

/*
 * Soft AP scan in joined state
 */
TEST_SETUP(softap_joined_scan)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_softap_config_s ap_config;
	wm_get_softapinfo(&ap_config, WM_SOFTAP_SSID, WM_SOFTAP_PASSWORD, WM_SOFTAP_CHANNEL);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(SOFTAP_MODE, &ap_config));
	WO_TEST_WAIT(g_conn, g_wo_queue);

	ST_END_TEST;
}

TEST_TEARDOWN(softap_joined_scan)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(softap_joined_scan)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_scan_ap(NULL));
	WO_TEST_WAIT(g_conn, g_wo_queue);
	ST_END_TEST;
}

/*
 * SoftAP Stop
 */
TEST_SETUP(softap_stop)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_softap_config_s ap_config;
	wm_get_nosoftapinfo(&ap_config);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(SOFTAP_MODE, &ap_config));
	ST_END_TEST;
}

TEST_TEARDOWN(softap_stop)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_F(softap_stop)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

/*
 * SoftAP(joined) Stop
 */
TEST_SETUP(softap_joined_stop)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_softap_config_s ap_config;
	wm_get_softapinfo(&ap_config, WM_SOFTAP_SSID, WM_SOFTAP_PASSWORD, WM_SOFTAP_CHANNEL);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(SOFTAP_MODE, &ap_config));
	WO_TEST_WAIT(g_conn, g_wo_queue);

	ST_END_TEST;
}

TEST_TEARDOWN(softap_joined_stop)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_F(softap_joined_stop)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

/*
 * STA(disconnection) --> SoftAP
 */
TEST_SETUP(disconn_to_softap)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	ST_END_TEST;
}

TEST_TEARDOWN(disconn_to_softap)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(disconn_to_softap)
{
	ST_START_TEST;
	wifi_manager_softap_config_s ap_config;
	wm_get_nosoftapinfo(&ap_config);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(SOFTAP_MODE, &ap_config));
	ST_END_TEST;
}

/*
 * STA(connected) --> SoftAP
 */
TEST_SETUP(conn_to_softap)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_ap_config_s apconfig;
	wm_get_apinfo(&apconfig, WM_AP_SSID, WM_AP_PASSWORD, WM_AP_AUTH, WM_AP_CRYPTO);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_connect_ap(&apconfig));
	WO_TEST_WAIT(g_conn, g_wo_queue);
	ST_END_TEST;
}

TEST_TEARDOWN(conn_to_softap)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(conn_to_softap)
{
	ST_START_TEST;
	wifi_manager_softap_config_s ap_config;
	wm_get_nosoftapinfo(&ap_config);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(SOFTAP_MODE, &ap_config));
	ST_END_TEST;
}

/*
 * SoftAP(no joined STA) --> STA
 */
TEST_SETUP(leave_to_sta)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_softap_config_s ap_config;
	wm_get_nosoftapinfo(&ap_config);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(SOFTAP_MODE, &ap_config));
	ST_END_TEST;
}

TEST_TEARDOWN(leave_to_sta)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(leave_to_sta)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(STA_MODE, NULL));
	ST_END_TEST;
}

/*
 * SoftAP(joined STA) --> STA
 */
TEST_SETUP(join_to_sta)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_softap_config_s ap_config;
	wm_get_softapinfo(&ap_config, WM_SOFTAP_SSID, WM_SOFTAP_PASSWORD, WM_SOFTAP_CHANNEL);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(SOFTAP_MODE, &ap_config));
	WO_TEST_WAIT(g_conn, g_wo_queue); // wait STA joined
	ST_END_TEST;
}

TEST_TEARDOWN(join_to_sta)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(join_to_sta)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(STA_MODE, NULL));
	ST_END_TEST;
}

void wm_run_stress_test2_single_ap(struct wt_options *opt)
{
	WM_AP_SSID = opt->ssid;
	WM_AP_PASSWORD = opt->password;
	WM_AP_AUTH = opt->auth_type;
	WM_AP_CRYPTO = opt->crypto_type;
	WM_SOFTAP_SSID = opt->softap_ssid;
	WM_SOFTAP_PASSWORD = opt->softap_password;
	WM_SOFTAP_CHANNEL = opt->softap_channel;

	/*  Create Queue */
	g_wo_queue = wo_create_queue();
	if (!g_wo_queue) {
		WT_LOGE(TAG, "create queue fail");
		return;
	}

	ST_SET_PACK(wifi);

	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 10000000, "station join", sta_join);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 10000000, "station scan", sta_scan);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 2000000, "station leave", sta_leave);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 5000000, "softap start", softap_start);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 5000000, "softap scan", softap_scan);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 5000000, "softap(joined) scan", softap_joined_scan);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 5000000, "softap stop", softap_stop);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 5000000, "softap(joined) stop", softap_joined_stop);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 5000000, "sta(disconnected) to softap", disconn_to_softap);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 5000000, "sta(connected) to softap", conn_to_softap);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 5000000, "softap(leave) to sta", leave_to_sta);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 5000000, "softap(join) to sta", join_to_sta);

	ST_RUN_TEST(wifi);
	ST_RESULT_TEST(wifi);

	/*  Destroy queue */
	wo_destroy_queue(g_wo_queue);
}

void wm_run_stress_test2(struct wt_options *opt)
{
	if (!opt->path) {
		wm_run_stress_test2_single_ap(opt);
	} else {
		int ap_count = 0;
		interop_ap_info_s *ap_info = NULL;
		memset(&g_apconfig_list, 0, sizeof(g_apconfig_list));
		int ret = 0;
		ret = wifi_interop_read_file(&g_apconfig_list, opt->path);
		if (ret < 0) {
			WT_LOGE(TAG, "-->%s: failed to open file \n", __FUNCTION__);
			return;
		}
		print_ap_config_list(&g_apconfig_list);
		ap_count = g_apconfig_list.ap_count;

		for (int i = 0; i < ap_count; i++) {
			ap_info = g_apconfig_list.ap_info + i;
			opt->ssid = ap_info->ap_config.ssid;
			opt->password = ap_info->ap_config.passphrase;
			opt->auth_type = ap_info->ap_config.ap_auth_type;
			opt->crypto_type = ap_info->ap_config.ap_crypto_type;

			wm_run_stress_test2_single_ap(opt);
		}
	}
}
