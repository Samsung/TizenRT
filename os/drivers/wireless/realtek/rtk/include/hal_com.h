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
#ifndef __HAL_COMMON_H__
#define __HAL_COMMON_H__

#include "HalVerDef.h"
#include "hal_pg.h"
#include "hal_intf.h"
#include "hal_phy.h"
#include "hal_phy_reg.h"
#include "hal_com_reg.h"
#include "hal_com_phycfg.h"
#ifdef RTW_HALMAC
#include "hal_com_c2h.h"
#endif

#if(PHYDM_LINUX_CODING_STYLE == 1)
#include "hal_com_c2h.h"
#endif

//----------------------------------------------------------------------------
//       Rate Definition
//----------------------------------------------------------------------------
//CCK
#define	RATR_1M					0x00000001
#define	RATR_2M					0x00000002
#define	RATR_55M					0x00000004
#define	RATR_11M					0x00000008
//OFDM 		
#define	RATR_6M					0x00000010
#define	RATR_9M					0x00000020
#define	RATR_12M					0x00000040
#define	RATR_18M					0x00000080
#define	RATR_24M					0x00000100
#define	RATR_36M					0x00000200
#define	RATR_48M					0x00000400
#define	RATR_54M					0x00000800
//MCS 1 Spatial Stream	
#define	RATR_MCS0					0x00001000
#define	RATR_MCS1					0x00002000
#define	RATR_MCS2					0x00004000
#define	RATR_MCS3					0x00008000
#define	RATR_MCS4					0x00010000
#define	RATR_MCS5					0x00020000
#define	RATR_MCS6					0x00040000
#define	RATR_MCS7					0x00080000
//MCS 2 Spatial Stream
#define	RATR_MCS8					0x00100000
#define	RATR_MCS9					0x00200000
#define	RATR_MCS10					0x00400000
#define	RATR_MCS11					0x00800000
#define	RATR_MCS12					0x01000000
#define	RATR_MCS13					0x02000000
#define	RATR_MCS14					0x04000000
#define	RATR_MCS15					0x08000000

//CCK
#define RATE_1M						BIT(0)
#define RATE_2M						BIT(1)
#define RATE_5_5M					BIT(2)
#define RATE_11M					BIT(3)
//OFDM 
#define RATE_6M						BIT(4)
#define RATE_9M						BIT(5)
#define RATE_12M					BIT(6)
#define RATE_18M					BIT(7)
#define RATE_24M					BIT(8)
#define RATE_36M					BIT(9)
#define RATE_48M					BIT(10)
#define RATE_54M					BIT(11)
//MCS 1 Spatial Stream
#define RATE_MCS0					BIT(12)
#define RATE_MCS1					BIT(13)
#define RATE_MCS2					BIT(14)
#define RATE_MCS3					BIT(15)
#define RATE_MCS4					BIT(16)
#define RATE_MCS5					BIT(17)
#define RATE_MCS6					BIT(18)
#define RATE_MCS7					BIT(19)
//MCS 2 Spatial Stream
#define RATE_MCS8					BIT(20)
#define RATE_MCS9					BIT(21)
#define RATE_MCS10					BIT(22)
#define RATE_MCS11					BIT(23)
#define RATE_MCS12					BIT(24)
#define RATE_MCS13					BIT(25)
#define RATE_MCS14					BIT(26)
#define RATE_MCS15					BIT(27)

// ALL CCK Rate
#define	RATE_ALL_CCK				RATR_1M|RATR_2M|RATR_55M|RATR_11M 
#define	RATE_ALL_OFDM_AG			RATR_6M|RATR_9M|RATR_12M|RATR_18M|RATR_24M|\
									RATR_36M|RATR_48M|RATR_54M	
#define	RATE_ALL_OFDM_1SS			RATR_MCS0|RATR_MCS1|RATR_MCS2|RATR_MCS3 |\
									RATR_MCS4|RATR_MCS5|RATR_MCS6	|RATR_MCS7	
#define	RATE_ALL_OFDM_2SS			RATR_MCS8|RATR_MCS9	|RATR_MCS10|RATR_MCS11|\
									RATR_MCS12|RATR_MCS13|RATR_MCS14|RATR_MCS15

/*------------------------------ Tx Desc definition Macro ------------------------*/ 
//#pragma mark -- Tx Desc related definition. --
//----------------------------------------------------------------------------
//-----------------------------------------------------------
//	Rate
//-----------------------------------------------------------
// CCK Rates, TxHT = 0
#define DESC_RATE1M				0x00
#define DESC_RATE2M				0x01
#define DESC_RATE5_5M				0x02
#define DESC_RATE11M				0x03

// OFDM Rates, TxHT = 0
#define DESC_RATE6M				0x04
#define DESC_RATE9M				0x05
#define DESC_RATE12M				0x06
#define DESC_RATE18M				0x07
#define DESC_RATE24M				0x08
#define DESC_RATE36M				0x09
#define DESC_RATE48M				0x0a
#define DESC_RATE54M				0x0b

// MCS Rates, TxHT = 1
#define DESC_RATEMCS0				0x0c
#define DESC_RATEMCS1				0x0d
#define DESC_RATEMCS2				0x0e
#define DESC_RATEMCS3				0x0f
#define DESC_RATEMCS4				0x10
#define DESC_RATEMCS5				0x11
#define DESC_RATEMCS6				0x12
#define DESC_RATEMCS7				0x13
#define DESC_RATEMCS8				0x14
#define DESC_RATEMCS9				0x15
#define DESC_RATEMCS10				0x16
#define DESC_RATEMCS11				0x17
#define DESC_RATEMCS12				0x18
#define DESC_RATEMCS13				0x19
#define DESC_RATEMCS14				0x1a
#define DESC_RATEMCS15				0x1b
#define DESC_RATEMCS16				0x1C
#define DESC_RATEMCS17				0x1D
#define DESC_RATEMCS18				0x1E
#define DESC_RATEMCS19				0x1F
#define DESC_RATEMCS20				0x20
#define DESC_RATEMCS21				0x21
#define DESC_RATEMCS22				0x22
#define DESC_RATEMCS23				0x23
#define DESC_RATEMCS24				0x24
#define DESC_RATEMCS25				0x25
#define DESC_RATEMCS26				0x26
#define DESC_RATEMCS27				0x27
#define DESC_RATEMCS28				0x28
#define DESC_RATEMCS29				0x29
#define DESC_RATEMCS30				0x2A
#define DESC_RATEMCS31				0x2B

#define DESC_RATEVHTSS1MCS0		0x2c
#define DESC_RATEVHTSS1MCS1		0x2d
#define DESC_RATEVHTSS1MCS2		0x2e
#define DESC_RATEVHTSS1MCS3		0x2f
#define DESC_RATEVHTSS1MCS4		0x30
#define DESC_RATEVHTSS1MCS5		0x31
#define DESC_RATEVHTSS1MCS6		0x32
#define DESC_RATEVHTSS1MCS7		0x33
#define DESC_RATEVHTSS1MCS8		0x34
#define DESC_RATEVHTSS1MCS9		0x35
#define DESC_RATEVHTSS2MCS0		0x36
#define DESC_RATEVHTSS2MCS1		0x37
#define DESC_RATEVHTSS2MCS2		0x38
#define DESC_RATEVHTSS2MCS3		0x39
#define DESC_RATEVHTSS2MCS4		0x3a
#define DESC_RATEVHTSS2MCS5		0x3b
#define DESC_RATEVHTSS2MCS6		0x3c
#define DESC_RATEVHTSS2MCS7		0x3d
#define DESC_RATEVHTSS2MCS8		0x3e
#define DESC_RATEVHTSS2MCS9		0x3f
#define DESC_RATEVHTSS3MCS0		0x40
#define DESC_RATEVHTSS3MCS1		0x41
#define DESC_RATEVHTSS3MCS2		0x42
#define DESC_RATEVHTSS3MCS3		0x43
#define DESC_RATEVHTSS3MCS4		0x44
#define DESC_RATEVHTSS3MCS5		0x45
#define DESC_RATEVHTSS3MCS6		0x46
#define DESC_RATEVHTSS3MCS7		0x47
#define DESC_RATEVHTSS3MCS8		0x48
#define DESC_RATEVHTSS3MCS9		0x49
#define DESC_RATEVHTSS4MCS0		0x4A
#define DESC_RATEVHTSS4MCS1		0x4B
#define DESC_RATEVHTSS4MCS2		0x4C
#define DESC_RATEVHTSS4MCS3		0x4D
#define DESC_RATEVHTSS4MCS4		0x4E
#define DESC_RATEVHTSS4MCS5		0x4F
#define DESC_RATEVHTSS4MCS6		0x50
#define DESC_RATEVHTSS4MCS7		0x51
#define DESC_RATEVHTSS4MCS8		0x52
#define DESC_RATEVHTSS4MCS9		0x53
										
typedef enum _FIRMWARE_SOURCE {
	FW_SOURCE_IMG_FILE = 0,
	FW_SOURCE_HEADER_FILE = 1,		//from header file
} FIRMWARE_SOURCE, *PFIRMWARE_SOURCE;

#define QSLT_BK							0x2//0x01
#define QSLT_BE							0x0
#define QSLT_VI							0x5//0x4
#define QSLT_VO							0x7//0x6
#define QSLT_BEACON						0x10
#define QSLT_HIGH						0x11
#define QSLT_MGNT						0x12
#define QSLT_CMD						0x13

// BK, BE, VI, VO, HCCA, MANAGEMENT, COMMAND, HIGH, BEACON.
//#define MAX_TX_QUEUE		9

#define TX_SELE_HQ			BIT(0)		// High Queue
#define TX_SELE_LQ			BIT(1)		// Low Queue
#define TX_SELE_NQ			BIT(2)		// Normal Queue
#define TX_SELE_EQ			BIT(3)		// Extern Queue

#define PageNum_128(_Len)		(u32)(((_Len)>>7) + ((_Len)&0x7F ? 1:0))
#define PageNum_256(_Len)		(u32)(((_Len)>>8) + ((_Len)&0xFF ? 1:0))
#define PageNum_512(_Len)		(u32)(((_Len)>>9) + ((_Len)&0x1FF ? 1:0))
#define PageNum(_Len, _Size)		(u32)(((_Len)/(_Size)) + ((_Len)&((_Size) - 1) ? 1:0))

#define	DYNAMIC_FUNC_DISABLE		(0x0)
#define DYNAMIC_ALL_FUNC_ENABLE		0xFFFFFFF

#define TBTT_PROHIBIT_SETUP_TIME 0x04 /* 128us, unit is 32us */
#define TBTT_PROHIBIT_HOLD_TIME 0x80 /* 4ms, unit is 32us*/
#define TBTT_PROHIBIT_HOLD_TIME_STOP_BCN 0x64 /* 3.2ms unit is 32us*/

void dump_chip_info(HAL_VERSION	ChipVersion);

#if (PHYDM_LINUX_CODING_STYLE==1)
#define BAND_CAP_2G			BIT0
#define BAND_CAP_5G			BIT1
#define BAND_CAP_BIT_NUM	2

#define BW_CAP_5M		BIT0
#define BW_CAP_10M		BIT1
#define BW_CAP_20M		BIT2
#define BW_CAP_40M		BIT3
#define BW_CAP_80M		BIT4
#define BW_CAP_160M		BIT5
#define BW_CAP_80_80M	BIT6
#define BW_CAP_BIT_NUM	7

#define PROTO_CAP_11B		BIT0
#define PROTO_CAP_11G		BIT1
#define PROTO_CAP_11N		BIT2
#define PROTO_CAP_11AC		BIT3
#define PROTO_CAP_BIT_NUM	4

#define WL_FUNC_P2P			BIT0
#define WL_FUNC_MIRACAST	BIT1
#define WL_FUNC_TDLS		BIT2
#define WL_FUNC_FTM			BIT3
#define WL_FUNC_BIT_NUM		4


s8 rtw_get_sta_rx_nss(_adapter *adapter, struct sta_info *psta);
#endif

u8	//return the final channel plan decision
hal_com_get_channel_plan(
	IN	PADAPTER	padapter,
	IN	u8			hw_channel_plan,	//channel plan from HW (efuse/eeprom)
	IN	u8			sw_channel_plan,	//channel plan from SW (registry/module param)
	IN	u8			def_channel_plan,	//channel plan used when the former two is invalid
	IN	BOOLEAN		AutoLoadFail
	);

u8	MRateToHwRate(u8 rate);

void	HalSetBrateCfg(
	IN PADAPTER		Adapter,
	IN u8			*mBratesOS,
	OUT u16			*pBrateCfg);

BOOLEAN
Hal_MappingOutPipe(
	IN	PADAPTER	pAdapter,
	IN	u8		NumOutPipe
	);

BOOLEAN
HAL_IsLegalChannel(
	IN	_adapter *	Adapter,
	IN	u32			Channel
	);

void hal_init_macaddr(_adapter *adapter);
void SetHwReg(PADAPTER padapter, u8 variable, u8 *val);
void GetHwReg(PADAPTER padapter, u8 variable, u8 *val);

#if defined (CONFIG_RTL8188F) ||defined (CONFIG_RTL8192E) ||defined (CONFIG_RTL8703B) ||defined (CONFIG_RTL8723B)||defined (CONFIG_RTL8723D)|| defined (CONFIG_RTL8711B) || defined (CONFIG_RTL8821C) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8710C) ||defined (CONFIG_RTL8192F)
typedef enum _RT_MEDIA_STATUS {
	RT_MEDIA_DISCONNECT = 0,
	RT_MEDIA_CONNECT       = 1
} RT_MEDIA_STATUS;


void GetHalODMVar(	
	PADAPTER				Adapter,
	HAL_ODM_VARIABLE		eVariable,
	PVOID					pValue1,
	PVOID					pValue2);

void SetHalODMVar(
	PADAPTER				Adapter,
	HAL_ODM_VARIABLE		eVariable,
	PVOID					pValue1,
	BOOLEAN					bSet);

u8 SetHalDefVar(_adapter *adapter, HAL_DEF_VARIABLE variable, void *value);
u8 GetHalDefVar(_adapter *adapter, HAL_DEF_VARIABLE variable, void *value);

#ifdef CONFIG_WOWLAN
typedef struct rtl_wow_pattern {
	u16	crc;
	u8	type;
	u32	mask[4];
} rtl_wow_pattern_t;

int rtw_hal_wow_set_pattern(_adapter *adapter, wowlan_pattern_t pattern);

void rtw_hal_wow_enable(_adapter *adapter);
void rtw_hal_wow_disable(_adapter *adapter);
#endif

void rtw_hal_set_fw_rsvd_page(_adapter* adapter, bool finished);

#ifdef CONFIG_LPS_PG
#define LPSPG_RSVD_PAGE_SET_MACID(_rsvd_pag, _value)		SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x00, 0, 8, _value)/*used macid*/
#define LPSPG_RSVD_PAGE_SET_MBSSCAMID(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x00, 8, 8, _value)/*used BSSID CAM entry*/
#define LPSPG_RSVD_PAGE_SET_PMC_NUM(_rsvd_pag, _value)		SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x00, 16, 8, _value)/*Max used Pattern Match CAM entry*/
#define LPSPG_RSVD_PAGE_SET_MU_RAID_GID(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x00, 24, 8, _value)/*Max MU rate table Group ID*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_NUM(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x04, 0, 8, _value)/*used Security CAM entry number*/
#define LPSPG_RSVD_PAGE_SET_DRV_RSVDPAGE_NUM(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x04, 8, 8, _value)/*Txbuf used page number for fw offload*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID1(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x08, 0, 8, _value)/*used Security CAM entry -1*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID2(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x08, 8, 8, _value)/*used Security CAM entry -2*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID3(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x08, 16, 8, _value)/*used Security CAM entry -3*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID4(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x08, 24, 8, _value)/*used Security CAM entry -4*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID5(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x0C, 0, 8, _value)/*used Security CAM entry -5*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID6(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x0C, 8, 8, _value)/*used Security CAM entry -6*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID7(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x0C, 16, 8, _value)/*used Security CAM entry -7*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID8(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x0C, 24, 8, _value)/*used Security CAM entry -8*/
enum lps_pg_hdl_id {
	LPS_PG_INFO_CFG = 0,
	LPS_PG_REDLEMEM,
	LPS_PG_PHYDM_DIS,
	LPS_PG_PHYDM_EN,
};

u8 rtw_hal_set_lps_pg_info(_adapter *adapter);

#endif

#endif
#endif //__HAL_COMMON_H__

