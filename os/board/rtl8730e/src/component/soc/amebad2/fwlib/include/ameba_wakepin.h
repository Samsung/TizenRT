#ifndef _HAL_WAKEPIN_H_
#define _HAL_WAKEPIN_H_

enum wakepin_config {
	LOW_LEVEL_WAKEUP	= 0, /* when GPIO level become high, will wakeup*/
	HIGH_LEVEL_WAKEUP	= 1, /* when GPIO level become low, will wakeup*/
	DISABLE_WAKEPIN	= 2, /* disable wakepin */
};

typedef struct {
	u32 wakepin;
	enum wakepin_config config;
} WAKEPIN_TypeDef;

extern u8 aon_wakepin[4];

extern int SOCPS_WakePinCheck(void);

extern void SOCPS_SetWakepin(u32 PinIdx, u32 Polarity);
extern void SOCPS_SetWakepinDebounce(u32 Dbnc_cycle, u32 Status);
extern VOID SOCPS_WakePinClearINT(u32 wakepin);

#endif  //_HAL_WAKEPIN_H_
