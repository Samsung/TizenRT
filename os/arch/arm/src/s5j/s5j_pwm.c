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
 *   Copyright (C) 2011-2012, 2014 Gregory Nutt. All rights reserved.
 *   Authors: Gregory Nutt <gnutt@nuttx.org>
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
#include <assert.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/pwm.h>

#include "up_arch.h"
#include "s5j_pwm.h"
#include "s5j_gpio.h"
#include "s5j_clock.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct s5j_pwmtimer_s {
	FAR const struct pwm_ops_s *ops;

	unsigned int base;
	uint8_t id;
	uint16_t pincfg;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static uint32_t pwm_getreg32(struct s5j_pwmtimer_s *priv, int offset)
{
	return getreg32(priv->base + offset);
}

static void pwm_putreg32(struct s5j_pwmtimer_s *priv, int offset, uint32_t value)
{
	putreg32(value, priv->base + offset);
}

static void pwm_modifyreg32(struct s5j_pwmtimer_s *priv, int offset,
				uint32_t clearbits, uint32_t setbits)
{
	modifyreg32(priv->base + offset, clearbits, setbits);
}

static unsigned int pwm_get_prescaler(struct s5j_pwmtimer_s *priv)
{
	unsigned int tcfg0 = pwm_getreg32(priv, S5J_PWM_TCFG0_OFFSET);

	if (priv->id == 0 || priv->id == 1) {
		return (tcfg0 & PWM_TCFG0_PRESCALER0_MASK) >> PWM_TCFG0_PRESCALER0_SHIFT;
	}

	return (tcfg0 & PWM_TCFG0_PRESCALER1_MASK) >> PWM_TCFG0_PRESCALER1_SHIFT;
}

static unsigned int pwm_get_divider(struct s5j_pwmtimer_s *priv)
{
	unsigned int tcfg1 = pwm_getreg32(priv, S5J_PWM_TCFG1_OFFSET);

	return (tcfg1 & PWM_TCFG1_DIVIDER_MUX_MASK(priv->id)) >> PWM_TCFG1_DIVIDER_MUX_SHIFT(priv->id);
}

static unsigned int pwm_clk_freq(struct s5j_pwmtimer_s *priv)
{
	return (s5j_clk_get_rate(CLK_DFT_OSCCLK) / (pwm_get_prescaler(priv) + 1))
						>> pwm_get_divider(priv);
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

	VERIFY(s5j_configgpio(priv->pincfg) == OK);

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
static int s5j_pwm_start(FAR struct pwm_lowerhalf_s *dev,
						 FAR const struct pwm_info_s *info)
{
	uint32_t tcntb;
	uint32_t tcmpb;
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;

	tcntb = pwm_clk_freq(priv) / info->frequency - 1;
	tcmpb = (((uint64_t)(tcntb + 1) * info->duty) / 65536) - 1;

	pwm_putreg32(priv, S5J_PWM_TCNTB_OFFSET(priv->id), tcntb);
	pwm_putreg32(priv, S5J_PWM_TCMPB_OFFSET(priv->id), tcmpb);

	pwm_modifyreg32(priv, S5J_PWM_TCON_OFFSET,
					PWM_TCON_TIM_MAN_UPDATE_MASK(priv->id),
					PWM_TCON_TIM_MAN_UPDATE_UPDATE(priv->id));

	/* Trigger to start */
	pwm_modifyreg32(priv, S5J_PWM_TCON_OFFSET,
					PWM_TCON_TIM_STARTSTOP_MASK(priv->id) |
					PWM_TCON_TIM_MAN_UPDATE_MASK(priv->id) |
					PWM_TCON_TIM_AUTO_RELOAD_MASK(priv->id),
					PWM_TCON_TIM_STARTSTOP_START(priv->id) |
					PWM_TCON_TIM_MAN_UPDATE_NOP(priv->id) |
					PWM_TCON_TIM_AUTO_RELOAD_ON(priv->id));

	return OK;
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

	pwm_modifyreg32(priv, S5J_PWM_TCON_OFFSET,
					PWM_TCON_TIM_STARTSTOP_MASK(priv->id),
					PWM_TCON_TIM_STARTSTOP_STOP(priv->id));

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

	/* Make sure that the output has been stopped */
	s5j_pwm_stop(dev);

	/* Then put the GPIO pins back to the default state */
	VERIFY(s5j_unconfiggpio(priv->pincfg) == OK);

	return OK;
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
static int s5j_pwm_ioctl(FAR struct pwm_lowerhalf_s *dev, int cmd,
						 unsigned long arg)
{
	return -ENOTTY;
}

/****************************************************************************
 * Name: s5j_pwm_reset
 *
 * Description:
 *   Reset pwm register value
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *   timer - A number identifying the timer use. The number of valid timer
 *     IDs varies with the S5J family but is somewhere in the range of
 *     {0,...,5}.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
static void s5j_pwm_reset(FAR struct pwm_lowerhalf_s *dev, int timer)
{
	FAR struct s5j_pwmtimer_s *priv = (FAR struct s5j_pwmtimer_s *)dev;

	if (priv->id < 2) {
		pwm_modifyreg32(priv, S5J_PWM_TCFG0_OFFSET,
						PWM_TCFG0_PRESCALER0_MASK,
						PWM_TCFG0_PRESCALER0_RESET);
	} else {
		pwm_modifyreg32(priv, S5J_PWM_TCFG0_OFFSET,
						PWM_TCFG0_PRESCALER1_MASK,
						PWM_TCFG0_PRESCALER1_RESET);
	}

	if (timer > 3) {
		timer = timer - 4;
	}

	pwm_modifyreg32(priv, S5J_PWM_TCFG1_OFFSET,
					PWM_TCFG1_DIVIDER_MUX_MASK(timer),
					PWM_TCFG1_DIVIDER_MUX_DIV1(timer));
}

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct pwm_ops_s g_pwm_ops = {
	.setup		= s5j_pwm_setup,
	.shutdown	= s5j_pwm_shutdown,
	.start		= s5j_pwm_start,
	.stop		= s5j_pwm_stop,
	.ioctl		= s5j_pwm_ioctl,
};

#ifdef CONFIG_S5J_PWM0
static struct s5j_pwmtimer_s g_pwm0_0 = {
	.ops	= &g_pwm_ops,
	.id		= 0,
	.pincfg	= GPIO_PWM_TOUT0,
	.base	= S5J_PWM0_BASE,
};
#endif

#ifdef CONFIG_S5J_PWM1
static struct s5j_pwmtimer_s g_pwm0_1 = {
	.ops	= &g_pwm_ops,
	.id		= 1,
	.pincfg	= GPIO_PWM_TOUT1,
	.base	= S5J_PWM0_BASE,
};
#endif

#ifdef CONFIG_S5J_PWM2
static struct s5j_pwmtimer_s g_pwm0_2 = {
	.ops	= &g_pwm_ops,
	.id		= 2,
	.pincfg	= GPIO_PWM_TOUT2,
	.base	= S5J_PWM0_BASE,
};
#endif

#ifdef CONFIG_S5J_PWM3
static struct s5j_pwmtimer_s g_pwm0_3 = {
	.ops	= &g_pwm_ops,
	.id		= 3,
	.pincfg	= GPIO_PWM_TOUT3,
	.base	= S5J_PWM0_BASE,
};
#endif

#ifdef CONFIG_S5J_PWM4
static struct s5j_pwmtimer_s g_pwm1_0 = {
	.ops	= &g_pwm_ops,
	.id		= 0,
	.pincfg	= GPIO_PWM_TOUT5,
	.base	= S5J_PWM1_BASE,
};
#endif

#ifdef CONFIG_S5J_PWM5
static struct s5j_pwmtimer_s g_pwm1_1 = {
	.ops	= &g_pwm_ops,
	.id		= 1,
	.pincfg	= GPIO_PWM_TOUT6,
	.base	= S5J_PWM1_BASE,
};
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5j_pwminitialize
 *
 * Description:
 *   Initialize one timer for use with the upper_level PWM driver.
 *
 * Input Parameters:
 *   timer - A number identifying the timer use. The number of valid timer
 *     IDs varies with the S5J family but is somewhere in the range of
 *     {0,...,5}.
 *
 * Returned Value:
 *   On success, a pointer to the lower-half PWM driver is returned.
 *   NULL is returned on any failure.
 *
 ****************************************************************************/
FAR struct pwm_lowerhalf_s *s5j_pwminitialize(int timer)
{
	struct pwm_lowerhalf_s *lower = NULL;

#ifdef CONFIG_S5J_PWM0
	if (timer == 0) {
		lower = (struct pwm_lowerhalf_s *)&g_pwm0_0;
	} else
#endif
#ifdef CONFIG_S5J_PWM1
	if (timer == 1) {
		lower = (struct pwm_lowerhalf_s *)&g_pwm0_1;
	} else
#endif
#ifdef CONFIG_S5J_PWM2
	if (timer == 2) {
		lower = (struct pwm_lowerhalf_s *)&g_pwm0_2;
	} else
#endif
#ifdef CONFIG_S5J_PWM3
	if (timer == 3) {
		lower = (struct pwm_lowerhalf_s *)&g_pwm0_3;
	} else
#endif
#ifdef CONFIG_S5J_PWM4
	if (timer == 4) {
		lower = (struct pwm_lowerhalf_s *)&g_pwm1_0;
	} else
#endif
#ifdef CONFIG_S5J_PWM5
	if (timer == 5) {
		lower = (struct pwm_lowerhalf_s *)&g_pwm1_1;
	} else
#endif
	{
		lldbg("ERROR: invalid PWM is requested\n");
		return NULL;
	}

	s5j_pwm_reset(lower, timer);

	return lower;
}
