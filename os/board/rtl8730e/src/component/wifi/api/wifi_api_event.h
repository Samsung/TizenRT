/**
  ******************************************************************************
  * @file    wifi_api_event.h
  * @author
  * @version
  * @date
  * @brief   This file provides the functions related to event handler mechanism.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __WIFI_API_EVENT_H
#define __WIFI_API_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup WIFI_API Wi-Fi APIs
 *  @brief      WIFI_API module
 *  @{
 */
/**********************************************************************************************
 *                                     wifi event structures
 *********************************************************************************************/
/** @addtogroup WIFI_Exported_Types Wi-Fi Exported Types
* @{
*/

/** @addtogroup WIFI_Exported_Enumeration_Types Enumeration Type
 * @{
 */

/**
  * @brief Join status flags for @ref RTW_EVENT_JOIN_STATUS.
  */
enum rtw_join_status {
	RTW_JOINSTATUS_UNKNOWN = 0,

	/* Intermediate connection states (new states should be added before RTW_JOINSTATUS_SUCCESS). */
	RTW_JOINSTATUS_STARTING,                  /**< Connection initiation. */
	RTW_JOINSTATUS_SCANNING,                  /**< Network scanning. */
	RTW_JOINSTATUS_AUTHENTICATING,            /**< Authentication in progress. */
	RTW_JOINSTATUS_AUTHENTICATED,             /**< Authentication completed. */
	RTW_JOINSTATUS_ASSOCIATING,               /**< Association in progress. */
	RTW_JOINSTATUS_ASSOCIATED,                /**< Association completed. */
	RTW_JOINSTATUS_4WAY_HANDSHAKING,          /**< 4-way handshake in progress. */
	RTW_JOINSTATUS_4WAY_HANDSHAKE_DONE,       /**< 4-way handshake completed. */
	RTW_JOINSTATUS_SUCCESS,                   /**< Connection successful. */

	/* Other connection result states (new states should be added after RTW_JOINSTATUS_SUCCESS). */
	RTW_JOINSTATUS_FAIL,                      /**< Connection failed. */
	RTW_JOINSTATUS_DISCONNECT,                /**< Disconnected after successful connection. */
};

/**
  * @brief  Wi-Fi event identifiers.
  */
enum rtw_event_id {
	RTW_EVENT_AP_STA_ASSOC = 0,          /**< SoftAP mode: Station associated */
	RTW_EVENT_AP_STA_DISASSOC,           /**< SoftAP mode: Station disassociated */
	RTW_EVENT_JOIN_STATUS,	          /**< STA mode: Connection status change (see example_wifi_event.c) */
	RTW_EVENT_CSI_DONE,               /**< CSI data ready (see example_wifi_csi.c) */
	RTW_EVENT_WPA_STA_WPS_START,      /**< STA mode: WPS procedure started */
	RTW_EVENT_WPA_WPS_FINISH,         /**< STA mode: WPS procedure completed */
	RTW_EVENT_WPA_EAPOL_START,        /**< STA mode: WPA enterprise authentication started */
	RTW_EVENT_WPA_EAPOL_RECVD,        /**< STA mode: EAPOL packet received during WPA enterprise authentication  */
	RTW_EVENT_DHCP_STATUS,            /**< STA mode: DHCP status report (see @ref rtw_event_dhcp_status)  */

	RTW_EVENT_MAX,
};
/** @} End of WIFI_Exported_Enumeration_Types group*/

/** @addtogroup WIFI_Exported_Structure_Types Structure Type
 * @{
 */

/**
 * @brief  Wi-Fi event handle struct.
 */
struct rtw_event_hdl_func_t {
	u16 evt_id;  /**< refer to @ref rtw_event_id */
	void (*handler)(u8 *evt_info);
};

/**
  * @brief  Report info for event @ref RTW_EVENT_JOIN_STATUS
  */
struct rtw_event_join_status_info {
	/* private paras */
	union {
		struct rtw_event_authenticating {

		} authenticating;  /* RTW_JOINSTATUS_AUTHENTICATING */
		struct rtw_event_authenticated {
			u16 reason_or_status_code;  /**< 802.11 reason code or status code from AP.*/
		} authenticated;  /* RTW_JOINSTATUS_AUTHENTICATED */
		struct rtw_event_associating {
			u8 is_reassoc;
		} associating;  /* RTW_JOINSTATUS_ASSOCIATING */
		struct rtw_event_associated {
			u16 reason_or_status_code;
			u8 is_reassoc;
		} associated;  /* RTW_JOINSTATUS_ASSOCIATED */
		struct rtw_event_join_fail {
			s32 fail_reason;  /**< Failure reason, refer to @ref RTK_FAIL, -@ref RTK_ERR_WIFI_CONN_INVALID_KEY, etc. */
			u16 reason_or_status_code;
		} fail;  /* RTW_JOINSTATUS_FAIL */
		struct rtw_event_disconnect {
			u16 disconn_reason;  /**< Disconnect reason, refer to @ref rtw_disconn_reason. */
		} disconnect;  /* RTW_JOINSTATUS_DISCONNECT */
	} priv;

	/* common paras */
	u8 status;  /**< refer to @ref rtw_join_status */
	u8 channel;
	u8 bssid[ETH_ALEN];
	s8 rssi;
	/* At the same time as reporting event info, frame content needs to be reported, which will be followed by event info. */
	u32 frame_len;  /**< 0: there is no frame followed; larger than 0: there is frame_len bytes of frame followed*/
	u8 frame[];  /**< if there is frame followed, point to head address */
};

/**
  * @brief  Report info for event @ref RTW_EVENT_AP_STA_ASSOC
  */
struct rtw_event_ap_sta_assoc {
	u8 sta_mac[ETH_ALEN];
	u32 frame_len;
	u8 frame[];
};

/**
  * @brief  Report info for event @ref RTW_EVENT_AP_STA_DISASSOC
  */
struct rtw_event_ap_sta_disassoc {
	u8 sta_mac[ETH_ALEN];
};

/**
  * @brief  Report info for event @ref RTW_EVENT_WPA_EAPOL_START
  */
struct rtw_event_wpa_eapol_start {
	u8 dst_mac[ETH_ALEN];
};

/**
  * @brief  Report info for event @ref RTW_EVENT_WPA_STA_WPS_START
  */
struct rtw_event_wpa_sta_wps_start {
	u8 peer_mac[ETH_ALEN];
};

/**
  * @brief  Report info for event @ref RTW_EVENT_DHCP_STATUS
  */
struct rtw_event_dhcp_status {
	u8 dhcp_status;		/**< only report DHCP_ADDRESS_ASSIGNED, DHCP_STOP, DHCP_TIMEOUT */
};

#pragma pack(1) /* csi report info should be 1 byte alignment */
/**
 * @brief  Layout of CSI report info.
 */

struct rtw_event_csi_report_info {
	u16 csi_signature;          /**< Unique pattern (0xABCD) to detect a new CSI packet. */
	u8 mac_addr[6];	            /**< MAC address of transmitter (Active CSI) or receiver (Passive CSI) for CSI triggering frame. */
	u8 trig_addr[6];	        /**< MAC address of destination (Active CSI) or source (Passive CSI) for CSI triggering frame (Reserved in METHOD4). */
	u32 hw_assigned_timestamp;  /**< CSI timestamp, unit: us. */
	u32 csi_sequence;           /**< CSI data sequence number. */
	u8 csi_valid;               /**< Indicates if current CSI raw data is valid. */
	u8 channel;                 /**< Operation channel. */
	u8 bandwidth;               /**< Operating bandwidth (0: 20MHz, 1: 40MHz). */
	u8 rx_rate;                 /**< RX packet rate used to obtain CSI info. */
	u8 protocol_mode;           /**< Protocol mode of response packet (0: OFDM, 1: HT, 2: VHT, 3: HE). */
	u16 num_sub_carrier;        /**< Number of subcarriers in CSI raw data */
	u8 num_bit_per_tone;        /**< CSI data word length (sum of I and Q). E.g., if using @ref RTW_CSI_ACCU_1BYTE accuracy (S(8,X)), num_bit_per_tone = 16. */
	s8 rssi[2];                 /**< rssi[0]: dBm, rssi[1]: reserved */
	s8 evm[2];                  /**< Error Vector Magnitude in dB (Reserved). */
	u8 rxsc;                    /**< Sub-20MHz channel used for packet transmission. */
	u8 n_rx;                    /**< Reserved. */
	u8 n_sts;                   /**< Reserved. */
	u8 trig_flag;               /**< CSI trigger source indicator (valid only in METHOD4, 0 if `trig_addr` valid) */
	u8 rsvd[6];                 /**< Ensure the total sizes of struct is 4-byte alignment */
	u32 csi_data_length;        /**< CSI raw data length, unit: byte. */
	u8 csi_data[];              /**< CSI raw data head address */
};
#pragma pack()

/** @} End of WIFI_Exported_Structure_Types group*/
/** @} End of WIFI_Exported_Types group*/
/** @} End of WIFI_API group*/
#ifdef __cplusplus
}
#endif

#endif //__WIFI_API_EVENT_H

