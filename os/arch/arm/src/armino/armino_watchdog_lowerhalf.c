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
#include <tinyara/arch.h>
#include "wdt.h"
#include "hal_aon_rtc_types.h"
#include "aon_rtc.h"
#include "reset_reason.h"

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
struct armino_wdg_lowerhalf_s {
	FAR const struct watchdog_ops_s *ops;
	xcpt_t handler;
	uint32_t timeout_ms;
	uint32_t timeout_left;		/* Record remaining timeout for PAUSE and RESUME */
	uint32_t wdt_status;		/* 0: WDT_STOP; 1: WDT_START; 2: WDT_PAUSE */
	uint32_t start_tick;		/* Use to calculate timeleft before timeout */
	uint32_t pause_start_tick;	/* Record the start tick of watchdog timer on pause */
	uint32_t total_pause_duration;	/* Record the total duration of watchdog timer on pause */
	bool int_mode;				/* True for timeout interrupt mode on */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int armino_wdg_start(FAR struct watchdog_lowerhalf_s *lower);
static int armino_wdg_stop(FAR struct watchdog_lowerhalf_s *lower);
static int armino_wdg_keepalive(FAR struct watchdog_lowerhalf_s *lower);
static int armino_wdg_getstatus(FAR struct watchdog_lowerhalf_s *lower, FAR struct watchdog_status_s *status);
static int armino_wdg_settimeout(FAR struct watchdog_lowerhalf_s *lower, uint32_t timeout);
static xcpt_t armino_wdg_capture(FAR struct watchdog_lowerhalf_s *lower, xcpt_t handler);
static int armino_wdg_ioctl(FAR struct watchdog_lowerhalf_s *lower, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
const struct watchdog_ops_s g_wdgops = {
	.start = armino_wdg_start,
	.stop = armino_wdg_stop,
	.keepalive = armino_wdg_keepalive,
	.getstatus = armino_wdg_getstatus,
	.settimeout = armino_wdg_settimeout,
	.capture = armino_wdg_capture,
	.ioctl = armino_wdg_ioctl,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: armino_nmi_interrupt
 *
 * Description:
 *   WDT timeout interrupt
 *
 * Input Parameters:
 *   Usual interrupt handler arguments.
 *
 * Returned Values:
 *   Always returns OK.
 *
 ****************************************************************************/

static int armino_nmi_interrupt(int irq, void *context, FAR void *arg)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)arg;
	static bool s_nmi_occur = false;

	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	if (s_nmi_occur)
	{
		aon_pmu_drv_wdt_change_not_rosc_clk();
		aon_pmu_drv_wdt_rst_dev_enable();
		while(1);
	}

	if(REBOOT_TAG_REQ == sys_get_reboot_tag()) {
		while(1);
	}

	s_nmi_occur = true;

	if (priv->int_mode) {
		/* Yes... NOTE:  This interrupt service routine (ISR) must reload
		* the WDT counter to prevent the reset.  Otherwise, we will reset
		* about 8 ticks.
		*/
		bk_misc_set_reset_reason(RESET_SOURCE_WATCHDOG);
		if (priv->handler) 
		{
			bk_wdt_feed();
			priv->handler(irq, context, NULL);
		}

	} else {

        (void)irqsave();
        bk_wdt_feed();
		if(REBOOT_TAG_REQ != sys_get_reboot_tag()) 
		{
			bk_misc_set_reset_reason(RESET_SOURCE_WATCHDOG);
		}
		aon_pmu_drv_wdt_change_not_rosc_clk();
		aon_pmu_drv_wdt_rst_dev_enable();
        PANIC();
		while(1);
	}

	return OK;
	
}
/****************************************************************************
 * Name: armino_wdg_start
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
static int armino_wdg_start(FAR struct watchdog_lowerhalf_s *lower)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}
	if (priv->wdt_status == WDT_STOP) {
		/* Start watchdog timer */
		bk_wdt_start(priv->timeout_ms);
		priv->wdt_status = WDT_START;
		priv->start_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);	// Record time right after wdt start
		priv->pause_start_tick = 0; 
		priv->total_pause_duration = 0;
		return OK;
	}

	return -EBUSY;
}

/****************************************************************************
 * Name: armino_wdg_stop
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
static int armino_wdg_stop(FAR struct watchdog_lowerhalf_s *lower)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	/* Stop watchdog timer */
	close_wdt();
	priv->wdt_status = WDT_STOP;

	return OK;
}

/****************************************************************************
 * Name: armino_wdg_keepalive
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
static int armino_wdg_keepalive(FAR struct watchdog_lowerhalf_s *lower)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}
	if (priv->wdt_status == WDT_START) {
		/* Petting the dog */
		bk_wdt_feed();

		/* Update start time and reset on_pause_duration */
		priv->start_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
		priv->pause_start_tick = 0; 
		priv->total_pause_duration = 0;
		return OK;
	}

	return -EPERM;
}

/****************************************************************************
 * Name: armino_wdg_getstatus
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
static int armino_wdg_getstatus(FAR struct watchdog_lowerhalf_s *lower, FAR struct watchdog_status_s *status)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
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

		time_elapse = ((bk_aon_rtc_get_current_tick(AON_RTC_ID_1) - priv->start_tick - priv->total_pause_duration) * 1000) / bk_rtc_get_clock_freq();
		status->timeleft = status->timeout - time_elapse;

		/* Return ACTIVE if watchdog timer is started */
		if (priv->wdt_status == WDT_START) {
			status->flags |= WDFLAGS_ACTIVE;
		}
	}

	return OK;
}

/****************************************************************************
 * Name: armino_wdg_settimeout
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
static int armino_wdg_settimeout(FAR struct watchdog_lowerhalf_s *lower, uint32_t timeout)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	/*  Reset the watchdog timer with new timeout value */
	priv->timeout_ms = timeout;

	/* Re-start watchdog only if the status is WDT_START */
	if (priv->wdt_status == WDT_START) {
		bk_wdt_start(priv->timeout_ms);
		priv->start_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);	// Record time right after wdt start
	}

	return OK;
}

/****************************************************************************
 * Name: armino_wdg_capture
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
static xcpt_t armino_wdg_capture(FAR struct watchdog_lowerhalf_s *lower, xcpt_t handler)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
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
		priv->timeout_left = priv->timeout_ms - ((bk_aon_rtc_get_current_tick(AON_RTC_ID_1) - priv->start_tick - priv->total_pause_duration) * 1000 / bk_rtc_get_clock_freq());
	}
	// watchdog_init(priv->timeout_left);	// Re-init watchdog with remaining timeout
	priv->timeout_ms = priv->timeout_left;
	priv->int_mode = false;

	/* Change to INT_MODE if handler exists */
	if (handler) {
		// watchdog_irq_init((void *)priv->handler, (uint32_t)priv);	// INT_MODE enabled
		priv->int_mode = true;
	}

	/* Re-start watchdog only if the status is WDT_START */
	if (priv->wdt_status == WDT_START) {
		bk_wdt_start(priv->timeout_ms);
		priv->start_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);	// Record time right after wdt start
		priv->pause_start_tick = 0; 
		priv->total_pause_duration = 0;
	}

	return oldhandler;
}

/****************************************************************************
 * Name: armino_wdg_ioctl
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
static int armino_wdg_ioctl(FAR struct watchdog_lowerhalf_s *lower, int cmd, unsigned long arg)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
	uint32_t time_elapse;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	switch (cmd) {
	case WDIOC_PAUSE: {
		/* Pause watchdog and get remaining timeout */
		time_elapse = ((bk_aon_rtc_get_current_tick(AON_RTC_ID_1) - priv->start_tick - priv->total_pause_duration) * 1000) / bk_rtc_get_clock_freq();	// Get time elapse in ms
		armino_wdg_stop((struct watchdog_lowerhalf_s *)priv);
		priv->pause_start_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);	// Record PAUSE start time
		priv->timeout_left = priv->timeout_ms - time_elapse;	// Record remaining timeout
		priv->wdt_status = WDT_PAUSE;
	}
	break;

	case WDIOC_RESUME: {
		/* Pre-assumption: the WDIOC_PAUSE can only be resumed by WDIOC_RESUME */
		if (priv->wdt_status == WDT_PAUSE) {
			uint32_t pause_duration = bk_aon_rtc_get_current_tick(AON_RTC_ID_1) - priv->pause_start_tick;
			priv->total_pause_duration += pause_duration;
			priv->pause_start_tick = 0;
			priv->wdt_status = WDT_START;
			bk_wdt_start(priv->timeout_left);
			
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
 * Name: armino_wdg_initialize
 *
 * Description:
 *   Initialize and register the armino watchdog. Watchdog timer is registered
 *   as 'devpath'.
 *
 ****************************************************************************/
int armino_wdg_initialize(const char *devpath)
{
	struct armino_wdg_lowerhalf_s *priv = NULL;
	priv = (struct armino_wdg_lowerhalf_s *)kmm_malloc(sizeof(struct armino_wdg_lowerhalf_s));

	if (!priv) {
		llvdbg("wdg_lowerhalf_s allocation error.\n");
		return -ENOMEM;
	}

	/* Set the priv to default */
	priv->ops = &g_wdgops;
	priv->handler = NULL;

	/* Initializes the watchdog, include time setting, mode register */
	// watchdog_init(timeout_ms);
	priv->wdt_status = WDT_STOP;
	priv->int_mode = false;

	irq_attach(ARMINO_IRQ_NMI, armino_nmi_interrupt, (void *)priv);
	up_enable_irq(ARMINO_IRQ_NMI);

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
	bk_wdt_start(timeout);
}

#endif

void up_wdog_keepalive(void)
{
	bk_wdt_feed();
}

void up_watchdog_disable(void)
{
	bk_wdt_stop();
}

#endif							//CONFIG_WATCHDOG
