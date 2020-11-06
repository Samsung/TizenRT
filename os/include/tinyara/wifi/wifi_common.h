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

#ifndef __WIFI_COMMON_H__
#define __WIFI_COMMON_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <stdint.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define WIFI_UTILS_MACADDR_LEN        6
#define WIFI_UTILS_MACADDR_STR_LEN    17
#define WIFI_UTILS_SSID_LEN           32
#define WIFI_UTILS_PASSPHRASE_LEN     64

/****************************************************************************
 * Enums
 ****************************************************************************/
/**
 * @brief time out option (used by message queue, uart, semaphore, mutex)
 *
 */
typedef enum {
	WIFI_UTILS_NO_WAIT = 0,      /**<  no wait contant          */
	WIFI_UTILS_FOREVER = -1,     /**<  wait until job finished  */
} wifi_utils_timeout_option;

/**
 * @brief <b> wifi result type FAIL, SUCCESS, INVALID ARGS</b>
 */
typedef enum {
	WIFI_UTILS_FAIL = -1,
	WIFI_UTILS_SUCCESS,
	WIFI_UTILS_INVALID_ARGS,
	WIFI_UTILS_TIMEOUT,
	WIFI_UTILS_BUSY,
	WIFI_UTILS_FILE_ERROR,
	WIFI_UTILS_ALREADY_CONNECTED,
} wifi_utils_result_e;

/**
  * @brief <b> wifi MAC/PHY standard types
  */
typedef enum {
	WIFI_UTILS_IEEE_80211_LEGACY,             /**<  IEEE 802.11a/g/b          */
	WIFI_UTILS_IEEE_80211_A,                  /**<  IEEE 802.11a              */
	WIFI_UTILS_IEEE_80211_B,                  /**<  IEEE 802.11b              */
	WIFI_UTILS_IEEE_80211_G,                  /**<  IEEE 802.11g              */
	WIFI_UTILS_IEEE_80211_N,                  /**<  IEEE 802.11n              */
	WIFI_UTILS_IEEE_80211_AC,                 /**<  IEEE 802.11ac             */
	WIFI_UTILS_NOT_SUPPORTED,                 /**<  Driver does not report    */
} wifi_utils_standard_type_e;

/**
 * @brief <b> wifi authentication type WPA, WPA2, WPS</b>
 */
typedef enum {
	WIFI_UTILS_AUTH_OPEN,                    /**<  open mode                                 */
	WIFI_UTILS_AUTH_WEP_SHARED,              /**<  use shared key (wep key)                  */
	WIFI_UTILS_AUTH_WPA_PSK,                 /**<  WPA_PSK mode                              */
	WIFI_UTILS_AUTH_WPA2_PSK,                /**<  WPA2_PSK mode                             */
	WIFI_UTILS_AUTH_WPA3_PSK,                /**<  WPA3_PSK mode                             */
	WIFI_UTILS_AUTH_WPA_AND_WPA2_PSK,        /**<  WPA_PSK and WPA_PSK mixed mode            */
	WIFI_UTILS_AUTH_WPA_PSK_ENT,             /**<  Enterprise WPA_PSK mode                   */
	WIFI_UTILS_AUTH_WPA2_PSK_ENT,            /**<  Enterprise WPA2_PSK mode                  */
	WIFI_UTILS_AUTH_WPA_AND_WPA2_PSK_ENT,    /**<  Enterprise WPA_PSK and WPA_PSK mixed mode */
	WIFI_UTILS_AUTH_IBSS_OPEN,               /**<  IBSS ad-hoc mode                          */
	WIFI_UTILS_AUTH_WPS,                     /**<  WPS mode                                  */
	WIFI_UTILS_AUTH_UNKNOWN,                 /**<  unknown type                              */
} wifi_utils_ap_auth_type_e;

/**
 * @brief wifi encryption type WEP, AES, TKIP
 */
typedef enum {
	WIFI_UTILS_CRYPTO_NONE,					 /**<  none encryption                           */
	WIFI_UTILS_CRYPTO_WEP_64,				 /**<  WEP encryption wep-40                     */
	WIFI_UTILS_CRYPTO_WEP_128,				 /**<  WEP encryption wep-104                    */
	WIFI_UTILS_CRYPTO_AES,					 /**<  AES encryption                            */
	WIFI_UTILS_CRYPTO_TKIP,					 /**<  TKIP encryption                           */
	WIFI_UTILS_CRYPTO_TKIP_AND_AES,			 /**<  TKIP and AES mixed encryption             */
	WIFI_UTILS_CRYPTO_AES_ENT,				 /**<  Enterprise AES encryption                 */
	WIFI_UTILS_CRYPTO_TKIP_ENT,				 /**<  Enterprise TKIP encryption                */
	WIFI_UTILS_CRYPTO_TKIP_AND_AES_ENT,		 /**<  Enterprise TKIP and AES mixed encryption  */
	WIFI_UTILS_CRYPTO_UNKNOWN,				 /**<  unknown encryption                        */
} wifi_utils_ap_crypto_type_e;

/**
 * @brief wifi status (connected, dis_connected, soft_ap)
 */
typedef enum {
	WIFI_UTILS_DISCONNECTED,                 /**<  wifi is disconnected  */
	WIFI_UTILS_CONNECTED,                    /**<  connected             */
	WIFI_UTILS_SOFTAP_MODE,                  /**<  soft ap mode          */
} wifi_utils_status_e;


/****************************************************************************
 * Structures
 ****************************************************************************/
/**
 * @brief wifi access point information
 */
struct wifi_utils_ap_scan_info {
	unsigned int channel;                                    /**<  Radio channel that the AP beacon was received on       */
	char ssid[WIFI_UTILS_SSID_LEN + 1];                      /**<  Service Set Identification (i.e. Name of Access Point) */
	unsigned int ssid_length;                                /**<  The length of Service Set Identification               */
	unsigned char bssid[WIFI_UTILS_MACADDR_STR_LEN + 1];     /**<  MAC address (xx:xx:xx:xx:xx:xx) of Access Point        */
	unsigned int max_rate;                                   /**<  Maximum data rate in kilobits/s                        */
	int rssi;                                                /**<  Receive Signal Strength Indication in dBm              */
	wifi_utils_standard_type_e phy_mode;                     /**< Supported MAC/PHY standard                              */
	wifi_utils_ap_auth_type_e ap_auth_type;                  /**<  @ref wifi_utils_ap_auth_type                           */
	wifi_utils_ap_crypto_type_e ap_crypto_type;              /**<  @ref wifi_utils_ap_crypto_type                         */
};
typedef struct wifi_utils_ap_scan_info wifi_utils_ap_scan_info_s;


/**
 * @brief wifi ap connect config
 */
struct wifi_utils_ap_config {
	char ssid[WIFI_UTILS_SSID_LEN + 1];                      /**<  Service Set Identification              */
	unsigned int ssid_length;                                /**<  Service Set Identification Length       */
	char passphrase[WIFI_UTILS_PASSPHRASE_LEN + 1];          /**<  ap passphrase(password)                 */
	unsigned int passphrase_length;                          /**<  ap passphrase length                    */
	wifi_utils_ap_auth_type_e ap_auth_type;                  /**<  @ref wifi_utils_ap_auth_type            */
	wifi_utils_ap_crypto_type_e ap_crypto_type;              /**<  @ref wifi_utils_ap_crypto_type          */
};
typedef struct wifi_utils_ap_config wifi_utils_ap_config_s;

/**
 * @brief soft ap mode config
 */
struct wifi_utils_softap_config {
	unsigned int channel;                                    /**<  soft ap wifi channel                                    */
	char ssid[WIFI_UTILS_SSID_LEN + 1];                      /**<  Service Set Identification                              */
	unsigned int ssid_length;                                /**<  Service Set Identification Length                       */
	char passphrase[WIFI_UTILS_PASSPHRASE_LEN + 1];          /**<  ap passphrase(password)                                 */
	unsigned int passphrase_length;                          /**<  ap passphrase length                                    */
	wifi_utils_ap_auth_type_e ap_auth_type;                  /**<  @ref wifi_utils_ap_auth_type                            */
	wifi_utils_ap_crypto_type_e ap_crypto_type;              /**<  @ref wifi_utils_ap_crypto_type                          */
	void (*inform_new_sta_join)(void);                       /**< @ref inform application about new station joining softAP */
};
typedef struct wifi_utils_softap_config wifi_utils_softap_config_s;

struct wifi_utils_scan_list {
	wifi_utils_ap_scan_info_s ap_info;
	struct wifi_utils_scan_list *next;
};
typedef struct wifi_utils_scan_list wifi_utils_scan_list_s;

typedef void (*wifi_utils_sta_connected)(wifi_utils_result_e res, void *arg);
typedef void (*wifi_utils_sta_disconnected)(void *arg);
typedef void (*wifi_utils_softap_sta_joined)(void *arg);
typedef void (*wifi_utils_softap_sta_left)(void *arg);
typedef void (*wifi_utils_scan_done)(wifi_utils_result_e res, wifi_utils_scan_list_s *slist, void *arg);

struct wifi_utils_cb {
	wifi_utils_sta_connected sta_connected;
	wifi_utils_sta_disconnected sta_disconnected;
	wifi_utils_softap_sta_joined softap_sta_joined;
	wifi_utils_softap_sta_left softap_sta_left;
	wifi_utils_scan_done scan_done;
};
typedef struct wifi_utils_cb wifi_utils_cb_s;

/**
 * @brief wifi information (ip address, mac address)
 */
struct wifi_utils_info {
	uint32_t ip4_address;                                 /**<  ip4 address                               */
	unsigned char mac_address[WIFI_UTILS_MACADDR_LEN];    /**<  MAC address of wifi interface             */
	int rssi;                                             /**<  Receive Signal Strength Indication in dBm */
	wifi_utils_status_e wifi_status;                      /**<  @ref wifi_utils_status                    */
};
typedef struct wifi_utils_info wifi_utils_info_s;

#endif             /* __WIFI_COMMON_H__ */
