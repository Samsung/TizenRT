/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/lwnl/lwnl80211.h>
#include <tinyara/lwnl/rtk_drv.h>

#include "wifi_conf.h"

#define vTaskDelay(t) usleep(t)
#define MACADDR_LEN 6

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
static WiFi_InterFace_ID_t g_mode = RTK_WIFI_NONE;

static struct lwnl80211_ops_s g_lwnl80211_drv_ops = {
	rtkdrv_init,			/* init */
	rtkdrv_deinit,			/* deinit */
	rtkdrv_scan_ap,			/* scan_ap */
	rtkdrv_connect_ap,		/* connect_ap */
	rtkdrv_disconnect_ap,   /* disconnect_ap */
	rtkdrv_get_info,		/* get_info */
	rtkdrv_start_sta,		/* start_sta */
	rtkdrv_start_softap,	/* start_softap */
	rtkdrv_stop_softap,		/* stop_softap */
	rtkdrv_set_autoconnect, /* set_autoconnect */
	NULL					/* drv_ioctl */
};

/*
 * Callback
 */
static int rtk_drv_callback_handler(int argc, char *argv[])
{
	RTKDRV_ENTER;
	int type = (int)(argv[1][0] - '0');

	switch (type) {
	case 1:
		lwnl80211_postmsg(LWNL80211_STA_CONNECTED, NULL);
		break;
	case 2:
		lwnl80211_postmsg(LWNL80211_STA_CONNECT_FAILED, NULL);
		break;
	case 3:
		lwnl80211_postmsg(LWNL80211_SOFTAP_STA_JOINED, NULL);
		break;
	case 4:
		lwnl80211_postmsg(LWNL80211_STA_DISCONNECTED, NULL);
		break;
	case 5:
		lwnl80211_postmsg(LWNL80211_SOFTAP_STA_LEFT, NULL);
		break;
	default:
		lwnl80211_postmsg(LWNL80211_UNKNOWN, NULL);
		break;
	}

	return 0;
}

static void linkup_handler(rtk_reason_t *reason)
{
	RTKDRV_ENTER;
	pid_t pid;
	char *argv[2];
	argv[1] = NULL;
	char data[2] = {0, 0};

	if (g_mode == RTK_WIFI_STATION_IF) {
		if (reason->reason_code == RTK_STATUS_SUCCESS) {
			data[0] = '1';
		} else {
			data[0] = '2';
		}
	} else if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		data[0] = '3';
	}
	argv[0] = data;

	pid = kernel_thread("lwnl80211_cbk_handler", 100, 1024, (main_t)rtk_drv_callback_handler, argv);
	if (pid < 0) {
		vddbg("pthread create fail(%d)\n", errno);
		return;
	}
}

static void linkdown_handler(rtk_reason_t *reason)
{
	RTKDRV_ENTER;
	pid_t pid;
	char *argv[2];
	argv[1] = NULL;
	char data[2] = {0, 0};

	data[0] = '4';
	if (g_mode == RTK_WIFI_STATION_IF) {
		data[0] = '4';
	} else if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		data[0] = '5';
	}
	argv[0] = data;

	pid = kernel_thread("lwnl80211_cbk_handler", 100, 1024, (main_t)rtk_drv_callback_handler, argv);
	if (pid < 0) {
		vddbg("pthread create fail(%d)\n", errno);
		return;
	}
}


int8_t wifi_scan_result_callback(wifi_utils_scan_list_s *utils_scan_input, int scan_num)
{
	lwnl80211_scan_list_s *scan_list = (lwnl80211_scan_list_s *)utils_scan_input;

	if (scan_list) {
		lwnl80211_postmsg(LWNL80211_SCAN_DONE, (void *)scan_list);
	} else {
		lwnl80211_postmsg(LWNL80211_SCAN_FAILED, NULL);
	}

	return RTK_STATUS_SUCCESS;
}

/*
 * Interface API
*/
lwnl80211_result_e rtkdrv_init(struct lwnl80211_lowerhalf_s *dev)
{
	RTKDRV_ENTER;
	lwnl80211_result_e result = LWNL80211_FAIL;
	if (g_mode == RTK_WIFI_NONE) {
		if (WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler) != RTK_STATUS_SUCCESS) {
			vddbg("[RTK] Link callback handles: register failed !\n");
			return result;
		}
		vdvdbg("[RTK] Link callback handles: registered\n");

		if (cmd_wifi_on(RTK_WIFI_STATION_IF) != RTK_STATUS_SUCCESS) {
			vddbg("[RTK] Failed to start STA mode\n");
			return result;
		}
		vdvdbg("[RTK] Start STA mode\n");

		g_mode = RTK_WIFI_STATION_IF;
		result = LWNL80211_SUCCESS;
		return result;
	} else {
		vddbg("[RTK] Already %d\n", g_mode);
	}

	return result;
}

lwnl80211_result_e rtkdrv_deinit(void)
{
	RTKDRV_ENTER;
	lwnl80211_result_e result = LWNL80211_FAIL;
	int ret = cmd_wifi_off();
	if (ret == RTK_STATUS_SUCCESS) {
		g_mode = RTK_WIFI_NONE;
		result = LWNL80211_SUCCESS;
	} else {
		vddbg("[RTK] Failed to stop STA mode\n");
	}
	return result;
}

lwnl80211_result_e rtkdrv_scan_ap(lwnl80211_ap_config_s *config)
{
	RTKDRV_ENTER;
	lwnl80211_result_e result = LWNL80211_FAIL;
	if (wifi_scan_networks(app_scan_result_handler, NULL) != RTW_SUCCESS) {
		vddbg("[RTK] WiFi scan fail\n");
		return result;
	}
	result = LWNL80211_SUCCESS;
	vddbg("[RTK] WIFi Scan success\n");
	return result;
}

lwnl80211_result_e rtkdrv_connect_ap(lwnl80211_ap_config_s *ap_connect_config, void *arg)
{
	RTKDRV_ENTER;
	if (!ap_connect_config) {
		return LWNL80211_INVALID_ARGS;
	}

	if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		vddbg("[RTK] SoftAP is running\n");
		return LWNL80211_FAIL;
	}
	int ret = cmd_wifi_connect((wifi_utils_ap_config_s *)ap_connect_config, arg);
	if (ret != RTK_STATUS_SUCCESS) {
		if (ret == RTK_STATUS_ALREADY_CONNECTED) {
			vdvdbg("[RTK] Connect failed (alreay connected: %d)\n", ret);
			return LWNL80211_ALREADY_CONNECTED;
		} else {
			vddbg("[RTK] Connect failed: %d, %s\n", ret, ap_connect_config->ssid);
			return LWNL80211_FAIL;
		}
	}
	vddbg("[RTK] Successfully joined the network: %s(%d)\n", ap_connect_config->ssid, ap_connect_config->ssid_length);

	return LWNL80211_SUCCESS;
}

lwnl80211_result_e rtkdrv_disconnect_ap(void *arg)
{
	RTKDRV_ENTER;
	int ret = cmd_wifi_disconnect();
	if (ret == RTK_STATUS_SUCCESS) {
		vddbg("[RTK] Disconnect success\n");
		return LWNL80211_SUCCESS;
	}
	vddbg("[RTK] WiFiNetworkLeave fail because of %d\n", ret);

	return LWNL80211_FAIL;
}

lwnl80211_result_e rtkdrv_get_info(lwnl80211_info *wifi_info)
{
	RTKDRV_ENTER;
	if (!wifi_info) {
		vddbg("[RTK] Get failed\n");
		return LWNL80211_INVALID_ARGS;
	}

	if (g_mode == RTK_WIFI_NONE) {
		vddbg("[RTK] WiFi driver is not initialized\n");
		return LWNL80211_INVALID_ARGS;
	}

	char mac_str[18] = {
		0,
	};
	(void)wifi_get_mac_address((char *)mac_str);

	int ret = sscanf(mac_str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%*c", &wifi_info->mac_address[0], &wifi_info->mac_address[1], &wifi_info->mac_address[2], &wifi_info->mac_address[3], &wifi_info->mac_address[4], &wifi_info->mac_address[5]);
	if (ret != MACADDR_LEN) {
		vddbg("[RTK] Failed to get MAC addr\n");
		return LWNL80211_FAIL;
	}

	wifi_info->rssi = (int)0;
	if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		wifi_info->wifi_status = LWNL80211_SOFTAP_MODE;
	} else {
		if (wifi_is_connected_to_ap() == RTW_SUCCESS) {
			wifi_info->wifi_status = LWNL80211_CONNECTED;
			int rssi_tmp;
			if (wifi_get_rssi(&rssi_tmp) == RTW_SUCCESS) {
				wifi_info->rssi = rssi_tmp;
			} else {
				vddbg("[RTK] Failed to get rssi\n");
				return LWNL80211_FAIL;
			}
		} else {
			wifi_info->wifi_status = LWNL80211_DISCONNECTED;
		}
	}

	return LWNL80211_SUCCESS;
}

lwnl80211_result_e rtkdrv_start_softap(lwnl80211_softap_config_s *softap_config)
{
	RTKDRV_ENTER;
	if (!softap_config) {
		return LWNL80211_INVALID_ARGS;
	}

	if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		vddbg("[RTK] softap is already running\n");
		return LWNL80211_FAIL;
	}

	if (cmd_wifi_ap((wifi_utils_softap_config_s *)softap_config) != RTK_STATUS_SUCCESS) {
		vddbg("[RTK] Failed to start AP mode\n");
		return LWNL80211_FAIL;
	}

	vdvdbg("[RTK] Start SoftAp mode\n");
	if (WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler) != RTK_STATUS_SUCCESS) {
			vddbg("[RTK] link callback handles: register failed !\n");
			return LWNL80211_FAIL;
		}
	vdvdbg("[RTK] link callback handles: registered\n");
	g_mode = RTK_WIFI_SOFT_AP_IF;

	return LWNL80211_SUCCESS;
}

lwnl80211_result_e rtkdrv_start_sta(void)
{
	RTKDRV_ENTER;
	if (g_mode == RTK_WIFI_STATION_IF) {
		vddbg("[RTK] Station mode is already running\n");
		return LWNL80211_FAIL;
	}

	if (WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler) != RTK_STATUS_SUCCESS) {
		vddbg("[RTK] link callback handles: register failed !\n");
		return LWNL80211_FAIL;
	}
	vdvdbg("[RTK] link callback handles: registered\n");

	if (cmd_wifi_off() == RTK_STATUS_SUCCESS) {
		g_mode = RTK_WIFI_NONE;
		vTaskDelay(50);
	} else {
		vddbg("[RTK] Failed to stop SoftAP mode\n");
	}

	if (cmd_wifi_on(RTK_WIFI_STATION_IF) != RTK_STATUS_SUCCESS) {
		vddbg("[RTK] Failed to strate station\n");
		return LWNL80211_FAIL;
	}

	vdvdbg("[RTK] Start station mode\n");
	g_mode = RTK_WIFI_STATION_IF;

	return LWNL80211_SUCCESS;
}

lwnl80211_result_e rtkdrv_stop_softap(void)
{
	RTKDRV_ENTER;

	if (g_mode == RTK_WIFI_SOFT_AP_IF) {

		if (cmd_wifi_off() == RTK_STATUS_SUCCESS) {
			g_mode = RTK_WIFI_NONE;
			vdvdbg("[RTK] Stop AP mode successfully\n");
		} else {
			vddbg("[RTK] Stop AP mode fail\n");
			return LWNL80211_FAIL;
		}
	} else {
		vddbg("[RTK] Mode is not AP mode\n");
		return LWNL80211_FAIL;
	}
	return LWNL80211_SUCCESS;
}

lwnl80211_result_e rtkdrv_set_autoconnect(uint8_t check)
{
	RTKDRV_ENTER;
	if (wifi_set_autoreconnect(check) != RTK_STATUS_SUCCESS) {
		vddbg("External Autoconnect failed to set %d", check);
		return LWNL80211_FAIL;
	}

	vdvdbg("[RTK] External Autoconnect set to %d\n", check);
	return LWNL80211_SUCCESS;
}

struct lwnl80211_lowerhalf_s *rtk_drv_initialize(void)
{
	RTKDRV_ENTER;
	struct lwnl80211_lowerhalf_s *dev = NULL;

	dev = (struct lwnl80211_lowerhalf_s *)kmm_malloc(sizeof(struct lwnl80211_lowerhalf_s));
	if (!dev) {
		return NULL;
	}

	dev->ops = &g_lwnl80211_drv_ops;

	return dev;
}
