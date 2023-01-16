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
 *   Copyright (C) 2020 Gregory Nutt. All rights reserved.
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
#include "timer_api.h"

#ifdef CONFIG_TIMER

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct amebalite_gpt_lowerhalf_s {
	const struct timer_ops_s *ops;	/* Lowerhalf operations */
	gtimer_t obj;
	bool started;				/* True: Timer is started */
	tccb_t callback;
	void *arg;					/* Argument passed to upper half callback */
	uint32_t gpt_timeout;
	bool freerunmode;			/* True: Free Run Mode. False: Alarm Mode */
	uint32_t num_of_4294967ms_elapse;	/* Used to calculate total time elapse in Free Run Mode */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int amebalite_gpt_start(struct timer_lowerhalf_s *lower);
static int amebalite_gpt_stop(struct timer_lowerhalf_s *lower);
static int amebalite_gpt_getstatus(struct timer_lowerhalf_s *lower, struct timer_status_s *status);
static int amebalite_gpt_settimeout(struct timer_lowerhalf_s *lower, uint32_t timeout);
static void amebalite_gpt_setcallback(struct timer_lowerhalf_s *lower, CODE tccb_t callback, void *arg);
static int amebalite_gpt_ioctl(struct timer_lowerhalf_s *lower, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct timer_ops_s g_timer_ops = {
	.start = amebalite_gpt_start,
	.stop = amebalite_gpt_stop,
	.getstatus = amebalite_gpt_getstatus,
	.settimeout = amebalite_gpt_settimeout,
	.setcallback = amebalite_gpt_setcallback,
	.ioctl = amebalite_gpt_ioctl,
};

static struct amebalite_gpt_lowerhalf_s g_gpt0_lowerhalf = {
	.ops = &g_timer_ops,
};

static struct amebalite_gpt_lowerhalf_s g_gpt1_lowerhalf = {
	.ops = &g_timer_ops,
};

static struct amebalite_gpt_lowerhalf_s g_gpt2_lowerhalf = {
	.ops = &g_timer_ops,
};

static struct amebalite_gpt_lowerhalf_s g_gpt3_lowerhalf = {
	.ops = &g_timer_ops,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: amebalite_gpt_handler
 *
 * Description:
 *   timer interrupt handler
 *
 * Input Parameters:
 *
 * Returned Values:
 *
 ****************************************************************************/
static void amebalite_gpt_handler(uint32_t data)
{
	struct amebalite_gpt_lowerhalf_s *priv = (struct amebalite_gpt_lowerhalf_s *)data;
	uint32_t next_interval_us = 0;
	DEBUGASSERT(priv);
	if (!priv) {
		return;
	}

	if (priv->freerunmode) {	/* Free Run Mode */
		priv->num_of_4294967ms_elapse++;
	} else {					/* Alarm Mode */
		if (priv->callback(&next_interval_us, priv->arg)) {
			if (next_interval_us > 0) {
				priv->gpt_timeout = next_interval_us;
				gtimer_reload(&priv->obj, priv->gpt_timeout);
			}
		} else {
			amebalite_gpt_stop((struct timer_lowerhalf_s *)priv);
		}
	}
}

/****************************************************************************
 * Name: amebalite_gpt_start
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
static int amebalite_gpt_start(struct timer_lowerhalf_s *lower)
{
	struct amebalite_gpt_lowerhalf_s *priv = (struct amebalite_gpt_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	if (!priv->started) {
		if (priv->freerunmode) {	/* Free Run Mode */
			priv->obj.handler = (void *)amebalite_gpt_handler;
			priv->obj.hid = (uint32_t) priv;
			gtimer_start_periodical(&priv->obj, 0xFFFFFFFF, priv->obj.handler, priv->obj.hid);
		} else {				/* Alarm Mode */
			if (priv->callback != NULL) {
				priv->obj.handler = (void *)amebalite_gpt_handler;
				priv->obj.hid = (uint32_t) priv;
				gtimer_start_periodical(&priv->obj, priv->gpt_timeout, priv->obj.handler, priv->obj.hid);
			} else {
				gtimer_start_periodical(&priv->obj, priv->gpt_timeout, NULL, 0);
			}
		}
		priv->started = true;

		tmrvdbg("Timer %d is started, callback %s\n", priv->obj.timer_id, priv->callback == NULL ? "none" : "set");
		return OK;
	}

	/* Return EBUSY to indicate that the timer was already running */
	tmrdbg("Error!! Timer %d was already running\n", priv->obj.timer_id);
	return -EBUSY;
}

/****************************************************************************
 * Name: amebalite_gpt_stop
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
static int amebalite_gpt_stop(struct timer_lowerhalf_s *lower)
{
	struct amebalite_gpt_lowerhalf_s *priv = (struct amebalite_gpt_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	if (priv->started) {
		gtimer_deinit(&priv->obj);
		priv->started = false;

		tmrvdbg("Timer %d is stopped\n", priv->obj.timer_id);
		return OK;
	}

	/* Return ENODEV to indicate that the timer was not running */
	tmrdbg("Error!! Timer %d is not running\n", priv->obj.timer_id);
	return -ENODEV;
}

/****************************************************************************
 * Name: amebalite_gpt_getstatus
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
static int amebalite_gpt_getstatus(struct timer_lowerhalf_s *lower, struct timer_status_s *status)
{
	struct amebalite_gpt_lowerhalf_s *priv = (struct amebalite_gpt_lowerhalf_s *)lower;
	uint32_t ticks;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	/* Return the status bit */
	status->flags = 0;
	if (priv->started) {
		status->flags |= TCFLAGS_ACTIVE;
	}

	if (priv->callback) {
		status->flags |= TCFLAGS_HANDLER;
	}

	/* Return the actual timeout in microseconds */
	status->timeout = priv->gpt_timeout;

	/* Get the time remaining until the timer expires (in microseconds). */
	ticks = gtimer_read_tick(&priv->obj);

	if (priv->freerunmode) {	/* Free Run Mode */
		status->timeleft = (uint64_t)((float)ticks * (1000000 / 32768) + (priv->num_of_4294967ms_elapse * 0xFFFFFFFF));
	} else {					/* Alarm Mode */
		status->timeleft = status->timeout - ticks * (1000000 / 32768);
	}

	return OK;
}

/****************************************************************************
 * Name: amebalite_gpt_settimeout
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
static int amebalite_gpt_settimeout(struct timer_lowerhalf_s *lower, uint32_t timeout)
{
	struct amebalite_gpt_lowerhalf_s *priv = (struct amebalite_gpt_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	if (priv->started) {
		return -EPERM;
	}

	gtimer_reload(&priv->obj, timeout);
	priv->gpt_timeout = timeout;

	return OK;
}

/****************************************************************************
 * Name: amebalite_gpt_setcallback
 *
 * Description:
 *   Set the user provided timeout callback.
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
static void amebalite_gpt_setcallback(struct timer_lowerhalf_s *lower, tccb_t callback, void *arg)
{
	struct amebalite_gpt_lowerhalf_s *priv = (struct amebalite_gpt_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (priv) {
		irqstate_t flags = irqsave();

		/* Save the new callback */
		priv->callback = callback;
		priv->arg = arg;

		/* If no callback is attached, the timer stops at the first interrupt */
		if (callback != NULL && priv->started) {
			priv->obj.handler = (void *)amebalite_gpt_handler;
			priv->obj.hid = (uint32_t) priv;
		} else {
			priv->obj.handler = NULL;
			priv->obj.hid = 0;
		}

		irqrestore(flags);
	}
}

/****************************************************************************
 * Name: amebalite_gpt_ioctl
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
static int amebalite_gpt_ioctl(struct timer_lowerhalf_s *lower, int cmd, unsigned long arg)
{
	struct amebalite_gpt_lowerhalf_s *priv = (struct amebalite_gpt_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	int ret = -EINVAL;

	tmrvdbg("GPT IOCTL cmd: %d, arg: %ld\n", cmd, arg);

	switch (cmd) {
	case TCIOC_SETDIV:
		break;
	case TCIOC_GETDIV:
		break;
	case TCIOC_SETMODE:
		if ((timer_mode_t)arg == MODE_FREERUN) {
			priv->freerunmode = true;
			priv->num_of_4294967ms_elapse = 0;
			ret = OK;
		} else if ((timer_mode_t)arg == MODE_ALARM) {
			priv->freerunmode = false;
			ret = OK;
		}
		break;
	case TCIOC_SETRESOLUTION:
		if ((timer_resolution_t)arg == TIME_RESOLUTION_MS) {
			// TBD
			ret = OK;
		}
		break;
	case TCIOC_SETIRQPRIO:
		ret = up_prioritize_irq(TIMx[priv->obj.timer_id], arg);
		break;
	case TCIOC_SETCLKSRC:
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
int amebalite_timer_initialize(const char *devpath, int timer)
{
	struct amebalite_gpt_lowerhalf_s *priv = NULL;

	switch (timer) {
	case TIMER0:
		priv = &g_gpt0_lowerhalf;
		priv->obj.timer_id = TIMER0;
		break;

	case TIMER1:
		priv = &g_gpt1_lowerhalf;
		priv->obj.timer_id = TIMER1;
		break;

	case TIMER2:
		priv = &g_gpt2_lowerhalf;
		priv->obj.timer_id = TIMER2;
		break;

	case TIMER3:
		priv = &g_gpt3_lowerhalf;
		priv->obj.timer_id = TIMER3;
		break;

	default:
		priv = NULL;
		return -ENODEV;
	}

	/* Initialize the elements of lower half state structure */
	priv->started = false;
	priv->callback = NULL;

	/* Initializes the timer device, include timer registers and interrupt */
	gtimer_init(&priv->obj, priv->obj.timer_id);

	/*
	 * Register the timer driver as /dev/timerX.  The returned value from
	 * timer_register is a handle that could be used with timer_unregister().
	 * REVISIT: The returned handle is discard here.
	 */
	if (!timer_register(devpath, (struct timer_lowerhalf_s *)priv)) {
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

#endif							/* CONFIG_TIMER */
