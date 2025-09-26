/**
  ******************************************************************************
  * @file    rtw_80211spec_enum.h
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

#ifndef	__RTW_80211SPEC_ENUM_H_
#define __RTW_80211SPEC_ENUM_H_

enum WIFI_FRAME_TYPE {
	WIFI_MGT_TYPE  =	(0),
	WIFI_CTRL_TYPE =	(BIT(2)),
	WIFI_DATA_TYPE =	(BIT(3)),
	WIFI_QOS_DATA_TYPE	= (BIT(7) | BIT(3)),	//!< QoS Data
};

enum WIFI_FRAME_TYPESUBTYPE {
	// below is for mgt frame
	WIFI_ASSOCREQ       = (0 | WIFI_MGT_TYPE),
	WIFI_ASSOCRSP       = (BIT(4) | WIFI_MGT_TYPE),
	WIFI_REASSOCREQ     = (BIT(5) | WIFI_MGT_TYPE),
	WIFI_REASSOCRSP     = (BIT(5) | BIT(4) | WIFI_MGT_TYPE),
	WIFI_PROBEREQ       = (BIT(6) | WIFI_MGT_TYPE),
	WIFI_PROBERSP       = (BIT(6) | BIT(4) | WIFI_MGT_TYPE),
	WIFI_BEACON         = (BIT(7) | WIFI_MGT_TYPE),
	WIFI_ATIM           = (BIT(7) | BIT(4) | WIFI_MGT_TYPE),
	WIFI_DISASSOC       = (BIT(7) | BIT(5) | WIFI_MGT_TYPE),
	WIFI_AUTH           = (BIT(7) | BIT(5) | BIT(4) | WIFI_MGT_TYPE),
	WIFI_DEAUTH         = (BIT(7) | BIT(6) | WIFI_MGT_TYPE),
	WIFI_ACTION         = (BIT(7) | BIT(6) | BIT(4) | WIFI_MGT_TYPE),

	// below is for control frame
	WIFI_PSPOLL         = (BIT(7) | BIT(5) | WIFI_CTRL_TYPE),
	WIFI_RTS            = (BIT(7) | BIT(5) | BIT(4) | WIFI_CTRL_TYPE),
	WIFI_CTS            = (BIT(7) | BIT(6) | WIFI_CTRL_TYPE),
	WIFI_ACK            = (BIT(7) | BIT(6) | BIT(4) | WIFI_CTRL_TYPE),
	WIFI_BA            	= (BIT(7) | BIT(4) | WIFI_CTRL_TYPE),
	WIFI_CFEND          = (BIT(7) | BIT(6) | BIT(5) | WIFI_CTRL_TYPE),
	WIFI_CFEND_CFACK    = (BIT(7) | BIT(6) | BIT(5) | BIT(4) | WIFI_CTRL_TYPE),
	WIFI_TRIGGER        = (BIT(5) | WIFI_CTRL_TYPE),

	// below is for data frame
	WIFI_DATA           = (0 | WIFI_DATA_TYPE),
	WIFI_DATA_CFACK     = (BIT(4) | WIFI_DATA_TYPE),
	WIFI_DATA_CFPOLL    = (BIT(5) | WIFI_DATA_TYPE),
	WIFI_DATA_CFACKPOLL = (BIT(5) | BIT(4) | WIFI_DATA_TYPE),
	WIFI_DATA_NULL      = (BIT(6) | WIFI_DATA_TYPE),
	WIFI_CF_ACK         = (BIT(6) | BIT(4) | WIFI_DATA_TYPE),
	WIFI_CF_POLL        = (BIT(6) | BIT(5) | WIFI_DATA_TYPE),
	WIFI_CF_ACKPOLL     = (BIT(6) | BIT(5) | BIT(4) | WIFI_DATA_TYPE),
	WIFI_QOS_DATA_NULL	= (BIT(6) | WIFI_QOS_DATA_TYPE),
};


enum _ELEMENT_ID {
	EID_SsId					= 0, /* service set identifier (0:32) */
	EID_SupRates				= 1, /* supported rates (1:8) */
	EID_FHParms				= 2, /* FH parameter set (5) */
	EID_DSParms				= 3, /* DS parameter set (1) */
	EID_CFParms				= 4, /* CF parameter set (6) */
	EID_Tim						= 5, /* Traffic Information Map (4:254) */
	EID_IbssParms				= 6, /* IBSS parameter set (2) */
	EID_Country					= 7, /* */

	/* Form 7.3.2: Information elements in 802.11E/D13.0, page 46. */
	EID_QBSSLoad				= 11,
	EID_EDCAParms				= 12,
	EID_TSpec					= 13,
	EID_TClass					= 14,
	EID_Schedule				= 15,
	/*  */

	EID_Ctext					= 16, /* challenge text*/
	EID_POWER_CONSTRAINT		= 32, /* Power Constraint*/

	/* vivi for WIFITest, 802.11h AP, 20100427 */
	/* 2010/12/26 MH The definition we can declare always!! */
	EID_PowerCap				= 33,
	EID_TPC				= 35,
	EID_SupportedChannels		= 36,
	EID_ChlSwitchAnnounce		= 37,

	EID_MeasureRequest			= 38, /* Measurement Request */
	EID_MeasureReport			= 39, /* Measurement Report */

	EID_ERPInfo				= 42,

	/* Form 7.3.2: Information elements in 802.11E/D13.0, page 46. */
	EID_TSDelay				= 43,
	EID_TCLASProc				= 44,
	EID_HTCapability			= 45,
	EID_QoSCap					= 46,
	/*  */

	EID_WPA2					= 48,
	EID_ExtSupRates			= 50,

	EID_FTIE					= 55, /* Defined in 802.11r */
	EID_Timeout				= 56, /* Defined in 802.11r */

	EID_SupRegulatory			= 59, /* Supported Requlatory Classes 802.11y */
	EID_HTInfo					= 61,
	EID_SecondaryChnlOffset		= 62,

	EID_BSSCoexistence			= 72, /* 20/40 BSS Coexistence */
	EID_BSSIntolerantChlReport	= 73,
	EID_OBSS					= 74, /* Overlapping BSS Scan Parameters */

	EID_LinkIdentifier			= 101, /* Defined in 802.11z */
	EID_WakeupSchedule		= 102, /* Defined in 802.11z */
	EID_ChnlSwitchTimeing		= 104, /* Defined in 802.11z */
	EID_PTIControl				= 105, /* Defined in 802.11z */
	EID_PUBufferStatus			= 106, /* Defined in 802.11z */

	EID_EXTCapability			= 127, /* Extended Capabilities */
	/* From S19:Aironet IE and S21:AP IP address IE in CCX v1.13, p16 and p18. */
	EID_Aironet					= 133, /* 0x85: Aironet Element for Cisco CCX */
	EID_CiscoIP					= 149, /* 0x95: IP Address IE for Cisco CCX */

	EID_CellPwr					= 150, /* 0x96: Cell Power Limit IE. Ref. 0x96. */

	EID_CCKM					= 156,

	EID_Vendor					= 221, /* 0xDD: Vendor Specific */

	EID_WAPI					= 68,
	EID_VHTCapability 			= 191, /* Based on 802.11ac D2.0 */
	EID_VHTOperation 			= 192, /* Based on 802.11ac D2.0 */
	EID_AID						= 197, /* Based on 802.11ac D4.0 */
	EID_OpModeNotification		= 199, /* Based on 802.11ac D3.0 */
};

enum _HT_CAP_AMPDU_FACTOR {
	MAX_AMPDU_FACTOR_8K		= 0,
	MAX_AMPDU_FACTOR_16K	= 1,
	MAX_AMPDU_FACTOR_32K	= 2,
	MAX_AMPDU_FACTOR_64K	= 3,
};

enum WIFI_REASON_CODE	{
	_RSON_RESERVED_					= 0,
	_RSON_UNSPECIFIED_				= 1,
	_RSON_AUTH_NO_LONGER_VALID_		= 2,
	_RSON_DEAUTH_STA_LEAVING_		= 3,
	_RSON_INACTIVITY_				= 4,
	_RSON_UNABLE_HANDLE_			= 5,
	_RSON_CLS2_						= 6,
	_RSON_CLS3_						= 7,
	_RSON_DISAOC_STA_LEAVING_		= 8,
	_RSON_ASOC_NOT_AUTH_			= 9,

	// WPA reason
	_RSON_INVALID_IE_				= 13,
	_RSON_MIC_FAILURE_				= 14,
	_RSON_4WAY_HNDSHK_TIMEOUT_		= 15,
	_RSON_GROUP_KEY_UPDATE_TIMEOUT_	= 16,
	_RSON_DIFF_IE_					= 17,
	_RSON_MLTCST_CIPHER_NOT_VALID_	= 18,
	_RSON_UNICST_CIPHER_NOT_VALID_	= 19,
	_RSON_AKMP_NOT_VALID_			= 20,
	_RSON_UNSUPPORT_RSNE_VER_		= 21,
	_RSON_INVALID_RSNE_CAP_			= 22,
	_RSON_IEEE_802DOT1X_AUTH_FAIL_	= 23,

	//belowing are Realtek definition
	_RSON_PMK_NOT_AVAILABLE_		= 24,
	_RSON_TDLS_TEAR_TOOFAR_			= 25,
	_RSON_TDLS_TEAR_UN_RSN_			= 26,
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

#endif

