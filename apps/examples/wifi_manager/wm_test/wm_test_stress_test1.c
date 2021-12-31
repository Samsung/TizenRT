
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

#include <pthread.h>
#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <queue.h>
#include <wifi_manager/wifi_manager.h>
#include <stress_tool/st_perf.h>
#include "wm_test.h"
#include "wm_test_log.h"
#include "wm_test_utils.h"

/* Macro*/
#define WM_TEST_TRIAL	   CONFIG_WIFIMANAGER_TEST_TRIAL
#define TAG "[WTS1]"

static char *WM_AP_SSID;
static char *WM_AP_PASSWORD;
static wifi_manager_ap_auth_type_e WM_AP_AUTH;
static wifi_manager_ap_crypto_type_e WM_AP_CRYPTO;

#define WM_TEST_SIGNAL										\
	do {													\
		pthread_mutex_lock(&g_wm_mutex);					\
		WT_LOG(TAG, "send signal");	\
		pthread_cond_signal(&g_wm_cond);					\
		pthread_mutex_unlock(&g_wm_mutex);					\
	} while (0)

#define WM_TEST_WAIT								\
	do {											\
		pthread_mutex_lock(&g_wm_mutex);			\
		WT_LOG(TAG, "wait signal");		\
		pthread_cond_wait(&g_wm_cond, &g_wm_mutex);	\
		pthread_mutex_unlock(&g_wm_mutex);			\
	} while (0)

/* callbacks*/
static void wm_sta_connected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_softap_sta_joined(wifi_manager_cb_msg_s msg, void *arg);
static void wm_softap_sta_left(wifi_manager_cb_msg_s msg, void *arg);
static void wm_scan_ap_done(wifi_manager_cb_msg_s msg, void *arg);

/* Global*/
static wifi_manager_cb_s g_wifi_callbacks = {
	wm_sta_connected,
	wm_sta_disconnected,
	wm_softap_sta_joined,
	wm_softap_sta_left,
	wm_scan_ap_done
};

static pthread_mutex_t g_wm_mutex;
static pthread_cond_t g_wm_cond;

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

void wm_softap_sta_joined(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->");
}

void wm_softap_sta_left(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->");
}

void wm_scan_ap_done(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->");
	WT_LOG(TAG, " result (%d) info pointer(%p)", msg.res, msg.scanlist);
}

/*
 * station join test
 */
TEST_SETUP(init)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	ST_END_TEST;
}

TEST_TEARDOWN(init)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_F(init)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

/**
 * Description: call deinit() before receiving connect done event.
 */
TEST_SETUP(connect)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_ap_config_s apconfig;
	wm_get_apinfo(&apconfig, WM_AP_SSID, WM_AP_PASSWORD, WM_AP_AUTH, WM_AP_CRYPTO);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_connect_ap(&apconfig));
	ST_END_TEST;
}

TEST_TEARDOWN(connect)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_F(connect)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

/**
 * Description: call deinit() after receiving connect done event.
 */
TEST_SETUP(connect_done)
{
	ST_START_TEST;

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_ap_config_s apconfig;
	wm_get_apinfo(&apconfig, WM_AP_SSID, WM_AP_PASSWORD, WM_AP_AUTH, WM_AP_CRYPTO);

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_connect_ap(&apconfig));
	WM_TEST_WAIT;

	ST_END_TEST;
}

TEST_TEARDOWN(connect_done)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_F(connect_done)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

/**
 * Description: call deinit() after disconnect() is executed
 */
TEST_SETUP(disconnect)
{
	ST_START_TEST;

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_ap_config_s apconfig;
	wm_get_apinfo(&apconfig, WM_AP_SSID, WM_AP_PASSWORD, WM_AP_AUTH, WM_AP_CRYPTO);
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_connect_ap(&apconfig));
	WM_TEST_WAIT;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_disconnect_ap());
	ST_END_TEST;
}

TEST_TEARDOWN(disconnect)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_F(disconnect)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

/**
 * Description: call deinit() after scan() is executed
 */
TEST_SETUP(scan)
{
	ST_START_TEST;

	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_scan_ap(NULL));

	ST_END_TEST;
}

TEST_TEARDOWN(scan)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_F(scan)
{
	ST_START_TEST;
	ST_EXPECT_EQ(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

static int _wm_initialize_signal(void)
{
	int status = pthread_cond_init(&g_wm_cond, NULL);
	if (status != 0) {
		printf("start_thread: ERROR pthread_cond_init failed, status=%d\n", status);
	}
	status = pthread_mutex_init(&g_wm_mutex, NULL);
	if (status != 0) {
		printf("start_thread: ERROR pthread_mutex_init failed, status=%d\n", status);
	}
	return 0;
}

static int _wm_deinitialize_signal(void)
{
	pthread_cond_destroy(&g_wm_cond);
	pthread_mutex_destroy(&g_wm_mutex);
	return 0;
}

void wm_run_stress_test1(struct wt_options *opt)
{
	WM_AP_SSID = opt->ssid;
	WM_AP_PASSWORD = opt->password;
	WM_AP_AUTH = opt->auth_type;
	WM_AP_CRYPTO = opt->crypto_type;

	_wm_initialize_signal();
	ST_SET_PACK(wifi);

	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 10000000, "init", init);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 2000000, "connect", connect);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 10000000, "connect_done", connect_done);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 5000000, "disconnect", disconnect);
	ST_SET_SMOKE(wifi, WM_TEST_TRIAL, 5000000, "scan", scan);

	ST_RUN_TEST(wifi);
	ST_RESULT_TEST(wifi);
	_wm_deinitialize_signal();
}
