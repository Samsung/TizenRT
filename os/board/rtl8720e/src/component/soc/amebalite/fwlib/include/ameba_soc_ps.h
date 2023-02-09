#ifndef _HAL_SOCPS_H_
#define _HAL_SOCPS_H_

typedef struct {
	u32 Module;
	u32 Status;
} PWRCFG_TypeDef;

typedef struct {
	u32 Module;
	u32 Group;
	u32 Msk_NP;
	u32 Msk_AP;
	u32 Msk_DSP;
} WakeEvent_TypeDef;

typedef struct {
	u32 Module;
	u32 MEM_SD;
	u32 MEM_DS;
	u32 MEM_LS;
} MEMMode_TypeDef;

typedef struct {
	u32 wakepin;
	u32 Status;
	u32 Polarity; /* 1 is high, 0 is low */
} WAKEPIN_TypeDef;

extern u8 aon_wakepin[2];

extern WakeEvent_TypeDef sleep_wevent_config[];
extern PWRCFG_TypeDef sleep_sram_config[];
extern WAKEPIN_TypeDef sleep_wakepin_config[];
extern PWRCFG_TypeDef np_pwrmgt_config[];
extern PWRCFG_TypeDef np_aon_pwrmgt_config[];
extern PWRCFG_TypeDef dsleep_aon_wevent_config[];


extern void SOCPS_SleepPG(void);
extern void SOCPS_SleepCG(void);

extern void SOCPS_deepsleepInit(void);
extern void SOCPS_sleepInit(void);
extern void SOCPS_AP_suspend_config(u32 type, u32 Protection);
extern void SOCPS_AP_resume_config(u32 type, u32 Protection);
extern void SOCPS_NP_suspend(u32 type);
extern void SOCPS_NP_resume(u32 type);
extern void SOCPS_PSRAM_ClkSet(u8 Source);
extern void SOCPS_Voltage_Switch(u8 sleep);


extern int SOCPS_AONWakeReason(void);
extern int SOCPS_WakePinCheck(void);
extern void SOCPS_AONTimer(u32 SDuration);
extern void SOCPS_AONTimerINT_EN(u32 Status);
extern u32 SOCPS_AONTimerGet(VOID);
extern VOID SOCPS_AONTimerClearINT(void);
extern void SOCPS_SetWakepin(u32 PinIdx, u32 Polarity);
extern void SOCPS_SetWakepinDebounce(u32 Dbnc_cycle, u32 Status);
extern VOID SOCPS_WakePinClearINT(u32 wakepin);

extern void SOCPS_SetNPWakeEvent_MSK0(u32 Option, u32 NewStatus);
extern void SOCPS_SetNPWakeEvent_MSK1(u32 Option, u32 NewStatus);
extern void SOCPS_SetNPWakeEvent(u32 Option, u32 Group, u32 NewStatus);
extern void SOCPS_SetAPWakeEvent_MSK0(u32 Option, u32 NewStatus);
extern void SOCPS_SetAPWakeEvent_MSK1(u32 Option, u32 NewStatus);
extern void SOCPS_SetAPWakeEvent(u32 Option, u32 Group, u32 NewStatus);
extern void SOCPS_SetDSPWakeEvent_MSK0(u32 Option, u32 NewStatus);
extern void SOCPS_SetDSPWakeEvent_MSK1(u32 Option, u32 NewStatus);
extern void SOCPS_SetDSPWakeEvent(u32 Option, u32 Group, u32 NewStatus);
extern void SOCPS_DeepSleep_RAM(void);

#endif  //_HAL_SOCPS_H_
