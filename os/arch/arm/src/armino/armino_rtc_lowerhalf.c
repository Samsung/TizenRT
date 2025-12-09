/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#include "aon_rtc.h"
#include "armino_rtc.h"

#ifdef CONFIG_RTC_DRIVER

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* This is the private type for the RTC state.  It must be cast compatible
 * with struct rtc_lowerhalf_s.
 */

struct armino_lowerhalf_s {
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

static int armino_rdtime(FAR struct rtc_lowerhalf_s *lower, FAR struct rtc_time *rtctime);
static int armino_settime(FAR struct rtc_lowerhalf_s *lower, FAR const struct rtc_time *rtctime);

#ifdef CONFIG_RTC_ALARM
static int armino_setalarm(FAR struct rtc_lowerhalf_s *lower, FAR const struct lower_setalarm_s *alarminfo);
static int armino_setrelative(FAR struct rtc_lowerhalf_s *lower, FAR const struct lower_setrelative_s *alarminfo);
static int armino_cancelalarm(FAR struct rtc_lowerhalf_s *lower, int alarmid);
static void armino_alarm_callback(aon_rtc_id_t id, uint8_t *name_p, void *param);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* ARMINO RTC driver operations */

static const struct rtc_ops_s g_rtc_ops = {
	.rdtime = armino_rdtime,
	.settime = armino_settime,
#ifdef CONFIG_RTC_ALARM
	.setalarm = armino_setalarm,
	.setrelative = armino_setrelative,
	.cancelalarm = armino_cancelalarm,
#endif
#ifdef CONFIG_RTC_IOCTL
	.ioctl = NULL,
#endif
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
	.destroy = NULL,
#endif
};

/* ARMINO RTC device state */

static struct armino_lowerhalf_s g_rtc_lowerhalf = {
	.ops = &g_rtc_ops,
};


/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: armino_alarm_callback
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
static void armino_alarm_callback(aon_rtc_id_t id, uint8_t *name_p, void *param)
{
	FAR struct armino_lowerhalf_s *rtc = &g_rtc_lowerhalf;

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
 * Name: armino_rdtime
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

static int armino_rdtime(FAR struct rtc_lowerhalf_s *lower, FAR struct rtc_time *rtctime)
{
	FAR struct armino_lowerhalf_s *rtc;
	time_t sec;
	int ret;
	
	DEBUGASSERT(lower != NULL && rtctime != NULL);
	rtc = (FAR struct armino_lowerhalf_s *)lower;
	ret = sem_wait(&rtc->devsem);
	if (ret < 0) {
		return ret;
	}
	/* The resolution of time is only 1 second */

	sec = up_rtc_time();

    if (gmtime_r(&sec, (FAR struct tm *)rtctime) == NULL) {
        sem_post(&rtc->devsem);
        return -EINVAL;
    }

    sem_post(&rtc->devsem);


	return OK;
}

/****************************************************************************
 * Name: armino_settime
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

static int armino_settime(FAR struct rtc_lowerhalf_s *lower, FAR const struct rtc_time *rtctime)
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
 * Name: armino_setalarm
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
static alarm_info_t s_armino_alarm = {0};

static int armino_setalarm(FAR struct rtc_lowerhalf_s *lower, FAR const struct lower_setalarm_s *alarminfo)
{
	FAR struct armino_lowerhalf_s *rtc;
	time_t alarm_time;
	time_t diff_time;
	uint32_t clock_freq;
	rtc_tick_t rel_ticks;
	int ret;

	DEBUGASSERT(lower != NULL && alarminfo != NULL && alarminfo->id == 0);
	rtc = (FAR struct armino_lowerhalf_s *)lower;

	/* Get exclusive access to the alarm */
	ret = sem_wait(&rtc->devsem);
	if (ret < 0) {
		return ret;
	}

	alarm_time = mktime((FAR struct tm *)&alarminfo->time);
	time_t current_time = up_rtc_time();

    if (alarm_time < current_time) {
        sem_post(&rtc->devsem);
        return -EINVAL;
    }

	diff_time = alarm_time - current_time;
	clock_freq = bk_rtc_get_clock_freq();
	rel_ticks = diff_time * clock_freq;

	snprintf(s_armino_alarm.name, sizeof(s_armino_alarm.name), "alarm%d", alarminfo->id);
	s_armino_alarm.period_tick = rel_ticks;
	s_armino_alarm.period_cnt = 1;	
	s_armino_alarm.callback = armino_alarm_callback;

	//TizenRT priv
	rtc->cb = alarminfo->cb;
	rtc->priv = alarminfo->priv;

	/* And set the alarm */
	ret = bk_alarm_register(AON_RTC_ID_1, &s_armino_alarm);
	if (ret < 0) {
		rtc->cb = NULL;
		rtc->priv = NULL;
	}

	sem_post(&rtc->devsem);
	return ret;
}
#endif

/****************************************************************************
 * Name: armino_setrelative
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
static int armino_setrelative(FAR struct rtc_lowerhalf_s *lower, FAR const struct lower_setrelative_s *alarminfo)
{
	FAR struct armino_lowerhalf_s *rtc;
	int ret;

	DEBUGASSERT(lower != NULL && alarminfo != NULL && alarminfo->id == 0);
	rtc = (FAR struct armino_lowerhalf_s *)lower;

	/* Get exclusive access to the alarm */

	ret = sem_wait(&rtc->devsem);
	if (ret < 0) {
		return ret;
	}

	snprintf(s_armino_alarm.name, sizeof(s_armino_alarm.name), "alarm%d", alarminfo->id);

	rtc_tick_t rel_ticks = alarminfo->reltime * bk_rtc_get_clock_freq();
	s_armino_alarm.period_tick = rel_ticks;
	s_armino_alarm.period_cnt = 1;	
	s_armino_alarm.callback = armino_alarm_callback;

	rtc->cb = alarminfo->cb;
	rtc->priv = alarminfo->priv;

	/* And set the alarm */
	ret = bk_alarm_register(AON_RTC_ID_1, &s_armino_alarm);
	if (ret < 0) {
		rtc->cb = NULL;
		rtc->priv = NULL;
	}

	sem_post(&rtc->devsem);
	return ret;
}
#endif

/****************************************************************************
 * Name: armino_cancelalarm
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
static int armino_cancelalarm(FAR struct rtc_lowerhalf_s *lower, int alarmid)
{
	FAR struct armino_lowerhalf_s *rtc = (FAR struct armino_lowerhalf_s *)lower;
	int ret;

	DEBUGASSERT(lower != NULL);

	ret = sem_wait(&rtc->devsem);
	if (ret < 0) {
		return ret;
	}

	rtc->cb = NULL;
	rtc->priv = NULL;

	ret = bk_alarm_unregister(AON_RTC_ID_1, s_armino_alarm.name);
	if (ret < 0) {
		sem_post(&rtc->devsem);
		return ret;
	}

	sem_post(&rtc->devsem);
	return ret;

}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: armino_rtc_lowerhalf
 *
 * Description:
 *   Instantiate the RTC lower half driver for the ARMINO.  General usage:
 *
 *     #include <tinyara/timers/rtc.h>
 *
 *     struct rtc_lowerhalf_s *lower;
 *     lower = armino_rtc_lowerhalf();
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

FAR struct rtc_lowerhalf_s *armino_rtc_lowerhalf(void)
{
	sem_init(&g_rtc_lowerhalf.devsem, 0, 1);

	return (FAR struct rtc_lowerhalf_s *)&g_rtc_lowerhalf;
}

#endif							/* CONFIG_RTC_DRIVER */
