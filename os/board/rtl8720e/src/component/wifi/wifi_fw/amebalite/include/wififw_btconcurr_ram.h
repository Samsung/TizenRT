#ifndef __WIFIFW_BTCONCURR_RAM_H__
#define __WIFIFW_BTCONCURR_RAM_H__

#ifndef __ASSEMBLY__
/*--------------------Define --------------------------------------------*/
#define DefaultTBTTOnPeriod         0xB
#define MaxTBTTOnPeriod             0x2A
#define MinTBTTOnPeriod             0x3
#define NULL_EARLY                  0x3
#define BTC_BCN_EARLY               DEFAULT_PS_BCN_EARLY + 1
#define BTC_FLEX_EXT_NUM            3
#define BTC_IQK_WAIT_CNT            6
#define BTC_AUTO_SLOT_TIME_THD      30

//for MAILBOX 0x11A4~0x11B7, OUT->WL2BT, IN->BT2WL
#define REG_MAILBOX_WL2BT_DATA0     0x0
#define REG_MAILBOX_WL2BT_DATA1     0x4
#define REG_MAILBOX_WL2BT_RDY       0x8  //bit[0] : OutReady, bit[1] : OutEmptyIntEn

#define REG_MAILBOX_BT2WL_DATA0     0xC
#define REG_MAILBOX_BT2WL_DATA1     0x10
#define REG_MAILBOX_BT2WL_RDY       0xA  //bit[0] : InReady, bit[1] : InRdyIntEn

#define BTC_PAN_DURATION_DEFAULT    50
#define BTC_PAN_DURATION_MAX        80
#define BTC_PAN_DURATION_MIN        30
#define BTC_PSPOLL_RTY_TIME         1

#define BTC_INTERVAL_UNDER_100      0
//TdmaIntervalMode=0: skip n-1 beacons , TdmaInterval: n
/*For example, BeaconInterval=40ms, we may set h2c 0x69 = 0xB 0x03,
so we omit 40ms(bcn early) and 80ms(bcn early) but noly care 120ms(bcn early) as slot.*/
#define BTC_INTERVAL_OVER_100       2
//TdmaIntervalMode=2: insert n 100ms , TdmaInterval: n,
/*For example, BeaconInterval=370ms, we may set h2c 0x69 = 0xB 0x83,
so we use 0ms 100ms(timer) 200ms(timer) 300ms(timer) and 370ms(bcn early) as slots. */
#define BTC_INTERVAL_SHORT          3
//TdmaIntervalMode=3: insert n 50ms , TdmaInterval: n,
/*For example, BeaconInterval=100ms, we may set h2c 0x69 = 0xB 0xc1,
so we use 0ms 50ms(timer) and 100ms(bcn early) as slots. */
#define BTC_DEFAULT_INTERVAL        102
#define BTC_SHORT_INTERVAL          51
#define BTC_MULTI_SLOT_TIMER        REG_TC3_CTRL

#define LEAK_AP_LIMIT               5
#define BTC_LEAK_AP_LIMIT           LEAK_AP_LIMIT
#define BTC_LEAK_AP_EXTEND          5

#define BTC_WAITBCN_TIMER           REG_TC4_CTRL
#define BTC_RETAIN_BT_SLOT_LEN      120

#define BTC_TIMER_MASK						REG_TC6_CTRL

#define SLOT_WIFI								0
#define SLOT_BT                     			1
#define SLOT_DEFAULT                		3
#define SEL_COEX_TABLE_1            		0x03   //0x6CC[1:0]=2'b11 -> select 0x6C0
#define SEL_COEX_TABLE_2            		0xFC   //0x6CC[1:0]=2'b00 -> select 0x6C4

#define BTC_TDMA0_3_PIN						_PB_2	//ALite MAC debugport[10]
#define BTC_TDMA1_4_PIN						_PB_3	//ALite MAC debugport[11]
#define BTC_TDMA2_5_PIN						_PA_15	//ALite MAC debugport[15]
#define BTC_TBTT_PIN						_PA_16	//ALite MAC debugport[16]

#define MAX_RANDOM_INDEX					50
/*--------------------Define Enum---------------------------------------*/
typedef enum _BTC_Mailbox_CMD_ {
	_BT_FW_PATCH                      			= 0x0a,
	_WL_OP_MODE                       			= 0x11,
	_WIFI_RF_CALIBRATION_EN           		= 0x15,
	_FORCE_BT_TXPWR                   		= 0x17,
	_BT_IGNORE_WLAN_ACT               		= 0x1b,
	_PSD_CONTROL                      			= 0x21,
	_BT_INFO_REPORT                   		= 0x23,
	_BT_PSD_REPORT                    		= 0x24,
	_BT_INFO_REPORT_BY_ITSELF         	= 0x27,
	_BT_INFO_REPORT_CONTROL           		= 0x28,
	_BT_SET_TXRETRY_REPORT_PARAMETER  	= 0x29,
	_BT_SET_PTA_TABLE                 		= 0x2A,
	_BT_MP_REPORT                     			= 0x30,
	_BT_PSD_MODE                      			= 0x31,
	_BT_SET_BT_LNA_CONSTRAINT         	= 0x32,
	_BT_LOOPBACK                      			= 0x33,
	_BT_CALIBRATION                   		= 0x34,
	_INTERNAL_SWITCH                  		= 0x35,
	_BT_CHECK_WLAN_STATE              		= 0x36,
	_CHANGE_WIFI_RF_STATE             		= 0x37,
	_BT_PAGE_SCAN_INTERVAL            		= 0x38,
	_BT_AUTO_SLOT                     			= 0x39,
	_MAILBOX_EN                       			= 0x40,
	_BT_CALIBRATION_FAIL              		= 0x41,
	_WIFI_TDMA_IN_BT_SLOT             		= 0x42,
	_WIFI_RPT_TDMA                    		= 0x43
} BTC_Mailbox_CMD, *PBTC_Mailbox_CMD;

typedef struct _BTC_PsTdma_Parm_ {
	u8		WifiActive			: 1;
	u8		TxNull1				: 1;
	u8  	TxNull1ok 			: 1;
	u8  	BTCalibration		: 1;
	u8    	BTChangeType		: 1;
	u8   	BTConnect 	 		: 1;
	u8   	BTRptConnect		: 1;
	u8   	AutoSlotBackup	: 1;

	u8  	BTCalibrationPeriod;

	u8  	BTCTCount			: 4;
	u8  	BTCTCTh				: 4;

	u32 	AutoSlotTimeBackup;
	u8  	AutoSlotCnt;
	u8   	AutoSlotFailCnt;
	u8   	A2DPEmptyThd;
	u8   	BCNIvl;
	u8   	HalfBCNIvl;
	u8   	WLANPeriodCount;
	u8    	NullExtensionCount;
	u8   	NAVTemp;
	u8    	ValueBackup2;

	u8  	WLANOnPeriod		: 1;
	u8   	DynamicSlotState	: 2;
	u8    	PanProtection		: 1;
	u8   	TdmaChange			: 1;
	u8   	WLPeriodCntIncrease	: 1;
	u8   	UnderBCNProtection	: 1;
	u8   	WifiWindowSlotStart : 1;

	u32 	BtSlotStartTime;
	u8   	H2cTdmaValue[7];
	u32  	DACSwingLevel;
	u32 	DACSwingInitValue;
	u8   	RAOffsetEn;
	u8   	RAOffset;
	u8  	RAOffsetMCS7;
	u8   	RAOffsetMCS6;
	u8   	RAOffsetMCS5;
	u32  	HIDPTATable;
	u32 	HIDPTATableBackUp;
	u8  	HIDSlotCnt;
	u8  	HIDPTATableInterval;
	u32 	HIDPTATableNew;

	u8   	PanDurUserMode		: 1;
	u8  	PanDuration				: 7;

	u8   	TxNullCnt;
	u8   	TxNullRtyCnt;
	u8   	TxNullOkCnt;

	u8   	TdmaLeakAPModeDisable	: 1;
	u8   	LeakAPCount           		: 7;

	u8  	LeakAPRxCount;
	u8   	LeakAPCountForObservation;

	u8   	TdmaIntervalMode		: 2;
	u8   	TdmaInterval       	: 6;
	u8   	TdmaIntervalCnt    	: 7;
	u8  	TdmaTimerFlag      	: 1;

	u8    	TDMASlotToggleCount;
	u8   	TbttISRCnt;    /*Count the TBTT function called by TBTT ISR but not TDMA.*/
	u8    	BtSlotLen[20];
	u8    	BtSlotCnt;
	u8   	RandomWLSlotCnt;
} BTC_PsTdma_Parm, *PBTC_PsTdma_Parm;

typedef struct _BTC_Mailbox_Parm_ {
	u8   	pBuffer[8];
} BTC_Mailbox_Parm, *PBTC_Mailbox_Parm;

typedef struct _BTC_Scoreboard_Parm_ {
	u32  	State;
} BTC_Scoreboard_Parm, *PBTC_Scoreboard_Parm;

/*------------------H2C Define Struct---------------------------------------*/
/*H2C Index: 0x60*/
typedef struct _H2C_BT_Tdma_Parm_ {
	/* B0 */
	u8  	Enable						: 1;
	u8    	ValueofReg870InBT       	: 1;    /* 1:0x870 = 0x300, 0:0x870 = 0x310*/
	u8    	HIDSlotToggle           	: 1;
	u8   	ValueOfReg870           	: 1;    /* when disable 1:0x870 = 0x310, 0:0x870 = 0x300*/
	/* when enable 1: 0x860 = 0x110, 0:0x860 = 0x210*/
	u8   	AutoWakeUp              	: 1;    /* when set 1, use null(0) and null(1) packet*/
	u8   	NoPS                    		: 1;    /* when set 1, in BT slot will pause WiFi Tx*/
	u8   	AllowBTHighPriority		: 1;
	u8   	ValueofReg870InWIFI		: 1;    /* 1:0x870 = 0x300, 0:0x870 = 0x310*/
	/* B1 */
	u8  	TBTTOnPeriod;
	/* B2 */
	u8  	B2Rsvd0                   		: 2;
	u8   	PanEnable               		: 1;
	u8   	B2Rsvd1                  		: 1;
	u8   	WifiWindowSlot          		: 1;
	u8   	B2Rsvd2                   		: 3;
	/* B3 */
	u8  	NoTxPause               		: 1;
	u8  	Valueof778InWIFI        	: 1;
	u8   	Valueof778InWIFIOnSCO	: 1;
	u8   	ValueofReg778           	: 1;    /* when disable 1: 778=3, 0: 778=1 */
	/* when enable 1:allow 32k, 0:not allow 32k */
	u8  	SCOOption               		: 1;
	u8   	ValueofReg860InWIFI		: 1;
	u8   	TwoAntenna              	: 1;
	u8  	ReduceWIFIPower         	: 1;
	/* B4 */
	u8 		ExtraDecisionOf778      	: 1;
	u8   	CCKPriorityToggle       	: 1;
	u8    	WifiAutoSlot            	: 1;
	u8    	CTS2Self                		: 1;
	u8  	ChangeCoexTable         	: 1;
	u8  	LongNAV                 		: 1;    /* use large NAV to protect BT slot */
	u8   	DynamicSlot             	: 1;
	u8   	ValueofReg778B1InBT		: 1;    /* 1:778[1]=1, 0:778[1]=0 in bt */
	/* B5 */
	u8  	Rsvd5;
	/* B6 */
	u8 		Rsvd6;
} H2C_BT_Tdma_Parm, *PH2C_BT_Tdma_Parm;

//H2C Index: 0x61
typedef struct _H2C_BT_Info_Parm_ {
	u8		Trigger		: 1;
	u8   	Rsvd0 		: 7;
	/* B1-B6 */
	u8   	Rsvd1[6];
} H2C_BT_Info_Parm, *PH2C_BT_Info_Parm;

/* H2C Index: 0x62 */
typedef struct _H2C_Force_BT_TxPower_Parm_ {
	u8 		PowerIndex;
	//B1-B6
	u8   	Rsvd1[6];
} H2C_Force_BT_TxPower_Parm, *PH2C_Force_BT_TxPower_Parm;

/* H2C Index: 0x63 */
typedef struct _H2C_BT_Ignore_WLACT_Parm_ {
	u8  	Enable		: 1;
	u8   	Rsvd0 		: 7;
	//B1-B6
	u8  	Rsvd1[6];
} H2C_BT_Ignore_WLACT_Parm, *PH2C_BT_Ignore_WLACT_Parm;

/* H2C Index: 0x64 */
typedef struct _H2C_DAC_Swing_Value_Parm_ {
	u8		Value;
	//B1-B6
	u8		Rsvd1[6];
} H2C_DAC_Swing_Value_Parm, *PH2C_DAC_Swing_Value_Parm;

/* H2C Index: 0x65 */
typedef struct _H2C_Ant_Sel_Reverse_Parm_ {
	u8 		AntReverse;
	u8 		SwitchOutside;
	//B2-B6
	u8 		Rsvd2[5];
} H2C_Ant_Sel_Reverse_Parm, *PH2C_Ant_Sel_Reverse_Parm;

/* H2C Index: 0x66 */
typedef struct _H2C_WL_Opmode_Parm_ {
	u8		Opmode;
	u8  	ChannelIndex;
	u8   	BandWidth;
	//B3-B6
	u8   	Rsvd3[4];
} H2C_WL_Opmode_Parm, *PH2C_WL_Opmode_Parm;

/* H2C Index: 0x6C */
typedef struct _H2C_BT_Page_Scan_Interval_Parm_ {
	u8  	LSB_Interval;
	u8   	MSB_Interval;
	//B2-B6
	u8   	Rsvd2[5];
} H2C_BT_Page_Scan_Interval_Parm, *PH2C_BT_Page_Scan_Interval_Parm;

/* H2C Index: 0x6D */
typedef struct _H2C_WL_Calibration_Parm_ {
	u8   	WLCalibrationEn	: 1;
	u8    	Rsvd0            		: 7;
	//B1-B6
	u8   	Rsvd1[6];
} H2C_WL_Calibration_Parm, *PH2C_WL_Calibration_Parm;

/* H2C Index: 0x6E */
typedef struct _H2C_GNTBT_Value_Parm_ {
	u8  	GntBtValue			: 1;
	u8    	Rsvd0          		: 7;
	//B1-B6
	u8    	Rsvd1[6];
} H2C_GNTBT_Value_Parm, *PH2C_GNTBT_Value_Parm;

/* H2C Index: 0x6F */
typedef struct _H2C_BT_OnlyTest_Parm_ {
	u8 		BTOnlyTest      	: 1;
	u8   	Rsvd0           		: 7;
	//B1-B6
	u8   	Rsvd1[6];
} H2C_BT_OnlyTest_Parm, *PH2C_BT_OnlyTest_Parm;

/* H2C Index: 0x70 */
typedef struct _H2C_BT_Init_Parm_ {
	u8   	Enhance3wireMode	: 1;
	u8   	HighPowerPAMode		: 1;
	u8   	DirectBtMode		: 1;
	u8   	Rsvd0             	: 5;
	//B1-B6
	u8    	Rsvd1[6];
} H2C_BT_Init_Parm, *PH2C_BT_Init_Parm;

/* H2C Index: 0x71 */
typedef struct _H2C_WL_PortID_Parm_ {
	u8   	PortID         		: 3;
	u8   	Rsvd0          		: 5;
	//B1-B6
	u8   	Rsvd1[6];
} H2C_WL_PortID_Parm, *PH2C_WL_PortID_Parm;

typedef enum _BTC_Mailbox_Status_ {
	_BT_ACTIVE_OUTREADY_NOT_ZERO    = BIT0,
	_BT_NON_ACTIVE                  = BIT1,
	_BT_PATCH_TIMEOUT               = BIT2,
	_BT_CONTENT_ERROR               = BIT3,
	_BT_CONTENT_CHECKSUM_ERROR      = BIT4,
} BTC_Mailbox_Status, *PBTC_Mailbox_Status;

typedef enum _BTC_Scoreboard_CMD_ {
	BTC_SB_BT_ENABLE        = BIT1,
	BTC_SB_BT_A2DP_STATE    = BIT4,
	BTC_SB_BT_UNDER_IQK     = BIT5,
	BTC_SB_BT_IQK_REQ       = BIT6,
	BTC_SB_BT_LPS           = BIT7,
	BTC_SB_BT_HID_NOTIFY    = BIT8,
	BTC_SB_BT_XTAL_WRITE    = BIT15,
	BTC_SB_BT_EFUSE         = BIT30
} BTC_Scoreboard_CMD, *PBTC_Scoreboard_CMD;

typedef enum _BTC_BT_PMC_State_ {
	_BT_IDLE_MODE           = 0,
	_BT_XTAL_EN_MODE        = 1,
	_BT_LDO_EN_MODE         = 2,
	_BT_PLL_EN_MODE         = 3,
	_BT_ACTIVE_MODE         = 4,
	_BT_LDO_DIS_MODE        = 5,
	_BT_LOW_POWER_MODE      = 6,
	_BT_CAL_EN_MODE         = 10,
	_BT_CALIBRATE_MODE      = 11,
	_BT_POWER_DOWN_MODE     = 12,
	_BT_LP2ACTIVE_MODE      = 13,
	_BT_AFELDO_MODE         = 14
} BTC_BT_PMC_State, *PBTC_BT_PMC_State;

typedef enum _BTC_PWRBitOverWrite_State_ {
	DisablePWRBitOverWrite   = 0,
	EnablePWRBitOverWrite    = 1,
	OverWritePWRBit0         = 2,
	OverWritePWRBit1         = 3
} BTC_PWRBitOverWrite_State, *PBTC_PWRBitOverWrite_State;

typedef enum _BTC_MultiPort_Role_ {
	PORT0      = 0,
	PORT1      = 1,
	PORT2      = 2,
	PORT3      = 3,
	PORT4      = 4
} BTC_MultiPort_Role, *PBTC_MultiPort_Role;

typedef enum _BTC_TdmaFlexibleExtension_State_ {
	_BT_TDMA_FLEX_IDLE_STATE = 0,
	_BT_TDMA_FLEX_NULL_STATE = 1
} BTC_TdmaFlexibleExtension_State, *PBTC_TdmaFlexibleExtension_State;

typedef enum _BTC_AOAC_Switch_State_ {
	BTC_AOAC_RF_OFF           = 0,
	BTC_AOAC_RF_ON            = 1,
	BTC_AOAC_BT_START_IQK     = 2,
	BTC_AOAC_BT_STOP_IQK      = 3
} BTC_AOAC_Switch_State, *PBTC_AOAC_Switch_State;

typedef enum _BTC_Function_Caller_ {
	TBTT_TDMA                 = 0,
	TBTT_ISR                  = 1,
	BCNEARLY_TDMA             = 2,
	BCNEARLY_ISR              = 3
} BTC_Function_Caller, *PBTC_Function_Caller;

/* C2H Index: 0x9 */
typedef enum _C2H_BT_INFO_ {
	RSP_WIFI_FW             = 0,
	RSP_WIFI_REQ            = 1,
	RSP_BT_CHANGE_STS       = 2,
	RSP_BT_IQK              = 3,
	RSP_BT_SB               = 4,
	RSP_BT_RPT_TDMA         = 5,
	RSP_BT_PSD_REPORT       = 6,
	RSP_BT_SLOT_LENGTH      = 7
} C2H_BT_INFO, *PC2H_BT_INFO;

/* H2C Index: 0x69 */
typedef enum _H2C_BTWIFI_CTRL_ {
	BT_SET_TXRETRY_REPORT   = 0,
	BT_SET_PTATABLE         = 1,
	BT_SET_PSD_MODE         = 2,
	BT_SET_LNA_CONSTRAINT   = 3,
	BT_MAILBOX_LB_TEST      = 4,
	BT_H2C2H_LB_TEST        = 5,
	BT_RA_RETRYPENALTY      = 6,
	BT_HID_SLOT_TABLE       = 7,
	BT_WIFI_RQT_INFO        = 8,
	BT_SET_PAN_DURATION     = 9,
	BT_SET_A2DP_EMPTY_THD   = 10,
	BT_SET_BCN_INTERVAL     = 11,
	BT_SET_LEAK_AP_MODE     = 12,
	BT_HID_SLOT_TABLE_2     = 13
} H2C_BTWIFI_CTRL, *PH2C_BTWIFI_CTRL;

typedef enum _GNTBT_CTRL_ {
	GNT_HW_PTA        = 0,
	GNT_SW_LOW        = 1,
	GNT_SW_HIGH       = 2
} GNTBT_CTRL, *PGNTBT_CTRL;

/*--------------------Define MACRO--------------------------------------*/
#define MAILBOX_MAX_LENGTH    7
/* Mailbox MP Rpt OPCODE */
#define BTMPRPT_OPCODE_WRITE_ADDR           0x0C
#define BTMPRPT_OPCODE_WRITE_VALUE          0x0D

/*--------------------Function declaration---------------------------------*/
extern void H2CHDL_BTInfo_8720E(u8  *pbuf);
extern void H2CHDL_ForceBTTxpwr_8720E(u8  *pbuf);
extern void H2CHDL_BTIgnoreWlanAct_8720E(u8   *pbuf);
extern void H2CHDL_AntSelReverse_8720E(u8   *pbuf);
extern void H2CHDL_WLOpmode_8720E(u8   *pbuf);
extern void H2CHDL_BTMpH2C_8720E(u8   *pbuf);
extern void H2CHDL_BTControl_8720E(u8   *pbuf);
extern void H2CHDL_BT_Page_Scan_Interval_8720E(u8 *pbuf);
extern void H2CHDL_GNT_BT_Ctrl_8720E(u8 *pbuf);
extern void H2CHDL_BT_Only_Test_8720E(u8 *pbuf);
extern void H2CHDL_WL_Port_ID_8720E(u8 *pbuf);
extern void C2HBTLoopback_8720E(u8  *pbuf);
extern void C2HBTMpRpt_8720E(u8  idx, u8  status, u8  req_num);
extern void C2HBTMailBoxStatus_8720E(u8  status);
extern void InitBTCoexTimer_8720E(u8 duration);
extern void Write778_8720E(u8 value);
extern void TdmaChangeCoexTable_8720E(u8  slot);
extern void BTNullSetting_8720E(u8  bPowerBit, u16 offset, u8  lsb, u8  msb);
extern void InitBTMailbox_8720E(void);
extern BOOLEAN FillMailbox_8720E(u8 cmd, u8 length, u8 *pparm);
extern void WL2BTMailboxSend_8720E(u8 cmd, u8 length, u8 *pparm);
extern void BT2WLMailboxINTHDL_8720E(void);
extern void BT2WLMailboxINTCheck_8720E(void);
extern void H2CHDL_BTPSTDMA_8720E(u8  *pbuf);
extern void BTC_PsTdmaActionDelay_8720E(void);
extern void BTC_PsTdmaAction_8720E(u8  *pbuf);
extern void H2CHDL_BTWifiCtrl_8720E(u8  *pbuf);
extern void H2CHDL_WLCalibration_8720E(u8 *pbuf);
extern void H2CHDL_BT_Init_Param_8720E(u8 *pbuf);
extern void C2HBTInfo_8720E(u8  option);
extern void C2HWlanInfo_8720E(void);
extern void InitBTCoex_8720E(void);
extern void BT_TDMA_Slot_Operation_0and3_8720E(u8  slot_num);
extern void BT_TDMA_Slot_Operation_1and4_8720E(u8  slot_num);
extern void BT_TDMA_Slot_Operation_2and5_8720E(u8  slot_num);
extern u8 DynamicSlotDecision_8720E(void);
extern u8 BTypeTDMAMultiSlots_8720E(void);
extern void BTypeTDMABCNEarly_8720E(u8  type);
extern void BTypeTDMATBTThdl_8720E(u8  type);
extern void BTypeTDMALeakAPOperation_8720E(void);
extern void BTypeTDMATimeOuthdl_8720E(void);
extern void BTC_RestoreDefaultCoexTable_8720E(void);
extern void BTC_PWRBitOverWrite_8720E(u8  PortNum, u8  mode);
extern void BTC_PsTdmaIssueNull_8720E(BOOLEAN Powerbit, u8 PortNum);
extern void BTC_PsTdmaOnBcn_8720E(void);
extern void BTC_BtIqkAction_8720E(u8  allow);
extern BOOLEAN BTC_BtIqkCheck_8720E(void);
extern void BTC_WlanInfoCounterReset_8720E(void);
extern BOOLEAN BTC_ProceedBtSlot_8720E(void);
extern void BTC_ProtectBCN_8720E(void);
extern void BTC_RestoreBtSlot_8720E(void);
extern u8 BTC_GenRandomValue_8720E(u8 range, u8 count);
extern void BTC_WifiWindowSlot_8720E(void);
extern void IssueBTQNull_8720E(u8  bPowerBit, u8  lsb, u8  msb);
extern void BTC_Set_GntBt_8720E(u8 control);
#if CONFIG_BTCOEX_SLOT_DEBUG
extern void InitBTCoexDebugPort_8720E(void);
extern void BTC_SlotGpioDbgCtrl_8720E(u32 GPIO_Pin, u32 Val);
#endif
extern void BTC_AOACSwitch_8720E(u8  state);

#endif /* #ifndef __ASSEMBLY__ */
#endif /*  __WIFIFW_BTCONCURR_RAM_H__ */
