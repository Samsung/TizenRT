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
 */

/**
  ******************************************************************************
  * @file    wifi_conf_basic.h
  * @author
  * @version
  * @brief   This file provides user interface for Wi-Fi station and AP mode configuration
  *             base on the functionalities provided by Realtek Wi-Fi driver.
  ******************************************************************************
  */
#ifndef __WIFI_CONF_BASIC_H
#define __WIFI_CONF_BASIC_H

/** @defgroup WIFI
 *  @brief      WIFI module
 *  @{
 */


#include "rtw_wifi_constants.h"
#include <platform_stdlib.h>
#include "wifi_intf_drv_to_bt.h"
#include "dlist.h"
#include <rtw_skbuff.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup WIFI_Exported_Constants WIFI Exported Constants
  * @{
  */

/** @defgroup API_INFO_Defs
   *@{
   */
/**
* @brief Create RTW_ENABLE_API_INFO
*/
#define RTW_ENABLE_API_INFO


/**
* @brief Create RTW_API_INFO
*/
#if defined RTW_ENABLE_API_INFO || defined __DOXYGEN__
#define RTW_API_INFO printf
#else
#define RTW_API_INFO(args)
#endif
/** @} */

/**
 * @brief The enumeration lists rcr mode under promisc
 */
typedef enum {
	RCR_ALL_PKT,
	RCR_AP_ALL
} promisc_rcr_mode;

typedef struct {
	promisc_rcr_mode filter_mode;
	void (*callback)(void *);
} promisc_para_t, *ppromisc_para_t;

/** @} */

/** @defgroup MAC_Defs
   *@{
   */
/**
* @brief compare mac address
*/
#define CMP_MAC( a, b)		(((a[0])==(b[0]))&& \
				((a[1])==(b[1]))&& \
				((a[2])==(b[2]))&& \
				((a[3])==(b[3]))&& \
				((a[4])==(b[4]))&& \
				((a[5])==(b[5])))

/**
* @brief mac format
*/
#define MAC_FMT			"%02x:%02x:%02x:%02x:%02x:%02x"
/** @} */


/** @defgroup WIFI_MAKEU32_Def
   *@{
   */
/**
* @brief Create u32_t value from bytes
*/
#define WIFI_MAKEU32(a,b,c,d)    (((u32_t)((a) & 0xff) << 24) | \
                               ((u32_t)((b) & 0xff) << 16) | \
                               ((u32_t)((c) & 0xff) << 8)  | \
                                (u32_t)((d) & 0xff))
/** @} */


/** @defgroup SCAN_Defs
   *@{
   */
/**
* @brief scan longest wait time
*/
#define SCAN_LONGEST_WAIT_TIME	(12000)

/**
* @brief enable for partial channel scan
*/
#define PSCAN_ENABLE		0x01

/**
* @brief set to select scan time to FAST_SURVEY_TO, otherwise SURVEY_TO
*/
#define PSCAN_FAST_SURVEY	0x02

/**
* @brief set to select scan time to FAST_SURVEY_TO and resend probe request
*/
#define PSCAN_SIMPLE_CONFIG	0x04
/** @} */


/** @defgroup BIT_Def
   *@{
   */
/**
* @brief define BIT
*/
#ifndef BIT
#define BIT(x)	((__u32)1 << (x))
#endif
/**
* @}
*/

/** @defgroup CUSTOM_IE_Def
   *@{
   */
/**
* @brief define _CUSTOM_IE_TYPE_
*/
#ifndef _CUSTOM_IE_TYPE_
#define _CUSTOM_IE_TYPE_
#endif

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
#ifndef _CUSTOM_IE_TYPE_
/**
 * @brief  The enumeration is transmission type for wifi custom ie.
 */
typedef enum {
	PROBE_REQ = BIT(0),  ///<probe request
	PROBE_RSP = BIT(1),  ///<probe response
	BEACON	  = BIT(2),     ///<beacon
	ASSOC_REQ = BIT(3), ///<assocation request
} rtw_custom_ie_type_t;
#endif

/**
 * @brief  The enumeration is wl band type.
 */
typedef enum  {
	WL_BAND_2_4G = 0,   ///<2.4g band
	WL_BAND_5G,            ///<5g band
	WL_BAND_2_4G_5G_BOTH, ///<2.4g&5g band
	WL_BANDMAX  ///< max band
} WL_BAND_TYPE;
/**
 * @brief The enumeration lists the results of the function.
 */
typedef enum {
	RTW_SUCCESS                      = 0,    /**< Success */
	RTW_TIMEOUT                      = 2,    /**< Timeout */
	RTW_INVALID_KEY                  = 4,        /**< Invalid key */

	RTW_ERROR                        = -1,   /**< Generic Error */
	RTW_BADARG                       = -2,   /**< Bad Argument */
	RTW_BUSY                         = -16,  /**< Busy */
	RTW_NOMEM                        = -27,  /**< No Memory */
} rtw_result_t;
/**
  * @}
  */


/** @addtogroup Structs
   *@{
   */

/** @brief The structure is pkt info.
   *@{
   */
struct rx_pkt_info {
	s8 recv_signal_power;
	MGN_RATE data_rate;
	u8 channel;
	u8 *buf;
	u32 len;
};
/**
 * @brief  The structure is join block param.
 */
typedef struct {
	void				*join_sema;
	unsigned int		join_timeout;
	unsigned char		block;
} internal_join_block_param_t;

/**
 * @brief  The structure is crypt info.
 */
struct rtw_crypt_info {
	u8 pairwise;
	u8 mac_addr[6];
	u8 wlan_idx;
	u16 key_len;
	u8 key[32];
	u8 key_idx;
	u8 driver_cipher;
	u8 transition_disable_exist;
	u8 transition_disable_bitmap;
};

/**
 * @brief The enumeration lists the type of pmksa operations.
 */
enum  {
	PMKSA_SET = 0,
	PMKSA_DEL = 1,
	PMKSA_FLUSH = 2,
};

/**
 * @brief  The structure is pmksa ops.
 */
struct rtw_pmksa_ops_t {
	u8 ops_id;
	u8 wlan_idx;
	u8 pmkid[16];
	u8 mac_addr[6];
	u8 pmk[32];/*pmksa is maintained in NP when use wpa_lite*/
};

/**
  * @brief  The structure is used to describe the unique 6-byte MAC address.
  */
typedef struct {
	unsigned char octet[6]; /**< Unique 6-byte MAC address */
} rtw_mac_t;

/**
  * @brief  The structure is used to describe the rtw_client_list.
  */
typedef struct {
	unsigned int    count;         /**< Number of associated clients in the list    */
	rtw_mac_t mac_list[AP_STA_NUM];   /**< max length array of MAC addresses */
	signed char rssi_list[AP_STA_NUM];   /**< max length array of client rssi */
} rtw_client_list_t;

/**
  * @brief  The structure is used to describe the SSID.
  */
typedef struct {
	unsigned char len;     /**< SSID length */
	unsigned char val[33]; /**< SSID name (AP name)  */
} rtw_ssid_t;

typedef int (*wifi_do_fast_connect_ptr)(void);
typedef int (*write_fast_connect_info_ptr)(unsigned int data1, unsigned int data2);
typedef void (*ap_channel_switch_callback_t)(unsigned char channel, rtw_channel_switch_res_t ret);
typedef void (*p_wlan_autoreconnect_hdl_t)(rtw_security_t, char *, int, char *, int, int, char);
typedef void (*rtw_joinstatus_callback_t)(rtw_join_status_t join_status);

/**
  * @brief  The structure is used for fullmac to get wpa_supplicant's info for STA connect,
  */
typedef struct rtw_wpa_supp_connect {
	u8 rsnxe_ie[RSNXE_MAX_LEN];
} rtw_wpa_supp_connect_t;

/**
  * @brief  The structure is used to describe the setting about SSID,
  *			security type, password and default channel, used to start AP mode.
  * @note  The data length of string pointed by ssid should not exceed 32,
  *        and the data length of string pointed by password should not exceed 64.
  */
typedef struct {
	rtw_ssid_t 			ssid;
	unsigned char		hidden_ssid;
	rtw_security_t		security_type;
	unsigned char 		*password;
	unsigned char 		password_len;
	unsigned char		channel;
} rtw_softap_info_t;

/**
  * @brief	The structure is used to describe the connection setting about SSID,
  * 		security type and password,etc., used when connecting to an AP.
  * @note  The data length of string pointed by ssid should not exceed 32,
  * 	   and the data length of string pointed by password should not exceed 64.
  * @note  If this struct is used for wifi connect, the channel is used to config
  * 	   whether it is a full channel scan(when channel is set to 0), or it will
  * 	   only scan one channel(do active scan on the configured channel).
  * @note  pscan_option set to PSCAN_FAST_SURVEY means do fast survey on the specified channel
  * 	   set to 0 means do normal scan on the specified channel or full channel.
  */
typedef struct {
	rtw_ssid_t					ssid;
	rtw_mac_t					bssid;
	rtw_security_t				security_type;
	unsigned char				*password;
	int 						password_len;
	int 						key_id;
	unsigned char				channel;		/**< set to 0 means full channel scan, set to other value means only scan on the specified channel */
	unsigned char				pscan_option;	/**< used when the specified channel is set, set to 0 for normal partial scan, set to PSCAN_FAST_SURVEY for fast survey*/
	unsigned char 				is_wps_trigger;	/**< connection triggered by WPS process**/
	rtw_joinstatus_callback_t	joinstatus_user_callback;	/**< user callback for processing joinstatus, please set to NULL if not use it */
	rtw_wpa_supp_connect_t	wpa_supp;
	rtw_mac_t		prev_bssid;
} rtw_network_info_t;

/**
  * @brief  The structure is used to store the WIFI setting gotten from WIFI driver.
  */
typedef struct {
	rtw_mode_t			mode;   /**< the mode of current wlan interface */
	unsigned char 		ssid[33];   /**< the ssid of connected AP or softAP */
	unsigned char		bssid[6];   /**< the bssid of connected AP or softAP */
	unsigned char		channel;
	rtw_security_t		security_type;   /**< the security type of connected AP or softAP */
	unsigned char 		password[RTW_MAX_PSK_LEN];   /**< the password of connected AP or softAP */
	unsigned char		key_idx;
	unsigned char		iw_mode;	/**< RTK_IW_MODE */
	unsigned char		alg;		/**< encryption algorithm */
	unsigned int		auth_type;
	unsigned char		is_wps_trigger;	/**< connection triggered by WPS process**/
} rtw_wifi_setting_t;

extern rtw_wifi_setting_t wifi_setting[2];

/**
  * @brief  The structure is used to describe the wifi user configuration
  */
struct  wifi_user_conf {
	unsigned char rtw_adaptivity_en;
	unsigned char rtw_adaptivity_mode;
	unsigned char rtw_adaptivity_th_l2h_ini;

	unsigned char rtw_tx_pwr_lmt_enable;	///< 0: disable, 1: enable, 2: Depend on efuse(flash)
	unsigned char rtw_tx_pwr_by_rate;	///< 0: disable, 1: enable, 2: Depend on efuse(flash)
	unsigned char rtw_trp_tis_cert_en;

	rtw_wpa_mode wifi_wpa_mode;
	unsigned char tdma_dig_enable;	///0:bb tdma dig on off, 1:bb tdma dig on

	unsigned char g_user_ap_sta_num;

	/* power save */
	unsigned char lps_dtim;
	unsigned char lps_enter_threshold;
	unsigned char rtw_power_mgnt;
	unsigned char rtw_lps_level;
	unsigned char smart_ps;
	unsigned char rtw_ips_level;
	unsigned char ips_ctrl_by_usr;
#ifdef CONFIG_WMMPS_STA
	unsigned char	uapsd_enable;
	unsigned char	uapsd_max_sp_len;
	unsigned char	uapsd_ac_enable;
#endif /* CONFIG_WMMPS_STA */

	/* AP */
	unsigned char bForwardingDisabled;

	unsigned char bChDeauthDisabled;

	unsigned char bAcceptAddbaReq;
	unsigned char bIssueAddbaReq;	///< 0: disable issue addba request, 1: enable issue addba request

	unsigned char ampdu_factor;	///for vht capability IE

	unsigned char bCheckDestAddress; ///< 0: don't check dest mac and ip address for station, 1: check dest mac and ip address for station


	unsigned char ap_compatibilty_enabled;/*!< The ap_compatibilty_enabled is used to configure the wlan settings, each bit controls one aspect.
	bit 0: (0: follow 802.11 spec, do not issue deauth, 1(default): issue deauth in 1st REAUTH_TO to be compatible with ap)
	bit 1: (0: do not check beacon info to connect with AP with multiple SSID, 1(default): check beacon info)
	bit 2: (0(default): do not issue deauth at start of auth, 1: issue deauth at start of auth)
	bit 3: (0: do not switch WEP auth algo unless WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG, 1(default): switch WEP auth algo from shared key to open system in 1st REAUTH_TO)
	other bits: reserved
	*/

	unsigned char set_channel_api_rfk;

	unsigned char max_roaming_times;

	unsigned char ap_polling_sta;

	unsigned char channel_plan;

	unsigned char rtw_802_11d_en;

	/* for auto reconnect*/
	unsigned char auto_reconnect_count;
	unsigned char auto_reconnect_interval; ///< in sec

	/* wifi driver's trx buffer number, each skb occupies about 1.8K bytes of heap, a little difference between different chips*/
	int skb_num_np;///< In INIC mode for all traffics except tx data, In single core mode for all traffics*/
	int skb_num_ap;///< In INIC mode for tx data packtes, not used in single core mode*/


	unsigned char cts2self; /*!< for RTS/CTS */


	unsigned char en_mcc; /*!< for MCC */

	unsigned char tx_shortcut_enable;
	unsigned char rx_shortcut_enable;

	/* for Concurrent Mode */
	unsigned char concurrent_enabled;	 ///< 0: STA or SoftAP only at any time, 1: STA and SoftAP may run at the same time

	unsigned char rx_ampdu_num;
	unsigned char cfg80211;
};

extern  struct wifi_user_conf wifi_user_config;
/**
  * @brief  The structure is power limit regu map.
  */
typedef struct _pwr_lmt_regu_remap {
	unsigned char	domain_code;
	unsigned char	PwrLmtRegu_2g;
	unsigned char	PwrLmtRegu_5g;
} pwr_lmt_regu_remap;

/**
  * @brief  The structure is used to describe the sw statistics
  */
typedef struct { /* software statistics for tx and rx*/
	unsigned long   rx_packets;             /*!< total packets received       */
	unsigned long   tx_packets;             /*!<total packets transmitted    */
	unsigned long   rx_dropped;             /*!< no space in buffers    */
	unsigned long   tx_dropped;             /*!< no space available  */
	unsigned long   rx_bytes;               /*!< total bytes received         */
	unsigned long   tx_bytes;               /*!< total bytes transmitted      */
	unsigned long   rx_overflow;            /*!< rx fifo overflow count       */
	unsigned int    tx_tp_kbps;
	unsigned int    rx_tp_kbps;
	unsigned int    max_skbbuf_used_number; /*!< max skb buffer used number       */
	unsigned int    skbbuf_used_number;     /*!< current used skbbuf number       */
	unsigned int    max_skbdata_used_number;/*!< max skb data used number       */
	unsigned int    skbdata_used_number;    /*!< current used skbdata number       */
} rtw_sw_statistics_t;

/**
  * @brief  The structure is used to describe the phy statistics
  */
typedef struct {
	signed char	rssi;          /*!<average rssi in 1 sec (for STA mode) */
	signed char	beacon_rssi;          /*!<average beacon rssi in 1 sec (for STA mode) */
	signed char	snr;          /*!< average snr in 1 sec (not include cck rate, for STA mode)*/
	/* todo*/
	unsigned int	false_alarm_cck;
	unsigned int	false_alarm_ofdm;
	unsigned int	cca_cck;
	unsigned int	cca_ofdm;
	unsigned int	tx_retry;
	unsigned short	tx_drop;
	unsigned int	rx_drop;
	unsigned int	supported_max_rate;
} rtw_phy_statistics_t;

/**
* @brief channel scan time
*/
typedef struct {
	unsigned short active_scan_time;      /*!< active scan time per channel, units: millisecond, default is 100ms */
	unsigned short passive_scan_time;     /*!< passive scan time per channel, units: millisecond, default is 110ms */
} rtw_channel_scan_time_t;

/**
  * @brief  The structure is used to describe the scan result of the AP.
  */
typedef struct {
	rtw_ssid_t              SSID;             /**< Service Set Identification (i.e. Name of Access Point)                    */
	rtw_mac_t               BSSID;            /**< Basic Service Set Identification (i.e. MAC address of Access Point)       */
	signed short		                  signal_strength;  /**< Receive Signal Strength Indication in dBm. <-90=Very poor, >-30=Excellent */
	rtw_bss_type_t          bss_type;         /**< Network type                                                              */
	rtw_security_t          security;         /**< Security type                                                             */
	rtw_wps_type_t          wps_type;         /**< WPS type                                                                  */
	unsigned int                      channel;          /**< Radio channel that the AP beacon was received on                          */
	rtw_802_11_band_t       band;             /**< Radio band                                                                */
} rtw_scan_result_t;

typedef rtw_result_t (*scan_user_callback_t)(unsigned int ap_num, void *user_data);
typedef rtw_result_t (*scan_report_each_mode_user_callback_t)(rtw_scan_result_t *scanned_ap_info, void *user_data);

/**
  * @brief  The structure is used to describe the scan parameters used for scan,
  * @note  The data length of string pointed by ssid should not exceed 32,
  *        and the data length of string pointed by password should not exceed 64.
  */
typedef struct {
	rtw_scan_option_t						options;
	char									*ssid;
	unsigned char							*channel_list;
	unsigned char							channel_list_num;
	rtw_channel_scan_time_t 				chan_scan_time;
	unsigned short							max_ap_record_num;     /**< config the max number of recorded AP, when set to 0, use default value 64 */
	void									*scan_user_data;
	scan_user_callback_t					scan_user_callback;   /**< used for normal asynchronized mode */
	scan_report_each_mode_user_callback_t	scan_report_each_mode_user_callback; /*used for RTW_SCAN_REPORT_EACH mode */
} rtw_scan_param_t;

/**
  * @brief  The structure is used to describe the raw data description
  */
struct raw_frame_desc_t {
	unsigned char wlan_idx;      /**< index of wlan interface which will transmit */
	unsigned char *buf;          /**< poninter of buf where raw data is stored*/
	unsigned short buf_len;      /**< the length of raw data*/
	MGN_RATE tx_rate;
	unsigned char retry_limit;
} ;

/**
  * @brief  The structure is used to describe the data description
  */
typedef struct {
	unsigned char wlan_idx;      /**< index of wlan interface which will transmit */
	unsigned char *buf;          /**< poninter of buf where raw data is stored*/
	unsigned short buf_len;      /**< the length of raw data*/
	unsigned short flags;        /**< send options*/
} raw_data_desc_t;

/**
  * @brief  The structure is used to set WIFI packet filter pattern.
  */
typedef struct {
	unsigned short	offset;     ///< Offset in bytes to start filtering (referenced to the start of the ethernet packet)
	unsigned short	mask_size;  ///< Size of the mask in bytes
	unsigned char	*mask;       ///< Pattern mask bytes to be ANDed with the pattern eg. "\xff00" (must be in network byte order)
	unsigned char	*pattern;    ///< Pattern bytes used to filter eg. "\x0800"  (must be in network byte order)
} rtw_packet_filter_pattern_t;

/**
  * @brief  The structure is used to describe the 802.11 frame info
  */
typedef struct {
	unsigned short i_fc;
	unsigned short i_dur;
	unsigned char i_addr1[6];
	unsigned char i_addr2[6];
	unsigned char i_addr3[6];
	unsigned short i_seq;
	unsigned char bssid[6];
	unsigned char encrypt;
	signed char rssi;
#if (defined(CONFIG_UNSUPPORT_PLCPHDR_RPT) && CONFIG_UNSUPPORT_PLCPHDR_RPT)
	rtw_rx_type_t type;
#endif
} ieee80211_frame_info_t;

/**
  * @brief  The structure is used to describe the packet filter info
  */
typedef struct {
	char filter_id;
	rtw_packet_filter_pattern_t patt;
	rtw_packet_filter_rule_t rule;
	unsigned char enable;
} rtw_packet_filter_info_t;

/**
  * @brief  The structure is used to describe the mac filter list
  */
typedef struct {
	struct list_head node;
	unsigned char mac_addr[6];
} rtw_mac_filter_list_t;

/**
  * @brief  The structure is used to describe the cfg parameters used for csi report,
  * @note  The mac_addr if not specified, the default value must be 0.
  */
typedef struct {
	rtw_csi_group_num group_num;
	rtw_csi_mode mode;
	rtw_csi_action act;
	rtw_csi_accuracy accuracy;
	rtw_csi_alg_opt alg_opt;
	rtw_csi_ch_opt ch_opt;
	unsigned char enable;
	unsigned char trig_period;
	unsigned char data_rate;
	unsigned char data_bw;
	unsigned char mac_addr[6];
	unsigned char multi_type;  /* 0&1 for multi sta CSI */
} rtw_csi_action_parm_t;

/**
  * @brief  The structure is used to describe the cfg parameters used for channel switch announcement,
  */
typedef struct {
	unsigned char new_chl;
	unsigned char chl_switch_cnt;
	unsigned char action_type;	/* 0: unicast csa action, 1: broadcast csa action, other values: disable transmit csa action */
	unsigned char bc_action_cnt; /* indicate the number of broadcast csa actions to send for each beacon interval. only valid when action_type = 1*/
	ap_channel_switch_callback_t callback;
} rtw_csa_parm_t;

#if defined CONFIG_RTL8735B || defined __DOXYGEN__
/**
  * @brief  The structure is used to describe the wowlan pattern
  */
typedef struct {
	unsigned char eth_da[6];
	unsigned char eth_sa[6];
	unsigned char eth_proto_type[2];
	unsigned char header_len[1];
	unsigned char ip_proto[1];
	unsigned char ip_sa[4];
	unsigned char ip_da[4];
	unsigned char src_port[2];
	unsigned char dest_port[2];
	unsigned char flag2[1];
	unsigned char mask[6];
	unsigned char window[2];
	unsigned char checksum[2];
	unsigned char urgent_pointer[2];
	unsigned char payload[64];
	unsigned char payload_mask[9];
} wowlan_pattern_t;
#else

/**
  * @brief  The structure is used to describe the wowlan pattern
  */
typedef struct {
	unsigned char eth_da[6];
	unsigned char eth_sa[6];
	unsigned char eth_proto_type[2];
	unsigned char header_len[1];
	//unsigned char header_content[8];
	unsigned char ip_proto[1];
	//unsigned char checksum[2];
	unsigned char ip_sa[4];
	unsigned char ip_da[4];
	unsigned char src_port[2];
	unsigned char dest_port[2];
	unsigned char mask[5];
} wowlan_pattern_t;
#endif

//----------------------------
/* ie format
 * +-----------+--------+-----------------------+
 * |element ID | length | content in length byte|
 * +-----------+--------+-----------------------+
 *
 * type: refer to CUSTOM_IE_TYPE
 */
#ifndef _CUS_IE_
#define _CUS_IE_
/**
 * @brief  The structure is used to set WIFI custom ie list,
 * 	and type match CUSTOM_IE_TYPE.
 * 	The ie will be transmitted according to the type.
 */
typedef struct _cus_ie {
	__u8 *ie;
	__u8 type;
} rtw_custom_ie_t, *p_rtw_custom_ie_t;
#endif /* _CUS_IE_ */

/**
 * @brief  The structure is used to describe net device
 */
struct net_device {
	void			*priv;		/* pointer to private data */
	unsigned char		dev_addr[6];	/* set during bootup */
};

/**
 * @brief  The structure is used to describe wlan info
 */
typedef struct {
	struct net_device	dev;		/* Binding wlan driver netdev */
	void			*skb;		/* pending Rx packet */
	unsigned int		tx_busy;
	unsigned int		rx_busy;
	unsigned char		enable;
	unsigned char		mac[6];
	_sema			netif_rx_sema;	/* prevent race condition on .skb in rltk_netif_rx() */
} Rltk_wlan_t;

extern Rltk_wlan_t rltk_wlan_info[NET_IF_NUM];

#define netdev_priv(dev)		dev->priv
#define rtw_is_netdev_enable(idx)	(rltk_wlan_info[idx].enable)
#define rtw_get_netdev(idx)		(&(rltk_wlan_info[idx].dev))

/**
  * @}
  */

/**
* @}
*/



/** @defgroup WIFI_Exported_Functions WIFI Exported Functions
  * @{
  */



/** @defgroup Basic_Functions
  * @{
  */

/******************************************************
 *               Function Declarations
 ******************************************************/
/**
 * @brief  Enable Wi-Fi.
 * - Bring the Wireless interface "Up".
 * @param[in]  mode: Decide to enable WiFi in which mode.
 * 	The optional modes are RTW_MODE_STA, RTW_MODE_AP,
 * 	RTW_MODE_STA_AP, RTW_MODE_PROMISC.
 * @return  RTW_SUCCESS: if the WiFi chip initialized successfully.
 * @return  RTW_ERROR: if the WiFi chip initialization failed.
 */
int wifi_on(rtw_mode_t mode);

/**
 * @brief  Disable Wi-Fi.
 * @param  None
 * @return  RTW_SUCCESS: deinit success,
 * 	wifi mode is changed to RTW_MODE_NONE.
 * @return  RTW_ERROR: otherwise.
 */
int wifi_off(void);

/**
 * @brief  Check if the specified wlan interface  is running.
 * @param[in]  wlan_idx: can be set as STA_WLAN_INDEX or SOFTAP_WLAN_INDEX.
 * @return  If the function succeeds, the return value is 1.
 * 	Otherwise, return 0.
 * @note  For STA mode, use STA_WLAN_INDEX
 * 	For AP mode, use SOFTAP_WLAN_INDEX
 */
int wifi_is_running(unsigned char wlan_idx);

/**
 * @brief  Join a Wi-Fi network.
 * 	Scan for, associate and authenticate with a Wi-Fi network.
 * @param[in]  connect_param: the pointer of a struct which store the connection
 * 	info, including ssid, bssid, password, etc, for details, please refer to struct
 * 	rtw_network_info_t in wifi_conf.h
 * @param[in]  block: if block is set to 1, it means synchronized wifi connect, and this
* 	API will return until connect is finished; if block is set to 0, it means asynchronized
* 	wifi connect, and this API will return immediately.
 * @return  RTW_SUCCESS: when the system is joined for synchronized wifi connect, when connect
* 	cmd is set successfully for asynchronized wifi connect.
 * @return  RTW_ERROR: if an error occurred.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function.
 * 	(@ref wifi_on())
 * @note  if bssid in connect_param is set, then bssid will be used for connect, otherwise ssid
 * 	is used for connect.
 */
int wifi_connect(rtw_network_info_t *connect_param, unsigned char block);

/**
 * @brief  Disassociates from current Wi-Fi network.
 * @param  None
 * @return  RTW_SUCCESS: On successful disassociation from the AP.
 * @return  RTW_ERROR: If an error occurred.
 */
int wifi_disconnect(void);

/**
 * @brief  get join status during wifi connectection
 * @param  None
 * @return join status, refer to macros in wifi_conf.c
 */
rtw_join_status_t wifi_get_join_status(void);

/**
 * @brief  Initiate a scan to search for 802.11 networks.
  * Synchronized scan and asynchronized scan can be confgiured by the input param block.
  * For asynchronized scan, there are two different ways about how the scan result will be reported.
  * The first way is that when scan is done ,the total number of scanned APs will be reported through
  * scan_user_callback, and the detailed scanned AP infos can be get by calling wifi_get_scan_records,
  * so in this way, scan_user_callback need to be registered in scan_param.
  * The second way is that every time a AP is scanned, this AP info will be directly reported by
  * scan_report_each_mode_user_callback, and when scan is done, scan_report_each_mode_user_callback will
  * report a NULL pointer for notification. So in this way, scan_report_each_mode_user_callback need to
  * be registered in scan_param, and RTW_SCAN_REPORT_EACH need to be set in scan_param->options.Also in
  * this mode, scan_user_callback is no need to be registered.
 * @param[in]  scan_param: refer to struct rtw_scan_param_t in wifi_conf.h.
 * @param[in]  block: If set to 1, it's synchronized scan and this API will return
 * 	after scan is done. If set to 0, it's asynchronized scan and this API will return
 * 	immediately.
 * @return  RTW_SUCCESS or RTW_ERROR for asynchronized scan, return RTW_ERROR or
 * 	scanned AP number for Synchronized scan.
 * @note  If this API is called, the scanned APs are stored in WiFi driver dynamic
 * 	allocated memory, for synchronized scan or asynchronized scan which not use RTW_SCAN_REPORT_EACH,
 * 	these memory will be freed when wifi_get_scan_records is called.
 */
int wifi_scan_networks(rtw_scan_param_t *scan_param, unsigned char block);


/**
 * @brief  Used for switching WiFi mode
 * @param[in]  mode: Decide to switch WiFi to which mode.
 * 	The optional modes are RTW_MODE_STA, RTW_MODE_AP,
 * 	RTW_MODE_STA_AP, RTW_MODE_PROMISC.
 * @return  RTW_SUCCESS: WiFi switch mode success.
 * @return  RTW_ERROR: WiFi switch mdoe fail.
 */
int wifi_set_mode(rtw_mode_t mode);

/**
 * @brief  Trigger Wi-Fi driver to start an infrastructure Wi-Fi network.
 * @param[in]  softAP_config:the pointer of a struct which store the softAP
 * 	configuration, please refer to struct rtw_softap_info_t in wifi_conf.h
 * @warning  If a STA interface is active when this function is called,
 * 	the softAP will start on the same channel as the STA.
 * 	It will NOT use the channel provided!
 * @return  RTW_SUCCESS: If successfully creates an AP.
 * @return  RTW_ERROR: If an error occurred.
 * @note  if hidden_ssid in softAP_config is set to 1, then this softAP will start
 * 	with hidden ssid.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function.
 * 	(@ref wifi_on())
 */
int wifi_start_ap(rtw_softap_info_t *softAP_config);

/**
 * @brief  Disable Wi-Fi interface-2.
 * @param  None
 * @return  RTW_SUCCESS: deinit success,
 * 	wifi mode is changed to RTW_MODE_STA.
 * @return  RTW_ERROR: otherwise.
 */
int wifi_stop_ap(void);
/**
 * @brief  Enable Wi-Fi interface-2.
 * @param  None
 * @return  RTW_SUCCESS: success,
 * 	wifi open RTW_MODE_AP .
 * @return  RTW_ERROR: otherwise.
 */
int _wifi_on_ap(void);
/**
 * @brief  Disable Wi-Fi interface-2.
 * @param  None
 * @return  RTW_SUCCESS: close ap mode,
 * @return  RTW_ERROR: otherwise.
 */
int _wifi_off_ap(void);
/**
* @}
*/

/**
  ******************************************************************************
  *below for promisc mode
  ******************************************************************************
  */

/** @} */
#ifdef __cplusplus
}
#endif

#endif // __WIFI_API_H



