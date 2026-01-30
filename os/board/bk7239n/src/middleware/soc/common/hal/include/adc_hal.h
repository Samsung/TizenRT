// Copyright 2020-2025 Beken
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

#include "adc_hw.h"
#include "adc_ll.h"
#include <driver/hal/hal_adc_types.h>
#include "adc_map.h"
#include <common/bk_err.h>
#include "hal_config.h"

typedef struct {
	adc_hw_t *hw;
} adc_hal_t;

#if defined(CONFIG_SARADC_V1P2)
#define ADC_CWT_COEF_NUM        (16)
void adc_hal_clear_int_status(adc_hal_t *hal);
bool adc_hal_is_fifo_empty(adc_hal_t *hal);
uint32_t adc_hal_get_adc_data(adc_hal_t *hal);
void adc_hal_sel_channel(adc_hal_t *hal, adc_chan_t chan);
bool adc_hal_check_adc_enable(adc_hal_t *hal);
void adc_hal_set_fifo_threshold(adc_hal_t *hal, uint32_t cnt);
uint32_t adc_hal_get_mode(adc_hal_t *hal);
void adc_hal_calib_init(void);
void adc_hal_set_cwt_calib(uint16_t *adc_data, uint16_t data_num);
void adc_hal_set_cwt(uint32_t *adc_data);
void hal_calib_apply(void);

#define adc_hal_enable_bypass_calib(hal)
#define adc_hal_disable_bypass_calib(hal)
#define adc_hal_check_adc_busy(hal)    false
#define adc_hal_set_steady_ctrl(hal, value)
#define adc_hal_set_sample_rate(hal, rate)
#define adc_hal_set_adc_filter(hal, filter)
#define adc_hal_is_analog_channel(hal, id)   true
#define adc_hal_is_valid_channel(hal, id)    true
#else // CONFIG_SARADC_V1P2
#define adc_hal_set_sleep_mode(hal)		adc_ll_set_sleep_mode((hal)->hw)
#define adc_hal_set_single_step_mode(hal)	adc_ll_set_single_step_mode((hal)->hw)
#define adc_hal_set_software_control_mode(hal)	adc_ll_set_software_control_mode((hal)->hw)
#define adc_hal_get_mode(hal)			adc_ll_get_adc_mode((hal)->hw)
#define adc_hal_set_continuous_mode(hal)	adc_ll_set_continuous_mode((hal)->hw)
#define adc_hal_enbale(hal)			adc_ll_enable((hal)->hw)
#define adc_hal_disbale(hal)			adc_ll_disable((hal)->hw)
#define adc_hal_sel_channel(hal, id)		adc_ll_sel_channel((hal)->hw, id)
#define adc_hal_wait_4_cycle(hal)		adc_ll_wait_4_cycle((hal)->hw)
#define adc_hal_wait_8_cycle(hal)		adc_ll_wait_8_cycle((hal)->hw)
#define adc_hal_clear_int_status(hal)		adc_ll_clear_int_status((hal)->hw)
#define adc_hal_enable_32m_clk(hal)		adc_ll_enable_32m_clk((hal)->hw)
#define adc_hal_disable_32m_clk(hal)		adc_ll_disable_32m_clk((hal)->hw)
#define adc_hal_set_pre_div(hal, div)		adc_ll_set_pre_div((hal)->hw, div)
#define adc_hal_set_sample_rate(hal, rate)	adc_ll_set_sample_rate((hal)->hw, rate)
#define adc_hal_set_adc_filter(hal, filter)	adc_ll_set_adc_filter((hal)->hw, filter)
#define adc_hal_is_fifo_empty(hal)		adc_ll_is_fifo_empty((hal)->hw)
#define adc_hal_is_fifo_full(hal)		adc_ll_is_fifo_full((hal)->hw)
#define adc_hal_check_adc_busy(hal)		adc_ll_check_adc_busy((hal)->hw)
#define adc_hal_check_adc_enable(hal)		adc_ll_check_adc_enable((hal)->hw)

#define adc_hal_get_adc_raw_data(hal)		adc_ll_get_adc_raw_data((hal)->hw)

#define adc_hal_set_fifo_threshold(hal, value)	adc_ll_set_fifo_threshold((hal)->hw, value)
#define adc_hal_set_steady_ctrl(hal, value)	adc_ll_set_steady_ctrl((hal)->hw, value)

#define adc_hal_is_over_flow(hal)		adc_ll_is_over_flow((hal)->hw)

#define adc_hal_get_adc_data(hal)		adc_ll_get_adc_data((hal)->hw)

#define adc_hal_enable_bypass_calib(hal)	adc_ll_enable_bypass_calib((hal)->hw)
#define adc_hal_disable_bypass_calib(hal)	adc_ll_disable_bypass_calib((hal)->hw)

#define adc_hal_is_analog_channel(hal, id)      adc_ll_is_analog_channel((hal)->hw, (id))
#define adc_hal_is_valid_channel(hal, id)       (adc_ll_is_analog_channel((hal)->hw, (id)) ||\
				adc_ll_is_digital_channel((hal)->hw, (id)))
#endif // CONFIG_SARADC_V1P2

bk_err_t adc_hal_init(adc_hal_t *hal);
bk_err_t adc_hal_deinit(adc_hal_t *hal);
bk_err_t adc_hal_set_clk(adc_hal_t *hal, adc_src_clk_t src_clk, uint32_t adc_clk);
bk_err_t adc_hal_set_mode(adc_hal_t *hal, adc_mode_t adc_mode);
bk_err_t adc_hal_start_commom(adc_hal_t *hal);
bk_err_t adc_hal_stop_commom(adc_hal_t *hal);
bk_err_t adc_hal_set_saturate_mode(adc_hal_t *hal, adc_saturate_mode_t mode);
uint16_t adc_hal_get_single_step_adc_data(adc_hal_t *hal);

#if defined(CONFIG_SARADC_V1P2)
bk_err_t adc_hal_set_vol_div(adc_chan_t adc_chan, adc_vol_div_t vol_div);
adc_vol_div_t adc_hal_get_vol_div(adc_chan_t adc_chan);
#endif
uint32_t adc_hal_get_temp_code_dft_25degree(void);
uint32_t adc_hal_get_temp_code_dft_threshold(void);
uint32_t adc_hal_get_temp_lsb_per_10degree(void);
uint32_t adc_hal_get_1Volt_value(void);
uint32_t adc_hal_get_1Volt_threshold(void);
uint32_t adc_hal_get_2Volt_value(void);
uint32_t adc_hal_get_2Volt_threshold(void);

#ifdef SOC_ADC_FIFO_DATA_SUPPORT
#define adc_hal_get_fifo_data(hal) adc_ll_get_fifo_data((hal)->hw)
#endif

#ifdef SOC_ADC_DC_OFFSET_SUPPORT
#define adc_hal_set_dc_offset(hal, value) adc_ll_set_dc_offset((hal)->hw, value)
#define adc_hal_get_dc_offset(hal, value) adc_ll_get_dc_offset((hal)->hw)
#endif

#ifdef SOC_ADC_GAIN_SUPPORT
#define adc_hal_set_gain(hal, value) adc_ll_set_gain((hal)->hw, value)
#define adc_hal_get_gain(hal) adc_ll_get_gain((hal)->hw)
#endif

#define SARADC_AUTOTEST    0

#if SARADC_AUTOTEST
bk_err_t adc_hal_set_div(adc_hal_t *hal, uint32_t div);
#endif

#if defined(CONFIG_SARADC_V1P2)
void adc_struct_dump(uint32_t start, uint32_t end);
#else
void adc_struct_dump(void);
#endif

#ifdef __cplusplus
}
#endif
