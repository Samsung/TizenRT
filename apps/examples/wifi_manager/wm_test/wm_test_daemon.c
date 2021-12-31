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

#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <queue.h>
#include <semaphore.h>
#include <string.h>
#include <wifi_manager/wifi_manager.h>
#include <stress_tool/st_perf.h>
#include "wm_test.h"
#include "wm_test_network.h"
#include "wm_test_utils.h"
#include "wm_test_log.h"

#define WM_TEST_TRIAL 3
#define TAG "[WT]"
static char *WM_AP_SSID;
static char *WM_AP_PASSWORD;
static wifi_manager_ap_auth_type_e WM_AP_AUTH;
static wifi_manager_ap_crypto_type_e WM_AP_CRYPTO;
static char *WM_SOFTAP_SSID;
static char *WM_SOFTAP_PASSWORD;
static int WM_SOFTAP_CHANNEL;
static sem_t g_wm_sem;


/*
 * callbacks
 */
static void wm_sta_connected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_softap_sta_join(wifi_manager_cb_msg_s msg, void *arg);
static void wm_softap_sta_leave(wifi_manager_cb_msg_s msg, void *arg);
static void wm_scan_done(wifi_manager_cb_msg_s msg, void *arg);

static wifi_manager_cb_s g_wifi_callbacks = {
	wm_sta_connected,
	wm_sta_disconnected,
	wm_softap_sta_join,
	wm_softap_sta_leave,
	wm_scan_done,
};

#define WM_TEST_SIGNAL								\
	do {											\
		sem_post(&g_wm_sem);						\
		WT_LOG(TAG, "send signal");	\
	} while (0)

#define WM_TEST_WAIT								\
	do {											\
		WT_LOG(TAG, "wait signal");	\
		sem_wait(&g_wm_sem);                        \
	} while (0)

void wm_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d)", msg.res);
	WM_TEST_SIGNAL;
}

void wm_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->");
	WM_TEST_SIGNAL;
}

void wm_softap_sta_join(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->");
	WM_TEST_SIGNAL;
}

void wm_softap_sta_leave(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->");
	WM_TEST_SIGNAL;
}

void wm_scan_done(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->");
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (msg.res != WIFI_MANAGER_SUCCESS || msg.scanlist == NULL) {
		WM_TEST_SIGNAL;
		return;
	}
	wt_print_scanlist(msg.scanlist);
	WM_TEST_SIGNAL;
}

static int run_procedure(void *arg)
{
	int nres = 0;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	/* Join to softAP*/
	WT_LOG(TAG, "connect AP");
	wifi_manager_ap_config_s apconfig;
	wm_get_apinfo(&apconfig, WM_AP_SSID, WM_AP_PASSWORD, WM_AP_AUTH, WM_AP_CRYPTO);
	res = wifi_manager_connect_ap(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "connect AP fail %d", res);
		return -1;
	}
	WT_LOG(TAG, "wait connect success event");
	WM_TEST_WAIT;

	/* Receive data */
	WT_LOG(TAG, "receive dummy data %d", WT_DATA_SIZE);
	nres = wt_receive_dummy(WT_DATA_SIZE);
	if (nres < 0) {
		WT_LOGE(TAG, "receive data fail %d", nres);
		return -1;
	}

	/* Set softAP */
	WT_LOG(TAG, "start softAP");
	wifi_manager_softap_config_s softap_config;
	wm_get_softapinfo(&softap_config, WM_SOFTAP_SSID, WM_SOFTAP_PASSWORD, WM_SOFTAP_CHANNEL);
	res = wifi_manager_set_mode(SOFTAP_MODE, &softap_config);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "set softap fail %d", res);
		return -1;
	}

	/* Wait join event */
	WT_LOG(TAG, "wait join event");
	WM_TEST_WAIT;

	/*  receive data */
	WT_LOG(TAG, "receive data size %d", WT_DATA_SIZE);
	nres = wt_receive_dummy(WT_DATA_SIZE);
	if (nres < 0) {
		WT_LOGE(TAG, "receive data fail %d", nres);
		return -1;
	}

	/*  Wait leave event */
	WT_LOG(TAG, "wait leave event");
	WM_TEST_WAIT;

	/* Start STA */
	WT_LOG(TAG, "start STA");
	res = wifi_manager_set_mode(STA_MODE, NULL);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "start STA fail %d", res);
		return -1;
	}

	return 0;
}

static void run_wm_daemon(void *arg)
{
	wifi_manager_result_e wres = WIFI_MANAGER_SUCCESS;
	int cnt = 0;

	WT_LOG(TAG, "initialize wi-fi");
	wres = wifi_manager_init(&g_wifi_callbacks);
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "init fail");
		return;
	}

	while (1) {
		WT_LOG(TAG, "=== test count %d===", cnt);
		if (run_procedure(arg) < 0) {
			WT_LOGE(TAG, "error in main task");
			break;
		}
	}

	WT_LOG(TAG, "deinit wi-fi\n");
	wres = wifi_manager_deinit();
	if (wres != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "deinit fail");
		return;
	}

	return;
}

void wm_run_test_daemon(struct wt_options *opt)
{
	WM_AP_SSID = opt->ssid;
	WM_AP_PASSWORD = opt->password;
	WM_AP_AUTH = opt->auth_type;
	WM_AP_CRYPTO = opt->crypto_type;
	WM_SOFTAP_SSID = opt->softap_ssid;
	WM_SOFTAP_PASSWORD = opt->softap_password;
	WM_SOFTAP_CHANNEL = opt->softap_channel;

	if (0 != sem_init(&g_wm_sem, 0, 0)) {
		WT_LOGE(TAG, "sem init fail %d", errno);
		return;
	}

	run_wm_daemon(NULL);

	sem_destroy(&g_wm_sem);
}
