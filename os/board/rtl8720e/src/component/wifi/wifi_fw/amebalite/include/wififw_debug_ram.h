#ifndef __WIFIFW_DEBUG_RAM_H__
#define __WIFIFW_DEBUG_RAM_H__
/*--------------------------Define -------------------------------------------*/
//
//**Note**:
//      If one of these register is used, we should register MACRO at RTLXXX_debug.h.
//
// Range List:
//      1.) 0x1B8 ~ 0x1BF : there are no control circuit. But these register can be R/W.
//      2.) 0x1C0 ~ 0x1C7: there are no control circuit. But these register can be R/W.
//      3.) 0x084 ~ 0x08F: there are no control circuit. But these register can be R/W.
//2 page 0 rsvd REG USERDEF

#define REG_FW_DBG0_V1							0x0050
#define REG_FW_DBG1_V1                         0x0054
#define REG_FW_DBG2_V1                     	0x0058
#define REG_FW_DBG3_V1                     	0x005C
#define REG_FW_DBG4_V1                       	0x0060
#define REG_FW_DBG5_V1                     	0x0064
#define REG_FW_DBG6_V1                     	0x0068
#define REG_FW_DBG7_V1                        	0x006C

#define REG_BLK_TST                           	0x0084
#define REG_HOST_MSG0                         	0x0088
#define REG_HOST_MSG1                       	0x008C

#define REG_SW_DEFINED_PAGE1             	0x01B8
#define REG_SW_DEFINED_PAGE2              	0x01BC
#define REG_MCUTST_I                        	0x01C0
#define REG_MCUTST_II                      		0x01C4

#define REG_FWLBK_DBG0			            	0x1100
#define REG_FWLBK_DBG1				    		0x1104
#define REG_FWLBK_DBG2				    		0x1108
#define REG_FWLBK_DBG3				    		0x110C

//3 1byte
#define REG_DBG_BYTE_0				    		REG_MCUTST_I		//1c0
#define REG_DBG_BYTE_1				    		REG_MCUTST_I+1		//1c1
#define REG_DBG_BYTE_2				    		REG_MCUTST_I+2		//1c2
#define REG_DBG_BYTE_3				    		REG_MCUTST_I+3		//1c3
#define REG_DBG_BYTE_4				    		REG_MCUTST_II+2		//1c6
#define REG_DBG_BYTE_5			 	    		REG_MCUTST_II+3		//1c7
#define REG_DBG_BYTE_6				    		REG_HOST_MSG0	 	//88
#define REG_DBG_BYTE_7				    		REG_HOST_MSG0+1		//89
#define REG_DBG_BYTE_8				    		REG_HOST_MSG0+2		//8a
#define REG_DBG_BYTE_9				    		REG_HOST_MSG0+3		//8b
#define REG_DBG_BYTE_10				    	REG_HOST_MSG1		//8C
#define REG_DBG_BYTE_11				    	REG_HOST_MSG1+1		//8D
#define REG_DBG_BYTE_12				   		REG_HOST_MSG1+2		//8E
#define REG_DBG_BYTE_13				    	REG_HOST_MSG1+3		//8F
#define REG_DBG_BYTE_14				    	REG_FW_DBG6_V1		//68
#define REG_DBG_BYTE_15				    	REG_FW_DBG6_V1+1		//69
#define REG_DBG_BYTE_16				    	REG_FW_DBG6_V1+2		//6A
#define REG_DBG_BYTE_17				    	REG_FW_DBG6_V1+3		//6B
#define REG_DBG_BYTE_18				    	REG_FW_DBG7_V1		//6C
#define REG_DBG_BYTE_19				    	REG_FW_DBG7_V1+1		//6D
#define REG_DBG_BYTE_20				    	REG_FW_DBG7_V1+2		//6E
#define REG_DBG_BYTE_21				    	REG_FW_DBG7_V1+3		//6F

//3 4byte
#define REG_DBG_DWORD_0         		    	REG_SW_DEFINED_PAGE1	//1b8
#define REG_DBG_DWORD_1        			    REG_BLK_TST			//84
#define REG_DBG_DWORD_2         		    	REG_FW_DBG0_V1   		//50
#define REG_DBG_DWORD_3         		    	REG_FW_DBG1_V1		//54
#define REG_DBG_DWORD_4        	 		    REG_FW_DBG2_V1		//58
#define REG_DBG_DWORD_5         		    	REG_FW_DBG3_V1		//5A
#define REG_DBG_DWORD_6         		    	REG_FW_DBG4_V1		//60
#define REG_DBG_DWORD_7         		    	REG_MACID_SLEEP3		//484

//3 2byte
#define REG_DBG_WORD_0        			    	REG_MCUTST_II		//1c4
#define REG_DBG_WORD_1        					REG_SW_DEFINED_PAGE2	//1BC
#define REG_DBG_WORD_2        			    	REG_SW_DEFINED_PAGE2+2	//1BE
#define REG_DBG_WORD_3        			    	REG_FW_DBG5_V1		//64
#define REG_DBG_WORD_4        			    	REG_FW_DBG5_V1+2		//66
#define REG_DBG_WORD_5         		    	REG_DBG_DWORD_1		//84
#define REG_DBG_WORD_6         		   	 	REG_DBG_DWORD_1+2		//86

//2 FWLBK
#define REG_FWLBK_DBG_BYTE16					REG_FWLBK_DBG0		//0x1100
#define REG_FWLBK_DBG_BYTE17			    	REG_FWLBK_DBG0+1		//0x1101
#define REG_FWLBK_DBG_BYTE18			    	REG_FWLBK_DBG0+2
#define REG_FWLBK_DBG_BYTE19			    	REG_FWLBK_DBG0+3
#define REG_FWLBK_DBG_BYTE20			    	REG_FWLBK_DBG3		//0x110c
#define REG_FWLBK_DBG_BYTE21			    	REG_FWLBK_DBG3+1
#define REG_FWLBK_DBG_BYTE22			    	REG_FWLBK_DBG3+2
#define REG_FWLBK_DBG_BYTE23			    	REG_FWLBK_DBG3+3

#define REG_FWLBK_DBG_WORD0			    	REG_FWLBK_DBG1		//0x1104
#define REG_FWLBK_DBG_WORD1		            REG_FWLBK_DBG1+2
#define REG_FWLBK_DBG_WORD2		            REG_FWLBK_DBG2		//0x1108
#define REG_FWLBK_DBG_WORD3			    	REG_FWLBK_DBG2+2

//FWLBK
#define FWLBK_DBG_PAYLD_OFSET           	REG_FWLBK_DBG_WORD0//The n'th byte of payload which compare error
#define FWLBK_DBG_TXBUF_DATA           		REG_FWLBK_DBG_BYTE16//The value of TXPKTBUF compared error
#define FWLBK_DBG_RXBUF_DATA              	REG_FWLBK_DBG_BYTE17//The value of RXPKTBUF compared error
#define FWLBK_DBG_RX_LEN                    	REG_FWLBK_DBG_WORD1//length field of RxDESC
#define FWLBK_DBG_RXFF_RP                  	REG_FWLBK_DBG_WORD2//RXFF Read Pointer
#define FWLBK_DBG_PKTCNT                    	REG_FWLBK_DBG_WORD3//Tx packet count or Rx Receive packet count
//#define FWLBK_DBG_DATARATE                  	    REG_FWLBK_DBG_BYTE18
#define FWLBK_DBG_DATARATE                	REG_DBG_WORD_6
#define FWLBK_DBG_RX_OK_CNT              	REG_FWLBK_DBG_BYTE19

#define FWLBK_DBG_CAM_0                     	REG_FWLBK_DBG_BYTE20
#define FWLBK_DBG_CAM_1                     	REG_FWLBK_DBG_BYTE21
#define FWLBK_DBG_POLL_OK_CNT              	REG_FWLBK_DBG_BYTE22
#define FWLBK_DBG_CTL                       	REG_FWLBK_DBG_BYTE23

//1  Indirect define

//Power saving debug
#define REG_PS_32K             			    	REG_DBG_DWORD_0		//1b8
#define REG_32K_MATCH_CNT			    		REG_DBG_BYTE_18		//6C
#define REG_AntSwitchOutSide    		    	REG_DBG_BYTE_12		//8E
#define REG_FWLBK_CTRL          		    	REG_DBG_WORD_5		//84
#define DBGFUN_PS_STATE_EN      				BIT4 //report to 0x88[15:0]

//3 Register Debug register for our purpose
// 1.) Firmware Component Error
#define REGDUMP_FW_ERR0         		    	REG_DBG_BYTE_0		//1c0
#define REGDUMP_FW_ERR1         		    	REG_DBG_BYTE_1		//1c1
#define REGDUMP_FW_ERR2         		    	REG_DBG_BYTE_2		//1c2

// 2.) Hardware Component Error
#define REGDUMP_HW_ERR0         		    	REG_DBG_BYTE_3		//1c3

// 3.) Program Counter
#define REGDUMP_CPUINST         		    	REG_DBG_DWORD_3 		//54

// 4.) Unlock MCU inifinite-loop
//  0xFF: unlock Overall MCU while(1)
#define REG_UNLOCK_MCU          		    	REG_DBG_BYTE_4		//1C6

// 5.) FWLBK
#define REG_FTR                 		    		REG_DBG_BYTE_10 		//8C
#define REG_FWLBK_CTRL_EXT              		REG_DBG_BYTE_9		//8b

// 6.) Efuse Part Number
//#define REG_PART_NUM            		REG_DBG_BYTE_9
// Bit[7]: part number report enable
// Bit[6:4]: ant num
// Bit[3:2]: app_type
// BIT[1:0]: interface_type
// 7.) H2CPTR Index
#define REGDUMP_H2CPTR_IDX      		    	REG_DBG_BYTE_8		//8A

// 8.) BT-COEX
#define WLCALIBRATION               			REG_GENTST
//#define REG_Null1_fail_counter	REG_DBG_BYTE_15
// 9.) BT-COEX , BT calibration indication bit for driver
#define REG_BT_Cal_Ind		    		    	REG_DBG_BYTE_11		//8D

// 10.) BT Patch code checksum report (2 Byte)

// 11.) Pass Security Info to driver
#define REG_SECURITY_INFO       		    	REG_DBG_DWORD_7		//484
#define REG_KEY_INDEX          	 		    	REG_DBG_BYTE_14		//68
#define REG_WOW_PATTERN_INDEX   	   	    REG_DBG_BYTE_15		//69

// 12.) update PS State to Driver
#define REG_Current_PS_STATE    		    	REG_DBG_BYTE_6		//88
#define REG_LastRpwm            		    	REG_DBG_BYTE_7		//89

// 13.) WoWlan wakeup reason
#define REG_WoW_REASON          		    	REG_DBG_BYTE_5   		//1C7

// 14.) SAPPS debug
#define REG_SAPPS_DEBUG_DWORD1 			    REG_DBG_DWORD_2		//50
#define REG_SAPPS_DEBUG_DWORD2 			    REG_DBG_DWORD_4		//58
#define REG_SAPPS_DEBUG_DWORD3 			    REG_DBG_DWORD_5		//5A
#define REG_SAPPS_DEBUG_DWORD4 			    REG_DBG_DWORD_6		//60

//REG_UNLOCK_MCU0 SubComponent
#define UNLOCK_MCU_TSF_STABLE   			BIT0
#define UNLOCK_MCU_CPU_MGQ      				BIT1
#define UNLOCK_MCU_RW_BB        				BIT2

#define DBG_ADDR(_addr)         				WriteMACRegDWord(REGDUMP_CPUINST, _addr);
#define ManualDbg(stop)            		 	while(ReadMACRegByte(0x01C3) <= stop) {}

//definition for 0x005D    RE G_AOAC_REASON
#define AOAC_ARP                            		BIT0
#define AOAC_ISSUE_ARP                      	BIT1
#define AOAC_IP_MISMATCH                    	BIT2
#define AOAC_ON_NDP             					BIT3
#define AOAC_ISSUE_NDP          				BIT4
#define AOAC_IPV6_MISMATCH     				BIT5

/*------------------------------Define Struct---------------------------------*/
#ifndef __ASSEMBLY__
//REGDUMP_FW_ERR0
typedef enum _FW_ERR0_STATUS_ {
	FES0_H2C_CMDID							= BIT0,
	FES0_H2C_PTR                				= BIT1,
	FES0_BB_RW                  				= BIT2,
	FES0_TXPKT_TXPAUSE          				= BIT3,
	FES0_TSF_STABLE             				= BIT4,
	FES0_TXSM_STABLE            				= BIT5,
	FES0_RPWM_STABLE            				= BIT6,
	FES0_C2H_TIMEOUT_ERR        			= BIT7,

} FW_ERR0_STATUS, *PFW_ERR0_STATUS;

//REGDUMP_FW_ERR1
typedef enum _FW_ERR1_STATUS_ {
	FES1_H2C_QUEUE_FULL         			= BIT0,
	FES1_C2H_QUEUE_FULL         			= BIT1,
	FES1_RXDMA_NOT_IDLE_ERR     			= BIT2,
	FES1_WOWLAN_PKT_PTR         			= BIT3,
	FES1_FD_LOCK_ERR            				= BIT4,
	FES1_TRY_EN_ERR             				= BIT5,
	FES1_WOWLAN_RX_ERR          				= BIT6,
	FES1_C2HEXT_QUEUE_FULL      			= BIT7
} FW_ERR1_STATUS, *PFW_ERR1_STATUS;

//REGDUMP_FW_ERR2
typedef enum _FW_ERR2_STATUS_ {
	FES2_DBGPKT_QUEUE_FULL      			= BIT0,
	FES2_PATCH_VAR_OVERFLOW     			= BIT1,
	FES2_PATCH_VAR_UNDERFLOW    			= BIT2,
	FES2_DDMA_BUSY_TIMEOUT      			= BIT3
} FW_ERR2_STATUS, *PFW_ERR2_STATUS;

//REGDUMP_HW_ERR0
typedef enum _HW_ERR0_STATUS_ {
	HES0_USB_MDIO_INTF_ERR      			= BIT0,
	HES0_PCIE_MDIO_INTF_ERR     			= BIT1,
	HES0_USB_PHY_CMD_ERR        			= BIT2,
	HES0_PCIE_PHY_CMD_ERR       			= BIT3

} HW_ERR0_STATUS, *PHW_ERR0_STATUS;

typedef enum _FW_ERR_POWER_STATUS_ {
	BIT_POWER_STATUS_CHANGE      			= BIT0,
	BIT_S2_Condition_Not_Macth         	= BIT1,
	BIT_S4_Condition_Not_Macth			= BIT2,
	BIT_32K_Condition_Not_Macth         	= BIT3
} FW_ERR_POWER_STATUS, *PFW_ERR_POWER_STATUS;

typedef enum _FW_ERR_POWER_REASON1_ {
	BIT_H2CHDL0x20         					= BIT0,
	BIT_S0ToS1_ERROR         					= BIT1,
	BIT_TxNull0_FAIL     						= BIT2,
	BIT_S2ToS3_ERROR         					= BIT3,
	BIT_TxNull1_FAIL            				= BIT4
} FW_ERR_POWER_REASON1, *PFW_ERR_POWER_REASON1;

typedef enum _FW_ERR_POWER_REASON2_ {
	BIT_S2_TXQUEUE_NOT_EMPTY  			= BIT0,
	BIT_TRX_PERIOD        	 				= BIT1,
	BIT_S2_RPWM_NOT_MATCH     				= BIT2,
	BIT_RLPSRFOn       						= BIT3,
	BIT_WIFI_ACTIVE        					= BIT4,
	BIT_SAPPS_ACTIVE             				= BIT5
} FW_ERR_POWER_REASON2, *PFW_ERR_POWER_REASON2;

typedef enum _FW_ERR_POWER_REASON3_ {
	BIT_S4_TXQUEUE_NOT_EMPTY       		= 1,
	BIT_TRX_TIM_DTIM          				= 2,
	BIT_S4_RPWM_NOT_MATCH     				= 3,
	BIT_USP_NOT_OVER         					= 4,
	BIT_TX_ACTIVE_INDEX 					= 5,
	BIT_RX_ACTIVE_INDEX 					= 6,
	BIT_BCN_DATA_OPEN            			= 7,
	BIT_RfOffLicenseForBCNRx    			= 8,
	BIT_WLANON_PERIOD          				= 9,
	BIT_SCAN_EN      							= 10,
	BIT_S4_TXCPUMGQ_NOT_EMPTY				= 11,
	BIT_LEPAP_NOT_END						= 12,
	BIT_TWTSP_NOT_END						= 13
} FW_ERR_POWER_REASON3, *PFW_ERR_POWER_REASON3;

typedef enum _FW_ERR_POWER_REASON4_ {
	BIT_RXFF_NOT_EMPTY         				= 1,
	BIT_MAC_SCHDULE_SET        				= 2,
	BIT_C2HPKT_REQ     						= 3,
	BIT_RXDMA_NOT_IDLE         				= 4,
	BIT_RXPKT_REQ            					= 5,
	BIT_WOW_WAKEUP 							= 6,
	BIT_BT_CALIBRATION 						= 7,
	BIT_CPUMQ_EMPTY 							= 8,
	BIT_IOT_DELAY_WAKEUP 					= 9,
	BIT_RCVING_TIMHIT_BCN 					= 10,
	BIT_BB_FUNC_NOT_RST						= 11,
	BIT_BCN_DCK_NOT_RDY 					= 12

} FW_ERR_POWER_REASON4, *PFW_ERR_POWER_REASON4;

/*------------------------------Funciton declaration--------------------------*/

#endif  /* #ifndef   __ASSEMBLY__ */
#endif /* __WIFIFW_DEBUG_H__ */
