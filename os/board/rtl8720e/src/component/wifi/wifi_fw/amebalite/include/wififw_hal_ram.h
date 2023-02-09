#ifndef __WIFIFW_HAL_RAM_H__
#define __WIFIFW_HAL_RAM_H__

#ifndef __ASSEMBLY__
/*--------------------------Define -------------------------------------------*/
//2 TaskBitMap0
#define RATEADAPTIVE        						BIT0
#define H2CEVENT            							BIT1
#define C2HEVENT            							BIT2
#define RLPSEVENT           							BIT3
#define REMOTEWAKEEVENT     						BIT4
#define APOFFLOADEVENT      						BIT5
#define SWTIMEREVENT        						BIT7
#define BTMAILBOXEVENT      						BIT8

//2 Common Interface for each HAL
//OS
#define OSSendSignalCommon      			OSSendSignalCommon_8720E
#define SendSignalCommon					SendSignalCommon_8720E
#define ISRSendSignalCommon     			ISRSendSignalCommon_8720E
/* Func Init */
#define HALInit								HALInit_8720E
#define InitInterrupt           			InitInterrupt_8720E
#define InitCommon              				InitCommon_8720E
//GTimer
#define InitGTimer32us              		InitGTimer32us_8720E
#define InitGTimer1ms               		InitGTimer1ms_8720E

/* Interrupt handle function */
#define WLAN_INT_HDL        				WLAN_INT_HDL_8720E
#define FT_INT_HDL          					FT_INT_HDL_8720E
#define InitWlanIMR             				InitWlanIMR_8720E
#define InitFTIMR               				InitFTIMR_8720E
#define InitMAC2LPIntByps					InitMAC2LPIntByps_8720E
/* 2 1.) WLAN ISR */
#define ISR_TXCCX               				ISR_TXCCX_8720E
#define ISR_RxDone              				ISR_RxDone_8720E
#define ISR_H2CCMD              				ISR_H2CCMD_8720E
#define ISR_BcnEarly            			ISR_BcnEarly_8720E
#define ISR_TBTT                				ISR_TBTT_8720E
#define ISR_P0_TBTT_AGG					ISR_P0_TBTT_AGG_8720E
#define ISR_TIM_NO_HIT						ISR_TIM_NO_HIT_8720E
#define ISR_TIM_HIT							ISR_TIM_HIT_8720E
#define ISR_HIOE_CHN0						ISR_HIOE_CHN0_8720E
#define ISR_HIOE_CHN1						ISR_HIOE_CHN1_8720E
#define ISR_HIOE_CHN_ON					ISR_HIOE_CHN_ON_8720E
#define ISR_HIOE_CHN_OFF					ISR_HIOE_CHN_OFF_8720E
#define ISR_BCNRx               				ISR_BCNRx_8720E
#define ISR_BCNRxErr						ISR_BCNRxErr_8720E
#define ISR_RxBMC_MD1           			ISR_RxBMC_MD1_8720E
#define ISR_RxBMC_MD0           			ISR_RxBMC_MD0_8720E
#define ISR_RxUnicast_MD1       			ISR_RxUnicast_MD1_8720E
#define ISR_RxUnicast_MD0       			ISR_RxUnicast_MD0_8720E
#define ISR_TXBCNERR            			ISR_TXBCNERR_8720E
#define ISR_TXBCNOK             				ISR_TXBCNOK_8720E
#define ISR_TSF_GT_THRES					ISR_TSF_GT_THRES_8720E
#define ISR_RXBCN_TIMEUP					ISR_RXBCN_TIMEUP_8720E
#define ISR_RXBCN_TIMEOUT_CNT			ISR_RXBCN_TIMEOUT_CNT_8720E
#define ISR_TxPKTIn             				ISR_TxPKTIn_8720E
#define ISR_WlanActOn           			ISR_WlanActOn_8720E
#define ISR_WlanActOff          			ISR_WlanActOff_8720E
#define ISR_TxrptCntFull        			ISR_TxrptCntFull_8720E
/* 2 1.) WLAN2 ISR */
#define ISR_TBTT1               				ISR_TBTT1_8720E
/* 2 3.) WLAN4 ISR */
#define ISR_BTScoreboard        			ISR_BTScoreboard_8720E
#define ISR_ZBMailbox           			ISR_ZBMailbox_8720E
#define ISR_GNT_BT_Rise        	 		ISR_GNT_BT_Rise_8720E
#define ISR_GNT_BT_Fall         			ISR_GNT_BT_Fall_8720E
/* 2 4.) FT ISR */
#define ISR_GTimer0           				ISR_GTimer0_8720E
#define ISR_GTimer1             				ISR_GTimer1_8720E
#define ISR_GTimer2             				ISR_GTimer2_8720E
#define ISR_GTimer3             				ISR_GTimer3_8720E
#define ISR_GTimer4             				ISR_GTimer4_8720E
#define ISR_GTimer5             				ISR_GTimer5_8720E
#define ISR_GTimer6             				ISR_GTimer6_8720E
#if CONFIG_LPS_I_TWT
#define ISR_PSTIMERA             			ISR_PSTIMERA_8720E
#define ISR_PSTIMERAEarly					ISR_PSTIMERAEarly_8720E
#define ISR_PSTIMERB             			ISR_PSTIMERB_8720E
#define ISR_PSTIMERBEarly					ISR_PSTIMERBEarly_8720E
#define ISR_PSTIMERC             			ISR_PSTIMERC_8720E
#define ISR_PSTIMERCEarly					ISR_PSTIMERCEarly_8720E
#define ISR_PSTIMERD             			ISR_PSTIMERD_8720E
#define ISR_PSTIMERDEarly					ISR_PSTIMERDEarly_8720E
#define ISR_PSTIMERE             			ISR_PSTIMERE_8720E
#define ISR_PSTIMEOUT0						ISR_PSTIMEOUT0_8720E
#define ISR_PSTIMEOUT1						ISR_PSTIMEOUT1_8720E
#define ISR_PSTIMEOUT2						ISR_PSTIMEOUT2_8720E
#define CHECKTWTClientNULL				CHECKTWTClientNULL_8720E
#define CHECKTWTClientSPEND				CHECKTWTClientSPEND_8720E
#define PSTWTEarlyProcess					PSTWTEarlyProcess_8720E
#define PSTWTBcnToProcess					PSTWTBcnToProcess_8720E
#define PSTWTBcnRxProcess					PSTWTBcnRxProcess_8720E
#define LPS_ITWTTimeUpdate				LPS_ITWTTimeUpdate_8720E
#define LPS_ITWTDurationUpdate			LPS_ITWTDurationUpdate_8720E
#define LPS_ITWTTIMERSetting				LPS_ITWTTIMERSetting_8720E
#define LPS_ITWTStatusSet					LPS_ITWTStatusSet_8720E
#define LPS_ITWTIMRSetting				LPS_ITWTIMRSetting_8720E
#define LPS_ITWTByPassLsysWakeInt		LPS_ITWTByPassLsysWakeInt_8720E
#define LPS_ITWTSPWORK						LPS_ITWTSPWORK_8720E
#define LPS_ITWT_PortSet 					LPS_ITWT_PortSet_8720E
#define LPS_ITWT_ClientSPSet				LPS_ITWT_ClientSPSet_8720E
#define LPS_ITWT_ClientSPEarlySet		LPS_ITWT_ClientSPEarlySet_8720E
#endif
#define ISR_RPWM                				ISR_RPWM_8720E
#define ISR_BCNERLY1            			ISR_BCNERLY1_8720E
#define ISR_BcnEarlyAGG         			ISR_BcnEarlyAGG_8720E
#define ISR_TxNull0             				ISR_TxNull0_8720E
#define ISR_TxNull1             				ISR_TxNull1_8720E
//Basic ISR
#define UpdateISRWlan           			UpdateISRWlan_8720E
#define UpdateISRFT             				UpdateISRFT_8720E
#define ClrAllFWUsedIMR         			ClrAllFWUsedIMR_8720E
#define ClrAllFWUsedISR         			ClrAllFWUsedISR_8720E
#define setfwimr 								setfwimr_8720E
#define clrfwimr 								clrfwimr_8720E
#define setwlanimr 							setwlanimr_8720E
#define clrwlanimr 							clrwlanimr_8720E
#define setftimr					  			setftimr_8720E
#define clrftimr                     			clrftimr_8720E

/* 3 4.) RAM func */
#define MacidDropAction         			MacidDropAction_8720E
#define MediaConnection         			MediaConnection_8720E
#define GetRPWMValue                    	GetRPWMValue_8720E
#define WaitHWStateReady                	WaitHWStateReady_8720E
#define SysClkDown                      		SysClkDown_8720E
#define SysClkUp                          	SysClkUp_8720E
#define SetTxPauseFWInfo            		SetTxPauseFWInfo_8720E
#define C2HSpeRPT								C2HSpeRPT_8720E
#define SetTxPauseDRVInfo					SetTxPauseDRVInfo_8720E
//CMD
#define H2CCmdFunc              				H2CCmdFunc_8720E
#define H2CInQueue              				H2CInQueue_8720E
#define H2CDeQueue              				H2CDeQueue_8720E
#define InitC2H                     			InitC2H_8720E
#define WaitC2HOk               				WaitC2HOk_8720E
#define C2HPKTRxDESC                		C2HPKTRxDESC_8720E
#define FillC2HContant                  	FillC2HContant_8720E
#define IssueC2HPKT                     		IssueC2HPKT_8720E
#define C2HDeQueue             	 			C2HDeQueue_8720E
#define C2HInQueue              				C2HInQueue_8720E
#define CheckMaxMacidNum        			CheckMaxMacidNum_8720E
#define SetMediaStatus          			SetMediaStatus_8720E
#define GetMediaStatus          			GetMediaStatus_8720E
#define MediaDisconnection				MediaDisconnection_8720E
#define MediaConnection					MediaConnection_8720E

// hioe
#define HIOESetSingleCmdValWM 			HIOESetSingleCmdValWM_8720E
#define HIOESetSingleCmdValBM 			HIOESetSingleCmdValBM_8720E
#define HIOECmdInitValTypeChk			HIOECmdInitValTypeChk_8720E
#define HIOEChkChanelIdle					HIOEChkChanelIdle_8720E
#define HIOEInitCmdFileToTXFF 			HIOEInitCmdFileToTXFF_8720E

#define HIOESetConfigFile 				HIOESetConfigFile_8720E
#define HIOEBackupRestore 				HIOEBackupRestore_8720E
#define HIOECHKHigh8kTXFFPGUseOverlap 		HIOECHKHigh8kTXFFPGUseOverlap_8720E
//ddma
#define InitDDMA					  			InitDDMA_8720E
#define WaitDDMARDY                			WaitDDMARDY_8720E
#define ChkDDMAChksum               		ChkDDMAChksum_8720E
#define CheckDDMADone               		CheckDDMADone_8720E

//lps
#define PwrGatedInitLLTAndPageAlloc PwrGatedInitLLTAndPageAlloc_8720E
#define Change_PS_State             		Change_PS_State_8720E
#define PS_S2_Condition_Match   		PS_S2_Condition_Match_8720E
#define PS_S4_Condition_Match   		PS_S4_Condition_Match_8720E
#define WriteTxPauseWithMask				WriteTxPauseWithMask_8720E
#define WaitTxStateMachineOk       		WaitTxStateMachineOk_8720E
#define WriteTxPause				     	WriteTxPause_8720E
#define PsOpenRF                         		PsOpenRF_8720E
#define PsCloseRF                        		PsCloseRF_8720E
#define IssueNullData                     	IssueNullData_8720E

#define PS_32K_Condition_Match  		PS_32K_Condition_Match_8720E
#define InitPS  								InitPS_8720E
#define ChangePSStateByRPWM  			ChangePSStateByRPWM_8720E
#define PSBcnEarlyProcess 				PSBcnEarlyProcess_8720E
#define PSBcnAggEarlyProcess 			PSBcnAggEarlyProcess_8720E
#define PsDtimToProcess				PsDtimToProcess_8720E
#define PsBcnToProcess 					PsBcnToProcess_8720E
#define TxPktInPSOn 							TxPktInPSOn_8720E
#define PSSleepTo32K 						PSSleepTo32K_8720E
#define PSRxBcnProcess 					PSRxBcnProcess_8720E
#define LowPowerRxBeacon 					LowPowerRxBeacon_8720E
#define PSPeriodCCA							PSPeriodCCA_8720E
#define BeaconModeBBAGCtableUpdate 	BeaconModeBBAGCtableUpdate_8720E
#define BeaconModeDCKbackup 				BeaconModeDCKbackup_8720E
#define PwrGatedBKRSTxReg					PwrGatedBKRSTxReg_8720E
#define PwrGatedBKRSCtrlInfo				PwrGatedBKRSCtrlInfo_8720E
#define PwrGatedBKRSMACTXPower			PwrGatedBKRSMACTXPower_8720E
#define PwrGatedInitDRFC 					PwrGatedInitDRFC_8720E
#define PwrGatedBKRSBBTSSIREG			PwrGatedBKRSBBTSSIREG_8720E
#define PwrGatedInitBBTRX					PwrGatedInitBBTRX_8720E
#define PwrGatedWifiParmBkRs				PwrGatedWifiParmBkRs_8720E
#define Set32KLpsOption 					Set32KLpsOption_8720E
#define InitBcnEarlyAdjustStat 			InitBcnEarlyAdjustStat_8720E
#define InitBcnEarlyAdjustPID 			InitBcnEarlyAdjustPID_8720E
#if CONFIG_BCNADJ_LPSOFFLOAD_V2
#define BcnEarlyADJPIDSearchBaseValue BcnEarlyADJPIDSearchBaseValue_8720E
#define BcnEarlyADJPIDCheckBaseValue	BcnEarlyADJPIDCheckBaseValue_8720E
#endif
#if CONFIG_BCNADJ_LPSOFFLOAD_V1
#define BcnEarlyADJSTSSearchBaseValue BcnEarlyADJSTSSearchBaseValue_8720E
#define BcnEarlyADJSTSCheckBaseValue	BcnEarlyADJSTSCheckBaseValue_8720E
#endif
#define SetSmartPSTimer					SetSmartPSTimer_8720E
#define SetPwrStateReg						SetPwrStateReg_8720E
#define ResetPSParm							ResetPSParm_8720E
#define ChkTxQueueIsEmpty					ChkTxQueueIsEmpty_8720E
#define Legacy_PS_Setting					Legacy_PS_Setting_8720E
#define PSModeSetting						PSModeSetting_8720E
#define ConfigListenBeaconPeriod		ConfigListenBeaconPeriod_8720E
#define PSSetMode								PSSetMode_8720E
#define PS_S2ToS3State						PS_S2ToS3State_8720E
#define PS_S2ToS0State						PS_S2ToS0State_8720E
#define PS_S3ToS2orS0State				PS_S3ToS2orS0State_8720E
#define PS_S0ToS1State						PS_S0ToS1State_8720E
#define PS_S1ToS0orS2State				PS_S1ToS0orS2State_8720E
#define PS_S2ToS4State						PS_S2ToS4State_8720E
#define PS_S2ToS5State		        		PS_S2ToS5State_8720E
#define PS_S5ToS2State		        		PS_S5ToS2State_8720E
#define PS_S4ToS2State						PS_S4ToS2State_8720E
#define SmartPS2InitTimerAndToGetRxPkt	SmartPS2InitTimerAndToGetRxPkt_8720E
#define SetBcnEarlyAndTimeout			SetBcnEarlyAndTimeout_8720E
#define EnlargeBcnEarlyAndTimeout		EnlargeBcnEarlyAndTimeout_8720E
#define PS_S0ToS6State						PS_S0ToS6State_8720E
#define PS_S6ToS0State						PS_S6ToS0State_8720E
//LPS offload
#define LPSOfld_HIOECmdFileInit 		LPSOfld_HIOECmdFileInit_8720E
#define LPSOfld_RxbcnModeCmdFileValInit	LPSOfld_RxbcnModeCmdFileValInit_8720E
#define LPSOfld_ExitRxbcnModeCmdFileValInit LPSOfld_ExitRxbcnModeCmdFileValInit_8720E
#define LPSOfld_SwtHIOECmdFile 			LPSOfld_SwtHIOECmdFile_8720E
#define LPSOfld_IMRSetting 				LPSOfld_IMRSetting_8720E
#define LPSOfld_SetBcnErlyandTimeout	LPSOfld_SetBcnErlyandTimeout_8720E
#define LPSOfld_FuncInit 					LPSOfld_FuncInit_8720E
#define LPSOfld_PGResume 			LPSOfld_PGResume_8720E
#define LPSOfld_HWCtrlTRXOnOff 			LPSOfld_HWCtrlTRXOnOff_8720E
#define LPSOfld_PSRxBcnProcess			LPSOfld_PSRxBcnProcess_8720E

//Sapps
#define InitSAPPS 							InitSAPPS_8720E
#define SAPPSTBTTHDL 						SAPPSTBTTHDL_8720E
#define SAPPSTimeOutHDL 					SAPPSTimeOutHDL_8720E
#define SAPPSEarlyHDL 						SAPPSEarlyHDL_8720E
#define SAPLPS 								SAPLPS_8720E
//IPS
#define InitInactivePS  					InitInactivePS_8720E
#define InactivePSTBTTHDL 				InactivePSTBTTHDL_8720E
#define InactivePSBCNAggEarlyHDL 		InactivePSBCNAggEarlyHDL_8720E
#define InactivePSEnableHEL 				InactivePSEnableHEL_8720E
//RA
#define InitCtrlInfo 						InitCtrlInfo_8720E
#if CONFIG_CSI_REPORT
#define HALBB_FW_BBReset					HALBB_FW_BBReset_8720E
#define CheckCSITxNullDone				CheckCSITxNullDone_8720E
#define CheckNoCSIClientEnable			CheckNoCSIClientEnable_8720E
#define CheckNextCSIClientTxnull		CheckNextCSIClientTxnull_8720E
#define CheckCSIClientTxnullRty			CheckCSIClientTxnullRty_8720E
#define IssueCSITXNull						IssueCSITXNull_8720E
#define SetPeriodTxNullAddrRate			SetPeriodTxNullAddrRate_8720E
#define H2CHDL_Set_CSI_TXNULL 			H2CHDL_Set_CSI_TXNULL_8720E
#define CSIEnableClientParameter		CSIEnableClientParameter_8720E
#define CSIDisableClientParameter		CSIDisableClientParameter_8720E
#endif
#define halbb_fw_read_rf_reg				halbb_fw_read_rf_reg_8720e
#define halbb_fw_write_rf_reg			halbb_fw_write_rf_reg_8720e
#define halbb_fw_set_bb_reg 				halbb_fw_set_bb_reg_8720e
#define halbb_fw_get_bb_reg				halbb_fw_get_bb_reg_8720e

//Wolwan
#define InitRemoteWakeUp 					InitRemoteWakeUp_8720E
#define RemoteWakeUp 						RemoteWakeUp_8720E
#define ClearRxBuff 							ClearRxBuff_8720E
#define H2CHDL_RemoteWakeUp 				H2CHDL_RemoteWakeUp_8720E
#define KeepAliveChk 						KeepAliveChk_8720E
#define TryPKT 								TryPKT_8720E
#define GetCheckSum 							GetCheckSum_8720E
#define GetSumValue 							GetSumValue_8720E
#define UpdateCheckSumVer2 				UpdateCheckSumVer2_8720E
#define FillIpv6PseudoHeader 			FillIpv6PseudoHeader_8720E
#define InitKeepAlive						InitKeepAlive_8720E
#define H2CHDL_KeepAlive					H2CHDL_KeepAlive_8720E
#define UpdateRxFFReadPtr_DropPkt		UpdateRxFFReadPtr_DropPkt_8720E
#define WaitRXDMAIdle						WaitRXDMAIdle_8720E
#define CHECKRXPKTNULL						CHECKRXPKTNULL_8720E
#define UpdateChInfoReadPtr_DropPkt	UpdateChInfoReadPtr_DropPkt_8720E
#define UpdatePhyStsReadPtr_DropPkt	UpdatePhyStsReadPtr_DropPkt_8720E
#define RxPHYSTSRelease 					RxPHYSTSRelease_8720E
#define RxCHInfoRelease 					RxCHInfoRelease_8720E
#define PageOffsetAlignment				PageOffsetAlignment_8720E
#define DropCHK								DropCHK_8720E
#define DIGByRSSILps  					DIGByRSSILps_8720E
#define EnableWoWLAN 						EnableWoWLAN_8720E
#define DisWoWLAN 							DisWoWLAN_8720E
#define H2CHDL_WoWLAN 						H2CHDL_WoWLAN_8720E
#define DropDecision 						DropDecision_8720E
#define FwDisConnectWakeUpHost			FwDisConnectWakeUpHost_8720E
#define DestinationMatch					DestinationMatch_8720E
#define PassSecurityInfoToDriver		PassSecurityInfoToDriver_8720E
#define GetRemoteControlInfo 			GetRemoteControlInfo_8720E
#define GetARPInfo 							GetARPInfo_8720E
#define OnARP									OnARP_8720E
#define GetNDPInfo 							GetNDPInfo_8720E
#define IsIPV6 								IsIPV6_8720E
#define OnNS									OnNS_8720E
#define Ipv4Match 							Ipv4Match_8720E
#define MatchUnicastFilter 				MatchUnicastFilter_8720E
#define WakeUpHostDecision				WakeUpHostDecision_8720E
#define CheckIV 								CheckIV_8720E
#define H2CHDL_AOACGlobalInfo			H2CHDL_AOACGlobalInfo_8720E
#define H2CHDL_AOACRsvdpage1				H2CHDL_AOACRsvdpage1_8720E
#define H2CHDL_AOACRsvdpage2 			H2CHDL_AOACRsvdpage2_8720E
#define H2CHDL_AOACRsvdpage3 			H2CHDL_AOACRsvdpage3_8720E
#define GetAddr 								GetAddr_8720E
#define ARP_hdl 								ARP_hdl_8720E
#define NDP_hdl 								NDP_hdl_8720E
#define InitDisconnectDecision 			InitDisconnectDecision_8720E
#define DisconnectChk						DisconnectChk_8720E
#define DisconnectTxNullChk 				DisconnectTxNullChk_8720E
#define H2CHDL_DisconnectDecision 		H2CHDL_DisconnectDecision_8720E
#define WakeUpHost              				WakeUpHost_8720E
#define issue_ARP								issue_ARP_8720E
#define IssueNA								IssueNA_8720E
#define GetIV                					GetIV_8720E
#define GetIVLenAndSecurityType 		GetIVLenAndSecurityType_8720E
#define GetMACHeaderLen         			GetMACHeaderLen_8720E
#define UpdateIV								UpdateIV_8720E
//GTK
#define AesTkipIvFun            			AesTkipIvFun_8720E
#define WepIvFun                				WepIvFun_8720E
#define RTmemcmpBackward       			RTmemcmpBackward_8720E
#define AssignIvToKeyRsc       			AssignIvToKeyRsc_8720E
#define AssignKeyRscToIV       			AssignKeyRscToIV_8720E
#define AesTkipIvCheck       				AesTkipIvCheck_8720E
#define WepIvCheck       					WepIvCheck_8720E

//Packet
#define ChangeTransmiteRate         		ChangeTransmiteRate_8720E
#define PowerBitSetting                 	PowerBitSetting_8720E
#define IssuePSPoll                       	IssuePSPoll_8720E
#define ChkandChangePS              		ChkandChangePS_8720E
#define IssueQNull                          	IssueQNull_8720E
#define CTS2SelfSetting 					CTS2SelfSetting_8720E
#define IssueCTS2Self                   	IssueCTS2Self_8720E
#define IssueRsvdPagePacketSetting 	IssueRsvdPagePacketSetting_8720E
#define ReadRxBuff                      		ReadRxBuff_8720E
#define WaitCPUMGQEmpty					WaitCPUMGQEmpty_8720E
#define PollingReg							PollingReg_8720E

//BTCoex
#define InitBTMailbox                  		InitBTMailbox_8720E
#define FillMailbox                    		FillMailbox_8720E
#define BT2WLMailboxINTHDL             	BT2WLMailboxINTHDL_8720E
#define BT2WLMailboxINTCheck           	BT2WLMailboxINTCheck_8720E
#define H2CHDL_BTPSTDMA                	H2CHDL_BTPSTDMA_8720E
#define BTC_PsTdmaActionDelay          	BTC_PsTdmaActionDelay_8720E
#define BTC_PsTdmaAction               	BTC_PsTdmaAction_8720E
#define H2CHDL_BTWifiCtrl              	H2CHDL_BTWifiCtrl_8720E
#define H2CHDL_WLCalibration           	H2CHDL_WLCalibration_8720E
#define H2CHDL_BT_Init_Param           	H2CHDL_BT_Init_Param_8720E
#define C2HWlanInfo                    		C2HWlanInfo_8720E
#define InitBTCoex                     		InitBTCoex_8720E
#define BT_TDMA_Slot_Operation_0and3	BT_TDMA_Slot_Operation_0and3_8720E
#define BT_TDMA_Slot_Operation_1and4	BT_TDMA_Slot_Operation_1and4_8720E
#define BT_TDMA_Slot_Operation_2and5	BT_TDMA_Slot_Operation_2and5_8720E
#define DynamicSlotDecision            	DynamicSlotDecision_8720E
#define BTypeTDMAMultiSlots            	BTypeTDMAMultiSlots_8720E
#define BTypeTDMABCNEarly              	BTypeTDMABCNEarly_8720E
#define BTypeTDMATBTThdl               	BTypeTDMATBTThdl_8720E
#define BTypeTDMALeakAPOperation		BTypeTDMALeakAPOperation_8720E
#define BTypeTDMATimeOuthdl				BTypeTDMATimeOuthdl_8720E
#define BTC_RestoreDefaultCoexTable	BTC_RestoreDefaultCoexTable_8720E
#define BTC_PWRBitOverWrite				BTC_PWRBitOverWrite_8720E
#define BTC_PsTdmaIssueNull            	BTC_PsTdmaIssueNull_8720E
#define BTC_PsTdmaOnBcn					BTC_PsTdmaOnBcn_8720E
#define BTC_BtIqkAction                	BTC_BtIqkAction_8720E
#define BTC_BtIqkCheck                 	BTC_BtIqkCheck_8720E
#define BTC_WlanInfoCounterReset		BTC_WlanInfoCounterReset_8720E
#define BTC_ProceedBtSlot              	BTC_ProceedBtSlot_8720E
#define BTC_ProtectBCN                 	BTC_ProtectBCN_8720E
#define BTC_RestoreBtSlot              	BTC_RestoreBtSlot_8720E
#define BTC_GenRandomValue             	BTC_GenRandomValue_8720E
#define BTC_WifiWindowSlot             	BTC_WifiWindowSlot_8720E
#define BTC_Set_GntBt             		BTC_Set_GntBt_8720E
#if CONFIG_BTCOEX_SLOT_DEBUG
#define InitBTCoexDebugPort            	InitBTCoexDebugPort_8720E
#define BTC_SlotGpioDbgCtrl            	BTC_SlotGpioDbgCtrl_8720E
#endif
#define BTC_AOACSwitch                 	BTC_AOACSwitch_8720E
//BTCoex
#define H2CHDL_BTInfo						H2CHDL_BTInfo_8720E
#define H2CHDL_ForceBTTxpwr				H2CHDL_ForceBTTxpwr_8720E
#define H2CHDL_BTIgnoreWlanAct			H2CHDL_BTIgnoreWlanAct_8720E
#define H2CHDL_AntSelReverse				H2CHDL_AntSelReverse_8720E
#define H2CHDL_WLOpmode					H2CHDL_WLOpmode_8720E
#define H2CHDL_BTMpH2C						H2CHDL_BTMpH2C_8720E
#define H2CHDL_BTControl					H2CHDL_BTControl_8720E
#define H2CHDL_BT_Page_Scan_Interval	H2CHDL_BT_Page_Scan_Interval_8720E
#define H2CHDL_GNT_BT_Ctrl				H2CHDL_GNT_BT_Ctrl_8720E
#define H2CHDL_BT_Only_Test				H2CHDL_BT_Only_Test_8720E
#define H2CHDL_WL_Port_ID					H2CHDL_WL_Port_ID_8720E
#define C2HBTLoopback						C2HBTLoopback_8720E
#define C2HBTMpRpt							C2HBTMpRpt_8720E
#define C2HBTMailBoxStatus				C2HBTMailBoxStatus_8720E
#define InitBTCoexTimer					InitBTCoexTimer_8720E
#define Write778								Write778_8720E
#define TdmaChangeCoexTable				TdmaChangeCoexTable_8720E
#define BTNullSetting						BTNullSetting_8720E
#define IssueBTQNull						IssueBTQNull_8720E
#define C2HBTInfo								C2HBTInfo_8720E
#define WL2BTMailboxSend					WL2BTMailboxSend_8720E

//other
#define TSF_After(a,b)  					((s32)(b)-(s32)(a)<0)
#define CheckCPUMGQEmpty					CheckCPUMGQEmpty_8720E

//sw tx queue
#if CONFIG_SW_QUEUE_TX
#define InitFreeRunTSF						InitFreeRunTSF_8720E
#define GetFreeRunCnt						GetFreeRunCnt_8720E
#define GetFreeRunCntU32					GetFreeRunCntU32_8720E
#define TxPktPollMgntQ						TxPktPollMgntQ_8720E
#define CPUMGQCancelTx						CPUMGQCancelTx_8720E
#define SWQTXCheckBusyQState				SWQTXCheckBusyQState_8720E
#define IsSWQTXAllQEmpty					IsSWQTXAllQEmpty_8720E
#define PS_PreTxNull						PS_PreTxNull_8720E
#define PS_PostTxNull1						PS_PostTxNull1_8720E
#define PS_PostTxNull0						PS_PostTxNull0_8720E
#define PostTxNull							PostTxNull_8720E
#define CheckTxNullResult					CheckTxNullResult_8720E
#define SWQTXTxFeedBack					SWQTXTxFeedBack_8720E
#define SWQTXEnqueueWaitQ					SWQTXEnqueueWaitQ_8720E
#define ISR_CPUMGN_POLLED_PKT_DONE	ISR_CPUMGN_POLLED_PKT_DONE_8720E
#endif

//2 KM4/KR4/DDMA access base
//3 MAC base address
#define RTL8720E_IOREG_AHB_BASE					0x40000000
//3 BB base address
#define RTL8720E_BBREG_AHB_BASE             	0x40010000
//3 RF base address
#define RTL8720E_ARFCREG_AHB_BASE             	0x40017000
#define RTL8720E_DRFCREG_AHB_BASE             	0x4001E000
//3 DDMA control base address
#define RTL8720E_DDMA_CTRL_AHB_BASE         	0x40000000 //0x4000_1200- 0x4000_12FF
//3 BT mailbox base address
#define RTL8720E_BT_MAILBOX_AHB_BASE        	0x400011A4
//3 LLT Table base
#define RTL8720E_LLT_AHB_BASE					0x40050000//256 byte/entry
//3 TxReport Buffer base address
#define RTL8720E_RPTBUF_AHB_BASE            	0x40040000
#define RTL8720E_CtrlInfo_AHB_BASE          	(RTL8720E_RPTBUF_AHB_BASE)
#define RTL8720E_TRXRPT_AHB_BASE            	(RTL8720E_RPTBUF_AHB_BASE + 0x280)
#define RTL8720E_CRC5_AHB_BASE              	(RTL8720E_RPTBUF_AHB_BASE + 0xA80)
#define RTL8720E_NANRPT_AHB_BASE             	(RTL8720E_RPTBUF_AHB_BASE + 0xA90)
//3 Rx packet buffer base address
#define RTL8720E_RXFF_AHB_BASE              	0x40030000
//3 Tx packet buffer base address
#define RTL8720E_TXFF_AHB_BASE              	0x40020000
//3 CAM base address
#define RTL8720E_SECCAM_AHB_BASE            	0x40050800
#define RTL8720E_BACAM_AHB_BASE             	0x40051000
#define RTL8720E_MBIDCAM_AHB_BASE 				0x40051800
//3 PHYSTS/CHANINFO
#define RTL8720E_PHYSTS_AHB_BASE            	0x40054000
#define RTL8720E_CHINFO_AHB_BASE            	0x40052000
//2 sysreg access
#define SYSTEM_CTRL_BASE_LPSYS_OFFSET		0x200
//3 retention access
#define RETENTION_RAM_AHB_FW_BASE			  	0x000C0130

//offset:
#define rtl_inb(offset)             				(*(volatile u8 *)((u32)(offset)))
#define rtl_inw(offset)             				(*(volatile u16 *)((u32)(offset)))
#define rtl_inl(offset)             				(*(volatile u32 *)((u32)(offset)))

#define rtl_outb(offset,val)        				do { (*(volatile u8 *)((u32)(offset)) = (val));    } while(0)
#define rtl_outw(offset,val)        				do { (*(volatile u16 *)((u32)(offset)) = (val));    } while(0)
#define rtl_outl(offset,val)        				do { (*(volatile u32 *)((u32)(offset)) = (val));    } while(0)

//3 MACREG
//offset :
#define WritePONRegDWord(offset, value)		rtl_outl((offset)+RTL8720E_IOREG_AHB_BASE, value)
#define WritePONRegWord(offset, value)		rtl_outw((offset)+RTL8720E_IOREG_AHB_BASE, value)
#define WritePONRegByte(offset, value) 		rtl_outb((offset)+RTL8720E_IOREG_AHB_BASE, value)

#define ReadPONRegDWord(offset)             	(rtl_inl((offset)+RTL8720E_IOREG_AHB_BASE))
#define ReadPONRegWord(offset)              	(rtl_inw((offset)+RTL8720E_IOREG_AHB_BASE))
#define ReadPONRegByte(offset)              	(rtl_inb((offset)+RTL8720E_IOREG_AHB_BASE))

//offset :
#define WriteMACRegDWord(offset, value)		rtl_outl((offset)+RTL8720E_IOREG_AHB_BASE, value)
#define WriteMACRegWord(offset, value)      	rtl_outw((offset)+RTL8720E_IOREG_AHB_BASE, value)
#define WriteMACRegByte(offset, value)      	rtl_outb((offset)+RTL8720E_IOREG_AHB_BASE, value)

#define ReadMACRegDWord(offset)             	(rtl_inl((offset)+RTL8720E_IOREG_AHB_BASE))
#define ReadMACRegWord(offset)              	(rtl_inw((offset)+RTL8720E_IOREG_AHB_BASE))
#define ReadMACRegByte(offset)              	(rtl_inb((offset)+RTL8720E_IOREG_AHB_BASE))

//3 LLT TABLE
#define WriteLLTTableDWord(offset, value)   rtl_outl((offset)+RTL8720E_LLT_AHB_BASE, value)
#define WriteLLTTableWord(offset, value)   	rtl_outw((offset)+RTL8720E_LLT_AHB_BASE, value)
#define WriteLLTTableByte(offset, value)   	rtl_outb((offset)+RTL8720E_LLT_AHB_BASE, value)

#define ReadLLTTableDWord(offset)           	(rtl_inl((offset)+RTL8720E_LLT_AHB_BASE))
#define ReadLLTTableWord(offset)            	(rtl_inw((offset)+RTL8720E_LLT_AHB_BASE))
#define ReadLLTTableByte(offset)            	(rtl_inb((offset)+RTL8720E_LLT_AHB_BASE))

//3 BB Reg
#define WritePortBBUlong(offset, value)		rtl_outl((offset)+RTL8720E_BBREG_AHB_BASE, value)
#define WritePortBBUshort(offset, value)  	rtl_outw((offset)+RTL8720E_BBREG_AHB_BASE, value)
#define WritePortBBUchar(offset, value)     rtl_outb((offset)+RTL8720E_BBREG_AHB_BASE, value)

#define ReadPortBBUlong(offset)             	(rtl_inl((offset)+RTL8720E_BBREG_AHB_BASE))
#define ReadPortBBUshort(offset)            	(rtl_inw((offset)+RTL8720E_BBREG_AHB_BASE))
#define ReadPortBBUchar(offset)             	(rtl_inb((offset)+RTL8720E_BBREG_AHB_BASE))

//3 SEC CAM
#define WriteSECCAMDWord(offset, value)  	rtl_outl((offset)+RTL8720E_SECCAM_AHB_BASE, value)
#define WriteSECCAMWord(offset, value)   	rtl_outw((offset)+RTL8720E_SECCAM_AHB_BASE, value)
#define WriteSECCAMByte(offset, value)   	rtl_outb((offset)+RTL8720E_SECCAM_AHB_BASE, value)

#define ReadSECCAMDWord(offset)          		(rtl_inl((offset)+RTL8720E_SECCAM_AHB_BASE))
#define ReadSECCAMWord(offset)           		(rtl_inw((offset)+RTL8720E_SECCAM_AHB_BASE))
#define ReadSECCAMByte(offset)           		(rtl_inb((offset)+RTL8720E_SECCAM_AHB_BASE))

//3 MBID CAM
#define WriteMBIDCAMDWord(offset, value)  	rtl_outl((offset)+RTL8720E_MBIDCAM_AHB_BASE, value)
#define WriteMBIDCAMWord(offset, value)   	rtl_outw((offset)+RTL8720E_MBIDCAM_AHB_BASE, value)
#define WriteMBIDCAMByte(offset, value)   	rtl_outb((offset)+RTL8720E_MBIDCAM_AHB_BASE, value)

#define ReadMBIDCAMDWord(offset)          		(rtl_inl((offset)+RTL8720E_MBIDCAM_AHB_BASE))
#define ReadMBIDCAMWord(offset)           		(rtl_inw((offset)+RTL8720E_MBIDCAM_AHB_BASE))
#define ReadMBIDCAMByte(offset)           		(rtl_inb((offset)+RTL8720E_MBIDCAM_AHB_BASE))

//3 BA CAM
#define WriteBACAMDWord(offset, value)  		rtl_outl((offset)+RTL8720E_BACAM_AHB_BASE, value)
#define WriteBACAMWord(offset, value)   		rtl_outw((offset)+RTL8720E_BACAM_AHB_BASE, value)
#define WriteBACAMByte(offset, value)   		rtl_outb((offset)+RTL8720E_BACAM_AHB_BASE, value)

#define ReadBACAMDWord(offset)          		(rtl_inl((offset)+RTL8720E_BACAM_AHB_BASE))
#define ReadBACAMWord(offset)           		(rtl_inw((offset)+RTL8720E_BACAM_AHB_BASE))
#define ReadBACAMByte(offset)           		(rtl_inb((offset)+RTL8720E_BACAM_AHB_BASE))

//3 ReportBUFF
#define WriteRPTBUFDWord(offset, value)  	rtl_outl((offset)+RTL8720E_RPTBUF_AHB_BASE, value)
#define WriteRPTBUFWord(offset, value)   	rtl_outw((offset)+RTL8720E_RPTBUF_AHB_BASE, value)
#define WriteRPTBUFByte(offset, value)   	rtl_outb((offset)+RTL8720E_RPTBUF_AHB_BASE, value)

#define ReadRPTBUFDWord(offset)          		(rtl_inl((offset)+RTL8720E_RPTBUF_AHB_BASE))
#define ReadRPTBUFWord(offset)           		(rtl_inw((offset)+RTL8720E_RPTBUF_AHB_BASE))
#define ReadRPTBUFByte(offset)           		(rtl_inb((offset)+RTL8720E_RPTBUF_AHB_BASE))

//3 RXFF
#define WriteRXFFDWord(offset, value)   		rtl_outl((offset)+RTL8720E_RXFF_AHB_BASE, value)
#define WriteRXFFWord(offset, value)    		rtl_outw((offset)+RTL8720E_RXFF_AHB_BASE, value)
#define WriteRXFFByte(offset, value)    		rtl_outb((offset)+RTL8720E_RXFF_AHB_BASE, value)

#define ReadRXFFDWord(offset)           		(rtl_inl((offset)+RTL8720E_RXFF_AHB_BASE))
#define ReadRXFFWord(offset)            			(rtl_inw((offset)+RTL8720E_RXFF_AHB_BASE))
#define ReadRXFFByte(offset)            			(rtl_inb((offset)+RTL8720E_RXFF_AHB_BASE))

//3 TXFF
#define WriteTXFFDWord(offset, value)     	rtl_outl((offset)+RTL8720E_TXFF_AHB_BASE, value)
#define WriteTXFFWord(offset, value)      	rtl_outw((offset)+RTL8720E_TXFF_AHB_BASE, value)
#define WriteTXFFByte(offset, value)      	rtl_outb((offset)+RTL8720E_TXFF_AHB_BASE, value)

#define ReadTXFFDWord(offset)             		(rtl_inl((offset)+RTL8720E_TXFF_AHB_BASE))
#define ReadTXFFWord(offset)              		(rtl_inw((offset)+RTL8720E_TXFF_AHB_BASE))
#define ReadTXFFByte(offset)              		(rtl_inb((offset)+RTL8720E_TXFF_AHB_BASE))

//3 Retention RAM
#define WriteRRamDWord(offset, value)     	rtl_outl((offset)+RETENTION_RAM_AHB_FW_BASE, value)
#define WriteRRamWord(offset, value)      	rtl_outw((offset)+RETENTION_RAM_AHB_FW_BASE, value)
#define WriteRRamByte(offset, value)     	 	rtl_outb((offset)+RETENTION_RAM_AHB_FW_BASE, value)

#define ReadRRamDWord(offset)             		(rtl_inl((offset)+RETENTION_RAM_AHB_FW_BASE))
#define ReadRRamWord(offset)              		(rtl_inw((offset)+RETENTION_RAM_AHB_FW_BASE))
#define ReadRRamByte(offset)              		(rtl_inb((offset)+RETENTION_RAM_AHB_FW_BASE))

//3 TX report buffer access

#define ReadCtrlInfoByte(macid, offset) 		rtl_inb(RTL8720E_CtrlInfo_AHB_BASE + (macid*40) + (offset))    //macid range: 0~16; offset range: 0~40
#define ReadCtrlInfoDWord(macid, offset) 	rtl_inl(RTL8720E_CtrlInfo_AHB_BASE + (macid*40) + (offset))    //macid range: 0~16; offset range: 0~40
#define WriteCtrlInfoByte(macid, offset, val)		rtl_outb(RTL8720E_CtrlInfo_AHB_BASE + (macid*40) + (offset), val)
#define WriteCtrlInfoDWord(macid, offset, val)	rtl_outl(RTL8720E_CtrlInfo_AHB_BASE + (macid*40) + (offset), val)


#define WriteTxrptByte(macid,offset,val)	rtl_outb(RTL8720E_TRXRPT_AHB_BASE + (macid<<4) + (offset), val) //macid range: 0~16; offset range: 0~15
#define WriteTxrptDWord(macid,offset,val)	rtl_outl(RTL8720E_TRXRPT_AHB_BASE + (macid<<4) + (offset), val)
#define ReadTxrptByte(macid,offset)   		rtl_inb(RTL8720E_TRXRPT_AHB_BASE + (macid<<4) + (offset))
#define ReadTxrptDWord(macid,offset)  		rtl_inl(RTL8720E_TRXRPT_AHB_BASE + (macid<<4) + (offset))

#define ReadCRC5Byte(macid, offset)  			rtl_inb(RTL8720E_CRC5_AHB_BASE + (macid) + (offset))       //rpt_idx range: 0~15; offset range: 0~1
#define WriteCRC5Byte(macid, offset, val)  	rtl_outb(RTL8720E_CRC5_AHB_BASE + (macid) + (offset), val)

#define ReadNANrptByte(offset)         			rtl_inb(RTL8720E_NANRPT_AHB_BASE  + (offset))
#define ReadNANrptDWORD(offset)        			rtl_inl(RTL8720E_NANRPT_AHB_BASE  + (offset))
#define WriteNANrptByte(offset, val)      	rtl_outb(RTL8720E_NANRPT_AHB_BASE + (offset), val)
#define WriteNANrptDWORD(offset, val)      	rtl_outl(RTL8720E_NANRPT_AHB_BASE + (offset), val)

//3 BT mailbox
#define WriteBTMailboxDWord(offset, value)	rtl_outl((offset)+RTL8720E_BT_MAILBOX_AHB_BASE, value)
#define WriteBTMailboxWord(offset, value)	rtl_outw((offset)+RTL8720E_BT_MAILBOX_AHB_BASE, value)
#define WriteBTmailboxByte(offset, value)	rtl_outb((offset)+RTL8720E_BT_MAILBOX_AHB_BASE, value)

#define ReadBTMailboxDWord(offset)        	(rtl_inl((offset)+RTL8720E_BT_MAILBOX_AHB_BASE))
#define ReadBTMailboxWord(offset)          	(rtl_inw((offset)+RTL8720E_BT_MAILBOX_AHB_BASE))
#define ReadBTMailboxByte(offset)        		(rtl_inb((offset)+RTL8720E_BT_MAILBOX_AHB_BASE))

//3 DDMA
#define WriteDDMADWord(offset, value)     	rtl_outl((offset)+RTL8720E_DDMA_CTRL_AHB_BASE, value)
#define WriteDDMAWord(offset, value)      	rtl_outw((offset)+RTL8720E_DDMA_CTRL_AHB_BASE, value)
#define WriteDDMAByte(offset, value)      	rtl_outb((offset)+RTL8720E_DDMA_CTRL_AHB_BASE, value)

#define ReadDDMADWord(offset)             		(rtl_inl((offset)+RTL8720E_DDMA_CTRL_AHB_BASE))
#define ReadDDMAWord(offset)                		(rtl_inw((offset)+RTL8720E_DDMA_CTRL_AHB_BASE))
#define ReadDDMAByte(offset)               		(rtl_inb((offset)+RTL8720E_DDMA_CTRL_AHB_BASE))

//3 physts
#define WritePHYSTSDWord(offset, value)     	rtl_outl((offset+RTL8720E_PHYSTS_AHB_BASE), value)
#define WritePHYSTWord(offset, value)      	rtl_outw((offset+RTL8720E_PHYSTS_AHB_BASE), value)
#define WritePHYSTByte(offset, value)      	rtl_outb((offset+RTL8720E_PHYSTS_AHB_BASE), value)

#define ReadPHYSTDWord(offset)             		(rtl_inl((offset+RTL8720E_PHYSTS_AHB_BASE)))
#define ReadPHYSTWord(offset)                	(rtl_inw((offset+RTL8720E_PHYSTS_AHB_BASE)))
#define ReadPHYSTByte(offset)               	(rtl_inb((offset+RTL8720E_PHYSTS_AHB_BASE)))

//3 channel info
#define WriteCHINFODWord(offset, value)     	rtl_outl((offset+RTL8720E_CHINFO_AHB_BASE), value)
#define WriteCHINFOWord(offset, value)      	rtl_outw((offset+RTL8720E_CHINFO_AHB_BASE), value)
#define WriteCHINFOByte(offset, value)      	rtl_outb((offset+RTL8720E_CHINFO_AHB_BASE), value)

#define ReadCHINFODWord(offset)             	(rtl_inl((offset+RTL8720E_CHINFO_AHB_BASE)))
#define ReadCHINFOWord(offset)                	(rtl_inw((offset+RTL8720E_CHINFO_AHB_BASE)))
#define ReadCHINFOByte(offset)               	(rtl_inb((offset+RTL8720E_CHINFO_AHB_BASE)))

//3 lpssyson
#define WriteLSONDWord(offset, value)        rtl_outl((offset)+SYSTEM_CTRL_BASE, value)
#define WriteLSONWord(offset, value)         	rtl_outw((offset)+SYSTEM_CTRL_BASE, value)
#define WriteLSONByte(offset, value)         	rtl_outb((offset)+SYSTEM_CTRL_BASE, value)

#define ReadLSONDWord(offset)                	(rtl_inl((offset)+SYSTEM_CTRL_BASE))
#define ReadLSONWord(offset)                 	(rtl_inw((offset)+SYSTEM_CTRL_BASE))
#define ReadLSONByte(offset)                 	(rtl_inb((offset)+SYSTEM_CTRL_BASE))

#define RFCRegToByteAddr(x)		((x)<<2)

//============================================================================
//       8720E Regsiter Bit and Content definition
//============================================================================
//WLAN PHY EN        0x0006
#define BIT_FEN_BB_GLB_RST     			BIT0
#define BIT_FEN_BB_RSTB              		BIT1
//MCUFWDL     0x0080
#define WINTINI_RDY                 			BIT6
#define RAM_DL_SEL                  			BIT7
//TXPAUSE 0x522
#define MAC_STOPBK                  			BIT0
#define MAC_STOPBE                  			BIT1
#define MAC_STOPVI                  			BIT2
#define MAC_STOPVO                  			BIT3
#define MAC_STOPMGQ                 			BIT4
#define MAC_STOPHIQ                 			BIT5
#define MAC_STOPBCNQ                		BIT6
#define MAC_STOPCPUMGQ			        	BIT7

/*------------------------Export global variable------------------------------*/
typedef struct _HAL_DATA_COMMON {
	u8  HCISel;
} HAL_DATA_COMMON, *PHAL_DATA_COMMON;

typedef struct _HAL_DATA_8720E {

	u32 fwimr[2];
	u32 fwisr[2];

	u32 wlanimr[6];
	u32 wlanisr[6];

	u32 ftimr[3];
	u32 ftisr[3];

} HAL_DATA_8720E, *PHAL_DATA_8720E;

#endif  /* __ASSEMBLY__ */
#endif  /* __WIFIFW_HAL_RAM_H__ */
