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
#include <wifi_manager/wifi_manager.h>
#include <stress_tool/st_perf.h>

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


/*
 * callbacks
 */
static void wm_sta_connected(wifi_manager_result_e);
static void wm_sta_disconnected(wifi_manager_disconnect_e);
static void wm_softap_sta_join(void);
static void wm_softap_sta_leave(void);
static void wm_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res);

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

/*
 * Callback
 */
void wm_sta_connected(wifi_manager_result_e res)
{
	printf(" T%d --> %s res(%d)\n", getpid(), __FUNCTION__, res);
	WM_TEST_SIGNAL;
}


void wm_sta_disconnected(wifi_manager_disconnect_e disconn)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
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

static void wm_get_apinfo(wifi_manager_ap_config_s *apconfig)
{
	strncpy(apconfig->ssid, WM_AP_SSID, sizeof(WM_AP_SSID));
	apconfig->ssid_length = strlen(WM_AP_SSID);
	apconfig->ap_auth_type = WM_AP_AUTH;
	if (WM_AP_AUTH != WIFI_MANAGER_AUTH_OPEN) {
		strncpy(apconfig->passphrase, WM_AP_PASSWORD, sizeof(WM_AP_PASSWORD));
		apconfig->passphrase_length = strlen(WM_AP_PASSWORD);
		apconfig->ap_crypto_type = WM_AP_CRYPTO;
	}
}

static void wm_get_softapinfo(wifi_manager_softap_config_s *ap_config)
{
	strncpy(ap_config->ssid, WM_SOFTAP_SSID, sizeof(WM_SOFTAP_SSID));
	strncpy(ap_config->passphrase, WM_SOFTAP_PASSWORD, sizeof(WM_SOFTAP_PASSWORD));
	ap_config->channel = WM_SOFTAP_CHANNEL;
}
/*
 * station join test
 */
TEST_SETUP(sta_join)
{
	ST_START_TEST;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	ST_END_TEST;
}

TEST_TEARDOWN(sta_join)
{
	ST_START_TEST;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_disconnect_ap());
	WM_TEST_WAIT;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(sta_join)
{
	ST_START_TEST;
	wifi_manager_ap_config_s apconfig;
	wm_get_apinfo(&apconfig);
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_connect_ap(&apconfig));
	WM_TEST_WAIT;

	ST_END_TEST;
}


/*
 * station scan test
 */
TEST_SETUP(sta_scan)
{
	ST_START_TEST;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	ST_END_TEST;
}

TEST_TEARDOWN(sta_scan)
{
	ST_START_TEST;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(sta_scan)
{
	ST_START_TEST;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_scan_ap());
	WM_TEST_WAIT;
	ST_END_TEST;
}

/*
 * station leave test
 */
TEST_SETUP(sta_leave)
{
	ST_START_TEST;

	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_ap_config_s apconfig;
	wm_get_apinfo(&apconfig);
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_connect_ap(&apconfig));
	WM_TEST_WAIT;

	ST_END_TEST;
}

TEST_TEARDOWN(sta_leave)
{
	ST_START_TEST;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(sta_leave)
{
	ST_START_TEST;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_disconnect_ap());
	WM_TEST_WAIT;

	ST_END_TEST;
}

/*
 * SoftAP Start
 */
TEST_SETUP(softap_start)
{
	ST_START_TEST;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	ST_END_TEST;
}


TEST_TEARDOWN(softap_start)
{
	ST_START_TEST;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(softap_start)
{
	ST_START_TEST;
	wifi_manager_softap_config_s ap_config;
	wm_get_softapinfo(&ap_config);
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(SOFTAP_MODE, &ap_config));
	ST_END_TEST;
}


/*
 * SoftAP Scan
 */
TEST_SETUP(softap_scan)
{
	ST_START_TEST;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_softap_config_s ap_config;
	wm_get_softapinfo(&ap_config);
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(SOFTAP_MODE, &ap_config));
	ST_END_TEST;
}


TEST_TEARDOWN(softap_scan)
{
	ST_START_TEST;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(softap_scan)
{
	ST_START_TEST;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_scan_ap());
	WM_TEST_WAIT;
	ST_END_TEST;
}


/*
 * SoftAP Stop
 */
TEST_SETUP(softap_stop)
{
	ST_START_TEST;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_init(&g_wifi_callbacks));
	wifi_manager_softap_config_s ap_config;
	wm_get_softapinfo(&ap_config);
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_set_mode(SOFTAP_MODE, &ap_config));
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
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

ST_SET_SMOKE_TAIL(WM_TEST_TRIAL, 10000000, "station join", sta_join);
ST_SET_SMOKE(WM_TEST_TRIAL, 2000000, "station leave", sta_leave, sta_join);
ST_SET_SMOKE(WM_TEST_TRIAL, 10000000, "station scan", sta_scan, sta_leave);
ST_SET_SMOKE(WM_TEST_TRIAL, 5000000, "softap start", softap_start, sta_scan);
ST_SET_SMOKE(WM_TEST_TRIAL, 5000000, "softap stop", softap_stop, softap_start);
ST_SET_PACK(wifi, softap_stop);

void wm_run_stress_test(void *arg)
{
	ST_RUN_TEST(wifi);
	ST_RESULT_TEST(wifi);
}
