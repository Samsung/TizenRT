/**
  ******************************************************************************
  * @file    rtw_80211spec_struct.h
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

#ifndef	__RTW_80211SPEC_STRUCT_H_
#define __RTW_80211SPEC_STRUCT_H_

struct wlan_ethhdr_t {
	unsigned char		daddr[ETH_ALEN];
	unsigned char		saddr[ETH_ALEN];
	unsigned short		type;
};

/* Fasten in ROM, cautious to revise. */
RTW_PACK_STRUCT_BEGIN
struct rtw_ieee80211_hdr {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
	u8 addr4[ETH_ALEN];
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

RTW_PACK_STRUCT_BEGIN
struct rtw_ieee80211_hdr_3addr {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

RTW_PACK_STRUCT_BEGIN
struct rtw_ieee80211_hdr_3addr_qos {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
	u16     qc;
}  RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

RTW_PACK_STRUCT_BEGIN
struct eapol {
	u8 snap[6];
	u16 ethertype;
	u8 version;
	u8 type;
	u16 length;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

RTW_PACK_STRUCT_BEGIN
struct HT_caps_element {
	union {
		struct {
			unsigned short	HT_caps_info;
			unsigned char	AMPDU_para;
			unsigned char	MCS_rate[16];
			unsigned short	HT_ext_caps;
			unsigned int	Beamforming_caps;
			unsigned char	ASEL_caps;
		}
		HT_cap_element;
		unsigned char HT_cap[26];
	}
	u;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

RTW_PACK_STRUCT_BEGIN
struct AC_param {
	unsigned char		ACI_AIFSN;
	unsigned char		CW;
	unsigned short	TXOP_limit;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

//move to rom, image cannot use this struct
RTW_PACK_STRUCT_BEGIN
struct WMM_para_element_rom {
	unsigned char		QoS_info;
	unsigned char		reserved;
	struct AC_param	ac_param[4];
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

/**
* struct rtw_ieee80211_ht_cap - HT capabilities
*
* This structure refers to "HT capabilities element" as
* described in 802.11n draft section 7.3.2.52
*/

RTW_PACK_STRUCT_BEGIN
struct rtw_ieee80211_ht_cap {
	unsigned short 	cap_info;
	unsigned char 	ampdu_params_info;
	unsigned char 	supp_mcs_set[16];
	unsigned short 	extended_ht_cap_info;
	unsigned int		tx_BF_cap_info;
	unsigned char	       antenna_selection_info;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

/**
 * struct rtw_ieee80211_ht_cap - HT additional information
 *
 * This structure refers to "HT information element" as
 * described in 802.11n draft section 7.3.2.53
 */
RTW_PACK_STRUCT_BEGIN
struct ieee80211_ht_addt_info {
	unsigned char 	control_chan;
	unsigned char		ht_param;
	unsigned short	operation_mode;
	unsigned short	stbc_param;
	unsigned char		basic_set[16];
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

RTW_PACK_STRUCT_BEGIN
struct ht_info_element_t {
	unsigned char	primary_channel;
	unsigned char	infos[5];
	unsigned char	MCS_rate[16];
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

RTW_PACK_STRUCT_BEGIN
struct addba_request {
	unsigned char		dialog_token;
	unsigned short	BA_para_set;
	unsigned short	BA_timeout_value;
	unsigned short	BA_starting_seqctrl;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

RTW_PACK_STRUCT_BEGIN
struct ieee80211_snap_hdr {

	u8    dsap;   /* always 0xAA */
	u8    ssap;   /* always 0xAA */
	u8    ctrl;   /* always 0x03 */
	u8    oui[P80211_OUI_LEN];    /* organizational universal id */

} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

#define SNAP_SIZE sizeof(struct ieee80211_snap_hdr)
#define SNAP_PROTOCOL_ID_SIZE	2

/* Represent channel details, subset of ieee80211_channel */
struct rtw_ieee80211_channel {
	/* enum ieee80211_band band; */
	/* u16 center_freq; */
	u16 hw_value;
	u32 flags;
	/* int max_antenna_gain; */
	/* int max_power; */
	/* int max_reg_power; */
	/* bool beacon_found; */
	/* u32 orig_flags; */
	/* int orig_mag; */
	/* int orig_mpwr; */
};

struct _NDIS_802_11_VARIABLE_IEs {
	uint8_t  ElementID;
	uint8_t  Length;
	/* Start pointer of data in this element. */
	uint8_t  data[1];
	/* Followed by Length bytes of data, but names are not defined here. */
};

struct dot11_ssid {
	u32  SsidLength;
	u8  Ssid[RTW_ESSID_MAX_SIZE + 4];
};

struct _NDIS_802_11_FIXED_IEs {
	u8  Timestamp[8];
	u16  BeaconInterval;
	u16  Capabilities;
};


#endif

