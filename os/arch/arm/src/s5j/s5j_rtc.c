/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/s5j/s5j_rtc.c
 *
 *   Copyright (C) 2009-2010, 2014-2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/arch.h>

#include <tinyara/irq.h>
#include <tinyara/rtc.h>
#include <arch/board/board.h>

#include <arch/chip/chip_types.h>
#include <chip.h>

#include "chip.h"
#include "up_arch.h"
#include "s5j_rtc.h"

#include <tinyara/kmalloc.h>

#ifdef CONFIG_RTC

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/***************************************************************************
 * This is the private type for the RTC state.  It must be cast compatible
 * with struct rtc_lowerhalf_s.
 ***************************************************************************/
#ifdef CONFIG_RTC_DRIVER
struct s5j_rtc_lowerhalf_s {
	FAR const struct rtc_ops_s *ops;

	/* Data following is private to this driver and not visible outside of
	 * this file.
	 */

	FAR struct s5j_rtc_lowerhalf_s *next;
	uint32_t irqno;
	uintptr_t base;
};
#endif

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* Prototypes for static methods in struct rtc_ops_s */

#ifdef CONFIG_RTC_DRIVER
static int s5j_rtc_rdtime(FAR struct rtc_lowerhalf_s *lower, FAR struct rtc_time *rtctime);
static int s5j_rtc_settime(FAR struct rtc_lowerhalf_s *lower, FAR const struct rtc_time *rtctime);
#endif

#ifdef CONFIG_RTC_ALARM

static int s5j_rtc_rdalarm(FAR struct rtc_lowerhalf_s *lower, FAR struct rtc_time *almtime);
static int s5j_rtc_setalarm(FAR struct rtc_lowerhalf_s *lower, FAR const struct rtc_time *rtctime);
static int s5j_rtc_aiealarm(FAR struct rtc_lowerhalf_s *lower, bool enable);
#endif

#ifdef CONFIG_RTC_PERIODIC
static int s5j_rtc_set_tick_period(FAR struct rtc_lowerhalf_s *lower, FAR unsigned long irqpfreq);
static int s5j_rtc_enable_tickirq(FAR struct rtc_lowerhalf_s *lower, bool enable);
#endif
/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_RTC_DRIVER
static const struct rtc_ops_s g_rtc_ops = {
	.rdtime = s5j_rtc_rdtime,
	.settime = s5j_rtc_settime,
#ifdef CONFIG_RTC_ALARM
	.almread = s5j_rtc_rdalarm,
	.almset = s5j_rtc_setalarm,
#endif
#ifdef CONFIG_RTC_PERIODIC
	.irqpread = NULL,
	.irqpset = s5j_rtc_set_tick_period,
#endif
#ifdef CONFIG_RTC_ALARM
	.aie = s5j_rtc_aiealarm,
#endif
#ifdef CONFIG_RTC_ONESEC
	.uie = NULL,
#endif
#ifdef CONFIG_RTC_PERIODIC
	.pie = s5j_rtc_enable_tickirq,
#endif
#ifdef CONFIG_RTC_EPOCHYEAR
	.rdepoch = NULL,
	.setepoch = NULL,
#endif
#ifdef CONFIG_RTC_ALARM
	.rdwkalm = NULL,
	.setwkalm = NULL,
#endif
#ifdef CONFIG_RTC_IOCTL
	.ioctl = NULL,
#endif
};

static struct rtc_lowerhalf_s g_rtc_lowerhalf = {
	.ops = &g_rtc_ops,
};

#endif
/****************************************************************************
 * Public Data
 ****************************************************************************/

volatile bool g_rtc_enabled = false;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rtc_bin2bcd
 *
 * Description:
 *   Converts binary time format into bcd
 *
 * Input Parameters:
 *   value - binary time value
 *
 * Returned Value:
 *   BCD converted value
 *
 ****************************************************************************/
static uint32_t rtc_bin2bcd(int value)
{
	uint32_t hundred = 0;
	uint32_t ten = 0;

	while (value >= 100) {
		hundred++;
		value -= 100;
	}

	while (value >= 10) {
		ten++;
		value -= 10;
	}

	return (hundred << 8) | (ten << 4) | value;
}

/****************************************************************************
 * Name: rtc_bcd2bin
 *
 * Description:
 *   Converts BCD time format into binary
 *
 * Input Parameters:
 *   value - BCD value
 *
 * Returned Value:
 *   binary converted value
 *
 ****************************************************************************/
static int rtc_bcd2bin(uint32_t value)
{
	uint32_t tens = ((value >> 4) & 0xf) * 10;
	uint32_t hundred = ((value >> 8) & 0xf) * 100;
	return (int)(hundred + tens + (value & 0x0f));
}

/****************************************************************************
 * Name: __isr_rtc_tick
 *
 * Description:
 *   RTC TICK ISR
 ****************************************************************************/
void __isr_rtc_tick(void)
{
	lldbg("%s\n", __func__);
	HW_REG32(S5J_RTC_BASE, INTP) = RTC_INTP_TIC_MASK;
}

/****************************************************************************
 * Name: __isr_rtc_alarm
 *
 * Description:
 *   RTC ALARM ISR
 ****************************************************************************/
void __isr_rtc_alarm(void)
{
	lldbg("%s\n", __func__);
	HW_REG32(S5J_RTC_BASE, INTP) = RTC_INTP_ALM_MASK;
}

/****************************************************************************
 * Name: s5j_gettime
 *
 * Description:
 *   HW level get current time function call
 *
 * Input Parameters:
 *   base - base address on RTC IP SFRs
 *   t - pointer on tm structure where current time values will be stored
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
int s5j_gettime(uintptr_t base, struct tm *t)
{
	uint32_t sec, min, hour, mday, mon, year;
	irqstate_t flags;
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	uint32_t wday;
#endif
	flags = irqsave();

	do {
		sec = getreg32(base + BCDSEC);
		min = getreg32(base + BCDMIN);
		hour = getreg32(base + BCDHOUR);
		mday = getreg32(base + BCDDAY);
		mon = getreg32(base + BCDMON);
		year = getreg32(base + BCDYEAR);
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
		wday = getreg32(base + BCDDAYWEEK);
#endif
	} while (sec != getreg32(base + BCDSEC));

	irqrestore(flags);

	t->tm_sec = rtc_bcd2bin(sec);
	t->tm_min = rtc_bcd2bin(min);
	t->tm_hour = rtc_bcd2bin(hour);
	t->tm_mday = rtc_bcd2bin(mday);
	t->tm_mon = rtc_bcd2bin(mon) - 1;	/* range 0-11, need to -1 */
	t->tm_year = rtc_bcd2bin(year);

#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	t->tm_wday = rtc_bcd2bin(wday);
#endif

	return OK;
}

/****************************************************************************
 * Name: s5j_settime
 *
 * Description:
 *   HW level set current time function call
 *
 * Input Parameters:
 *   base - base address on RTC IP SFRs
 *   t - pointer on tm structure with values to set
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
int s5j_settime(uintptr_t base, struct tm *t)
{
	uint32_t sec;
	uint32_t min;
	uint32_t hour;
	uint32_t mday;
	uint32_t mon;
	uint32_t year;
	irqstate_t flags;
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	uint32_t wday;
#endif

	flags = irqsave();

	sec = rtc_bin2bcd(t->tm_sec);
	min = rtc_bin2bcd(t->tm_min);
	hour = rtc_bin2bcd(t->tm_hour);
	mday = rtc_bin2bcd(t->tm_mday);
	mon = rtc_bin2bcd(t->tm_mon);
	year = rtc_bin2bcd(t->tm_year);
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	wday = rtc_bin2bcd(t->tm_wday);
#endif

	putreg32(sec, base + BCDSEC);
	putreg32(min, base + BCDMIN);
	putreg32(hour, base + BCDHOUR);
	putreg32(mday, base + BCDDAY);
	putreg32(mon, base + BCDMON);
	putreg32(year, base + BCDYEAR);
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	putreg32(wday, base + BCDDAYWEEK);
#endif

	irqrestore(flags);

	return OK;
}

#ifdef CONFIG_RTC_ALARM

/****************************************************************************
 * Name: s5j_getalarm
 *
 * Description:
 *   HW level get alarm function call
 *
 * Input Parameters:
 *   base - base address on RTC IP SFRs
 *   t - pointer on tm structure where alarm values will be stored
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
static int s5j_getalarm(uintptr_t base, struct tm *t)
{
	uint32_t sec;
	uint32_t min;
	uint32_t hour;
	uint32_t mday;
	uint32_t mon;
	uint32_t year;
	irqstate_t flags;
	flags = irqsave();

	do {
		sec = getreg32(base + ALMSEC);
		min = getreg32(base + ALMMIN);
		hour = getreg32(base + ALMHOUR);
		mday = getreg32(base + ALMDAY);
		mon = getreg32(base + ALMMON);
		year = getreg32(base + ALMYEAR);
	} while (sec != getreg32(base + ALMSEC));

	irqrestore(flags);

	t->tm_sec = rtc_bcd2bin(sec);
	t->tm_min = rtc_bcd2bin(min);
	t->tm_hour = rtc_bcd2bin(hour);
	t->tm_mday = rtc_bcd2bin(mday);
	t->tm_mon = rtc_bcd2bin(mon) - 1;	/* range 0-11, need to -1 */
	t->tm_year = rtc_bcd2bin(year);
	return OK;
}

/****************************************************************************
 * Name: s5j_setalarm
 *
 * Description:
 *   HW level set alarm function call
 *
 * Input Parameters:
 *   base - base address on RTC IP SFRs
 *   t - pointer on tm structure with values to set
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
static int s5j_setalarm(uintptr_t base, struct tm *t)
{
	uint32_t sec;
	uint32_t min;
	uint32_t hour;
	uint32_t mday;
	uint32_t mon;
	uint32_t year;
	irqstate_t flags;

	flags = irqsave();

	sec = rtc_bin2bcd(t->tm_sec);
	min = rtc_bin2bcd(t->tm_min);
	hour = rtc_bin2bcd(t->tm_hour);
	mday = rtc_bin2bcd(t->tm_mday);
	mon = rtc_bin2bcd(t->tm_mon);
	year = rtc_bin2bcd(t->tm_year);

	putreg32(sec, base + ALMSEC);
	putreg32(min, base + ALMMIN);
	putreg32(hour, base + ALMHOUR);
	putreg32(mday, base + ALMDAY);
	putreg32(mon, base + ALMMON);
	putreg32(year, base + ALMYEAR);

	irqrestore(flags);

	return OK;
}

#endif							/* CONFIG_RTC_ALARM */

/****************************************************************************
 * Name: s5j_rtc_rdtime
 *
 * Description:
 *   Part of rtc_ops_s structure, used when CONFIG_RTC_ALARM is enabled.
 *   Gets current time.
 *
 * Input Parameters:
 *   lower - Lower half rtc structure pointer, corresponded to s5j_rtc
 *   rtctime - pointer on rtc_time structure where current time
 *   will be stored
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
#ifdef CONFIG_RTC_DRIVER
static int s5j_rtc_rdtime(FAR struct rtc_lowerhalf_s *lower, FAR struct rtc_time *rtctime)
{
	return s5j_gettime(S5J_RTC_BASE, (FAR struct tm *)rtctime);
}
#endif

#ifdef CONFIG_RTC_ALARM
/****************************************************************************
 * Name: s5j_rtc_rdalarm
 *
 * Description:
 *   Part of rtc_ops_s structure, used when CONFIG_RTC_ALARM is enabled.
 *   Set ALARM time.
 *
 * Input Parameters:
 *   lower - Lower half rtc structure pointer, corresponded to s5j_rtc
 *   almtime - pointer on rtc_time structure where current alarm time
 *   will be stored
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
static int s5j_rtc_rdalarm(FAR struct rtc_lowerhalf_s *lower, FAR struct rtc_time *almtime)
{
	return s5j_getalarm(S5J_RTC_BASE, (FAR struct tm *)almtime);
}

/****************************************************************************
 * Name: s5j_rtc_setalarm
 *
 * Description:
 *   Part of rtc_ops_s structure, used when CONFIG_RTC_ALARM is enabled.
 *   Set ALARM time.
 *
 * Input Parameters:
 *   lower - Lower half rtc structure pointer, corresponded to s5j_rtc
 *   almtime - pointer on rtc_time structure with desired alarm time
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
static int s5j_rtc_setalarm(FAR struct rtc_lowerhalf_s *lower, FAR const struct rtc_time *almtime)
{
	return s5j_setalarm(S5J_RTC_BASE, (FAR struct tm *)almtime);
}

/****************************************************************************
 * Name: s5j_rtc_aiealarm
 *
 * Description:
 *   Part of rtc_ops_s structure, used when CONFIG_RTC_ALARM is enabled.
 *   Enables ALARM interrupt.
 *   RTC ISR __isr_rtc_alarm will be called.
 *
 * Input Parameters:
 *   lower - Lower half rtc structure pointer, corresponded to s5j_rtc
 *   enable - enable od disable alarm interrupt
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
static int s5j_rtc_aiealarm(FAR struct rtc_lowerhalf_s *lower, bool enable)
{
	if (enable) {
		irq_attach(IRQ_TOP_RTC_ALARM, (xcpt_t) __isr_rtc_alarm);
		up_enable_irq(IRQ_TOP_RTC_ALARM);

		HW_REG32(S5J_RTC_BASE, RTCALM) |= RTC_GLB_ALM_EN;	/* Enable Alarm Global */
	} else {
		HW_REG32(S5J_RTC_BASE, RTCALM) &= ~RTC_GLB_ALM_EN;	/* Disable Alarm Global */
	}
	return OK;

	return OK;
}
#endif							/* CONFIG_RTC_ALARM */

#ifdef CONFIG_RTC_PERIODIC
/****************************************************************************
 * Name: s5j_rtc_set_tick_period
 *
 * Description:
 *   Part of rtc_ops_s structure, used when CONFIG_RTC_PERIODIC is enabled.
 *   Defines period (not freq) of rtc interrupts with 1 sec resolution.
 *
 * Input Parameters:
 *   lower - Lower half rtc structure pointer, corresponded to s5j_rtc
 *   irqfreq - periodic interrupt period, in seconds
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
static int s5j_rtc_set_tick_period(FAR struct rtc_lowerhalf_s *lower, FAR unsigned long irqpfreq)
{
	/* Set TICKCNT value for every 1 sec  */
	/*
	 * RTC Tick Interrupt Period
	 * Period = (n + 1) /(Tick clock source frequency) second. (n = tick counter value
	 * Refer to Tick Interrupt Resolution in S5JT200 UM
	 */
	HW_REG32(S5J_RTC_BASE, TICCNT) = RTC_CLK_FREQ * irqpfreq + 1;	/* in sec */
	return OK;
}

/****************************************************************************
 * Name: s5j_rtc_enable_tickirq
 *
 * Description:
 *   Part of rtc_ops_s structure, used when CONFIG_RTC_PERIODIC is enabled.
 *   Enables periodic interrupts, with defined period
 *   by s5j_rtc_set_tick_period function call.
 *   RTC ISR __isr_rtc_tick will be periodically called.
 *
 * Input Parameters:
 *   lower - Lower half rtc structure pointer, corresponded to s5j_rtc
 *   enable - enable od disable periodic interrupt
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
static int s5j_rtc_enable_tickirq(FAR struct rtc_lowerhalf_s *lower, bool enable)
{
	if (enable) {
		irq_attach(IRQ_TOP_RTC_TIC, (xcpt_t) __isr_rtc_tick);
		up_enable_irq(IRQ_TOP_RTC_TIC);

		HW_REG32(S5J_RTC_BASE, RTCCON) |= RTCCON_TICEN;		/* Enable Tick Timer */
	} else {
		HW_REG32(S5J_RTC_BASE, RTCCON) &= ~RTCCON_TICEN;	/* disable Tick Timer */
	}
	return OK;
}
#endif

/****************************************************************************
 * Name: s5j_rtc_settime
 *
 * Description:
 *   Initialize the hardware RTC per the selected configuration.
 *   This function is called once during the OS initialization sequence
 *
 * Input Parameters:
 *   lower - Lower half rtc structure pointer, corresponded to s5j_rtc
 *   rtctime - pointer on rtc_time structure with values to set
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
#ifdef CONFIG_RTC_DRIVER
static int s5j_rtc_settime(FAR struct rtc_lowerhalf_s *lower, FAR const struct rtc_time *rtctime)
{
	return s5j_settime(S5J_RTC_BASE, (FAR struct tm *)rtctime);
}
#endif

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/****************************************************************************
 * Name: up_rtc_initialize
 *
 * Description:
 *   Initialize the hardware RTC per the selected configuration.
 *   This function is called once during the OS initialization sequence.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/
int s5j_rtc_initialize(void)
{
	irqstate_t flags;
	struct tm t;

	/* No clocking setup need be performed. The Real-time Clock is continuously clocked
	 * at 32768 Hz (SCLK). The Power Management Controller has no effect on RTC behavior.
	 */

	flags = irqsave();

	/* Set RTC input clock to external XTAL oscillator (32768HZ) explicily*/
	HW_REG32(0x800A0000, 0x554) = 0x1;

	/* Reset RTC and Tick timer. */
	HW_REG32(S5J_RTC_BASE, RTCCON) = 0x0;

	/* Enable RTC counter, Tick clock 32768, Tick Timer enable */
	HW_REG32(S5J_RTC_BASE, RTCCON) |= RTCCON_CTLEN | RTCCON_TICCKSEL(clk_32768HZ);

	t.tm_sec = 0;
	t.tm_min = 0;
	t.tm_hour = 0;
	t.tm_mday = CONFIG_START_DAY;
	t.tm_mon = CONFIG_START_MONTH;
	t.tm_year = CONFIG_START_YEAR - CONFIG_RTC_BASE_YEAR;

	s5j_settime(S5J_RTC_BASE, &t);

#ifdef CONFIG_RTC_ALARM

	irq_attach(IRQ_TOP_RTC_ALARM, (xcpt_t) __isr_rtc_alarm);
	up_enable_irq(IRQ_TOP_RTC_ALARM);

	/* Set Alarm after 1 min */
	t.tm_sec = 0;
	t.tm_min = 0;
	t.tm_hour = 0;
	t.tm_mday = CONFIG_START_DAY;
	t.tm_mon = CONFIG_START_MONTH;
	t.tm_year = CONFIG_START_YEAR - CONFIG_RTC_BASE_YEAR;
	s5j_setalarm(S5J_RTC_BASE, &t);

	HW_REG32(S5J_RTC_BASE, RTCALM) = RTC_YEAR_ALM_EN | RTC_MONTH_ALM_EN | RTC_DAY_ALM_EN | RTC_HOUR_ALM_EN | RTC_MIN_ALM_EN | RTC_SEC_ALM_EN;

#endif

	irqrestore(flags);

	return OK;
}

/****************************************************************************
 * Name: up_rtc_getdatetime
 *
 * Description:
 *   Get the current date and time from the date/time RTC.  This interface
 *   is only supported by the date/time RTC hardware implementation.
 *   It is used to replace the system timer.  It is only used by the RTOS during
 *   initialization to set up the system time when CONFIG_RTC and CONFIG_RTC_DATETIME
 *   are selected (and CONFIG_RTC_HIRES is not). Due to low resoluton of RTC,
 *   subsecond values will be lost.
 *
 * Input Parameters:
 *   tp - pointer on tm type structure where date/time will be saved
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/

int up_rtc_getdatetime(FAR struct tm *tp)
{
	return s5j_gettime(S5J_RTC_BASE, tp);
}

/****************************************************************************
 * Name: up_rtc_time
 *
 * Description:
 *   Get the current time in seconds.  This is similar to the standard time()
 *   function.  This interface is only required if the low-resolution RTC/counter
 *   hardware implementation selected.  It is only used by the RTOS during
 *   initialization to set up the system time when CONFIG_RTC is set but neither
 *   CONFIG_RTC_HIRES nor CONFIG_RTC_DATETIME are set.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   time_t value in seconds
 *
 ****************************************************************************/

#ifndef CONFIG_RTC_HIRES
time_t up_rtc_time(void)
{
	struct tm t;

	s5j_gettime(S5J_RTC_BASE, &t);

	return mktime(&t);
}
#endif

/****************************************************************************
 * Name: up_rtc_gettime
 *
 * Description:
 *   Returns current time value in timespec structure.
 *   Exists when RTC_HIRES configuration is enabled.
 *   nsec value is always 0 due to low actual resolution of RTC counter.
 *
 * Input Parameters:
 *   tp - pointer on timespec structure where time value will be stored.
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/

#ifdef CONFIG_RTC_HIRES
int up_rtc_gettime(FAR struct timespec *tp)
{
	struct tm t;

	s5j_gettime(S5J_RTC_BASE, &t);

	tp->tv_sec = mktime(&t);
	tp->tv_nsec = 0;

	return OK;
}
#endif

/****************************************************************************
 * Name: up_rtc_settime
 *
 * Description:
 *   Function called by OS to set RTC date/time value.
 *
 * Input Parameters:
 *  tp - pointer on timespec structure with desired value.
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/

int up_rtc_settime(FAR const struct timespec *tp)
{
	FAR struct tm newtime;

	(void)gmtime_r(&tp->tv_sec, &newtime);

	return s5j_settime(S5J_RTC_BASE, &newtime);

}

/****************************************************************************
 * Name: up_rtcinitialize
 *
 * Description:
 *   Function called by OS to initialize RTC.
 *   Initialyizes HW and sublmits lower half OPS structure.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   OK
 *
 ****************************************************************************/

int up_rtcinitialize(void)
{
#ifdef CONFIG_S5J_RTC
	int ret;
	s5j_rtc_initialize();
	ret = rtc_initialize(0, &g_rtc_lowerhalf);
	if (ret < 0) {
		lldbg("Failed the RTC initialize. (%d)\n", ret);
		return ret;
	}

	/* Set RTC enable flag */
	g_rtc_enabled = true;
#else
	lldbg("no support Hardware RTC functions\n");
#endif
	return OK;
}

#endif
