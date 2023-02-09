#ifndef __WIFIFW_CMD_RAM_H__
#define __WIFIFW_CMD_RAM_H__

/*--------------------Define --------------------------------------------*/
#define HALBB_START_H2CID	0x40
#define HALBB_END_H2CID		0x5f
#define CSICLIENTNUM		0x3

//==================C2H event format ====================
// Field     SYNC       CMD_LEN     CONTENT    CMD_SEQ      CMD_ID
// BITS  [127:120]  [119:112]      [111:16]          [15:8]        [7:0]
typedef struct _C2H_EVT_HDR {
	u8  cmd_id;
	u8  cmd_seq;
	u8  content[32];//change 12B to 32B for FTM C2H
	u8  cmd_len;
} C2H_EVT_HDR, *PC2H_EVT_HDR;

//Register C2H Command ID here
typedef enum _C2H_CMD_ {
	C2HID_DEBUG								= 0,
	C2HID_H2C2HLB							= 1,
	C2HID_TXBF								= 2,
	C2HID_CCX_TXRPT						= 3,
	C2HID_AP_REQ_TXRPT					= 4,
	C2HID_INIT_RATE_COLLECT			= 5,
	C2HID_PSD_RPT							= 6,
	C2HID_SCAN_COMPLETE					= 7,
	C2HID_PSD_CONTROL					= 8,
	C2HID_BT_INFORMATION				= 9,
	C2HID_BT_LOOPBACK					= 0x0A,
	C2HID_BT_MP_REPORT					= 0x0B,
	C2HID_RA_RPT							= 0x0C,
	C2HID_SPE_STATIS						= 0x0D,
	C2HID_RA_PARA_RPT					= 0x0E,
	C2HID_DTP_RPT							= 0x0F,
	C2HID_CURRENT_CHANNEL 				= 0x10,
	C2HID_IQK_OFFLOAD					= 0x11,
	C2HID_BB_GAIN_REPORT				= 0x12,
	C2HID_RateAdaptive_RPT_88E		= 0x13,	// for 88e smic RA debug
	C2HID_GPIOWAKEUP						= 0x14,
	C2HID_MAILBOX_STATUS				= 0x15,
	C2HID_H2C_PKT_Response				= 0x16, //mips
	C2HID_MCC_STATUS 						= 0x17,
	C2HID_NAN_EVENT						= 0x18,
	C2HID_Efuse_Hidden_REPORT			= 0x19,
	C2HID_FTMC2H_RPT						= 0x1B,
	C2HID_FTMSession_END				= 0x1C,
	C2HID_DRVFTMC2H_RPT					= 0x1D,
	C2HID_BCN_EARLY_RPT					= 0x1E,
	C2HID_ECSA_RPT						= 0x1F,
	C2HID_TX_PAUSE_RPT					= 0x20,
	C2HID_MACID_PAUSE_RPT				= 0x21,
	C2HID_Customer_String_rpt			= 0x24,
	C2HID_Customer_String_rpt_1		= 0x25,
	C2HID_WLAN_INFO						= 0x27,

	C2HID_DEBUG_CODE						= 0xFE,
	C2HID_EXTEND_IND						= 0xFF
} C2H_CMD, *PC2H_CMD;

/*--------------------Define MACRO--------------------------------------*/
//3 HMETFR
#define H2CINTBOX_BITMASK				0xF
#define H2CINTBOX_NUMMASK				0x3
#define BIT_PTR_NOMATCH				BIT7

//RXDEC
#define BIT_MASK_RXDEC_LENGTH		0x3FFF
#define BIT_RXDEC_C2HINDEX    		BIT4

// H2C
#define LENGTH_H2C						8
#define LENGTH_H2C_CMD_HDR			sizeof(H2C_CMD_HDR)
#define LENGTH_H2C_CONTENT			(LENGTH_H2C -LENGTH_H2C_CMD_HDR)
#define ADOPT_H2C_VALUE				BIT7
#define LENGTH_H2C_BUFF				10
#define H2C_Q_FULL(x, y)				(((((x) == 0) && ((y) == (LENGTH_H2C_BUFF - 1)))||((y) == ((x) - 1))) ? _TRUE : _FALSE)
#define H2C_Q_EMPTY(x, y)				(((x) == (y)) ? _TRUE : _FALSE)
#define H2CPTR_REACH_BOTTOM(x)		(((x) == LENGTH_H2C_BUFF) ? _TRUE : _FALSE)

// C2H
#define LENGTH_C2H						16
#define LENGTH_C2H_EVT_HDR			2
#define LENGTH_C2H_LEN_FIELD			1
#define LENGTH_C2H_SYNC				1
#define LENGTH_C2H_CONTENT 			(LENGTH_C2H - LENGTH_C2H_EVT_HDR - LENGTH_C2H_LEN_FIELD - LENGTH_C2H_SYNC)
#define LENGTH_C2H_BUFF				8//10, modified by jackie;
#define WAIT_C2H_CHECK_CNT 			(1000)
#define CHECK_C2H_ONCE_TIME 			(10)
#define C2H_DATA_RDY					0xFF
#define C2HPTR_REACH_BOTTOM(x)		(((x) == LENGTH_C2H_BUFF) ? _TRUE : _FALSE)
#define C2H_Q_EMPTY(x, y)				(((x) == (y)) ? _TRUE : _FALSE)
#define C2H_Q_FULL(x, y)				(((((x) == 0) && ((y) == (LENGTH_C2H_BUFF - 1)))||((y) == ((x) - 1))) ? _TRUE : _FALSE)
#define C2HPKT_POLL						BIT0

#define MACID_NUM_NEW					(MACID_NUM/8)

/*--------------------Define Struct---------------------------------------*/
//3 Class5: WOWLAN

//H2C Index: 0x80
typedef struct _wwlan_enable_ {
	u8		fun_En: 1;
	u8		pattern_match_En: 1;
	u8		magic_pkt_En: 1;
	u8		unicast_En: 1;
	u8		All_pkt_drop_En: 1;
	u8		GPIO_Active_type_En: 1;
	u8		Rekey_Wakeup_En: 1;
	u8		Discon_Wakeup_En: 1;
} WWlan_En, *PWWlan_En;

typedef struct _wwlan_parm_ {
	WWlan_En		enable;

	u8		gpio_num: 7;
	u8		DataPinWakeUp: 1;

	u8		gpio_duration;
	u8		gpio_pulse_en: 1;
	u8		gpio_pulse_count: 7;

	u8		usbphy_disable: 1;
	u8		hst2dev_en: 1;
	u8		gpio_duration_ms: 1;
	u8		rsvd0: 5;
	//not H2C from here
	u8		nullpkt: 1;
	u8		DriverRdy: 1;
	u8		rsvd: 6;
} WWlan_Parm, *PWWlan_Parm;

//H2C Index: 0x81
typedef struct _remote_wakeup_enable_ {
	u8		RemoteWakeUpCtrlEn: 1;
	u8		ArpEn: 1;
	u8		NdpEn: 1;
	u8		GtkEn: 1;
	u8		NLOOffloadEn: 1;
	u8		RealWoWLanEn: 1;
	u8		RealWoWLanV2En: 1;
	u8		MatchApplQQ: 1;

	u8		P2POffloadEn: 1;
	u8		Runtime_PM_En: 1;
	u8		NetbiosDropEn: 1;
	u8 		TkipOffloadEn: 1;
	u8		TcpSyncDropEn: 1;
	u8		rsvd: 3;

	u8		ARP_ACTION: 1;
	u8		rsvd2: 3;
	u8		FWParsingUntilWakeup: 1;
	u8		FWParsingAfterWakeup: 1;
	u8		rsvd3: 2;
} Remote_wakeup_enable, *PRemote_wakeup_enable;

//H2C Index: 0x82
typedef struct _AOAC_GLOBAL_INFO_ {
	u8		PairwiseEncAlg;
	u8		GroupEncAlg;
	u8		rsvd[5];
} AOAC_GLOBAL_INFO, *PAOAC_GLOBAL_INFO;

//H2C Index: 0x83
typedef struct _AOAC_RSVDPAGE1_ {
	u8		loc_remoteCtrlInfo;
	u8		loc_arp;
	u8		loc_ndp;
	u8		loc_gtkRsp;
	u8		loc_gtkInfo;
	u8		loc_gtkExtMem;
	u8		loc_ndpInfo;
} AOAC_RSVDPAGE1, *PAOAC_RSVDPAGE1;

//H2C Index: 0x84
typedef struct _AOAC_RSVDPAGE2_ {
	u8		loc_routerSolicitation;
	u8		loc_bubblePacket;
	u8		loc_teredoInfo;
	u8		loc_RealwowInfo;
	u8		loc_KeepAlivePkt;
	u8		loc_AckPattern;
	u8		loc_WakeupPattern;
} AOAC_RSVDPAGE2, *PAOAC_RSVDPAGE2;

// H2C Index: 0x88
typedef struct _AOAC_RSVDPAGE3_ {
	u8		loc_NLOInfo;
	u8		loc_AOACReport;
} AOAC_RSVDPAGE3, *PAOAC_RSVDPAGE3;

//H2C Index: 0x3
typedef struct _keep_alive_parm_ {
	u8		Enable: 1;
	u8 		AdoptUserSetting: 1;
	u8		PktType: 1;
	u8		DelayKeepalive: 1;
	u8		rsvd: 3;
	u8		KeepAliveNullTx: 1;

	u8		KeepAliveCountLimit;
	u8		KeepAliveCount;
	u8		SameTBTTRsvdPageCount;
} KeepAlive_Parm, *PKeepAlive_Parm;

typedef struct _remote_wakeup_ {
	Remote_wakeup_enable	funcEn;
	WWlan_Parm	WWlanCtrl;
	u8		Wakeup: 1;
	u8		delay_wake_wait: 1;
	u8		delay_wake_flag: 1;
	u8		rsvd: 5;

	u8		MACAddr[6];
	u8		BSSIDAddr[6];
	u8		Debug[7];
	u8		AntSwitchOutside;
	u8		TwoAnt;
	u8		AntSelReverse;
	u8		RunTimePMFilter;
	//for modify kr4 memory align warning
	u8		TotalRSSIByFw_0;
	u8		TotalRSSIByFw_1;
	u8		TotalRSSIByFw_2;
	u8		TotalRSSIByFw_3;
} RemoteWakeup_Parm, *PRemoteWakeup_Parm;

//H2C Index: 0x4
typedef struct _disconnect_parm_ {
	u8		Enable: 1;
	u8		AdoptUserSetting: 1;
	u8		TryOkBcnFailCountEn: 1;
	u8 		DisconnectEN: 1;
	u8		rsvd: 4;

	u8		DisconnectCountLimit;
	u8		TryPKTCountLimit;
	u8		TryOkBcnFailCountLimit;
	u8		DisconnectCount;
	u8		TryPKTCount;
	u8		BCNCount;
	u8		WHCKPatchEn;
} Disconnect_Parm, *PDisconnect_Parm;

//3 Class8: Testing
typedef struct _H2C_EVT_CONTENT {
	u8		Content[LENGTH_H2C];
} H2C_EVT_CONTENT, *PH2C_EVT_CONTENT;

typedef struct _H2C_EVT_ {
	H2C_EVT_CONTENT    H2CFWBuff[LENGTH_H2C_BUFF];
	u8		H2CRPtr;
	u8		H2CWPtr;
} H2C_EVT, *PH2C_EVT;

typedef struct _C2H_FW_EVT_ {
	C2H_EVT_HDR        c2h_sfbuff[LENGTH_C2H_BUFF];
	u8		c2h_r_ptr;
	u8		c2h_w_ptr;
} C2H_FW_EVT, *PC2H_FW_EVT;

/* H2C Index: 0x06 */
typedef struct _H2CParam_InitOffload_ {
	u8		Cmd_Index;
	u8		Loc_Data;
} H2CParam_InitOffload, *PH2CParam_InitOffload;

typedef struct _MEDIA_STATUS_ {
	BOOLEAN	bConnected[MACID_NUM_NEW];
	u8		MaxConnectedMacid;
} MEDIA_STATUS, *PMEDIA_STATUS;

/*--------------------Define --------------------------------------------*/
/* Register H2C Command ID here */
typedef enum _H2C_CMD_ {
	/* 1 Class1: Common */
	H2CID_RSVDPAGE								= 0x00,
	H2CID_JOININFO              					= 0x01,
	H2CID_SCAN                  					= 0x02,
	H2CID_KEEP_ALIVE            					= 0x03,
	H2CID_DISCONNECT_DECISION   				= 0x04,
	H2CID_PSD_OFFLOAD           					= 0x05,
	H2CID_Customer_string_rpt   				= 0x06,
	rsvd3                       						= 0x07,
	H2CID_AP_OFFLOAD            					= 0x08,
	H2CID_BCN_RsvdPage          					= 0x09,
	H2CID_Probersp_RsvdPage     				= 0x0A,
	H2CID_TXPower_Index_Offlaod 				= 0x0B,
	H2CID_AP_PS_OFFLOAD         				= 0x0C,
	H2CID_AP_PS_OFFLOAD_CTRL    				= 0x0D,
	H2CID_ECSA                  					= 0x0F,

	H2CID_FAST_CS_RSVDPAGE     	 			= 0x10,
	H2CID_FAST_CHANNEL_SWITCH   				= 0x11,
	H2CID_BB_GAIN_REPORT        				= 0x12,
	H2CID_GPIO_CTRL             					= 0x13,
	H2CID_HW_INFO               					= 0x14,
	H2CID_MCC_RQT_TSF      						= 0x15,
	H2CID_MCC_MACID_BITMAP     	 			= 0x16,
	H2CID_MCC_CTRL              					= 0x18,
	H2CID_MCC_TIME_PARAM         				= 0x19,
	H2CID_MCC_IQK_PARAM         				= 0x1A,
	H2CID_NAN_CTRL              					= 0x1B,
	H2CID_SINGLE_CHANNEL_SWITCH 				= 0x1C,
	H2CID_SINGLE_CHANNEL_SWITCH_V2 			= 0x1D,
	H2CID_TX_PAUSE_DRV_INFO					= 0x1E,
	H2CID_MACID_PAUSE_DRV_INFO  				= 0x1F,
	/* 1 Class2: Power Save */
	H2CID_SETPWRMODE            					= 0x20,
	H2CID_PSTURNINGPARM         				= 0x21,
	H2CID_PSTURNINGPARM2        				= 0x22,
	H2CID_PSLPSPARM             					= 0x23,
	H2CID_P2PPS_OFFLOAD         				= 0x24,
	H2CID_PS_SCAN               					= 0x25,
	H2CID_SAPPS                 					= 0x26,
	H2CID_INACTIVE_PS           					= 0x27,
	H2CID_NOLINK_PS             					= 0x28,
	H2CID_PARTIAL_OFF_CTRL   					= 0x29,
	H2CID_PARTIAL_OFF_PARM  					= 0x2A,
	H2CID_LPS_I_TWT_INFO1         				= 0x31,
	H2CID_LPS_I_TWT_INFO2         				= 0x32,
	H2CID_LPS_I_TWT_INFO3         				= 0x33,
	H2CID_S1_NULL1_CTRL         				= 0x3F,
	/* 1 Class3: Dynamic Mechaism */
	H2CID_MACID_CFG             					= 0x40,
	H2CID_TxBF                  					= 0x41,
	H2CID_RSSI_SETTING          					= 0x42,
	H2CID_AP_REQ_TXRPT          					= 0x43,
	H2CID_INIT_RATE_COLLECT     				= 0x44,
	H2CID_IQK_OFFLOAD           					= 0x45,
	H2CID_MACID_CFG_3SS        	 				= 0x46,
	H2CID_RA_PARA_ADJUST        				= 0x47,
	H2CID_DYNAMIC_TX_PATH       				= 0x48,
	H2CID_FW_TRACE_EN          		 			= 0x49,
	H2CID_MACID_CFG2							= 0x54,
	H2CID_ADJUST_RA_MASK						= 0x55,
	H2CID_RA_D_O_TIMER							= 0x56,
	H2CID_ADAPTIVITY							= 0x57,
	H2CID_CSI_Report								= 0x58,
	/* 1 Class4: BT Coex */
	H2CID_B_TYPE_TDMA           					= 0x60,
	H2CID_BT_INFO               					= 0x61,
	H2CID_FORCE_BT_TXPWR        				= 0x62,
	H2CID_BT_IGNORE_WLANACT     				= 0x63,
	H2CID_DAC_SWING_VALUE       				= 0x64,
	H2CID_ANT_SEL_REVERSE       				= 0x65,
	H2CID_WL_OPMODE             					= 0x66,
	H2CID_BT_MP_OPERATION       				= 0x67,
	H2CID_BT_CONTROL           			 		= 0x68,
	H2CID_BT_WIFICTRL           					= 0x69,
	H2CID_BT_PATCH_DOWNLOAD     				= 0x6A,
	H2CID_BT_SCO_eSCO_OPERATION				= 0x6B,
	H2CID_BT_Page_Scan_Interval			 	= 0x6C,
	H2CID_WL_Calibraion         				= 0x6D,
	H2CID_GNT_BT_CTRL           					= 0x6E,
	H2CID_BT_ONLY_TEST          					= 0x6F,
	H2CID_BT_INIT_PARAM         				= 0x70,
	H2CID_WL_PORT_ID            					= 0x71,
	/* 1 Class5: WOWLAN */
	H2CID_WoWLAN                					= 0x80,
	H2CID_RemoteWakeCtrl        				= 0x81,
	H2CID_AOAC_Global_info      				= 0x82,
	H2CID_AOAC_Rsvdpage1        				= 0x83,
	H2CID_AOAC_Rsvdpage2        				= 0x84,
	H2CID_D0_Scan_offload_info  				= 0x85,
	H2CID_D0_Scan_offload_ctrl  				= 0x86,
	H2CID_Switch_channel        				= 0x87,
	H2CID_AOAC_Rsvdpage3        				= 0x88,
	H2CID_GPIO_WF_Customize     				= 0x89,
	H2CID_P2P_RsvdPage          					= 0x8A,
	H2CID_P2P_Offload           					= 0x8B,
	/* 1 Class6: LTECOEX */
	H2CID_LTECOEX_EN            					= 0xA0,
	H2CID_WLAN_High_Priority    				= 0xA1,
	/* 1 Class7: FTM */
	H2CID_FTM                   						= 0xB0,
	H2CID_FTMRPT                					= 0xB1,
	/* 1 Class8: Patch */
	H2CID_TSF_RESET             					= 0xC0,
	H2CID_BB_NHM                					= 0xC1,
	H2CID_BCN_Ignore_EDCCA      				= 0xC2,
	H2CID_Efuse_Hidden_Request  				= 0xC3,
	H2CID_APoffload_MultiWakeupPulse 		= 0xC4,
	H2CID_BCN_HWSEQ	        					= 0xC5,   //8814A BCN HWSEQ issue for Seam
	H2CID_Customer_string1      				= 0xC6,
	H2CID_Customer_string2      				= 0xC7,
	H2CID_Customer_string3      				= 0xC8,
	H2CID_IOTDRV_CloseRF      					= 0xCF,
	/* 1 Class9: Testing */
	H2CID_H2C2HLB               					= 0xE0,
	H2CID_FW_Verification       				= 0xE4,
	/* 1 Class10:FW Offload */
	CMD_ID_FW_OFFLOAD_H2C       				= 0xFF
} H2C_CMD, *PH2C_CMD;

typedef enum _C2H_CMD_SUBID_ {
	C2HEXTID_DEBUG_PRT          = 0,


} C2H_CMD_SUBID, *PC2H_CMD_SUBID;

/*--------------------------Define MACRO--------------------------------------*/
#define FIX_DELAY_FORWARD					BIT5

//MACID Drop Action
#define MACID_DROP_DISABLE      			0
#define MACID_DROP_ENABLE       			1

/*------------------------------Define Struct---------------------------------*/
//================H2C command format====================
typedef struct _H2C_CMD_HDR {
	u8 cmd_id;
} H2C_CMD_HDR, *PH2C_CMD_HDR;

//3 Class1: Common
/* H2C Index: 0x0 */
typedef struct _H2CParam_RsvdPage_ {
	RsvdPageLoc loc;
} H2CParam_RsvdPage, *PH2CParam_RsvdPage;

/* H2C Index: 0x1 */
typedef struct _H2CParam_JoinInfo_ {
	BOOLEAN     bConnected: 1;
	BOOLEAN     bMacid_ind: 1;
	u8          Miracast: 1;
	u8          Miracast_role: 1;
	u8          Macid_Dest_Role: 4;
	u8          macid;
	u8          macid_end;
} H2CParam_JoinInfo, *PH2CParam_JoinInfo;

//3 Class2: Power Save
/* H2C Index: 0x20 */
typedef struct _H2CParam_SetPwrMode_parm_ {
	LEGACY_PS_PARM      PwrModeParm;
} H2CParam_PwrMode, *PH2CParam_PwrMode;

/* H2C Index: 0x21 */
typedef struct _H2CParam_PSTuningParm_ {
	PS_TUNING_PPARM    PSTuningParm;
} H2CParam_PSTuningParm, *PH2CParam_PSTuningParm;

/* H2C Index: 0x26 */
typedef struct _H2CParam_SAPPSParm_ {
	u8  Enable: 1;
	u8  EnPS: 1;
	u8  EnLPRx: 1;
	u8  Manual32k: 1;   //revised by isaac on 20141202
	u8  rsvd: 4;
	u8  Duration;
	u8 SegNum;
} H2CParam_SAPPSParm, *PH2CParam_SAPPSParm;

/* H2C Index: 0x27 */
typedef struct _H2CParam_INACTIVEPSParm_ {
	u8      Enable: 1;
	u8      IgnorePsCondition: 1;
	u8      rsvd: 6;
	u8      Frequency;
	u8      Duration;
	u8      FrequencyCount;
} H2CParam_INACTIVEPSParm, *PH2CParam_INACTIVEPSParm;

//3 Class3: Dynamic Mechaism
//H2C Index: 0x58
//TX null data
typedef struct _CSI_Txnull_Parm_ {
	u8		Enable;
	u16	TxNullRate;
	u8 		Macid;
	u8		NullPeriod;
	u8		TxRevAddr[6];
	u8		TxNullPowerBit;
	//u8		TxNull0Cycle;
	//u8		TxNull0CycleOK;
	//u8		TxNull0retrycnt;
	//u8		TxNull1Cycle;
	//u8		TxNull1CycleOK;
	//u8		TxNull1retrycnt;
	u8		TxNullCycle;
	u8		TxNullCycleOK;
	u8		TxNullretrycnt;
	u8		TxNullCycleOn;
} CSITxnull_Parm, *PCSITxnull_Parm;

typedef struct _CSI_Report_Parm_ {
	u8 		CSIWorkEn;
	u8 		TxNullPeriod;
	CSITxnull_Parm CSITxnullParm[CSICLIENTNUM];
} CSIReport_Parm, *PCSIReport_Parm;

//3 Class7: Class no define

//3 Class8: Testing

/* H2C Index: 0xE0 */
typedef struct _H2CParam_H2C2H_ {
	u8                 H2C2HPara;
} H2CParam_H2C2H, *PH2CParam_H2C2H;

/*--------------------Function declaration---------------------------------*/
//brian for function both called in isrhdl or not
extern void SendSignalCommon_8720E(u32  event);
extern void OSSendSignalCommon_8720E(u32  event);
extern void H2CInQueue_8720E(void);
extern void H2CDeQueue_8720E(void);
extern BOOLEAN WaitC2HOk_8720E(u16 TriggerAddr);
extern void InitC2H_8720E(void);
extern void C2HPKTRxDESC_8720E(u16 PktLength, u16 C2HDescOffset);
extern void FillC2HContant_8720E(u16 C2HPKTOffset, PC2H_EVT_HDR pc2h_hdr);
extern void IssueC2HPKT_8720E(PC2H_EVT_HDR pc2h_hdr);
extern void C2HDeQueue_8720E(void);
extern void C2HInQueue_8720E(PC2H_EVT_HDR pc2h_hdr);
extern u8 CheckMaxMacidNum_8720E(void);
extern void SetMediaStatus_8720E(u8 macid, u8 status);
extern BOOLEAN GetMediaStatus_8720E(u8 macid);
extern void MediaConnection_8720E(u8 macid);
extern void MediaDisconnection_8720E(u8 macid);
extern void C2HSpeRPT_8720E(void);
extern void H2CCmdFunc_8720E(u8  CmdID, u8  *content);
extern void SetTxPauseDRVInfo_8720E(u8 BitMask, u8 UserValue);
extern void MacidDropAction_8720E(u8 action, u8 macid);

#endif  /* __WIFIFW_CMD_RAM_H__ */
