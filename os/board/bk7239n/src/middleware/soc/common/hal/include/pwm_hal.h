// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_config.h"
#include "pwm_hw.h"
#include "pwm_ll.h"
#include <driver/hal/hal_pwm_types.h>

typedef struct {
	pwm_hw_t *hw;
	pwm_unit_t id;
} pwm_hal_t;

typedef struct {
	uint32_t t1;
	uint32_t t2;
	uint32_t t3;
	uint32_t t4;
} pwm_hal_config_t;

typedef struct {
	pwm_capture_edge_t edge;
} pwm_hal_capture_config_t;

typedef struct {
} pwm_hal_timer_config_t;

typedef struct {
} pwm_hal_int_config_t;

bk_err_t pwm_hal_init(pwm_hal_t *hal);

bk_err_t pwm_hal_init_pwm(pwm_hal_t *hal, pwm_chan_t chan, const pwm_hal_config_t *config);

bk_err_t pwm_hal_start_common(pwm_hal_t *hal, pwm_chan_t chan);
bk_err_t pwm_hal_stop_common(pwm_hal_t *hal, pwm_chan_t chan);

bk_err_t pwm_hal_init_capture(pwm_hal_t *hal, pwm_chan_t chan, const pwm_hal_capture_config_t *config);
#define pwm_hal_get_capture_value(hal, chan) pwm_ll_get_capture_value((hal)->hw, chan)

bk_err_t pwm_hal_timer_set_config(pwm_hal_t *hal, pwm_chan_t chan, const pwm_hal_timer_config_t *config);
bk_err_t pwm_hal_timer_start(pwm_hal_t *hal, pwm_chan_t chan);
bk_err_t pwm_hal_timer_stop(pwm_hal_t *hal, pwm_chan_t chan);

#define pwm_hal_set_mode_timer(hal, chan) pwm_ll_set_mode_timer((hal)->hw, chan)
#define pwm_hal_enable_interrupt(hal, chan) pwm_ll_enable_interrupt((hal)->hw, chan)
#define pwm_hal_disable_interrupt(hal, chan) pwm_ll_disable_interrupt((hal)->hw, chan)
#define pwm_hal_get_interrupt_status(hal) pwm_ll_get_interrupt_status((hal)->hw)
#define pwm_hal_clear_interrupt_status(hal, status) pwm_ll_clear_interrupt_status((hal)->hw, (status))
#define pwm_hal_clear_chan_interrupt_status(hal, chan) pwm_ll_clear_chan_interrupt_status((hal)->hw, (chan))
#define pwm_hal_is_interrupt_triggered(hal, chan, status) pwm_ll_is_interrupt_triggered((hal)->hw, (chan), (status))

#define pwm_hal_is_chan_started(hal, chan) pwm_ll_is_chan_started((hal)->hw, (chan))

#define pwm_hal_set_t1(hal, chan, t1) pwm_ll_set_t1((hal)->hw, (chan), (t1))
#define pwm_hal_set_t2(hal, chan, t2) pwm_ll_set_t2((hal)->hw, (chan), (t2))
#define pwm_hal_set_t3(hal, chan, t3) pwm_ll_set_t3((hal)->hw, (chan), (t3))
#define pwm_hal_set_t4(hal, chan, t4) pwm_ll_set_t4((hal)->hw, (chan), (t4))
#define pwm_hal_set_init_signal_low(hal, chan) pwm_ll_set_init_signal_low((hal)->hw, (chan))
#define pwm_hal_set_init_signal_high(hal, chan) pwm_ll_set_init_signal_high((hal)->hw, (chan))

#define pwm_hal_get_t1(hal, chan) pwm_ll_get_t1((hal)->hw, (chan))
#define pwm_hal_get_t4(hal, chan) pwm_ll_get_t4((hal)->hw, (chan))

#define pwm_hal_is_duty_valid(period, t1, t2, t3) pwm_ll_is_duty_valid((period), (t1), (t2), (t3))
#define pwm_hal_is_capture_edge_valid(edge) pwm_ll_is_capture_edge_valid((edge))

#define pwm_hal_reset_config_to_default(hal, chan) pwm_ll_reset_config_to_default((hal)->hw, (chan))
#define pwm_hal_load_new_config(hal, chan) pwm_ll_load_new_config((hal)->hw, (chan))

#define pwm_hal_is_hardware_group(hal, chan1, chan2) pwm_ll_is_hardware_group((hal)->hw, (chan1), (chan2))
#define pwm_hal_start_hardware_group(hal, chan1, chan2) pwm_ll_start_hardware_group((hal)->hw, (chan1), (chan2))
#define pwm_hal_stop_hardware_group(hal, chan1, chan2) pwm_ll_stop_hardware_group((hal)->hw, (chan1), (chan2))

#define pwm_hal_is_pwm2_interrupt(hal, chan) pwm_ll_is_pwm2_interrupt((hal)->hw, (chan))
#if CFG_HAL_DEBUG_PWM
void pwm_struct_dump(void);
#else
#define pwm_struct_dump()
#endif

#ifdef __cplusplus
}
#endif
