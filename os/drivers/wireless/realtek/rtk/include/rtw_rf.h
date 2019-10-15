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
#ifndef __RTW_RF_H_
#define __RTW_RF_H_

#include <hal_pg.h>

#define OFDM_PHY 1
#define MIXED_PHY 2
#define CCK_PHY 3

#define NumRates (13)

// slot time for 11g
#define SHORT_SLOT_TIME 9
#define NON_SHORT_SLOT_TIME 20

#define RTL8711_RF_MAX_SENS 6
#define RTL8711_RF_DEF_SENS 4

#define CENTER_CH_2G_40M_NUM 9
#define CENTER_CH_2G_NUM 14
#define CENTER_CH_5G_20M_NUM 28 /* 20M center channels */
#define CENTER_CH_5G_40M_NUM 14 /* 40M center channels */
#define CENTER_CH_5G_80M_NUM 7  /* 80M center channels */
#define CENTER_CH_5G_160M_NUM 3 /* 160M center channels */
#define CENTER_CH_5G_ALL_NUM (CENTER_CH_5G_20M_NUM + CENTER_CH_5G_40M_NUM + CENTER_CH_5G_80M_NUM)

//
// We now define the following channels as the max channels in each channel plan.
// 2G, total 14 chnls
// {1,2,3,4,5,6,7,8,9,10,11,12,13,14}
// 5G, total 25 chnls
// {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,144,149,153,157,161,165}
#define MAX_CHANNEL_NUM_2G CENTER_CH_2G_NUM
#define MAX_CHANNEL_NUM_5G CENTER_CH_5G_20M_NUM
#if defined(NOT_SUPPORT_5G)
#define MAX_CHANNEL_NUM MAX_CHANNEL_NUM_2G
#else
#define MAX_CHANNEL_NUM (MAX_CHANNEL_NUM_2G + MAX_CHANNEL_NUM_5G) //14+28
#endif

#ifndef NOT_SUPPORT_5G
#ifndef NOT_SUPPORT_80M
extern u8 center_ch_5g_80m[CENTER_CH_5G_80M_NUM];
#endif
extern u8 center_ch_5g_all[CENTER_CH_5G_ALL_NUM];
#endif

#define NUM_REGULATORYS 1

//Country codes
#define USA 0x555320
#define EUROPE 0x1 //temp, should be provided later
#define JAPAN 0x2  //temp, should be provided later

struct regulatory_class {
	u32 starting_freq; //MHz,
	u8 channel_set[MAX_CHANNEL_NUM];
	u8 channel_cck_power[MAX_CHANNEL_NUM];  //dbm
	u8 channel_ofdm_power[MAX_CHANNEL_NUM]; //dbm
	u8 txpower_limit;						//dbm
	u8 channel_spacing;						//MHz
	u8 modem;
};

typedef enum _CAPABILITY {
	cESS = 0x0001,
	cIBSS = 0x0002,
	cPollable = 0x0004,
	cPollReq = 0x0008,
	cPrivacy = 0x0010,
	cShortPreamble = 0x0020,
	cPBCC = 0x0040,
	cChannelAgility = 0x0080,
	cSpectrumMgnt = 0x0100,
	cQos = 0x0200, // For HCCA, use with CF-Pollable and CF-PollReq
	cShortSlotTime = 0x0400,
	cAPSD = 0x0800,
	cRM = 0x1000, // RRM (Radio Request Measurement)
	cDSSS_OFDM = 0x2000,
	cDelayedBA = 0x4000,
	cImmediateBA = 0x8000,
} CAPABILITY,
	*PCAPABILITY;

enum _REG_PREAMBLE_MODE {
	PREAMBLE_LONG = 1,
	PREAMBLE_AUTO = 2,
	PREAMBLE_SHORT = 3,
};

enum _RTL8712_RF_MIMO_CONFIG_ {
	RTL8712_RFCONFIG_1T = 0x10,
	RTL8712_RFCONFIG_2T = 0x20,
	RTL8712_RFCONFIG_1R = 0x01,
	RTL8712_RFCONFIG_2R = 0x02,
	RTL8712_RFCONFIG_1T1R = 0x11,
	RTL8712_RFCONFIG_1T2R = 0x12,
	RTL8712_RFCONFIG_TURBO = 0x92,
	RTL8712_RFCONFIG_2T2R = 0x22
};

typedef enum _RF90_RADIO_PATH {
	RF90_PATH_A = 0, //Radio Path A
	RF90_PATH_B = 1, //Radio Path B
	RF90_PATH_C = 2, //Radio Path C
	RF90_PATH_D = 3  //Radio Path D
} RF90_RADIO_PATH_E,
	*PRF90_RADIO_PATH_E;

// Bandwidth Offset
#define HAL_PRIME_CHNL_OFFSET_DONT_CARE 0
#define HAL_PRIME_CHNL_OFFSET_LOWER 1
#define HAL_PRIME_CHNL_OFFSET_UPPER 2

//
// Represent Extention Channel Offset in HT Capabilities
// This is available only in 40Mhz mode.
//
typedef enum _EXTCHNL_OFFSET {
	EXTCHNL_OFFSET_NO_EXT = 0,
	EXTCHNL_OFFSET_UPPER = 1,
	EXTCHNL_OFFSET_NO_DEF = 2,
	EXTCHNL_OFFSET_LOWER = 3,
} EXTCHNL_OFFSET,
	*PEXTCHNL_OFFSET;

typedef enum _VHT_DATA_SC {
	VHT_DATA_SC_DONOT_CARE = 0,
	VHT_DATA_SC_20_UPPER_OF_80MHZ = 1,
	VHT_DATA_SC_20_LOWER_OF_80MHZ = 2,
	VHT_DATA_SC_20_UPPERST_OF_80MHZ = 3,
	VHT_DATA_SC_20_LOWEST_OF_80MHZ = 4,
	VHT_DATA_SC_20_RECV1 = 5,
	VHT_DATA_SC_20_RECV2 = 6,
	VHT_DATA_SC_20_RECV3 = 7,
	VHT_DATA_SC_20_RECV4 = 8,
	VHT_DATA_SC_40_UPPER_OF_80MHZ = 9,
	VHT_DATA_SC_40_LOWER_OF_80MHZ = 10,
} VHT_DATA_SC,
	*PVHT_DATA_SC_E;

extern const u8 _ch_width_to_bw_cap[];
#define ch_width_to_bw_cap(bw) (((bw) < CHANNEL_WIDTH_MAX) ? _ch_width_to_bw_cap[(bw)] : 0)

#if (PHYDM_LINUX_CODING_STYLE == 1)
#define RF_TYPE_VALID(rf_type) (rf_type < RF_TYPE_MAX)

extern const u8 _rf_type_to_rf_tx_cnt[];
#define rf_type_to_rf_tx_cnt(rf_type) (RF_TYPE_VALID(rf_type) ? _rf_type_to_rf_tx_cnt[rf_type] : 0)

extern const u8 _rf_type_to_rf_rx_cnt[];
#define rf_type_to_rf_rx_cnt(rf_type) (RF_TYPE_VALID(rf_type) ? _rf_type_to_rf_rx_cnt[rf_type] : 0)
#endif

/* 2007/11/15 MH Define different RF type. */
#define BB_GAIN_2G 0
#if SUPPORT_5G_CHANNEL
#define BB_GAIN_5GLB1 1
#define BB_GAIN_5GLB2 2
#define BB_GAIN_5GMB1 3
#define BB_GAIN_5GMB2 4
#define BB_GAIN_5GHB 5
#endif

#if SUPPORT_5G_CHANNEL
#define BB_GAIN_NUM 6
#else
#define BB_GAIN_NUM 1
#endif

u32 rtw_ch2freq(u32 ch);
u32 rtw_freq2ch(u32 freq);

#endif //_RTL8711_RF_H_
