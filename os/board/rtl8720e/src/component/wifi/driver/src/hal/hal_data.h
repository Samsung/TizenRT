#ifndef __HAL_DATA_H__
#define __HAL_DATA_H__


#include "hal_com.h"
#if (PHYDM_VERSION == 2)
#include "phydm/phydm_precomp.h"
#elif (PHYDM_VERSION == 1)
#include "OUTSRC/phydm_precomp.h"
#endif

#ifdef CONFIG_BT_COEXIST
#include <hal_btcoex.h>
#if !defined(CONFIG_RTL8723D)
#include "gpio_irq_api.h"
#endif
#endif

#include <hal_btcoex_wifionly.h>

#ifdef CONFIG_RTL8188E
#include "rtl8188e/rtl8188e_hal.h"
#endif

#ifdef CONFIG_RTL8711B
#include "rtl8711b_hal.h"
#endif

#ifdef CONFIG_RTL8721D
#include "rtl8721d_hal.h"
#endif

#ifdef CONFIG_RTL8730A
#include "rtl8730a_hal.h"
#endif

#ifdef CONFIG_RTL8730E
#include "rtl8730e_hal.h"
#endif

#ifdef CONFIG_RTL8720E
#include "rtl8720e_hal.h"
#endif

#ifdef CONFIG_RTL8188F
#include "rtl8188f/rtl8188f_hal.h"
#endif

#ifdef CONFIG_RTL8192E
#include "rtl8192e/rtl8192e_hal.h"
#endif
#ifdef CONFIG_RTL8723D
#include "rtl8723d/rtl8723d_hal.h"
#endif

#ifdef CONFIG_RTL8195B
#include "rtl8195b/rtl8195b_hal.h"
#endif

#ifdef CONFIG_RTL8710C
#include "rtl8710c/rtl8710c_hal.h"
#endif

#ifdef CONFIG_RTL8735B
#include "rtl8735b/rtl8735b_hal.h"
#endif
//
// <Roger_Notes> For RTL8723 WiFi/BT/GPS multi-function configuration. 2010.10.06.
//
typedef enum _RT_MULTI_FUNC {
	RT_MULTI_FUNC_NONE	= 0x00,
	RT_MULTI_FUNC_WIFI 	= 0x01,
	RT_MULTI_FUNC_BT 		= 0x02,
	RT_MULTI_FUNC_GPS 	= 0x04,
} RT_MULTI_FUNC, *PRT_MULTI_FUNC;
//
// <Roger_Notes> For RTL8723 WiFi PDn/GPIO polarity control configuration. 2010.10.08.
//
typedef enum _RT_POLARITY_CTL {
	RT_POLARITY_LOW_ACT 	= 0,
	RT_POLARITY_HIGH_ACT 	= 1,
} RT_POLARITY_CTL, *PRT_POLARITY_CTL;

// For RTL8723 regulator mode. by tynli. 2011.01.14.
typedef enum _RT_REGULATOR_MODE {
	RT_SWITCHING_REGULATOR 	= 0,
	RT_LDO_REGULATOR 			= 1,
} RT_REGULATOR_MODE, *PRT_REGULATOR_MODE;

#define CHANNEL_MAX_NUMBER						14	// 14 is the max channel number
#define CHANNEL_MAX_NUMBER_2G					14
#define CHANNEL_MAX_NUMBER_5G					54	// Please refer to "phy_GetChnlGroup8812A" and "Hal_ReadTxPowerInfo8812A"
#define CHANNEL_MAX_NUMBER_5G_80M				7

// Tx Power Limit Table Size
#define MAX_REGULATION_NUM						(TXPWR_LMT_MAX_REGULATION_NUM - 1)       /* ignore NCC */
#define MAX_2_4G_BANDWITH_NUM					2 // 20M, 40M
#if defined(NOT_SUPPORT_RF_MULTIPATH) && (!defined(CONFIG_80211AC_VHT))
#define MAX_RATE_SECTION_NUM					3 // CCk, OFDM, HT
#define MAX_BASE_NUM_IN_PHY_REG_PG_2_4G			3 //  CCK:1,OFDM:1, HT:1(MCS0_MCS7)
#else
#define MAX_RATE_SECTION_NUM					10
#define MAX_BASE_NUM_IN_PHY_REG_PG_2_4G			10 //  CCK:1,OFDM:1, HT:4, VHT:4
#endif
#define MAX_5G_BANDWITH_NUM						4
#define MAX_BASE_NUM_IN_PHY_REG_PG_5G			9 // OFDM:1, HT:4, VHT:4

#define PAGE_SIZE_128	128
#define PAGE_SIZE_256	256
#define PAGE_SIZE_512	512

#define HCI_SUS_ENTER		0
#define HCI_SUS_LEAVING		1
#define HCI_SUS_LEAVE		2
#define HCI_SUS_ENTERING	3
#define HCI_SUS_ERR			4

#ifdef CONFIG_MCC_IQK_OFFLOAD
#define MAX_IQK_INFO_BACKUP_CHNL_NUM	5
#define MAX_IQK_INFO_BACKUP_REG_NUM		10
#endif

#if (PHYDM_VERSION == 2)
//structure
#define DM_ODM_T struct dm_struct
#define PDM_ODM_T struct dm_struct*
#define DIG_T struct phydm_dig_struct
#define pDIG_T struct phydm_dig_struct*
#define pFAT_T struct phydm_fat_struct*
#endif

typedef struct _BB_INIT_REGISTER {
	u16	offset;
	u32	value;

} BB_INIT_REGISTER, *PBB_INIT_REGISTER;

struct 	dm_priv {
//	u8	DM_Type;
//	u8	DMFlag;
//	u8	InitDMFlag;
	u32	InitODMFlag;

	//* Upper and Lower Signal threshold for Rate Adaptive*/
	int	UndecoratedSmoothedPWDB;
	int	UndecoratedSmoothedCCK;
	int	EntryMinUndecoratedSmoothedPWDB;
	int	EntryMaxUndecoratedSmoothedPWDB;
	int	MinUndecoratedPWDBForDM;
	int	LastMinUndecoratedPWDBForDM;


	//for High Power
//	u8 	bDynamicTxPowerEnable;
//	u8 	LastDTPLvl;
//	u8 	DynamicTxHighPowerLvl;//Add by Jacken Tx Power Control for Near/Far Range 2008/03/06

	//for tx power tracking
//	u8	bTXPowerTracking;
//	u8	TXPowercount;
//	u8	bTXPowerTrackingInit;
//	u8	TxPowerTrackControl;	//for mp mode, turn off txpwrtracking as default

	u8	PowerIndex_backup[6];

//	s32	OFDM_Pkt_Cnt;

	// Add for Reading Initial Data Rate SEL Register 0x484 during watchdog. Using for fill tx desc. 2011.3.21 by Thomas
	u8	INIDATA_RATE[32];
};

#ifdef CONFIG_CSI
struct csi_priv {
	struct rx_csi_pool *csi_pool;  /* csi buffer pool */
	struct rx_csi_data *handing_buf;  /* handing csi packet */
	u8 csi_curr_flush_cnt;
	u8 trig_addr[6];
	u8 num_bit_per_tone;
	u16 num_sub_carrier;
};
#endif /* CONFIG_CSI */

#ifdef CONFIG_RTL8188F
struct kfree_data_t {
	u8 flag;
	s8 bb_gain[BB_GAIN_NUM][RF_PATH_MAX];
	s8 thermal;
};

#define KFREE_FLAG_ON				BIT0
#define KFREE_FLAG_THERMAL_K_ON		BIT1
#endif

struct hal_spec_t {
	char *ic_name;
	u8 macid_num;

	u8 sec_cam_ent_num;
	u8 sec_cap;

	u8 rfpath_num_2g: 4;	/* used for tx power index path */
	u8 rfpath_num_5g: 4;	/* used for tx power index path */

	u8 max_tx_cnt;
	u8 tx_nss_num: 4;
	u8 rx_nss_num: 4;
	u8 band_cap;	/* value of BAND_CAP_XXX */
	u8 bw_cap;		/* value of BW_CAP_XXX */
	u8 port_num;
	u8 proto_cap;	/* value of PROTO_CAP_XXX */
	u8 wl_func;		/* value of WL_FUNC_XXX */
	u8 hci_type;	/* value of HCI Type */
#if (PHYDM_VERSION == 3)
	struct protocol_cap_t protocol_cap;
#endif
};

#ifdef RTW_HALMAC
struct phy_spec_t {
	u32 trx_cap;
	u32 stbc_cap;
	u32 ldpc_cap;
	u32 txbf_param;
	u32 txbf_cap;
};
#endif

#ifdef CONFIG_MCC_IQK_OFFLOAD
struct hal_iqk_reg_backup {
	u8 central_chnl;
	u8 bw_mode;
	u32 reg_backup[MAX_RF_PATH][MAX_IQK_INFO_BACKUP_REG_NUM];
};
#endif

typedef struct hal_com_data {
	struct rtw_hal_com_t hal_com;/*shared with ax submodules*/
#if (PHYDM_VERSION == 3)
	void *bb;/*ax halbb, useless for phydm*/
	void *rf;/*ax halrf, useless for phydm*/
#elif (PHYDM_VERSION == 2)
	struct dm_priv	dmpriv;
	DM_ODM_T 		odmpriv;
#endif
#ifdef CONFIG_CSI
	struct csi_priv csipriv;
#endif

	HAL_VERSION			VersionID;
//	RT_MULTI_FUNC		MultiFunc; // For multi-function consideration.
//	RT_POLARITY_CTL		PolarityCtl; // For Wifi PDn Polarity control.
	RT_REGULATOR_MODE	RegulatorMode; // switching regulator or LDO
	u16	CustomerID;
	u16 ForcedDataRate;// Force Data Rate. 0: Auto, 0x02: 1M ~ 0x6C: 54M.

	u16	FirmwareVersion;
	u16	FirmwareVersionRev;
	u16	FirmwareSubVersion;
	u16	FirmwareSignature;
//	u8	PGMaxGroup;
	//current WIFI_PHY values
	u32	ReceiveConfig;
	u16	BasicRateSet;
	u8 tx_rate_ToS_cfg; //8 bit control 8 ToS type
	u8 tx_rate_by_ToS[8]; //record different ToS's tx rate which configured by user
	u8 	hci_sus_state;
#if defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8735B)
	u8	rx_tsf_addr_filter_config; /* for 8822B USE */
#endif
#ifdef RTW_HALMAC
	struct phy_spec_t phy_spec;
#endif
	//rf_ctrl
	u8	rf_chip;
	u8	rf_type;
	u8	PackageType;
#if defined(CONFIG_PLATFORM_8711B) \
	|| defined(CONFIG_PLATFORM_8721D) || defined (CONFIG_RTL8188F) \
	|| defined (CONFIG_RTL8192E) || defined (CONFIG_RTL8723D) \
	|| defined(CONFIG_PLATFORM_8710C) \
	|| defined(CONFIG_PLATFORM_AMEBAD2) \
	|| defined (CONFIG_PLATFORM_AMEBALITE) \
	|| defined(CONFIG_PLATFORM_8735B)
	u32	ChipID;
#endif
	u8	NumTotalRFPath;

	u8	BoardType;

	//
	// EEPROM setting.
	//
//	u16	EEPROMVID;
//	u16	EEPROMPID;
//	u16	EEPROMSVID;
//	u16	EEPROMSDID;
	u8	EEPROMCustomerID;
//	u8	EEPROMSubCustomerID;
	u8	EEPROMVersion;
	u8	EEPROMRegulatory;
#if defined(CONFIG_RTL8710C)
	u8	EEPROMRFPwrSaveMode;
#endif

#if defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8730A) || defined(CONFIG_RTL8735B) || defined(CONFIG_RTL8720E) || defined(CONFIG_RTL8730E)

	u8	tx_bbswing_24G;
#if defined(SUPPORT_5G_CHANNEL)
	u8	tx_bbswing_5G;
#endif
#endif
#ifdef CONFIG_RTL8188F
	struct kfree_data_t kfree_data;
#endif
#if defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8735B)
	u32	rf_offset_table_flag;
	u16	rf_offset_addr;
	u8	rf_offset_5g_rx_lna;
	s8  rf_offset_5g_rx_lna1;
	s8  rf_offset_5g_rx_lna2;
#endif
//	u8	bTXPowerDataReadFromEEPORM;
#if (PHYDM_VERSION == 1)
	u8	EEPROMThermalMeter;
#else
	u8  eeprom_thermal_meter;
#endif

#if (PHYDM_VERSION == 2)
	u32 firmware_size;
	u16 firmware_version;
	u16 firmware_sub_version;
#endif
//	u8	bAPKThermalMeterIgnore;

//	BOOLEAN 			EepromOrEfuse;
	//u8				EfuseMap[2][HWSET_MAX_SIZE_512]; //92C:256bytes, 88E:512bytes, we use union set (512bytes)

#if defined(CONFIG_RTL8723B) || defined(CONFIG_RTL8703B) || defined(CONFIG_RTL8723D)
	u8	adjuseVoltageVal;
	u8	need_restore;
	BB_INIT_REGISTER	RegForRecover[5];
#endif

#if defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8735B)
	u8				EfuseUsedPercentage;
#endif
#if	defined(CONFIG_RTL8735B)
	u8	is_fine_tune_rx_spur;
	u8	adc_clk_src;
	u8	da_clk_src;
	u8	dram_type;
	u8	dram_size;
#endif
	//u8	bIQKInitialized;

	u8	Regulation2_4G;
#if defined(SUPPORT_5G_CHANNEL)
	u8	Regulation5G;
#endif

	s8	TxPwrByRateOffset[TX_PWR_BY_RATE_NUM_BAND]
	[TX_PWR_BY_RATE_NUM_RF]
	[TX_PWR_BY_RATE_NUM_RF]
	[TX_PWR_BY_RATE_NUM_RATE];

	u8	Index24G_CCK_Base[MAX_RF_PATH][CHANNEL_MAX_NUMBER];
	u8	Index24G_BW40_Base[MAX_RF_PATH][CHANNEL_MAX_NUMBER];
	//If only one tx, only BW20 and OFDM are used.
	s8	OFDM_24G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
	s8	BW20_24G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#if !defined(NOT_SUPPORT_RF_MULTIPATH)
	s8	CCK_24G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
	s8	BW40_24G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#endif

	/* 5G TX power info for target TX power*/
#if defined(SUPPORT_5G_CHANNEL)
	u8	Index5G_BW40_Base[MAX_RF_PATH][CENTER_CH_5G_ALL_NUM];
#if !defined(NOT_SUPPORT_80M)
	u8	Index5G_BW80_Base[MAX_RF_PATH][CENTER_CH_5G_80M_NUM];
#endif
	s8	OFDM_5G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
	s8	BW20_5G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
	s8	BW40_5G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#if !defined(NOT_SUPPORT_80M)
	s8	BW80_5G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#endif

#endif

	//2 Power Limit Table
	// Power Limit Table for 2.4G
	s8	TxPwrLimit_2_4G[MAX_REGULATION_NUM]
	[MAX_2_4G_BANDWITH_NUM]
	[MAX_RATE_SECTION_NUM]
	[CHANNEL_MAX_NUMBER_2G]
	[MAX_RF_PATH];
#if defined(SUPPORT_5G_CHANNEL)
	// Power Limit Table for 5G
	s8	TxPwrLimit_5G[MAX_REGULATION_NUM]
	[MAX_5G_BANDWITH_NUM]
	[MAX_RATE_SECTION_NUM]
	[CHANNEL_MAX_NUMBER_5G]
	[MAX_RF_PATH];
#endif

	// Store the original power by rate value of the base of each rate section of rf path A & B
	u8	TxPwrByRateBase2_4G[TX_PWR_BY_RATE_NUM_RF]
	[TX_PWR_BY_RATE_NUM_RF]
	[MAX_BASE_NUM_IN_PHY_REG_PG_2_4G];
#if defined(SUPPORT_5G_CHANNEL)
	u8	TxPwrByRateBase5G[TX_PWR_BY_RATE_NUM_RF]
	[TX_PWR_BY_RATE_NUM_RF]
	[MAX_BASE_NUM_IN_PHY_REG_PG_5G];
#endif

#if(RTL8188E_SUPPORT == 1)
	u8	TxPwrLevelCck[MAX_RF_PATH][CHANNEL_MAX_NUMBER];
	u8	TxPwrLevelHT40_1S[MAX_RF_PATH][CHANNEL_MAX_NUMBER];	// For HT 40MHZ pwr
	u8	TxPwrLevelHT40_2S[MAX_RF_PATH][CHANNEL_MAX_NUMBER];	// For HT 40MHZ pwr
	u8	TxPwrHt20Diff[MAX_RF_PATH][CHANNEL_MAX_NUMBER];// HT 20<->40 Pwr diff
	u8	TxPwrLegacyHtDiff[MAX_RF_PATH][CHANNEL_MAX_NUMBER];// For HT<->legacy pwr diff
#endif

	// For power group
//	u8	PwrGroupHT20[MAX_RF_PATH][CHANNEL_MAX_NUMBER];
//	u8	PwrGroupHT40[MAX_RF_PATH][CHANNEL_MAX_NUMBER];

//	u8	LegacyHTTxPowerDiff;// Legacy to HT rate power diff
	// The current Tx Power Level
	u8	CurrentCckTxPwrIdx;
	u8	CurrentOfdm24GTxPwrIdx;
	u8	CurrentBW2024GTxPwrIdx;
#if !defined(NOT_SUPPORT_40M)
	u8	CurrentBW4024GTxPwrIdx;
#endif
	//u32	AntennaTxPath;					// Antenna path Tx
	//u32	AntennaRxPath;					// Antenna path Rx
	//u8	BluetoothCoexist;
//	u8	ExternalPA;

#if defined(CONFIG_RTL8188F) ||defined (CONFIG_RTL8723D) || defined(CONFIG_RTL8188E)

	/* PHY DM & DM Section */
	u8			INIDATA_RATE[32/*MACID_NUM_SW_LIMIT*/];
	/* Upper and Lower Signal threshold for Rate Adaptive*/
	int			EntryMinUndecoratedSmoothedPWDB;
	int			EntryMaxUndecoratedSmoothedPWDB;
	int			MinUndecoratedPWDBForDM;
#endif

#if defined(CONFIG_RTL8192E) || defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8730A) || defined(CONFIG_RTL8735B) || \
	defined(CONFIG_RTL8720E) || defined(CONFIG_RTL8730E)
	u8  u1ForcedIgiLb;
	u8	PAType_2G;
	u8	LNAType_2G;
	u8	external_pa_2g;
	u8	external_lna_2g;
#if defined(SUPPORT_5G_CHANNEL)
	u8	PAType_5G;
	u8	LNAType_5G;
	u8	external_pa_5g;
	u8	external_lna_5g;
#endif
	u8	TypeGLNA;
	u8	TypeGPA;
	u8	TypeALNA;
	u8	TypeAPA;
	u8	INIDATA_RATE[32/*MACID_NUM_SW_LIMIT*/];
#endif
	//u8	bLedOpenDrain; // Support Open-drain arrangement for controlling the LED. Added by Roger, 2009.10.16.

	//u32	LedControlNum;
	//u32	LedControlMode;
	//u8	b1x1RecvCombine;	// for 1T1R receive combining

	//u8	bCurrentTurboEDCA;
	BOOLEAN     bSwChnl;
	BOOLEAN     bSetChnlBW;
	BOOLEAN     bChnlBWInitialized;

#if defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8730A) || defined(CONFIG_RTL8735B) || \
	defined(CONFIG_RTL8720E)  || defined(CONFIG_RTL8730E)
	u8	bDumpRxPkt;//for debug
	u8	bDumpTxPkt;//for debug
	u8  bDisableTXPowerTraining;
	/* PHY DM & DM Section */
	_lock		IQKSpinLock;
#if defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8735B)
	_lock		RfRwSpinLock;
	_lock		GntBTSpinLock;
#endif
#endif

	u8			bNeedIQK;

	u32	AcParam_BE; //Original parameter for BE, use for EDCA turbo.
#if defined(NOT_SUPPORT_RF_MULTIPATH)
	BB_REGISTER_DEFINITION_T	PHYRegDef[1];	//Radio A
	u32	RfRegChnlVal[1];
#else
	BB_REGISTER_DEFINITION_T	PHYRegDef[4];	//Radio A/B/C/D
	u32	RfRegChnlVal[2];
#endif

	//RDG enable
//	BOOLEAN	 bRDGEnable;

#if defined(CONFIG_RTL8711B) || defined(CONFIG_RTL8188F) || defined(CONFIG_RTL8192E) ||defined (CONFIG_RTL8723D) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8188E) || defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8730A) || defined(CONFIG_RTL8735B) || \
	defined(CONFIG_RTL8720E) || defined(CONFIG_RTL8730E)
	//for host message to fw
	u8	LastHMEBoxNum;
#endif

	u8	fw_ractrl;
//	u8	RegTxPause;
	// Beacon function related global variable.
//	u32	RegBcnCtrlVal;
	u8	RegFwHwTxQCtrl;
	u8	RegReg542;
//	u8	RegCR_1;
	u16	RegRRSR;
	//_lock			odm_stainfo_lock;

#ifdef CONFIG_BT_COEXIST
	u8	ant_path; /* for 8723B s0/s1 selection	 */
	u8	EEPROMBluetoothCoexist;
	u8	EEPROMBluetoothType;
	u8	EEPROMBluetoothAntNum;
	BT_COEXIST	bt_coexist;
	/* Upper and Lower Signal threshold for Rate Adaptive*/
	int	entry_min_undecorated_smoothed_pwdb;
#endif

#ifdef CONFIG_BT_COEXIST_SOC
	u8	EEPROMBluetoothCoexist;
#endif

//#ifdef CONFIG_ANTENNA_DIVERSITY
	u8	CurAntenna;
	u8	AntDivCfg;
	u8	TRxAntDivType;
	u8	AntDivGPIO;
//#endif

//	u8	FwRsvdPageStartOffset; //2010.06.23. Added by tynli. Reserve page start offset except beacon in TxQ.

	// 2010/08/09 MH Add CU power down mode.
	BOOLEAN		pwrdown;

	// Add for dual MAC  0--Mac0 1--Mac1
//	u32	interfaceIndex;

	u8	OutEpQueueSel;
	u8	OutEpNumber;

	// 2010/12/10 MH Add for USB aggreation mode dynamic shceme.
//	BOOLEAN		UsbRxHighSpeedMode;

	// 2010/11/22 MH Add for slim combo debug mode selective.
	// This is used for fix the drawback of CU TSMC-A/UMC-A cut. HW auto suspend ability. Close BT clock.
	//BOOLEAN		SlimComboDbg;

	u16	EfuseUsedBytes;

	u8	AMPDUDensity;

	// Auto FSM to Turn On, include clock, isolation, power control for MAC only
	u8			bMacPwrCtrlOn;

#if defined(CONFIG_SDIO_HCI) || defined(CONFIG_GSPI_HCI)

	//
	// For SDIO Interface HAL related
	//

	//
	// SDIO ISR Related
	//
//	u32			IntrMask[1];
//	u32			IntrMaskToSet[1];
//	LOG_INTERRUPT		InterruptLog;
//	u32			sdio_himr;
//	u32			sdio_hisr;

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
	u8			SdioRxFIFOCnt;
//	u16			SdioRxFIFOSize;
#endif //CONFIG_SDIO_HCI

#if defined (CONFIG_PCI_HCI) || defined(CONFIG_LX_HCI) || defined(CONFIG_AXI_HCI)
//	u32	TransmitConfig;
#if defined(CONFIG_AXI_HCI)
	u32	IntArray[4];
	u32	IntrMask[4];
#else
	u32	IntArray[3];
	u32	IntrMask[3];
#endif
//	u8	bDefaultAntenna;
//	u8	bIQKInitialized;

//	u8	bInterruptMigration;
//	u8	bDisableTxInt;
	u16	RxExpectTag;
#ifdef CONFIG_DEBUG_DYNAMIC
	struct hal_debug	debug_info;
#endif

#endif //CONFIG_PCI_HCI || CONFIG_LX_HCI || defined(CONFIG_AXI_HCI)

	struct hal_spec_t hal_spec;

#ifdef CONFIG_MP_INCLUDED
	BOOLEAN				bCCKinCH14;
#endif

#ifdef CONFIG_MCC_IQK_OFFLOAD
	struct hal_iqk_reg_backup iqk_reg_backup[MAX_IQK_INFO_BACKUP_CHNL_NUM];
#endif

#ifdef RTW_HALMAC
	u8 drv_rsvd_page_number;
	u8 not_xmitframe_fw_dl; /*not use xmitframe to download fw*/
#endif

#if (PHYDM_VERSION == 2)
	u8 phydm_op_mode;
#endif
	u32  prv_traffic_idx;
	bool is_turbo_edca;


} HAL_DATA_COMMON, *PHAL_DATA_COMMON;

#if (PHYDM_VERSION == 2)
#define TXPWR_LMT_RS_CCK	0
#define TXPWR_LMT_RS_OFDM	1
#define TXPWR_LMT_RS_HT 	2
#define TXPWR_LMT_RS_VHT	3

//register definition
#define KFREE_FLAG_ON				BIT(0)
#define KFREE_FLAG_THERMAL_K_ON 	BIT(1)


#define REG_BLUE_TOOTH	rBlue_Tooth
#define REG_CCK_0_AFE_SETTING	rCCK0_AFESetting
#define REG_CONFIG_ANT_A	rConfig_AntA
#define REG_CONFIG_ANT_B	rConfig_AntB

#define REG_FPGA0_IQK	rFPGA0_IQK
#define REG_FPGA0_RFMOD rFPGA0_RFMOD
#define REG_FPGA0_TX_GAIN_STAGE rFPGA0_TxGainStage
#define REG_FPGA0_XA_RF_INTERFACE_OE	rFPGA0_XA_RFInterfaceOE
#define REG_FPGA0_XB_RF_INTERFACE_OE	rFPGA0_XB_RFInterfaceOE
#define REG_FPGA0_XA_HSSI_PARAMETER1	rFPGA0_XA_HSSIParameter1
#define REG_FPGA0_XA_LSSI_PARAMETER 	rFPGA0_XA_LSSIParameter
#define REG_FPGA0_XB_HSSI_PARAMETER1	rFPGA0_XB_HSSIParameter1
#define REG_FPGA0_XB_LSSI_PARAMETER 	rFPGA0_XB_LSSIParameter
#define REG_FPGA0_XB_LSSI_READ_BACK 	rFPGA0_XB_LSSIReadBack
#define REG_FPGA0_XAB_RF_INTERFACE_SW	rFPGA0_XAB_RFInterfaceSW
#define REG_FPGA0_XAB_RF_PARAMETER		rFPGA0_XAB_RFParameter
#define REG_FPGA0_XCD_RF_INTERFACE_SW	rFPGA0_XCD_RFInterfaceSW
#define REG_FPGA0_XCD_SWITCH_CONTROL	rFPGA0_XCD_SwitchControl

#define REG_IQK_AGC_RSP 	rIQK_AGC_Rsp
#define REG_IQK_AGC_PTS 	rIQK_AGC_Pts
#define REG_IQK_AGC_CONT	rIQK_AGC_Cont

#define REG_OFDM_0_AGC_RSSI_TABLE	rOFDM0_AGCRSSITable
#define REG_OFDM_0_ECCA_THRESHOLD		rOFDM0_ECCAThreshold
#define REG_OFDM_0_RX_IQ_EXT_ANTA	rOFDM0_RxIQExtAnta
#define REG_OFDM_0_TRX_PATH_ENABLE	rOFDM0_TRxPathEnable
#define REG_OFDM_0_TR_MUX_PAR	rOFDM0_TRMuxPar
#define REG_OFDM_0_XA_AGC_CORE1 rOFDM0_XAAGCCore1
#define REG_OFDM_0_XB_AGC_CORE1 rOFDM0_XBAGCCore1
#define REG_OFDM_0_XA_RX_IQ_IMBALANCE	rOFDM0_XARxIQImbalance
#define REG_OFDM_0_XB_RX_IQ_IMBALANCE	rOFDM0_XBRxIQImbalance
#define REG_OFDM_0_XA_TX_IQ_IMBALANCE	rOFDM0_XATxIQImbalance
#define REG_OFDM_0_XB_TX_IQ_IMBALANCE	rOFDM0_XBTxIQImbalance
#define REG_OFDM_0_XC_TX_AFE	rOFDM0_XCTxAFE
#define REG_OFDM_0_XD_TX_AFE	rOFDM0_XDTxAFE

#define REG_TX_AGC_A_CCK_11_CCK_1_JAGUAR	rTxAGC_A_CCK11_CCK1_JAguar
#define REG_TX_AGC_A_CCK_1_MCS32	rTxAGC_A_CCK1_Mcs32
#define REG_TX_AGC_A_MCS11_MCS8_JAGUAR	rTxAGC_A_MCS11_MCS8_JAguar
#define REG_TX_AGC_A_MCS15_MCS12_JAGUAR	rTxAGC_A_MCS15_MCS12_JAguar
#define REG_TX_AGC_A_MCS19_MCS16_JAGUAR	rTxAGC_A_MCS19_MCS16_JAguar
#define REG_TX_AGC_A_MCS23_MCS20_JAGUAR	rTxAGC_A_MCS23_MCS20_JAguar
#define REG_TX_AGC_A_MCS3_MCS0_JAGUAR	rTxAGC_A_MCS3_MCS0_JAguar
#define REG_TX_AGC_A_MCS7_MCS4_JAGUAR	rTxAGC_A_MCS7_MCS4_JAguar
#define REG_TX_AGC_A_MCS03_MCS00	rTxAGC_A_Mcs03_Mcs00
#define REG_TX_AGC_A_MCS07_MCS04	rTxAGC_A_Mcs07_Mcs04
#define REG_TX_AGC_A_MCS11_MCS08	rTxAGC_A_Mcs11_Mcs08
#define REG_TX_AGC_A_MCS15_MCS12	rTxAGC_A_Mcs15_Mcs12
#define REG_TX_AGC_A_NSS1_INDEX3_NSS1_INDEX0_JAGUAR	rTxAGC_A_Nss1Index3_Nss1Index0_JAguar
#define REG_TX_AGC_A_NSS1_INDEX7_NSS1_INDEX4_JAGUAR	rTxAGC_A_Nss1Index7_Nss1Index4_JAguar
#define REG_TX_AGC_A_NSS2_INDEX1_NSS1_INDEX8_JAGUAR	rTxAGC_A_Nss2Index1_Nss1Index8_JAguar
#define REG_TX_AGC_A_NSS2_INDEX5_NSS2_INDEX2_JAGUAR	rTxAGC_A_Nss2Index5_Nss2Index2_JAguar
#define REG_TX_AGC_A_NSS2_INDEX9_NSS2_INDEX6_JAGUAR	rTxAGC_A_Nss2Index9_Nss2Index6_JAguar
#define REG_TX_AGC_A_NSS3_INDEX3_NSS3_INDEX0_JAGUAR	rTxAGC_A_Nss3Index3_Nss3Index0_JAguar
#define REG_TX_AGC_A_NSS3_INDEX7_NSS3_INDEX4_JAGUAR	rTxAGC_A_Nss3Index7_Nss3Index4_JAguar
#define REG_TX_AGC_A_NSS3_INDEX9_NSS3_INDEX8_JAGUAR	rTxAGC_A_Nss3Index9_Nss3Index8_JAguar
#define REG_TX_AGC_A_OFDM18_OFDM6_JAGUAR	rTxAGC_A_Ofdm18_Ofdm6_JAguar
#define REG_TX_AGC_A_OFDM54_OFDM24_JAGUAR	rTxAGC_A_Ofdm54_Ofdm24_JAguar
#define REG_TX_AGC_A_RATE18_06	rTxAGC_A_Rate18_06
#define REG_TX_AGC_A_RATE54_24	rTxAGC_A_Rate54_24
#define REG_TX_AGC_B_CCK_11_A_CCK_2_11	rTxAGC_B_CCK11_A_CCK2_11
#define REG_TX_AGC_B_CCK_11_CCK_1_JAGUAR	rTxAGC_B_CCK11_CCK1_JAguar
#define REG_TX_AGC_B_CCK_1_55_MCS32	rTxAGC_B_CCK1_55_Mcs32
#define REG_TX_AGC_B_MCS11_MCS8_JAGUAR	rTxAGC_B_MCS11_MCS8_JAguar
#define REG_TX_AGC_B_MCS15_MCS12_JAGUAR	rTxAGC_B_MCS15_MCS12_JAguar
#define REG_TX_AGC_B_MCS19_MCS16_JAGUAR	rTxAGC_B_MCS19_MCS16_JAguar
#define REG_TX_AGC_B_MCS23_MCS20_JAGUAR	rTxAGC_B_MCS23_MCS20_JAguar
#define REG_TX_AGC_B_MCS3_MCS0_JAGUAR	rTxAGC_B_MCS3_MCS0_JAguar
#define REG_TX_AGC_B_MCS7_MCS4_JAGUAR	rTxAGC_B_MCS7_MCS4_JAguar
#define REG_TX_AGC_B_MCS03_MCS00	rTxAGC_B_Mcs03_Mcs00
#define REG_TX_AGC_B_MCS07_MCS04	rTxAGC_B_Mcs07_Mcs04
#define REG_TX_AGC_B_MCS11_MCS08	rTxAGC_B_Mcs11_Mcs08
#define REG_TX_AGC_B_MCS15_MCS12	rTxAGC_B_Mcs15_Mcs12
#define REG_TX_AGC_B_NSS1_INDEX3_NSS1_INDEX0_JAGUAR	rTxAGC_B_Nss1Index3_Nss1Index0_JAguar
#define REG_TX_AGC_B_NSS1_INDEX7_NSS1_INDEX4_JAGUAR	rTxAGC_B_Nss1Index7_Nss1Index4_JAguar
#define REG_TX_AGC_B_NSS2_INDEX1_NSS1_INDEX8_JAGUAR	rTxAGC_B_Nss2Index1_Nss1Index8_JAguar
#define REG_TX_AGC_B_NSS2_INDEX5_NSS2_INDEX2_JAGUAR	rTxAGC_B_Nss2Index5_Nss2Index2_JAguar
#define REG_TX_AGC_B_NSS2_INDEX9_NSS2_INDEX6_JAGUAR	rTxAGC_B_Nss2Index9_Nss2Index6_JAguar
#define REG_TX_AGC_B_NSS3_INDEX3_NSS3_INDEX0_JAGUAR	rTxAGC_B_Nss3Index3_Nss3Index0_JAguar
#define REG_TX_AGC_B_NSS3_INDEX7_NSS3_INDEX4_JAGUAR	rTxAGC_B_Nss3Index7_Nss3Index4_JAguar
#define REG_TX_AGC_B_NSS3_INDEX9_NSS3_INDEX8_JAGUAR	rTxAGC_B_Nss3Index9_Nss3Index8_JAguar
#define REG_TX_AGC_B_OFDM18_OFDM6_JAGUAR	rTxAGC_B_Ofdm18_Ofdm6_JAguar
#define REG_TX_AGC_B_OFDM54_OFDM24_JAGUAR	rTxAGC_B_Ofdm54_Ofdm24_JAguar
#define REG_TX_AGC_B_RATE18_06	rTxAGC_B_Rate18_06
#define REG_TX_AGC_B_RATE54_24	rTxAGC_B_Rate54_24

#define REG_TX_CCK_BBON rTx_CCK_BBON
#define REG_TX_CCK_RFON rTx_CCK_RFON
#define REG_TX_OFDM_BBON	rTx_OFDM_BBON
#define REG_TX_OFDM_RFON	rTx_OFDM_RFON
#define REG_TX_IQK	rTx_IQK
#define REG_TX_IQK_TONE_A	rTx_IQK_Tone_A
#define REG_TX_IQK_TONE_B	rTx_IQK_Tone_B
#define REG_TX_IQK_PI_A rTx_IQK_PI_A
#define REG_TX_IQK_PI_B rTx_IQK_PI_B
#define REG_TX_POWER_AFTER_IQK_A	rTx_Power_After_IQK_A
#define REG_TX_POWER_AFTER_IQK_B	rTx_Power_After_IQK_B
#define REG_TX_POWER_BEFORE_IQK_A	rTx_Power_Before_IQK_A
#define REG_TX_POWER_BEFORE_IQK_B	rTx_Power_Before_IQK_B
#define REG_TX_TO_RX	rTx_To_Rx
#define REG_TX_TO_TX	rTx_To_Tx

#define REG_RX_CCK	rRx_CCK
#define REG_RX_IQK	rRx_IQK
#define REG_RX_IQK_TONE_A	rRx_IQK_Tone_A
#define REG_RX_IQK_TONE_B	rRx_IQK_Tone_B
#define REG_RX_IQK_PI_A rRx_IQK_PI_A
#define REG_RX_IQK_PI_B rRx_IQK_PI_B
#define REG_RX_OFDM rRx_OFDM
#define REG_RX_POWER_AFTER_IQK_A_2	rRx_Power_After_IQK_A_2
#define REG_RX_POWER_AFTER_IQK_B_2	rRx_Power_After_IQK_B_2
#define REG_RX_POWER_BEFORE_IQK_A_2 rRx_Power_Before_IQK_A_2
#define REG_RX_POWER_BEFORE_IQK_B_2 rRx_Power_Before_IQK_B_2
#define REG_RX_TO_RX	rRx_TO_Rx
#define REG_RX_WAIT_CCA rRx_Wait_CCA
#define REG_RX_WAIT_RIFS	rRx_Wait_RIFS


#define REG_TX_IQK_TONE_B	rTx_IQK_Tone_B
#define REG_TX_IQK_PI_A rTx_IQK_PI_A

#define REG_PMPD_ANAEN	rPMPD_ANAEN
#define REG_SLEEP	rSleep
#define REG_STANDBY rStandby

#define RX_SMOOTH_FACTOR	Rx_Smooth_Factor

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
#endif // #if (PHYDM_VERSION == 2)

#if defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8730A) || defined(CONFIG_RTL8735B) || \
	defined(CONFIG_RTL8720E)  || defined(CONFIG_RTL8730E)
//#define ODM_RF_PATH_A RF_PATH_A
//#define ODM_RF_PATH_B RF_PATH_B
//#define ODM_RF_PATH_C RF_PATH_C
//#define ODM_RF_PATH_D RF_PATH_D
//#define ODM_RF_PATH_AB RF_PATH_AB

#define ODM_COMP_COMMON	BIT(30)

typedef struct _ROM_INFO {
	u1Byte	EEPROMVersion;
	u1Byte	CrystalCap;
	u8Byte	DebugComponents;
	u4Byte	DebugLevel;
} ROM_INFO, *PROM_INFO;
extern ROM_INFO					ROMInfo;

#endif // #if defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8710C)


typedef struct hal_com_data HAL_DATA_TYPE, *PHAL_DATA_TYPE;
#define GET_HAL_DATA(__pAdapter)	((HAL_DATA_TYPE *)((__pAdapter)->HalData))
#define GET_HAL_SPEC(__pAdapter)			(&(GET_HAL_DATA((__pAdapter))->hal_spec))
#define rtw_is_hw_init_completed(adapter)		(((PADAPTER)adapter)->hw_init_completed == _TRUE)


/* alias for phydm coding style */
#define REG_A_TX_SCALE_JAGUAR			rA_TxScale_Jaguar

#define RX_SMOOTH_FACTOR	Rx_Smooth_Factor

#endif //__HAL_DATA_H__
