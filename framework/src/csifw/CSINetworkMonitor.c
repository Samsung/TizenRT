/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

#include "include/CSINetworkMonitor.h"
#include "include/csifw_log.h"
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

CSIFW_RES network_monitor_init(network_status_listener network_status_callback)
{
	CSIFW_RES res = CSIFW_OK;
	//regsiter callback
	if (wifi_manager_register_cb(&g_wifi_callbacks)!= WIFI_MANAGER_SUCCESS)
	{
		CSIFW_LOGE("[network_monitor_init] wifi_manager_register_cb fail");
		return CSIFW_ERROR;
	}
	g_network_status_callback = network_status_callback;
	CSIFW_LOGD("network monitor init done");
	return res;
}

CSIFW_RES network_monitor_deinit(void)
{
	CSIFW_LOGD("Deregistering network_monitor");

	if (wifi_manager_unregister_cb(&g_wifi_callbacks)!= WIFI_MANAGER_SUCCESS)
	{
		CSIFW_LOGE("[network_monitor_deinit] wifi_manager_unregister_cb fail");
		return CSIFW_ERROR;
	}
	CSIFW_LOGD("network monitor deinit done");
	return CSIFW_OK;
}


CSIFW_RES checkWifiConnection(void)
{
	CSIFW_LOGD("Checking WIFI connection . . . . . .");
	wifi_manager_info_s wminfo;
	//get info from wifi manager
	if (wifi_manager_get_info(&wminfo)!= WIFI_MANAGER_SUCCESS) {
		CSIFW_LOGE("get info from wifi manager fail");
		return CSIFW_ERROR;
	}
	// check connection status
	if (wminfo.status == AP_CONNECTED) {
		CSIFW_LOGD("WIFI is connected");
	} else if (wminfo.status == AP_DISCONNECTED) {
		CSIFW_LOGE("WIFI is not connected");
		return CSIFW_ERROR_WIFI_NOT_CONNECTED;
	} else {
		CSIFW_LOGE("WIFI status is unknown");
		return CSIFW_ERROR;
	}
	CSIFW_LOGI("[CSIFW_NM] WIFI CONNECTION CHECK SUCCESS");
	return CSIFW_OK;
}

static void wm_cb_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	CSIFW_LOGD("[CSIFW_NM]--> res(%d)", msg.res);
	CSIFW_LOGI("[CSIFW_NM] bssid %02x:%02x:%02x:%02x:%02x:%02x",
		   msg.bssid[0], msg.bssid[1],
		   msg.bssid[2], msg.bssid[3],
		   msg.bssid[4], msg.bssid[5]);
	//inform CSI Service
	g_network_status_callback(WIFI_CONNECTED);
}

static void wm_cb_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
	CSIFW_LOGI("[CSIFW_NM] WIFI Disconnected --> res(%d) reason %d", msg.res, msg.reason);
	//inform CSI Service
	g_network_status_callback(WIFI_DISCONNECTED);

}

