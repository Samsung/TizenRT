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
 ******************************************************************************/

/**
  ******************************************************************************
  * @file    wifi_constants.h
  * @author
  * @version
  * @brief   This file provides the data types used for wlan API.
  ******************************************************************************
  */

#ifndef _WIFI_CONSTANTS_H
#define _WIFI_CONSTANTS_H

/** @defgroup WIFI
 *  @brief      WIFI module
 *  @{
 */

#ifdef	__cplusplus
extern "C" {
#endif

/** @defgroup WIFI_Exported_Constants WIFI Exported Constants
  * @{
  */


/** @defgroup WLAN_Defs
   *@{
   */
#ifndef WLAN0_NAME
#define WLAN0_NAME		"wlan0"    /**< WLAN0 NAME */
#endif

#ifndef WLAN1_NAME
#define WLAN1_NAME		"wlan1"    /**< WLAN1 NAME */
#endif

#ifndef WLAN0_IDX
#define WLAN0_IDX	0  /**< wlan0 index */
#endif

#ifndef WLAN1_IDX
#define WLAN1_IDX	1  /**< wlan1 index */
#endif

#ifndef STA_WLAN_INDEX
#define STA_WLAN_INDEX	0
#endif

#ifndef SOFTAP_WLAN_INDEX
#define SOFTAP_WLAN_INDEX	1
#endif

#ifndef STA_WLAN_NAME
#define STA_WLAN_NAME	"wlan0"
#endif

#ifndef SOFTAP_WLAN_NAME
#define SOFTAP_WLAN_NAME	"wlan1"
#endif


/** @} */


/** @addtogroup MAC_Defs
   *@{
   */
/**
 * @brief  mac address format.
 */
#define MAC_ARG(x)		((u8*)(x))[0],((u8*)(x))[1],\
				((u8*)(x))[2],((u8*)(x))[3],\
				((u8*)(x))[4],((u8*)(x))[5]
/** @} */


/** @defgroup Security_Defs
   *@{
   */
#define WEP_ENABLED         0x0001      /**< wep enable */
#define TKIP_ENABLED       0x0002      /**< tkip enable */
#define AES_ENABLED        0x0004      /**< aes enable */
#define WSEC_SWFLAG        0x0008    /**< WSEC SWFLAG */
#define AES_CMAC_ENABLED        0x0010   /**< aes cmac enable */
#define ENTERPRISE_ENABLED			0x0020   /**< enterprise enable */
#define SHARED_ENABLED  0x00008000     /**< shared enable */
#define WPA_SECURITY    0x00200000      /**< wpa */
#define WPA2_SECURITY   0x00400000     /**< wpa2 */
#define WPA3_SECURITY		0x00800000  /**< wpa3 */
#define WPS_ENABLED     0x10000000      /**< wps  enable*/
#define RTW_MAX_PSK_LEN		(64) /**< maxmum psk length */
#define RTW_MIN_PSK_LEN		(8)   /**< minimum psk length */
#define MCSSET_LEN			16      /**<mcsset length */
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup WIFI_Exported_Types WIFI Exported Types
  * @{
  */

/** @defgroup Enums
   *@{
   */

/**
 * @brief The enumeration lists the results of the function.
 */
typedef enum {
	RTW_SUCCESS                      = 0,    /**< Success */
	RTW_PENDING                      = 1,    /**< Pending */
	RTW_TIMEOUT                      = 2,    /**< Timeout */
	RTW_PARTIAL_RESULTS              = 3,    /**< Partial results */
	RTW_INVALID_KEY                  = 4,        /**< Invalid key */
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
	RTW_NOTDOWN                      = -5,  /**< Not down */
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
} rtw_result_t;

/**
 * @brief The enumeration lists the possible security types to set when connection.\n
 *			Station mode supports OPEN, WEP, and WPA2.\n
 *			AP mode support OPEN and WPA2.
 */
typedef enum {
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
} rtw_security_t;


/**
 * @brief The enumeration lists wpa mode
 */
typedef enum {
	WPA_AUTO_MODE,
	WPA_ONLY_MODE,
	WPA2_ONLY_MODE,
	WPA3_ONLY_MODE,
	WPA_WPA2_MIXED_MODE,
	WPA2_WPA3_MIXED_MODE
} rtw_wpa_mode;

/**
 * @brief The enumeration lists encryption types
 */
typedef enum {
	RTW_ENCRYPTION_UNKNOWN = 0,    /**< unknown encryption type*/
	RTW_ENCRYPTION_OPEN = 1,           /**< open encryption type*/
	RTW_ENCRYPTION_WEP40 = 2,         /**< WEP40 encryption type*/
	RTW_ENCRYPTION_WPA_TKIP = 3,   /**< WPA+TKIP encryption type*/
	RTW_ENCRYPTION_WPA_AES = 4,    /**< WPA+AES encryption type*/
	RTW_ENCRYPTION_WPA2_TKIP = 5,/**< WPA2+TKIP encryption type*/
	RTW_ENCRYPTION_WPA2_AES = 6,  /**< WPA2+AES encryption type*/
	RTW_ENCRYPTION_WPA2_MIXED = 7,  /**< WPA2+MIXED encryption type*/
	RTW_ENCRYPTION_WEP104 = 9,          /**< WEP104 encryption type*/
	RTW_ENCRYPTION_UNDEF = 0xFF    /**< undefined encryption type*/
} rtw_encryption_t;


/**
 * @brief The enumeration lists false and true
 */
typedef enum {
	RTW_FALSE = 0,   /**< false*/
	RTW_TRUE  = 1    /**< true*/
} rtw_bool_t;

/**
* @brief The enumeration lists band types
*/
typedef enum {
	RTW_802_11_BAND_5GHZ   = 0, /**< Denotes 5GHz radio band */
	RTW_802_11_BAND_2_4GHZ = 1  /**< Denotes 2.4GHz radio band */
} rtw_802_11_band_t;

/**
* @brief The enumeration lists all the country codes able to set to Wi-Fi driver.
*/
typedef enum {
	/* CHANNEL PLAN */
	RTW_COUNTRY_WORLD1,	///< 0x20
	RTW_COUNTRY_ETSI1,	///< 0x21
	RTW_COUNTRY_FCC1,	///< 0x22
	RTW_COUNTRY_MKK1,	///< 0x23
	RTW_COUNTRY_ETSI2,	///<0x24
	RTW_COUNTRY_FCC2,	///< 0x2A
	RTW_COUNTRY_WORLD2,	///< 0x47
	RTW_COUNTRY_MKK2,	///< 0x58
	RTW_COUNTRY_GLOBAL,	///< 0x41

	/* SPECIAL */
	RTW_COUNTRY_WORLD,	///< WORLD1
	RTW_COUNTRY_EU,		///< ETSI1

	/* JAPANESE */
	RTW_COUNTRY_JP,		///< MKK1

	/* FCC , 19 countries*/
	RTW_COUNTRY_AS,		///< FCC2
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
	RTW_COUNTRY_AD,		///< ETSI1
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

} rtw_country_code_t;

/**
* @brief The enumeration lists the trp tis types.
*/
typedef enum {
	RTW_TRP_TIS_DISABLE = 0,
	RTW_TRP_TIS_NORMAL = 1,
	RTW_TRP_TIS_DYNAMIC = 3,				///< enable dynamic mechanism
	RTW_TRP_TIS_FIX_ACK_RATE = 5,			///< fix ack rate to 6M
	RTW_TRP_TIS_FIX_PHY_ACK_HIGH_RATE = 9		///< fix phy ack rate to RATE_54M | RATE_48M | RATE_36M | RATE_24M | RATE_18M | RATE_12M | RATE_9M | RATE_6M
} rtw_trp_tis_mode_t;

/**
  * @brief The enumeration lists the supported operation mode by WIFI driver,
  *			including station and AP mode.
  */
typedef enum {
	RTW_MODE_NONE = 0,  ///<none
	RTW_MODE_STA,          ///<sta mode
	RTW_MODE_AP,           ///<ap mode
	RTW_MODE_STA_AP,   ///< ap and sta mode
	RTW_MODE_PROMISC,///<promisc mode
	RTW_MODE_P2P         ///<p2p mode
} rtw_mode_t;

/**
  * @brief The enumeration lists the scan status
  */
typedef enum {
	RTW_SCAN_FULL = 0,   ///< scan full  mode
	RTW_SCAN_SOCIAL,    ///< scan social  mode
	RTW_SCAN_ONE         ///< scan one mode
} rtw_scan_mode_t;

/**
  * @brief  The enumeration lists the supported autoreconnect mode by WIFI driver.
  */
typedef enum {
	RTW_AUTORECONNECT_DISABLE,  ///< disable auto reconnect
	RTW_AUTORECONNECT_FINITE,    ///< finite auto reconnect
	RTW_AUTORECONNECT_INFINITE ///< infinite auto reconnect
} rtw_autoreconnect_mode_t;

/**
  * @brief  The enumeration lists the status to describe the connection link.
  */
typedef enum {
	RTW_LINK_DISCONNECTED = 0, ///< disconnect status
	RTW_LINK_CONNECTED             ///< connect status
} rtw_link_status_t;


/**
  * @brief  The enumeration lists the bss types.
  */
typedef enum {
	RTW_BSS_TYPE_INFRASTRUCTURE = 0, /**< Denotes infrastructure network                  */
	RTW_BSS_TYPE_ADHOC          = 1, /**< Denotes an 802.11 ad-hoc IBSS network           */
	RTW_BSS_TYPE_ANY            = 2, /**< Denotes either infrastructure or ad-hoc network */

	RTW_BSS_TYPE_UNKNOWN        = -1 /**< May be returned by scan function if BSS type is unknown. Do not pass this to the Join function */
} rtw_bss_type_t;

/**
  * @brief  The enumeration lists the scan options.
  */
typedef enum {
	RTW_SCAN_ACTIVE              = 0x01,     /**< active scan */
	RTW_SCAN_PASSIVE             = 0x02,    /**< passive scan*/
	RTW_SCAN_NO_HIDDEN_SSID      = 0x04, /**< Filter hidden ssid APs*/
	RTW_SCAN_REPORT_EACH         = 0x08    /**< report each */
} rtw_scan_option_t;

/**
  * @brief  The enumeration lists the command type.
  */
typedef enum {
	COMMAND1					= 0x01   /**< COMMAND1 */
} rtw_command_type;

/**
  * @brief  The enumeration lists the WPS types.
  */
typedef enum {
	RTW_WPS_TYPE_DEFAULT 		    	= 0x0000,   /**< default type */
	RTW_WPS_TYPE_USER_SPECIFIED 		= 0x0001,  /**< user specified type */
	RTW_WPS_TYPE_MACHINE_SPECIFIED   	= 0x0002,  /**< machine specified type */
	RTW_WPS_TYPE_REKEY 			        = 0x0003,       /**< retry key type */
	RTW_WPS_TYPE_PUSHBUTTON 		    = 0x0004,    /**< push button type */
	RTW_WPS_TYPE_REGISTRAR_SPECIFIED 	= 0x0005,  /**< register specified type */
	RTW_WPS_TYPE_NONE                   = 0x0006,              /**< none */
	RTW_WPS_TYPE_WSC                    = 0x0007                /**< wsc type */
} rtw_wps_type_t;


/**
  * @brief  The enumeration lists all the network bgn mode.
  */
typedef enum {
	RTW_NETWORK_B   = 1,    /**< b mode */
	RTW_NETWORK_A   = 2,    /**< a mode */
	RTW_NETWORK_BG  = 3,   /**< bg mode */
	RTW_NETWORK_A_N   = 10,    /**< a+n mode */
	RTW_NETWORK_BGN = 13, /**< bgn mode */
	RTW_NETWORK_A_N_AC   = 26,    /**< a+n+ac mode */
	RTW_NETWORK_BGN_AX = 45, /**< bgn+ax mode */
	RTW_NETWORK_A_N_AC_AX   = 58    /**< a+n+ac+ax mode */
} rtw_network_mode_t;

/**
  * @brief  The enumeration lists the interfaces.
  */
typedef enum {
	RTW_STA_INTERFACE     = 0, /**<  STA or Client Interface  */
	RTW_AP_INTERFACE      = 1 /**<  SoftAP Interface         */
} rtw_interface_t;

/**
  * @brief  The enumeration lists the packet filter rules.
  */
typedef enum {
	RTW_POSITIVE_MATCHING  = 0, /**< Receive the data matching with this pattern and discard the other data   */
	RTW_NEGATIVE_MATCHING  = 1  /**< Discard the data matching with this pattern and receive the other data */
} rtw_packet_filter_rule_t;

/**
  * @brief  The enumeration lists the promisc levels.
  */
typedef enum {
	RTW_PROMISC_DISABLE = 0,  /**< Disable the promisc */
	RTW_PROMISC_ENABLE = 1,   /**< Fetch all ethernet packets */
	RTW_PROMISC_ENABLE_1 = 2, /**< Fetch only B/M packets */
	RTW_PROMISC_ENABLE_2 = 3, /**< Fetch all 802.11 packets*/
	RTW_PROMISC_ENABLE_3 = 4, /**< Fetch only B/M 802.11 packets*/
	RTW_PROMISC_ENABLE_4 = 5, /**< Fetch all 802.11 packets & MIMO PLCP headers. Please note that the PLCP header would be struct rtw_rx_info_t defined in wifi_structures.h*/
	RTW_PROMISC_ENABLE_5 = 6 /**< Fetch all unicast 802.11 packets, data frame and beacon in same BSSID*/
} rtw_rcr_level_t;

#if (defined(CONFIG_UNSUPPORT_PLCPHDR_RPT) && CONFIG_UNSUPPORT_PLCPHDR_RPT) || defined __DOXYGEN__
/**
 * @brief The enumeration lists the promisc rx type.
 */
typedef enum {
	RTW_RX_NORMAL = 0,  /**< The supported 802.11 packet*/
	RTW_RX_UNSUPPORT = 1 /**<  Unsupported 802.11 packet info */
} rtw_rx_type_t;
#endif

/**
  * @brief The enumeration lists the join status.
  */
typedef enum {
	RTW_JOINSTATUS_UNKNOWN = 0,    /**< unknown */
	RTW_JOINSTATUS_STARTING,          /**< starting phase */
	RTW_JOINSTATUS_SCANNING,          /**< scanning phase */
	RTW_JOINSTATUS_AUTHENTICATING,  /**< authenticating phase */
	RTW_JOINSTATUS_AUTHENTICATED,   /**< authenticated phase */
	RTW_JOINSTATUS_ASSOCIATING,       /**< associating phase */
	RTW_JOINSTATUS_ASSOCIATED,         /**< associated phase */
	RTW_JOINSTATUS_4WAY_HANDSHAKING, /**< 4 way handshaking phase */
	RTW_JOINSTATUS_4WAY_HANDSHAKE_DONE, /**< 4 way handshake done phase */
	RTW_JOINSTATUS_SUCCESS,                        /**< join success  */
	RTW_JOINSTATUS_FAIL,                               /**< join fail  */
	RTW_JOINSTATUS_DISCONNECT,                 /**< disconnect */
	//TODO: RTW_JOINSTATUS_ABORTED,
} rtw_join_status_t;

/**
  * @brief  The enumeration lists the disconnect reasons.
  */
typedef enum {
	RTW_NO_ERROR,        /**< no error */
	RTW_NONE_NETWORK,   /**< none network */
	RTW_AUTH_FAIL,            /**< auth fail */
	RTW_ASSOC_FAIL,          /**< assocation fail */
	RTW_WRONG_PASSWORD, /**< wrong password */
	RTW_4WAY_HANDSHAKE_TIMEOUT, /**< 4 way handshake timeout*/
	RTW_CONNECT_FAIL,  /**< connect fail*/
	RTW_DHCP_FAIL,        /**< dhcp fail*/
	RTW_UNKNOWN,         /**< unknown*/
} rtw_connect_error_flag_t;

/**
  * @brief channel switch
  */
typedef enum {
	RTW_CH_SWITCH_FAIL = -1,           ///<channel switch fail
	RTW_CH_SWITCH_SUCCESS = 0,      ///<channel switch success
} rtw_channel_switch_res_t;

/**
  * @brief The enumeration lists the power status.
  */
typedef enum {
	RTW_TX_PWR_PERCENTAGE_100 = 0, /**< 100%, default target output power.	 */
	RTW_TX_PWR_PERCENTAGE_75 = 1, /**< 75% */
	RTW_TX_PWR_PERCENTAGE_50 = 2, /**< 50% */
	RTW_TX_PWR_PERCENTAGE_25 = 3, /**< 25% */
	RTW_TX_PWR_PERCENTAGE_12_5 = 4, /**< 12.5% */
} rtw_tx_pwr_percentage_t;

/**
  * @brief pm option
  */
typedef enum {
	RTW_PM_OPTION_IPS_MODE,       ///< ips mode
	RTW_PM_OPTION_LPS_MODE,      ///< lps mode
	RTW_PM_OPTION_TDMA_PARAM,///<tdma param mode
} rtw_pm_option_t;

/**
  * @brief wowlan option
  */
typedef enum {
	RTW_WOWLAN_CTRL,            ///< control mode
	RTW_WOWLAN_SET_PATTREN,///< set pattren mode
	RTW_WOWLAN_REDOWNLOAD_FW,///< redownload fw
} rtw_wowlan_option_t;

/**
  * @brief  The enumeration is event type indicated from wlan driver.
  */
typedef enum {
	/* common event */
	WIFI_EVENT_STA_ASSOC = 0,		///<used in p2p, simple config, 11s, customer
	WIFI_EVENT_STA_DISASSOC,		///<used in p2p, customer
	WIFI_EVENT_RX_MGNT,				///<used in p2p, customer
	WIFI_EVENT_CONNECT,				///<used in ipv6 example, p2p, wifi_manager example
	WIFI_EVENT_DISCONNECT,			///<used in wifi_manager example, p2p, eap

	WIFI_EVENT_GROUP_KEY_CHANGED,
	WIFI_EVENT_RECONNECTION_FAIL,
	WIFI_EVENT_ICV_ERROR,
	WIFI_EVENT_CHALLENGE_FAIL,

	WIFI_EVENT_JOIN_STATUS,			///<internally use for joinstatus indicate

	/* p2p event */
	WIFI_EVENT_P2P_SEND_ACTION_DONE,

	/* wpa event */
	WIFI_EVENT_WPA_STA_WPS_START,
	WIFI_EVENT_WPA_WPS_FINISH,
	WIFI_EVENT_WPA_EAPOL_START,
	WIFI_EVENT_WPA_EAPOL_RECVD,

	/* csi rx done event */
	WIFI_EVENT_CSI_DONE,

	WIFI_EVENT_MAX,
} rtw_event_indicate_t;

/**
  * @brief Power Mgnt
  */
typedef enum {
	PS_MODE_ACTIVE	= 0	, ///< active mode
	PS_MODE_MIN		,       ///< min mode
	PS_MODE_MAX		,       ///< max mode
	PS_MODE_DTIM		,      ///<dtim mode
	PS_MODE_VOIP		,      ///< voip mode
	PS_MODE_UAPSD_WMM	, ///< uapsd wmm mode
	PS_MODE_UAPSD		,       ///< uapsd mode mode
	PS_MODE_IBSS		,       ///< ibss mode
	PS_MODE_WWLAN		,///< wwlan mode
	PS_MODE_RESUME		,///< resume mode
	PM_Radio_Off		,      ///< radio off mode
	PM_Card_Disable		,      ///<card disable
	PS_MODE_NUM                 ///< mode nums
} Power_Mgnt;

/**
  * The enumeration lists the power save status.
  */
typedef enum {
	LPS_NORMAL = 0, ///< normal
	LPS_LCLK,          ///< lclk
	LPS_PG,             ///<pg
	LPS_LEVEL_MAX,
} lps_level;


/**
  * @brief The enumeration lists the BIT 7 HT Rate.
  */
typedef enum {
	MGN_1M		= 0x02,     /**< 0x02 */
	MGN_2M		= 0x04,     /**< 0x04 */
	MGN_5_5M	= 0x0B,     /**< 0x0B */
	MGN_6M		= 0x0C,     /**< 0x0C */
	MGN_9M		= 0x12,     /**< 0x12 */
	MGN_11M 	= 0x16,     /**< 0x16 */
	MGN_12M = 0x18,      /**< 0x18 */
	MGN_18M = 0x24,      /**< 0x24 */
	MGN_24M = 0x30,      /**< 0x30 */
	MGN_36M = 0x48,      /**< 0x48 */
	MGN_48M = 0x60,      /**< 0x60 */
	MGN_54M = 0x6C,      /**< 0x6C */
	MGN_MCS32	= 0x7F,  /**< 0x7f */
	MGN_MCS0,   /**< 0x80 */
	MGN_MCS1,   /**< 0x81 */
	MGN_MCS2,   /**< 0x82 */
	MGN_MCS3,   /**< 0x83 */
	MGN_MCS4,   /**< 0x84 */
	MGN_MCS5,   /**< 0x85 */
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
	MGN_HE1SS_MCS0 = 0xd0,   /**< 0xd0 */
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
	MGN_HE4SS_MCS11 = 0xff,  /**< 0xff */
	MGN_UNKNOWN
} MGN_RATE;

/**
  * @brief The enumeration lists the BIT 7 HT Rate.
  */
typedef enum {
	CSI_ACT_EN,    /**< enable or disable csi func */
	CSI_ACT_CFG,  /**< config csi parameters */
	CSI_ACT_MAX
} rtw_csi_action;

/**
  * @brief csi group num
  */
typedef enum {
	CSI_GROUP_NUM_1 = 0,  /**< per tone */
	CSI_GROUP_NUM_2,         /**< per 2tone */
	CSI_GROUP_NUM_4,        /**< per 4tone */
	CSI_GROUP_NUM_16,     /**< per 16tone */
	CSI_GROUP_NUM_MAX
} rtw_csi_group_num;

/**
  * @brief csi mode
  */
typedef enum {
	CSI_MODE_NORMAL = 0,   ///<   normal mode
	CSI_MODE_NDP,                ///<   ndp mode
	CSI_MODE_RX_RESP,         ///<    rx rsp mode
	CSI_MODE_MAX,                 ///<    max mode
} rtw_csi_mode;

/**
  * @brief The enumeration lists the BIT 7 HT Rate.
  */
typedef enum {
	CSI_ACCU_1BYTE = 0, /**< CSI_ACCU_1BYTE: S(8,4) */
	CSI_ACCU_2BYTES,  /**< CSI_ACCU_2BYTE: S(16,12) */
	CSI_ACCU_MAX
} rtw_csi_accuracy;

/**
  * @}
  */

/**
  * @}
  */

#ifdef	__cplusplus
}
#endif


/**
  * @}
  */
#endif /* _WIFI_CONSTANTS_H */
