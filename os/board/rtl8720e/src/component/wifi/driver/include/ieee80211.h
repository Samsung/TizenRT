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
#ifndef __IEEE80211_H
#define __IEEE80211_H

#ifndef CONFIG_RTL8711FW
#else

#include <list.h>

#endif

#define MGMT_QUEUE_NUM 5

#define ETH_ALEN	6
#define ETH_TYPE_LEN		2
#define PAYLOAD_TYPE_LEN	1

/* STA flags */
#define WLAN_STA_AUTH BIT(0)
#define WLAN_STA_ASSOC BIT(1)
#define WLAN_STA_PS BIT(2)
#define WLAN_STA_TIM BIT(3)
#define WLAN_STA_PERM BIT(4)
#define WLAN_STA_AUTHORIZED BIT(5)
#define WLAN_STA_PENDING_POLL BIT(6) /* pending activity poll not ACKed */
#define WLAN_STA_SHORT_PREAMBLE BIT(7)
#define WLAN_STA_PREAUTH BIT(8)
#define WLAN_STA_WME BIT(9)
#define WLAN_STA_MFP BIT(10)
#define WLAN_STA_HT BIT(11)
#define WLAN_STA_WPS BIT(12)
#define WLAN_STA_MAYBE_WPS BIT(13)
#define WLAN_STA_NONERP BIT(31)

#define IEEE_CMD_SET_WPA_PARAM			1
#define IEEE_CMD_SET_WPA_IE				2
#define IEEE_CMD_SET_ENCRYPTION			3
#define IEEE_CMD_MLME						4

#define IEEE_PARAM_WPA_ENABLED				1
#define IEEE_PARAM_TKIP_COUNTERMEASURES		2
#define IEEE_PARAM_DROP_UNENCRYPTED			3
#define IEEE_PARAM_PRIVACY_INVOKED			4
#define IEEE_PARAM_AUTH_ALGS					5
#define IEEE_PARAM_IEEE_802_1X				6
#define IEEE_PARAM_WPAX_SELECT				7

#define AUTH_ALG_OPEN_SYSTEM			0x1
#define AUTH_ALG_SHARED_KEY			0x2
#define AUTH_ALG_SAE					0x8
#define AUTH_ALG_LEAP				0x00000004

#define IEEE_MLME_STA_DEAUTH				1
#define IEEE_MLME_STA_DISASSOC			2

#define IEEE_CRYPT_ERR_UNKNOWN_ALG			2
#define IEEE_CRYPT_ERR_UNKNOWN_ADDR			3
#define IEEE_CRYPT_ERR_CRYPT_INIT_FAILED		4
#define IEEE_CRYPT_ERR_KEY_SET_FAILED			5
#define IEEE_CRYPT_ERR_TX_KEY_SET_FAILED		6
#define IEEE_CRYPT_ERR_CARD_CONF_FAILED		7


#define	IEEE_CRYPT_ALG_NAME_LEN			16

#define WPA_CIPHER_NONE 			BIT(0)
#define WPA_CIPHER_WEP40 			BIT(1)
#define WPA_CIPHER_WEP104 		BIT(2)
#define WPA_CIPHER_TKIP 			BIT(3)
#define WPA_CIPHER_CCMP 			BIT(4)
#define WPA_CIPHER_AES_128_CMAC 	BIT(5)
#define WPA_CIPHER_GCMP 			BIT(6)
#define WPA_CIPHER_SMS4 			BIT(7)
#define WPA_CIPHER_GCMP_256 		BIT(8)
#define WPA_CIPHER_CCMP_256 		BIT(9)
#define WPA_CIPHER_BIP_GMAC_128 	BIT(11)
#define WPA_CIPHER_BIP_GMAC_256 	BIT(12)
#define WPA_CIPHER_BIP_CMAC_256 	BIT(13)
#define WPA_CIPHER_GTK_NOT_USED 	BIT(14)


#define WPA_SELECTOR_LEN 4
//extern u16 RTW_WPA_VERSION ;
//extern u8 WPA_AUTH_KEY_MGMT_NONE[];
//extern u8 WPA_AUTH_KEY_MGMT_UNSPEC_802_1X[];
//extern u8 WPA_AUTH_KEY_MGMT_PSK_OVER_802_1X[];
//extern u8 WPA_CIPHER_SUITE_WRAP[];

#define WPA_KEY_MGMT_IEEE8021X BIT(0)
#define WPA_KEY_MGMT_PSK BIT(1)
#define WPA_KEY_MGMT_NONE BIT(2)
#define WPA_KEY_MGMT_IEEE8021X_NO_WPA BIT(3)
#define WPA_KEY_MGMT_WPA_NONE BIT(4)
#define WPA_KEY_MGMT_FT_IEEE8021X BIT(5)
#define WPA_KEY_MGMT_FT_PSK BIT(6)
#define WPA_KEY_MGMT_IEEE8021X_SHA256 BIT(7)
#define WPA_KEY_MGMT_PSK_SHA256 BIT(8)
#define WPA_KEY_MGMT_WPS BIT(9)
#define WPA_KEY_MGMT_SAE BIT(10)
#define WPA_KEY_MGMT_FT_SAE BIT(11)
#define WPA_KEY_MGMT_WAPI_PSK BIT(12)
#define WPA_KEY_MGMT_WAPI_CERT BIT(13)
#define WPA_KEY_MGMT_CCKM BIT(14)
#define WPA_KEY_MGMT_OSEN BIT(15)
#define WPA_KEY_MGMT_IEEE8021X_SUITE_B BIT(16)
#define WPA_KEY_MGMT_IEEE8021X_SUITE_B_192 BIT(17)
#define WPA_KEY_MGMT_FILS_SHA256 BIT(18)
#define WPA_KEY_MGMT_FILS_SHA384 BIT(19)
#define WPA_KEY_MGMT_FT_FILS_SHA256 BIT(20)
#define WPA_KEY_MGMT_FT_FILS_SHA384 BIT(21)
#define WPA_KEY_MGMT_OWE BIT(22)
#define WPA_KEY_MGMT_DPP BIT(23)

#define RSN_HEADER_LEN 4
#define RSN_SELECTOR_LEN 4
#define RSNXE_MIN_LEN 3

//tern u8 RSN_CIPHER_SUITE_WRAP[];

typedef enum _RATEID_IDX_ {
	RATEID_IDX_BGN_40M_2SS = 0,
	RATEID_IDX_BGN_40M_1SS = 1,
	RATEID_IDX_BGN_20M_2SS_BN = 2,
	RATEID_IDX_BGN_20M_1SS_BN = 3,
	RATEID_IDX_GN_N2SS = 4,
	RATEID_IDX_GN_N1SS = 5,
	RATEID_IDX_BG = 6,
	RATEID_IDX_G = 7,
	RATEID_IDX_B = 8,
	RATEID_IDX_VHT_2SS = 9,
	RATEID_IDX_VHT_1SS = 10,
} RATEID_IDX, *PRATEID_IDX;

#define IsSupported24G(band_type) ((band_type) & BAND_CAP_2G? _TRUE : _FALSE)
#define IsSupported5G(band_type) ((band_type) & BAND_CAP_5G ?  _TRUE : _FALSE)

#define IsLegacyOnly(NetType)  ((NetType) == ((NetType) & (WLAN_MD_11BG | WLAN_MD_11A)))

#define IsSupportedTxCCK(NetType) ((NetType) & (WLAN_MD_11B) ? _TRUE : _FALSE)
#define IsSupportedTxOFDM(NetType) ((NetType) & (WLAN_MD_11G | WLAN_MD_11A) ? _TRUE : _FALSE)

#define is_supported_ht(NetType) ((NetType) & (WLAN_MD_11N) ? _TRUE : _FALSE)
#define is_supported_vht(NetType) ((NetType) & (WLAN_MD_11AC) ? _TRUE : _FALSE)
#define is_supported_he(NetType) ((NetType) & (WLAN_MD_11AX) ? _TRUE : _FALSE)

typedef struct ieee_param {
	u32 cmd;
	u8 sta_addr[ETH_ALEN];
	union {
		struct {
			u8 name;
			u32 value;
		} wpa_param;
		struct {
			u32 len;
			u8 reserved[32];
#ifdef __CC_ARM
			u8 data[1];
#else
			u8 data[0];
#endif
		} wpa_ie;
		struct {
			int command;
			int reason_code;
		} mlme;
		struct {
			u8 alg[IEEE_CRYPT_ALG_NAME_LEN];
			u8 set_tx;
			u32 err;
			u8 idx;
			u8 seq[8]; /* sequence counter (set: RX, get: TX) */
			u16 key_len;
#ifdef __CC_ARM
			u8 key[1];
#else
			u8 key[0];
#endif
		} crypt;

		struct {
			u16 aid;
			u16 capability;
			int flags;
			u8 tx_supp_rates[16];
			struct rtw_ieee80211_ht_cap ht_cap;
		} add_sta;
		struct {
			u8	reserved[2];//for set max_num_sta
#ifdef __CC_ARM
			u8	buf[1];
#else
			u8	buf[0];
#endif
		} bcn_ie;

	} u;
} ieee_param;

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
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
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
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
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
struct rtw_ieee80211_hdr_qos {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
	u8 addr4[ETH_ALEN];
	u16	qc;
}  RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
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
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
struct eapol {
	u8 snap[6];
	u16 ethertype;
	u8 version;
	u8 type;
	u16 length;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif

#define IEEE80211_3ADDR_LEN 24
#define IEEE80211_4ADDR_LEN 30
#define IEEE80211_FCS_LEN    4

#define MIN_FRAG_THRESHOLD     256U
#define	MAX_FRAG_THRESHOLD     2346U

/* Frame control field constants */
#define RTW_IEEE80211_FCTL_VERS		0x0002
#define RTW_IEEE80211_FCTL_FTYPE		0x000c
#define RTW_IEEE80211_FCTL_STYPE		0x00f0
#define RTW_IEEE80211_FCTL_TODS		0x0100
#define RTW_IEEE80211_FCTL_FROMDS		0x0200
#define RTW_IEEE80211_FCTL_MOREFRAGS	0x0400
#define RTW_IEEE80211_FCTL_RETRY		0x0800
#define RTW_IEEE80211_FCTL_PM		0x1000
#define RTW_IEEE80211_FCTL_MOREDATA	0x2000
#define RTW_IEEE80211_FCTL_WEP		0x4000
#define RTW_IEEE80211_FCTL_ORDER		0x8000

#define RTW_IEEE80211_FTYPE_MGMT		0x0000
#define RTW_IEEE80211_FTYPE_CTL		0x0004
#define RTW_IEEE80211_FTYPE_DATA		0x0008

/* management */
#define RTW_IEEE80211_STYPE_ASSOC_REQ	0x0000
#define RTW_IEEE80211_STYPE_ASSOC_RESP 	0x0010
#define RTW_IEEE80211_STYPE_REASSOC_REQ	0x0020
#define RTW_IEEE80211_STYPE_REASSOC_RESP	0x0030
#define RTW_IEEE80211_STYPE_PROBE_REQ	0x0040
#define RTW_IEEE80211_STYPE_PROBE_RESP	0x0050
#define RTW_IEEE80211_STYPE_BEACON		0x0080
#define RTW_IEEE80211_STYPE_ATIM		0x0090
#define RTW_IEEE80211_STYPE_DISASSOC	0x00A0
#define RTW_IEEE80211_STYPE_AUTH		0x00B0
#define RTW_IEEE80211_STYPE_DEAUTH		0x00C0
#define RTW_IEEE80211_STYPE_ACTION		0x00D0

/* control */
#define RTW_IEEE80211_STYPE_PSPOLL		0x00A0
#define RTW_IEEE80211_STYPE_RTS		0x00B0
#define RTW_IEEE80211_STYPE_CTS		0x00C0
#define RTW_IEEE80211_STYPE_ACK		0x00D0
#define RTW_IEEE80211_STYPE_CFEND		0x00E0
#define RTW_IEEE80211_STYPE_CFENDACK	0x00F0

/* data */
#define RTW_IEEE80211_STYPE_DATA		0x0000
#define RTW_IEEE80211_STYPE_DATA_CFACK	0x0010
#define RTW_IEEE80211_STYPE_DATA_CFPOLL	0x0020
#define RTW_IEEE80211_STYPE_DATA_CFACKPOLL	0x0030
#define RTW_IEEE80211_STYPE_NULLFUNC	0x0040
#define RTW_IEEE80211_STYPE_CFACK		0x0050
#define RTW_IEEE80211_STYPE_CFPOLL		0x0060
#define RTW_IEEE80211_STYPE_CFACKPOLL	0x0070
#define RTW_IEEE80211_QOS_DATAGRP		0x0080
#define RTW_IEEE80211_QoS_DATAGRP		RTW_IEEE80211_QOS_DATAGRP
/* Mesh Control 802.11s */
#define RTW_IEEE80211_QOS_CTL_MESH_CONTROL_PRESENT  BIT(8)
/* Mesh flags */
#define MESH_FLAGS_AE_A4 	0x1
#define MESH_FLAGS_AE_A5_A6	0x2
#define MESH_FLAGS_AE		0x3
#define MESH_FLAGS_PS_DEEP	0x4

#define RTW_IEEE80211_SCTL_FRAG		0x000F
#define RTW_IEEE80211_SCTL_SEQ		0xFFF0


#define RTW_ERP_INFO_NON_ERP_PRESENT BIT(0)
#define RTW_ERP_INFO_USE_PROTECTION BIT(1)
#define RTW_ERP_INFO_BARKER_PREAMBLE_MODE BIT(2)

/* QoS,QOS */
#define NORMAL_ACK			0
#define NO_ACK				1
#define NON_EXPLICIT_ACK	2
#define BLOCK_ACK			3

#ifndef ETH_P_PAE
#define ETH_P_PAE 0x888E /* Port Access Entity (IEEE 802.1X) */
#endif /* ETH_P_PAE */

#define ETH_P_PREAUTH 0x88C7 /* IEEE 802.11i pre-authentication */

#define ETH_P_ECONET	0x0018

#ifndef ETH_P_80211_RAW
#define ETH_P_80211_RAW (ETH_P_ECONET + 1)
#endif

/* IEEE 802.11 defines */

#define P80211_OUI_LEN 3

RTW_PACK_STRUCT_BEGIN
struct ieee80211_snap_hdr {

	u8    dsap;   /* always 0xAA */
	u8    ssap;   /* always 0xAA */
	u8    ctrl;   /* always 0x03 */
	u8    oui[P80211_OUI_LEN];    /* organizational universal id */

} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

#define SNAP_SIZE sizeof(struct ieee80211_snap_hdr)

#define WLAN_FC_GET_TYPE(fc) ((fc) & RTW_IEEE80211_FCTL_FTYPE)
#define WLAN_FC_GET_STYPE(fc) ((fc) & RTW_IEEE80211_FCTL_STYPE)

#define WLAN_QC_GET_TID(qc) ((qc) & 0x0f)

#define WLAN_GET_SEQ_FRAG(seq) ((seq) & RTW_IEEE80211_SCTL_FRAG)
#define WLAN_GET_SEQ_SEQ(seq)  ((seq) & RTW_IEEE80211_SCTL_SEQ)



/* Authentication algorithms */
#define WLAN_AUTH_OPEN 0
#define WLAN_AUTH_SHARED_KEY 1

#define WLAN_AUTH_CHALLENGE_LEN 128

#define WLAN_CAPABILITY_BSS (1<<0)
#define WLAN_CAPABILITY_IBSS (1<<1)
#define WLAN_CAPABILITY_CF_POLLABLE (1<<2)
#define WLAN_CAPABILITY_CF_POLL_REQUEST (1<<3)
#define WLAN_CAPABILITY_PRIVACY (1<<4)
#define WLAN_CAPABILITY_SHORT_PREAMBLE (1<<5)
#define WLAN_CAPABILITY_PBCC (1<<6)
#define WLAN_CAPABILITY_CHANNEL_AGILITY (1<<7)
#define WLAN_CAPABILITY_SHORT_SLOT (1<<10)

/* Status codes */
#define WLAN_STATUS_SUCCESS 0
#define WLAN_STATUS_UNSPECIFIED_FAILURE 1
#define WLAN_STATUS_CAPS_UNSUPPORTED 10
#define WLAN_STATUS_REASSOC_NO_ASSOC 11
#define WLAN_STATUS_ASSOC_DENIED_UNSPEC 12
#define WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG 13
#define WLAN_STATUS_UNKNOWN_AUTH_TRANSACTION 14
#define WLAN_STATUS_CHALLENGE_FAIL 15
#define WLAN_STATUS_AUTH_TIMEOUT 16
#define WLAN_STATUS_AP_UNABLE_TO_HANDLE_NEW_STA 17
#define WLAN_STATUS_ASSOC_DENIED_RATES 18
/* 802.11b */
#define WLAN_STATUS_ASSOC_DENIED_NOSHORT 19
#define WLAN_STATUS_ASSOC_DENIED_NOPBCC 20
#define WLAN_STATUS_ASSOC_DENIED_NOAGILITY 21

/* WPA3-SAE*/
#define WLAN_STATUS_ANTI_CLOGGING_TOKEN_REQ 		76
#define WLAN_STATUS_UNSUPPORTED_FCC_GROUP   		77
#define WLAN_STATUS_UNKNOWN_PASSWORD_IDENTIFIER		123

/* Reason codes */
#define WLAN_REASON_UNSPECIFIED 1
#define WLAN_REASON_PREV_AUTH_NOT_VALID 2
#define WLAN_REASON_DEAUTH_LEAVING 3
#define WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY 4
#define WLAN_REASON_DISASSOC_AP_BUSY 5
#define WLAN_REASON_CLASS2_FRAME_FROM_NONAUTH_STA 6
#define WLAN_REASON_CLASS3_FRAME_FROM_NONASSOC_STA 7
#define WLAN_REASON_DISASSOC_STA_HAS_LEFT 8
#define WLAN_REASON_STA_REQ_ASSOC_WITHOUT_AUTH 9
#define	WLAN_REASON_MIC_FAILURE 14
#define WLAN_REASON_4WAY_HANDSHAKE_TIMEOUT 15
#define WLAN_REASON_ACTIVE_ROAM 65533
#define WLAN_REASON_JOIN_WRONG_CHANNEL       65534
#define WLAN_REASON_EXPIRATION_CHK 65535

/* 802.11s */
#define WLAN_REASON_MESH_PEER_CANCELED 52
#define WLAN_REASON_MESH_MAX_PEERS 53
#define WLAN_REASON_MESH_CONFIG 54
#define WLAN_REASON_MESH_CLOSE 55
#define WLAN_REASON_MESH_MAX_RETRIES 56
#define WLAN_REASON_MESH_CONFIRM_TIMEOUT 57
#define WLAN_REASON_MESH_INVALID_GTK 58
#define WLAN_REASON_MESH_INCONSISTENT_PARAM 59
#define WLAN_REASON_MESH_INVALID_SECURITY 60
#define WLAN_REASON_MESH_PATH_ERROR 61
#define WLAN_REASON_MESH_PATH_NOFORWARD  62
#define WLAN_REASON_MESH_PATH_DEST_UNREACHABLE 63
#define WLAN_REASON_MAC_EXISTS_IN_MBSS 64
#define WLAN_REASON_MESH_CHAN_REGULATORY 65
#define WLAN_REASON_MESH_CHAN 66

/* Information Element IDs */
#define WLAN_EID_SSID 0
#define WLAN_EID_SUPP_RATES 1
#define WLAN_EID_FH_PARAMS 2
#define WLAN_EID_DS_PARAMS 3
#define WLAN_EID_CF_PARAMS 4
#define WLAN_EID_TIM 5
#define WLAN_EID_IBSS_PARAMS 6
#define WLAN_EID_CHALLENGE 16
/* EIDs defined by IEEE 802.11h - START */
#define WLAN_EID_PWR_CONSTRAINT 32
#define WLAN_EID_PWR_CAPABILITY 33
#define WLAN_EID_TPC_REQUEST 34
#define WLAN_EID_TPC_REPORT 35
#define WLAN_EID_SUPPORTED_CHANNELS 36
#define WLAN_EID_CHANNEL_SWITCH 37
#define WLAN_EID_MEASURE_REQUEST 38
#define WLAN_EID_MEASURE_REPORT 39
#define WLAN_EID_QUITE 40
#define WLAN_EID_IBSS_DFS 41
/* EIDs defined by IEEE 802.11h - END */
#define WLAN_EID_ERP_INFO 42
#define WLAN_EID_HT_CAP 45
#define WLAN_EID_RSN 48
#define WLAN_EID_EXT_SUPP_RATES 50
#define WLAN_EID_MOBILITY_DOMAIN 54
#define WLAN_EID_FAST_BSS_TRANSITION 55
#define WLAN_EID_TIMEOUT_INTERVAL 56
#define WLAN_EID_RIC_DATA 57
#define WLAN_EID_HT_OPERATION 61
#define WLAN_EID_SECONDARY_CHANNEL_OFFSET 62
#define WLAN_EID_MULTIPLE_BSSID 71
#define WLAN_EID_20_40_BSS_COEXISTENCE 72
#define WLAN_EID_20_40_BSS_INTOLERANT 73
#define WLAN_EID_OVERLAPPING_BSS_SCAN_PARAMS 74
#define WLAN_EID_MMIE 76
#define WLAN_EID_NONTRANSMITTED_BSSID_CAPABILITY 83
#define WLAN_EID_MULTIPLE_BSSID_INDEX 85
#define WLAN_EID_VHT_CAPABILITY 191
#define WLAN_EID_VHT_OPERATION 192
#define WLAN_EID_VHT_OPERATING_MODE_NOTIFICATION 199
#define WLAN_EID_MESH_CONFIG 113
#define WLAN_EID_MESH_ID 114
#define WLAN_EID_MESH_PEERING_MANAGEMENT 117
#define WLAN_EID_GANN 125
#define WLAN_EID_RANN 126
#define WLAN_EID_EXT_CAP 127
#define WLAN_EID_PREQ 130
#define WLAN_EID_PREP 131
#define WLAN_EID_PERR 132
#define WLAN_EID_MIC 140
#define WLAN_EID_TWT 216
#define WLAN_EID_VENDOR_SPECIFIC 221
#define WLAN_EID_GENERIC (WLAN_EID_VENDOR_SPECIFIC)

/* EID Extension */
#define WLAN_EID_EXTENSION 255
#define WLAN_EID_EXT_OWE_DH_PARAM 32	//WPA3
#define WLAN_EID_EXT_HE_CAPABILITY 35
#define WLAN_EID_EXT_HE_OPERATION 36
#define WLAN_EID_EXT_HE_MU_EDCA 38
#define WLAN_EID_EXT_SPATIAL_REUSE_PARA 39
#define WLAN_EID_EXT_BSS_COLOR_CHANGE 42
#define WLAN_EID_EXT_MULTIPLE_BSSID_CONFIGURATION 55	//MBSSID
#define WLAN_EID_EXT_NON_INGERITANCE_ELEMENT 56		//MBSSID

#define WLAN_EID_EXT_CAP_MAX_LEN 10

#define IEEE80211_MGMT_HDR_LEN 24
#define IEEE80211_DATA_HDR3_LEN 24
#define IEEE80211_DATA_HDR4_LEN 30


#define IEEE80211_STATMASK_SIGNAL (1<<0)
#define IEEE80211_STATMASK_RSSI (1<<1)
#define IEEE80211_STATMASK_NOISE (1<<2)
#define IEEE80211_STATMASK_RATE (1<<3)
#define IEEE80211_STATMASK_WEMASK 0x7


#define IEEE80211_CCK_MODULATION    (1<<0)
#define IEEE80211_OFDM_MODULATION   (1<<1)

#define IEEE80211_24GHZ_BAND     (1<<0)
#define IEEE80211_52GHZ_BAND     (1<<1)

#define IEEE80211_CCK_RATE_LEN  		4
#define IEEE80211_NUM_OFDM_RATESLEN	8


#define IEEE80211_CCK_RATE_1MB		        0x02
#define IEEE80211_CCK_RATE_2MB		        0x04
#define IEEE80211_CCK_RATE_5MB		        0x0B
#define IEEE80211_CCK_RATE_11MB		        0x16
#define IEEE80211_OFDM_RATE_LEN 		8
#define IEEE80211_OFDM_RATE_6MB		        0x0C
#define IEEE80211_OFDM_RATE_9MB		        0x12
#define IEEE80211_OFDM_RATE_12MB		0x18
#define IEEE80211_OFDM_RATE_18MB		0x24
#define IEEE80211_OFDM_RATE_24MB		0x30
#define IEEE80211_OFDM_RATE_36MB		0x48
#define IEEE80211_OFDM_RATE_48MB		0x60
#define IEEE80211_OFDM_RATE_54MB		0x6C
#define IEEE80211_BASIC_RATE_MASK		0x80

#define IEEE80211_CCK_RATE_1MB_MASK		(1<<0)
#define IEEE80211_CCK_RATE_2MB_MASK		(1<<1)
#define IEEE80211_CCK_RATE_5MB_MASK		(1<<2)
#define IEEE80211_CCK_RATE_11MB_MASK		(1<<3)
#define IEEE80211_OFDM_RATE_6MB_MASK		(1<<4)
#define IEEE80211_OFDM_RATE_9MB_MASK		(1<<5)
#define IEEE80211_OFDM_RATE_12MB_MASK		(1<<6)
#define IEEE80211_OFDM_RATE_18MB_MASK		(1<<7)
#define IEEE80211_OFDM_RATE_24MB_MASK		(1<<8)
#define IEEE80211_OFDM_RATE_36MB_MASK		(1<<9)
#define IEEE80211_OFDM_RATE_48MB_MASK		(1<<10)
#define IEEE80211_OFDM_RATE_54MB_MASK		(1<<11)


#define IEEE80211_CCK_RATES_MASK	        0x0000000F
#define IEEE80211_CCK_BASIC_RATES_MASK	(IEEE80211_CCK_RATE_1MB_MASK | \
	IEEE80211_CCK_RATE_2MB_MASK)
#define IEEE80211_CCK_DEFAULT_RATES_MASK	(IEEE80211_CCK_BASIC_RATES_MASK | \
        IEEE80211_CCK_RATE_5MB_MASK | \
        IEEE80211_CCK_RATE_11MB_MASK)

#define IEEE80211_OFDM_RATES_MASK		0x00000FF0
#define IEEE80211_OFDM_BASIC_RATES_MASK	(IEEE80211_OFDM_RATE_6MB_MASK | \
	IEEE80211_OFDM_RATE_12MB_MASK | \
	IEEE80211_OFDM_RATE_24MB_MASK)
#define IEEE80211_OFDM_DEFAULT_RATES_MASK	(IEEE80211_OFDM_BASIC_RATES_MASK | \
	IEEE80211_OFDM_RATE_9MB_MASK  | \
	IEEE80211_OFDM_RATE_18MB_MASK | \
	IEEE80211_OFDM_RATE_36MB_MASK | \
	IEEE80211_OFDM_RATE_48MB_MASK | \
	IEEE80211_OFDM_RATE_54MB_MASK)
#define IEEE80211_DEFAULT_RATES_MASK (IEEE80211_OFDM_DEFAULT_RATES_MASK | \
                                IEEE80211_CCK_DEFAULT_RATES_MASK)

#define IEEE80211_NUM_OFDM_RATES	    8
#define IEEE80211_NUM_CCK_RATES	            4
#define IEEE80211_OFDM_SHIFT_MASK_A         4

typedef enum _RATR_TABLE_MODE {
	RATR_INX_WIRELESS_NGB = 0,	/* BGN 40 Mhz 2SS 1SS */
	RATR_INX_WIRELESS_NG = 1,		/* GN or N */
	RATR_INX_WIRELESS_NB = 2,		/* BGN 20 Mhz 2SS 1SS  or BN */
	RATR_INX_WIRELESS_N = 3,
	RATR_INX_WIRELESS_GB = 4,
	RATR_INX_WIRELESS_G = 5,
	RATR_INX_WIRELESS_B = 6,
	RATR_INX_WIRELESS_MC = 7,
	RATR_INX_WIRELESS_AC_N = 8,
} RATR_TABLE_MODE, *PRATR_TABLE_MODE;

#define	MGN_MCS0_SG			0xc0
#define	MGN_MCS1_SG			0xc1
#define	MGN_MCS2_SG			0xc2
#define	MGN_MCS3_SG			0xc3
#define	MGN_MCS4_SG			0xc4
#define	MGN_MCS5_SG			0xc5
#define	MGN_MCS6_SG			0xc6
#define	MGN_MCS7_SG			0xc7
#define	MGN_MCS8_SG			0xc8
#define	MGN_MCS9_SG			0xc9
#define	MGN_MCS10_SG		0xca
#define	MGN_MCS11_SG		0xcb
#define	MGN_MCS12_SG		0xcc
#define	MGN_MCS13_SG		0xcd
#define	MGN_MCS14_SG		0xce
#define	MGN_MCS15_SG		0xcf

#define IS_HT_RATE(_rate)				(((_rate) & 0x80) ? _TRUE : _FALSE)
#define IS_CCK_RATE(_rate) 				(MGN_1M == _rate || _rate == MGN_2M || _rate == MGN_5_5M || _rate == MGN_11M )
#define IS_OFDM_RATE(_rate)				(MGN_6M <= _rate && _rate <= MGN_54M )

/* NOTE: This data is for statistical purposes; not all hardware provides this
 *       information for frames received.  Not setting these will not cause
 *       any adverse affects. */
struct ieee80211_rx_stats {
	//u32 mac_time[2];
	s8 rssi;
	u8 signal;
	u8 noise;
	u8 received_channel;
	u16 rate; /* in 100 kbps */
	//u8 control;
	u8 mask;
	u8 freq;
	u16 len;
};

/* IEEE 802.11 requires that STA supports concurrent reception of at least
 * three fragmented frames. This define can be increased to support more
 * concurrent frames, but it should be noted that each entry can consume about
 * 2 kB of RAM and increasing cache size will slow down frame reassembly. */
#define IEEE80211_FRAG_CACHE_LEN 4

struct ieee80211_frag_entry {
	u32 first_frag_time;
	uint seq;
	uint last_frag;
	uint qos;   //jackson
	uint tid;	//jackson
	struct sk_buff *skb;
	u8 src_addr[ETH_ALEN];
	u8 dst_addr[ETH_ALEN];
};

struct ieee80211_stats {
	uint tx_unicast_frames;
	uint tx_multicast_frames;
	uint tx_fragments;
	uint tx_unicast_octets;
	uint tx_multicast_octets;
	uint tx_deferred_transmissions;
	uint tx_single_retry_frames;
	uint tx_multiple_retry_frames;
	uint tx_retry_limit_exceeded;
	uint tx_discards;
	uint rx_unicast_frames;
	uint rx_multicast_frames;
	uint rx_fragments;
	uint rx_unicast_octets;
	uint rx_multicast_octets;
	uint rx_fcs_errors;
	uint rx_discards_no_buffer;
	uint tx_discards_wrong_sa;
	uint rx_discards_undecryptable;
	uint rx_message_in_msg_fragments;
	uint rx_message_in_bad_msg_fragments;
};

struct ieee80211_softmac_stats {
	uint rx_ass_ok;
	uint rx_ass_err;
	uint rx_probe_rq;
	uint tx_probe_rs;
	uint tx_beacons;
	uint rx_auth_rq;
	uint rx_auth_rs_ok;
	uint rx_auth_rs_err;
	uint tx_auth_rq;
	uint no_auth_rs;
	uint no_ass_rs;
	uint tx_ass_rq;
	uint rx_ass_rq;
	uint tx_probe_rq;
	uint reassoc;
	uint swtxstop;
	uint swtxawake;
};

#define SEC_KEY_1         (1<<0)
#define SEC_KEY_2         (1<<1)
#define SEC_KEY_3         (1<<2)
#define SEC_KEY_4         (1<<3)
#define SEC_ACTIVE_KEY    (1<<4)
#define SEC_AUTH_MODE     (1<<5)
#define SEC_UNICAST_GROUP (1<<6)
#define SEC_LEVEL         (1<<7)
#define SEC_ENABLED       (1<<8)

#define SEC_LEVEL_0      0 /* None */
#define SEC_LEVEL_1      1 /* WEP 40 and 104 bit */
#define SEC_LEVEL_2      2 /* Level 1 + TKIP */
#define SEC_LEVEL_2_CKIP 3 /* Level 1 + CKIP */
#define SEC_LEVEL_3      4 /* Level 2 + CCMP */

#define WEP_KEYS 4
#define WEP_KEY_LEN 13

#ifdef CONFIG_IEEE80211W
#define BIP_MAX_KEYID 5
#define BIP_AAD_SIZE  20
#endif /* CONFIG_IEEE80211W */


#if defined(CONFIG_RTL8711FW)

struct ieee80211_security {
	u16 active_key: 2,
		enabled: 1,
		auth_mode: 2,
		auth_algo: 4,
		unicast_uses_group: 1;
	u8 key_sizes[WEP_KEYS];
	u8 keys[WEP_KEYS][WEP_KEY_LEN];
	u8 level;
	u16 flags;
} __attribute__((packed));

#endif

/*

 802.11 data frame from AP

      ,-------------------------------------------------------------------.
Bytes |  2   |  2   |    6    |    6    |    6    |  2   | 0..2312 |   4  |
      |------|------|---------|---------|---------|------|---------|------|
Desc. | ctrl | dura |  DA/RA  |   TA    |    SA   | Sequ |  frame  |  fcs |
      |      | tion | (BSSID) |         |         | ence |  data   |      |
      `-------------------------------------------------------------------'

Total: 28-2340 bytes

*/

struct ieee80211_header_data {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[6];
	u8 addr2[6];
	u8 addr3[6];
	u16 seq_ctrl;
};

#define BEACON_PROBE_SSID_ID_POSITION 12

/* Management Frame Information Element Types */
#define MFIE_TYPE_SSID       0
#define MFIE_TYPE_RATES      1
#define MFIE_TYPE_FH_SET     2
#define MFIE_TYPE_DS_SET     3
#define MFIE_TYPE_CF_SET     4
#define MFIE_TYPE_TIM        5
#define MFIE_TYPE_IBSS_SET   6
#define MFIE_TYPE_CHALLENGE  16
#define MFIE_TYPE_ERP        42
#define MFIE_TYPE_RSN	     48
#define MFIE_TYPE_RATES_EX   50
#define MFIE_TYPE_GENERIC    221

#if defined(CONFIG_RTL8711FW)

struct ieee80211_info_element_hdr {
	u8 id;
	u8 len;
} __attribute__((packed));

struct ieee80211_info_element {
	u8 id;
	u8 len;
	u8 data[0];
} __attribute__((packed));
#endif

/*
 * These are the data types that can make up management packets
 *
	u16 auth_algorithm;
	u16 auth_sequence;
	u16 beacon_interval;
	u16 capability;
	u8 current_ap[ETH_ALEN];
	u16 listen_interval;
	struct {
		u16 association_id:14, reserved:2;
	} __attribute__ ((packed));
	u32 time_stamp[2];
	u16 reason;
	u16 status;
*/

#define IEEE80211_DEFAULT_TX_ESSID "Penguin"
#define IEEE80211_DEFAULT_BASIC_RATE 10


#if defined(CONFIG_RTL8711FW)


struct ieee80211_authentication {
	struct ieee80211_header_data header;
	u16 algorithm;
	u16 transaction;
	u16 status;
	//struct ieee80211_info_element_hdr info_element;
} __attribute__((packed));


struct ieee80211_probe_response {
	struct ieee80211_header_data header;
	u32 time_stamp[2];
	u16 beacon_interval;
	u16 capability;
	struct ieee80211_info_element info_element;
} __attribute__((packed));

struct ieee80211_probe_request {
	struct ieee80211_header_data header;
	/*struct ieee80211_info_element info_element;*/
} __attribute__((packed));

struct ieee80211_assoc_request_frame {
	struct rtw_ieee80211_hdr_3addr header;
	u16 capability;
	u16 listen_interval;
	//u8 current_ap[ETH_ALEN];
	struct ieee80211_info_element_hdr info_element;
} __attribute__((packed));

struct ieee80211_assoc_response_frame {
	struct rtw_ieee80211_hdr_3addr header;
	u16 capability;
	u16 status;
	u16 aid;
//	struct ieee80211_info_element info_element; /* supported rates */
} __attribute__((packed));
#endif

/* SWEEP TABLE ENTRIES NUMBER*/
#define MAX_SWEEP_TAB_ENTRIES		  42
#define MAX_SWEEP_TAB_ENTRIES_PER_PACKET  7
/* MAX_RATES_LENGTH needs to be 12.  The spec says 8, and many APs
 * only use 8, and then use extended rates for the remaining supported
 * rates.  Other APs, however, stick all of their supported rates on the
 * main rates information element... */
#define MAX_RATES_LENGTH                  ((u8)12)
#define MAX_RATES_EX_LENGTH               ((u8)16)
#define MAX_NETWORK_COUNT                  128
#define MAX_CHANNEL_NUMBER                 161
#define IEEE80211_SOFTMAC_SCAN_TIME	  400
//(HZ / 2)
#define IEEE80211_SOFTMAC_ASSOC_RETRY_TIME (HZ * 2)

#define CRC_LENGTH                 4U

#define MAX_WPA_IE_LEN (256)
#define MAX_WPS_IE_LEN (512)
#define MAX_P2P_IE_LEN (256)
#define MAX_WFD_IE_LEN (128)

#define NETWORK_EMPTY_ESSID (1<<0)
#define NETWORK_HAS_OFDM    (1<<1)
#define NETWORK_HAS_CCK     (1<<2)

#define IEEE80211_DTIM_MBCAST 4
#define IEEE80211_DTIM_UCAST 2
#define IEEE80211_DTIM_VALID 1
#define IEEE80211_DTIM_INVALID 0

#define IEEE80211_PS_DISABLED 0
#define IEEE80211_PS_UNICAST IEEE80211_DTIM_UCAST
#define IEEE80211_PS_MBCAST IEEE80211_DTIM_MBCAST
#define IW_ESSID_MAX_SIZE 32
#define IW_PASSPHRASE_MAX_SIZE 64
#if 0
struct ieee80211_network {
	/* These entries are used to identify a unique network */
	u8 bssid[ETH_ALEN];
	u8 channel;
	/* Ensure null-terminated for any debug msgs */
	u8 ssid[IW_ESSID_MAX_SIZE + 1];
	u8 ssid_len;
	u8	rssi;	//relative signal strength
	u8	sq;		//signal quality

	/* These are network statistics */
	//struct ieee80211_rx_stats stats;
	u16 capability;
	u16	aid;
	u8 rates[MAX_RATES_LENGTH];
	u8 rates_len;
	u8 rates_ex[MAX_RATES_EX_LENGTH];
	u8 rates_ex_len;

	u8 edca_parmsets[18];

	u8 mode;
	u8 flags;
	u8 time_stamp[8];
	u16 beacon_interval;
	u16 listen_interval;
	u16 atim_window;
	u8 wpa_ie[MAX_WPA_IE_LEN];
	size_t wpa_ie_len;
	u8 rsn_ie[MAX_WPA_IE_LEN];
	size_t rsn_ie_len;
	u8 country[6];
	u8 dtim_period;
	u8 dtim_data;
	u8 power_constraint;
	u8 qosinfo;
	u8 qbssload[5];
	u8 network_type;
	int join_res;
	unsigned long	last_scanned;
};
#endif
/*
join_res:
-1: authentication fail
-2: association fail
> 0: TID
*/

enum ieee80211_state {

	/* the card is not linked at all */
	IEEE80211_NOLINK = 0,

	/* IEEE80211_ASSOCIATING* are for BSS client mode
	 * the driver shall not perform RX filtering unless
	 * the state is LINKED.
	 * The driver shall just check for the state LINKED and
	 * defaults to NOLINK for ALL the other states (including
	 * LINKED_SCANNING)
	 */

	/* the association procedure will start (wq scheduling)*/
	IEEE80211_ASSOCIATING,
	IEEE80211_ASSOCIATING_RETRY,

	/* the association procedure is sending AUTH request*/
	IEEE80211_ASSOCIATING_AUTHENTICATING,

	/* the association procedure has successfully authentcated
	 * and is sending association request
	 */
	IEEE80211_ASSOCIATING_AUTHENTICATED,

	/* the link is ok. the card associated to a BSS or linked
	 * to a ibss cell or acting as an AP and creating the bss
	 */
	IEEE80211_LINKED,

	/* same as LINKED, but the driver shall apply RX filter
	 * rules as we are in NO_LINK mode. As the card is still
	 * logically linked, but it is doing a syncro site survey
	 * then it will be back to LINKED state.
	 */
	IEEE80211_LINKED_SCANNING,

};

#define DEFAULT_MAX_SCAN_AGE (15 * HZ)
#define DEFAULT_FTS 2346
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"

#if defined(PLATFORM_FREERTOS) //Baron change func to macro
#define is_multicast_mac_addr(Addr) ((((Addr[0]) & 0x01) == 0x01) && ((Addr[0]) != 0xff))
#define is_broadcast_mac_addr(Addr) ((((Addr[0]) & 0xff) == 0xff) && (((Addr[1]) & 0xff) == 0xff) && \
(((Addr[2]) & 0xff) == 0xff) && (((Addr[3]) & 0xff) == 0xff) && (((Addr[4]) & 0xff) == 0xff) && \
(((Addr[5]) & 0xff) == 0xff))
#define is_zero_mac_addr(Addr)	((Addr[0] == 0x00) && (Addr[1] == 0x00) && (Addr[2] == 0x00) &&   \
                    (Addr[3] == 0x00) && (Addr[4] == 0x00) && (Addr[5] == 0x00))
#endif // PLATFORM_FREERTOS

#define CFG_IEEE80211_RESERVE_FCS (1<<0)
#define CFG_IEEE80211_COMPUTE_FCS (1<<1)

#define MAXTID	16

#define IEEE_A            (1<<0)
#define IEEE_B            (1<<1)
#define IEEE_G            (1<<2)
#define IEEE_MODE_MASK    (IEEE_A|IEEE_B|IEEE_G)

//Baron move to ieee80211.c
int ieee80211_is_empty_essid(const char *essid, int essid_len);
int ieee80211_get_hdrlen(u16 fc);

/* Action category code */
enum rtw_ieee80211_category {
	RTW_WLAN_CATEGORY_SPECTRUM_MGMT = 0,
	RTW_WLAN_CATEGORY_QOS = 1,
	RTW_WLAN_CATEGORY_DLS = 2,
	RTW_WLAN_CATEGORY_BACK = 3,
	RTW_WLAN_CATEGORY_PUBLIC = 4, //IEEE 802.11 public action frames
	RTW_WLAN_CATEGORY_RADIO_MEASUREMENT  = 5,
	RTW_WLAN_CATEGORY_FT = 6,
	RTW_WLAN_CATEGORY_HT = 7,
	RTW_WLAN_CATEGORY_SA_QUERY = 8,
	RTW_WLAN_CATEGORY_WNM = 10,
	RTW_WLAN_CATEGORY_UNPROTECTED_WNM = 11, /* add for CONFIG_IEEE80211W, none 11w also can use */
	RTW_WLAN_CATEGORY_TDLS = 12,
	RTW_WLAN_CATEGORY_MESH_ACTION = 13,
	RTW_WLAN_CATEGORY_MULTIHOP_ACTION = 14,
	RTW_WLAN_CATEGORY_SELF_PROTECTED = 15, /* add for CONFIG_IEEE80211W, none 11w also can use */
	RTW_WLAN_CATEGORY_WMM = 17,
	RTW_WLAN_CATEGORY_VHT = 21,
	RTW_WLAN_CATEGORY_TWT = 22,
	RTW_WLAN_CATEGORY_P2P = 0x7f,//P2P action frames
};

/* SPECTRUM_MGMT action code */
enum rtw_ieee80211_spectrum_mgmt_actioncode {
	RTW_WLAN_ACTION_SPCT_MSR_REQ = 0,
	RTW_WLAN_ACTION_SPCT_MSR_RPRT = 1,
	RTW_WLAN_ACTION_SPCT_TPC_REQ = 2,
	RTW_WLAN_ACTION_SPCT_TPC_RPRT = 3,
	RTW_WLAN_ACTION_SPCT_CHL_SWITCH = 4,
	RTW_WLAN_ACTION_SPCT_EXT_CHL_SWITCH = 5,
};

enum _PUBLIC_ACTION {
	ACT_PUBLIC_BSSCOEXIST = 0, // 20/40 BSS Coexistence
	ACT_PUBLIC_DSE_ENABLE = 1,
	ACT_PUBLIC_DSE_DEENABLE = 2,
	ACT_PUBLIC_DSE_REG_LOCATION = 3,
	ACT_PUBLIC_EXT_CHL_SWITCH = 4,
	ACT_PUBLIC_DSE_MSR_REQ = 5,
	ACT_PUBLIC_DSE_MSR_RPRT = 6,
	ACT_PUBLIC_MP = 7, // Measurement Pilot
	ACT_PUBLIC_DSE_PWR_CONSTRAINT = 8,
	ACT_PUBLIC_VENDOR = 9, // for WIFI_DIRECT
	ACT_PUBLIC_GAS_INITIAL_REQ = 10,
	ACT_PUBLIC_GAS_INITIAL_RSP = 11,
	ACT_PUBLIC_GAS_COMEBACK_REQ = 12,
	ACT_PUBLIC_GAS_COMEBACK_RSP = 13,
	ACT_PUBLIC_TDLS_DISCOVERY_RSP = 14,
	ACT_PUBLIC_LOCATION_TRACK = 15,
	ACT_PUBLIC_MAX
};

/* BACK action code */
enum rtw_ieee80211_back_actioncode {
	RTW_WLAN_ACTION_ADDBA_REQ = 0,
	RTW_WLAN_ACTION_ADDBA_RESP = 1,
	RTW_WLAN_ACTION_DELBA = 2,
};

enum rtw_ieee80211_spec_mgnt_actioncode {
	RTW_WLAN_ACTION_CSA = 4,
	RTW_WLAN_ACTION_EXTENDED_CSA = 5,
};

/* HT features action code */
enum rtw_ieee80211_ht_actioncode {
	RTW_WLAN_ACTION_NOTIFY_CH_WIDTH = 0,
	RTW_WLAN_ACTION_SM_PS = 1,
	RTW_WLAN_ACTION_PSPM = 2,
	RTW_WLAN_ACTION_PCO_PHASE = 3,
	RTW_WLAN_ACTION_MIMO_CSI_MX = 4,
	RTW_WLAN_ACTION_MIMO_NONCP_BF = 5,
	RTW_WLAN_ACTION_MIMP_CP_BF = 6,
	RTW_WLAN_ACTION_ASEL_INDICATES_FB = 7,
	RTW_WLAN_ACTION_HI_INFO_EXCHG = 8,
};

/* VHT features action code */
enum rtw_ieee80211_vht_actioncode {
	RTW_WLAN_ACTION_VHT_COMPRESSED_BEAMFORMING = 0,
	RTW_WLAN_ACTION_VHT_GROUPID_MANAGEMENT = 1,
	RTW_WLAN_ACTION_VHT_OPMODE_NOTIFICATION = 2,
};

/* BACK (block-ack) parties */
enum rtw_ieee80211_back_parties {
	RTW_WLAN_BACK_RECIPIENT = 0,
	RTW_WLAN_BACK_INITIATOR = 1,
	RTW_WLAN_BACK_TIMER = 2,
};

enum secondary_ch_offset {
	RTW_WLAN_SCN = 0, /* no secondary channel */
	RTW_WLAN_SCA = 1, /* secondary channel above */
	RTW_WLAN_SCB = 3,  /* secondary channel below */
};

/* Self Protected Action codes */
enum rtw_ieee80211_self_protected_actioncode {
	RTW_WLAN_SP_RESERVED = 0,
	RTW_WLAN_SP_MESH_PEERING_OPEN = 1,
	RTW_WLAN_SP_MESH_PEERING_CONFIRM = 2,
	RTW_WLAN_SP_MESH_PEERING_CLOSE = 3,
	RTW_WLAN_SP_MGK_INFORM = 4,
	RTW_WLAN_SP_MGK_ACK = 5,
};

/* Mesh action codes */
enum rtw_ieee80211_mesh_actioncode {
	RTW_WLAN_MESH_ACTION_LINK_METRIC_REPORT,
	RTW_WLAN_MESH_ACTION_HWMP_PATH_SELECTION,
	RTW_WLAN_MESH_ACTION_GATE_ANNOUNCEMENT,
	RTW_WLAN_MESH_ACTION_CONGESTION_CONTROL_NOTIFICATION,
	RTW_WLAN_MESH_ACTION_MCCA_SETUP_REQUEST,
	RTW_WLAN_MESH_ACTION_MCCA_SETUP_REPLY,
	RTW_WLAN_MESH_ACTION_MCCA_ADVERTISEMENT_REQUEST,
	RTW_WLAN_MESH_ACTION_MCCA_ADVERTISEMENT,
	RTW_WLAN_MESH_ACTION_MCCA_TEARDOWN,
	RTW_WLAN_MESH_ACTION_TBTT_ADJUSTMENT_REQUEST,
	RTW_WLAN_MESH_ACTION_TBTT_ADJUSTMENT_RESPONSE,
};

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

#define OUI_MICROSOFT 0x0050f2 /* Microsoft (also used in Wi-Fi specs)
				* 00:50:F2 */
#define WME_OUI_TYPE 2
#define WME_OUI_SUBTYPE_INFORMATION_ELEMENT 0
#define WME_OUI_SUBTYPE_PARAMETER_ELEMENT 1
#define WME_OUI_SUBTYPE_TSPEC_ELEMENT 2
#define WME_VERSION 1

#define WME_ACTION_CODE_SETUP_REQUEST 0
#define WME_ACTION_CODE_SETUP_RESPONSE 1
#define WME_ACTION_CODE_TEARDOWN 2

#define WME_SETUP_RESPONSE_STATUS_ADMISSION_ACCEPTED 0
#define WME_SETUP_RESPONSE_STATUS_INVALID_PARAMETERS 1
#define WME_SETUP_RESPONSE_STATUS_REFUSED 3

#define WME_TSPEC_DIRECTION_UPLINK 0
#define WME_TSPEC_DIRECTION_DOWNLINK 1
#define WME_TSPEC_DIRECTION_BI_DIRECTIONAL 3


#define OUI_BROADCOM 0x00904c /* Broadcom (Epigram) */

#define VENDOR_HT_CAPAB_OUI_TYPE 0x33 /* 00-90-4c:0x33 */

u8 *wifi_rom_get_ie_ex(u8 *in_ie, uint in_len, u8 eid, u8 *oui, u8 oui_len, u8 *ie, uint *ielen);
int rtw_ies_remove_ie(u8 *ies, uint *ies_len, uint offset, u8 eid, u8 *oui, u8 oui_len);

int rtw_get_wapi_ie(u8 *in_ie, uint in_len, u8 *wapi_ie, u16 *wapi_len);

u8 rtw_is_wps_ie(u8 *ie_ptr, uint *wps_ielen);

void dump_ies(u8 *buf, u32 buf_len);
void dump_wps_ie(u8 *ie, u32 ie_len);

int wifi_rom_parse_sec_ie_akm_mgc(u8 *ie, int ie_len, int *auth_key_mgmt, int *mgmt_group_cipher);
u16 rtw_mcs_rate(u8 rf_type, u8 bw_40MHz, u8 short_GI_20, u8 short_GI_40, unsigned char *MCS_rate);


#endif /* __IEEE80211_H */

