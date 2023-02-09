/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
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
#ifndef __RTW_MLME_EXT_H_
#define __RTW_MLME_EXT_H_


//	Commented by Albert 20101105
//	Increase the SURVEY_TO value from 100 to 150  ( 100ms to 150ms )
//	The Realtek 8188CE SoftAP will spend around 100ms to send the probe response after receiving the probe request.
//	So, this driver tried to extend the dwell time for each scanning channel.
//	This will increase the chance to receive the probe response from SoftAP.

#if CONFIG_AUTO_RECONNECT

#endif

//TODO
#define FAST_SURVEY_TO	(25) //Fast connection time, scan only partial channel
#define SURVEY_TO		(100) //Reduce connection time
#define MAX_CNT_SCAN_TIMES	(3) //Max all-channel loop scan times when connect to but fail scan the target ap
#define MAX_CNT_AUTH_TIMES	(3) //Max connect times in case of receiving deauth/deassoc during auth&&assoc procedure and not reporting disconnect event
#define PASSIVE_SURVEY_TO		(110) //Passive scan should > 102.4ms
//#define SURVEY_TO		(300) //Increase time to stay each channel - Alex Fang
#define REAUTH_TO		(800) //(50)
#define REASSOC_TO		(300) //(50)
//#define DISCONNECT_TO	(3000)
#define ADDBA_TO			(2000)
#define DEFRAG_TO (2000)

#define LINKED_TO (1) //unit:2 sec, 1x2=2 sec

#define AP_CSA_TO			(102)

#define REAUTH_LIMIT	(4)
#define REASSOC_LIMIT	(4)
#define READDBA_LIMIT	(2)


#define ROAMING_LIMIT	8

//#define	IOCMD_REG0		0x10250370
//#define	IOCMD_REG1		0x10250374
//#define	IOCMD_REG2		0x10250378

//#define	FW_DYNAMIC_FUN_SWITCH	0x10250364

//#define	WRITE_BB_CMD		0xF0000001
//#define	SET_CHANNEL_CMD	0xF3000000
//#define	UPDATE_RA_CMD	0xFD0000A2

#define _HW_STATE_NOLINK_		0x00
#define _HW_STATE_ADHOC_		0x01
#define _HW_STATE_STATION_ 	0x02
#define _HW_STATE_AP_			0x03
#define _HW_STATE_MONITOR_ 	0x04

#define RTW_MAX_SCAN_REPORT_NODE 64

#define		_1M_RATE_	0
#define		_2M_RATE_	1
#define		_5M_RATE_	2
#define		_11M_RATE_	3
#define		_6M_RATE_	4
#define		_9M_RATE_	5
#define		_12M_RATE_	6
#define		_18M_RATE_	7
#define		_24M_RATE_	8
#define		_36M_RATE_	9
#define		_48M_RATE_	10
#define		_54M_RATE_	11

#define MAX_COUNTRY_NUM 250

#define WAIT_FOR_BCN_TO_MIN	(6000)
#define WAIT_FOR_BCN_TO_MAX	(20000)

#define DEL_STA_MAC_ID	0x7F

#define rltk_ap_compatibility_is_enable(a,b) ((a & b) ? 1 : 0)

//
// Channel Plan Type.
// Note:
//	We just add new channel plan when the new channel plan is different from any of the following
//	channel plan.
//	If you just wnat to customize the acitions(scan period or join actions) about one of the channel plan,
//	customize them in RT_CHANNEL_INFO in the RT_CHANNEL_LIST.
//
typedef enum _RT_CHANNEL_DOMAIN {
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

	//===== Add new channel plan above this line===============//
	RT_CHANNEL_DOMAIN_REALTEK_DEFINE = 0x7F,
	RT_CHANNEL_DOMAIN_MAX = 0x80
} RT_CHANNEL_DOMAIN, *PRT_CHANNEL_DOMAIN;

typedef enum _RT_CHANNEL_DOMAIN_2G {
	RTW_RD_2G_NULL = 0,	/* Do not support 2G*/
	RTW_RD_2G_01 = 1,	/* Worldwide 13, Canada */
	RTW_RD_2G_02 = 2,	/* Europe, Japan */
	RTW_RD_2G_03 = 3,		/* Worldwide 11, US, Canada, Korea */
	RTW_RD_2G_04 = 4,		/* Japan */
	RTW_RD_2G_05 = 5,	/* France */
	RTW_RD_2G_06 = 6,	/* 2G Global, include channel 14 */
	//===== Add new channel plan above this line===============//
	RT_CHANNEL_DOMAIN_2G_MAX,
} RT_CHANNEL_DOMAIN_2G, *PRT_CHANNEL_DOMAIN_2G;

typedef enum _RT_CHANNEL_DOMAIN_5G {
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
	//===== Add new channel plan above this line===============//
	RT_CHANNEL_DOMAIN_5G_MAX
} RT_CHANNEL_DOMAIN_5G, *PRT_CHANNEL_DOMAIN_5G;

#define rtw_is_channel_plan_valid(chplan) (chplan<RT_CHANNEL_DOMAIN_MAX || chplan == RT_CHANNEL_DOMAIN_REALTEK_DEFINE)

typedef struct _RT_CHANNEL_PLAN {
	unsigned char	Channel[MAX_CHANNEL_NUM];
	unsigned char	Len;
} RT_CHANNEL_PLAN, *PRT_CHANNEL_PLAN;

typedef struct _RT_CHANNEL_PLAN_2G {
	unsigned char	Channel[MAX_CHANNEL_NUM_2G];
	unsigned char	Len;
} RT_CHANNEL_PLAN_2G, *PRT_CHANNEL_PLAN_2G;

typedef struct _RT_CHANNEL_PLAN_5G {
	unsigned char	Channel[MAX_CHANNEL_NUM_5G];
	unsigned char	Len;
} RT_CHANNEL_PLAN_5G, *PRT_CHANNEL_PLAN_5G;

typedef struct _RT_CHANNEL_PLAN_MAP {
	unsigned char	ChannelPlan;
	unsigned char	Index2G;
#if defined(SUPPORT_5G_CHANNEL)
	unsigned char	Index5G;
#endif
	unsigned char	PwrLmt2G; /* value of REGULATION_TXPWR_LMT */
#if defined(SUPPORT_5G_CHANNEL)
	unsigned char	PwrLmt5G;
#endif
} RT_CHANNEL_PLAN_MAP, *PRT_CHANNEL_PLAN_MAP;


typedef struct _RT_CHANNEL_PLAN_COUNTRY_MAP {
	char *country_code;
	RT_CHANNEL_DOMAIN domain;

} RT_CHANNEL_PLAN_COUNTRY_MAP, *PRT_CHANNEL_PLAN_COUNTRY_MAP;



enum Associated_AP {
	atherosAP	= 0,
	broadcomAP	= 1,
	ciscoAP		= 2,
	marvellAP	= 3,
	ralinkAP	= 4,
	realtekAP	= 5,
	airgocapAP 	= 6,
	unknownAP	= 7,
	maxAP,
};

enum SCAN_STATE {
	SCAN_DISABLE = 0,
	SCAN_START = 1,
	SCAN_TXNULL = 2,
	SCAN_PROCESS = 3,
	SCAN_COMPLETE = 4,
	/* backop */
	SCAN_BACKING_OP = 5,
	SCAN_BACK_OP = 6,
	SCAN_LEAVING_OP = 7,
	SCAN_LEAVE_OP = 8,
	SCAN_STATE_MAX,
};

enum WIFI_INDICATE_MODE {
	WIFI_INDICATE_DISABLE = 0,
	WIFI_INDICATE_NORMAL = 1,
	WIFI_INDICATE_WILD = 2,
	WIFI_INDICATE_MAX,
};

struct	ss_res {
	int	state;
	int	bss_cnt;
	int	channel_idx;
	int	scan_mode;
	NDIS_802_11_SSID ssid[RTW_SSID_SCAN_AMOUNT];
#ifdef CONFIG_IEEE80211K
	u32 token; 	/* 0: use to identify caller */
	u16 duration;	/* 0: use default */
#endif
};

typedef struct  transition_disable {
	u8 bitmap; //Transition Disable Bitmap
	u32 AKM_suite; //Secure algorithm (keymgmt)
	u8 ssid[RTW_ESSID_MAX_SIZE + 1]; /**< SSID name (AP name)  */
	u8 passphrase[IW_PASSPHRASE_MAX_SIZE + 1];
	int ssid_len;      /**< SSID length */
	int passphrase_len;
} transition_disable_ctx, *ptransition_disable_ctx;

//#define AP_MODE	0x0C
//#define STATION_MODE	0x08
//#define AD_HOC_MODE	0x04
//#define NO_LINK_MODE	0x00

#define WIFI_FW_NULL_STATE		_HW_STATE_NOLINK_
#define	WIFI_FW_STATION_STATE		_HW_STATE_STATION_
#define	WIFI_FW_AP_STATE		_HW_STATE_AP_

#define	WIFI_FW_AUTH_NULL		0x00000100
#define	WIFI_FW_AUTH_STATE		0x00000200
#define	WIFI_FW_AUTH_SUCCESS		0x00000400

#define	WIFI_FW_ASSOC_STATE		0x00002000
#define	WIFI_FW_ASSOC_SUCCESS		0x00004000

#define	WIFI_FW_LINKING_STATE		(WIFI_FW_AUTH_NULL | WIFI_FW_AUTH_STATE | WIFI_FW_AUTH_SUCCESS |WIFI_FW_ASSOC_STATE)

struct FW_Sta_Info {
	struct sta_info	*psta;
	u32	status;
	u32	rx_pkt;
	u32	retry;
	NDIS_802_11_RATES_EX  SupportedRates;
};

/*
 * Usage:
 * When one iface acted as AP mode and the other iface is STA mode and scanning,
 * it should switch back to AP's operating channel periodically.
 * Parameters info:
 * When the driver scanned RTW_SCAN_NUM_OF_CH channels, it would switch back to AP's operating channel for
 * RTW_STAY_AP_CH_MILLISECOND * SURVEY_TO milliseconds.
 * Example:
 * For chip supports 2.4G + 5GHz and AP mode is operating in channel 1,
 * RTW_SCAN_NUM_OF_CH is 8, RTW_STAY_AP_CH_MILLISECOND is 3 and SURVEY_TO is 100.
 * When it's STA mode gets set_scan command,
 * it would
 * 1. Doing the scan on channel 1.2.3.4.5.6.7.8
 * 2. Back to channel 1 for 300 milliseconds
 * 3. Go through doing site survey on channel 9.10.11.36.40.44.48.52
 * 4. Back to channel 1 for 300 milliseconds
 * 5. ... and so on, till survey done.
 */
#if defined CONFIG_STA_MODE_SCAN_UNDER_AP_MODE && defined CONFIG_CONCURRENT_MODE
#define RTW_SCAN_NUM_OF_CH			8
#define RTW_STAY_AP_CH_MILLISECOND	3	// this value is a multiplier,for example, when this value is 3, it would stay AP's op ch for 
// 3 * SURVEY_TO millisecond.
#endif //defined CONFIG_STA_MODE_SCAN_UNDER_AP_MODE && defined CONFIG_CONCURRENT_MODE

#ifndef _CUS_IE_
#define _CUS_IE_
typedef struct _cus_ie {
	u8 *ie;
	u8 type;
} rtw_custom_ie_t, *p_rtw_custom_ie_t;
#endif /* _CUS_IE_ */

struct mlme_ext_info {
	u32	state;
	u32	reauth_count;
	u32	reassoc_count;
	u32	link_count;
	u32	auth_seq;
	u32	auth_algo;	// 802.11 auth, could be open, shared, auto
	u32	authModeToggle;
	u32	enc_algo;//encrypt algorithm;
	u32	key_index;	// this is only valid for legendary wep, 0~3 for key id.
	u32	iv;
	u8	chg_txt[128];
	u16	aid;
	u16	bcn_interval;
	u16	capability;
	u8	slotTime;
	u8	preamble_mode;
	u8	WMM_enable;
	u8	DTIM_period;
	u8	ERP_IE;
	u8	HT_enable;
	u8	HT_caps_enable;
	u8	HT_info_enable;
	u8	HT_protection;
	u8	agg_enable_bitmap;
	u8	ADDBA_retry_count;
	u8	candidate_tid_bitmap;
	u8	dialogToken;
	// Accept ADDBA Request
	BOOLEAN bAcceptAddbaReq;
	u8	bwmode_updated;
	u8	hidden_ssid_mode;

	u8	cur_channel;
	u8	cur_bwmode;
	u8	cur_ch_offset;//PRIME_CHNL_OFFSET

	p_rtw_custom_ie_t cus_ven_ie;
	u8 cus_ie_num;

	struct ADDBA_request		ADDBA_req;
	struct WMM_para_element	WMM_param;
	struct HT_caps_element	HT_caps;
	struct HT_info_element		HT_info;

#ifdef CONFIG_80211AX_HE
	struct HE_caps_element		HE_caps;
	unsigned char					HE_op[HE_OP_ELE_MAX_LEN];
#endif

	struct FW_Sta_Info		FW_sta_info[NUM_STA];

#ifdef CONFIG_STA_MODE_SCAN_UNDER_AP_MODE
	u8 scan_cnt;
#endif //CONFIG_STA_MODE_SCAN_UNDER_AP_MODE
#ifdef CONFIG_SAE_SUPPORT
	u8 user_group_id;			//for user to set group id
	struct sae_data *sae_priv;
	_lock sae_lock;
#endif

	int pmk_cache_enable;
	struct pmksa_cache *pmksa;

#ifdef CONFIG_CONCURRENT_MODE
	u8 bConcurrentFlushingSTA;
#endif

};

// The channel information about this channel including joining, scanning, and power constraints.
#define PSCAN_ENABLE          0x01 //enable for partial channel scan
#define PSCAN_FAST_SURVEY     0x02 //set to select scan time to FAST_SURVEY_TO and resend probe request
#define PSCAN_SIMPLE_CONFIG   0x04 //set to select scan time to FAST_SURVEY_TO and resend probe request
#define PSCAN_PASSIVE_SCAN    0x08 //partial passive channel scan
#define PSCAN_SET_SSID_DONE   0x80 //When receive probe response, this bit is set to 1

#define PSCAN_DISABLE_MASK    0xFE //disable PSCAN_ENABLE
#define PSCAN_CLEAR_SSID_DONE 0x7F //clear PSCAN_SET_SSID_DONE
#define PSCAN_CLEAR_PASSIVE_SCAN 0xF7 //clear PSCAN_PASSIVE_SCAN
#define PSCAN_RETRY_TIMES        7 //the retry times of resending probe request when PSCAN_FAST_SURVEY is set

typedef struct _RT_CHANNEL_INFO {
	u8				ChannelNum;		// The channel number.
	RT_SCAN_TYPE	ScanType;		// Scan type such as passive or active scan.
#ifdef CONFIG_FIND_BEST_CHANNEL
	u32				rx_count;
#endif
#ifdef CONFIG_DFS
	u8 hidden_bss_cnt; /* per scan count */
#endif
	u8	pscan_config;
} RT_CHANNEL_INFO, *PRT_CHANNEL_INFO;

extern int rtw_is_channel_set_contains_channel(RT_CHANNEL_INFO *channel_set, const u32 channel_num, int *pchannel_idx);

struct mlme_ext_priv {
	u16	mgnt_seq;
#ifdef CONFIG_IEEE80211W
	u16	sa_query_seq;
	u64 mgnt_80211w_IPN;
	u64 mgnt_80211w_IPN_rx;
	u8 key_type_11w; // for debug
#endif /* CONFIG_IEEE80211W */

	//struct fw_priv 	fwpriv;

	u8	wireless_mode_support;//A, B, G, auto
	u8	band_type_support;
	NDIS_802_11_RATES_EX	default_supported_rates;

	u8	cur_wireless_mode;	// NETWORK_TYPE
	u8	max_chan_nums;
	RT_CHANNEL_INFO		channel_set[MAX_CHANNEL_NUM + 1];
	u8	basicrate[NumRates];
	u8	datarate[NumRates];

	struct ss_res		sitesurvey_res;

	//for ap mode, network includes ap's cap_info
	_timer		survey_timer;
	_timer		link_timer;
	//_timer		ADDBA_timer;

#ifdef CONFIG_IEEE80211R
	_timer		ft_link_timer;
	_timer		ft_roam_timer;
#endif

	u8	scan_abort;
	u8	user_tx_rate; // TXRATE when USERATE is set.

	u8	retry; //retry for issue probereq

	unsigned char bstart_bss;

	/* for softap power save */
#if CONFIG_AUTO_RECONNECT
	_timer reconnect_timer;
	u8 reconnect_deauth_filtered;
	u8 reconnect_times;
	u8 reconnect_cnt;
	u16 reconnect_timeout; // the unit is second
	u8 saved_alg;
	u8 saved_essid[32 + 1];
	u8 saved_key[32 + 1];
	u8 saved_key_idx;
	u8 saved_wpa_passphrase[IW_PASSPHRASE_MAX_SIZE + 1];
	u8 saved_eap_method;
	u8 auto_reconnect;
#endif
	u8 partial_scan;

	_timer csa_timer;
	u8 csa_channel;
	u8 csa_ongoing;//flag

	transition_disable_ctx transition_disable;
};

enum {
	RTW_CHF_2G = BIT0,
	RTW_CHF_5G = BIT1,
	RTW_CHF_DFS = BIT2,
	RTW_CHF_LONG_CAC = BIT3,
	RTW_CHF_NON_DFS = BIT4,
	RTW_CHF_NON_LONG_CAC = BIT5,
	RTW_CHF_NON_OCP = BIT6,
};
#define mlmeext_scan_state(mlmeext) ((mlmeext)->sitesurvey_res.state)

int rtw_chset_search_ch(RT_CHANNEL_INFO *ch_set, const u32 ch);
u8 rtw_chset_is_chbw_valid(RT_CHANNEL_INFO *ch_set, u8 ch, u8 bw, u8 offset);

bool rtw_mlme_band_check(_adapter *adapter, const u32 ch);
u8 init_channel_set(_adapter *padapter, u8 ChannelPlan, RT_CHANNEL_INFO *channel_set);
void free_mlme_ext_priv(_adapter *padapter);
extern void init_addba_retry_timer(_adapter *padapter, struct sta_info *psta);
#ifdef CONFIG_IEEE80211W
extern void init_dot11w_expire_timer(_adapter *padapter, struct sta_info *psta);
#endif /* CONFIG_IEEE80211W */
extern struct xmit_frame *alloc_mgtxmitframe(struct xmit_priv *pxmitpriv);
extern struct xmit_frame *alloc_FwRsvdframe(struct xmit_priv *pxmitpriv, u32 size);
//void fill_fwpriv(_adapter * padapter, struct fw_priv *pfwpriv);

unsigned char networktype_to_raid(unsigned char network_type);
extern void wifi_rom_updateBratetbl(u8 *mBratesOS);
void wifi_rom_updateBratetbl_softap(u8 *bssrateset, u32 bssratelen);

void Save_DM_Func_Flag(_adapter *padapter);
void Restore_DM_Func_Flag(_adapter *padapter);
void Switch_DM_Func(_adapter *padapter, u32 mode, u8 enable);

u8 wifi_rom_get_offset_by_chbw(u8 ch, u8 bw, u8 *r_offset);

void wifi_rom_write_cam(u8 entry, u16 ctrl, u8 *mac, u8 *key);

void CAM_empty_entry(PADAPTER Adapter, u8 ucIndex);


int allocate_fw_sta_entry(_adapter *padapter);
void flush_all_cam_entry(_adapter *padapter);

BOOLEAN IsLegal5GChannel(PADAPTER Adapter, u8 channel);

void rtw_scan_one_channel(_adapter *padapter);
u8 rtw_scan_collect_bss_info(_adapter *padapter, union recv_frame *precv_frame, WLAN_BSSID_EX *bssid);
void rtw_scan_update_network(WLAN_BSSID_EX *dst, WLAN_BSSID_EX *src, _adapter *padapter, bool update_ie);

int get_bsstype(unsigned short capability);

int is_client_associated_to_ap(_adapter *padapter);

void HT_caps_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
void HT_info_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);

void VCS_update(_adapter *padapter, struct sta_info *psta);

void update_wireless_mode(_adapter *padapter);
int update_sta_support_rate(_adapter *padapter, u8 *pvar_ie, uint var_ie_len, int cam_idx);

//for sta/adhoc mode
void update_sta_info(_adapter *padapter, struct sta_info *psta);
unsigned int wifi_rom_update_basic_rate(unsigned char *ptn, unsigned int ptn_sz);
unsigned int wifi_rom_update_supported_rate(unsigned char *ptn, unsigned int ptn_sz);
unsigned int wifi_rom_update_mcs_rate(struct HT_caps_element *pHT_caps);
int wifi_rom_cckrates_included(unsigned char *rate, int ratelen);
int wifi_rom_cckratesonly_included(unsigned char *rate, int ratelen);
unsigned int wifi_rom_ratetbl2rateset(u8 datarate[], u8 basicrate[], unsigned char *rateset);
void wifi_rom_HT_caps_handler(struct HT_caps_element *HT_caps, PNDIS_802_11_VARIABLE_IEs pIE);

void set_sta_rate(_adapter *padapter, struct sta_info *psta);

unsigned int receive_disconnect(_adapter *padapter, unsigned char *MacAddr, unsigned short reason);

unsigned char wifi_rom_get_highest_rateidx(u32 mask);
int support_short_GI(_adapter *padapter, struct HT_caps_element *pHT_caps, u8 bwmode);
unsigned int is_ap_in_tkip(_adapter *padapter);
unsigned int is_ap_in_wep(_adapter *padapter);
unsigned int should_forbid_n_rate(_adapter *padapter);

void report_join_res(_adapter *padapter, int res);
void rtw_scan_one_beacon_report(_adapter *padapter, union recv_frame *precv_frame);
void report_surveydone_event(_adapter *padapter);
void report_del_sta_event(_adapter *padapter, unsigned char *MacAddr, unsigned short reason);
void report_add_sta_event(_adapter *padapter, unsigned char *MacAddr, int cam_idx);

void beacon_timing_control(_adapter *padapter);
extern u8 set_tx_beacon_cmd(_adapter *padapter);
unsigned int setup_beacon_frame(_adapter *padapter, unsigned char *beacon_frame);

void start_clnt_assoc(_adapter *padapter);
void start_clnt_auth(_adapter *padapter);
void start_clnt_join(_adapter *padapter);

void mlmeext_joinbss_event_callback(_adapter *padapter, int join_res);
void mlmeext_sta_del_event_callback(_adapter *padapter);
void mlmeext_sta_add_event_callback(_adapter *padapter, struct sta_info *psta);

void linked_status_chk(_adapter *padapter);

void rtw_scan_timer_hdl(void *FunctionContext);
void link_timer_hdl(void *FunctionContext);

#define set_survey_timer(mlmeext, ms) \
	do { \
		rtw_set_timer(&(mlmeext)->survey_timer, (ms)); \
	} while(0)

#define set_link_timer(mlmeext, ms) \
	do { \
		rtw_set_timer(&(mlmeext)->link_timer, (ms)); \
	} while(0)

#ifdef CONFIG_CONCURRENT_MODE
sint check_buddy_mlmeinfo_state(_adapter *padapter, u32 state);
void concurrent_chk_joinbss_done(_adapter *padapter, int join_res);
#endif //CONFIG_CONCURRENT_MODE

#ifdef CONFIG_PLATFORM_ARM_SUN8I
#define BUSY_TRAFFIC_SCAN_DENY_PERIOD	8000
#else
#define BUSY_TRAFFIC_SCAN_DENY_PERIOD	12000
#endif

extern u8 _rtw_free_stainfo_hdl(_adapter *padapter, unsigned char *pbuf);

#if CONFIG_AUTO_RECONNECT
extern void reconnect_timer_hdl(void *FunctionContext);
#endif

extern void csa_timer_hdl(void *FunctionContext);
extern void csa_process(_adapter *padapter, u8 new_ch_no, u8 new_ch_count);

#endif

