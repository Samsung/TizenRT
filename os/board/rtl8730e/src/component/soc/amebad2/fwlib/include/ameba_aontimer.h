#ifndef _HAL_AONTIMER_H_
#define _HAL_AONTIMER_H_

extern void SOCPS_AONTimer(u32 SDuration);
extern void SOCPS_AONTimerINT_EN(u32 Status);
extern u32 SOCPS_AONTimerGet(VOID);
extern VOID SOCPS_AONTimerClearINT(void);

#endif  //_HAL_AONTIMER_H_