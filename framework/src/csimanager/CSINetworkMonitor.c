/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#include "csifw_log.h"
#include "CSINetworkMonitor.h"
#include <wifi_manager/wifi_manager.h>

static network_status_listener g_network_status_callback;

static void wm_cb_sta_connected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg);

static wifi_manager_cb_s g_wifi_callbacks = {
	wm_cb_sta_connected,
	wm_cb_sta_disconnected,
	NULL,
	NULL,
	NULL,
};

int network_monitor_init(network_status_listener network_status_callback)
{
	CSIFW_LOGD("Initializing network monitor with callback: %p", network_status_callback);
	
	if (wifi_manager_register_cb(&g_wifi_callbacks) != WIFI_MANAGER_SUCCESS) {
		CSIFW_LOGE("Failed to register WiFi manager callbacks");
		return 0;
	}
	
	g_network_status_callback = network_status_callback;
	CSIFW_LOGD("Network monitor initialized successfully");
	return 1;
}

int network_monitor_deinit(void)
{
	CSIFW_LOGD("Deinitializing network monitor");
	
	if (wifi_manager_unregister_cb(&g_wifi_callbacks) != WIFI_MANAGER_SUCCESS) {
		CSIFW_LOGE("Failed to unregister WiFi manager callbacks");
		return 0;
	}
	g_network_status_callback = NULL;
	CSIFW_LOGD("Network monitor deinitialized successfully");
	return 1;
}

int checkWifiConnection(void)
{
	wifi_manager_info_s wminfo;
	memset(&wminfo, 0, sizeof(wifi_manager_info_s));
	
	if (wifi_manager_get_info(&wminfo) != WIFI_MANAGER_SUCCESS) {
		CSIFW_LOGE("Failed to get WiFi manager info");
		return 0;
	}
	
	if (wminfo.status == AP_CONNECTED) {
		CSIFW_LOGI("WiFi connected");
	} else if (wminfo.status == AP_DISCONNECTED) {
		CSIFW_LOGI("WiFi disconnected");
		return 0;
	} else {
		CSIFW_LOGE("Unknown WiFi status");
		return 0;
	}
	return 1;
}

static void wm_cb_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	CSIFW_LOGI("WiFi connected - BSSID: %02x:%02x:%02x:%02x:%02x:%02x, Result: %d",
		msg.bssid[0], msg.bssid[1], msg.bssid[2],
		msg.bssid[3], msg.bssid[4], msg.bssid[5],
		msg.res);
		
	if (g_network_status_callback) {
		CSIFW_LOGD("Invoking network status callback with WIFI_CONNECTED");
		g_network_status_callback(WIFI_CONNECTED);
	} else {
		CSIFW_LOGE("Network status callback is NULL");
	}
}

static void wm_cb_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
	CSIFW_LOGI("WiFi disconnected - Result: %d, Reason: %d", 
		msg.res, msg.reason);
		
	if (g_network_status_callback) {
		CSIFW_LOGD("Invoking network status callback with WIFI_DISCONNECTED");
		g_network_status_callback(WIFI_DISCONNECTED);
	} else {
		CSIFW_LOGE("Network status callback is NULL");
	}
}
