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
 * Pre-proessor Definitions
 ****************************************************************************/
#define TREG_INVAL      0xFFFF

#define TCFG0           0x0000
#define TCFG1           0x0004
#define TCON            0x0008

#define TCNTB(x)        (0x000C + x * 0xC)
#define TCMPB(x)        (x < 4 ? (0x0010 + x * 0xC) : TREG_INVAL)
#define TCNTO(x)        (x < 4 ? (0x0014 + x * 0xC) : 0x0040)

#define TINT_CSTAT      0x0044

#define TIMER_PREDIVID  256

#define TIME_NS         1
#define TIME_MS         0

#define TIMER_SRC_PCLK  0
#define TIMER_SRC_SCLK  1

#define MAX_NO_PWM      5
#define TOTAL_NUMBER_OF_TIMER      10

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* sfr offset */
struct pwm_intdata {
	int pwm_no;
	int64_t int_count;
	int32_t reload;
	int32_t manual_update;
	int32_t start;
	int32_t invert;
	int32_t prescaler;
	int32_t div_mux;
};

/* This structure represents the state of one PWM timer */
struct s5j_pwmtimer_s {
	FAR const struct pwm_ops_s *ops;	/* PWM operations */

	int id;
	void *base;
	int irq;

	/* Add private data structures if it is needed */
	cycle_t timer_pclk;			/* input clock Hz */
	int pre_scaler;
	int divider;
	int freq;
	int dutycycle;				//0~100
	void *callback;

	struct pwm_intdata *intdata;
};

/****************************************************************************
 * Static Function Prototypes
 ****************************************************************************/
/* Register access */

static uint32_t pwm_getreg(struct s5j_pwmtimer_s *priv, int offset);
static void pwm_putreg(struct s5j_pwmtimer_s *priv, int offset, uint32_t value);

static void pwm_dumpregs(struct s5j_pwmtimer_s *priv, FAR const char *msg);

/* Timer management */
static int pwm_interrupt(struct s5j_pwmtimer_s *priv);
static int pwm_timer_interrupt(int irq, void *context);

/* PWM driver methods */

static int pwm_setup(FAR struct pwm_lowerhalf_s *dev);
static int pwm_shutdown(FAR struct pwm_lowerhalf_s *dev);

#ifdef CONFIG_PWM_PULSECOUNT
static int pwm_start(FAR struct pwm_lowerhalf_s *dev, FAR const struct pwm_info_s *info, FAR void *handle);
#else
static int pwm_start(FAR struct pwm_lowerhalf_s *dev, FAR const struct pwm_info_s *info);
#endif

static int pwm_stop(FAR struct pwm_lowerhalf_s *dev);
static int pwm_ioctl(FAR struct pwm_lowerhalf_s *dev, int cmd, unsigned long arg);

static int pwm_set_frequency_dutycycle(FAR struct pwm_lowerhalf_s *dev, int32_t frequency, int32_t duty);

static FAR struct pwm_lowerhalf_s *s5j_pwminitialize(int timer);

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* This is the list of lower half PWM driver methods used by the upper half driver */

static bool pwm_initialized[2] = { false, false };

static const struct pwm_ops_s g_pwmops = {
	.setup = pwm_setup,
	.shutdown = pwm_shutdown,
	.start = pwm_start,
	.stop = pwm_stop,
	.ioctl = pwm_ioctl,
};

static struct s5j_pwmtimer_s g_pwmdev[TOTAL_NUMBER_OF_TIMER];

/* PWM/Timer Definitions ****************************************************/
/* The following definitions are used to identify the various time types */

struct pwm_intdata intdata[MAX_NO_PWM] = {
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
	}, {
		.pwm_no = 3,
		.int_count = 0,
		.reload = (1 << 19),
		.manual_update = (1 << 17),
		.start = (1 << 16),
		.invert = (1 << 18),
		.prescaler = 8,
		.div_mux = 12,
	},
	{
		.pwm_no = 4,
		.int_count = 0,
		.reload = (1 << 22),
		.manual_update = (1 << 21),
		.start = (1 << 20),
		.invert = TREG_INVAL,
		.prescaler = 8,
		.div_mux = 16,
	},
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pwm_interrupt
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
static int pwm_interrupt(struct s5j_pwmtimer_s *priv)
{
	/* TODO pwm_interrupt */
	return 0;
}

/****************************************************************************
 * Name: pwm_timer_interrupt
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
static int pwm_timer_interrupt(int irq, void *context)
{
	int32_t pwm_no;
	int32_t int_stat;
	FAR struct s5j_pwmtimer_s *priv;

	pwm_no = irq - IRQ_PWM0_0;
	priv = &g_pwmdev[pwm_no];

	/* Clear interrupt pending register */
	int_stat = __raw_readl(priv->base + TINT_CSTAT) & 0x1F;
	int_stat |= (1 << (5 + pwm_no));
	__raw_writel(int_stat, priv->base + TINT_CSTAT);

	return pwm_interrupt(priv);
}

/****************************************************************************
 * Name: pwm_setup
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

static int pwm_setup(FAR struct pwm_lowerhalf_s *dev)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	//struct pwm_intdata *pwm_data = priv->intdata;

	int gpio_TOUT;

	lldbg("pwm setup\n");

	// T20 has the output port(TOUT0~6).
	if (priv->id < 7) {
		gpio_TOUT = s5j_gpio(GPP2, (priv->id));

		gpio_cfg_pin(gpio_TOUT, GPIO_FUNC(2));
		gpio_set_pull(gpio_TOUT, GPIO_PULL_NONE);
	}
	//This code enables "XGPIO27" to enable Buzz power. It should be removed because of specific board configuration.
	gpio_TOUT = s5j_gpio(GPG3, 3);
	gpio_cfg_pin(gpio_TOUT, GPIO_FUNC(1));
	gpio_set_pull(gpio_TOUT, GPIO_PULL_NONE);
	gpio_set_value(gpio_TOUT, 1);

	//__raw_writel(0x0F0F, priv->base + TCFG0);
	//__raw_writel(0x44444, priv->base + TCFG1);

	return OK;
}

/****************************************************************************
 * Name: pwm_shutdown
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

static int pwm_shutdown(FAR struct pwm_lowerhalf_s *dev)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	//struct pwm_intdata *pwm_data = priv->intdata;

	int gpio_TOUT;

	lldbg("pwm shutdown\n");

	// GPIO input mode, pull-down
	if (priv->id < 7) {
		gpio_TOUT = s5j_gpio(GPP2, (priv->id));

		gpio_cfg_pin(gpio_TOUT, GPIO_FUNC(0));
		gpio_set_pull(gpio_TOUT, GPIO_PULL_DOWN);
	}

	/* Make sure that the output has been stopped */
	pwm_stop(dev);

	return OK;
}

/****************************************************************************
 * Name: pwm_start
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
static int pwm_start(FAR struct pwm_lowerhalf_s *dev, FAR const struct pwm_info_s *info, FAR void *handle)
#else
static int pwm_start(FAR struct pwm_lowerhalf_s *dev, FAR const struct pwm_info_s *info)
#endif
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	struct pwm_intdata *pwm_data = priv->intdata;

	//unsigned long flag;
	int32_t tcon;
	int32_t int_stat;

	lldbg("pwm start\n");

#ifdef CONFIG_PWM_PULSECOUNT
	priv->callback = handle;
#endif

	pwm_set_frequency_dutycycle(dev, info->frequency, info->duty);

	/* Enable interrupt */
	int_stat = __raw_readl(priv->base + TINT_CSTAT) & 0x1F;
	int_stat |= (1 << priv->id);
	__raw_writel(int_stat, priv->base + TINT_CSTAT);

	tcon = __raw_readl(priv->base + TCON);

	/* interrupt enable */
	//up_enable_irq(priv->irq);
	/* Manual update */
	__raw_writel(tcon | pwm_data->manual_update, priv->base + TCON);

#if 0
	if (type == TIMER_TYPE_ONE_SHOT) {	/* ONESHOT */
		tcon &= ~(pwm_data->reload);
	} else						/* INTERVAL */
#endif
		tcon |= pwm_data->reload;

	tcon |= pwm_data->start;

	__raw_writel(tcon, priv->base + TCON);

	return 0;
}

/****************************************************************************
 * Name: pwm_stop
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

static int pwm_stop(FAR struct pwm_lowerhalf_s *dev)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	struct pwm_intdata *pwm_data = priv->intdata;

	//unsigned long flag;
	int32_t tcon;
	int32_t int_stat;

	lldbg("pwm stop\n");

	/* Disable interrupt */
	int_stat = __raw_readl(priv->base + TINT_CSTAT) & 0x1F;
	int_stat &= ~(1 << pwm_data->pwm_no);
	__raw_writel(int_stat, priv->base + TINT_CSTAT);

	tcon = __raw_readl(priv->base + TCON);
	tcon &= ~pwm_data->start;
	tcon &= ~pwm_data->reload;
	tcon &= ~pwm_data->invert;
	__raw_writel(tcon, priv->base + TCON);

	/* interrupt disable */
	up_disable_irq(priv->irq);

	priv->callback = NULL;

	return 0;
}

static int pwm_set_dutycycle(FAR struct pwm_lowerhalf_s *dev, int32_t duty)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	struct pwm_intdata *pwm_data = priv->intdata;
	int32_t tcntb;
	int32_t tcmpb;
	int32_t id;
	int64_t tmp;

	if (duty > 100) {
		duty = 100;
	}

	priv->dutycycle = duty;
	id = pwm_data->pwm_no;

	tcntb = __raw_readl(priv->base + TCNTB(id));

	tmp = ((tcntb + 1) * duty) / 100 - 1;	// duty range :0 ~ 100
	tcmpb = (int32_t)tmp;

	if (priv->id == 4 || priv->id == 9) {
		//There is no TBMPB register for Timer4,9
	} else {
		__raw_writel(tcmpb, priv->base + TCMPB(id));
	}

	return 0;
}

static int pwm_set_pulsewidth(FAR struct pwm_lowerhalf_s *dev, int32_t frequency)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	struct pwm_intdata *pwm_data = priv->intdata;
	int32_t tcntb;
	int32_t id;

	int32_t timerclockfreq;

	timerclockfreq = PWM_PCLK / priv->pre_scaler / priv->divider;
	if ((timerclockfreq / 2) < frequency) {
		//PWM can't be generate the freq
		frequency = timerclockfreq / 2;
	}

	priv->freq = frequency;
	id = pwm_data->pwm_no;

	tcntb = (priv->timer_pclk / priv->pre_scaler / priv->divider / frequency)
			- 1;
	__raw_writel(tcntb, priv->base + TCNTB(id));

	return 0;
}

static int pwm_set_frequency_dutycycle(FAR struct pwm_lowerhalf_s *dev, int32_t frequency, int32_t duty)
{
	pwm_set_pulsewidth(dev, frequency);
	pwm_set_dutycycle(dev, duty);

	return 0;
}

/* scaler_val : 1 ~ 256 */
static int pwm_set_prescaler(FAR struct pwm_lowerhalf_s *dev, int32_t scaler_val)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	struct pwm_intdata *pwm_data = priv->intdata;

	int i;
	int32_t id;
	int32_t tcntb;
	int32_t val;
	int32_t nPWM;
	//unsigned long flag;

	if (priv->id < 5) {
		nPWM = 0;
	} else {
		nPWM = 5;
	}

	if (pwm_data->pwm_no < 2) {
		for (i = 0; i < 2; i++) {
			g_pwmdev[nPWM + i].pre_scaler = scaler_val;

			pwm_data = priv->intdata;

			if ((__raw_readl(priv->base + TCON)
				 & (g_pwmdev[nPWM + i].intdata->start)) != 0) {
				id = g_pwmdev[nPWM + i].intdata->pwm_no;

				tcntb = (g_pwmdev[nPWM + i].timer_pclk / g_pwmdev[nPWM + i].pre_scaler / g_pwmdev[nPWM + i].divider / g_pwmdev[nPWM + i].freq)
						- 1;
				__raw_writel(tcntb, g_pwmdev[nPWM + i].base + TCNTB(id));
			}
		}
	} else {
		for (i = 2; i < 5; i++) {
			g_pwmdev[nPWM + i].pre_scaler = scaler_val;

			if ((__raw_readl(priv->base + TCON)
				 & (g_pwmdev[nPWM + i].intdata->start)) != 0) {
				id = g_pwmdev[nPWM + i].intdata->pwm_no;

				tcntb = (g_pwmdev[nPWM + i].timer_pclk / g_pwmdev[nPWM + i].pre_scaler / g_pwmdev[nPWM + i].divider / g_pwmdev[nPWM + i].freq)
						- 1;
				__raw_writel(tcntb, g_pwmdev[nPWM + i].base + TCNTB(id));
			}
		}
	}

	val = __raw_readl(priv->base + TCFG0);
	val &= ~(0xFF << pwm_data->prescaler);
	val |= ((scaler_val - 1) << pwm_data->prescaler);
	__raw_writel(val, priv->base + TCFG0);

	return 0;
}

/* scaler_val : 0 ~ 4 */
static int pwm_set_divider(FAR struct pwm_lowerhalf_s *dev, int32_t div_val)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	struct pwm_intdata *pwm_data = priv->intdata;
	//unsigned long flag;
	int32_t val;

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

	val = __raw_readl(priv->base + TCFG1);
	val &= ~(0xF << pwm_data->div_mux);
	val |= (div_val << pwm_data->div_mux);
	__raw_writel(val, priv->base + TCFG1);

	pwm_set_pulsewidth(dev, priv->freq);
	pwm_set_dutycycle(dev, priv->dutycycle);

	return 0;
}

/****************************************************************************
 * Name: pwm_ioctl
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

static int pwm_ioctl(FAR struct pwm_lowerhalf_s *dev, int cmd, unsigned long arg)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;
	int32_t val;
	int32_t tcfg0;
	//unsigned long flag;

	val = (int32_t)arg;

	lldbg("pwm ioctl: cmd: 0x%x arg:0x%x\n", arg);

	switch (cmd) {
	case PWM_SET_PRESCALER: {
		return pwm_set_prescaler(dev, val);
	}

	case PWM_SET_DIVIDER: {
		return pwm_set_divider(dev, val);
	}

	case PWM_SET_DEADZONE: {
		if (val >= 256) {
			val = 255;
		}
		tcfg0 = __raw_readl(priv->base + TCFG0);
		tcfg0 &= ~(0xFF << 16);
		tcfg0 |= (val << 16);
		__raw_writel(tcfg0, priv->base + TCFG0);
	}
	break;

	case PWM_SET_TCMP: {
		if (TCMPB(priv->id) != TREG_INVAL) {
			__raw_writel(val, priv->base + TCMPB(priv->id));
		}
	}
	break;

	case PWM_SET_DUTY_CYCLE: {
		pwm_set_dutycycle(dev, val);
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
static FAR struct pwm_lowerhalf_s *s5j_pwminitialize(int timer)
{
	FAR struct s5j_pwmtimer_s *lower;
	FAR struct s5j_pwmtimer_s *tmp;

	int32_t i;
	int32_t nPWM;
	int32_t nTimer;
	uint32_t PWM_BASE_ADDR;

	/*  lldbg("s5j_pwminitialize\n"); */

	if (timer < 5) {
		nPWM = 0;
		nTimer = timer;
		PWM_BASE_ADDR = PWM0_BASE;
	} else {
		nPWM = 1;
		nTimer = timer - 5;
		PWM_BASE_ADDR = PWM1_BASE;
	}

	/* Timer info initialization */

	/* Have we already initialized? */
	if (!pwm_initialized[nPWM]) {
		/* set prescaler to 1/16, set divider MUX to 1/16 */
		__raw_writel(0x0101, PWM_BASE_ADDR + TCFG0);
		__raw_writel(0x0000, PWM_BASE_ADDR + TCFG1);
		__raw_writel(0, PWM_BASE_ADDR + TCON);

		lower = &g_pwmdev[timer];

		for (i = 0; i < 5; i++) {
			tmp = &g_pwmdev[nPWM * 5 + i];
			tmp->intdata = &intdata[i];
			tmp->id = nPWM * 5 + i;
			tmp->irq = (IRQ_PWM0_0) + nPWM * 5 + i;

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
	if (timer < 7) {
		/* IRQ register */
		irq_attach(lower->irq, pwm_timer_interrupt);
		up_disable_irq(lower->irq);

		/* IRQ trigger = level */
		__raw_writel(0x1, 0x800A0000 + 0x044c);
	}

	return (FAR struct pwm_lowerhalf_s *)lower;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 *
 * @brief    pwm driver register (/dev/pwm0)
 * @param    void
 * @return   void
 * @note     note here
 */
void pwmdrv_register(void)
{
	struct pwm_lowerhalf_s *pwm;
	int ret;
	int ch;
	char devicename[10];

	for (ch = 0; ch < 10; ch++) {

		/* Call s5j_pwminitialize() to get an instance of the PWM interface */
		pwm = s5j_pwminitialize(ch);
		if (!pwm) {
			dbg("Failed to get the S5J PWM lower half\n");
			return;
		}

		/* Register the PWM driver at "/dev/pwm" */
		snprintf(devicename, 10, "/dev/pwm%d", ch);
		ret = pwm_register(devicename, pwm);
		if (ret < 0) {
			adbg("pwm_register failed: %d\n", ret);
			return;
		}
	}

	return;
}

/**
 *
 * @brief    pwm driver register with channel and device name
 * @param    unsigned int channel : pwm channel
 * @param    char* devicename     : the buffer of device name
 * @return    == 0: register ok
 *            <  0: register fail
 * @note     note here
 */
int s5j_board_pwm_setup(int32_t channel, char *devicename)
{
	struct pwm_lowerhalf_s *pwm;
	int ret;

	lldbg("s5j_board_pwm_setup\n");

	/* Call s5j_pwminitialize() to get an instance of the PWM interface */
	pwm = s5j_pwminitialize(channel);
	if (!pwm) {
		dbg("Failed to get the S5J PWM lower half\n");
		return -ENODEV;
	}

	/* Register the PWM driver at "/dev/pwm0" */
	ret = pwm_register(devicename, pwm);
	if (ret < 0) {
		adbg("pwm_register failed: %d\n", ret);
		return ret;
	}

	return OK;
}
