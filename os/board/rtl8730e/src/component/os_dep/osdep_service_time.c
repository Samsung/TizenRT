/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#include <osdep_service.h>
#include <stdio.h>

uint32_t rtw_get_current_time(void)
{
	return (u32) clock();
}

u32 rtw_systime_to_ms(u32 sys_time)
{
	return (sys_time * 1000L / TICK_PER_SEC);
}

u32 rtw_systime_to_sec(u32 sys_time)
{
	return (sys_time / TICK_PER_SEC);
}

u32 rtw_ms_to_systime(u32 ms)
{
	return (ms * TICK_PER_SEC / 1000L);
}

u32 rtw_sec_to_systime(u32 sec)
{
	return (sec * TICK_PER_SEC);
}

// the input parameter start use the same unit as returned by rtw_get_current_time
u32 rtw_get_passing_time_ms(u32 start)
{
	return rtw_systime_to_ms(rtw_get_current_time() - start);
}

s32 rtw_get_time_interval_ms(u32 start, u32 end)
{
	return rtw_systime_to_ms(end - start);
}

void rtw_msleep_os(int ms)
{
	usleep((unsigned int)ms * 1000);
}

void rtw_usleep_os(int us)
{
	usleep((unsigned int)us);
}

void rtw_mdelay_os(int ms)
{
	up_mdelay((unsigned long)ms);
}

void rtw_udelay_os(int us)
{
	up_udelay((unsigned long)us);
}

void rtw_yield_os(void)
{
	sched_yield();
}


