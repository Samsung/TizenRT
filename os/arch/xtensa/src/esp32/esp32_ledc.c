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

#include <arch/chip/ledc_struct.h>
#include <tinyara/ledc.h>
#include <tinyara/kmalloc.h>
#include <tinyara/irq.h>

#include "chip/esp32_ledc.h"
#include "periph_ctrl.h"
#include "chip/esp32_soc.h"
#include "chip/esp32_gpio_sigmap.h"
#include "chip/esp32_iomux.h"
#include "chip/esp32_gpio.h"
#include "esp32_ledc.h"
#include "esp32_gpio.h"
#include "rom/esp32_gpio.h"

#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_GPIO       (18)
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_HS_CH1_GPIO       (19)
#define LEDC_HS_CH1_CHANNEL    LEDC_CHANNEL_1

#define LEDC_LS_TIMER          LEDC_TIMER_1
#define LEDC_LS_MODE           LEDC_LOW_SPEED_MODE
#define LEDC_LS_CH2_GPIO       (4)
#define LEDC_LS_CH2_CHANNEL    LEDC_CHANNEL_2
#define LEDC_LS_CH3_GPIO       (5)
#define LEDC_LS_CH3_CHANNEL    LEDC_CHANNEL_3

#define LEDC_TEST_CH_NUM       (4)
#define LEDC_TEST_DUTY         (4000)
#define LEDC_TEST_FADE_TIME    (3000)

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct esp32_ledctimer_s {
	FAR const struct ledc_ops_s *ops;
	ledc_timer_config_t *tmrcfg;
	ledc_channel_config_t *chlcfg;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
typedef struct {
	uint16_t speed_mode;
	uint16_t direction;
	uint32_t target_duty;
	int cycle_num;
	int scale;
	ledc_fade_mode_t mode;
} ledc_fade_t;

static ledc_fade_t *s_ledc_fade_rec[LEDC_SPEED_MODE_MAX][LEDC_CHANNEL_MAX];

#define LEDC_VAL_NO_CHANGE        (-1)
#define LEDC_STEP_NUM_MAX         (1023)
#define LEDC_DUTY_DECIMAL_BIT_NUM (4)
#define LEDC_HPOINT_VAL_MAX       (LEDC_HPOINT_HSCH1_V)

static int ledc_fade_channel_init(ledc_mode_t speed_mode, ledc_channel_t channel)
{
	if (s_ledc_fade_rec[speed_mode][channel] == NULL) {
		s_ledc_fade_rec[speed_mode][channel] = (ledc_fade_t *)kmm_calloc(1, sizeof(ledc_fade_t));
	}
	if (s_ledc_fade_rec[speed_mode][channel]) {

		return OK;
	} else {
		return -1;
	}
}

static int ledc_fade_channel_deinit(ledc_mode_t speed_mode, ledc_channel_t channel)
{
	if (s_ledc_fade_rec[speed_mode][channel]) {
		kmm_free(s_ledc_fade_rec[speed_mode][channel]);
		s_ledc_fade_rec[speed_mode][channel] = NULL;
	}
	return OK;
}

static void ledc_ls_timer_update(ledc_mode_t speed_mode, ledc_timer_t timer_sel)
{
	if (speed_mode == LEDC_LOW_SPEED_MODE) {
		LEDC.timer_group[speed_mode].timer[timer_sel].conf.low_speed_update = 1;
	}
}

static IRAM_ATTR void ledc_ls_channel_update(ledc_mode_t speed_mode, ledc_channel_t channel_num)
{
	if (speed_mode == LEDC_LOW_SPEED_MODE) {
		LEDC.channel_group[speed_mode].channel[channel_num].conf0.low_speed_update = 1;
	}
}

static int ledc_enable_intr_type(ledc_mode_t speed_mode, uint32_t channel, ledc_intr_type_t type)
{
	uint32_t value;
	uint32_t intr_type = type;
	value = LEDC.int_ena.val;
	uint8_t int_en_base = LEDC_DUTY_CHNG_END_HSCH0_INT_ENA_S;
	if (speed_mode == LEDC_LOW_SPEED_MODE) {
		int_en_base = LEDC_DUTY_CHNG_END_LSCH0_INT_ENA_S;
	}
	if (intr_type == LEDC_INTR_FADE_END) {
		LEDC.int_ena.val = value | BIT(int_en_base + channel);
	} else {
		LEDC.int_ena.val = (value & (~(BIT(int_en_base + channel))));
	}
	return OK;
}

static int ledc_get_max_duty(ledc_mode_t speed_mode, ledc_channel_t channel)
{
	// The arguments are checked before internally calling this function.
	int timer_sel = LEDC.channel_group[speed_mode].channel[channel].conf0.timer_sel;
	int max_duty = (1 << (LEDC.timer_group[speed_mode].timer[timer_sel].conf.duty_resolution));
	return max_duty;
}

int ledc_timer_set(ledc_mode_t speed_mode, ledc_timer_t timer_sel, uint32_t clock_divider, uint32_t duty_resolution, ledc_clk_src_t clk_src)
{
	LEDC.timer_group[speed_mode].timer[timer_sel].conf.clock_divider = clock_divider;
	LEDC.timer_group[speed_mode].timer[timer_sel].conf.tick_sel = clk_src;
	LEDC.timer_group[speed_mode].timer[timer_sel].conf.duty_resolution = duty_resolution;
	ledc_ls_timer_update(speed_mode, timer_sel);
	return OK;
}

static IRAM_ATTR int ledc_duty_config(ledc_mode_t speed_mode, ledc_channel_t channel_num, int hpoint_val, int duty_val, uint32_t duty_direction, uint32_t duty_num, uint32_t duty_cycle, uint32_t duty_scale)
{
	if (hpoint_val >= 0) {
		LEDC.channel_group[speed_mode].channel[channel_num].hpoint.hpoint = hpoint_val & LEDC_HPOINT_HSCH1_V;
	}
	if (duty_val >= 0) {
		LEDC.channel_group[speed_mode].channel[channel_num].duty.duty = duty_val;
	}
	LEDC.channel_group[speed_mode].channel[channel_num].conf1.val = ((duty_direction & LEDC_DUTY_INC_HSCH0_V) << LEDC_DUTY_INC_HSCH0_S) | ((duty_num & LEDC_DUTY_NUM_HSCH0_V) << LEDC_DUTY_NUM_HSCH0_S) | ((duty_cycle & LEDC_DUTY_CYCLE_HSCH0_V) << LEDC_DUTY_CYCLE_HSCH0_S) | ((duty_scale & LEDC_DUTY_SCALE_HSCH0_V) << LEDC_DUTY_SCALE_HSCH0_S);
	ledc_ls_channel_update(speed_mode, channel_num);
	return OK;
}

int ledc_bind_channel_timer(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t timer_idx)
{
	LEDC.channel_group[speed_mode].channel[channel].conf0.timer_sel = timer_idx;
	ledc_ls_channel_update(speed_mode, channel);
	return OK;
}

int ledc_timer_rst(ledc_mode_t speed_mode, uint32_t timer_sel)
{
	LEDC.timer_group[speed_mode].timer[timer_sel].conf.rst = 1;
	LEDC.timer_group[speed_mode].timer[timer_sel].conf.rst = 0;
	ledc_ls_timer_update(speed_mode, timer_sel);
	return OK;
}

int ledc_timer_config(const ledc_timer_config_t *timer_conf)
{
	uint32_t freq_hz = timer_conf->freq_hz;
	uint32_t duty_resolution = timer_conf->duty_resolution;
	uint32_t timer_num = timer_conf->timer_num;
	uint32_t speed_mode = timer_conf->speed_mode;
	periph_module_enable(PERIPH_LEDC_MODULE);

	int ret = OK;
	uint32_t precision = (0x1 << duty_resolution);	// 2**depth
	// Try calculating divisor based on LEDC_APB_CLK
	ledc_clk_src_t timer_clk_src = LEDC_APB_CLK;
	// div_param is a Q10.8 fixed point value
	uint64_t div_param = ((uint64_t) LEDC_APB_CLK_HZ << 8) / freq_hz / precision;
	if (div_param < 256) {
		// divisor is too low
		ret = -1;
	}
	if (div_param > LEDC_DIV_NUM_HSTIMER0_V) {
		// APB_CLK results in divisor which too high. Try using REF_TICK as clock source.
		timer_clk_src = LEDC_REF_TICK;
		div_param = ((uint64_t) LEDC_REF_CLK_HZ << 8) / freq_hz / precision;
		if (div_param < 256 || div_param > LEDC_DIV_NUM_HSTIMER0_V) {
			ret = -1;
		}
	} else {
		if (speed_mode == LEDC_LOW_SPEED_MODE) {
			//for now, we only select 80mhz for slow clk of LEDC low speed channels.
			LEDC.conf.slow_clk_sel = 1;
		}
	}
	// set timer parameters
	ledc_timer_set(speed_mode, timer_num, div_param, duty_resolution, timer_clk_src);
	// reset timer
	ledc_timer_rst(speed_mode, timer_num);
	return ret;
}

int ledc_channel_config(const ledc_channel_config_t *ledc_conf)
{
	uint32_t speed_mode = ledc_conf->speed_mode;
	uint32_t gpio_num = ledc_conf->gpio_num;
	uint32_t ledc_channel = ledc_conf->channel;
	uint32_t timer_select = ledc_conf->timer_sel;
	uint32_t intr_type = ledc_conf->intr_type;
	uint32_t duty = ledc_conf->duty;
	uint32_t hpoint = ledc_conf->hpoint;

	periph_module_enable(PERIPH_LEDC_MODULE);

	int ret = OK;
	/*set channel parameters */
	/*   channel parameters decide how the waveform looks like in one period */
	/*   set channel duty and hpoint value, duty range is (0 ~ ((2 ** duty_resolution) - 1)), max hpoint value is 0xfffff */
	ledc_set_duty_with_hpoint(speed_mode, ledc_channel, duty, hpoint);
	/*update duty settings */
	ledc_update_duty(speed_mode, ledc_channel);
	/*bind the channel with the timer */
	ledc_bind_channel_timer(speed_mode, ledc_channel, timer_select);
	/*set interrupt type */
	ledc_enable_intr_type(speed_mode, ledc_channel, intr_type);
	//printf("LEDC_PWM CHANNEL %1u|GPIO %02u|Duty %04u|Time %01u",
	//  ledc_channel, gpio_num, duty, timer_select);
	/*set LEDC signal in gpio matrix */
	esp32_configgpio(gpio_num, OUTPUT_FUNCTION_2);
	if (speed_mode == LEDC_HIGH_SPEED_MODE) {
		gpio_matrix_out(gpio_num, LEDC_HS_SIG_OUT0_IDX + ledc_channel, 0, 0);
	} else {
		gpio_matrix_out(gpio_num, LEDC_LS_SIG_OUT0_IDX + ledc_channel, 0, 0);
	}
	return ret;
}

int ledc_update_duty(ledc_mode_t speed_mode, ledc_channel_t channel)
{
	LEDC.channel_group[speed_mode].channel[channel].conf0.sig_out_en = 1;
	LEDC.channel_group[speed_mode].channel[channel].conf1.duty_start = 1;
	ledc_ls_channel_update(speed_mode, channel);
	return OK;
}

int ledc_stop(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t idle_level)
{
	LEDC.channel_group[speed_mode].channel[channel].conf0.idle_lv = idle_level & 0x1;
	LEDC.channel_group[speed_mode].channel[channel].conf0.sig_out_en = 0;
	LEDC.channel_group[speed_mode].channel[channel].conf1.duty_start = 0;
	ledc_ls_channel_update(speed_mode, channel);
	return OK;
}

int ledc_set_duty_with_hpoint(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty, uint32_t hpoint)
{
	/* The channel configuration should not be changed before the fade operation is done. */
	ledc_duty_config(speed_mode, channel,	//uint32_t chan_num,
					 hpoint,	//uint32_t hpoint_val,
					 duty << 4,	//uint32_t duty_val,the least 4 bits are decimal part
					 1,			//uint32_t increase,
					 1,			//uint32_t duty_num,
					 1,			//uint32_t duty_cycle,
					 0			//uint32_t duty_scale
					);
	return OK;
}

int ledc_set_duty(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty)
{
	/* The channel configuration should not be changed before the fade operation is done. */
	ledc_duty_config(speed_mode, channel,	//uint32_t chan_num,
					 LEDC_VAL_NO_CHANGE, duty << 4,	//uint32_t duty_val,the least 4 bits are decimal part
					 1,			//uint32_t increase,
					 1,			//uint32_t duty_num,
					 1,			//uint32_t duty_cycle,
					 0			//uint32_t duty_scale
					);
	return OK;
}

void IRAM_ATTR ledc_fade_isr(void *arg)
{
	int channel;
	uint32_t intr_status = LEDC.int_st.val;	//read LEDC interrupt status.
	LEDC.int_clr.val = intr_status;	//clear LEDC interrupt status.
	int speed_mode = LEDC_HIGH_SPEED_MODE;
	for (channel = 0; channel < LEDC_CHANNEL_MAX; channel++) {
		if (intr_status & (BIT(LEDC_DUTY_CHNG_END_HSCH0_INT_ST_S + channel) | BIT(LEDC_DUTY_CHNG_END_LSCH0_INT_ST_S + channel))) {
			if (intr_status & BIT(LEDC_DUTY_CHNG_END_HSCH0_INT_ST_S + channel)) {
				speed_mode = LEDC_HIGH_SPEED_MODE;
			} else {
				speed_mode = LEDC_LOW_SPEED_MODE;
			}
			if (s_ledc_fade_rec[speed_mode][channel] == NULL) {
				//fade object not initialized yet.
				continue;
			}
			uint32_t duty_cur = LEDC.channel_group[speed_mode].channel[channel].duty_rd.duty_read >> LEDC_DUTY_DECIMAL_BIT_NUM;
			if (duty_cur == s_ledc_fade_rec[speed_mode][channel]->target_duty) {
				continue;
			}
			uint32_t duty_tar = s_ledc_fade_rec[speed_mode][channel]->target_duty;
			int scale = s_ledc_fade_rec[speed_mode][channel]->scale;
			if (scale == 0) {
				continue;
			}
			int cycle = s_ledc_fade_rec[speed_mode][channel]->cycle_num;
			int delta = s_ledc_fade_rec[speed_mode][channel]->direction == LEDC_DUTY_DIR_DECREASE ? duty_cur - duty_tar : duty_tar - duty_cur;
			int step = delta / scale > LEDC_STEP_NUM_MAX ? LEDC_STEP_NUM_MAX : delta / scale;
			if (delta > scale) {
				ledc_duty_config(speed_mode, channel, LEDC_VAL_NO_CHANGE, duty_cur << LEDC_DUTY_DECIMAL_BIT_NUM, s_ledc_fade_rec[speed_mode][channel]->direction, step, cycle, scale);
			} else {
				ledc_duty_config(speed_mode, channel, LEDC_VAL_NO_CHANGE, duty_tar << LEDC_DUTY_DECIMAL_BIT_NUM, s_ledc_fade_rec[speed_mode][channel]->direction, 1, 1, 0);
			}
			LEDC.channel_group[speed_mode].channel[channel].conf1.duty_start = 1;
		}
	}
	LEDC.int_clr.val = intr_status;	//clear LEDC interrupt status.
}

static int _ledc_set_fade_with_step(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t target_duty, int scale, int cycle_num)
{

	uint32_t duty_cur = LEDC.channel_group[speed_mode].channel[channel].duty_rd.duty_read >> LEDC_DUTY_DECIMAL_BIT_NUM;
	// When duty == max_duty, meanwhile, if scale == 1 and fade_down == 1, counter would overflow.
	if (duty_cur == ledc_get_max_duty(speed_mode, channel)) {
		duty_cur -= 1;
	}
	s_ledc_fade_rec[speed_mode][channel]->speed_mode = speed_mode;
	s_ledc_fade_rec[speed_mode][channel]->target_duty = target_duty;
	s_ledc_fade_rec[speed_mode][channel]->cycle_num = cycle_num;
	s_ledc_fade_rec[speed_mode][channel]->scale = scale;
	int step_num = 0;
	int dir = LEDC_DUTY_DIR_DECREASE;
	if (scale > 0) {
		if (duty_cur > target_duty) {
			s_ledc_fade_rec[speed_mode][channel]->direction = LEDC_DUTY_DIR_DECREASE;
			step_num = (duty_cur - target_duty) / scale;
			step_num = step_num > LEDC_STEP_NUM_MAX ? LEDC_STEP_NUM_MAX : step_num;
		} else {
			s_ledc_fade_rec[speed_mode][channel]->direction = LEDC_DUTY_DIR_INCREASE;
			dir = LEDC_DUTY_DIR_INCREASE;
			step_num = (target_duty - duty_cur) / scale;
			step_num = step_num > LEDC_STEP_NUM_MAX ? LEDC_STEP_NUM_MAX : step_num;
		}
	}

	if (scale > 0 && step_num > 0) {
		ledc_duty_config(speed_mode, channel, LEDC_VAL_NO_CHANGE, duty_cur << 4, dir, step_num, cycle_num, scale);
		//printf("cur duty: %d; target: %d, step: %d, cycle: %d; scale: %d; dir: %d\n",
		//      duty_cur, target_duty, step_num, cycle_num, scale, dir);
	} else {
		ledc_duty_config(speed_mode, channel, LEDC_VAL_NO_CHANGE, target_duty << 4, dir, 0, 1, 0);
		//  printf("Set to target duty: %d", target_duty);
	}
	return OK;
}

uint32_t ledc_get_freq(ledc_mode_t speed_mode, ledc_timer_t timer_num)
{
	uint32_t freq = 0;
	uint32_t timer_source_clk = LEDC.timer_group[speed_mode].timer[timer_num].conf.tick_sel;
	uint32_t duty_resolution = LEDC.timer_group[speed_mode].timer[timer_num].conf.duty_resolution;
	uint32_t clock_divider = LEDC.timer_group[speed_mode].timer[timer_num].conf.clock_divider;
	uint32_t precision = (0x1 << duty_resolution);
	if (timer_source_clk == LEDC_APB_CLK) {
		freq = ((uint64_t) LEDC_APB_CLK_HZ << 8) / precision / clock_divider;
	} else {
		freq = ((uint64_t) LEDC_REF_CLK_HZ << 8) / precision / clock_divider;
	}
	return freq;
}

int ledc_set_fade_with_time(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t target_duty, int max_fade_time_ms)
{

	ledc_fade_channel_init(speed_mode, channel);

	int timer_sel = LEDC.channel_group[speed_mode].channel[channel].conf0.timer_sel;
	uint32_t freq = ledc_get_freq(speed_mode, timer_sel);
	uint32_t duty_cur = LEDC.channel_group[speed_mode].channel[channel].duty_rd.duty_read >> LEDC_DUTY_DECIMAL_BIT_NUM;
	uint32_t duty_delta = target_duty > duty_cur ? target_duty - duty_cur : duty_cur - target_duty;

	if (duty_delta == 0) {
		return _ledc_set_fade_with_step(speed_mode, channel, target_duty, 0, 0);
	}
	int total_cycles = max_fade_time_ms * freq / 1000;
	if (total_cycles == 0) {
		//ESP_LOGW(LEDC_TAG, LEDC_FADE_TOO_FAST_STR);
		return _ledc_set_fade_with_step(speed_mode, channel, target_duty, 0, 0);
	}
	int scale, cycle_num;
	if (total_cycles > duty_delta) {
		scale = 1;
		cycle_num = total_cycles / duty_delta;
		if (cycle_num > LEDC_DUTY_NUM_HSCH0_V) {
			//  ESP_LOGW(LEDC_TAG, LEDC_FADE_TOO_SLOW_STR);
			cycle_num = LEDC_DUTY_NUM_HSCH0_V;
		}
	} else {
		cycle_num = 1;
		scale = duty_delta / total_cycles;
	}
	return _ledc_set_fade_with_step(speed_mode, channel, target_duty, scale, cycle_num);
}

int ledc_fade_start(ledc_mode_t speed_mode, ledc_channel_t channel, ledc_fade_mode_t fade_mode)
{
	s_ledc_fade_rec[speed_mode][channel]->mode = fade_mode;
	// Clear interrupt status of channel
	int duty_resolution_ch0 = (speed_mode == LEDC_HIGH_SPEED_MODE) ? LEDC_DUTY_CHNG_END_HSCH0_INT_ENA_S : LEDC_DUTY_CHNG_END_LSCH0_INT_ENA_S;
	LEDC.int_clr.val |= BIT(duty_resolution_ch0 + channel);
	// Enable interrupt for channel
	ledc_enable_intr_type(speed_mode, channel, LEDC_INTR_FADE_END);
	ledc_update_duty(speed_mode, channel);
	return OK;
}

int ledc_fade_func_install()
{
	return irq_attach(ETS_LEDC_INTR_SOURCE, (xcpt_t) ledc_fade_isr, NULL);
}

void ledc_fade_func_uninstall()
{

	int channel, mode;
	for (mode = 0; mode < LEDC_SPEED_MODE_MAX; mode++) {
		for (channel = 0; channel < LEDC_CHANNEL_MAX; channel++) {
			ledc_fade_channel_deinit(mode, channel);
		}
	}
	return;
}

/****************************************************************************
 * Name: esp32_ledc_setup
 *
 * Description:
 *   This method is called when the driver is opened.  The lower half driver
 *   should configure and initialize the device so that it is ready for use.
 *   It should not, however, output pulses until the start method is called.
 *
 * Input parameters:
 *   dev - A reference to the lower half LEDC driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 * Assumptions:
 *   APB1 or 2 clocking for the GPIOs has already been configured by the RCC
 *   logic at power up.
 *
 ****************************************************************************/

static int esp32_ledc_setup(FAR struct ledc_lowerhalf_s *dev)
{
	FAR struct esp32_ledctimer_s *priv = (FAR struct esp32_ledctimer_s *)dev;

	ledc_timer_config(priv->tmrcfg);
	ledc_channel_config(priv->chlcfg);
	ledc_fade_func_install();
	return OK;
}

/****************************************************************************
 * Name: esp32_ledc_start
 *
 * Description:
 *   (Re-)initialize the timer resources and start the pulsed output
 *
 * Input parameters:
 *   dev - A reference to the lower half LEDC driver state structure
 *   info - A reference to the characteristics of the pulsed output
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/
static int esp32_ledc_start(FAR struct ledc_lowerhalf_s *dev, FAR const struct ledc_info_s *info)
{
	FAR struct esp32_ledctimer_s *priv = (FAR struct esp32_ledctimer_s *)dev;

	if (info->duty != priv->chlcfg->duty) {
		priv->chlcfg->duty = info->duty;
	}

	if (info->ishwfade) {
		ledc_set_fade_with_time(priv->chlcfg->speed_mode, priv->chlcfg->channel, priv->chlcfg->duty, LEDC_TEST_FADE_TIME);
		ledc_fade_start(priv->chlcfg->speed_mode, priv->chlcfg->channel, LEDC_FADE_NO_WAIT);
	} else {
		ledc_set_duty(priv->chlcfg->speed_mode, priv->chlcfg->channel, priv->chlcfg->duty);
		ledc_update_duty(priv->chlcfg->speed_mode, priv->chlcfg->channel);
	}
	return OK;
}

/****************************************************************************
 * Name: esp32_ledc_stop
 *
 * Description:
 *   Stop the pulsed output and reset the timer resources
 *
 * Input parameters:
 *   dev - A reference to the lower half LEDC driver state structure
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
static int esp32_ledc_stop(FAR struct ledc_lowerhalf_s *dev)
{
	FAR struct esp32_ledctimer_s *priv = (FAR struct esp32_ledctimer_s *)dev;
	ledc_stop(priv->chlcfg->speed_mode, priv->chlcfg->channel, 1);
	return OK;
}

/****************************************************************************
 * Name: esp32_ledc_shutdown
 *
 * Description:
 *   This method is called when the driver is closed.  The lower half driver
 *   stop pulsed output, free any resources, disable the timer hardware, and
 *   put the system into the lowest possible power usage state
 *
 * Input parameters:
 *   dev - A reference to the lower half LEDC driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/
static int esp32_ledc_shutdown(FAR struct ledc_lowerhalf_s *dev)
{
	FAR struct esp32_ledctimer_s *priv = (FAR struct esp32_ledctimer_s *)dev;
	/* Make sure that the output has been stopped */
	ledc_fade_func_uninstall();
	ledc_stop(priv->chlcfg->speed_mode, priv->chlcfg->channel, 1);
	periph_module_disable(PERIPH_LEDC_MODULE);
	return OK;
}

/****************************************************************************
 * Name: esp32_ledc_ioctl
 *
 * Description:
 *   Lower-half logic may support platform-specific ioctl commands
 *
 * Input parameters:
 *   dev - A reference to the lower half LEDC driver state structure
 *   cmd - The ioctl command
 *   arg - The argument accompanying the ioctl command
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/
static int esp32_ledc_ioctl(FAR struct ledc_lowerhalf_s *dev, int cmd, unsigned long arg)
{
	return -ENOTTY;
}

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct ledc_ops_s g_ledc_ops = {
	.setup = esp32_ledc_setup,
	.shutdown = esp32_ledc_shutdown,
	.start = esp32_ledc_start,
	.stop = esp32_ledc_stop,
	.ioctl = esp32_ledc_ioctl,
};

static ledc_timer_config_t ledc_timer = {
	.duty_resolution = LEDC_TIMER_13_BIT,	// resolution of PWM duty
	.freq_hz = 5000,			// frequency of PWM signal
	.speed_mode = LEDC_HS_MODE,	// timer mode
	.timer_num = LEDC_HS_TIMER,	// timer index
};

static ledc_channel_config_t ledc_channel = {
	.channel = LEDC_HS_CH0_CHANNEL,
	.duty = 50,
	.gpio_num = LEDC_HS_CH0_GPIO,
	.speed_mode = LEDC_HS_MODE,
	.timer_sel = LEDC_HS_TIMER
};

static struct esp32_ledctimer_s g_ledc = {
	.ops = &g_ledc_ops,
	.tmrcfg = &ledc_timer,
	.chlcfg = &ledc_channel,
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: esp32_ledcinitialize
 *
 * Description:
 *   Initialize one timer for use with the upper_level LEDC driver.
 *
 * Input Parameters:
 *   timer - A number identifying the timer use. The number of valid timer
 *     IDs varies with the ESP32 family but is somewhere in the range of
 *     {0,...,5}.
 *
 * Returned Value:
 *   On success, a pointer to the lower-half LEDC driver is returned.
 *   NULL is returned on any failure.
 *
 ****************************************************************************/
FAR struct ledc_lowerhalf_s *esp32_ledcinitialize()
{
	return (struct ledc_lowerhalf_s *)&g_ledc;
}
