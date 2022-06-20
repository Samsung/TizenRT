/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 *
 *   Copyright (C) 2021 Gregory Nutt. All rights reserved.
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

#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/rtc.h>

#include "up_arch.h"

#include "chip.h"
#include "amebalite_rtc.h"
#include "rtc_api.h"

#ifdef CONFIG_RTC_DRIVER

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* This is the private type for the RTC state.  It must be cast compatible
 * with struct rtc_lowerhalf_s.
 */

struct amebalite_lowerhalf_s {
	/* This is the contained reference to the read-only, lower-half
	 * operations vtable (which may lie in FLASH or ROM)
	 */

	FAR const struct rtc_ops_s *ops;

	/* Data following is private to this driver and not visible outside of
	 * this file.
	 */

	sem_t devsem;				/* Threads can only exclusively access the RTC */

#ifdef CONFIG_RTC_ALARM
	/* Alarm callback information */

	volatile rtc_alarm_callback_t cb;	/* Callback when the alarm expires */
	volatile FAR void *priv;	/* Private argument to accompany callback */
#endif
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Prototypes for static methods in struct rtc_ops_s */

static int amebalite_rdtime(FAR struct rtc_lowerhalf_s *lower, FAR struct rtc_time *rtctime);
static int amebalite_settime(FAR struct rtc_lowerhalf_s *lower, FAR const struct rtc_time *rtctime);

#ifdef CONFIG_RTC_ALARM
static int amebalite_setalarm(FAR struct rtc_lowerhalf_s *lower, FAR const struct lower_setalarm_s *alarminfo);
static int amebalite_setrelative(FAR struct rtc_lowerhalf_s *lower, FAR const struct lower_setrelative_s *alarminfo);
static int amebalite_cancelalarm(FAR struct rtc_lowerhalf_s *lower, int alarmid);
static alarm_irq_handler amebalite_alarm_callback(void);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* AMEBALITE RTC driver operations */

static const struct rtc_ops_s g_rtc_ops = {
	.rdtime = amebalite_rdtime,
	.settime = amebalite_settime,
#ifdef CONFIG_RTC_ALARM
	.setalarm = amebalite_setalarm,
	.setrelative = amebalite_setrelative,
	.cancelalarm = amebalite_cancelalarm,
#endif
#ifdef CONFIG_RTC_IOCTL
	.ioctl = NULL,
#endif
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
	.destroy = NULL,
#endif
};

/* AMEBALITE RTC device state */

static struct amebalite_lowerhalf_s g_rtc_lowerhalf = {
	.ops = &g_rtc_ops,
};

static alarm_t g_amebalite_alarm;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: amebalite_alarm_callback
 *
 * Description:
 *   This is the function that is called from the RTC driver when the alarm
 *   goes off.  It just invokes the upper half drivers callback.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_RTC_ALARM
static alarm_irq_handler amebalite_alarm_callback(void)
{
	FAR struct amebalite_lowerhalf_s *rtc = &g_rtc_lowerhalf;

	/* Sample and clear the callback information to minimize the window in
	 * time in which race conditions can occur.
	 */
	
	rtc_alarm_callback_t cb = (rtc_alarm_callback_t)rtc->cb;
	FAR void *arg = (FAR void *)rtc->priv;

	rtc->cb = NULL;
	rtc->priv = NULL;

	/* Perform the callback */

	if (cb != NULL) {
		cb(arg, 0);
	}
}
#endif							/* CONFIG_RTC_ALARM */

/****************************************************************************
 * Name: amebalite_rdtime
 *
 * Description:
 *   Implements the rdtime() method of the RTC driver interface
 *
 * Input Parameters:
 *   lower   - A reference to RTC lower half driver state structure
 *   rcttime - The location in which to return the current RTC time.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on any failure.
 *
 ****************************************************************************/

static int amebalite_rdtime(FAR struct rtc_lowerhalf_s *lower, FAR struct rtc_time *rtctime)
{
	FAR struct amebalite_lowerhalf_s *rtc;
	time_t timer;
	int ret;

	DEBUGASSERT(lower != NULL);
	rtc = (FAR struct amebalite_lowerhalf_s *)lower;
	sem_wait(&rtc->devsem);
	/* The resolution of time is only 1 second */

	timer = up_rtc_time();

	/* Convert the one second epoch time to a struct tm */

	ret = gmtime_r(&timer, (FAR struct tm *)rtctime);

	sem_post(&rtc->devsem);

	if (ret == 0) return -EOVERFLOW;

	return OK;
}

/****************************************************************************
 * Name: amebalite_settime
 *
 * Description:
 *   Implements the settime() method of the RTC driver interface
 *
 * Input Parameters:
 *   lower   - A reference to RTC lower half driver state structure
 *   rcttime - The new time to set
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on any failure.
 *
 ****************************************************************************/

static int amebalite_settime(FAR struct rtc_lowerhalf_s *lower, FAR const struct rtc_time *rtctime)
{
	struct timespec ts;

	/* Convert the struct rtc_time to a time_t.  Here we assume that struct
	 * rtc_time is cast compatible with struct tm.
	 */

	ts.tv_sec = mktime((FAR struct tm *)rtctime);
	ts.tv_nsec = 0;

	/* Now set the time (to one second accuracy) */

	return up_rtc_settime(&ts);
}

/****************************************************************************
 * Name: amebalite_setalarm
 *
 * Description:
 *   Set a new alarm.  This function implements the setalarm() method of the
 *   RTC driver interface
 *
 * Input Parameters:
 *   lower     - A reference to RTC lower half driver state structure
 *   alarminfo - Provided information needed to set the alarm
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on any failure.
 *
 ****************************************************************************/

#ifdef CONFIG_RTC_ALARM
static int amebalite_setalarm(FAR struct rtc_lowerhalf_s *lower, FAR const struct lower_setalarm_s *alarminfo)
{
	FAR struct amebalite_lowerhalf_s *rtc;
	int ret;

	DEBUGASSERT(lower != NULL && alarminfo != NULL && alarminfo->id == 0);
	rtc = (FAR struct amebalite_lowerhalf_s *)lower;

	/* Get exclusive access to the alarm */

	ret = sem_wait(&rtc->devsem);
	if (ret < 0) {
		return ret;
	}

	g_amebalite_alarm.hour = alarminfo->time.tm_hour;
	g_amebalite_alarm.yday = alarminfo->time.tm_yday;
	g_amebalite_alarm.min = alarminfo->time.tm_min;
	g_amebalite_alarm.sec = alarminfo->time.tm_sec;

	ret = -EINVAL;
	if (alarminfo->id == 0) {
		struct timespec ts;

		/* Convert the RTC time to a timespec (1 second accuracy) */

		ts.tv_sec = mktime((FAR struct tm *)&alarminfo->time);
		ts.tv_nsec = 0;

		/* Remember the callback information */

		rtc->cb = alarminfo->cb;
		rtc->priv = alarminfo->priv;

		/* And set the alarm */
		ret = rtc_set_alarm(&g_amebalite_alarm, (alarm_irq_handler)amebalite_alarm_callback);
		if (ret < 0) {
			rtc->cb = NULL;
			rtc->priv = NULL;
		}
	}

	sem_post(&rtc->devsem);
	return ret;
}
#endif

/****************************************************************************
 * Name: amebalite_setrelative
 *
 * Description:
 *   Set a new alarm relative to the current time.  This function implements
 *   the setrelative() method of the RTC driver interface
 *
 * Input Parameters:
 *   lower - A reference to RTC lower half driver state structure
 *   alarminfo - Provided information needed to set the alarm
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on any failure.
 *
 ****************************************************************************/

#ifdef CONFIG_RTC_ALARM
static int amebalite_setrelative(FAR struct rtc_lowerhalf_s *lower, FAR const struct lower_setrelative_s *alarminfo)
{
	FAR struct amebalite_lowerhalf_s *rtc;
	FAR struct timespec ts;
	int ret = -EINVAL;

	DEBUGASSERT(lower != NULL && alarminfo != NULL && alarminfo->id == 0);
	rtc = (FAR struct amebalite_lowerhalf_s *)lower;

	/* Get exclusive access to the alarm */

	ret = sem_wait(&rtc->devsem);
	if (ret < 0) {
		return ret;
	}

	ret = -EINVAL;
	if (alarminfo->id == 0 && alarminfo->reltime > 0) {
		/* Get the current time in seconds */
		/* The resolution of time is only 1 second */

		ts.tv_sec = up_rtc_time();
		ts.tv_nsec = 0;

		/* Add the seconds offset.  Add one to the number of seconds because
		 * we are unsure of the phase of the timer.
		 */

		ts.tv_sec += (alarminfo->reltime + 1);

		/* Remember the callback information */

		rtc->cb = alarminfo->cb;
		rtc->priv = alarminfo->priv;

		struct rtc_time l_rtctime;
		if (gmtime_r(&ts.tv_sec, (struct tm *)&l_rtctime) == 0) ret = -EOVERFLOW;

		/* And set the alarm */
		g_amebalite_alarm.hour = l_rtctime.tm_hour;
		g_amebalite_alarm.yday = l_rtctime.tm_yday;
		g_amebalite_alarm.min = l_rtctime.tm_min;
		g_amebalite_alarm.sec = l_rtctime.tm_sec;

		ret = rtc_set_alarm(&g_amebalite_alarm, (alarm_irq_handler)amebalite_alarm_callback);
		if (ret < 0) {
			rtc->cb = NULL;
			rtc->priv = NULL;
		}
	}

	sem_post(&rtc->devsem);
	return ret;
}
#endif

/****************************************************************************
 * Name: amebalite_cancelalarm
 *
 * Description:
 *   Cancel the current alarm.  This function implements the cancelalarm()
 *   method of the RTC driver interface
 *
 * Input Parameters:
 *   lower - A reference to RTC lower half driver state structure
 *   alarminfo - Provided information needed to set the alarm
 *
 * Returned Value:
 *   Zero (OK) is returned on success;
 *
 ****************************************************************************/

#ifdef CONFIG_RTC_ALARM
static int amebalite_cancelalarm(FAR struct rtc_lowerhalf_s *lower, int alarmid)
{
	/* We cancel the alarm by alarm by disabling the alarm and the alarm
	 * interrupt.
	 */
	rtc_disable_alarm();

	return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: amebalite_rtc_lowerhalf
 *
 * Description:
 *   Instantiate the RTC lower half driver for the AMEBALITE.  General usage:
 *
 *     #include <tinyara/timers/rtc.h>
 *
 *     struct rtc_lowerhalf_s *lower;
 *     lower = amebalite_rtc_lowerhalf();
 *     rtc_initialize(0, lower);
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   On success, a non-NULL RTC lower interface is returned.  NULL is
 *   returned on any failure.
 *
 ****************************************************************************/

FAR struct rtc_lowerhalf_s *amebalite_rtc_lowerhalf(void)
{
	sem_init(&g_rtc_lowerhalf.devsem, 0, 1);

	return (FAR struct rtc_lowerhalf_s *)&g_rtc_lowerhalf;
}

#endif							/* CONFIG_RTC_DRIVER */
