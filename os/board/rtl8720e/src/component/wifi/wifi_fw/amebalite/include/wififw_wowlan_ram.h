#ifndef __WIFIFW_WoWLAN_RAM_H__
#define __WIFIFW_WoWLAN_RAM_H__

/*--------------------Define MACRO--------------------------------------*/
#define IOT_WAITHOST_TO     5
#define UDP_LENGTH_OFFSET                   4
#define UDP_CHECK_SUM_OFFSET                6
#define ROL32( A, n )	( ((A) << (n)) | ( ((A)>>(32-(n)))  & ( (1UL << (n)) - 1 ) ) )
#define ROR32( A, n ) 	ROL32( (A), 32-(n) )

#define FORCE_WAKE_UP								BIT0
#define PATTERN_MATCH_EN            				BIT1
#define MAGIC_PKT_EN                				BIT2
#define UNICAST_EN                  					BIT3
#define WOWHCI_EN                   					BIT5
#define REG_WoW_REASON          					REG_DBG_BYTE_5   //1C7
#define RXDMA_IDLE                  					BIT1
#define STOP_RXDMA                  					BIT2
//3 wowlan rx used type
#define Rx_Pairwisekey          					0x01
#define Rx_GTK                  						0x02
#define Rx_Fourway_Handshake    					0x03
#define Rx_DisAssoc             						0x04
#define Rx_DeAuth               						0x08
#define Rx_ArpRequest           					0x09
#define Rx_NS                       					0x0a
#define RX_EAPREQ_IDENTIFY 						0x0b
#define FWDecisionDisconnect    					0x10
#define Rx_MagicPkt             						0x21
#define Rx_UnicastPkt           					0x22
#define Rx_PatternPkt           					0x23
#define RTD3_SSID_Match         					0x24
#define HW_RX_WAKEUP_PATTERN    					0x25
#define RX_RealWoW_V2_WakeupPkt 				0x30
#define RX_RealWoW_V2_AckLost   				0x31
#define Enable_Fail_DMAIdle     					0x40
#define Enable_Fail_DMAPause    					0x41
#define RTime_Fail_DMAIdle      					0x42
#define RTime_Fail_DMAPause     					0x43
#define RxShift_Error      	 						0x44
#define BT_Wakeup_Host          					0x45
#define NLO_SSID_Match          					0x55
#define AP_Offload_WakeUp       					0x66
#define CLK_32K_UNLOCK          					0xFD
#define CLK_32K_LOCK            					0xFE

#define DEFAULT_DISCONNECT_COUNT_LIMIT      	8       /*UNIT: beacon interval*/
#define DEFAULT_TRY_PKT_LIMIT               	3
#define KeepAliveUnit                       		10      /*UNIT: beacon interval*/
#define DEFAULT_KEEP_ALIVE_COUNT_LIMIT      	0x0A    /*Unit: BCN interval*/
#define WAIT_RXDMA_CNT                      		(2000)
#define CHECK_RXDMA_ONCE_TIME               	(10)
#define MAGIC_WAKEUP                        		BIT7
#define UNICAST_WAKEUP                      		BIT6
#define PATTERN_WAKEUP                      		BIT5
#define DMAIDLE                             			BIT1
#define DMAPAUSE                            			BIT2
#define ProtectedFrame                      		BIT6

#define ARP_SEL_PASSIVE_RESPONSE 				0
#define ARP_SEL_KEEPALIVE_UNICAST_GATEWAY	1

#define NDP_SEL_PASSIVE_RESPONSE            	0
#define NDP_SEL_KEEPALIVE_UNICAST_GATEWAY	1

#define KEEPALIVE_TYPE_NULLDATA             	0
#define KEEPALIVE_TYPE_ARP_RESPONSE         	1

#define ARP_ACTION_ARP_RESPONSE             	0
#define ARP_ACTION_WAKEUP_HOST              	1

#define WAKEUP_HOST_FROM_AP_OFFLOAD         	0
#define WAKEUP_HOST_FROM_WOWLAN             	1

/* WakeupHostDecision function Control */
#define CHECK_WAKEUP_BY_DESC                		BIT0
#define CHECK_WAKEUP_BY_INT                 		BIT1

/* below is IPv6 header offset */
#define IPv6_ALEN               						16
#define IPv6_NEXT_HEADER_OFFSET 				6
#define IPv6_SRC_ADDRESS_OFFSET 				8
#define IPv6_DES_ADDRESS_OFFSET		 			24
#define IPv6_HEADER             						40
#define ICMPV6_CHECKSUM_OFFSET         2

/* below is IPv6 header offset */
#define ICMPV6_TARGETADDRESS_OFFSET        	8
#define ICMPV6_OPTION_TARGET_ADDR_OFFSET	26
#define ICMPV6_HEADER           					32

#define SHA1_MAC_LEN 								20
#define LLC_LEN 										8
#define IEEE802_1X_HDR_LEN 						4
#define EAPOL_KEY_RPYCNT_SIZE 					8
#define WPA_REPLAY_COUNTER_LEN 					8
#define WPA_NONCE_LEN 								32
#define SYMBOL_CHECK_PTK                    		BIT0
#define SYMBOL_CHECK_GTK                    		BIT1

#define AOAC_REPORT_VERSION      0x1
/*--------------------Define Enum---------------------------------------*/
enum WIFI_FRAME_TYPE {
	WIFI_MGT_TYPE				= (0),
	WIFI_CTRL_TYPE 			= (BIT2),
	WIFI_DATA_TYPE 			= (BIT3),
};
//add 80211ax newtype here 20211021
enum WIFI_FRAME_SUBTYPE {
	/* below is for mgt frame */
	WIFI_ASSOCREQ       		= (0 | WIFI_MGT_TYPE),
	WIFI_ASSOCRSP       		= (BIT4 | WIFI_MGT_TYPE),
	WIFI_REASSOCREQ     		= (BIT5 | WIFI_MGT_TYPE),
	WIFI_REASSOCRSP     		= (BIT5 | BIT4 | WIFI_MGT_TYPE),
	WIFI_PROBEREQ       		= (BIT6 | WIFI_MGT_TYPE),
	WIFI_PROBERSP       		= (BIT6 | BIT4 | WIFI_MGT_TYPE),
	WIFI_BEACON         		= (BIT7 | WIFI_MGT_TYPE),
	WIFI_ATIM          	 		= (BIT7 | BIT4 | WIFI_MGT_TYPE),
	WIFI_DISASSOC       		= (BIT7 | BIT5 | WIFI_MGT_TYPE),
	WIFI_AUTH           			= (BIT7 | BIT5 | BIT4 | WIFI_MGT_TYPE),
	WIFI_DEAUTH         		= (BIT7 | BIT6 | WIFI_MGT_TYPE),
	WIFI_ACTION         		= (BIT7 | BIT6 | BIT4 | WIFI_MGT_TYPE),

	/* below is for control frame */
	WIFI_PSPOLL         		= (BIT7 | BIT5 | WIFI_CTRL_TYPE),
	WIFI_RTS            			= (BIT7 | BIT5 | BIT4 | WIFI_CTRL_TYPE),
	WIFI_CTS            			= (BIT7 | BIT6 | WIFI_CTRL_TYPE),
	WIFI_ACK            			= (BIT7 | BIT6 | BIT4 | WIFI_CTRL_TYPE),
	WIFI_CFEND          		= (BIT7 | BIT6 | BIT5 | WIFI_CTRL_TYPE),
	WIFI_CFEND_CFACK    		= (BIT7 | BIT6 | BIT5 | BIT4 | WIFI_CTRL_TYPE),
	//new for ax add 211022 david
	WIFI_TRIGGER				= (BIT5 | WIFI_CTRL_TYPE),
	WIFI_NDP_ANCMT			= (BIT6 | BIT4 | WIFI_CTRL_TYPE),

	/* below is for data frame */
	WIFI_DATA           			= (0 | WIFI_DATA_TYPE),
	WIFI_DATA_CFACK     		= (BIT4 | WIFI_DATA_TYPE),
	WIFI_DATA_CFPOLL    		= (BIT5 | WIFI_DATA_TYPE),
	WIFI_DATA_CFACKPOLL 	= (BIT5 | BIT4 | WIFI_DATA_TYPE),
	WIFI_DATA_NULL      		= (BIT6 | WIFI_DATA_TYPE),
	WIFI_CF_ACK         		= (BIT6 | BIT4 | WIFI_DATA_TYPE),
	WIFI_CF_POLL        		= (BIT6 | BIT5 | WIFI_DATA_TYPE),
	WIFI_CF_ACKPOLL     		= (BIT6 | BIT5 | BIT4 | WIFI_DATA_TYPE),
	WIFI_QOS_DATA       		= (BIT7 | WIFI_DATA_TYPE),
	WIFI_QOS_NULL       		= (BIT7 | BIT6 | WIFI_DATA_TYPE),
};

/*--------------------Define Struct---------------------------------------*/
typedef struct _AOAC_REPORT_ {
	u8		pPTKTXIV[8];
	u8		pReplayCntEapolKey[8];
	u8   	pGroupKey[32];          /*the last updated GTK data*/
	u8   	u1KeyIdx;               /*the last updated GTK index*/
	u8   	u1SecurityType;         /*per received packet*/
	u8   	u1WowPatternIdx;        /*per received packet*/
	u8   	u1RptFormatVer;         /*In old FW, this is a 1-byte rsvd, FW clear this byte certainly*/
	u8   	u1Rsvd[4];              /*In old FW, these 4 bytes are not include in AOAC rpt. Don't use these 4 bytes. Just for 8-byte alignment*/
	u8   	u4RxPTKIV[8];           /*Unicast IV*/
	u8   	u4RxGTKIV_0[8];         /*Broadcast/Mulicast IV, 4 GTK index*/
	u8   	u4RxGTKIV_1[8];
	u8   	u4RxGTKIV_2[8];
	u8   	u4RxGTKIV_3[8];
} AOAC_REPORT, *PAOAC_REPORT;

typedef enum {
	NO_Encryption				= 0,
	WEP40_Encryption  		= 1,
	TKIP_Encryption   		= 2,
	Reserved_Encryption  	= 3,
	AESCCMP_Encryption  		= 4,
	WEP104_Encryption  		= 5,
} RT_ENC_ALG, *PRT_ENC_ALG;

typedef struct _ieee802_1x_hdr {
	u8 		version;
	u8 		type;
	u16 	length;
	/* followed by length octets of data */
} ieee802_1x_hdr, *pieee802_1x_hdr;

typedef struct _wpa_eapol_key {
	u8 		type;
	/* Note: key_info, key_length, and key_data_length are unaligned */
	u8 		key_info[2];
	u8 		key_length[2];
	u8 		replay_counter[WPA_REPLAY_COUNTER_LEN];
	u8 		key_nonce[WPA_NONCE_LEN];
	u8 		key_iv[16];
	u8 		key_rsc[8];
	u8 		key_id[8]; /* Reserved in IEEE 802.11i/RSN */
	u8	 	key_mic[16];
	u8 		key_data_length[2];
	/* followed by key_data_length bytes of key_data */
} wpa_eapol_key, *pwpa_eapol_key;

typedef struct _H2C_EAPOL_KEY_STRUCT {
	ieee802_1x_hdr	ieee802_1x_hdr_data;
	wpa_eapol_key		eapol_key_data;
	u8		key_data[128];
} H2C_EAPOL_KEY_STRUCT, *pH2C_EAPOL_KEY_STRUCT;

typedef struct _H2C_WPA_PTK {
	u8		kck[16]; /* EAPOL-Key Key Confirmation Key (KCK) */
	u8		kek[16]; /* EAPOL-Key Key Encryption Key (KEK) */
	u8  	tk1[16]; /* Temporal Key 1 (TK1) */
	union {
		u8		tk2[16]; /*Temporal Key 2 (TK2)*/
		struct {
			u8  tx_mic_key[8];
			u8  rx_mic_key[8];
		} Athu;
	} U;
} H2C_WPA_PTK;

typedef struct _H2C_WPA_TWO_WAY_PARA {
	H2C_WPA_PTK     wpa_ptk_value;
} H2C_WPA_TWO_WAY_PARA, *PH2C_WPA_TWO_WAY_PARA;

typedef struct _SECURITY_INFO_ {
	H2C_WPA_TWO_WAY_PARA wpa_two_way_para;
	H2C_EAPOL_KEY_STRUCT gtk_req;
} SECURITY_INFO, *PSECURITY_INFO;

typedef struct _ARP_INFO_ {
	u8		A3Addr[6];/*gateway's address*/
	u8		IPV4[4];
	u8		THA[6];
	u8 		TPA[4];
} ARP_INFO, *PARP_INFO;

typedef struct _NDP_INFO_ {
	u8 		Enable: 1;
	u8 		bCheckRemoveIP: 1;  /*Need to Check Sender IP or not*/
	u8 		Rsvd: 6;  /*Need to Check Sender IP or not*/
	u8 		NumberOfTargetIP; /*Number of Check IP which NA query IP*/
	u8 		TagetLinkAddress[6];  /*Maybe support change MAC address !!*/
	u8 		RemoteIPv6Address[16]; /*Just respond IP*/
	u8 		TargetIP[2][16]; /*target IP*/
} NDP_INFO, *PNDP_INFO;

typedef struct _IPV6_PSEUDO_HEADER_INFO {
	u8 		SourceAddress[16];
	u8 		DestinationAddress[16];
	u32 	PayPloadLength;
	u8 		rsvd1[3];
	u8 		NextHeader;
} IPV6_PSEUDO_HEADER_INFO, *PIPV6_PSEUDO_HEADER_INFO;

typedef struct _AOAC_INFO_ {
	ARP_INFO    ARPInfo;
	NDP_INFO    NDPInfo[2];
	IPV6_PSEUDO_HEADER_INFO Ipv6PseudoHeaderInfo;
	SECURITY_INFO SecurityInfo;
	AOAC_REPORT AOACReport;
} AOAC_INFO, *PAOAC_INFO;

struct mic_data {
	u32  	K0, K1;         // Key
	u32  	L, R;           // Current state
	u32  	M;              // Message accumulator (single word)
	u32 	nBytesInM;      // # bytes in M
};
/*--------------------Define Enum---------------------------------------*/

typedef enum _CHECK_SUM_PROTOCOL_ {
	IP_CHECK_SUM    = 0,
	TCP_CHECK_SUM   = 6,
	UDP_CHECK_SUM   = 17,
	ICMPV6_CHECK_SUM = 58,
} CHECK_SUM_PROTOCOL, *PCHECK_SUM_PROTOCOL;

/*--------------------Function declaration---------------------------------*/
extern u8 CheckIV_8720E(u8 *IVToCheck, u8 raaddr, BOOLEAN unicast_check);
extern u8 DropCHK_8720E(void);
extern BOOLEAN WaitRXDMAIdle_8720E(void);
extern BOOLEAN CHECKRXPKTNULL_8720E(void);
extern void UpdateChInfoReadPtr_DropPkt_8720E(u16 RPtr);
extern void UpdatePhyStsReadPtr_DropPkt_8720E(u16 RPtr);
extern void RxPHYSTSRelease_8720E(void);
extern void RxCHInfoRelease_8720E(void);
extern void EnableWoWLAN_8720E(void);
extern void DisWoWLAN_8720E(void);
extern BOOLEAN DropDecision_8720E(void);
extern void FwDisConnectWakeUpHost_8720E(IN u8 type);
extern BOOLEAN Ipv4Match_8720E(u16 addrl_frame_ctrl, u8 macHeaderLen);
extern BOOLEAN MatchUnicastFilter_8720E(u16 addrl_frame_ctrl, u8 macHeaderLen);
extern BOOLEAN WakeUpHostDecision_8720E(u8 RxDescWakeReason, u8 CheckMode);
extern BOOLEAN DestinationMatch_8720E(u16 addrl_frame_ctrl);
extern void UpdateRxFFReadPtr_DropPkt_8720E(u16 RPtr);
extern void H2CHDL_WoWLAN_8720E(u8 *pbuf);
extern void H2CHDL_AOACGlobalInfo_8720E(u8 *pbuf);
extern void H2CHDL_AOACRsvdpage1_8720E(u8 *pbuf);
extern void H2CHDL_AOACRsvdpage2_8720E(u8 *pbuf);
extern void H2CHDL_AOACRsvdpage3_8720E(u8 *pbuf);
extern void GetAddr_8720E(void);
extern void ARP_hdl_8720E(u8 bFunEnable);
extern void NDP_hdl_8720E(u8 bFunEnable);
extern BOOLEAN OnARP_8720E(u16 addrl_frame_ctrl, u8 MacHeaderLen);
extern void GetARPInfo_8720E(u8 pagenum);
extern void GetNDPInfo_8720E(u8 pagenum);
extern BOOLEAN OnNS_8720E(u16 addrl_frame_ctrl, u8 MacHeaderLen);
extern BOOLEAN IsIPV6_8720E(u16 addrl_frame_ctrl, u8 MacHeaderLen);
extern void PassSecurityInfoToDriver_8720E(u8 pagenum);
extern void GetRemoteControlInfo_8720E(u8 pagenum);
extern void InitDisconnectDecision_8720E(void);
extern void H2CHDL_DisconnectDecision_8720E(u8 *pbuf);
extern void InitKeepAlive_8720E(void);
extern void H2CHDL_KeepAlive_8720E(u8 *pbuf);
extern u16 PageOffsetAlignment_8720E(IN u16 offset);
extern void GetIV_8720E(u16 addrl_frame_ctrl, u8 MacHeaderLen, u8 IvLen, u8 *IV);
extern u8 GetIVLenAndSecurityType_8720E(u8 raaddr);
extern u8 GetMACHeaderLen_8720E(u8 macheader, u8 FrameCtrl, u8 raaddr);
extern void UpdateIV_8720E(u8 *buf, u8 retry_bit);
extern s8 RTmemcmpBackward_8720E(const void *Dest, const void *Src, u8 count);
extern u8 AesTkipIvCheck_8720E(u8 *rx_iv_ori, u8 *rx_iv, u8 SecurityType);
extern u8 WepIvCheck_8720E(u8 *rx_iv_ori, u8 *rx_iv);
extern void AssignIvToKeyRsc_8720E(u8 *pattrib_iv, u8 *KeyRsc, u8 *KeyID, u8 SecurityType);
extern void AssignKeyRscToIV_8720E(u8 *pattrib_iv, u8 *KeyRsc, u8 SecurityType);
extern void AesTkipIvFun_8720E(u8 *pattrib_iv, u8 RetryBit, u8 SecurityType);
extern void WepIvFun_8720E(u8 *iv, u8 retry_bit);
extern void DisconnectChk_8720E(void);
extern void DisconnectTxNullChk_8720E(IN u8 pwr);
extern void InitRemoteWakeUp_8720E(void);
extern void RemoteWakeUp_8720E(void);
extern void ClearRxBuff_8720E(void);
extern void H2CHDL_RemoteWakeUp_8720E(u8 *pbuf);
extern void issue_ARP_8720E(u8 *TargetIP, u8 *TargetMAC, u8  retry_bit, u8  arpsel);
extern u32 GetSumValue_8720E(u32 offset, u16 pktLen);
extern u16 GetCheckSum_8720E(u8 protocol, u16 pktLen, u32 src_addr, u32 dest_addr, u32 input);
extern void UpdateCheckSumVer2_8720E(u8 mode, u32 offset);
extern void FillIpv6PseudoHeader_8720E(u8 *TargetIP, u8 *OurIP, u32  PayLoadLength, u8  NextHeader);
extern void IssueNA_8720E(u8 *TargetIP, u8 *TargetMAC, u8 *OurIP, u8 *OurMAC, u8  retry_bit, u8  ndpsel);
extern void KeepAliveChk_8720E(void);
extern void TryPKT_8720E(void);
extern void WakeUpHost_8720E(u8 source);

#endif  /* __WIFIFW_WoWLAN_RAM_H__ */
