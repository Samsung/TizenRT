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
#include "wifi_conf.h"
#include "rtk_wifi_utils.h"
#include <debug.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/netmgr/netdev_mgr.h>
// #include "freertos/wrapper.h"
#include "osdep_service.h"
/* WLAN CONFIG ---------------------------------------------------------------*/
#define RTK_OK          0		/*!< RTK_err_t value indicating success (no error) */
#define RTK_FAIL        -1		/*!< Generic RTK_err_t code indicating failure */

#define RTK_ERR_NO_MEM          0x101	/*!< Out of memory */
#define RTK_ERR_INVALID_ARG     0x102	/*!< Invalid argument */
#define RTK_ERR_INVALID_STATE   0x103	/*!< Invalid state */
#define RTK_ERR_INVALID_SIZE    0x104	/*!< Invalid size */
#define RTK_ERR_NOT_FOUND       0x105	/*!< Requested resource not found */
#define RTK_ERR_NOT_SUPPORTED   0x106	/*!< Operation or feature not supported */
#define RTK_ERR_TIMEOUT         0x107	/*!< Operation timed out */
#define RTK_ERR_INVALID_RRTKONSE    0x108	/*!< Received rRTKonse was invalid */
#define RTK_ERR_INVALID_CRC     0x109	/*!< CRC or checksum was invalid */
#define RTK_ERR_INVALID_VERSION     0x10A	/*!< Version was invalid */
#define RTK_ERR_INVALID_MAC     0x10B	/*!< MAC address was invalid */

#define RTK_ERR_WIFI_BASE       0x3000	/*!< Starting number of WiFi error codes */
#define RTK_ERR_MESH_BASE       0x4000	/*!< Starting number of MESH error codes */

#define vTaskDelay(t)   usleep(t*1000)

/* Private define ------------------------------------------------------------*/

//typedef unsigned char    bool;

static WiFi_InterFace_ID_t g_mode = RTK_WIFI_NONE;
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
void print_scan_result(rtw_scan_result_t *record);

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
};

static trwifi_scan_list_s *g_scan_list;
static int g_scan_num;
extern struct netdev *ameba_nm_dev_wlan0;
rtw_result_t app_scan_result_handler_legacy(rtw_scan_handler_result_t *malloced_scan_result);
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

	rtw_mutex_get(&scanlistbusy);
	vddbg("scan Timer expired : sizeof(ap_scan_list_s) =%d, scan_number=%d \r\n", sizeof(ap_scan_list_s), scan_number);

	if (saved_scan_list) {
		rtw_mfree((unsigned char *)saved_scan_list, sizeof(ap_scan_list_s) * scan_number);
		saved_scan_list = NULL;
	}
	scan_number = 0;
	rtw_del_timer(&(scan_timer));
	rtw_mutex_put(&scanlistbusy);
}

static int save_scan_list(trwifi_scan_list_s *p_scan_list)
{
	rtw_mutex_get(&scanlistbusy);
	// If application calls scan before scan result free(before timeout), release scan list and cancel timer
	if (scan_number) {
		if (saved_scan_list) {
			rtw_mfree((unsigned char *)saved_scan_list, sizeof(ap_scan_list_s) * scan_number);
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
	saved_scan_list = (ap_scan_list_s *)rtw_malloc(sizeof(ap_scan_list_s) * g_scan_num);
	if (saved_scan_list == NULL) {
		if (scan_timer.timer_hdl != NULL) {
			rtw_cancel_timer(&(scan_timer));
			rtw_del_timer(&(scan_timer));
		}
		rtw_mutex_put(&scanlistbusy);
		return RTW_NOMEM;
	}

	while(p_scan_list) {
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
	rtw_mutex_put(&scanlistbusy);

	return RTW_SUCCESS;
}

static void _free_scanlist(void)
{
	while (g_scan_list) {
		trwifi_scan_list_s *cur = g_scan_list;
		g_scan_list = g_scan_list->next;
		rtw_mfree((unsigned char *)cur, sizeof(trwifi_scan_list_s));
	}
	g_scan_num = 0;
}

rtw_result_t app_scan_result_handler(unsigned int scanned_AP_num, void *user_data)
{
	/* To avoid gcc warnings */
	(void) user_data;

	rtw_scan_result_t *scanned_AP_info;
	rtw_scan_handler_result_t scan_result_report;
	char *scan_buf = NULL;
	unsigned int i = 0;

	if (scanned_AP_num == 0) {/* scanned no AP*/
		TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
		return RTW_ERROR;
	}

	scan_buf = (char *)rtw_zmalloc(scanned_AP_num * sizeof(rtw_scan_result_t));
	if (scan_buf == NULL) {
		TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
		return RTW_ERROR;
	}

	if (wifi_get_scan_records(&scanned_AP_num, scan_buf) < 0) {
		rtw_mfree((u8 *)scan_buf, 0);
		TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
		return RTW_ERROR;
	}

	for (i = 0; i < scanned_AP_num; i++) {
		scanned_AP_info = (rtw_scan_result_t *)(scan_buf + i * sizeof(rtw_scan_result_t));
		scanned_AP_info->SSID.val[scanned_AP_info->SSID.len] = 0; /* Ensure the SSID is null terminated */

#if ATCMD_VER == ATVER_2
		if (is_str_utf8(scanned_AP_info->SSID.val)) {
			unsigned char GBKbuf[33];
			int GBKbuflen = 0;
			memset(&GBKbuf[0], 0x00, 33);
			SwitchToGbk(scanned_AP_info->SSID.val, scanned_AP_info->SSID.len, (unsigned char *)&GBKbuf, &GBKbuflen);
			strcpy((char *)scanned_AP_info->SSID.val, (char *)GBKbuf);
			scanned_AP_info->SSID.len = strlen((char *)GBKbuf);
		}
#endif
			//print_scan_result(scanned_AP_info); /* Suppress Realtek's ATWS printout format
			scan_result_report.scan_complete = FALSE;
			scan_result_report.user_data = user_data;
			rtw_memcpy(&scan_result_report.ap_details, scanned_AP_info, sizeof(scan_result_report.ap_details));
			app_scan_result_handler_legacy(&scan_result_report);
	}
	scan_result_report.scan_complete = TRUE;
	app_scan_result_handler_legacy(&scan_result_report);

	rtw_mfree((u8 *)scan_buf, 0);
	// Removed post_scanevent here as it is already done in the previous call to app_scan_result_handler_legacy()
	// TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_DONE, (void *)g_scan_list);

#if (defined(SUPPORT_UART_LOG_SERVICE) && SUPPORT_UART_LOG_SERVICE) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
	at_printf("\r\n[ATWS] OK");
	at_printf(STR_END_OF_ATCMD_RET);
#endif

	return RTW_SUCCESS;
}

#if 0 //deprecated function
int parse_scan_with_ssid_res(char*buf, int buflen, char *target_ssid, void *user_data)
{
	if (rltk_wlan_scan_with_ssid_by_extended_security_is_enable()) {
			int plen = 0, scan_cnt = 0;
			rtw_scan_handler_result_t scan_result_report;
			scan_result_report.scan_complete = FALSE;
			scan_result_report.user_data = user_data;
			while (plen < buflen) {
				int len;
				rtw_memset(&scan_result_report.ap_details, 0, sizeof(scan_result_report.ap_details));
				len = (int)*(buf + plen);
				if (len == 0) break;
				scan_result_report.ap_details.SSID.len = len - 1 - 6 - 4 - 4 - 1 - 1;
				rtw_memcpy(&scan_result_report.ap_details.SSID.val, (buf + plen + 1 + 6 + 4 + 4 + 1 + 1), scan_result_report.ap_details.SSID.len);
				rtw_memcpy(&scan_result_report.ap_details.BSSID.octet, buf + plen + 1, 6);
				rtw_memcpy(&scan_result_report.ap_details.signal_strength, buf + plen + 1 + 6, 4);
				rtw_memcpy(&scan_result_report.ap_details.security, buf + plen + 1 + 6 + 4, 4);
				scan_result_report.ap_details.wps_type = (int)*(buf + plen + 1 + 6 + 4 + 4);
				scan_result_report.ap_details.channel = (int)*(buf + plen + 1 + 6 + 4 + 4 + 1);
				app_scan_result_handler(&scan_result_report);
				plen += len;
				scan_cnt++;
			}
			//RTW_API_INFO("\n\rwifi_scan: scan count = %d, scan_cnt);
			rltk_wlan_enable_scan_with_ssid_by_extended_security(0);
			scan_result_report.scan_complete = TRUE;
			app_scan_result_handler(&scan_result_report);
			return RTW_SUCCESS;
		} else {
			RTW_API_INFO("scan_with_ssid_by_extended_security is not enabled\n");
			return RTW_ERROR;
		}
}
#endif
/*
 * Callback
 */
/* Does not seems to be used anymore */
// extern unsigned char ap_bssid[ETH_ALEN];
static int rtk_drv_callback_handler(int type)
{
	switch (type) {
	case 1:
	{
		trwifi_cbk_msg_s msg = {TRWIFI_REASON_UNKNOWN, {0,}, NULL};
		// rtw_memcpy(msg.bssid, ap_bssid, ETH_ALEN);
		trwifi_post_event(ameba_nm_dev_wlan0, LWNL_EVT_STA_CONNECTED, &msg, sizeof(trwifi_cbk_msg_s));
		break;
	}
	case 2:
		trwifi_post_event(ameba_nm_dev_wlan0, LWNL_EVT_STA_CONNECT_FAILED, NULL, 0);
		break;
	case 3:
		trwifi_post_event(ameba_nm_dev_wlan0, LWNL_EVT_SOFTAP_STA_JOINED, NULL, 0);
		break;
	case 4:
		trwifi_post_event(ameba_nm_dev_wlan0, LWNL_EVT_STA_DISCONNECTED, NULL, 0);
		break;
	case 5:
		trwifi_post_event(ameba_nm_dev_wlan0, LWNL_EVT_SOFTAP_STA_LEFT, NULL, 0);
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

	if (g_mode == RTK_WIFI_STATION_IF) {
		if (reason->reason_code == RTK_STATUS_SUCCESS) {
			type = 1;
		} else {
			type = 2;
		}
	} else if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		type = 3;
	}
	(void)rtk_drv_callback_handler(type);
}

/*static*/ void linkdown_handler(rtk_reason_t *reason)
{
	//RTKDRV_ENTER;
	int type = 4;
	if (g_mode == RTK_WIFI_STATION_IF) {
		type = 4;
	} else if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		type = 5;
	}
	(void)rtk_drv_callback_handler(type);
}


//
// Interface API
//
trwifi_result_e wifi_netmgr_utils_init(struct netdev *dev)
{
	trwifi_result_e wuret = TRWIFI_FAIL;

	if (g_mode == RTK_WIFI_NONE) {
		int ret = RTK_STATUS_SUCCESS;

		ret = WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler);

		if (ret != RTK_STATUS_SUCCESS) {
			ndbg("[RTK] Link callback handles: register failed !\n");
			return wuret;
		} else {
			nvdbg("[RTK] Link callback handles: registered\n");
		}

		ret = cmd_wifi_on(RTK_WIFI_STATION_IF);

		if (ret != RTK_STATUS_SUCCESS) {
			ndbg("[RTK] Failed to start STA mode\n");
			return wuret;
		}
		g_mode = RTK_WIFI_STATION_IF;
		/*extern const char lib_wlan_rev[];
		RTW_API_INFO("\n\rwlan_version %s\n", lib_wlan_rev);*/
		wuret = TRWIFI_SUCCESS;
#ifndef CONFIG_ENABLE_HOMELYNK
		softap_flag = 0;
#endif //#ifndef CONFIG_ENABLE_HOMELYNK
		rtw_mutex_init(&scanlistbusy);
	} else {
		ndbg("Already %d\n", g_mode);
	}
	return wuret;
	//return TRWIFI_FAIL;
}

trwifi_result_e wifi_netmgr_utils_deinit(struct netdev *dev)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int ret = cmd_wifi_off();
	if (ret == RTK_STATUS_SUCCESS) {
		g_mode = RTK_WIFI_NONE;
		wuret = TRWIFI_SUCCESS;
		rtw_mutex_get(&scanlistbusy);
		if (scan_timer.timer_hdl != NULL) {
			rtw_cancel_timer(&(scan_timer));
			rtw_del_timer(&(scan_timer));
		}
		if (saved_scan_list) {
			rtw_mfree((unsigned char *)saved_scan_list, sizeof(ap_scan_list_s) * scan_number);
			saved_scan_list = NULL;
		}
		scan_number = 0;
		rtw_mutex_put(&scanlistbusy);
		rtw_mutex_free(&scanlistbusy);
	} else {
		ndbg("[RTK] Failed to stop STA mode\n");
	}
	//wlan_deinitialize();
	return wuret;
}

trwifi_result_e wifi_netmgr_utils_scan_ap(struct netdev *dev, trwifi_scan_config_s *config)
{
	g_scan_num = 0;
	g_scan_list = NULL;
	char *channel_list = NULL;
	int i = 0;
	int ch_valid = 0;
	rtw_scan_param_t scan_param = {0};

	rtw_memset(&scan_param, 0, sizeof(rtw_scan_param_t));
	scan_param.scan_user_callback = app_scan_result_handler; //print_ssid_scan_result

	if (config) {
		for (i = 0; i < valid_ch_list_size; i++){
			if (config->channel == valid_ch_list[i]){
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
				rtw_memset(config->ssid, 0, sizeof(config->ssid));
				config->ssid_length = 0;
				printf("[ATWs]: _AT_WLAN_SCAN_WITH_CHANNEL_ [%d]\n\r", scan_param.channel_list[0]);
			}
			else {
				/* Scan with SSID and channel */
				printf("[ATWs]: _AT_WLAN_SCAN_WITH_SSID_AND_CHANNEL [%d], [%s]\n\r", scan_param.channel_list[0], (char *)config->ssid);
			}
		}

		if(config->ssid_length > 0 || (ch_valid)) {
			/* Scan with SSID, or Scan with SSID And Channel */
			scan_param.ssid[0].ssid = (char *)config->ssid;
			if(config->ssid_length > 0 && config->channel == 0) {
				printf("[ATWs]: _AT_WLAN_SCAN_WITH_SSID_ [%s]\n\r", (char *)config->ssid);
			}
		} else {
			RTW_API_INFO("Invalid channel range\n");
			return TRWIFI_FAIL;
		}

		if (wifi_scan_networks(&scan_param, 0) != RTW_SUCCESS) {
			if (channel_list) {
				free(channel_list);
			}
			return TRWIFI_FAIL;
		}
	} else {
		if (wifi_scan_networks(&scan_param, 0) != RTW_SUCCESS) {
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
	char *channel_list = NULL;
	int i,j = 0;
	char ch_valid, scan_all_ch = 0;
	rtw_scan_param_t scan_param = {0};

	rtw_memset(&scan_param, 0, sizeof(rtw_scan_param_t));
	scan_param.scan_user_callback = app_scan_result_handler;

	if (config) {
		if (config->scan_ap_config_count) {
			if (config->scan_ap_config_count > SSID_SCAN_NUM) {
				RTW_API_INFO("ERROR: SSID count exceeded, maximum allowed:%d given:%d\n\r",SSID_SCAN_NUM,config->scan_ap_config_count);
				TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
				return TRWIFI_INVALID_ARGS;
			}
			for (i = 0; i < config->scan_ap_config_count; i++) {
				/* Scan all channels if any channel in scan config is set to 0 */
				if (config->scan_ap_config[i].channel == 0) {
					scan_all_ch = 1;
					/* Skip checking of channel validity if scanning all channels */
					break;
				}
				ch_valid = 0;
				/* Check that channels provided for each AP are valid */
				for (j = 0; j < valid_ch_list_size; j++) {
					if (config->scan_ap_config[i].channel == valid_ch_list[j]) {
						ch_valid = 1;
						break;
					}
				}
				if (!ch_valid) {
					RTW_API_INFO("ERROR: Invalid channel for AP %s, given channel %d\n\r",(char *)config->scan_ap_config[i].ssid, config->scan_ap_config[i].channel);
					TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
					return TRWIFI_INVALID_ARGS;
				}
			}

			if (scan_all_ch) {
				scan_param.channel_list_num = 0;
			} else {
				channel_list = (char *)malloc(config->scan_ap_config_count);
				if (!channel_list) {
					RTW_API_INFO("ERROR: Can't malloc memory for channel list\n\r");
					TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
					return TRWIFI_FAIL;
				}
				scan_param.channel_list = (unsigned char *)channel_list;
				scan_param.channel_list_num = config->scan_ap_config_count;
			}

			/* Prepare scan param */
			for (i = 0; i < config->scan_ap_config_count; i++) {
				scan_param.ssid[i].ssid = (char *)config->scan_ap_config[i].ssid;
				/* Prepare list of channels to scan if not scanning all channels */
				if (!scan_all_ch) {
					*(channel_list + i) = (u8)config->scan_ap_config[i].channel;
				}
			}
		}

		/* If scan_all is set, set scan option to RTW_SCAN_ALL to scan for specific AP + other APs responding to NULL probe req */
		if (config->scan_all) {
			scan_param.options = RTW_SCAN_ALL;
		} else {
			if (config->scan_ap_config_count == 0) {
				/* do not scan if scan_all is false and scan_ap_config_count is 0 */
				RTW_API_INFO("[RTK][WARN] scan_ap_config_count is 0. Do not scan. \n\r");
				TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
				return TRWIFI_INVALID_ARGS;
			}
		}
		if (wifi_scan_networks(&scan_param, 0) != RTW_SUCCESS) {
			if (channel_list) {
				free(channel_list);
			}
			TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
			return TRWIFI_FAIL;
		}
		if (channel_list) {
			free(channel_list);
		}
	} else {
		if (wifi_scan_networks(&scan_param, 0) != RTW_SUCCESS) {
			RTW_API_INFO("[RTK] [ERR] WiFi scan fail\n");
			TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
			return TRWIFI_FAIL;
		}
	}
	nvdbg("[RTK] WIFi Scan success\n");
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

	if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		if (wifi_netmgr_utils_deinit(dev)) {
			ndbg("[RTK] Failed to stop AP mode\n");
			return TRWIFI_FAIL;
		}
		vTaskDelay(20);
		if (wifi_netmgr_utils_init(dev) < 0) {
			ndbg("\n\rERROR: Wifi on failed!");
			return TRWIFI_FAIL;
		}
	}

	ap_channel = 0;

	rtw_mutex_get(&scanlistbusy);
	if (scan_number) {
		int i;
		for (i = 0 ; i < scan_number ; i++) {
			if((strncmp(saved_scan_list[i].ssid, ap_connect_config->ssid, ap_connect_config->ssid_length) == 0)
				&& (ap_connect_config->ap_auth_type == saved_scan_list[i].ap_auth_type)
				&& (ap_connect_config->ap_crypto_type == saved_scan_list[i].ap_crypto_type)) {
				ap_channel = saved_scan_list[i].channel;
			vddbg("[RTK] Scanned AP info : ssid = %s, auth = %d, crypt = %d, channel = %d\n", ap_connect_config->ssid, ap_connect_config->ap_auth_type, ap_connect_config->ap_crypto_type, ap_channel);
				break;
			}
		}
	}
	rtw_mutex_put(&scanlistbusy);

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

	if (signal_quality) {
		wuret = TRWIFI_FAIL;
		if (g_mode != RTK_WIFI_NONE) {
			rtw_sw_statistics_t sw_stats = {0};
			u8 channel;
			u32 tx_rty = 0;
			if (g_mode == RTK_WIFI_SOFT_AP_IF) {
				if (wifi_get_channel(1, &channel) < 0){
					/* Failed to get channel */
					signal_quality->channel = 0;
				}
				else {
					signal_quality->channel = channel;
				}
				wifi_get_sw_statistic(SOFTAP_WLAN_INDEX, &sw_stats);
				tx_rty = wifi_get_tx_retry(SOFTAP_WLAN_INDEX);
			} else if (g_mode == RTK_WIFI_STATION_IF) {
				wifi_get_sw_statistic(STA_WLAN_INDEX, &sw_stats);
				tx_rty = wifi_get_tx_retry(STA_WLAN_INDEX);
				if (wifi_is_connected_to_ap() == RTK_STATUS_SUCCESS) {
					rtw_phy_statistics_t phy_statistics;
					if (wifi_fetch_phy_statistic(&phy_statistics) == RTK_STATUS_SUCCESS){
						memcpy(&signal_quality->snr, &phy_statistics.snr, sizeof(signed char));
						signal_quality->max_rate = (unsigned int)phy_statistics.supported_max_rate;
					}
					if (wifi_get_channel(0, &channel) < 0){
						/* Failed to get channel */
						signal_quality->channel = 0;
					}
					else {
						signal_quality->channel = channel;
					}
					signal_quality->network_bw = wifi_get_current_bw();
				}
			}
			signal_quality->tx_drop = sw_stats.tx_dropped;
			signal_quality->rx_drop = sw_stats.rx_dropped;
			signal_quality->tx_retry = tx_rty;
			wuret = TRWIFI_SUCCESS;
		} else {
			ndbg("[RTK] Failed to get signal quality, wifi not initialized \n");
		}
	}

	return wuret;
}

trwifi_result_e wifi_netmgr_utils_get_info(struct netdev *dev, trwifi_info *wifi_info)
{
	trwifi_result_e wuret = TRWIFI_INVALID_ARGS;
	if (wifi_info) {
		wuret = TRWIFI_FAIL;
		if (g_mode != RTK_WIFI_NONE) {
			wifi_info->rssi = (int)0;
			if (g_mode == RTK_WIFI_SOFT_AP_IF) {
				wifi_info->wifi_status = TRWIFI_SOFTAP_MODE;
			} else if (g_mode == RTK_WIFI_STATION_IF) {
				if (wifi_is_connected_to_ap() == RTK_STATUS_SUCCESS) {
					wifi_info->wifi_status = TRWIFI_CONNECTED;
					rtw_phy_statistics_t phy_statistics;
					if (wifi_fetch_phy_statistic(&phy_statistics) == RTK_STATUS_SUCCESS){
						wifi_info->rssi = (int)phy_statistics.rssi;
					}
				} else {
					wifi_info->wifi_status = TRWIFI_DISCONNECTED;
				}
			}
			wuret = TRWIFI_SUCCESS;
		} else {
			ndbg("[RTK] need to init... get info fail\n");
		}
	}

	return wuret;
}

trwifi_result_e wifi_netmgr_utils_get_wpa_supplicant_state(struct netdev *dev, trwifi_wpa_states *wpa_supplicant_state)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int key_mgmt = 0;
	rtw_join_status_t previous_join_status;

	if (g_mode == RTK_WIFI_STATION_IF){
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

		/* key_mgmt will return the value used in the last disconnected network */
		key_mgmt = wifi_get_key_mgmt();
		wpa_supplicant_state->wpa_supplicant_key_mgmt = key_mgmt;
		wuret = TRWIFI_SUCCESS;
	}
	return wuret;
}

trwifi_result_e wifi_netmgr_utils_get_driver_info(struct netdev *dev, trwifi_driver_info *driver_info)
{
	trwifi_result_e wuret = TRWIFI_SUCCESS;
	char lib_ver[64];
	if (wifi_get_lib_version(lib_ver) != RTW_ERROR){
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

	if (g_mode == RTK_WIFI_SOFT_AP_IF)
		ndbg("[RTK] softap is already running!\n");

	trwifi_result_e ret = TRWIFI_FAIL;

	ret = WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler);
	if (ret != RTK_STATUS_SUCCESS) {
		ndbg("[RTK] Link callback handles: register failed !\n");
		return TRWIFI_FAIL;
	} else {
		nvdbg("[RTK] Link callback handles: registered\n");
	}

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
	g_mode = RTK_WIFI_SOFT_AP_IF;
	nvdbg("[RTK] SoftAP with SSID: %s has successfully started!\n", softap_config->ssid);
#ifndef CONFIG_ENABLE_HOMELYNK
	softap_flag = 1;
#endif //#ifndef CONFIG_ENABLE_HOMELYNK
	ret = TRWIFI_SUCCESS;

	return ret;
}

trwifi_result_e wifi_netmgr_utils_start_sta(struct netdev *dev)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int ret = RTK_STATUS_SUCCESS;

	if (g_mode == RTK_WIFI_STATION_IF)
		ndbg("[RTK] station is already running!\n");

	ret = WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler);
	if (ret != RTK_STATUS_SUCCESS) {
		ndbg("[RTK] Link callback handles: register failed !\n");
		return wuret;
	} else {
		nvdbg("[RTK] Link callback handles: registered\n");
	}

	cmd_wifi_off();
	vTaskDelay(20);
	ret = cmd_wifi_on(RTK_WIFI_STATION_IF);
	if (ret == RTK_STATUS_SUCCESS) {
		g_mode = RTK_WIFI_STATION_IF;
		wuret = TRWIFI_SUCCESS;
#ifndef CONFIG_ENABLE_HOMELYNK
		softap_flag = 0;
#endif //#ifndef CONFIG_ENABLE_HOMELYNK
	} else {
		ndbg("[RTK] Failed to start STA mode\n");
	}
	return wuret;
}

trwifi_result_e wifi_netmgr_utils_stop_softap(struct netdev *dev)
{
	trwifi_result_e wuret = TRWIFI_FAIL;
	int ret;
	if (g_mode == RTK_WIFI_SOFT_AP_IF) {
		ret = cmd_wifi_stop_ap();
		if (ret == RTK_STATUS_SUCCESS) {
			g_mode = RTK_WIFI_NONE;
			wuret = TRWIFI_SUCCESS;
#ifndef CONFIG_ENABLE_HOMELYNK
			softap_flag = 0;
#endif //#ifndef CONFIG_ENABLE_HOMELYNK
			nvdbg("[RTK] Stop AP mode successfully\n");
		} else {
			ndbg("[RTK] Stop AP mode fail\n");
		}
	} else {
		ndbg("[RTK] Mode is not AP mode\n");
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
	if ((wifi_is_connected_to_ap() == RTK_STATUS_SUCCESS) || (g_mode == RTK_WIFI_SOFT_AP_IF)){
		RTW_API_INFO("[RTK] Failed to set channel plan, disconnect from AP or stop SoftAP before setting\n");
		return wuret;
	}

	int ret = wifi_set_chplan(chplan);
	if (ret == RTK_STATUS_SUCCESS) {
		wuret = TRWIFI_SUCCESS;
		RTW_API_INFO("[RTK] Successfully set channel plan to %x\n", chplan);
	} else {
		RTW_API_INFO("[RTK] Failed to set channel plan, invalid channel plan %x\n", chplan);
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

void print_scan_result(rtw_scan_result_t *record)
{
#if (defined(SUPPORT_UART_LOG_SERVICE) && SUPPORT_UART_LOG_SERVICE) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
	at_printf("%s,%d,%s,%d,"MAC_FMT"", record->SSID.val, record->channel,
			  (record->security == RTW_SECURITY_OPEN) ? "Open" :
			  (record->security == RTW_SECURITY_WEP_PSK) ? "WEP" :
			  (record->security == RTW_SECURITY_WPA_TKIP_PSK) ? "WPA TKIP" :
			  (record->security == RTW_SECURITY_WPA_AES_PSK) ? "WPA AES" :
			  (record->security == RTW_SECURITY_WPA2_AES_PSK) ? "WPA2 AES" :
			  (record->security == RTW_SECURITY_WPA2_TKIP_PSK) ? "WPA2 TKIP" :
			  (record->security == RTW_SECURITY_WPA2_MIXED_PSK) ? "WPA2 Mixed" :
			  (record->security == RTW_SECURITY_WPA_WPA2_MIXED) ? "WPA/WPA2 AES" :
			  (record->security == RTW_SECURITY_WPA2_ENTERPRISE) ? "WPA2 Enterprise" :
			  (record->security == RTW_SECURITY_WPA_WPA2_ENTERPRISE) ? "WPA/WPA2 Enterprise" :
#ifdef CONFIG_SAE_SUPPORT
			  (record->security == RTW_SECURITY_WPA3_AES_PSK) ? "WPA3-SAE AES" :
#endif
			  "Unknown",
			  record->signal_strength, MAC_ARG(record->BSSID.octet));

#else
	RTW_API_INFO("%s\t ", (record->bss_type == RTW_BSS_TYPE_ADHOC) ? "Adhoc" : "Infra");
	RTW_API_INFO(MAC_FMT, MAC_ARG(record->BSSID.octet));
	RTW_API_INFO(" %d\t ", record->signal_strength);
	RTW_API_INFO(" %d\t  ", record->channel);
	RTW_API_INFO(" %d\t  ", (unsigned int)record->wps_type);
	RTW_API_INFO("%s\t\t ", (record->security == RTW_SECURITY_OPEN) ? "Open" :
				 (record->security == RTW_SECURITY_WEP_PSK) ? "WEP" :
				 (record->security == RTW_SECURITY_WPA_TKIP_PSK) ? "WPA TKIP" :
				 (record->security == RTW_SECURITY_WPA_AES_PSK) ? "WPA AES" :
				 (record->security == RTW_SECURITY_WPA2_AES_PSK) ? "WPA2 AES" :
				 (record->security == RTW_SECURITY_WPA2_TKIP_PSK) ? "WPA2 TKIP" :
				 (record->security == RTW_SECURITY_WPA2_MIXED_PSK) ? "WPA2 Mixed" :
				 (record->security == RTW_SECURITY_WPA_WPA2_MIXED_PSK) ? "WPA/WPA2 TKIP and AES" :
				 (record->security == RTW_SECURITY_WPA2_AES_ENTERPRISE) ? "WPA2 Enterprise" :
				 (record->security == RTW_SECURITY_WPA_WPA2_AES_ENTERPRISE) ? "WPA/WPA2 Enterprise" :
#ifdef CONFIG_SAE_SUPPORT
				 (record->security == RTW_SECURITY_WPA3_AES_PSK) ? "WPA3-SAE AES" :
#endif
				 "Unknown");

	RTW_API_INFO(" %s ", record->SSID.val);
	RTW_API_INFO("\r\n");

#endif
}

rtw_result_t app_scan_result_handler_legacy(rtw_scan_handler_result_t *malloced_scan_result)
{
	trwifi_scan_list_s *scan_list;

	if (malloced_scan_result->scan_complete != TRUE) {
		scan_list = (trwifi_scan_list_s *)rtos_mem_zmalloc(sizeof(trwifi_scan_list_s));
		if (scan_list == NULL) {
			ndbg("\r\n[app_scan_result_handler]:Fail to malloc scan_list\r\n");
			return RTW_ERROR;
		}
		rtw_scan_result_t *record = &malloced_scan_result->ap_details;
		record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */
		scan_list->ap_info.channel = record->channel;
		strncpy(scan_list->ap_info.ssid, (const char *)record->SSID.val, record->SSID.len);
		scan_list->ap_info.ssid_length = record->SSID.len;
		snprintf((char *)scan_list->ap_info.bssid, TRWIFI_MACADDR_STR_LEN + 1, "%02x:%02x:%02x:%02x:%02x:%02x", record->BSSID.octet[0], record->BSSID.octet[1], record->BSSID.octet[2], record->BSSID.octet[3], record->BSSID.octet[4], record->BSSID.octet[5]);
		scan_list->ap_info.max_rate = 0;
		scan_list->ap_info.rssi = record->signal_strength;
		scan_list->ap_info.phy_mode = 0x00000004; //bit2 is set to 1 if support 11n
		switch (record->security) {
		case RTW_SECURITY_OPEN:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_OPEN;
			break;
		case RTW_SECURITY_WEP_SHARED:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WEP_SHARED;
			break;
		case RTW_SECURITY_WEP_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WEP_SHARED;
			break;
		case RTW_SECURITY_WPA_TKIP_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_PSK;
			break;
		case RTW_SECURITY_WPA_AES_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_PSK;
			break;
		case RTW_SECURITY_WPA_MIXED_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_PSK;
			break;
		case RTW_SECURITY_WPA2_AES_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK;
			break;
		case RTW_SECURITY_WPA2_TKIP_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK;
			break;
		case RTW_SECURITY_WPA2_MIXED_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK;
			break;
		case RTW_SECURITY_WPA_WPA2_TKIP_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_AND_WPA2_PSK;
			break;
		case RTW_SECURITY_WPA_WPA2_AES_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_AND_WPA2_PSK;
			break;
		case RTW_SECURITY_WPA_WPA2_MIXED_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_AND_WPA2_PSK;
			break;
		case RTW_SECURITY_WPA2_AES_CMAC:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK;
			break;
		case RTW_SECURITY_WPA_TKIP_ENTERPRISE:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_PSK_ENT;
			break;
		case RTW_SECURITY_WPA_AES_ENTERPRISE:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_PSK_ENT;
			break;
		case RTW_SECURITY_WPA_MIXED_ENTERPRISE:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_PSK_ENT;
			break;
		case RTW_SECURITY_WPA2_TKIP_ENTERPRISE:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK_ENT;
			break;
		case RTW_SECURITY_WPA2_AES_ENTERPRISE:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK_ENT;
			break;
		case RTW_SECURITY_WPA2_MIXED_ENTERPRISE:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_PSK_ENT;
			break;
		case RTW_SECURITY_WPA_WPA2_TKIP_ENTERPRISE:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_AND_WPA2_PSK_ENT;
			break;
		case RTW_SECURITY_WPA_WPA2_AES_ENTERPRISE:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_AND_WPA2_PSK_ENT;
			break;
		case RTW_SECURITY_WPA_WPA2_MIXED_ENTERPRISE:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA_AND_WPA2_PSK_ENT;
			break;
		case RTW_SECURITY_WPS_OPEN:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPS;
			break;
		case RTW_SECURITY_WPS_SECURE:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPS;
			break;
		case RTW_SECURITY_WPA3_AES_PSK:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA3_PSK;
			break;
		case RTW_SECURITY_WPA2_WPA3_MIXED:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_WPA2_AND_WPA3_PSK;
			break;
		case RTW_SECURITY_WPA3_ENTERPRISE:
		default:
			scan_list->ap_info.ap_auth_type = TRWIFI_AUTH_UNKNOWN;
			break;
		}

		switch (record->security) {
		case RTW_SECURITY_OPEN:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_NONE;
			break;
		case RTW_SECURITY_WEP_SHARED:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_WEP_64;
			break;
		case RTW_SECURITY_WEP_PSK:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_WEP_128;
			break;
		case RTW_SECURITY_WPA_TKIP_PSK:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP;
			break;
		case RTW_SECURITY_WPA_AES_PSK:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES;
			break;
		case RTW_SECURITY_WPA_MIXED_PSK:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_AND_AES;
			break;
		case RTW_SECURITY_WPA2_AES_PSK:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES;
			break;
		case RTW_SECURITY_WPA2_TKIP_PSK:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP;
			break;
		case RTW_SECURITY_WPA2_MIXED_PSK:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_AND_AES;
			break;
		case RTW_SECURITY_WPA_WPA2_TKIP_PSK:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP;
			break;
		case RTW_SECURITY_WPA_WPA2_AES_PSK:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES;
			break;
		case RTW_SECURITY_WPA_WPA2_MIXED_PSK:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_AND_AES;
			break;
		case RTW_SECURITY_WPA2_AES_CMAC:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_UNKNOWN;
			break;
		case RTW_SECURITY_WPA_TKIP_ENTERPRISE:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_ENT;
			break;
		case RTW_SECURITY_WPA_AES_ENTERPRISE:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES_ENT;
			break;
		case RTW_SECURITY_WPA_MIXED_ENTERPRISE:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_AND_AES_ENT;
			break;
		case RTW_SECURITY_WPA2_TKIP_ENTERPRISE:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_ENT;
			break;
		case RTW_SECURITY_WPA2_AES_ENTERPRISE:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES_ENT;
			break;
		case RTW_SECURITY_WPA2_MIXED_ENTERPRISE:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_AND_AES_ENT;
			break;
		case RTW_SECURITY_WPA_WPA2_TKIP_ENTERPRISE:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_ENT;
			break;
		case RTW_SECURITY_WPA_WPA2_AES_ENTERPRISE:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES_ENT;
			break;
		case RTW_SECURITY_WPA_WPA2_MIXED_ENTERPRISE:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_TKIP_AND_AES_ENT;
			break;
		case RTW_SECURITY_WPS_OPEN:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_NONE;
			break;
		case RTW_SECURITY_WPS_SECURE:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES;
			break;
		case RTW_SECURITY_WPA3_AES_PSK:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES;
			break;
		case RTW_SECURITY_WPA2_WPA3_MIXED:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_AES;
			break;
		case RTW_SECURITY_WPA3_ENTERPRISE:
		default:
			scan_list->ap_info.ap_crypto_type = TRWIFI_CRYPTO_UNKNOWN;
			break;
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
		if (res != RTW_SUCCESS)
			vddbg("\r\nFail to malloc scan_list\r\n");
		TRWIFI_POST_SCANEVENT(ameba_nm_dev_wlan0, LWNL_EVT_SCAN_DONE, (void *)g_scan_list);
		_free_scanlist();
		if (g_scan_list) {
			ndbg("SCAN list is not initialized\n");
			return RTW_ERROR;
		}
	}
	return RTW_SUCCESS;
}
