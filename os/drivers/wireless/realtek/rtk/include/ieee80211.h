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
 ******************************************************************************/
#ifndef __IEEE80211_H
#define __IEEE80211_H

#ifndef CONFIG_RTL8711FW

#if defined PLATFORM_OS_XP
#include <ntstrsafe.h>
#endif
#if defined PLATFORM_LINUX
#include <linux/wireless.h>
#endif
#else

#include <list.h>

#endif

#define MGMT_QUEUE_NUM 5

#define ETH_ALEN 6
#define ETH_TYPE_LEN 2
#define PAYLOAD_TYPE_LEN 1

#ifdef CONFIG_AP_MODE

#define RTL_IOCTL_HOSTAPD (SIOCIWFIRSTPRIV + 28)

/* RTL871X_IOCTL_HOSTAPD ioctl() cmd: */
enum {
	RTL871X_HOSTAPD_FLUSH = 1,
	RTL871X_HOSTAPD_ADD_STA = 2,
	RTL871X_HOSTAPD_REMOVE_STA = 3,
	RTL871X_HOSTAPD_GET_INFO_STA = 4,
	/* REMOVED: PRISM2_HOSTAPD_RESET_TXEXC_STA = 5, */
	RTL871X_HOSTAPD_GET_WPAIE_STA = 5,
	RTL871X_SET_ENCRYPTION = 6,
	RTL871X_GET_ENCRYPTION = 7,
	RTL871X_HOSTAPD_SET_FLAGS_STA = 8,
	RTL871X_HOSTAPD_GET_RID = 9,
	RTL871X_HOSTAPD_SET_RID = 10,
	RTL871X_HOSTAPD_SET_ASSOC_AP_ADDR = 11,
	RTL871X_HOSTAPD_SET_GENERIC_ELEMENT = 12,
	RTL871X_HOSTAPD_MLME = 13,
	RTL871X_HOSTAPD_SCAN_REQ = 14,
	RTL871X_HOSTAPD_STA_CLEAR_STATS = 15,
	RTL871X_HOSTAPD_SET_BEACON = 16,
	RTL871X_HOSTAPD_SET_WPS_BEACON = 17,
	RTL871X_HOSTAPD_SET_WPS_PROBE_RESP = 18,
	RTL871X_HOSTAPD_SET_WPS_ASSOC_RESP = 19,
	RTL871X_HOSTAPD_SET_HIDDEN_SSID = 20,
	RTL871X_HOSTAPD_SET_MACADDR_ACL = 21,
	RTL871X_HOSTAPD_ACL_ADD_STA = 22,
	RTL871X_HOSTAPD_ACL_REMOVE_STA = 23,
};

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

#endif

#define IEEE_CMD_SET_WPA_PARAM 1
#define IEEE_CMD_SET_WPA_IE 2
#define IEEE_CMD_SET_ENCRYPTION 3
#define IEEE_CMD_MLME 4

#define IEEE_PARAM_WPA_ENABLED 1
#define IEEE_PARAM_TKIP_COUNTERMEASURES 2
#define IEEE_PARAM_DROP_UNENCRYPTED 3
#define IEEE_PARAM_PRIVACY_INVOKED 4
#define IEEE_PARAM_AUTH_ALGS 5
#define IEEE_PARAM_IEEE_802_1X 6
#define IEEE_PARAM_WPAX_SELECT 7

#define AUTH_ALG_OPEN_SYSTEM 0x1
#define AUTH_ALG_SHARED_KEY 0x2
#define AUTH_ALG_LEAP 0x00000004

#define IEEE_MLME_STA_DEAUTH 1
#define IEEE_MLME_STA_DISASSOC 2

#define IEEE_CRYPT_ERR_UNKNOWN_ALG 2
#define IEEE_CRYPT_ERR_UNKNOWN_ADDR 3
#define IEEE_CRYPT_ERR_CRYPT_INIT_FAILED 4
#define IEEE_CRYPT_ERR_KEY_SET_FAILED 5
#define IEEE_CRYPT_ERR_TX_KEY_SET_FAILED 6
#define IEEE_CRYPT_ERR_CARD_CONF_FAILED 7

#define IEEE_CRYPT_ALG_NAME_LEN 16

#define WPA_CIPHER_NONE BIT(0)
#define WPA_CIPHER_WEP40 BIT(1)
#define WPA_CIPHER_WEP104 BIT(2)
#define WPA_CIPHER_TKIP BIT(3)
#define WPA_CIPHER_CCMP BIT(4)

#define WPA_SELECTOR_LEN 4

#define RSN_HEADER_LEN 4
#define RSN_SELECTOR_LEN 4

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
} RATEID_IDX,
	*PRATEID_IDX;

enum NETWORK_TYPE {
	WIRELESS_INVALID = 0,
	//Sub-Element
	WIRELESS_11B = BIT(0),	// tx: cck only , rx: cck only, hw: cck
	WIRELESS_11G = BIT(1),	// tx: ofdm only, rx: ofdm & cck, hw: cck & ofdm
	WIRELESS_11A = BIT(2),	// tx: ofdm only, rx: ofdm only, hw: ofdm only
	WIRELESS_11_24N = BIT(3), // tx: MCS only, rx: MCS & cck, hw: MCS & cck
	WIRELESS_11_5N = BIT(4),  // tx: MCS only, rx: MCS & ofdm, hw: ofdm only
	WIRELESS_AUTO = BIT(5),
	WIRELESS_11AC = BIT(6),

	//Combination
	WIRELESS_11BG = (WIRELESS_11B | WIRELESS_11G),						 // tx: cck & ofdm, rx: cck & ofdm & MCS, hw: cck & ofdm
	WIRELESS_11G_24N = (WIRELESS_11G | WIRELESS_11_24N),				 // tx: ofdm & MCS, rx: ofdm & cck & MCS, hw: cck & ofdm
	WIRELESS_11A_5N = (WIRELESS_11A | WIRELESS_11_5N),					 // tx: ofdm & MCS, rx: ofdm & MCS, hw: ofdm only
	WIRELESS_11B_24N = (WIRELESS_11B | WIRELESS_11_24N),				 /* tx: ofdm & cck & MCS, rx: ofdm & cck & MCS, hw: ofdm & cck */
	WIRELESS_11BG_24N = (WIRELESS_11B | WIRELESS_11G | WIRELESS_11_24N), // tx: ofdm & cck & MCS, rx: ofdm & cck & MCS, hw: ofdm & cck
	WIRELESS_11_24AC = (WIRELESS_11G | WIRELESS_11AC),
	WIRELESS_11_5AC = (WIRELESS_11A | WIRELESS_11AC),

	/* Type for registry default wireless mode */
	WIRELESS_11AGN = (WIRELESS_11A | WIRELESS_11G | WIRELESS_11_24N | WIRELESS_11_5N), // tx: ofdm & MCS, rx: ofdm & MCS, hw: ofdm only
	WIRELESS_11ABGN = (WIRELESS_11A | WIRELESS_11B | WIRELESS_11G | WIRELESS_11_24N | WIRELESS_11_5N),
	WIRELESS_MODE_24G = (WIRELESS_11B | WIRELESS_11G | WIRELESS_11_24N),
	WIRELESS_MODE_5G = (WIRELESS_11A | WIRELESS_11_5N | WIRELESS_11AC),
	WIRELESS_MODE_MAX = (WIRELESS_11A | WIRELESS_11B | WIRELESS_11G | WIRELESS_11_24N | WIRELESS_11_5N | WIRELESS_11AC),
};

#define SUPPORTED_24G_NETTYPE_MSK (WIRELESS_11B | WIRELESS_11G | WIRELESS_11_24N)
#define SUPPORTED_5G_NETTYPE_MSK (WIRELESS_11A | WIRELESS_11_5N)

#define IsLegacyOnly(NetType) ((NetType) == ((NetType) & (WIRELESS_11BG | WIRELESS_11A)))

#define IsSupported24G(NetType) ((NetType)&SUPPORTED_24G_NETTYPE_MSK ? _TRUE : _FALSE)
#define IsSupported5G(NetType) ((NetType)&SUPPORTED_5G_NETTYPE_MSK ? _TRUE : _FALSE)

#define IsEnableHWCCK(NetType) IsSupported24G(NetType)
#define IsEnableHWOFDM(NetType) ((NetType) & (WIRELESS_11G | WIRELESS_11_24N | SUPPORTED_5G_NETTYPE_MSK) ? _TRUE : _FALSE)

#define IsSupportedRxCCK(NetType) IsEnableHWCCK(NetType)
#define IsSupportedRxOFDM(NetType) IsEnableHWOFDM(NetType)
#define IsSupportedRxHT(NetType) IsEnableHWOFDM(NetType)

#define IsSupportedTxCCK(NetType) ((NetType) & (WIRELESS_11B) ? _TRUE : _FALSE)
#define IsSupportedTxOFDM(NetType) ((NetType) & (WIRELESS_11G | WIRELESS_11A) ? _TRUE : _FALSE)

#define is_supported_ht(NetType) ((NetType) & (WIRELESS_11_24N | WIRELESS_11_5N) ? _TRUE : _FALSE)
#define is_supported_vht(NetType) ((NetType) & (WIRELESS_11AC) ? _TRUE : _FALSE)

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
#ifdef CONFIG_AP_MODE
		struct {
			u16 aid;
			u16 capability;
			int flags;
			u8 tx_supp_rates[16];
			struct rtw_ieee80211_ht_cap ht_cap;
		} add_sta;
		struct {
			u8 reserved[2]; //for set max_num_sta
#ifdef __CC_ARM
			u8 buf[1];
#else
			u8 buf[0];
#endif
		} bcn_ie;
#endif

	} u;
} ieee_param;

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#include "pack_begin.h"
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
#include "pack_end.h"
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#include "pack_begin.h"
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
#include "pack_end.h"
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#include "pack_begin.h"
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
	u16 qc;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#include "pack_end.h"
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
struct rtw_ieee80211_hdr_3addr_qos {
	u16 frame_ctl;
	u16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	u16 seq_ctl;
	u16 qc;
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#include "pack_end.h"
#endif

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#include "pack_begin.h"
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
#include "pack_end.h"
#endif

#define IEEE80211_3ADDR_LEN 24
#define IEEE80211_4ADDR_LEN 30
#define IEEE80211_FCS_LEN 4

#define MIN_FRAG_THRESHOLD 256U
#define MAX_FRAG_THRESHOLD 2346U

/* Frame control field constants */
#define RTW_IEEE80211_FCTL_VERS 0x0002
#define RTW_IEEE80211_FCTL_FTYPE 0x000c
#define RTW_IEEE80211_FCTL_STYPE 0x00f0
#define RTW_IEEE80211_FCTL_TODS 0x0100
#define RTW_IEEE80211_FCTL_FROMDS 0x0200
#define RTW_IEEE80211_FCTL_MOREFRAGS 0x0400
#define RTW_IEEE80211_FCTL_RETRY 0x0800
#define RTW_IEEE80211_FCTL_PM 0x1000
#define RTW_IEEE80211_FCTL_MOREDATA 0x2000
#define RTW_IEEE80211_FCTL_WEP 0x4000
#define RTW_IEEE80211_FCTL_ORDER 0x8000

#define RTW_IEEE80211_FTYPE_MGMT 0x0000
#define RTW_IEEE80211_FTYPE_CTL 0x0004
#define RTW_IEEE80211_FTYPE_DATA 0x0008

/* management */
#define RTW_IEEE80211_STYPE_ASSOC_REQ 0x0000
#define RTW_IEEE80211_STYPE_ASSOC_RESP 0x0010
#define RTW_IEEE80211_STYPE_REASSOC_REQ 0x0020
#define RTW_IEEE80211_STYPE_REASSOC_RESP 0x0030
#define RTW_IEEE80211_STYPE_PROBE_REQ 0x0040
#define RTW_IEEE80211_STYPE_PROBE_RESP 0x0050
#define RTW_IEEE80211_STYPE_BEACON 0x0080
#define RTW_IEEE80211_STYPE_ATIM 0x0090
#define RTW_IEEE80211_STYPE_DISASSOC 0x00A0
#define RTW_IEEE80211_STYPE_AUTH 0x00B0
#define RTW_IEEE80211_STYPE_DEAUTH 0x00C0

/* control */
#define RTW_IEEE80211_STYPE_PSPOLL 0x00A0
#define RTW_IEEE80211_STYPE_RTS 0x00B0
#define RTW_IEEE80211_STYPE_CTS 0x00C0
#define RTW_IEEE80211_STYPE_ACK 0x00D0
#define RTW_IEEE80211_STYPE_CFEND 0x00E0
#define RTW_IEEE80211_STYPE_CFENDACK 0x00F0

/* data */
#define RTW_IEEE80211_STYPE_DATA 0x0000
#define RTW_IEEE80211_STYPE_DATA_CFACK 0x0010
#define RTW_IEEE80211_STYPE_DATA_CFPOLL 0x0020
#define RTW_IEEE80211_STYPE_DATA_CFACKPOLL 0x0030
#define RTW_IEEE80211_STYPE_NULLFUNC 0x0040
#define RTW_IEEE80211_STYPE_CFACK 0x0050
#define RTW_IEEE80211_STYPE_CFPOLL 0x0060
#define RTW_IEEE80211_STYPE_CFACKPOLL 0x0070
#define RTW_IEEE80211_QOS_DATAGRP 0x0080
#define RTW_IEEE80211_QoS_DATAGRP RTW_IEEE80211_QOS_DATAGRP

#define RTW_IEEE80211_SCTL_FRAG 0x000F
#define RTW_IEEE80211_SCTL_SEQ 0xFFF0

#define RTW_ERP_INFO_NON_ERP_PRESENT BIT(0)
#define RTW_ERP_INFO_USE_PROTECTION BIT(1)
#define RTW_ERP_INFO_BARKER_PREAMBLE_MODE BIT(2)

/* QoS,QOS */
#define NORMAL_ACK 0
#define NO_ACK 1
#define NON_EXPLICIT_ACK 2
#define BLOCK_ACK 3

#ifndef ETH_P_PAE
#define ETH_P_PAE 0x888E /* Port Access Entity (IEEE 802.1X) */
#endif					 /* ETH_P_PAE */

#define ETH_P_PREAUTH 0x88C7 /* IEEE 802.11i pre-authentication */

#define ETH_P_ECONET 0x0018

#ifndef ETH_P_80211_RAW
#define ETH_P_80211_RAW (ETH_P_ECONET + 1)
#endif

/* IEEE 802.11 defines */

#define P80211_OUI_LEN 3

RTW_PACK_STRUCT_BEGIN
struct ieee80211_snap_hdr {

	u8 dsap;				/* always 0xAA */
	u8 ssap;				/* always 0xAA */
	u8 ctrl;				/* always 0x03 */
	u8 oui[P80211_OUI_LEN]; /* organizational universal id */

} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

#ifdef PLATFORM_WINDOWS

#pragma pack(1)
struct ieee80211_snap_hdr {

	u8 dsap;				/* always 0xAA */
	u8 ssap;				/* always 0xAA */
	u8 ctrl;				/* always 0x03 */
	u8 oui[P80211_OUI_LEN]; /* organizational universal id */
};
#pragma pack()

#endif

#define SNAP_SIZE sizeof(struct ieee80211_snap_hdr)

#define WLAN_FC_GET_TYPE(fc) ((fc)&RTW_IEEE80211_FCTL_FTYPE)
#define WLAN_FC_GET_STYPE(fc) ((fc)&RTW_IEEE80211_FCTL_STYPE)

#define WLAN_QC_GET_TID(qc) ((qc)&0x0f)

#define WLAN_GET_SEQ_FRAG(seq) ((seq)&RTW_IEEE80211_SCTL_FRAG)
#define WLAN_GET_SEQ_SEQ(seq) ((seq)&RTW_IEEE80211_SCTL_SEQ)

/* Authentication algorithms */
#define WLAN_AUTH_OPEN 0
#define WLAN_AUTH_SHARED_KEY 1

#define WLAN_AUTH_CHALLENGE_LEN 128

#define WLAN_CAPABILITY_BSS (1 << 0)
#define WLAN_CAPABILITY_IBSS (1 << 1)
#define WLAN_CAPABILITY_CF_POLLABLE (1 << 2)
#define WLAN_CAPABILITY_CF_POLL_REQUEST (1 << 3)
#define WLAN_CAPABILITY_PRIVACY (1 << 4)
#define WLAN_CAPABILITY_SHORT_PREAMBLE (1 << 5)
#define WLAN_CAPABILITY_PBCC (1 << 6)
#define WLAN_CAPABILITY_CHANNEL_AGILITY (1 << 7)
#define WLAN_CAPABILITY_SHORT_SLOT (1 << 10)

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
#define WLAN_REASON_JOIN_WRONG_CHANNEL 65534

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
#define WLAN_EID_20_40_BSS_COEXISTENCE 72
#define WLAN_EID_20_40_BSS_INTOLERANT 73
#define WLAN_EID_OVERLAPPING_BSS_SCAN_PARAMS 74
#define WLAN_EID_MMIE 76
#define WLAN_EID_VHT_CAPABILITY 191
#define WLAN_EID_VHT_OPERATION 192
#define WLAN_EID_VHT_OPERATING_MODE_NOTIFICATION 199
#define WLAN_EID_VENDOR_SPECIFIC 221
#define WLAN_EID_GENERIC (WLAN_EID_VENDOR_SPECIFIC)

#define IEEE80211_MGMT_HDR_LEN 24
#define IEEE80211_DATA_HDR3_LEN 24
#define IEEE80211_DATA_HDR4_LEN 30

#define IEEE80211_STATMASK_SIGNAL (1 << 0)
#define IEEE80211_STATMASK_RSSI (1 << 1)
#define IEEE80211_STATMASK_NOISE (1 << 2)
#define IEEE80211_STATMASK_RATE (1 << 3)
#define IEEE80211_STATMASK_WEMASK 0x7

#define IEEE80211_CCK_MODULATION (1 << 0)
#define IEEE80211_OFDM_MODULATION (1 << 1)

#define IEEE80211_24GHZ_BAND (1 << 0)
#define IEEE80211_52GHZ_BAND (1 << 1)

#define IEEE80211_CCK_RATE_LEN 4
#define IEEE80211_NUM_OFDM_RATESLEN 8

#define IEEE80211_CCK_RATE_1MB 0x02
#define IEEE80211_CCK_RATE_2MB 0x04
#define IEEE80211_CCK_RATE_5MB 0x0B
#define IEEE80211_CCK_RATE_11MB 0x16
#define IEEE80211_OFDM_RATE_LEN 8
#define IEEE80211_OFDM_RATE_6MB 0x0C
#define IEEE80211_OFDM_RATE_9MB 0x12
#define IEEE80211_OFDM_RATE_12MB 0x18
#define IEEE80211_OFDM_RATE_18MB 0x24
#define IEEE80211_OFDM_RATE_24MB 0x30
#define IEEE80211_OFDM_RATE_36MB 0x48
#define IEEE80211_OFDM_RATE_48MB 0x60
#define IEEE80211_OFDM_RATE_54MB 0x6C
#define IEEE80211_BASIC_RATE_MASK 0x80

#define IEEE80211_CCK_RATE_1MB_MASK (1 << 0)
#define IEEE80211_CCK_RATE_2MB_MASK (1 << 1)
#define IEEE80211_CCK_RATE_5MB_MASK (1 << 2)
#define IEEE80211_CCK_RATE_11MB_MASK (1 << 3)
#define IEEE80211_OFDM_RATE_6MB_MASK (1 << 4)
#define IEEE80211_OFDM_RATE_9MB_MASK (1 << 5)
#define IEEE80211_OFDM_RATE_12MB_MASK (1 << 6)
#define IEEE80211_OFDM_RATE_18MB_MASK (1 << 7)
#define IEEE80211_OFDM_RATE_24MB_MASK (1 << 8)
#define IEEE80211_OFDM_RATE_36MB_MASK (1 << 9)
#define IEEE80211_OFDM_RATE_48MB_MASK (1 << 10)
#define IEEE80211_OFDM_RATE_54MB_MASK (1 << 11)

#define IEEE80211_CCK_RATES_MASK 0x0000000F
#define IEEE80211_CCK_BASIC_RATES_MASK (IEEE80211_CCK_RATE_1MB_MASK | \
										IEEE80211_CCK_RATE_2MB_MASK)
#define IEEE80211_CCK_DEFAULT_RATES_MASK (IEEE80211_CCK_BASIC_RATES_MASK | \
										  IEEE80211_CCK_RATE_5MB_MASK |    \
										  IEEE80211_CCK_RATE_11MB_MASK)

#define IEEE80211_OFDM_RATES_MASK 0x00000FF0
#define IEEE80211_OFDM_BASIC_RATES_MASK (IEEE80211_OFDM_RATE_6MB_MASK |  \
										 IEEE80211_OFDM_RATE_12MB_MASK | \
										 IEEE80211_OFDM_RATE_24MB_MASK)
#define IEEE80211_OFDM_DEFAULT_RATES_MASK (IEEE80211_OFDM_BASIC_RATES_MASK | \
										   IEEE80211_OFDM_RATE_9MB_MASK |    \
										   IEEE80211_OFDM_RATE_18MB_MASK |   \
										   IEEE80211_OFDM_RATE_36MB_MASK |   \
										   IEEE80211_OFDM_RATE_48MB_MASK |   \
										   IEEE80211_OFDM_RATE_54MB_MASK)
#define IEEE80211_DEFAULT_RATES_MASK (IEEE80211_OFDM_DEFAULT_RATES_MASK | \
									  IEEE80211_CCK_DEFAULT_RATES_MASK)

#define IEEE80211_NUM_OFDM_RATES 8
#define IEEE80211_NUM_CCK_RATES 4
#define IEEE80211_OFDM_SHIFT_MASK_A 4

/* BIT 7 HT Rate*/
enum MGN_RATE {
	MGN_1M = 0x02,
	MGN_2M = 0x04,
	MGN_5_5M = 0x0B,
	MGN_6M = 0x0C,
	MGN_9M = 0x12,
	MGN_11M = 0x16,
	MGN_12M = 0x18,
	MGN_18M = 0x24,
	MGN_24M = 0x30,
	MGN_36M = 0x48,
	MGN_48M = 0x60,
	MGN_54M = 0x6C,
	MGN_MCS32 = 0x7F,
	MGN_MCS0,
	MGN_MCS1,
	MGN_MCS2,
	MGN_MCS3,
	MGN_MCS4,
	MGN_MCS5,
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
	MGN_UNKNOWN
};

typedef enum _RATR_TABLE_MODE {
	RATR_INX_WIRELESS_NGB = 0, /* BGN 40 Mhz 2SS 1SS */
	RATR_INX_WIRELESS_NG = 1,  /* GN or N */
	RATR_INX_WIRELESS_NB = 2,  /* BGN 20 Mhz 2SS 1SS  or BN */
	RATR_INX_WIRELESS_N = 3,
	RATR_INX_WIRELESS_GB = 4,
	RATR_INX_WIRELESS_G = 5,
	RATR_INX_WIRELESS_B = 6,
	RATR_INX_WIRELESS_MC = 7,
	RATR_INX_WIRELESS_AC_N = 8,
} RATR_TABLE_MODE,
	*PRATR_TABLE_MODE;

#define MGN_MCS0_SG 0xc0
#define MGN_MCS1_SG 0xc1
#define MGN_MCS2_SG 0xc2
#define MGN_MCS3_SG 0xc3
#define MGN_MCS4_SG 0xc4
#define MGN_MCS5_SG 0xc5
#define MGN_MCS6_SG 0xc6
#define MGN_MCS7_SG 0xc7
#define MGN_MCS8_SG 0xc8
#define MGN_MCS9_SG 0xc9
#define MGN_MCS10_SG 0xca
#define MGN_MCS11_SG 0xcb
#define MGN_MCS12_SG 0xcc
#define MGN_MCS13_SG 0xcd
#define MGN_MCS14_SG 0xce
#define MGN_MCS15_SG 0xcf

#define IS_HT_RATE(_rate) (((_rate)&0x80) ? _TRUE : _FALSE)
#define IS_CCK_RATE(_rate) (MGN_1M == _rate || _rate == MGN_2M || _rate == MGN_5_5M || _rate == MGN_11M)
#define IS_OFDM_RATE(_rate) (MGN_6M <= _rate && _rate <= MGN_54M)

/* NOTE : This data is for statistical purposes; not all hardware provides this
 *       information for frames received.  Not setting these will not cause
 *       any adverse affects. */
struct ieee80211_rx_stats {
	s8 rssi;
	u8 signal;
	u8 noise;
	u8 received_channel;
	u16 rate; /* in 100 kbps */
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
	uint qos; //jackson
	uint tid; //jackson
	struct sk_buff *skb;
	u8 src_addr[ETH_ALEN];
	u8 dst_addr[ETH_ALEN];
};

#ifndef PLATFORM_FREEBSD //Baron BSD has already defined
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
#endif //PLATFORM_FREEBSD
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

#define SEC_KEY_1 (1 << 0)
#define SEC_KEY_2 (1 << 1)
#define SEC_KEY_3 (1 << 2)
#define SEC_KEY_4 (1 << 3)
#define SEC_ACTIVE_KEY (1 << 4)
#define SEC_AUTH_MODE (1 << 5)
#define SEC_UNICAST_GROUP (1 << 6)
#define SEC_LEVEL (1 << 7)
#define SEC_ENABLED (1 << 8)

#define SEC_LEVEL_0 0	  /* None */
#define SEC_LEVEL_1 1	  /* WEP 40 and 104 bit */
#define SEC_LEVEL_2 2	  /* Level 1 + TKIP */
#define SEC_LEVEL_2_CKIP 3 /* Level 1 + CKIP */
#define SEC_LEVEL_3 4	  /* Level 2 + CCMP */

#define WEP_KEYS 4
#define WEP_KEY_LEN 13

#ifdef CONFIG_IEEE80211W
#define BIP_MAX_KEYID 5
#define BIP_AAD_SIZE 20
#endif /* CONFIG_IEEE80211W */

#if defined(PLATFORM_LINUX) || defined(CONFIG_RTL8711FW)

struct ieee80211_security {
	u16 active_key:2,
		enabled:1,
		auth_mode:2,
		auth_algo:4,
		unicast_uses_group:1;
	u8 key_sizes[WEP_KEYS];
	u8 keys[WEP_KEYS][WEP_KEY_LEN];
	u8 level;
	u16 flags;
} __attribute__((packed));

#endif

#ifdef PLATFORM_WINDOWS

#pragma pack(1)
struct ieee80211_security {
	u16 active_key:2,
		enabled:1,
		auth_mode:2,
		auth_algo:4,
		unicast_uses_group:1;
	u8 key_sizes[WEP_KEYS];
	u8 keys[WEP_KEYS][WEP_KEY_LEN];
	u8 level;
	u16 flags;
};
#pragma pack()

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
#define MFIE_TYPE_SSID 0
#define MFIE_TYPE_RATES 1
#define MFIE_TYPE_FH_SET 2
#define MFIE_TYPE_DS_SET 3
#define MFIE_TYPE_CF_SET 4
#define MFIE_TYPE_TIM 5
#define MFIE_TYPE_IBSS_SET 6
#define MFIE_TYPE_CHALLENGE 16
#define MFIE_TYPE_ERP 42
#define MFIE_TYPE_RSN 48
#define MFIE_TYPE_RATES_EX 50
#define MFIE_TYPE_GENERIC 221

#if defined(PLATFORM_LINUX) || defined(CONFIG_RTL8711FW)

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

#ifdef PLATFORM_WINDOWS

#pragma pack(1)
struct ieee80211_info_element_hdr {
	u8 id;
	u8 len;
};

struct ieee80211_info_element {
	u8 id;
	u8 len;
	u8 data[0];
};
#pragma pack()

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

#if defined(PLATFORM_LINUX) || defined(CONFIG_RTL8711FW)

struct ieee80211_authentication {
	struct ieee80211_header_data header;
	u16 algorithm;
	u16 transaction;
	u16 status;
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
} __attribute__((packed));

struct ieee80211_assoc_request_frame {
	struct rtw_ieee80211_hdr_3addr header;
	u16 capability;
	u16 listen_interval;
	struct ieee80211_info_element_hdr info_element;
} __attribute__((packed));

struct ieee80211_assoc_response_frame {
	struct rtw_ieee80211_hdr_3addr header;
	u16 capability;
	u16 status;
	u16 aid;
} __attribute__((packed));
#endif

#ifdef PLATFORM_WINDOWS

#pragma pack(1)

struct ieee80211_authentication {
	struct ieee80211_header_data header;
	u16 algorithm;
	u16 transaction;
	u16 status;
};

struct ieee80211_probe_response {
	struct ieee80211_header_data header;
	u32 time_stamp[2];
	u16 beacon_interval;
	u16 capability;
	struct ieee80211_info_element info_element;
};

struct ieee80211_probe_request {
	struct ieee80211_header_data header;
};

struct ieee80211_assoc_request_frame {
	struct rtw_ieee80211_hdr_3addr header;
	u16 capability;
	u16 listen_interval;
	struct ieee80211_info_element_hdr info_element;
};

struct ieee80211_assoc_response_frame {
	struct rtw_ieee80211_hdr_3addr header;
	u16 capability;
	u16 status;
	u16 aid;
};

#pragma pack()

#endif

/* SWEEP TABLE ENTRIES NUMBER*/
#define MAX_SWEEP_TAB_ENTRIES 42
#define MAX_SWEEP_TAB_ENTRIES_PER_PACKET 7
/* MAX_RATES_LENGTH needs to be 12.  The spec says 8, and many APs
 * only use 8, and then use extended rates for the remaining supported
 * rates.  Other APs, however, stick all of their supported rates on the
 * main rates information element... */
#define MAX_RATES_LENGTH ((u8)12)
#define MAX_RATES_EX_LENGTH ((u8)16)
#define MAX_NETWORK_COUNT 128
#define MAX_CHANNEL_NUMBER 161
#define IEEE80211_SOFTMAC_SCAN_TIME 400
#define IEEE80211_SOFTMAC_ASSOC_RETRY_TIME (HZ * 2)

#define CRC_LENGTH 4U

#define MAX_WPA_IE_LEN (256)
#define MAX_WPS_IE_LEN (512)
#define MAX_P2P_IE_LEN (256)
#define MAX_WFD_IE_LEN (128)

#define NETWORK_EMPTY_ESSID (1 << 0)
#define NETWORK_HAS_OFDM (1 << 1)
#define NETWORK_HAS_CCK (1 << 2)

#define IEEE80211_DTIM_MBCAST 4
#define IEEE80211_DTIM_UCAST 2
#define IEEE80211_DTIM_VALID 1
#define IEEE80211_DTIM_INVALID 0

#define IEEE80211_PS_DISABLED 0
#define IEEE80211_PS_UNICAST IEEE80211_DTIM_UCAST
#define IEEE80211_PS_MBCAST IEEE80211_DTIM_MBCAST
#define IW_ESSID_MAX_SIZE 32
#define IW_WPA2_PASSPHRASE_MAX_SIZE 64
#define IW_WPA3_PASSPHRASE_MAX_SIZE 128
#define IW_PASSPHRASE_MAX_SIZE IW_WPA3_PASSPHRASE_MAX_SIZE

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
#define MAC_ARG(x) ((u8 *)(x))[0], ((u8 *)(x))[1], ((u8 *)(x))[2], ((u8 *)(x))[3], ((u8 *)(x))[4], ((u8 *)(x))[5]

#define is_multicast_mac_addr(Addr) ((((Addr[0]) & 0x01) == 0x01) && ((Addr[0]) != 0xff))
#define is_broadcast_mac_addr(Addr) ((((Addr[0]) & 0xff) == 0xff) && (((Addr[1]) & 0xff) == 0xff) && \
(((Addr[2]) & 0xff) == 0xff) && (((Addr[3]) & 0xff) == 0xff) && \
									 (((Addr[4]) & 0xff) == 0xff) && \
(((Addr[5]) & 0xff) == 0xff))

#define CFG_IEEE80211_RESERVE_FCS (1 << 0)
#define CFG_IEEE80211_COMPUTE_FCS (1 << 1)

#define MAXTID 16

#define IEEE_A (1 << 0)
#define IEEE_B (1 << 1)
#define IEEE_G (1 << 2)
#define IEEE_MODE_MASK (IEEE_A | IEEE_B | IEEE_G)

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
	RTW_WLAN_CATEGORY_RADIO_MEASUREMENT = 5,
	RTW_WLAN_CATEGORY_FT = 6,
	RTW_WLAN_CATEGORY_HT = 7,
	RTW_WLAN_CATEGORY_SA_QUERY = 8,
	RTW_WLAN_CATEGORY_UNPROTECTED_WNM = 11, /* add for CONFIG_IEEE80211W, none 11w also can use */
	RTW_WLAN_CATEGORY_TDLS = 12,
	RTW_WLAN_CATEGORY_SELF_PROTECTED = 15, /* add for CONFIG_IEEE80211W, none 11w also can use */
	RTW_WLAN_CATEGORY_WMM = 17,
	RTW_WLAN_CATEGORY_P2P = 0x7f, //P2P action frames
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

#ifdef CONFIG_TDLS
enum TDLS_ACTION_FIELD {
	TDLS_SETUP_REQUEST = 0,
	TDLS_SETUP_RESPONSE = 1,
	TDLS_SETUP_CONFIRM = 2,
	TDLS_TEARDOWN = 3,
	TDLS_PEER_TRAFFIC_INDICATION = 4,
	TDLS_CHANNEL_SWITCH_REQUEST = 5,
	TDLS_CHANNEL_SWITCH_RESPONSE = 6,
	TDLS_PEER_PSM_REQUEST = 7,
	TDLS_PEER_PSM_RESPONSE = 8,
	TDLS_PEER_TRAFFIC_RESPONSE = 9,
	TDLS_DISCOVERY_REQUEST = 10,
	TDLS_DISCOVERY_RESPONSE = 14, //it's used in public action frame
};

#define TUNNELED_PROBE_REQ 15
#define TUNNELED_PROBE_RSP 16
#endif //CONFIG_TDLS

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

/* BACK (block-ack) parties */
enum rtw_ieee80211_back_parties {
	RTW_WLAN_BACK_RECIPIENT = 0,
	RTW_WLAN_BACK_INITIATOR = 1,
	RTW_WLAN_BACK_TIMER = 2,
};

enum secondary_ch_offset {
	RTW_WLAN_SCN = 0, /* no secondary channel */
	RTW_WLAN_SCA = 1, /* secondary channel above */
	RTW_WLAN_SCB = 3, /* secondary channel below */
};

#define OUI_MICROSOFT 0x0050f2 /* Microsoft (also used in Wi-Fi specs) \
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

u8 *rtw_get_ie_ex(u8 *in_ie, uint in_len, u8 eid, u8 *oui, u8 oui_len, u8 *ie, uint *ielen);
int rtw_ies_remove_ie(u8 *ies, uint *ies_len, uint offset, u8 eid, u8 *oui, u8 oui_len);

int rtw_get_wapi_ie(u8 *in_ie, uint in_len, u8 *wapi_ie, u16 *wapi_len);

u8 rtw_is_wps_ie(u8 *ie_ptr, uint *wps_ielen);

void dump_ies(u8 *buf, u32 buf_len);
void dump_wps_ie(u8 *ie, u32 ie_len);

#ifdef CONFIG_P2P_NEW
u8 *rtw_get_p2p_ie(u8 *in_ie, uint in_len, u8 *p2p_ie, uint *p2p_ielen);
u8 *rtw_get_p2p_attr(u8 *p2p_ie, uint p2p_ielen, u8 target_attr_id, u8 *buf_attr, u32 *len_attr);
#endif //CONFIG_P2P
#ifdef CONFIG_P2P
void dump_p2p_ie(u8 *ie, u32 ie_len);
u8 *rtw_get_p2p_attr_content(u8 *p2p_ie, uint p2p_ielen, u8 target_attr_id, u8 *buf_content, uint *len_content);
u32 rtw_set_p2p_attr_content(u8 *pbuf, u8 attr_id, u16 attr_len, u8 *pdata_attr);
void rtw_WLAN_BSSID_EX_remove_p2p_attr(WLAN_BSSID_EX *bss_ex, u8 attr_id);
#endif

#ifdef CONFIG_WFD
int rtw_get_wfd_ie(u8 *in_ie, int in_len, u8 *wfd_ie, uint *wfd_ielen);
int rtw_get_wfd_attr_content(u8 *wfd_ie, uint wfd_ielen, u8 target_attr_id, u8 *attr_content, uint *attr_contentlen);
#endif // CONFIG_WFD

void rtw_get_bcn_info(struct wlan_network *pnetwork);

void rtw_macaddr_cfg(u8 *mac_addr);

u16 rtw_mcs_rate(u8 rf_type, u8 bw_40MHz, u8 short_GI_20, u8 short_GI_40, unsigned char *MCS_rate);
#endif /* __IEEE80211_H */
