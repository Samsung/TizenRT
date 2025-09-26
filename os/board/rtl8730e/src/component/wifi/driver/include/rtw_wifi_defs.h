/**
  ******************************************************************************
  * @file    rtw_wifi_defs.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef	__RTW_WIFI_DEFS_H_
#define __RTW_WIFI_DEFS_H_

#if defined (__ICCARM__)
#ifndef _WEAK
#define _WEAK           __weak
#endif
#else
#define _WEAK           __attribute__ ((weak))
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

#ifndef NAN_WLAN_INDEX
#define NAN_WLAN_INDEX	2
#endif

#ifndef STA_WLAN_NAME
#define STA_WLAN_NAME	"wlan0"
#endif

#ifndef SOFTAP_WLAN_NAME
#define SOFTAP_WLAN_NAME	"wlan1"
#endif

#ifndef NAN_WLAN_NAME
#define NAN_WLAN_NAME	"wlan2"
#endif

#define IsSupported24G(band_type) ((band_type) & BAND_CAP_2G? _TRUE : _FALSE)
#define IsSupported5G(band_type) ((band_type) & BAND_CAP_5G ?  _TRUE : _FALSE)

#define IsLegacyOnly(NetType)  ((NetType) == ((NetType) & (WLAN_MD_11BG | WLAN_MD_11A)))

#define IsSupportedTxOFDM(NetType) ((NetType) & (WLAN_MD_11G | WLAN_MD_11A) ? _TRUE : _FALSE)

#define is_supported_ht(NetType) ((NetType) & (WLAN_MD_11N) ? _TRUE : _FALSE)
#define is_supported_vht(NetType) ((NetType) & (WLAN_MD_11AC) ? _TRUE : _FALSE)
#define is_supported_he(NetType) ((NetType) & (WLAN_MD_11AX) ? _TRUE : _FALSE)

#define IS_HT_RATE(_rate)				(((_rate) & 0x80) ? _TRUE : _FALSE)
#define IS_CCK_RATE(_rate) 				(MGN_1M == _rate || _rate == MGN_2M || _rate == MGN_5_5M || _rate == MGN_11M )
#define IS_OFDM_RATE(_rate)				(MGN_6M <= _rate && _rate <= MGN_54M )

/**
 * @brief  mac address format.
 */
#define MAC_ARG(x)		((u8*)(x))[0],((u8*)(x))[1],\
				((u8*)(x))[2],((u8*)(x))[3],\
				((u8*)(x))[4],((u8*)(x))[5]
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"

#define is_broadcast_mac_addr(Addr) ((((Addr[0]) & 0xff) == 0xff) && (((Addr[1]) & 0xff) == 0xff) && \
(((Addr[2]) & 0xff) == 0xff) && (((Addr[3]) & 0xff) == 0xff) && (((Addr[4]) & 0xff) == 0xff) && \
(((Addr[5]) & 0xff) == 0xff))
#define is_zero_mac_addr(Addr)	((Addr[0] == 0x00) && (Addr[1] == 0x00) && (Addr[2] == 0x00) &&   \
                    (Addr[3] == 0x00) && (Addr[4] == 0x00) && (Addr[5] == 0x00))

/**
  * @}
  */


/** @defgroup Security_Defs
   *@{
   */
#define AUTH_ALG_OPEN_SYSTEM	0x1
#define AUTH_ALG_SHARED_KEY	0x2
#define AUTH_ALG_SAE		0x8
#define AUTH_ALG_LEAP		0x00000004

#define WEP_ENABLED		0x0001		/**< wep enable */
#define TKIP_ENABLED		0x0002		/**< tkip enable */
#define AES_ENABLED		0x0004		/**< aes enable */
#define WSEC_SWFLAG		0x0008		/**< WSEC SWFLAG */
#define AES_CMAC_ENABLED	0x0010		/**< aes cmac enable */
#define ENTERPRISE_ENABLED	0x0020		/**< enterprise enable */
#define OWE_ENABLED		0X0040		/**< owe enable */
#define SHARED_ENABLED		0x00008000	/**< shared enable */
#define WPA_SECURITY		0x00200000	/**< wpa */
#define WPA2_SECURITY		0x00400000	/**< wpa2 */
#define WPA3_SECURITY		0x00800000	/**< wpa3 */
#define WPS_ENABLED		0x10000000	/**< wps  enable*/

/*cipher suite from 802.11-2016 p884*/
#define WIFI_CIPHER_SUITE_WEP_40			0x000FAC01
#define WIFI_CIPHER_SUITE_TKIP				0x000FAC02
#define WIFI_CIPHER_SUITE_CCMP_128		0x000FAC04
#define WIFI_CIPHER_SUITE_WEP_104			0x000FAC05
#define WIFI_CIPHER_SUITE_BIP_CMAC_128	0x000FAC06
#define WIFI_CIPHER_SUITE_GCMP				0x000FAC08
#define WIFI_CIPHER_SUITE_GCMP_256		0x000FAC09
#define WIFI_CIPHER_SUITE_CCMP_256		0x000FAC0A
#define WIFI_CIPHER_SUITE_BIP_GMAC_128	0x000FAC0B
#define WIFI_CIPHER_SUITE_BIP_GMAC_256	0x000FAC0C
#define WIFI_CIPHER_SUITE_BIP_CMAC_256	0x000FAC0D

#define RTW_WPA2_MAX_PSK_LEN		(64)
#define RTW_WPA3_MAX_PSK_LEN		(128)		/**< maxmum psk length */
#define RTW_MAX_PSK_LEN		RTW_WPA3_MAX_PSK_LEN
#define RTW_MIN_PSK_LEN		(8)		/**< minimum psk length */
#define MCSSET_LEN		16		/**<mcsset length */
#define RTW_OWE_KEY_LEN		32 /*32(Temporarily support group 19 with 256 bit public key)*/

/* adaptivity */
#define RTW_ADAPTIVITY_EN_DISABLE			0
#define RTW_ADAPTIVITY_EN_ENABLE			1

#ifndef CONFIG_WIFI_CRITICAL_CODE_SECTION
#define CONFIG_WIFI_CRITICAL_CODE_SECTION
#endif

#define RTW_SEND_AND_WAIT_ACK 				2
#define RTW_SEND_BY_HIGH_RATE				4 // IEEE80211_OFDM_RATE_54MB
#define RTW_NAV_BY_USER						8

/**
  * @}
  */

/** @defgroup Packet_Defs
   *@{
   */
#define	PACKET_NORMAL			0
#define	PACKET_DHCP			1
#define	PACKET_ARP			2
#define	PACKET_EAPOL			3

/**
  * @}
  */


/** @defgroup WPA_MODE_CHECK_Defs
   *@{
   */
#define WPA2_MODE_CHECK(wpa_mode) ((wpa_mode == WPA_AUTO_MODE) || (wpa_mode == WPA2_ONLY_MODE) || (wpa_mode == WPA3_ONLY_MODE) || \
									(wpa_mode == WPA_WPA2_MIXED_MODE) || (wpa_mode == WPA2_WPA3_MIXED_MODE))
#define WPA_MODE_CHECK(wpa_mode) ((wpa_mode == WPA_AUTO_MODE) ||(wpa_mode == WPA_ONLY_MODE) ||(wpa_mode == WPA_WPA2_MIXED_MODE))

/**
  * @}
  */


/**
  * @}
  */

/** @defgroup WIFI_Exported_Types WIFI Exported Types
* @{
*/

/** @addtogroup Enums
   *@{
   */

/* struct iw_encode_ext ->alg */
enum RTW_IW_ENC_ALG {
	RTW_ENCODE_ALG_NONE = 0,
	RTW_ENCODE_ALG_WEP,
	RTW_ENCODE_ALG_TKIP,
	RTW_ENCODE_ALG_CCMP,
	RTW_ENCODE_ALG_PMK,
	RTW_ENCODE_ALG_AES_CMAC //IGTK
};



/* Modes of operation */
enum RTK_IW_MODE {
	RTW_MODE_INFRA,			/* Multi cell network, roaming, ... */
	RTW_MODE_MASTER			/* Synchronisation master or Access Point */
};

/**
* @brief  The enumeration lists the promisc levels.
*/
enum rtw_promisc_level {
	RTW_PROMISC_DISABLE = 0,	/**< Disable the promisc */
	RTW_PROMISC_ENABLE = 1,	/**< Fetch all ethernet packets */
	RTW_PROMISC_ENABLE_1 = 2, /**< Fetch only B/M packets */
	RTW_PROMISC_ENABLE_2 = 3, /**< Fetch all 802.11 packets*/
	RTW_PROMISC_ENABLE_3 = 4, /**< Fetch only B/M 802.11 packets*/
	RTW_PROMISC_ENABLE_4 = 5, /**< Fetch all 802.11 packets & MIMO PLCP headers. Please note that the PLCP header would be struct rtw_rx_info_t defined in wifi_conf.h*/
	RTW_PROMISC_ENABLE_5 = 6 /**< Fetch all unicast 802.11 packets, data frame and beacon in same BSSID*/
};

/**
 * @brief The enumeration lists band type
 */
enum band_type {
	BAND_ON_24G	= 0,   /**< band is on 2.4G                          */
	BAND_ON_5G	= 1,   /**< band is on 5G                          */
	BAND_ON_6G	= 2,   /**< band is on 6G                          */
	BAND_MAX,                  /**< max band                          */
};

/**
 * @brief The enumeration lists wpa mode
 */
enum rtw_wpa_mode_type {
	WPA_AUTO_MODE,  /**< wpa auto mode                       */
	WPA_ONLY_MODE,   /**< wpa only mode                       */
	WPA2_ONLY_MODE, /**< wpa2 only mode                       */
	WPA3_ONLY_MODE, /**< wpa3 only mode                       */
	WPA_WPA2_MIXED_MODE, /**< wpa and wpa2  mixed mode                       */
	WPA2_WPA3_MIXED_MODE /**< wpa2 and wpa3  mixed mode                       */
};


/**
 * @brief The enumeration lists the possible security types to set when connection.\n
 *			Station mode supports OPEN, WEP, and WPA2.\n
 *			AP mode support OPEN and WPA2.
 */
enum rtw_security {
	RTW_SECURITY_OPEN               = 0,                                                            /**< Open security                           */
	RTW_SECURITY_WEP_PSK            = (WEP_ENABLED),                                                /**< WEP Security with open authentication   */
	RTW_SECURITY_WEP_SHARED         = (WEP_ENABLED | SHARED_ENABLED),                               /**< WEP Security with shared authentication */

	RTW_SECURITY_WPA_TKIP_PSK       = (WPA_SECURITY | TKIP_ENABLED),                                /**< WPA Security with TKIP                  */
	RTW_SECURITY_WPA_AES_PSK        = (WPA_SECURITY | AES_ENABLED),                                 /**< WPA Security with AES                   */
	RTW_SECURITY_WPA_MIXED_PSK      = (WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),                  /**< WPA Security with AES & TKIP            */
	RTW_SECURITY_WPA2_TKIP_PSK		= (WPA2_SECURITY | TKIP_ENABLED),								/**< WPA2 Security with TKIP				 */
	RTW_SECURITY_WPA2_AES_PSK       = (WPA2_SECURITY | AES_ENABLED),                                /**< WPA2 Security with AES                  */
	RTW_SECURITY_WPA2_MIXED_PSK     = (WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED),                 /**< WPA2 Security with AES & TKIP           */
	RTW_SECURITY_WPA_WPA2_TKIP_PSK  = (WPA_SECURITY | WPA2_SECURITY | TKIP_ENABLED),                /**< WPA/WPA2 Security with TKIP             */
	RTW_SECURITY_WPA_WPA2_AES_PSK   = (WPA_SECURITY | WPA2_SECURITY | AES_ENABLED),                 /**< WPA/WPA2 Security with AES              */
	RTW_SECURITY_WPA_WPA2_MIXED_PSK = (WPA_SECURITY  | WPA2_SECURITY | TKIP_ENABLED | AES_ENABLED), /**< WPA/WPA2 Security with AES & TKIP       */
	RTW_SECURITY_WPA3_AES_PSK	 = (WPA3_SECURITY | AES_ENABLED),				/**< WPA3-SAE with AES security			   */
	RTW_SECURITY_WPA3_OWE	 = (WPA3_SECURITY | OWE_ENABLED | AES_ENABLED),				/**< WPA3-OWE with AES security			   */
	RTW_SECURITY_WPA2_WPA3_MIXED = (WPA2_SECURITY | WPA3_SECURITY | AES_ENABLED), /**< WPA3-SAE/WPA2 with AES security		   */
	RTW_SECURITY_WPA2_AES_CMAC      = (WPA2_SECURITY | AES_CMAC_ENABLED),                           /**< WPA2 Security with AES and Management Frame Protection */

#ifdef CONFIG_PLATFORM_TIZENRT_OS
	RTW_SECURITY_WPA_TKIP_ENTERPRISE       = (WPA_SECURITY | TKIP_ENABLED | ENTERPRISE_ENABLED),                               /**< WPA Security with TKIP via 802.1X authentication            */
	RTW_SECURITY_WPA_AES_ENTERPRISE        = (WPA_SECURITY | AES_ENABLED | ENTERPRISE_ENABLED),                                /**< WPA Security with AES via 802.1X authentication             */
	RTW_SECURITY_WPA_MIXED_ENTERPRISE      = (WPA_SECURITY | AES_ENABLED | TKIP_ENABLED | ENTERPRISE_ENABLED),                 /**< WPA Security with AES & TKIP via 802.1X authentication      */
	RTW_SECURITY_WPA2_TKIP_ENTERPRISE      = (WPA2_SECURITY | TKIP_ENABLED | ENTERPRISE_ENABLED),                              /**< WPA2 Security with TKI via 802.1X authenticationP           */
	RTW_SECURITY_WPA2_AES_ENTERPRISE       = (WPA2_SECURITY | AES_ENABLED | ENTERPRISE_ENABLED),                               /**< WPA2 Security with AES via 802.1X authentication            */
	RTW_SECURITY_WPA2_MIXED_ENTERPRISE     = (WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED | ENTERPRISE_ENABLED),                /**< WPA2 Security with AES & TKIP via 802.1X authentication     */
	RTW_SECURITY_WPA_WPA2_TKIP_ENTERPRISE  = (WPA_SECURITY | WPA2_SECURITY | TKIP_ENABLED | ENTERPRISE_ENABLED),               /**< WPA/WPA2 Security with TKIP via 802.1X authentication       */
	RTW_SECURITY_WPA_WPA2_AES_ENTERPRISE   = (WPA_SECURITY | WPA2_SECURITY | AES_ENABLED | ENTERPRISE_ENABLED),                /**< WPA/WPA2 Security with AES via 802.1X authentication        */
	RTW_SECURITY_WPA_WPA2_MIXED_ENTERPRISE = (WPA_SECURITY | WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED | ENTERPRISE_ENABLED), /**< WPA/WPA2 Security with AES & TKIP via 802.1X authentication */

	RTW_SECURITY_WPS_OPEN        = (WPS_ENABLED),                                 /**< WPS with open security                  */
	RTW_SECURITY_WPS_SECURE      = (WPS_ENABLED | AES_ENABLED),                   /**< WPS with AES security                   */

	RTW_SECURITY_WPA3_ENTERPRISE = (WPA3_SECURITY | ENTERPRISE_ENABLED),          /**< WPA3 Security via 802.1X authentication */
	RTW_SECURITY_UNKNOWN         = -1,                                            /**< May be returned by scan function if security is unknown. Do not pass this to the join function! */

	RTW_SECURITY_FORCE_32_BIT    = 0x7fffffff                                     /**< Exists only to force rtw_security_t type to 32 bits */
#endif //CONFIG_PLATFORM_TIZENRT_OS
};

/**
 * @brief The enumeration lists encryption types
 */
enum rtw_encryption_type {
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
};

enum WIFI_STATUS_CODE {
	_STATS_SUCCESSFUL_			= 0,
	_STATS_FAILURE_				= 1,
	_STATS_CAP_FAIL_			= 10,
	_STATS_NO_ASOC_				= 11,
	_STATS_OTHER_				= 12,
	_STATS_NO_SUPP_ALG_			= 13,
	_STATS_OUT_OF_AUTH_SEQ_		= 14,
	_STATS_CHALLENGE_FAIL_		= 15,
	_STATS_AUTH_TIMEOUT_		= 16,
	_STATS_UNABLE_HANDLE_STA_	= 17,
	_STATS_RATE_FAIL_			= 18,
	_STATS_REFUSED_TEMPORARILY_ = 30,
	_STATS_INVALID_PMKID_		= 53,
	_STATS_SAE_HASH_TO_ELEMENT_ = 126,
};

/**
  * @brief The enumeration lists the BIT 7 HT Rate.
  */
enum mgn_rate_type {
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
};

/**
  * @brief csi trig management frame subtype.
  */
enum trig_frame_mgnt_type {
	CSI_TRIG_ASSOCREQ   = BIT(0),
	CSI_TRIG_ASSOCRSP   = BIT(1),
	CSI_TRIG_REASSOCREQ = BIT(2),
	CSI_TRIG_REASSOCRSP = BIT(3),
	CSI_TRIG_PROBEREQ   = BIT(4),
	CSI_TRIG_PROBERSP   = BIT(5),
	CSI_TRIG_BEACON     = BIT(8),
	CSI_TRIG_ATIM       = BIT(9),
	CSI_TRIG_DISASSOC   = BIT(10),
	CSI_TRIG_AUTH       = BIT(11),
	CSI_TRIG_DEAUTH     = BIT(12),
	CSI_TRIG_ACTION     = BIT(13)
};

/**
  * @brief csi trig control frame subtype.
  */
enum trig_frame_ctrl_type {
	CSI_TRIG_TRIGGER     = BIT(2),
	CSI_TRIG_BA          = BIT(9),
	CSI_TRIG_PSPOLL      = BIT(10),
	CSI_TRIG_RTS         = BIT(11),
	CSI_TRIG_CTS         = BIT(12),
	CSI_TRIG_ACK         = BIT(13),
	CSI_TRIG_CFEND       = BIT(14),
	CSI_TRIG_CFEND_CFACK = BIT(15)
};

/**
  * @brief csi trig data frame subtype.
  */
enum trig_frame_data_type {
	CSI_TRIG_DATA           = BIT(0),
	CSI_TRIG_DATA_CFACK     = BIT(1),
	CSI_TRIG_DATA_CFPOLL    = BIT(2),
	CSI_TRIG_DATA_CFACKPOLL = BIT(3),
	CSI_TRIG_DATA_NULL      = BIT(4),
	CSI_TRIG_CF_ACK         = BIT(5),
	CSI_TRIG_CF_POLL        = BIT(6),
	CSI_TRIG_CF_ACKPOLL     = BIT(7),
	CSI_TRIG_QOS_DATA       = BIT(8),
	CSI_TRIG_QOS_DATA_NULL	= BIT(12)
};

/**
  * @brief csi enable or config
  */
enum rtw_csi_action_type {
	CSI_ACT_EN,    /**< enable or disable csi func */
	CSI_ACT_CFG,  /**< config csi parameters */
	CSI_ACT_MAX
};

/**
  * @brief csi group num
  */
enum rtw_csi_group_type {
	CSI_GROUP_NUM_1 = 0,  /**< per tone */
	CSI_GROUP_NUM_2,         /**< per 2tone */
	CSI_GROUP_NUM_4,        /**< per 4tone */
	CSI_GROUP_NUM_16,     /**< per 16tone */
	CSI_GROUP_NUM_MAX
};

/**
  * @brief csi mode
  */
enum rtw_csi_mode_type {
	CSI_MODE_NORMAL = 0,   ///<   normal mode
	CSI_MODE_NDP,                ///<   ndp mode
	CSI_MODE_RX_RESP,         ///<    rx rsp mode
	CSI_MODE_MAX,                 ///<    max mode
};

/**
  * @brief csi accuracy.
  */
enum rtw_csi_accuracy_type {
	CSI_ACCU_1BYTE = 0, /**< CSI_ACCU_1BYTE: S(8,4) */
	CSI_ACCU_2BYTES,  /**< CSI_ACCU_2BYTE: S(16,12) */
	CSI_ACCU_MAX
};

/**
  * @brief csi alg_opt.
  */
enum rtw_csi_alg_opt_type {
	CSI_ALG_LS = 0,  /**< ls algo */
	CSI_ALG_SMOTHING, /**< smothing algo */
	CSI_ALG_MAX            /**< other algo */
};

/**
  * @brief csi ch_opt.
  */
enum rtw_csi_ch_opt_type {
	CSI_CH_LEGACY = 0, /**< legacy part(L-LTF) channel estmation result */
	CSI_CH_NON_LEGACY,  /**< non-legacy(HT-LTF) part */
	CSI_CH_MAX
};

/**
  * @brief csi play_role for sta mode.
  */
enum rtw_csi_op_role {
	CSI_OP_ROLE_TRX = 0,  /**< both trx */
	CSI_OP_ROLE_TX  = 1,  /**< only tx csi triggering frame */
	CSI_OP_ROLE_RX  = 2,  /**< only rx csi triggering frame for fetching csi report */
	CSI_OP_ROLE_MAX
};

/**
  * @brief The enumeration lists the power status.
  */
enum rtw_tx_pwr_percentage {
	RTW_TX_PWR_PERCENTAGE_100	= 0, /**< 100%, default target output power.	 */
	RTW_TX_PWR_PERCENTAGE_75	= 1, /**< 75% */
	RTW_TX_PWR_PERCENTAGE_50	= 2, /**< 50% */
	RTW_TX_PWR_PERCENTAGE_25	= 3, /**< 25% */
	RTW_TX_PWR_PERCENTAGE_12_5	= 4, /**< 12.5% */
};

/**
  * @brief Power Mgnt
  */
enum power_mgnt_mode {
	PS_MODE_ACTIVE	= 0	, ///< active mode
	PS_MODE_LEGACY		,       ///< legacy mode
	PS_MODE_UAPSD_WMM	, ///< uapsd wmm mode
};

/**
* @brief The enumeration lists the trp tis types.
*/
enum rtw_trp_tis_mode {
	RTW_TRP_TIS_DISABLE = 0,
	RTW_TRP_TIS_NORMAL = 1,
	RTW_TRP_TIS_DYNAMIC = 3,					///< enable dynamic mechanism
	RTW_TRP_TIS_FIX_ACK_RATE = 5,			///< fix ack rate to 6M
	RTW_TRP_TIS_FIX_PHY_ACK_HIGH_RATE = 9	///< fix phy ack rate to RATE_54M | RATE_48M | RATE_36M | RATE_24M | RATE_18M | RATE_12M | RATE_9M | RATE_6M
};

/**
* @brief The enumeration lists the edcca mode types.
*/
enum rtw_edcca_mode {
	RTW_EDCCA_NORM	= 0, /* normal */
	RTW_EDCCA_ADAPT	= 1, /* adaptivity */
	RTW_EDCCA_CS	= 2, /* carrier sense */
	RTW_EDCCA_DISABLE	= 9, /* disable */
};

/**
* @brief The enumeration lists the antdiv mode types.
*/
enum rtw_antdiv_mode {
	RTW_ANTDIV_AUTO     = 0,    /* auto antdiv */
	RTW_ANTDIV_FIX_MAIN = 1,    /* fix main ant */
	RTW_ANTDIV_FIX_AUX  = 2,    /* fix aux ant */
	RTW_ANTDIV_DISABLE  = 0xF,  /* disable antdiv*/
};

/**
* @brief The enumeration lists band types
*/
enum rtw_802_11_band {
	RTW_802_11_BAND_5GHZ   = 0, /**< Denotes 5GHz radio band */
	RTW_802_11_BAND_2_4GHZ = 1,  /**< Denotes 2.4GHz radio band */
	RTW_802_11_BAND_NOUSE = 0xFFFFFFFF
};

/**
  * @brief  The enumeration lists the bss types.
  */
enum rtw_bss_type {
	RTW_BSS_TYPE_INFRASTRUCTURE 	= 0, /**< Denotes infrastructure network                  */
	RTW_BSS_TYPE_ADHOC          		= 1, /**< Denotes an 802.11 ad-hoc IBSS network           */
	RTW_BSS_TYPE_ANY            			= 2, /**< Denotes either infrastructure or ad-hoc network */
	RTW_BSS_TYPE_UNKNOWN        		= 0xFFFFFFFF /**< May be returned by scan function if BSS type is unknown. Do not pass this to the Join function */
};

/**
  * @brief  The enumeration lists the scan options.
  */
enum rtw_scan_option {
	RTW_SCAN_NOUSE			= 0x00,  /**< default value */
	RTW_SCAN_ACTIVE              	= 0x01,     /**< active scan */
	RTW_SCAN_PASSIVE             	= 0x02,    /**< passive scan*/
	RTW_SCAN_NO_HIDDEN_SSID	= 0x04, /**< Filter hidden ssid APs*/
	RTW_SCAN_REPORT_EACH	= 0x08,    /**< report each */
	RTW_SCAN_WITH_P2P		= 0x10,    /**< for P2P usage */
	/* TizenRT customization: scan multiple SSIDs */
#ifdef CONFIG_PLATFORM_TIZENRT_OS
	RTW_SCAN_ALL			= 0x20	/**< Scan specific AP and also report other APs responding to NULL probe req */
#endif //CONFIG_PLATFORM_TIZENRT_OS
};

/**
  * @brief  The enumeration lists the WPS types.
  */
enum rtw_wps_type {
	RTW_WPS_TYPE_DEFAULT 		    		= 0x0000,	/**< default type */
	RTW_WPS_TYPE_USER_SPECIFIED 		= 0x0001,	/**< user specified type */
	RTW_WPS_TYPE_MACHINE_SPECIFIED   	= 0x0002,	/**< machine specified type */
	RTW_WPS_TYPE_REKEY 			        	= 0x0003,	/**< retry key type */
	RTW_WPS_TYPE_PUSHBUTTON 		    	= 0x0004,	/**< push button type */
	RTW_WPS_TYPE_REGISTRAR_SPECIFIED 	= 0x0005,	/**< register specified type */
	RTW_WPS_TYPE_NONE                   		= 0x0006, 	/**< none */
	RTW_WPS_TYPE_WSC                    		= 0x0007,	/**< wsc type */
	RTW_WPS_TYPE_NOUSE					= 0xffffffff		/**< unsed type */
};

/**
  * @brief The enumeration lists the join status.
  */
enum rtw_join_status_type {
	RTW_JOINSTATUS_UNKNOWN = 0,				/**< unknown */

	/* The intermediate states of Linking should be added
		in front of RTW_JOINSTATUS_SUCCESS */
	RTW_JOINSTATUS_STARTING,				/**< starting phase */
	RTW_JOINSTATUS_SCANNING,				/**< scanning phase */
	RTW_JOINSTATUS_AUTHENTICATING,			/**< authenticating phase */
	RTW_JOINSTATUS_AUTHENTICATED,			/**< authenticated phase */
	RTW_JOINSTATUS_ASSOCIATING,				/**< associating phase */
	RTW_JOINSTATUS_ASSOCIATED,			 	/**< associated phase */
	RTW_JOINSTATUS_4WAY_HANDSHAKING,		/**< 4 way handshaking phase */
	RTW_JOINSTATUS_4WAY_HANDSHAKE_DONE,	/**< 4 way handshake done phase */
	RTW_JOINSTATUS_SUCCESS,					/**< join success  */

	/* The other result states of Linking should be added
		in back of RTW_JOINSTATUS_SUCCESS */
	RTW_JOINSTATUS_FAIL,						/**< join fail  */
	RTW_JOINSTATUS_DISCONNECT,				/**< disconnect */
};

/**
  * @brief The enumeration lists the supported operation mode by WIFI driver,
  *			including station and AP mode.
  */
enum rtw_mode_type {
	RTW_MODE_NONE	= 0,		///<none
	RTW_MODE_STA		= 0x0001,	///<sta mode
	RTW_MODE_AP		= 0x0002,	///<ap mode
	RTW_MODE_NAN		= 0x0004,	///<nan mode
	RTW_MODE_MAX
};

/**
  * @brief  The enumeration lists the supported autoreconnect mode by WIFI driver.
  */
enum rtw_autoreconnect_mode {
	RTW_AUTORECONNECT_DISABLE,  ///< disable auto reconnect
	RTW_AUTORECONNECT_FINITE,    ///< finite auto reconnect
	RTW_AUTORECONNECT_INFINITE ///< infinite auto reconnect
};

/**
  * @brief  The enumeration lists the interfaces.
  */
enum rtw_interface_type {
	RTW_STA_INTERFACE     = 0, /**<  STA or Client Interface  */
	RTW_AP_INTERFACE      = 1 /**<  SoftAP Interface         */
};

/* For freertos, core, hal, rf, halbb. */
enum phl_phy_idx {
	HW_PHY_0,
	HW_PHY_1,
	HW_PHY_MAX
};

/**
  * @brief  The enumeration lists the packet filter rules.
  */
enum rtw_packet_filter_rule {
	RTW_POSITIVE_MATCHING  = 0, /**< Receive the data matching with this pattern and discard the other data   */
	RTW_NEGATIVE_MATCHING  = 1  /**< Discard the data matching with this pattern and receive the other data */
};

/**
  * @brief channel switch
  */
enum rtw_channel_switch_res {
	RTW_CH_SWITCH_FAIL = -1,           ///<channel switch fail
	RTW_CH_SWITCH_SUCCESS = 0,      ///<channel switch success
};

/**
  * @brief  The enumeration is event type indicated from wlan driver.
  */
enum rtw_event_indicate {
	/* common event */
	WIFI_EVENT_STA_ASSOC = 0,		///<used in p2p, simple config, 11s, customer
	WIFI_EVENT_STA_DISASSOC,		///<used in p2p, customer
	WIFI_EVENT_RX_MGNT,				///<used in p2p, customer
	WIFI_EVENT_RX_MGNT_AP,
	WIFI_EVENT_EXTERNAL_AUTH_REQ,

	WIFI_EVENT_GROUP_KEY_CHANGED,
	WIFI_EVENT_RECONNECTION_FAIL,
	WIFI_EVENT_ICV_ERROR,
	WIFI_EVENT_CHALLENGE_FAIL,

	WIFI_EVENT_JOIN_STATUS,

	/* wpa event */
	WIFI_EVENT_WPA_STA_WPS_START,
	WIFI_EVENT_WPA_WPS_FINISH,
	WIFI_EVENT_WPA_EAPOL_START,
	WIFI_EVENT_WPA_EAPOL_RECVD,
	WIFI_EVENT_WPA_STA_4WAY_START,
	WIFI_EVENT_WPA_AP_4WAY_START,
	WIFI_EVENT_WPA_STA_4WAY_RECV,
	WIFI_EVENT_WPA_AP_4WAY_RECV,
	WIFI_EVENT_WPA_SET_PSK_INFO,
	/* owe event */
	WIFI_EVENT_OWE_START_CALC,
	WIFI_EVENT_OWE_PEER_KEY_RECV,
	/* kvr event */
#if defined(CONFIG_IEEE80211V) || defined(CONFIG_IEEE80211K) || defined(CONFIG_IEEE80211R)
	WIFI_EVENT_KVR_CAP_UPDATE,
#if defined(CONFIG_IEEE80211V) || defined(CONFIG_IEEE80211K)
	WIFI_EVENT_NB_RESP_RECV,
#endif
#ifdef CONFIG_IEEE80211V
	WIFI_EVENT_BTM_REQ_RECV,
	WIFI_EVENT_BTM_DEBUG_CMD,
#endif
#ifdef CONFIG_IEEE80211R
	WIFI_EVENT_FT_AUTH_START,
	WIFI_EVENT_FT_RX_MGNT,
#endif
#endif

	/* csi rx done event */
	WIFI_EVENT_CSI_DONE,

	/* flash event */
	WIFI_EVENT_DEAUTH_INFO_FLASH,
#if defined (CONFIG_CLINTWOOD) && CONFIG_CLINTWOOD
	WIFI_EVENT_RFK_NOTIFY,
	WIFI_EVENT_CHANNEL_NOT_SUPPORTED,
#endif
	WIFI_EVENT_MAX,
};

/**
  * The enumeration lists the flash operation status.
  */
enum flash_operation_type {
	FLASH_READ = 0,   /**< read  flash                       */
	FLASH_WRITE,       /**< write  flash                       */
};

/**
  * The enumeration lists the power save status.
  */
enum ips_level_type {
	IPS_WIFI_OFF = 0,
	IPS_WIFI_PG,
	IPS_LEVEL_MAX,
};

/**
  * The enumeration lists the p2p role type.
  */
enum rtw_p2p_role {
	P2P_ROLE_DISABLE = 0,
	P2P_ROLE_DEVICE = 1,
	P2P_ROLE_CLIENT = 2,
	P2P_ROLE_GO = 3
};

enum gen_ie_type {
	P2PWPS_PROBE_REQ_IE = 0,
	P2PWPS_PROBE_RSP_IE,
	P2PWPS_BEACON_IE,
	P2PWPS_ASSOC_REQ_IE,
	P2PWPS_ASSOC_RSP_IE
};

enum UAPSD_MAX_SP {
	NO_LIMIT,
	TWO_MSDU,
	FOUR_MSDU,
	SIX_MSDU
};

/**
  * @}
  */

/**
  * @}
  */

#endif
