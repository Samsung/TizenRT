// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <common/sys_config.h>
#include <stdint.h>
#include <stdbool.h>
#include <common/bk_err.h>
#include <common/bk_include.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief WiFi APIs Version 2 (New WiFi API)
 * @addtogroup bk_api_wifi_v2 New WiFi API group
 * @{
 */

/**
 * @brief WiFi enum
 * @defgroup bk_api_wifi_v2_enum enum
 * @ingroup bk_api_wifi_v2
 * @{
 */

#define BK_ERR_WIFI_NOT_INIT            (BK_ERR_WIFI_BASE - 1) /**< WiFi is not initialized, call bk_wifi_init() to init the WiFi */
#define BK_ERR_WIFI_STA_NOT_STARTED     (BK_ERR_WIFI_BASE - 2) /**< STA is not started, call bk_wifi_sta_start() to start the STA */
#define BK_ERR_WIFI_AP_NOT_STARTED      (BK_ERR_WIFI_BASE - 3) /**< AP is not initialized, call bk_wifi_ap_start() to start the AP */
#define BK_ERR_WIFI_CHAN_RANGE          (BK_ERR_WIFI_BASE - 4) /**< Invalid channel range */
#define BK_ERR_WIFI_COUNTRY_POLICY      (BK_ERR_WIFI_BASE - 5) /**< Invalid country policy */
#define BK_ERR_WIFI_RESERVED_FIELD      (BK_ERR_WIFI_BASE - 6) /**< Reserved fields not 0 */
#define BK_ERR_WIFI_MONITOR_IP          (BK_ERR_WIFI_BASE - 7) /**< Monitor is in progress */
#define BK_ERR_WIFI_STA_NOT_CONFIG      (BK_ERR_WIFI_BASE - 8) /**< STA is not configured, call bk_wifi_sta_config() to configure it */
#define BK_ERR_WIFI_AP_NOT_CONFIG       (BK_ERR_WIFI_BASE - 9) /**< AP is not configured, call bk_wifi_ap_config() to configure it */
#define BK_ERR_WIFI_DRIVER              (BK_ERR_WIFI_BASE - 10) /**< Internal WiFi driver error */
#define BK_ERR_WIFI_MONITOR_ENTER       (BK_ERR_WIFI_BASE - 11) /**< WiFi failed to enter monitor mode */
#define BK_ERR_WIFI_DRIVER_DEL_VIF      (BK_ERR_WIFI_BASE - 12) /**< WiFi driver failed to delete WiFi virtual interface */
#define BK_ERR_WIFI_DRIVER_AP_START     (BK_ERR_WIFI_BASE - 13) /**< WiFi driver failed to start BK AP */
#define BK_ERR_WIFI_CHAN_NUMBER         (BK_ERR_WIFI_BASE - 14) /**< Invalid channel number */

#define WIFI_MIN_CHAN_NUM    1      /**< Minimum supported channel number */
#define WIFI_MAX_CHAN_NUM    14     /**< Maximum supported channel number */
#if CONFIG_WIFI_BAND_5G
#define WIFI_5G_MIN_CHAN_NUM    36      /**< Minimum supported channel number */
#define WIFI_5G_MAX_CHAN_NUM    177     /**< Maximum supported channel number */
#endif

#define WIFI_SCAN_MAX_SSID_NUM   6    /**< Maximum number of ssid when scan */
#define WIFI_MAX_SCAN_CHAN_DUR    200    /**< scan duration param, need less than 200ms */
#define WIFI_CHANNEL_NUM_2G4    14    /**< Maximum supported 2.4G channel number */
#define WIFI_CHANNEL_NUM_5G      28    /**< Maximum supported 5G channel numbe*/
#define WIFI_2BAND_MAX_CHAN_NUM    (WIFI_CHANNEL_NUM_2G4 + WIFI_CHANNEL_NUM_5G)

#define WIFI_SSID_STR_LEN    (32+1) /**< Maximum **NULL-terminated** WiFi SSID length */
#define WIFI_BSSID_LEN       6      /**< Length of BSSID */
#define WIFI_MAC_LEN         6      /**< Length of MAC */

#define WIFI_PASSWORD_LEN    (64+1) /**< Maximum **NULL-terminated** WiFi password length */

/**
 * @brief default init configuration
 * */
#define WIFI_DEFAULT_INIT_CONFIG() {\
	.features = 0,\
	.os_funcs = &g_wifi_os_funcs, \
	.os_val = &g_wifi_os_variable,\
	}

/**
 * @brief default STA configuration
 * */
#define WIFI_DEFAULT_STA_CONFIG() {\
	.ssid = "sta_default_ssid",\
	.bssid = {0},\
	.channel = 0,\
	.security = WIFI_SECURITY_AUTO,\
	.password = "12345678",\
	.reserved = {0},\
	}

/**
 * @brief default AP configuration
 * */
#define WIFI_DEFAULT_AP_CONFIG() {\
	.ssid = "ap_default_ssid",\
	.password = "",\
	.channel = 0,\
	.security = WIFI_SECURITY_WPA2_MIXED,\
	.hidden = 0,\
	.max_con = 0,\
	.reserved = {0},\
	}

/**
 * @brief default Scan configuration
 */
#define WIFI_DEFAULT_SCAN_CONFIG() {\
	.ssid = "scan_default_ssid",\
	.reserved = {0},\
}

/**
 * @brief default Filter configuration
 */
#define WIFI_DEFAULT_FILTER_CONFIG() {\
	.rx_all_default_mgmt = 1,\
	.rx_probe_req = 0,\
	.rx_probe_rsp = 0,\
	.rx_all_beacon = 0,\
	.rx_action = 0,\
	.reserved = 0,\
}

/**
 * @brief default FTM configuration
 */
#define WIFI_DEFAULT_FTM_CONFIG() {\
	.ftm_per_burst = 10,\
	.nb_ftm_rsp = 10,\
}

/**
 * enum ieee80211_band - supported frequency bands
 *
 * The bands are assigned this way because the supported
 * bitrates differ in these bands.
 *
 * @IEEE80211_BAND_2GHZ: 2.4GHz ISM band
 * @IEEE80211_BAND_5GHZ: around 5GHz band (4.9-5.7)
 * @IEEE80211_BAND_6GHZ: around 6GHz band (5.925-7.125)
 * @IEEE80211_BAND_60GHZ: around 60 GHz band (58.32 - 64.80 GHz)
 * @IEEE80211_NUM_BANDS: number of defined bands
 */
typedef enum {
	IEEE80211_BAND_2GHZ = 0,
	IEEE80211_BAND_5GHZ = 1,
	IEEE80211_BAND_6GHZ = 2,
	IEEE80211_BAND_60GHZ = 3,

	/* keep last */
	IEEE80211_NUM_BANDS
} wifi_band_t;

/**
 * @brief WiFi public event type
 */
typedef enum {
	EVENT_WIFI_SCAN_DONE = 0,      /**< WiFi scan done event */
	EVENT_WIFI_STA_ASSOCIATED,     /**< WiFi associated event */
	EVENT_WIFI_STA_CONNECTED,      /**< The BK STA is connected */
	EVENT_WIFI_STA_DISCONNECTED,   /**< The BK STA is disconnected */

	EVENT_WIFI_AP_CONNECTED,       /**< A STA is connected to the BK AP */
	EVENT_WIFI_AP_DISCONNECTED,    /**< A STA is disconnected from the BK AP */

	EVENT_WIFI_NETWORK_FOUND,      /**< The BK STA find target AP */
	EVENT_WIFI_BEACON_HINT,        /**< Beacon found on NO_IR channel */
	EVENT_WIFI_REGDOMAIN_CHANGED,  /**< Regulatory domain changed */

	EVENT_WIFI_COUNT,              /**< WiFi event count */
} wifi_event_t;

typedef enum {
	WIFI_REASON_RESERVED = 0,
	WIFI_REASON_UNSPECIFIED = 1,
	WIFI_REASON_PREV_AUTH_NOT_VALID = 2,
	WIFI_REASON_DEAUTH_LEAVING = 3,
	WIFI_REASON_DISASSOC_DUE_TO_INACTIVITY = 4,
	WIFI_REASON_DISASSOC_AP_BUSY = 5,
	WIFI_REASON_CLASS2_FRAME_FROM_NONAUTH_STA = 6,
	WIFI_REASON_CLASS3_FRAME_FROM_NONASSOC_STA = 7,
	WIFI_REASON_DISASSOC_STA_HAS_LEFT = 8,
	WIFI_REASON_STA_REQ_ASSOC_WITHOUT_AUTH = 9,
	WIFI_REASON_PWR_CAPABILITY_NOT_VALID = 10,
	WIFI_REASON_SUPPORTED_CHANNEL_NOT_VALID = 11,
	WIFI_REASON_BSS_TRANSITION_DISASSOC = 12,
	WIFI_REASON_INVALID_IE = 13,
	WIFI_REASON_MICHAEL_MIC_FAILURE = 14,
	WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT = 15,
	WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT = 16,
	WIFI_REASON_IE_IN_4WAY_DIFFERS = 17,
	WIFI_REASON_GROUP_CIPHER_NOT_VALID = 18,
	WIFI_REASON_PAIRWISE_CIPHER_NOT_VALID = 19,
	WIFI_REASON_AKMP_NOT_VALID = 20,
	WIFI_REASON_UNSUPPORTED_RSN_IE_VERSION = 21,
	WIFI_REASON_INVALID_RSN_IE_CAPAB = 22,
	WIFI_REASON_IEEE_802_1X_AUTH_FAILED = 23,
	WIFI_REASON_CIPHER_SUITE_REJECTED = 24,
	WIFI_REASON_TDLS_TEARDOWN_UNREACHABLE = 25,
	WIFI_REASON_TDLS_TEARDOWN_UNSPECIFIED = 26,
	WIFI_REASON_SSP_REQUESTED_DISASSOC = 27,
	WIFI_REASON_NO_SSP_ROAMING_AGREEMENT = 28,
	WIFI_REASON_BAD_CIPHER_OR_AKM = 29,
	WIFI_REASON_NOT_AUTHORIZED_THIS_LOCATION = 30,
	WIFI_REASON_SERVICE_CHANGE_PRECLUDES_TS = 31,
	WIFI_REASON_UNSPECIFIED_QOS_REASON = 32,
	WIFI_REASON_NOT_ENOUGH_BANDWIDTH = 33,
	WIFI_REASON_DISASSOC_LOW_ACK = 34,
	WIFI_REASON_EXCEEDED_TXOP = 35,
	WIFI_REASON_STA_LEAVING = 36,
	WIFI_REASON_END_TS_BA_DLS = 37,
	WIFI_REASON_UNKNOWN_TS_BA = 38,
	WIFI_REASON_TIMEOUT = 39,
	WIFI_REASON_PEERKEY_MISMATCH = 45,
	WIFI_REASON_AUTHORIZED_ACCESS_LIMIT_REACHED = 46,
	WIFI_REASON_EXTERNAL_SERVICE_REQUIREMENTS = 47,
	WIFI_REASON_INVALID_FT_ACTION_FRAME_COUNT = 48,
	WIFI_REASON_INVALID_PMKID = 49,
	WIFI_REASON_INVALID_MDE = 50,
	WIFI_REASON_INVALID_FTE = 51,
	WIFI_REASON_MESH_PEERING_CANCELLED = 52,
	WIFI_REASON_MESH_MAX_PEERS = 53,
	WIFI_REASON_MESH_CONFIG_POLICY_VIOLATION = 54,
	WIFI_REASON_MESH_CLOSE_RCVD = 55,
	WIFI_REASON_MESH_MAX_RETRIES = 56,
	WIFI_REASON_MESH_CONFIRM_TIMEOUT = 57,
	WIFI_REASON_MESH_INVALID_GTK = 58,
	WIFI_REASON_MESH_INCONSISTENT_PARAMS = 59,
	WIFI_REASON_MESH_INVALID_SECURITY_CAP = 60,
	WIFI_REASON_MESH_PATH_ERROR_NO_PROXY_INFO = 61,
	WIFI_REASON_MESH_PATH_ERROR_NO_FORWARDING_INFO = 62,
	WIFI_REASON_MESH_PATH_ERROR_DEST_UNREACHABLE = 63,
	WIFI_REASON_MAC_ADDRESS_ALREADY_EXISTS_IN_MBSS = 64,
	WIFI_REASON_MESH_CHANNEL_SWITCH_REGULATORY_REQ = 65,
	WIFI_REASON_MESH_CHANNEL_SWITCH_UNSPECIFIED = 66,

	WIFI_REASON_BEACON_LOST = 256,       /**< The BK STA can't detect the beacon of the connected AP */
	WIFI_REASON_NO_AP_FOUND = 257,       /**< Can't find the target AP */
	WIFI_REASON_WRONG_PASSWORD = 258,    /**< The password is wrong */
	WIFI_REASON_DISCONNECT_BY_APP = 259, /**< The BK STA disconnected by application */
	WIFI_REASON_DHCP_TIMEOUT = 260,      /**<The BK STA dhcp timeout, 20s**/
	WIFI_REASON_MAX,                     /**<The BK STA connect success*/
} wifi_err_reason_t;

typedef enum {
	WIFI_SECURITY_NONE,            /**< Open system. */
	WIFI_SECURITY_WEP,             /**< WEP security, **it's unsafe security, please don't use it** */
	WIFI_SECURITY_WPA_TKIP,        /**< WPA TKIP */
	WIFI_SECURITY_WPA_AES,         /**< WPA AES */
	WIFI_SECURITY_WPA_MIXED,       /**< WPA AES or TKIP */
	WIFI_SECURITY_WPA2_TKIP,       /**< WPA2 TKIP */
	WIFI_SECURITY_WPA2_AES,        /**< WPA2 AES */
	WIFI_SECURITY_WPA2_MIXED,      /**< WPA2 AES or TKIP */
	WIFI_SECURITY_WPA3_SAE,        /**< WPA3 SAE */
	WIFI_SECURITY_WPA3_WPA2_MIXED, /**< WPA3 SAE or WPA2 AES */
	WIFI_SECURITY_EAP,             /**< EAP */
	WIFI_SECURITY_OWE,             /**< OWE */
	WIFI_SECURITY_AUTO,            /**< WiFi automatically detect the security type */
} wifi_security_t;

typedef enum {
	/* for STA mode */
	WIFI_LINKSTATE_STA_IDLE = 0,      /**< sta mode is idle */
	WIFI_LINKSTATE_STA_CONNECTING,    /**< sta mode is connecting */
	WIFI_LINKSTATE_STA_DISCONNECTED,  /**< sta mode is disconnected */
	WIFI_LINKSTATE_STA_CONNECTED,     /**< sta mode is connected */
	WIFI_LINKSTATE_STA_CONNECT_FAILED,     /**< sta mode is connec fail */
	WIFI_LINKSTATE_STA_GOT_IP,        /**< sta mode got ip */
	WIFI_LINKSTATE_STA_SCAN_DONE,
	/* for AP mode */
	WIFI_LINKSTATE_AP_CONNECTED,      /**< softap mode, a client association success */
	WIFI_LINKSTATE_AP_DISCONNECTED,   /**< softap mode, a client disconnect */
	WIFI_LINKSTATE_AP_CONNECT_FAILED, /**< softap mode, a client association failed */
	WIFI_LINKSTATE_MAX,               /**< reserved */
	//TODO maybe we can provide more precise link status
} wifi_link_state_t;

typedef enum {
	WIFI_COUNTRY_POLICY_AUTO,   /**< Country policy is auto, use the country info of AP to which the station is connected */
	WIFI_COUNTRY_POLICY_MANUAL, /**< Country policy is manual, always use the configured country info */
} wifi_country_policy_t;            /**< WiFi country policy */

typedef enum {
	WIFI_SECOND_CHANNEL_NONE = 0,/**< HT20 mode, no secondary channel */
	WIFI_SECOND_CHANNEL_ABOVE,   /**< Second channel is above the primary channel */
	WIFI_SECOND_CHANNEL_BELOW,   /**< Second channel is below the primary channel */
} wifi_second_channel_t;

typedef enum {
    WIFI_REGU_NULL    = 0,
    WIFI_REGU_FCC     = 1,
    WIFI_REGU_ETSI    = 2,
    WIFI_REGU_MKK     = 3,
    WIFI_REGU_EXT     = 4,
}wifi_regulation_t;
    
typedef enum {
    WIFI_RS_NULL =0,
    WIFI_RS_CCK  =1,
    WIFI_RS_OFDM =2,
    WIFI_RS_HT   =3,
    WIFI_RS_VHT  =4,
    WIFI_RS_HE   =5,
}wifi_ratesection_t;


typedef enum {
	WIFI_TX_RAW_DATA,   /**< Wi-Fi tx raw data */
	WIFI_TX_RAW_SPECIAL_DATTA, /**< Wi-Fi tx raw speacial data */
} wifi_tx_raw_pkt_t;            /**< wifi_tx_raw_pkt_t */

/**
 * @}
 */

/**
 * @brief WLAN struct  type
 * @defgroup bk_api_wifi_v2_typedef struct
 * @ingroup bk_api_wifi_v2 struct
 * @{
 */

typedef struct {
	uint64_t features;          /**< WiFi feature bitmaps */
	void *os_funcs;  /**< WiFi OS functions */
	void *os_val;		/**<WiFI Values*/
	uint8_t reserved[16];       /**< Reserved */
} wifi_init_config_t;

typedef struct {
	uint8_t primary;                 /**< Primary channel */
	wifi_second_channel_t second;    /**< Secondary cahnnel */
} wifi_channel_t;

/**
 * @brief WiFi capability interface ID
 */
typedef enum {
	WIFI_CAPA_ID_ERP_EN= 0,     /**< Update Wi-Fi 11g support capability */
	WIFI_CAPA_ID_HT_EN,         /**< Update Wi-Fi 11n support capability */
	WIFI_CAPA_ID_VHT_EN,        /**< Update Wi-Fi 11ac support capability */
	WIFI_CAPA_ID_HE_EN,         /**< Update Wi-Fi 11ax support capability */
	WIFI_CAPA_ID_TX_AMPDU_EN,   /**< Update Wi-Fi TX AMPDU support capability */
	WIFI_CAPA_ID_RX_AMPDU_EN,   /**< Update Wi-Fi RX AMPDU support capability */
	WIFI_CAPA_ID_TX_AMPDU_NUM,  /**< Update Wi-Fi TX AMPDU maximum number */
	WIFI_CAPA_ID_RX_AMPDU_NUM,  /**< Update Wi-Fi RX AMPDU maximum number */
	WIFI_CAPA_ID_VHT_MCS,       /**< Update Wi-Fi VHT MCS */
	WIFI_CAPA_ID_HE_MCS,        /**< Update Wi-Fi HE MCS */
	WIFI_CAPA_ID_B40_EN,        /**< Update Wi-Fi 40M bandwidth support capability */
	WIFI_CAPA_ID_STBC_EN,       /**< Update Wi-Fi STBC support capability */
	WIFI_CAPA_ID_SGI_EN,        /**< Update Wi-Fi Short GI support capability */
	WIFI_CAPA_ID_LDPC_EN,       /**< Update Wi-Fi LDPC support capability */
	WIFI_CAPA_ID_BEAMFORMEE_EN, /**< Update Wi-Fi Beamforming support capability */
	WIFI_CAPA_ID_11B_ONLY_EN,   /**< Update Wi-Fi 11b support capability */
	WIFI_CAPA_ID_MAX,
}wifi_capability_t;
/**
 * enum wifi_vht_mcs_support_t - VHT MCS support definitions
 * @WIFI_VHT_MCS_SUPPORT_0_7: MCSes 0-7 are supported for the number of streams
 * @WIFI_VHT_MCS_SUPPORT_0_8: MCSes 0-8 are supported
 * @WIFI_VHT_MCS_SUPPORT_0_9: MCSes 0-9 are supported
 * @WIFI_VHT_MCS_NOT_SUPPORTED: This number of streams isn't supported
 */
typedef enum {
	WIFI_VHT_MCS_SUPPORT_0_7    = 0,
	WIFI_VHT_MCS_SUPPORT_0_8    = 1,
	WIFI_VHT_MCS_SUPPORT_0_9    = 2,
	WIFI_VHT_MCS_NOT_SUPPORTED  = 3,
}wifi_vht_mcs_support_t;
/**
 * enum wifi_he_mcs_support_t - HE MCS support definitions
 * @WIFI_HE_MCS_SUPPORT_0_7: MCSes 0-7 are supported for the number of streams
 * @WIFI_HE_MCS_SUPPORT_0_9: MCSes 0-9 are supported
 * @WIFI_HE_MCS_SUPPORT_0_11: MCSes 0-11 are supported
 * @WIFI_HE_MCS_NOT_SUPPORTED: This number of streams isn't supported
 */
typedef enum {
	WIFI_HE_MCS_SUPPORT_0_7     = 0,
	WIFI_HE_MCS_SUPPORT_0_9     = 1,
	WIFI_HE_MCS_SUPPORT_0_11    = 2,
	WIFI_HE_MCS_NOT_SUPPORTED   = 3,
}wifi_he_mcs_support_t;

/**
 * enum wifi_video_quality_t - video quality definitions
 * @WIFI_VIDEO_QUALITY_FD: video quality choose Fluent definition(FD)
 * @WIFI_VIDEO_QUALITY_SD: video quality choose Standard definition(SD)
 * @WIFI_VIDEO_QUALITY_HD: video quality choose High definition(HD)
 */
typedef enum {
	WIFI_VIDEO_QUALITY_FD     = 0,
	WIFI_VIDEO_QUALITY_SD     = 1,
	WIFI_VIDEO_QUALITY_HD     = 2,
}wifi_video_quality_t;

/* wpa_vendor_elem_frame */
enum wifi_sta_vsie {
	WIFI_VENDOR_ELEM_ASSOC_REQ = 0,
	WIFI_VENDOR_ELEM_PROBE_REQ = 1,
	NUM_WIFI_VENDOR_ELEM_FRAMES
};

/* Wi-Fi debug log configiration bit */
enum wifi_pkt_dbg_bit {
	WIFI_TX_DBG_LOG_DATA_PKT = (1<<0),
	WIFI_TX_DBG_LOG_MGMT_PKT = (1<<1),
	WIFI_TX_DBG_LOG_PUSH_PKT = (1<<2),
	WIFI_RX_DBG_LOG_DATA_PKT = (1<<8),
	WIFI_RX_DBG_LOG_MGMT_PKT = (1<<9)
};

#define BK_MAX_STA_TYPE 4
#define OUI_BEKEN           0xC8478C
#define WLAN_EID_VENDOR_SPECIFIC 221
typedef enum sta_type{
	STA_TYPE1,
	STA_TYPE2,
	STA_TYPE3,
	STA_TYPE4,
	MAX_STA_TYPE = BK_MAX_STA_TYPE,
}BK_STA_TYPE;

struct bk_vise{
	uint8_t len;
	uint8_t buf[255];
};

typedef struct {
	char ssid[WIFI_SSID_STR_LEN];      /**< SSID of AP to be connected */
	uint8_t bssid[WIFI_BSSID_LEN];     /**< BSSID of AP to be connected, fast connect only */
	uint8_t channel;                   /**< Primary channel of AP to be connected, fast connect only */
	wifi_security_t security;          /**< Security of AP to be connected */
	char  password[WIFI_PASSWORD_LEN]; /**< Security key or PMK of the wlan. */
	uint8_t psk[WIFI_PASSWORD_LEN];    /**< PSK of AP */

	uint8_t ip_addr[4];                 /**< IP address */
	uint8_t netmask[4];                 /**< NET address */
	uint8_t gw[4];                      /**< GW address */
	uint8_t dns1[4];                    /**< DNS address */
	uint8_t is_not_support_auto_fci;    /**< is not support auto fast connect */
	uint8_t is_user_fast_connect;       /**< is use user fast connect */
	uint8_t pmf;                        /**< is support pmf */
	uint8_t tk[16];                     /**< WPA3 tk key */

	/* auto reconnect configuration */
	int auto_reconnect_count;          /**< auto reconnect max count, 0 for always reconnect */
	int auto_reconnect_timeout;        /**< auto reconnect timeout in secs, 0 for no timeout */
	bool disable_auto_reconnect_after_disconnect;  /**< disable auto reconnect if deauth/disassoc by AP when in connected state */

	struct bk_vise *vsies[NUM_WIFI_VENDOR_ELEM_FRAMES];
	uint8_t reserved[32];              /**< reserved, **must set to 0** */
#ifdef CONFIG_CONNECT_THROUGH_PSK_OR_SAE_PASSWORD
	uint8_t psk[65];
	uint8_t psk_len;
	bool psk_calculated;
#endif

#ifdef CONFIG_INCLUDE_WPA2_ENTERPRISE
	/* starts of WPA2-Enterprise/WPA3-Enterprise configuration */
	char eap[16];                      /**< phase1 authType: TLS/TTLS/SIM */
	char identity[32];                 /**< user identity */
	char ca[32];                       /**< CA certificate filename */
	char client_cert[32];              /**< client's Certification filename in PEM,DER format */
	char private_key[32];              /**< client's private key filename in PEM,DER format */
	char private_key_passwd[32];       /**< client's private key password */
	char phase1[32];                   /**< client's phase1 parameters */
#endif
} wifi_sta_config_t;

typedef struct {
	wifi_link_state_t state;           /**<Wifi linkstate*/
	wifi_err_reason_t reason_code;     /**<Wifi disconnect reason code, success will be WIFI_REASON_MAX*/
} wifi_linkstate_reason_t;

typedef struct {
	wifi_link_state_t state;           /**< The WiFi connection status */
	int aid;                           /**< STA AID */
	int rssi;                          /**< The RSSI of AP the BK STA is connected */
	char ssid[WIFI_SSID_STR_LEN];      /**< SSID of AP the BK STA is connected */
	uint8_t bssid[WIFI_BSSID_LEN];     /**< BSSID of AP the BK STA is connected */
	uint8_t channel;                   /**< Primary channel of AP the BK STA is connected */
	wifi_security_t security;          /**< Security of AP the BK STA is connected */
	char  password[WIFI_PASSWORD_LEN]; /**< Passord of AP the BK STA is connected */
} wifi_link_status_t;


enum {
	SCAN_TYPE_CC = 0x1,
};

typedef struct {
	char ssid[WIFI_SCAN_MAX_SSID_NUM][WIFI_SSID_STR_LEN];     /**< SSID to be scaned */
	u8 ssid_cnt;      /**< number of SSID to be scaned */
	u8 scan_type;     /**< 0: active scan; 1: passive scan*/
	u8 chan_cnt;      /**< scan channel cnt*/
	u8 flag;          /**< country code flag*/
	u8 chan_nb[WIFI_2BAND_MAX_CHAN_NUM];     /**< scan channel number 2.4g+5g*/
	u8 scan_band;     /**< scan band,2.4G total channel or 5G total channel*/
	u32 duration;     /**< scan duration,ms*/
} wifi_scan_config_t;

typedef struct {
	char ssid[WIFI_SSID_STR_LEN];      /**< SSID of AP found by scan */
	uint8_t bssid[WIFI_BSSID_LEN];     /**< BSSID of AP found by scan */
	int  rssi;                         /**< RSSI of AP found by scan */
	uint8_t channel;                   /**< The channel of the AP found by scan */
	wifi_security_t security;          /**< The security type of the AP found by scan */
	uint8_t reserved[16];              /**< Reserved, **must be zero** */
} wifi_scan_ap_info_t;

typedef struct {
	int ap_num;                        /**< The number of AP found by scan */
	wifi_scan_ap_info_t *aps;          /**< The APs found by scan */
} wifi_scan_result_t;

typedef struct {
	char ssid[WIFI_SSID_STR_LEN];      /**< The SSID of BK AP */
	char password[WIFI_PASSWORD_LEN];  /**< Password of BK AP, ignored in an open system.*/
	uint8_t channel;                   /**< The channel of BK AP, 0 indicates default TODO */
	wifi_security_t security;          /**< Security type of BK AP, default value TODO */
	uint8_t hidden: 1;                 /**< Whether the BK AP is hidden */
	uint8_t acs: 1;                    /**< Whether Auto Channel Selection is enabled */
	uint8_t vsie_len;                  /**< Beacon/ProbeResp Vendor Specific IE len */
	uint8_t vsie[255];                 /**< Beacon/ProbeResp Vendor Specific IE */
	uint8_t max_con;                   /**< Max number of stations allowed to connect to BK AP, TODO default value? */ 
	u8 max_statype_num[4];
	uint8_t reserved[32];              /**< Reserved, **must be zero** */
} wifi_ap_config_t;

/**
 * @brief Wlan station information definition
 */
typedef struct wlan_ap_sta {
	uint8_t addr[6];
	uint32_t ipaddr;
	int8_t rssi;
} wlan_ap_sta_t;

/**
 * @brief Wlan connected stations information definition
 */
typedef struct wlan_ap_stas {
	wlan_ap_sta_t *sta;
	int num;
} wlan_ap_stas_t;

/**
 * @brief softap beacon/probe response's vendor IEs
 * Dynamic change softap's vendor specific IEs.
 */
typedef struct wlan_ap_vsie {
	uint8_t len;
	uint8_t vsie[255];
} wlan_ap_vsie_t;

/**
 * @brief STA vendor IEs for (re)assoc, scan, p2p, p2p go, etc.
 * Dynamic change STA's vendor specific IEs.
 */
typedef struct wlan_sta_vsie {
	int frame;
	uint8_t len;
	uint8_t vsie[255];
} wlan_sta_vsie_t;

typedef struct {
	uint32_t rx_mcast_frame: 1;        /**< Set this bit to enable callback to receive multicast/broadcast*/
	uint32_t rx_dup_frame: 1;          /**< Set this bit to enable callback to receive the duplicated*/
	uint32_t reserved: 30;             /**< Reserved bits, **must set to 0** */
} wifi_monitor_config_t;

typedef struct {
	int rssi;                         /**< RSSI of the received frame in monitor mode */
} wifi_frame_info_t;

/**
 * @brief Monitor callback to receive frames relayed by WiFi driver.
 *
 * @attention 1. The memory of frame and frame_info will be freed immediately when the
 *               callback is returned. If we want to use the frame or frame_info in other
 *               task, make a copy of them and handle them accordingly.
 * @attention 2. Don't do too much work in the callback because the callback is called in
 *               context of WiFi core thread, too much work may impact the performance of
 *               WiFi.
 **/
typedef bk_err_t (*wifi_monitor_cb_t)(const uint8_t *frame, uint32_t len, const wifi_frame_info_t *frame_info);

typedef struct {
	uint32_t rx_all_default_mgmt: 1;  /**< Set the bit to enable the callback to receive all management frame recived by WiFi driver */
	uint32_t rx_probe_req: 1;         /**< Set the bit to enable the callback to receive probe request */
	uint32_t rx_probe_rsp: 1;         /**< Set the bit to enable the callback to receive probe response */
	uint32_t rx_all_beacon: 1;        /**< Set the bit to enable the callback to receive all beacon */
	uint32_t rx_action: 1;            /**< Set the bit to enable the callback to receive action frame */
	uint32_t reserved: 27;            /**< Reserved bits, **must set to 0** */
} wifi_filter_config_t;

/**
 * @brief Filter callback to receive frames relayed by WiFi driver.
 *
 * @attention 1. The memory of frame and frame_info will be freed immediately when the
 *               callback is returned. If we want to use the frame or frame_info in other
 *               task, make a copy of them and handle them accordingly.
 * @attention 2. Don't do too much work in the callback because the callback is called in
 *               context of WiFi core thread, too much work may impact the performance of
 *               WiFi.
 **/
typedef bk_err_t (*wifi_filter_cb_t)(const uint8_t *frame, uint32_t len, const wifi_frame_info_t *frame_info);

typedef struct {
	char                  cc[3];          /**< country code string */
	uint8_t               schan;          /**< 2.4G start channel */
	uint8_t               nchan;          /**< 2.4G scan channel number */
	int8_t                max_tx_power;   /**< maximum tx power */
#if CONFIG_WIFI_BAND_5G
	uint8_t               schan_5g[WIFI_CHANNEL_NUM_5G];          /**<Triplet fields: 5G start channel */
	uint8_t               nchan_5g[WIFI_CHANNEL_NUM_5G];          /**<Triplet fields: 5G scan number */
	int8_t                max_tx_power_5g[WIFI_CHANNEL_NUM_5G];   /**<Triplet fields: maximum tx power */
	uint8_t               triplet_cnt;          /**<Triplet fields cnt */
#endif
} wifi_country_t;

typedef struct {
	uint32_t scan_id; /**< Scan ID */
	uint32_t scan_use_time;/**< scan time. us */
} wifi_event_scan_done_t;

typedef struct {
	char    ssid[WIFI_SSID_STR_LEN];      /**< SSID found to be connected */
	uint8_t bssid[WIFI_BSSID_LEN];        /**< BSSID found to be connected */
} wifi_event_network_found_t;

typedef struct {
	char    ssid[WIFI_SSID_STR_LEN];      /**< SSID of connected AP */
	uint8_t bssid[WIFI_BSSID_LEN];        /**< BSSID of connected AP*/
} wifi_event_sta_connected_t;

typedef struct {
	int disconnect_reason;                /**< Disconnect reason of BK STA */
	bool local_generated;                 /**< if disconnect is request by local */
} wifi_event_sta_disconnected_t;

typedef struct {
	uint8_t mac[WIFI_MAC_LEN];            /**< MAC of the STA connected to the BK AP */
} wifi_event_ap_connected_t;

typedef struct {
	uint8_t mac[WIFI_MAC_LEN];            /**< MAC of the STA disconnected from the BK AP */
} wifi_event_ap_disconnected_t;

typedef struct {
	uint8_t initiator; /**< 0: core, 1: user, 2: driver, 3: dot11d */
	uint8_t type;    /**< 0: country, 1: world, 2: custom, 3: intersection */
	char alpha2[2];  /**< country code */
} wifi_event_reg_change_t;

/**
 * @brief Wi-Fi Statistic info.
 */
typedef struct {
	uint32_t tx_success_count;	/**< Number of TX successes, 0 if unavailable */
	uint32_t tx_retry_count;	/**< Number of TX retries, 0 if unavailable */
	uint32_t tx_fail_count; 	/**< Number of TX failures, 0 if unavailable */
	uint32_t rx_success_count;	/**< Number of RX successes, 0 if unavailable */
	uint32_t rx_crcerror_count; /**< Number of RX FCS errors, 0 if unavailable */
	uint32_t mic_error_count;	/**< Number of MIC errors, 0 if unavailable */
	int8_t noise;				/**< Average noise level in dBm, -128 if unavailable */
	uint16_t phyrate; 		/**< Maximum used PHY rate, 0 if unavailable */
	uint16_t txrate;			/**< Average used TX rate, 0 if unavailable */
	uint16_t rxrate;			/**< Average used RX rate, 0 if unavailable */
	int8_t rssi;				/**< Average beacon RSSI in dBm, -128 if unavailable */
	uint8_t bandwidth;		/**< Average used bandwidth, 0 if unavailable */
	uint8_t idle_time_per;		/**< Percent of idle time, 0 if unavailable */
} wifi_statistics_info_t;


/**
 * @brief Wi-Fi RAW TX control.
 */
typedef struct {
	uint32_t tx_rate;       /**< RAW Data TX rate information */
	uint32_t tx_power;      /**< RAW Data TX power information */
	uint32_t tx_retry_cnt;  /**< RAW Data TX retry count */
} raw_tx_cntrl_t;

/**
 * @brief Wi-Fi RAW TX control.
 */
typedef struct {
	uint8_t* pkt;                   /**< RAW data tx packet address */
	int      len;                   /**< RAW data tx packet length */
	int      type;                  /**< Raw data tx packet type length */
	raw_tx_cntrl_t tx_cntrl;        /**< RAW data tx control information */
} wifi_raw_tx_info_t;

/// FTM configuration and results
/**
 * @brief Wi-Fi fine timing measurement configuration.
 */
typedef struct {
	uint8_t ftm_per_burst;             /**< Number of FTMs per Burst */
	uint8_t nb_ftm_rsp;                /**< Number of FTM responders on which we do the measurements */
} wifi_ftm_config_t;

/**
 * @brief Wi-Fi fine timing measurement results.
 */
typedef struct {
	uint8_t bssid[WIFI_BSSID_LEN];     /**< MAC Address of the FTM responder */
	uint32_t rtt;                      /**< Round Trip Time (in ns) */
	float distance;                    /**< Distance (in m) */
} wifi_ftm_rsp_info_t;

typedef struct {
	uint8_t nb_ftm_rsp;                /**< Number of FTM responders for which we have a measurement */
	wifi_ftm_rsp_info_t *rsp;          /**< Per-responder measurements */
}wifi_ftm_results_t;

/// CSI configuration and results
/**
 * @brief Wi-Fi Channel State Information configuration and results.
 */
#define FILTER_SRC_MAC_MAX (8)
#define FILTER_DST_MAC_MAX (8)
typedef struct {
	bool enable; 			/**< enable or disable csi active, true: enable, false: disable */
	uint32_t interval; 		/**< interval of csi active, unit: ms */
}wifi_csi_active_mode_config_t;

typedef struct {
	uint8_t proto_type_bmp; /**< b`0001:NON_HT,b`0010:HT, b`0100:VHT, b`1000:HE */
	uint8_t cbw_bmp; /**< b`0001:20MHz, b`0010:40MHz, b`0011: 20Mhz || 40Mhz*/
	uint8_t filter_mac_addr_type; /**< 0:all mac;1:src && dst;2: dst only; 3:src only; 4:src || dst*/
	uint8_t filter_src_mac_num; /**< Number of source MAC addresses to filter */
	uint8_t filter_src_mac[WIFI_MAC_LEN * FILTER_SRC_MAC_MAX]; /**< Source MAC filter list (max 16 entries) */
	uint8_t filter_dst_mac_num; /**< Number of destination MAC addresses to filter */
	uint8_t filter_dst_mac[WIFI_MAC_LEN * FILTER_DST_MAC_MAX]; /**< Destination MAC filter list (max 8 entries) */
	uint32_t filter_mgmt_frame_bmp; /**< Bitmap for management frames to capture ,enable:0x80000000 */
	uint32_t filter_ctrl_frame_bmp; /**< Bitmap for control frames to capture ,enable:0x80000000 */
	uint32_t filter_data_frame_bmp; /**< Bitmap for data frames to capture ,enable:0x80000000 */
}wifi_csi_filter_config_t;

typedef struct {
	bool enable; 			/**< enable or disable csi pure receiving, true: enable, false: disable */
	wifi_csi_filter_config_t filter_config; /**< filter config */
}wifi_csi_receive_mode_config_t;

enum {
	CSI_RATE_6M= 0,    // 6 Mbps
    CSI_RATE_9M,       // 9 Mbps
    CSI_RATE_12M,      // 12 Mbps
    CSI_RATE_18M,      // 18 Mbps
    CSI_RATE_24M,      // 24 Mbps
    CSI_RATE_36M,      // 36 Mbps
    CSI_RATE_48M,      // 48 Mbps
    CSI_RATE_54M       // 54 Mbps
};

struct wifi_rx_pkt_t{
	uint8_t proto_type; /**< 0x0:non HT packet; 0x2:HT packet;0x4:VHT packet;0x5:HE packet */
	uint8_t band; /**< 0x0:2.4G; 0x1:5G*/
	uint8_t pri_chan; /**< primary channel on which this packet is received */
	uint8_t rate; /**< PHY rate encoding of the packet. Only valid for non-HT packet */
	uint8_t mcs; /**< Modulation Coding Scheme. Only valid for HT,VHT,HE packet*/
	uint8_t cbw; /**< Channel Bandwidth of the packet. 0: 20MHz; 1: 40MHz */
	int8_t rssi; /**< Received Signal Strength Indicator(RSSI) of packet. unit: dBm */
	uint8_t snr; /**< SNR of AGC stage */
	uint8_t agc_gain; /**< Post-AGC link gain */
	uint8_t gi_type; /**< Guide Interval(GI). */
	uint8_t stbc; /**< Space Time Block Code(STBC). 0: non STBC packet; 1: STBC packet */
	uint8_t pkt_len; /**< length of packet including Frame Check Sequence(FCS) */
	uint8_t src_mac[WIFI_MAC_LEN]; /**< Source MAC Address of the CSI info receive */
	uint8_t dst_mac[WIFI_MAC_LEN]; /**< Destination MAC Address of the CSI info receive */
	uint32_t timestamp; /**< timestamp. The local time when this packet is received. unit: microsecond */
};

struct csi_cfr_t{
	float real; /**< csi real part */
	float imag; /**< csi imaginary part*/
};

struct csi_info_t{
	uint8_t ltf_type; /**<0:invalid; 1:L-LTF; 2:HT_LTF; 3: VHT_LTF; 4: HE; 5: STBC_HT_LTF; 6: STBC_VHT_LTF; 7: STBC_HE_LTF */
	uint8_t sc_num; /**< The number of CSI subcarrier */
	struct csi_cfr_t *csi_data; /**<pointer to csi_data, data_len is sc_num */
};

#define MAX_NUM 3
struct wifi_csi_info_t{
	struct wifi_rx_pkt_t rx_ctrl; /**< rx packet ctrl info*/
	struct csi_info_t csi_info[MAX_NUM]; /**< csi data info, max is 3*/
};

typedef void (* wifi_csi_cb_t)(struct wifi_csi_info_t *info);

/**
 * @brief Wi-Fi tx statistics.
 */
struct tx_stats_t
{
    uint32_t tx_single_total;           /**< tx single mpdu total count */
    uint32_t tx_single_retry;           /**< tx single mpdu retry count */
    uint32_t tx_agg_total;              /**< tx mpdu of ampdu total count */
    uint32_t tx_agg_fail;               /**< tx mpdu of ampdu fail count */
    uint32_t tx_drop;
    uint32_t rx_drop;
    uint32_t tx_retry;
    double   max_rate;
    uint8_t  bandwidth;
    uint8_t  chan;
    uint8_t  band;
    char     snr;
};

typedef struct {
    wifi_regulation_t     regulation; /**< region regulation  0 REGUALTION_NULL 1  REGUALTION_FCC 2 REGUALTION_ETSI 3 REGUALTION_JP 4 REGUALTION_EXT*/
    wifi_ratesection_t    ratesection; /**< ratesection 0 RATE_NULL 1 RATE_CCK 2 RATE_OFDM 3 RATE_HT 4 RATE_VHT 5 RATE_HE*/
    uint8_t   channel; /**< channel  range from 1-14*/
    uint8_t   value; /**< value max tx power*/
}wifi_tx_pwr_lmt_t;

/**
 * @brief BC/MC Filter.
 */
enum
{
    WIFI_BCMC_CLOSE      = 0,
    WIFI_MC_ON,
    WIFI_BC_ON,
    WIFI_BC_MC_ON,
};

/**
 * @brief IEEE 802.11 Standards
 */
typedef enum
{
    WIFI_STANDARD_NONE = 0,
    WIFI_STANDARD_11A,
    WIFI_STANDARD_11B,
    WIFI_STANDARD_11G,
    WIFI_STANDARD_11N,
    WIFI_STANDARD_11AC,
    WIFI_STANDARD_11AX,

    WIFI_STANDARD_MAX
} wifi_standard;

typedef bk_err_t (*wifi_beacon_cc_rxed_t)(void *ctxt, uint8_t *cc, uint8_t cc_len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
