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
#include <time.h>
#include <limits.h>

#include <tinyara/clock.h>
#include <tinyara/timer.h>

#include "imxrt_clock.h"
#include "imxrt_pit.h"
#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
#include "chip/imxrt102x_config.h"
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#include "chip/imxrt105x_config.h"
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

#if defined(FSL_FEATURE_PIT_HAS_CHAIN_MODE) && FSL_FEATURE_PIT_HAS_CHAIN_MODE
/* @brief Check chained status for channels */
bool ch_status[4] = {false, false, false, false,};
#endif

struct imxrt_pit_lowerhalf_s {
	const struct timer_ops_s    *ops;       /* Lowerhalf operations */
	struct imxrt_pit_chipinfo_s *pit;
	pit_config_t                 config;
	bool                         started;   /* True: Timer has been started */
	tccb_t                       callback;
	void                        *arg;       /* Argument passed to upper half callback */
};

static const struct timer_ops_s pit_timer_ops = {
	.start       = imxrt_pit_start,
	.stop        = imxrt_pit_stop,
	.getstatus   = imxrt_pit_getstatus,
	.settimeout  = imxrt_pit_settimeout,
	.setcallback = imxrt_pit_setcallback,
	.ioctl       = imxrt_pit_ioctl,
};

static struct imxrt_pit_lowerhalf_s g_pit_lowerhalf = {
	.ops		= &pit_timer_ops,
};

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

#if defined(FSL_FEATURE_PIT_TIMER_COUNT) && (FSL_FEATURE_PIT_TIMER_COUNT)
	/* Clear the timer enable bit for all channels to make sure the channel's timer is disabled. */
	for (uint8_t i = 0U; i < FSL_FEATURE_PIT_TIMER_COUNT; i++) {
		base->CHANNEL[i].TCTRL &= ~PIT_TCTRL_TEN_MASK;
	}
#endif /* FSL_FEATURE_PIT_TIMER_COUNT */

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
 * brief Start the lifetime counter
 *
 * The lifetime timer is a 64-bit timer which chains timer 0 and timer 1 together.
 * Timer 0 and 1 are chained by calling the PIT_SetTimerChainMode before using this timer.
 * The period of lifetime timer is equal to the "period of timer 0 * period of timer 1".
 *
 * param base PIT peripheral base address
 */
static inline void imxrt_pit_start_lifetimetimercount(PIT_Type *base)
{
	tmrvdbg("Start lifetime counter\n");
	base->CHANNEL[kPIT_Chnl_1].LDVAL = 0xFFFFFFFF;
	base->CHANNEL[kPIT_Chnl_1].TCTRL = 0x0;
	base->CHANNEL[kPIT_Chnl_1].TCTRL |= PIT_TCTRL_CHN_MASK;
	base->CHANNEL[kPIT_Chnl_1].TCTRL |= PIT_TCTRL_TEN_MASK;

	base->CHANNEL[kPIT_Chnl_0].LDVAL = 0xFFFFFFFF;
	base->CHANNEL[kPIT_Chnl_0].TCTRL |= PIT_TCTRL_TEN_MASK;
}

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

#endif /* FSL_FEATURE_PIT_HAS_LIFETIME_TIMER */

/****************************************************************************
 * Name: imxrt_pit_settimeout
 *
 * Description:
 *   Set a new timeout value (and reset the timer)
 *   If the timeout value is larger than 32-bit
 *   This will try to enter Chain Mode if channels
 *   available. If no channel available for Chain,
 *   return failure.
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
		imxrt_pit_settimerperiod(priv->pit->base, kPIT_Chnl_3, ticks);
#if defined(FSL_FEATURE_PIT_HAS_CHAIN_MODE) && FSL_FEATURE_PIT_HAS_CHAIN_MODE
	} else if (ticks >= UINT_MAX) {
		/* Chain mode supported on channel 2 and 3 only */
		tmrvdbg("Enter CHAIN mode\n");
		uint32_t ch_ticks1 = (uint32_t)freq / USEC_PER_SEC;
		uint32_t ch_ticks2 = (uint32_t)timeout;
		tmrvdbg("ticks:%llu ch_ticks1:%d ch_ticks2:%d\n", ticks, ch_ticks1, ch_ticks2);

		imxrt_pit_settimerchainmode(priv->pit->base, kPIT_Chnl_3, true);
		ch_status[kPIT_Chnl_3] = true;

		imxrt_pit_settimerperiod(priv->pit->base, kPIT_Chnl_3, ch_ticks1);
		imxrt_pit_settimerperiod(priv->pit->base, kPIT_Chnl_2, ch_ticks2);
	}
#endif
	return OK;
}

struct imxrt_pit_chipinfo_s *imxrt_pit_configure(pit_config_t *config)
{
	struct imxrt_pit_chipinfo_s *pit = NULL;
	pit = &imxrt_pit_chipinfo;

	imxrt_pit_init(pit->base, config);
	imxrt_pit_start_lifetimetimercount(pit->base);

	return pit;
}

void imxrt_pit_setisr(struct imxrt_pit_chipinfo_s *pit, xcpt_t handler, void *arg)
{
	irq_attach(pit->irq_id, handler, arg);

	up_enable_irq(pit->irq_id);
}

/****************************************************************************
 * Name: imxrt_pit_handler
 *
 * Description:
 *   timer interrupt handler
 *
 * Input Parameters:
 *
 * Returned Values:
 *
 ****************************************************************************/
static int imxrt_pit_handler(int irq, void *context, void *arg)
{
	struct imxrt_pit_lowerhalf_s *priv = (struct imxrt_pit_lowerhalf_s *)arg;
	struct imxrt_pit_chipinfo_s *pit = priv->pit;
	uint32_t next_interval_us = 0;
	uint32_t freq;
	uint64_t ticks;

	imxrt_pit_clearstatusflags(pit->base, kPIT_Chnl_3, kPIT_TimerFlag);
	if (ch_status[kPIT_Chnl_3])
		imxrt_pit_clearstatusflags(pit->base, kPIT_Chnl_2, kPIT_TimerFlag);

	if (priv->callback(&next_interval_us, priv->arg)) {
		if (next_interval_us > 0) {
			freq = imxrt_clock_getperclkfreq();
			ticks = ((uint64_t)freq * (uint64_t)next_interval_us) / USEC_PER_SEC;
			if (ch_status[kPIT_Chnl_3]) {
				uint32_t ch_ticks1 = (uint32_t)freq / USEC_PER_SEC;
				uint32_t ch_ticks2 = (uint32_t)next_interval_us;
				imxrt_pit_settimerperiod(priv->pit->base, kPIT_Chnl_3, ch_ticks1);
				imxrt_pit_settimerperiod(priv->pit->base, kPIT_Chnl_2, ch_ticks2);
				}
		}
	} else {
		imxrt_pit_stop((struct timer_lowerhalf_s *)priv);
	}

	return OK;
}

/****************************************************************************
 * Name: imxrt_pit_start
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
static int imxrt_pit_start(struct timer_lowerhalf_s *lower)
{
	struct imxrt_pit_lowerhalf_s *priv = (struct imxrt_pit_lowerhalf_s *)lower;

	if (!priv->started) {
		if (priv->callback != NULL) {
			imxrt_pit_enableinterrupts(priv->pit->base, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
		}

		imxrt_pit_setisr(priv->pit, imxrt_pit_handler, priv);
		imxrt_pit_starttimer(priv->pit->base, kPIT_Chnl_3);

		priv->started = true;
#if defined(FSL_FEATURE_PIT_HAS_CHAIN_MODE) && FSL_FEATURE_PIT_HAS_CHAIN_MODE
		if (ch_status[kPIT_Chnl_3])
			imxrt_pit_starttimer(priv->pit->base, kPIT_Chnl_2);

		tmrvdbg("ch_status:%d/%d/%d/%d\n", ch_status[0], ch_status[1], ch_status[2], ch_status[3]);
#endif
		tmrvdbg("PIT Timer started, callback %s\n", priv->callback == NULL ? "none" : "set");
		return OK;
	}

	/* Return EBUSY to indicate that the timer was already running */
	tmrvdbg("Error!! PIT Timer was already running\n");
	return -EBUSY;
}

/****************************************************************************
 * Name: imxrt_pit_stop
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
static int imxrt_pit_stop(struct timer_lowerhalf_s *lower)
{
	struct imxrt_pit_lowerhalf_s *priv = (struct imxrt_pit_lowerhalf_s *)lower;

	if (priv->started) {
		imxrt_pit_disableinterrupts(priv->pit->base, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
		imxrt_pit_stoptimer(priv->pit->base, kPIT_Chnl_3);
		imxrt_pit_setisr(priv->pit, NULL, NULL);

		priv->started = false;

#if defined(FSL_FEATURE_PIT_HAS_CHAIN_MODE) && FSL_FEATURE_PIT_HAS_CHAIN_MODE
		if (ch_status[kPIT_Chnl_3]) {
			imxrt_pit_stoptimer(priv->pit->base, kPIT_Chnl_2);
			imxrt_pit_settimerchainmode(priv->pit->base, kPIT_Chnl_3, false);
			ch_status[kPIT_Chnl_3] = false;
		}

		tmrvdbg("ch_status:%d/%d/%d/%d\n", ch_status[0], ch_status[1], ch_status[2], ch_status[3]);
#endif
		tmrvdbg("PIT Timer is stopped\n");
		return OK;
	}

	/* Return ENODEV to indicate that the timer was not running */

	tmrvdbg("Error!! PIT Timer is not running\n");
	return -ENODEV;
}

/****************************************************************************
 * Name: imxrt_pit_getstatus
 *
 * Description:
 *   get PIT status
 *
 * Input Parameters:
 *   lower   - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *   status  - A pointer saved current PIT status
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int imxrt_pit_getstatus(struct timer_lowerhalf_s *lower,
							struct timer_status_s *status)
{
	struct imxrt_pit_lowerhalf_s *priv = (struct imxrt_pit_lowerhalf_s *)lower;
	uint32_t timeout_ticks1, timeout_ticks2, cur_ticks1, cur_ticks2;
	uint64_t timeout_ticks, cur_ticks, timeout, timeleft;
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
	if (ch_status[kPIT_Chnl_3]) {
		timeout_ticks1 = imxrt_pit_gettimerperiod(priv->pit->base, kPIT_Chnl_3);
		timeout_ticks2 = imxrt_pit_gettimerperiod(priv->pit->base, kPIT_Chnl_2);
		timeout_ticks = (uint64_t)timeout_ticks1 * (uint64_t)timeout_ticks2;
	} else {
		timeout_ticks = imxrt_pit_gettimerperiod(priv->pit->base, kPIT_Chnl_3);
	}
	timeout = ((uint64_t)USEC_PER_SEC * (uint64_t)timeout_ticks) / freq;
	if (timeout > UINT32_MAX) {
		tmrdbg("timeout:%lld converted to UINT32_MAX\n", timeout);
		timeout = UINT32_MAX;
	}
	status->timeout = (uint32_t)timeout;

	/* Get the time remaining until the timer expires (in microseconds). */
	if (ch_status[kPIT_Chnl_3]) {
		cur_ticks1 = imxrt_pit_getcurrenttimercount(priv->pit->base, kPIT_Chnl_3);
		cur_ticks2 = imxrt_pit_getcurrenttimercount(priv->pit->base, kPIT_Chnl_2);
		cur_ticks = (uint64_t)cur_ticks1 * (uint64_t)cur_ticks2;
	} else {
		cur_ticks = imxrt_pit_getcurrenttimercount(priv->pit->base, kPIT_Chnl_3);
	}
	timeleft = ((uint64_t)USEC_PER_SEC * (uint64_t)cur_ticks) / freq;
	if (timeleft > UINT32_MAX) {
		tmrdbg("timeleft:%lld converted to UINT32_MAX\n", timeleft);
		timeout = UINT32_MAX;
	}
	status->timeleft = (uint32_t)timeleft;

	return OK;
}

/****************************************************************************
 * Name: imxrt_pit_setcallback
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
static void imxrt_pit_setcallback(struct timer_lowerhalf_s *lower,
								tccb_t callback, void *arg)
{
	struct imxrt_pit_lowerhalf_s *priv = (struct imxrt_pit_lowerhalf_s *)lower;
	irqstate_t flags = irqsave();

	/* Save the new callback */
	priv->callback = callback;
	priv->arg      = arg;

	if (priv->callback != NULL) {
		tmrvdbg("callback is not NULL\n");
		imxrt_pit_setisr(priv->pit, imxrt_pit_handler, priv);
		imxrt_pit_enableinterrupts(priv->pit->base, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
	} else {
		tmrvdbg("callback is NULL\n");
		imxrt_pit_disableinterrupts(priv->pit->base, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
		imxrt_pit_setisr(priv->pit, NULL, NULL);
	}

	irqrestore(flags);
}

/****************************************************************************
 * Name: imxrt_pit_ioctl
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
static int imxrt_pit_ioctl(struct timer_lowerhalf_s *lower, int cmd,
						unsigned long arg)
{
	struct imxrt_pit_lowerhalf_s *priv = (struct imxrt_pit_lowerhalf_s *)lower;
	uint64_t total_time;
	uint32_t freq;
	uint64_t ticks;
	struct tm *l_time;

	tmrvdbg("PIT IOCTL cmd: %d, arg: %ld\n", cmd, arg);

	switch (cmd) {
	case TCIOC_GETLIFETIME:
		freq = imxrt_clock_getperclkfreq();
		ticks = imxrt_pit_getlifetimetimercount(priv->pit->base);
		total_time = ((ULLONG_MAX - ticks) * USEC_PER_SEC) / freq;

		total_time = total_time / USEC_PER_SEC;

		l_time = localtime((const time_t *)&total_time);
		memcpy((uintptr_t)arg, l_time, sizeof(struct tm));
		break;
	default:
		tmrdbg("Invalid cmd %d\n", cmd);
		return -EINVAL;
	}
	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int imxrt_pit_initialize(const char *devpath)
{
	pit_config_t config;
	struct imxrt_pit_lowerhalf_s *lower = NULL;
	lower = &g_pit_lowerhalf;

	/* Initialize the elements of lower half state structure */
	lower->started  = false;
	lower->callback = NULL;

	imxrt_pit_getdefaultconfig(&config);
	memcpy(&(lower->config), &config, sizeof(pit_config_t));
	lower->pit = imxrt_pit_configure(&config);

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
