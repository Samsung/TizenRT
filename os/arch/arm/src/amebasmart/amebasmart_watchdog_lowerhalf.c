/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
#include <stdint.h>
#include <errno.h>
#include <tinyara/kmalloc.h>
#include <tinyara/watchdog.h>
#include <tinyara/irq.h>
#include "wdt_api.h"

#ifdef CONFIG_WATCHDOG

#define WDT_STOP	0
#define WDT_START	1
#define WDT_PAUSE	2

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* This structure provides the private representation of the "lower-half"
 * driver state structure.  This structure must be cast-compatible with the
 * well-known watchdog_lowerhalf_s structure.
 */
struct amebasmart_wdg_lowerhalf_s {
	FAR const struct watchdog_ops_s *ops;
	xcpt_t handler;
	uint32_t timeout_ms;
	uint32_t timeout_left;		/* Record remaining timeout for PAUSE and RESUME */
	uint32_t wdt_status;		/* 0: WDT_STOP; 1: WDT_START; 2: WDT_PAUSE */
	uint32_t start_tick;		/* Use to calculate timeleft before timeout */
	uint32_t on_pause_duration;	/* Record the duration of watchdog timer on pause */
	bool int_mode;				/* True for timeout interrupt mode on */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int amebasmart_wdg_start(FAR struct watchdog_lowerhalf_s *lower);
static int amebasmart_wdg_stop(FAR struct watchdog_lowerhalf_s *lower);
static int amebasmart_wdg_keepalive(FAR struct watchdog_lowerhalf_s *lower);
static int amebasmart_wdg_getstatus(FAR struct watchdog_lowerhalf_s *lower, FAR struct watchdog_status_s *status);
static int amebasmart_wdg_settimeout(FAR struct watchdog_lowerhalf_s *lower, uint32_t timeout);
static xcpt_t amebasmart_wdg_capture(FAR struct watchdog_lowerhalf_s *lower, xcpt_t handler);
static int amebasmart_wdg_ioctl(FAR struct watchdog_lowerhalf_s *lower, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
const struct watchdog_ops_s g_wdgops = {
	.start = amebasmart_wdg_start,
	.stop = amebasmart_wdg_stop,
	.keepalive = amebasmart_wdg_keepalive,
	.getstatus = amebasmart_wdg_getstatus,
	.settimeout = amebasmart_wdg_settimeout,
	.capture = amebasmart_wdg_capture,
	.ioctl = amebasmart_wdg_ioctl,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: amebsmart_wdg_start
 *
 * Description:
 *   Start the watchdog timer, resetting the time to the current timeout,
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int amebasmart_wdg_start(FAR struct watchdog_lowerhalf_s *lower)
{
	struct amebasmart_wdg_lowerhalf_s *priv = (struct amebasmart_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}
	if (priv->wdt_status == WDT_STOP) {
		/* Start watchdog timer */
		watchdog_start();
		priv->wdt_status = WDT_START;
		priv->start_tick = SYSTIMER_TickGet();	// Record time right after wdt start
		priv->on_pause_duration = 0;	// Reset on_pause_duration
		return OK;
	}

	return -EBUSY;
}

/****************************************************************************
 * Name: amebasmart_wdg_stop
 *
 * Description:
 *   Stop the watchdog timer
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int amebasmart_wdg_stop(FAR struct watchdog_lowerhalf_s *lower)
{
	struct amebasmart_wdg_lowerhalf_s *priv = (struct amebasmart_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	/* Stop watchdog timer */
	watchdog_stop();
	priv->wdt_status = WDT_STOP;

	return OK;
}

/****************************************************************************
 * Name: amebasmart_wdg_keepalive
 *
 * Description:
 *   Reset the watchdog timer to the current timeout value, prevent any
 *   imminent watchdog timeouts.  This is sometimes referred as "pinging"
 *   the watchdog timer or "petting the dog".
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int amebasmart_wdg_keepalive(FAR struct watchdog_lowerhalf_s *lower)
{
	struct amebasmart_wdg_lowerhalf_s *priv = (struct amebasmart_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}
	if (priv->wdt_status == WDT_START) {
		/* Petting the dog */
		watchdog_refresh();

		/* Update start time and reset on_pause_duration */
		priv->start_tick = SYSTIMER_TickGet();
		priv->on_pause_duration = 0;
		return OK;
	}

	return -EPERM;
}

/****************************************************************************
 * Name: amebasmart_wdg_getstatus
 *
 * Description:
 *   Get the current watchdog timer status
 *
 * Input Parameters:
 *   lower  - A pointer the publicly visible representation of the "lower-half"
 *            driver state structure.
 *   status - The location to return the watchdog status information.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int amebasmart_wdg_getstatus(FAR struct watchdog_lowerhalf_s *lower, FAR struct watchdog_status_s *status)
{
	struct amebasmart_wdg_lowerhalf_s *priv = (struct amebasmart_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	/* Reture watchdog flag */
	status->flags = WDFLAGS_RESET;

	/* Return CAPTURE if interrupt mode is enabled */
	if (priv->int_mode) {
		status->flags |= WDFLAGS_CAPTURE;
	}

	/* Reture timeout in ms */
	status->timeout = priv->timeout_ms;

	/* Return WAIT if watchdog timer is on pause */
	if (priv->wdt_status == WDT_PAUSE) {
		status->flags |= WDFLAGS_WAIT;
		status->timeleft = priv->timeout_left;
	} else {
		uint32_t time_elapse;
		time_elapse = ((SYSTIMER_TickGet() - priv->start_tick - priv->on_pause_duration) * 31) / 1000;	// Get time elapse in ms
		status->timeleft = status->timeout - time_elapse;

		/* Return ACTIVE if watchdog timer is started */
		if (priv->wdt_status == WDT_START) {
			status->flags |= WDFLAGS_ACTIVE;
		}
	}




	return OK;
}

/****************************************************************************
 * Name: amebasmart_wdg_settimeout
 *
 * Description:
 *   Set a new timeout value (and reset the watchdog timer)
 *
 * Input Parameters:
 *   lower   - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *   timeout - The new timeout value in milliseconds.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int amebasmart_wdg_settimeout(FAR struct watchdog_lowerhalf_s *lower, uint32_t timeout)
{
	struct amebasmart_wdg_lowerhalf_s *priv = (struct amebasmart_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	/*  Reset the watchdog timer with new timeout value */
	priv->timeout_ms = timeout;
	watchdog_init(priv->timeout_ms);
	if (priv->int_mode) {
		watchdog_irq_init((void *)priv->handler, (uint32_t)priv);
	}

	/* Re-start watchdog only if the status is WDT_START */
	if (priv->wdt_status == WDT_START) {
		watchdog_start();
	}

	return OK;
}

/****************************************************************************
 * Name: amebasmart_wdg_capture
 *
 * Description:
 *   Don't reset on watchdog timer timeout; instead, call this user provider
 *   timeout handler.  NOTE:  Providing handler==NULL will restore the reset
 *   behavior.
 *
 * Input Parameters:
 *   lower      - A pointer the publicly visible representation of the "lower-half"
 *                driver state structure.
 *   newhandler - The new watchdog expiration function pointer.  If this
 *                function pointer is NULL, then the reset-on-expiration
 *                behavior is restored,
 *
 * Returned Values:
 *   The previous watchdog expiration function pointer or NULL is there was
 *   no previous function pointer, i.e., if the previous behavior was
 *   reset-on-expiration (NULL is also returned if an error occurs).
 *
 ****************************************************************************/
static xcpt_t amebasmart_wdg_capture(FAR struct watchdog_lowerhalf_s *lower, xcpt_t handler)
{
	struct amebasmart_wdg_lowerhalf_s *priv = (struct amebasmart_wdg_lowerhalf_s *)lower;
	xcpt_t oldhandler;
	DEBUGASSERT(priv);
	if (!priv) {
		return NULL;
	}

	/* Record previous handler as the oldhandler */
	oldhandler = priv->handler;

	/* Set the new handler */
	priv->handler = handler;

	/* Set to default RESET_MODE first */
	if (priv->wdt_status == WDT_START) {	// Skip the timeout_left calculation if the status is WDT_PAUSE
		priv->timeout_left = priv->timeout_ms - ((SYSTIMER_TickGet() - priv->start_tick - priv->on_pause_duration) * 31) / 1000;
	}
	watchdog_init(priv->timeout_left);	// Re-init watchdog with remaining timeout
	priv->int_mode = false;

	/* Change to INT_MODE if handler exists */
	if (handler) {
		watchdog_irq_init((void *)priv->handler, (uint32_t)priv);	// INT_MODE enabled
		priv->int_mode = true;
	}

	/* Re-start watchdog only if the status is WDT_START */
	if (priv->wdt_status == WDT_START) {
		watchdog_start();
	}

	return oldhandler;
}

/****************************************************************************
 * Name: amebasmart_wdg_ioctl
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
static int amebasmart_wdg_ioctl(FAR struct watchdog_lowerhalf_s *lower, int cmd, unsigned long arg)
{
	struct amebasmart_wdg_lowerhalf_s *priv = (struct amebasmart_wdg_lowerhalf_s *)lower;
	uint32_t time_elapse;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	switch (cmd) {
	case WDIOC_PAUSE: {
		/* Pause watchdog and get remaining timeout */
		time_elapse = ((SYSTIMER_TickGet() - priv->start_tick - priv->on_pause_duration) * 31) / 1000;	// Get time elapse in ms
		amebasmart_wdg_stop((struct watchdog_lowerhalf_s *)priv);
		priv->on_pause_duration = SYSTIMER_TickGet();	// Record PAUSE start time
		priv->timeout_left = priv->timeout_ms - time_elapse;	// Record remaining timeout
		priv->wdt_status = WDT_PAUSE;
	}
	break;

	case WDIOC_RESUME: {
		/* Pre-assumption: the WDIOC_PAUSE can only be resumed by WDIOC_RESUME */
		if (priv->wdt_status == WDT_PAUSE) {
			/* Resume watchdog and init watchdog with remaining timeout */
			priv->on_pause_duration = SYSTIMER_TickGet() - priv->on_pause_duration;
			watchdog_init(priv->timeout_left);
			if (priv->int_mode) {
				watchdog_irq_init((void *)priv->handler, (uint32_t)priv);
			}
			watchdog_start();
			priv->wdt_status = WDT_START;
		}
	}
	break;

	default: {
		llvdbg("invalid cmd for wdg ioctl.\n");
		return -EINVAL;
	}
	}
	return OK;
}

/****************************************************************************
 * Name: amebasmart_wdg_initialize
 *
 * Description:
 *   Initialize and register the amebasmart watchdog. Watchdog timer is registered
 *   as 'devpath'.
 *
 ****************************************************************************/
int amebasmart_wdg_initialize(const char *devpath, uint32_t timeout_ms)
{
	struct amebasmart_wdg_lowerhalf_s *priv = NULL;
	priv = (struct amebasmart_wdg_lowerhalf_s *)kmm_malloc(sizeof(struct amebasmart_wdg_lowerhalf_s));

	if (!priv) {
		llvdbg("wdg_lowerhalf_s allocation error.\n");
		return -ENOMEM;
	}

	/* Set the priv to default */
	priv->ops = &g_wdgops;
	priv->handler = NULL;

	/* Initializes the watchdog, include time setting, mode register */
	watchdog_init(timeout_ms);
	priv->wdt_status = WDT_STOP;
	priv->int_mode = false;

	/* Register the watchdog driver as /dev/wdgX. */
	if (!watchdog_register(devpath, (FAR struct watchdog_lowerhalf_s *)priv)) {
		/*
		 * The actual cause of the failure may have been a failure to allocate
		 * perhaps a failure to register the timer driver (such as if the
		 * 'depath' were not unique).  We know here but we return EEXIST to
		 * indicate the failure (implying the non-unique devpath).
		 */
		kmm_free(priv);
		return -EEXIST;
	}
	
	return OK;
}

#ifdef CONFIG_WATCHDOG_FOR_IRQ
void up_wdog_init(uint16_t timeout)
{
	watchdog_init(timeout);
}

void up_wdog_keepalive(void)
{
	watchdog_refresh();
}
#endif

void up_watchdog_disable(void)
{
	watchdog_stop();
}

#endif							//CONFIG_WATCHDOG

