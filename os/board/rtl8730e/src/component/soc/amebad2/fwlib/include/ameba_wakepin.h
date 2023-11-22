#ifndef _HAL_WAKEPIN_H_
#define _HAL_WAKEPIN_H_

#define	WAKEPIN_0							((u32)0x00000000)/*!< see aon_wakepin */
#define	WAKEPIN_1							((u32)0x00000001)/*!< see aon_wakepin */
#define	WAKEPIN_2							((u32)0x00000002)/*!< see aon_wakepin */
#define	WAKEPIN_3							((u32)0x00000003)/*!< see aon_wakepin */

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
