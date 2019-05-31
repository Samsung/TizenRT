/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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
/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <string.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/clock.h>
#include <tinyara/timer.h>

#include "imxrt_clock.h"
#include "imxrt_pit.h"
#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
#include "imxrt102x_irq.h"
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#include "imxrt105x_irq.h"
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Gets the instance from the base address to be used to gate or ungate the module clock
 *
 * @param base PIT peripheral base address
 *
 * @return The PIT instance
 */
static uint32_t imxrt_pit_getinstance(PIT_Type *base);
static int imxrt_pit_start(struct timer_lowerhalf_s *lower);
static int imxrt_pit_stop(struct timer_lowerhalf_s *lower);
static int imxrt_pit_getstatus(struct timer_lowerhalf_s *lower, struct timer_status_s *status);
static int imxrt_pit_settimeout(struct timer_lowerhalf_s *lower, uint32_t timeout);
static void imxrt_pit_setcallback(struct timer_lowerhalf_s *lower, tccb_t callback, void *arg);
static int imxrt_pit_ioctl(struct timer_lowerhalf_s *lower, int cmd, unsigned long arg);


/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to PIT bases for each instance. */
static PIT_Type *const s_pitBases[] = PIT_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to PIT clocks for each instance. */
static const clock_ip_name_t s_pitClocks[] = PIT_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

static struct imxrt_pit_chipinfo_s imxrt_pit_chipinfo = {
	.base   = PIT,
	.irq_id = IMXRT_IRQ_PIT,
};

bool busy_status[4] = {false, false, false, false,};
bool ch_status[4] = {false, false, false, false,};

int active = 0;

struct imxrt_pit_lowerhalf_s {
	const struct timer_ops_s    *ops;       /* Lowerhalf operations */
	struct imxrt_pit_chipinfo_s *pit;
	pit_config_t                 config;
	bool                         started;   /* True: Timer has been started */
	tccb_t                       callback;
	void                        *arg;       /* Argument passed to upper half callback */
	pit_chnl_t		channel;
};

static const struct timer_ops_s pit_timer_ops = {
	.start       = imxrt_pit_start,
	.stop        = imxrt_pit_stop,
	.getstatus   = imxrt_pit_getstatus,
	.settimeout  = imxrt_pit_settimeout,
	.setcallback = imxrt_pit_setcallback,
	.ioctl       = imxrt_pit_ioctl,
};

static struct imxrt_pit_lowerhalf_s g_pit0_lowerhalf = {
	.ops		= &pit_timer_ops,
	.channel	= kPIT_Chnl_0,
};

static struct imxrt_pit_lowerhalf_s g_pit1_lowerhalf = {
	.ops		= &pit_timer_ops,
	.channel	= kPIT_Chnl_1,
};

static struct imxrt_pit_lowerhalf_s g_pit2_lowerhalf = {
	.ops		= &pit_timer_ops,
	.channel	= kPIT_Chnl_2,
};

static struct imxrt_pit_lowerhalf_s g_pit3_lowerhalf = {
	.ops		= &pit_timer_ops,
	.channel	= kPIT_Chnl_3,
};

//bool initialized = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t imxrt_pit_getinstance(PIT_Type *base)
{
	uint32_t instance;

	/* Find the instance index from base address mappings. */
	for (instance = 0; instance < ARRAY_SIZE(s_pitBases); instance++) {
		if (s_pitBases[instance] == base) {
			break;
		}
	}

	assert(instance < ARRAY_SIZE(s_pitBases));

	return instance;
}

/*!
 * brief Ungates the PIT clock, enables the PIT module, and configures the peripheral for basic operations.
 *
 * note This API should be called at the beginning of the application using the PIT driver.
 *
 * param base   PIT peripheral base address
 * param config Pointer to the user's PIT config structure
 */
void imxrt_pit_init(PIT_Type *base, const pit_config_t *config)
{
	assert(config);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
		/* Ungate the PIT clock*/
		imxrt_clock_enableclock(s_pitClocks[imxrt_pit_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if defined(FSL_FEATURE_PIT_HAS_MDIS) && FSL_FEATURE_PIT_HAS_MDIS
		/* Enable PIT timers */
		base->MCR &= ~PIT_MCR_MDIS_MASK;
#endif

		/* Config timer operation when in debug mode */
		if (config->enableRunInDebug) {
			base->MCR &= ~PIT_MCR_FRZ_MASK;
		} else {
			base->MCR |= PIT_MCR_FRZ_MASK;
		}
}

/*!
 * brief Gates the PIT clock and disables the PIT module.
 *
 * param base PIT peripheral base address
 */
void imxrt_pit_deinit(PIT_Type *base)
{
#if defined(FSL_FEATURE_PIT_HAS_MDIS) && FSL_FEATURE_PIT_HAS_MDIS
	/* Disable PIT timers */
	base->MCR |= PIT_MCR_MDIS_MASK;
#endif

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	/* Gate the PIT clock*/
	imxrt_clock_disableclock(s_pitClocks[imxrt_pit_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

#if defined(FSL_FEATURE_PIT_HAS_LIFETIME_TIMER) && FSL_FEATURE_PIT_HAS_LIFETIME_TIMER

/*!
 * brief Reads the current lifetime counter value.
 *
 * The lifetime timer is a 64-bit timer which chains timer 0 and timer 1 together.
 * Timer 0 and 1 are chained by calling the PIT_SetTimerChainMode before using this timer.
 * The period of lifetime timer is equal to the "period of timer 0 * period of timer 1".
 * For the 64-bit value, the higher 32-bit has the value of timer 1, and the lower 32-bit
 * has the value of timer 0.
 *
 * param base PIT peripheral base address
 *
 * return Current lifetime timer value
 */
uint64_t imxrt_pit_getlifetimetimercount(PIT_Type *base)
{
	uint32_t valueH = 0U;
	uint32_t valueL = 0U;

	/* LTMR64H should be read before LTMR64L */
	valueH = base->LTMR64H;
	valueL = base->LTMR64L;

	return (((uint64_t)valueH << 32U) + (uint64_t)(valueL));
}
#endif

static int imxrt_pit_settimeout(struct timer_lowerhalf_s *lower, uint32_t timeout)
{
	uint32_t freq;
	uint64_t ticks;
	struct imxrt_pit_lowerhalf_s *priv = (struct imxrt_pit_lowerhalf_s *)lower;

	if (priv->started) {
		return -EPERM;
	}

	freq = imxrt_clock_getperclkfreq();
	ticks = ((uint64_t)freq * (uint64_t)timeout) / USEC_PER_SEC;

	if (ticks < UINT_MAX) {
		imxrt_pit_settimerperiod(priv->pit->base, priv->channel, ticks);
	} else if (ticks >= UINT_MAX) {
		pit_chnl_t chained = priv->channel - 1;
		if (busy_status[chained] == false) {
			tmrvdbg("Enter CHAIN mode\n");
			uint32_t ch_ticks1 = (uint32_t)freq /USEC_PER_SEC;
			uint32_t ch_ticks2 = (uint32_t)timeout;
			tmrvdbg("ticks:%llu ch_ticks1:%d ch_ticks2:%d\n", ticks, ch_ticks1, ch_ticks2);
			imxrt_pit_settimerchainmode(priv->pit->base, priv->channel, true);
			imxrt_pit_settimerperiod(priv->pit->base, priv->channel, ch_ticks1);
			imxrt_pit_settimerperiod(priv->pit->base, chained, ch_ticks2);
			ch_status[priv->channel] = true;
		} else {
			tmrvdbg("CHAIN mode cannot be used as channel[%d] is Busy\n", chained);
			return -EBUSY;
		}
	}
	return OK;
}

struct imxrt_pit_chipinfo_s *imxrt_pit_configure(int timer, pit_config_t *config, pit_chnl_t channel)
{
	struct imxrt_pit_chipinfo_s *pit = NULL;
	pit = &imxrt_pit_chipinfo;

	imxrt_pit_init(pit->base, config);
	pit->base->CHANNEL[channel].TCTRL &= ~PIT_TCTRL_TEN_MASK;
	return pit;
}

void imxrt_pit_setisr(struct imxrt_pit_chipinfo_s *pit, xcpt_t handler, void *arg)
{
	irq_attach(pit->irq_id, handler, arg);

	up_enable_irq(pit->irq_id);
}

static int imxrt_pit_handler(int irq, void *context, void *arg)
{
	struct imxrt_pit_lowerhalf_s *priv = (struct imxrt_pit_lowerhalf_s *)arg;
	struct imxrt_pit_chipinfo_s *pit = priv->pit;
	uint32_t next_interval_us = 0;
	uint32_t freq;
	uint64_t ticks;

	imxrt_pit_clearstatusflags(pit->base, priv->channel, kPIT_TimerFlag);

	if (priv->callback(&next_interval_us, priv->arg)) {
		if (next_interval_us > 0) {
			freq = imxrt_clock_getperclkfreq();
			ticks = ((uint64_t)freq * (uint64_t)next_interval_us) / USEC_PER_SEC;
			imxrt_pit_settimerperiod(priv->pit->base, priv->channel, ticks);
		}
	} else {
		imxrt_pit_stop((struct timer_lowerhalf_s *)priv);
	}

	return OK;
}

static int imxrt_pit_start(struct timer_lowerhalf_s *lower)
{
	struct imxrt_pit_lowerhalf_s *priv = (struct imxrt_pit_lowerhalf_s *)lower;

	if (!priv->started) {
		if (priv->callback != NULL) {
			//imxrt_pit_setisr(priv->pit, imxrt_pit_handler, priv);
			imxrt_pit_enableinterrupts(priv->pit->base, priv->channel, kPIT_TimerInterruptEnable);
		}

		if (active == 0) {
			imxrt_pit_setisr(priv->pit, imxrt_pit_handler, priv);
		}

		if (ch_status[priv->channel])
			imxrt_pit_starttimer(priv->pit->base, (priv->channel-1));

		imxrt_pit_starttimer(priv->pit->base, priv->channel);

		priv->started = true;
		busy_status[priv->channel] = true;
		if (ch_status[priv->channel]) {
			busy_status[priv->channel-1] = true;
		}

		active++;

		tmrvdbg("PIT Timer Channel %d is started, callback %s\n", priv->channel, priv->callback == NULL ? "none" : "set");
		return OK;
	}

	/* Return EBUSY to indicate that the timer was already running */

	tmrdbg("Error!! PIT Timer %d was already running\n", priv->channel);
	return -EBUSY;
}

static int imxrt_pit_stop(struct timer_lowerhalf_s *lower)
{
	struct imxrt_pit_lowerhalf_s *priv = (struct imxrt_pit_lowerhalf_s *)lower;

	if (priv->started) {
		imxrt_pit_disableinterrupts(priv->pit->base, priv->channel, kPIT_TimerInterruptEnable);

		imxrt_pit_stoptimer(priv->pit->base, priv->channel);
		if (ch_status[priv->channel]) {
			imxrt_pit_stoptimer(priv->pit->base, priv->channel-1);
			imxrt_pit_settimerchainmode(priv->pit->base, priv->channel, false);
			ch_status[priv->channel] = false;
			busy_status[priv->channel-1] = false;
		}

		if (active == 1) {
			imxrt_pit_setisr(priv->pit, NULL, NULL);
		}
		priv->started = false;

		busy_status[priv->channel] = false;

		active--;
		tmrvdbg("ch_status:%d/%d/%d/%d\n", ch_status[0], ch_status[1], ch_status[2], ch_status[3]);
		tmrvdbg("busy_status:%d/%d/%d/%d\n", busy_status[0], busy_status[1], busy_status[2], busy_status[3]);
		tmrvdbg("PIT Timer %d is stopped\n", priv->channel);
		return OK;
	}

	/* Return ENODEV to indicate that the timer was not running */

	tmrdbg("Error!! PIT Timer %d is not running\n", priv->channel);
	return -ENODEV;
}

static int imxrt_pit_getstatus(struct timer_lowerhalf_s *lower,
							struct timer_status_s *status)
{
	struct imxrt_pit_lowerhalf_s *priv = (struct imxrt_pit_lowerhalf_s *)lower;
	uint32_t ticks, timeout_ticks, cur_ticks;
	uint32_t freq;

	freq = imxrt_clock_getperclkfreq();

	/* Return the status bit */

	status->flags = 0;
	if (priv->started) {
		status->flags |= TCFLAGS_ACTIVE;
	}

	if (priv->callback) {
		status->flags |= TCFLAGS_HANDLER;
	}

	/* Return the actual timeout in microseconds */
	timeout_ticks = imxrt_pit_gettimerperiod(priv->pit->base, priv->channel);
	status->timeout = ((uint64_t)USEC_PER_SEC * (uint64_t)timeout_ticks) / freq;

	/* Get the time remaining until the timer expires (in microseconds). */
	cur_ticks = imxrt_pit_getcurrenttimercount(priv->pit->base, priv->channel);
	ticks = timeout_ticks - cur_ticks;
	status->timeleft = ((uint64_t)USEC_PER_SEC * (uint64_t)ticks) / freq;

	return OK;
}

static void imxrt_pit_setcallback(struct timer_lowerhalf_s *lower,
								tccb_t callback, void *arg)
{
	struct imxrt_pit_lowerhalf_s *priv = (struct imxrt_pit_lowerhalf_s *)lower;
	irqstate_t flags = irqsave();

	/* Save the new callback */
	priv->callback = callback;
	priv->arg      = arg;

	if (priv->callback != NULL) {
		imxrt_pit_setisr(priv->pit, imxrt_pit_handler, priv);
		imxrt_pit_enableinterrupts(priv->pit->base, priv->channel, kPIT_TimerInterruptEnable);
	} else {
		imxrt_pit_disableinterrupts(priv->pit->base, priv->channel, kPIT_TimerInterruptEnable);
		imxrt_pit_setisr(priv->pit, NULL, NULL);
	}

	irqrestore(flags);
}

static int imxrt_pit_ioctl(struct timer_lowerhalf_s *lower, int cmd,
						unsigned long arg)
{
	return OK;
}


int imxrt_pit_initialize(const char *devpath, int timer)
{
	pit_config_t config;
	struct imxrt_pit_lowerhalf_s *lower = NULL;

	switch (timer) {
	case IMXRT_PIT_CH0:
		lower = &g_pit0_lowerhalf;
		break;

	case IMXRT_PIT_CH1:
		lower = &g_pit1_lowerhalf;
		break;

	case IMXRT_PIT_CH2:
		lower = &g_pit2_lowerhalf;
		break;

	case IMXRT_PIT_CH3:
		lower = &g_pit3_lowerhalf;
		break;
	}

	if (!lower) {
		return -ENODEV;
	}

	/* Initialize the elements of lower half state structure */
	lower->started  = false;
	lower->callback = NULL;

	imxrt_pit_getdefaultconfig(&config);
	memcpy(&(lower->config), &config, sizeof(pit_config_t));
	lower->pit = imxrt_pit_configure(timer, &config, timer);
	lower->channel = timer;

	if (lower->pit == NULL) {
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
