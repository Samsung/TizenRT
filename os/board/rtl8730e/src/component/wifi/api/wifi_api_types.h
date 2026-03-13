/**
  ******************************************************************************
  * @file    wifi_api_types.h
  * @author
  * @version
  * @date
  * @brief   This file provides user interface for Wi-Fi station and AP mode configuration
  *             base on the functionalities provided by Realtek Wi-Fi driver.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __WIFI_API_TYPES_H
#define __WIFI_API_TYPES_H

#ifndef CONFIG_FULLMAC
#include "ameba.h"
#include "rtw_wifi_common.h"
#include "rtw_byteorder.h"
#include "dlist.h"
#include "platform_stdlib.h"
#if !(defined(ZEPHYR_WIFI) && defined(CONFIG_WHC_HOST))
#include "os_wrapper.h"
#include "rtw_misc.h"
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup WIFI_API Wi-Fi APIs
 *  @brief      WIFI_API module
 *  @{
 */
/** @addtogroup WIFI_Exported_Constants Wi-Fi Exported Constants
 * @{
 */

#define STA_WLAN_INDEX	    0
#define SOFTAP_WLAN_INDEX	1
#define NAN_WLAN_INDEX	    2
#define NONE_WLAN_INDEX	    0xFF

/** When set to this value, a fast survey is conducted with a scan time of 25 ms on the specified channel.
 *  Otherwise, a normal scan is performed with a duration of 110 ms on the specified channel. */
#define RTW_PSCAN_FAST_SURVEY	0x02

/** Maximum number of channels in each channel plan. Defined as 14 for 2.4GHz only, or 42 when 5GHz is supported.*/
#if !defined(SUPPORT_5G_CHANNEL)
#define	RTW_MAX_CHANNEL_NUM			14
#else
#define	RTW_MAX_CHANNEL_NUM			42
#endif

/** When set to this value, disables DPK (Digital Pre-Distortion Calibration) during RF calibration.*/
#define RTW_RFK_DIS_DPK     BIT(0)

#define RTW_ESSID_MAX_SIZE	32  /**< Maximum length of SSID is 32 bytes as per 802.11 specification. */
#define RTW_WPA2_MAX_PSK_LEN		(64)    /**< Maximum pre-shared key (PSK) length for WPA2. */
#define RTW_WPA3_MAX_PSK_LEN		(128)		/**< Maximum PSK length for WPA3. */
/* TODO: rom should also check RTW_PASSPHRASE_MAX_SIZE. */
#define RTW_MAX_PSK_LEN		RTW_WPA3_MAX_PSK_LEN /**< Maximum PSK length used. */
#define RTW_MIN_PSK_LEN		(8)		/**< Minimum PSK length. */

#define MAC_ARG(x)		((u8*)(x))[0],((u8*)(x))[1],\
				((u8*)(x))[2],((u8*)(x))[3],\
				((u8*)(x))[4],((u8*)(x))[5]      /**< Formats MAC address for printing. Usage Example: RTK_LOGS(NOTAG, RTK_LOG_INFO, "MAC addr="MAC_FMT"\n", MAC_ARG(mac_addr));*/
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"  /**< Format string for printing MAC address.*/

#define INVALID_RX_SNR			127

/* SECCAM sec_type define, equal to rxdesc format */
#define _NO_PRIVACY_    0x0
#define _WEP40_         0x1
#define _TKIP_          0x2
#define _TKIP_WTMIC_    0x3
#define _AES_           0x4	//_CCMP_128_ + _CCMP_256_
#define _WEP104_        0x5
#define _SMS4_          0x6	//_WAPI_
#define _GCMP_          0x7	//_GCMP_128_ + _GCMP_256_
#define _GCMP_256_      (_GCMP_ | BIT(3))
#define _CCMP_256_      (_AES_ | BIT(3))
#define _GCM_WAPI_      (_SMS4_ | BIT(3)) //_GCM_WAPI_
#define _BIP_           0x8
#define _BIP_GMAC_256   0x9

/** @} End of WIFI_Exported_Constants group*/


/** @addtogroup WIFI_Exported_Types Wi-Fi Exported Types
* @{
*/
/** @addtogroup WIFI_Exported_Enumeration_Types Enumeration Type
 * @{
 */

/**
 * @brief  Security flags for @ref rtw_security (size: u32).
 */
enum rtw_security_flag {
	WEP_ENABLED          = BIT(0),     /**< WEP encryption */
	TKIP_ENABLED         = BIT(1),     /**< TKIP encryption */
	AES_ENABLED          = BIT(2),     /**< AES (CCMP) encryption */
	AES_CMAC_ENABLED     = BIT(4),     /**< AES-CMAC for Management Frame Protection */
	ENTERPRISE_ENABLED   = BIT(5),     /**< Enterprise (802.1X) authentication */
	OWE_ENABLED          = BIT(6),     /**< Opportunistic Wireless Encryption */
	SHARED_ENABLED       = BIT(15),    /**< Shared Key authentication */
	WPA_SECURITY         = BIT(21),    /**< WPA protocol */
	WPA2_SECURITY        = BIT(22),    /**< WPA2 protocol */
	WPA3_SECURITY        = BIT(23),    /**< WPA3 protocol */
};

/**
  * @brief  Enumerates the disconnect reasons used in @ref rtw_event_join_status_info
  *         when a disconnect event (@ref RTW_JOINSTATUS_DISCONNECT) occurs (size: u16).
  *         The reasons include both standard 802.11 specification-based reasons and custom-defined
  *         reasons by the driver and application layers.
  */
enum rtw_disconn_reason {
#ifndef CONFIG_FULLMAC
	/*802.11 spec reason codes, received after connection in AP's deauth or disassoc frame.*/
	RTW_DISCONN_RSN_80211_UNSPECIFIED                         = 1,
	RTW_DISCONN_RSN_80211_PREV_AUTH_NOT_VALID 			      = 2,
	RTW_DISCONN_RSN_80211_DEAUTH_LEAVING                      = 3,
	RTW_DISCONN_RSN_80211_DISASSOC_DUE_TO_INACTIVITY          = 4,
	RTW_DISCONN_RSN_80211_DISASSOC_AP_BUSY                    = 5,
	RTW_DISCONN_RSN_80211_CLASS2_FRAME_FROM_NONAUTH_STA       = 6,
	RTW_DISCONN_RSN_80211_CLASS3_FRAME_FROM_NONASSOC_STA      = 7,
	RTW_DISCONN_RSN_80211_DISASSOC_STA_HAS_LEFT               = 8,
	RTW_DISCONN_RSN_80211_STA_REQ_ASSOC_WITHOUT_AUTH          = 9,
	RTW_DISCONN_RSN_80211_PWR_CAPABILITY_NOT_VALID            = 10,
	RTW_DISCONN_RSN_80211_SUPPORTED_CHANNEL_NOT_VALID         = 11,
	RTW_DISCONN_RSN_80211_INVALID_IE                          = 13,
	RTW_DISCONN_RSN_80211_MICHAEL_MIC_FAILURE                 = 14,
	RTW_DISCONN_RSN_80211_4WAY_HANDSHAKE_TIMEOUT              = 15,
	RTW_DISCONN_RSN_80211_GROUP_KEY_UPDATE_TIMEOUT            = 16,
	RTW_DISCONN_RSN_80211_IE_IN_4WAY_DIFFERS                  = 17,
	RTW_DISCONN_RSN_80211_GROUP_CIPHER_NOT_VALID              = 18,
	RTW_DISCONN_RSN_80211_PAIRWISE_CIPHER_NOT_VALID           = 19,
	RTW_DISCONN_RSN_80211_AKMP_NOT_VALID                      = 20,
	RTW_DISCONN_RSN_80211_UNSUPPORTED_RSN_IE_VERSION          = 21,
	RTW_DISCONN_RSN_80211_INVALID_RSN_IE_CAPAB                = 22,
	RTW_DISCONN_RSN_80211_IEEE_802_1X_AUTH_FAILED             = 23,
	RTW_DISCONN_RSN_80211_CIPHER_SUITE_REJECTED               = 24,
#endif
	/*RTK defined: Driver-detected issues causing disconnection. */
	RTW_DISCONN_RSN_DRV_BASE                            = 60000,
	RTW_DISCONN_RSN_DRV_AP_LOSS                         = 60001, /**< <!-- DIAG: --> no rx for a long time*/
	RTW_DISCONN_RSN_DRV_AP_CHANGE                       = 60002, /**< <!-- DIAG: --> AP change*/
	RTW_DISCONN_RSN_DRV_BASE_END                        = 60099,

	/*RTK defined: Application layer call some API to cause wifi disconnect.*/
	RTW_DISCONN_RSN_APP_BASE                            = 60100,
	RTW_DISCONN_RSN_APP_DISCONN                         = 60101, /**< <!-- DIAG: --> by APP*/
	RTW_DISCONN_RSN_APP_CONN_WITHOUT_DISCONN            = 60102, /**< <!-- DIAG: --> disconnect before connecting*/
	RTW_DISCONN_RSN_APP_BASE_END                        = 60199,

	RTW_DISCONN_RSN_MAX                                 = 65535,/*0xffff*/
};

/**
 * @brief  Supported Wi-Fi frequency bands (size: u8).
 */
/*TODO: rom should check because (#define BAND_CAP_2G BIT(0), #define BAND_CAP_5G BIT(1)) from rom_rtw_defs.h*/
enum rtw_support_band {
	RTW_SUPPORT_BAND_RSVD = 0,
	RTW_SUPPORT_BAND_2_4G = BIT(0),                    /**< 2.4 GHz band. */
	RTW_SUPPORT_BAND_5G = BIT(1),                      /**< 5 GHz band. */
	RTW_SUPPORT_BAND_2_4G_5G_BOTH = (BIT(0) | BIT(1)), /**< Both 2.4 GHz and 5 GHz bands. */
	RTW_SUPPORT_BAND_MAX                               /**< Maximum band type (for bounds checking). */
};

/**
 * @brief  Speaker-related setting types (size: u8).
 */
enum rtw_speaker_set_type {
	RTW_SPEAKER_SET_INIT = 0,
	RTW_SPEAKER_SET_LATCH_I2S_COUNT = 1,
	RTW_SPEAKER_SET_TSF_TIMER = 2,
};

/**
 * @brief  Transmission frame types for Wi-Fi custom IE (size: u8).
 */
enum rtw_cus_ie_frame_type {
	RTW_CUS_IE_PROBEREQ = BIT(0),  /**< Probe request. */
	RTW_CUS_IE_PROBERSP = BIT(1),  /**< Probe response. */
	RTW_CUS_IE_BEACON	  = BIT(2),     /**< Beacon. */
	RTW_CUS_IE_ASSOCREQ = BIT(3), /**< Assocation request. */
};

/**
 * @brief  Promisc mode filter options (size: u8).
 */
enum rtw_promisc_filter_mode {
	RTW_PROMISC_FILTER_ALL_PKT,  /**< Receive all packets in the air. */
	RTW_PROMISC_FILTER_AP_ALL     /**< Receive all packtets sent by connected AP. */
};

/**
 * @brief  Promisc callback return values (size: u8).
 */
enum rtw_promisc_drv_hdl {
	RTW_PROMISC_NEED_DRV_HDL,  /**< Driver will continue processing this packet. */
	RTW_PROMISC_BYPASS_DRV_HDL     /**< Driver will bypass this packet. */
};

/**
 * @brief  Wi-Fi frequency bands for STA/AP operation (size: u8).
 */
enum rtw_band_type {
	RTW_BAND_ON_24G	= 0,   /**< Operating on the 2.4 GHz band. */
	RTW_BAND_ON_5G	= 1,   /**< Operating on the 5 GHz band. */
	RTW_BAND_ON_6G	= 2,   /**< Operating on the 6 GHz band. */
	RTW_BAND_MAX,          /**< Maximum band type (for bounds checking).*/
};

/**
 * @brief Enumeration of WPA (Wi-Fi Protected Access) modes (size: u8), specifying different security protocol version.
 */
enum rtw_wpa_mode {
	RTW_WPA_AUTO_MODE,       /**< Auto-select WPA versions based on network configuration. (default mode after driver initialization) */
	RTW_WPA_ONLY_MODE,       /**< WPA only */
	RTW_WPA2_ONLY_MODE,      /**< WPA2 only */
	RTW_WPA3_ONLY_MODE,      /**< WPA3 only */
	RTW_WPA_WPA2_MIXED_MODE, /**< Mixed mode supporting both WPA and WPA2 */
	RTW_WPA2_WPA3_MIXED_MODE /**< Mixed mode supporting both WPA2 and WPA3 */
};


/**
 * @brief  Security types for Wi-Fi connections (size: u32).
 * @note  Default Authentication and Key Management (AKM) is PSK for WPA/WPA2 and SAE for WPA3.
 *        For Enterprise mode in WPA/WPA2/WPA3, use the @ref ENTERPRISE_ENABLED flag, e.g.,
 *        @ref WPA3_SECURITY | @ref ENTERPRISE_ENABLED.
 */
enum rtw_security {
	RTW_SECURITY_OPEN               = 0,                                                            /**< Open (no security) */
	RTW_SECURITY_WEP_PSK            = (WEP_ENABLED),                                                /**< WEP security with open authentication */
	RTW_SECURITY_WEP_SHARED         = (WEP_ENABLED | SHARED_ENABLED),                               /**< WEP security with shared authentication */

	RTW_SECURITY_WPA_TKIP_PSK       = (WPA_SECURITY | TKIP_ENABLED),                                /**< WPA security with TKIP encryption */
	RTW_SECURITY_WPA_AES_PSK        = (WPA_SECURITY | AES_ENABLED),                                 /**< WPA security with AES (CCMP) encryption */
	RTW_SECURITY_WPA_MIXED_PSK      = (WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),                  /**< WPA security allowing both AES and TKIP */
	RTW_SECURITY_WPA2_TKIP_PSK		= (WPA2_SECURITY | TKIP_ENABLED),								/**< WPA2 security with TKIP encryption */
	RTW_SECURITY_WPA2_AES_PSK       = (WPA2_SECURITY | AES_ENABLED),                                /**< WPA2 security with AES (CCMP) encryption */
	RTW_SECURITY_WPA2_MIXED_PSK     = (WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED),                 /**< WPA2 security with allowing both AES and TKIP */
	RTW_SECURITY_WPA_WPA2_TKIP_PSK  = (WPA_SECURITY | WPA2_SECURITY | TKIP_ENABLED),                /**< WPA/WPA2 security with TKIP encryption */
	RTW_SECURITY_WPA_WPA2_AES_PSK   = (WPA_SECURITY | WPA2_SECURITY | AES_ENABLED),                 /**< WPA/WPA2 security with AES (CCMP) encryption */
	RTW_SECURITY_WPA_WPA2_MIXED_PSK = (WPA_SECURITY  | WPA2_SECURITY | TKIP_ENABLED | AES_ENABLED), /**< WPA/WPA2 security allowing both AES and TKIP */
	RTW_SECURITY_WPA3_AES_PSK       = (WPA3_SECURITY | AES_ENABLED),                                /**< WPA3-SAE security with AES (CCMP) encryption */
	RTW_SECURITY_WPA3_OWE           = (WPA3_SECURITY | OWE_ENABLED | AES_ENABLED),                  /**< WPA3-OWE (Wi-Fi Enhanced Open) security with AES (CCMP) encryption */
	RTW_SECURITY_WPA2_WPA3_MIXED    = (WPA2_SECURITY | WPA3_SECURITY | AES_ENABLED),                /**< WPA3-SAE/WPA2 with AES (CCMP) encryption */
	RTW_SECURITY_WPA2_AES_CMAC      = (WPA2_SECURITY | AES_CMAC_ENABLED),                           /**< WPA2 security with AES and Management Frame Protection */
};

/**
  * @brief Supported PHY rates (size: u8):
  *        - CCK rates: 1M, 2M, 5.5M, 11M
  *        - OFDM rates: 6M, 9M, 12M, 18M, 24M, 36M, 48M, 54M
  *        - HT rates: MCS0 to MCS7
  *        - VHT rates: MCS0 to MCS8
  *        - HE rates: MCS0 to MCS9
  * @note For HT/VHT/HE PHY, only single spatial stream is supported.
  */
enum rtw_rate {
	RTW_RATE_1M		= 0x02,     /**< 0x02 */
	RTW_RATE_2M		= 0x04,     /**< 0x04 */
	RTW_RATE_5_5M	= 0x0B,     /**< 0x0B */
	RTW_RATE_6M		= 0x0C,     /**< 0x0C */
	RTW_RATE_9M		= 0x12,     /**< 0x12 */
	RTW_RATE_11M 	= 0x16,     /**< 0x16 */
	RTW_RATE_12M = 0x18,      /**< 0x18 */
	RTW_RATE_18M = 0x24,      /**< 0x24 */
	RTW_RATE_24M = 0x30,      /**< 0x30 */
	RTW_RATE_36M = 0x48,      /**< 0x48 */
	RTW_RATE_48M = 0x60,      /**< 0x60 */
	RTW_RATE_54M = 0x6C,      /**< 0x6C */
	RTW_RATE_MCS32	= 0x7F,  /**< 0x7f */
	RTW_RATE_MCS0,   /**< 0x80 */
	RTW_RATE_MCS1,   /**< 0x81 */
	RTW_RATE_MCS2,   /**< 0x82 */
	RTW_RATE_MCS3,   /**< 0x83 */
	RTW_RATE_MCS4,   /**< 0x84 */
	RTW_RATE_MCS5,   /**< 0x85 */
	RTW_RATE_MCS6,
	RTW_RATE_MCS7,

	RTW_RATE_VHT1SS_MCS0 = 0xa0,
	RTW_RATE_VHT1SS_MCS1,
	RTW_RATE_VHT1SS_MCS2,
	RTW_RATE_VHT1SS_MCS3,
	RTW_RATE_VHT1SS_MCS4,
	RTW_RATE_VHT1SS_MCS5,
	RTW_RATE_VHT1SS_MCS6,
	RTW_RATE_VHT1SS_MCS7,
	RTW_RATE_VHT1SS_MCS8,
	RTW_RATE_VHT1SS_MCS9,

	RTW_RATE_HE1SS_MCS0 = 0xd0,   /**< 0xd0 */
	RTW_RATE_HE1SS_MCS1,
	RTW_RATE_HE1SS_MCS2,
	RTW_RATE_HE1SS_MCS3,
	RTW_RATE_HE1SS_MCS4,
	RTW_RATE_HE1SS_MCS5,
	RTW_RATE_HE1SS_MCS6,
	RTW_RATE_HE1SS_MCS7,
	RTW_RATE_HE1SS_MCS8,
	RTW_RATE_HE1SS_MCS9,
	RTW_RATE_HE1SS_MCS10,
	RTW_RATE_HE1SS_MCS11,

	RTW_RATE_UNKNOWN = 0xff  /**< 0xff */
};

enum rtw_gi_ltf_cap {
	RTW_HE_32_GI_4X_LTF	= BIT(0), /** HE 3.2us 4x LTF */
	RTW_HE_08_GI_4X_LTF	= BIT(1), /** HE 0.8us 4x LTF */
	RTW_HE_16_GI_2X_LTF	= BIT(2), /** HE 1.6us 2x LTF */
	RTW_HE_08_GI_2X_LTF	= BIT(3), /** HE 0.8us 2x LTF */
	RTW_HE_16_GI_1X_LTF	= BIT(4), /** HE 1.6us 1x LTF */
	RTW_HE_08_GI_1X_LTF	= BIT(5), /** HE 0.8us 1x LTF */
	RTW_HE_GI_LTF_ALL	= 0xFF /** support all GI-LTF modes */
};

/**
  * @brief CSI triggering management frame subtypes (size: u16).
  */
enum rtw_csi_trig_frame_mgnt {
	RTW_CSI_TRIG_ASSOCREQ   = BIT(0),
	RTW_CSI_TRIG_ASSOCRSP   = BIT(1),
	RTW_CSI_TRIG_REASSOCREQ = BIT(2),
	RTW_CSI_TRIG_REASSOCRSP = BIT(3),
	RTW_CSI_TRIG_PROBEREQ   = BIT(4),
	RTW_CSI_TRIG_PROBERSP   = BIT(5),
	RTW_CSI_TRIG_BEACON     = BIT(8),
	RTW_CSI_TRIG_ATIM       = BIT(9),
	RTW_CSI_TRIG_DISASSOC   = BIT(10),
	RTW_CSI_TRIG_AUTH       = BIT(11),
	RTW_CSI_TRIG_DEAUTH     = BIT(12),
	RTW_CSI_TRIG_ACTION     = BIT(13)
};

/**
  * @brief CSI triggering control frame subtypes (size: u16).
  */
enum rtw_csi_trig_frame_ctrl {
	RTW_CSI_TRIG_TRIGGER     = BIT(2),
	RTW_CSI_TRIG_BA          = BIT(9),
	RTW_CSI_TRIG_PSPOLL      = BIT(10),
	RTW_CSI_TRIG_RTS         = BIT(11),
	RTW_CSI_TRIG_CTS         = BIT(12),
	RTW_CSI_TRIG_ACK         = BIT(13),
	RTW_CSI_TRIG_CFEND       = BIT(14),
	RTW_CSI_TRIG_CFEND_CFACK = BIT(15)
};

/**
  * @brief CSI triggering data frame subtypes (size: u16).
  */
enum rtw_csi_trig_frame_data {
	RTW_CSI_TRIG_DATA           = BIT(0),
	RTW_CSI_TRIG_DATA_CFACK     = BIT(1),
	RTW_CSI_TRIG_DATA_CFPOLL    = BIT(2),
	RTW_CSI_TRIG_DATA_CFACKPOLL = BIT(3),
	RTW_CSI_TRIG_DATA_NULL      = BIT(4),
	RTW_CSI_TRIG_CF_ACK         = BIT(5), /**< CF-Ack (no data) */
	RTW_CSI_TRIG_CF_POLL        = BIT(6),
	RTW_CSI_TRIG_CF_ACKPOLL     = BIT(7),
	RTW_CSI_TRIG_QOS_DATA       = BIT(8),
	RTW_CSI_TRIG_QOS_DATA_NULL	= BIT(12)
};

/**
  * @brief CSI action types for wifi_csi_config() (size: u8).
  */
enum rtw_csi_action_type {
	RTW_CSI_ACT_EN,           /**< enable or disable csi func */
	RTW_CSI_ACT_CFG,          /**< config csi parameters */
	RTW_CSI_ACT_MAX
};

/**
  * @brief CSI info subcarrier decimation for reducing data volume (size: u8).
  */
enum rtw_csi_group_num {
	RTW_CSI_GROUP_NUM_1 = 0,  /**< per tone */
	RTW_CSI_GROUP_NUM_2,      /**< per 2tone */
	RTW_CSI_GROUP_NUM_4,      /**< per 4tone */
	RTW_CSI_GROUP_NUM_8_16,   /**< per 8tone for dplus; per 16tone for others */
	RTW_CSI_GROUP_NUM_MAX
};

/**
  * @brief CSI mode for fetching CSI info (size: u8).
  */
enum rtw_csi_mode {
	RTW_CSI_MODE_NORMAL = 0,  /**< Rx normal mode (estimating CSI by the currently received packet). */
	RTW_CSI_MODE_NDP,         /**< Rx ndp mode: not support */
	RTW_CSI_MODE_RX_RESP,     /**< Rx response mode (estimating CSI by receiving ACK for the previous transmission). */
	RTW_CSI_MODE_MAX,
};

/**
  * @brief  CSI accuracy specifies the format of CSI raw data represented as complex numbers (size: u8).
  * @note
  *       - Each CSI data point consists of an I (In-phase, real part) and Q (Quadrature, imaginary part) value.
  *         Both I and Q use the same format for a given accuracy setting.
  *       - For example, the format S(8,3) means a signed 8-bit number with 3 fractional bits.
  */
enum rtw_csi_accuracy {
	RTW_CSI_ACCU_1BYTE = 0,   /**< S(8,3) for dplus and S(8,4) for others */
	RTW_CSI_ACCU_2BYTES,      /**< S(16,11) for dplus and S(16,12) for others */
	RTW_CSI_ACCU_MAX
};

/**
  * @brief CSI algorithm options (size: u8).
  */
enum rtw_csi_alg {
	RTW_CSI_ALG_LS = 0,       /**< Least Squares algorithm */
	RTW_CSI_ALG_SMOTHING,     /**< Smoothing algorithm */
	RTW_CSI_ALG_MAX           /**< Other algorithm */
};

/**
  * @brief CSI acquisition from leagcy or non-legacy LTF (size: u8).
  */
enum rtw_csi_ch {
	RTW_CSI_CH_LEGACY = 0,    /**< legacy part(L-LTF) channel estmation result */
	RTW_CSI_CH_NON_LEGACY,    /**< non-legacy(HT-LTF) part */
	RTW_CSI_CH_MAX
};

/**
  * @brief CSI operation role (size: u8).
  */
enum rtw_csi_role {
	RTW_CSI_OP_ROLE_TRX = 0,  /**< both trx */
	RTW_CSI_OP_ROLE_TX  = 1,  /**< only tx csi triggering frame */
	RTW_CSI_OP_ROLE_RX  = 2,  /**< only rx csi triggering frame for fetching csi report */
	RTW_CSI_OP_ROLE_MAX
};


/**
* @brief Total Radiated Power (TRP) and Total Isotropic Sensitivity (TIS) certification modes (size: u8).
*/
enum rtw_trp_tis_mode {
	RTW_TRP_TIS_DISABLE = 0,               /**< Disable TRP/TIS certification (default) */
	RTW_TRP_TIS_NORMAL = 1,
};

/**
* @brief EDCCA modes (size: u8).
*/
enum rtw_edcca_mode {
	RTW_EDCCA_NORM	= 0, /**< Dynamic threshold adjustment based on real-time RSSI measurements (default). */
	RTW_EDCCA_ADAPT	= 1, /**< Fixed threshold to comply with ETSI Adaptivity Test requirements. */
	RTW_EDCCA_CS	= 2, /**< Carrier Sense for Japan's interference avoidance regulations. */
	RTW_EDCCA_DISABLE	= 9, /**< Disables EDCCA, allowing MAC transmission without signal verification. */
};

/**
* @brief  Antenna diversity mode types (size: u8).
*/
enum rtw_antdiv_mode {
	RTW_ANTDIV_AUTO     = 0,    /**< Automatic antenna diversity selection. */
	RTW_ANTDIV_FIX_MAIN = 1,    /**< Fixed main antenna. */
	RTW_ANTDIV_FIX_AUX  = 2,    /**< Fixed auxiliary antenna. */
	RTW_ANTDIV_DISABLE  = 0xF,  /**< Disable antenna diversity.*/
};

/**
  * @brief  BSS types (size: u8).
  */
enum rtw_bss_type {
	RTW_BSS_TYPE_INFRASTRUCTURE     = 0, /**< Infrastructure network */
	RTW_BSS_TYPE_WTN_HELPER         = 1, /**< WTN helper network */
	RTW_BSS_TYPE_UNKNOWN
};

/**
  * @brief  Wi-Fi scan options (size: u8).
  */
enum rtw_scan_type {
	RTW_SCAN_NOUSE          = 0x00,  /**< Default value */
	RTW_SCAN_ACTIVE         = 0x01,  /**< Active scan */
	RTW_SCAN_PASSIVE        = 0x02,  /**< Passive scan*/
	RTW_SCAN_NO_HIDDEN_SSID = 0x04,  /**< Filter out hidden SSID APs*/
	RTW_SCAN_REPORT_EACH    = 0x08,  /**< Report each found AP immediately */
	RTW_SCAN_WITH_P2P       = 0x10,  /**< For P2P usage */
	RTW_SCAN_FOR_ZRPP       = 0x20,  /**< For Zero R-mesh Provisioning Protocol usage */
};

/**
  * @brief  WPS (Wi-Fi Protected Setup) types (size: u8).
  */
enum rtw_wps_type {
	RTW_WPS_TYPE_DEFAULT                = 0,	/**< Default type */
	RTW_WPS_TYPE_USER_SPECIFIED         = 1,	/**< User-specified type */
	RTW_WPS_TYPE_MACHINE_SPECIFIED      = 2,	/**< Machine-specified type */
	RTW_WPS_TYPE_REKEY                  = 3,	/**< Retry key type */
	RTW_WPS_TYPE_PUSHBUTTON             = 4,	/**< Push button type */
	RTW_WPS_TYPE_REGISTRAR_SPECIFIED    = 5,	/**< Register-specified type */
	RTW_WPS_TYPE_NONE                   = 6, 	/**< none */
	RTW_WPS_TYPE_WSC                    = 7,	/**< Wi-Fi Simple Configuration (WSC) type */
};

/**
  * @brief Supported operation mode by the Wi-Fi driver (size: u8).
  */
enum rtw_drv_op_mode {
	RTW_MODE_NONE   = 0,   /**< None */
	RTW_MODE_STA    = 1,   /**< STA mode */
	RTW_MODE_AP     = 2,   /**< SoftAP mode */
	RTW_MODE_NAN    = 3,   /**< NAN mode */
	RTW_MODE_MAX
};

/**
  * @brief  Power save levels for Inactive Power Save (IPS) mode (size: u8).
  */
enum rtw_ips_level {
	RTW_IPS_WIFI_OFF = 0,  /**< The Wi-Fi is fully powered off during IPS to maximize power savings. */
	RTW_IPS_WIFI_PG,       /**< The Wi-Fi module enters Power Gating, partially turning off RF/BB/MAC, allowing quicker exit from IPS. Only supported by Amebadplus. */
	RTW_IPS_LEVEL_MAX,
};

/**
 * @brief  The maximum service period lengths for U-APSD (Unscheduled Automatic Power Save Delivery) operations (size: u8).
 */
enum rtw_uapsd_max_sp {
	RTW_UAPSD_NO_LIMIT,  /**< No limit on service period length */
	RTW_UAPSD_TWO_MSDU,  /**< Two MSDUs per service period */
	RTW_UAPSD_FOUR_MSDU, /**< Four MSDUs per service period */
	RTW_UAPSD_SIX_MSDU   /**< Six MSDUs per service period */
};

/**
 * @brief Enumeration of supported Wi-Fi network modes (size: u8).
 */
/*TODO: rom should check because moved from rom_rtw_defs.h*/
enum rtw_wireless_mode {
	RTW_80211_INVALID = 0,
	RTW_80211_B       = BIT(0),
	RTW_80211_A       = BIT(1),
	RTW_80211_G       = BIT(2),
	RTW_80211_N       = BIT(3),
	RTW_80211_AC      = BIT(4),
	RTW_80211_AX      = BIT(5),

	/* Type for current wireless mode */
	RTW_80211_BG      = (RTW_80211_B | RTW_80211_G),
	RTW_80211_GN      = (RTW_80211_G | RTW_80211_N),
	RTW_80211_AN      = (RTW_80211_A | RTW_80211_N),
	RTW_80211_BN      = (RTW_80211_B | RTW_80211_N),
	RTW_80211_BGN     = (RTW_80211_B | RTW_80211_G | RTW_80211_N),
	RTW_80211_BGAX    = (RTW_80211_B | RTW_80211_G | RTW_80211_AX),
	RTW_80211_GAX     = (RTW_80211_G | RTW_80211_AX),
	RTW_80211_A_AC    = (RTW_80211_A | RTW_80211_AC),
	RTW_80211_A_AX    = (RTW_80211_A | RTW_80211_AX),

	/* Capability -Type for registry default wireless mode */
	RTW_80211_AGN     = (RTW_80211_A | RTW_80211_G | RTW_80211_N),
	RTW_80211_ABGN    = (RTW_80211_A | RTW_80211_B | RTW_80211_G | RTW_80211_N),
	RTW_80211_ANAC    = (RTW_80211_A | RTW_80211_N | RTW_80211_AC),
	RTW_80211_24G_MIX = (RTW_80211_B | RTW_80211_G | RTW_80211_N  | RTW_80211_AX),
	RTW_80211_5G_MIX  = (RTW_80211_A | RTW_80211_N | RTW_80211_AC | RTW_80211_AX),
	RTW_80211_MAX     = (RTW_80211_24G_MIX | RTW_80211_5G_MIX),
};

/**
 * @brief TX power limit regulatory domains (size: u8).
 */
/*TODO: rom should check because moved from rom_rtw_defs.h*/
enum rtw_txpwr_lmt {
	TXPWR_LMT_FCC = 0,      /**< Federal Communications Commission (USA). */
	TXPWR_LMT_MKK = 1,      /**< Japan */
	TXPWR_LMT_ETSI = 2,     /**< European Telecommunications Standards Institute (CE).*/
	TXPWR_LMT_IC = 3,       /**< Canada */
	TXPWR_LMT_KCC = 4,      /**< South Korea */
	TXPWR_LMT_ACMA = 5, 	/**< Australia */
	TXPWR_LMT_CHILE = 6,    /**< Chile */
	TXPWR_LMT_MEXICO = 7,   /**< Mexico */
	TXPWR_LMT_WW = 8,       /**< Worldwide, The mininum of all */
	TXPWR_LMT_GL = 9,		/**< Global */
	TXPWR_LMT_UKRAINE = 10, /**< Ukraine */
	TXPWR_LMT_CN = 11,       /**< China */
	TXPWR_LMT_QATAR = 12,   /**< Qatar */
	TXPWR_LMT_UK = 13,      /**< Great Britain (United Kingdom; England) */
	TXPWR_LMT_NCC = 14,     /**< Taiwan */
	TXPWR_LMT_EXT = 15,     /**< Customer Customization */

	/* ===== Add new power limit above this line. ===== */
	TXPWR_LMT_CONST_MAX     /**< unchanging part define max */
};

/**
 * @brief Wi-Fi frame types (size: u8).
 */
enum rtw_frame_type {
	RTW_MGT_TYPE  =	(0),                     /**< Management frame */
	RTW_CTRL_TYPE =	(BIT(2)),                /**< Control frame */
	RTW_DATA_TYPE =	(BIT(3)),                /**< Data frame */
	RTW_QOS_DATA_TYPE	= (BIT(7) | BIT(3)), /**< QoS data frame */
};

/**
 * @brief Wi-Fi frame subtypes (size: u8).
 */
enum rtw_frame_type_subtype {
	// below is for mgt frame
	RTW_ASSOCREQ       = (0 | RTW_MGT_TYPE),
	RTW_ASSOCRSP       = (BIT(4) | RTW_MGT_TYPE),
	RTW_REASSOCREQ     = (BIT(5) | RTW_MGT_TYPE),
	RTW_REASSOCRSP     = (BIT(5) | BIT(4) | RTW_MGT_TYPE),
	RTW_PROBEREQ       = (BIT(6) | RTW_MGT_TYPE),
	RTW_PROBERSP       = (BIT(6) | BIT(4) | RTW_MGT_TYPE),
	RTW_BEACON         = (BIT(7) | RTW_MGT_TYPE),
	RTW_ATIM           = (BIT(7) | BIT(4) | RTW_MGT_TYPE),
	RTW_DISASSOC       = (BIT(7) | BIT(5) | RTW_MGT_TYPE),
	RTW_AUTH           = (BIT(7) | BIT(5) | BIT(4) | RTW_MGT_TYPE),
	RTW_DEAUTH         = (BIT(7) | BIT(6) | RTW_MGT_TYPE),
	RTW_ACTION         = (BIT(7) | BIT(6) | BIT(4) | RTW_MGT_TYPE),

	// below is for control frame
	RTW_PSPOLL         = (BIT(7) | BIT(5) | RTW_CTRL_TYPE),
	RTW_RTS            = (BIT(7) | BIT(5) | BIT(4) | RTW_CTRL_TYPE),
	RTW_CTS            = (BIT(7) | BIT(6) | RTW_CTRL_TYPE),
	RTW_ACK            = (BIT(7) | BIT(6) | BIT(4) | RTW_CTRL_TYPE),
	RTW_BA            	= (BIT(7) | BIT(4) | RTW_CTRL_TYPE),
	RTW_CFEND          = (BIT(7) | BIT(6) | BIT(5) | RTW_CTRL_TYPE),
	RTW_CFEND_CFACK    = (BIT(7) | BIT(6) | BIT(5) | BIT(4) | RTW_CTRL_TYPE),
	RTW_TRIGGER        = (BIT(5) | RTW_CTRL_TYPE),

	// below is for data frame
	RTW_DATA           = (0 | RTW_DATA_TYPE),
	RTW_DATA_CFACK     = (BIT(4) | RTW_DATA_TYPE),
	RTW_DATA_CFPOLL    = (BIT(5) | RTW_DATA_TYPE),
	RTW_DATA_CFACKPOLL = (BIT(5) | BIT(4) | RTW_DATA_TYPE),
	RTW_DATA_NULL      = (BIT(6) | RTW_DATA_TYPE),
	RTW_CF_ACK         = (BIT(6) | BIT(4) | RTW_DATA_TYPE),
	RTW_CF_POLL        = (BIT(6) | BIT(5) | RTW_DATA_TYPE),
	RTW_CF_ACKPOLL     = (BIT(6) | BIT(5) | BIT(4) | RTW_DATA_TYPE),
	RTW_QOS_DATA_NULL	= (BIT(6) | RTW_QOS_DATA_TYPE),
};

/**
 * @brief RMesh node types (size: u8).
 */
enum rtw_rmesh_node_type {
	RMESH_SELF_NODE = 0,
	RMESH_FATHER_NODE = 1,
	RMESH_ROOT_NODE = 2,
};

/**
 * @brief update_masks field definition for struct rtw_tx_advanced_cfg{} (size: u16).
 */

enum rtw_tx_advanced_cfg_update_masks {
	RTW_UPDATE_TXCFG_TX_LIFE_TIME_BEBK      = BIT(0),
	RTW_UPDATE_TXCFG_TX_LIFE_TIME_VIVO      = BIT(1),
	RTW_UPDATE_TXCFG_BCN_TX_PROTECT_TIME    = BIT(2),
	RTW_UPDATE_TXCFG_BCN_RX_PROTECT_TIME    = BIT(3),
	RTW_UPDATE_TXCFG_NAV_UPDATE_TH          = BIT(4),
	RTW_UPDATE_TXCFG_IGNORE_TX_NAV          = BIT(5),
	RTW_UPDATE_TXCFG_PARAM_ALL              = 0xFFFF,
};

/**
 * @brief update_masks field definition for struct rtw_conn_step_retries{} (size: u16).
 */

enum rtw_conn_step_retries_update_masks {
	RTW_UPDATE_CONN_RESCAN                = BIT(0),
	RTW_UPDATE_CONN_REAUTH                = BIT(1),
	RTW_UPDATE_CONN_SAE_REAUTH            = BIT(2),
	RTW_UPDATE_CONN_REASSOC               = BIT(3),
	RTW_UPDATE_CONN_RESEND_EAPOL          = BIT(4),
	RTW_UPDATE_CONN_PARAM_ALL             = 0xFFFF,
};

/** @} End of WIFI_Exported_Enumeration_Types group*/

/** @addtogroup WIFI_Exported_Structure_Types Structure Type
 * @{
 */
/**********************************************************************************************
 *                                  common structures
 *********************************************************************************************/
#pragma pack(1)/*rtw_ssid and rtw_mac are 1 byte alignment for some issues long long ago*/
/**
  * @brief  Describes the SSID (Service Set Identification), which is the network name of an Access Point.
  */
struct rtw_ssid {
	u8		len;     /**< SSID length. Should not > @ref RTW_ESSID_MAX_SIZE.  */
	u8		val[RTW_ESSID_MAX_SIZE + 1]; /**< SSID value, terminated with a null character.*/
};

/**
  * @brief  Represents a unique 6-byte MAC address.
  */
struct rtw_mac {
	u8		octet[6]; /**< Unique 6-byte MAC address. */
};
#pragma pack()

/**********************************************************************************************
 *                                   scan structures
 *********************************************************************************************/

#pragma pack(1)/*scan related structs are 1 byte alignment for some issues long long ago*/
/**
  * @brief  Describes channel busyness for Automatic Channel Selection (ACS).
  */
struct rtw_acs_mntr_rpt {
	u16 meas_time; /**< Measurements duration on this channel, unit: ms.*/
	u16 busy_time; /**< Time the primary channel was sensed busy, unit: ms.*/
	u16 tx_time;   /**< Time spent transmitting frame on this channel, unit: ms.*/
	s8 noise;      /**< Measured noise power on this channel, unit: dbm.*/
	u8 channel;    /**< Scanned channel number. */
};

/**
  * @brief  Contains details of a scanned AP.
  */
struct rtw_scan_result {
	struct rtw_ssid    ssid;             /**< SSID of the AP. */
	struct rtw_mac     bssid;            /**< BSSID (MAC address) of the AP. */
	s16                signal_strength;  /**< Receive Signal Strength Indication (RSSI) in dBm: <-90 Very poor, >-30 Excellent. */
	u8				   bss_type;         /**< BSS type. Common value: @ref RTW_BSS_TYPE_INFRASTRUCTURE.*/
	u32                security;         /**< Security type of the AP: @ref RTW_SECURITY_OPEN, @ref RTW_SECURITY_WEP_PSK, etc. */
	u8				   wps_type;         /**< WPS types supported by the AP: @ref RTW_WPS_TYPE_DEFAULT, @ref RTW_WPS_TYPE_USER_SPECIFIED, etc. */
	u32                channel;          /**< Radio channel where the AP beacon was detected.*/
	u8                 band;             /**< Frequency band used by the AP: @ref RTW_BAND_ON_5G, @ref RTW_BAND_ON_24G. */

	/** The wireless spectrum management regulations followed by the AP. Coded according to ISO 3166 standard. \n
	 *  Reference: ameba_wifi_country_code_table_usrcfg.c for specific values. \n
	 *  Example: For China, country_code[0] = 'C', country_code[1] = 'N'. */
	u8                 country_code[2];
	u8                 wireless_mode;    /**< Wireless mode: @ref RTW_80211_B, @ref RTW_80211_A, etc.*/
	u8                 rom_rsvd[3];
};

/**
  * @brief  Specifies the scan duration per channel.
  */
struct rtw_channel_scan_time {
	u16		active_scan_time;      /**< Active scan duration per channel (ms, default: 110). */
	u16		passive_scan_time;     /**< Passive scan duration per channel (ms, default: 110). */
};

/**
  * @brief  Describes the scan parameters used for Wi-Fi scanning.
  */
struct rtw_scan_param {
	u8                               options; /**< Scan option: @ref RTW_SCAN_ACTIVE, @ref RTW_SCAN_PASSIVE, etc.*/
	u8                              *ssid;    /**< Target SSID to scan for. Length should not exceed @ref RTW_ESSID_MAX_SIZE. */
	u8                              *channel_list;      /**< List of specific channels to scan. */
	u8                               channel_list_num;  /**< Number of channels in `channel_list`.*/
	struct rtw_channel_scan_time     chan_scan_time;    /**< Scan duration for each channel.*/
	u8								 probe_req_num; 	/**< Number of probe request frames to issue in an active scan channel.*/

	/** Maximum number of APs to record. When set to 0, use default value 64.
	 *  APs with the lowest RSSI are discarded if scanned APs exceed this number. */
	u16                              max_ap_record_num;
	void                            *scan_user_data;   /**< User-defined data passed to callback functions for handling scan results. */
	u8                              rom_rsvd[8];       //resverd for next cut

	/** @brief Callback for normal asynchronous mode.
	  * @param[in] ap_num: Total number of scanned APs.
	  * @param[in] user_data: Pointer to user data (see `scan_user_data`).
	  * @return @ref RTK_SUCCESS or @ref RTK_FAIL.
	  */
	s32(*scan_user_callback)(u32 ap_num, void *user_data);

	/** @brief Callback for @ref RTW_SCAN_REPORT_EACH mode.
	  * @param[in] scanned_ap_info: Pointer to details of a scanned AP.
	  * @param[in] user_data: Pointer to user data (see `scan_user_data`).
	  * @return @ref RTK_SUCCESS or @ref RTK_FAIL.
	  */
	s32(*scan_report_each_mode_user_callback)(struct rtw_scan_result *scanned_ap_info, void *user_data);

	/** @brief  Callback for reporting ACS (Automatic Channel Selection) info.
	  * @param[in] scanned_ap_info: Pointer to channel busyness information.
	  * @return @ref RTK_SUCCESS or @ref RTK_FAIL.
	  */
	s32(*scan_report_acs_user_callback)(struct rtw_acs_mntr_rpt *acs_mntr_rpt);
};
#pragma pack()

/**********************************************************************************************
 *                                     connect structures
 *********************************************************************************************/
/**
  * @brief  Stores wpa_supplicant info for STA connection (Linux host only, not needed for RTOS).
  */
struct rtw_wpa_supp_connect {
	u8 rsnxe_ie[RSNXE_MAX_LEN];  /**< The RSNXE IE from beacon of the AP which STA is trying to connect.*/
};

/**
  * @brief	Describes connection settings for connecting to an AP
  * @note
  *        1. If used for Wi-Fi connect, setting `channel` to 0 means full channel scan;
  *           setting `channel` to a specific value means do active scan on that channel.
  *        2. Set `pscan_option` to @ref RTW_PSCAN_FAST_SURVEY for fast survey (active scan on
  *           specified channel, 25ms each, up to 7 attempts); 0 for normal scan.
  */
struct rtw_network_info {
	struct rtw_ssid				ssid;  /**< AP's SSID (max length: @ref RTW_ESSID_MAX_SIZE). */
	struct rtw_mac				bssid; /**< AP's MAC address. */
	u32							security_type; /**< Necessarily set for WEP (@ref RTW_SECURITY_WEP_PSK, @ref RTW_SECURITY_WEP_SHARED). Auto-adjusted for others. */
	u8				           *password;	   /**< AP's password. */
	s32 						password_len;  /**< Password length (max: @ref RTW_MAX_PSK_LEN). */
	s32 						key_id;		   /**< WEP key ID (0-3). Only for WEP.*/
	u8				            channel;	   /**< 0 for full scan, other values to scan specific channel. */
	u8 						    pscan_option;	/**< @ref RTW_PSCAN_FAST_SURVEY for fast survey, 0 for normal scan. */
	u8 				            is_wps_trigger;	/**< Indicates if connection is triggered by WPS. */
	struct rtw_wpa_supp_connect	wpa_supp;   /**< Used by Linux host for STA connect details (not used by RTOS). */
	struct rtw_mac				prev_bssid; /**< BSSID of the AP before roaming. */
	u8							by_reconn; /**< Indicates if connection is triggered by auto-reconnect. */
	u8							rom_rsvd[4];
};

/**
  * @brief  Stores sme auth info for STA connection (Linux host only, not needed for RTOS).
  */
struct rtw_sme_auth_info {
	u16 auth_alg;
	u16 capability;
	u8 ht_info[22];

	s32 rssi;
	u8 bssid[ETH_ALEN];
	u8 tx_chan;

	u16 sae_trans, sae_status;

	u8 key_len, key_index;
	u8 key[13];

	size_t data_len;
	u8 data[];
};

/**
 * @brief  Defines retry limits for different connection steps: authentication, association, and key exchange.
 * @note   All retry limits are capped at 10.
 */
struct rtw_conn_step_retries {
	u16 update_masks;                /**< Mask subfield. If a parameter is set, its corresponding bit in update_masks must also be set. @ref RTW_UPDATE_CONN_RESCAN... */
	u8 rescan_limit : 4;             /**< Retry limit for scan when joinbss. */
	u8 reauth_limit : 4;             /**< Retry limit for authentication (open/shared key). */
	u8 sae_reauth_limit : 4;         /**< Retry limit for SAE authentication. */
	u8 reassoc_limit : 4;            /**< Retry limit for association. */
	u8 eapol_key_rsend_limit : 4;    /**< Retry limit for the 4-way handshake process. */
};

/**********************************************************************************************
 *                                     wifi_status structures
 *********************************************************************************************/
/**
  * @brief  Stores Wi-Fi settings retrieved from the Wi-Fi driver.
  * @note	Structure size must remain unchanged for compatibility.
  */
struct rtw_wifi_setting {
	u8		mode;   /**< Current WLAN interface mode: @ref RTW_MODE_STA, @ref RTW_MODE_AP, @ref RTW_MODE_NAN. */
	u8 		ssid[33];   /**< SSID of connected AP or SoftAP (null-terminated string). */
	u8		bssid[6];   /**< BSSID (MAC address) of connected AP or SoftAP. */
	u8		channel;    /**< Operating channel of connected AP or SoftAP. */
	u32		security_type; /**< Security type of connected AP or SoftAP: @ref RTW_SECURITY_OPEN, @ref RTW_SECURITY_WEP_PSK, etc.*/
	u8 		password[RTW_MAX_PSK_LEN];   /**< Password of connected AP or SoftAP. */
	u8		key_idx;      /**< WEP key index (only valid when security type is WEP).*/
	u8		alg;		/**< Reserved for internal driver compatibility; users can ignore. */
	u32		auth_type;  /**< Reserved for internal driver use; users can ignore. */
	u8		is_wps_trigger;	/**< Indicates if connection was triggered by WPS process. */
	u32		rom_rsvd;
};

/**
  * @brief  Traffic statistics for Wi-Fi interfaces.
  */
union rtw_traffic_stats {
	struct rtw_sta_traffic_stats {
		u32	rx_packets;			/**< Total packets received (excluding custom packets).*/
		u32	tx_packets;			/**< Total packets transmitted .*/
		u8	cur_rx_data_rate;	/**< Current rx data rate. Values: @ref RTW_RATE_1M, @ref RTW_RATE_2M, etc. */
		u8	cur_tx_data_rate;	/**< Current tx data rate. Values: @ref RTW_RATE_1M, @ref RTW_RATE_2M, etc. */
	} sta; /**< Statistic for STA mode.*/
	struct rtw_ap_traffic_stats {
		u32	rx_packets;			/**< Total packets received (excluding custom packets).*/
		u32	tx_packets;			/**< Total packets transmitted.*/
	} ap; /**< Statistic for SoftAP mode.*/
};

/**
  * @brief  Wi-Fi related physical layer statistics.
  */
union rtw_phy_stats {
	struct rtw_sta_phy_stats {
		s8	rssi;          /**< Average mixed rssi in last 1 sec. */
		s8	data_rssi;          /**< Average data rssi in last 1 sec. */
		s8	beacon_rssi;          /**< Average beacon rssi in last 1 sec. */
		s8	snr;          /**< Average snr in 1 sec (excluding CCK rate).*/
	} sta; /**< Statistics for STA mode */
	struct rtw_ap_phy_stats {
		s8	data_rssi;          /**< Average data rssi in last 1 sec. */
	} ap; /**< Statistics for SoftAP mode */
	struct rtw_cmn_phy_stats {
		u8  cca_clm; /**< Channel loading measurement ratio by cca (the ratio of CCA = 1 in number of samples). driver do clm every 2 seconds, the value is the lastest result. */
		u8	edcca_clm; /**< Channel loading measurement ratio by edcca (the ratio of EDCCA = 1 in number of samples). The value is also the lastest result. */
		u8	clm_channel; /**< Channel corresponding to the latest clm result.*/
	} cmn; /**< Common statistic.*/
};

/**********************************************************************************************
 *                                     softap structures
 *********************************************************************************************/
/**
  * @brief  Describes the configuration for starting AP mode.
  *			security type, password and default channel, used to start AP mode.
  * @note  SSID length should not exceed @ref RTW_ESSID_MAX_SIZE.
  *        Password length should not exceed @ref RTW_MAX_PSK_LEN.
  */
struct rtw_softap_info {
	struct rtw_ssid		ssid;          /**< SoftAP SSID and its length. */
	u8		            hidden_ssid;   /**< Set to 1 to hide SSID in SoftAP beacon. */
	u32					security_type; /**< Security type: @ref RTW_SECURITY_OPEN, @ref RTW_SECURITY_WEP_PSK, etc.*/
	u8 		           *password;      /**< Pointer to SoftAP password. */
	u8 		            password_len;  /**< The length of password. */
	u8		            channel;       /**< Desired operating channel for the SoftAP. */
};

#ifndef CONFIG_FULLMAC
/**
  * @brief  Stores information about clients associated with the SoftAP.
  */
struct rtw_client_list {
	u32    count;         /**< Number of associated clients.    */
	struct rtw_mac mac_list[MACID_HW_MAX_NUM - 2]; /**< Array of client MAC addresses. */
};
#endif

/**
  * @brief  Configuration parameters for Channel Switch Announcement (CSA).
  */
struct rtw_csa_parm {
	u8 new_chl; /**< Target channel to switch to. */
	u8 chl_switch_cnt; /**< Countdown to channel switch, in units of 102ms (`chl_switch_cnt`*102ms). */
	u8 action_type;	/**< CSA action frame type: 0 - unicast, 1 - broadcast, other - disable CSA action frame. */
	u8 bc_action_cnt; /**< Number of broadcast CSA actions sent per beacon interval. Only valid when `action_type = 1`.*/
	u8 chl_switch_mode; /**< Restrictions on connected STAs transmission until a channel switch: 1 - disable STAs from transmitting any frames; 0 - no restrictions. */
	/** @brief Callback function invoked after channel switch completion.
	  * @param[in] channel:  New channel number.
	  * @param[in] ret: Result of channel switch: @ref RTK_FAIL or @ref RTK_SUCCESS.
	  */
	void (*callback)(u8 channel, s8 ret);
};

/**********************************************************************************************
 *                                     promisc structures
 *********************************************************************************************/
/**
*@brief  Provides detailed information about a received packet.
*/
struct rtw_rx_pkt_info {
	s8 recv_signal_power;  /**< Received signal strength indicator (RSSI) in dBm. */
	s8 snr;                /**< Signal-to-noise ratio in dB. Unsupported ICs include Amebalite, Amebasmart, Amebagreen2 using INVALID_RX_SNR*/
	u8 data_rate;          /**< Data rate of the received packet. Values: @ref RTW_RATE_1M, @ref RTW_RATE_2M, etc. */
	u8 channel;            /**< Channel on which the packet was received. */
	u8 *buf;               /**< Pointer to the buffer containing the received packet data. */
	u32 len;               /**< Length of the data in the buffer. */
};

/**
*@brief Describes the chosen promisc mode and its associated callback function.
*/
struct rtw_promisc_para {
	/** @brief Specify which packets to receive by setting filtering conditions.
		- @ref RTW_PROMISC_FILTER_ALL_PKT : No address filtering.
		- @ref RTW_PROMISC_FILTER_AP_ALL : Receive only packtets sent to or from the connected AP.
		*/
	u8 filter_mode;
	/** @brief Callback function to handle received packets.
	  * @param[in] pkt_info: Pointer to struct rtw_rx_pkt_info containing packet details.
	  * @return Determines whether the driver should continue processing the packet after user handling:
	  *  - @ref RTW_PROMISC_NEED_DRV_HDL : Driver continues processing the packet. Typically needed when STA remains connected.
	  *  - @ref RTW_PROMISC_BYPASS_DRV_HDL : Driver drops the packet. Typically used when STA does not need to remain connected.
	  */
	u8(*callback)(struct rtw_rx_pkt_info *pkt_info);
};

/**
*@brief Provide necessary parameters for set EDCA
*/
struct rtw_edca_param {
	u8	aci;  /**< AC_BE[0], AC_BK[1], AC_VI[2], AC_VO[3] */
	u8	aifsn; /**< Arbitration inter-frame space number,specifies the inter-frame interval (waiting time) before transmission: unit: (*slot_time), + sifs*/
	u8	cw_max; /**< Maximum contention window: unit: *slot_time */
	u8	cw_min; /**< Minimum contention window: unit: *slot_time */
	u16	txop_limit;/**< Indicates that a single MSDU or MMPDU in addition to a protection frame exchange can be transmitted at any rate*/
	u8	slot_time;/**< The slot time value mentioned in 802.11 specification in units, Recommended value: 20us for 2G band, 9us for 5G band[value 0 = use internal chipset default] */
};

/**
*@brief Provide optional parameters for improving tx performance in special scenario
*/

struct rtw_tx_advanced_cfg {
	u16 update_masks;             /**< Mask subfield. If a parameter is set, its corresponding bit in update_masks must also be set. @ref RTW_UPDATE_TXCFG_TX_LIFE_TIME_BEBK... */
	u16 pkt_lifetime_bebk;        /**< Packet lifetime in units of 256us for AC_BE/AC_BK. */
	u16 pkt_lifetime_vivo;        /**< Packet lifetime in units of 256us for AC_VI/AC_VO. */
	u16 tx_bcn_protect_time;      /**< A reserved period for Beacon TX, preventing other transmissions, unit:32us */
	u8 rx_bcn_protect_time;       /**< A reserved period for Beacon RX, preventing other transmissions, unit:2.048ms; */
	u8 rx_nav_update_th;          /**< Rx NAV (Network Allocation Vector) update threshold in units of 128us[can not tx during Rx NAV]. */
	u8 b_ignore_tx_nav : 1;       /**< Queue BKF not need to wait TX Nav finished. */
};

/**********************************************************************************************
 *                                     speaker structures
 *********************************************************************************************/
/**
 * @brief Union for different Wi-Fi speaker configuration settings.
 */
union rtw_speaker_set {
	struct rtw_speaker_init {
		u8 mode;                     /**< 0 for slave, 1 for master. */
		u8 relay_en : 1;             /**< Relay control. */
	} init; /**< For Wi-Fi speaker setting case @ref RTW_SPEAKER_SET_INIT.*/
	struct rtw_speaker_i2s {
		u8 port;           /**< Port selection for TSFT trigger: 0 for port 0, 1 for port 1. */
		u8 latch_period;   /**< Audio latch period: 0 for 4.096ms, 1 for 8.192ms. */
	} latch_i2s_count; /**< For Wi-Fi speaker setting case @ref RTW_SPEAKER_SET_LATCH_I2S_COUNT.*/
	struct rtw_speaker_tsf_timer {
		u8 enable;			/**< 1 for enable, 0 for disable. */
		u64 tsft;           /**< Unit us. */
		u8 port;           /**< Port selection for TSFT trigger: 0 for port 0, 1 for port 1. */
	} tsf_timer; /**< For Wi-Fi speaker setting case @ref RTW_SPEAKER_SET_TSF_TIMER.*/
};

/**
 * @brief Struct for latched value request.
 */
struct rtw_speaker_read_latch_req {
	u8 i2s_tx;	/**< 1 for requesting I2S TX counter, 0 for requesting I2S RX counter. */
};

/**
 * @brief Struct for latched value report.
 */
struct rtw_speaker_read_latch_rpt {
	u64 tsf_us;			/**< latched Wi-Fi TSFT us value */
	u16 tsf_ns;			/**< latched Wi-Fi TSFT ns value */
	u64 i2s_counter;	/**< latched I2S counter value */
};

/**********************************************************************************************
 *                                     csi structures
 *********************************************************************************************/
/**
  * @brief  Configuration parameters used for csi report.
  * @note  The mac_addr if not specified, the default value must be 0.
  */
struct rtw_csi_action_parm {
	u8 group_num;   /**< CSI info subcarrier decimation. val: @ref RTW_CSI_GROUP_NUM_1, @ref RTW_CSI_GROUP_NUM_2... */
	u8 accuracy;    /**< The format (bit length) of CSI raw data for both I and Q components. val: @ref RTW_CSI_ACCU_1BYTE, @ref RTW_CSI_ACCU_2BYTES */
	u8 alg_opt;     /**< Reserved. */
	u8 ch_opt;      /**< CSI acquisition from leagcy or non-legacy LTF. val: @ref RTW_CSI_CH_LEGACY, @ref RTW_CSI_CH_NON_LEGACY. */
	u8 csi_role;    /**< The CSI operation role, val: @ref RTW_CSI_OP_ROLE_TRX, @ref RTW_CSI_OP_ROLE_TX, @ref RTW_CSI_OP_ROLE_RX. */
	u8 mode;        /**< Mode for fetching CSI. val: @ref RTW_CSI_MODE_NORMAL, @ref RTW_CSI_MODE_RX_RESP*/
	u8 act;         /**< Enable CSI or configure CSI parameters. val: @ref RTW_CSI_ACT_EN, @ref RTW_CSI_ACT_CFG. */
	u16 trig_frame_mgnt;	/**< Management frame type(s) of rx CSI triggering frame. Used for @ref RTW_CSI_MODE_NORMAL. val: @ref RTW_CSI_TRIG_ASSOCREQ... */
	u16 trig_frame_ctrl;	/**< Control frame type(s) of rx CSI triggering frame. Used for @ref RTW_CSI_MODE_NORMAL. val: @ref RTW_CSI_TRIG_TRIGGER... */
	u16 trig_frame_data;	/**< Data frame type(s) of rx CSI triggering frame. Used for @ref RTW_CSI_MODE_NORMAL. val: @ref RTW_CSI_TRIG_DATA... */
	u8 enable;      /**< 0: disable Wi-Fi CSI function; 1: enable Wi-Fi CSI function. */
	u8 trig_period; /**< CSI sounding rate, unit: 320us (recommended value: 15~255). */
	u8 data_rate; /**< Tx data rate of CSI triggering frame, invalid in @ref RTW_CSI_MODE_RX_RESP mode. val: only support OFDM/HT rate. */
	u8 data_bw;   /**< The bandwidth of CSI triggering frame, invalid in @ref RTW_CSI_MODE_RX_RESP mode. val: 0 for 20MHz, 1 for 40MHz (only supported by dplus). */
	u8 mac_addr[6]; /**< Destination address (MAC address) for CSI triggering frame, invalid when set `multi_type = 1`.*/
	u8 multi_type;  /**< 0: unicast csi triggering frame; 1: broadcast csi triggering frame. Valid in Active CSI. */

	/** Indicate role for transmitting CSI triggering frame in METHOD4 and
	 * role for transmitting response ACK for CSI triggering frame in METHOD1_Variant,
	 * others are reserved. \n
	 * Value=1 ~ 15 (0 is reserved)*/
	u8 trig_flag;
};


/**********************************************************************************************
 *                                     other structures
 *********************************************************************************************/
/**
  * @brief  Power limit regulation map.
  */
struct _pwr_lmt_regu_remap {
	u8	domain_code;
	u8	PwrLmtRegu_2g;	/**< Not distinguish 2.4G and 5G; just set PwrLmtRegu_2g. */
	u8	PwrLmtRegu_5g;
};

/**
  * @brief  Describes a raw frame to be transmitted.
  */
struct rtw_raw_frame_desc {
	u8 wlan_idx;      /**< Index of the Wi-Fi interface for transmission.  */
	u8 device_id;     /**< Reserved for internal driver use; users can ignore.*/
	u8 *buf;          /**< Poninter of buf where raw data is stored.*/
	u16 buf_len;      /**< Length of raw data (including MAC header and frame body).*/
	u8 tx_rate;	     /**< Tx rate of tx_raw packets. Val: @ref RTW_RATE_1M, @ref RTW_RATE_2M, etc.*/
	u8 retry_limit;  /**< Number of tx retry if sending the raw packet fails. */
	u8 ac_queue;      /**< Access Category Queue: 0/3 for BE, 1/2 for BK, 4/5 for VI, 6/7 for VO. */
	u8 sgi : 1;       /**< Short Guard Interval: 1 to enable, 0 to disable. */
	u8 agg_en : 1;    /**< Frame Aggregation: 1 to enable, 0 to disable for tx raw frames. */
	u8 bw_40_en : 1;    /**< Bandwidth: 1 to 40M, 0 to 20M. */
};

/**
 * @brief  Sets the custom Information Element (IE) for Wi-Fi frames.
 *
 */
struct rtw_custom_ie {
	/** IE Format:
	 * <table>
	 * <tr><th>1byte</th><th>1byte</th><th>length bytes</th></tr>
	 * <tr><td>element ID</td><td>length</td><td>content</td></tr>
	 * </table>
	 */
	u8 *ie;
	u8 type;    /**< The type of TX frame to attach the custom IE: @ref RTW_CUS_IE_PROBEREQ, @ref RTW_CUS_IE_PROBERSP, etc.*/
};

/**
 * @brief  Describes a single WiFi channel's information.
 */

struct rtw_channel_info {
	u8 channel;      /**< Channel number */
	u8 scan_type;    /**< Scan type: 0 for active, 1 for passive */
};

/**
 * @brief  Contains a list of available WiFi channels.
 */
struct rtw_channel_list {
	u8 ch_num;         /**< Number of available channel in the list.    */
	struct rtw_channel_info ch_info[RTW_MAX_CHANNEL_NUM]; /**< Array of channel information */
};


/**
 * @brief  Describes channel plan and country code.
 */
struct rtw_country_code_table {
	u8 char2[2];   /**< Country code. */
	u8 channel_plan; /**< Channel plan code. */
	u8 pwr_lmt;      /**< Tx power limit index. */
};

/**
 * @brief Controls transmit power settings.
 */
struct rtw_tx_power_ctl_info {
	s8	tx_pwr_force; /**< User-specified TX power for all rates, in units of 0.25 dBm.*/
	u8	b_tx_pwr_force_enbale : 1; /**< 1 for enable, 0 for disable. */
};

/**
 * @brief Configuration for Automatic Channel Selection (ACS).
 */
struct rtw_acs_config {
	u8 band; /**< Frequency band: @ref RTW_SUPPORT_BAND_2_4G, etc. */
};

/**
 * @brief Informations of RMesh node.
 */
struct rtw_rmesh_node_info {
	u8 mac[6]; /**< MAC addressof node.*/
	u8 layer; /**< layer of node.*/
	u8 is_rnat; /**< 1 for node is rnat, 0 for node is not rnat.*/
};

/** @} End of WIFI_Exported_Structure_Types group*/
/** @} End of WIFI_Exported_Types group*/
/** @} End of WIFI_API group */

/* not included in any api groups*/
extern  struct wifi_user_conf wifi_user_config;
extern struct rtw_wifi_setting wifi_setting[2];

#ifdef __cplusplus
}
#endif

#endif // __WIFI_API_TYPES_H
