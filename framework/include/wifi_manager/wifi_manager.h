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
 * @file wifi_manager/wifi_manager.h
 * @brief Provides APIs for Wi-Fi Manager
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

/**
 * @brief Status of Wi-Fi interface such as connected or disconnected
 */
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
 * @brief Result types of Wi-Fi Manager APIs such as FAIL, SUCCESS, or INVALID ARGS
 */
typedef enum {
	WIFI_MANAGER_FAIL = -1,
	WIFI_MANAGER_SUCCESS,
	WIFI_MANAGER_INVALID_ARGS,
	WIFI_MANAGER_INITIALIZED,
	WIFI_MANAGER_DEINITIALIZED,
	WIFI_MANAGER_TIMEOUT,
	WIFI_MANAGER_BUSY,
	WIFI_MANAGER_ALREADY_CONNECTED,
} wifi_manager_result_e;

/**
 * @brief Mode of Wi-Fi interface such as station mode or ap mode
 */
typedef enum {
	WIFI_NONE = -1,
	STA_MODE,
	SOFTAP_MODE,
	WIFI_MODE_CHANGING,
	WIFI_INITIALIZING,
	WIFI_DEINITIALIZING,
	WIFI_FAILURE
} wifi_manager_mode_e;

/**
 * @brief Reconnection mode of Wi-Fi interface
 */
typedef enum {
	WIFI_RECONN_INTERVAL,
	WIFI_RECONN_EXPONENT,
	WIFI_RECONN_NONE,
} wifi_manager_reconn_type_e;

/**
 * @brief Result types of nearby access point scanning
 */
typedef enum {
	WIFI_SCAN_FAIL = -1,
	WIFI_SCAN_SUCCESS,
} wifi_manager_scan_result_e;

/**
 * @brief Keep information of nearby access points as scan results
 */
struct wifi_manager_scan_info_s {
	char ssid[33];	// 802.11 spec defined unspecified or uint8
	char bssid[18];	// char string e.g. xx:xx:xx:xx:xx:xx
	int8_t rssi;		// received signal strength indication
	uint8_t channel;	// channel/frequency
	uint8_t phy_mode;	// 0:legacy 1: 11N HT
	struct wifi_manager_scan_info_s *next;
};

typedef struct wifi_manager_scan_info_s wifi_manager_scan_info_s;

/**
 * @brief Include callback functions which are asynchronously called after Wi-Fi Manager APIs are called
 */
typedef struct {
	void (*sta_connected)(wifi_manager_result_e);	// in station mode, connected to ap
	void (*sta_disconnected)(void);		// in station mode, disconnected from ap
	void (*softap_sta_joined)(void);	// in softap mode, a station joined
	void (*softap_sta_left)(void);		// in softap mode, a station left
	void (*scan_ap_done)(wifi_manager_scan_info_s **, wifi_manager_scan_result_e); // scanning ap is done
} wifi_manager_cb_s;

/**
 * @brief Keep Wi-Fi Manager information including ip/mac address, ssid, rssi, etc.
 */
typedef struct {
	char ip4_address[18];
	char ssid[32];
	char mac_address[6];	   /**<  MAC address of wifi interface             */
	int rssi;
	connect_status_e status;
	wifi_manager_mode_e mode;
} wifi_manager_info_s;

/**
 * @brief Specify information of soft access point (softAP) such as ssid and channel number
 */
typedef struct {
	char ssid[32];
	uint16_t channel;
	char passphrase[64];
} wifi_manager_softap_config_s;

/**
 * @brief Specify the policy of reconnect when the device is disconnected
 */
typedef struct {
	wifi_manager_reconn_type_e type;
	// interval: if type is INTERVAL, it will try to connect AP every interval second
	//           if type is EXPONENTIAL, it is initial wait time.
	int interval;
	// max_interval: it is the maximum wait time if type is EXPONENTIAL
	//             : it is not used if type is INTERVAL
	int max_interval; // 
} wifi_manager_reconnect_config_s;

/**
 * @brief Wi-Fi authentication type such as WPA, WPA2, or WPS
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
 * @brief Wi-Fi encryption type such as WEP, AES, or TKIP
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
 * @brief Specify which access point (AP) a client connects to
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
 * @details @b #include <wifi_manager/wifi_manager.h>
 * @param[in] wmcb callback functions called when wi-fi events happen
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since TizenRT v1.1
 */
wifi_manager_result_e wifi_manager_init(wifi_manager_cb_s *wmcb);

/**
 * @brief Deinitialize Wi-Fi Manager including stoping Wi-Fi interface.
 * @details @b #include <wifi_manager/wifi_manager.h>
 * @param[in] none
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since TizenRT v1.1
 */
wifi_manager_result_e wifi_manager_deinit(void);

/**
 * @brief Change the Wi-Fi mode to station or AP.
 * @details @b #include <wifi_manager/wifi_manager.h>
 * @param[in] mode Wi-Fi mode (station or AP)
 * @param[in] config In case of AP mode, AP configuration infomation should be given including ssid, channel, and passphrase.
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since TizenRT v1.1
 */
wifi_manager_result_e wifi_manager_set_mode(wifi_manager_mode_e mode, wifi_manager_softap_config_s *config);

/**
 * @brief Retrieve current status of Wi-Fi interface including mode, connection status, ssid, received signal strengh indication, and ip address.
 * @details @b #include <wifi_manager/wifi_manager.h>
 * @param[out] retrieved information including  mode, connection status, ssid, received signal strengh indication, and ip address.
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since TizenRT v1.1
 */
wifi_manager_result_e wifi_manager_get_info(wifi_manager_info_s *info);

/**
 * @brief Connect to an access point.
 * @details @b #include <wifi_manager/wifi_manager.h>
 * @param[in] config ssid, passphrase, authentication type, and cryto type of the access point which the wi-fi interface connect to.
 * @param[in] reconn_config reconnect type, interval, minimum or maximun intervalis set
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since TizenRT v1.1
 */
wifi_manager_result_e wifi_manager_connect_ap_config(wifi_manager_ap_config_s *config, wifi_manager_reconnect_config_s *reconn_config);

/**
 * @brief Connect to an access point.
 * @details @b #include <wifi_manager/wifi_manager.h>
 * @param[in] config ssid, passphrase, authentication type, and cryto type of the access point which the wi-fi interface connect to.
 * @param[in] conn_config reconnect type, interval, minimum or maximun intervalis set 
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since TizenRT v1.1
 */
wifi_manager_result_e wifi_manager_connect_ap(wifi_manager_ap_config_s *config);


/**
 * @brief Disconnect from the connected access point
 * @details @b #include <wifi_manager/wifi_manager.h>
 * @param[in] none
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since TizenRT v1.1
 */
wifi_manager_result_e wifi_manager_disconnect_ap(void);

/**
 * @brief Scan nearby access points
 * @details @b #include <wifi_manager/wifi_manager.h>
 * @param[in] none
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since TizenRT v1.1
 */
wifi_manager_result_e wifi_manager_scan_ap(void);

/**
 * @brief Save the AP configuration at persistent storage
 * @details @b #include <wifi_manager/wifi_manager.h>
 * @param[in] config AP configuration infomation should be given including ssid, channel, and passphrase.
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since TizenRT v1.x
 */
wifi_manager_result_e wifi_manager_save_config(wifi_manager_ap_config_s *config);

/**
 * @brief Get the AP configuration which was saved
 * @details @b #include <wifi_manager/wifi_manager.h>
 * @param[in] config The pointer of AP configuration infomation which will be filled
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since TizenRT v1.x
 */

wifi_manager_result_e wifi_manager_get_config(wifi_manager_ap_config_s *config);

/**
 * @brief Remove the AP configuration which was saved
 * @details @b #include <wifi_manager/wifi_manager.h>
 * @return On success, WIFI_MANAGER_SUCCESS (i.e., 0) is returned. On failure, non-zero value is returned.
 * @since TizenRT v1.x
 */
wifi_manager_result_e wifi_manager_remove_config(void);

#endif
/**
 *@}
 */
