/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <errno.h>
#include <wifi_manager/wifi_manager.h>
#include <netutils/netlib.h>
#include <limits.h>
#include "wm_test.h"
#include "wm_test_log.h"
#include "wm_test_utils.h"

static sem_t g_wm_sem;
static struct wt_options *g_opt;
static char g_bssid[20];
static char g_connected_ap_mac[20];

#define TAG "[WTCR]"
#define BSSID_LEN 20

#define WM_TEST_SIGNAL              \
	do {                            \
		sem_post(&g_wm_sem);        \
		WT_LOG(TAG, "send signal"); \
	} while (0)

#define WM_TEST_WAIT                \
	do {                            \
		WT_LOG(TAG, "wait signal"); \
		sem_wait(&g_wm_sem);       \
	} while (0)


static void _wt_sta_connected(wifi_manager_cb_msg_s msg, void *arg);
static void _wt_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg);
static void _wt_scan_done(wifi_manager_cb_msg_s msg, void *arg);

static wifi_manager_cb_s g_wifi_callbacks = {
	_wt_sta_connected,
	_wt_sta_disconnected,
	NULL,
	NULL,
	_wt_scan_done,
};

void _wt_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->res(%d)", msg.res);
	WT_LOG(TAG, "bssid %02x:%02x:%02x:%02x:%02x:%02x",
		msg.bssid[0], msg.bssid[1],
		msg.bssid[2], msg.bssid[3],
		msg.bssid[4], msg.bssid[5]);

	if (WIFI_MANAGER_SUCCESS == msg.res) {
		WT_LOG(TAG, "Station Connected");
		snprintf(g_connected_ap_mac, sizeof(g_connected_ap_mac), "%02x:%02x:%02x:%02x:%02x:%02x",
			msg.bssid[0], msg.bssid[1],
			msg.bssid[2], msg.bssid[3],
			msg.bssid[4], msg.bssid[5]);
	} else {
		WT_LOGE(TAG, "Station Not Connected");
	}
	WM_TEST_SIGNAL;
}

void _wt_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d) reason %d", msg.res, msg.reason);
	WM_TEST_SIGNAL;
}

void _wt_scan_done(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "-->");
	if (msg.res != WIFI_MANAGER_SUCCESS || msg.scanlist == NULL) {
		WM_TEST_SIGNAL;
		return;
	}
	wt_print_scanlist(msg.scanlist);

	/* Make sure you copy the scan results into a local data structure.
	 * It will be deleted as soon as you exit this function.
	 */
	wifi_manager_scan_info_s *wifi_scan_iter = msg.scanlist;
	int best_rssi = INT_MIN;
	while (wifi_scan_iter != NULL) {
		if (strncmp(wifi_scan_iter->ssid, g_opt->ssid, WIFIMGR_SSID_LEN) == 0) {
			if (best_rssi < wifi_scan_iter->rssi) {
				best_rssi = wifi_scan_iter->rssi;
				strncpy(g_bssid, wifi_scan_iter->bssid, strlen(wifi_scan_iter->bssid));
			}
		}
		wifi_scan_iter = wifi_scan_iter->next;
	}
	WM_TEST_SIGNAL;
}

static wifi_manager_result_e wm_connect_by_rssi_init(void)
{
	WT_ENTER;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	res = wifi_manager_init(&g_wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "Failed to init wifi_manager");
		return res;
	}
	WT_LEAVE;
	return res;
}

static wifi_manager_result_e wm_connect_by_rssi_deinit(void)
{
	WT_ENTER;
	wifi_manager_result_e res = wifi_manager_deinit();
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "WiFi Manager failed to stop");
		return res;
	}
	WT_LEAVE;
	return res;
}

static wifi_manager_result_e _wt_scan(void)
{
	WT_ENTER;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	res = wifi_manager_scan_ap(NULL);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "Failed to scan");
		return res;
	}
	WM_TEST_WAIT; // wait the scan result
	WT_LEAVE;
	return res;
}

static wifi_manager_result_e _wt_connect(void *arg)
{
	WT_ENTER;
	struct wt_options *ap_info = (struct wt_options *)arg;
	wifi_manager_ap_config_s apconfig;
	strncpy(apconfig.ssid, ap_info->ssid, WIFIMGR_SSID_LEN);
	apconfig.ssid_length = strlen(ap_info->ssid);
	if (apconfig.ssid_length >= WIFIMGR_SSID_LEN) {
		apconfig.ssid[WIFIMGR_SSID_LEN] = '\0';
	}
	apconfig.ap_auth_type = ap_info->auth_type;
	if (ap_info->auth_type != WIFI_MANAGER_AUTH_OPEN) {
		strncpy(apconfig.passphrase, ap_info->password, WIFIMGR_PASSPHRASE_LEN);
		apconfig.passphrase_length = strlen(ap_info->password);
		if (apconfig.passphrase_length >= WIFIMGR_PASSPHRASE_LEN) {
			apconfig.passphrase[WIFIMGR_PASSPHRASE_LEN] = '\0';
		}
		apconfig.ap_crypto_type = ap_info->crypto_type;
	} else {
		apconfig.passphrase[0] = '\0';
		apconfig.passphrase_length = 0;
		apconfig.ap_crypto_type = ap_info->crypto_type;
	}
	wt_print_wifi_ap_profile(&apconfig, "Connecting AP Info");
	wifi_manager_result_e res = wifi_manager_connect_ap(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "AP connect failed");
		return res;
	}
	/* Wait for DHCP connection */
	WT_LOG(TAG, "wait join done");
	WM_TEST_WAIT;
	WT_LEAVE;
	return res;
}

static void _wt_disconnect()
{
	WT_ENTER;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	/* Disconnect AP */
	res = wifi_manager_disconnect_ap();
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "Failed to disconnect ap (%d)", res);
		return;
	}
	WM_TEST_WAIT;
	WT_LEAVE;
}


static void wm_run_connect_by_rssi(void *arg)
{
	WT_ENTER;
	wifi_manager_result_e ret = WIFI_MANAGER_SUCCESS;
	g_opt = (struct wt_options *)arg;
	int passed = 0, failed = 0;
	for (int i = 0; i < g_opt->repeat; i++) {
		WT_LOG(TAG, "====== Iteration %d starts ======", i + 1);
		ret = wm_connect_by_rssi_init();
		if (ret != WIFI_MANAGER_SUCCESS) {
			WT_LOGE(TAG, "wm_connect_by_rssi_init failed");
			failed++;
			sleep(1); //sleep for 1 sec before next iteration
			goto next_iteration;
		}
		memset(g_bssid, '\0', sizeof(g_bssid));
		memset(g_connected_ap_mac, '\0', sizeof(g_connected_ap_mac));
		int retry_count = 5;
		while (retry_count) {
			_wt_scan();
			if (g_bssid[0] != '\0')
				break;
			// scan till g_bssid get filled or retry count is not got expired
			sleep(1); // retry after 1 sec sleep
			retry_count--;
		}
		if (0 == retry_count || g_bssid[0] == '\0') {
			WT_LOGE(TAG, "Scan failed even after multiple retry");
			failed++;
			ret = wm_connect_by_rssi_deinit();
			if (ret != WIFI_MANAGER_SUCCESS) {
				WT_LOGE(TAG, "wm_connect_by_rssi_deinit failed. so exit");
				break;
			}
			goto next_iteration;
		}

		// connect to ap
		retry_count = 5;
		while (retry_count) {
			// connect till g_connected_ap_mac get filled or retry count is not got expired
			_wt_connect(arg);
			if (g_connected_ap_mac[0] != '\0')
				break;
			sleep(1); // retry after 1 sec sleep
			retry_count--;
		}
		if (0 == retry_count) {
			WT_LOGE(TAG, "Connect ap failed even after multiple retry");
			failed++;
			ret = wm_connect_by_rssi_deinit();
			if (ret != WIFI_MANAGER_SUCCESS) {
				WT_LOGE(TAG, "wm_connect_by_rssi_deinit failed. so exit");
				break;
			}
			goto next_iteration;
		} 
		WT_LOG(TAG, "Mac with best rssi value in scan for given ssid %s", g_bssid);
		WT_LOG(TAG, "Connected ap Mac %s", g_connected_ap_mac);
		if (strncmp(g_bssid, g_connected_ap_mac, BSSID_LEN) == 0) {
			WT_LOG(TAG, "ap is connected to best rssi for given ssid");
			passed++;
		} else {
			WT_LOG(TAG, "ap is not connected to best rssi for given ssid");
			failed++;
		}
		_wt_disconnect();
		ret = wm_connect_by_rssi_deinit();
		if (ret != WIFI_MANAGER_SUCCESS) {
			WT_LOGE(TAG, "wm_connect_by_rssi_deinit failed. so exit");
			break;
		}
	next_iteration:
		WT_LOG(TAG, "====== Iteration %d ends ======", i + 1);
	}
	printf("============================================================================\n");
	printf("TEST CASE:Best Rssi Connect Check\n");
	printf("----------------------------------------------------------------------------\n");
	printf("TotalCount - Pass - Fail\n");
	printf("   %-11d%-8d%-5d\n", g_opt->repeat, passed, failed);
	printf("----------------------------------------------------------------------------\n");
	printf("============================================================================\n");
	WT_LEAVE;
}

void wm_connectbyrssi_test(void *arg)
{
	WT_ENTER;
	int res = sem_init(&g_wm_sem, 0, 0);
	if (res < 0) {
		WT_LOGE(TAG, "Failed to initialize semaphore %d", errno);
		return;
	}
	wm_run_connect_by_rssi(arg);
	sem_destroy(&g_wm_sem);
	WT_LEAVE;
}
