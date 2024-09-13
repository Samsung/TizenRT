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

#pragma once

#include <stdint.h>
#include <sys/time.h>

#define TRWIFI_MACADDR_LEN			  6
#define TRWIFI_MACADDR_STR_LEN		  17
#define TRWIFI_SSID_LEN				  32
#define TRWIFI_PASSPHRASE_LEN		  64
#define TRWIFI_CBK_MSG_INITIALIZER {0, {0,}, NULL}

/*  reason */
/* It's on-going definitions to get wi-fi error reason
 * It's not defined yet. REASON_UNKNOWN is sample definition to test.
 * It'll be added later.
 */
#define TRWIFI_REASON_UNKNOWN 0

/*  if serialization is failed then driver should let pass the fail event to applications*/
#define TRWIFI_POST_SCANEVENT(dev, evt, scanlist)                             \
	do {                                                                      \
		if (evt == LWNL_EVT_SCAN_DONE && scanlist != NULL) {                  \
			uint8_t *buffer = NULL;                                           \
			int lwnl_res = trwifi_serialize_scaninfo(&buffer, scanlist);      \
			if (lwnl_res < 0) {                                               \
				trwifi_post_event(dev, LWNL_EVT_SCAN_FAILED, NULL, 0);        \
			} else {                                                          \
				trwifi_post_event(dev, LWNL_EVT_SCAN_DONE, buffer, lwnl_res); \
				kmm_free(buffer);                                             \
			}                                                                 \
		} else {                                                              \
			trwifi_post_event(dev, LWNL_EVT_SCAN_FAILED, NULL, 0);            \
		}                                                                     \
	} while (0)

typedef enum {
	TRWIFI_FAIL = -1,
	TRWIFI_SUCCESS,
	TRWIFI_INVALID_ARGS,
	TRWIFI_TIMEOUT,
	TRWIFI_BUSY,
	TRWIFI_FILE_ERROR,
	TRWIFI_ALREADY_CONNECTED,
	TRWIFI_NOT_SUPPORTED,
} trwifi_result_e;

typedef enum {
	TRWIFI_IEEE_80211_LEGACY,			  /**<	IEEE 802.11a/g/b		  */
	TRWIFI_IEEE_80211_A,				  /**<	IEEE 802.11a			  */
	TRWIFI_IEEE_80211_B,				  /**<	IEEE 802.11b			  */
	TRWIFI_IEEE_80211_G,				  /**<	IEEE 802.11g			  */
	TRWIFI_IEEE_80211_N,				  /**<	IEEE 802.11n			  */
	TRWIFI_IEEE_80211_AC,				  /**<	IEEE 802.11ac			  */
	TRWIFI_MODE_NOT_SUPPORTED,			  /**<	Driver does not report	  */
} trwifi_standard_type_e;

typedef enum {
	TRWIFI_AUTH_OPEN,					 /**<  open mode								 */
	TRWIFI_AUTH_WEP_SHARED,				 /**<  use shared key (wep key)					 */
	TRWIFI_AUTH_WPA_PSK,				 /**<  WPA_PSK mode								 */
	TRWIFI_AUTH_WPA2_PSK,				 /**<  WPA2_PSK mode							 */
	TRWIFI_AUTH_WPA3_PSK,				 /**<  WPA3_PSK mode							 */
	TRWIFI_AUTH_WPA_AND_WPA2_PSK,		 /**<  WPA_PSK and WPA_PSK mixed mode			 */
	TRWIFI_AUTH_WPA2_AND_WPA3_PSK,		 /**<  WPA2_PSK and WPA3_PSK mixed mode			 */
	TRWIFI_AUTH_WPA_PSK_ENT,			 /**<  Enterprise WPA_PSK mode					 */
	TRWIFI_AUTH_WPA2_PSK_ENT,			 /**<  Enterprise WPA2_PSK mode					 */
	TRWIFI_AUTH_WPA_AND_WPA2_PSK_ENT,	 /**<  Enterprise WPA_PSK and WPA_PSK mixed mode */
	TRWIFI_AUTH_IBSS_OPEN,				 /**<  IBSS ad-hoc mode						     */
	TRWIFI_AUTH_WPS,					 /**<  WPS mode									 */
	TRWIFI_AUTH_UNKNOWN,				 /**<  unknown type								 */
} trwifi_ap_auth_type_e;

typedef enum {
	TRWIFI_CRYPTO_NONE,					 /**<  none encryption							 */
	TRWIFI_CRYPTO_WEP_64,				 /**<  WEP encryption wep-40					 */
	TRWIFI_CRYPTO_WEP_128,				 /**<  WEP encryption wep-104					 */
	TRWIFI_CRYPTO_AES,					 /**<  AES encryption							 */
	TRWIFI_CRYPTO_TKIP,					 /**<  TKIP encryption							 */
	TRWIFI_CRYPTO_TKIP_AND_AES,			 /**<  TKIP and AES mixed encryption			 */
	TRWIFI_CRYPTO_AES_ENT,				 /**<  Enterprise AES encryption				 */
	TRWIFI_CRYPTO_TKIP_ENT,				 /**<  Enterprise TKIP encryption				 */
	TRWIFI_CRYPTO_TKIP_AND_AES_ENT,		 /**<  Enterprise TKIP and AES mixed encryption	 */
	TRWIFI_CRYPTO_UNKNOWN,				 /**<  unknown encryption						 */
} trwifi_ap_crypto_type_e;

typedef enum {
	TRWIFI_DISCONNECTED,			/**<  wifi is disconnected	*/
	TRWIFI_CONNECTED,			/**<  connected				*/
	TRWIFI_SOFTAP_MODE,			/**<  soft ap mode			*/
} trwifi_status_e;

typedef enum {
	LWNL_REQ_WIFI_INIT,
	LWNL_REQ_WIFI_DEINIT,
	LWNL_REQ_WIFI_SCANAP,
	LWNL_REQ_WIFI_GETINFO,
	LWNL_REQ_WIFI_SETAUTOCONNECT,
	LWNL_REQ_WIFI_STARTSTA,
	LWNL_REQ_WIFI_CONNECTAP,
	LWNL_REQ_WIFI_DISCONNECTAP,
	LWNL_REQ_WIFI_STARTSOFTAP,
	LWNL_REQ_WIFI_STOPSOFTAP,
	LWNL_REQ_WIFI_IOCTL,
	LWNL_REQ_WIFI_SCAN_MULTI_APS,
	LWNL_REQ_WIFI_SET_CHANNEL_PLAN,
	LWNL_REQ_WIFI_GET_SIGNAL_QUALITY,
	LWNL_REQ_WIFI_GET_DISCONNECT_REASON,
	LWNL_REQ_WIFI_GET_DRIVER_INFO,
	LWNL_REQ_WIFI_GET_WPA_SUPPLICANT_STATE,
	LWNL_REQ_WIFI_UNKNOWN
} lwnl_req_wifi;

/**
 * @event LWNL_EVT_STA_CONNECTED
 *        if trwifi_connect_ap() were succeeded then a driver should
 *        generate LWNL_EVT_STA_CONNECTED. A driver can pass the event by calling
 *        trwifi_post_event(struct netdev *, LWNL_EVT_STA_CONNECTED, trwifi_cbk_msg_s *, sizeof(trwifi_cbk_msg_s))
 *
 * @event LWNL_EVT_STA_CONNECT_FAILED
 *        if trwifi_connect_ap() were failed then a driver should generate
 *        LWNL_EVT_STA_CONNECT_FAILED. A driver can pass the event by calling
 *        trwifi_post_event(struct netdev *, LWNL_EVT_STA_CONNECTED, trwifi_cbk_msg_s *, sizeof(trwifi_cbk_msg_s))
 *        reason in trwifi_cbk_msg_s must be written.
 *
 * @event LWNL_EVT_STA_DISCONNECTED:
 *        The event can be happened after a target succeeded to connect to an AP.
 *        There're two cases. the first case is wifi_manager called trwifi_disconnect_ap().
 *        A driver should generate the LWNL_EVT_STA_DISCONNECTED event, if it is connected to an AP.
 *        The other case is an AP is disconnected unexpectedly. For example a connected AP is turn off.
 *        The driver have to pass the event with reason code by calling
 *        trwifi_post_event(struct netdev *, LWNL_EVT_STA_DISCONNECTED, trwifi_cbk_msg_s *, sizeof(trwifi_cbk_msg_s))
 *        reason in trwifi_cbk_msg_s must be written.
 *
 * @event LWNL_EVT_SOFTAP_STA_JOINED:
 *        if a station is joined while wi-fi is softap mode after calling trwifi_start_softap() then
 *        LWNL_EVT_SOFTAP_STA_JOINED event should be generated.
 *        A driver can pass the event by calling
 *        trwifi_post_event(struct netdev *, LWNL_EVT_SOFTAP_STA_JOINED, trwifi_cbk_msg_s *, sizeof(trwifi_cbk_msg_s))
 *        bssid in trwifi_cbk_msg_s must be written.
 *
 * @event LWNL_EVT_SOFTAP_STA_LEFT:
 *        If a station were left then a wi-fi driver should generate LWNL_EVT_SOFTAP_STA_LEFT.
 *        A driver can pass the event by calling
 *        trwifi_post_event(struct netdev *, LWNL_EVT_SOFTAP_STA_LEFT, trwifi_cbk_msg_s *, sizeof(trwifi_cbk_msg_s))
 *        reason in trwifi_cbk_msg_s must be written.
 *
 * @event LWNL_EVT_SCAN_DONE
 *        a wi-fi driver generates the event if it succeeded to scan.
 *        a wi-fi driver must generate LWNL_EVT_SCAN_DONE or LWNL_EVT_SCAN_FAILED
 *        if it returns success about trwifi_scan_ap()
 *        Unlike other events it uses TRWIFI_POST_SCANEVENT() because of serializing scan information.
 *        The driver can pass the event by calling
 *        TRWIFI_POST_SCANEVENT(struct netdev *, LWNL_EVT_SCAN_DONE, trwifi_scan_list_s *)
 *
 * @event LWNL_EVT_SCAN_FAILED
 *        a wi-fi driver generates the event if it failed to scan.
 *        a wi-fi driver must generate LWNL_EVT_SCAN_DONE or LWNL_EVT_SCAN_FAILED
 *        if it returns success to trwifi_scan_ap()
 *        The driver can pass the event by calling
 *        trwifi_post_event(struct netdev *, LWNL_EVT_SCAN_FAILED, NULL, 0);
 */
typedef enum {
	LWNL_EVT_STA_CONNECTED,
	LWNL_EVT_STA_CONNECT_FAILED,
	LWNL_EVT_STA_DISCONNECTED,
	LWNL_EVT_SOFTAP_STA_JOINED,
	LWNL_EVT_SOFTAP_STA_LEFT,
	LWNL_EVT_SCAN_DONE,
	LWNL_EVT_SCAN_FAILED,
	LWNL_EVT_EXIT,
	LWNL_EVT_UNKNOWN,
} lwnl_cb_wifi;

typedef struct {
	uint32_t reason;
	char bssid[6];
	void *data;
} trwifi_cbk_msg_s;

typedef struct {
	char ssid[TRWIFI_SSID_LEN + 1];				 /**<  Service Set Identification		  */
	unsigned int ssid_length;						/**<  Service Set Identification Length	 */
	char passphrase[TRWIFI_PASSPHRASE_LEN + 1];	 /**<  ap passphrase(password)			  */
	unsigned int passphrase_length;					/**<  ap passphrase length				 */
	trwifi_ap_auth_type_e ap_auth_type;			 /**<  @ref trwifi_ap_auth_type		   */
	trwifi_ap_crypto_type_e ap_crypto_type;		 /**<  @ref trwifi_ap_crypto_type	   */
} trwifi_ap_config_s;

typedef struct {
	unsigned int channel;								   /**<	 Radio channel that the AP beacon was received on		*/
	char ssid[TRWIFI_SSID_LEN + 1];						/**<  Service Set Identification (i.e. Name of Access Point) */
	unsigned int ssid_length;							   /**<	 The length of Service Set Identification				*/
	unsigned char bssid[TRWIFI_MACADDR_STR_LEN + 1];	/**<  MAC address (xx:xx:xx:xx:xx:xx) of Access Point		 */
	unsigned int max_rate;								   /**<	 Maximum data rate in kilobits/s						*/
	int rssi;											   /**<	 Receive Signal Strength Indication in dBm				*/
	trwifi_standard_type_e phy_mode;					/**< Supported MAC/PHY standard								 */
	trwifi_ap_auth_type_e ap_auth_type;					/**<  @ref trwifi_ap_auth_type							  */
	trwifi_ap_crypto_type_e ap_crypto_type;				/**<  @ref trwifi_ap_crypto_type						  */
} trwifi_ap_scan_info_s;

typedef struct {
	unsigned int channel;                             /**<  wifi channel, =0 is full channel scan */
	char ssid[TRWIFI_SSID_LEN + 1];                  /**<  Service Set Identification            */
	unsigned int ssid_length;                         /**<  Service Set Identification Length     */
} trwifi_scan_config_s;

#define SCAN_SSID_CNT (6)

typedef struct {
	trwifi_scan_config_s scan_ap_config[SCAN_SSID_CNT];
	unsigned int scan_ap_config_count;                         /**<  the count of SSID to scan  */
	bool scan_all;											/**<  Flag to enable scanning specific AP + other APs responding to NULL probe req  */
} trwifi_scan_multi_configs_s;

typedef struct {
	unsigned int channel;								   /**<	 soft ap wifi channel				*/
	char ssid[TRWIFI_SSID_LEN + 1];						/**<  Service Set Identification		 */
	unsigned int ssid_length;							   /**<	 Service Set Identification Length	*/
	char passphrase[TRWIFI_PASSPHRASE_LEN + 1];			/**<  ap passphrase(password)			 */
	unsigned int passphrase_length;						   /**<	 ap passphrase length				*/
	trwifi_ap_auth_type_e ap_auth_type;					/**<  @ref trwifi_ap_auth_type		  */
	trwifi_ap_crypto_type_e ap_crypto_type;				/**<  @ref trwifi_ap_crypto_type	  */
	void (*inform_new_sta_join)(void);						/**< @ref new station joining softAP	*/
} trwifi_softap_config_s;

typedef struct trwifi_scan_list {
	trwifi_ap_scan_info_s ap_info;
	struct trwifi_scan_list *next;
} trwifi_scan_list_s;

typedef struct {
	int rssi;                       /**<     Received Signal Strength Indication in dBm */
	trwifi_status_e wifi_status;    /**<  @ref trwifi_status                     */
	struct timeval disconn_time;    // optional: store the last time when the connection is disconnected
	int32_t reason_code;            // optional: the reason why connection is disconnected. value is vendor specific
} trwifi_info;

typedef struct {
	unsigned int channel;	/**<	 wifi channel			*/
	signed char snr;		/**<	 signal to noise ratio	*/
	unsigned long tx_drop;
	unsigned long rx_drop;
	unsigned int tx_retry;	/**<	 tx retry count		*/
	int network_bw;			/**<	 network bandwidth		*/
	unsigned int max_rate;	/**<	 max rate			*/
} trwifi_signal_quality;

typedef struct {
	char lib_version[64];	/**<	 library version			*/
} trwifi_driver_info;

typedef struct {
	int wpa_supplicant_state;	/**<	 wpa_supplicant state		*/
	int wpa_supplicant_key_mgmt;	/**<	 wpa_supplicant key mgmt	*/
} trwifi_wpa_states;

typedef uint16_t trwifi_cmd;
/*  Set/Get Power save mode */
#define TRWIFI_MSG_GET_POWERMODE 1
#define TRWIFI_MSG_SET_POWERMODE 2
/*  Get driver statistics */
#define TRWIFI_MSG_GET_STATS 3

/*  power mode(command) value */
#define TRWIFI_POWERMODE_ON 1
#define TRWIFI_POWERMODE_OFF 2

/*  TizenRT Wi-Fi Message */
typedef struct {
	trwifi_cmd cmd;
	void *data;
} trwifi_msg_s;

/*  Statistics message */
typedef struct {
	trwifi_cmd cmd;
	struct timeval start;
	struct timeval end;
	uint32_t tx_retransmit;
	uint32_t tx_drop;
	uint32_t rx_drop;
	uint32_t tx_success_cnt;
	uint32_t tx_success_bytes;
	uint32_t rx_cnt;
	uint32_t rx_bytes;
	uint32_t tx_try;
	uint32_t rssi_avg;
	uint32_t rssi_min;
	uint32_t rssi_max;
	uint32_t beacon_miss_cnt;
} trwifi_msg_stats_s;

struct netdev;
/**
 * @brief   Initialize wi-fi library
 *
 * @param[in]   dev : struct netdev registered by netdev_register()
 *
 * @function_type  synchronous call
 *
 * @description    run STA mode when wi-fi library initialized.
 *
 * @return TRWIFI_SUCCESS : success
 * @return TRWIFI_FAIL    : fail
 */
typedef trwifi_result_e (*trwifi_init)(struct netdev *dev);

/**
 * @brief   De-initialize wi-fi library
 *
 * @param[in]   dev : struct netdev registered by netdev_register()
 *
 * @function_type  synchronous call
 *
 * @return TRWIFI_SUCCESS      : success
 * @return TRWIFI_FAIL         : fail
 * @return TRWIFI_INVALID_ARGS : arguments are invalid
 */
typedef trwifi_result_e (*trwifi_deinit)(struct netdev *dev);

/**
 * @brief   Scan access points
 *
 * @param[in]   dev    : struct netdev registered by netdev_register()
 * @param[in]   config : an access point information to scan.
 *
 * @function_type  asynchronous call : Send event by trwifi_post_event()
 * @event  LWNL_EVT_SCAN_DONE    : scan success
 * @event  LWNL_EVT_SCAN_FAILED  : scan fail
 *
 * @description If the call is successful, then it must generate
 *              events(LWNL_EVT_SCAN_DONE or LWNL_EVT_SCAN_FAILED).
 *              If the call fails then it shouldn't generate events.
 *
 * @return TRWIFI_SUCCESS      : success (should generate an event.)
 * @return TRWIFI_FAIL         : fail (shouldn't generate an event.)
 * @return TRWIFI_INVALID_ARGS : arguments are invalid
 *
 * @note   A driver must generate event if it return success.
 *         A driver can check whether SSID is set by ssid_length in config.
 *         If ssid_length is 0 then SSID isn't set. wifi_manager which send requests to a driver will
 *         check validation of SSID(ex. length of SSID).
 *         In the same way, if channel in config is 0 then channel isn't configured.
 *         If channel is 0 then SSID must be set. wifi_manager will check this.
 *         So a driver doesn't need to consider such case both channel and ssid_length are 0
 *
 *         SCAN type
 *           1) full scan: config is null
 *           2) scan with specific SSID: ssid is set in config
 *           3) scan with specific channel: channel is set in config
 *           4) scan with specific SSID and channel: Both ssid and channel are set in config
 *
 */
typedef trwifi_result_e (*trwifi_scan_ap)(struct netdev *dev, trwifi_scan_config_s *config);

/**
 * @brief   Scan access points with multiple access point information
 *
 * @param[in]   dev     : struct netdev registered by netdev_register()
 * @param[in]   configs : multiple access point information to scan.
 *
 * @function_type  asynchronous call : Send event by trwifi_post_event()
 * @event  LWNL_EVT_SCAN_DONE    : scan success
 * @event  LWNL_EVT_SCAN_FAILED  : scan fail
 *
 * @description If the call is successful, then it must generate
 *              events(LWNL_EVT_SCAN_DONE or LWNL_EVT_SCAN_FAILED).
 *              If the call fails then it shouldn't generate events.
 *
 * @return TRWIFI_SUCCESS      : success (should generate an event.)
 * @return TRWIFI_FAIL         : fail (shouldn't generate an event.)
 * @return TRWIFI_INVALID_ARGS : arguments are invalid
 *
 * @note   A driver must generate event if it return success.
 *         A driver can check whether access point information is set by config count in configs.
 *         If scan_ap_config_count is 0 then access point information isn't set.
 *         For other detailed settings including SCAN type, refer to the contents of trwifi_scan_ap().
 *
 */
typedef trwifi_result_e (*trwifi_scan_multi_aps)(struct netdev *dev, trwifi_scan_multi_configs_s *configs);

/**
 * @brief   Connect to an access point
 *
 * @param[in]   dev    : struct netdev registered by netdev_register()
 * @param[in]   config : an access point information to connect
 * @param[in]   arg    : not used.
 *
 * @function_type  asynchronous call : Send event by trwifi_post_event()
 * @event LWNL_EVT_STA_CONNECTED      : connection success
 * @event LWNL_EVT_STA_CONNECT_FAILED : connection fail
 *
 * @description if the call is successful, then it should generate
 *              events(LWNL_EVT_STA_CONNECTED or LWNL_EVT_STA_CONNECT_FAILED).
 *              if the call fails then it shouldn't generate events.
 *              In STA mode and it's connected to an access point, wi-fi library can generates
 *              LWNL_EVT_STA_DISCONNECTED event when it's disconnected to AP unexpectedly.
 *
 * @return TRWIFI_SUCCESS      : success (should generate an event)
 * @return TRWIFI_FAIL         : fail (should not generate an event)
 * @return TRWIFI_INVALID_ARGS : arguments are invalid
 */
typedef trwifi_result_e (*trwifi_connect_ap)(struct netdev *dev, trwifi_ap_config_s *config, void *arg);

/**
 * @brief   Disconnect to an access point
 *
 * @param[in]   dev    : struct netdev registered by netdev_register()
 * @param[in]   arg    : not used.
 *
 * @function_type  asynchronous call : Send event by trwifi_post_event()
 * @event LWNL_EVT_STA_DISCONNECTED : disconnected to an access point
 *
 * @description   If wi-fi library is in STA mode and it's connected to an access point,
 *                and it return success then it should generate LWNL_EVT_STA_DISCONNECTED event
 *                if an AP is disconnected.
 *
 * @return TRWIFI_SUCCESS      : success
 * @return TRWIFI_FAIL         : fail
 * @return TRWIFI_INVALID_ARGS : arguments are invalid
 */
typedef trwifi_result_e (*trwifi_disconnect_ap)(struct netdev *dev, void *arg);

/**
 * @brief   Change to STA mode
 *
 * @param[in]   dev    : struct netdev registered by netdev_register()
 *
 * @function_type  synchronous call
 *
 * @description    The call changes wi-fi to STA mode.
 *                 If wi-fi is in STA mode this API is not called, because wi-fi manager manages
 *                 Wi-Fi state.
 *
 * @note   Even though a wifi_manager requested to change to STA mode or to stop wi-fi
 *         while wi-fi driver is in softap mode and there are joined nodes,
 *         a wi-fi driver shouldn't generate LWNL_EVT_SOFTAP_STA_LEFT event.
 *
 * @return TRWIFI_SUCCESS      : success
 * @return TRWIFI_FAIL         : fail
 * @return TRWIFI_INVALID_ARGS : arguments are invalid
 */
typedef trwifi_result_e (*trwifi_start_sta)(struct netdev *dev);

/**
 * @brief   Start SoftAP mode
 *
 * @param[in]   dev    : struct netdev registered by netdev_register()
 * @param[in]   config : SoftAP configuration @ref trwifi_softap_config_s
 *
 * @function_type  synchronous call
 *
 * @description    The call changes wi-fi to SoftAP mode.
 *                 If wi-fi is in SoftAP mode this API is not called, because wi-fi manager manages Wi-Fi state.
 *                 In this state, wi-fi library can generate LWNL_EVT_SOFTAP_STA_JOINED or
 *                 LWNL_EVT_SOFTAP_STA_LEFT events.
 *                 LWNL_EVT_SOFTAP_STA_JOINED is called when a wi-fi device is connected to softAP.
 *                 LWNL_EVT_SOFTAP_STA_LEFT is called when a wi-fi device is left from softAP.
 *
 * @note   wifi_manager which manage TizenRT wi-fi calls trwifi_start_softap() if a wi-fi driver
 *         isn't connected to an AP. wifi_manager called trwifi_disconnect_ap() before it calls
 *         trwifi_start_softap() if a wi-fi driver connected to an AP. So it's guaranteed that
 *         a wi-fi driver isn't connected to an AP when wifi_manager calls the API.
 *
 * @return TRWIFI_SUCCESS      : success
 * @return TRWIFI_FAIL         : fail
 * @return TRWIFI_INVALID_ARGS : arguments are invalid
 */
typedef trwifi_result_e (*trwifi_start_softap)(struct netdev *dev, trwifi_softap_config_s *config);

/**
 * @brief   Stop to SoftAP mode
 *
 * @param[in]   dev    : struct netdev registered by netdev_register()
 *
 * @function_type  synchronous call
 *
 * @description    Stop softAP mode.
 *                 @ref trwifi_start_sta or @ref trwifi_deinit can be called after this API
 *
 * @note   A wi-fi driver shouldn't generate an events even though there is a node connected to
 *         an wi-fi driver.
 *
 * @return TRWIFI_SUCCESS      : success
 * @return TRWIFI_FAIL         : fail
 * @return TRWIFI_INVALID_ARGS : arguments are invalid
 */
typedef trwifi_result_e (*trwifi_stop_softap)(struct netdev *dev);

/**
 * @brief   Set auto-connection to wi-fi library
 *
 * @param[in]   dev    : struct netdev registered by netdev_register()
 * @param[in]   chk    : 0: disable
 *                       1: enable
 *
 * @function_type  synchronous call
 *
 * @description    enable auto-connection features in Wi-Fi library.
 *                 If it's enabled and it connected to an access point then
 *                 it should automatically connect to an access point when
 *                 it is disconnected due to an exceptional situation(e.g power off on an access point,
 *                 weak signal). when wi-fi is disconnected it should generate LWNL_EVT_STA_DISCONNECTED event.
 *                 And it generate LWNL_EVT_STA_CONNECTED event when it succeeds to reconnect to an access point.
 *                 During auto connect state, if @ref trwifi_disconnect_ap() is called then it stop the auto-connect.
 *                 otherwise if it's disabled, it should generate LWNL_EVT_STA_DISCONNECTED event.
 *                 exceptional situation means all cases except calling @ref trwifi_disconnect_ap()
 *
 * @return TRWIFI_SUCCESS      : success
 * @return TRWIFI_FAIL         : fail
 * @return TRWIFI_INVALID_ARGS : arguments are invalid
 */
typedef trwifi_result_e (*trwifi_set_autoconnect)(struct netdev *dev, uint8_t chk);

/**
 * @brief   Get wi-fi information
 *
 * @param[in]   dev    : struct netdev registered by netdev_register()
 * @param[out]  info   : wi-fi information
 *
 * @function_type  synchronous call
 *
 * @description    Refer trwifi_info. only rssi is required. Other members are not used.
 *
 * @return TRWIFI_SUCCESS      : success
 * @return TRWIFI_FAIL         : fail
 * @return TRWIFI_INVALID_ARGS : arguments are invalid
 */
typedef trwifi_result_e (*trwifi_get_info)(struct netdev *dev, trwifi_info *info);

/**
 * @brief   Set driver speicific operation
 *
 * @param[in]   dev    : struct netdev registered by netdev_register()
 * @param[in]   cmd    : driver specific command
 * @param[in]   arg    : driver specific argument for command
 *
 * @function_type  synchronous call
 *
 * @description    Set driver specific operation. it's decided by a vendor.
 *                 If arg is not null arg is freed by upper layer. So a wi-fi driver shouldn't
 *                 free it.
 *
 * @return TRWIFI_SUCCESS      : success
 * @return TRWIFI_FAIL         : fail
 * @return TRWIFI_INVALID_ARGS : arguments are invalid
 * @return TRWIFI_MODE_NOT_SUPPORTED: operation is not supported
 */
typedef trwifi_result_e (*trwifi_drv_ioctl)(struct netdev *dev, trwifi_msg_s *msg);

/**
 * @brief   Set channel plan
 *
 * @param[in]   dev    : struct netdev registered by netdev_register()
 * @param[in]   cmd    : driver specific command
 * @param[in]   arg    : driver specific argument for command
 *
 * @function_type  synchronous call
 *
 * @description    Set channel plan. It's decided by a user.
 *                 The usable channels will be decided by this channel plan.
 *
 * @return TRWIFI_SUCCESS      : success
 * @return TRWIFI_FAIL         : fail
 * @return TRWIFI_INVALID_ARGS : arguments are invalid
 */
typedef trwifi_result_e (*trwifi_set_channel_plan)(struct netdev *dev, uint8_t chplan);

typedef trwifi_result_e (*trwifi_get_signal_quality)(struct netdev *dev, trwifi_signal_quality *signal_quality);

typedef trwifi_result_e (*trwifi_get_deauth_reason)(struct netdev *dev, int *deauth_reason);

typedef trwifi_result_e (*trwifi_get_driver_info)(struct netdev *dev, trwifi_driver_info *driver_info);

typedef trwifi_result_e (*trwifi_get_wpa_supplicant_state)(struct netdev *dev, trwifi_wpa_states *wpa_supplicant_state);

struct trwifi_ops {
	trwifi_init init;
	trwifi_deinit deinit;
	trwifi_scan_ap scan_ap;
	trwifi_connect_ap connect_ap;
	trwifi_disconnect_ap disconnect_ap;
	trwifi_get_info get_info;
	trwifi_start_sta start_sta;
	trwifi_start_softap start_softap;
	trwifi_stop_softap stop_softap;
	trwifi_set_autoconnect set_autoconnect;
	trwifi_drv_ioctl drv_ioctl;
	trwifi_scan_multi_aps scan_multi_aps;
	trwifi_set_channel_plan set_channel_plan;
	trwifi_get_signal_quality get_signal_quality;
	trwifi_get_deauth_reason get_deauth_reason;
	trwifi_get_driver_info get_driver_info;
	trwifi_get_wpa_supplicant_state get_wpa_supplicant_state;
};

int trwifi_serialize_scaninfo(uint8_t **buffer, trwifi_scan_list_s *scan_list);
int trwifi_post_event(struct netdev *dev, lwnl_cb_wifi evt, void *buffer, uint32_t buf_len);
