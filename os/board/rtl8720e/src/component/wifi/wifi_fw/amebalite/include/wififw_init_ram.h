#ifndef __WIFIFW_INIT_RAM_H__
#define __WIFIFW_INIT_RAM_H__


/* REG_CLKDIV 0x0008 */
#define MAC_CLK_20M				(0x2 << BIT_SHIFT_MAC_CLK_SEL)
#define MAC_CLK_40M				(0x1 << BIT_SHIFT_MAC_CLK_SEL)
#define BIT_SHIFT_PRETX_AGGR_TIME_MAX                8
#define BIT_SHIFT_MAC_CLK_SEL                        18

/* REG_FWHW_TXQ_HWSSN_CTRL[31:24] */
#define BIT_HWSEQ_VO_EN		BIT0
#define BIT_HWSEQ_VI_EN		BIT1
#define BIT_HWSEQ_BE_EN		BIT2
#define BIT_HWSEQ_BK_EN		BIT3
#define BIT_HWSEQ_MGT_EN		BIT4
#define BIT_HWSEQ_HI_EN		BIT5
#define BIT_HWSEQ_BCN_EN		BIT6
#define BIT_HWSEQ_CPUM_EN		BIT7
/* General Purpose Timer (Offset: 0x0150 - 0x016Fh) */
#define MODE_TIMER					1
#define MODE_COUNTER				0

#define TC_ENABLE						BIT0
#define TC_TIMER_MODE				BIT1
#define TC_INT_ENABLE				BIT2
/*--------------------Function declaration---------------------------------*/
extern void InitGTimer32us_8720E(u16 timer_addr, u8 _mode, u8 Unit2s, u8 Unit8ms, u8 Unit32us);
extern void InitGTimer1ms_8720E(u16 timer_addr, u8 _mode, u8 Unit65s, u8 Unit256ms, u8 Unit1ms);
extern void driver_fw_flow_ipc_int(VOID *Data, u32 IrqStatus, u32 ChanNum);
extern void wifi_FW_Vera_test(void);
extern void wifi_FW_init_ram(void);
extern void TaskCommon_8720E(void);
extern void TaskInit_8720E(void);
extern void Wifi_FW_TaskIdle(void);
extern void Fw_test_main(void);
extern void FwIntInit(void);
extern void TimerTest(void);
extern void SOCZB_test_main(void);

#endif /* __WIFIFW_INIT_RAM_H__ */
