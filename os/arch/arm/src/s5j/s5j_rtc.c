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
/* This is the private type for the RTC state.  It must be cast compatible
 * with struct rtc_lowerhalf_s.
 */

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

/**
 *
 * @brief    Converts a binary to 3 digit BCD format.
 * @param    int value :  The byte to be converted.
 * @return   The value in BCD representation
 */

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

/**
 *
 * @brief    Convert from 3 digit BCD to binary.
 * @param    uint32_t value : The BCD value to be converted.
 * @return   The value in binary representation.
 */

static int rtc_bcd2bin(uint32_t value)
{
	uint32_t tens = ((value >> 4) & 0xf) * 10;
	uint32_t hundred = ((value >> 8) & 0xf) * 100;
	return (int)(hundred + tens + (value & 0x0f));
}

/**
 * @brief	 Function Description : This function is Interrupt Service Routine of RTC Tick Timer
 *
 */
void __isr_rtc_tick(void)
{
	lldbg("%s\n", __func__);
	HW_REG32(S5J_RTC_BASE, INTP) = (0x1 << 0);
}

void __isr_rtc_alarm(void)
{
	lldbg("%s\n", __func__);
	HW_REG32(S5J_RTC_BASE, INTP) = (0x1 << 1);
}

#ifdef CONFIG_RTC_ALARM
static int s5j_rtc_enable_alarm_interrupt(uint32_t sec)
{
	/*
	 * RTC Tick Interrupt Period
	 * Period = (n + 1) /(Tick clock source frequency) second. (n = tick counter value
	 * Refer to Tick Interrupt Resolution in S5JT200 UM
	 */
	irq_attach(IRQ_TOP_RTC_ALARM, (xcpt_t)__isr_rtc_tick);
	up_enable_irq(IRQ_TOP_RTC_ALARM);

	/* Set TICKCNT value for every 1 sec  */
	HW_REG32(S5J_RTC_BASE, TICCNT) = 32768 * sec;
	HW_REG32(S5J_RTC_BASE, RTCCON) &= ~(0x1 << 8);
	HW_REG32(S5J_RTC_BASE, RTCCON) |= (0x1 << 8);	/* Enable Tick Timer */

	return 0;
}
#endif

/**
 *
 * @brief    Time get from S5JXXXX RTC
 * @param    uintptr_t base : base address of RTC
 * @param    struct tm *t : time structure
 * @return    == 0: read ok
 * @note
 *
 *  Now convert the RTC date to fields in struct tm format:
 *  Days: 1-31 match in both cases.
 *   Month: S5J is 1-12, struct tm is 0-11.
 *   Years: S5H is 00-99, struct tm is years since 1900.
 *
 */
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
/**
 *
 * @brief    Alarm get from S5JXXXX RTC
 * @param    uintptr_t base : base address of RTC
 * @param    struct tm *t : time structure
 * @return    == 0: read ok
 * @note
 *
 *  Now convert the RTC date to fields in struct tm format:
 *  Days: 1-31 match in both cases.
 *   Month: S5J is 1-12, struct tm is 0-11.
 *   Years: S5H is 00-99, struct tm is years since 1900.
 *
 */
int s5j_getalarm(uintptr_t base, struct tm *t)
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

int s5j_setalarm(uintptr_t base, struct tm *t)
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

/**
 *
 * @brief    This function is called when allocation  calls RDTIME IOCTL. It returns the RTC time.
 * @param    struct rtc_lowerhalf_s *lower : handle for RTC driver
 * @param    struct rtc_time *rtctime *lower : Return address pointer for RTC driver
 * @return   Current time of this system
 */

#ifdef CONFIG_RTC_DRIVER
static int s5j_rtc_rdtime(FAR struct rtc_lowerhalf_s *lower, FAR struct rtc_time *rtctime)
{
	return s5j_gettime(S5J_RTC_BASE, (FAR struct tm *)rtctime);
}
#endif

#ifdef CONFIG_RTC_ALARM
/**
 * @brief    This function is called when allocation  calls ALM_READ. It returns the RTC alarm time.
 * @param    struct rtc_lowerhalf_s *lower : handle for RTC driver
 * @param    struct rtc_time *almtime : Return address pointer for RTC driver
 * @return   Current alarm time of this system
 *
 */

static int s5j_rtc_rdalarm(FAR struct rtc_lowerhalf_s *lower, FAR struct rtc_time *almtime)
{
	return s5j_getalarm(S5J_RTC_BASE, (FAR struct tm *)almtime);
}

/**
 * @brief    This function is called when allocation  calls ALM_SET. It returns the RTC alarm time.
 * @param    struct rtc_lowerhalf_s *lower : handle for RTC driver
 * @param    struct rtc_time *almtime : Return address pointer for RTC driver
 * @return   Current alarm time of this system
 *
 */
static int s5j_rtc_setalarm(FAR struct rtc_lowerhalf_s *lower, FAR const struct rtc_time *almtime)
{
	return s5j_setalarm(S5J_RTC_BASE, (FAR struct tm *)almtime);
}

static int s5j_rtc_aiealarm(FAR struct rtc_lowerhalf_s *lower, bool enable)
{
	if (enable) {
		irq_attach(IRQ_TOP_RTC_ALARM, (xcpt_t) __isr_rtc_alarm);
		up_enable_irq(IRQ_TOP_RTC_ALARM);

		HW_REG32(S5J_RTC_BASE, RTCALM) &= ~(0x1 << 6);
		HW_REG32(S5J_RTC_BASE, RTCALM) |= (0x1 << 6);	/* Enable Alarm Global */
	} else {
		/*
		 * irq_attach(IRQ_TOP_RTC_TIC, (xcpt_t)__isr_rtc_tick);
		 * up_enable_irq(IRQ_TOP_RTC_TIC);
		 */
		HW_REG32(S5J_RTC_BASE, RTCALM) &= ~(0x1 << 6);	/* Disable Alarm Global */
	}
	return 0;

	return 0;
}
#endif							/* CONFIG_RTC_ALARM */

#ifdef CONFIG_RTC_PERIODIC

static int s5j_rtc_set_tick_period(FAR struct rtc_lowerhalf_s *lower, FAR unsigned long irqpfreq)
{
	/* Set TICKCNT value for every 1 sec  */
	/*
	 * RTC Tick Interrupt Period
	 * Period = (n + 1) /(Tick clock source frequency) second. (n = tick counter value
	 * Refer to Tick Interrupt Resolution in S5JT200 UM
	 */
	HW_REG32(S5J_RTC_BASE, TICCNT) = 32768 * irqpfreq + 1;	/* in sec */
	return 0;
}

static int s5j_rtc_enable_tickirq(FAR struct rtc_lowerhalf_s *lower, bool enable)
{
	if (enable) {
		irq_attach(IRQ_TOP_RTC_TIC, (xcpt_t) __isr_rtc_tick);
		up_enable_irq(IRQ_TOP_RTC_TIC);

		HW_REG32(S5J_RTC_BASE, RTCCON) &= ~(0x1 << 8);
		HW_REG32(S5J_RTC_BASE, RTCCON) |= (0x1 << 8);	/* Enable Tick Timer */
	} else {
		/*
		 * irq_attach(IRQ_TOP_RTC_TIC, (xcpt_t)__isr_rtc_tick);
		 * up_enable_irq(IRQ_TOP_RTC_TIC);
		 */
		HW_REG32(S5J_RTC_BASE, RTCCON) &= ~(0x1 << 8);	/* disable Tick Timer */
	}
	return 0;
}
#endif

/**
 *
 * @brief    Initialize the hardware RTC per the selected configuration.  This function is
 *           called once during the OS initialization sequence
 * @return   == 0: Success
 *            < 0: Failure
 * @note     note here
 */

#ifdef CONFIG_RTC_DRIVER
static int s5j_rtc_settime(FAR struct rtc_lowerhalf_s *lower, FAR const struct rtc_time *rtctime)
{
	return s5j_settime(S5J_RTC_BASE, (FAR struct tm *)rtctime);
}
#endif

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/**
 *
 * @brief    Initialize the hardware RTC per the selected configuration.  This function is
 *           called once during the OS initialization sequence
 * @return   == 0: Success
 *            < 0: Failure
 * @note     note here
 */
int up_rtc_initialize(void)
{
	irqstate_t flags;
	struct tm t;

	/* No clocking setup need be performed. The Real-time Clock is continuously clocked
	 * at 32768 Hz (SCLK). The Power Management Controller has no effect on RTC behavior.
	 */

	flags = irqsave();

	/* Change MUX to RTC clock */
	HW_REG32(0x800A0000, 0x554) = 0x1;

	/* Reset RTC and Tick timer. */
	HW_REG32(S5J_RTC_BASE, RTCCON) = 0x0;
	/*
	 * Enable RTC counter, Tick clock 32768, Tick Timer enable
	 */
	HW_REG32(S5J_RTC_BASE, RTCCON) |= ((0x1 << 0) |	/* Enable RTC */
									   (0x0 << 4));	/* Set Clock Source 32.768Khz */
	/*(0x1 << 9)); // clock out selection */

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

	HW_REG32(S5J_RTC_BASE, RTCALM) = (/*0x1 << 6 | */ /* Enable Alarm Global */
										 0x1 << 5 |	/* Enable Year Alarm */
										 0x1 << 4 |	/* Enable Month Alarm */
										 0x1 << 3 |	/* Enable Day Alarm */
										 0x1 << 2 |	/* Enable Hour Alarm */
										 0x1 << 1 |	/* Enable Min Alarm */
										 0x1 << 0);	/* Enable Sec Alarm */
	/*
	 * Wakeup Source Setting
	 * Wakeup by TRTC_TICK, TRTC_ALARM
	 * HW_REG32(0x80090608, 0x0) = (0x1 << 4 | 0x1 << 3);
	 */
#endif

	irqrestore(flags);

	return OK;
}

/**
 *
 * @brief    Get the current date and time from the date/time RTC.  This interface
 *           is only supported by the date/time RTC hardware implementation.
 *           It is used to replace the system timer.  It is only used by the RTOS during
 *           initialization to set up the system time when CONFIG_RTC and CONFIG_RTC_DATETIME
 *           are selected (and CONFIG_RTC_HIRES is not).
 * @param    struct tm *tp : The location to return the high resolution time value.
 * @return   == 0: Success
 *            < 0: Failure
 * @note     Some date/time RTC hardware is capability of sub-second accuracy.  That
 *           sub-second accuracy is lost in this interface.  However, since the system time
 *           is reinitialized on each power-up/reset, there will be no timing inaccuracy in
 *           the long run.
 */
int up_rtc_getdatetime(FAR struct tm *tp)
{
	return s5j_gettime(S5J_RTC_BASE, tp);
}

/**
 *
 * @brief    Get the current time in seconds.  This is similar to the standard time()
 *           function.  This interface is only required if the low-resolution RTC/counter
 *           hardware implementation selected.  It is only used by the RTOS during
 *           initialization to set up the system time when CONFIG_RTC is set but neither
 *           CONFIG_RTC_HIRES nor CONFIG_RTC_DATETIME are set.
 * @return   The current time in seconds
 * @note     note here
 */

#ifndef CONFIG_RTC_HIRES
time_t up_rtc_time(void)
{
	struct tm t;

	s5j_gettime(S5J_RTC_BASE, &t);

	return mktime(&t);
}
#endif

/**
 *
 * @brief    Get the current time from the high resolution RTC clock/counter.  This interface
 *           is only supported by the high-resolution RTC/counter hardware implementation.
 *           It is used to replace the system timer.
 * @param    struct timespec *tp : The location to return the high resolution time value.
 * @return   == 0: Success
 *            < 0: Failure
 * @note     note here
 */
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

/**
 *
 * @brief    Set the RTC to the provided time.  All RTC implementations must be able to
 *           set their time based on a standard timespec.
 * @param    struct timespec *tp : the time to use
 * @return   == 0: Success
 *            < 0: Failure
 * @note     note here
 */

int up_rtc_settime(FAR const struct timespec *tp)
{
	FAR struct tm newtime;

	(void)gmtime_r(&tp->tv_sec, &newtime);

	return s5j_settime(S5J_RTC_BASE, &newtime);

}

/**
 *
 * @brief    Initialize RTC driver
 * @param    uintptr_t base : S5JXXXX RTC base address
 * @param    int irqno : S5JXXXX RTC IRQ number
 * @return   == 0: Fail to allocate memory for RTC driver
 *            > 0: Driver initialization is success.
 */

#ifdef CONFIG_RTC_DRIVER
int s5j_rtc_initialize(uintptr_t base, int irqno)
{
	cal_clk_enable(gate_top_rtc);
	up_rtc_initialize();
	return 0;
}
#endif

int up_rtcinitialize(void)
{
#ifdef CONFIG_S5J_RTC
	int ret;
	s5j_rtc_initialize(S5J_RTC_BASE, IRQ_TOP_RTC_TIC);
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
	return 0;
}

#ifdef CONFIG_SIDK_S5JT200_AUTOTEST
int s5j_rtc_pmu_wakeup(int msec)
{
	HW_REG32(S5J_RTC_BASE, TICCNT) = (32768 / 1000) * msec + 1;	/* in sec */
	irq_attach(IRQ_TOP_RTC_TIC, (xcpt_t) __isr_rtc_tick);
	up_enable_irq(IRQ_TOP_RTC_TIC);
	HW_REG32(S5J_RTC_BASE, RTCCON) &= ~(0x1 << 8);
	HW_REG32(S5J_RTC_BASE, RTCCON) |= (0x1 << 8);	/* Enable Tick Timer */

	HW_REG32(PMU_BASE, 0x81c) = 0xFCBA;	//check PMU dstop wokeup
	return 0;
}
#endif
#endif
