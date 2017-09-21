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

/**
 * @defgroup Wi-Fi_Manager Wi-Fi_Manager
 * @ingroup Wi-Fi_Manager
 * @brief Provides APIs for Wi-Fi Manager
 * @{
 */

/**
 * @file wifi_manager.h
 * @brief Provides APIs for Wi-Fi Manager
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

typedef enum {
	// STA mode status
	AP_DISCONNECTED,
	AP_CONNECTED,

	// SOFT AP mode status
	CLIENT_CONNECTED,
	CLIENT_DISCONNECTED,

	// Unkown
	STATUS_UNKNOWN
} connect_status_e;

/**
 * @brief <b> wifi result type FAIL, SUCCESS, INVALID ARGS</b>
 */
typedef enum {
	WIFI_MANAGER_FAIL = -1,
	WIFI_MANAGER_SUCCESS,
	WIFI_MANAGER_INVALID_ARGS,
	WIFI_MANAGER_INITIALIZED,
	WIFI_MANAGER_DEINITIALIZED,
	WIFI_MANAGER_TIMEOUT,
	WIFI_MANAGER_BUSY,
} wifi_manager_result_e;

typedef enum {
	WIFI_NONE = -1,
	STA_MODE,
	SOFTAP_MODE
} wifi_manager_mode_e;

typedef enum {
	WIFI_SCAN_FAIL = -1,
	WIFI_SCAN_SUCCESS,
} wifi_manager_scan_result_e;

struct wifi_manager_scan_info_s {
	char ssid[33];	// 802.11 spec defined unspecified or uint8
	char bssid[18];	// char string e.g. xx:xx:xx:xx:xx:xx
	int8_t rssi;		// received signal strength indication
	uint8_t channel;	// channel/frequency
	uint8_t phy_mode;	// 0:legacy 1: 11N HT
	struct wifi_manager_scan_info_s *next;
};

typedef struct wifi_manager_scan_info_s wifi_manager_scan_info_s;

typedef struct {
	void (*sta_connected)(void);		// in station mode, connected to ap
	void (*sta_disconnected)(void);		// in station mode, disconnected from ap
	void (*softap_sta_joined)(void);	// in softap mode, a station joined
	void (*softap_sta_left)(void);		// in softap mode, a station left
	void (*scan_ap_done)(wifi_manager_scan_info_s **, wifi_manager_scan_result_e); // scanning ap is done
} wifi_manager_cb_s;

typedef struct {
	char ip4_address[18];
	char ssid[32];
	char mac_address[6];	   /**<  MAC address of wifi interface             */
	int rssi;
	connect_status_e status;
	wifi_manager_mode_e mode;
} wifi_manager_info_s;

typedef struct {
	char ssid[32];
	uint16_t channel;
	char passphrase[64];
} wifi_manager_softap_config_s;

/**
 * @brief <b> wifi authentication type WPA, WPA2, WPS</b>
 */
typedef enum {
	WIFI_MANAGER_AUTH_OPEN,					   /**<  open mode                      */
	WIFI_MANAGER_AUTH_WEP_SHARED,			   /**<  use shared key (wep key)       */
	WIFI_MANAGER_AUTH_WPA_PSK,				   /**<  WPA_PSK mode                   */
	WIFI_MANAGER_AUTH_WPA2_PSK,				   /**<  WPA2_PSK mode                  */
	WIFI_MANAGER_AUTH_WPA_AND_WPA2_PSK,		   /**<  WPA_PSK and WPA_PSK mixed mode */
	WIFI_MANAGER_AUTH_UNKNOWN,				   /**<  unknown type                   */
} wifi_manager_ap_auth_type_e;

/**
 * @brief wifi encryption type WEP, AES, TKIP
 */
typedef enum {
	WIFI_MANAGER_CRYPTO_NONE,				   /**<  none encryption                */
	WIFI_MANAGER_CRYPTO_WEP_64,				   /**<  WEP encryption wep-40          */
	WIFI_MANAGER_CRYPTO_WEP_128,			   /**<  WEP encryption wep-104         */
	WIFI_MANAGER_CRYPTO_AES,				   /**<  AES encryption                 */
	WIFI_MANAGER_CRYPTO_TKIP,				   /**<  TKIP encryption                */
	WIFI_MANAGER_CRYPTO_TKIP_AND_AES,		   /**<  TKIP and AES mixed encryption  */
	WIFI_MANAGER_CRYPTO_UNKNOWN,			   /**<  unknown encryption             */
} wifi_manager_ap_crypto_type_e;

/**
 * @brief wifi ap connect config
 */
typedef struct {
	char ssid[32];							 /**<  Service Set Identification         */
	unsigned int ssid_length;				 /**<  Service Set Identification Length  */
	char passphrase[64];					 /**<  ap passphrase(password)            */
	unsigned int passphrase_length;			 /**<  ap passphrase length               */
	wifi_manager_ap_auth_type_e ap_auth_type;	  /**<  @ref wifi_utils_ap_auth_type   */
	wifi_manager_ap_crypto_type_e ap_crypto_type;  /**<  @ref wifi_utils_ap_crypto_type */
} wifi_manager_ap_config_s;

/**
 * @brief Initialize Wi-Fi Manager including starting Wi-Fi interface.
 * @param[in] callback functions called when wi-fi events happen
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since Tizen RT v1.1
 */
wifi_manager_result_e wifi_manager_init(wifi_manager_cb_s *wmcb);

/**
 * @brief Deinitialize Wi-Fi Manager including stoping Wi-Fi interface.
 * @param[in] none
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since Tizen RT v1.1
 */
wifi_manager_result_e wifi_manager_deinit(void);

/**
 * @brief Change the Wi-Fi mode to station or AP.
 * @param[in] Wi-Fi mode (station or AP)
 * @param[in] In case of AP mode, AP configuration infomation should be given including ssid, channel, and passphrase.
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since Tizen RT v1.1
 */
wifi_manager_result_e wifi_manager_set_mode(wifi_manager_mode_e mode, wifi_manager_softap_config_s *config);

/**
 * @brief Retrieve current status of Wi-Fi interface including mode, connection status, ssid, received signal strengh indication, and ip address.
 * @param[out] retrieved information including  mode, connection status, ssid, received signal strengh indication, and ip address.
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since Tizen RT v1.1
 */
wifi_manager_result_e wifi_manager_get_info(wifi_manager_info_s *info);

/**
 * @brief Connect to an access point.
 * @param[in] ssid, passphrase, authentication type, and cryto type of the access point which the wi-fi interface connect to.
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since Tizen RT v1.1
 */
wifi_manager_result_e wifi_manager_connect_ap(wifi_manager_ap_config_s *config);

/**
 * @brief Disconnect from the connected access point
 * @param[in] none
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since Tizen RT v1.1
 */
wifi_manager_result_e wifi_manager_disconnect_ap(void);

/**
 * @brief Scan nearby access points
 * @param[in] none
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since Tizen RT v1.1
 */
wifi_manager_result_e wifi_manager_scan_ap(void);

#endif

/**
 *@}
 */
