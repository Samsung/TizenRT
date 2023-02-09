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
#ifndef _RTW_MP_H_
#define _RTW_MP_H_

//	00 - Success
//	11 - Error
#define STATUS_SUCCESS				(0x00000000L)
#define STATUS_PENDING				(0x00000103L)

#define STATUS_UNSUCCESSFUL			(0xC0000001L)
#define STATUS_INSUFFICIENT_RESOURCES		(0xC000009AL)
#define STATUS_NOT_SUPPORTED			(0xC00000BBL)

#define NDIS_STATUS_SUCCESS			((NDIS_STATUS)STATUS_SUCCESS)
#define NDIS_STATUS_PENDING			((NDIS_STATUS)STATUS_PENDING)
#define NDIS_STATUS_NOT_RECOGNIZED		((NDIS_STATUS)0x00010001L)
#define NDIS_STATUS_NOT_COPIED			((NDIS_STATUS)0x00010002L)
#define NDIS_STATUS_NOT_ACCEPTED		((NDIS_STATUS)0x00010003L)
#define NDIS_STATUS_CALL_ACTIVE			((NDIS_STATUS)0x00010007L)

#define NDIS_STATUS_FAILURE			((NDIS_STATUS)STATUS_UNSUCCESSFUL)
#define NDIS_STATUS_RESOURCES			((NDIS_STATUS)STATUS_INSUFFICIENT_RESOURCES)
#define NDIS_STATUS_CLOSING			((NDIS_STATUS)0xC0010002L)
#define NDIS_STATUS_BAD_VERSION			((NDIS_STATUS)0xC0010004L)
#define NDIS_STATUS_BAD_CHARACTERISTICS		((NDIS_STATUS)0xC0010005L)
#define NDIS_STATUS_ADAPTER_NOT_FOUND		((NDIS_STATUS)0xC0010006L)
#define NDIS_STATUS_OPEN_FAILED			((NDIS_STATUS)0xC0010007L)
#define NDIS_STATUS_DEVICE_FAILED		((NDIS_STATUS)0xC0010008L)
#define NDIS_STATUS_MULTICAST_FULL		((NDIS_STATUS)0xC0010009L)
#define NDIS_STATUS_MULTICAST_EXISTS		((NDIS_STATUS)0xC001000AL)
#define NDIS_STATUS_MULTICAST_NOT_FOUND		((NDIS_STATUS)0xC001000BL)
#define NDIS_STATUS_REQUEST_ABORTED		((NDIS_STATUS)0xC001000CL)
#define NDIS_STATUS_RESET_IN_PROGRESS		((NDIS_STATUS)0xC001000DL)
#define NDIS_STATUS_CLOSING_INDICATING		((NDIS_STATUS)0xC001000EL)
#define NDIS_STATUS_NOT_SUPPORTED		((NDIS_STATUS)STATUS_NOT_SUPPORTED)
#define NDIS_STATUS_INVALID_PACKET		((NDIS_STATUS)0xC001000FL)
#define NDIS_STATUS_OPEN_LIST_FULL		((NDIS_STATUS)0xC0010010L)
#define NDIS_STATUS_ADAPTER_NOT_READY		((NDIS_STATUS)0xC0010011L)
#define NDIS_STATUS_ADAPTER_NOT_OPEN		((NDIS_STATUS)0xC0010012L)
#define NDIS_STATUS_NOT_INDICATING		((NDIS_STATUS)0xC0010013L)
#define NDIS_STATUS_INVALID_LENGTH		((NDIS_STATUS)0xC0010014L)
#define NDIS_STATUS_INVALID_DATA		((NDIS_STATUS)0xC0010015L)
#define NDIS_STATUS_BUFFER_TOO_SHORT		((NDIS_STATUS)0xC0010016L)
#define NDIS_STATUS_INVALID_OID			((NDIS_STATUS)0xC0010017L)
#define NDIS_STATUS_ADAPTER_REMOVED		((NDIS_STATUS)0xC0010018L)
#define NDIS_STATUS_UNSUPPORTED_MEDIA		((NDIS_STATUS)0xC0010019L)
#define NDIS_STATUS_GROUP_ADDRESS_IN_USE	((NDIS_STATUS)0xC001001AL)
#define NDIS_STATUS_FILE_NOT_FOUND		((NDIS_STATUS)0xC001001BL)
#define NDIS_STATUS_ERROR_READING_FILE		((NDIS_STATUS)0xC001001CL)
#define NDIS_STATUS_ALREADY_MAPPED		((NDIS_STATUS)0xC001001DL)
#define NDIS_STATUS_RESOURCE_CONFLICT		((NDIS_STATUS)0xC001001EL)
#define NDIS_STATUS_NO_CABLE			((NDIS_STATUS)0xC001001FL)

#define NDIS_STATUS_INVALID_SAP			((NDIS_STATUS)0xC0010020L)
#define NDIS_STATUS_SAP_IN_USE			((NDIS_STATUS)0xC0010021L)
#define NDIS_STATUS_INVALID_ADDRESS		((NDIS_STATUS)0xC0010022L)
#define NDIS_STATUS_VC_NOT_ACTIVATED		((NDIS_STATUS)0xC0010023L)
#define NDIS_STATUS_DEST_OUT_OF_ORDER		((NDIS_STATUS)0xC0010024L)  // cause 27
#define NDIS_STATUS_VC_NOT_AVAILABLE		((NDIS_STATUS)0xC0010025L)  // cause 35,45
#define NDIS_STATUS_CELLRATE_NOT_AVAILABLE	((NDIS_STATUS)0xC0010026L)  // cause 37
#define NDIS_STATUS_INCOMPATABLE_QOS		((NDIS_STATUS)0xC0010027L)  // cause 49
#define NDIS_STATUS_AAL_PARAMS_UNSUPPORTED	((NDIS_STATUS)0xC0010028L)  // cause 93
#define NDIS_STATUS_NO_ROUTE_TO_DESTINATION	((NDIS_STATUS)0xC0010029L)  // cause 3

typedef enum _ANTENNA_PATH {
	ANTENNA_NONE	= 0x00,
	ANTENNA_D		,
	ANTENNA_C		,
	ANTENNA_CD		,
	ANTENNA_B		,
	ANTENNA_BD		,
	ANTENNA_BC		,
	ANTENNA_BCD 	,
	ANTENNA_A		,
	ANTENNA_AD		,
	ANTENNA_AC		,
	ANTENNA_ACD 	,
	ANTENNA_AB		,
	ANTENNA_ABD 	,
	ANTENNA_ABC 	,
	ANTENNA_ABCD
} ANTENNA_PATH;


#define MAX_MP_XMITBUF_SZ 	2048
#define NR_MP_XMITFRAME		8

struct mp_xmit_frame {
	_list	list;

	struct pkt_attrib attrib;

	_pkt *pkt;

	int frame_tag;

	_adapter *padapter;

	uint mem[(MAX_MP_XMITBUF_SZ >> 2)];
};

struct mp_wiparam {
	u32 bcompleted;
	u32 act_type;
	u32 io_offset;
	u32 io_value;
};

typedef void(*wi_act_func)(void *padapter);

struct mp_tx {
	u8 stop;
	u32 count, sended;
	u8 payload;
	struct pkt_attrib attrib;
	struct tx_desc desc;
	u8 *pallocated_buf;
	u8 *buf;
	u32 buf_size, write_size;
	//_thread_hdl_	PktTxThread;
	struct task_struct	MpXmitThread;
};

#define MP_MAX_LINES		1000
#define MP_MAX_LINES_BYTES	256

typedef struct _RT_PMAC_PKT_INFO {
	UCHAR			MCS;
	UCHAR			Nss;
	UCHAR			Nsts;
	UINT			N_sym;
	UCHAR			SIGA2B3;
	UCHAR			PPDU_Type;
	UCHAR			HE_ERSU_RU106;
} RT_PMAC_PKT_INFO, *PRT_PMAC_PKT_INFO;

typedef struct _RT_PMAC_TX_INFO {
	u8			bEnPMacTx: 1;		/* 0: Disable PMac 1: Enable PMac */
	u8			Mode: 3;				/* 0: Packet TX 3:Continuous TX */
	u8			Ntx: 4;				/* 0-7 */
	u16			TX_RATE;			/* MPT_RATE_E */
	u16			TX_RATE_HEX;
	u8			TX_SC;
	u8			bSGI: 1;
	u8			bSPreamble: 1;
	u8			bSTBC: 1;
	u8			bLDPC: 1;
	u8			NDP_sound: 1;
	u8			BandWidth: 3;		/* 0: 20 1:40 2:80Mhz */
	u8			m_STBC;			/* bSTBC + 1 */
	u16			PacketPeriod;
	u32			PacketCount;
	u32			PacketLength;
	u8			PacketPattern;
	u16			SFD;
	u8			SignalField;
	u8			ServiceField;
	u16			LENGTH;
	u8			CRC16[2];
	u8			LSIG[3];
	u8			HT_SIG[6];
	u8			VHT_SIG_A[6];
	u8			VHT_SIG_B[4];
	u8			VHT_SIG_B_CRC;
	u8			VHT_Delimiter[4];
	u8			MacAddress[6];
	u16			RU_Alloc;
} RT_PMAC_TX_INFO, *PRT_PMAC_TX_INFO;

typedef void (*MPT_WORK_ITEM_HANDLER)(IN void *Adapter);
typedef struct _MPT_CONTEXT {
	// Indicate if we have started Mass Production Test.
	BOOLEAN			bMassProdTest;

	// Indicate if the driver is unloading or unloaded.
	BOOLEAN			bMptDrvUnload;

	_sema			MPh2c_Sema;
	_timer			MPh2c_timeout_timer;
// Event used to sync H2c for BT control

	BOOLEAN		MptH2cRspEvent;
	BOOLEAN		MptBtC2hEvent;
	BOOLEAN		bMPh2c_timeout;

	/* 8190 PCI does not support NDIS_WORK_ITEM. */
	// Work Item for Mass Production Test.
	//NDIS_WORK_ITEM	MptWorkItem;
//	RT_WORK_ITEM		MptWorkItem;
	// Event used to sync the case unloading driver and MptWorkItem is still in progress.
//	NDIS_EVENT		MptWorkItemEvent;
	// To protect the following variables.
//	NDIS_SPIN_LOCK		MptWorkItemSpinLock;
	// Indicate a MptWorkItem is scheduled and not yet finished.
	BOOLEAN			bMptWorkItemInProgress;
	// An instance which implements function and context of MptWorkItem.
	MPT_WORK_ITEM_HANDLER	CurrMptAct;

	// 1=Start, 0=Stop from UI.
	u32			MptTestStart;
	// _TEST_MODE, defined in MPT_Req2.h
	u32			MptTestItem;
	// Variable needed in each implementation of CurrMptAct.
	u32			MptActType; 	// Type of action performed in CurrMptAct.
	// The Offset of IO operation is depend of MptActType.
	u32			MptIoOffset;
	// The Value of IO operation is depend of MptActType.
	u32			MptIoValue;
	// The RfPath of IO operation is depend of MptActType.
	u32			MptRfPath;

	WIRELESS_MODE		MptWirelessModeToSw;	// Wireless mode to switch.
	u8			MptChannelToSw; 	// Channel to switch.
	u8			MptInitGainToSet; 	// Initial gain to set.
	//u32			bMptAntennaA; 		// TRUE if we want to use antenna A.
	u32			MptBandWidth;		// bandwidth to switch.
	u32			MptRateIndex;		// rate index.
	// Register value kept for Single Carrier Tx test.
	u8			btMpCckTxPower;
	// Register value kept for Single Carrier Tx test.
	u8			btMpOfdmTxPower;
	// For MP Tx Power index
	u8			TxPwrLevel[2];	// rf-A, rf-B

	// Content of RCR Regsiter for Mass Production Test.
	u32			MptRCR;
	// TRUE if we only receive packets with specific pattern.
	BOOLEAN			bMptFilterPattern;
	// Rx OK count, statistics used in Mass Production Test.
	u32			MptRxOkCnt;
	// Rx CRC32 error count, statistics used in Mass Production Test.
	u32			MptRxCrcErrCnt;

	BOOLEAN			bCckContTx;	// TRUE if we are in CCK Continuous Tx test.
	BOOLEAN			bOfdmContTx;	// TRUE if we are in OFDM Continuous Tx test.
	BOOLEAN			bStartContTx; 	// TRUE if we have start Continuous Tx test.
	// TRUE if we are in Single Carrier Tx test.
	BOOLEAN			bSingleCarrier;
	// TRUE if we are in Carrier Suppression Tx Test.
	BOOLEAN			bCarrierSuppression;
	//TRUE if we are in Single Tone Tx test.
	BOOLEAN			bSingleTone;

	// ACK counter asked by K.Y..
	BOOLEAN			bMptEnableAckCounter;
	u32			MptAckCounter;

	// SD3 Willis For 8192S to save 1T/2T RF table for ACUT	Only fro ACUT delete later ~~~!
	//s8		BufOfLines[2][MAX_LINES_HWCONFIG_TXT][MAX_BYTES_LINE_HWCONFIG_TXT];
	//s8			BufOfLines[2][MP_MAX_LINES][MP_MAX_LINES_BYTES];
	//s32			RfReadLine[2];

	u8		APK_bound[2];	//for APK	path A/path B
	BOOLEAN		bMptIndexEven;

	u8			h2cReqNum;
	u8			c2hBuf[20];

	u8          btInBuf[100];
	u32			mptOutLen;
	u8          mptOutBuf[100];
#if defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8730A) || defined(CONFIG_RTL8735B) || defined(CONFIG_RTL8720E) || defined(CONFIG_RTL8730E)
	RT_PMAC_TX_INFO		PMacTxInfo;
	RT_PMAC_PKT_INFO	PMacPktInfo;
#endif
} MPT_CONTEXT, *PMPT_CONTEXT;
//#endif

//#define RTPRIV_IOCTL_MP 					( SIOCIWFIRSTPRIV + 0x17)
enum {
	WRITE_REG = 1,
	READ_REG,
	WRITE_RF,
	READ_RF,
	WRITE_SYS,
	READ_SYS,
	MP_START,
	MP_STOP,
	MP_RATE,
	MP_CHANNEL,
	MP_BANDWIDTH,
	MP_TXPOWER,
	MP_RXPATH,
	MP_ANT_TX,
	MP_ANT_RX,
	MP_CTX,
	MP_HW_TX,
	MP_QUERY,
	MP_ARX,
	MP_PSD,
	MP_PWRTRK,
	MP_THER,
	MP_IOCTL,
	EFUSE_GET,
	EFUSE_SET,
	CONFIG_GET,
	CONFIG_SET,
	MP_RESET_STATS,
	MP_DUMP,
	MP_PHYPARA,
	MP_SetRFPathSwh,
	MP_QueryDrvStats,
	MP_SetBT,
	TEST_CFG,
	MP_NULL,
	MP_GET_TXPOWER_INX,
	MP_SET_PREAMBLE,
	MP_DISABLE_BT_COEXIST,
	MP_PwrCtlDM,
	MP_IQK,
	MP_LCK,
	MP_DPK,
	MP_GET_TSSIDE,
	MP_SET_TSSIDE,
	MP_PHYDM,
	MP_TX_PLCP_USER,
	MP_TX_PLCP_DATA
};

struct mp_priv {
	_adapter *papdater;

	//Testing Flag
	u32 mode;//0 for normal type packet, 1 for loopback packet (16bytes TXCMD)

	u32 prev_fw_state;

	//OID cmd handler
	struct mp_wiparam workparam;
//	u8 act_in_progress;

	//Tx Section
	u8 TID;
	u32 tx_pktcount;
	u32 pktInterval;
	struct mp_tx tx;

	//Rx Section
	u8 rx_pkt_by_mac;
	u32 rx_pktcount;
	u32 rx_crcerrpktcount;
	u32 rx_macpktcount;
	u32 rx_pktloss;

	struct recv_stat rxstat;

	//RF/BB relative
	u8 channel;
	u8 do_dpk;
	u8 bandwidth;
	u8 prime_channel_offset;
	u8 txpoweridx;
	u8 txpoweridx_b;
	u8 rateidx; /*in MGN_RATE format */
	u32 preamble;
//	u8 modem;
	u32 CrystalCap;
//	u32 curr_crystalcap;

	u16 antenna_tx;
	u16 antenna_rx;
//	u8 curr_rfpath;

	/* add for support pmac tx: start */
	u8 ppdu_type;  //0:cck;1:legacy;2:ht_mf;3:ht_gf;4:vht;5:he_su;6:he_er_su;7:he_mu_ofdma;8:he_tb
	u8 er_su_ru_106_en;  //he_er_su:0-242tone;1-106tone
	u16 ru_alloc;
	/* add for support pmac tx: end */

	u8 check_mp_pkt;
	u32 rssi_avg_cal;
	u32 rssi_avg;
	u32 rssi_count;

	u8 bSetTxPower;
	u8 bCCKTxPowerAdjust;
	u8 bFAStatistics;
	u8 mp_dm;
	struct wlan_network mp_network;
	NDIS_802_11_MAC_ADDRESS network_macaddr;

	u8 *pallocated_mp_xmitframe_buf;
	u8 *pmp_xmtframe_buf;
	_queue free_mp_xmitqueue;
	u32 free_mp_xmitframe_cnt;

	MPT_CONTEXT MptCtx;
};

typedef struct _IOCMD_STRUCT_ {
	u8	cmdclass;
	u16	value;
	u8	index;
} IOCMD_STRUCT;

struct rf_reg_param {
	u32 path;
	u32 offset;
	u32 value;
};

struct bb_reg_param {
	u32 offset;
	u32 value;
};
//=======================================================================

#define LOWER 	_TRUE
#define RAISE	_FALSE

/* Hardware Registers */
#if 0
#if 0
#define IOCMD_CTRL_REG			0x102502C0
#define IOCMD_DATA_REG			0x102502C4
#else
#define IOCMD_CTRL_REG			0x10250370
#define IOCMD_DATA_REG			0x10250374
#endif

#define IOCMD_GET_THERMAL_METER		0xFD000028

#define IOCMD_CLASS_BB_RF		0xF0
#define IOCMD_BB_READ_IDX		0x00
#define IOCMD_BB_WRITE_IDX		0x01
#define IOCMD_RF_READ_IDX		0x02
#define IOCMD_RF_WRIT_IDX		0x03
#endif
#define BB_REG_BASE_ADDR		0x800

/* MP variables */
#if 0
#define _2MAC_MODE_	0
#define _LOOPBOOK_MODE_	1
#endif
typedef enum _MP_MODE_ {
	MP_OFF,
	MP_ON,
	MP_ERR,
	MP_CONTINUOUS_TX,
	MP_SINGLE_CARRIER_TX,
	MP_CARRIER_SUPPRISSION_TX,
	MP_SINGLE_TONE_TX,
	MP_PACKET_TX,
	MP_PACKET_RX
} MP_MODE;

typedef enum _TEST_MODE {
	TEST_NONE,
	PACKETS_TX,
	PACKETS_RX,
	CONTINUOUS_TX,
	OFDM_Single_Tone_TX,
	CCK_Carrier_Suppression_TX
} TEST_MODE;

#define MAX_RF_PATH_NUMS	MAX_RF_PATH


extern u8 mpdatarate[NumRates];

/* MP set force data rate base on the definition. */
typedef enum _MPT_RATE_INDEX {
	/* CCK rate. */
	MPT_RATE_1M,	/* 0 */
	MPT_RATE_2M,
	MPT_RATE_55M,
	MPT_RATE_11M,	/* 3 */

	/* OFDM rate. */
	MPT_RATE_6M,	/* 4 */
	MPT_RATE_9M,
	MPT_RATE_12M,
	MPT_RATE_18M,
	MPT_RATE_24M,
	MPT_RATE_36M,
	MPT_RATE_48M,
	MPT_RATE_54M,	/* 11 */

	/* HT rate. */
	MPT_RATE_MCS0,	/* 12 */
	MPT_RATE_MCS1,
	MPT_RATE_MCS2,
	MPT_RATE_MCS3,
	MPT_RATE_MCS4,
	MPT_RATE_MCS5,
	MPT_RATE_MCS6,
	MPT_RATE_MCS7,	/* 19 */
	MPT_RATE_MCS8,
	MPT_RATE_MCS9,
	MPT_RATE_MCS10,
	MPT_RATE_MCS11,
	MPT_RATE_MCS12,
	MPT_RATE_MCS13,
	MPT_RATE_MCS14,
	MPT_RATE_MCS15,	/* 27 */
	MPT_RATE_MCS16,
	MPT_RATE_MCS17, // #29
	MPT_RATE_MCS18,
	MPT_RATE_MCS19,
	MPT_RATE_MCS20,
	MPT_RATE_MCS21,
	MPT_RATE_MCS22, // #34
	MPT_RATE_MCS23,
	MPT_RATE_MCS24,
	MPT_RATE_MCS25,
	MPT_RATE_MCS26,
	MPT_RATE_MCS27, // #39
	MPT_RATE_MCS28, // #40
	MPT_RATE_MCS29, // #41
	MPT_RATE_MCS30, // #42
	MPT_RATE_MCS31, // #43

	/* VHT rate. Total: 20*/
	MPT_RATE_VHT1SS_MCS0 = 100,//  #44
	MPT_RATE_VHT1SS_MCS1, // #
	MPT_RATE_VHT1SS_MCS2,
	MPT_RATE_VHT1SS_MCS3,
	MPT_RATE_VHT1SS_MCS4,
	MPT_RATE_VHT1SS_MCS5,
	MPT_RATE_VHT1SS_MCS6, // #
	MPT_RATE_VHT1SS_MCS7,
	MPT_RATE_VHT1SS_MCS8,
	MPT_RATE_VHT1SS_MCS9, //#53
	MPT_RATE_VHT2SS_MCS0, /* #54 */
	MPT_RATE_VHT2SS_MCS1,
	MPT_RATE_VHT2SS_MCS2,
	MPT_RATE_VHT2SS_MCS3,
	MPT_RATE_VHT2SS_MCS4,
	MPT_RATE_VHT2SS_MCS5,
	MPT_RATE_VHT2SS_MCS6,
	MPT_RATE_VHT2SS_MCS7,
	MPT_RATE_VHT2SS_MCS8,
	MPT_RATE_VHT2SS_MCS9, /* #63 */
	MPT_RATE_VHT3SS_MCS0,
	MPT_RATE_VHT3SS_MCS1,
	MPT_RATE_VHT3SS_MCS2,
	MPT_RATE_VHT3SS_MCS3,
	MPT_RATE_VHT3SS_MCS4,
	MPT_RATE_VHT3SS_MCS5,
	MPT_RATE_VHT3SS_MCS6, /*  #126 */
	MPT_RATE_VHT3SS_MCS7,
	MPT_RATE_VHT3SS_MCS8,
	MPT_RATE_VHT3SS_MCS9,
	MPT_RATE_VHT4SS_MCS0,
	MPT_RATE_VHT4SS_MCS1, /*  #131 */
	MPT_RATE_VHT4SS_MCS2,
	MPT_RATE_VHT4SS_MCS3,
	MPT_RATE_VHT4SS_MCS4,
	MPT_RATE_VHT4SS_MCS5,
	MPT_RATE_VHT4SS_MCS6, /*  #136 */
	MPT_RATE_VHT4SS_MCS7,
	MPT_RATE_VHT4SS_MCS8,
	MPT_RATE_VHT4SS_MCS9,
	MPT_RATE_HE1SS_MCS0,
	MPT_RATE_HE1SS_MCS1,
	MPT_RATE_HE1SS_MCS2,
	MPT_RATE_HE1SS_MCS3,
	MPT_RATE_HE1SS_MCS4,
	MPT_RATE_HE1SS_MCS5,
	MPT_RATE_HE1SS_MCS6,
	MPT_RATE_HE1SS_MCS7,
	MPT_RATE_HE1SS_MCS8,
	MPT_RATE_HE1SS_MCS9,
	MPT_RATE_HE1SS_MCS10,
	MPT_RATE_HE1SS_MCS11,/* 151 */
	MPT_RATE_LAST
} MPT_RATE_E, *PMPT_RATE_E;

#define MAX_TX_PWR_INDEX_N_MODE 64	// 0x3F

typedef enum _POWER_MODE_ {
	POWER_LOW = 0,
	POWER_NORMAL
} POWER_MODE;


#define RX_PKT_BROADCAST	1
#define RX_PKT_DEST_ADDR	2
#define RX_PKT_PHY_MATCH	3

#if 0
#define RPTMaxCount 0x000FFFFF;

// parameter 1 : BitMask
// 	bit 0  : OFDM PPDU
//	bit 1  : OFDM False Alarm
//	bit 2  : OFDM MPDU OK
//	bit 3  : OFDM MPDU Fail
//	bit 4  : CCK PPDU
//	bit 5  : CCK False Alarm
//	bit 6  : CCK MPDU ok
//	bit 7  : CCK MPDU fail
//	bit 8  : HT PPDU counter
//	bit 9  : HT false alarm
//	bit 10 : HT MPDU total
//	bit 11 : HT MPDU OK
//	bit 12 : HT MPDU fail
//	bit 15 : RX full drop
typedef enum _RXPHY_BITMASK_ {
	OFDM_PPDU_BIT = 0,
	OFDM_FALSE_BIT,
	OFDM_MPDU_OK_BIT,
	OFDM_MPDU_FAIL_BIT,
	CCK_PPDU_BIT,
	CCK_FALSE_BIT,
	CCK_MPDU_OK_BIT,
	CCK_MPDU_FAIL_BIT,
	HT_PPDU_BIT,
	HT_FALSE_BIT,
	HT_MPDU_BIT,
	HT_MPDU_OK_BIT,
	HT_MPDU_FAIL_BIT,
} RXPHY_BITMASK;
#endif

typedef enum _ENCRY_CTRL_STATE_ {
	HW_CONTROL,		//hw encryption& decryption
	SW_CONTROL,		//sw encryption& decryption
	HW_ENCRY_SW_DECRY,	//hw encryption & sw decryption
	SW_ENCRY_HW_DECRY	//sw encryption & hw decryption
} ENCRY_CTRL_STATE;

typedef enum _PREAMBLE {
	Long_Preamble	= 0x01,
	Short_Preamble		,
	Long_GI 			,
	Short_GI
} PREAMBLE;

typedef enum	_MPT_TXPWR_DEF {
	MPT_CCK,
	MPT_OFDM, // L and HT OFDM
	MPT_OFDM_AND_HT,
	MPT_HT,
	MPT_VHT
} MPT_TXPWR_DEF;

typedef enum _OFDM_TX_MODE {
	OFDM_ALL_OFF		= 0,
	OFDM_ContinuousTx	= 1,
	OFDM_SingleCarrier	= 2,
	OFDM_SingleTone 	= 4,
} OFDM_TX_MODE;

typedef enum _MP_PPDU_TYPE {
	RTW_MP_TYPE_CCK = 0,
	RTW_MP_TYPE_LEGACY,
	RTW_MP_TYPE_HT_MF,
	RTW_MP_TYPE_HT_GF,
	RTW_MP_TYPE_VHT,
	RTW_MP_TYPE_HE_SU,
	RTW_MP_TYPE_HE_ER_SU,
	RTW_MP_TYPE_HE_MU_OFDMA,
	RTW_MP_TYPE_HE_TB
} PPDU_TYPE;

#define PPDU_TYPE_STR(idx)\
	(idx == RTW_MP_TYPE_CCK) ? "CCK" :\
	(idx == RTW_MP_TYPE_LEGACY) ? "LEGACY" :\
	(idx == RTW_MP_TYPE_HT_MF) ? "HT_MF" :\
	(idx == RTW_MP_TYPE_HT_GF) ? "HT_GF" :\
	(idx == RTW_MP_TYPE_VHT) ? "VHT" :\
	(idx == RTW_MP_TYPE_HE_SU) ? "HE_SU" :\
	(idx == RTW_MP_TYPE_HE_ER_SU) ? "HE_ER_SU" :\
	(idx == RTW_MP_TYPE_HE_MU_OFDMA) ? "HE_MU" :\
	(idx == RTW_MP_TYPE_HE_TB) ? "HE_TB" :\
	"UNknow"

#define MPT_IS_CCK_RATE(_value)		(MPT_RATE_1M <= _value && _value <= MPT_RATE_11M)
#define MPT_IS_OFDM_RATE(_value)	(MPT_RATE_6M <= _value && _value <= MPT_RATE_54M)
#define MPT_IS_HT_RATE(_value)		(MPT_RATE_MCS0 <= _value && _value <= MPT_RATE_MCS31)
#define MPT_IS_HT_1S_RATE(_value)	(MPT_RATE_MCS0 <= _value && _value <= MPT_RATE_MCS7)
#define MPT_IS_HT_2S_RATE(_value)	(MPT_RATE_MCS8 <= _value && _value <= MPT_RATE_MCS15)
#define MPT_IS_HT_3S_RATE(_value)	(MPT_RATE_MCS16 <= _value && _value <= MPT_RATE_MCS23)
#define MPT_IS_HT_4S_RATE(_value)	(MPT_RATE_MCS24 <= _value && _value <= MPT_RATE_MCS31)

#define MPT_IS_VHT_RATE(_value)		(MPT_RATE_VHT1SS_MCS0 <= _value && _value <= MPT_RATE_VHT4SS_MCS9)
#define MPT_IS_VHT_1S_RATE(_value)	(MPT_RATE_VHT1SS_MCS0 <= _value && _value <= MPT_RATE_VHT1SS_MCS9)
#define MPT_IS_VHT_2S_RATE(_value)	(MPT_RATE_VHT2SS_MCS0 <= _value && _value <= MPT_RATE_VHT2SS_MCS9)
#define MPT_IS_VHT_3S_RATE(_value)	(MPT_RATE_VHT3SS_MCS0 <= _value && _value <= MPT_RATE_VHT3SS_MCS9)
#define MPT_IS_VHT_4S_RATE(_value)	(MPT_RATE_VHT4SS_MCS0 <= _value && _value <= MPT_RATE_VHT4SS_MCS9)

#define MPT_IS_HE_1S_RATE(_value)	(MPT_RATE_HE1SS_MCS0 <= _value && _value <= MPT_RATE_HE1SS_MCS11)

#define MPT_IS_2SS_RATE(_rate) ((MPT_RATE_MCS8 <= _rate && _rate <= MPT_RATE_MCS15) || \
	(MPT_RATE_VHT2SS_MCS0 <= _rate && _rate <= MPT_RATE_VHT2SS_MCS9))
#define MPT_IS_3SS_RATE(_rate) ((MPT_RATE_MCS16 <= _rate && _rate <= MPT_RATE_MCS23) || \
	(MPT_RATE_VHT3SS_MCS0 <= _rate && _rate <= MPT_RATE_VHT3SS_MCS9))
#define MPT_IS_4SS_RATE(_rate) ((MPT_RATE_MCS24 <= _rate && _rate <= MPT_RATE_MCS31) || \
	(MPT_RATE_VHT4SS_MCS0 <= _rate && _rate <= MPT_RATE_VHT4SS_MCS9))

//=======================================================================
//extern struct mp_xmit_frame *alloc_mp_xmitframe(struct mp_priv *pmp_priv);
//extern int free_mp_xmitframe(struct xmit_priv *pxmitpriv, struct mp_xmit_frame *pmp_xmitframe);

extern s32 init_mp_priv(_adapter *padapter);
extern void free_mp_priv(struct mp_priv *pmp_priv);
extern s32 wifi_hal_mp_init(_adapter *padapter, u8 Channel);
extern void MPT_DeInitAdapter(_adapter *padapter);
extern s32 mp_start_test(_adapter *padapter);
extern void mp_stop_test(_adapter *padapter);

//=======================================================================
//extern void	IQCalibrateBcut(_adapter * pAdapter);

//extern u32	bb_reg_read(_adapter * Adapter, u16 offset);
//extern u8	bb_reg_write(_adapter * Adapter, u16 offset, u32 value);
//extern u32	rf_reg_read(_adapter * Adapter, u8 path, u8 offset);
//extern u8	rf_reg_write(_adapter * Adapter, u8 path, u8 offset, u32 value);

//extern u32	get_bb_reg(_adapter * Adapter, u16 offset, u32 bitmask);
//extern u8	set_bb_reg(_adapter * Adapter, u16 offset, u32 bitmask, u32 value);
//extern u32	get_rf_reg(_adapter * Adapter, u8 path, u8 offset, u32 bitmask);
//extern u8	set_rf_reg(_adapter * Adapter, u8 path, u8 offset, u32 bitmask, u32 value);

extern u32 _read_rfreg(_adapter *padapter, u8 rfpath, u32 addr, u32 bitmask);
extern void _write_rfreg(_adapter *padapter, u8 rfpath, u32 addr, u32 bitmask, u32 val);

extern u32 read_macreg(_adapter *padapter, u32 addr, u32 sz);
extern void write_macreg(_adapter *padapter, u32 addr, u32 val, u32 sz);
extern u32 read_bbreg(_adapter *padapter, u32 addr, u32 bitmask);
extern void write_bbreg(_adapter *padapter, u32 addr, u32 bitmask, u32 val);
extern u32 read_rfreg(_adapter *padapter, u8 rfpath, u32 addr);
extern void write_rfreg(_adapter *padapter, u8 rfpath, u32 addr, u32 val);

extern void wifi_hal_mp_resetbb(_adapter *pAdapter);
extern void wifi_hal_mp_settxedca(_adapter *pAdapter);
extern void wifi_hal_mp_rx_cca(_adapter *pAdapter, bool cca_en);
extern int wifi_hal_mp_arx(struct net_device *dev, struct rtw_point *wrqu);
extern void wifi_hal_mp_txdesc(PADAPTER adapter, struct mp_priv *pmp_priv);
extern void wifi_hal_mp_dump(_adapter *padapter, u32 column);
extern void wifi_hal_mp_rx_filter(_adapter *padapter);
extern void wifi_hal_mp_dpk(struct net_device *dev, struct rtw_point *wrqu);
extern void PHY_SetRFPathSwitch(PADAPTER pAdapter, BOOLEAN is_main);

extern void	SetChannel(_adapter *pAdapter);
extern void	SetBandwidth(_adapter *pAdapter);
extern void	SetTxPower(_adapter *pAdapter);
extern void	SetAntennaPathPower(_adapter *pAdapter);
//extern void	SetTxAGCOffset(_adapter * pAdapter, u32 ulTxAGCOffset);
extern void	SetDataRate(_adapter *pAdapter);

extern void	SetAntenna(_adapter *pAdapter);

//extern void	SetCrystalCap(_adapter * pAdapter);

extern s32	SetThermalMeter(_adapter *pAdapter, u8 target_ther);
extern void	GetThermalMeter(_adapter *pAdapter, u8 *value);

extern void	SetContinuousTx(_adapter *pAdapter, u8 bStart);
extern void	SetSingleCarrierTx(_adapter *pAdapter, u8 bStart);
extern void	SetSingleToneTx(_adapter *pAdapter, u8 bStart);
extern void	SetCarrierSuppressionTx(_adapter *pAdapter, u8 bStart);
extern void PhySetTxPowerLevel(_adapter *pAdapter);

extern void	fill_txdesc_for_mp(_adapter *padapter, struct tx_desc *ptxdesc);
extern void	SetPacketTx(_adapter *padapter);
extern void	SetPacketRx(_adapter *pAdapter, u8 bStartRx);

extern void	ResetPhyRxPktCount(_adapter *pAdapter);
extern u32	GetPhyRxPktReceived(_adapter *pAdapter);
extern u32	GetPhyRxPktCRC32Error(_adapter *pAdapter);

extern s32	SetPowerTracking(_adapter *padapter, u8 enable);
extern void	GetPowerTracking(_adapter *padapter, u8 *enable);

extern u32	wifi_hal_mp_query_psd(_adapter *pAdapter, u8 *data, int data_len);
extern void wifi_hal_mp_crystal_cap(_adapter *pAdapter, u32 CrystalCapVal);


extern void Hal_SetAntenna(_adapter *pAdapter);
extern void Hal_SetBandwidth(_adapter *pAdapter);

extern void Hal_SetTxPower(_adapter *pAdapter);
extern void Hal_SetCarrierSuppressionTx(_adapter *pAdapter, u8 bStart);
extern void Hal_SetSingleToneTx(_adapter *pAdapter, u8 bStart);
extern void Hal_SetSingleCarrierTx(_adapter *pAdapter, u8 bStart);
extern void Hal_SetContinuousTx(_adapter *pAdapter, u8 bStart);
extern void Hal_SetBandwidth(_adapter *pAdapter);

extern void Hal_SetDataRate(_adapter *pAdapter);
extern void Hal_SetChannel(_adapter *pAdapter);
extern void Hal_SetAntennaPathPower(_adapter *pAdapter);
extern s32 Hal_SetThermalMeter(_adapter *pAdapter, u8 target_ther);
extern s32 Hal_SetPowerTracking(_adapter *padapter, u8 enable);
extern void Hal_GetPowerTracking(_adapter *padapter, u8 *enable);
extern void Hal_GetThermalMeter(_adapter *pAdapter, u8 *value);
extern void Hal_mpt_SwitchRfSetting(_adapter *pAdapter);
extern void Hal_MPT_CCKTxPowerAdjust(_adapter *Adapter);
extern void Hal_MPT_CCKTxPowerAdjustbyIndex(_adapter *pAdapter, BOOLEAN beven);
extern void Hal_TriggerRFThermalMeter(_adapter *pAdapter);
extern u8 Hal_ReadRFThermalMeter(_adapter *pAdapter);
extern void Hal_SetCCKContinuousTx(_adapter *pAdapter, u8 bStart);
extern void Hal_SetOFDMContinuousTx(_adapter *pAdapter, u8 bStart);
extern void _rtw_mp_xmit_priv(struct xmit_priv *pxmitpriv);
extern void MP_PHY_SetRFPathSwitch(_adapter *pAdapter, BOOLEAN bMain);
extern u32 mpt_ProQueryCalTxPower(_adapter *pAdapter, u8 RfPath);
extern void MPT_PwrCtlDM(PADAPTER padapter, u32 bstart);
extern u8 MgntRateToMPTRate(u8 rate);
#endif //_RTW_MP_H_

