#ifndef __WIFIFW_INT_RAM_H__
#define __WIFIFW_INT_RAM_H__

/*--------------------Define --------------------------------------------*/
#define CLKIs32K  1
#define CLKIs40M  0

/*--------------------Function declaration---------------------------------*/
extern void WLAN_INT_HDL_8720E(void);
extern void FT_INT_HDL_8720E(void);
extern void ISRSendSignalCommon_8720E(u32  event);
extern void ISR_TBTT_8720E(void);
extern void ISR_BCNRx_8720E(void);
extern void ISR_BCNRxErr_8720E(void);
extern void ISR_P0_TBTT_AGG_8720E(void);
extern void ISR_TIM_NO_HIT_8720E(void);
extern void ISR_TIM_HIT_8720E(void);
extern void ISR_HIOE_CHN0_8720E(void);
extern void ISR_HIOE_CHN1_8720E(void);
extern void ISR_HIOE_CHN_ON_8720E(void);
extern void ISR_HIOE_CHN_OFF_8720E(void);
extern void ISR_TSF_GT_THRES_8720E(void);
extern void ISR_RXBCN_TIMEUP_8720E(void);
extern void ISR_RXBCN_TIMEOUT_CNT_8720E(void);
extern void ISR_RxBMC_MD0_8720E(void);
extern void ISR_RxBMC_MD1_8720E(void);
extern void ISR_RxUnicast_MD1_8720E(void);
extern void ISR_RxUnicast_MD0_8720E(void);
extern void ISR_TxPKTIn_8720E(void);
extern void ISR_GTimer0_8720E(void);
extern void ISR_GTimer1_8720E(void);
extern void ISR_GTimer2_8720E(void);
extern void ISR_RPWM_8720E(void);
extern void ISR_GTimer5_8720E(void);
extern void ISR_BcnEarlyAGG_8720E(void);
extern void ISR_GTimer6_8720E(void);
extern void ISR_WlanActOn_8720E(void);
extern void ISR_WlanActOff_8720E(void);
extern void ISR_TxNull0_8720E(void);
extern void ISR_TxNull1_8720E(void);
extern void ISR_PSTIMERA_8720E(void);
extern void ISR_PSTIMERAEarly_8720E(void);
extern void ISR_PSTIMEOUT0_8720E(void);
extern void ISR_PSTIMERB_8720E(void);
extern void ISR_PSTIMERBEarly_8720E(void);
extern void ISR_PSTIMEOUT1_8720E(void);
extern void ISR_PSTIMERC_8720E(void);
extern void ISR_PSTIMERCEarly_8720E(void);
extern void ISR_PSTIMEOUT2_8720E(void);
extern void ISR_PSTIMERD_8720E(void);
extern void ISR_PSTIMERDEarly_8720E(void);
extern void ISR_PSTIMERE_8720E(void);
extern void ISR_TBTT1_8720E(void);
extern void ISR_TXCCX_8720E(void);
extern void ISR_RxDone_8720E(void);
extern void ISR_H2CCMD_8720E(void);
extern void ISR_HRCV_8720E(void);
extern void ISR_BcnEarly_8720E(void);
extern void ISR_GTimer3_8720E(void);
extern void ISR_GTimer4_8720E(void);
extern void ISR_BCNERLY1_8720E(void);
extern void ISR_BTScoreboard_8720E(void);
extern void ISR_ZBMailbox_8720E(void);
extern void ISR_GNT_BT_Rise_8720E(void);
extern void ISR_GNT_BT_Fall_8720E(void);
extern void ISR_CPUMGN_POLLED_PKT_DONE_8720E(void);
extern void ClrAllFWUsedIMR_8720E(void);
extern void ClrAllFWUsedISR_8720E(void);
extern void UpdateISRWlan_8720E(void);
extern void UpdateISRFT_8720E(void);
extern void setfwimr_8720E(u32 index, u32 bitmask);
extern void clrfwimr_8720E(u32 index, u32 bitmask);
extern void setwlanimr_8720E(u32 index, u32 bitmask);
extern void clrwlanimr_8720E(u32 index, u32 bitmask);
extern void setftimr_8720E(u32 index, u32 bitmask);
extern void clrftimr_8720E(u32 index, u32 bitmask);
#if CONFIG_RATE_ADAPTIVE
extern void ISR_TxrptCntFull_8720E(void);
#endif


#endif  /* __WIFIFW_INT_RAM_H__ */
