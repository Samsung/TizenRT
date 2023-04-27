/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef	__RTW_80211SPEC_STRUCT_H_
#define __RTW_80211SPEC_STRUCT_H_

struct wlan_ethhdr_t {
	unsigned char		daddr[ETH_ALEN];
	unsigned char		saddr[ETH_ALEN];
	unsigned short		type;
};

/* Fasten in ROM, cautious to revise. */
struct rtw_ieee80211_hdr {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
	u8 addr4[ETH_ALEN];
} _PACKED_;

struct rtw_ieee80211_hdr_3addr {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
} _PACKED_;


struct rtw_ieee80211_hdr_3addr_qos {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
	u16     qc;
}  _PACKED_;

struct eapol {
	u8 snap[6];
	u16 ethertype;
	u8 version;
	u8 type;
	u16 length;
} _PACKED_;

struct HT_caps_element {
	union {
		struct {
			unsigned short	HT_caps_info;
			unsigned char	AMPDU_para;
			unsigned char	MCS_rate[16];
			unsigned short	HT_ext_caps;
			unsigned int	Beamforming_caps;
			unsigned char	ASEL_caps;
		} _PACKED_
		HT_cap_element;
		unsigned char HT_cap[26];
	} _PACKED_
	u;
} _PACKED_;

struct AC_param {
	unsigned char		ACI_AIFSN;
	unsigned char		CW;
	unsigned short	TXOP_limit;
} _PACKED_;

//move to rom, image cannot use this struct
struct WMM_para_element_rom {
	unsigned char		QoS_info;
	unsigned char		reserved;
	struct AC_param	ac_param[4];
} _PACKED_;

/**
* struct rtw_ieee80211_ht_cap - HT capabilities
*
* This structure refers to "HT capabilities element" as
* described in 802.11n draft section 7.3.2.52
*/

struct rtw_ieee80211_ht_cap {
	unsigned short 	cap_info;
	unsigned char 	ampdu_params_info;
	unsigned char 	supp_mcs_set[16];
	unsigned short 	extended_ht_cap_info;
	unsigned int		tx_BF_cap_info;
	unsigned char	       antenna_selection_info;
} _PACKED_;

/**
 * struct rtw_ieee80211_ht_cap - HT additional information
 *
 * This structure refers to "HT information element" as
 * described in 802.11n draft section 7.3.2.53
 */
struct ieee80211_ht_addt_info {
	unsigned char 	control_chan;
	unsigned char		ht_param;
	unsigned short	operation_mode;
	unsigned short	stbc_param;
	unsigned char		basic_set[16];
} _PACKED_;

struct HT_info_element {
	unsigned char	primary_channel;
	unsigned char	infos[5];
	unsigned char	MCS_rate[16];
} _PACKED_;

struct ADDBA_request {
	unsigned char		dialog_token;
	unsigned short	BA_para_set;
	unsigned short	BA_timeout_value;
	unsigned short	BA_starting_seqctrl;
} _PACKED_;

struct ieee80211_snap_hdr {

	u8    dsap;   /* always 0xAA */
	u8    ssap;   /* always 0xAA */
	u8    ctrl;   /* always 0x03 */
	u8    oui[P80211_OUI_LEN];    /* organizational universal id */

} _PACKED_;

#define SNAP_SIZE sizeof(struct ieee80211_snap_hdr)

#if defined(CONFIG_IEEE80211K) || defined(CONFIG_RTW_WNM)
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
#endif

typedef struct _NDIS_802_11_VARIABLE_IEs {
	uint8_t  ElementID;
	uint8_t  Length;
	uint8_t  data[1];
} NDIS_802_11_VARIABLE_IEs, *PNDIS_802_11_VARIABLE_IEs;

typedef struct _NDIS_802_11_SSID {
	u32  SsidLength;
	u8  Ssid[RTW_ESSID_MAX_SIZE + 4];
} NDIS_802_11_SSID;

typedef struct _NDIS_802_11_FIXED_IEs {
	u8  Timestamp[8];
	u16  BeaconInterval;
	u16  Capabilities;
} NDIS_802_11_FIXED_IEs;


#endif

