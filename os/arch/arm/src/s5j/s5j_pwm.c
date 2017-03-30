/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/s5j_pwm.c
 *
 *   Copyright (C) 2013-2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * The Samsung sample code has a BSD compatible license that requires this
 * copyright notice:
 *
 *   Copyright (c) 2016 Samsung Electronics, Inc.
 *   All rights reserved.
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
 */
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/pwm.h>

#include <arch/chip/irq.h>
#include <arch/chip/chip_types.h>

#include "chip.h"
#include "up_arch.h"

#include "s5j_pwm.h"

/****************************************************************************
 * Static Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* This is the list of lower half PWM driver methods used by the upper half driver */

static bool pwm_initialized[2] = { false, false };

static const struct pwm_ops_s g_pwmops = {
	.setup = s5j_pwm_setup,
	.shutdown = s5j_pwm_shutdown,
	.start = s5j_pwm_start,
	.stop = s5j_pwm_stop,
	.ioctl = s5j_pwm_ioctl,
};

static struct s5j_pwmtimer_s g_pwmdev[TOTAL_NUMBER_OF_TIMER];

/* PWM/Timer Definitions ****************************************************/
/* The following definitions are used to identify the various time types */

struct s5j_pwm_intdata intdata[MAX_NO_PWM] = {
	{
		.pwm_no = 0,
		.int_count = 0,
		.reload = (1 << 3),
		.manual_update = (1 << 1),
		.start = (1 << 0),
		.invert = (1 << 2),
		.prescaler = 0,
		.div_mux = 0,
	},
	{
		.pwm_no = 1,
		.int_count = 0,
		.reload = (1 << 11),
		.manual_update = (1 << 9),
		.start = (1 << 8),
		.invert = (1 << 10),
		.prescaler = 0,
		.div_mux = 4,
	},
	{
		.pwm_no = 2,
		.int_count = 0,
		.reload = (1 << 15),
		.manual_update = (1 << 13),
		.start = (1 << 12),
		.invert = (1 << 14),
		.prescaler = 8,
		.div_mux = 8,
	},
	{
		.pwm_no = 3,
		.int_count = 0,
		.reload = (1 << 19),
		.manual_update = (1 << 17),
		.start = (1 << 16),
		.invert = (1 << 18),
		.prescaler = 8,
		.div_mux = 12,
	},
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5j_pwm_interrupt
 *
 * Description:
 *   Handle timer interrupts.
 *
 * Input parameters:
 *   priv - A reference to the lower half PWM driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/
static int s5j_pwm_interrupt(struct s5j_pwmtimer_s *priv)
{
	return 0;
}

/****************************************************************************
 * Name: s5j_pwm_timer_interrupt
 *
 * Description:
 *   Handle pwm 0..4 interrupts.
 *
 * Input parameters:
 *   Standard NuttX interrupt inputs
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/
static int s5j_pwm_timer_interrupt(int irq, void *context, void *arg)
{
	signed int pwm_no;
	signed int irq_no;
	signed int int_stat;
	FAR struct s5j_pwmtimer_s *priv;

	irq_no = irq - IRQ_PWM0_0;

	if (irq_no > 4) {
		pwm_no = irq_no - 1;
	} else {
		pwm_no = irq_no;
	}

	priv = &g_pwmdev[pwm_no];

	/* Clear interrupt pending register */
	int_stat = getreg32(priv->base + TINT_CSTAT) & 0x1F;
	int_stat |= (1 << (5 + irq_no));
	putreg32(int_stat, priv->base + TINT_CSTAT);

	return s5j_pwm_interrupt(priv);
}

/****************************************************************************
 * Name: s5j_pwm_setup
 *
 * Description:
 *   This method is called when the driver is opened.  The lower half driver
 *   should configure and initialize the device so that it is ready for use.
 *   It should not, however, output pulses until the start method is called.
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 * Assumptions:
 *   APB1 or 2 clocking for the GPIOs has already been configured by the RCC
 *   logic at power up.
 *
 ****************************************************************************/

static int s5j_pwm_setup(FAR struct pwm_lowerhalf_s *dev)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;

	int gpio_TOUT;
	int channel;

	channel = priv->id;

	//skip channel4
	if (channel >= 4) {
		channel += 1;
	}
	// T20 has the output port(TOUT0~6).
	if (channel < 7) {
		gpio_TOUT = s5j_gpio(GPP2, (channel));

		gpio_cfg_pin(gpio_TOUT, GPIO_FUNC(2));
		gpio_set_pull(gpio_TOUT, GPIO_PULL_NONE);
	}

	return OK;
}

/****************************************************************************
 * Name: s5j_pwm_shutdown
 *
 * Description:
 *   This method is called when the driver is closed.  The lower half driver
 *   stop pulsed output, free any resources, disable the timer hardware, and
 *   put the system into the lowest possible power usage state
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/

static int s5j_pwm_shutdown(FAR struct pwm_lowerhalf_s *dev)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;

	int gpio_TOUT;
	int channel;

	channel = priv->id;

	//skip channel4
	if (channel >= 4) {
		channel += 1;
	}
	// GPIO input mode, pull-down
	if (channel < 7) {
		gpio_TOUT = s5j_gpio(GPP2, channel);

		gpio_cfg_pin(gpio_TOUT, GPIO_FUNC(0));
		gpio_set_pull(gpio_TOUT, GPIO_PULL_DOWN);
	}

	/* Make sure that the output has been stopped */
	s5j_pwm_stop(dev);

	return OK;
}

/****************************************************************************
 * Name: s5j_pwm_start
 *
 * Description:
 *   (Re-)initialize the timer resources and start the pulsed output
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *   info - A reference to the characteristics of the pulsed output
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/
#ifdef CONFIG_PWM_PULSECOUNT
static int s5j_pwm_start(FAR struct pwm_lowerhalf_s *dev, FAR const struct pwm_info_s *info, FAR void *handle)
#else
static int s5j_pwm_start(FAR struct pwm_lowerhalf_s *dev, FAR const struct pwm_info_s *info)
#endif
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	struct s5j_pwm_intdata *pwm_data = priv->intdata;

	signed int tcon, int_stat;
#ifdef CONFIG_PWM_PULSECOUNT
	priv->callback = handle;
#endif

	s5j_pwm_set_frequency_dutycycle(dev, info->frequency, info->duty);

	/* Enable interrupt */
	int_stat = getreg32(priv->base + TINT_CSTAT) & 0x1F;
	int_stat |= (1 << priv->id);
	putreg32(int_stat, priv->base + TINT_CSTAT);

	tcon = getreg32(priv->base + TCON);

	/* interrupt enable */
	//up_enable_irq(priv->irq);

	/* Manual update */
	putreg32(tcon | pwm_data->manual_update, priv->base + TCON);

#if 0
	if (type == TIMER_TYPE_ONE_SHOT) {	/* ONESHOT */
		tcon &= ~(pwm_data->reload);
	} else						/* INTERVAL */
#endif

		tcon |= pwm_data->reload;

	tcon |= pwm_data->start;

	putreg32(tcon, priv->base + TCON);

	return 0;
}

/****************************************************************************
 * Name: s5j_pwm_stop
 *
 * Description:
 *   Stop the pulsed output and reset the timer resources
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 * Assumptions:
 *   This function is called to stop the pulsed output at anytime.  This
 *   method is also called from the timer interrupt handler when a repetition
 *   count expires... automatically stopping the timer.
 *
 ****************************************************************************/

static int s5j_pwm_stop(FAR struct pwm_lowerhalf_s *dev)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	struct s5j_pwm_intdata *pwm_data = priv->intdata;

	signed int tcon, int_stat;

	/* Disable interrupt */
	int_stat = getreg32(priv->base + TINT_CSTAT) & 0x1F;
	int_stat &= ~(1 << pwm_data->pwm_no);
	putreg32(int_stat, priv->base + TINT_CSTAT);

	tcon = getreg32(priv->base + TCON);
	tcon &= ~pwm_data->start;
	tcon &= ~pwm_data->reload;
	tcon &= ~pwm_data->invert;
	putreg32(tcon, priv->base + TCON);

	/* interrupt disable */
	up_disable_irq(priv->irq);

	priv->callback = NULL;

	return 0;
}

static int s5j_pwm_set_dutycycle(FAR struct pwm_lowerhalf_s *dev, signed int duty)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	struct s5j_pwm_intdata *pwm_data = priv->intdata;
	signed int tcntb;
	signed int tcmpb;
	signed int id;
	int64_t tmp;

	if (duty > 100) {
		duty = 100;
	}

	priv->dutycycle = duty;
	id = pwm_data->pwm_no;

	tcntb = getreg32(priv->base + TCNTB(id));

	tmp = ((tcntb + 1) * duty) / 100 - 1;	// duty range :0 ~ 100
	tcmpb = (signed int)tmp;

	putreg32(tcmpb, priv->base + TCMPB(id));

	return 0;
}

static int s5j_pwm_set_pulsewidth(FAR struct pwm_lowerhalf_s *dev, signed int frequency)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	struct s5j_pwm_intdata *pwm_data = priv->intdata;
	signed int tcntb;
	signed int id;

	signed int timerclockfreq;

	timerclockfreq = PWM_PCLK / priv->pre_scaler / priv->divider;
	if ((timerclockfreq / 2) < frequency) {
		//PWM can't be generate the freq
		frequency = timerclockfreq / 2;
	}

	priv->freq = frequency;
	id = pwm_data->pwm_no;

	tcntb = (priv->timer_pclk / priv->pre_scaler / priv->divider / frequency) - 1;
	putreg32(tcntb, priv->base + TCNTB(id));

	return 0;
}

static int s5j_pwm_set_frequency_dutycycle(FAR struct pwm_lowerhalf_s *dev, signed int frequency, signed int duty)
{
	s5j_pwm_set_pulsewidth(dev, frequency);
	s5j_pwm_set_dutycycle(dev, duty);

	return 0;
}

/* scaler_val : 1 ~ 256 */
static int s5j_pwm_set_prescaler(FAR struct pwm_lowerhalf_s *dev, signed int scaler_val)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	struct s5j_pwm_intdata *pwm_data = priv->intdata;

	int i;
	signed int id;
	signed int tcntb;
	signed int val;
	signed int nPWM;
	//unsigned long flag;

	if (priv->id < 4) {
		nPWM = 0;
	} else {
		nPWM = 4;
	}

	if (pwm_data->pwm_no < 2) {
		for (i = 0; i < 2; i++) {
			g_pwmdev[nPWM + i].pre_scaler = scaler_val;

			pwm_data = priv->intdata;

			if ((getreg32(priv->base + TCON) & (g_pwmdev[nPWM + i].intdata->start)) != 0) {
				id = g_pwmdev[nPWM + i].intdata->pwm_no;

				tcntb = (g_pwmdev[nPWM + i].timer_pclk / g_pwmdev[nPWM + i].pre_scaler / g_pwmdev[nPWM + i].divider / g_pwmdev[nPWM + i].freq) - 1;
				putreg32(tcntb, g_pwmdev[nPWM + i].base + TCNTB(id));
			}
		}
	} else {
		for (i = 2; i < 4; i++) {
			g_pwmdev[nPWM + i].pre_scaler = scaler_val;

			if ((getreg32(priv->base + TCON) & (g_pwmdev[nPWM + i].intdata->start)) != 0) {
				id = g_pwmdev[nPWM + i].intdata->pwm_no;

				tcntb = (g_pwmdev[nPWM + i].timer_pclk / g_pwmdev[nPWM + i].pre_scaler / g_pwmdev[nPWM + i].divider / g_pwmdev[nPWM + i].freq) - 1;
				putreg32(tcntb, g_pwmdev[nPWM + i].base + TCNTB(id));
			}
		}
	}

	val = getreg32(priv->base + TCFG0);
	val &= ~(0xFF << pwm_data->prescaler);
	val |= ((scaler_val - 1) << pwm_data->prescaler);
	putreg32(val, priv->base + TCFG0);

	return 0;
}

/* scaler_val : 0 ~ 4 */
static int s5j_pwm_set_divider(FAR struct pwm_lowerhalf_s *dev, signed int div_val)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	struct s5j_pwm_intdata *pwm_data = priv->intdata;
	signed int val;

	switch (div_val) {
	case 0: {
		priv->divider = 1;
	}
	break;
	case 1: {
		priv->divider = 2;
	}
	break;
	case 2: {
		priv->divider = 4;
	}
	break;
	case 3: {
		priv->divider = 8;
	}
	break;
	case 4: {
		priv->divider = 16;
	}
	break;
	default:
		return -EINVAL;
	}

	val = getreg32(priv->base + TCFG1);
	val &= ~(0xF << pwm_data->div_mux);
	val |= (div_val << pwm_data->div_mux);
	putreg32(val, priv->base + TCFG1);

	s5j_pwm_set_pulsewidth(dev, priv->freq);
	s5j_pwm_set_dutycycle(dev, priv->dutycycle);

	return 0;
}

/****************************************************************************
 * Name: s5j_pwm_ioctl
 *
 * Description:
 *   Lower-half logic may support platform-specific ioctl commands
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *   cmd - The ioctl command
 *   arg - The argument accompanying the ioctl command
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/

static int s5j_pwm_ioctl(FAR struct pwm_lowerhalf_s *dev, int cmd, unsigned long arg)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	signed int val, tcfg0;

	val = (signed int)arg;

	switch (cmd) {
	case PWM_SET_PRESCALER: {
		return s5j_pwm_set_prescaler(dev, val);
	}

	case PWM_SET_DIVIDER: {
		return s5j_pwm_set_divider(dev, val);
	}

	case PWM_SET_DEADZONE: {
		if (val >= 256) {
			val = 255;
		}
		tcfg0 = getreg32(priv->base + TCFG0);
		tcfg0 &= ~(0xFF << 16);
		tcfg0 |= (val << 16);
		putreg32(tcfg0, priv->base + TCFG0);
	}
	break;

	case PWM_SET_TCMP: {
		if (TCMPB(priv->id) != TREG_INVAL) {
			putreg32(val, priv->base + TCMPB(priv->id));
		}
	}
	break;

	case PWM_SET_DUTY_CYCLE: {
		s5j_pwm_set_dutycycle(dev, val);
	}
	break;

	default:
		break;
	}

	lldbg("TIMER%d\n", priv->id);

	return 0;
}

/****************************************************************************
 * Name: s5j_pwminitialize
 *
 * Description:
 *   Initialize one timer for use with the upper_level PWM driver.
 *
 * Input Parameters:
 *   timer - A number identifying the timer use.  The number of valid timer
 *     IDs varies with the S5J MCU and MCU family but is somewhere in
 *     the range of {1,..,14}.
 *
 * Returned Value:
 *   On success, a pointer to the S5J lower half PWM driver is returned.
 *   NULL is returned on any failure.
 *
 ****************************************************************************/
FAR struct pwm_lowerhalf_s *s5j_pwminitialize(int timer)
{
	struct s5j_pwmtimer_s *lower, *tmp;

	unsigned int i;
	unsigned int nPWM;
	unsigned int nTimer;
	unsigned int PWM_BASE_ADDR;

	if (timer < 4) {
		nPWM = 0;
		nTimer = timer;
		PWM_BASE_ADDR = PWM0_BASE;
	} else {
		nPWM = 1;
		nTimer = timer - 4;
		PWM_BASE_ADDR = PWM1_BASE;
	}

	/* Timer info initialization */

	/* Have we already initialized? */
	if (!pwm_initialized[nPWM]) {
		/* set prescaler to 1/16, set divider MUX to 1/16 */
		putreg32(0x0101, PWM_BASE_ADDR + TCFG0);
		putreg32(0x0000, PWM_BASE_ADDR + TCFG1);
		putreg32(0, PWM_BASE_ADDR + TCON);

		lower = &g_pwmdev[timer];

		for (i = 0; i < 4; i++) {
			tmp = &g_pwmdev[nPWM * 4 + i];
			tmp->intdata = &intdata[i];
			tmp->id = nPWM * 4 + i;
			tmp->irq = (IRQ_PWM0_0) + nPWM * 4 + i;

			tmp->base = (void *)PWM_BASE_ADDR;

			tmp->ops = &g_pwmops;

			/* clock setting or clock information */
			/* it should be replaced with cal code. */
			tmp->timer_pclk = PWM_PCLK;

			tmp->pre_scaler = 2;
			tmp->divider = 1;

			/* set default frenquency : 100hz */
			tmp->freq = 100;

			/* set default duty cycle: 50% */
			tmp->dutycycle = 50;
		}

		/* Now we are initialized */
		pwm_initialized[nPWM] = true;
	} else {
		lower = &g_pwmdev[timer];
		//frequency
		lower->freq = lower->timer_pclk / (lower->pre_scaler) / lower->divider;
	}

	/* There is only 6 IRQ ID (PWM0_0 ~ PWM1_1 */
	if (timer < TOTAL_NUMBER_OF_PWMOUT) {
		/* IRQ register */
		irq_attach(lower->irq, s5j_pwm_timer_interrupt, NULL);
		up_disable_irq(lower->irq);

		/* IRQ trigger = level */
		putreg32(0x1, 0x800A0000 + 0x044c);
	}

	return (FAR struct pwm_lowerhalf_s *)lower;
}
