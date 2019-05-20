/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
  ******************************************************************************
  * @file    rtk_wifi_utils.h
  * @author
  * @version
  * @brief   This file provides user interface for Wi-Fi station and AP mode configuration 
  *             base on the functionalities provided by Realtek Wi-Fi driver.
  ******************************************************************************
  */
#ifndef __RTK_WIFI_UTILS_H
#define __RTK_WIFI_UTILS_H

#include "wifi_common.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#include <sys/types.h>

/* rtk return values */
#define RTK_STATUS_SUCCESS                             0	// Successfully completed
#define RTK_STATUS_ERROR                               1	// Error  - unspecified
#define RTK_STATUS_COMMAND_FAILED                      2	// Failed - command failed
#define RTK_STATUS_COMMAND_UNKNOWN                     3	// Failed - command unknown
#define RTK_STATUS_NOT_STARTED                         4	// Failed - mode never initiated
#define RTK_STATUS_ALREADY_STARTED                     5	// Failed - mode already started
#define RTK_STATUS_SUPPLICANT_START_FAILED             6	// Failed - start up of wpa_supplicant failed
#define RTK_STATUS_PARAM_FAILED                        7	// Failed - parameter specified not valid
#define RTK_STATUS_ALREADY_CONNECTED                   8	// Failed - WiFi already connected
#define RTK_STATUS_NOT_CONNECTED                       9	// Failed - WiFi not connected
#define RTK_STATUS_SECURITY_FAILED                     10	// Failed - security setup failed
#define RTK_STATUS_NOT_ALLOWED                         11	// Failed - not allowed
#define RTK_STATUS_NOT_SUPPORTED                       12	// Failed - function not supported (maybe due to missing dependencies to filesystem)

/* Added in Join failed scenarios:*/
#define RTK_REASON_NETWORK_CONFIGURATION_NOT_FOUND 201
#define RTK_REASON_NETWORK_AUTHENTICATION_FAILED   202
#define RTK_REASON_ASSOCIATION_REQ_FAILED          203

typedef _int8_t int8_t;
typedef _uint8_t uint8_t;

typedef _int16_t int16_t;
typedef _uint16_t uint16_t;

typedef _int32_t int32_t;
typedef _uint32_t uint32_t;

typedef enum WiFi_InterFace_ID {
	RTK_WIFI_NONE,					// default
	RTK_WIFI_STATION_IF,			// Station mode (turns on wpa_supplicant)
	RTK_WIFI_SOFT_AP_IF,			// Soft AP mode (turns on hostapd)
	RTK_WIFI_P2P_IF					// P2P mode (turns on wpa_supplicant)
} WiFi_InterFace_ID_t;

typedef struct rtk_reason {
	uint32_t reason_code;				// Reason codes - 0 for success - error code see 'rtk reason codes' above
	uint8_t locally_generated;			// Which side cause link down, 1 = locally, 0 = remotely - valid for STA mode only
	int8_t ssid_len;					// length of ssid - # of valid octets
	uint8_t ssid[33];	// 802.11 spec defined up to 32 octets of data
	char bssid[17];	// BSS identification, char string e.g. xx:xx:xx:xx:xx:xx
} rtk_reason_t;

#define RTW_LWIP_LAYER 1
#define RTW_AUTO_RECONNECT 1

typedef void (*rtk_network_link_callback_t)(rtk_reason_t *reason);

typedef rtw_result_t (*rtw_scan_result_handler_t)( rtw_scan_handler_result_t* malloced_scan_result );
extern rtw_result_t app_scan_result_handler( rtw_scan_handler_result_t* malloced_scan_result );

extern int8_t WiFiRegisterLinkCallback(rtk_network_link_callback_t link_up, rtk_network_link_callback_t link_down);
extern int8_t cmd_wifi_on(WiFi_InterFace_ID_t interface_id);
extern int8_t cmd_wifi_off(void);
extern int wifi_scan_networks(rtw_scan_result_handler_t results_handler, void* user_data);
extern int8_t cmd_wifi_connect(wifi_utils_ap_config_s *ap_connect_config, void *arg);
extern int wifi_get_mac_address(char * mac);
extern int wifi_is_connected_to_ap( void );
extern int wifi_get_rssi(int *pRSSI);
extern int8_t cmd_wifi_ap(wifi_utils_softap_config_s *softap_config);
extern int wifi_set_autoreconnect(uint8_t mode);

#endif
