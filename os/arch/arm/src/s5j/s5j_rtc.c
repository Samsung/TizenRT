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

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include "up_arch.h"

#include "s5j_rtc.h"

#ifdef CONFIG_RTC
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#if !defined(CONFIG_RTC_DATETIME)
#error "S5J does not support other than CONFIG_RTC_DATETIME."
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/
#ifdef CONFIG_RTC_ALARM
/* Callback to use when the alarm expires */
static alarmcb_t g_alarmcb;
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/
/* g_rtc_enabled is set true after the RTC has successfully initialized */
volatile bool g_rtc_enabled = false;

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct rtc_regvals_s {
	uint8_t bcdsec;
	uint8_t bcdmin;
	uint8_t bcdhour;
	uint8_t bcdday;
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	uint8_t bcddayweek;
#endif
	uint8_t bcdmon;
	uint16_t bcdyear;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static uint32_t rtc_bin2bcd(int value)
{
	uint32_t hundreds;
	uint32_t tens;

	hundreds = tens = 0;

	while (value >= 100) {
		hundreds++;
		value -= 100;
	}

	while (value >= 10) {
		tens++;
		value -= 10;
	}

	return (hundreds << 8) | (tens << 4) | value;
}

static int rtc_bcd2bin(uint32_t value)
{
	uint32_t tens = ((value >> 4) & 0xf) * 10;
	uint32_t hundreds = ((value >> 8) & 0xf) * 100;
	return (int)(hundreds + tens + (value & 0xf));
}

static void rtc_breakout(FAR const struct tm *tm,
		FAR struct rtc_regvals_s *regvals)
{
	regvals->bcdsec     = rtc_bin2bcd(tm->tm_sec);
	regvals->bcdmin     = rtc_bin2bcd(tm->tm_min);
	regvals->bcdhour    = rtc_bin2bcd(tm->tm_hour);
	regvals->bcdday     = rtc_bin2bcd(tm->tm_mday);
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	regvals->bcddayweek = rtc_bin2bcd(tm->tm_wday + 1);
#endif
	regvals->bcdmon     = rtc_bin2bcd(tm->tm_mon + 1);
	regvals->bcdyear    = rtc_bin2bcd(tm->tm_year);
}

static void rtc_wprunlock(void)
{
	modifyreg32(S5J_RTC_RTCCON, 0, RTC_RTCCON_CTLEN_ENABLE);
}

static void rtc_wprlock(void)
{
	modifyreg32(S5J_RTC_RTCCON, RTC_RTCCON_CTLEN_ENABLE, 0);
}

#if defined(CONFIG_RTC_ALARM)
static int rtc_alarm_handler(int irq, void *context, FAR void *arg)
{
	if (getreg32(S5J_RTC_INTP) & RTC_INTP_ALARM) {
		/* Clear pending flags */
		putreg32(RTC_INTP_ALARM, S5J_RTC_INTP);

		/* Disable alarm */
		modifyreg32(S5J_RTC_RTCALM, RTC_RTCALM_ALMEN_MASK,
						RTC_RTCALM_ALMEN_DISABLE);

		/* Alarm callback */
		g_alarmcb();
		g_alarmcb = NULL;
	}

	return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: up_rtc_getdatetime
 *
 * Description:
 *   Get the current date and time from the date/time RTC. This interface
 *   is only supported by the date/time RTC hardware implementation.
 *   It is used to replace the system timer. It is only used by the RTOS
 *   during initialization to set up the system time when CONFIG_RTC and
 *   CONFIG_RTC_DATETIME are selected (and CONFIG_RTC_HIRES is not).
 *
 *   NOTE: Some date/time RTC hardware is capability of sub-second accuracy.
 *   That sub-second accuracy is lost in this interface. However, since the
 *   system time is reinitialized on each power-up/reset, there will be no
 *   timing inaccuracy in the long run.
 *
 * Input Parameters:
 *   tp - The location to return the high resolution time value.
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/
int up_rtc_getdatetime(FAR struct tm *tp)
{
	irqstate_t flags;
	struct rtc_regvals_s regvals;

	flags = irqsave();

	/* read bcd counters */
	do {
		regvals.bcdsec     = getreg32(S5J_RTC_BCDSEC);
		regvals.bcdmin     = getreg32(S5J_RTC_BCDMIN);
		regvals.bcdhour    = getreg32(S5J_RTC_BCDHOUR);
		regvals.bcdday     = getreg32(S5J_RTC_BCDDAY);
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
		regvals.bcddayweek = getreg32(S5J_RTC_BCDDAYWEEK);
#endif
		regvals.bcdmon     = getreg32(S5J_RTC_BCDMON);
		regvals.bcdyear    = getreg32(S5J_RTC_BCDYEAR);
	} while (regvals.bcdsec != getreg32(S5J_RTC_BCDSEC));

	irqrestore(flags);

	tp->tm_sec  = rtc_bcd2bin(regvals.bcdsec);
	tp->tm_min  = rtc_bcd2bin(regvals.bcdmin);
	tp->tm_hour = rtc_bcd2bin(regvals.bcdhour);
	tp->tm_mday = rtc_bcd2bin(regvals.bcdday);
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	tp->tm_wday = rtc_bcd2bin(regvals.bcddayweek) - 1;
#endif
	tp->tm_mon  = rtc_bcd2bin(regvals.bcdmon) - 1;
	tp->tm_year = rtc_bcd2bin(regvals.bcdyear);

	return OK;
}

/****************************************************************************
 * Name: up_rtc_setdatetime
 *
 * Description:
 *   Set the RTC to the provided time. RTC implementations which provide
 *   up_rtc_getdatetime() (CONFIG_RTC_DATETIME is selected) should provide
 *   this function.
 *
 * Input Parameters:
 *   tp - the time to use
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/
int up_rtc_setdatetime(FAR struct tm *tm)
{
	irqstate_t flags;
	struct rtc_regvals_s regvals;

	rtc_breakout(tm, &regvals);

	flags = irqsave();

	rtc_wprunlock();

	/* update BCD counters */
	putreg32(regvals.bcdsec, S5J_RTC_BCDSEC);
	putreg32(regvals.bcdmin, S5J_RTC_BCDMIN);
	putreg32(regvals.bcdhour, S5J_RTC_BCDHOUR);
	putreg32(regvals.bcdday, S5J_RTC_BCDDAY);
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
	putreg32(regvals.bcddayweek, S5J_RTC_BCDDAYWEEK);
#endif
	putreg32(regvals.bcdmon, S5J_RTC_BCDMON);
	putreg32(regvals.bcdyear, S5J_RTC_BCDYEAR);

	rtc_wprlock();

	irqrestore(flags);

	return OK;
}

/****************************************************************************
 * Name: up_rtc_settime
 *
 * Description:
 *   Set the RTC to the provided time.  All RTC implementations must be able
 *   to set their time based on a standard timespec.
 *
 * Input Parameters:
 *   tp - the time to use
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/
int up_rtc_settime(FAR const struct timespec *tp)
{
	struct tm t;

	/* convert timepsec to tm */
	gmtime_r(&tp->tv_sec, &t);

	return up_rtc_setdatetime(&t);
}

#ifdef CONFIG_RTC_ALARM
/****************************************************************************
 * Name: s5j_rtc_setalarm
 *
 * Description:
 *   Set up an alarm
 *
 * Input Parameters:
 *   tp       - the time to set the alarm
 *   callback - the function to call when the alarm expires
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno on failure
 *
 ****************************************************************************/
int s5j_rtc_setalarm(FAR const struct timespec *tp, alarmcb_t callback)
{
	struct tm t;
	struct rtc_regvals_s regvals;
	irqstate_t flags;

	/* convert timepsec to tm */
	gmtime_r(&tp->tv_sec, &t);

	rtc_breakout(&t, &regvals);

	flags = irqsave();

	if (g_alarmcb != NULL) {
		irqrestore(flags);
		return -EBUSY;
	}

	/* Set the alarm */
	putreg32(regvals.bcdsec,  S5J_RTC_ALMSEC);
	putreg32(regvals.bcdmin,  S5J_RTC_ALMMIN);
	putreg32(regvals.bcdhour, S5J_RTC_ALMHOUR);
	putreg32(regvals.bcdday,  S5J_RTC_ALMDAY);
	putreg32(regvals.bcdmon,  S5J_RTC_ALMMON);
	putreg32(regvals.bcdyear, S5J_RTC_ALMYEAR);

	/* Enable RTC alarm */
	putreg32(RTC_RTCALM_ALMEN_ENABLE | RTC_RTCALM_YEAREN_ENABLE |
		 RTC_RTCALM_MONEN_ENABLE | RTC_RTCALM_DAYEN_ENABLE |
		 RTC_RTCALM_HOUREN_ENABLE | RTC_RTCALM_MINEN_ENABLE |
		 RTC_RTCALM_SECEN_ENABLE,
		 S5J_RTC_RTCALM);

	/* save the callback function pointer */
	g_alarmcb = callback;
	irq_attach(IRQ_TOP_RTC_ALARM, rtc_alarm_handler, NULL);
	up_enable_irq(IRQ_TOP_RTC_ALARM);

	irqrestore(flags);

	return OK;
}

/****************************************************************************
 * Name: s5j_rtc_cancelalarm
 *
 * Description:
 *   Cancel a pending alarm
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno on failure
 *
 ****************************************************************************/
int s5j_rtc_cancelalarm(void)
{
	if (g_alarmcb == NULL)
		return -ENODATA;

	/* Disable alarm */
	modifyreg32(S5J_RTC_RTCALM, RTC_RTCALM_ALMEN_MASK,
						RTC_RTCALM_ALMEN_DISABLE);

	g_alarmcb = NULL;

	return OK;
}
#endif /* CONFIG_RTC_ALARM */

/****************************************************************************
 * Name: up_rtc_initialize
 *
 * Description:
 *   Initialize the hardware RTC per the selected configuration. This
 *   function is called once during the OS initialization sequence.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno on failure
 *
 ****************************************************************************/
int up_rtc_initialize(void)
{
	/* Disable RTC Alarm */
	putreg32(0, S5J_RTC_RTCALM);

	/* Clear interrupt pending (if any) */
	putreg32(RTC_INTP_ALARM | RTC_INTP_TIMETIC0, S5J_RTC_INTP);

	rtc_wprunlock();

	/* Reset to all initial state */
	modifyreg32(S5J_RTC_RTCCON,
			RTC_RTCCON_TICCKSEL0_MASK | RTC_RTCCON_CLKRST_MASK |
			RTC_RTCCON_CNTSEL_MASK | RTC_RTCCON_CLKSEL_MASK,
			RTC_RTCCON_TICCKSEL0_32768HZ |
			RTC_RTCCON_CLKRST_ENABLE |
			RTC_RTCCON_CNTSEL_MERGE_BCDCNT |
			RTC_RTCCON_CLKSEL_DIV32768);

	/* Fix invalid reset value of BCDDAY and BCDMON which is based 1. */
	if (getreg32(S5J_RTC_BCDDAY) == 0) {
		putreg32(1, S5J_RTC_BCDDAY);
	}

	if (getreg32(S5J_RTC_BCDMON) == 0) {
		putreg32(1, S5J_RTC_BCDMON);
	}

	/* OS supports to convert epoch only after 1970. Set year to 2010. */
	if (getreg32(S5J_RTC_BCDYEAR) == 0) {
		putreg32(0x110, S5J_RTC_BCDYEAR);
	}

	rtc_wprlock();

	g_rtc_enabled = true;

	return OK;
}
#endif /* CONFIG_RTC */
