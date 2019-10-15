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

#ifndef __HAL_DATA_H__
#define __HAL_DATA_H__

#include "hal_com.h"
#if (PHYDM_LINUX_CODING_STYLE == 1)
#include "phydm/phydm_precomp.h"
#else
#include "OUTSRC/phydm_precomp.h"
#endif

#ifdef CONFIG_BT_COEXIST
#include <hal_btcoex.h>
#if !defined(CONFIG_RTL8723D) && !defined(CONFIG_RTL8821C) && !defined(CONFIG_RTL8723B) && !defined(CONFIG_RTL8703B)
#include "gpio_irq_api.h"
#endif
#endif

#include <hal_btcoex_wifionly.h>

#ifdef CONFIG_RTL8188E
#include "rtl8188e/rtl8188e_hal.h"
#endif
#ifdef CONFIG_RTL8195A
#include "rtl8195a_hal.h"
#endif

#ifdef CONFIG_RTL8711B
#include "rtl8711b_hal.h"
#endif

#ifdef CONFIG_RTL8721D
#include "rtl8721d_hal.h"
#endif

#ifdef CONFIG_RTL8188F
#include "rtl8188f/rtl8188f_hal.h"
#endif

#ifdef CONFIG_RTL8192F
#include "rtl8192f/rtl8192f_hal.h"
#endif

#ifdef CONFIG_RTL8192E
#include "rtl8192e/rtl8192e_hal.h"
#endif

#ifdef CONFIG_RTL8723B
#include "rtl8723b/rtl8723b_hal.h"
#endif

#ifdef CONFIG_RTL8703B
#include "rtl8703b/rtl8703b_hal.h"
#endif

#ifdef CONFIG_RTL8723D
#include "rtl8723d/rtl8723d_hal.h"
#endif

#ifdef CONFIG_RTL8821C
#include "rtl8821c/rtl8821c_hal.h"
#endif

#ifdef CONFIG_RTL8195B
#include "rtl8195b/rtl8195b_hal.h"
#endif

#ifdef CONFIG_RTL8710C
#include "rtl8710c/rtl8710c_hal.h"
#endif
//
// <Roger_Notes> For RTL8723 WiFi/BT/GPS multi-function configuration. 2010.10.06.
//
typedef enum _RT_MULTI_FUNC {
	RT_MULTI_FUNC_NONE = 0x00,
	RT_MULTI_FUNC_WIFI = 0x01,
	RT_MULTI_FUNC_BT = 0x02,
	RT_MULTI_FUNC_GPS = 0x04,
} RT_MULTI_FUNC,
	*PRT_MULTI_FUNC;
//
// <Roger_Notes> For RTL8723 WiFi PDn/GPIO polarity control configuration. 2010.10.08.
//
typedef enum _RT_POLARITY_CTL {
	RT_POLARITY_LOW_ACT = 0,
	RT_POLARITY_HIGH_ACT = 1,
} RT_POLARITY_CTL,
	*PRT_POLARITY_CTL;

// For RTL8723 regulator mode. by tynli. 2011.01.14.
typedef enum _RT_REGULATOR_MODE {
	RT_SWITCHING_REGULATOR = 0,
	RT_LDO_REGULATOR = 1,
} RT_REGULATOR_MODE,
	*PRT_REGULATOR_MODE;

#define CHANNEL_MAX_NUMBER 14 // 14 is the max channel number
#define CHANNEL_MAX_NUMBER_2G 14
#define CHANNEL_MAX_NUMBER_5G 54 // Please refer to "phy_GetChnlGroup8812A" and "Hal_ReadTxPowerInfo8812A"
#define CHANNEL_MAX_NUMBER_5G_80M 7

// Tx Power Limit Table Size
#define MAX_REGULATION_NUM 3	// FCC, ETSI, MKK
#define MAX_2_4G_BANDWITH_NUM 2 // 20M, 40M
#if defined(NOT_SUPPORT_RF_MULTIPATH) && defined(NOT_SUPPORT_VHT)
#define MAX_RATE_SECTION_NUM 3			  // CCk, OFDM, HT
#define MAX_BASE_NUM_IN_PHY_REG_PG_2_4G 3 //  CCK:1,OFDM:1, HT:1(MCS0_MCS7)
#else
#define MAX_RATE_SECTION_NUM 10
#define MAX_BASE_NUM_IN_PHY_REG_PG_2_4G 10 //  CCK:1,OFDM:1, HT:4, VHT:4
#endif
#define MAX_5G_BANDWITH_NUM 4
#define MAX_BASE_NUM_IN_PHY_REG_PG_5G 9 // OFDM:1, HT:4, VHT:4

#define PAGE_SIZE_128 128
#define PAGE_SIZE_256 256
#define PAGE_SIZE_512 512

#define HCI_SUS_ENTER 0
#define HCI_SUS_LEAVING 1
#define HCI_SUS_LEAVE 2
#define HCI_SUS_ENTERING 3
#define HCI_SUS_ERR 4

#ifdef RTW_RX_AGGREGATION
typedef enum _RX_AGG_MODE {
	RX_AGG_DISABLE,
	RX_AGG_DMA,
	RX_AGG_USB,
	RX_AGG_MIX
} RX_AGG_MODE;
#endif

#if (PHYDM_LINUX_CODING_STYLE == 1)
//structure
#define DM_ODM_T struct dm_struct
#define PDM_ODM_T struct dm_struct *
#define DIG_T struct phydm_dig_struct
#define pDIG_T struct phydm_dig_struct *
#define pFAT_T struct phydm_fat_struct *
#endif

typedef struct _BB_INIT_REGISTER {
	u16 offset;
	u32 value;

} BB_INIT_REGISTER, *PBB_INIT_REGISTER;

struct dm_priv {
	u32 InitODMFlag;

	//* Upper and Lower Signal threshold for Rate Adaptive*/
	int UndecoratedSmoothedPWDB;
	int UndecoratedSmoothedCCK;
	int EntryMinUndecoratedSmoothedPWDB;
	int EntryMaxUndecoratedSmoothedPWDB;
	int MinUndecoratedPWDBForDM;
	int LastMinUndecoratedPWDBForDM;

	u8 PowerIndex_backup[6];

// Add for Reading Initial Data Rate SEL Register 0x484 during watchdog. Using for fill tx desc. 2011.3.21 by Thomas
#ifndef NO_CALLER
	u8 INIDATA_RATE[32];
#endif
};

#ifdef CONFIG_LITTLE_WIFI_MCU_FUNCTION_THREAD
#define MCUCMDQUEUEDEPTH 15
#define MCUCMDLENGTH 2
#define MACIDNUM 128
typedef struct _cmd_queue_ {
	u32 FwCmdContent[MCUCMDLENGTH];
} CMD_QUEUE;
#endif

#ifdef CONFIG_RF_GAIN_OFFSET
#ifdef CONFIG_RTL8188F
struct kfree_data_t {
	u8 flag;
	s8 bb_gain[BB_GAIN_NUM][RF_PATH_MAX];
	s8 thermal;
};

#define KFREE_FLAG_ON BIT0
#define KFREE_FLAG_THERMAL_K_ON BIT1
#endif
#endif

#if (PHYDM_LINUX_CODING_STYLE == 1)
struct hal_spec_t {
	char *ic_name;
	u8 macid_num;

	u8 sec_cam_ent_num;
	u8 sec_cap;

	u8 rfpath_num_2g:4; /* used for tx power index path */
	u8 rfpath_num_5g:4; /* used for tx power index path */
	u8 txgi_max;		  /* maximum tx power gain index */
	u8 txgi_pdbm;		  /* tx power gain index per dBm */

	u8 max_tx_cnt;
	u8 tx_nss_num:4;
	u8 rx_nss_num:4;
	u8 band_cap; /* value of BAND_CAP_XXX */
	u8 bw_cap;   /* value of BW_CAP_XXX */
	u8 port_num;
	u8 proto_cap; /* value of PROTO_CAP_XXX */
	u8 wl_func;   /* value of WL_FUNC_XXX */

	u8 rx_tsf_filter:1;

	u8 pg_txpwr_saddr;		/* starting address of PG tx power info */
	u8 pg_txgi_diff_factor; /* PG tx power gain index diff to tx power gain index */

	u8 hci_type; /* value of HCI Type */
};
#endif

#ifdef CONFIG_PHY_CAPABILITY_QUERY
struct phy_spec_t {
	u32 trx_cap;
	u32 stbc_cap;
	u32 ldpc_cap;
	u32 txbf_param;
	u32 txbf_cap;
};
#endif

typedef struct hal_com_data {
	HAL_VERSION VersionID;
	//	RT_MULTI_FUNC		MultiFunc; // For multi-function consideration.
	//	RT_POLARITY_CTL		PolarityCtl; // For Wifi PDn Polarity control.
	RT_REGULATOR_MODE RegulatorMode; // switching regulator or LDO
	u16 CustomerID;
	u16 ForcedDataRate; // Force Data Rate. 0: Auto, 0x02: 1M ~ 0x6C: 54M.

	u16 FirmwareVersion;
	u16 FirmwareVersionRev;
	u16 FirmwareSubVersion;
	u16 FirmwareSignature;
	//current WIFI_PHY values
	u32 ReceiveConfig;
	//	WIRELESS_MODE		CurrentWirelessMode;
	enum channel_width CurrentChannelBW;
	BAND_TYPE CurrentBandType; //0:2.4G, 1:5G

	u8 CurrentChannel;
	u8 nCur40MhzPrimeSC; // Control channel sub-carrier
	u8 CurrentCenterFrequencyIndex1;
#if !defined(NOT_SUPPORT_80M)
	u8 nCur80MhzPrimeSC; //used for primary 40MHz of 80MHz mode
#endif

	u16 BasicRateSet;
	u8 hci_sus_state;
#ifdef RTW_IQK_FW_OFFLOAD
	u8 RegIQKFWOffload;
#endif
#if defined(CONFIG_RTL8821C) || defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8710C)
	u8 rx_tsf_addr_filter_config; /* for 8822B/8821C USE */
#endif
#ifdef CONFIG_PHY_CAPABILITY_QUERY
	struct phy_spec_t phy_spec;
#endif
	//rf_ctrl
	u8 rf_chip;
	u8 rf_type;
	u8 PackageType;
#if defined(CONFIG_PLATFORM_8195A) || defined(CONFIG_PLATFORM_8711B) || defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_RTL8188F) || defined(CONFIG_RTL8192F) || defined(CONFIG_RTL8192E) || defined(CONFIG_RTL8723D) || defined(CONFIG_RTL8723B) || defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_RTL8703B)
	u8 ChipID;
#endif
	u8 NumTotalRFPath;

	u8 BoardType;

	u8 EEPROMCustomerID;
	u8 EEPROMVersion;
	u8 EEPROMRegulatory;

#if defined(CONFIG_RTL8821C) || defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8710C)
	u8 tx_bbswing_24G;
	u8 tx_bbswing_5G;
#endif
#ifdef CONFIG_RF_GAIN_OFFSET
#ifdef CONFIG_RTL8188F
	struct kfree_data_t kfree_data;
#endif
#ifdef CONFIG_RTL8195B
	u32 rf_offset_table_flag;
	u16 rf_offset_addr;
	u8 rf_offset_5g_rx_lna;
	s8 rf_offset_5g_rx_lna1;
	s8 rf_offset_5g_rx_lna2;
#endif
#endif
#if (PHYDM_LINUX_CODING_STYLE == 1)
	u8 rf_power_tracking_type;
	u8 eeprom_thermal_meter;
	u8 crystal_cap;
	u32 firmware_size;
	u16 firmware_version;
	u16 firmware_sub_version;
#else
	u8 EEPROMThermalMeter;
#endif

#if defined(CONFIG_RTL8723B) || defined(CONFIG_RTL8703B) || defined(CONFIG_RTL8723D) || defined(CONFIG_RTL8192F)
	u8 adjuseVoltageVal;
	u8 need_restore;
	BB_INIT_REGISTER RegForRecover[5];
#endif

#if defined(CONFIG_RTL8821C) || defined(CONFIG_RTL8192F) || defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8710C)
	u8 EfuseUsedPercentage;
#endif
#ifdef HAL_EFUSE_MEMORY
	EFUSE_HAL EfuseHal;
#endif

	u8 Regulation2_4G;
#if !defined(NOT_SUPPORT_5G)
	u8 Regulation5G;
#endif

	s8 TxPwrByRateOffset[TX_PWR_BY_RATE_NUM_BAND]
						[TX_PWR_BY_RATE_NUM_RF]
						[TX_PWR_BY_RATE_NUM_RF]
						[TX_PWR_BY_RATE_NUM_RATE];

	u8 Index24G_CCK_Base[MAX_RF_PATH][CHANNEL_MAX_NUMBER];
	u8 Index24G_BW40_Base[MAX_RF_PATH][CHANNEL_MAX_NUMBER];
	//If only one tx, only BW20 and OFDM are used.
	s8 OFDM_24G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
	s8 BW20_24G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#if !defined(NOT_SUPPORT_RF_MULTIPATH)
	s8 CCK_24G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
	s8 BW40_24G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#endif

/* 5G TX power info for target TX power*/
#if !defined(NOT_SUPPORT_5G)
	u8 Index5G_BW40_Base[MAX_RF_PATH][CENTER_CH_5G_ALL_NUM];
#if !defined(NOT_SUPPORT_80M)
	u8 Index5G_BW80_Base[MAX_RF_PATH][CENTER_CH_5G_80M_NUM];
#endif
	s8 OFDM_5G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
	s8 BW20_5G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
	s8 BW40_5G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#if !defined(NOT_SUPPORT_80M)
	s8 BW80_5G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#endif

#endif

	s8 TxPwrLimit_2_4G[MAX_REGULATION_NUM][MAX_2_4G_BANDWITH_NUM][MAX_RATE_SECTION_NUM][CHANNEL_MAX_NUMBER_2G][MAX_RF_PATH];
#if !defined(NOT_SUPPORT_5G)
	s8 TxPwrLimit_5G[MAX_REGULATION_NUM]
					[MAX_5G_BANDWITH_NUM]
					[MAX_RATE_SECTION_NUM]
					[CHANNEL_MAX_NUMBER_5G]
					[MAX_RF_PATH];
#endif
	// Store the original power by rate value of the base of each rate section of rf path A & B
	u8 TxPwrByRateBase2_4G[TX_PWR_BY_RATE_NUM_RF]
						  [TX_PWR_BY_RATE_NUM_RF]
						  [MAX_BASE_NUM_IN_PHY_REG_PG_2_4G];
#if !defined(NOT_SUPPORT_5G)
	u8 TxPwrByRateBase5G[TX_PWR_BY_RATE_NUM_RF][TX_PWR_BY_RATE_NUM_RF][MAX_BASE_NUM_IN_PHY_REG_PG_5G];
#endif

#if (RTL8188E_SUPPORT == 1)
	u8 TxPwrLevelCck[MAX_RF_PATH][CHANNEL_MAX_NUMBER];
	u8 TxPwrLevelHT40_1S[MAX_RF_PATH][CHANNEL_MAX_NUMBER]; // For HT 40MHZ pwr
	u8 TxPwrLevelHT40_2S[MAX_RF_PATH][CHANNEL_MAX_NUMBER]; // For HT 40MHZ pwr
	u8 TxPwrHt20Diff[MAX_RF_PATH][CHANNEL_MAX_NUMBER];	 // HT 20<->40 Pwr diff
	u8 TxPwrLegacyHtDiff[MAX_RF_PATH][CHANNEL_MAX_NUMBER]; // For HT<->legacy pwr diff
#endif

	// The current Tx Power Level
	u8 CurrentCckTxPwrIdx;
	u8 CurrentOfdm24GTxPwrIdx;
	u8 CurrentBW2024GTxPwrIdx;
	u8 CurrentBW4024GTxPwrIdx;

	u8 CrystalCap;

#if defined(CONFIG_RTL8188F) || defined(CONFIG_RTL8723B) || defined(CONFIG_RTL8703B) || defined(CONFIG_RTL8723D) || defined(CONFIG_RTL8188E)
	/* PHY DM & DM Section */
	u8 INIDATA_RATE[32 /*MACID_NUM_SW_LIMIT*/];
	/* Upper and Lower Signal threshold for Rate Adaptive*/
	int EntryMinUndecoratedSmoothedPWDB;
	int EntryMaxUndecoratedSmoothedPWDB;
	int MinUndecoratedPWDBForDM;
#endif

#if defined(CONFIG_RTL8821C) || defined(CONFIG_RTL8192F) || defined(CONFIG_RTL8192E) || defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8710C)
	u8 u1ForcedIgiLb;
	u8 PAType_2G;
	u8 LNAType_2G;
	u8 external_pa_2g;
	u8 external_lna_2g;
#if !defined(NOT_SUPPORT_5G)
	u8 PAType_5G;
	u8 LNAType_5G;
	u8 external_pa_5g;
	u8 external_lna_5g;
#endif
	u8 TypeGLNA;
	u8 TypeGPA;
	u8 TypeALNA;
	u8 TypeAPA;
	u8 rfe_type;
	u8 INIDATA_RATE[32 /*MACID_NUM_SW_LIMIT*/];
#endif
	BOOLEAN bSwChnl;
	BOOLEAN bSetChnlBW;
	BOOLEAN bChnlBWInitialized;

#if defined(CONFIG_RTL8821C) || defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8710C)
	u8 bDumpRxPkt; //for debug
	u8 bDumpTxPkt; //for debug
	u8 bDisableTXPowerTraining;
	/* PHY DM & DM Section */
	_lock IQKSpinLock;
#endif

#if (PHYDM_LINUX_CODING_STYLE == 1)
	u8 bIQKInitialized;
	u8 bNeedIQK;
	u8 neediqk_24g; // xhl modify
#endif
	u32 AcParam_BE; //Original parameter for BE, use for EDCA turbo.
#if defined(NOT_SUPPORT_RF_MULTIPATH)
	BB_REGISTER_DEFINITION_T PHYRegDef[1]; //Radio A
	u32 RfRegChnlVal[1];
#else
	BB_REGISTER_DEFINITION_T PHYRegDef[4]; //Radio A/B/C/D
	u32 RfRegChnlVal[2];
#endif

//RDG enable
#if (defined(CONFIG_RTL8711B) || defined(CONFIG_RTL8188F) || defined(CONFIG_RTL8192E) || defined(CONFIG_RTL8723B) || defined(CONFIG_RTL8703B) || defined(CONFIG_RTL8723D) || defined(CONFIG_RTL8821C)) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8188E) || defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8192F)
	//for host message to fw
	u8 LastHMEBoxNum;
#endif

	u8 fw_ractrl;
	u8 RegFwHwTxQCtrl;
	u8 RegReg542;
	u16 RegRRSR;

	struct dm_priv dmpriv;
	DM_ODM_T odmpriv;
//_lock			odm_stainfo_lock;
#ifdef DBG_CONFIG_ERROR_DETECT
	struct sreset_priv srestpriv;
#endif

#ifdef CONFIG_BT_COEXIST
	u8 ant_path; /* for 8723B s0/s1 selection	 */
	u8 EEPROMBluetoothCoexist;
	u8 EEPROMBluetoothType;
	u8 EEPROMBluetoothAntNum;
	BT_COEXIST bt_coexist;
#if defined(CONFIG_RTL8723D) || defined(CONFIG_RTL8723B) || defined(CONFIG_RTL8703B)
	u16 rfe_type;
#endif
	/* Upper and Lower Signal threshold for Rate Adaptive*/
	int entry_min_undecorated_smoothed_pwdb;
#endif

	u8 CurAntenna;
	u8 AntDivCfg;
	u8 TRxAntDivType;

	// 2010/08/09 MH Add CU power down mode.
	BOOLEAN pwrdown;

	u8 OutEpQueueSel;
	u8 OutEpNumber;
	u16 EfuseUsedBytes;

#ifdef CONFIG_P2P
	struct P2P_PS_Offload_t p2p_ps_offload;
#endif

	u8 AMPDUDensity;

	// Auto FSM to Turn On, include clock, isolation, power control for MAC only
	u8 bMacPwrCtrlOn;

#ifdef RTW_RX_AGGREGATION
	RX_AGG_MODE rxagg_mode;

	/* For RX Aggregation DMA Mode */
	u8 rxagg_dma_size;
	u8 rxagg_dma_timeout;
#endif /* RTW_RX_AGGREGATION */

#if defined(CONFIG_SDIO_HCI) || defined(CONFIG_GSPI_HCI)

//
// For SDIO Interface HAL related
//
#ifdef CONFIG_WOWLAN
	u32 sdio_himr;
	u32 sdio_hisr;
#endif
#ifdef RTW_HALMAC
#ifdef CONFIG_RTL8821C
	u16 tx_high_page;
	u16 tx_low_page;
	u16 tx_normal_page;
	u16 tx_extra_page;
	u16 tx_pub_page;
	u16 max_oqt_page;
	u32 max_xmit_size_vovi;
	u32 max_xmit_size_bebk;
#endif
#endif

	//
	// SDIO Tx FIFO related.
	//
	// HIQ, MID, LOW, PUB free pages; padapter->xmitpriv.free_txpg
	//	u8			SdioTxFIFOFreePage[TX_FREE_PG_QUEUE];
	//	_lock		SdioTxFIFOFreePageLock;
	//	_thread_hdl_	SdioXmitThread;
	//	_sema		SdioXmitSema;
	//	_sema		SdioXmitTerminateSema;

	//
	// SDIO Rx FIFO related.
	//
	u8 SdioRxFIFOCnt;
#endif //CONFIG_SDIO_HCI

#ifdef CONFIG_USB_HCI
	BOOLEAN UsbRxHighSpeedMode;
	u32 UsbBulkOutSize;
	u8 usb_intf_start;

	// Interrupt relatd register information.
	u32 IntArray[3]; //HISR0,HISR1,HSISR
	u32 IntrMask[3];
#ifdef CONFIG_USB_TX_AGGREGATION
	u8 UsbTxAggMode;
	u8 UsbTxAggDescNum;
#endif
#ifdef CONFIG_USB_RX_AGGREGATION
	u16 HwRxPageSize; // Hardware setting

	/* For RX Aggregation USB Mode */
	u8 rxagg_usb_size;
	u8 rxagg_usb_timeout;
#endif
#endif //CONFIG_USB_HCI

#if defined(CONFIG_PCI_HCI) || defined(CONFIG_LX_HCI) || defined(CONFIG_AXI_HCI)
#if defined(CONFIG_AXI_HCI)
	u32 IntArray[4];
	u32 IntrMask[4];
#else
	u32 IntArray[3];
	u32 IntrMask[3];
#endif
#ifdef CONFIG_SUPPORT_HW_WPS_PBC
	u8 bGpioHwWpsPbc;
#endif
	u16 RxExpectTag;
#ifdef CONFIG_DEBUG_DYNAMIC
	struct hal_debug debug_info;
#endif

#endif //CONFIG_PCI_HCI || CONFIG_LX_HCI || defined(CONFIG_AXI_HCI)

#if (PHYDM_LINUX_CODING_STYLE == 1)
	struct hal_spec_t hal_spec;
#endif

#ifdef CONFIG_MP_INCLUDED
	BOOLEAN bCCKinCH14;
#endif

#ifdef CONFIG_TX_EARLY_MODE
	u8 bEarlyModeEnable;
#endif

#ifdef CONFIG_LITTLE_WIFI_MCU_FUNCTION_THREAD
	struct task_struct littlewifipriv;
	//CMD_QUEUE           FwCmdQueue[MCUCMDQUEUEDEPTH];
	//fw section
	u32 WifiMcuCmdBitMap;
	u8 bConnected[MACIDNUM / 8];
	BOOLEAN PMUTaskRAEn;

	u8 BcnIgnoreEdccaEn;
	u8 BtCoexInitFlag;

#ifdef CONFIG_POWER_SAVING
	struct task_struct enter32kpriv;

#ifdef TDMA_POWER_SAVING
	struct task_struct TDMApriv;
#endif //#ifdef TDMA_POWER_SAVING

	PS_PARM PSParmpriv;
	u8 ScanEn;
#endif //#ifdef CONFIG_POWER_SAVING

#ifdef CONFIG_BT_MAILBOX
	BtCoex_info BtCoex;
	C2H_INFO C2hInfo;
	struct task_struct C2Hpriv;
	gpio_irq_t BtbxGpioBtn;
#endif //#ifdef CONFIG_BT_MAILBOX
#endif

#ifdef CONFIG_SW_MAILBOX_EN
	_queue MailboxQueue;
	struct task_struct Mailboxpriv;
#endif //#ifdef CONFIG_SW_MAILBOX_EN

#ifdef RTW_HALMAC
	u8 drv_rsvd_page_number;
	u8 not_xmitframe_fw_dl; /*not use xmitframe to download fw*/
#endif

#if (PHYDM_LINUX_CODING_STYLE == 1)
	u8 phydm_op_mode;
	u32 prv_traffic_idx;
	bool is_turbo_edca;
#endif

} HAL_DATA_COMMON, *PHAL_DATA_COMMON;

#if (PHYDM_LINUX_CODING_STYLE == 1)
#define TXPWR_LMT_RS_CCK 0
#define TXPWR_LMT_RS_OFDM 1
#define TXPWR_LMT_RS_HT 2
#define TXPWR_LMT_RS_VHT 3

//register definition
#define KFREE_FLAG_ON BIT(0)
#define KFREE_FLAG_THERMAL_K_ON BIT(1)

#define REG_BLUE_TOOTH rBlue_Tooth
#define REG_CCK_0_AFE_SETTING rCCK0_AFESetting
#define REG_CONFIG_ANT_A rConfig_AntA
#define REG_CONFIG_ANT_B rConfig_AntB

#define REG_FPGA0_IQK rFPGA0_IQK
#define REG_FPGA0_RFMOD rFPGA0_RFMOD
#define REG_FPGA0_TX_GAIN_STAGE rFPGA0_TxGainStage
#define REG_FPGA0_XA_RF_INTERFACE_OE rFPGA0_XA_RFInterfaceOE
#define REG_FPGA0_XB_RF_INTERFACE_OE rFPGA0_XB_RFInterfaceOE
#define REG_FPGA0_XA_HSSI_PARAMETER1 rFPGA0_XA_HSSIParameter1
#define REG_FPGA0_XA_LSSI_PARAMETER rFPGA0_XA_LSSIParameter
#define REG_FPGA0_XB_HSSI_PARAMETER1 rFPGA0_XB_HSSIParameter1
#define REG_FPGA0_XB_LSSI_PARAMETER rFPGA0_XB_LSSIParameter
#define REG_FPGA0_XB_LSSI_READ_BACK rFPGA0_XB_LSSIReadBack
#define REG_FPGA0_XAB_RF_INTERFACE_SW rFPGA0_XAB_RFInterfaceSW
#define REG_FPGA0_XAB_RF_PARAMETER rFPGA0_XAB_RFParameter
#define REG_FPGA0_XCD_RF_INTERFACE_SW rFPGA0_XCD_RFInterfaceSW
#define REG_FPGA0_XCD_SWITCH_CONTROL rFPGA0_XCD_SwitchControl

#define REG_IQK_AGC_RSP rIQK_AGC_Rsp
#define REG_IQK_AGC_PTS rIQK_AGC_Pts
#define REG_IQK_AGC_CONT rIQK_AGC_Cont

#define REG_OFDM_0_AGC_RSSI_TABLE rOFDM0_AGCRSSITable
#define REG_OFDM_0_ECCA_THRESHOLD rOFDM0_ECCAThreshold
#define REG_OFDM_0_RX_IQ_EXT_ANTA rOFDM0_RxIQExtAnta
#define REG_OFDM_0_TRX_PATH_ENABLE rOFDM0_TRxPathEnable
#define REG_OFDM_0_TR_MUX_PAR rOFDM0_TRMuxPar
#define REG_OFDM_0_XA_AGC_CORE1 rOFDM0_XAAGCCore1
#define REG_OFDM_0_XB_AGC_CORE1 rOFDM0_XBAGCCore1
#define REG_OFDM_0_XA_RX_IQ_IMBALANCE rOFDM0_XARxIQImbalance
#define REG_OFDM_0_XB_RX_IQ_IMBALANCE rOFDM0_XBRxIQImbalance
#define REG_OFDM_0_XA_TX_IQ_IMBALANCE rOFDM0_XATxIQImbalance
#define REG_OFDM_0_XB_TX_IQ_IMBALANCE rOFDM0_XBTxIQImbalance
#define REG_OFDM_0_XC_TX_AFE rOFDM0_XCTxAFE
#define REG_OFDM_0_XD_TX_AFE rOFDM0_XDTxAFE

#define REG_TX_AGC_A_CCK_11_CCK_1_JAGUAR rTxAGC_A_CCK11_CCK1_JAguar
#define REG_TX_AGC_A_CCK_1_MCS32 rTxAGC_A_CCK1_Mcs32
#define REG_TX_AGC_A_MCS11_MCS8_JAGUAR rTxAGC_A_MCS11_MCS8_JAguar
#define REG_TX_AGC_A_MCS15_MCS12_JAGUAR rTxAGC_A_MCS15_MCS12_JAguar
#define REG_TX_AGC_A_MCS19_MCS16_JAGUAR rTxAGC_A_MCS19_MCS16_JAguar
#define REG_TX_AGC_A_MCS23_MCS20_JAGUAR rTxAGC_A_MCS23_MCS20_JAguar
#define REG_TX_AGC_A_MCS3_MCS0_JAGUAR rTxAGC_A_MCS3_MCS0_JAguar
#define REG_TX_AGC_A_MCS7_MCS4_JAGUAR rTxAGC_A_MCS7_MCS4_JAguar
#define REG_TX_AGC_A_MCS03_MCS00 rTxAGC_A_Mcs03_Mcs00
#define REG_TX_AGC_A_MCS07_MCS04 rTxAGC_A_Mcs07_Mcs04
#define REG_TX_AGC_A_MCS11_MCS08 rTxAGC_A_Mcs11_Mcs08
#define REG_TX_AGC_A_MCS15_MCS12 rTxAGC_A_Mcs15_Mcs12
#define REG_TX_AGC_A_NSS1_INDEX3_NSS1_INDEX0_JAGUAR rTxAGC_A_Nss1Index3_Nss1Index0_JAguar
#define REG_TX_AGC_A_NSS1_INDEX7_NSS1_INDEX4_JAGUAR rTxAGC_A_Nss1Index7_Nss1Index4_JAguar
#define REG_TX_AGC_A_NSS2_INDEX1_NSS1_INDEX8_JAGUAR rTxAGC_A_Nss2Index1_Nss1Index8_JAguar
#define REG_TX_AGC_A_NSS2_INDEX5_NSS2_INDEX2_JAGUAR rTxAGC_A_Nss2Index5_Nss2Index2_JAguar
#define REG_TX_AGC_A_NSS2_INDEX9_NSS2_INDEX6_JAGUAR rTxAGC_A_Nss2Index9_Nss2Index6_JAguar
#define REG_TX_AGC_A_NSS3_INDEX3_NSS3_INDEX0_JAGUAR rTxAGC_A_Nss3Index3_Nss3Index0_JAguar
#define REG_TX_AGC_A_NSS3_INDEX7_NSS3_INDEX4_JAGUAR rTxAGC_A_Nss3Index7_Nss3Index4_JAguar
#define REG_TX_AGC_A_NSS3_INDEX9_NSS3_INDEX8_JAGUAR rTxAGC_A_Nss3Index9_Nss3Index8_JAguar
#define REG_TX_AGC_A_OFDM18_OFDM6_JAGUAR rTxAGC_A_Ofdm18_Ofdm6_JAguar
#define REG_TX_AGC_A_OFDM54_OFDM24_JAGUAR rTxAGC_A_Ofdm54_Ofdm24_JAguar
#define REG_TX_AGC_A_RATE18_06 rTxAGC_A_Rate18_06
#define REG_TX_AGC_A_RATE54_24 rTxAGC_A_Rate54_24
#define REG_TX_AGC_B_CCK_11_A_CCK_2_11 rTxAGC_B_CCK11_A_CCK2_11
#define REG_TX_AGC_B_CCK_11_CCK_1_JAGUAR rTxAGC_B_CCK11_CCK1_JAguar
#define REG_TX_AGC_B_CCK_1_55_MCS32 rTxAGC_B_CCK1_55_Mcs32
#define REG_TX_AGC_B_MCS11_MCS8_JAGUAR rTxAGC_B_MCS11_MCS8_JAguar
#define REG_TX_AGC_B_MCS15_MCS12_JAGUAR rTxAGC_B_MCS15_MCS12_JAguar
#define REG_TX_AGC_B_MCS19_MCS16_JAGUAR rTxAGC_B_MCS19_MCS16_JAguar
#define REG_TX_AGC_B_MCS23_MCS20_JAGUAR rTxAGC_B_MCS23_MCS20_JAguar
#define REG_TX_AGC_B_MCS3_MCS0_JAGUAR rTxAGC_B_MCS3_MCS0_JAguar
#define REG_TX_AGC_B_MCS7_MCS4_JAGUAR rTxAGC_B_MCS7_MCS4_JAguar
#define REG_TX_AGC_B_MCS03_MCS00 rTxAGC_B_Mcs03_Mcs00
#define REG_TX_AGC_B_MCS07_MCS04 rTxAGC_B_Mcs07_Mcs04
#define REG_TX_AGC_B_MCS11_MCS08 rTxAGC_B_Mcs11_Mcs08
#define REG_TX_AGC_B_MCS15_MCS12 rTxAGC_B_Mcs15_Mcs12
#define REG_TX_AGC_B_NSS1_INDEX3_NSS1_INDEX0_JAGUAR rTxAGC_B_Nss1Index3_Nss1Index0_JAguar
#define REG_TX_AGC_B_NSS1_INDEX7_NSS1_INDEX4_JAGUAR rTxAGC_B_Nss1Index7_Nss1Index4_JAguar
#define REG_TX_AGC_B_NSS2_INDEX1_NSS1_INDEX8_JAGUAR rTxAGC_B_Nss2Index1_Nss1Index8_JAguar
#define REG_TX_AGC_B_NSS2_INDEX5_NSS2_INDEX2_JAGUAR rTxAGC_B_Nss2Index5_Nss2Index2_JAguar
#define REG_TX_AGC_B_NSS2_INDEX9_NSS2_INDEX6_JAGUAR rTxAGC_B_Nss2Index9_Nss2Index6_JAguar
#define REG_TX_AGC_B_NSS3_INDEX3_NSS3_INDEX0_JAGUAR rTxAGC_B_Nss3Index3_Nss3Index0_JAguar
#define REG_TX_AGC_B_NSS3_INDEX7_NSS3_INDEX4_JAGUAR rTxAGC_B_Nss3Index7_Nss3Index4_JAguar
#define REG_TX_AGC_B_NSS3_INDEX9_NSS3_INDEX8_JAGUAR rTxAGC_B_Nss3Index9_Nss3Index8_JAguar
#define REG_TX_AGC_B_OFDM18_OFDM6_JAGUAR rTxAGC_B_Ofdm18_Ofdm6_JAguar
#define REG_TX_AGC_B_OFDM54_OFDM24_JAGUAR rTxAGC_B_Ofdm54_Ofdm24_JAguar
#define REG_TX_AGC_B_RATE18_06 rTxAGC_B_Rate18_06
#define REG_TX_AGC_B_RATE54_24 rTxAGC_B_Rate54_24

#define REG_TX_CCK_BBON rTx_CCK_BBON
#define REG_TX_CCK_RFON rTx_CCK_RFON
#define REG_TX_OFDM_BBON rTx_OFDM_BBON
#define REG_TX_OFDM_RFON rTx_OFDM_RFON
#define REG_TX_IQK rTx_IQK
#define REG_TX_IQK_TONE_A rTx_IQK_Tone_A
#define REG_TX_IQK_TONE_B rTx_IQK_Tone_B
#define REG_TX_IQK_PI_A rTx_IQK_PI_A
#define REG_TX_IQK_PI_B rTx_IQK_PI_B
#define REG_TX_POWER_AFTER_IQK_A rTx_Power_After_IQK_A
#define REG_TX_POWER_AFTER_IQK_B rTx_Power_After_IQK_B
#define REG_TX_POWER_BEFORE_IQK_A rTx_Power_Before_IQK_A
#define REG_TX_POWER_BEFORE_IQK_B rTx_Power_Before_IQK_B
#define REG_TX_TO_RX rTx_To_Rx
#define REG_TX_TO_TX rTx_To_Tx

#define REG_RX_CCK rRx_CCK
#define REG_RX_IQK rRx_IQK
#define REG_RX_IQK_TONE_A rRx_IQK_Tone_A
#define REG_RX_IQK_TONE_B rRx_IQK_Tone_B
#define REG_RX_IQK_PI_A rRx_IQK_PI_A
#define REG_RX_IQK_PI_B rRx_IQK_PI_B
#define REG_RX_OFDM rRx_OFDM
#define REG_RX_POWER_AFTER_IQK_A_2 rRx_Power_After_IQK_A_2
#define REG_RX_POWER_AFTER_IQK_B_2 rRx_Power_After_IQK_B_2
#define REG_RX_POWER_BEFORE_IQK_A_2 rRx_Power_Before_IQK_A_2
#define REG_RX_POWER_BEFORE_IQK_B_2 rRx_Power_Before_IQK_B_2
#define REG_RX_TO_RX rRx_TO_Rx
#define REG_RX_WAIT_CCA rRx_Wait_CCA
#define REG_RX_WAIT_RIFS rRx_Wait_RIFS

#define REG_TX_IQK_TONE_B rTx_IQK_Tone_B
#define REG_TX_IQK_PI_A rTx_IQK_PI_A

#define REG_PMPD_ANAEN rPMPD_ANAEN
#define REG_SLEEP rSleep
#define REG_STANDBY rStandby

#define RX_SMOOTH_FACTOR Rx_Smooth_Factor

//function redefine
#define hw_rate_to_m_rate HwRateToMRate
#define phy_set_tx_power_index_by_rate_section PHY_SetTxPowerIndexByRateSection
#define phy_store_tx_power_by_rate PHY_StoreTxPowerByRate
#define _rtw_memset rtw_memset
#define _rtw_memcpy rtw_memcpy
#define _rtw_memcmp rtw_memcmp
#define hal_func HalFunc
#define set_hw_reg_handler SetHwRegHandler
#define get_hal_def_var_handler GetHalDefVarHandler
#endif // #if (PHYDM_LINUX_CODING_STYLE == 1)

#if defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8710C)
#define ODM_COMP_COMMON BIT(30)

typedef struct _ROM_INFO {
	u1Byte EEPROMVersion;
	u1Byte CrystalCap;
	u8Byte DebugComponents;
	u4Byte DebugLevel;
} ROM_INFO, *PROM_INFO;
extern ROM_INFO ROMInfo;

#endif // #if defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8710C)

typedef struct hal_com_data HAL_DATA_TYPE, *PHAL_DATA_TYPE;
#define GET_HAL_DATA(__pAdapter) ((HAL_DATA_TYPE *)((__pAdapter)->HalData))
#define GET_HAL_SPEC(__pAdapter) (&(GET_HAL_DATA((__pAdapter))->hal_spec))
#define rtw_is_hw_init_completed(adapter) (((PADAPTER)adapter)->hw_init_completed == _TRUE)

/* alias for phydm coding style */
#define REG_A_TX_SCALE_JAGUAR rA_TxScale_Jaguar

#define RX_SMOOTH_FACTOR Rx_Smooth_Factor

#endif //__HAL_DATA_H__
