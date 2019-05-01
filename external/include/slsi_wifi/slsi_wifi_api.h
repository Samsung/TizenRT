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
* @defgroup WiFi WiFi
* SLSI WiFi API for TinyAra
* @{
*
* @brief S.LSI WiFi API for TinyARA.
*
* This is the Samsung LSI WiFi API for TinyARA OS. It contains support for both station and soft-ap
* mode on the 2.4Ghz frequency.
* The API has support for security modes WPA/WPA2/WEP with various ciphers.
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
#ifdef CONFIG_WIFI_MANAGER
#define CTRL_IFNAME	CONFIG_WIFIMGR_STA_IFNAME
#else
#define CTRL_IFNAME "wl1"
#endif



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

/* slsi wifi network interface - api parameter defines */
typedef enum WiFi_InterFace_ID {
	SLSI_WIFI_NONE,
	SLSI_WIFI_STATION_IF,	// Station mode (turns on wpa_supplicant)
	SLSI_WIFI_SOFT_AP_IF,	// Soft AP mode (turns on hostapd)
	SLSI_WIFI_P2P_IF		// P2P mode (turns on wpa_supplicant)
} WiFi_InterFace_ID_t;

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

typedef struct slsi_ht_config {
	uint16_t ht_capab_info;		// Supported fields: HT_CAP_INFO_GREEN_FIELD, HT_CAP_INFO_SHORT_GI20MHZ
	uint8_t mcs_index[10];		// 0 uses default, supported HT-MCS rates - in ASCII hex: 0xffff0000000000000000
} slsi_ht_config_t;

typedef struct slsi_security_config {
	uint32_t secmode;
	char passphrase[SLSI_PASSPHRASE_LEN];
} slsi_security_config_t;

typedef struct slsi_vendor_ie {
	uint8_t oui[3];				// Organizational Unique ID
	uint8_t *content;			// the ponter to the data allocated for the Vendor IE block
	uint8_t content_length;		// total size of the allocated buffer
} slsi_vendor_ie_t;

typedef struct slsi_ap_config {
	uint8_t ssid[SLSI_SSID_LEN + 1];	// 802.11 spec defined unspecified or uint8
	int8_t ssid_len;					// length of ssid - # of valid octets
	uint32_t beacon_period;				// beacon period, default 100 TU
	uint32_t DTIM;						// Delivery Traffic Information Message (range 1..255), default 2
	uint8_t channel;					// channel/frequency
	uint8_t phy_mode;					// 0:legacy 1: 11N HT
	slsi_ht_config_t ht_mode;			// requires CONFIG_HT_OVERRIDES enabled, see slsi_ht_config_t
	slsi_security_config_t *security;	// use NULL if security mode is open
	slsi_vendor_ie_t *vsie;				// Vender specific IE block
} slsi_ap_config_t;

typedef struct slsi_scan_info {
	uint8_t ssid[SLSI_SSID_LEN + 1];	// 802.11 spec defined unspecified or uint8
	char bssid[SLSI_MACADDR_STR_LEN];	// char string e.g. xx:xx:xx:xx:xx:xx
	int8_t ssid_len;					// length of ssid - # of valid octets
	int8_t rssi;						// rssi level of scanned device
	uint32_t beacon_period;				// beacon period used (default 100)
	uint8_t channel;					// channel/frequency
	uint8_t phy_mode;					// 0:legacy 1: 11N HT
	uint8_t bss_type;					// 0:infrastructure, 1:independent
	uint8_t wps_support;				// boolean 1 supported, 0 not supported
	uint8_t num_sec_modes;				// number of elements of security modes in sec_mode
	slsi_ht_config_t ht_mode;			// See slsi_ht_config_t
	slsi_security_config_t *sec_modes;	// list of security modes
	struct slsi_scan_info *next;
} slsi_scan_info_t;

typedef struct slsi_reason {
	uint32_t reason_code;				// Reason codes - 0 for success - error code see 'SLSI reason codes' above
	uint8_t locally_generated;			// Which side cause link down, 1 = locally, 0 = remotely - valid for STA mode only
	int8_t ssid_len;					// length of ssid - # of valid octets
	uint8_t ssid[SLSI_SSID_LEN + 1];	// 802.11 spec defined up to 32 octets of data
	char bssid[SLSI_MACADDR_STR_LEN];	// BSS identification, char string e.g. xx:xx:xx:xx:xx:xx
} slsi_reason_t;

typedef int8_t (*network_scan_result_handler_t)(slsi_reason_t *reason);
typedef void (*slsi_network_link_callback_t)(slsi_reason_t *reason);

/**
 * Set the network configuration and start Wi-Fi interface
 * @interface_id    interface id to use (STATION_IF = 0 or SOFT_AP_IF = 1)
 * @ap_config       network configurations needed if SOFT_AP_IF is selected as
 *                  WiFi_InterFace_ID_t else NULL. The caller owns this structure.
 * Return: Completed successfully or failed
 *
 * Set the network configuration and start Wi-Fi interface in either AP mode or
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
 * Stop the Wi-Fi interface
 * Return: Completed successfully or failed
 *
 * Stop the interface that currently associated AP, or abort the current
 * connection process. A WiFiStop implies a stop of the Wi-Fi stack,
 * driver and IP stack if nothing else needs it.
 * If the selected interface is not running, the function will return success
 */
int8_t WiFiStop(void);

/**
 * Register callback functions for WiFi link
 * @link_up        callback function to register for the link up event
 * @link_down    callback function to register for the link down event
 * Return: Completed successfully or failed
 *
 * Register callback functions that gets called when a change in network link
 * status occurs, these carry a slsi_reason_t to tell why link is up or down
 */
int8_t WiFiRegisterLinkCallback(slsi_network_link_callback_t link_up, slsi_network_link_callback_t link_down);

/**
 * Register callback functions for WiFi scan
 * @scan_result_handler     callback function to register for the link up event
 * Return: Completed successfully or failed
 *
 * Register callback functions that gets called when a scan trigger gets
 * response for its request has done.
 */
int8_t WiFiRegisterScanCallback(network_scan_result_handler_t scan_result_handler);

/**
 * Scan for Wi-Fi network
 *
 * Return: Scan initiated successfully or failed
 *
 * Start a scan for Wi-Fi AP in the surroundings.
 * result is returned to the callback handler registered with
 * WiFiRegisterScanCallback(). Events are : Scan complete event
 * or Scan aborted.
 * The scan results are retrieved using WiFiGetScanResults().
 */
int8_t WiFiScanNetwork(void);

/**
 * Return current scan results list
 * @scan_results        A pointer to a linked list of scan results
 *                      The caller takes owner responsibility of this list,
 *                      and must free each item on the list after use.
 *                      For convince the function WiFiFreeScanResults() can
 *                      be used to free the list.
 *
 * Return: Completed successfully or failed
 *
 * Request available list of scan results for Wi-Fi AP in the surroundings. The
 * results are returned as the list of structure slsi_ap_info_t linked via next
 * pointer.
 * The list must be freed by the caller, hence passing an existing list to this
 * function will cause a memory leak.
 */
int8_t WiFiGetScanResults(slsi_scan_info_t **scan_results);

/**
 * Free the result list returned by WifiGetScanResults.
 * Does nothing if a NULL pointer is passed.
 * Sets *scan_results to NULL after operation is done.
 * @scan_results       A pointer to the linked list to free
 * Return: Always success
 */
int8_t WiFiFreeScanResults(slsi_scan_info_t **scan_results);

/**
 * Join a known Wi-Fi AP
 * @ssid               SSID to be used for the AP
 * @ssid_len           Length of SSID (# of valid octets in @ssid)
 * @bssid              preferred BSSID to be used when multiple APs has same SSID.
 *                     Set to NULL for best effort
 * @security_config    a pointer to data to slsi_security_config_t containing passphrase, keymgmt and cipher.
 *                     The caller owns this struct, use NULL if open security is requred.
 * Return: Completed successfully or failed
 *
 * Join a known Wi-Fi AP with specified SSID and security key. The authentication
 * type needed will automatically be resolved by the Wi-Fi stack.
 * The function will return after the connection is tried.
 */
int8_t WiFiNetworkJoin(uint8_t *ssid, uint8_t ssid_len, uint8_t *bssid, const slsi_security_config_t *security_config);

/**
 * Leave a connected AP
 * Return: Completed successfully or failed
 *
 * Close the connection to a connected AP. This will not close down the Wi-Fi
 * stack but simply disconnect.
 */
int8_t WiFiNetworkLeave(void);

/**
 * Request get tx power.
 *  @dbm: a pointer to data to return value in dbm.
 * Return: success or failure
 */
int8_t WiFiGetTxPower(uint8_t *dbm);

/**
 * Request set tx power
 *  @dbm: a pointer to value between 12 and 30 dbm
 * Return: success or failure
 */
int8_t WiFiSetTxPower(uint8_t *dbm);

/**
 * Request get own mac address
 *  @mac: a pointer to data to return mac address (xx:xx:xx:xx:xx:xx)
 * Return: success or failure
 */
int8_t WiFiGetMac(uint8_t *mac);

/**
 * Request rssi level of connection
 *  @rssi: a pointer to data to return level of rssi
 * Return: success or failure
 */
int8_t WiFiGetRssi(int8_t *rssi);

/**
 * Request channel of connection
 *  @channel: a pointer to data to return channel of current connection
 * Return: success or failure
 */
int8_t WiFiGetChannel(int8_t *channel);

/**
 * Request connected status:
 * In STA mode:
 *   @count      1 for connected in STA mode, count of connected devices in AP
 *               mode if count > 1 details are updated, otherwise NULL
 *               0 for disconnected
 *   @details    a pointer to a data structure which will be filled in with
 *               BSSID and SSID of AP if connected. Set to NULL if this
 *               information is not needed
 * In AP mode:
 *   @details    unused - PASS NULL
 *   @count      the number of currently connected stations.
 *
 * Return: success or failure
 *   In case of failure count will be set to the last know information and
 *   details will be filled as best effort (hence all values that could not
 *   be extracted will have the value 0)
 */
int8_t WiFiIsConnected(uint8_t *count, slsi_reason_t *details);

/**
 * Request the current mode of operation.
 *  @mode:       sets to one of SLSI_WIFI_NONE, SLSI_WIFI_STATION_IF,
 *               SLSI_WIFI_SOFT_AP_IF
 * Return: success or failure
 */
int8_t WiFiGetOpMode(WiFi_InterFace_ID_t *mode);

/**
 * Request to set country code
 *  @country_code    a pointer to data to containing a two capital letter country code
 * Return: Completed successfully or failed
 */
int8_t WiFiSetCountryCode(const char *country_code);

/**
 * Request to save network configuration file
 * Return: Completed successfully or failed
 *
 *   Will save current network state in configuration file. E.g. if auto
 *   connect is enabled and a network is saved, next time Wi-Fi is started
 *   it will look into configuration file and try to connect known network
 */
int8_t WiFiSaveConfig(void);

/**
 * Request to set the autoconnect of external driver.
 *	@check		1 for enabling and 0 for disabling external autoconnect
 * Return: Completed successfully or failed
 *
 *   Will set external autoconnect enabled or disabled
 */
int8_t WiFiSetAutoconnect(uint8_t check);

#undef EXTERN
#ifdef  __cplusplus
}
#endif
#endif							/* SLSI_WIFI_API_H_ */
		 /**@} *///end of doxygen defgroup
