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

#include <stdio.h>
#include <wifi_manager/wifi_manager.h>
#include <stress_tool/st_perf.h>

/*
 * Macro
 */
#define WICTL_AP_SSID         CONFIG_WICTL_STRESS_AP_SSID
#define WICTL_AP_PASSWORD     CONFIG_WICTL_STRESS_AP_PASSPHRASE
#define WICTL_AP_AUTH         CONFIG_WICTL_STRESS_AP_AUTHENTICATION
#define WICTL_AP_CRYPTO       CONFIG_WICTL_STRESS_AP_CRYPTO
#define WICTL_TEST_TRIAL      CONFIG_WICTL_STRESS_TRIAL
#define WICTL_SOFTAP_SSID     CONFIG_WICTL_STRESS_SOFTAP_SSID
#define WICTL_SOFTAP_PASSWORD CONFIG_WICTL_STRESS_SOFTAP_PASSWORD
#define WICTL_SOFTAP_CHANNEL  CONFIG_WICTL_STRESS_SOFTAP_CHANNEL


#define WICTL_TEST_SIGNAL										\
	do {													\
		pthread_mutex_lock(&g_wictl_mutex);					\
		printf("T%d send signal\n", getpid());	\
		pthread_cond_signal(&g_wictl_cond);					\
		pthread_mutex_unlock(&g_wictl_mutex);					\
	} while (0)

#define WICTL_TEST_WAIT								\
	do {											\
		pthread_mutex_lock(&g_wictl_mutex);			\
		printf(" T%d wait signal\n", getpid());		\
		pthread_cond_wait(&g_wictl_cond, &g_wictl_mutex);	\
		pthread_mutex_unlock(&g_wictl_mutex);			\
	} while (0)


/*
 * callbacks
 */
static void wictl_sta_connected(wifi_manager_result_e);
static void wictl_sta_disconnected(wifi_manager_disconnect_e);
static void wictl_softap_sta_join(void);
static void wictl_softap_sta_leave(void);
static void wictl_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res);

/*
 * Global
 */
static wifi_manager_cb_s g_wifi_callbacks = {
	wictl_sta_connected,
	wictl_sta_disconnected,
	wictl_softap_sta_join,
	wictl_softap_sta_leave,
	wictl_scan_done,
};

static pthread_mutex_t g_wictl_mutex = PTHREAD_MUTEX_INITIALIZER;;
static pthread_cond_t g_wictl_cond = PTHREAD_COND_INITIALIZER;

/*
 * Callback
 */
void wictl_sta_connected(wifi_manager_result_e res)
{
	printf(" T%d --> %s res(%d)\n", getpid(), __FUNCTION__, res);
	WICTL_TEST_SIGNAL;
}


void wictl_sta_disconnected(wifi_manager_disconnect_e disconn)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
	WICTL_TEST_SIGNAL;
}


void wictl_softap_sta_join(void)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
	WICTL_TEST_SIGNAL;
}


void wictl_softap_sta_leave(void)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
	WICTL_TEST_SIGNAL;
}


void wictl_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (scan_result == NULL) {
		WICTL_TEST_SIGNAL;
		return;
	}
	wifi_manager_scan_info_s *wifi_scan_iter = *scan_result;
	while (wifi_scan_iter != NULL) {
		printf("WiFi AP SSID: %-20s, WiFi AP BSSID: %-20s, WiFi Rssi: %d\n",
			   wifi_scan_iter->ssid, wifi_scan_iter->bssid, wifi_scan_iter->rssi);
		wifi_scan_iter = wifi_scan_iter->next;
	}
	WICTL_TEST_SIGNAL;
}

static void wictl_get_apinfo(wifi_manager_ap_config_s *apconfig)
{
	strncpy(apconfig->ssid, WICTL_AP_SSID, sizeof(WICTL_AP_SSID));
	apconfig->ssid_length = strlen(WICTL_AP_SSID);
	apconfig->ap_auth_type = WICTL_AP_AUTH;
	if (WICTL_AP_AUTH != WIFI_MANAGER_AUTH_OPEN) {
		strncpy(apconfig->passphrase, WICTL_AP_PASSWORD, sizeof(WICTL_AP_PASSWORD));
		apconfig->passphrase_length = strlen(WICTL_AP_PASSWORD);
		apconfig->ap_crypto_type = WICTL_AP_CRYPTO;
	}
}

static void wictl_get_softapinfo(wifi_manager_softap_config_s *ap_config)
{
	strncpy(ap_config->ssid, WICTL_SOFTAP_SSID, sizeof(WICTL_SOFTAP_SSID));
	strncpy(ap_config->passphrase, WICTL_SOFTAP_PASSWORD, sizeof(WICTL_SOFTAP_PASSWORD));
	ap_config->channel = WICTL_SOFTAP_CHANNEL;
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
	WICTL_TEST_WAIT;
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_deinit());
	ST_END_TEST;
}

TEST_F(sta_join)
{
	ST_START_TEST;
	wifi_manager_ap_config_s apconfig;
	wictl_get_apinfo(&apconfig);
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_connect_ap(&apconfig));
	WICTL_TEST_WAIT;

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
	WICTL_TEST_WAIT;
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
	wictl_get_apinfo(&apconfig);
	ST_EXPECT(WIFI_MANAGER_SUCCESS, wifi_manager_connect_ap(&apconfig));
	WICTL_TEST_WAIT;

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
	WICTL_TEST_WAIT;

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
	wictl_get_softapinfo(&ap_config);
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
	wictl_get_softapinfo(&ap_config);
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
	WICTL_TEST_WAIT;
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
	wictl_get_softapinfo(&ap_config);
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

ST_SET_SMOKE_TAIL(WICTL_TEST_TRIAL, 10000000, "station join", sta_join);
ST_SET_SMOKE(WICTL_TEST_TRIAL, 2000000, "station leave", sta_leave, sta_join);
ST_SET_SMOKE(WICTL_TEST_TRIAL, 10000000, "station scan", sta_scan, sta_leave);
ST_SET_SMOKE(WICTL_TEST_TRIAL, 5000000, "softap start", softap_start, sta_scan);
ST_SET_SMOKE(WICTL_TEST_TRIAL, 5000000, "softap stop", softap_stop, softap_start);
ST_SET_PACK(wifi, softap_stop);

void wictl_run_stress_test(void *arg)
{
	ST_RUN_TEST(wifi);
	ST_RESULT_TEST(wifi);
}
