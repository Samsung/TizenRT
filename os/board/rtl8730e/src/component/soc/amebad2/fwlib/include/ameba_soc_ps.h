#ifndef _HAL_SOCPS_H_
#define _HAL_SOCPS_H_

typedef struct {
	u32 Module;
	u32 MEM_SD;
	u32 MEM_DS;
	u32 MEM_LS;
} MEMMode_TypeDef;

typedef struct {
	u32 Module;
	u32 Status;
} PWRCFG_TypeDef;

typedef struct {
	u32 Module;
	u8 Group;
	u8 Msk_Lp;
	u8 Msk_Np;
	u8 Msk_Ap;
} WakeEvent_TypeDef;

extern WakeEvent_TypeDef sleep_wevent_config[];
extern PWRCFG_TypeDef sleep_sram_config[];
extern PWRCFG_TypeDef km0_pwrmgt_config[];
extern PWRCFG_TypeDef aon_pwrmgt_config[];
extern PWRCFG_TypeDef dsleep_aon_wevent_config[];

extern void SOCPS_SleepPG(void);
extern void SOCPS_SleepCG(void);

extern void SOCPS_SleepInit(void);
extern u32 SOCPS_DsleepWakeStatusGet(void);

extern void SOCPS_SetNPWakeEvent_MSK0_HP(u32 Option, u32 NewStatus);
extern void SOCPS_SetNPWakeEvent_MSK1_HP(u32 Option, u32 NewStatus);

extern int SOCPS_AONWakeReason(void);


extern VOID SOCPS_MMUReFill(VOID);

extern void SOCPS_SetLPWakeEvent_MSK0(u32 Option, u32 NewStatus);
extern void SOCPS_SetLPWakeEvent_MSK1(u32 Option, u32 NewStatus);
extern void SOCPS_SetLPWakeEvent(u32 Option, u32 Group, u32 NewStatus);
extern void SOCPS_SetNPWakeEvent_MSK0(u32 Option, u32 NewStatus);
extern void SOCPS_SetNPWakeEvent_MSK1(u32 Option, u32 NewStatus);
extern void SOCPS_SetNPWakeEvent(u32 Option, u32 Group, u32 NewStatus);
extern void SOCPS_SetAPWakeEvent_MSK0(u32 Option, u32 NewStatus);
extern void SOCPS_SetAPWakeEvent_MSK1(u32 Option, u32 NewStatus);
extern void SOCPS_SetAPWakeEvent(u32 Option, u32 Group, u32 NewStatus);

extern void SOCPS_SWRLDO_Suspend(u32 new_status);
extern void SOCPS_SleepCG_RAM(void);
extern void SOCPS_SleepPG_RAM(void);
extern void SOCPS_DeepSleep_RAM(void);
extern void SOCPS_LPWHP_ipc_int(VOID *Data, u32 IrqStatus, u32 ChanNum);

extern void SOCPS_deepsleepInit(void);
extern void SOCPS_WakeEvent_Init(void);
extern void SOCPS_AP_WakeEvent_Init(void);
extern void SOCPS_SetPowerCut_Time(u32 time);

#endif  //_HAL_SOCPS_H_
