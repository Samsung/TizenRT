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

#include <arch/chip/mcpwm_struct.h>
#include <tinyara/pwm.h>
#include <tinyara/kmalloc.h>

#include "periph_ctrl.h"
#include "chip/esp32_soc.h"
#include "chip/esp32_gpio_sigmap.h"
#include "chip/esp32_iomux.h"
#include "chip/esp32_gpio.h"
#include "esp32_pwm.h"
#include "esp32_gpio.h"
#include "rom/esp32_gpio.h"

#define MCPWM_BASE_CLK (2 * APB_CLK_FREQ)	//2*APB_CLK_FREQ 160Mhz
#define MCPWM_CLK_PRESCL 15		//MCPWM clock prescale
#define TIMER_CLK_PRESCALE 9	//MCPWM timer prescales
#define MCPWM_CLK (MCPWM_BASE_CLK / (MCPWM_CLK_PRESCL + 1))
#define OFFSET_FOR_GPIO_IDX_1  6
#define OFFSET_FOR_GPIO_IDX_2 75

#define GPIO_PWM0A_OUT 18
#define GPIO_PWM0B_OUT 19

/****************************************************************************
 * Private Types
 ****************************************************************************/
static mcpwm_dev_t *MCPWM[2] = {&MCPWM0, &MCPWM1};

mcpwm_config_t pwm0_hw_conf;

struct esp32_pwmtimer_s {
	FAR const struct pwm_ops_s *ops;
	mcpwm_unit_t id;
	mcpwm_timer_t timer_num;
	mcpwm_pin_config_t *pinconf;
	mcpwm_config_t *hwconf;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
int mcpwm_start(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num)
{
	MCPWM[mcpwm_num]->timer[timer_num].mode.start = 2;
	return OK;
}

int mcpwm_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num)
{
	MCPWM[mcpwm_num]->timer[timer_num].mode.start = 0;
	return OK;
}

int mcpwm_gpio_init(mcpwm_unit_t mcpwm_num, mcpwm_io_signals_t io_signal, int gpio_num)
{
	periph_module_enable(PERIPH_PWM0_MODULE + mcpwm_num);

	bool mcpwm_gpio_sig = (io_signal <= MCPWM2B);
	if (mcpwm_num == MCPWM_UNIT_0) {
		if (mcpwm_gpio_sig) {
			esp32_configgpio(gpio_num, OUTPUT_FUNCTION_2);
			gpio_matrix_out(gpio_num, PWM0_OUT0A_IDX + io_signal, 0, 0);
		} else {
			esp32_configgpio(gpio_num, INPUT_FUNCTION_2);
			gpio_matrix_in(gpio_num, PWM0_SYNC0_IN_IDX + io_signal - OFFSET_FOR_GPIO_IDX_1, 0);
		}
	} else {					//MCPWM_UNIT_1
		if (mcpwm_gpio_sig) {
			esp32_configgpio(gpio_num, OUTPUT_FUNCTION_2);
			gpio_matrix_out(gpio_num, PWM1_OUT0A_IDX + io_signal, 0, 0);
		} else if (io_signal >= MCPWM_SYNC_0 && io_signal < MCPWM_FAULT_2) {
			esp32_configgpio(gpio_num, INPUT_FUNCTION_2);
			gpio_matrix_in(gpio_num, PWM1_SYNC0_IN_IDX + io_signal - OFFSET_FOR_GPIO_IDX_1, 0);
		} else {
			esp32_configgpio(gpio_num, INPUT_FUNCTION_2);
			gpio_matrix_in(gpio_num, PWM1_SYNC0_IN_IDX + io_signal - OFFSET_FOR_GPIO_IDX_2, 0);
		}
	}
	return OK;
}

int mcpwm_set_pin(mcpwm_unit_t mcpwm_num, const mcpwm_pin_config_t *mcpwm_pin)
{
	mcpwm_gpio_init(mcpwm_num, MCPWM0A, mcpwm_pin->mcpwm0a_out_num);	//MCPWM0A
	mcpwm_gpio_init(mcpwm_num, MCPWM0B, mcpwm_pin->mcpwm0b_out_num);	//MCPWM0B
	return OK;
}

int mcpwm_set_duty(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, mcpwm_operator_t op_num, ub16_t duty)
{

	uint32_t previous_period = MCPWM[mcpwm_num]->timer[timer_num].period.period;
	uint32_t set_duty = previous_period * duty / 100;
	MCPWM[mcpwm_num]->channel[timer_num].cmpr_value[op_num].cmpr_val = set_duty;
	MCPWM[mcpwm_num]->channel[timer_num].cmpr_cfg.a_upmethod = BIT(0);
	MCPWM[mcpwm_num]->channel[timer_num].cmpr_cfg.b_upmethod = BIT(0);
	return OK;
}

int mcpwm_set_frequency(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, uint32_t frequency)
{
	uint32_t mcpwm_num_of_pulse;
	uint32_t previous_period;
	uint32_t set_duty_a, set_duty_b;
	mcpwm_num_of_pulse = MCPWM_CLK / (frequency * (TIMER_CLK_PRESCALE + 1));
	previous_period = MCPWM[mcpwm_num]->timer[timer_num].period.period;
	MCPWM[mcpwm_num]->timer[timer_num].period.prescale = TIMER_CLK_PRESCALE;
	MCPWM[mcpwm_num]->timer[timer_num].period.period = mcpwm_num_of_pulse;
	MCPWM[mcpwm_num]->timer[timer_num].period.upmethod = 0;
	set_duty_a = (((MCPWM[mcpwm_num]->channel[timer_num].cmpr_value[0].cmpr_val) * mcpwm_num_of_pulse) / previous_period);
	set_duty_b = (((MCPWM[mcpwm_num]->channel[timer_num].cmpr_value[1].cmpr_val) * mcpwm_num_of_pulse) / previous_period);
	MCPWM[mcpwm_num]->channel[timer_num].cmpr_value[0].cmpr_val = set_duty_a;
	MCPWM[mcpwm_num]->channel[timer_num].cmpr_value[1].cmpr_val = set_duty_b;
	MCPWM[mcpwm_num]->channel[timer_num].cmpr_cfg.a_upmethod = 0;
	MCPWM[mcpwm_num]->channel[timer_num].cmpr_cfg.b_upmethod = 0;
	return OK;
}

int mcpwm_set_duty_type(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, mcpwm_operator_t op_num, mcpwm_duty_type_t duty_num)
{
	if (op_num == MCPWM_OPR_A) {
		if (MCPWM[mcpwm_num]->timer[timer_num].mode.mode == MCPWM_UP_COUNTER) {
			if (duty_num == MCPWM_DUTY_MODE_1) {
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utez = 1;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utea = 2;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utep = 0;
			} else {			//MCPWM_DUTY_MODE_0
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utez = 2;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utea = 1;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utep = 0;
			}
		} else if (MCPWM[mcpwm_num]->timer[timer_num].mode.mode == MCPWM_DOWN_COUNTER) {
			if (duty_num == MCPWM_DUTY_MODE_1) {
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dtep = 2;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dtea = 1;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dtez = 0;
			} else {			//MCPWM_DUTY_MODE_0
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dtep = 1;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dtea = 2;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dtez = 0;
			}
		} else {				//Timer count up-down
			if (duty_num == MCPWM_DUTY_MODE_1) {
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utez = 1;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utea = 2;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dtea = 1;
			} else {			//MCPWM_DUTY_MODE_0
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utez = 2;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utea = 1;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dtea = 2;
			}

		}
	}

	if (op_num == MCPWM_OPR_B) {
		if (MCPWM[mcpwm_num]->timer[timer_num].mode.mode == MCPWM_UP_COUNTER) {
			if (duty_num == MCPWM_DUTY_MODE_1) {
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utez = 1;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].uteb = 2;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utep = 0;
			} else {			//MCPWM_DUTY_MODE_0
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utez = 2;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].uteb = 1;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utep = 0;
			}
		} else if (MCPWM[mcpwm_num]->timer[timer_num].mode.mode == MCPWM_DOWN_COUNTER) {
			if (duty_num == MCPWM_DUTY_MODE_1) {
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dtep = 2;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dteb = 1;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dtez = 0;
			} else {			//MCPWM_DUTY_MODE_0
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dtep = 1;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dteb = 2;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dtez = 0;
			}
		} else {				//Timer count up-down
			if (duty_num == MCPWM_DUTY_MODE_1) {
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utez = 1;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].uteb = 2;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dteb = 1;
			} else {			//MCPWM_DUTY_MODE_0
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].utez = 2;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].uteb = 1;
				MCPWM[mcpwm_num]->channel[timer_num].generator[op_num].dteb = 2;
			}
		}
	}
	return OK;
}

int mcpwm_init(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, const mcpwm_config_t *mcpwm_conf)
{
	periph_module_enable(PERIPH_PWM0_MODULE + mcpwm_num);

	MCPWM[mcpwm_num]->clk_cfg.prescale = MCPWM_CLK_PRESCL;
	mcpwm_set_frequency(mcpwm_num, timer_num, mcpwm_conf->frequency);
	MCPWM[mcpwm_num]->timer[timer_num].mode.mode = mcpwm_conf->counter_mode;

	mcpwm_set_duty(mcpwm_num, timer_num, 0, mcpwm_conf->cmpr_a);
	mcpwm_set_duty(mcpwm_num, timer_num, 1, mcpwm_conf->cmpr_b);
	mcpwm_set_duty_type(mcpwm_num, timer_num, 0, mcpwm_conf->duty_mode);
	mcpwm_set_duty_type(mcpwm_num, timer_num, 1, mcpwm_conf->duty_mode);
	mcpwm_start(mcpwm_num, timer_num);
	MCPWM[mcpwm_num]->timer_sel.operator0_sel = 0;
	MCPWM[mcpwm_num]->timer_sel.operator1_sel = 1;
	MCPWM[mcpwm_num]->timer_sel.operator2_sel = 2;
	MCPWM[mcpwm_num]->update_cfg.global_up_en = 1;
	MCPWM[mcpwm_num]->update_cfg.global_force_up = 1;
	MCPWM[mcpwm_num]->update_cfg.global_force_up = 0;
	return OK;
}

/****************************************************************************
 * Name: esp32_pwm_setup
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

static int esp32_pwm_setup(FAR struct pwm_lowerhalf_s *dev)
{
	FAR struct esp32_pwmtimer_s *priv = (FAR struct esp32_pwmtimer_s *)dev;
	mcpwm_set_pin(priv->id, priv->pinconf);
	return OK;
}

/****************************************************************************
 * Name: esp32_pwm_start
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
static int esp32_pwm_start(FAR struct pwm_lowerhalf_s *dev, FAR const struct pwm_info_s *info)
{
	FAR struct esp32_pwmtimer_s *priv = (FAR struct esp32_pwmtimer_s *)dev;

	pwm0_hw_conf.frequency = info->frequency;
	pwm0_hw_conf.cmpr_a = info->duty;
	pwm0_hw_conf.cmpr_b = info->duty;

	mcpwm_init(priv->id, priv->timer_num, priv->hwconf);
	return OK;
}

/****************************************************************************
 * Name: esp32_pwm_stop
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
static int esp32_pwm_stop(FAR struct pwm_lowerhalf_s *dev)
{
	FAR struct esp32_pwmtimer_s *priv = (FAR struct esp32_pwmtimer_s *)dev;
	mcpwm_stop(priv->id, priv->timer_num);
	return OK;
}

/****************************************************************************
 * Name: esp32_pwm_shutdown
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
static int esp32_pwm_shutdown(FAR struct pwm_lowerhalf_s *dev)
{
	FAR struct esp32_pwmtimer_s *priv = (FAR struct esp32_pwmtimer_s *)dev;
	/* Make sure that the output has been stopped */
	esp32_pwm_stop(dev);
	periph_module_disable(PERIPH_PWM0_MODULE + priv->id);
	return OK;
}

/****************************************************************************
 * Name: esp32_pwm_ioctl
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
static int esp32_pwm_ioctl(FAR struct pwm_lowerhalf_s *dev, int cmd, unsigned long arg)
{
	return -ENOTTY;
}

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct pwm_ops_s g_pwm_ops = {
	.setup = esp32_pwm_setup,
	.shutdown = esp32_pwm_shutdown,
	.start = esp32_pwm_start,
	.stop = esp32_pwm_stop,
	.ioctl = esp32_pwm_ioctl,
};

mcpwm_pin_config_t pwm0_pin_config = {
	.mcpwm0a_out_num = GPIO_PWM0A_OUT,
	.mcpwm0b_out_num = GPIO_PWM0B_OUT,
};

mcpwm_config_t pwm0_hw_conf = {
	.frequency = 1000,			//frequency = 1000Hz
	.cmpr_a = 60,				//duty cycle of PWMxA = 60.0%
	.cmpr_b = 50,				//duty cycle of PWMxb = 50.0%
	.counter_mode = MCPWM_UP_COUNTER,
	.duty_mode = MCPWM_DUTY_MODE_0,
};

static struct esp32_pwmtimer_s g_pwm0_0 = {
	.ops = &g_pwm_ops,
	.id = 0,
	.timer_num = 0,
	.pinconf = &pwm0_pin_config,
	.hwconf = &pwm0_hw_conf,
};

static struct esp32_pwmtimer_s g_pwm0_1 = {
	.ops = &g_pwm_ops,
	.id = 1,
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: esp32_pwminitialize
 *
 * Description:
 *   Initialize one timer for use with the upper_level PWM driver.
 *
 * Input Parameters:
 *   timer - A number identifying the timer use. The number of valid timer
 *     IDs varies with the ESP32 family but is somewhere in the range of
 *     {0,...,5}.
 *
 * Returned Value:
 *   On success, a pointer to the lower-half PWM driver is returned.
 *   NULL is returned on any failure.
 *
 ****************************************************************************/
FAR struct pwm_lowerhalf_s *esp32_pwminitialize(int id)
{
	struct pwm_lowerhalf_s *lower = NULL;
	if (id == 0) {
		lower = (struct pwm_lowerhalf_s *)&g_pwm0_0;
	} else {
		lower = (struct pwm_lowerhalf_s *)&g_pwm0_1;
	}

	return lower;
}
