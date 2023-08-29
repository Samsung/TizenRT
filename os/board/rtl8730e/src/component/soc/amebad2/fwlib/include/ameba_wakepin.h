#ifndef _HAL_WAKEPIN_H_
#define _HAL_WAKEPIN_H_

typedef struct {
	u32 wakepin;
	u32 Status;
	u32 Polarity; /* 1 is high, 0 is low */
} WAKEPIN_TypeDef;

extern u8 aon_wakepin[4];

extern WAKEPIN_TypeDef sleep_wakepin_config[];
extern int SOCPS_WakePinCheck(void);

extern void SOCPS_SetWakepin(u32 PinIdx, u32 Polarity);
extern void SOCPS_SetWakepinDebounce(u32 Dbnc_cycle, u32 Status);
extern VOID SOCPS_WakePinClearINT(u32 wakepin);

#endif  //_HAL_WAKEPIN_H_
