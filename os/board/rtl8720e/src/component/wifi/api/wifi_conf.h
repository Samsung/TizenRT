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
  * @file    wifi_conf.h
  * @author
  * @version
  * @brief   This file provides user interface for Wi-Fi station and AP mode configuration
  *             base on the functionalities provided by Realtek Wi-Fi driver.
  ******************************************************************************
  */
#ifndef __WIFI_CONF_H
#define __WIFI_CONF_H

/** @defgroup WIFI
 *  @brief      WIFI module
 *  @{
 */


#include "rtw_wifi_constants.h"
#include <platform_stdlib.h>
#include "rtw_intf_bt.h"
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
#define SCAN_LONGEST_WAIT_TIME	(4500)

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
	PROBE_REQ = BIT(0),
	PROBE_RSP = BIT(1),
	BEACON	  = BIT(2),
	ASSOC_REQ = BIT(3),
} rtw_custom_ie_type_t;
#endif /* _CUSTOM_IE_TYPE_ */

typedef enum  {
	WL_BAND_2_4G = 0,   ///<2.4g band
	WL_BAND_5G,            ///<5g band
	WL_BAND_2_4G_5G_BOTH, ///<2.4g&5g band
	WL_BANDMAX  ///< max band
} WL_BAND_TYPE;

/**
  * @}
  */


/** @addtogroup Structs
   *@{
   */
/**
 * @brief  The enumeration is join block param.
 */
typedef struct {
	void				*join_sema;
	unsigned int		join_timeout;
	unsigned char		block;
} internal_join_block_param_t;

/**
  * @brief  The structure is used to describe the unique 6-byte MAC address.
  */
typedef struct {
	unsigned char octet[6]; /**< Unique 6-byte MAC address */
} rtw_mac_t;

/**
  * @brief  The structure is used to describe the maclist.
  */
typedef struct {
	unsigned int    count;         /**< Number of MAC addresses in the list    */
	rtw_mac_t mac_list[1];   /**< Variable length array of MAC addresses */
} rtw_maclist_t;

/**
  * @brief  The structure is used to describe the bss info of the network.\n
  *			It include the version, BSSID, beacon_period, capability, SSID,
  *			channel, atm_window, dtim_period, RSSI e.g.
  */
typedef struct {
	unsigned int      version;                ///< version field
	unsigned int      length;                 ///< byte length of data in this record
	/* starting at version and including IEs */
	rtw_mac_t     BSSID;
	unsigned short      beacon_period;          ///< units are Kusec
	unsigned short      capability;             ///< Capability information
	unsigned char       SSID_len;
	unsigned char       SSID[32];
	unsigned char       channel;
	unsigned short      atim_window;            ///< units are Kusec
	unsigned char       dtim_period;            ///< DTIM period
	signed short       RSSI;                   ///< receive signal strength (in dBm)

	unsigned char       n_cap;                  ///< BSS is 802.11N Capable
	unsigned int      nbss_cap;               ///< 802.11N BSS Capabilities (based on HT_CAP_*)
	unsigned char       basic_mcs[MCSSET_LEN];  ///< 802.11N BSS required MCS set

	unsigned short      ie_offset;              ///< offset at which IEs start, from beginning
	unsigned int      ie_length;             ///< byte length of Information Elements
} rtw_bss_info_t;

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
typedef void (*p_wlan_autoreconnect_hdl_t)(rtw_security_t, char *, int, char *, int, int);
typedef void (*rtw_joinstatus_callback_t)(rtw_join_status_t join_status);

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
	rtw_joinstatus_callback_t	joinstatus_user_callback;	/**< user callback for processing joinstatus, please set to NULL if not use it */
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
	unsigned char 		password[65];   /**< the password of connected AP or softAP */
	unsigned char		key_idx;
	unsigned char		iw_mode;	/**< RTK_IW_MODE */
	unsigned char		alg;		/**< RTW_IW_ENC_ALG */
	unsigned int		auth_type;
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

	unsigned char rtw_cmd_tsk_spt_wap3;

	rtw_wpa_mode wifi_wpa_mode;

	unsigned char g_user_ap_sta_num;

	/* power save */
	unsigned char lps_dtim;
	unsigned char lps_enter_threshold;
	unsigned char rtw_power_mgnt;
	unsigned char rtw_lps_level;
	unsigned char smart_ps;

	/* AP */
	unsigned char bForwardingDisabled;

	unsigned char bChDeauthDisabled;

	unsigned char bAcceptAddbaReq;
	unsigned char bIssueAddbaReq;	///< 0: disable issue addba request, 1: enable issue addba request

	unsigned char ampdu_factor;	///for vht capability IE

	unsigned char bCheckDestAddress; ///< 0: don't check dest mac and ip address for station, 1: check dest mac and ip address for station

	/*
	The ap_compatibilty_enabled is used to configure the wlan settings, each bit controls one aspect.
	bit 0: (0: follow 802.11 spec, do not issue deauth, 1(default): issue deauth in 1st REAUTH_TO to be compatible with ap)
	bit 1: (0: do not check beacon info to connect with AP with multiple SSID, 1(default): check beacon info)
	bit 2: (0(default): do not issue deauth at start of auth, 1: issue deauth at start of auth)
	bit 3: (0: do not switch WEP auth algo unless WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG, 1(default): switch WEP auth algo from shared key to open system in 1st REAUTH_TO)
	other bits: reserved
	*/
	unsigned char ap_compatibilty_enabled;

	unsigned char max_roaming_times;

	unsigned char ap_polling_sta;

	unsigned char channel_plan;

	unsigned char country_code;

	/*for auto reconnect*/
	unsigned char auto_reconnect_count;
	unsigned char auto_reconnect_interval; // in sec

	/*wifi driver's trx buffer number, each skb occupies about 1.8K bytes of heap, a little difference between different chips*/
	int skb_num_np;/*In INIC mode for all traffics except tx data, In single core mode for all traffics*/
	int skb_num_ap;/*In INIC mode for tx data packtes, not used in single core mode*/

	/* for RTS/CTS */
	unsigned char cts2self;

	/* for MCC */
	unsigned char en_mcc;
} ;

extern  struct wifi_user_conf wifi_user_config;

/**
  * @brief  The structure is used to describe the psk info
  */
struct psk_info {
	unsigned char index;                  ///<  index
	unsigned char psk_essid[32 + 4]; ///< refer to NDIS_802_11_LENGTH_SSID + 4
	unsigned char psk_passphrase[64 + 1]; ///< refer to IW_PASSPHRASE_MAX_SIZE + 1
	unsigned char wpa_global_PSK[20 * 2]; ///< refer to A_SHA_DIGEST_LEN * 2
};

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
	unsigned int    max_skbbuf_used_number; /*!< max skb buffer used number       */
	unsigned int    skbbuf_used_number;     /*!< current used skbbuf number       */
	unsigned int    max_skbdata_used_number;/*!< max skb data used number       */
	unsigned int    skbdata_used_number;    /*!< current used skbdata number       */
} rtw_sw_statistics_t;

/**
  * @brief  The structure is used to describe the phy statistics
  */
typedef struct {
	signed char rssi;          /*!<average rssi in 1 sec */
	signed char snr;          /*!< average snr in 1 sec (not include cck rate)*/
	/* todo*/
	unsigned int false_alarm_cck;
	unsigned int false_alarm_ofdm;
	unsigned int cca_cck;
	unsigned int cca_ofdm;
	unsigned int tx_retry;
	unsigned short tx_drop;
	unsigned int rx_drop;
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

typedef struct rtw_scan_handler_result {
    rtw_scan_result_t ap_details;
    rtw_bool_t        scan_complete;
    void*               user_data;
} rtw_scan_handler_result_t;

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
  * @brief  The structure is used to describe the data description
  */
typedef struct {
	unsigned char wlan_idx;      /**< index of wlan interface which will transmit */
	unsigned char *buf;          /**< poninter of buf where raw data is stored*/
	unsigned short buf_len;      /**< the length of raw data*/
	unsigned short flags;        /**< send options*/
	unsigned char tx_rate;       /**< specific tx rate, please refer to enum MGN_RATE in wifi_constants.h*/
	unsigned char retry_limit;   /**< retry limit configure, when set to 0, will use default retry limit 12*/
	/* todo*/
	unsigned int tx_power;
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
#if (defined(CONFIG_UNSUPPORT_PLCPHDR_RPT) && CONFIG_UNSUPPORT_PLCPHDR_RPT) || defined __DOXYGEN__
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
	unsigned char enable;
	unsigned char trig_period;  /* unit:ms*/
	unsigned char data_rate;
	unsigned char mac_addr[6];
	unsigned char multi_type;/* 0&1 for multi sta CSI */
} rtw_csi_action_parm_t;

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
	int	(*hard_start_xmit)(struct sk_buff *skb, struct net_device *dev);
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
 * @brief  Check if Wi-Fi has connected to AP before dhcp.
 * @param  None
 * @return  RTW_SUCCESS: If conneced.
 * @return  RTW_ERROR: If not connect.
 */
int wifi_is_connected_to_ap(void);

/**
 * @brief  get join status during wifi connectection
 * @param  None
 * @return join status, refer to macros in wifi_conf.c
 */
rtw_join_status_t wifi_get_join_status(void);

/**
 * @brief  set join status during wifi connectection
 * @param  join status
 * @return None
 */
void wifi_set_join_status(rtw_join_status_t status);

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
 * @brief  Get scan results
 * @param[inout]  AP_num: input the pointer to the number of scanned ap info which
 * 	want to get, output the number of scanned ap info whicn can actually get.
 * @param[in]  scan_buf: pointer to the buf where scan result will be stored, the
 * 	scanned AP info will be stored one by one in form of struct rtw_scan_result_t.
 * @return  RTW_SUCCESS or RTW_ERROR.
 * @note  For synchronized scan or asynchronized scan which do not config RTW_SCAN_REPORT_EACH,
 * 	if once called wifi_scan_networks but not use this API to get scanned AP info,
 * 	driver memory for these scanned AP will not be freed until next time
 * 	wifi_scan_networks is called.
 * @note  For asynchronized scan which config RTW_SCAN_REPORT_EACH, every time a
 * 	AP is scanned, the AP info will be directly reported through scan_report_each_mode_user_callback
 * 	and freed after user callback executed, thus there is no need to use this function to get scan result.
 */
int wifi_get_scan_records(unsigned int *AP_num, char *scan_buf);

/**
 * @brief  Abort onoging wifi scan
 * @return  RTW_SUCCESS or RTW_ERROR.
 * @note  This is an asynchronized function and will return immediately, return value
 * 	only indicates whether the scan abort cmd is successfully notified to driver or not.
 * 	When scan is actually aborted, the user callback registered in wifi_scan_networks
 * 	will be executed.If there is no wifi scan in progress, this function will just return
 * 	RTW_SUCCESS and user callback won't be executed.
 */
int wifi_scan_abort(void);

/**
 * @brief  Enable or disable IPS. IPS is the abbreviation of Inactive Power Save mode.
 * 	Wi-Fi automatically turns RF off if it is not associated to AP.
 * @param[in]  enable: It could be TRUE or FALSE.
 * 	FALSE means disable ips, TRUE means enable ips.
 * @return  RTW_SUCCESS if setting IPS successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_ips_enable(u8 enable);

/**
 * @brief  Enable or disable LPS. LPS is the abbreviation of Leisure Power Save mode.
 * 	Wi-Fi automatically turns RF off during the association to AP
 * 	if traffic is not busy, while it also automatically turns RF on
 * 	to listen to the beacon of the associated AP.
 * @param[in]  enable: It could be TRUE or FALSE.
 * 	FALSE means disable LPS, TRUE means enable LPS.
 * @return  RTW_SUCCESS if setting LPS successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_lps_enable(u8 enable);

/**
 * @brief  Set reconnection mode with configuration.
 * @param[in]  mode: Set 1/0 to enalbe/disable the reconnection mode.
 * @return  0 if success, otherwise return -1.
 * @note  Defining CONFIG_AUTO_RECONNECT in "rtw_autoconf.h" needs to be
 * 	done before compiling, or this API won't be effective.
 */
int wifi_config_autoreconnect(__u8 mode);

/**
 * @brief  Get the result of setting reconnection mode.
 * @param[out]  mode: Point to the result of setting reconnection mode.
 * 	mode = 0 means autoreconnect is off
 *	mode = 1 means autoreconnect is on
 * @return  0 if get the status of autoreconnect success, otherwise return -1.
 * @note  Defining CONFIG_AUTO_RECONNECT in "rtw_autoconf.h" needs to be
 * 	done before compiling, or this API won't be effective.
 */
int wifi_get_autoreconnect(__u8 *mode);

/**
 * @brief  set the timeout value for checking no beacon
 * @param  the value of timeout, in seconds.
 * @return None
 */
void wifi_set_no_beacon_timeout(unsigned char timeout_sec);

/**
 * @brief  Get reason code of latest disassociation or deauthentication.
 * @param[out]  reason_code: A pointer to the variable where the
 * 	reason code (rtw_connect_error_flag_t) will be written.
 * @return  RTW_SUCCESS or RTW ERROR.
 */
int wifi_get_disconn_reason_code(unsigned short *reason_code);

/**
 * @brief  Used for switching WiFi mode
 * - Switch Wifi Mode to @param[in]
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
 * @brief  Get the associated clients with SoftAP.
 * @param[out]  client_list_buffer: The location where the client
 * 	list will be stored.
 * @param[in]  buffer_length: The buffer length is reserved for future use.
 * 	Currently, buffer length is set to a fixed value: 25.
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got.
 */
int wifi_get_associated_client_list(
	void			*client_list_buffer,
	unsigned short	buffer_length);

/**
 * @brief  delete a STA
 * @param[in]  wlan_idx: the wlan interface index, should be SOFTAP_WLAN_INDEX.
 * @param[in]  hwaddr: the pointer to the MAC address of the STA
 * 	which will be deleted
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  this function should be used when operating as AP
 */
int wifi_del_station(unsigned char wlan_idx, unsigned char *hwaddr);

/* For INIC wifi_set_channel function. */
int wifi_set_channel(int channel);

/**
 * @brief  Retrieves the current Media Access Control (MAC) address
 *	(or Ethernet hardware address) of the 802.11 device.
 * @param[in]  mac: Pointer to the struct rtw_mac_t which contain obtained mac address.
 */
int wifi_get_mac_address(rtw_mac_t *mac);

/**
 * @brief  Get current Wi-Fi setting from driver.
 * @param[in]  wlan_idx: STA_WLAN_IDX or SOFTAP_WLAN_IDX.
 * @param[out]  psetting: Points to the rtw_wifi_setting_t structure which information is gotten.
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got.
 * @note  the mode in rtw_wifi_setting_t corresponding to the wifi mode of current wlan_idx, if in
 * 	station mode, the info in rtw_wifi_setting_t except mode will correspond to the AP it connected,
 * 	if in AP mode, the info in rtw_wifi_setting_t will correspond to the softAP itself.
 */
int wifi_get_setting(unsigned char wlan_idx, rtw_wifi_setting_t *psetting);

/**
 * @brief  This function is used to get wifi mode
 * 	for station mode when connecting to AP.
 * @param[in]  no
 * @return  0-unconnected, 4-wifi4, 5-wifi5, 6-wifi6.
 */
int wifi_get_network_mode(void);

/**
 * @brief  Set the network mode according to the data rate its supported.
 * 	Driver works in BGN mode in default after driver initialization.
 * 	This function is used to change wireless network mode
 * 	for station mode before connecting to AP.
 * @param[in]  mode: Network mode to set. The value can be
 * 	RTW_NETWORK_B, RTW_NETWORK_BG and RTW_NETWORK_BGN.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_set_network_mode(rtw_network_mode_t mode);

/**
  * @}
  */


/** @defgroup Extended_Functions
  * @{
  */

/**
 * @brief  Set the chip to start or stop the promiscuous mode.
 *            For promisc mode.
 * @param[in]  enabled: enabled can be set 0, 1, 2, 3 and 4.
 * 	if enabled is zero, disable the promisc, else enable the promisc.
 * 	0) means disable the promisc.
 * 	1) means enable the promisc special for all ethernet frames.
 * 	2) means enable the promisc special for Broadcast/Multicast
 * 	   ethernet frames.
 * 	3) means enable the promisc special for all 802.11 frames.
 * 	4) means enable the promisc special for Broadcast/Multicast
 * 	   802.11 frames.
 * @param[in]  callback: the callback function which will
 * 	receive and process the network data.
 * @param[in]  len_used: specify if the the promisc data length is used.
 * 	If len_used set to 1, packet(frame data) length will be saved
 * 	and transferred to callback function.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  This function can implement vendor specified simple configure.
 * @note  To fetch Ethernet frames, the len_used should be set to 1
 */
int wifi_set_promisc(
	rtw_rcr_level_t	enabled,
	void	(*callback)(unsigned char *, unsigned int, void *),
	unsigned char	len_used);

/**
 * @brief  Let Wi-Fi enter promiscuous mode.
 *            For promisc mode.
 * @param[in]  None
 * @return  None
 */
void wifi_enter_promisc_mode(void);

#if defined CONFIG_PROMISC || defined __DOXYGEN__

/**
 * @brief  Initialize packet filter related data.
 *            For promisc mode.
 * @param[in]  None
 * @return  None
 */
void wifi_init_packet_filter(void);

/**
 * @brief  Add packet filter.
 *            For promisc mode.
 * @param[in]  filter_id: The filter id.
 * 	filter id can be num between 0 to 4.
 * @param[in]  patt: Point to the filter pattern.
 * @param[in]  rule: Point to the filter rule.
 * @return  0 if success, otherwise return -1.
 * @note  For now, the maximum number of filters is 5.
 */
int wifi_add_packet_filter(
	unsigned char					filter_id,
	rtw_packet_filter_pattern_t		*patt,
	rtw_packet_filter_rule_t		rule);

/**
 * @brief  Enable the packet filter.
 *            For promisc mode.
 * @param[in]  filter_id: The filter id.
 * 	filter id can be num between 0 to 4.
 * @return  0 if success, otherwise return -1.
 * @note  The filter can be used only if it has been enabled.
 */
int wifi_enable_packet_filter(unsigned char filter_id);

/**
 * @brief  Disable the packet filter.
 *            For promisc mode.
 * @param[in]  filter_id: The filter id.
 * 	filter id can be num between 0 to 4.
 * @return  0 if success, otherwise return -1.
 */
int wifi_disable_packet_filter(unsigned char filter_id);

/**
 * @brief  Remove the packet filter.
 *            For promisc mode.
 * @param[in]  filter_id: The filter id.
 * 	filter id can be num between 0 to 4.
 * @return  0 if success, otherwise return -1.
 */
int wifi_remove_packet_filter(unsigned char filter_id);

/**
 * @brief: Filter out the retransmission MIMO packet in promisc mode.
 *            For promisc mode.
 * @param[in]  enable: set 1 to enable filter retransmission pkt function,
 * 	set 0 to disable this filter function.
 * @param[in]  filter_interval_ms: if 'enable' equals 0, it's useless;
 * 	if 'enable' equals 1, this value:
 * 	- indicate the time(ms) below which an adjacent pkt received
 * 	will be claimed a retransmission pkt;
 * 	- if it has the same length with the previous pkt, and driver
 * 	will drop all retransmission pkts;
 * @return 0 if success, otherwise return -1.
 * @e.g.  For example, if the packet transmission time interval is 10ms,
 * 	but driver receives two packets with the same length within 3ms
 * 	then the second packet will be dropped if configed as
 * 	wifi_retransmit_packet_filter(1,3).
 */
int wifi_retransmit_packet_filter(u8 enable, u8 filter_interval_ms);

/**
 * @brief  Only receive the packets sent by the specified
 * 	ap and phone in promisc mode.
 *    For promisc mode.
 * @param[in]  enable: set 1 to enable filter,
 * 	set 0 to disable this filter function.
 * @param[in]  ap_mac: if 'enable' equals 0, it's useless;
 * 	if 'enable' equals 1, this value is the ap's mac address.
 * @param[in]  phone_mac: if 'enable' equals 0, it's useless;
 * 	if 'enable' equals 1, this value is the phone's mac address.
 * @return  None.
 * @note  Please invoke this function as
 * 	"wifi_filter_by_ap_and_phone_mac(0,NULL,NULL)" before exiting
 * 	promisc mode if you enabled it during the promisc mode.
 */
void wifi_filter_by_ap_and_phone_mac(
	u8		enable,
	void	*ap_mac,
	void	*phone_mac);

/**
 * @brief  config to report ctrl packet or not under promisc mode.
 *            For promisc mode.
 * @param[in]  enable: set 1 to enable ctrl packet report,
 * 	set 0 to disable ctrl packet report.
 * @return  0 if success, otherwise return -1.
 * @note this function can only be used under promisc mode,
 * 	i.e. between wifi_set_promisc(enable,...,...) and
 * 	wifi_set_promisc(disable,...,...)
 */
int wifi_promisc_ctrl_packet_rpt(u8 enable);
#endif


/**
 * @brief  check whether current wifi driver is mp or not.
 * @param  None
 * @return  1: drv_mode is RTW_DRV_MODE_MP
 * 	0: drv_mode is not RTW_DRV_MODE_MP
 */
u8 wifi_driver_is_mp(void);

/**
 * @brief  Set Management Frame Protection Support.
 * @param[in] value:
 * 	0) NO_MGMT_FRAME_PROTECTION: not support
 * 	1) MGMT_FRAME_PROTECTION_OPTIONAL: capable
 * 	2) MGMT_FRAME_PROTECTION_REQUIRED: required
 * @return  RTW_SUCCESS if setting Management Frame Protection Support successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_mfp_support(unsigned char value);

/**
 * @brief  Set group id of SAE.
 * @param[in] value:group id which want to be set
 * @return  RTW_SUCCESS if setting is successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_group_id(unsigned char value);

/**
 * @brief  enable or disable pmk cache.
 * @param[in] value:1 for enable, 0 for disable
 * @return  RTW_SUCCESS if setting is successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_pmk_cache_enable(unsigned char value);

/**
 * @brief  Set psk related info, including ssid, passphrase, psk.
 * @param[in]  psk_data: pointer to the structure that can set
 * 	psk related info in driver.{@ref struct psk_info}
 * @return  NULL.
 * @note  can be used to set psk related info to driver when fast connect
 */
void wifi_psk_info_set(struct psk_info *psk_data);

/**
 * @brief  Get psk related info, including ssid, passphrase, psk.
 * @param[out]  psk_data: pointer to the structure that will restore
 * 	psk related info. {ref struct psk_info}
 * @return  NULL.
 * @note  can be used to get current psk related info after connected
 * 	to AP successfully，and may write these info
 * 	to flash for fast connect next time
 */
void wifi_psk_info_get(struct psk_info *psk_data);

/**
 * @brief  Get enctryption ccmp key used by wifi(sta mode only)
 * @param[out]  UncstKey: the location where the CCMP TK(tempary key)
 *  will be stored
 * @param[out]  group_key: the location where the CCMP GTK(group key)
 *  will be stored
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got
 */
int wifi_get_ccmp_key(unsigned char *uncst_key, unsigned char *group_key);

/**
 * @brief  Show the TX and RX statistic information which counted by software(wifi driver,not phy layer).
 * @param[in]  idx: the wlan interface index, can be STA_WLAN_INDEX or SOFTAP_WLAN_IDX.
 * @return  NULL.
 */
int wifi_get_sw_statistic(unsigned char idx, rtw_sw_statistics_t *sw_statistics);

/**
 * @brief  fetch statistic info about wifi.
 * @param[in]  idx: the wlan interface index, can be STA_WLAN_INDEX or SOFT_WLAN_INDEX.
 * @param[out]  phy_statistic: The location where the statistic
 * 	info will be stored, for detail info, please refer to structrtw_phy_statistics .
 * @return  RTW_SUCCESS: If the statistic info is successfully get.
 * @return  RTW_ERROR: If the statistic info is not successfully get.
 * @note  the statistic info will only be valid after connected to AP successfully.
 */
int wifi_fetch_phy_statistic(unsigned char wlan_idx, rtw_phy_statistics_t *phy_statistic);

/**
 * @brief  get current remaining number of packets in HW TX buffer.
 * @param[in]  None
 * @return  RTW_ERROR or the remaining packets number.
 */
int wifi_get_txbuf_pkt_num(void);

/**
 * @brief  config mode of HW indicating packets(mgnt and data)
 * 	and SW reporting packets to wifi_indication().
 * @param[in]  enable
 * 	0) WIFI_INDICATE_DISABLE: disable mode(default), HW only
 * 	   indicate bssid-matched pkts and SW don't report.
 * 	1) WIFI_INDICATE_NORMAL: HW only indicate bssid-matched
 * 	   pkts and SW report.
 * 	2) WIFI_INDICATE_WILD: HW indicate all pkts and SW report.
 * @return  None
 */
void wifi_set_indicate_mgnt(int enable);

/**
 * @brief  Get antenna infomation.
 * @param[in]  antenna: Point to the antenna value obtained from driver,
 * 	0 means main antenna, 1 means aux antenna.
 * @return  0 if get successfully, otherwise return -1.
 */

#if defined CONFIG_ANTENNA_DIVERSITY ||defined __DOXYGEN__
int wifi_get_antenna_info(unsigned char *antenna);
#endif // #ifdef CONFIG_ANTENNA_DIVERSITY

//-------------------------------------------------------------//
/*
 * @brief get band type
 * @return  the support band type.
 * 	0) WL_BAND_2_4G: only support 2.4G
 * 	1) WL_BAND_5G: only support 5G
 * 	2) WL_BAND_2_4G_5G_BOTH: support both 2.4G and 5G
 */
WL_BAND_TYPE wifi_get_band_type(void);

/**
 * @brief  get an auto channel
 * @param[in]  wlan_idx: the wlan interface index, can be STA_WLAN_INDEX or SOFTAP_WLAN_INDEX.
 * @param[in]  channel_set: the pointer to the channel set
 * 	auto channel will select from this channel set
 * @param[in]  channel_num: the number of channel in channel set
 * @return  the selected auto channel
 */
int wifi_get_auto_chl(
	unsigned char wlan_idx,
	unsigned char *channel_set,
	unsigned char channel_num);

/**
 * @brief	Get wifi TSF register[63:32]&[31:0].
 * @param[in]	port: wifi port 0/1.
 * @return TSF[63:0] or 0
 */
u64 wifi_get_tsf(unsigned char port_id);

/**
 * @brief  switch to a new channel in AP mode and using CSA to inform sta
 * @param[in]  new_chl: the new channel will be switched to.
 * @param[in]  chl_switch_cnt: the channel switch cnt,
* 	after chl_switch_cnt*102ms, ap will switch to new channel.
 * @param[in]  callback: this callback will be called after channel switch is done,
 * 	and will return the new channel number and channel switch result.
 * @return  RTW_SUCCESS or RTW_ERROR, only indicate whether channel switch cmd is
 * 	successfully set to wifi driver.
 * @note  this function should be used when operating as AP.
 */
int wifi_ap_switch_chl_and_inform(
	unsigned char new_chl,
	unsigned char chl_switch_cnt,
	ap_channel_switch_callback_t callback);

/**
 * @brief  initialize mac address filter list
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_init_mac_filter(void);

/**
 * @brief  add mac address to mac filter list then this address
 * 	will be rejected during authentication
 * @param[in]  hwaddr: the mac address which will be added to
 * 	mac filter list.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_add_mac_filter(unsigned char *hwaddr);

/**
 * @brief  delete mac address from mac filter list
 * @param[in]  hwaddr: the mac address which will be deleted
 * 	from mac filter list.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_del_mac_filter(unsigned char *hwaddr);

/**
 * @brief  Set the wps phase
 * 	wps: Wi-Fi Protected Setup
 * @param[in]  is_trigger_wps: to trigger wps function or not
 * 	is_trigger_wps value should only be 0 or 1
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_wps_phase(unsigned char is_trigger_wps);

/**
 * @brief  set WPS IE in Probe Request/Probe Response/Beacon/
 * 	Association Request/Association Response
 * @param[in]  wlan_idx: the wlan interface index, can be STA_WLAN_INDEX or SOFTAP_WLAN_INDEX
 * @param[in]  buf: the pointer to buf which store the WPS IE
 * @param[in]  buf_len: the length of buf
 * @param[in]  flags:
 * 	P2PWPS_PROBE_REQ_IE: set WPS ie in Probe Request
 * 	P2PWPS_PROBE_RSP_IE: set WPS ie in Probe Response
 * 	P2PWPS_BEACON_IE: set WPS ie in Beacon
 * 	P2PWPS_ASSOC_REQ_IE: set WPS ie in Association Request
 * 	P2PWPS_ASSOC_RSP_IE: set WPS ie in Association Response
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_set_gen_ie(unsigned char wlan_idx, char *buf, __u16 buf_len, __u16 flags);

/**
 * @brief  Set the eap phase to wifi driver
 * @param[in]  is_trigger_eap: to trigger 802.1X EAP function or not
 * @return    RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_eap_phase(unsigned char is_trigger_eap);

/**
 * @brief  get the current eap phase from wifi driver
 * @param[in]  None
 * @return  1: eap_phase is equal to 1, indicate 802.1X EAP is triggered
 * @return  0: eap_phase is equal to 0, indicate 802.1X EAP is not triggered
 */
unsigned char wifi_get_eap_phase(void);

/**
 * @brief  Set the current eap authentication method to wifi driver
 * @param[in]  eap_method: the current 802.1x EAP authentication method
 * @return    RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_eap_method(unsigned char eap_method);

/**
 * @brief  send 802.1X EAP frame
 * @param[in]  wlan_idx: the wlan interface index,
 * 	wlan_idx should be STA_WLAN_INDEX or SOFTAP_WLAN_INDEX
 * @param[in]  buf: the pointer to buf which store the EAP frame
 * @param[in]  buf_len: the length of buf
 * @param[in]  flags: reserved, set to 0
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_if_send_eapol(unsigned char wlan_idx, char *buf, __u16 buf_len, __u16 flags);

/**
 * @brief  Set ble scan duty when coex.
 * @param[in]  duty: the desired scan duty for ble
 * @return  1: if the scan duty is successfully set
 * 	-1: if the scan duty is not successfully set.
 */
int wifi_btcoex_set_ble_scan_duty(u8 duty);

/**
 * @brief  Setup custom ie list. (Information Element)
 * @warning  This API can't be executed twice before deleting
 * 	the previous custom ie list.
 * @param[in]  cus_ie: Pointer to WIFI CUSTOM IE list.
 * @param[in]  ie_num: The number of WIFI CUSTOM IE list.
 * @return  0 if success, otherwise return -1.
 */
int wifi_add_custom_ie(void *cus_ie, int ie_num);

/**
 * @brief  Update the item in WIFI CUSTOM IE list.
 * 	(Information Element)
 * @param[in]  cus_ie: Pointer to WIFI CUSTOM IE address.
 * @param[in]  ie_index: Index of WIFI CUSTOM IE list.
 * @return  0 if success, otherwise return -1.
 */
int wifi_update_custom_ie(void *cus_ie, int ie_index);

/**
 * @brief  Delete WIFI CUSTOM IE list. (Information Element)
 * @param[in]  wlan_idx,
 * 	wlan_idx should be STA_WLAN_INDEX or SOFTAP_WLAN_INDEX
 * @return  0 if success, otherwise return -1.
 */
int wifi_del_custom_ie(unsigned char wlan_idx);

/**
 * @brief  send raw frame
 * @param[in]  raw_data_desc: the pointer of raw_data_desc_t,
 * 	which describe related information, include the pointer of raw frame and so on.
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_send_raw_frame(raw_data_desc_t *raw_data_desc);

/**
 * @brief  Control initial tx rate by different ToS value in IP header.
 * @param[in]  enable: set 1 to add control for specified tx_rate for
 * 	corresponding ToS_value, set 0 to disable initial rate control for this ToS_value.
 * @param[in]  ToS_precedence: range from 0 to 7, corresponding to IP precedence in TOS field of IP header(BIT7~5).
 * @param[in]  tx_rate: initial tx rate for packet which has the same ToS value as setted.Please
 * 	refer to enum MGN_RATE in wifi_constants.h for rate definition.
 * @return  RTW_SUCCESS or RTW_ERROR.
 * @note this function only take IP Precedence(BIT 7~5 in ToS field) into consideration.
 */
int wifi_set_tx_rate_by_ToS(unsigned char enable, unsigned char ToS_precedence, unsigned char tx_rate);

/**
 * @brief  Set EDCA parameter.
 * @param[in]  AC_param: format is shown as in below ,
 * +--------------------------+-------------+-------------+
 * |        TXOP Limit        |ECWmin/ECWmax|  ACI/AIFSN  |
 * +--------------------------+-------------+-------------+
 * 	BIT31~16 corresponding to TXOP Limit, BIT15~8 corresponding
 * 	to ECWmin/ECWmax, BIT7~0 corresponding to ACI/AIFSN.
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_EDCA_param(unsigned int AC_param);

/**
 * @brief  enable or disable CCA for TX
 * @param[in]  enable: 1 for enable, 0 for disable
 * @return  RTW_SUCCESS or RTW_ERROR.
 * @note when disable, both CCA and EDCCA will be disabled.
 */
int wifi_set_TX_CCA(unsigned char enable);

/**
 * @brief  set duration and send a CTS2SELF frame
 * @param[in]  wlan_idx: the wlan interface index, can be STA_WLAN_IDX or SOFTAP_WLAN_IDX
 * @param[in]  duration: the duration value for the CTS2SELF frame
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_set_cts2self_duration_and_send(unsigned char wlan_idx, unsigned short duration);

#if defined CONFIG_WOWLAN_TCP_KEEP_ALIVE || defined __DOXYGEN__
/**
 * @brief  construct a tcp packet that offload to wlan. wlan would keep sending this packet to tcp server.
 *
 * @param[in]  socket_fd : tcp socket
 * @param[in]  content : tcp payload
 * @param[in]  len : tcp payload size
 * @param[in]  interval_ms : send this packeter every interval_ms milliseconds
 * @param[in]  resend_ms : if packet fails to send at interval_ms, then resend it after resemd_ms
 * @param[in]  wake_sys : if packet fails to send after resend several times, should wlan wake system
 * @return  RTW_SUCCESS
 */
int wifi_set_tcp_keep_alive_offload(int socket_fd, uint8_t *content, size_t len, uint32_t interval_ms, uint32_t resend_ms, uint8_t wake_sys);
#endif

#if defined CONFIG_WOWLAN_DHCP_RENEW || defined __DOXYGEN__
/**
 * @brief  construct a udp renew packet that offload to wlan. wlan would keep sending this packet to dhcp server.
 *
 * @return  RTW_SUCCESS
 */
int wifi_set_dhcp_offload(void);
#endif



#if defined CONFIG_ARP_KEEP_ALIVE || defined __DOXYGEN__
/**
 * @brief   use ARP response as keep alive packet instead of null frame
 *
 * @param[in]   enable : enable or disable ARP response keep alive
 * @return  RTW_SUCCESS
 */
int wifi_wowlan_set_arp_rsp_keep_alive(int enable);
#endif

#if defined CONFIG_WOWLAN_DTIMTO || defined __DOXYGEN__
/**
 * @brief   tcp keep alive fine tune
 *
 * @param[in]   dtimto_enable : enable or disable
 * @param[in]   retry_inc : change dtim to this value
 * @param[in]   ack_timeout : timeout for wait tcp ack
 * @param[in]   dtim : TBTT interval
 * @return  RTW_SUCCESS
 */
int wifi_wowlan_set_dtimto(uint8_t dtimto_enable, uint8_t retry_inc, uint8_t ack_timeout, uint8_t dtim);
#endif

/**
 * @brief   smart dtim in wowlan keep alive
 *
 * @param[in]   check_period : check period of TBTT
 * @param[in]   threshold : threshold of beacon lose
 * @param[in]   change_dtim : change dtim
 * @param[in]   dtim : TBTT interval
 * @return  RTW_SUCCESS
 */
#if defined CONFIG_SMART_DTIM  || defined __DOXYGEN
int wifi_wowlan_set_smartdtim(uint8_t check_period, uint8_t threshold, uint8_t change_dtim, uint8_t dtim);
#endif

/**
 * @brief   wowlan parameter setting
 *
 * @param[in]   fwdis_period : fw decision disconnect check period
 * @param[in]   fwdis_trypktnum : fw decision disconnect null packet retry
 * @param[in]   pno_enable : pno method enable
 * @param[in]   pno_timeout : timeout for wait ap enable
 * @param[in]   l2_keepalive_period : send period of l2 keep alive
 * @return  RTW_SUCCESS
 */
#if defined CONFIG_WOWLAN_PARAM || defined __DOXYGEN__
int wifi_wowlan_set_wowlan_param(u8  fwdis_period,
								 u8  fwdis_trypktnum,
								 u8  pno_enable,
								 u8  pno_timeout,
								 u8  l2_keepalive_period);
#endif


#if defined CONFIG_ARP_REQUEST_KEEP_ALIVE  || defined __DOXYGEN__
/**
 * @brief   use ARP request as keep alive packet instead of null frame
 *
 * @param[in]   powerbit : power bit setting
 * @param[in]   dtim1to : change to dtim1
 * @return  RTW_SUCCESS
 */
int wifi_wowlan_set_arpreq_keepalive(u8  powerbit,
									 u8  dtim1to);
#endif

#if defined CONFIG_WOWLAN_IO_WDT || defined __DOXYGEN__
/**
 * @brief   set gpio pull ctrl in wowlan
 *
 * @param[in]   gpio : gpiof number
 * @param[in]   interval : pull ctrl interval
 * @return  RTW_SUCCESS
 */
int wifi_wowlan_set_wdt(u8  gpio,
						u8  interval);
#endif

// WoWlan related
//-------------------------------------------------------------//
#if defined CONFIG_WOWLAN || defined __DOXYGEN__
/**
 * @brief  control wowlan mode enable and disable
 * @param[in]  enable:
 * 	0 means WOWLAN_DISABLE
 * 	1 means WOWLAN_ENABLE
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_wowlan_ctrl(int enable);

/**
 * @brief  set wowlan customer pattern
 * @param[in]  pattern: refer to struct wowlan_pattern_t
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_wowlan_set_pattern(wowlan_pattern_t pattern);
#endif

/**
 * @brief  set the csi parameters and enable or disable csi func(sta or softap)
 * @param[in]  act_param: A pointer to the param
 * @Example:
 *    rtw_csi_action_parm_t act_param = {0};
 *    act_param.mode = 2;
 *    ...
 *    wifi_csi_config(&act_param);
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_csi_config(rtw_csi_action_parm_t *act_param);

/**
 * @brief  get csi raw data and csi hdr info
 * @param[in]  buf_len: buffer size for getting csi info
 * @param[in]  csi_buf: the pointer to csi data buffer
 * @param[in]  len: the size of csi raw data
 * @Example:
 *    unsigned char *csi_buf = NULL;
 *    unsigned int len;
 *    csi_buf = rtw_zmalloc(buf_len);
 *    wifi_csi_report(buf_len,csi_buf,&len);
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_csi_report(u32 buf_len, u8 *csi_buf, u32 *len);

/**
 * @brief  get transmit power table
 * @param[in]  powertable: powertable
 * @Example:
 *    u32* powertable = NULL;
 *    ret = wifi_get_tx_powertable(powertable);
 * @return  RTW_SUCCESS or RTW_ERROR
 */
#ifdef CONFIG_PLATFORM_TIZENRT_OS
int wifi_get_tx_powertable(u32* powertable);
#endif

/**
 * @brief  Set PTA type when coex.
 * @param[in]  type: the PTA type(PTA_BT/PTA_WIFI/PTA_AUTO).
 * @return  Null.
 */
void wifi_btcoex_set_pta(pta_type_t type);

/**
  * @brief Write BT RFK  data to RFC
  * @param  calibration data
  * @retval  BT RFK result(1:success  0:failed)
  */
int wifi_btcoex_bt_rfk(struct bt_rfk_param *rfk_param);

/**
 * @brief  Set global variable wifi_wpa_mode.
 * @param[in]  wpa_mode:
 	(WPA_AUTO_MODE,WPA_ONLY_MODE, WPA2_ONLY_MODE,
	WPA3_ONLY_MODE,WPA_WPA2_MIXED_MODE, WPA2_WPA3_MIXED_MODE).
 * @return  0:success  -1:fail.
 */
int wifi_set_wpa_mode(rtw_wpa_mode wpa_mode);

/**
 * @brief  Dynamically modify the working mode of pmf.
 * @param[in]  pmf_mode:
 	0: none
 	1: optional
 	2: required
 * @return  0:success  -1:fail.
 */
int wifi_set_pmf_mode(u8 pmf_mode);

#ifdef __cplusplus
}
#endif

#endif // __WIFI_API_H

