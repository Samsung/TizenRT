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
#ifndef	__RTW_RF_H_
#define __RTW_RF_H_

#define OFDM_PHY		1
#define MIXED_PHY		2
#define CCK_PHY		3

// slot time for 11g
#define SHORT_SLOT_TIME					9
#define NON_SHORT_SLOT_TIME				20


#define CENTER_CH_2G_40M_NUM	9
#define CENTER_CH_2G_NUM		14
#define CENTER_CH_5G_20M_NUM	28	/* 20M center channels */
#define CENTER_CH_5G_40M_NUM	14	/* 40M center channels */
#define CENTER_CH_5G_80M_NUM	7	/* 80M center channels */
#define CENTER_CH_5G_160M_NUM	3	/* 160M center channels */
#define CENTER_CH_5G_ALL_NUM	(CENTER_CH_5G_20M_NUM + CENTER_CH_5G_40M_NUM + CENTER_CH_5G_80M_NUM)

//
// We now define the following channels as the max channels in each channel plan.
// 2G, total 14 chnls
// {1,2,3,4,5,6,7,8,9,10,11,12,13,14}
// 5G, total 25 chnls
// {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,144,149,153,157,161,165}
#define	MAX_CHANNEL_NUM_2G				CENTER_CH_2G_NUM
#define	MAX_CHANNEL_NUM_5G				CENTER_CH_5G_20M_NUM
#if !defined(SUPPORT_5G_CHANNEL)
#define	MAX_CHANNEL_NUM					MAX_CHANNEL_NUM_2G
#else
#define	MAX_CHANNEL_NUM					(MAX_CHANNEL_NUM_2G + MAX_CHANNEL_NUM_5G) //14+28
#endif

#if defined(NOT_SUPPORT_RF_MULTIPATH)
#define RTW_MAX_RF_PATH					1
#define MAX_TX_COUNT				1
#else
#define RTW_MAX_RF_PATH					2	// Max 4 for ss larger than 2
#define MAX_TX_COUNT				4	//It must always set to 4, otherwise read efuse table secquence will be wrong.
#endif
#define	MAX_CHNL_GROUP_24G		6 		// ch1~2, ch3~5, ch6~8,ch9~11,ch12~13,CH 14 total three groups
#define	MAX_CHNL_GROUP_5G		14

#define NUM_REGULATORYS	1

//Country codes
#define USA							0x555320
#define EUROPE						0x1 //temp, should be provided later	
#define JAPAN						0x2 //temp, should be provided later	


typedef enum _CAPABILITY {
	cESS			= 0x0001,
	cIBSS			= 0x0002,
	cPollable		= 0x0004,
	cPollReq			= 0x0008,
	cPrivacy		= 0x0010,
	cShortPreamble	= 0x0020,
	cPBCC			= 0x0040,
	cChannelAgility	= 0x0080,
	cSpectrumMgnt	= 0x0100,
	cQos			= 0x0200,	// For HCCA, use with CF-Pollable and CF-PollReq
	cShortSlotTime	= 0x0400,
	cAPSD			= 0x0800,
	cRM				= 0x1000,	// RRM (Radio Request Measurement)
	cDSSS_OFDM	= 0x2000,
	cDelayedBA		= 0x4000,
	cImmediateBA	= 0x8000,
} CAPABILITY, *PCAPABILITY;

enum	_REG_PREAMBLE_MODE {
	PREAMBLE_LONG	= 1,
	PREAMBLE_AUTO	= 2,
	PREAMBLE_SHORT	= 3,
};



struct center_chs_ent_t {
	u8 ch_num;
	const u8 *chs;
};

struct op_chs_ent_t {
	u8 ch_num;
	const u8 *chs;
};

#ifndef _RTW_RF_C_
extern const u8 _ch_width_to_bw_cap[];
extern u8 center_ch_5g_80m[CENTER_CH_5G_80M_NUM];
extern u8 center_ch_5g_all[CENTER_CH_5G_ALL_NUM];
#endif
#define ch_width_to_bw_cap(bw) (((bw) < CHANNEL_WIDTH_MAX) ? _ch_width_to_bw_cap[(bw)] : 0)

#define RF_TYPE_VALID(rf_type) (rf_type < RF_TYPE_MAX)

extern const u8 _rf_type_to_rf_tx_cnt[];
#define rf_type_to_rf_tx_cnt(rf_type) (RF_TYPE_VALID(rf_type) ? _rf_type_to_rf_tx_cnt[rf_type] : 0)

extern const u8 _rf_type_to_rf_rx_cnt[];
#define rf_type_to_rf_rx_cnt(rf_type) (RF_TYPE_VALID(rf_type) ? _rf_type_to_rf_rx_cnt[rf_type] : 0)

#define BB_GAIN_2G 0
#define BB_GAIN_5GLB1 1
#define BB_GAIN_5GLB2 2
#define BB_GAIN_5GMB1 3
#define BB_GAIN_5GMB2 4
#define BB_GAIN_5GHB 5

u32 wifi_rom_ch2freq(u32 ch);
u32 wifi_rom_freq2ch(u32 freq);

/* only check channel ranges */
#define rtw_is_2g_ch(ch) (ch >= 1 && ch <= 14)
#define rtw_is_5g_ch(ch) ((ch) >= 36 && (ch) <= 177)


u8 wifi_rom_get_op_chs_by_cch_bw(u8 cch, u8 bw, u8 **op_chs, u8 *op_ch_num);

#endif

