// Copyright 2022-2025 Beken
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

#include <soc/soc.h>
#include "pwm_ll.h"
#include "driver/pwm_types.h"
#include <driver/hal/hal_pwm_types.h>

#ifdef __cplusplus
extern "C" {
#endif

bk_err_t pwm_hal_init(void);
bk_err_t pwm_hal_deinit(void);
bk_err_t pwm_hal_chan_alloc_io(pwm_chan_t sw_ch, pwm_chan_t tim_ch);
bk_err_t pwm_hal_chan_alloc_capture_io(pwm_chan_t sw_ch, pwm_chan_t tim_cap_ch);
bk_err_t pwm_hal_set_polarity(pwm_chan_t sw_ch, pwm_polarity_t polarity);
bk_err_t pwm_hal_set_ocm(pwm_chan_t sw_ch, uint8_t ocm_val);
bk_err_t pwm_hal_set_init_level(pwm_chan_t sw_ch, uint8_t init_level);
bk_err_t pwm_hal_set_ocpe(pwm_chan_t sw_ch, uint8_t ocpe_val);
bk_err_t pwm_hal_set_arpe(pwm_chan_t sw_ch, uint8_t arpe_val);
bk_err_t pwm_hal_set_ocmpe(pwm_chan_t sw_ch, uint8_t ocmpe_val);
bk_err_t pwm_hal_set_single_chan_tim_enable(pwm_chan_t sw_ch, uint32_t value);
bk_err_t pwm_hal_set_multi_chan_tim_enable(uint32_t multi_ch, uint32_t value);
bk_err_t pwm_hal_set_single_chan_enable(pwm_chan_t sw_ch, uint32_t value);
bk_err_t pwm_hal_set_chan_config(pwm_chan_t sw_ch, const pwm_period_duty_config_t *config);
bk_err_t pwm_hal_set_group_config(const pwm_period_duty_config_t *config, pwm_chan_t tim_id, uint32_t dead_cycle);
bk_err_t pwm_hal_set_uie(pwm_chan_t tim_id, uint32_t value);
bk_err_t pwm_hal_set_cc1ie(pwm_chan_t tim_id, uint32_t value);
#if defined(CONFIG_BK7239N_MP) && (CONFIG_BK7239N_MP == 1)
bk_err_t pwm_hal_get_pwm_interrupt_status(uint32_t *int_status, uint32_t group_num);
bk_err_t pwm_hal_clear_interrupt_status(uint32_t *int_status, uint32_t group_num);
bool pwm_hal_is_uif_triggered(pwm_chan_t tim_id, uint32_t *status, uint32_t group_num);
bool pwm_hal_is_cc1if_triggered(pwm_chan_t tim_id, uint32_t *status, uint32_t group_num);
#else
uint64_t pwm_hal_get_pwm_interrupt_status(void);
bk_err_t pwm_hal_clear_interrupt_status(uint64_t status);
bool pwm_hal_is_uif_triggered(pwm_chan_t tim_id, uint64_t status);
bool pwm_hal_is_cc1if_triggered(pwm_chan_t tim_id, uint64_t status);
#endif
void pwm_hal_set_sync_all_enable(bool is_enable);
void pwm_hal_sync_all_shadow(void);
bk_err_t pwm_hal_init_capture(pwm_chan_t chan, pwm_capture_edge_t edge);
uint32_t pwm_hal_get_ccr1_shadow(pwm_chan_t tim_id);
uint32_t pwm_hal_get_ccr2_shadow(pwm_chan_t tim_id);
uint32_t pwm_hal_get_tim_arr(pwm_chan_t tim_id);
uint32_t pwm_hal_get_ccr2(pwm_chan_t tim_id);
uint32_t pwm_hal_get_capture_int_type(pwm_chan_t tim_id);
bk_err_t pwm_hal_set_fade_scale(pwm_chan_t tim_id, uint32_t value);
uint32_t pwm_hal_get_fade_scale(pwm_chan_t tim_id);
bk_err_t pwm_hal_set_fade_num(pwm_chan_t tim_id, uint32_t value);
uint32_t pwm_hal_get_fade_num(pwm_chan_t tim_id);
bk_err_t pwm_hal_set_fade_intv_cycle(pwm_chan_t tim_id, uint32_t value);
bk_err_t pwm_hal_set_fade_inc_dec(pwm_chan_t tim_id, uint32_t value);
bk_err_t pwm_hal_set_fade_enable(pwm_chan_t tim_id, uint32_t value);
uint32_t pwm_hal_get_ccmr_chan_polarity(pwm_chan_t chan);

#if CONFIG_PWM_PM_CB_SUPPORT
void pwm_hal_pm_backup(uint32_t chan, uint32_t *pm_backup, uint32_t reg_count);
void pwm_hal_pm_restore(uint32_t chan, uint32_t *pm_backup, uint32_t reg_count);
#endif

#ifdef __cplusplus
}
#endif
