/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <debug.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <wifi_manager/wifi_manager.h>
#include <protocols/dhcpc.h>
#include <protocols/dhcpd.h>
#include <slsi_wifi/slsi_wifi_api.h>
#include <semaphore.h>
#include "wifi_utils.h"
#include "wifi_mutex.h"
#include "wifi_net.h"

static void *g_dhcp_handle = NULL;
static dhcp_sta_joined g_new_sta_join = NULL;
#ifdef CONFIG_ENABLE_IOTIVITY
mqd_t g_dw_nwevent_mqfd;
#endif

/*
 * Iotivity stack receives network events from wifi manager module
 * this will be changed later
 */
#ifdef CONFIG_ENABLE_IOTIVITY
void __tizenrt_manual_linkset(const char *msg)
{
	int ret = mq_send(g_dw_nwevent_mqfd, msg, 3, 42);
	if (ret < 0) {
		ndbg("send message fail\n");
		return;
	}
}
#endif

static wifi_manager_info_s g_manager_info;
static wifi_manager_cb_s *g_manager_callback = NULL;
static wifi_mutex *w_mutex = NULL;
static wifi_mutex *w_info_mutex = NULL;

#define CHECK_WITHOUT_RETURN(msg)			\
do {										\
	if (w_info_mutex == NULL || w_mutex == NULL) {	\
		ndbg("%s: Wi-Fi Manager is not initialized.\n", msg);	\
		return;	\
	}	\
} while (0)

#define CHECK_WITH_RETURN(msg)			\
	do {										\
		if (w_info_mutex == NULL || w_mutex == NULL) {	\
			ndbg("%s: Wi-Fi Manager is not initialized.\n", msg);	\
			return WIFI_MANAGER_DEINITIALIZED;	\
		}	\
	} while (0)

static wifi_utils_result_e start_dhcp_client(void)
{
	struct dhcpc_state state;
	int ret;

	g_dhcp_handle = dhcpc_open(CTRL_IFNAME);
	if (g_dhcp_handle == NULL) {
		ndbg("Invalid dhcp handle\n");
		return WIFI_UTILS_FAIL;
	}
	ret = dhcpc_request(g_dhcp_handle, &state);
	if (ret != OK) {
		dhcpc_close(g_dhcp_handle);
		g_dhcp_handle = NULL;
		return WIFI_UTILS_FAIL;
	}

	netlib_set_ipv4addr(CTRL_IFNAME, &state.ipaddr);
	netlib_set_ipv4netmask(CTRL_IFNAME, &state.netmask);
	netlib_set_dripv4addr(CTRL_IFNAME, &state.default_router);

	nvdbg("IP address : %s ----\n", inet_ntoa(state.ipaddr));

	return WIFI_UTILS_SUCCESS;
}

static wifi_utils_result_e stop_dhcp_client(void)
{
	if (g_dhcp_handle == NULL) {
		return WIFI_UTILS_FAIL;
	} else {
		dhcpc_close(g_dhcp_handle);
		g_dhcp_handle = NULL;
	}

	return WIFI_UTILS_SUCCESS;
}

static wifi_utils_result_e start_dhcp_server(void)
{
	struct in_addr in = {.s_addr = 0x012fa8c0 };
	netlib_set_ipv4addr(CTRL_IFNAME, &in);
	in.s_addr = 0x00ffffff;
	netlib_set_ipv4netmask(CTRL_IFNAME, &in);
	in.s_addr = 0x012fa8c0;
	netlib_set_dripv4addr(CTRL_IFNAME, &in);

	if (dhcpd_start(CTRL_IFNAME, g_new_sta_join) < 0) {
		ndbg("DHCP Server - started fail\n");
		return WIFI_UTILS_FAIL;
	}

	nvdbg("DHCP Server - started success\n");
	return WIFI_UTILS_SUCCESS;
}

static wifi_utils_result_e stop_dhcp_server(void)
{
	struct in_addr in = { .s_addr = INADDR_NONE };
	netlib_set_ipv4addr(CTRL_IFNAME, &in);
	netlib_set_ipv4netmask(CTRL_IFNAME, &in);
	netlib_set_dripv4addr(CTRL_IFNAME, &in);

	dhcpd_stop();

	return WIFI_UTILS_SUCCESS;
}

static void wifi_status_set(connect_status_e status)
{
#ifdef CONFIG_DEBUG_NET_INFO
	static char connect_status_str[5][25] = {
		// STA mode
		"AP_DISCONNECTED",
		"AP_CONNECTED",

		// SOFTAP mode
		"CLIENT_CONNECTED",
		"CLIENT_DISCONNECTED"
	};
#endif

	if (g_manager_info.status != status) {
		nvdbg("Wifi Network Status Changes from [%s] to [%s]", connect_status_str[g_manager_info.status], connect_status_str[status]);
		g_manager_info.status = status;
	}
}

static void wifi_linkup_event_func(void)
{
	CHECK_WITHOUT_RETURN("linkup");

	wifi_manager_cb_s *wifi_cb = g_manager_callback;
	wifi_utils_info info;

	wifi_mutex_acquire(w_info_mutex, WIFI_UTILS_FOREVER);
	if (g_manager_info.mode == STA_MODE || (g_manager_info.mode == WIFI_MODE_CHANGING && g_manager_info.next_mode == STA_MODE)) {
		nvdbg("WIFI CONNECTED AP - STA MODE");
		wifi_status_set(AP_CONNECTED);

		/* Perform DHCP client */
		if (start_dhcp_client() != WIFI_UTILS_SUCCESS) {
			ndbg("DHCP client start failed\n");
			g_manager_info.mode = WIFI_FAILURE;
			wifi_mutex_release(w_info_mutex);
			return;
		}
		if (stop_dhcp_client() != WIFI_UTILS_SUCCESS) {
			ndbg("DHCP client stop failed\n");
		}

		char ip4_add_str[18] = { 0, };
		if (wifi_utils_get_info(&info) == WIFI_UTILS_SUCCESS) {
			wifi_net_ip4_addr_to_ip4_str(info.ip4_address, ip4_add_str);
			g_manager_info.rssi = info.rssi;
			g_manager_info.status = info.wifi_status;
		} else {
			g_manager_info.rssi = 0;
			g_manager_info.status = STATUS_UNKNOWN;
		}
		strncpy(g_manager_info.ip4_address, ip4_add_str, 18);
		wifi_mutex_release(w_info_mutex);

		if (wifi_cb != NULL && wifi_cb->sta_connected != NULL) {
			wifi_cb->sta_connected();
		} else {
			ndbg("Callback wifimanager ap_connect failed\n");
		}
	} else if (g_manager_info.mode == SOFTAP_MODE || (g_manager_info.mode == WIFI_MODE_CHANGING && g_manager_info.next_mode == SOFTAP_MODE)) {
		nvdbg("CONNECTED FROM CLIENT - SOFT AP MODE");
		wifi_status_set(CLIENT_CONNECTED);
		wifi_mutex_release(w_info_mutex);

		/* No callbacks here, in case new client joins, we invoke callback
		 * from DHCP server instead
		 */
	} else {
		ndbg("wifi manager linkup failed: invalid mode(%d)\n", g_manager_info.mode);
		wifi_mutex_release(w_info_mutex);
	}

#ifdef CONFIG_ENABLE_IOTIVITY
	__tizenrt_manual_linkset("gen");
#endif
	/* TODO: Import files from source
	 * sendStatusTrigger (TRIGGER_NETWORK_CHANGED, DAWIT_NW_CHANGED_UP);
	 */
}

static void wifi_linkdown_event_func(void)
{
// here, send a message to callback handler and rest of this function should be done by the callback handler.
// wifi_manager_init() creates the callback handler and deinit() joins the callback handler.
	CHECK_WITHOUT_RETURN("linkdown");

	wifi_manager_cb_s *wifi_cb = g_manager_callback;

	wifi_mutex_acquire(w_info_mutex, WIFI_UTILS_FOREVER);
	if (g_manager_info.mode == STA_MODE || (g_manager_info.mode == WIFI_MODE_CHANGING && g_manager_info.pre_mode == STA_MODE) || \
			(g_manager_info.mode == WIFI_DEINITIALIZING && g_manager_info.pre_mode == STA_MODE)) {
		nvdbg("WIFI DISCONNECTED AP - STA MODE");
		g_manager_info.ssid[0] = '\0';
		g_manager_info.ip4_address[0] = '\0';
		g_manager_info.rssi = 0;
		wifi_status_set(AP_DISCONNECTED);
		wifi_mutex_release(w_info_mutex);

		if (wifi_cb != NULL && wifi_cb->sta_disconnected != NULL) {
			wifi_cb->sta_disconnected();
		} else {
			ndbg("Callback wifimanager ap_disconnected failed\n");
		}
	} else if (g_manager_info.mode == SOFTAP_MODE || (g_manager_info.mode == WIFI_MODE_CHANGING && g_manager_info.pre_mode == SOFTAP_MODE) || \
			(g_manager_info.mode == WIFI_DEINITIALIZING && g_manager_info.pre_mode == SOFTAP_MODE)) {
		nvdbg("DISCONNECTED FROM CLIENT - SOFT AP MODE");
		wifi_status_set(CLIENT_DISCONNECTED);
		wifi_mutex_release(w_info_mutex);

		if (wifi_cb != NULL && wifi_cb->softap_sta_left != NULL) {
			wifi_cb->softap_sta_left();
		} else {
			ndbg("Callback wifimanager ap_disconnected failed\n");
		}
	} else {
		ndbg("wifi manager linkdown failed: invalid mode(%d)\n", g_manager_info.mode);
		wifi_mutex_release(w_info_mutex);
	}

#ifdef CONFIG_ENABLE_IOTIVITY
	__tizenrt_manual_linkset("del");
#endif

	/* TODO: Import files from source
	 * sendStatusTrigger(TRIGGER_NETWORK_CHANGED, DAWIT_NW_CHANGED_DOWN);
	 */
}

static wifi_manager_result_e wifi_free_scan_results(wifi_manager_scan_info_s **wifi_manager_scan_info)
{
	wifi_manager_scan_info_s *curr_record;
	wifi_manager_scan_info_s *next_record;

	curr_record = *wifi_manager_scan_info;
	while (curr_record != NULL) {
		next_record = curr_record->next;
		free(curr_record);
		curr_record = next_record;
	}

	*wifi_manager_scan_info = NULL;
	return WIFI_MANAGER_SUCCESS;
}

static wifi_manager_result_e wifi_fetch_scan_results(slsi_scan_info_t **wifi_scan_info, wifi_manager_scan_info_s **wifi_manager_scan_info)
{
	wifi_manager_scan_info_s *curr_record;
	wifi_manager_scan_info_s *prev_record = NULL;
	slsi_scan_info_t *wifi_scan_iter = NULL;
	int i = 0;
	if (*wifi_scan_info == NULL) {
		return WIFI_MANAGER_FAIL;
	}
	/* Initialize pointer */
	wifi_scan_iter = *wifi_scan_info;

	do {
		curr_record = (wifi_manager_scan_info_s *)malloc(sizeof(wifi_manager_scan_info_s));
		if (curr_record == NULL) {
			wifi_free_scan_results(wifi_manager_scan_info);
			return WIFI_MANAGER_FAIL;
		}
		if (!prev_record) {
			*wifi_manager_scan_info = curr_record;
		} else {
			prev_record->next = curr_record;
		}

		memset(curr_record->ssid, 0x00, SLSI_SSID_LEN + 1);
		memset(curr_record->bssid, 0x00, SLSI_MACADDR_STR_LEN);
		curr_record->rssi = wifi_scan_iter->rssi;
		curr_record->channel = wifi_scan_iter->channel;
		curr_record->phy_mode = wifi_scan_iter->phy_mode;
		strncpy(curr_record->ssid, (char *)wifi_scan_iter->ssid, SLSI_SSID_LEN + 1);
		strncpy(curr_record->bssid, (char *)wifi_scan_iter->bssid, SLSI_MACADDR_STR_LEN);
		prev_record = curr_record;
		wifi_scan_iter = wifi_scan_iter->next;
		i++;
	} while (wifi_scan_iter != NULL);

	curr_record->next = NULL;
	printf("%d records scanned\n", i);

	return WIFI_MANAGER_SUCCESS;
}

static wifi_manager_result_e wifi_scan_result_callback(slsi_reason_t *reason)
{
	CHECK_WITH_RETURN("scan_result_callback");

	wifi_manager_scan_info_s *wifi_manager_scan_info = 0;
	wifi_manager_cb_s *wifi_cb = g_manager_callback;

	if (reason->reason_code == 0) {
		slsi_scan_info_t *wifi_scan_result;
		WiFiGetScanResults(&wifi_scan_result);
		if (wifi_fetch_scan_results(&wifi_scan_result, &wifi_manager_scan_info) == WIFI_MANAGER_SUCCESS) {
			wifi_cb->scan_ap_done(&wifi_manager_scan_info, WIFI_SCAN_SUCCESS);
			wifi_free_scan_results(&wifi_manager_scan_info);
		}
		WiFiFreeScanResults(&wifi_scan_result);
	} else {
		ndbg("Scan failed %d\n");
		wifi_cb->scan_ap_done(&wifi_manager_scan_info, WIFI_SCAN_FAIL);
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}

/**
 * Public API
 */

wifi_manager_result_e wifi_manager_connect_ap(wifi_manager_ap_config_s *config)
{
	CHECK_WITH_RETURN("connect_ap");

	if (config == NULL) {
		return WIFI_MANAGER_INVALID_ARGS;
	}

	if ((config->ssid_length > 31) || (config->passphrase_length > 63)) {
		ndbg("AP configuration fails: too long ssid or passphrase\n");
		ndbg("Make sure that length of SSID < 32 and length of passphrase < 64\n");
		return WIFI_MANAGER_INVALID_ARGS;
	}

	wifi_utils_info info;
	wifi_utils_ap_config_s util_config;

	wifi_mutex_acquire(w_mutex, WIFI_UTILS_FOREVER);
	if (wifi_utils_get_info(&info) != WIFI_UTILS_SUCCESS) {
		ndbg("Wi-Fi info retrival fails.\n");
		wifi_mutex_release(w_mutex);
		return WIFI_MANAGER_FAIL;
	}

	if (info.wifi_status == WIFI_UTILS_SOFTAP_MODE) {
		ndbg("Current mode soft ap mode, can not connect ap");
		wifi_mutex_release(w_mutex);
		return WIFI_MANAGER_FAIL;
	}

	strncpy(util_config.ssid, config->ssid, config->ssid_length + 1);
	util_config.ssid_length = config->ssid_length;
	strncpy(util_config.passphrase, config->passphrase, config->passphrase_length + 1);
	util_config.passphrase_length = config->passphrase_length;
	util_config.ap_auth_type = config->ap_auth_type;
	util_config.ap_crypto_type = config->ap_crypto_type;

	wifi_utils_result_e result = wifi_utils_connect_ap(&util_config);
	if (result != WIFI_UTILS_SUCCESS) {
		ndbg("Wifi AP connect fail");
		wifi_mutex_release(w_mutex);
		return WIFI_MANAGER_FAIL;
	}
	wifi_mutex_release(w_mutex);

	wifi_mutex_acquire(w_info_mutex, WIFI_UTILS_FOREVER);
	strncpy(g_manager_info.ssid, config->ssid, config->ssid_length + 1);
	wifi_mutex_release(w_info_mutex);

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_disconnect_ap(void)
{
	CHECK_WITH_RETURN("disconnect_ap");

	wifi_utils_result_e result = WIFI_UTILS_SUCCESS;

	wifi_mutex_acquire(w_mutex, WIFI_UTILS_FOREVER);
	result = wifi_utils_disconnect_ap();
	wifi_mutex_release(w_mutex);

	return result;
}

wifi_manager_result_e wifi_manager_init(wifi_manager_cb_s *wmcb)
{
	wifi_utils_result_e result = WIFI_UTILS_SUCCESS;
	wifi_utils_info info;

	if (wmcb == NULL) {
			ndbg("Wi-Fi Manager init fail because of no callbacks");
			return WIFI_MANAGER_FAIL;
	}

	if (w_info_mutex == NULL) {
		w_info_mutex = (wifi_mutex *)malloc(sizeof(wifi_mutex));
		if (w_info_mutex == NULL) {
			ndbg("w_info_mutex malloc fails.\n");
			return WIFI_MANAGER_FAIL;
		}
		result = wifi_mutex_create(w_info_mutex);
		if (result != WIFI_UTILS_SUCCESS) {
			ndbg("w_info_mutex_create fail");
			goto error_without_mutex_release;
		}
	}

	wifi_mutex_acquire(w_info_mutex, WIFI_UTILS_FOREVER);
	if (w_mutex != NULL) {
		ndbg("WI-FI is already initialized.\n");
		wifi_mutex_release(w_info_mutex);
		return WIFI_MANAGER_INITIALIZED;
	}

	w_mutex = (wifi_mutex *)malloc(sizeof(wifi_mutex));
	if (w_mutex == NULL) {
		ndbg("w_mutex malloc fails.\n");
		wifi_mutex_release(w_info_mutex);
		goto error_with_mutex_destory;
	}

	result = wifi_mutex_create(w_mutex);
	if (result != WIFI_UTILS_SUCCESS) {
		ndbg("wifi_mutex_create fail");
		wifi_mutex_release(w_info_mutex);
		goto error_with_mutex_destory;
	}

	/* wifi_manager mode is switched to wifi_initializing */
	g_manager_info.pre_mode = WIFI_NONE;
	g_manager_info.next_mode = STA_MODE;
	g_manager_info.mode = WIFI_INITIALIZING;
	wifi_mutex_release(w_info_mutex);

#ifdef CONFIG_ENABLE_IOTIVITY
	struct mq_attr lq_attr;
	lq_attr.mq_maxmsg = 10;
	lq_attr.mq_msgsize = 4;
	lq_attr.mq_flags = 0;
	g_dw_nwevent_mqfd = mq_open("netlink_evtq", O_WRONLY | O_CREAT, 0666, &lq_attr);

	if (g_dw_nwevent_mqfd == (mqd_t)ERROR) {
		ndbg("iotivity connect event message queue init fail");
		goto error_with_mutex_destory_all;
	}
#endif

	wifi_mutex_acquire(w_mutex, WIFI_UTILS_FOREVER);
	result = wifi_utils_init();
	if (result != WIFI_UTILS_SUCCESS) {
		ndbg("wifi_utils_init fail");
		goto error_with_mutex_release;
	}

	wifi_utils_register_callback(wifi_linkup_event_func, wifi_linkdown_event_func);

	if (wifi_utils_get_info(&info) != WIFI_UTILS_SUCCESS) {
		ndbg("wifi_utils_get_info fail");
		goto error_with_mutex_release;
	}
	wifi_mutex_release(w_mutex);

	/* update wifi_manager info by being protected by w_info_mutex */
	wifi_mutex_acquire(w_info_mutex, WIFI_UTILS_FOREVER);
	g_manager_info.ssid[0] = '\0';
	g_manager_info.ip4_address[0] = '\0';
	g_manager_info.rssi = 0;
	g_manager_info.mode = STA_MODE;
	g_manager_callback = wmcb;
	wifi_status_set(AP_DISCONNECTED);
	strncpy(g_manager_info.mac_address, info.mac_address, 6);
	wifi_mutex_release(w_info_mutex);

	return WIFI_MANAGER_SUCCESS;

error_with_mutex_release:
	wifi_mutex_release(w_mutex);

#ifdef CONFIG_ENABLE_IOTIVITY
error_with_mutex_destory_all:
#endif
	wifi_mutex_acquire(w_info_mutex, WIFI_UTILS_FOREVER);
	g_manager_info.mode = WIFI_NONE;

	result = wifi_mutex_destroy(w_mutex);
	if (result != WIFI_UTILS_SUCCESS) {
		ndbg("wifi_mutex_destroy fail %d", result);
	}
	wifi_mutex_release(w_info_mutex);

error_with_mutex_destory:
	result = wifi_mutex_destroy(w_info_mutex);
	if (result != WIFI_UTILS_SUCCESS) {
		ndbg("w_info_mutex_destroy fail %d", result);
	}

error_without_mutex_release:
	if (w_mutex != NULL) {
		free(w_mutex);
		w_mutex = NULL;
	}
	if (w_info_mutex != NULL) {
		free(w_info_mutex);
		w_info_mutex = NULL;
	}

	return WIFI_MANAGER_FAIL;
}

wifi_manager_result_e wifi_manager_deinit()
{
	CHECK_WITH_RETURN("deinit");

	wifi_utils_result_e result = WIFI_UTILS_SUCCESS;
	wifi_manager_result_e ret = WIFI_MANAGER_SUCCESS;

	wifi_mutex_acquire(w_info_mutex, WIFI_UTILS_FOREVER);
	/* wifi_manager mode is switched to wifi_deinitializing */
	if ((g_manager_info.mode != WIFI_INITIALIZING) && (g_manager_info.mode != WIFI_MODE_CHANGING) && \
			(g_manager_info.mode != WIFI_DEINITIALIZING)) {
		g_manager_info.pre_mode = g_manager_info.mode;
		g_manager_info.next_mode = WIFI_NONE;
		g_manager_info.mode = WIFI_DEINITIALIZING;
	} else {
		ndbg("Can't deinitilize wifi_manager from the current wifi mode %d\n", g_manager_info.mode);
		wifi_mutex_release(w_info_mutex);
		return WIFI_MANAGER_FAIL;
	}

	if ((g_manager_info.mode == SOFTAP_MODE) || (g_manager_info.mode == WIFI_FAILURE)) {
		stop_dhcp_server();
	}
	wifi_mutex_release(w_info_mutex);

	wifi_mutex_acquire(w_mutex, WIFI_UTILS_FOREVER);
	result = wifi_utils_deinit();
	if (result != WIFI_UTILS_SUCCESS) {
		ndbg("wifi_utils_deinit fail");
		ret = WIFI_MANAGER_FAIL;
	}
	wifi_mutex_release(w_mutex);

	wifi_mutex_acquire(w_info_mutex, WIFI_UTILS_FOREVER);
	ndbg("w_mutex->semcount: %d, w_mutex->flags: %d\n", w_mutex->semcount, w_mutex->flags);

	result = wifi_mutex_destroy(w_mutex);
	if (result != WIFI_UTILS_SUCCESS) {
		ndbg("wifi_mutex_destroy fail %d", result);
		ret = WIFI_MANAGER_FAIL;
	}

	if (ret != WIFI_MANAGER_FAIL) {
		g_manager_info.mode = WIFI_NONE;
	} else {
		g_manager_info.mode = WIFI_FAILURE;
	}

	if (w_mutex != NULL) {
		free(w_mutex);
		w_mutex = NULL;
	}
	wifi_mutex_release(w_info_mutex);

	result = wifi_mutex_destroy(w_info_mutex);
	if (result != WIFI_UTILS_SUCCESS) {
		ndbg("w_info_mutex_destroy fail %d", result);
	}
	if (w_info_mutex != NULL) {
		free(w_info_mutex);
		w_info_mutex = NULL;
	}

	return ret;
}

wifi_manager_result_e wifi_manager_set_mode(wifi_manager_mode_e mode, wifi_manager_softap_config_s *config)
{
	CHECK_WITH_RETURN("set_mode");

	wifi_utils_result_e result = WIFI_UTILS_SUCCESS;

	if (mode != STA_MODE && mode != SOFTAP_MODE) {
		ndbg("wifi manager set mode failed: invalid mode\n");
		return WIFI_MANAGER_INVALID_ARGS;
	}

	if ((mode == SOFTAP_MODE) && ((config == NULL) || (strlen(config->ssid) > 31) || (strlen(config->passphrase) > 63))) {
		ndbg("SoftAP configuration fails: no config or too long ssid or passphrase\n");
		ndbg("Make sure that length of SSID < 32 and length of passphrase < 64\n");
		return WIFI_MANAGER_INVALID_ARGS;
	}

	wifi_mutex_acquire(w_info_mutex, WIFI_UTILS_FOREVER);
	if (g_manager_info.mode == mode) {
		ndbg("wifi manager set mode failed: current mode is the same as requested.\n");
		wifi_mutex_release(w_info_mutex);
		return WIFI_MANAGER_SUCCESS;
	}

	/* wifi mode is switched to WIFI_MODE_CHANGING */
	if ((g_manager_info.mode != STA_MODE) && (g_manager_info.mode != SOFTAP_MODE)) {
		ndbg("Can't change wifi mode from current mode %d.\n", g_manager_info.mode);
		wifi_mutex_release(w_info_mutex);
		return WIFI_MANAGER_FAIL;
	}

	g_manager_info.pre_mode = g_manager_info.mode;
	g_manager_info.next_mode = mode;
	g_manager_info.mode = WIFI_MODE_CHANGING;
	wifi_mutex_release(w_info_mutex);

	// Wifi mode is changed to SOFT AP
	if (mode == SOFTAP_MODE) {
		wifi_utils_softap_config_s softap_config;

		softap_config.channel = config->channel;
		softap_config.ap_crypto_type = WIFI_UTILS_CRYPTO_AES;
		softap_config.ap_auth_type = WIFI_UTILS_AUTH_WPA2_PSK;
		softap_config.ssid_length = strlen(config->ssid);
		softap_config.passphrase_length = strlen(config->passphrase);
		strncpy(softap_config.ssid, config->ssid, softap_config.ssid_length + 1);
		strncpy(softap_config.passphrase, config->passphrase, softap_config.passphrase_length + 1);
		softap_config.inform_new_sta_join = g_manager_callback->softap_sta_joined;

		wifi_mutex_acquire(w_mutex, WIFI_UTILS_FOREVER);
		g_new_sta_join = softap_config.inform_new_sta_join;
		result = wifi_utils_start_softap(&softap_config);
		if (result != WIFI_UTILS_SUCCESS) {
			ndbg("Starting softap mode failed.");
			goto error_with_failure;
		}

		if (start_dhcp_server() != WIFI_UTILS_SUCCESS) {
			ndbg("Starting DHCP server failed.\n");
			goto error_with_failure;
		}
		wifi_mutex_release(w_mutex);

		/* update wifi_manager_info */
		wifi_mutex_acquire(w_info_mutex, WIFI_UTILS_FOREVER);
		g_manager_info.mode = SOFTAP_MODE;
		strncpy(g_manager_info.ssid, config->ssid, softap_config.ssid_length + 1);
		wifi_status_set(CLIENT_DISCONNECTED);
		wifi_mutex_release(w_info_mutex);

		nvdbg("Wifi mode is changed to SOFT AP");
	}
	// Wifi mode is changed to station
	else {
		wifi_mutex_acquire(w_mutex, WIFI_UTILS_FOREVER);
		result = wifi_utils_stop_softap();
		if (result != WIFI_UTILS_SUCCESS) {
			ndbg("Stoping softap failed.");
			goto error_with_failure;
		}

		stop_dhcp_server();

		result = wifi_utils_start_sta();
		if (result != WIFI_UTILS_SUCCESS) {
			ndbg("Starting STA failed.");
			goto error_with_failure;
		}
		wifi_mutex_release(w_mutex);

		/* update wifi_manager_info */
		wifi_mutex_acquire(w_info_mutex, WIFI_UTILS_FOREVER);
		g_manager_info.mode = STA_MODE;
		wifi_status_set(AP_DISCONNECTED);
		wifi_mutex_release(w_info_mutex);

		nvdbg("Wifi mode is changed to station");
	}

	return WIFI_MANAGER_SUCCESS;

error_with_failure:
	wifi_mutex_release(w_mutex);
	wifi_mutex_acquire(w_info_mutex, WIFI_UTILS_FOREVER);
	g_manager_info.mode = WIFI_FAILURE;
	wifi_mutex_release(w_info_mutex);

	return WIFI_MANAGER_FAIL;
}

wifi_manager_result_e wifi_manager_get_info(wifi_manager_info_s *info)
{
	CHECK_WITH_RETURN("get_info");

	if (info == NULL) {
		ndbg("info is null.");
		return WIFI_MANAGER_FAIL;
	}

	wifi_mutex_acquire(w_info_mutex, WIFI_UTILS_FOREVER);
	*info = g_manager_info;
	wifi_mutex_release(w_info_mutex);

	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_scan_ap(void)
{
	CHECK_WITH_RETURN("scan_ap");

	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	if (g_manager_callback->scan_ap_done == NULL) {
		ndbg("Missing callback for WiFi scan");
		return WIFI_MANAGER_INVALID_ARGS;
	}

	wifi_mutex_acquire(w_mutex, WIFI_UTILS_FOREVER);

	WiFiRegisterScanCallback(wifi_scan_result_callback);
	if (WiFiScanNetwork() == SLSI_STATUS_SUCCESS) {
		ndbg("WiFI scan succeeds.");
		ret = WIFI_MANAGER_SUCCESS;
	}

	wifi_mutex_release(w_mutex);

	return ret;
}
