
#include "csifw/CSINetworkMonitor.h"
#include <wifi_manager/wifi_manager.h>

static network_status_listener g_network_status_callback;

static void wm_cb_sta_connected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg);
static CSIFW_RES checkWifiConnection();

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
	res = checkWifiConnection();
	if (res != CSIFW_OK) {
		if (res == CSIFW_ERROR_WIFI_NOT_CONNECTED) {
			CSIFW_LOGD("[network_monitor_init] Wifi is not connected");
		} else {
			CSIFW_LOGE("Some error in checking wifi connection");
		}
	}
	CSIFW_LOGD("Connection check result-> Connected");

	//regsiter callback
	if (wifi_manager_register_cb(&g_wifi_callbacks)!= WIFI_MANAGER_SUCCESS)
	{
		CSIFW_LOGE("[network_monitor_init] wifi_manager_register_cb fail");
		return CSIFW_ERROR;
	}
	g_network_status_callback = network_status_callback;
	return res;
}

CSIFW_RES network_monitor_deinit()
{
	CSIFW_LOGI("Deregistering network_monitor");

	if (wifi_manager_unregister_cb(&g_wifi_callbacks)!= WIFI_MANAGER_SUCCESS)
	{
		CSIFW_LOGE("[network_monitor_deinit] wifi_manager_unregister_cb fail");
		return CSIFW_ERROR;
	}
	CSIFW_LOGI("network_monitor_deinit done");
	return CSIFW_OK;
}


static CSIFW_RES checkWifiConnection()
{
	// Initial Wi-fi Connection checking.
	CSIFW_LOGI("Checking WIFI connection . . . . . .");
	wifi_manager_info_s wminfo;
	//get info from wifi manager
	if (wifi_manager_get_info(&wminfo)!= WIFI_MANAGER_SUCCESS) {
		CSIFW_LOGE("get info fail");
		return CSIFW_ERROR;
	}
	// check connection status
	if (wminfo.status == AP_CONNECTED) {
		CSIFW_LOGD("WIFI is connected");
	} else if (wminfo.status == AP_DISCONNECTED) {
		CSIFW_LOGD("WIFI is not connected");
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
	CSIFW_LOGI("[CSIFW_NM]--> res(%d)", msg.res);
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
