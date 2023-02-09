#ifndef __WLAN_POWERSAVE_RAM_H__
#define __WLAN_POWERSAVE_RAM_H__

/*--------------------Define --------------------------------------------*/
#define MACID_CLIENT						0
#define MAX_LEAKAGE_LIMIT   				3
#define LEAKAGE_DELAY_TIME					5
#define MAC_RS_TXREG_NUM 	20
#define MAC_RS_TXPWRREG_NUM 	102

/*TIMER allcation*/
#define TIMER_TC0_CTRL						REG_TC0_CTRL    //instead of PowerTranning
#define TIMER_TC1_CTRL						REG_TC1_CTRL    //by PS or APOffload(AP mode)
#define TIMER_TC2_CTRL						REG_TC2_CTRL    //by PS
#define TIMER_TC3_CTRL						REG_TC3_CTRL    //by BT Multi-slot TDMA
#define TIMER_TC4_CTRL						REG_TC4_CTRL    //Rsvd for driver
#define TIMER_TCUNIT						REG_TCUNIT_BASE
#define TIMER_TC5_CTRL						REG_TC5_CTRL    //by BT i2c timeout ( 92e only)
#define TIMER_TC6_CTRL						REG_TC6_CTRL    //by BT TDMA

#define COUNTER_WAITFCS					TIMER_TC0_CTRL //start a protect timer for MAC not receive FCS after lps offload tim hit
#define COUNTER_BCNTO						TIMER_TC1_CTRL  // counter used by BCNTO is the same with DTIM
#define COUNTER_DTIM						TIMER_TC1_CTRL
#define COUNTER_PSTRX 						TIMER_TC2_CTRL
#define COUNTER_WAITHOST					TIMER_TC3_CTRL
#define COUNTER_DELAYKEEPALIVE			TIMER_TC5_CTRL
#define COUNTER_CSITXNULL					TIMER_TC5_CTRL
#define REG_PGRSStateInd					REG_FWLBK_DBG0
#define MACPGResumeDone					(0x12345678)
/*
    PS_RX_INFO[7:0]: Power Save RX Information Register
    initial value: 0x00
    REG III.220 (Offset 0x 0692h) PS_RX_INFO Register Definition
*/
#define RXDATAIN0								BIT0  //PSTX
#define RXDATAIN1								BIT1  //PSRX
#define RXDATAIN2								BIT2
#define RXMGTIN0								BIT3
#define RXCTRLIN0								BIT4

//Power Save Tuning Parameter
#define BCN_RX_ON_DELAY         			2        //Beacons are transmitted in 1MHz, then 1bit per 1microsecond. If 2ms are added, it prevents from losing approximately 250bytes in worst case.
#define BCN_EARLY_32K_MARGIN    			2        // 1ms would be enough, but 2ms is better for multicast receiving in open space
#define BCN_EARLY_DRV_MARGIN    			1        // 1ms would be better in open space

#define BCN_RECEIVE_32K_BUFFER  		(BCN_RX_ON_DELAY+BCN_EARLY_32K_MARGIN)
#define BCN_RECEIVE_DRV_BUFFER  		(BCN_RX_ON_DELAY+BCN_EARLY_DRV_MARGIN)

#define BCN_DISTRIBUTION        			8  //5ms ,generally is 5 ms
#define DEFAULT_BCN_TO_PERIOD   		(BCN_RECEIVE_32K_BUFFER+BCN_DISTRIBUTION)  //choose the bigger one
#define DEFAULT_BCN_TO_LIMIT    			2  // 1

#define DEFAULT_BCN_TO_TIMES_LIMIT  	2
#define DEFAULT_DTIM_TIMEOUT    			15  // 7         // 7 ms
#define DEFAULT_PS_TIMEOUT      			20  // 20       // 20 ms
#define DEFAULT_PS_DTIM_PERIOD  		7

#define DEFAULT_PS_BCN_EARLY_MIN     	1
#define DEFAULT_PS_BCN_EARLY   	     	3
#define DEFAULT_PS_BCN_EARLY_128US   	0
#define DEFAULT_PS_BCN_EARLY_32US    	0
#define DEFAULT_PS_BCN_EARLY_32K   	 	5
#define DEFAULT_PS_BCN_EARLY_128US_32K	0
#define DEFAULT_PS_BCN_EARLY_32US_32K	0
#define DEFAULT_BCN_TO_PERIOD_32K			15

#define DEFAULT_PS_BCN_EARLY_V_32K   	 5
#define DEFAULT_PS_BCN_EARLY_V_128US_32K   0
#define DEFAULT_PS_BCN_EARLY_V_32US_32K    0

#define DEFAULT_PS_32K_EARLY(x)       	(((DEFAULT_PS_BCN_EARLY_V_32K+(x)) << 5)+(DEFAULT_PS_BCN_EARLY_V_128US_32K<< 2)+(DEFAULT_PS_BCN_EARLY_V_32US_32K))
#define BCN_PS_32K_SHIFT_MAX(x)        	(((DEFAULT_PS_BCN_EARLY_V_32K+(x)) << 5)+(DEFAULT_PS_BCN_EARLY_V_128US_32K<< 2)+(DEFAULT_PS_BCN_EARLY_V_32US_32K))
#define DEFAULT_PS_EARLY(x)       		(((DEFAULT_PS_BCN_EARLY+(x)) << 5)+(DEFAULT_PS_BCN_EARLY_128US<< 2)+(DEFAULT_PS_BCN_EARLY_32US))
#define BCN_PS_SHIFT_MAX(x)        		(((DEFAULT_PS_BCN_EARLY+(x)) << 5)+(DEFAULT_PS_BCN_EARLY_128US<< 2)+(DEFAULT_PS_BCN_EARLY_32US))

#define BCN_ADVANCE_MAX         			10
#define BCN_EARLY_MARGIN        			1       //unit 256us
#define RF_ON_COMPENSATION          		50//70   1030        //unit 1us
#define BCN_RX_INT_DELAY              		90//  70   1030   //unit 1us
#define BCN_DELAY_MAX           			15
#define BCN_CALCULATION_MAX     			(BCN_ADVANCE_MAX+BCN_DELAY_MAX)

#define BCN_ADJUST_COUNT        			100
#define BCN_EXCLUDED_NUM        			(BCN_ADJUST_COUNT/20)
#define BCN_ENLARGE_NUM         			5
#define SHIFT_COMPENSATION      			1
#define BCN_DRIFT_COMPENSATION  		2

#define NULL_DATA0_ALLOW        			1
#define NULL_DATA0_DENY         			0

#define PS_RF_OFF               				0
#define PS_GO_ON                				BIT0
#define PS_TX_NULL              				BIT1
#define PS_RF_ON               	 			BIT2
#define PS_REGISTER_ACTIVE      			BIT3

//0x009C
#define PS_32K_IN               				BIT0
#define PS_32K_PG               				BIT4
#define PS_ACK                  				BIT6
#define PS_TOGGLE              			 	BIT7

#define PS_STATE_MASK           			(0x0F)
#define PS_IS_TX_NULL(x)        			((x) & PS_TX_NULL )
#define PS_IS_CLK_ON(x)         			((x) & (PS_RF_OFF |PS_ALL_ON ))
#define PS_IS_RF_OFF(x)         			((x)|PS_RF_OFF)
#define PS_IS_RF_ON(x)          			((x) & (PS_RF_ON))
#define PS_IS_ACTIVE(x)         			((x) & (PS_REGISTER_ACTIVE))

#define PS_STATUS_S0            			(PS_REGISTER_ACTIVE | PS_RF_ON)                    //(1,1,0) all on = register active + rf on
#define PS_STATUS_S1            			(PS_REGISTER_ACTIVE | PS_RF_ON | PS_TX_NULL)        //(1,1,1) all on + tx null(1)
#define PS_STATUS_S2            			(PS_RF_ON)                                         //(0,1,0) register sleep + rf on
#define PS_STATUS_S3            			(PS_RF_ON | PS_TX_NULL)                            //(0,1,1) register sleep + rf on + tx null(0)
#define PS_STATUS_S4            			0                                                 //(0,0,0) all OFF
#define PS_STATUS_S5            			(PS_TX_NULL )                                      //(0,0,1) SCAN = register sleep + rf on + scan enable
#define PS_STATUS_S6            			(PS_REGISTER_ACTIVE)                               //(1,0,0) NoA off = register active + rf off

/* DATA FIN Condition Flags */
#define STA_DATA_OPEN           			BIT0  // indicate that FW open due to TIM = 1 condition. (PS-POLL as trigger frame)
#define BC_DATA_OPEN            			BIT1   // indicate that FW open due to DTIM = 1 condition.  (BC & MC)
#define QOS_DATA_OPEN           			BIT2 // indicate that FW open due to UAPSD trigger condition. (QNULL)
#define C2H_DATA_OPEN          				BIT3   // indicate that FW open due to C2H event 
#define BCN_DATA_OPEN           			BIT4
#define APP_DATA_OPEN           			BIT5

#define ALL_80211_DATA_OPEN     			(STA_DATA_OPEN | BC_DATA_OPEN | QOS_DATA_OPEN)
#define IS_80211_DATA_OPEN(x)			((x) & ALL_80211_DATA_OPEN)
#define IS_C2H_DATA_OPEN(x)     			((x) & C2H_DATA_OPEN)

#define SET_DATA_OPEN(x, type)  		((x) |= (type))
#define CLR_DATA_OPEN(x, type)  		((x) &= (~type))
#define IS_DATA_OPEN(x, type)   			((x) & (type))

/* pwr state */
#define PS_TYPE_32KPERMISSION       	0
#define PS_TYPE_CURRENT_PS_STATE    	1
#define PS_TYPE_LASTRPWM            		2

#if CONFIG_PS_DEBUG
#define DBG_LPS_WRITE(x,y)		(x = y)
#define DBG_LPS_INCREASE(x)		(++x)
#define DBG_LPS_SM(x,y) 			(SysMib.PSDbgParm.DbgLpsSM[x] = y)
#else
#define DBG_LPS_WRITE(x,y)
#define DBG_LPS_INCREASE(x)
#define DBG_LPS_SM(x,y)
#endif

/*--------------------Define Enum---------------------------------------*/
typedef enum _RxListenBeaconMode_ {
	RLBM_MIN                = 0,
	RLBM_MAX,
	RLBM_SELF_DEFINED
} RxListenBeaconMode, *PRxListenBeaconMode;

typedef enum _RT_MEDIA_STATUS {
	RT_MEDIA_DISCONNECT     = 0,
	RT_MEDIA_CONNECT        = 1
} RT_MEDIA_STATUS;

typedef enum _PS_MODE_SETTING_SELECTION_ {
	MODE_SETTING_ACTIVE     = 0,
	MODE_SETTING_LEGACY     = 1,
	MODE_SETTING_WMMPS      = 2

} PS_MODE_SETTING_SELECTION, *PPS_MODE_SETTING_SELECTION;

typedef struct _LEGACY_PS_PPARM_ {
	u8		ps_mode: 7;
	u8		ClkRequestEnable: 1;
	u8   	RLBM: 4;            //RX Listen BCN Mode
	u8 		smart_ps: 4;
	u8		AwakeInterval;      //Unit: beacon interval, this field is only valid in PS Self-Defined mode
	u8		bAllQueueUAPSD: 1;    // 1: all queue are uapsd 0: not all queue are uapsd
	u8		bMaxTrackingBcnMode: 1;
	u8		bBCNEarlyReport: 1;
	u8		XtalSel: 1;
	u8 		rsvd0: 4;
	u8		PwrState;
	u8		lpsofldEn: 1;
	u8		waitbcnfcsEn: 1;
	u8		rsvd1: 6;
	u8		BcnRecevingTime : 5;
	u8		BcnListenInterval : 2;
	u8		AdoptUserSetting: 1;
} LEGACY_PS_PARM, *PLEGACY_PS_PARM;

typedef struct _PS_TUNING_PPARM_ {
	u8		BCNTOLimit;
	u8		DTIMTimeOut;
	u8		Adopt: 1;
	u8		PSTimeOut: 7;
	u8		PS_DTIMPeriod;
	u8		rsvd1;
} PS_TUNING_PPARM, *PPS_TUNING_PPARM;

/*--------------------Define Struct---------------------------------------*/
typedef struct _BEACON_ADJUST_PARM_ {
	u8		BcnReceive32KBuffer;          //unit 128us
	u8		BcnReceiveDrvBuffer;          //unit 128us
	u16   	BcnAdjustCount;
	u8    	BcnExcludedNum;
	u8    	BcnEnlargeNum;
} BEACON_ADJUST_PARM, *PBEACON_ADJUST_PARM;

typedef struct _PS_PARM_ {
	u8		Enter32KHzPermission: 1;
	u8		bAllQueueUAPSD: 1;
	u8		ps_dtim_flag: 1;            // indicate dtim of current beacon.
	u8		pstrx_rxcnt_period: 1;
	u8 		NoConnect32k: 1;
	u8		TxNull0: 1;
	u8		TxNull1: 1;
	u8		rsvd0: 1;

	u8 		TxNull0ok: 1;
	u8		TxNull1ok: 1;
	u8		RfOffLicenseForBCNRx: 1;    //filen: After we received ps_bcn_cnt beacons, we can sleep(rf off).
	u8		BCNAggEn: 1;
	u8		IsGoingTo32K: 1;
	u8		bMaxTrackingBcnMode: 1;
	u8		BcnTraceDone: 1;
	u8		BcnEarlyPeriodFlag: 1;

	u8		BcnEarlySettingFlag: 1;
	u8		bBCNEarlyReport: 1;
	u8		BcnEnter32kMode: 1;
	u8		XtalSel: 1;
	u8		rsvd: 4;
	/*
	    filen: to indicate whether it is smart power saving or not
	    0: Legacy PS
	    1: Smart PS(RX use ps_poll)
	    2: Smart PS (RX use null_data(0))
	*/
	u8		smart_ps: 4;                //enum SMART_PS_MODE
	u8		RLBM: 4;                    // RX BCN MODE (min, max, active, ...)
	u8		AwakeInterval;
	u8		ps_mode;                    // ps type (avtive, legacy, wmmps)
	u8		ClkRequestEnable;
	u8		last_rpwm;
	u8		current_ps_state;
	u8 		ps_data_open;
	u8		ps_dtim_period;
	u8		ps_dtim_cnt;
	u8		ps_bcn_to;                  //unit 128us        // beacon timeout (ms).
	u8		bcn_to_cnt;                 // indicate the total number of contnuous BCN_TO we have received.
	u8		bcn_to_times_cnt;        //20140806
	u8		ps_drv_early_itv;       // 32us
	u8		BcnAheadShift;          //unit 32us
	u8		BcnEarlyShift;          //unit 32us
	u8		BcnEarlyShiftMax;           //unit 32us
	u8		DefaultBcnEarly;            //unit 32us
	u8 		RxBcnCount;
	u8		TBTTCount;
	u8		CurrentEarly;           //unit 32us
	u8 		CurrentTimeOut;         //unit 128us
	u8		ReachBcnLimitCount;
	u8		XtalDelay;
	union {
		u8		BcnDurationArray[2];
		u16	BcnDuration;
	} BcnDurationUnion;
	u32	TSFOnTBTT;              //unit in TU
	//ben modify for open/close RF speedup.
	union {
		u8		TSFOnRxBcnArray[4];
		u32	TSFOnRxBcn;
	} TSFOnRxBcnUnion;
	union {
		u8		TSFOnBcnEarlyArray[4];
		u32	TSFOnBcnEarly;
	} TSFOnBcnEarlyUnion;
	BEACON_ADJUST_PARM BeaconAdjustParm;
	u8		rcv_action;
	u8		AckNull0;
	u8 		Fix_Delay_forward;
	u8 		bTIM_Flag;
	u8		Null0_rty_cnt;
	u8 		Null0_rty_lmt;
	u8 		BCNNumForDRV;//driver deric
	u8 		ScanEn;
#if CONFIG_LEAPY_AP
	u8 TrafficLeakageCnt;
	BOOLEAN waiting_leakage_check;
#endif
#if CONFIG_BEACON_MODE
	u32 BCN_G7_DCK;
	u32 BCN_DCKG0;
	u32 BCN_DCKG1;
	u32 BCN_DCKG2;
	u32 BCN_DCKG3;
	u32 BCN_DCKG4;
	u32 BCN_DCKG5;
	u32 BCN_DCKG6;
	u32 BCN_DCKG7;
	u32 BB4454;
	u32 BCNModeCH;
#endif

} PS_PARM, *PPS_PARM;

typedef enum _SMART_PS_MODE_FOR_LEGACY_ {
	SMART_PS_MODE_LEGACY_PWR1           = 0,        // TX: pwr bit = 1, RX: PS_POLL
	SMART_PS_MODE_TX_PWR0               = 1,        // TX: pwr bit = 0, RX: PS_POLL
	SMART_PS_MODE_TRX_PWR0              = 2,         // TX: pwr bit = 0, RX: NULL(0)
	SMART_PS_MODE_TX_PWR1				= 3 		// TX: pwr bit = 1, RX: NULL(0)
} SMART_PS_MODE_FOR_LEGACY, *PSMART_PS_MODE_FOR_LEGACY;

typedef struct _SAPPS_PARM_ {
	u8      Enable: 1;
	u8      EnPS: 1;
	u8      EnLPRx: 1;
	u8      Manual32k: 1;
	u8      Active: 1;
	u8      FlagToWaitHostCutPower: 1;
	u8      RF2Path: 1;
	u8      rsvd: 1;
	u8      Duration;
	u8      Duration_On;
	u8      Duration_Off;
	u8      AntSwitchOutside;
	u8      TwoAnt;
	u8      AntSelReverse;
	u8      TxPathCtrl;
} SAPPSParm, *PSAPPSParm;

typedef struct _INACTIVEPS_Parm_ {
	u8      Enable: 1;
	u8      IgnorePsCondition: 1;
	u8      rsvd: 6;
	u8      Frequency;
	u8      Duration;
	u8      FrequencyCount;
} INACTIVEPSParm, *PINACTIVEPSParm;

typedef struct _LPSOFFLOAD_Parm_ {
	u8	MACRXBCNOfldEn: 1;
	u8	HWCtrlTRXOnOff: 1;
	u8	ByPassLsysWakeEn: 1;
	u8	WaitFCSOptionEn: 1;
	u8	RcvingTimhitBcn: 1;
	u8	ReceiveTimhit: 1;
	u8	BKForPGResDone: 1;
	u8	InHWCtrlOnOffPeriod: 1;
	u16 RxbcnModeCmdFileStartAddr;
	u16 RxbcnModeCmdFileEndAddr;
	u16 ExitRxbcnModeCmdFileStartAddr;
	u16 ExitRxbcnModeCmdFileEndAddr;
	u16 TRXOnCmdFileHWStartAddr;
	u16 TRXOnCmdFileFWStartAddr;
	u16 TRXOnCmdFileEndAddr;
	u16 TRXOffCmdFileStartAddr;
	u16 TRXOffCmdFileEndAddr;
	u16 BBBkRsCmdFileStartAddr;
	u16 BBBkRsCmdFileEndAddr;
	u16 RF0x18RSCmdFileStartAddr;
	u16 RF0x18RSCmdFileEndAddr;
	u16 Delay2USCmdFileStartAddr;
	u16 Delay2USCmdFileEndAddr;
	u16 PGHIOECH1BKStartAddr;
	u16 PGHIOECH1BKEndAddr;
	u16 PGRSCmdFileStartAddr;
	u16 PGRSCmdFileEndAddr;
	u16 PGRSStartDelayStartAddr;
	u16 PGRSStartDelayEndAddr;
	u16 BcnIntervalBackup;
} LPSOFFLOADParm, *PLPSOFFLOADParm;

typedef struct _Bcnearly_adjust_PID_ {
	u32 	PerDeltaTSF;  //unit us
	u32 	DeltaTSF;
	u32 	PerError;
	u32 	Error;
	u32 	Per1_TSFOnRxBcn;
	u32 	Per2_TSFOnRxBcn;
	u32 	PbaseMinDeltaTSF;
	u8		PbaseGetTime;
	u8		PbaseGetting;
	u16 	Pinc;
	u8  	LimitOutput;  //unit 32us
	u8		MaxOutput;	 //unit 32us
	u8		PITOOutput;	 //unit 32us
	u8		BaseOutput;	 //unit 32us
	u8		IBaseMinOutput;
	u8		Phase1Output;	 //unit 32us
	u8		Phase2Output;	 //unit 32us
	u8		Phase3StbAdd;
	u8		BaseOPGetTime;
	u8		BaseOutputSetDown;
	u8		BcnLossCount;
	u32 	DeltaTSFSum;  //unit us
} BCNEARLY_ADJUST_PID, *PBCNEARLY_ADJUST_PID;

typedef struct _Bcnearly_adjust_Stat_ {
	u32 	DeltaTSF;
	u32 	DeltaTSFMax;
	u32 	Per1_TSFOnRxBcn;
	u32 	Per2_TSFOnRxBcn;
	//u8      BcnMissMinRadio;
	u8      BcnDelay64us[128];
	u8      DelayCnt[128];
	u8 		Phase1MinSet;
	u8		Phase2CntFull;
	//u8	Phase3StbCK;
	u8		BcnLossCount;
	u32 	DeltaTSFSum;  //unit us
	u8		Phase1Cnt;
	u8		Phase1Output;	 //unit 32us
	u8		Phase2Cnt;
	u8		Phase2Output;	 //unit32us
	u8		Phase3Stbflag;
	u8		Phase3StbAdd;
	u8		LimitOutput;
} BCNEARLY_ADJUST_Stat, *PBCNEARLY_ADJUST_Stat;

typedef struct _LPSPG_PARM_ {
	u8      LpsPG_En;
	u32   	LPS_Option;
	u16 HIOE_STR_ADDR;
	u16 HIOE_END_ADDR;
	u8     	HioeRestoreDone: 1;
	u8     	HioeBackupDone: 1;
	u8	rsvd: 6;
	u32	ARFC18;
	u32	DRFC18;
	u32 	ARFC0;
	u32 	DRFC0;
	u32 BB8038;
	u32 BB803C;
	u8 RxBcnMode_status;
	u8 BKBcnMode_status;
	u32 mactxreg[MAC_RS_TXREG_NUM];
	u32 macid0ctrlinfo[10];
	u32 mactxpower[MAC_RS_TXPWRREG_NUM];
	u32 tssi_bbpage56[64];
	u32 tssi_bbpage58[64];
} LPSPG_PARM, *PLPSPG_PARM;

typedef enum _ITWTCLT_ {
	ITWTCLIENTA               = 0x0,
	ITWTCLIENTB               = 0x1,
	ITWTCLIENTC               = 0x2,
	ITWTCLIENTD               = 0x3,
} ITWTCLT, *PITWTCLT;

typedef struct _ITWTCLIENT_PARM_ {
	u8		TWT_En;
	u8		TRIGGER_En;
	u8		TXPAUSE;
	u8		ANNOUNCED;
	u16	DURATION;
	u32	INTERVAL;
	u32	TWTIME_H;
	u32	TWTIME_L;
	u8		SPSTARTFLAG;
	u8		TWTEarly;
} ITWTCLIENT_PARM, *PITWTCLIENT_PARM;

typedef struct _LPS_ITWT_PARM_ {
	u8		FLOW_ID;//client number
	u8		TWTVALID;
	u8		TWTBCNTo;
	u32	TWTTSFRxBcn;
	u32	TWTTSFKeepAlive;
	ITWTCLIENT_PARM ITWTCLIENT1;
	ITWTCLIENT_PARM ITWTCLIENT2;
	ITWTCLIENT_PARM ITWTCLIENT3;
	ITWTCLIENT_PARM ITWTCLIENT4;
} LPS_ITWT_PARM, *PLPS_ITWT_PARM;

typedef struct _PS_DBGPARM_ {
	//DW
	u8		DbgLpsSM[4];//state machine
	u16	IssueTxNull0;
	u16	TxNull0OK;
	u16	TxNull0Timeout;
	u16 	TxNull0Preempted;
	u16	IssueTxNull1;
	u16 	TxNull1OK;
	u16	TxNull1Timeout;
	u16	TxNull1Preempted;
} PS_DBGPARM, *PPS_DBGPARM;
/*--------------------Function declaration---------------------------------*/
extern void SetPwrStateReg_8720E(u8 PwrStateType, u8 value);
extern void ResetPSParm_8720E(void);
extern BOOLEAN ChkTxQueueIsEmpty_8720E(void);
extern void Legacy_PS_Setting_8720E(void);
extern void PSModeSetting_8720E(u8 on) ;
extern void ConfigListenBeaconPeriod_8720E(u8  RLBM, u8 AwakeInterval);
extern void PSSetMode_8720E(PLEGACY_PS_PARM pparm);
extern void PS_S2ToS0State_8720E(void);
extern void PS_S3ToS2orS0State_8720E(void);
extern void PS_S1ToS0orS2State_8720E(void);
extern void PS_S2ToS4State_8720E(void);
extern void PS_S2ToS5State_8720E(void);
extern void PS_S5ToS2State_8720E(void);
extern void PS_S4ToS2State_8720E(u8  ReleaseTxPause);
extern void SetBcnEarlyAndTimeout_8720E(u8 BcnAheadShift, u8 BcnEarlyShift, u8 BcnTimeout, u8 ReasonCode);
extern void EnlargeBcnEarlyAndTimeout_8720E(void);
extern void PS_S6ToS0State_8720E(void);
extern void PS_S0ToS6State_8720E(void);
extern void Change_PS_State_8720E(u8, u8);
extern u8 PS_S4_Condition_Match_8720E(void);
extern BOOLEAN PS_S2_Condition_Match_8720E(void);
extern BOOLEAN PS_32K_Condition_Match_8720E(void);
extern void PS_S2ToS3State_8720E(u8 nulldata0Allow);
extern void PS_S0ToS1State_8720E(void);
extern void SmartPS2InitTimerAndToGetRxPkt_8720E(void);
extern void InitPS_8720E(void);
extern void SetSmartPSTimer_8720E(void);
extern void ChangePSStateByRPWM_8720E(void);
extern void PSBcnEarlyProcess_8720E(void);
extern void PSBcnAggEarlyProcess_8720E(void);
extern void PsDtimToProcess_8720E(void);
extern void PsBcnToProcess_8720E(void);
extern void TxPktInPSOn_8720E(void);
extern void PSSleepTo32K_8720E(void);
extern void PSRxBcnProcess_8720E(void);
extern void InitSAPPS_8720E(void);
extern void SAPPSTBTTHDL_8720E(void);
extern void SAPPSTimeOutHDL_8720E(void);
extern void SAPPSEarlyHDL_8720E(void);
extern void SAPLPS_8720E(void);
extern BOOLEAN CHECKTWTClientNULL_8720E(void);
extern BOOLEAN CHECKTWTClientSPEND_8720E(void);
extern void PSTWTEarlyProcess_8720E(void);
extern void PSTWTBcnToProcess_8720E(void);
extern void PSTWTBcnRxProcess_8720E(void);
extern void LPS_ITWTTimeUpdate_8720E(u8 clientnum);
extern void LPS_ITWTDurationUpdate_8720E(u8 clientnum);
extern void LPS_ITWTTIMERSetting_8720E(u8 portnum, u8 clientnum);
extern void LPS_ITWTStatusSet_8720E(u8 clientnum);
extern void LPS_ITWTIMRSetting_8720E(u8 clientnum);
extern void LPS_ITWTByPassLsysWakeInt_8720E(BOOLEAN ByPassLsysWakeEn);
extern void LPS_ITWTSPWORK_8720E(BOOLEAN ITWTSTART);
extern void LPS_ITWT_PortSet_8720E(u8 portnum, u8 clientnum);
extern void LPS_ITWT_ClientSPSet_8720E(u8 clientnum, u32 TWTH, u32 TWTL);
extern void LPS_ITWT_ClientSPEarlySet_8720E(u8 clientnum, u8 TWTEarly);
extern void InitInactivePS_8720E(void);
extern void InactivePSTBTTHDL_8720E(void);
extern void InactivePSBCNAggEarlyHDL_8720E(void);
extern void InactivePSEnableHEL_8720E(u8 op);
extern void InitBcnEarlyAdjustStat_8720E(void);
extern void BcnEarlyAdjustByDeltaTSFStat_8720E(void);
extern void InitBcnEarlyAdjustPID_8720E(void);
extern void BcnEarlyAdjustByDeltaTSFPID_8720E(void);
extern void BcnEarlyADJSTSSearchBaseValue_8720E(void);
extern void BcnEarlyADJSTSCheckBaseValue_8720E(void);
extern void BcnEarlyADJPIDSearchBaseValue_8720E(void);
extern void BcnEarlyADJPIDCheckBaseValue_8720E(void);
extern void Set32KLpsOption_8720E(u32 *LpsControl, u8 CutPower);
extern void CheckTrafficLeakage(void);

#endif  //__WLAN_POWERSAVE_RAM_H__
