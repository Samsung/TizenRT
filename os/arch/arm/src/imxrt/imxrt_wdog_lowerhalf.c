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
 * os/arch/arm/src/imxrt/imxrt_wdog_lowerhalf.c
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author:  Janne Rosberg <janne@offcode.fi>
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

#include <tinyara/config.h>
#include <stdint.h>
#include <errno.h>

#include <tinyara/kmalloc.h>
#include <tinyara/watchdog.h>
#include <tinyara/irq.h>

#include "imxrt_wdog.h"
#include "chip/imxrt_wdog.h"

#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
#include "chip/imxrt102x_config.h"
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#include "chip/imxrt105x_config.h"
#else
#error Unrecognized i.MX RT architecture
#endif

#ifdef CONFIG_WATCHDOG
static int imxrt_wdog_start(FAR struct watchdog_lowerhalf_s *lower);
static int imxrt_wdog_stop(FAR struct watchdog_lowerhalf_s *lower);
static int imxrt_wdog_keepalive(FAR struct watchdog_lowerhalf_s *lower);
static int imxrt_wdog_getstatus(FAR struct watchdog_lowerhalf_s *lower, FAR struct watchdog_status_s *status);
static int imxrt_wdog_settimeout(FAR struct watchdog_lowerhalf_s *lower, uint32_t timeout);
static xcpt_t imxrt_wdog_capture(FAR struct watchdog_lowerhalf_s *lower, xcpt_t handler);
static int imxrt_wdog_ioctl(FAR struct watchdog_lowerhalf_s *lower, int cmd, unsigned long arg);

const struct watchdog_ops_s g_wdgops = {
	.start = imxrt_wdog_start,
	.stop = imxrt_wdog_stop,
	.keepalive = imxrt_wdog_keepalive,
	.getstatus = imxrt_wdog_getstatus,
	.settimeout = imxrt_wdog_settimeout,
	.capture = imxrt_wdog_capture,
	.ioctl = imxrt_wdog_ioctl,
};

/* This structure provides the private representation of the "lower-half"
 * driver state structure.  This structure must be cast-compatible with the
 * well-known watchdog_lowerhalf_s structure.
 */

struct imxrt_wdog_lowerhalf_s {
	FAR const struct watchdog_ops_s *ops;
	WDOG_Type *base;
	wdog_config_t config;
	xcpt_t handler;
};

static int imxrt_wdog_start(FAR struct watchdog_lowerhalf_s *lower)
{
	struct imxrt_wdog_lowerhalf_s *priv = (struct imxrt_wdog_lowerhalf_s *)lower;
	DEBUGASSERT(priv);

	imxrt_wdog_init(priv->base, &priv->config);
	return OK;
}

static int imxrt_wdog_stop(FAR struct watchdog_lowerhalf_s *lower)
{
	struct imxrt_wdog_lowerhalf_s *priv = (struct imxrt_wdog_lowerhalf_s *)lower;
	DEBUGASSERT(priv);

	imxrt_wdog_deinit(priv->base);

	return OK;
}

static int imxrt_wdog_keepalive(FAR struct watchdog_lowerhalf_s *lower)
{
	struct imxrt_wdog_lowerhalf_s *priv = (struct imxrt_wdog_lowerhalf_s *)lower;
	DEBUGASSERT(priv);

	imxrt_wdog_refresh(priv->base);

	return OK;
}

static int imxrt_wdog_getstatus(FAR struct watchdog_lowerhalf_s *lower, FAR struct watchdog_status_s *status)
{
	int flag;
	struct imxrt_wdog_lowerhalf_s *priv = (struct imxrt_wdog_lowerhalf_s *)lower;
	DEBUGASSERT(priv);

	status->flags = WDFLAGS_RESET;
	flag = imxrt_wdog_getstatusflags(priv->base);

	if ((flag & WDOG_WCR_WDE_MASK != 0) && (flag & WDOG_WCR_WDW_MASK == 0)) {
		status->flags = WDFLAGS_ACTIVE;
	} else if ((flag & WDOG_WCR_WDE_MASK != 0) && (flag & WDOG_WCR_WDW_MASK != 0)) {
		status->flags = WDFLAGS_WAIT;
	}

	status->timeout = priv->config.timeoutValue;
	/* TO BE UPDATED */
	status->timeleft = 0;

	return OK;
}

static int imxrt_wdog_settimeout(FAR struct watchdog_lowerhalf_s *lower, uint32_t timeout)
{
	struct imxrt_wdog_lowerhalf_s *priv = (struct imxrt_wdog_lowerhalf_s *)lower;
	DEBUGASSERT(priv);

	/* Set to new timeout. */
	priv->config.timeoutValue = (uint16_t)timeout;

	return OK;
}

static xcpt_t imxrt_wdog_capture(FAR struct watchdog_lowerhalf_s *lower, xcpt_t handler)
{
#if 0
	struct imxrt_wdog_lowerhalf_s *priv = (struct imxrt_wdog_lowerhalf_s *)lower;
	xcpt_t oldhandler;

	DEBUGASSERT(priv);

	oldhandler = priv->handler;
	priv->config.enableInterrupt = true;

	/* Set the new handler. */
	priv->handler = handler;
	irq_attach(WDOG1_IRQn, handler, NULL);

	return oldhandler;
#endif
	/* Not Implemented yet. */
	return NULL;
}

static int imxrt_wdog_ioctl(FAR struct watchdog_lowerhalf_s *lower, int cmd, unsigned long arg)
{
	struct imxrt_wdog_lowerhalf_s *priv = (struct imxrt_wdog_lowerhalf_s *)lower;
	DEBUGASSERT(priv);

	switch (cmd) {
	case WDIOC_PAUSE: {
		/* Set WDOG_WCR_WDW to 1 for suspending WDOG timer operation. */
		priv->config.workMode.enableWait = true;

		/* Restart the watchdog with new configuration. */
		imxrt_wdog_init(priv->base, &priv->config);
	}
	break;

	case WDIOC_RESUME: {
		/* Reset WDOG_WCR_WDW to continue WDOG timer operation. */
		priv->config.workMode.enableWait = false;

		/* Restart the watchdog with new configuration. */
		imxrt_wdog_init(priv->base, &priv->config);
	}
	break;

	default: {
		llvdbg("invalid cmd for wdog ioctl.\n");
		return ERROR;
	}
	}
	return OK;
}

/****************************************************************************
 * Name: imxrt_wdog_initialize
 *
 * Description:
 *   Initialize and register the imxrt watchdog. Watchdog timer is registered
 *   as 'devpath'.
 *
 ****************************************************************************/
int imxrt_wdog_initialize(const char *devpath, int wdog_base)
{
	struct imxrt_wdog_lowerhalf_s *priv = NULL;

	priv = (struct imxrt_wdog_lowerhalf_s *)kmm_malloc(sizeof(struct imxrt_wdog_lowerhalf_s));
	if (!priv) {
		llvdbg("wdog_lowerhalf_s allocation error.\n");
		return -ENOMEM;
	}

	/* Set the priv to default */
	priv->ops = &g_wdgops;
	priv->handler = NULL;

	switch (wdog_base) {
	case IMXRT_WDOG1:
		priv->base = WDOG1;
		break;
	case IMXRT_WDOG2:
		priv->base = WDOG2;
		break;
	}

	imxrt_wdog_getdefaultconfig(&priv->config);

	/* Register the watchdog driver as /dev/wdogX. */
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
#endif //CONFIG_WATCHDOG
