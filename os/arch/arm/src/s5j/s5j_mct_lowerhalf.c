/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/s5j_mct_lowerhalf.c
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
#include "s5j_mct.h"

#ifdef CONFIG_TIMER

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct s5j_lowerhalf_s {
	FAR const struct timer_ops_s	*ops;		/* Lower half operations */

	FAR struct s5j_mct_priv_s		*mct;
	bool							started;	/* True: Timer has been started */
	tccb_t							callback;
	FAR void						*arg;		/* Argument passed to upper half callback */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int s5j_timer_handler(int irq, void *context, void *arg);

static int s5j_mct_start(FAR struct timer_lowerhalf_s *lower);
static int s5j_mct_stop(struct timer_lowerhalf_s *lower);
static int s5j_mct_settimeout(FAR struct timer_lowerhalf_s *lower,
								uint32_t timeout);
static void s5j_mct_setcallback(FAR struct timer_lowerhalf_s *lower,
								CODE tccb_t callback, FAR void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct timer_ops_s g_timer_ops = {
	.start       = s5j_mct_start,
	.stop        = s5j_mct_stop,
	.getstatus   = NULL,
	.settimeout  = s5j_mct_settimeout,
	.setcallback = s5j_mct_setcallback,
	.ioctl       = NULL,
};

#ifdef CONFIG_S5J_TIMER0
static struct s5j_lowerhalf_s g_mct0_lowerhalf = {
	.ops         = &g_timer_ops,
};
#endif

#ifdef CONFIG_S5J_TIMER1
static struct s5j_lowerhalf_s g_mct1_lowerhalf = {
	.ops         = &g_timer_ops,
};
#endif

#ifdef CONFIG_S5J_TIMER2
static struct s5j_lowerhalf_s g_mct2_lowerhalf = {
	.ops         = &g_timer_ops,
};
#endif

#ifdef CONFIG_S5J_TIMER3
static struct s5j_lowerhalf_s g_mct3_lowerhalf = {
	.ops         = &g_timer_ops,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: s5j_timer_handler
 *
 * Description:
 *   timer interrupt handler
 *
 * Input Parameters:
 *
 * Returned Values:
 *
 ****************************************************************************/
static int s5j_timer_handler(int irq, void *context, void *arg)
{
	FAR struct s5j_lowerhalf_s *priv = (FAR struct s5j_lowerhalf_s *)arg;
	uint32_t next_interval_us = 0;

	s5j_mct_ack_irq(priv->mct);

	if (priv->callback(&next_interval_us, priv->arg)) {
		if (next_interval_us > 0) {
			s5j_mct_setperiod(priv->mct, next_interval_us);
		}
	} else {
		s5j_mct_stop((struct timer_lowerhalf_s *)priv);
	}

	return OK;
}

/****************************************************************************
 * Name: s5j_start
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
static int s5j_mct_start(FAR struct timer_lowerhalf_s *lower)
{
	FAR struct s5j_lowerhalf_s *priv = (FAR struct s5j_lowerhalf_s *)lower;

	if (!priv->started) {

		/* use interval mode */
		s5j_mct_setmode(priv->mct, false);

		if (priv->callback != NULL) {
			s5j_mct_setisr(priv->mct, s5j_timer_handler, priv);
			s5j_mct_enableint(priv->mct);
		}

		s5j_mct_enable(priv->mct);
		priv->started = true;

		return OK;
	}

	/* Return EBUSY to indicate that the timer was already running */
	return -EBUSY;
}

/****************************************************************************
 * Name: s5j_stop
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
static int s5j_mct_stop(struct timer_lowerhalf_s *lower)
{
	FAR struct s5j_lowerhalf_s *priv = (FAR struct s5j_lowerhalf_s *)lower;

	if (priv->started) {
		s5j_mct_disableint(priv->mct);
		s5j_mct_disable(priv->mct);
		s5j_mct_setisr(priv->mct, NULL, NULL);
		priv->started = false;

		return OK;
	}

	/* Return ENODEV to indicate that the timer was not running */
	return -ENODEV;
}

/****************************************************************************
 * Name: s5j_settimeout
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
static int s5j_mct_settimeout(FAR struct timer_lowerhalf_s *lower, uint32_t timeout)
{
	FAR struct s5j_lowerhalf_s *priv = (FAR struct s5j_lowerhalf_s *)lower;

	/* TODO: max timeout */
	const uint32_t maxtimeout = 0x0FFFFFFF;

	if (priv->started) {
		return -EPERM;
	}

	s5j_mct_setperiod(priv->mct, timeout > maxtimeout ? maxtimeout : timeout);

	return OK;
}

/****************************************************************************
 * Name: s5j_setcallback
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
static void s5j_mct_setcallback(FAR struct timer_lowerhalf_s *lower,
								tccb_t callback, FAR void *arg)
{
	FAR struct s5j_lowerhalf_s *priv = (FAR struct s5j_lowerhalf_s *)lower;
	irqstate_t flags = irqsave();

	/* Save the new callback */
	priv->callback = callback;
	priv->arg      = arg;

	if (callback != NULL && priv->started) {
		s5j_mct_setisr(priv->mct, s5j_timer_handler, priv);
		s5j_mct_enableint(priv->mct);
	} else {
		s5j_mct_disableint(priv->mct);
		s5j_mct_setisr(priv->mct, NULL, NULL);
	}

	irqrestore(flags);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int s5j_timer_initialize(FAR const char *devpath, int timer)
{
	FAR struct s5j_lowerhalf_s *lower = NULL;

	switch (timer) {
#ifdef CONFIG_S5J_TIMER0
	case S5J_MCT_CHANNEL0:
		lower = &g_mct0_lowerhalf;
		break;
#endif

#ifdef CONFIG_S5J_TIMER1
	case S5J_MCT_CHANNEL1:
		lower = &g_mct1_lowerhalf;
		break;
#endif

#ifdef CONFIG_S5J_TIMER2
	case S5J_MCT_CHANNEL2:
		lower = &g_mct2_lowerhalf;
		break;
#endif

#ifdef CONFIG_S5J_TIMER3
	case S5J_MCT_CHANNEL3:
		lower = &g_mct3_lowerhalf;
		break;
#endif
	}

	if (!lower) {
		return -ENODEV;
	}

	/* Initialize the elements of lower half state structure */
	lower->started  = false;
	lower->callback = NULL;
	lower->mct = s5j_mct_init(timer);

	if (lower->mct == NULL) {
		return -EINVAL;
	}

	/*
	 * Register the timer driver as /dev/timerX.  The returned value from
	 * timer_register is a handle that could be used with timer_unregister().
	 * REVISIT: The returned handle is discard here.
	 */
	if (!timer_register(devpath, (FAR struct timer_lowerhalf_s *)lower)) {
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
#endif /* CONFIG_TIMER */
