/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef __WLAN_80211_UTILS_H__
#define __WLAN_80211_UTILS_H__

#include "utils_scsc.h"

/* Frame Control Fields */
#define SLSI_80211_FCTRL_TYPE            0x000c
#define SLSI_80211_FCTRL_SUBTYPE            0x00f0
#define SLSI_80211_FCTRL_TODS             0x0100
#define SLSI_80211_FCTRL_FROMDS           0x0200

/* Frame Types */
#define SLSI_80211_TYPE_MGMT            0x0000
#define SLSI_80211_TYPE_CTRL             0x0004
#define SLSI_80211_TYPE_DATA            0x0008

/* Management Frames */
#define SLSI_80211_SUBTYPE_ASSOC_REQ       0x0000
#define SLSI_80211_SUBTYPE_ASSOC_RESP      0x0010
#define SLSI_80211_SUBTYPE_REASSOC_REQ     0x0020
#define SLSI_80211_SUBTYPE_REASSOC_RESP    0x0030
#define SLSI_80211_SUBTYPE_PROBE_REQ       0x0040
#define SLSI_80211_SUBTYPE_PROBE_RESP      0x0050
#define SLSI_80211_SUBTYPE_BEACON          0x0080
#define SLSI_80211_SUBTYPE_DISASSOC        0x00A0
#define SLSI_80211_SUBTYPE_AUTH            0x00B0
#define SLSI_80211_SUBTYPE_DEAUTH          0x00C0
#define SLSI_80211_SUBTYPE_ACTION          0x00D0

#define SLSI_80211_HT_MCS_MASK_LEN               10

#define SLSI_80211_MAX_SSID_LEN 32

#define SLSI_MHZ_TO_KHZ(freq)        ((freq) * 1000)
#define SLSI_KHZ_TO_MHZ(freq)        ((freq) * 1000)
#define SLSI_DBI_TO_MBI(gain)        ((gain) * 100)
#define SLSI_DBM_TO_MBM(gain)       ((gain) * 100)

#define WLAN_OUI_MICROSOFT              0x0050f2
#define WLAN_OUI_TYPE_MICROSOFT_WPA     1
#define WLAN_OUI_TYPE_MICROSOFT_WMM     2
#define WLAN_OUI_TYPE_MICROSOFT_WPS     4

#define WLAN_OUI_WFA            0x506f9a
#define WLAN_OUI_TYPE_WFA_P2P       9
//#define BIT(x) (1 << (x))
#define WLAN_CAPABILITY_PRIVACY BIT(4)
#define WLAN_EID_VENDOR_SPECIFIC 221

#define WPA_PROTO_WPA BIT(0)
#define WPA_PROTO_RSN BIT(1)
#define WPA_PROTO_WAPI BIT(2)
#define WPA_PROTO_OSEN BIT(3)

#define WPA_AUTH_ALG_OPEN BIT(0)
#define WPA_AUTH_ALG_SHARED BIT(1)
#define WPA_AUTH_ALG_LEAP BIT(2)
#define WPA_AUTH_ALG_FT BIT(3)
#define WPA_AUTH_ALG_SAE BIT(4)

#define WPA_CIPHER_WEP40 BIT(1)
#define WPA_CIPHER_WEP104 BIT(2)

#define SLSI_REG_RULE(start, end, bw, gain, eirp, reg_flags)    \
{                                   \
	.start_freq_khz = SLSI_MHZ_TO_KHZ(start),           \
	.end_freq_khz = SLSI_MHZ_TO_KHZ(end),           \
	.max_bandwidth_khz = SLSI_MHZ_TO_KHZ(bw),           \
	.max_antenna_gain = SLSI_DBI_TO_MBI(gain),      \
	.max_eirp = SLSI_DBM_TO_MBM(eirp),          \
	.flags = reg_flags,                     \
}

enum slsi_80211_eid {
	WLAN_EID_SSID = 0,
	WLAN_EID_SUPP_RATES = 1,
	WLAN_EID_FH_PARAMS = 2,		/* reserved now */
	WLAN_EID_DS_PARAMS = 3,
	WLAN_EID_CF_PARAMS = 4,
	WLAN_EID_TIM = 5,
	WLAN_EID_IBSS_PARAMS = 6,
	WLAN_EID_COUNTRY = 7,
	WLAN_EID_RSN = 48,
	WLAN_EID_HT_CAPABILITY = 45,
};

enum slsi_80211_band {
	SLSI_80211_BAND_2GHZ,
	SLSI_80211_BAND_5GHZ,
	SLSI_80211_NUM_BANDS
};

enum slsi_80211_channel_flags {
	SLSI_80211_CHAN_DISABLED = 1 << 0,
	SLSI_80211_CHAN_NO_IR = 1 << 1,
	SLSI_80211_CHAN_RADAR = 1 << 2,
};

enum slsi_80211_iftype {
	SLSI_80211_IFTYPE_UNSPECIFIED,
	SLSI_80211_IFTYPE_ADHOC,
	SLSI_80211_IFTYPE_STATION,
	SLSI_80211_IFTYPE_AP,
	SLSI_80211_IFTYPE_P2P_CLIENT,
	SLSI_80211_IFTYPE_P2P_GO,
	SLSI_80211_IFTYPE_P2P_DEVICE,
};

enum slsi_80211_chan_width {
	SLSI_80211_CHAN_WIDTH_20_NOHT,
	SLSI_80211_CHAN_WIDTH_20,
	SLSI_80211_CHAN_WIDTH_40,
	SLSI_80211_CHAN_WIDTH_80,
};

enum slsi_80211_auth_type {
	SLSI_80211_AUTHTYPE_OPEN_SYSTEM,
	SLSI_80211_AUTHTYPE_SHARED_KEY,
	SLSI_80211_AUTHTYPE_FT,
	SLSI_80211_AUTHTYPE_AUTOMATIC,
	SLSI_80211_AUTHTYPE_NETWORK_EAP,
	SLSI_80211_AUTHTYPE_SAE,
	SLSI_80211_AUTHTYPE_UNSUPPORTED,
};

/**
 * enum slsi_80211_tx_power_setting - TX power adjustment
 * @SLSI_80211_TX_POWER_AUTOMATIC: automatically determine transmit power
 * @SLSI_80211_TX_POWER_LIMITED: limit TX power by the mBm parameter
 * @SLSI_80211_TX_POWER_FIXED: fix TX power to the mBm parameter
 */
enum slsi_80211_tx_power_setting {
	SLSI_80211_TX_POWER_AUTOMATIC,
	SLSI_80211_TX_POWER_LIMITED,
	SLSI_80211_TX_POWER_FIXED,
};

/**
 * enum slsi_80211_channel_type - channel type
 * @SLSI_80211_CHAN_NO_HT: 20 MHz, non-HT channel
 * @SLSI_80211_CHAN_HT20: 20 MHz HT channel
 * @SLSI_80211_CHAN_HT40MINUS: HT40 channel, secondary channel
 *  below the control channel
 * @SLSI_80211_CHAN_HT40PLUS: HT40 channel, secondary channel
 *  above the control channel
 */
enum slsi_80211_channel_type {
	SLSI_80211_CHAN_NO_HT,
	SLSI_80211_CHAN_HT20,
	SLSI_80211_CHAN_HT40MINUS,
	SLSI_80211_CHAN_HT40PLUS
};

/**
 * enum slsi_80211_key_type - Key Type
 * @SLSI_80211_KEYTYPE_GROUP: Group (broadcast/multicast) key
 * @SLSI_80211_KEYTYPE_PAIRWISE: Pairwise (unicast/individual) key
 * @SLSI_80211_KEYTYPE_PEERKEY: PeerKey (DLS)
 * @NUM_SLSI_80211_KEYTYPES: number of defined key types
 */
enum slsi_80211_key_type {
	SLSI_80211_KEYTYPE_GROUP,
	SLSI_80211_KEYTYPE_PAIRWISE,
	SLSI_80211_KEYTYPE_PEERKEY,

	NUM_SLSI_80211_KEYTYPES
};

/**
 * enum slsi_80211_reg_rule_flags - regulatory rule flags
 *
 * @SLSI_80211_RRF_NO_OFDM: OFDM modulation not allowed
 * @SLSI_80211_RRF_NO_CCK: CCK modulation not allowed
 * @SLSI_80211_RRF_NO_INDOOR: indoor operation not allowed
 * @SLSI_80211_RRF_NO_OUTDOOR: outdoor operation not allowed
 * @SLSI_80211_RRF_DFS: DFS support is required to be used
 * @SLSI_80211_RRF_PTP_ONLY: this is only for Point To Point links
 * @SLSI_80211_RRF_PTMP_ONLY: this is only for Point To Multi Point links
 * @SLSI_80211_RRF_NO_IR: no mechanisms that initiate radiation are allowed,
 *  this includes probe requests or modes of operation that require
 *  beaconing.
 * @SLSI_80211_RRF_AUTO_BW: maximum available bandwidth should be calculated
 *  base on contiguous rules and wider channels will be allowed to cross
 *  multiple contiguous/overlapping frequency ranges.
 * @SLSI_80211_RRF_GO_CONCURRENT: See &SLSI_80211_FREQUENCY_ATTR_GO_CONCURRENT
 * @SLSI_80211_RRF_NO_HT40MINUS: channels can't be used in HT40- operation
 * @SLSI_80211_RRF_NO_HT40PLUS: channels can't be used in HT40+ operation
 * @SLSI_80211_RRF_NO_80MHZ: 80MHz operation not allowed
 * @SLSI_80211_RRF_NO_160MHZ: 160MHz operation not allowed
 */
enum slsi_80211_reg_rule_flags {
	SLSI_80211_RRF_NO_OFDM = 1 << 0,
	SLSI_80211_RRF_NO_CCK = 1 << 1,
	SLSI_80211_RRF_NO_INDOOR = 1 << 2,
	SLSI_80211_RRF_NO_OUTDOOR = 1 << 3,
	SLSI_80211_RRF_DFS = 1 << 4,
	SLSI_80211_RRF_PTP_ONLY = 1 << 5,
	SLSI_80211_RRF_PTMP_ONLY = 1 << 6,
	SLSI_80211_RRF_NO_IR = 1 << 7,
	__SLSI_80211_RRF_NO_IBSS = 1 << 8,
	SLSI_80211_RRF_AUTO_BW = 1 << 11,
	SLSI_80211_RRF_GO_CONCURRENT = 1 << 12,
	SLSI_80211_RRF_NO_HT40MINUS = 1 << 13,
	SLSI_80211_RRF_NO_HT40PLUS = 1 << 14,
	SLSI_80211_RRF_NO_80MHZ = 1 << 15,
	SLSI_80211_RRF_NO_160MHZ = 1 << 16,
};

#define SLSI_80211_RRF_PASSIVE_SCAN SLSI_80211_RRF_NO_IR
#define SLSI_80211_RRF_NO_IBSS      SLSI_80211_RRF_NO_IR
#define SLSI_80211_RRF_NO_IR        SLSI_80211_RRF_NO_IR
#define SLSI_80211_RRF_NO_HT40      (SLSI_80211_RRF_NO_HT40MINUS | \
				     SLSI_80211_RRF_NO_HT40PLUS)

/* 802.11n HT capabilities masks (for cap_info) */
#define SLSI_80211_HT_CAP_LDPC_CODING       0x0001
#define SLSI_80211_HT_CAP_SUP_WIDTH_20_40   0x0002
#define SLSI_80211_HT_CAP_GRN_FLD       0x0010
#define SLSI_80211_HT_CAP_SGI_20            0x0020
#define SLSI_80211_HT_CAP_SGI_40            0x0040
#define SLSI_80211_HT_CAP_RX_STBC       0x0300
#define     SLSI_80211_HT_CAP_RX_STBC_SHIFT 8
#define SLSI_80211_HT_CAP_DELAY_BA      0x0400
#define SLSI_80211_HT_CAP_MAX_AMSDU     0x0800
#define SLSI_80211_HT_CAP_RESERVED      0x2000
#define SLSI_80211_HT_CAP_40MHZ_INTOLERANT  0x4000

enum wpa_alg {
	SLSI_WPA_ALG_NONE,
	SLSI_WPA_ALG_WEP,
	SLSI_WPA_ALG_TKIP,
	SLSI_WPA_ALG_CCMP,
	SLSI_WPA_ALG_IGTK,
	SLSI_WPA_ALG_PMK,
	SLSI_WPA_ALG_GCMP,
	SLSI_WPA_ALG_SMS4
};

/*
 * Maximum length of AMPDU that the STA can receive.
 * Length = 2 ^ (13 + max_ampdu_length_exp) - 1 (octets)
 */
enum slsi_80211_max_ampdu_length_exp {
	SLSI_80211_HT_MAX_AMPDU_8K = 0,
	SLSI_80211_HT_MAX_AMPDU_16K = 1,
	SLSI_80211_HT_MAX_AMPDU_32K = 2,
	SLSI_80211_HT_MAX_AMPDU_64K = 3
};

/* Minimum MPDU start spacing */
enum slsi_80211_min_mpdu_spacing {
	SLSI_80211_HT_MPDU_DENSITY_NONE = 0,	/* No restriction */
	SLSI_80211_HT_MPDU_DENSITY_0_25 = 1,	/* 1/4 usec */
	SLSI_80211_HT_MPDU_DENSITY_0_5 = 2,	/* 1/2 usec */
	SLSI_80211_HT_MPDU_DENSITY_1 = 3,	/* 1 usec */
	SLSI_80211_HT_MPDU_DENSITY_2 = 4,	/* 2 usec */
	SLSI_80211_HT_MPDU_DENSITY_4 = 5,	/* 4 usec */
	SLSI_80211_HT_MPDU_DENSITY_8 = 6,	/* 8 usec */
	SLSI_80211_HT_MPDU_DENSITY_16 = 7	/* 16 usec */
};

/* cipher suite selectors */
#define SLSI_WLAN_CIPHER_SUITE_USE_GROUP    0x000FAC00
#define SLSI_WLAN_CIPHER_SUITE_WEP40        0x000FAC01
#define SLSI_WLAN_CIPHER_SUITE_TKIP     0x000FAC02
/* reserved:                0x000FAC03 */
#define SLSI_WLAN_CIPHER_SUITE_CCMP     0x000FAC04
#define SLSI_WLAN_CIPHER_SUITE_WEP104   0x000FAC05
#define SLSI_WLAN_CIPHER_SUITE_AES_CMAC 0x000FAC06
#define SLSI_WLAN_CIPHER_SUITE_NO_GROUP_ADDR    0x000FAC07
#define SLSI_WLAN_CIPHER_SUITE_GCMP     0x000FAC08
#define SLSI_WLAN_CIPHER_SUITE_GCMP_256 0x000FAC09
#define SLSI_WLAN_CIPHER_SUITE_CCMP_256 0x000FAC0A
#define SLSI_WLAN_CIPHER_SUITE_BIP_GMAC_128 0x000FAC0B
#define SLSI_WLAN_CIPHER_SUITE_BIP_GMAC_256 0x000FAC0C
#define SLSI_WLAN_CIPHER_SUITE_BIP_CMAC_256 0x000FAC0D

#define SLSI_WLAN_CIPHER_SUITE_SMS4     0x00147201

struct slsi_80211_reg_rule {
	u32 start_freq_khz;
	u32 end_freq_khz;
	u32 max_bandwidth_khz;
	u32 max_antenna_gain;
	u32 max_eirp;
	u32 flags;
};

struct slsi_80211_channel {
	enum slsi_80211_band band;
	u16 center_freq;
	u16 hw_value;
	u32 flags;
} STRUCT_PACKED;

/**
 * struct slsi_80211_chan_def - channel definition
 * @chan: the (control) channel
 * @width: channel width
 * @center_freq1: center frequency of first segment
 * @center_freq2: center frequency of second segment
 *	(only with 80+80 MHz)
 */
struct slsi_80211_chan_def {
	struct slsi_80211_channel *chan;
	enum slsi_80211_chan_width width;
	u32 center_freq1;
	u32 center_freq2;
};

struct slsi_80211_rate {
	u32 flags;
	u16 bitrate;
	u16 hw_value;
	u16 hw_value_short;
} STRUCT_PACKED;

struct slsi_80211_mcs_info {
	u8 rx_mask[SLSI_80211_HT_MCS_MASK_LEN];
	__le16 rx_highest;
	u8 tx_params;
} STRUCT_PACKED;

struct slsi_80211_sta_ht_cap {
	u16 cap;
	bool ht_supported;
	u8 ampdu_factor;
	u8 ampdu_density;
	struct slsi_80211_mcs_info mcs;
} STRUCT_PACKED;

struct slsi_80211_supported_band {
	struct slsi_80211_channel *channels;
	struct slsi_80211_rate *bitrates;
	enum slsi_80211_band band;
	int n_channels;
	int n_bitrates;
	struct slsi_80211_sta_ht_cap ht_cap;
} STRUCT_PACKED;

struct slsi_key_params {
	const u8 *key;
	const u8 *seq;
	int key_len;
	int seq_len;
	u32 cipher;
};

struct slsi_80211_vendor_ie {
	u8 element_id;
	u8 len;
	u8 oui[3];
	u8 oui_type;
} STRUCT_PACKED;

struct slsi_80211_hdr {
	__le16 frame_control;
	__le16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	__le16 seq_ctrl;
	u8 addr4[ETH_ALEN];
} STRUCT_PACKED __aligned(2);

struct slsi_80211_mgmt {
	__le16 frame_control;
	__le16 duration;
	u8 da[ETH_ALEN];
	u8 sa[ETH_ALEN];
	u8 bssid[ETH_ALEN];
	__le16 seq_ctrl;
	union {
		struct {
			__le64 timestamp;
			__le16 beacon_int;
			__le16 capab_info;
			u8 variable[0];
		} STRUCT_PACKED beacon;
		struct {
			u8 variable[0];
		} STRUCT_PACKED probe_req;
		struct {
			__le64 timestamp;
			__le16 beacon_int;
			__le16 capab_info;
			u8 variable[0];
		} STRUCT_PACKED probe_resp;
		struct {
			__le16 auth_alg;
			__le16 auth_transaction;
			__le16 status_code;
			u8 variable[0];
		} STRUCT_PACKED auth;
		struct {
			__le16 reason_code;
		} STRUCT_PACKED deauth;
		struct {
			__le16 capab_info;
			__le16 listen_interval;
			u8 variable[0];
		} STRUCT_PACKED assoc_req;
		struct {
			__le16 capab_info;
			__le16 status_code;
			__le16 aid;
			u8 variable[0];
		} STRUCT_PACKED assoc_resp, reassoc_resp;
		struct {
			__le16 capab_info;
			__le16 listen_interval;
			u8 current_ap[ETH_ALEN];
			u8 variable[0];
		} STRUCT_PACKED reassoc_req;
		struct {
			__le16 reason_code;
		} STRUCT_PACKED disassoc;
	} u;
} STRUCT_PACKED __aligned(2);

/**
 * Functions to check the Frame type
 * @frame_ctrl: in little endian
 */
static inline bool slsi_80211_is_mgmt(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE)) == cpu_to_le16(SLSI_80211_TYPE_MGMT));
}

static inline bool slsi_80211_is_ctrl(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE)) == cpu_to_le16(SLSI_80211_TYPE_CTRL));
}

static inline bool slsi_80211_is_data(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE)) == cpu_to_le16(SLSI_80211_TYPE_DATA));
}

/**
 * Functions to check if frame is a particular Managment Frame Subtype
 * @frame_ctrl: in little endian
 */
static inline bool slsi_80211_is_assoc_req(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE | SLSI_80211_FCTRL_SUBTYPE)) == cpu_to_le16(SLSI_80211_TYPE_MGMT | SLSI_80211_SUBTYPE_ASSOC_REQ));
}

static inline bool slsi_80211_is_assoc_resp(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE | SLSI_80211_FCTRL_SUBTYPE)) == cpu_to_le16(SLSI_80211_TYPE_MGMT | SLSI_80211_SUBTYPE_ASSOC_RESP));
}

static inline bool slsi_80211_is_reassoc_req(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE | SLSI_80211_FCTRL_SUBTYPE)) == cpu_to_le16(SLSI_80211_TYPE_MGMT | SLSI_80211_SUBTYPE_REASSOC_REQ));
}

static inline bool slsi_80211_is_reassoc_resp(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE | SLSI_80211_FCTRL_SUBTYPE)) == cpu_to_le16(SLSI_80211_TYPE_MGMT | SLSI_80211_SUBTYPE_REASSOC_RESP));
}

static inline bool slsi_80211_is_probe_req(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE | SLSI_80211_FCTRL_SUBTYPE)) == cpu_to_le16(SLSI_80211_TYPE_MGMT | SLSI_80211_SUBTYPE_PROBE_REQ));
}

static inline bool slsi_80211_is_probe_resp(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE | SLSI_80211_FCTRL_SUBTYPE)) == cpu_to_le16(SLSI_80211_TYPE_MGMT | SLSI_80211_SUBTYPE_PROBE_RESP));
}

static inline bool slsi_80211_is_beacon(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE | SLSI_80211_FCTRL_SUBTYPE)) == cpu_to_le16(SLSI_80211_TYPE_MGMT | SLSI_80211_SUBTYPE_BEACON));
}

static inline bool slsi_80211_is_disassoc(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE | SLSI_80211_FCTRL_SUBTYPE)) == cpu_to_le16(SLSI_80211_TYPE_MGMT | SLSI_80211_SUBTYPE_DISASSOC));
}

static inline bool slsi_80211_is_auth(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE | SLSI_80211_FCTRL_SUBTYPE)) == cpu_to_le16(SLSI_80211_TYPE_MGMT | SLSI_80211_SUBTYPE_AUTH));
}

static inline bool slsi_80211_is_deauth(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE | SLSI_80211_FCTRL_SUBTYPE)) == cpu_to_le16(SLSI_80211_TYPE_MGMT | SLSI_80211_SUBTYPE_DEAUTH));
}

static inline bool slsi_80211_is_action(__le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_80211_FCTRL_TYPE | SLSI_80211_FCTRL_SUBTYPE)) == cpu_to_le16(SLSI_80211_TYPE_MGMT | SLSI_80211_SUBTYPE_ACTION));
}

/**
 * Check if TO DS is set in the Frame Control
 */
static inline bool slsi_80211_has_tods(__le16 fc)
{
	return (fc & cpu_to_le16(SLSI_80211_FCTRL_TODS)) != 0;
}

/**
 * Get pointer to the destination address of given IEEE 802.11 frame
 */
static inline u8 *slsi_80211_get_DA(struct slsi_80211_hdr *hdr)
{
	if (slsi_80211_has_tods(hdr->frame_control)) {
		return hdr->addr3;
	} else {
		return hdr->addr1;
	}
}
#endif							/* __WLAN_80211_UTILS_H__ */
