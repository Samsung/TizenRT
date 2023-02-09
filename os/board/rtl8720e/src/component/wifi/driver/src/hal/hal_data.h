#ifndef __HAL_DATA_H__
#define __HAL_DATA_H__


#include "hal_com.h"
#if (PHYDM_VERSION == 2)
#include "phydm/phydm_precomp.h"
#endif

#ifdef CONFIG_RTL8721D
#include "rtl8721d_hal.h"
#endif

#ifdef CONFIG_RTL8721F
#include "rtl8721f_hal.h"
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

#ifdef CONFIG_RTL8195B
#include "rtl8195b/rtl8195b_hal.h"
#endif

#ifdef CONFIG_RTL8710C
#include "rtl8710c/rtl8710c_hal.h"
#endif

#ifdef CONFIG_RTL8735B
#include "rtl8735b/rtl8735b_hal.h"
#endif

enum _EFUSE_DEF_TYPE {
	TYPE_EFUSE_MAX_SECTION				= 0,
	TYPE_EFUSE_REAL_CONTENT_LEN			= 1,
	TYPE_AVAILABLE_EFUSE_BYTES_BANK		= 2,
	TYPE_AVAILABLE_EFUSE_BYTES_TOTAL	= 3,
	TYPE_EFUSE_MAP_LEN					= 4,
	TYPE_EFUSE_PROTECT_BYTES_BANK		= 5,
	TYPE_EFUSE_CONTENT_LEN_BANK			= 6,
	TYPE_EFUSE_REMAIN_LEN				= 7,
};
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

// Tx Power Limit Table Size
#define MAX_REGULATION_NUM						TXPWR_LMT_MAX_REGULATION_NUM
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

//efuse
#define	EFUSE_WIFI				0
#define 	EFUSE_MAP_SIZE			512

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
	u32	InitODMFlag;
	//* Upper and Lower Signal threshold for Rate Adaptive*/
	int	UndecoratedSmoothedPWDB;
	int	UndecoratedSmoothedCCK;
	int	EntryMinUndecoratedSmoothedPWDB;
	int	EntryMaxUndecoratedSmoothedPWDB;
	int	MinUndecoratedPWDBForDM;
	int	LastMinUndecoratedPWDBForDM;
	u8	PowerIndex_backup[6];
};

#ifdef CONFIG_CSI
struct csi_priv {
	struct rx_csi_pool *csi_pool;  /* csi buffer pool */
	struct rx_csi_data *handing_buf;  /* handing csi packet */
	u8 csi_handle_flag;  /* 0: wait new csi; 1: handling curr csi */
	u16 per_macid_csi_en;  /* latch per sta csi en or dis, bitx indicate macid=x*/
	u8 num_csi_sta;  /* record the number of sta which en csi*/
	u8 num_bit_per_tone;
	u16 num_sub_carrier;
};
#endif /* CONFIG_CSI */

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

	u16 max_ampdu_buffer_size;
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

typedef struct hal_com_data {
	struct rtw_hal_com_t	hal_com;	/*shared with ax submodules*/
#if (PHYDM_VERSION == 3)
	void			*bb;		/*ax halbb, useless for phydm*/
	void			*rf;		/*ax halrf, useless for phydm*/
#elif (PHYDM_VERSION == 2)
	struct dm_priv		dmpriv;
	DM_ODM_T 		odmpriv;
#endif
#ifdef CONFIG_CSI
	struct csi_priv		csipriv;
#endif
	HAL_VERSION		VersionID;
	/* current WIFI_PHY values */

	u16			BasicRateSet;
	u8			tx_rate_ToS_cfg;	/* 8 bit control 8 ToS type */
	u8			tx_rate_by_ToS[8];	/*record different ToS's tx rate which configured by user */

#ifdef RTW_HALMAC
	struct phy_spec_t	phy_spec;
#endif
	u8			rf_type;
#if defined(CONFIG_RTL8721F) || defined(CONFIG_RTL8721D)
	u32			ChipID;
#endif
	u8			NumTotalRFPath;
	u8			EEPROMRegulatory;

#if ((PHYDM_VERSION == 2) || (PHYDM_VERSION == 3))
	u8			eeprom_thermal_meter;
#endif

	u8			Regulation2_4G;
#if defined(SUPPORT_5G_CHANNEL)
	u8			Regulation5G;
#endif

	/* PHY DM & DM Section */
	_lock			IQKSpinLock;
	u8			bNeedIQK;
	u32			AcParam_BE; //Original parameter for BE, use for EDCA turbo.

#if defined(CONFIG_RTL8721D)
#if defined(NOT_SUPPORT_RF_MULTIPATH)
	BB_REGISTER_DEFINITION_T	PHYRegDef[1];	//Radio A
	u32	RfRegChnlVal[1];
#else
	BB_REGISTER_DEFINITION_T	PHYRegDef[4];	//Radio A/B/C/D
	u32	RfRegChnlVal[2];
#endif // NOT_SUPPORT_RF_MULTIPATH
#endif // CONFIG_RTL8721D

	/* for host message to fw */
	u8			LastHMEBoxNum;

#ifdef CONFIG_BT_COEXIST
	u8			ant_path; /* for 8730E BT s0/s1 selection	 */
	u8			EEPROMBluetoothCoexist;
	u8			EEPROMBluetoothType;
	u8			EEPROMBluetoothAntNum;
	/* Upper and Lower Signal threshold for Rate Adaptive*/
	int			entry_min_undecorated_smoothed_pwdb;
#endif

#ifdef CONFIG_BT_COEXIST_SOC
	u8			EEPROMBluetoothCoexist;
#endif

	u8			CurAntenna;
	u8			AntDivCfg;
	u8			AntDivGPIO;
	u8			AMPDUDensity;

	// Auto FSM to Turn On, include clock, isolation, power control for MAC only
	u8			bMacPwrCtrlOn;
#if defined(CONFIG_AXI_HCI)
	u32			IntArray[4];
	u32			IntrMask[4];
#else
	u32			IntArray[3];
	u32			IntrMask[3];
#endif

	u16			RxExpectTag;

	struct hal_spec_t	hal_spec;

#ifdef RTW_HALMAC
	u8			drv_rsvd_page_number;
	u8			not_xmitframe_fw_dl; /*not use xmitframe to download fw*/
#endif

#if (PHYDM_VERSION == 2)
	u8			phydm_op_mode;
#endif

	_mutex fw_h2c_mutex;
	_mutex set_chan_mutex;
	u16			interface_type;	//USB,SDIO,SPI,PCI

#if defined(CONFIG_RTL8735B) || defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8721F)
	u8			fw_ractrl;
#endif

#if defined(CONFIG_RTL8735B)
	u32			ReceiveConfig;
	u8			dram_type;
	u8			dram_size;
	u8			external_lna_2g;
#if defined(SUPPORT_5G_CHANNEL)
	u8			external_pa_5g;
	u8			external_lna_5g;
#endif // SUPPORT_5G_CHANNEL
	u8			INIDATA_RATE[32];	/* MACID_NUM_SW_LIMIT: 32 */
	_lock			GntBTSpinLock;
#endif // CONFIG_RTL8735B

} HAL_DATA_COMMON, *PHAL_DATA_COMMON;

#if (PHYDM_VERSION == 2)
#define TXPWR_LMT_RS_CCK	0
#define TXPWR_LMT_RS_OFDM	1
#define TXPWR_LMT_RS_HT 	2
#define TXPWR_LMT_RS_VHT	3

//register definition
#define KFREE_FLAG_ON		BIT(0)
#define KFREE_FLAG_THERMAL_K_ON	BIT(1)


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

#define ODM_COMP_COMMON	BIT(30)

typedef struct _ROM_INFO {
	u1Byte	EEPROMVersion;
	u1Byte	CrystalCap;
	u8Byte	DebugComponents;
	u4Byte	DebugLevel;
} ROM_INFO, *PROM_INFO;
extern ROM_INFO					ROMInfo;

typedef struct hal_com_data HAL_DATA_TYPE, *PHAL_DATA_TYPE;

#define GET_HAL_SPEC(__pAdapter)			(&(rtw_get_haldata((__pAdapter))->hal_spec))
#define rtw_is_hw_init_completed(adapter)		(((PADAPTER)adapter)->hw_init_completed == _TRUE)

/* alias for phydm coding style */
#define REG_A_TX_SCALE_JAGUAR			rA_TxScale_Jaguar

#define RX_SMOOTH_FACTOR	Rx_Smooth_Factor

#endif //__HAL_DATA_H__
