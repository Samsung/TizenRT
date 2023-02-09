#ifndef __WIFIFW_SYSMIB_H__
#define __WIFIFW_SYSMIB_H__


/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

//3 1.) Xdata Section
typedef struct _mib_info_ {
	void (*intdis)(void);
	void (*inten)(void);

	/* 3 Task Bitmap */
	u32                     TaskBitMap;
	u8                      TaskBitMap1;
	u8                      Debug[2];

	u8                      TxPauseReasonCode;
	u8                      RxPauseReasonCode;	  //3
	u16 					 RXFF0_BNDY;

	/* 3 HAL */
	HAL_DATA_8720E          IntHdl;

	/* 3 Wlan */
	/* Packet Location */
	RsvdPageLoc             RPL;
	H2C_EVT                 H2CEvt;
	C2H_FW_EVT             C2HFWEvt;
	u8                      H2C_rptr;

	/* Media Status */
	MEDIA_STATUS            MediaStatus;
#if CONFIG_RATE_ADAPTIVE
	struct _PPHYDM 	PPHYDM;
#endif

	PS_PARM                 PSParm;
	PS_TUNING_PPARM         PSTParm;
#if CONFIG_BCNEARLY_ADJUST_V2
	BCNEARLY_ADJUST_PID   	BcnEarlyAdjustPID;
#endif /* #if CONFIG_BCNEARLY_ADJUST_V2 */

#if CONFIG_BCNADJ_LPSOFFLOAD_V2
	BCNEARLY_ADJUST_PID   	BcnEarlyAdjustPID;
#endif /* #if CONFIG_BCNADJ_LPSOFFLOAD_V2 */

#if CONFIG_BCNEARLY_ADJUST_V1
	BCNEARLY_ADJUST_Stat	BcnEarlyAdjustStat;
#endif/* #if CONFIG_BCNEARLY_ADJUST_V1 */

#if CONFIG_BCNADJ_LPSOFFLOAD_V1
	BCNEARLY_ADJUST_Stat	BcnEarlyAdjustStat;
#endif/* #if CONFIG_BCNADJ_LPSOFFLOAD_V1 */

#if CONFIG_SAPPS
	SAPPSParm               SAPPSCtrl;
#endif  /* CONFIG_SAPPS */

#if CONFIG_INACTIVE_PS
	INACTIVEPSParm          InactivePSCtrl;
#endif  /* #if CONFIG_INACTIVE_PS */

	LPSOFFLOADParm	LPSOffloadParm;
	LPSPG_PARM	   		LPSPGParm;
	PS_DBGPARM			PSDbgParm;
#if CONFIG_LPS_I_TWT
	LPS_ITWT_PARM		LPSITWTParm;
#endif
#if CONFIG_BTCOEX
	BTC_PsTdma_Parm                  BtcPsTdma;
	H2C_BT_Tdma_Parm                 H2cBtTdma;                //H2C Index: 0x60
	H2C_Ant_Sel_Reverse_Parm         H2cAntSelReverse;         //H2C Index: 0x65
	H2C_WL_Opmode_Parm               H2cWlOpmode;              //H2C Index: 0x66
	H2C_BT_Page_Scan_Interval_Parm   H2cBtPageScanInterval;    //H2C Index: 0x6C
	H2C_WL_Calibration_Parm          H2cWlCalibration;         //H2C Index: 0x6D
	H2C_BT_OnlyTest_Parm             H2cBtOnlyTest;            //H2C Index: 0x6F
	H2C_BT_Init_Parm                 H2cBtInit;                //H2C Index: 0x70
	H2C_WL_PortID_Parm               H2cWlPortID;              //H2C Index: 0x71
#endif  /* #if CONFIG_BTCOEX */

#if CONFIG_BTMAILBOX
	BTC_Mailbox_Parm                 BtcMailbox;
#endif /* #if CONFIG_BTMAILBOX */

#if CONFIG_BTSCOREBRD
	BTC_Scoreboard_Parm              BtcScoreboard;
#endif /* CONFIG_BTSCOREBRD */

	RemoteWakeup_Parm       RemoteWakeUpCtrl;
	AOAC_INFO               AOACInfo;
	AOAC_GLOBAL_INFO        AOACGlobalInfo;
	AOAC_RSVDPAGE1          AOACPage1;
	AOAC_RSVDPAGE2          AOACPage2;
	AOAC_RSVDPAGE3          AOACPage3;

	KeepAlive_Parm          KeepAliveParm;

	Disconnect_Parm         DisconnectParm;

#if CONFIG_CSI_REPORT
	CSIReport_Parm          CSIReportParm;
#endif

#if CONFIG_TSF_RESET_PATCH
	u8                      TSF_RST_Port[2];
#endif  /* #if CONFIG_TSF_RESET_PATCH */

#if CONFIG_TXBEACON_IGNORE_EDCCA
	u8                      BcnIgnoreEdccaEn;
#endif  /* CONFIG_TXBEACON_IGNORE_EDCCA */

#if CONFIG_TX_PAUSE_DRV_INFO
	u8						TxPauseDRVInfo;
	u8						TxPauseFWInfo_0;
#endif /* #if CONFIG_TX_PAUSE_DRV_INFO */
#if CONFIG_TPBASE_RA
	TPBASE_RA_PARM TPBaseRAParm[MACID_NUM];
#endif

#if CONFIG_PERIOD_CCA
	u8						period_cca_en;
	u8						period_cca_reg_backup_done;
	u8						preiod_cca_bbreg_backup[7];
	u8						period_cca_rssi;
#endif
	u32                    RF_18;
} mib_info, *Pmib_info;

#ifdef __WIFIFW_INIT_RAM_C__

mib_info         SysMib;

#else

extern                mib_info         SysMib;

#endif /* #ifdef __WIFIFW_INIT_RAM_C__ */





/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/
typedef struct _H2C_S1_null1_ctrl_parm_ {
	u8 S1_null1_ctrl_en: 1;
	u8 S1_null1_fail_limit: 7;
	u8 S1_null1_interval;
} H2C_S1_null1_ctrl_parm, *PH2C_S1_null1_ctrl_parm;

typedef struct _S1_null1_parm_ {
	u32 S1_null1_unlock_time;
	u8 S1_null1_fail_cnt;
	u8 S1_null1_delay_flag: 1;
	u8 rsvd: 7;
} S1_null1_parm, *PS1_null1_parm;

typedef struct _S1_null1_ivl_patch_ {
	S1_null1_parm s1_null1_parm;
	H2C_S1_null1_ctrl_parm s1_null1_ctrl_parm;
} S1_null1_ivl_patch, *PS1_null1_ivl_patch;


typedef struct _mib_info_8720E_ {
	S1_null1_ivl_patch s1_null1_ivl_patch;
} mib_info_8720E, *Pmib_info_8720E;

/*------------------------Export global variable------------------------------*/
#ifdef  __WIFIFW_INIT_RAM_C__

mib_info_8720E                SysMib8720E;

#else

extern               mib_info_8720E                SysMib8720E;

#endif  /* #ifdef    __WIFIFW_INIT_RAM_C__ */

/*------------------------Export global variable------------------------------*/
/*------------------------------Function declaration--------------------------*/
#endif  /* __WIFIFW_SYSMIB_H__ */

