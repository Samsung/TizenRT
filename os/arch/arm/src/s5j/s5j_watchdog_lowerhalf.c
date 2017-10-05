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
 * os/arch/arm/src/s5j/s5j_watchdog_lowerhalf.c
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

#include <stdbool.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/watchdog.h>
#include <tinyara/kmalloc.h>

#include "up_arch.h"
#include "s5j_watchdog.h"
#include "s5j_clock.h"

#define S5J_WDT_MAXTIMEOUT		1000000

#ifndef CONFIG_S5J_WDT_DEFTIMEOUT
#define CONFIG_S5J_WDT_DEFTIMEOUT	7000
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* This structure provides the private representation of the "lower-half"
 * driver state structure.  This structure must be cast-compatible with the
 * well-known watchdog_lowerhalf_s structure.
 */

struct s5j_lowerhalf_s {
	FAR const struct watchdog_ops_s *ops;
	FAR struct s5j_lowerhalf_s *next;
	xcpt_t handler;
	uint32_t reload;
	uint32_t timeout;
	bool started;
	unsigned int clk_handle;
	uint32_t prescaler;
	uint32_t divider;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* Interrupt hanlding *******************************************************/

static int s5j_wdg_interrupt(int irq, FAR void *context);

/* "Lower half" driver methods **********************************************/

static int s5j_wdg_start(FAR struct watchdog_lowerhalf_s *lower);
static int s5j_wdg_stop(FAR struct watchdog_lowerhalf_s *lower);
static int s5j_wdg_keepalive(FAR struct watchdog_lowerhalf_s *lower);
static int s5j_wdg_getstatus(FAR struct watchdog_lowerhalf_s *lower, FAR struct watchdog_status_s *status);
static int s5j_wdg_settimeout(FAR struct watchdog_lowerhalf_s *lower, uint32_t timeout);
static xcpt_t s5j_wdg_capture(FAR struct watchdog_lowerhalf_s *lower, xcpt_t handler);
static int s5j_wdg_ioctl(FAR struct watchdog_lowerhalf_s *lower, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* "Lower half" driver methods */

static const struct watchdog_ops_s g_wdgops = {
	.start = s5j_wdg_start,
	.stop = s5j_wdg_stop,
	.keepalive = s5j_wdg_keepalive,
	.getstatus = s5j_wdg_getstatus,
	.settimeout = s5j_wdg_settimeout,
	.capture = s5j_wdg_capture,
	.ioctl = s5j_wdg_ioctl,
};

/* "Lower half" driver state */

FAR struct s5j_lowerhalf_s *wdt_head;
FAR struct s5j_lowerhalf_s *wdt_tail;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: s5j_wdg_interrupt
 *
 * Description:
 *   WDG early warning interrupt
 *
 * Input Parameters:
 *   Usual interrupt handler arguments.
 *
 * Returned Values:
 *   Always returns OK.
 *
 ****************************************************************************/

static int s5j_wdg_interrupt(int irq, FAR void *context)
{
	FAR struct s5j_lowerhalf_s *priv;

	for (priv = wdt_head; priv; priv = priv->next) {
		if (irq == IRQ_WDT) {
			break;
		}
	}

	DEBUGASSERT(priv);

	if (priv->handler) {
		/* Yes... NOTE:  This interrupt service routine (ISR) must reload
		 * the WDG counter to prevent the reset.  Otherwise, we will reset
		 * upon return.
		 */

		priv->handler(irq, context, NULL);
	}

	s5j_watchdog_clear_int();
	lldbg("%s\n", __func__);

	return OK;
}

/****************************************************************************
 * Name: s5j_wdg_start
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

static int s5j_wdg_start(FAR struct watchdog_lowerhalf_s *lower)
{
	FAR struct s5j_lowerhalf_s *priv = (FAR struct s5j_lowerhalf_s *)lower;

	DEBUGASSERT(priv);

	/* Set watchdog timer enable bit, interrupt enable bit and reset enable bit
	 * to start watchdog timer.
	 */

	s5j_watchdog_enable();

	priv->started = true;

	return OK;
}

/****************************************************************************
 * Name: s5j_wdg_stop
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

static int s5j_wdg_stop(FAR struct watchdog_lowerhalf_s *lower)
{
	FAR struct s5j_lowerhalf_s *priv = (FAR struct s5j_lowerhalf_s *)lower;

	DEBUGASSERT(priv);

	/* Clear watchdog timer enable bit, interrupt enable bit and reset enable bit
	 * to stop watchdog timer.
	 */

	s5j_watchdog_disable();

	priv->started = false;

	return OK;
}

/****************************************************************************
 * Name: s5j_wdg_keepalive
 *
 * Description:
 *   Reset the watchdog timer to the current timeout value, prevent any
 *   imminent watchdog timeouts.  This is sometimes referred as "pinging"
 *   the watchdog timer or "petting the dog".
 *
 *   The application program must write in the WWDG_CR register at regular
 *   intervals during normal operation to prevent an MCU reset. This operation
 *   must occur only when the counter value is lower than the window register
 *   value.
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int s5j_wdg_keepalive(FAR struct watchdog_lowerhalf_s *lower)
{
	FAR struct s5j_lowerhalf_s *priv = (FAR struct s5j_lowerhalf_s *)lower;

	DEBUGASSERT(priv);

	s5j_watchdog_set_curr(priv->reload);

	return OK;
}

/****************************************************************************
 * Name: s5j_wdg_getstatus
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

static int s5j_wdg_getstatus(FAR struct watchdog_lowerhalf_s *lower, FAR struct watchdog_status_s *status)
{
	FAR struct s5j_lowerhalf_s *priv = (FAR struct s5j_lowerhalf_s *)lower;
	uint32_t elapsed;
	uint16_t reload;

	DEBUGASSERT(priv);

	status->flags = WDFLAGS_RESET;

	/* Return ACTIVE if watchdog timer is started */

	if (priv->started) {
		status->flags |= WDFLAGS_ACTIVE;
	}

	if (priv->handler) {
		status->flags |= WDFLAGS_CAPTURE;
	}

	/* Return the actual timeout is milliseconds */

	status->timeout = priv->timeout;

	/* Get the time remaining until the watchdog expires (in miliseconds) */

	reload = s5j_watchdog_get_curr();
	elapsed = priv->reload - reload;
	status->timeleft = (priv->timeout * elapsed) / (priv->reload + 1);

#ifdef CONFIG_S5J_WATCHDOG_DEBUG
	lldbg("Status     :\n");
	lldbg("  flags    : %08x\n", status->flags);
	lldbg("  timeout  : %d\n", status->timeout);
	lldbg("  timeleft : %d\n", status->flags);
#endif

	return OK;
}

/****************************************************************************
 * Name: s5j_wdg_settimeout
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

static int s5j_wdg_settimeout(FAR struct watchdog_lowerhalf_s *lower, uint32_t timeout)
{
	FAR struct s5j_lowerhalf_s *priv = (FAR struct s5j_lowerhalf_s *)lower;
	DEBUGASSERT(priv != NULL);
	uint32_t freq, reload = 0;

	if (timeout < 1 || timeout > S5J_WDT_MAXTIMEOUT) {
		lldbg("Cannot represent time out= %d > %d\n", timeout, S5J_WDT_MAXTIMEOUT);
		return -ERANGE;
	}

	/*
	 * See S5JT200 WTCON reg
	 * sidk_s5jt200 uses OSC 26Mhz
	 * divider = 2^division factor*16
	 * Prescaler  = Prescaler value + 1
	 */

	freq = S5J_WDT_OSC / (priv->prescaler + 1) / (16 << priv->divider);
	reload = timeout * ((freq / 1000)) - 1;

#ifdef CONFIG_S5J_WATCHDOG_DEBUG
	lldbg("timeout %d, reload %d, s5j clk_wdt %12ld, wdt freq= %12ld\n", timeout, reload, S5J_WDT_OSC, freq);
#endif

	s5j_watchdog_set_reload_val(reload);
	s5j_watchdog_set_curr(reload);
	priv->timeout = timeout;
	priv->reload = reload;

	return OK;
}

/****************************************************************************
 * Name: s5j_wdg_capture
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

static xcpt_t s5j_wdg_capture(FAR struct watchdog_lowerhalf_s *lower, xcpt_t handler)
{
	FAR struct s5j_lowerhalf_s *priv = (FAR struct s5j_lowerhalf_s *)lower;
	xcpt_t oldhandler;

	oldhandler = priv->handler;

	/* Set the new handler */

	priv->handler = handler;

	if (handler) {
		s5j_watchdog_irq_enable();

		up_enable_irq(IRQ_WDT);
	} else {
		s5j_watchdog_irq_disable();

		up_disable_irq(IRQ_WDT);
	}

	return oldhandler;
}

/****************************************************************************
 * Name: s5j_wdg_ioctl
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

static int s5j_wdg_ioctl(FAR struct watchdog_lowerhalf_s *lower, int cmd, unsigned long arg)
{
	/* WDIOC_MINTIME: Set the minimum ping time.  If two keepalive ioctls
	 * are received within this time, a reset event will be generated.
	 * Argument: A 32-bit time value in milliseconds.
	 */

	if (cmd == WDIOC_MINTIME) {
		lldbg("%s, WDIOC_MINTIME\n", __func__);
	}

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5j_wdg_initialize
 *
 * Description:
 *   Initialize the WDG watchdog timer.  The watchdog timer is initialized and
 *   registers as 'devpath'.  The initial state of the watchdog timer is
 *   disabled.
 *
 * Input Parameters:
 *   devpath - The full path to the watchdog.  This should be of the form
 *     /dev/watchdog0
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/

int s5j_wdg_initialize(FAR const char *devpath)
{
	FAR struct s5j_lowerhalf_s *priv;

	/* Allocate memory space for S5J watchdog lower half data structure */

	priv = (struct s5j_lowerhalf_s *)kmm_zalloc(sizeof(struct s5j_lowerhalf_s));
	if (!priv) {
		llvdbg("Private data allocation error\n");
		return -ENOMEM;
	}

	/* Initialize the driver private data structure */

	priv->ops = &g_wdgops;
	priv->clk_handle = s5j_clk_get_rate(CLK_DFT_OSCCLK);
	priv->next = NULL;

	if (wdt_head == NULL) {
		wdt_head = wdt_tail = priv;
	} else {
		wdt_tail->next = priv;
		wdt_tail = priv;
	}

	priv->divider = 0;
	priv->prescaler = 0;

	/* Clock with OSC 26Mhz */
	/* t_watchdog = 1/(CLK/(Prescaler value, WTCON[15:8] + 1)/Division_factor, WTCON[4:3]) */
	/* Set to Freq 1.625Mhz = 1 / CLK(26Mhz) / (1) / (16) */

	s5j_watchdog_clk_set(priv->prescaler, priv->divider);

	/* set WDT reset & Intterupt */

#ifdef CONFIG_S5J_WATCHDOG_RESET
	s5j_watchdog_reset_enable();
#endif

#ifdef CONFIG_S5J_WATCHDOG_INT
	s5j_watchdog_irq_enable();

	/* Attach our EWI interrupt handler (But don't enable it yet) */

	(void)irq_attach(priv->irqno, s5j_wdg_interrupt);
	up_enable_irq(priv->irqno);
#endif

	s5j_wdg_stop((FAR struct watchdog_lowerhalf_s *)priv);

	/* Select an arbitrary initial timeout value.  But don't start the watchdog
	 * yet. NOTE: If the "Hardware watchdog" feature is enabled through the
	 * device option bits, the watchdog is automatically enabled at power-on.
	 */

	s5j_wdg_settimeout((FAR struct watchdog_lowerhalf_s *)priv, CONFIG_S5J_WDT_DEFTIMEOUT);

	/* Register the watchdog driver as /dev/watchdog0 */

	(void)watchdog_register(devpath, (FAR struct watchdog_lowerhalf_s *)priv);

	return OK;
}
