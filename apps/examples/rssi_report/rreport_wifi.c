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
#include <stdio.h>
#include <stdint.h>
#include <wifi_manager/wifi_manager.h>
#include "rreport_config.h"

#define WM_TEST_SIGNAL								\
	do {											\
		sem_post(&g_wm_sem);						\
		printf("T%d send signal\n", getpid());	\
	} while (0)

#define WM_TEST_WAIT								\
	do {											\
		printf("T%d wait signal\n", getpid());	\
		sem_wait(&g_wm_sem);                        \
	} while (0)



/*
 * Callbacks
 */
static void wm_sta_connected(wifi_manager_result_e);
static void wm_sta_disconnected(wifi_manager_disconnect_e);
static void wm_softap_sta_join(void);
static void wm_softap_sta_leave(void);
static void wm_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res);

static wifi_manager_cb_s wifi_callbacks = {
	wm_sta_connected,
	wm_sta_disconnected,
	wm_softap_sta_join,
	wm_softap_sta_leave,
	wm_scan_done,
};

static sem_t g_wm_sem = SEM_INITIALIZER(0);

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
	sleep(2);
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
		printf("WiFi AP SSID: %-25s, BSSID: %-20s, Rssi: %d, Auth: %d, Crypto: %d\n",
			   wifi_scan_iter->ssid, wifi_scan_iter->bssid, wifi_scan_iter->rssi,
			   wifi_scan_iter->ap_auth_type, wifi_scan_iter->ap_crypto_type);
		wifi_scan_iter = wifi_scan_iter->next;
	}
	WM_TEST_SIGNAL;
}

int rr_connect_ap(char *ssid, char *passwd)
{
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	wifi_manager_ap_config_s ap_config;
	memset(&ap_config, 0, sizeof(ap_config));

	strncpy(ap_config.ssid, ssid, WIFIMGR_SSID_LEN);
	ap_config.ssid_length = strlen(ssid);

	strncpy(ap_config.passphrase, passwd, WIFIMGR_PASSPHRASE_LEN);
	ap_config.passphrase_length = strlen(passwd);

	ap_config.ap_auth_type = RR_AP_AUTH;
	ap_config.ap_crypto_type = RR_AP_CRYPTO;

	res = wifi_manager_init(&wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		RR_ERR;
		return -1;
	}

	res = wifi_manager_connect_ap(&ap_config);
	if (res != WIFI_MANAGER_SUCCESS) {
		RR_ERR;
		return -1;
	} else {
		WM_TEST_WAIT;
	}

	return 0;
}

int rr_get_rssi(void)
{
	wifi_manager_info_s info;
	wifi_manager_result_e res = wifi_manager_get_info(&info);
	if (res != WIFI_MANAGER_SUCCESS) {
		RR_ERR;
		return -1;
	}

	return info.rssi;
}
