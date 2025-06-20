/**
  ******************************************************************************
  * @file    rom_rtw_defs.h
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

#ifndef __ROM_RTW_DEFS_H_
#define __ROM_RTW_DEFS_H_

#define TAG_WLAN_DRV   "WLAN"
#define TAG_WLAN_COEX  "COEX"
#define TAG_WLAN_INIC  "INIC"
#define TAG_WLAN_NAN   "NAN"

/**
  * @brief  The enumeration lists all the network mode.
  */
enum wlan_mode {
	WLAN_MD_INVALID = 0,
	WLAN_MD_11B	= BIT(0),
	WLAN_MD_11A	= BIT(1),
	WLAN_MD_11G	= BIT(2),
	WLAN_MD_11N	= BIT(3),
	WLAN_MD_11AC	= BIT(4),
	WLAN_MD_11AX	= BIT(5),

	/* Type for current wireless mode */
	WLAN_MD_11BG	= (WLAN_MD_11B | WLAN_MD_11G),
	WLAN_MD_11GN	= (WLAN_MD_11G | WLAN_MD_11N),
	WLAN_MD_11AN	= (WLAN_MD_11A | WLAN_MD_11N),
	WLAN_MD_11BN	= (WLAN_MD_11B | WLAN_MD_11N),
	WLAN_MD_11BGN	= (WLAN_MD_11B | WLAN_MD_11G | WLAN_MD_11N),
	WLAN_MD_11BGAC = (WLAN_MD_11B | WLAN_MD_11G | WLAN_MD_11AC),
	WLAN_MD_11BGAX = (WLAN_MD_11B | WLAN_MD_11G | WLAN_MD_11AX),
	WLAN_MD_11GAC  = (WLAN_MD_11G | WLAN_MD_11AC),
	WLAN_MD_11GAX  = (WLAN_MD_11G | WLAN_MD_11AX),
	WLAN_MD_11A_AC = (WLAN_MD_11A | WLAN_MD_11AC),
	WLAN_MD_11A_AX = (WLAN_MD_11A | WLAN_MD_11AX),

	/* Capability -Type for registry default wireless mode */
	WLAN_MD_11AGN	= (WLAN_MD_11A | WLAN_MD_11G | WLAN_MD_11N),
	WLAN_MD_11ABGN	= (WLAN_MD_11A | WLAN_MD_11B | WLAN_MD_11G | WLAN_MD_11N),
	WLAN_MD_11ANAC	= (WLAN_MD_11A | WLAN_MD_11N | WLAN_MD_11AC),
	WLAN_MD_11BGNAC = (WLAN_MD_11B | WLAN_MD_11G | WLAN_MD_11N | WLAN_MD_11AC),
	WLAN_MD_11GNAC  = (WLAN_MD_11G | WLAN_MD_11N | WLAN_MD_11AC),
	WLAN_MD_24G_MIX = (WLAN_MD_11B | WLAN_MD_11G | WLAN_MD_11N  | WLAN_MD_11AX),
	WLAN_MD_5G_MIX	= (WLAN_MD_11A | WLAN_MD_11N | WLAN_MD_11AC | WLAN_MD_11AX),
	WLAN_MD_6G_MIX 	= (WLAN_MD_11A | WLAN_MD_11AX),
	WLAN_MD_MAX	= (WLAN_MD_24G_MIX | WLAN_MD_5G_MIX),
};

enum _RT_CHANNEL_DOMAIN_2G {
	RTW_RD_2G_NULL = 0,	/* Do not support 2G*/
	RTW_RD_2G_01 = 1,	/* Worldwide 13, Canada */
	RTW_RD_2G_02 = 2,	/* Europe, Japan */
	RTW_RD_2G_03 = 3,		/* Worldwide 11, US, Canada, Korea */
	RTW_RD_2G_04 = 4,		/* Japan */
	RTW_RD_2G_05 = 5,	/* France */
	RTW_RD_2G_06 = 6,	/* 2G Global, include channel 14 */
	//===== Add new channel plan above this line===============//
	RT_CHANNEL_DOMAIN_2G_MAX,
};

enum _RT_CHANNEL_DOMAIN_5G {
	RTW_RD_5G_NULL = 0,		/*Do not support 5G*/
	RTW_RD_5G_01 = 1,		/*US, Canada, Australia, New Zealand, Mexico (w/o Weather radar), (w/o Ch120~Ch128)*/
	RTW_RD_5G_02 = 2,		/*Europe, Japan (W52, W53, W56)*/
	RTW_RD_5G_03 = 3,		/*Australia, New Zealand, US, Korea*/
	RTW_RD_5G_04 = 4,		/*Russia (w/o Ch136, Ch140)*/
	RTW_RD_5G_05 = 5,		/*Korea*/
	RTW_RD_5G_06 = 6,		/*FCC w/o DFS Channels, Jordan*/
	RTW_RD_5G_07 = 7,		/*Bolivia, Chile, El Salvador, Venezuela, China */
	RTW_RD_5G_08 = 8,		/*Venezuela, Indonesia (w/o Ch165)*/
	RTW_RD_5G_09 = 9,		/*China, Taiwan w/o DFS, Qatar*/
	RTW_RD_5G_10 = 10,		/*Japan (W52, W53), Israel, Bahrain, Egypt, India, China, Malaysia*/
	RTW_RD_5G_11 = 11,		/*Japan (W56)*/
	RTW_RD_5G_12 = 12,		/*Taiwan, (w/o Weather radar)(w/o Ch52, Ch120~Ch128)*/
	RTW_RD_5G_13 = 13,		/*Taiwan, Band2, Band4*/
	RTW_RD_5G_14 = 14,		/*Europe, Japan (W52), w/o DFS, Band1 only*/
	RTW_RD_5G_15 = 15,		/*Latin America, Indonesia*/
	RTW_RD_5G_16 = 16,		/*Lebanon*/
	RTW_RD_5G_17 = 17,		/*Russia*/
	RTW_RD_5G_18 = 18,		/*Taiwan (w/o Weather radar)(w/o Ch120~Ch128)*/
	RTW_RD_5G_19 = 19,		/*ETSI, FCC (w/o Weather radar)(w/o Ch120~Ch128)*/
	RTW_RD_5G_20 = 20,		/*Argentina(w/o Weather radar)(w/o Ch120~Ch128, Ch165)*/
	RTW_RD_5G_21 = 21,		/*Russia*/
	RTW_RD_5G_22 = 22,		/*US (include Ch144), Chile, Korea (2018 Dec 05 New standard, include ch144)*/
	RTW_RD_5G_23 = 23,		/*Malaysia*/
	RTW_RD_5G_24 = 24,		/*Japan, Europe*/
	RTW_RD_5G_25 = 25,		/*Europe, FCC*/
	RTW_RD_5G_26 = 26,		/*FCC*/
	RTW_RD_5G_27 = 27,		/*FCC w/o Weather radar(w/o Ch120~Ch128)*/
	RTW_RD_5G_28 = 28,		/*FCC w/o Band3 China*/
	RTW_RD_5G_29 = 29,		/*FCC w/o Band3*/
	RTW_RD_5G_30 = 30,		/*ETSI w/o DFS Band2&3*/
	RTW_RD_5G_31 = 31,		/*Europe*/
	RTW_RD_5G_32 = 32,		/*Europe*/
	RTW_RD_5G_33 = 33,		/*Canada (w/o Weather radar)(w/o Ch120~Ch128, include Ch144)*/
	RTW_RD_5G_34 = 34,		/*Australia, New Zealand (w/o Weather radar)(w/o Ch120~Ch128)*/
	RTW_RD_5G_35 = 35,		/**/
	RTW_RD_5G_36 = 36,		/*5G Worldwide (include Ch144) Band2&3 w/i DFS for AP mode.*/
	RTW_RD_5G_37 = 37,		/*5G Worldwide Band1&2*/
	RTW_RD_5G_38 = 38,		/*Chile (Band2,Band3)*/
	RTW_RD_5G_39 = 39,		/*Japan*/
	RTW_RD_5G_40 = 40,		/*Japan*/
	RTW_RD_5G_41 = 41,		/*Japan*/
	RTW_RD_5G_42 = 42,		/*Europe*/
	RTW_RD_5G_43 = 43,		/*Japan, FCC*/
	RTW_RD_5G_44 = 44,		/*FCC*/
	RTW_RD_5G_45 = 45,		/*FCC w/o Band3*/
	RTW_RD_5G_46 = 46,		/*Indonesia*/
	RTW_RD_5G_47 = 47,		/*Japan, Singapore*/
	RTW_RD_5G_48 = 48,		/*Japan*/
	RTW_RD_5G_49 = 49,		/**/
	RTW_RD_5G_50 = 50,		/*Russia*/
	RTW_RD_5G_51 = 51,		/*Tunisia*/
	RTW_RD_5G_52 = 52,		/*US (include Ch144)(2018 Dec 05 New standard, include ch144)Add FCC 5.9G Channel*/
	RTW_RD_5G_53 = 53,		/*Korea*/
	RTW_RD_5G_54 = 54,
	RTW_RD_5G_55 = 55,      /*5G Worldwide (include Ch144 and FCC 5.9G) Band2&3 w/i DFS for AP mode. */
	RTW_RD_5G_56 = 56,      /* Canada (w/o Weather radar) (w/o Ch120~Ch128, include Ch144) Add Canada 5.9G Channel*/
	RTW_RD_5G_57 = 57,      /*Europe*/
	RTW_RD_5G_58 = 58,      /*Russia*/
	RTW_RD_5G_59 = 59,      /*Bolivia*/
	RTW_RD_5G_60 = 60,      /*ETSI (include ch169/ch173)*/
	RTW_RD_5G_61 = 61,      /*Malaysia(include ch169/ch173)*/
	RTW_RD_5G_62 = 62,
	//===== Add new channel plan above this line===============//
	RT_CHANNEL_DOMAIN_5G_MAX
};

// Tx Power Limit Table Size
enum _REGULATION_TXPWR_LMT {
	TXPWR_LMT_FCC = 0,
	TXPWR_LMT_MKK = 1,      /* Japan */
	TXPWR_LMT_ETSI = 2,     /* CE */
	TXPWR_LMT_IC = 3,       /* Canada */
	TXPWR_LMT_KCC = 4,      /* South Korea */
	TXPWR_LMT_ACMA = 5, 	/* Australia */
	TXPWR_LMT_CHILE = 6,    /* Chile */
	TXPWR_LMT_MEXICO = 7,   /* Mexico */
	TXPWR_LMT_WW = 8,       /* Worldwide, The mininum of all */
	TXPWR_LMT_GL = 9,		/* Global */
	TXPWR_LMT_UKRAINE = 10, /* Ukraine */
	TXPWR_LMT_CN = 11,       /* China */
	TXPWR_LMT_QATAR = 12,   /* Qatar */
	TXPWR_LMT_UK = 13,      /* Great Britain (United Kingdom; England) */
	TXPWR_LMT_NCC = 14,     /* Taiwan */
	TXPWR_LMT_EXT = 15,     /* Customer Customization */

	/* ===== Add new power limit above this line. ===== */
	TXPWR_LMT_MAX           /* Not support */
};


#define TXPWR_LMT_MAX_REGULATION_NUM  TXPWR_LMT_MAX
#define COUNTRY_CODE_LEN 2

//
// Channel Plan Type.
// Note:
//	We just add new channel plan when the new channel plan is different from any of the following
//	channel plan.
//	If you just wnat to customize the acitions(scan period or join actions) about one of the channel plan,
//	customize them in RT_CHANNEL_INFO in the RT_CHANNEL_LIST.
//
enum _RT_CHANNEL_DOMAIN {
	//===== new channel plan mapping, (2GDOMAIN_5GDOMAIN) =====//
	RT_CHANNEL_DOMAIN_WORLD_NULL = 0x20,
	RT_CHANNEL_DOMAIN_ETSI1_NULL = 0x21,
	RT_CHANNEL_DOMAIN_FCC1_NULL = 0x22,
	RT_CHANNEL_DOMAIN_MKK1_NULL = 0x23,
	RT_CHANNEL_DOMAIN_ETSI2_NULL = 0x24,
	RT_CHANNEL_DOMAIN_FCC1_FCC1 = 0x25,
	RT_CHANNEL_DOMAIN_WORLD_ETSI1 = 0x26,
	RT_CHANNEL_DOMAIN_MKK1_MKK1 = 0x27,
	RT_CHANNEL_DOMAIN_WORLD_KCC1 = 0x28,
	RT_CHANNEL_DOMAIN_WORLD_FCC2 = 0x29,
	RT_CHANNEL_DOMAIN_FCC2_NULL = 0x2A,
	RT_CHANNEL_DOMAIN_IC1_IC2 = 0x2B,
	RT_CHANNEL_DOMAIN_MKK2_NULL = 0x2C,
	RT_CHANNEL_DOMAIN_WORLD_CHILE1 = 0x2D,
	RT_CHANNEL_DOMAIN_WORLD1_WORLD1 = 0x2E,
	RT_CHANNEL_DOMAIN_WORLD_CHILE2 = 0x2F,
	RT_CHANNEL_DOMAIN_WORLD_FCC3 = 0x30,
	RT_CHANNEL_DOMAIN_WORLD_FCC4 = 0x31,
	RT_CHANNEL_DOMAIN_WORLD_FCC5 = 0x32,
	RT_CHANNEL_DOMAIN_WORLD_FCC6 = 0x33,
	RT_CHANNEL_DOMAIN_FCC1_FCC7 = 0x34,
	RT_CHANNEL_DOMAIN_WORLD_ETSI2 = 0x35,
	RT_CHANNEL_DOMAIN_WORLD_ETSI3 = 0x36,
	RT_CHANNEL_DOMAIN_MKK1_MKK2 = 0x37,
	RT_CHANNEL_DOMAIN_MKK1_MKK3 = 0x38,
	RT_CHANNEL_DOMAIN_FCC1_NCC1 = 0x39,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI1 = 0x3A,
	RT_CHANNEL_DOMAIN_ETSI1_ACMA1 = 0x3B,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI6 = 0x3C,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI12 = 0x3D,
	RT_CHANNEL_DOMAIN_KCC1_KCC2 = 0x3E,
	RT_CHANNEL_DOMAIN_FCC1_FCC11 = 0x3F,
	RT_CHANNEL_DOMAIN_FCC1_NCC2 = 0x40,
	RT_CHANNEL_DOMAIN_GLOBAL_NULL = 0x41,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI4 = 0x42,
	RT_CHANNEL_DOMAIN_FCC1_FCC2 = 0x43,
	RT_CHANNEL_DOMAIN_FCC1_NCC3 = 0x44,
	RT_CHANNEL_DOMAIN_WORLD_ACMA1 = 0x45,
	RT_CHANNEL_DOMAIN_FCC1_FCC8 = 0x46,
	RT_CHANNEL_DOMAIN_WORLD_ETSI6 = 0x47,
	RT_CHANNEL_DOMAIN_WORLD_ETSI7 = 0x48,
	RT_CHANNEL_DOMAIN_WORLD_ETSI8 = 0x49,
	RT_CHANNEL_DOMAIN_IC2_IC2 = 0x4A,
	RT_CHANNEL_DOMAIN_KCC1_KCC3 = 0x4B,
	RT_CHANNEL_DOMAIN_FCC1_FCC15 = 0x4C,
	RT_CHANNEL_DOMAIN_FCC2_MEX1 = 0x4D,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI22 = 0x4E,
	RT_CHANNEL_DOMAIN_NULL_MKK9 = 0x4F,
	RT_CHANNEL_DOMAIN_WORLD_ETSI9 = 0x50,
	RT_CHANNEL_DOMAIN_WORLD_ETSI10 = 0x51,
	RT_CHANNEL_DOMAIN_WORLD_ETSI11 = 0x52,
	RT_CHANNEL_DOMAIN_FCC1_NCC4 = 0x53,
	RT_CHANNEL_DOMAIN_WORLD_ETSI12 = 0x54,
	RT_CHANNEL_DOMAIN_FCC1_FCC9 = 0x55,
	RT_CHANNEL_DOMAIN_WORLD_ETSI13 = 0x56,
	RT_CHANNEL_DOMAIN_FCC1_FCC10 = 0x57,
	RT_CHANNEL_DOMAIN_MKK2_MKK4 = 0x58,
	RT_CHANNEL_DOMAIN_WORLD_ETSI14 = 0x59,
	RT_CHANNEL_DOMAIN_NULL_FCC19 = 0x5A,
	RT_CHANNEL_DOMAIN_NULL_FCC20 = 0x5B,
	RT_CHANNEL_DOMAIN_NULL_FCC21 = 0x5C,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI23	= 0x5D,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI2 = 0x5E,
	RT_CHANNEL_DOMAIN_MKK2_MKK10 = 0x5F,
	RT_CHANNEL_DOMAIN_FCC1_FCC5 = 0x60,
	RT_CHANNEL_DOMAIN_FCC2_FCC7 = 0x61,
	RT_CHANNEL_DOMAIN_FCC2_FCC1 = 0x62,
	RT_CHANNEL_DOMAIN_WORLD_ETSI15 = 0x63,
	RT_CHANNEL_DOMAIN_MKK2_MKK5 = 0x64,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI16 = 0x65,
	RT_CHANNEL_DOMAIN_FCC1_FCC14 = 0x66,
	RT_CHANNEL_DOMAIN_FCC1_FCC12 = 0x67,
	RT_CHANNEL_DOMAIN_FCC2_FCC14 = 0x68,
	RT_CHANNEL_DOMAIN_FCC2_FCC12 = 0x69,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI17 = 0x6A,
	RT_CHANNEL_DOMAIN_WORLD_FCC16 = 0x6B,
	RT_CHANNEL_DOMAIN_WORLD_FCC13 = 0x6C,
	RT_CHANNEL_DOMAIN_FCC2_FCC15 = 0x6D,
	RT_CHANNEL_DOMAIN_WORLD_FCC12 = 0x6E,
	RT_CHANNEL_DOMAIN_NULL_ETSI8 = 0x6F,
	RT_CHANNEL_DOMAIN_NULL_ETSI18 = 0x70,
	RT_CHANNEL_DOMAIN_NULL_ETSI17 = 0x71,
	RT_CHANNEL_DOMAIN_NULL_ETSI19 = 0x72,
	RT_CHANNEL_DOMAIN_WORLD_FCC7 = 0x73,
	RT_CHANNEL_DOMAIN_FCC2_FCC17 = 0x74,
	RT_CHANNEL_DOMAIN_WORLD_ETSI20 = 0x75,
	RT_CHANNEL_DOMAIN_FCC2_FCC11 = 0x76,
	RT_CHANNEL_DOMAIN_WORLD_ETSI21 = 0x77,
	RT_CHANNEL_DOMAIN_FCC1_FCC18 = 0x78,
	RT_CHANNEL_DOMAIN_MKK2_MKK1 = 0x79,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI25 = 0x7A,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI24 = 0x7B,
	RT_CHANNEL_DOMAIN_ETSI1_ETSI26 = 0x7C,
	RT_CHANNEL_DOMAIN_MKK1_MKK11 = 0x7D,
	RT_CHANNEL_DOMAIN_MKK2_MKK11 = 0x7E,
	RT_CHANNEL_DOMAIN_WORLDWIDE = 0x7F,

	/* ===== Add new channel plan above this line. ===== */
	RT_CHANNEL_DOMAIN_MAX
};

#define BAND_CAP_2G			BIT(0)
#define BAND_CAP_5G			BIT(1)

// Scan type including active and passive scan.
enum _RT_SCAN_TYPE {
	SCAN_PASSIVE = 1,
	SCAN_ACTIVE,
};

enum channel_width {
	CHANNEL_WIDTH_20		= 0,
	CHANNEL_WIDTH_40		= 1,
	CHANNEL_WIDTH_80		= 2,
	CHANNEL_WIDTH_160		= 3,
	CHANNEL_WIDTH_80_80	= 4,
	CHANNEL_WIDTH_5		= 5,
	CHANNEL_WIDTH_10	= 6,
	CHANNEL_WIDTH_MAX	= 7,
};

#define WLANHDR_OFFSET	64
#define TXDESC_OFFSET TXDESC_SIZE

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

enum _NDIS_802_11_AUTHENTICATION_MODE {
	Ndis802_11AuthModeWPA = 3,
	Ndis802_11AuthModeWPAPSK = 4,
	Ndis802_11AuthModeWPA2 = 7,
	Ndis802_11AuthModeWPA2PSK = 8,
	Ndis802_11AuthModeWPA3 = 9,
	Ndis802_11AuthModeWPA3PSK = 10,
	Ndis802_11AuthModeMax               // Not a real mode, defined as upper bound
};

#define MAX_IE_SZ	768 //384//

/* Extended Capabilities: n bytes */
#define GET_EXT_CAP_MBSSID(_pEleStart)					LE_BITS_TO_1BYTE(((u8 *)(_pEleStart) + 2), 6, 1)
#define SET_EXT_CAP_MBSSID(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart) + 2), 6, 1, _val)

enum VCS_TYPE {
	NONE_VCS,
	RTS_CTS,
	CTS_TO_SELF
};

#define WAIT_FOR_BCN_TO_MIN		(6000)
#define WAIT_FOR_BCN_TO_MAX		(20000)

#define A_SHA_DIGEST_LEN		20
#define INFO_ELEMENT_SIZE       128

/* SECCAM sec_type define */
#define _NO_PRIVACY_	0x0
#define _WEP40_		0x1
#define _TKIP_		0x2
#define _TKIP_WTMIC_	0x3
#define _AES_		0x4	//_CCMP_128_
#define _WEP104_	0x5
#define _SMS4_		0x6	//_WAPI_
#define _GCMP_		0x7
#define _GCMP_256_	(_GCMP_ | BIT(3))
#define _CCMP_256_	(_AES_ | BIT(3))
#define _GCM_WAPI_	(_SMS4_ | BIT(3))		//_GCM_WAPI_
#define _BIP_		0x8

#define BW_CAP_5M		BIT0
#define BW_CAP_10M		BIT1
#define BW_CAP_20M		BIT2
#define BW_CAP_40M		BIT3
#define BW_CAP_80M		BIT4
#define BW_CAP_160M		BIT5
#define BW_CAP_80_80M		BIT6

enum chan_offset {
	CHAN_OFFSET_NO_EXT = 0,	/*SCN - no secondary channel*/
	CHAN_OFFSET_UPPER = 1,	/*SCA - secondary channel above*/
	CHAN_OFFSET_NO_DEF = 2,	/*Reserved*/
	CHAN_OFFSET_LOWER = 3,	/*SCB - secondary channel below*/
	CHAN_OFFSET_40M_UPPER = 4,
	CHAN_OFFSET_40M_LOWER = 5,
	CHAN_OFFSET_80M_UPPER = 6,
	CHAN_OFFSET_80M_LOWER = 7,
};

enum ENCRYP_PROTOCOL_E {
	ENCRYP_PROTOCOL_OPENSYS,   //open system
	ENCRYP_PROTOCOL_WEP,       //WEP
	ENCRYP_PROTOCOL_WPA,       //WPA
	ENCRYP_PROTOCOL_WPA2,      //WPA2
	ENCRYP_PROTOCOL_WPA_WPA2,  //WPA & WPA2
	ENCRYP_PROTOCOL_WAPI,      //WAPI: Not support in this version
	ENCRYP_PROTOCOL_MAX
};

/**
 * enum mfp_options - Management frame protection (IEEE 802.11w) options
 */
enum mfp_options {
	NO_MGMT_FRAME_PROTECTION = 0,
	MGMT_FRAME_PROTECTION_OPTIONAL = 1,
	MGMT_FRAME_PROTECTION_REQUIRED = 2,
};
#define MGMT_FRAME_PROTECTION_DEFAULT 3

/* B2-B3: PTKSA Replay Counter */
#define WPA_CAPABILITY_PTKSA(x)	(((u32)((x) & 0x00000003) << 2))
#define WPA_CAPABILITY_MFPR BIT(6)
#define WPA_CAPABILITY_MFPC BIT(7)

enum dot11AuthAlgrthmNum {
	dot11AuthAlgrthm_Open = 0,
	dot11AuthAlgrthm_Shared = 1,
	dot11AuthAlgrthm_8021X = 2,
	dot11AuthAlgrthm_SAE = 3,
	dot11AuthAlgrthm_MaxNum = 7
};

enum channel_group {
	CHANNEL_GROUP_2G = 0,
	CHANNEL_GROUP_5G_LOW,
	CHANNEL_GROUP_5G_MID,
	CHANNEL_GROUP_5G_HIGH,
	CHANNEL_GROUP_NUM
};


#define _AUTH_ALGM_NUM_			2
#define _AUTH_SEQ_NUM_			2
#define _BEACON_ITERVAL_		2
#define _CAPABILITY_			2
#define _RSON_CODE_				2
#define _ASOC_ID_				2
#define _STATUS_CODE_			2
#define _TIMESTAMP_				8

/* Values in HE spec */
#define TXOP_DUR_RTS_TH_DISABLED	1023

#define SN_LESS(a, b)		(((a-b)&0x800)!=0)
#define SN_EQUAL(a, b)	(a == b)

#define SHA256_MAC_LEN 32

#define CCK_ONLY_RATE_NUM		4
#define OFDM_ONLY_RATE_NUM		8
#define NumRates			12
#define OFDM_ONLY_BASIC_RATE_NUM	3

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

#define RTW_RX_HANDLED			2
#define RTW_RFRAME_UNAVAIL		3
#define RTW_RFRAME_PKT_UNAVAIL	4
#define RTW_RBUF_UNAVAIL		5
#define RTW_RBUF_PKT_UNAVAIL	6
#define RTW_SDIO_READ_PORT_FAIL	7


enum rf_type {
	RF_1T1R			= 0,
	RF_1T2R			= 1,
	RF_2T2R			= 2,
	RF_2T3R			= 3,
	RF_2T4R			= 4,
	RF_3T3R			= 5,
	RF_3T4R			= 6,
	RF_4T4R			= 7,
	RF_TYPE_MAX,
};

enum _IFACE_TYPE {
	IFACE_PORT0, //mapping to port0 for C/D series chips
	IFACE_PORT1, //mapping to port1 for C/D series chip
	IFACE_PORT2,
	MAX_IFACE_PORT,
};

enum _WIRELESS_MODE {
	WIRELESS_MODE_UNKNOWN = 0x00,
	WIRELESS_MODE_A = 0x01,
	WIRELESS_MODE_B = 0x02,
	WIRELESS_MODE_G = 0x04,
	WIRELESS_MODE_AUTO = 0x08,
	WIRELESS_MODE_N_24G = 0x10,
	WIRELESS_MODE_N_5G = 0x20,
	WIRELESS_MODE_AC_5G = 0x40,
	WIRELESS_MODE_AC_24G  = 0x80,
	WIRELESS_MODE_AC_ONLY  = 0x100,
};

#define _HW_MSR_STATE_NOLINK_		0x00
#define _HW_MSR_STATE_NAN_		0x01		//only smart valid
#define _HW_MSR_STATE_STATION_ 		0x02
#define _HW_MSR_STATE_AP_		0x03
#define _HW_MSR_STATE_MONITOR_		0x04

enum _hw_port {
	HW_PORT0,
	HW_PORT1,
	HW_PORT2,
};

#define CHANNEL_MAX_NUMBER						14	// 14 is the max channel number
#define CHANNEL_MAX_NUMBER_2G					14
#define CHANNEL_MAX_NUMBER_5G					54	// Please refer to "phy_GetChnlGroup8812A" and "Hal_ReadTxPowerInfo8812A"
#define CHANNEL_MAX_NUMBER_5G_80M				7

#define MAX_REGULATION_NUM	TXPWR_LMT_MAX_REGULATION_NUM

#define MAX_2_4G_BANDWITH_NUM					2 // 20M, 40M
#if !defined(CONFIG_80211AC_VHT)
#define MAX_RATE_SECTION_NUM					3 // CCk, OFDM, HT
#define MAX_BASE_NUM_IN_PHY_REG_PG_2_4G			3 //  CCK:1,OFDM:1, HT:1(MCS0_MCS7)
#else
#define MAX_RATE_SECTION_NUM					10
#define MAX_BASE_NUM_IN_PHY_REG_PG_2_4G			10 //  CCK:1,OFDM:1, HT:4, VHT:4
#endif
#define MAX_5G_BANDWITH_NUM						4
#define MAX_BASE_NUM_IN_PHY_REG_PG_5G			9 // OFDM:1, HT:4, VHT:4

enum hal_rate_mode {
	HAL_LEGACY_MODE	= 0,
	HAL_HT_MODE	= 1,
	HAL_VHT_MODE	= 2,
	HAL_HE_MODE	= 3
};

#if !defined(SUPPORT_5G_CHANNEL)
#define TX_PWR_BY_RATE_NUM_BAND			1
#else
#define TX_PWR_BY_RATE_NUM_BAND			2
#endif

#if !defined(CONFIG_80211AC_VHT)
#define RTW_TX_PWR_BY_RATE_NUM_RF			1
#define TX_PWR_BY_RATE_NUM_RATE			20 // CCK 1M~11M, OFDM 6M~54M, MCS0~7
#else
#define RTW_TX_PWR_BY_RATE_NUM_RF			1
#define TX_PWR_BY_RATE_NUM_RATE			30 // CCK 1M~11M, OFDM 6M~54M, MCS0~7, VHT MCS0~MCS9
#endif

#endif //__ROM_RTW_DEFS_H_
