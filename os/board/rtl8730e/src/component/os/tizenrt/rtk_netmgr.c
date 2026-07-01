/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/
#include <tinyara/config.h>
#include <pthread.h>
#include "rtk_wifi_utils.h"
#include <debug.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include "common/defs.h"
#include "os_wrapper.h"
/* WLAN CONFIG ---------------------------------------------------------------*/

static BOOL g_bridge_on = FALSE;
static BOOL g_netmgr_init = FALSE;
#include <tinyara/net/if/wifi.h>
trwifi_result_e wifi_netmgr_utils_init(struct netdev *dev);
trwifi_result_e wifi_netmgr_utils_deinit(struct netdev *dev);
trwifi_result_e wifi_netmgr_utils_scan_ap(struct netdev *dev, trwifi_scan_config_s *config);
trwifi_result_e wifi_netmgr_utils_scan_multi_ap(struct netdev *dev, trwifi_scan_multi_configs_s *config);
trwifi_result_e wifi_netmgr_utils_connect_ap(struct netdev *dev, trwifi_ap_config_s *ap_connect_config, void *arg);
trwifi_result_e wifi_netmgr_utils_disconnect_ap(struct netdev *dev, void *arg);
trwifi_result_e wifi_netmgr_utils_get_info(struct netdev *dev, trwifi_info *wifi_info);
trwifi_result_e wifi_netmgr_utils_start_sta(struct netdev *dev);
trwifi_result_e wifi_netmgr_utils_start_softap(struct netdev *dev, trwifi_softap_config_s *softap_config);
trwifi_result_e wifi_netmgr_utils_stop_softap(struct netdev *dev);
trwifi_result_e wifi_netmgr_utils_set_autoconnect(struct netdev *dev, uint8_t check);
trwifi_result_e wifi_netmgr_utils_ioctl(struct netdev *dev, trwifi_msg_s *msg);
trwifi_result_e wifi_netmgr_utils_set_chplan(struct netdev *dev, uint8_t chplan);
trwifi_result_e wifi_netmgr_utils_get_signal_quality(struct netdev *dev, trwifi_signal_quality *signal_quality);
trwifi_result_e wifi_netmgr_utils_get_disconn_reason(struct netdev *dev, int *deauth_reason);
trwifi_result_e wifi_netmgr_utils_get_driver_info(struct netdev *dev, trwifi_driver_info *driver_info);
trwifi_result_e wifi_netmgr_utils_get_wpa_supplicant_state(struct netdev *dev, trwifi_wpa_states *wpa_supplicant_state);
trwifi_result_e wifi_netmgr_utils_disable_11ax_mode(struct netdev *dev, uint8_t disable);
trwifi_result_e wifi_netmgr_utils_set_bridge(struct netdev *dev, uint8_t control);

struct trwifi_ops g_trwifi_drv_ops = {
	wifi_netmgr_utils_init,			/* init */
	wifi_netmgr_utils_deinit,			/* deinit */
	wifi_netmgr_utils_scan_ap,			/* scan_ap */
	wifi_netmgr_utils_connect_ap,		/* connect_ap */
	wifi_netmgr_utils_disconnect_ap,   /* disconnect_ap */
	wifi_netmgr_utils_get_info,		/* get_info */
	wifi_netmgr_utils_start_sta,		/* start_sta */
	wifi_netmgr_utils_start_softap,	/* start_softap */
	wifi_netmgr_utils_stop_softap,		/* stop_softap */
	wifi_netmgr_utils_set_autoconnect, /* set_autoconnect */
	wifi_netmgr_utils_ioctl,					/* drv_ioctl */
	wifi_netmgr_utils_scan_multi_ap,	/* scan_multi_ap */
	wifi_netmgr_utils_set_chplan,		/* set_chplan */
	wifi_netmgr_utils_get_signal_quality,		/* get_signal_quality */
	wifi_netmgr_utils_get_disconn_reason,		/* get_deauth_reason */
	wifi_netmgr_utils_get_driver_info,			/* get_driver_info */
	wifi_netmgr_utils_get_wpa_supplicant_state,	/* get_wpa_supplicant_state */
	wifi_netmgr_utils_disable_11ax_mode,	/* disable_11ax_mode */
#ifdef CONFIG_ENABLE_HOMELYNK
	wifi_netmgr_utils_set_bridge,		/* set_bridge */
#endif //#ifdef CONFIG_ENABLE_HOMELYNK
};

static trwifi_scan_list_s *g_scan_list;
static int g_scan_num;
extern struct netdev *ameba_nm_dev_wlan0;
extern struct netdev *ameba_nm_dev_wlan1;
static s32 app_scan_result_handler_legacy(rtw_scan_handler_result_t *malloced_scan_result);
#ifndef CONFIG_ENABLE_HOMELYNK
int softap_flag = 0;
#endif //#ifndef CONFIG_ENABLE_HOMELYNK


#define SCAN_TIMER_DURATION 180000

struct ap_scan_list {
	unsigned int channel;
	char ssid[TRWIFI_SSID_LEN + 1];
	unsigned int ssid_length;
	int rssi;
	trwifi_ap_auth_type_e ap_auth_type;
	trwifi_ap_crypto_type_e ap_crypto_type;
};
typedef struct ap_scan_list ap_scan_list_s;

static int valid_ch_list[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 149, 153, 157, 161, 165};
static int valid_ch_list_size = sizeof(valid_ch_list)/sizeof(valid_ch_list[0]);
static ap_scan_list_s *saved_scan_list = NULL;
static uint32_t scan_number = 0;
static _timer scan_timer;
static _mutex scanlistbusy;	//protects shared variables saved_scan_list,scan_number and scan_timer

static void _scan_timer_handler(void *FunctionContext)
{
	vddbg("scan Timer expired : release saved scan list\r\n");

	rtos_mutex_take(scanlistbusy, MUTEX_WAIT_TIMEOUT);
	vddbg("scan Timer expired : sizeof(ap_scan_list_s) =%d, scan_number=%d \r\n", sizeof(ap_scan_list_s), scan_number);

	if (saved_scan_list) {
		rtos_mem_free((void *)saved_scan_list);
		saved_scan_list = NULL;
	}
	scan_number = 0;
	rtw_del_timer(&(scan_timer));
	rtos_mutex_give(scanlistbusy);
}

static int save_scan_list(trwifi_scan_list_s *p_scan_list)
{
	rtos_mutex_take(scanlistbusy, MUTEX_WAIT_TIMEOUT);
	// If application calls scan before scan result free(before timeout), release scan list and cancel timer
	if (scan_number) {
		if (saved_scan_list) {
			rtos_mem_free((void *)saved_scan_list);
			saved_scan_list = NULL;
		}
		rtw_cancel_timer(&(scan_timer));
		vddbg("scan is called before timeout\r\n");
	} else {
		rtw_init_timer(&(scan_timer), &(scan_timer), _scan_timer_handler, &(scan_timer), "dynamic_chk_timer");
		vddbg("Start scan timer\n");
	}
	rtw_set_timer(&(scan_timer), SCAN_TIMER_DURATION);

	scan_number	 = 0;
	saved_scan_list = (ap_scan_list_s *)rtos_mem_malloc(sizeof(ap_scan_list_s) * g_scan_num);
	if (saved_scan_list == NULL) {
		if (scan_timer.timer_hdl != NULL) {
			rtw_cancel_timer(&(scan_timer));
			rtw_del_timer(&(scan_timer));
		}
		rtos_mutex_give(scanlistbusy);
		return RTK_ERR_NOMEM;
	}

	while (p_scan_list) {
		strncpy(saved_scan_list[scan_number].ssid, p_scan_list->ap_info.ssid, p_scan_list->ap_info.ssid_length);
		saved_scan_list[scan_number].ssid_length = p_scan_list->ap_info.ssid_length;
		saved_scan_list[scan_number].channel = p_scan_list->ap_info.channel;
		saved_scan_list[scan_number].rssi = p_scan_list->ap_info.rssi;
		saved_scan_list[scan_number].ap_auth_type = p_scan_list->ap_info.ap_auth_type;
		saved_scan_list[scan_number].ap_crypto_type = p_scan_list->ap_info.ap_crypto_type;
		p_scan_list = p_scan_list->next;
		scan_number++;
	}

	if (scan_number != g_scan_num)
		vddbg("Total scan list temp = %d, g_scan = %d\n", scan_number, g_scan_num);
	rtos_mutex_give(scanlistbusy);

	return RTK_SUCCESS;
}

static void _free_scanlist(void)
{
	while (g_scan_list) {
		trwifi_scan_list_s *cur = g_scan_list;
		g_scan_list = g_scan_list->next;
		rtos_mem_free((void *)cur);
	}
	g_scan_num = 0;
}

static s32 app_scan_result_handler(unsigned int scanned_AP_num, void *user_data)
{
	/* To avoid gcc warnings */
	(void) user_data;

	struct rtw_scan_result *scanned_AP_info;
	rtw_scan_handler_result_t scan_result_report;
	struct rtw_scan_result *scan_buf = NULL;
	unsigned int i = 0;

	if (scanned_AP_num == 0) {/* scanned no AP*/
		TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
		return RTK_FAIL;
	}

	scan_buf = (struct rtw_scan_result *)rtos_mem_zmalloc(scanned_AP_num * sizeof(struct rtw_scan_result));
	if (scan_buf == NULL) {
		TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
		return RTK_FAIL;
	}

	if (wifi_get_scan_records(&scanned_AP_num, scan_buf) < 0) {
		rtos_mem_free((void *)scan_buf);
		TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
		return RTK_FAIL;
	}

	for (i = 0; i < scanned_AP_num; i++) {
		scanned_AP_info = &scan_buf[i];
		scanned_AP_info->ssid.val[scanned_AP_info->ssid.len] = 0; /* Ensure the SSID is null terminated */

		scan_result_report.scan_complete = FALSE;
		scan_result_report.user_data = user_data;
		memcpy(&scan_result_report.ap_details, scanned_AP_info, sizeof(scan_result_report.ap_details));
		app_scan_result_handler_legacy(&scan_result_report);
	}
	scan_result_report.scan_complete = TRUE;
	app_scan_result_handler_legacy(&scan_result_report);

	rtos_mem_free((void *)scan_buf);

	return RTK_SUCCESS;
}

/*
 * Callback
 */
static int rtk_drv_callback_handler(int type)
{
	switch (type) {
	case 1:
	{
		trwifi_cbk_msg_s msg = {TRWIFI_REASON_UNKNOWN, {0,}, NULL};
		trwifi_post_event(ameba_nm_dev_wlan0, LWNL_EVT_STA_CONNECTED, &msg, sizeof(trwifi_cbk_msg_s));
		break;
	}
	case 2:
		trwifi_post_event(ameba_nm_dev_wlan0, LWNL_EVT_STA_CONNECT_FAILED, NULL, 0);
		break;
	case 3:
#ifndef CONFIG_ENABLE_HOMELYNK
		trwifi_post_event(ameba_nm_dev_wlan0, LWNL_EVT_SOFTAP_STA_JOINED, NULL, 0);
#else
		trwifi_post_event(ameba_nm_dev_wlan1, LWNL_EVT_SOFTAP_STA_JOINED, NULL, 0);
#endif //#ifndef CONFIG_ENABLE_HOMELYNK
		break;
	case 4:
		trwifi_post_event(ameba_nm_dev_wlan0, LWNL_EVT_STA_DISCONNECTED, NULL, 0);
		break;
	case 5:
#ifndef CONFIG_ENABLE_HOMELYNK
		trwifi_post_event(ameba_nm_dev_wlan0, LWNL_EVT_SOFTAP_STA_LEFT, NULL, 0);
#else
		trwifi_post_event(ameba_nm_dev_wlan1, LWNL_EVT_SOFTAP_STA_LEFT, NULL, 0);
#endif //#ifndef CONFIG_ENABLE_HOMELYNK
		break;
	default:
		trwifi_post_event(ameba_nm_dev_wlan0, LWNL_EVT_UNKNOWN, NULL, 0);
		break;
	}
	return 0;
}

/*static*/ void linkup_handler(rtk_reason_t *reason)
{
	//RTKDRV_ENTER;
	int type = 0;
	/* Check interface ID callback is called from to know which state it is in */
	if (reason->if_id == RTK_WIFI_STATION_IF) {
		if (reason->reason_code == RTW_JOINSTATUS_SUCCESS) {
			type = 1; // STA connected
		} else {
			type = 2; // STA failed to connect
		}
	} else if (reason->if_id == RTK_WIFI_SOFT_AP_IF) {
		type = 3; // STA joined SoftAP
	}
	(void)rtk_drv_callback_handler(type);
}

/*static*/ void linkdown_handler(rtk_reason_t *reason)
{
	//RTKDRV_ENTER;
	int type = 4;
	if (reason->if_id == RTK_WIFI_STATION_IF) {
		if (reason->reason_code == RTW_JOINSTATUS_FAIL) {
			type = 2; // STA failed to connect
		} else if (reason->reason_code == RTW_JOINSTATUS_DISCONNECT) {
			type = 4; // STA disconnected
		}
	} else if (reason->if_id == RTK_WIFI_SOFT_AP_IF) {
		type = 5; // STA left SoftAP
	}
	(void)rtk_drv_callback_handler(type);
}

rtk_network_link_callback_t g_link_up = NULL;
rtk_network_link_callback_t g_link_down = NULL;

int8_t WiFiRegisterLinkCallback(rtk_network_link_callback_t link_up, rtk_network_link_callback_t link_down)
{
	if (!g_link_up) {
		g_link_up = link_up;
	}
	if (!g_link_down) {
		g_link_down = link_down;
	}

	return RTK_STATUS_SUCCESS;
}
//
// Interface API
//
trwifi_result_e wifi_netmgr_utils_init(struct netdev *dev)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	ndbg("\n[RTK] Init netmgr with dev %s\n",dev->ifname);

	int ret = RTK_STATUS_ERROR;
	ret = WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler);
	if (ret != RTK_STATUS_SUCCESS) {
		printf("[RTK] Link callback handles: register failed !\n");
		return RTK_STATUS_ERROR;
	} else {
		printf("[RTK] Link callback handles: registered\n");
	}

	/* At this stage, no action needs to be done by RTK driver for wlan1 */
	if (!memcmp(dev->ifname, "wlan0", 5)) {
		ret = cmd_wifi_on(RTK_WIFI_STATION_IF);

		if (ret != RTK_STATUS_SUCCESS) {
			ndbg("[RTK] Failed to start STA mode\n");
			return wuret;
		}
#ifndef CONFIG_ENABLE_HOMELYNK
		softap_flag = 0;
#endif //#ifndef CONFIG_ENABLE_HOMELYNK
		rtos_mutex_create(&scanlistbusy);
		g_netmgr_init = TRUE;
	} else if (memcmp(dev->ifname, "wlan1", 5)) {
		ndbg("Invalid interface %s\n",dev->ifname);
		return wuret;
	} 

	wuret = TRWIFI_SUCCESS;
	return wuret;
}

trwifi_result_e wifi_netmgr_utils_deinit(struct netdev *dev)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int ret = cmd_wifi_off();
	if (ret == RTK_STATUS_SUCCESS) {
		wuret = TRWIFI_SUCCESS;
		/* Deinit actions only need to be done for wlan0, wlan1 deinit will be handled in cmd_wifi_stop_ap() */
		if (!memcmp(dev->ifname, "wlan0", 5)) {
			rtos_mutex_take(scanlistbusy, MUTEX_WAIT_TIMEOUT);
			if (scan_timer.timer_hdl != NULL) {
				rtw_cancel_timer(&(scan_timer));
				rtw_del_timer(&(scan_timer));
			}
			if (saved_scan_list) {
				rtos_mem_free((void *)saved_scan_list);
				saved_scan_list = NULL;
			}
			scan_number = 0;
			rtos_mutex_give(scanlistbusy);
			rtos_mutex_delete(scanlistbusy);
			scanlistbusy = NULL;
			g_netmgr_init = FALSE;
		}
	} else {
		ndbg("[RTK] Failed to stop STA mode\n");
	}
	return wuret;
}

trwifi_result_e wifi_netmgr_utils_scan_ap(struct netdev *dev, trwifi_scan_config_s *config)
{
	g_scan_num = 0;
	g_scan_list = NULL;
	char *channel_list = NULL;
	int i = 0;
	int ch_valid = 0;
	struct rtw_scan_param scan_param = {0};

	memset(&scan_param, 0, sizeof(struct rtw_scan_param));
	scan_param.scan_user_callback = app_scan_result_handler; //print_ssid_scan_result
	/* Set the maximum number of APs in scan result to 100 */
	scan_param.max_ap_record_num = 100;

	if (config) {
		for (i = 0; i < valid_ch_list_size; i++) {
			if (config->channel == valid_ch_list[i]) {
				ch_valid = 1;
				break;
			}
		}

		if (ch_valid) {
			/* If Channel Information is given by User */
			channel_list = (char *)malloc(1); //only do 1 channel instead of multiple channels for now
			if (!channel_list) {
				printf("[ATWs]ERROR: Can't malloc memory for channel list\n\r");
				return TRWIFI_FAIL;
			}
			//parse command channel list
			for (i = 0; i < 1 ; i++) { //This loop should be scalable when multiple channels are required by customer
				*(channel_list + i) = (u8)config->channel;
			}

			scan_param.channel_list = (unsigned char *)channel_list;
			scan_param.channel_list_num = 1;

			if (config->ssid_length == 0) {
				/* Scan with channel Only */
				memset(config->ssid, 0, sizeof(config->ssid));
				config->ssid_length = 0;
				printf("[ATWs]: _AT_WLAN_SCAN_WITH_CHANNEL_ [%d]\n\r", scan_param.channel_list[0]);
			}
			else {
				/* Scan with SSID and channel */
				printf("[ATWs]: _AT_WLAN_SCAN_WITH_SSID_AND_CHANNEL [%d], [%s]\n\r", scan_param.channel_list[0], (char *)config->ssid);
			}
		}

		if (config->ssid_length > 0 || (ch_valid)) {
			/* Scan with SSID, or Scan with SSID And Channel */
			scan_param.ssid = (u8 *)config->ssid;
			if(config->ssid_length > 0 && config->channel == 0) {
				printf("[ATWs]: _AT_WLAN_SCAN_WITH_SSID_ [%s]\n\r", (char *)config->ssid);
			}
		} else {
			dbg_noarg("Invalid channel range\n");
			return TRWIFI_FAIL;
		}

		if (wifi_scan_networks(&scan_param, 0) != RTK_SUCCESS) {
			if (channel_list) {
				free(channel_list);
			}
			return TRWIFI_FAIL;
		}
	} else {
		if (wifi_scan_networks(&scan_param, 0) != RTK_SUCCESS) {
			//ndbg("[RTK] [ERR] WiFi scan fail(%d)\n", ret);
			return TRWIFI_FAIL;
		}
	}

	nvdbg("[RTK] WIFi Scan success\n");
	if (channel_list) {
		free(channel_list);
	}
	return TRWIFI_SUCCESS;
}

trwifi_result_e wifi_netmgr_utils_scan_multi_ap(struct netdev *dev, trwifi_scan_multi_configs_s *config)
{
	g_scan_num = 0;
	g_scan_list = NULL;
	int i = 0;
	int j = 0;
	char ch_valid = 0;
	char scan_all_ch = 0;
	u8 channel_list[SCAN_SSID_CNT] = {0};
	u8 channel_list_num = 0;
	struct rtw_scan_multi_param scan_multi_param = {0};

	scan_multi_param.scan_user_callback = app_scan_result_handler;
	scan_multi_param.max_ap_record_num = 100;

	if (config) {
		if (config->scan_ap_config_count > SCAN_SSID_CNT) {
			dbg_noarg("ERROR: SSID count exceeded, maximum allowed:%d given:%d\n\r", SCAN_SSID_CNT, config->scan_ap_config_count);
			TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
			return TRWIFI_INVALID_ARGS;
		}

		if ((config->scan_ap_config_count == 0) && !config->scan_all) {
			dbg_noarg("[RTK][WARN] scan_ap_config_count is 0. Do not scan. \n\r");
			TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
			return TRWIFI_INVALID_ARGS;
		}

		if (config->scan_all) {
			scan_multi_param.options = RTW_SCAN_ALL;
		}

		if (config->scan_ap_config_count) {
			for (i = 0; i < config->scan_ap_config_count; i++) {
				/* If any AP channel is 0, scan all channels */
				if (config->scan_ap_config[i].channel == 0) {
					scan_all_ch = 1;
					break;
				}

				ch_valid = 0;
				for (j = 0; j < valid_ch_list_size; j++) {
					if (config->scan_ap_config[i].channel == valid_ch_list[j]) {
						ch_valid = 1;
						break;
					}
				}

				if (!ch_valid) {
					dbg_noarg("ERROR: Invalid channel for AP %s, given channel %d\n\r", (char *)config->scan_ap_config[i].ssid, config->scan_ap_config[i].channel);
					TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
					return TRWIFI_INVALID_ARGS;
				}
			}

			for (i = 0; i < config->scan_ap_config_count; i++) {
				if (config->scan_ap_config[i].ssid_length > 0) {
					scan_multi_param.ssid[i].len = config->scan_ap_config[i].ssid_length;
					scan_multi_param.ssid[i].ssid = (u8 *)config->scan_ap_config[i].ssid;
				}

				if (!scan_all_ch) {
					char duplicated_channel = 0;
					for (j = 0; j < channel_list_num; j++) {
						if (channel_list[j] == (u8)config->scan_ap_config[i].channel) {
							duplicated_channel = 1;
							break;
						}
					}

					if (!duplicated_channel) {
						channel_list[channel_list_num] = (u8)config->scan_ap_config[i].channel;
						channel_list_num++;
					}
				}
			}

			if (!scan_all_ch && channel_list_num > 0) {
				scan_multi_param.channel_list = channel_list;
				scan_multi_param.channel_list_num = channel_list_num;
			}
		}
	}

	if (wifi_scan_networks_multi(&scan_multi_param, 0) != RTK_SUCCESS) {
		dbg_noarg("[RTK] [ERR] WiFi multi scan fail\n");
		TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
		return TRWIFI_FAIL;
	}

	nvdbg("[RTK] WiFi Multi Scan success\n");
	return TRWIFI_SUCCESS;
}

trwifi_result_e wifi_netmgr_utils_connect_ap(struct netdev *dev, trwifi_ap_config_s *ap_connect_config, void *arg)
{
	trwifi_result_e wuret = TRWIFI_INVALID_ARGS;
	if (!ap_connect_config) {
		return wuret;
	}

	int ret;
	uint32_t ap_channel;
	wuret = TRWIFI_FAIL;
	ap_channel = 0;

	rtos_mutex_take(scanlistbusy, MUTEX_WAIT_TIMEOUT);
	if (scan_number) {
		int i;
		for (i = 0 ; i < scan_number ; i++) {
			if ((strncmp(saved_scan_list[i].ssid, ap_connect_config->ssid, ap_connect_config->ssid_length) == 0)
				&& (ap_connect_config->ssid_length == saved_scan_list[i].ssid_length)
				&& (ap_connect_config->ap_auth_type == saved_scan_list[i].ap_auth_type)
				&& (ap_connect_config->ap_crypto_type == saved_scan_list[i].ap_crypto_type)) {
				ap_channel = saved_scan_list[i].channel;
				vddbg("[RTK] Scanned AP info : ssid = %s, auth = %d, crypt = %d, channel = %d\n", ap_connect_config->ssid, ap_connect_config->ap_auth_type, ap_connect_config->ap_crypto_type, ap_channel);
				/* TizenRT customization: prefer 5GHz */
				if (wifi_user_config.sta_prefer_5ghz_enable == 0 || (saved_scan_list[i].channel > 14 && saved_scan_list[i].rssi > wifi_user_config.sta_prefer_5ghz_rssi_threshold)) {
					break;
				}
			}
		}
	}
	rtos_mutex_give(scanlistbusy);

	ret = cmd_wifi_connect(ap_connect_config, arg, ap_channel);
	if (ret != RTK_STATUS_SUCCESS) {
		ndbg("[RTK] WiFiNetworkJoin failed: %d, %s\n", ret, ap_connect_config->ssid);
		return wuret;
	} else {
		wuret = TRWIFI_SUCCESS;
		nvdbg("[RTK] Successfully joined the network: %s(%d)\n", ap_connect_config->ssid,
			  ap_connect_config->ssid_length);
	}

	return wuret;
}

trwifi_result_e wifi_netmgr_utils_disconnect_ap(struct netdev *dev, void *arg)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int ret = cmd_wifi_disconnect();
	if (ret == RTK_STATUS_SUCCESS) {
		nvdbg("[RTK] WiFiNetworkLeave success\n");
		wuret = TRWIFI_SUCCESS;
	} else {
		ndbg("[RTK] WiFiNetworkLeave fail because of %d\n", ret);
	}

	return wuret;
}

trwifi_result_e wifi_netmgr_utils_get_signal_quality(struct netdev *dev, trwifi_signal_quality *signal_quality)
{
	trwifi_result_e wuret = TRWIFI_INVALID_ARGS;
	int ret;

	if (signal_quality) {
		wuret = TRWIFI_FAIL;
		if (g_netmgr_init) {
			union rtw_traffic_stats traffic_stats = {0};
			union rtw_phy_stats phy_stats = {0};
			struct rtw_wifi_setting wifi_setting_local = {0};
			u32 tx_rty = 0;
#ifndef CONFIG_ENABLE_HOMELYNK
			if (wifi_is_running(SOFTAP_WLAN_INDEX)) {
				if (wifi_get_setting(SOFTAP_WLAN_INDEX, &wifi_setting_local) < 0) {
					/* Failed to get channel */
					signal_quality->channel = 0;
				}
				else {
					signal_quality->channel = wifi_setting_local.channel;
				}
				ret = wifi_get_traffic_stats(SOFTAP_WLAN_INDEX, &traffic_stats);
				if (ret != RTK_SUCCESS) {
					dbg_noarg("[RTK] Failed to get SoftAP traffic stats. ret=%d\n", ret);
				}
				signal_quality->tx_drop = traffic_stats.ap.tx_drop;
				signal_quality->rx_drop = traffic_stats.ap.rx_drop;
				ret = wifi_get_tx_retry(SOFTAP_WLAN_INDEX, &tx_rty);
				if (ret != RTK_SUCCESS) {
					dbg_noarg("[RTK] Failed to get tx retry for softap. ret=%d\n", ret);
				}
			} else
#endif //#ifndef CONFIG_ENABLE_HOMELYNK
			{
				ret = wifi_get_traffic_stats(STA_WLAN_INDEX, &traffic_stats);
				if (ret != RTK_SUCCESS) {
					dbg_noarg("[RTK] Failed to get STA traffic stats. ret=%d\n", ret);
				}
				signal_quality->tx_drop = traffic_stats.sta.tx_drop;
				signal_quality->rx_drop = traffic_stats.sta.rx_drop;
				ret = wifi_get_tx_retry(STA_WLAN_INDEX, &tx_rty);
				if (ret != RTK_SUCCESS) {
					dbg_noarg("[RTK] Failed to get tx retry for sta. ret=%d\n", ret);
				}
				u8 connect_status = RTW_JOINSTATUS_UNKNOWN;
				if (wifi_get_join_status(&connect_status) != RTK_SUCCESS) {
					dbg_noarg("[RTK] Failed to get connection status!");
					return wuret;
				}
				if (connect_status == RTW_JOINSTATUS_SUCCESS) {
					if (wifi_get_phy_stats(STA_WLAN_INDEX, NULL, &phy_stats) == RTK_SUCCESS) {
						memcpy(&signal_quality->snr, &phy_stats.sta.snr, sizeof(signed char));
						signal_quality->max_rate = (unsigned int)phy_stats.sta.supported_max_rate;
					}
					if (wifi_get_setting(STA_WLAN_INDEX, &wifi_setting_local) < 0) {
						/* Failed to get channel */
						signal_quality->channel = 0;
					}
					else {
						signal_quality->channel = wifi_setting_local.channel;
					}
					signal_quality->network_bw = wifi_get_current_bw();
				}
			}
			signal_quality->tx_retry = tx_rty;
			wuret = TRWIFI_SUCCESS;
		} else {
			ndbg("[RTK] Failed to get signal quality, netmgr not initialized \n");
		}
	}

	return wuret;
}

trwifi_result_e wifi_netmgr_utils_get_info(struct netdev *dev, trwifi_info *wifi_info)
{
	trwifi_result_e wuret = TRWIFI_INVALID_ARGS;
	if (wifi_info) {
		wuret = TRWIFI_FAIL;
		if (g_netmgr_init) {
			wifi_info->rssi = (int)0;
			/* Used by TizenRT */
			wifi_info->wifi_status = TRWIFI_DISCONNECTED;
			if (wifi_is_running(SOFTAP_WLAN_INDEX)) {
				/* Used by TizenRT */
				wifi_info->wifi_status = TRWIFI_SOFTAP_MODE;
			}
			/* STA is always on so we can always get stats if we are connected */
			u8 connect_status = RTW_JOINSTATUS_UNKNOWN;
			if(wifi_get_join_status(&connect_status) != RTK_SUCCESS) {
				dbg_noarg("[RTK] Failed to get connection status!");
				return wuret;
			}
			if (connect_status == RTW_JOINSTATUS_SUCCESS) {
				/* Used by TizenRT */
				wifi_info->wifi_status = TRWIFI_CONNECTED;
				union rtw_phy_stats phy_statistics;
				if (wifi_get_phy_stats(STA_WLAN_INDEX, NULL, &phy_statistics) == RTK_SUCCESS){
					wifi_info->rssi = (int)phy_statistics.sta.rssi;
				}
				struct rtw_wifi_setting wlan_setting = {0};
				if (wifi_get_setting(STA_WLAN_INDEX, &wlan_setting) == RTK_SUCCESS) {
					memcpy(wifi_info->bssid, wlan_setting.bssid, ETH_ALEN);
				} else {
					ndbg("[RTK] Fail to get bssid\n");
				}
			}
#ifdef CONFIG_ENABLE_HOMELYNK
			if (g_bridge_on) {
				wifi_info->wifi_status = TRWIFI_BRIDGE_MODE;
			}
#endif //#ifdef CONFIG_ENABLE_HOMELYNK
			wuret = TRWIFI_SUCCESS;
		} else {
			ndbg("[RTK] need to init netmgr... get info fail\n");
		}
	}
	return wuret;
}

trwifi_result_e wifi_netmgr_utils_get_wpa_supplicant_state(struct netdev *dev, trwifi_wpa_states *wpa_supplicant_state)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int key_mgmt = 0;
	u8 previous_join_status;

	/* This API is used to check the supplicant state before disconnection, so use the join status before disconnection to check */
	previous_join_status = wifi_get_prev_join_status();

		switch (previous_join_status) {
			case RTW_JOINSTATUS_UNKNOWN:
				wpa_supplicant_state->wpa_supplicant_state = WPA_INACTIVE;
				break;
			case RTW_JOINSTATUS_SCANNING:
				wpa_supplicant_state->wpa_supplicant_state = WPA_SCANNING;
				break;
			case RTW_JOINSTATUS_AUTHENTICATING:
				wpa_supplicant_state->wpa_supplicant_state = WPA_AUTHENTICATING;
				break;
			case RTW_JOINSTATUS_ASSOCIATING:
				wpa_supplicant_state->wpa_supplicant_state = WPA_ASSOCIATING;
				break;
			case RTW_JOINSTATUS_ASSOCIATED:
				wpa_supplicant_state->wpa_supplicant_state = WPA_ASSOCIATED;
				break;
			case RTW_JOINSTATUS_4WAY_HANDSHAKING:
				wpa_supplicant_state->wpa_supplicant_state = WPA_4WAY_HANDSHAKE;
				break;
			case RTW_JOINSTATUS_4WAY_HANDSHAKE_DONE:
			case RTW_JOINSTATUS_SUCCESS:
				wpa_supplicant_state->wpa_supplicant_state = WPA_COMPLETED;
				break;
			case RTW_JOINSTATUS_FAIL:
			case RTW_JOINSTATUS_DISCONNECT:
				wpa_supplicant_state->wpa_supplicant_state = WPA_DISCONNECTED;
				break;
			default:
				break;
		}

	/* key_mgmt will return the value used in the previous connected network */
	key_mgmt = wifi_get_prev_key_mgmt();
	wpa_supplicant_state->wpa_supplicant_key_mgmt = key_mgmt;
	wuret = TRWIFI_SUCCESS;
	return wuret;
}

trwifi_result_e wifi_netmgr_utils_get_driver_info(struct netdev *dev, trwifi_driver_info *driver_info)
{
	trwifi_result_e wuret = TRWIFI_SUCCESS;
	char lib_ver[64];
	if (wifi_get_lib_version(lib_ver) != RTK_FAIL){
		memcpy(driver_info->lib_version, lib_ver, sizeof(lib_ver));
	}
	else {
		wuret = TRWIFI_FAIL;
	}
	return wuret;
}

trwifi_result_e wifi_netmgr_utils_start_softap(struct netdev *dev, trwifi_softap_config_s *softap_config)
{
	int i = 0;
	int ch_valid = 0;

	if (!softap_config) {
		return TRWIFI_INVALID_ARGS;
	}

	if (wifi_is_running(SOFTAP_WLAN_INDEX)) {
		ndbg("[RTK] softap is already running!\n");
		return TRWIFI_FAIL;
	}
	trwifi_result_e ret = TRWIFI_FAIL;

	for (i = 0; i < valid_ch_list_size; i++){
		if (softap_config->channel == valid_ch_list[i]){
			ch_valid = 1;
			break;
		}
	}
	if (!ch_valid){
		ndbg("\r\nAP channel is wrong: %d;\n", softap_config->channel);
		return TRWIFI_INVALID_ARGS;
	}

	ret = cmd_wifi_ap(softap_config);
	if (ret != RTK_STATUS_SUCCESS) {
		ndbg("[RTK] Failed to start AP mode(%d)\n", ret);
		return ret;
	}
	nvdbg("[RTK] SoftAP with SSID: %s has successfully started!\n", softap_config->ssid);
#ifndef CONFIG_ENABLE_HOMELYNK
	softap_flag = 1;
#endif //#ifndef CONFIG_ENABLE_HOMELYNK
	ret = TRWIFI_SUCCESS;

	return ret;
}

trwifi_result_e wifi_netmgr_utils_start_sta(struct netdev *dev)
{
	trwifi_result_e wuret = TRWIFI_SUCCESS;

#ifndef CONFIG_ENABLE_HOMELYNK
	softap_flag = 0;
#endif //#ifndef CONFIG_ENABLE_HOMELYNK

	return wuret;
}

trwifi_result_e wifi_netmgr_utils_stop_softap(struct netdev *dev)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int ret;
	if (wifi_is_running(SOFTAP_WLAN_INDEX)) {
		ret = cmd_wifi_stop_ap();
		if (ret == RTK_STATUS_SUCCESS) {
#ifndef CONFIG_ENABLE_HOMELYNK
			softap_flag = 0;
#endif //#ifndef CONFIG_ENABLE_HOMELYNK
			wuret = TRWIFI_SUCCESS;
			nvdbg("[RTK] Stop AP mode successfully\n");
		} else {
			ndbg("[RTK] Stop AP mode fail\n");
		}
	} else {
		ndbg("[RTK] AP mode is not running\n");
	}
	return wuret;
}

trwifi_result_e wifi_netmgr_utils_set_autoconnect(struct netdev *dev, uint8_t check)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int ret = RTK_STATUS_SUCCESS; //TODO: Force result to success as autoreconnect is bypassed for now
//	ret = wifi_set_autoreconnect(check);
	if (ret == RTK_STATUS_SUCCESS) {
		wuret = TRWIFI_SUCCESS;
		nvdbg("[RTK] External Autoconnect set to %d\n", check);
	} else {
		ndbg("[RTK] External Autoconnect failed to set %d", check);
	}
	return wuret;
}

trwifi_result_e wifi_netmgr_utils_set_bridge(struct netdev *dev, uint8_t control)
{
	trwifi_result_e wuret = TRWIFI_SUCCESS;
	if (control) {
		g_bridge_on = TRUE;
	} else {
		g_bridge_on = FALSE;
	}
	nvdbg("[RTK] External Bridge mode set to %d\n", control);
	return wuret;
}

trwifi_result_e wifi_netmgr_utils_disable_11ax_mode(struct netdev *dev, uint8_t disable)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	u8 connect_status = RTW_JOINSTATUS_UNKNOWN;
	if (wifi_get_join_status(&connect_status) != RTK_SUCCESS) {
		dbg_noarg("[RTK] Failed to get connection status!");
		return wuret;
	}

	if (((connect_status != RTW_JOINSTATUS_UNKNOWN) && (connect_status != RTW_JOINSTATUS_FAIL) && (connect_status != RTW_JOINSTATUS_DISCONNECT)) || (wifi_is_running(SOFTAP_WLAN_INDEX))){
		dbg_noarg("[RTK] Failed to change network mode, disconnect from AP or stop SoftAP first\n");
		return wuret;
	}
	enum rtw_wireless_mode mode = RTW_80211_MAX;
	if (disable) {
		mode = (mode & (~RTW_80211_AX));
	}
	if (wifi_set_wireless_mode(mode) == RTK_SUCCESS) {
		wuret = TRWIFI_SUCCESS;
	}
	return wuret;
}

trwifi_result_e wifi_netmgr_utils_ioctl(struct netdev *dev, trwifi_msg_s *msg)
{
	if (msg->cmd == TRWIFI_MSG_SET_POWERMODE) {
		int *mode = (int *)msg->data;
		int lps_mode;
		if (*mode == TRWIFI_POWERMODE_ON) {
			ndbg("[RTK] set power mode on\n");
			lps_mode = TRUE;
			wifi_set_lps_enable(lps_mode);
			return TRWIFI_SUCCESS;
		} else if (*mode == TRWIFI_POWERMODE_OFF) {
			ndbg("[RTK] set power mode off\n");
			lps_mode = FALSE;
			wifi_set_lps_enable(lps_mode);
			return TRWIFI_SUCCESS;
		}
	}
	return TRWIFI_NOT_SUPPORTED;
}

trwifi_result_e wifi_netmgr_utils_set_chplan(struct netdev *dev, uint8_t chplan)
{
	trwifi_result_e wuret = TRWIFI_FAIL;

	u8 connect_status = RTW_JOINSTATUS_UNKNOWN;
	if(wifi_get_join_status(&connect_status) != RTK_SUCCESS) {
		dbg_noarg("[RTK] Failed to get connection status!");
		return wuret;
	}

	if (((connect_status != RTW_JOINSTATUS_UNKNOWN) && (connect_status != RTW_JOINSTATUS_FAIL) && (connect_status != RTW_JOINSTATUS_DISCONNECT)) || (wifi_is_running(SOFTAP_WLAN_INDEX))){
		dbg_noarg("[RTK] Failed to set channel plan, disconnect from AP or stop SoftAP before setting\n");
		return wuret;
	}

	int ret = wifi_set_chplan(chplan);
	if (ret == RTK_SUCCESS) {
		wuret = TRWIFI_SUCCESS;
		dbg_noarg("[RTK] Successfully set channel plan to %x\n", chplan);
	} else {
		dbg_noarg("[RTK] Failed to set channel plan, invalid channel plan %x\n", chplan);
	}
	return wuret;
}

trwifi_result_e wifi_netmgr_utils_get_disconn_reason(struct netdev *dev, int *deauth_reason)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	*deauth_reason = wifi_get_last_reason();
	wuret = TRWIFI_SUCCESS;
	return wuret;
}

static s32 app_scan_result_handler_legacy(rtw_scan_handler_result_t *malloced_scan_result)
{
	trwifi_scan_list_s *scan_list;

	if (malloced_scan_result->scan_complete != TRUE) {
		scan_list = (trwifi_scan_list_s *)rtos_mem_zmalloc(sizeof(trwifi_scan_list_s));
		if (scan_list == NULL) {
			ndbg("\r\n[app_scan_result_handler]:Fail to malloc scan_list\r\n");
			return RTK_FAIL;
		}
		struct rtw_scan_result *record = &malloced_scan_result->ap_details;
		record->ssid.val[record->ssid.len] = 0; /* Ensure the SSID is null terminated */
		scan_list->ap_info.channel = record->channel;
		strncpy(scan_list->ap_info.ssid, (const char *)record->ssid.val, record->ssid.len);
		scan_list->ap_info.ssid_length = record->ssid.len;
		snprintf((char *)scan_list->ap_info.bssid, TRWIFI_MACADDR_STR_LEN + 1, "%02x:%02x:%02x:%02x:%02x:%02x", record->bssid.octet[0], record->bssid.octet[1], record->bssid.octet[2], record->bssid.octet[3], record->bssid.octet[4], record->bssid.octet[5]);
		scan_list->ap_info.max_rate = 0;
		scan_list->ap_info.rssi = record->signal_strength;
		switch (record->security) {
		case RTW_SECURITY_OPEN:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_OPEN;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_NONE;
			break;
		case RTW_SECURITY_WEP_SHARED:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WEP_SHARED;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_WEP_64;
			break;
		case RTW_SECURITY_WEP_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WEP_SHARED;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_WEP_128;
			break;
		case RTW_SECURITY_WPA_TKIP_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_PSK;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP;
			break;
		case RTW_SECURITY_WPA_AES_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_PSK;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES;
			break;
		case RTW_SECURITY_WPA_MIXED_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_PSK;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_AND_AES;
			break;
		case RTW_SECURITY_WPA2_AES_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES;
			break;
		case RTW_SECURITY_WPA2_TKIP_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP;
			break;
		case RTW_SECURITY_WPA2_MIXED_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_AND_AES;
			break;
		case RTW_SECURITY_WPA_WPA2_TKIP_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_AND_WPA2_PSK;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP;
			break;
		case RTW_SECURITY_WPA_WPA2_AES_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_AND_WPA2_PSK;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES;
			break;
		case RTW_SECURITY_WPA_WPA2_MIXED_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_AND_WPA2_PSK;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_AND_AES;
			break;
		case RTW_SECURITY_WPA2_AES_CMAC:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_UNKNOWN;
			break;
		case (RTW_SECURITY_WPA_TKIP_PSK | ENTERPRISE_ENABLED):
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_PSK_ENT;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_ENT;
			break;
		case (RTW_SECURITY_WPA_AES_PSK | ENTERPRISE_ENABLED):
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_PSK_ENT;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES_ENT;
			break;
		case (RTW_SECURITY_WPA_MIXED_PSK | ENTERPRISE_ENABLED):
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_PSK_ENT;
			break;
		case (RTW_SECURITY_WPA2_TKIP_PSK | ENTERPRISE_ENABLED):
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK_ENT;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_ENT;
			break;
		case (RTW_SECURITY_WPA2_AES_PSK | ENTERPRISE_ENABLED):
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK_ENT;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES_ENT;
			break;
		case (RTW_SECURITY_WPA2_MIXED_PSK | ENTERPRISE_ENABLED):
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK_ENT;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_AND_AES_ENT;
			break;
		case (RTW_SECURITY_WPA_WPA2_TKIP_PSK | ENTERPRISE_ENABLED):
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_AND_WPA2_PSK_ENT;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_ENT;
			break;
		case (RTW_SECURITY_WPA_WPA2_AES_PSK | ENTERPRISE_ENABLED):
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_AND_WPA2_PSK_ENT;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES_ENT;
			break;
		case (RTW_SECURITY_WPA_WPA2_MIXED_PSK | ENTERPRISE_ENABLED):
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_AND_WPA2_PSK_ENT;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_AND_AES_ENT;
			break;
		// TODO: WPS is not under security type now
		// case RTW_SECURITY_WPS_OPEN:
		// scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPS;
		// scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_NONE;
		// 	break;
		// case RTW_SECURITY_WPS_SECURE:
		// scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPS;
		// scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES;
		// 	break;
		case RTW_SECURITY_WPA3_AES_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA3_PSK;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES;
			break;
		case RTW_SECURITY_WPA2_WPA3_MIXED:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_AND_WPA3_PSK;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_AND_AES;
			break;
		case (RTW_SECURITY_WPA3_AES_PSK | ENTERPRISE_ENABLED): // WPA3 Enterprise not in TizenRT yet
		default:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_UNKNOWN;
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_UNKNOWN;
			break;
		}

		if (record->wireless_mode & RTW_80211_AX){
			/* TizenRT currently doesn't have AX in their enum */
			if (record->band == RTW_BAND_ON_5G){
				scan_list->ap_info.phy_mode = TRWIFI_IEEE_80211_AC;
			} else if (record->band == RTW_BAND_ON_24G){
				scan_list->ap_info.phy_mode = TRWIFI_IEEE_80211_N;
			}
		}
		else if (record->wireless_mode & RTW_80211_AC){
			scan_list->ap_info.phy_mode = TRWIFI_IEEE_80211_AC;
		}
		else if (record->wireless_mode & RTW_80211_N){
			scan_list->ap_info.phy_mode = TRWIFI_IEEE_80211_N;
		}
		else if (record->wireless_mode & RTW_80211_G){
			scan_list->ap_info.phy_mode = TRWIFI_IEEE_80211_G;
		}
		else if (record->wireless_mode & RTW_80211_A){
			scan_list->ap_info.phy_mode = TRWIFI_IEEE_80211_A;
		}
		else if (record->wireless_mode & RTW_80211_B){
			scan_list->ap_info.phy_mode = TRWIFI_IEEE_80211_B;
		}

		if (g_scan_list == NULL) {
			g_scan_list = scan_list;
			g_scan_num = 1;
		} else {
			trwifi_scan_list_s *cur = g_scan_list;
			int idx;
			for (idx = 1; idx < g_scan_num; idx++) {
				cur = cur->next;
			}
			cur->next = scan_list;
			scan_list->next = NULL;
			g_scan_num++;
		}
	} else {
		nvdbg("SCAN DONE: Calling wifi_scan_result_callback\r\n");
		int res;
		trwifi_scan_list_s *p_scan_list = g_scan_list;

		res = save_scan_list(p_scan_list);
		if (res != RTK_SUCCESS)
			vddbg("\r\nFail to malloc scan_list\r\n");
		TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_DONE, (void *)g_scan_list);
		_free_scanlist();
		if (g_scan_list) {
			ndbg("SCAN list is not initialized\n");
			return RTK_FAIL;
		}
	}
	return RTK_SUCCESS;
}
