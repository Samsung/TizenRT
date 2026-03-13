/**
  ******************************************************************************
  * @file    wifi_intf_drv_to_app_internal.h
  * @author
  * @version
  * @date
  * @brief   The API of this file is for internal use and does not guarantee stability.
  *          If customers using RTOS need to use it, please contact realtek to provide an official API.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __WIFI_CONF_INTERNAL_H
#define __WIFI_CONF_INTERNAL_H

#include "wifi_api_types.h"
#ifndef CONFIG_FULLMAC
#include "lwip_netconf.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define RTW_SEND_AND_WAIT_ACK 				2
#define RTW_SEND_BY_HIGH_RATE				4 // IEEE80211_OFDM_RATE_54MB
#define RTW_NAV_BY_USER						8

#define MAX_WPA_IE_LEN (257)
#define MAX_WPS_IE_LEN (512)
#define MAX_P2P_IE_LEN (256)
#define MAX_WFD_IE_LEN (128)
#define MAX_FT_IE_LEN (256)

#define RTW_OWE_KEY_LEN		32 /*32(Temporarily support group 19 with 256 bit public key)*/

/**
 * @brief The enumeration lists the type of pmksa operations.
 */
enum {
	PMKSA_SET = 0,
	PMKSA_DEL = 1,
	PMKSA_FLUSH = 2,
};

/**
  * @brief  only internal used events.
  */
enum  {
	/* only internal used */
	RTW_EVENT_INTERNAL_BASE				= 100,
	RTW_EVENT_RX_MGNT					= 101,
	RTW_EVENT_RX_MGNT_AP				= 102,
	RTW_EVENT_EXTERNAL_AUTH_REQ			= 103,

	RTW_EVENT_WPA_STA_4WAY_START		= 104,
	RTW_EVENT_WPA_AP_4WAY_START			= 105,
	RTW_EVENT_WPA_STA_4WAY_RECV			= 106,
	RTW_EVENT_WPA_AP_4WAY_RECV			= 107,
	RTW_EVENT_WPA_SET_PSK_INFO			= 108,

	RTW_EVENT_OWE_START_CALC			= 109,
	RTW_EVENT_OWE_PEER_KEY_RECV			= 110,

	RTW_EVENT_KVR_CAP_UPDATE			= 111,
	RTW_EVENT_NB_RESP_RECV				= 112,
	RTW_EVENT_BTM_REQ_RECV				= 113,
	RTW_EVENT_BTM_DEBUG_CMD				= 114,
	RTW_EVENT_FT_AUTH_START				= 115,
	RTW_EVENT_FT_RX_MGNT				= 116,

	RTW_EVENT_DEAUTH_INFO_FLASH			= 117,

	RTW_EVENT_SME_AUTH_TIMEOUT			= 118,
	RTW_EVENT_SME_ASSOC_TIMEOUT			= 119,
	RTW_EVENT_SME_RX_MLME_MGNT			= 120,
	RTW_EVENT_SME_TX_MLME_MGNT			= 121,
	RTW_EVENT_SME_RX_ASSOC_RESP			= 122,
	RTW_EVENT_SME_RX_UNPROT_MLME_MGMT	= 123,

	RTW_EVENT_WPA_P2P_CHANNEL_RDY		= 124, /**< STA mode: inform host channel switch ready */
	RTW_EVENT_WTN_ZRPP_GET_AP_INFO		= 125,

	RTW_EVENT_INTERNAL_MAX,
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

/**********************************************************************************************
 *                                     wpa_lite structures
 *********************************************************************************************/
/**
 * @brief  The structure is pmksa ops.
 */
struct rtw_pmksa_ops_t {
	u8 ops_id;
	u8 wlan_idx;
	u8 pmkid[16];
	u8 mac_addr[6];
	u8 pmk[32]; /**< Pmksa is maintained in NP when use wpa_lite.*/
};

/**
 * @brief  The structure is crypt info.
 */
struct rtw_crypt_info {
	u8 pairwise;            /**<  Indicate pairwise(1) key or group key(0). */
	u8 mac_addr[6];
	u8 wlan_idx;
	u16 key_len;
	u8 key[32];
	u8 key_idx;
	u8 driver_cipher;
	u8 transition_disable_exist;
	u8 transition_disable_bitmap;
	u8 device_id : 5;       /**<  tx_raw: Flag for no linked peer, and will be converted to camid when force_cam_entry = 1. */
	u8 force_cam_entry : 1; /**<  tx_raw must set force_cam_entry = 1. Upper layer direct set specified cam entry, not dependent on 4 way or stainfo. */
	u8 rpt_mode : 1;
};

/**
 * @brief  The structure is status of wpa_4way.
 */
struct rtw_wpa_4way_status {
	u8 mac_addr[6];             /**< Mac addr of 4-way interactive peer device. */
	u8 wlan_idx;              /**< Index of wlan interface. */
	u8 is_start : 1;          /**< Start(1) or stop(0) of 4way/2way exchange. */
	u8 is_grpkey_update : 1;  /**< Indicate first key change(0) or update grp_key change(1). */
	u8 is_success : 1;        /**< Result of 4way/2way exchange: 0-fail; 1-success. */
};

/**
* @brief  The structure is used to store configuration of SAE protocol
* which is used in secure auth process.
*/
struct wpa_sae_param_t {
	unsigned char 		peer_mac[6];
	unsigned char 		self_mac[6];
	u8					h2e;  /**< A flag indicating the use of Hash-to-Element (H2E) optimization in SAE. */
	u8					sae_reauth_limit;
	u8					rsn_auth_key_mgmt[4];
};

/**
 * @brief  The structure is used to describe the DH(Diffie-Hellman) params of
 *         OWE(Opportunistic Wireless Encryption).
 * @note  Temporarily support group 19 with 256 bit public key, i.e., group = 19, pub_key_len = 32.
 */
struct rtw_owe_param_t {
	u16 group;                   /**< Specify the DH group number used for public key generation. */
	u8 pub_key[RTW_OWE_KEY_LEN]; /**< The public key for OWE. */
	u8 pub_key_len;              /**< The length of the public key.*/
	u8 peer_mac[6];              /**< The MAC address of the peer device.*/
};

struct rtw_kvr_param_t {
#if defined(CONFIG_IEEE80211V) || defined(CONFIG_IEEE80211K) || defined(CONFIG_IEEE80211R)
	u8 nb_active;
	u8 btm_active;
	unsigned char 		peer_mac[6];
	unsigned char 		self_mac[6];
#ifdef CONFIG_IEEE80211R
	u8 ft_active;
	u16 mdid;
	u8 ft_cap;
	u8 privacy;
	u32 grp_privacy;
	u8 ie[5 + MAX_WPA_IE_LEN + MAX_FT_IE_LEN]; /**<  1.NP->AP: rsnie; 2.AP->NP: mdie+rsnie+ftie*/
	u32 ielen;
#endif
#endif
};

struct rtw_event_deauth_info_flash {
	struct deauth_info *deauth_data;
	u32 deauth_date_len;
	u8 flash_operation;  /* FLASH_READ or FLASH_WRITE */
};

struct rtw_event_sme_auth_timeout {
	u8 bssid[6];
};

/* Structs for events which need report both info and frame at the same time */
struct rtw_event_sme_rx_assoc_resp {
	u8 uapsd_ac_enable;
	u32 frame_len;
	u8 frame[];
};

struct rtw_event_rx_mgnt {
	u8 channel;
	u8 frame_type;
	u32 frame_len;
	u8 frame[];
};

struct rtw_event_nb_resp_recv {
	u8 from_btm;
	u32 frame_len;
	u8 frame[];
};

/* Structs for events which only need report frame */
struct rtw_event_report_frame {
	u32 frame_len;
	u8 frame[];
};

#ifndef CONFIG_FULLMAC
/**
 * @brief  The structure is join block param.
 */
struct internal_block_param {
	void				*sema;
};

/**
 * @brief  The structure is used to describe net device.
 */
struct net_device {
	void			*priv;		/**<  Pointer to private data. */
	unsigned char		dev_addr[6];	/**<  Set during bootup. */
	u8 iface_type;
};

/**
 * @brief  The structure is used to describe wlan info.
 */
struct _Rltk_wlan_t {
	struct net_device	dev;		/**<  Binding wlan driver netdev. */
	void			*skb;		/**<  Pending Rx packet. */
	unsigned int		tx_busy;
	unsigned int		rx_busy;
	unsigned char		enable;
	rtos_sema_t			netif_rx_sema;	/**<  Prevent race condition on .skb in rltk_netif_rx(). */
};
extern struct _Rltk_wlan_t rltk_wlan_info[NET_IF_NUM];

#define netdev_priv(dev)		dev->priv
#define rtw_is_netdev_enable(idx)	(rltk_wlan_info[idx].enable)
#define rtw_get_netdev(idx)		(&(rltk_wlan_info[idx].dev))
#endif

/**
  * @brief  Old version raw data description, only used for driver internal send mgnt frames.
  */
struct _raw_data_desc_t {
	unsigned char		wlan_idx;      /**< Index of wlan interface which will transmit. */
	unsigned char		*buf;          /**< Poninter of buf where raw data is stored.*/
	unsigned short		buf_len;      /**< The length of raw data.*/
	unsigned short		flags;        /**< Send options.*/
};

#ifdef CONFIG_NAN
#define MAX_MATCHING_FILTERS           (16)
#define MAX_MATCHING_FILTER_LEN        (32)
/**
 * @brief Describes a NAN function Rx / Tx filter.
 */
struct rtw_nan_func_filter {
	u8 filter[MAX_MATCHING_FILTER_LEN]; /**  the content of the filter. */
	u8 len; /** the length of the filter. */
};

/**
 * @brief Describes a NAN function corresponding to struct cfg80211_nan_func.
 */
struct rtw_nan_func_t {
	u8 type;
	u8 service_id[6];
	u8 publish_type;
	bool close_range;
	bool publish_bcast;
	bool subscribe_active;
	u8 followup_id;
	u8 followup_reqid;
	struct rtw_mac followup_dest;
	u32 ttl;
	const u8 *serv_spec_info;
	u8 serv_spec_info_len;
	bool srf_include;
	const u8 *srf_bf;
	u8 srf_bf_len;
	u8 srf_bf_idx;
	struct rtw_mac *srf_macs;
	int srf_num_macs;
	struct rtw_nan_func_filter *rx_filters;
	struct rtw_nan_func_filter *tx_filters;
	u8 num_tx_filters;
	u8 num_rx_filters;
	u8 instance_id;
	u64 cookie;
};
#endif

/**
  * @brief  the parameters for edcca test, only used for driver internal to set the edcca.
  */
struct rtw_edcca_param_t {
	unsigned int edcca_mode; /**< 0: normal; 1: ETSI; 2 Janpan; 9: Disable */
	int edcca_th; /**< EDCCA threshold, unit is dBm, the scope is [-128, 127], and the minimum step is 1.*/
};

/**
 * @brief  Set the current Media Access Control (MAC) address
 *	(or Ethernet hardware address) of the 802.11 device.
 * @param[in]  idx: Set STA or SoftAP mac address.
 * @param[in]  mac: Pointer to the mac address.
 * @param[in]  efuse: Set mac address to efuse or set to RAM.
 * @return  RTK_SUCCESS or RTK_FAIL
 * @note  Set to Efuse(efuse = 1): effective after reboot.
 * Set to RAM(efuse = 0): reboot will lose effectiveness. (RECOMMENDED)
 */
int wifi_set_mac_address(int idx, unsigned char *mac, u8 efuse);

/**
 * @brief  for wifi certification of ETSI mode
 * @param[in]  edcca_mode: 0: normal; 1: ETSI; 2 Janpan; 9: Disable
 * @param[in]  edcca_th: EDCCA threshold, unit is dBm, the scope is
 *	[-60, -80], and the minimum step is 1.
 * @return  null.
 */
void wifi_set_edcca_param(struct rtw_edcca_param_t *param);

/**
 * @brief  Set global variable wifi_wpa_mode.
 * @param[in]  wpa_mode:
 	(RTW_WPA_AUTO_MODE,RTW_WPA_ONLY_MODE, RTW_WPA2_ONLY_MODE,
	RTW_WPA3_ONLY_MODE,RTW_WPA_WPA2_MIXED_MODE, RTW_WPA2_WPA3_MIXED_MODE).
 * @return  0:success  -1:fail.
 */
int wifi_set_wpa_mode(u8 wpa_mode);

/**
 * @brief  Dynamically modify the working mode of pmf.
 * @param[in]  pmf_mode:
 	0: none
 	1: optional
 	2: required
 * @return  0:success  -1:fail.
 */
int wifi_set_pmf_mode(u8 pmf_mode);

/**
 * @brief  wpa notify wifi driver status of 4-way/2-way handshake.
 * wifi driver will do disconnect and autoreconnect when fail & inform coex
 * @param[in] rtw_wap_4way_status
 * @return  null.
 */
void wifi_wpa_4way_status_indicate(struct rtw_wpa_4way_status *rpt_4way);

/**
 * @brief  notify wifi driver the ip address is successfully obtained
 * @param[in] None
 * @return  null.
 */
void wifi_dhcp_success_indicate(void);

/**
 * @brief  for wpa to set key to driver
 * @param[in]  rtw_crypt_info
 * @return  0:success  -1:fail.
 */
int wifi_wpa_add_key(struct rtw_crypt_info *crypt);

/**
 * @brief  for wpa to set/del/flush pmksa
 * @param[in]  pmksa_ops
 * @return  null.
 */
void wifi_wpa_pmksa_ops(struct rtw_pmksa_ops_t *pmksa_ops);

/**
 * @brief  for owe to set DH params
 * @param[in]  owe_param
 * @return  null.
 */
void wifi_set_owe_param(struct rtw_owe_param_t *owe_param);

/**
 * @brief  Set Management Frame Protection Support.
 * @param[in] value:
 * 	0) NO_MGMT_FRAME_PROTECTION: not support
 * 	1) MGMT_FRAME_PROTECTION_OPTIONAL: capable
 * 	2) MGMT_FRAME_PROTECTION_REQUIRED: required
 * @return  RTK_SUCCESS if setting Management Frame Protection Support successful.
 * @return  RTK_FAIL otherwise.
 */
int wifi_set_mfp_support(unsigned char value);


/**
 * @brief  Set group id of SAE.
 * @param[in] value:group id which want to be set
 * @return  RTK_SUCCESS if setting is successful.
 * @return  RTK_FAIL otherwise.
 */
int wifi_set_group_id(unsigned char value);

/**
 * @brief  Enqueue join cmd.
 * @param[in]  None
 * @return  RTK_SUCCESS if enqueue join cmd successful.
 * @return  RTK_FAIL otherwise.
 */
int wifi_start_join_cmd(void);

/**
 * @brief  for wpa supplicant indicate sae status.
 * @param[in] wlan_idx:STA_WLAN_IDX or SOFTAP_WLAN_IDX.
 * @param[in] status:sae_status which will be indicated.
 * @param[in] mac_addr:pointer of sae peer's mac_addr .
 * @return  RTK_SUCCESS if setting is successful.
 * @return  RTK_FAIL otherwise.
 */
int wifi_sae_status_indicate(u8 wlan_idx, u16 status, u8 *mac_addr);

/**
 * @brief  send raw frame
 * @param[in]  raw_data_desc: the pointer of struct _raw_data_desc_t,
 * 	which describe related information, include the pointer of raw frame and so on.
 * @return  RTK_FAIL or RTW SUCCESS
 */
int wifi_send_mgnt(struct _raw_data_desc_t *raw_data_desc);

/**
 * @brief  for wpa supplicant indicate ft status.
 * @param[in] status:ft_status which will be indicated.
 * @return  null.
 */
int wifi_ft_status_indicate(struct rtw_kvr_param_t *kvr_param, u16 status);

/**
 * @brief  enable or disable pmk cache.
 * @param[in] value:1 for enable, 0 for disable
 * @return  RTK_SUCCESS if setting is successful.
 * @return  RTK_FAIL otherwise.
 */
int wifi_set_pmk_cache_enable(unsigned char value);

/**
 * @brief  Set the wps phase
 * 	wps: Wi-Fi Protected Setup
 * @param[in]  is_trigger_wps: to trigger wps function or not
 * 	is_trigger_wps value should only be 0 or 1
 * @return  RTK_SUCCESS or RTK_FAIL
 */
int wifi_set_wps_phase(unsigned char is_trigger_wps);

/**
 * @brief  Set the eap phase to wifi driver
 * @param[in]  is_trigger_eap: to trigger 802.1X EAP function or not
 * @return    RTK_SUCCESS or RTK_FAIL
 */
int wifi_set_eap_phase(unsigned char is_trigger_eap);

/**
 * @brief  get the current eap phase from wifi driver
 * @param[in]  eap_phase: Pointer to store the eap phase
 * 1: indicate 802.1X EAP is triggered
 * 0: indicate 802.1X EAP is not triggered
 * @return    RTK_SUCCESS or RTK_FAIL
 */
int wifi_get_eap_phase(u8 *eap_phase);

/**
 * @brief  Set the current eap authentication method to wifi driver
 * @param[in]  eap_method: the current 802.1x EAP authentication method
 * @return    RTK_SUCCESS or RTK_FAIL
 */
int wifi_set_eap_method(unsigned char eap_method);

/**
 * @brief  send 802.1X EAP frame
 * @param[in]  wlan_idx: the wlan interface index,
 * 	wlan_idx should be STA_WLAN_INDEX or SOFTAP_WLAN_INDEX
 * @param[in]  buf: the pointer to buf which store the EAP frame
 * @param[in]  buf_len: the length of buf
 * @param[in]  flags: reserved, set to 0
 * @return  RTK_FAIL or RTW SUCCESS
 */
int wifi_if_send_eapol(unsigned char wlan_idx, char *buf, u16 buf_len, u16 flags);

/**
  * @brief  for bt to trigger wl pll ready
  * @param[in]  need [1-wake wl pll ready; 0-off wl pll]
  * @return  RTK_SUCCESS or RTK_FAIL.
  */
int wifi_wake_pll_rdy_in_ps_state(u8 need);

/**
  * @brief  for bt on to disable/enable ips/lps function
  * @param[in] enable [DISABLE-disable and leave ips/lps; ENABLE-Release the control of BT ON over IPS/LPS]
  */
void wifi_ps_en_by_bt_on(u8 enable);

/**
 * @brief  Enable Wi-Fi interface-2.
 * @return
 *    - @ref RTK_SUCCESS : Success, wifi open RTW_MODE_AP .
 *    - @ref RTK_FAIL : Otherwise.
 */
int _wifi_on_ap(void);
/**
 * @brief  Disable Wi-Fi interface-2.
 * @return
 *    - @ref RTK_SUCCESS : Close ap mode.
 *    - @ref RTK_FAIL : Otherwise.
 */
int _wifi_off_ap(void);

/**
 * @brief  Set WPS IE in Probe Request/Probe Response/Beacon/
 * 	Association Request/Association Response.
 * @param[in]  wlan_idx: The wlan interface index, can be STA_WLAN_INDEX or SOFTAP_WLAN_INDEX.
 * @param[in]  buf: The pointer to buf which store the WPS IE.
 * @param[in]  buf_len: The length of buf.
 * @param[in]  flags:
 *                  - \b P2PWPS_PROBE_REQ_IE: Set WPS ie in Probe Request.
 *                  - \b P2PWPS_PROBE_RSP_IE: Set WPS ie in Probe Response.
 *                  - \b P2PWPS_BEACON_IE: Set WPS ie in Beacon.
 *                  - \b P2PWPS_ASSOC_REQ_IE: Set WPS ie in Association Request.
 *                  - \b P2PWPS_ASSOC_RSP_IE: Set WPS ie in Association Response.
 * @return  @ref RTK_FAIL or @ref RTK_SUCCESS.
 */
int wifi_set_gen_ie(unsigned char wlan_idx, char *buf, u16 buf_len, u16 flags);

void wifi_event_init(void);
void wifi_indication(u32 event, u8 *evt_info, s32 evt_len);
void wifi_indication_ext(u32 event, u8 *info_buf, s32 info_len, u8 *frame_buf, s32 frame_len);

#ifdef __cplusplus
}
#endif

#endif

