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
#include <wifi_manager/wifi_manager.h>
#include "wm_test.h"

/*
 * Macro
 */
#define WM_AP_SSID         CONFIG_WIFIMANAGER_TEST_AP_SSID
#define WM_AP_PASSWORD     CONFIG_WIFIMANAGER_TEST_AP_PASSPHRASE
#define WM_AP_AUTH         CONFIG_WIFIMANAGER_TEST_AP_AUTHENTICATION
#define WM_AP_CRYPTO       CONFIG_WIFIMANAGER_TEST_AP_CRYPTO
#define WM_TEST_TRIAL	   CONFIG_WIFIMANAGER_TEST_TRIAL
#define WM_SOFTAP_SSID     CONFIG_WIFIMANAGER_TEST_SOFTAP_SSID
#define WM_SOFTAP_PASSWORD CONFIG_WIFIMANAGER_TEST_SOFTAP_PASSWORD
#define WM_SOFTAP_CHANNEL  CONFIG_WIFIMANAGER_TEST_SOFTAP_CHANNEL

#define WIFIMGR_SSID ""
#define WIFIMGR_PWD ""
#define WIFIMGR_AUTH WIFI_MANAGER_AUTH_WPA2_PSK
#define WIFIMGR_CRYPTO WIFI_MANAGER_CRYPTO_AES


#define WM_TEST_SIGNAL										\
	do {													\
		pthread_mutex_lock(&g_wm_mutex);					\
		printf("T%d send signal\n", getpid());	\
		pthread_cond_signal(&g_wm_cond);					\
		pthread_mutex_unlock(&g_wm_mutex);					\
	} while (0)

#define WM_TEST_WAIT								\
	do {											\
		pthread_mutex_lock(&g_wm_mutex);			\
		printf(" T%d wait signal\n", getpid());		\
		pthread_cond_wait(&g_wm_cond, &g_wm_mutex);	\
		pthread_mutex_unlock(&g_wm_mutex);			\
	} while (0)

#define WM_CONN_FAIL 1
#define WM_CONN_SUCCESS 2

/*
 * callbacks
 */
static void wm_sta_connected(wifi_manager_result_e);
static void wm_sta_disconnected(wifi_manager_disconnect_e);
static void wm_softap_sta_join(void);
static void wm_softap_sta_leave(void);
static void wm_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res);

/*
 * State
 */
static void run_init(void *arg);
static int run_connecting(wifi_manager_ap_config_s *ap_config);
static int run_connected(void);
static int run_reconnecting(void);
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

static pthread_mutex_t g_wm_mutex = PTHREAD_MUTEX_INITIALIZER;;
static pthread_cond_t g_wm_cond = PTHREAD_COND_INITIALIZER;
static int g_conn_res = 0;
/*
 * Callback
 */
void wm_sta_connected(wifi_manager_result_e res)
{
	printf(" T%d --> %s res(%d)\n", getpid(), __FUNCTION__, res);
	if (WIFI_MANAGER_SUCCESS == res) {
		g_conn_res = WM_CONN_SUCCESS;
	} else {
		g_conn_res = WM_CONN_FAIL;
	}
	WM_TEST_SIGNAL;
}


void wm_sta_disconnected(wifi_manager_disconnect_e disconn)
{
	printf(" T%d --> %s %d\n", getpid(), __FUNCTION__, disconn);
	g_conn_res = WM_CONN_FAIL;
	WM_TEST_SIGNAL;
}


void wm_softap_sta_join(void)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
	WM_TEST_SIGNAL;
}


void wm_softap_sta_leave(void)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
	WM_TEST_SIGNAL;
}


void wm_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (scan_result == NULL) {
		WM_TEST_SIGNAL;
		return;
	}
	wifi_manager_scan_info_s *wifi_scan_iter = *scan_result;
	while (wifi_scan_iter != NULL) {
		printf("WiFi AP SSID: %-20s, WiFi AP BSSID: %-20s, WiFi Rssi: %d, AUTH: %d, CRYPTO: %d\n",
			   wifi_scan_iter->ssid, wifi_scan_iter->bssid, wifi_scan_iter->rssi,
			   wifi_scan_iter->ap_auth_type, wifi_scan_iter->ap_crypto_type);
		wifi_scan_iter = wifi_scan_iter->next;
	}
	WM_TEST_SIGNAL;
}

static void print_wifi_ap_profile(wifi_manager_ap_config_s *config, char *title)
{
	printf("====================================\n");
	if (title) {
		printf("%s\n", title);
	}
	printf("------------------------------------\n");
	printf("SSID: %s\n", config->ssid);
	printf("security type (%d) (%d)\n", config->ap_auth_type, config->ap_crypto_type);
	printf("====================================\n");
}

#define WM_ERROR(res) printf("[WMERR] code(%d), %s\t%s:%d\n",\
							 res, __FUNCTION__, __FILE__, __LINE__)

static void run_init(void *arg)
{
	wifi_manager_result_e res = wifi_manager_init(&g_wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		WM_ERROR(res);
		return;
	}

	int cnt_auto_connect = 0;

	/* Set AP Configuration */
	struct options *ap_info = (struct options *)arg;
	wifi_manager_ap_config_s apconfig;
	strncpy(apconfig.ssid, ap_info->ssid, WIFIMGR_SSID_LEN);
	apconfig.ssid_length = strlen(ap_info->ssid);
	apconfig.ssid[WIFIMGR_SSID_LEN] = '\0';
	apconfig.ap_auth_type = ap_info->auth_type;
	if (ap_info->auth_type != WIFI_MANAGER_AUTH_OPEN) {
		strncpy(apconfig.passphrase, ap_info->password, WIFIMGR_PASSPHRASE_LEN);
		apconfig.passphrase[WIFIMGR_PASSPHRASE_LEN] = '\0';
		apconfig.passphrase_length = strlen(ap_info->password);
		apconfig.ap_crypto_type = ap_info->crypto_type;
	} else {
		apconfig.passphrase[0] = '\0';
		apconfig.passphrase_length = 0;
		apconfig.ap_crypto_type = ap_info->crypto_type;
	}

	print_wifi_ap_profile(&apconfig, "Connecting AP Info");

	/*  Run Auto Test */
	int state = 1;
	while (1) {
		if (state == 1) {
			state = run_connecting(&apconfig);
		} else if (state == 2) {
			cnt_auto_connect++;
			state = run_connected();
		} else if (state == 3) {
			state = run_reconnecting();
		}
	}

	printf("terminate program total (%d)\n", cnt_auto_connect);
	return;
}


static void wm_get_info(wifi_manager_ap_config_s *arg)
{
	printf("T%d -->%s\n", getpid(), __FUNCTION__);
	wifi_manager_ap_config_s apconfig;
	wifi_manager_result_e res = wifi_manager_get_config(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("Get AP configuration failed\n");
		return;
	}
	print_wifi_ap_profile(&apconfig, "Stored Wi-Fi Information");
}

static int run_connecting(wifi_manager_ap_config_s *ap_config)
{
	printf("-->%s\n", __FUNCTION__);

	wifi_manager_result_e res = wifi_manager_connect_ap(ap_config);
	if (res != WIFI_MANAGER_SUCCESS) {
		WM_ERROR(res);
		printf("wait 1 second\n");
		sleep(1);
		return 1;
	}

	WM_TEST_WAIT;
	if (g_conn_res == WM_CONN_FAIL) {
		//wm_get_info(ap_config);
		return 1; // connect again
	} else if (g_conn_res == WM_CONN_SUCCESS) {
		return 2; // connected, wait disconnect message
	} else {
		printf("program is corrupted %s\n", __FUNCTION__);
		assert(0);
	}
	return 0;
}

static int run_connected(void)
{
	printf("-->%s\n", __FUNCTION__);
	WM_TEST_WAIT;
	if (g_conn_res == WM_CONN_FAIL) {
		return 1;
	} else {
		printf("program is corrupted %s\n", __FUNCTION__);
		assert(0);
	}
	return 0;
}

static int run_reconnecting(void)
{
	printf("-->%s\n", __FUNCTION__);
	WM_TEST_WAIT;
	if (g_conn_res == WM_CONN_SUCCESS) {
		return 2;
	} else {
		printf("program is corrupted %s\n", __FUNCTION__);
		assert(0);
	}
	return 0;
}

void wm_test_on_off(void *arg)
{
	run_init(arg);
}
