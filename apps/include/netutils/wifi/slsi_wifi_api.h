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
* @defgroup Wi-Fi Wi-Fi
* @brief Introduce Wi-Fi APIs for TizenRT.
* @details This is the Samsung LSI Wi-Fi API for TizenRT OS.
* It contains support for both station and soft-ap mode on the 2.4Ghz frequency.
* The API has support for security modes WPA/WPA2/WEP with various ciphers.
*
* @ingroup NETWORK
* @{
*
*/

/**
 * @file slsi_wifi_api.h
 * @brief Provides APIs for SLSI Wi-Fi APIs
 */

/**
 * @cond HIDDEN
 */
#ifndef SLSI_WIFI_API_H_
#define SLSI_WIFI_API_H_
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/* interface name to use */
#define CTRL_IFNAME             "wl1"
#define CTRL_P2P_IFNAME         "wl2"
#define CTRL_P2P_GROUP_IFNAME   "wl3"
#ifndef BIT
#define BIT(x) (1 << (x))
#endif

typedef enum {
	SLSI_SEC_MODE_OPEN			= 0,				// 00000000
	SLSI_SEC_MODE_WEP			= BIT(1),			// 00000010
	SLSI_SEC_MODE_WEP_SHARED	= BIT(2),			// 00000100
	SLSI_SEC_MODE_WPA_TKIP		= BIT(3),			// 00001000
	SLSI_SEC_MODE_WPA_CCMP		= BIT(4),			// 00010000
	SLSI_SEC_MODE_WPA_MIXED		= (BIT(3) | BIT(4)),// 00011000
	SLSI_SEC_MODE_WPA2_TKIP		= BIT(5),			// 00100000
	SLSI_SEC_MODE_WPA2_CCMP		= BIT(6),			// 01000000
	SLSI_SEC_MODE_WPA2_MIXED	= (BIT(5) | BIT(6)),// 01100000
	SLSI_SEC_MODE_EAP			= BIT(7)			// 10000000
} slsi_security_mode_t;

/* Length defines */
#define SLSI_MACADDR_LEN        6
#define SLSI_MACADDR_STR_LEN    18
/* The maximum length of an SSID - excluding '\0' is case of all characters used */
#define SLSI_SSID_LEN           32
#define SLSI_PASSPHRASE_LEN     64

/* SLSI return values */
#define SLSI_STATUS_SUCCESS                             0	// Successfully completed
#define SLSI_STATUS_ERROR                               1	// Error  - unspecified
#define SLSI_STATUS_COMMAND_FAILED                      2	// Failed - command failed
#define SLSI_STATUS_COMMAND_UNKNOWN                     3	// Failed - command unknown
#define SLSI_STATUS_NOT_STARTED                         4	// Failed - mode never initiated
#define SLSI_STATUS_ALREADY_STARTED                     5	// Failed - mode already started
#define SLSI_STATUS_SUPPLICANT_START_FAILED             6	// Failed - start up of wpa_supplicant failed
#define SLSI_STATUS_PARAM_FAILED                        7	// Failed - parameter specified not valid
#define SLSI_STATUS_ALREADY_CONNECTED                   8	// Failed - WiFi already connected
#define SLSI_STATUS_NOT_CONNECTED                       9	// Failed - WiFi not connected
#define SLSI_STATUS_SECURITY_FAILED                     10	// Failed - security setup failed
#define SLSI_STATUS_NOT_ALLOWED                         11	// Failed - not allowed
#define SLSI_STATUS_NOT_SUPPORTED                       12	// Failed - function not supported (maybe due to missing dependencies to filesystem)

/* SLSI reason codes */
/* Return values used from ieee802_11_defs.h
 * (Reason codes (IEEE 802.11-2007, 7.3.1.7, Table 7-22)
 * SLSI Wi-Fi own defined reason codes are starting from 200
 */
/* Added in Join failed scenarios:*/
#define SLSI_REASON_NETWORK_CONFIGURATION_NOT_FOUND 201
#define SLSI_REASON_NETWORK_AUTHENTICATION_FAILED   202
#define SLSI_REASON_ASSOCIATION_REQ_FAILED          203
#ifdef CONFIG_SLSI_WIFI_P2P_API
/* Added in p2p connect scenarios:*/
#define SLSI_REASON_PROV_DISC_FAILED                204
#define SLSI_REASON_GO_NEG_FAILED                   205
#define SLSI_REASON_GROUP_FORMATION_FAILED          206
#define SLSI_REASON_WPS_EVENT_FAILED                207
#define SLSI_REASON_CONNECT_ATTEMPT_FAILED          208
#define SLSI_REASON_TIMEOUT                         209
#define SLSI_REASON_OPERATION_FAILED                210
#endif
/**
 * Specify interval between scans performed by lower layers.
 * Value is in seconds. Default is 30 seconds,
 * but during connection it is lowered to 10 seconds.
 * Minimum value is 10 and maximum value is 60
 */
#define SLSI_SCAN_INTERVAL          30
#define SLSI_SCAN_INTERVAL_CONNECT  10

/**
 * Specify expiration of scans results in lower layers
 * Value is in seconds (default is 120 seconds)
 */
#define SLSI_BSS_EXPIRE_AGE         120

/**
 * Specify whether lower Wi-Fi layers should handle automatic reconnect
 * in station mode.
 * Values: enabled - 1, disabled - 0 (default is enabled 1)
 */
#define SLSI_SAVE_CONFIG            1

/**
 * Specify whether API layers should wait for multiple scan for network
 * attempts if device not found first time in station mode. Raising the
 * count will hold the link_up response for a longer period
 * Values: 3 is default
 */
#define SLSI_STA_JOIN_SCAN_ATTEMPT  3
/**
 * @endcond
 */

/** @brief Defines slsi wifi network interface api parameters. */
typedef enum WiFi_InterFace_ID {
	SLSI_WIFI_NONE,         /**< Unknown mode (error case) */
	SLSI_WIFI_STATION_IF,	/**< Station mode (turns on wpa_supplicant) */
	SLSI_WIFI_SOFT_AP_IF,	/**< Soft AP mode (turns on hostapd) */
#ifdef CONFIG_SLSI_WIFI_P2P_API
	SLSI_WIFI_P2P_IF		/**< P2P mode (turns on wpa_supplicant */
#endif
} WiFi_InterFace_ID_t;

#ifdef CONFIG_SLSI_WIFI_P2P_API
/* Defines for P2P interface */

#define SLSI_P2P_INTENT_CLI 1
#define SLSI_P2P_INTENT_ANY 6
#define SLSI_P2P_INTENT_GO 15

#define SLSI_P2P_MAX_DEVICE_NAME_LEN (SLSI_SSID_LEN - 10)  /* 10 = DIRECT-xy- */

typedef enum {
    SLSI_P2P_CONN_ACCEPT      = 0,
    SLSI_P2P_CONN_REJECT      = 1
} slsi_p2p_connect_response_t;

/* enum containing available configuration methods for p2p */
typedef enum config_method {
    SLSI_P2P_PBC     = 0x01,
    SLSI_P2P_DISPLAY = 0x02,
    SLSI_P2P_KEYPAD  = 0x04,
    SLSI_P2P_P2PS    = 0x08 // Not supported, but found devices could support this
} slsi_config_method_t;

/* struct containing peer information about p2p devices
 * connected to us
 */
typedef struct peer_info {
    uint8_t              device_name[SLSI_SSID_LEN + 1];
    char                 device_address[SLSI_MACADDR_STR_LEN];
    slsi_config_method_t config_methods;
    uint8_t              primary_device_type[8];
    uint8_t              secondary_device_type[8];
    uint8_t              device_capability;
    uint8_t              group_capability;
} slsi_peer_info_t;

typedef struct slsi_find_info {
    uint32_t reason_code;
    slsi_peer_info_t peer_info;
} slsi_find_info_t;

#endif //CONFIG_SLSI_WIFI_P2P_API
/**
 *@cond HIDDEN
 */

/* Capabilities bit mask
 * Return values defined in ieee802_11_defs.h
 #define HT_CAP_INFO_LDPC_CODING_CAP        ((u16) BIT(0))
 #define HT_CAP_INFO_SUPP_CHANNEL_WIDTH_SET ((u16) BIT(1))
 #define HT_CAP_INFO_SMPS_MASK              ((u16) (BIT(2) | BIT(3)))
 #define HT_CAP_INFO_SMPS_STATIC            ((u16) 0)
 #define HT_CAP_INFO_SMPS_DYNAMIC           ((u16) BIT(2))
 #define HT_CAP_INFO_SMPS_DISABLED          ((u16) (BIT(2) | BIT(3)))
 #define HT_CAP_INFO_GREEN_FIELD            ((u16) BIT(4))
 #define HT_CAP_INFO_SHORT_GI20MHZ          ((u16) BIT(5))
 #define HT_CAP_INFO_SHORT_GI40MHZ          ((u16) BIT(6))
 #define HT_CAP_INFO_TX_STBC                ((u16) BIT(7))
 #define HT_CAP_INFO_RX_STBC_MASK           ((u16) (BIT(8) | BIT(9)))
 #define HT_CAP_INFO_RX_STBC_1              ((u16) BIT(8))
 #define HT_CAP_INFO_RX_STBC_12             ((u16) BIT(9))
 #define HT_CAP_INFO_RX_STBC_123            ((u16) (BIT(8) | BIT(9)))
 #define HT_CAP_INFO_DELAYED_BA             ((u16) BIT(10))
 #define HT_CAP_INFO_MAX_AMSDU_SIZE         ((u16) BIT(11))
 #define HT_CAP_INFO_DSSS_CCK40MHZ          ((u16) BIT(12))
 // B13 - Reserved (was PSMP support during P802.11n development)
 #define HT_CAP_INFO_40MHZ_INTOLERANT       ((u16) BIT(14))
 #define HT_CAP_INFO_LSIG_TXOP_PROTECT_SUPPORT    ((u16) BIT(15))*/

/**
 * @endcond
 */

/** @brief The typedef structure to support 802.11 HT (High-throughput) mode */
typedef struct slsi_ht_config {
	uint16_t ht_capab_info;		/**< Supported fields: HT_CAP_INFO_GREEN_FIELD, HT_CAP_INFO_SHORT_GI20MHZ */
	uint8_t mcs_index[10];		/**< 0 uses default, supported HT-MCS rates - in ASCII hex: 0xffff0000000000000000 */
} slsi_ht_config_t;

/** @brief The typedef structure storing 802.11 security mode and passpharase */
typedef struct slsi_security_config {
	uint32_t secmode;							/**< 802.11 security authentication mode */
	char passphrase[SLSI_PASSPHRASE_LEN];		/**< Pre-shared key for authentication, (maximum length: 64) */
} slsi_security_config_t;

/** @brief The typedef structure for vendor specific IE (Information Element) block */
typedef struct slsi_vendor_ie {
	uint8_t content_length;				/**< Total size of the allocated buffer */
	uint8_t oui[3];						/**< Organizational Unique ID */
	uint8_t *content;					/**< The ponter to the data allocated for the Vendor IE block */
	struct slsi_vendor_ie *next;		/**<  pointer to next element if used as a list */
} slsi_vendor_ie_t;

/** @brief The typedef structure of 802.11 network information */
typedef struct slsi_ap_config {
	uint8_t ssid[SLSI_SSID_LEN + 1];	/**< Name of AP, 802.11 spec. defined unspecified or uint8 */
	int8_t ssid_len;					/**< The length of ssid - # of valid octets */
	uint32_t beacon_period;				/**< Period of beacon frame, default 100 TU */
	uint32_t DTIM;						/**< Delivery Traffic Information Message (range 1..255), default 2 */
	uint8_t channel;					/**< Information of channel/frequency */
	uint8_t phy_mode;					/**< HT mode supporting, 0: legacy 1: 11N HT */
	slsi_ht_config_t ht_mode;			/**< CONFIG_HT_OVERRIDES required, @ref slsi_ht_config_t */
	slsi_security_config_t *security;	/**< @ref slsi_security_config_t (use NULL if security mode is open) */
	slsi_vendor_ie_t *vsie;				/**< @ref slsi_vendor_ie_t */
} slsi_ap_config_t;

/** @brief The typedef structure to be used for storing scan results from Wi-Fi driver */
typedef struct slsi_scan_info {
	uint8_t ssid[SLSI_SSID_LEN + 1];	/**< Name of AP, 802.11 spec. defined unspecified or uint8 */
	char bssid[SLSI_MACADDR_STR_LEN];	/**< BSS identification, char string e.g. xx:xx:xx:xx:xx:xx */
	int8_t ssid_len;					/**< The length of ssid - # of valid octets */
	int8_t rssi;						/**< RSSI level of scanned device */
	uint32_t beacon_period;				/**< Beacon period used (default 100) */
	uint8_t channel;					/**< Information of channel/frequency */
	uint8_t phy_mode;					/**< 802.11 PHY mode, 0: legacy 1: 11N HT */
	uint8_t bss_type;					/**< Type of BSS, 0: infrastructure, 1: independent */
	uint8_t wps_support;				/**< Support of WPS, boolean 1 supported, 0 not supported */
	uint8_t num_sec_modes;				/**< The number of elements of security modes in sec_mode */
	slsi_ht_config_t ht_mode;			/**< @ref slsi_ht_config_t */
	slsi_security_config_t *sec_modes;	/**< @ref slsi_security_config_t, The list of security modes */
	struct slsi_scan_info *next;        /**< Pointer to next scan results */
    slsi_vendor_ie_t *vsie;             /**< Vendor Specific IEs from an AP */
} slsi_scan_info_t;


/** @brief The typedef structure to be used to sharing reason code */
typedef struct slsi_reason {
	uint32_t reason_code;				/**< Reason codes - 0 for success, others @see apps/wpa_supplicant/src/common/ieee802_11_defs.h*/
	uint8_t locally_generated;			/**< Which side cause link down, 1 = locally, 0 = remotely - valid for STA mode only */
	int8_t ssid_len;					/**< The length of ssid - # of valid octets */
	uint8_t ssid[SLSI_SSID_LEN + 1];	/**< Name of AP, 802.11 spec defined up to 32 octets of data */
	char bssid[SLSI_MACADDR_STR_LEN];	/**< BSS identification, char string e.g. xx:xx:xx:xx:xx:xx */
} slsi_reason_t;


/** @brief The callback function to handle event of the scan results from wpa_supplicant */
typedef int8_t (*slsi_scan_result_callback_t)(slsi_reason_t* reason);

/** @brief The callback function to handle link event from wpa_supplicant */
typedef void (*slsi_network_link_callback_t)(slsi_reason_t *reason);

/**
 *@cond HIDDEN
 */
typedef struct te_func_ {
    char* (*WiFiWpaSendRequest)(char *,char*,int8_t*);
    int8_t (*WiFiGetBssid)(char**);
    int8_t (*WiFiSetSecurity)(const slsi_security_config_t *, const char *);
    int8_t (*WiFiGetNetwork)(uint8_t* , uint8_t , char** );
} te_func_t;

/**
 * @endcond
 */
/**
 * @brief Set the network configuration and start Wi-Fi interface.
 * @param [in] interface_id    Interface id to use (STATION_IF = 0 or SOFT_AP_IF = 1)
 * @param [in] ap_config       Network configurations needed if SOFT_AP_IF is selected as
 *                  WiFi_InterFace_ID_t else NULL. The caller owns this structure.
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 * @details Set the network configuration and start Wi-Fi interface in either AP mode or
 * Station Mode. Will start either Hostapd or wpa_supplicant process depending on the mode
 * selected.
 * A WiFiStart implies the init of the Wi-Fi stack + driver + IP stack if
 * not already running.
 * The hostapd will activate the AP automatically when started.
 * In case the application invokes WiFiStart multiple times (i.e. with
 * STATION_IF in the first and SOFTAP_IF in  the  second) the  sequential
 * requests will  be  met  with  an error response.
 * This is because the Wi-Fi sub system cannot run in both station and SoftAP
 * mode at the same time.
 * This function will use the chosen channel to select the proper mode for
 * phy_mode=legacy using the following rules:
 * -     If channel=0 the Automatic Channel Selection(ACS) in hostapd will be
 *         enabled and the hardware mode will be set to 11G.
 * -    If channel=[1-13] the channel will be set as selected and the hardware
 *         mode will be set to 11G
 * -    If channel=14 then the channel will be set as select and the hardware
 *         mode will be set to 11B (only allowed in Japan).
 * -    If channel=[36-161] the channel will be set as selected and hardware
 *         mode will be set to 11A (unless HT_mode specified then 11N)
 */
int8_t WiFiStart(WiFi_InterFace_ID_t interface_id, const slsi_ap_config_t *ap_config);

/**
 * @brief Stop the Wi-Fi interface.
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 *
 * @details Stop the interface that currently associated AP, or abort the current
 * connection process. A WiFiStop implies a stop of the Wi-Fi stack,
 * driver and IP stack if nothing else needs it.
 * If the selected interface is not running, the function will return success.
 */
int8_t WiFiStop(void);

/**
 * @brief Register callback functions for WiFi link.
 * @param [in] link_up      Callback function to register for the link up event
 * @param [in] link_down    Callback function to register for the link down event
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 *
 * @details Register callback functions that gets called when a change in network link
 * status occurs, these carry a slsi_reason_t to tell why link is up or down
 */
int8_t WiFiRegisterLinkCallback(slsi_network_link_callback_t link_up, slsi_network_link_callback_t link_down);

/**
 * @brief Register callback functions for WiFi scan.
 * @param [in] scan_result_handler     Callback function to register for the link up event
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 *
 * @details Register callback functions that gets called when a scan trigger gets
 * response for its request has done.
 */
int8_t WiFiRegisterScanCallback(slsi_scan_result_callback_t scan_result_handler);

/**
 * @brief Scan for Wi-Fi network.
 *
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 *
 * @details Start a scan for Wi-Fi AP in the surroundings.
 * result is returned to the callback handler registered with
 * WiFiRegisterScanCallback(). Events are : Scan complete event
 * or Scan aborted.
 * The scan results are retrieved using WiFiGetScanResults().
 */
int8_t WiFiScanNetwork(void);

/**
 * Scan for specific Wi-Fi network
 * @param ssid              SSID of the AP that is to be search for (can be hidden SSID)
 * @param ssid_len          length in bytes for the SSID
 * @param security_config   the security config to use with the specific SSID
 * @return: Scan initiated successfully or failed
 *
 * Start a scan for specific Wi-Fi AP in the surroundings.
 * result is returned to the callback handler registered with
 * WiFiRegisterScanCallback(). Events are : Scan complete event
 * or Scan aborted.
 * The scan results are retrieved using WiFiGetScanResults().
 */
int8_t WiFiScanSpecificNetwork(uint8_t* ssid, uint8_t ssid_len,
        const slsi_security_config_t *security_config);
/**
 * @brief Return current scan results list.
 * @param [out] scan_results        A pointer to a linked list of scan results
 * - The caller takes owner responsibility of this list, and must free each item on the list after use.
 * - For convince the function WiFiFreeScanResults() can be used to free the list.
 *
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 *
 * @details Request available list of scan results for Wi-Fi AP in the surroundings. The
 * results are returned as the list of structure slsi_ap_info_t linked via next
 * pointer.
 * The list must be freed by the caller, hence passing an existing list to this
 * function will cause a memory leak.
 */
int8_t WiFiGetScanResults(slsi_scan_info_t **scan_results);

/**
 * @brief Free the result list returned by WifiGetScanResults.
 * Does nothing if a NULL pointer is passed.
 * Sets *scan_results to NULL after operation is done.
 * @param [in] scan_results       A pointer to the linked list to free
 * @return Returns always as succeed.
 */
int8_t WiFiFreeScanResults(slsi_scan_info_t **scan_results);

/**
 * @brief Join a known Wi-Fi AP.
 * @param [in] ssid               SSID to be used for the AP
 * @param [in] ssid_len           Length of SSID (# of valid octets in @ssid)
 * @param [in] bssid              preferred BSSID to be used when multiple APs has same SSID.
 *                                Set to NULL for best effort
 * @param [in] security_config    A pointer to data to slsi_security_config_t containing passphrase, keymgmt and cipher.
 *                                The caller owns this struct, use NULL if open security is requred.
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 *
 * @details Join a known Wi-Fi AP with specified SSID and security key. The authentication
 * type needed will automatically be resolved by the Wi-Fi stack.
 * The function will return after the connection is tried.
 */
int8_t WiFiNetworkJoin(uint8_t *ssid, uint8_t ssid_len, uint8_t *bssid, const slsi_security_config_t *security_config);

/**
 * @brief Leave a connected AP.
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 *
 * @details Close the connection to a connected AP. This will not close down the Wi-Fi
 * stack but simply disconnect.
 */
int8_t WiFiNetworkLeave(void);

/**
 * @brief Request get tx power.
 * @param [out] dbm A pointer to data to return value in dbm
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 */
int8_t WiFiGetTxPower(uint8_t *dbm);

/**
 * @brief Request set tx power.
 * @param [in] dbm A pointer to value between 12 and 30 dbm
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 */
int8_t WiFiSetTxPower(uint8_t *dbm);

/**
 * @brief Request get own mac address.
 * @param [out] mac A pointer to data to return mac address (xx:xx:xx:xx:xx:xx)
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 */
int8_t WiFiGetMac(uint8_t *mac);

/**
 * @brief Request rssi level of connection.
 * @param [out] rssi A pointer to data to return level of rssi
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 */
int8_t WiFiGetRssi(int8_t *rssi);

/**
 * @brief Request channel of connection.
 * @param [out] channel A pointer to data to return channel of current connection
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 */
int8_t WiFiGetChannel(int8_t *channel);

/**
 * @brief Request connected status.
 * @param [out] count      Connection status according to operation mode
 * - In STA mode: Equal and larger than 1 for connected, NULL for disconnected
 * - In AP mode: The count of connected devices in an AP
 * @param [out] details    A pointer to a data structure which will be filled with BSSID and SSID of AP
 * - In STA mode: Information of connected AP, if disconnected NULL
 * - In AP mode: Unused
 *
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 * @details  In case of failure count will be set to the last know information and
 *           details will be filled as best effort (hence all values that could not
 *           be extracted will have the value 0)
 */
int8_t WiFiIsConnected(uint8_t *count, slsi_reason_t *details);

/**
 * @brief Request the current mode of operation.
 * @param [out] mode       Wi-Fi operation mode (one of SLSI_WIFI_NONE, SLSI_WIFI_STATION_IF and SLSI_WIFI_SOFT_AP_IF)
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 */
int8_t WiFiGetOpMode(WiFi_InterFace_ID_t *mode);

/**
 * @brief Request to set country code.
 * @param [in] country_code    A pointer to data to containing a two capital letter country code
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 */
int8_t WiFiSetCountryCode(const char *country_code);

/**
 * @brief Request to save network configuration file.
 * @return On success, 0 is returned. On failure, a negative vaule is returned.
 *
 * @details  Will save current network state in configuration file. E.g. if auto
 *           connect is enabled and a network is saved, next time Wi-Fi is started
 *           it will look into configuration file and try to connect known network
 */
int8_t WiFiSaveConfig(void);


#ifdef CONFIG_SLSI_WIFI_P2P_API
/**
 * Set WiFiP2P device name
 * @param device_name       Name of own device
 * @param device_name_len   Length of device name included
 * @return                  SLSI_STATUS_SUCCESS or other failure, see @SLSI status codes
 *
 *  Will change P2P Device Name in supplicant and furthermore be used as extended postfix
 *  for the WiFi Direct ssid name, e.g. DIRECT-xy-[POSTFIX-]device_name, if not set
 *  last part of MAC address is used instead, e.g. DIRECT-xy-[POSTFIX-]1234, if postfix
 *  name length exceeds SLSI_SSID_LEN, the name will be cropped
 */
int8_t WiFiP2PSetDeviceName(uint8_t* device_name, uint8_t device_name_len);

/**
 * Start WiFiP2P Listen
 * @return              SLSI_STATUS_SUCCESS or other failure, see @SLSI status codes
 */
int8_t WiFiP2PListen(void);

/**
 * Stop WiFiP2P Listen
 * @return              SLSI_STATUS_SUCCESS or other failure, see @SLSI status codes
 */
int8_t WiFiP2PStopListen(void);

/**
 * Discover WiFiP2P devices nearby.
 * @param timeout           timeout value in seconds. WiFiP2P find will be stopped after timeout.
 *                          0 indicates no timeout, max value is size of parameter type.
 * @param device_address    Array containing one specific device to look for. Leave empty to look
 *                          for all P2P devices.
 * @return                  SLSI_STATUS_SUCCESS or other failure, see @SLSI status codes
 *
 *  Devices found will be returned in find callback function see @slsi_p2p_find_handler_t
 *  callback function below. If device address specified only this device will be reported in
 *  callback
 */
int8_t WiFiP2PFind(uint32_t timeout, char device_address[SLSI_MACADDR_STR_LEN]);

/**
 * Stop P2P find explicitly
 * @return              SLSI_STATUS_SUCCESS or other failure, see @SLSI status codes
 */
int8_t WiFiP2PStopFind(void);

/**
 * Callback function to be called when nearby devices are found
 * @param find_info     struct with found peers and reason code
 */
typedef void (*slsi_p2p_find_callback_t)(slsi_find_info_t *find_info);

/**
 * Register callback handler for find results
 * @param find_handler  callback handler function that will be registered
 * @return              SLSI_STATUS_SUCCESS or other failure, see @SLSI status codes
 */
int8_t WiFiP2PRegisterFindCallback(slsi_p2p_find_callback_t find_handler);

/**
 * Start Autonomous GO
 * @param channel       GO Operating channel
 * @param persistent    Indicates if it will be a persistent group or not.
 *                      It has to be FALSE as of now.
 * @return              SLSI_STATUS_SUCCESS or other failure, see @SLSI status codes
 */
int8_t WiFiP2PCreateGroup(uint8_t channel, bool persistent);

/**
 * Stop Autonomous GO
 * @return              SLSI_STATUS_SUCCESS or other failure, see @SLSI status codes
 */
int8_t WiFiP2PRemoveGroup(void);

/**
 * Try to connect to p2p device
 * @param device_address    Address of device for connection establishment
 * @param config_method     Configuration method to be used for authentication,
 *                          see @slsi_config_method_t for details
 * @return                  SLSI_STATUS_SUCCESS or other failure, see @SLSI status codes
 *
 *  See slsi_p2p_connect_ind_t handler for exchange of provisioning information
 *  during connection establishment
 */
int8_t WiFiP2PConnect(char device_address[SLSI_MACADDR_STR_LEN],
        slsi_config_method_t config_method);

/**
 * This is invoked when it received connection indication from a peer.
 * @param peer_info     received information of peer device, see @slsi_peer_info_t structure
 * @param method        Configuration method which is used, see @slsi_config_method_t
 * @param response      Application can accept or reject the request, see @slsi_p2p_connect_response_t
 * @param go_intent     Intent to become a Group Owner, value between 1 and 15
 * @param passphrase    Passphrase to be displayed in case of DISPLAY config method,
 *                      passphrase entered by applicaiton in case of KEYPAD config method, or
 *                      left empty (= NULL) if config method is PBC
 * @return              SLSI_STATUS_SUCCESS or other failure, see @SLSI status codes
 *
 *  Note: Implementation of connection handler should never call back into API
 */
typedef void (*slsi_p2p_connect_ind_t)(slsi_peer_info_t *peer_info,
        slsi_config_method_t config_method,
        slsi_p2p_connect_response_t *response,
        uint8_t *go_intent,
        char **passphrase);

/**
 * Disconnect the connection
 * @return      SLSI_STATUS_SUCCESS or other failure, see @SLSI status codes
 *
 *  Will disconnect the ongoing connection to Wi-Fi Direct peer in case of GO or CLI.
 *  In case of Autonomous GO, the WiFiP2PRemoveGroup must be used instead
 *
 */
int8_t WiFiP2PDisconnect(void);

/**
 * Used to register connection callback handler function
 * @param connect_handler   Callback function for connection indication, see
 *                          slsi_p2p_connect_ind_t for details
 * @return                  SLSI_STATUS_SUCCESS or other failure, see @SLSI status codes
 * This needs to be invoked prior to using the P2PFind api.
 */
int8_t WiFiP2PRegisterConnectionHandler(slsi_p2p_connect_ind_t connect_handler);

/**
 * Used to register link callback handler functions
 * @param link_up   Callback for link up, see @slsi_network_link_callback_t for details
 * @param link_down Callback handler for link down, see @slsi_network_link_callback_t for details
 * @return          SLSI_STATUS_SUCCESS or other failure, see @SLSI status codes
 */
int8_t WiFiP2PRegisterLinkCallback(slsi_network_link_callback_t link_up, slsi_network_link_callback_t link_down);

#endif //CONFIG_SLSI_WIFI_P2P_API

#undef EXTERN
#ifdef  __cplusplus
}
#endif
#endif							/* SLSI_WIFI_API_H_ */
		 /**@} *///end of doxygen defgroup
