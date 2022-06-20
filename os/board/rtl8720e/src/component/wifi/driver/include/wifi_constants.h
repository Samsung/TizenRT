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
 ******************************************************************************
  * @file    wifi_constants.h
  * @author
  * @version
  * @brief   This file provides the data types used for wlan API.
  ******************************************************************************
  */

#ifndef _WIFI_CONSTANTS_H
#define _WIFI_CONSTANTS_H

/** @addtogroup nic NIC
 *  @ingroup    wlan
 *  @brief      NIC functions
 *  @{
 */

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef WLAN0_NAME
#define WLAN0_NAME		"wlan0"
#endif
#ifndef WLAN1_NAME
#define WLAN1_NAME		"wlan1"
#endif

#ifndef WLAN0_IDX
#define WLAN0_IDX	0
#endif
#ifndef WLAN1_IDX
#define WLAN1_IDX	1
#endif

#define WEP_ENABLED        0x0001
#define TKIP_ENABLED       0x0002
#define AES_ENABLED        0x0004
#define WSEC_SWFLAG        0x0008
#define AES_CMAC_ENABLED        0x0010
#define ENTERPRISE_ENABLED			0x0020

#define SHARED_ENABLED  0x00008000
#define WPA_SECURITY    0x00200000
#define WPA2_SECURITY   0x00400000
#define WPA3_SECURITY		0x00800000
#define WPS_ENABLED     0x10000000

#define RTW_MAX_PSK_LEN		(64)
#define RTW_MIN_PSK_LEN		(8)

#define MCSSET_LEN			16

#define MAC_ARG(x)		((u8*)(x))[0],((u8*)(x))[1],\
				((u8*)(x))[2],((u8*)(x))[3],\
				((u8*)(x))[4],((u8*)(x))[5]

/**
  * @brief  The enumeration lists the results of the function.
  */
enum {
	RTW_SUCCESS                      = 0,    /**< Success */
	RTW_PENDING                      = 1,    /**< Pending */
	RTW_TIMEOUT                      = 2,    /**< Timeout */
	RTW_PARTIAL_RESULTS              = 3,    /**< Partial results */
	RTW_INVALID_KEY                  = 4,    /**< Invalid key */
	RTW_DOES_NOT_EXIST               = 5,    /**< Does not exist */
	RTW_NOT_AUTHENTICATED            = 6,    /**< Not authenticated */
	RTW_NOT_KEYED                    = 7,    /**< Not keyed */
	RTW_IOCTL_FAIL                   = 8,    /**< IOCTL fail */
	RTW_BUFFER_UNAVAILABLE_TEMPORARY = 9,    /**< Buffer unavailable temporarily */
	RTW_BUFFER_UNAVAILABLE_PERMANENT = 10,   /**< Buffer unavailable permanently */
	RTW_WPS_PBC_OVERLAP              = 11,   /**< WPS PBC overlap */
	RTW_CONNECTION_LOST              = 12,   /**< Connection lost */

	RTW_ERROR                        = -1,   /**< Generic Error */
	RTW_BADARG                       = -2,   /**< Bad Argument */
	RTW_BADOPTION                    = -3,   /**< Bad option */
	RTW_NOTUP                        = -4,   /**< Not up */
	RTW_NOTDOWN                      = -5,   /**< Not down */
	RTW_NOTAP                        = -6,   /**< Not AP */
	RTW_NOTSTA                       = -7,   /**< Not STA  */
	RTW_BADKEYIDX                    = -8,   /**< BAD Key Index */
	RTW_RADIOOFF                     = -9,   /**< Radio Off */
	RTW_NOTBANDLOCKED                = -10,  /**< Not  band locked */
	RTW_NOCLK                        = -11,  /**< No Clock */
	RTW_BADRATESET                   = -12,  /**< BAD Rate valueset */
	RTW_BADBAND                      = -13,  /**< BAD Band */
	RTW_BUFTOOSHORT                  = -14,  /**< Buffer too short */
	RTW_BUFTOOLONG                   = -15,  /**< Buffer too long */
	RTW_BUSY                         = -16,  /**< Busy */
	RTW_NOTASSOCIATED                = -17,  /**< Not Associated */
	RTW_BADSSIDLEN                   = -18,  /**< Bad SSID len */
	RTW_OUTOFRANGECHAN               = -19,  /**< Out of Range Channel */
	RTW_BADCHAN                      = -20,  /**< Bad Channel */
	RTW_BADADDR                      = -21,  /**< Bad Address */
	RTW_NORESOURCE                   = -22,  /**< Not Enough Resources */
	RTW_UNSUPPORTED                  = -23,  /**< Unsupported */
	RTW_BADLEN                       = -24,  /**< Bad length */
	RTW_NOTREADY                     = -25,  /**< Not Ready */
	RTW_EPERM                        = -26,  /**< Not Permitted */
	RTW_NOMEM                        = -27,  /**< No Memory */
	RTW_ASSOCIATED                   = -28,  /**< Associated */
	RTW_RANGE                        = -29,  /**< Not In Range */
	RTW_NOTFOUND                     = -30,  /**< Not Found */
	RTW_WME_NOT_ENABLED              = -31,  /**< WME Not Enabled */
	RTW_TSPEC_NOTFOUND               = -32,  /**< TSPEC Not Found */
	RTW_ACM_NOTSUPPORTED             = -33,  /**< ACM Not Supported */
	RTW_NOT_WME_ASSOCIATION          = -34,  /**< Not WME Association */
	RTW_SDIO_ERROR                   = -35,  /**< SDIO Bus Error */
	RTW_WLAN_DOWN                    = -36,  /**< WLAN Not Accessible */
	RTW_BAD_VERSION                  = -37,  /**< Incorrect version */
	RTW_TXFAIL                       = -38,  /**< TX failure */
	RTW_RXFAIL                       = -39,  /**< RX failure */
	RTW_NODEVICE                     = -40,  /**< Device not present */
	RTW_UNFINISHED                   = -41,  /**< To be finished */
	RTW_NONRESIDENT                  = -42,  /**< access to nonresident overlay */
	RTW_DISABLED                     = -43   /**< Disabled in this build */
};
typedef signed long rtw_result_t;

/**
  * @brief  The enumeration lists the possible security types to set when connection.\n
  *			Station mode supports OPEN, WEP, and WPA2.\n
  *			AP mode support OPEN and WPA2.
  */
enum {
	RTW_SECURITY_OPEN           = 0,                                                /**< Open security                           */
	RTW_SECURITY_WEP_PSK        = WEP_ENABLED,                                      /**< WEP Security with open authentication   */
	RTW_SECURITY_WEP_SHARED     = (WEP_ENABLED | SHARED_ENABLED),                   /**< WEP Security with shared authentication */
	RTW_SECURITY_WPA_TKIP_PSK   = (WPA_SECURITY  | TKIP_ENABLED),                   /**< WPA Security with TKIP                  */
	RTW_SECURITY_WPA_AES_PSK    = (WPA_SECURITY  | AES_ENABLED),                    /**< WPA Security with AES                   */
	RTW_SECURITY_WPA_MIXED_PSK  = ( WPA_SECURITY | AES_ENABLED | TKIP_ENABLED ),    /**< WPA Security with AES & TKIP            */
	RTW_SECURITY_WPA2_AES_PSK   = (WPA2_SECURITY | AES_ENABLED),                    /**< WPA2 Security with AES                  */
	RTW_SECURITY_WPA2_TKIP_PSK  = (WPA2_SECURITY | TKIP_ENABLED),                   /**< WPA2 Security with TKIP                 */
	RTW_SECURITY_WPA2_MIXED_PSK = (WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED),     /**< WPA2 Security with AES & TKIP           */
	RTW_SECURITY_WPA_WPA2_TKIP_PSK = ( WPA_SECURITY  | WPA2_SECURITY | TKIP_ENABLED), /**< WPA/WPA2 Security with TKIP           */
	RTW_SECURITY_WPA_WPA2_AES_PSK = ( WPA_SECURITY  | WPA2_SECURITY | AES_ENABLED),   /**< WPA/WPA2 Security with AES            */
	RTW_SECURITY_WPA_WPA2_MIXED_PSK = ( WPA_SECURITY  | WPA2_SECURITY | TKIP_ENABLED | AES_ENABLED), /**< WPA/WPA2 Security with AES & TKIP      */
	RTW_SECURITY_WPA_WPA2_MIXED = (WPA_SECURITY  | WPA2_SECURITY),                  /**< WPA/WPA2 Security                       */
	RTW_SECURITY_WPA2_AES_CMAC = (WPA2_SECURITY | AES_CMAC_ENABLED),                /**< WPA2 Security with AES and Management Frame Protection*/
	RTW_SECURITY_WPA2_ENTERPRISE = (WPA2_SECURITY | ENTERPRISE_ENABLED),            /**< WPA2 Security with 802.1X authentication>*/
	RTW_SECURITY_WPA_WPA2_ENTERPRISE = (WPA_SECURITY | WPA2_SECURITY | ENTERPRISE_ENABLED), /** <WPA/WPA2 Security with 802.1X authentication>*/

	RTW_SECURITY_WPS_OPEN       = WPS_ENABLED,                                      /**< WPS with open security                  */
	RTW_SECURITY_WPS_SECURE     = (WPS_ENABLED | AES_ENABLED),                      /**< WPS with AES security                   */

	RTW_SECURITY_WPA3_AES_PSK 	= (WPA3_SECURITY | AES_ENABLED),                    /**< WPA3-SAE with AES security              */
	RTW_SECURITY_WPA2_WPA3_MIXED = (WPA2_SECURITY | WPA3_SECURITY | AES_ENABLED),   /**< WPA3-SAE/WPA2 with AES security         */

	RTW_SECURITY_UNKNOWN        = -1,                                               /**< May be returned by scan function if security is unknown. Do not pass this to the join function! */

	RTW_SECURITY_FORCE_32_BIT   = 0x7fffffff                                        /**< Exists only to force rtw_security_t type to 32 bits */
};
typedef unsigned long rtw_security_t;

enum {
	RTW_ENCRYPTION_UNKNOWN = 0,
	RTW_ENCRYPTION_OPEN = 1,
	RTW_ENCRYPTION_WEP40 = 2,
	RTW_ENCRYPTION_WPA_TKIP = 3,
	RTW_ENCRYPTION_WPA_AES = 4,
	RTW_ENCRYPTION_WPA2_TKIP = 5,
	RTW_ENCRYPTION_WPA2_AES = 6,
	RTW_ENCRYPTION_WPA2_MIXED = 7,
	RTW_ENCRYPTION_WEP104 = 9,
	RTW_ENCRYPTION_UNDEF = 0xFF,
};
typedef unsigned long rtw_encryption_t;

enum {
	RTW_FALSE = 0,
	RTW_TRUE  = 1
};
typedef unsigned long rtw_bool_t;

/**
  * @brief  The enumeration lists the band types.
  */
enum {
	RTW_802_11_BAND_5GHZ   = 0, /**< Denotes 5GHz radio band   */
	RTW_802_11_BAND_2_4GHZ = 1  /**< Denotes 2.4GHz radio band */
};
typedef unsigned long rtw_802_11_band_t;

/**
  * @brief  The enumeration lists all the country codes able to set to Wi-Fi driver.
  */
enum {
	/* CHANNEL PLAN */
	RTW_COUNTRY_WORLD1,	// 0x20
	RTW_COUNTRY_ETSI1,	// 0x21
	RTW_COUNTRY_FCC1,	// 0x22
	RTW_COUNTRY_MKK1,	// 0x23
	RTW_COUNTRY_ETSI2,	// 0x24
	RTW_COUNTRY_FCC2,	// 0x2A
	RTW_COUNTRY_WORLD2,	// 0x47
	RTW_COUNTRY_MKK2,	// 0x58
	RTW_COUNTRY_GLOBAL,	// 0x41

	/* SPECIAL */
	RTW_COUNTRY_WORLD,	// WORLD1
	RTW_COUNTRY_EU,		// ETSI1

	/* JAPANESE */
	RTW_COUNTRY_JP,		// MKK1

	/* FCC , 19 countries*/
	RTW_COUNTRY_AS,		// FCC2
	RTW_COUNTRY_BM,
	RTW_COUNTRY_CA,
	RTW_COUNTRY_DM,
	RTW_COUNTRY_DO,
	RTW_COUNTRY_FM,
	RTW_COUNTRY_GD,
	RTW_COUNTRY_GT,
	RTW_COUNTRY_GU,
	RTW_COUNTRY_HT,
	RTW_COUNTRY_MH,
	RTW_COUNTRY_MP,
	RTW_COUNTRY_NI,
	RTW_COUNTRY_PA,
	RTW_COUNTRY_PR,
	RTW_COUNTRY_PW,
	RTW_COUNTRY_TW,
	RTW_COUNTRY_US,
	RTW_COUNTRY_VI,

	/* others,  ETSI */
	RTW_COUNTRY_AD,		// ETSI1
	RTW_COUNTRY_AE,
	RTW_COUNTRY_AF,
	RTW_COUNTRY_AI,
	RTW_COUNTRY_AL,
	RTW_COUNTRY_AM,
	RTW_COUNTRY_AN,
	RTW_COUNTRY_AR,
	RTW_COUNTRY_AT,
	RTW_COUNTRY_AU,
	RTW_COUNTRY_AW,
	RTW_COUNTRY_AZ,
	RTW_COUNTRY_BA,
	RTW_COUNTRY_BB,
	RTW_COUNTRY_BD,
	RTW_COUNTRY_BE,
	RTW_COUNTRY_BF,
	RTW_COUNTRY_BG,
	RTW_COUNTRY_BH,
	RTW_COUNTRY_BL,
	RTW_COUNTRY_BN,
	RTW_COUNTRY_BO,
	RTW_COUNTRY_BR,
	RTW_COUNTRY_BS,
	RTW_COUNTRY_BT,
	RTW_COUNTRY_BY,
	RTW_COUNTRY_BZ,
	RTW_COUNTRY_CF,
	RTW_COUNTRY_CH,
	RTW_COUNTRY_CI,
	RTW_COUNTRY_CL,
	RTW_COUNTRY_CN,
	RTW_COUNTRY_CO,
	RTW_COUNTRY_CR,
	RTW_COUNTRY_CX,
	RTW_COUNTRY_CY,
	RTW_COUNTRY_CZ,
	RTW_COUNTRY_DE,
	RTW_COUNTRY_DK,
	RTW_COUNTRY_DZ,
	RTW_COUNTRY_EC,
	RTW_COUNTRY_EE,
	RTW_COUNTRY_EG,
	RTW_COUNTRY_ES,
	RTW_COUNTRY_ET,
	RTW_COUNTRY_FI,
	RTW_COUNTRY_FR,
	RTW_COUNTRY_GB,
	RTW_COUNTRY_GE,
	RTW_COUNTRY_GF,
	RTW_COUNTRY_GH,
	RTW_COUNTRY_GL,
	RTW_COUNTRY_GP,
	RTW_COUNTRY_GR,
	RTW_COUNTRY_GY,
	RTW_COUNTRY_HK,
	RTW_COUNTRY_HN,
	RTW_COUNTRY_HR,
	RTW_COUNTRY_HU,
	RTW_COUNTRY_ID,
	RTW_COUNTRY_IE,
	RTW_COUNTRY_IL,
	RTW_COUNTRY_IN,
	RTW_COUNTRY_IQ,
	RTW_COUNTRY_IR,
	RTW_COUNTRY_IS,
	RTW_COUNTRY_IT,
	RTW_COUNTRY_JM,
	RTW_COUNTRY_JO,
	RTW_COUNTRY_KE,
	RTW_COUNTRY_KH,
	RTW_COUNTRY_KN,
	RTW_COUNTRY_KP,
	RTW_COUNTRY_KR,
	RTW_COUNTRY_KW,
	RTW_COUNTRY_KY,
	RTW_COUNTRY_KZ,
	RTW_COUNTRY_LA,
	RTW_COUNTRY_LB,
	RTW_COUNTRY_LC,
	RTW_COUNTRY_LI,
	RTW_COUNTRY_LK,
	RTW_COUNTRY_LR,
	RTW_COUNTRY_LS,
	RTW_COUNTRY_LT,
	RTW_COUNTRY_LU,
	RTW_COUNTRY_LV,
	RTW_COUNTRY_MA,
	RTW_COUNTRY_MC,
	RTW_COUNTRY_MD,
	RTW_COUNTRY_ME,
	RTW_COUNTRY_MF,
	RTW_COUNTRY_MK,
	RTW_COUNTRY_MN,
	RTW_COUNTRY_MO,
	RTW_COUNTRY_MQ,
	RTW_COUNTRY_MR,
	RTW_COUNTRY_MT,
	RTW_COUNTRY_MU,
	RTW_COUNTRY_MV,
	RTW_COUNTRY_MW,
	RTW_COUNTRY_MX,
	RTW_COUNTRY_MY,
	RTW_COUNTRY_NG,
	RTW_COUNTRY_NL,
	RTW_COUNTRY_NO,
	RTW_COUNTRY_NP,
	RTW_COUNTRY_NZ,
	RTW_COUNTRY_OM,
	RTW_COUNTRY_PE,
	RTW_COUNTRY_PF,
	RTW_COUNTRY_PG,
	RTW_COUNTRY_PH,
	RTW_COUNTRY_PK,
	RTW_COUNTRY_PL,
	RTW_COUNTRY_PM,
	RTW_COUNTRY_PT,
	RTW_COUNTRY_PY,
	RTW_COUNTRY_QA,
	RTW_COUNTRY_RS,
	RTW_COUNTRY_RU,
	RTW_COUNTRY_RW,
	RTW_COUNTRY_SA,
	RTW_COUNTRY_SE,
	RTW_COUNTRY_SG,
	RTW_COUNTRY_SI,
	RTW_COUNTRY_SK,
	RTW_COUNTRY_SN,
	RTW_COUNTRY_SR,
	RTW_COUNTRY_SV,
	RTW_COUNTRY_SY,
	RTW_COUNTRY_TC,
	RTW_COUNTRY_TD,
	RTW_COUNTRY_TG,
	RTW_COUNTRY_TH,
	RTW_COUNTRY_TN,
	RTW_COUNTRY_TR,
	RTW_COUNTRY_TT,
	RTW_COUNTRY_TZ,
	RTW_COUNTRY_UA,
	RTW_COUNTRY_UG,
	RTW_COUNTRY_UY,
	RTW_COUNTRY_UZ,
	RTW_COUNTRY_VC,
	RTW_COUNTRY_VE,
	RTW_COUNTRY_VN,
	RTW_COUNTRY_VU,
	RTW_COUNTRY_WF,
	RTW_COUNTRY_WS,
	RTW_COUNTRY_YE,
	RTW_COUNTRY_YT,
	RTW_COUNTRY_ZA,
	RTW_COUNTRY_ZW,

	RTW_COUNTRY_MAX

};
typedef unsigned long rtw_country_code_t;

/**
  * @brief  The enumeration lists the trp_tis types.
  */
enum {
	RTW_TRP_TIS_DISABLE = 0,
	RTW_TRP_TIS_NORMAL = 1,
	RTW_TRP_TIS_DYNAMIC = 3,				// enable dynamic mechanism
	RTW_TRP_TIS_FIX_ACK_RATE = 5,			// fix ack rate to 6M
	RTW_TRP_TIS_FIX_PHY_ACK_HIGH_RATE = 9,		// fix phy ack rate to RATE_54M | RATE_48M | RATE_36M | RATE_24M | RATE_18M | RATE_12M | RATE_9M | RATE_6M
};
typedef unsigned long rtw_trp_tis_mode_t;

/**
  * @brief  The enumeration lists the supported operation mode by WIFI driver,
  *			including station and AP mode.
  */
enum {
	RTW_MODE_NONE = 0,
	RTW_MODE_STA,
	RTW_MODE_AP,
	RTW_MODE_STA_AP,
	RTW_MODE_PROMISC,
	RTW_MODE_P2P
};
typedef unsigned long rtw_mode_t;

enum {
	RTW_SCAN_FULL = 0,
	RTW_SCAN_SOCIAL,
	RTW_SCAN_ONE
};
typedef unsigned long rtw_scan_mode_t;

/**
  * @brief  The enumeration lists the supported autoreconnect mode by WIFI driver.
  */
typedef enum {
	RTW_AUTORECONNECT_DISABLE,
	RTW_AUTORECONNECT_FINITE,
	RTW_AUTORECONNECT_INFINITE
} rtw_autoreconnect_mode_t;

/**
  * @brief  The enumeration lists the status to describe the connection link.
  */
enum {
	RTW_LINK_DISCONNECTED = 0,
	RTW_LINK_CONNECTED
};
typedef unsigned long rtw_link_status_t;


/**
  * @brief  The enumeration lists the bss types.
  */
enum {
	RTW_BSS_TYPE_INFRASTRUCTURE = 0, /**< Denotes infrastructure network                  */
	RTW_BSS_TYPE_ADHOC          = 1, /**< Denotes an 802.11 ad-hoc IBSS network           */
	RTW_BSS_TYPE_ANY            = 2, /**< Denotes either infrastructure or ad-hoc network */

	RTW_BSS_TYPE_UNKNOWN        = -1 /**< May be returned by scan function if BSS type is unknown. Do not pass this to the Join function */
};
typedef unsigned long rtw_bss_type_t;

/**
  * @brief  The enumeration lists the scan options.
  */
enum {
	RTW_SCAN_ACTIVE              = 0x01,
	RTW_SCAN_PASSIVE             = 0x02,
	RTW_SCAN_NO_HIDDEN_SSID      = 0x04, /**< Filter hidden ssid APs*/
	RTW_SCAN_REPORT_EACH         = 0x08,
};
typedef unsigned int rtw_scan_option_t;

enum {
	COMMAND1					= 0x01
};
typedef unsigned long rtw_command_type;

enum {
	RTW_WPS_TYPE_DEFAULT 		    	= 0x0000,
	RTW_WPS_TYPE_USER_SPECIFIED 		= 0x0001,
	RTW_WPS_TYPE_MACHINE_SPECIFIED   	= 0x0002,
	RTW_WPS_TYPE_REKEY 			        = 0x0003,
	RTW_WPS_TYPE_PUSHBUTTON 		    = 0x0004,
	RTW_WPS_TYPE_REGISTRAR_SPECIFIED 	= 0x0005,
	RTW_WPS_TYPE_NONE                   = 0x0006,
	RTW_WPS_TYPE_WSC                    = 0x0007
};
typedef unsigned long rtw_wps_type_t;

/**
  * @brief  The enumeration lists all the network bgn mode.
  */
enum {
	RTW_NETWORK_B   = 1,
	RTW_NETWORK_BG  = 3,
	RTW_NETWORK_BGN = 11
};
typedef unsigned long rtw_network_mode_t;

/**
  * @brief  The enumeration lists the interfaces.
  */
enum {
	RTW_STA_INTERFACE     = 0, /**< STA or Client Interface  */
	RTW_AP_INTERFACE      = 1, /**< SoftAP Interface         */
};
typedef unsigned long rtw_interface_t;

/**
  * @brief  The enumeration lists the packet filter rules.
  */
enum {
	RTW_POSITIVE_MATCHING  = 0, /**< Receive the data matching with this pattern and discard the other data   */
	RTW_NEGATIVE_MATCHING  = 1  /**< Discard the data matching with this pattern and receive the other data */
};
typedef unsigned long rtw_packet_filter_rule_t;

/**
  * @brief  The enumeration lists the promisc levels.
  */
enum {
	RTW_PROMISC_DISABLE = 0,  /**< Disable the promisc */
	RTW_PROMISC_ENABLE = 1,   /**< Fetch all ethernet packets */
	RTW_PROMISC_ENABLE_1 = 2, /**< Fetch only B/M packets */
	RTW_PROMISC_ENABLE_2 = 3, /**< Fetch all 802.11 packets*/
	RTW_PROMISC_ENABLE_3 = 4, /**< Fetch only B/M 802.11 packets*/
	RTW_PROMISC_ENABLE_4 = 5, /**< Fetch all 802.11 packets & MIMO PLCP headers. Please note that the PLCP header would be struct rtw_rx_info_t defined in wifi_structures.h*/
	RTW_PROMISC_ENABLE_5 = 6, /**< Fetch all unicast 802.11 packets, data frame and beacon in same BSSID*/
};
typedef unsigned long rtw_rcr_level_t;

/**
  * @brief  The enumeration lists the promisc rx type.
  */
#if defined(CONFIG_UNSUPPORT_PLCPHDR_RPT) && CONFIG_UNSUPPORT_PLCPHDR_RPT
enum {
	RTW_RX_NORMAL = 0,  /**< The supported 802.11 packet*/
	RTW_RX_UNSUPPORT = 1,  /**<  Unsupported 802.11 packet info */
};
typedef unsigned long rtw_rx_type_t;
#endif

enum {
	RTW_JOINSTATUS_UNKNOWN = 0,
	RTW_JOINSTATUS_STARTING,
	RTW_JOINSTATUS_SCANNING,
	RTW_JOINSTATUS_AUTHENTICATING,
	RTW_JOINSTATUS_AUTHENTICATED,
	RTW_JOINSTATUS_ASSOCIATING,
	RTW_JOINSTATUS_ASSOCIATED,
	RTW_JOINSTATUS_4WAY_HANDSHAKING,
	RTW_JOINSTATUS_4WAY_HANDSHAKE_DONE,
	RTW_JOINSTATUS_SUCCESS,
	RTW_JOINSTATUS_FAIL,
	RTW_JOINSTATUS_DISCONNECT,
	//TODO: RTW_JOINSTATUS_ABORTED,
};
typedef unsigned int rtw_join_status_t;

/**
  * @brief  The enumeration lists the disconnect reasons.
  */
enum {
	RTW_NO_ERROR,
	RTW_NONE_NETWORK,
	RTW_AUTH_FAIL,
	RTW_ASSOC_FAIL,
	RTW_WRONG_PASSWORD,
	RTW_4WAY_HANDSHAKE_TIMEOUT,
	RTW_CONNECT_FAIL,
	RTW_DHCP_FAIL,
	RTW_UNKNOWN,
};
typedef unsigned long rtw_connect_error_flag_t;

enum {
	RTW_CH_SWITCH_FAIL = -1,
	RTW_CH_SWITCH_SUCCESS = 0,
};
typedef signed int rtw_channel_switch_res_t;

enum {
	RTW_TX_PWR_PERCENTAGE_100 = 0, /* 100%, default target output power.	 */
	RTW_TX_PWR_PERCENTAGE_75 = 1, /* 75% */
	RTW_TX_PWR_PERCENTAGE_50 = 2, /* 50% */
	RTW_TX_PWR_PERCENTAGE_25 = 3, /* 25% */
	RTW_TX_PWR_PERCENTAGE_12_5 = 4, /* 12.5% */
};
typedef unsigned long rtw_tx_pwr_percentage_t;

enum {
	RTW_PM_OPTION_IPS_MODE,
	RTW_PM_OPTION_LPS_MODE,
	RTW_PM_OPTION_TDMA_PARAM,
};
typedef unsigned char rtw_pm_option_t;

enum {
	RTW_WOWLAN_CTRL,
	RTW_WOWLAN_SET_PATTREN,
	RTW_WOWLAN_REDOWNLOAD_FW,
};
typedef unsigned char rtw_wowlan_option_t;

/**
  * @brief  The enumeration is event type indicated from wlan driver.
  */
enum _WIFI_EVENT_INDICATE {
	/* common event */
	WIFI_EVENT_STA_ASSOC = 0,		//used in p2p, simple config, 11s, customer
	WIFI_EVENT_STA_DISASSOC,		//used in p2p, customer
	WIFI_EVENT_RX_MGNT,				//used in p2p, customer
	WIFI_EVENT_CONNECT,				//used in ipv6 example, p2p, wifi_manager example
	WIFI_EVENT_DISCONNECT,			//used in wifi_manager example, p2p, eap, atcmd……

	WIFI_EVENT_GROUP_KEY_CHANGED,
	WIFI_EVENT_RECONNECTION_FAIL,
	WIFI_EVENT_ICV_ERROR,
	WIFI_EVENT_CHALLENGE_FAIL,

	WIFI_EVENT_JOIN_STATUS,			//internally use for joinstatus indicate

	/* p2p event */
	WIFI_EVENT_P2P_SEND_ACTION_DONE = 15,

	/* wpa event */
	WIFI_EVENT_WPA_STA_WPS_START = 20,
	WIFI_EVENT_WPA_WPS_FINISH,
	WIFI_EVENT_WPA_EAPOL_START,
	WIFI_EVENT_WPA_EAPOL_RECVD,

	/* 11s event */
	WIFI_EVENT_11S_PATHSEL_GEN_RREQ = 59,
	WIFI_EVENT_11S_PATHSEL_GEN_RERR,
	WIFI_EVENT_11S_PATHSEL_RECV_RREQ,
	WIFI_EVENT_11S_PATHSEL_RECV_RREP,
	WIFI_EVENT_11S_PATHSEL_RECV_RERR,
	WIFI_EVENT_11S_PATHSEL_RECV_PANN,
	WIFI_EVENT_11S_PATHSEL_RECV_RANN,

	WIFI_EVENT_11S_PATHSEL_GEN_PREQ = 150,
	WIFI_EVENT_11S_PATHSEL_GEN_PERR,
	WIFI_EVENT_11S_PATHSEL_RECV_PREQ,
	WIFI_EVENT_11S_PATHSEL_RECV_PREP,
	WIFI_EVENT_11S_PATHSEL_RECV_PERR,
	WIFI_EVENT_11S_PATHSEL_RECV_GANN,

	/* csi rx done event */
	WIFI_EVENT_CSI_DONE = 160,

	WIFI_EVENT_MAX,
};
typedef unsigned long rtw_event_indicate_t;

/* power save*/
enum lps_level {
	LPS_NORMAL = 0,
	LPS_LCLK,
	LPS_PG,
	LPS_LEVEL_MAX,
};

enum Power_Mgnt {
	PS_MODE_ACTIVE	= 0	,
	PS_MODE_MIN		,
	PS_MODE_MAX		,
	PS_MODE_DTIM		,
	PS_MODE_VOIP		,
	PS_MODE_UAPSD_WMM	,
	PS_MODE_UAPSD		,
	PS_MODE_IBSS		,
	PS_MODE_WWLAN		,
	PS_MODE_RESUME		,
	PM_Radio_Off		,
	PM_Card_Disable		,
	PS_MODE_NUM
};

enum LPS_mode { // for lps_mode
	LPS_MODE_NONE = 0,
	LPS_MODE_NORMAL,
	LPS_MODE_RESUME,
	LPS_MODE_NUM
};

enum IPS_mode { // for ips_mode
	IPS_MODE_NONE = 0,
	IPS_MODE_NORMAL,
	IPS_MODE_RESUME,
	IPS_MODE_NUM
};

/* BIT 7 HT Rate*/
enum MGN_RATE {
	MGN_1M		= 0x02,
	MGN_2M		= 0x04,
	MGN_5_5M	= 0x0B,
	MGN_6M		= 0x0C,
	MGN_9M		= 0x12,
	MGN_11M 	= 0x16,
	MGN_12M = 0x18,
	MGN_18M = 0x24,
	MGN_24M = 0x30,
	MGN_36M = 0x48,
	MGN_48M = 0x60,
	MGN_54M = 0x6C,
	MGN_MCS32	= 0x7F,
	MGN_MCS0,
	MGN_MCS1,
	MGN_MCS2,
	MGN_MCS3,
	MGN_MCS4,
	MGN_MCS5,
	MGN_MCS6,
	MGN_MCS7,
	MGN_MCS8,
	MGN_MCS9,
	MGN_MCS10,
	MGN_MCS11,
	MGN_MCS12,
	MGN_MCS13,
	MGN_MCS14,
	MGN_MCS15,
	MGN_MCS16,
	MGN_MCS17,
	MGN_MCS18,
	MGN_MCS19,
	MGN_MCS20,
	MGN_MCS21,
	MGN_MCS22,
	MGN_MCS23,
	MGN_MCS24,
	MGN_MCS25,
	MGN_MCS26,
	MGN_MCS27,
	MGN_MCS28,
	MGN_MCS29,
	MGN_MCS30,
	MGN_MCS31,
	MGN_VHT1SS_MCS0,
	MGN_VHT1SS_MCS1,
	MGN_VHT1SS_MCS2,
	MGN_VHT1SS_MCS3,
	MGN_VHT1SS_MCS4,
	MGN_VHT1SS_MCS5,
	MGN_VHT1SS_MCS6,
	MGN_VHT1SS_MCS7,
	MGN_VHT1SS_MCS8,
	MGN_VHT1SS_MCS9,
	MGN_VHT2SS_MCS0,
	MGN_VHT2SS_MCS1,
	MGN_VHT2SS_MCS2,
	MGN_VHT2SS_MCS3,
	MGN_VHT2SS_MCS4,
	MGN_VHT2SS_MCS5,
	MGN_VHT2SS_MCS6,
	MGN_VHT2SS_MCS7,
	MGN_VHT2SS_MCS8,
	MGN_VHT2SS_MCS9,
	MGN_VHT3SS_MCS0,
	MGN_VHT3SS_MCS1,
	MGN_VHT3SS_MCS2,
	MGN_VHT3SS_MCS3,
	MGN_VHT3SS_MCS4,
	MGN_VHT3SS_MCS5,
	MGN_VHT3SS_MCS6,
	MGN_VHT3SS_MCS7,
	MGN_VHT3SS_MCS8,
	MGN_VHT3SS_MCS9,
	MGN_VHT4SS_MCS0,
	MGN_VHT4SS_MCS1,
	MGN_VHT4SS_MCS2,
	MGN_VHT4SS_MCS3,
	MGN_VHT4SS_MCS4,
	MGN_VHT4SS_MCS5,
	MGN_VHT4SS_MCS6,
	MGN_VHT4SS_MCS7,
	MGN_VHT4SS_MCS8,
	MGN_VHT4SS_MCS9,
	MGN_HE1SS_MCS0 = 0xd0,
	MGN_HE1SS_MCS1,
	MGN_HE1SS_MCS2,
	MGN_HE1SS_MCS3,
	MGN_HE1SS_MCS4,
	MGN_HE1SS_MCS5,
	MGN_HE1SS_MCS6,
	MGN_HE1SS_MCS7,
	MGN_HE1SS_MCS8,
	MGN_HE1SS_MCS9,
	MGN_HE1SS_MCS10,
	MGN_HE1SS_MCS11,
	MGN_HE2SS_MCS0,
	MGN_HE2SS_MCS1,
	MGN_HE2SS_MCS2,
	MGN_HE2SS_MCS3,
	MGN_HE2SS_MCS4,
	MGN_HE2SS_MCS5,
	MGN_HE2SS_MCS6,
	MGN_HE2SS_MCS7,
	MGN_HE2SS_MCS8,
	MGN_HE2SS_MCS9,
	MGN_HE2SS_MCS10,
	MGN_HE2SS_MCS11,
	MGN_HE3SS_MCS0,
	MGN_HE3SS_MCS1,
	MGN_HE3SS_MCS2,
	MGN_HE3SS_MCS3,
	MGN_HE3SS_MCS4,
	MGN_HE3SS_MCS5,
	MGN_HE3SS_MCS6,
	MGN_HE3SS_MCS7,
	MGN_HE3SS_MCS8,
	MGN_HE3SS_MCS9,
	MGN_HE3SS_MCS10,
	MGN_HE3SS_MCS11,
	MGN_HE4SS_MCS0,
	MGN_HE4SS_MCS1,
	MGN_HE4SS_MCS2,
	MGN_HE4SS_MCS3,
	MGN_HE4SS_MCS4,
	MGN_HE4SS_MCS5,
	MGN_HE4SS_MCS6,
	MGN_HE4SS_MCS7,
	MGN_HE4SS_MCS8,
	MGN_HE4SS_MCS9,
	MGN_HE4SS_MCS10,
	MGN_HE4SS_MCS11 = 0xff,
	MGN_UNKNOWN
};

enum rtw_csi_action {
	CSI_ACT_EN,  /* enable or disable csi func */
	CSI_ACT_CFG,  /* config csi parameters */
	CSI_ACT_MAX
};

enum rtw_csi_group_num {
	CSI_GROUP_NUM_1 = 0,  /* per tone */
	CSI_GROUP_NUM_2,  /* per 2tone */
	CSI_GROUP_NUM_4,  /* per 4tone */
	CSI_GROUP_NUM_16,  /* per 16tone */
	CSI_GROUP_NUM_MAX
};

enum rtw_csi_mode {
	CSI_MODE_NORMAL = 0,
	CSI_MODE_NDP,
	CSI_MODE_RX_RESP,
	CSI_MODE_MAX,
};

enum rtw_csi_accuracy {
	CSI_ACCU_1BYTE = 0,  /* CSI_ACCU_1BYTE: S(8,4) */
	CSI_ACCU_2BYTES,  /* CSI_ACCU_2BYTE: S(16,12) */
	CSI_ACCU_MAX,
};

#ifdef	__cplusplus
}
#endif

/*\@}*/

#endif /* _WIFI_CONSTANTS_H */
