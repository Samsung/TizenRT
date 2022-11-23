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
#ifndef __HAL_COM_PHYCFG_H__
#define __HAL_COM_PHYCFG_H__

#define		PathA                     			0x0	// Useless
#define		PathB                     			0x1
#define		PathC                     			0x2
#define		PathD                     			0x3

typedef enum _RATE_SECTION {
	CCK = 0,
	OFDM,
	HT_MCS0_MCS7,
	HT_MCS8_MCS15,
	HT_MCS16_MCS23,
	HT_MCS24_MCS31,
	VHT_1SSMCS0_1SSMCS9,
	VHT_2SSMCS0_2SSMCS9,
	VHT_3SSMCS0_3SSMCS9,
	VHT_4SSMCS0_4SSMCS9,
	RATE_SECTION_NUM,
} RATE_SECTION;

typedef enum _RF_TX_NUM {
	RF_1TX = 0,
	RF_2TX,
	RF_3TX,
	RF_4TX,
	RF_MAX_TX_NUM,
	RF_TX_NUM_NONIMPLEMENT,
} RF_TX_NUM;

#if (RTL8721D_SUPPORT == 1) || (RTL8710C_SUPPORT == 1) || (RTL8730A_SUPPORT == 1) || (RTL8735B_SUPPORT == 1) || (RTL8720E_SUPPORT == 1) || (RTL8730E_SUPPORT == 1)
#define MAX_POWER_INDEX 		0x7F //8721d TXAGC power index extends 1 bit (6 bit->7bit), 0.25dB a step
#else
#define MAX_POWER_INDEX 		0x3F
#endif

typedef enum _REGULATION_TXPWR_LMT {
	TXPWR_LMT_FCC = 0,
	TXPWR_LMT_MKK = 1,
	TXPWR_LMT_ETSI = 2,
	TXPWR_LMT_IC = 3,
	TXPWR_LMT_KCC = 4,
	TXPWR_LMT_ACMA = 5,
	TXPWR_LMT_CHILE = 6,
	TXPWR_LMT_MEXICO = 7,
	TXPWR_LMT_WW = 8, // WW13, The mininum of ETSI,MKK
	TXPWR_LMT_GL = 9, // Global, The mininum of all
	TXPWR_LMT_UKRAINE = 10,
	TXPWR_LMT_CN = 11,
	TXPWR_LMT_QATAR = 12,
	TXPWR_LMT_UK = 13,
	TXPWR_LMT_NCC = 14,
	TXPWR_LMT_MAX_REGULATION_NUM = 15,
	TXPWR_LMT_NA = 16  /* Not support, choose NA */
} REGULATION_TXPWR_LMT;

/*------------------------------Define structure----------------------------*/
typedef struct _BB_REGISTER_DEFINITION {
	u32 rfintfs;			// set software control:
	//		0x870~0x877[8 bytes]

	u32 rfintfo; 			// output data:
	//		0x860~0x86f [16 bytes]

	u32 rfintfe; 			// output enable:
	//		0x860~0x86f [16 bytes]

	u32 rf3wireOffset;	// LSSI data:
	//		0x840~0x84f [16 bytes]

	u32 rfHSSIPara2; 	// wire parameter control2 :
	//		0x824~0x827,0x82c~0x82f, 0x834~0x837, 0x83c~0x83f [16 bytes]

	u32 rfLSSIReadBack; 	//LSSI RF readback data SI mode
	//		0x8a0~0x8af [16 bytes]

	u32 rfLSSIReadBackPi; 	//LSSI RF readback data PI mode 0x8b8-8bc for Path A and B

} BB_REGISTER_DEFINITION_T, *PBB_REGISTER_DEFINITION_T;


//----------------------------------------------------------------------
s32
phy_TxPwrIdxToDbm(
	IN	PADAPTER		Adapter,
	IN	WIRELESS_MODE	WirelessMode,
	IN	u8				TxPwrIdx
);

u8
PHY_GetTxPowerByRateBase(
	IN	PADAPTER		Adapter,
	IN	u8				Band,
	IN	u8				RfPath,
	IN	u8				TxNum,
	IN	RATE_SECTION	RateSection
);

u8
PHY_GetRateSectionIndexOfTxPowerByRate(
	IN	PADAPTER	pAdapter,
	IN	u32			RegAddr,
	IN	u32			BitMask
);

VOID
PHY_GetRateValuesOfTxPowerByRate(
	IN	PADAPTER	pAdapter,
	IN	u32			RegAddr,
	IN	u32			BitMask,
	IN	u32			Value,
	OUT	u8			*RateIndex,
	OUT	s8			*PwrByRateVal,
	OUT	u8			*RateNum
);

u8
PHY_GetRateIndexOfTxPowerByRate(
	IN	u8	Rate
);

VOID
PHY_SetTxPowerIndexByRateSection(
	IN	PADAPTER		pAdapter,
	IN	u8				RFPath,
	IN	u8				Channel,
	IN	u8				RateSection
);

s8
PHY_GetTxPowerByRate(
	IN	PADAPTER	pAdapter,
	IN	u8			Band,
	IN	u8			RFPath,
	IN	u8			TxNum,
	IN	u8			RateIndex
);

VOID
PHY_SetTxPowerByRate(
	IN	PADAPTER	pAdapter,
	IN	u8			Band,
	IN	u8			RFPath,
	IN	u8			TxNum,
	IN	u8			Rate,
	IN	s8			Value
);

VOID
PHY_SetTxPowerLevelByPath(
	IN	PADAPTER	Adapter,
	IN	u8			channel,
	IN	u8			path
);

VOID
PHY_SetTxPowerIndexByRateArray(
	IN	PADAPTER		pAdapter,
	IN	u8				RFPath,
	IN	enum channel_width	BandWidth,
	IN	u8				Channel,
	IN	u8				*Rates,
	IN	u8				RateArraySize
);

VOID
PHY_InitTxPowerByRate(
	IN	PADAPTER	pAdapter
);

VOID
PHY_SetTxPwrTrkIndex(
	IN	PADAPTER			pAdapter,
	IN	u8					RFPath,
	IN	u8 				Rates
);

VOID
PHY_StoreTxPowerByRate(
	IN	PADAPTER	pAdapter,
	IN	u32			Band,
	IN	u32			RfPath,
	IN	u32			TxNum,
	IN	u32			RegAddr,
	IN	u32			BitMask,
	IN	u32			Data
);

VOID
PHY_TxPowerByRateConfiguration(
	IN  PADAPTER			pAdapter
);

u8
PHY_GetTxPowerIndexBase(
	IN	PADAPTER		pAdapter,
	IN	u8				RFPath,
	IN	u8				Rate,
	IN	enum channel_width	BandWidth,
	IN	u8				Channel,
	OUT PBOOLEAN		bIn24G
);

s8
PHY_GetTxPowerLimit(
	IN	PADAPTER		Adapter,
	IN	u32				RegPwrTblSel,
	IN	enum band_type		Band,
	IN	enum channel_width	Bandwidth,
	IN	u8				RfPath,
	IN	u8				DataRate,
	IN	u8				Channel
);

VOID
PHY_SetTxPowerLimit(
	IN	PADAPTER			Adapter,
	IN	u8					Regulation,
	IN	u8					Band,
	IN	u8					Bandwidth,
	IN	u8					RateSection,
	IN	u8					RfPath,
	IN	u8					Channel,
	IN	u8					PowerLimit
);

VOID
PHY_ConvertTxPowerLimitToPowerIndex(
	IN	PADAPTER			Adapter
);

VOID
PHY_InitTxPowerLimit(
	IN	PADAPTER			Adapter
);

s8
PHY_GetTxPowerTrackingOffset(
	PADAPTER	pAdapter,
	u8			Rate,
	u8			RFPath
);

u8
PHY_GetTxPowerIndex(
	IN	PADAPTER			pAdapter,
	IN	u8					RFPath,
	IN	u8					Rate,
	IN	enum channel_width		BandWidth,
	IN	u8					Channel
);

VOID
PHY_SetTxAgcToHw(
	IN	PADAPTER		pAdapter
);


VOID
PHY_SetTxPowerIndex(
	IN	PADAPTER		pAdapter,
	IN	u32				PowerIndex,
	IN	u8				RFPath,
	IN	u8				Rate
);

VOID
Hal_ChannelPlanToRegulation(
	IN	PADAPTER		Adapter,
	IN	u16				ChannelPlan
);

#endif //__HAL_COMMON_H__

