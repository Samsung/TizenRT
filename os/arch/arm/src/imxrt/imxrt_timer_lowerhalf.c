/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/imxrt/imxrt_timer_lowerhalf.c
 *
 *   Copyright (C) 2013-2014 Gregory Nutt. All rights reserved.
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
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/timer.h>
#include <tinyara/irq.h>

#include "chip.h"
#include "imxrt_gpt.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct imxrt_gpt_lowerhalf_s {
	const struct timer_ops_s    *ops;       /* Lowerhalf operations */
	struct imxrt_gpt_chipinfo_s *gpt;
	gpt_config_t                 config;
	bool                         started;   /* True: Timer has been started */
	tccb_t                       callback;
	void                        *arg;       /* Argument passed to upper half callback */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int imxrt_timer_handler(int irq, void *context, void *arg);

static int imxrt_gpt_start(struct timer_lowerhalf_s *lower);
static int imxrt_gpt_stop(struct timer_lowerhalf_s *lower);
static int imxrt_gpt_getstatus(struct timer_lowerhalf_s *lower,
							struct timer_status_s *status);
static int imxrt_gpt_settimeout(struct timer_lowerhalf_s *lower,
								uint32_t timeout);
static void imxrt_gpt_setcallback(struct timer_lowerhalf_s *lower,
								CODE tccb_t callback, void *arg);
static int imxrt_gpt_ioctl(struct timer_lowerhalf_s *lower, int cmd,
						unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct timer_ops_s g_timer_ops = {
	.start       = imxrt_gpt_start,
	.stop        = imxrt_gpt_stop,
	.getstatus   = imxrt_gpt_getstatus,
	.settimeout  = imxrt_gpt_settimeout,
	.setcallback = imxrt_gpt_setcallback,
	.ioctl       = imxrt_gpt_ioctl,
};

static struct imxrt_gpt_lowerhalf_s g_gpt0_lowerhalf = {
	.ops         = &g_timer_ops,
};

static struct imxrt_gpt_lowerhalf_s g_gpt1_lowerhalf = {
	.ops         = &g_timer_ops,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: imxrt_gpt_handler
 *
 * Description:
 *   timer interrupt handler
 *
 * Input Parameters:
 *
 * Returned Values:
 *
 ****************************************************************************/
static int imxrt_gpt_handler(int irq, void *context, void *arg)
{
	struct imxrt_gpt_lowerhalf_s *priv = (struct imxrt_gpt_lowerhalf_s *)arg;
	struct imxrt_gpt_chipinfo_s *gpt = priv->gpt;
	uint32_t next_interval_us = 0;
	uint32_t ticks;

	imxrt_gpt_clearstatusflags(gpt->base, kGPT_OutputCompare1Flag);

	if (priv->callback(&next_interval_us, priv->arg)) {
		if (next_interval_us > 0) {
			ticks = imxrt_gpt_convert_time_tick(TIME2TICK, priv->config.clockSource, next_interval_us);
			imxrt_gpt_setoutputcomarevalue(gpt->base, kGPT_OutputCompare_Channel1, ticks);
		}
	} else {
		imxrt_gpt_stop((struct timer_lowerhalf_s *)priv);
	}

	return OK;
}

/****************************************************************************
 * Name: imxrt_gpt_start
 *
 * Description:
 *   Start the timer, resetting the time to the current timeout,
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int imxrt_gpt_start(struct timer_lowerhalf_s *lower)
{
	struct imxrt_gpt_lowerhalf_s *priv = (struct imxrt_gpt_lowerhalf_s *)lower;

	if (!priv->started) {
		if (priv->callback != NULL) {
			imxrt_gpt_setisr(priv->gpt, imxrt_gpt_handler, priv);
			imxrt_gpt_enableinterrupts(priv->gpt->base, kGPT_OutputCompare1InterruptEnable);
		}

		imxrt_gpt_starttimer(priv->gpt->base);
		priv->started = true;

		tmrvdbg("Timer %d is started, callback %s\n", priv->gpt->base == GPT1 ? 0 : 1, priv->callback == NULL ? "none" : "set");
		return OK;
	}

	/* Return EBUSY to indicate that the timer was already running */

	tmrdbg("Error!! Timer %d was already running\n", priv->gpt->base == GPT1 ? 0 : 1);
	return -EBUSY;
}

/****************************************************************************
 * Name: imxrt_gpt_stop
 *
 * Description:
 *   Stop the timer
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int imxrt_gpt_stop(struct timer_lowerhalf_s *lower)
{
	struct imxrt_gpt_lowerhalf_s *priv = (struct imxrt_gpt_lowerhalf_s *)lower;

	if (priv->started) {
		imxrt_gpt_disableinterrupts(priv->gpt->base, kGPT_OutputCompare1InterruptEnable);
		imxrt_gpt_stoptimer(priv->gpt->base);
		imxrt_gpt_setisr(priv->gpt, NULL, NULL);
		priv->started = false;

		tmrvdbg("Timer %d is stopped\n", priv->gpt->base == GPT1 ? 0 : 1);
		return OK;
	}

	/* Return ENODEV to indicate that the timer was not running */

	tmrdbg("Error!! Timer %d is not running\n", priv->gpt->base == GPT1 ? 0 : 1);
	return -ENODEV;
}

/****************************************************************************
 * Name: imxrt_gpt_getstatus
 *
 * Description:
 *   get GPT status
 *
 * Input Parameters:
 *   lower   - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *   status  - A pointer saved current GPT status
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int imxrt_gpt_getstatus(struct timer_lowerhalf_s *lower,
							struct timer_status_s *status)
{
	struct imxrt_gpt_lowerhalf_s *priv = (struct imxrt_gpt_lowerhalf_s *)lower;
	uint32_t ticks;

	/* Return the status bit */

	status->flags = 0;
	if (priv->started) {
		status->flags |= TCFLAGS_ACTIVE;
	}

	if (priv->callback) {
		status->flags |= TCFLAGS_HANDLER;
	}

	/* Return the actual timeout in microseconds */

	ticks = imxrt_gpt_getoutputcomparevalue(priv->gpt->base, kGPT_OutputCompare_Channel1);
	status->timeout = imxrt_gpt_convert_time_tick(TICK2TIME, priv->config.clockSource, ticks);

	/* Get the time remaining until the timer expires (in microseconds). */

	ticks = imxrt_gpt_getcurrenttimercount(priv->gpt->base);
	status->timeleft = imxrt_gpt_convert_time_tick(TICK2TIME, priv->config.clockSource, ticks);

	return OK;
}

/****************************************************************************
 * Name: imxrt_gpt_settimeout
 *
 * Description:
 *   Set a new timeout value (and reset the timer)
 *
 * Input Parameters:
 *   lower   - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *   timeout - The new timeout value in microseconds.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int imxrt_gpt_settimeout(struct timer_lowerhalf_s *lower, uint32_t timeout)
{
	struct imxrt_gpt_lowerhalf_s *priv = (struct imxrt_gpt_lowerhalf_s *)lower;
	uint32_t ticks;

	if (priv->started) {
		return -EPERM;
	}

	ticks = imxrt_gpt_convert_time_tick(TIME2TICK, priv->config.clockSource, timeout);
	imxrt_gpt_setoutputcomarevalue(priv->gpt->base, kGPT_OutputCompare_Channel1, ticks);

	return OK;
}

/****************************************************************************
 * Name: imxrt_gpt_setcallback
 *
 * Description:
 *   Call this user provided timeout callback.
 *
 * Input Parameters:
 *   lower    - A pointer the publicly visible representation of the
 *              "lower-half" driver state structure.
 *   callback - The new timer expiration function pointer. If this
 *              function pointer is NULL, then the reset-on-expiration
 *              behavior is restored,
 *  arg       - Argument that will be provided in the callback
 *
 * Returned Values:
 *   The previous timer expiration function pointer or NULL is there was
 *   no previous function pointer.
 *
 ****************************************************************************/
static void imxrt_gpt_setcallback(struct timer_lowerhalf_s *lower,
								tccb_t callback, void *arg)
{
	struct imxrt_gpt_lowerhalf_s *priv = (struct imxrt_gpt_lowerhalf_s *)lower;
	irqstate_t flags = irqsave();

	/* Save the new callback */
	priv->callback = callback;
	priv->arg      = arg;

	if (callback != NULL && priv->started) {
		imxrt_gpt_setisr(priv->gpt, imxrt_gpt_handler, priv);
		imxrt_gpt_enableinterrupts(priv->gpt->base, kGPT_OutputCompare1InterruptEnable);
	} else {
		imxrt_gpt_disableinterrupts(priv->gpt->base, kGPT_OutputCompare1InterruptEnable);
		imxrt_gpt_setisr(priv->gpt, NULL, NULL);
	}

	irqrestore(flags);
}

/****************************************************************************
 * Name: imxrt_gpt_ioctl
 *
 * Description:
 *   Any ioctl commands that are not recognized by the "upper-half" driver
 *   are forwarded to the lower half driver through this method.
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *   cmd   - The ioctl command value
 *   arg   - The optional argument that accompanies the 'cmd'.  The
 *           interpretation of this argument depends on the particular
 *           command.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int imxrt_gpt_ioctl(struct timer_lowerhalf_s *lower, int cmd,
						unsigned long arg)
{
	struct imxrt_gpt_lowerhalf_s *priv = (struct imxrt_gpt_lowerhalf_s *)lower;
	int ret = ERROR;
	gpt_clock_source_t clock_source = priv->config.clockSource;

	tmrvdbg("GPT IOCTL cmd: %d, arg: %ld\n", cmd, arg);

	switch (cmd) {
	case TCIOC_SETDIV:
		if (clock_source == kGPT_ClockSource_Osc) {
			if (arg <= GPT_OSCDIV_MAXVAL) {
				imxrt_gpt_setoscclockdivider(priv->gpt->base, arg);
				priv->config.divider = arg;
				ret = OK;
			} else {
				tmrdbg("Invalid Value of OSC Divider (%d)\n", arg);
			}
		} else {
			if (arg <= GPT_DIV_MAXVAL) {
				imxrt_gpt_setclockdivider(priv->gpt->base, arg);
				priv->config.divider = arg;
				ret = OK;
			} else {
				tmrdbg("Invalid Value of Divider (%d)\n", arg);
			}
		}
		break;
	case TCIOC_GETDIV:
		if (clock_source == kGPT_ClockSource_Osc) {
			ret = (int)imxrt_gpt_getoscclockdivider(priv->gpt->base);
		} else {
			ret = (int)imxrt_gpt_getclockdivider(priv->gpt->base);
		}
		break;
	default:
		tmrdbg("Invalid cmd %d\n", cmd);
		break;
	}

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int imxrt_timer_initialize(const char *devpath, int timer)
{
	struct imxrt_gpt_lowerhalf_s *lower = NULL;
	gpt_config_t config;

	switch (timer) {
	case IMXRT_GPT_CH0:
		lower = &g_gpt0_lowerhalf;
		break;

	case IMXRT_GPT_CH1:
		lower = &g_gpt1_lowerhalf;
		break;
	}

	if (!lower) {
		return -ENODEV;
	}

	/* Initialize the elements of lower half state structure */
	lower->started  = false;
	lower->callback = NULL;

	imxrt_gpt_getdefaultconfig(&config);
	memcpy(&(lower->config), &config, sizeof(gpt_config_t));
	lower->gpt = imxrt_gpt_configure(timer, &config);

	if (lower->gpt == NULL) {
		return -EINVAL;
	}

	/*
	 * Register the timer driver as /dev/timerX.  The returned value from
	 * timer_register is a handle that could be used with timer_unregister().
	 * REVISIT: The returned handle is discard here.
	 */
	if (!timer_register(devpath, (struct timer_lowerhalf_s *)lower)) {
		/*
		 * The actual cause of the failure may have been a failure to allocate
		 * perhaps a failure to register the timer driver (such as if the
		 * 'depath' were not unique).  We know here but we return EEXIST to
		 * indicate the failure (implying the non-unique devpath).
		 */
		return -EEXIST;
	}

	return OK;
}
