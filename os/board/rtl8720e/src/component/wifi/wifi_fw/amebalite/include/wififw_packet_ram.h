#ifndef __WIFIFW_PACKET_RAM_H__
#define __WIFIFW_PACKET_RAM_H__

/*--------------------------Define -------------------------------------------*/
// retry limit
#define RTY_LMT_NULLDATA			8
#define RTY_LMT_QNULL				8
#define RTY_LMT_BTQNULL			8
#define RTY_LMT_ARP					24
#define RTY_LMT_PSPOLL				24
#define RTY_LMT_NDP					24
#define RTY_LMT_CTS2SELF			0	//Alan ,use 0 is correct, suggest by Ausin-Lo

//issue rsvdpage marco
#define _ISSUE_BTQNULL				1
#define _ISSUE_DBGPKT_				2
#define _ISSUE_FCSNULL_			3
#define _ISSUE_LTEQNULL_      		4
#define _ISSUE_NDPA_           		5
#define _ISSUE_PROBERSP_			6
#define _ISSUE_PROBEREQ_			7
#define _ISSUE_GTKRSP_            	8
#define _ISSUE_PSPOLL_            	9
#define _ISSUE_NULL_             	10
#define _ISSUE_QNULL_           	11
#define _ISSUE_CTS2SELF_        	12
#define _ISSUE_REALWOW_KA_     	13
#define _ISSUE_RS_          			14
#define _ISSUE_BUBBLE_        		15
#define _ISSUE_ARP_         			16
#define _ISSUE_NA_            		17

//CPU_MGT_INFORMATION
#define CPUMGT_POLL					BIT5
#define P0BCN_POLL					BIT4
#define P1BCN_POLL					BIT6
#define NANBCN_POLL					BIT7//used for port2
/*--------------------------Define MACRO--------------------------------------*/

#define RATE_CCK_1M					0x00
#define RATE_CCK_2M					0x01
#define RATE_CCK_5M					0x02
#define RATE_CCK_11M				0x03
#define RATE_OFDM_6M        		0x04
#define RATE_OFDM_9M        		0x05
#define RATE_OFDM_12M       		0x06
#define RATE_OFDM_18M       		0x07
#define RATE_OFDM_24M       		0x08
#define RATE_OFDM_36M       		0x09
#define RATE_OFDM_48M       		0x0A
#define RATE_OFDM_54M       		0x0B
#define RATE_HT_MCS0        		0x0C
#define RATE_HT_MCS1        		0x0D
#define RATE_HT_MCS2        		0x0E
#define RATE_HT_MCS3        		0x0F
#define RATE_HT_MCS4        		0x10
#define RATE_HT_MCS5        		0x11
#define RATE_HT_MCS6        		0x12
#define RATE_HT_MCS7        		0x13
#define RATE_HT_MCS8        		0x14
#define RATE_HT_MCS9        		0x15
#define RATE_HT_MCS10       		0x16
#define RATE_HT_MCS11       		0x17
#define RATE_HT_MCS12       		0x18
#define RATE_HT_MCS13				0x19
#define RATE_HT_MCS14       		0x1A
#define RATE_HT_MCS15       		0x1B
#define RATE_HT_MCS16       		0x1C
#define RATE_HT_MCS17				0x1D
#define RATE_HT_MCS18       		0x1E
#define RATE_HT_MCS19       		0x1F
#define RATE_HT_MCS20       		0x20
#define RATE_HT_MCS21       		0x21
#define RATE_HT_MCS22       		0x22
#define RATE_HT_MCS23       		0x23
#define RATE_HT_MCS24       		0x24
#define RATE_HT_MCS25       		0x25
#define RATE_HT_MCS26       		0x26
#define RATE_HT_MCS27       		0x27
#define RATE_HT_MCS28       		0x28
#define RATE_HT_MCS29       		0x29
#define RATE_HT_MCS30       		0x2A
#define RATE_HT_MCS31       		0x2B
#define RATE_VHT1SS_MCS0    		0x2C
#define RATE_VHT1SS_MCS1    		0x2D
#define RATE_VHT1SS_MCS2    		0x2E
#define RATE_VHT1SS_MCS3    		0x2F
#define RATE_VHT1SS_MCS4    		0x30
#define RATE_VHT1SS_MCS5    		0x31
#define RATE_VHT1SS_MCS6    		0x32
#define RATE_VHT1SS_MCS7    		0x33
#define RATE_VHT1SS_MCS8    		0x34
#define RATE_VHT1SS_MCS9    		0x35
#define RATE_VHT2SS_MCS0    		0x36
#define RATE_VHT2SS_MCS1    		0x37
#define RATE_VHT2SS_MCS2    		0x38
#define RATE_VHT2SS_MCS3    		0x39
#define RATE_VHT2SS_MCS4    		0x3A
#define RATE_VHT2SS_MCS5    		0x3B
#define RATE_VHT2SS_MCS6    		0x3C
#define RATE_VHT2SS_MCS7    		0x3D
#define RATE_VHT2SS_MCS8    		0x3E
#define RATE_VHT2SS_MCS9    		0x3F
#define RATE_VHT3SS_MCS0    		0x40
#define RATE_VHT3SS_MCS1    		0x41
#define RATE_VHT3SS_MCS2    		0x42
#define RATE_VHT3SS_MCS3    		0x43
#define RATE_VHT3SS_MCS4    		0x44
#define RATE_VHT3SS_MCS5    		0x45
#define RATE_VHT3SS_MCS6    		0x46
#define RATE_VHT3SS_MCS7    		0x47
#define RATE_VHT3SS_MCS8    		0x48
#define RATE_VHT3SS_MCS9    		0x49
#define RATE_VHT4SS_MCS0    		0x4A
#define RATE_VHT4SS_MCS1    		0x4B
#define RATE_VHT4SS_MCS2    		0x4C
#define RATE_VHT4SS_MCS3    		0x4D
#define RATE_VHT4SS_MCS4    		0x4E
#define RATE_VHT4SS_MCS5    		0x4F
#define RATE_VHT4SS_MCS6    		0x50
#define RATE_VHT4SS_MCS7    		0x51
#define RATE_VHT4SS_MCS8    		0x52
#define RATE_VHT4SS_MCS9    		0x53
//add HE rate here
#define RATE_HE1SS_MCS0				0x54
#define RATE_HE1SS_MCS1				0x55
#define RATE_HE1SS_MCS2				0x56
#define RATE_HE1SS_MCS3				0x57
#define RATE_HE1SS_MCS4				0x58
#define RATE_HE1SS_MCS5				0x59
#define RATE_HE1SS_MCS6				0x5A
#define RATE_HE1SS_MCS7				0x5B
#define RATE_HE1SS_MCS8				0x5C
#define RATE_HE1SS_MCS9				0x5D
#define RATE_HE1SS_MCS10			0x5E
#define RATE_HE1SS_MCS11			0x5F
#define RATE_HE2SS_MCS0				0x60
#define RATE_HE2SS_MCS1				0x61
#define RATE_HE2SS_MCS2				0x62
#define RATE_HE2SS_MCS3				0x63
#define RATE_HE2SS_MCS4				0x64
#define RATE_HE2SS_MCS5				0x65
#define RATE_HE2SS_MCS6				0x66
#define RATE_HE2SS_MCS7				0x67
#define RATE_HE2SS_MCS8				0x68
#define RATE_HE2SS_MCS9				0x69
#define RATE_HE2SS_MCS10			0x6A
#define RATE_HE2SS_MCS11			0x6B

#if 0
#define RATE_HE_NSS1_MCS0			0x60
#define RATE_HE_NSS1_MCS1			0x61
#define RATE_HE_NSS1_MCS2			0x62
#define RATE_HE_NSS1_MCS3			0x63
#define RATE_HE_NSS1_MCS4			0x64
#define RATE_HE_NSS1_MCS5			0x65
#define RATE_HE_NSS1_MCS6			0x66
#define RATE_HE_NSS1_MCS7			0x67
#define RATE_HE_NSS1_MCS8			0x68
#define RATE_HE_NSS1_MCS9			0x69
#define RATE_HE_NSS1_MCS10		0x6A
#define RATE_HE_NSS1_MCS11		0x6B
#endif
/*------------------------------Define Struct---------------------------------*/
typedef struct  _RsvdPageLoc_ {
	u8      loc_probersp;
	u8      loc_pspoll;
	u8      loc_nulldata;
	u8      loc_QosNull;
	u8      loc_BT_QoSNull;
	u8      loc_CTS2Self;
	u8      loc_LTECOEX_QoSNull;
} RsvdPageLoc, *PRsvdPageLoc;

/*----------------------------Define-------------------------------------*/
#define TX_PKT_WAIT_Q_SIZE			16
#define TX_PKT_BUSY_Q_SIZE			1
#define IS_CPUMGQ_TXING()				(!CheckCPUMGQEmpty())
#define CPUMGQ_TX_TIMEOUT_US			5000

enum TSF_HIGH_LOW_VALUE {
	TSF_LOW         = 0,
	TSF_HIGH        = 1
};

typedef enum _TX_RESULT_ {
	TXRESULT_TX_DONE 				= 0,
	TXRESULT_TX_TIMEOUT 		= 1,
	TXRESULT_TX_PREEMPTED 		= 2,
	TXRESULT_CANCELED 			= 3,
	TXRESULT_TX_ACKED 			= 4,
	TXRESULT_TX_DROPPED 		= 5,
	TXRESULT_MAX
} TX_RESULT, *PTX_RESULT;
//TX PS Poll
typedef struct {
	u16 		RsvPageLoc;
	u8			RtyLmt;
} TX_PSPOLL_PKT_CONTEXT, *PTX_PSPOLL_PKT_CONTEXT;

//TX null data
typedef struct {
	u16 		u2RsvPageLoc;
	BOOLEAN 	bPowerBit;
	u8			RtyLmt;
} TX_NULL_PKT_CONTEXT, *PTX_NULL_PKT_CONTEXT;
//CTS2Self
typedef struct {
	u8 			location;
	u16 		NAV;
} TX_CTS_TO_SELF_PKT_CONTEXT, *PTX_CTS_TO_SELF_PKT_CONTEXT;
//Arp
typedef struct {
	u8  		TargetIP[4];
	u8  		TargetMAC[6];
	u8  		retry_bit;
	u8  		arpsel;
} TX_ARP, *PTX_ARP;
//NA
typedef struct {
	u8    		TargetIP[4];
	u8    		TargetMAC[6];
	u8    		OurIP[4];
	u8    		OurMAC[6];
	u8    		retry_bit;
	u8    		ndpsel;
} TX_NA, *PTX_NA;

typedef union _TX_PKT_CONTEXT_ {
	TX_NULL_PKT_CONTEXT			Null;
	TX_PSPOLL_PKT_CONTEXT		PSPOLL;
	TX_CTS_TO_SELF_PKT_CONTEXT	CTS2Self;
	TX_ARP	ARP;
	TX_NA		NA;
} TX_PKT_CONTEXT, *PTX_PKT_CONTEXT;

typedef BOOLEAN(*PRE_TX_CALLBACK)(PTX_PKT_CONTEXT, u32);
typedef void (*POST_TX_CALLBACK)(TX_RESULT, u32);

typedef struct _TX_PKT_ENTRY_ {
	PRE_TX_CALLBACK		PreTxCallback;
	POST_TX_CALLBACK		PostTxCallback;
	u32 		IssueTxTime;
	u32 		HWTxTime;
	TX_PKT_CONTEXT		TxPktCxt;
	u8 			id;
	u8 			canceled;
	u16 		timeout;
	u8 			has_txfb;
} TX_PKT_ENTRY, *PTX_PKT_ENTRY;

/*--------------------Function declaration---------------------------------*/
extern void InitFreeRunTSF_8720E(void);
extern u64 GetFreeRunCnt_8720E(void);
extern u32 GetFreeRunCntU32_8720E(BOOLEAN u1HighLowValue);
extern u8 TxPktPollMgntQ_8720E(void);
extern BOOLEAN CPUMGQCancelTx_8720E(void);
extern void SWQTXCheckBusyQState_8720E(void);
extern BOOLEAN IsSWQTXAllQEmpty_8720E(void);
extern BOOLEAN PS_PreTxNull_8720E(PTX_PKT_CONTEXT  pcontext, u32 queue_time);
extern void PS_PostTxNull1_8720E(IN  TX_RESULT result, IN  u32 tx_time);
extern void PS_PostTxNull0_8720E(IN  TX_RESULT result, IN  u32 tx_time);
extern void PostTxNull_8720E(IN  TX_RESULT result, IN  u32 tx_time);
extern u8 CheckTxNullResult_8720E(u8 PORT_ID, u8 Null_Type);
extern void SWQTXTxFeedBack_8720E(u8 acked/*, void* context*/);
extern u8 SWQTXEnqueueWaitQ_8720E(TX_PKT_ENTRY txpkt_entry);
#if 0
extern BOOLEAN PreTxPSPoll_8720E(PTX_PKT_CONTEXT  pcontext, u32 queue_time);
extern void PostTxPSPoll_8720E(IN  TX_RESULT result, IN  u32 tx_time);
extern BOOLEAN PreTxNA_8720E(PTX_PKT_CONTEXT  pcontext, u32 queue_time);
extern void PostTxNA_8720E(IN	TX_RESULT result, IN  u32 tx_time);
extern BOOLEAN PreTxARP_8720E(PTX_PKT_CONTEXT	pcontext, u32 queue_time);
extern void PostTxARP_8720E(IN  TX_RESULT result, IN  u32 tx_time);
#endif
extern void ChangeTransmiteRate_8720E(u16 offset) ;
extern void PowerBitSetting_8720E(u8 bPowerBit, u16 offset);
extern void IssuePSPoll_8720E(void);
extern void ChkandChangePS_8720E(u8 bPowerBit);
extern void IssueQNull_8720E(u8 bPowerBit);
extern void CTS2SelfSetting_8720E(u16 offset, u16 NAV);
extern void IssueCTS2Self_8720E(u8 location, u16 NAV);
extern u16 IssueRsvdPagePacketSetting_8720E(u8 PageNum,  BOOLEAN bHwSEQEn,  u8 RtyLmt,  u8 TxInd);
extern u8 ReadRxBuff_8720E(u16 headaddr, u8  offset);
extern BOOLEAN WaitCPUMGQEmpty_8720E(void);
extern u8 PollingReg_8720E(u16 RegOffset, u8 PollingBit, u8 PollingResult, u16 PollingLimit, u16 *PollingCount);
extern BOOLEAN IssueNullData_8720E(u8 bPowerBit, u8 RtyLmt);
extern BOOLEAN CheckCPUMGQEmpty_8720E(void);

#endif  /* __WIFIFW_PACKET_RAM_H__ */
