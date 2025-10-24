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

#include <soc/soc.h>
#include "hal_port.h"
#include "aon_rtc_hw.h"
#include <driver/hal/hal_aon_rtc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AON_RTC_LL_REG_BASE(_aon_rtc_unit_id)    (SOC_AON_RTC_REG_BASE + (AON_RTC_SOC_REG_ADDRESS_OFFSET * _aon_rtc_unit_id))

static inline void aon_rtc_ll_enable(aon_rtc_hw_t *hw)
{
	hw->ctrl.en = 1;
}

static inline void aon_rtc_ll_disable(aon_rtc_hw_t *hw)
{
	hw->ctrl.en = 0;
}

static inline bool aon_rtc_ll_is_enable(aon_rtc_hw_t *hw)
{
	return hw->ctrl.en;
}

static inline void aon_rtc_ll_stop_counter(aon_rtc_hw_t *hw)
{
	hw->ctrl.cnt_stop = 1;
}

static inline void aon_rtc_ll_start_counter(aon_rtc_hw_t *hw)
{
	hw->ctrl.cnt_stop = 0;
}

static inline bool aon_rtc_ll_is_counter_stop(aon_rtc_hw_t *hw)
{
	return hw->ctrl.cnt_stop;
}

static inline void aon_rtc_ll_reset_counter(aon_rtc_hw_t *hw)
{
	hw->ctrl.cnt_reset = 1;
}

static inline void aon_rtc_ll_clear_reset_counter(aon_rtc_hw_t *hw)
{
	hw->ctrl.cnt_reset = 0;
}

static inline bool aon_rtc_ll_is_counter_reset(aon_rtc_hw_t *hw)
{
	return hw->ctrl.cnt_reset;
}

static inline void aon_rtc_ll_clear_ctrl(aon_rtc_hw_t *hw)
{
	hw->ctrl.ctrl_v = 0;
}

static inline void aon_rtc_ll_set_tick_val(aon_rtc_hw_t *hw, uint32_t val)
{
	hw->tick_val = val;
}

static inline void aon_rtc_ll_set_tick_val_hi(aon_rtc_hw_t *hw, uint32_t val)
{
	hw->tick_val_hi = val;
}

static inline uint32_t aon_rtc_ll_get_tick_val(aon_rtc_hw_t *hw)
{
	return hw->tick_val;
}

static inline uint32_t aon_rtc_ll_get_tick_val_hi(aon_rtc_hw_t *hw)
{
	return hw->tick_val_hi;
}

static inline void aon_rtc_ll_enable_tick_int(aon_rtc_hw_t *hw)
{
	hw->ctrl.tick_int_en = 1;
}

static inline void aon_rtc_ll_disable_tick_int(aon_rtc_hw_t *hw)
{
	hw->ctrl.tick_int_en = 0;
}

static inline bool aon_rtc_ll_is_tick_int_enable(aon_rtc_hw_t *hw)
{
	return hw->ctrl.tick_int_en;
}

static inline bool aon_rtc_ll_get_tick_int_status(aon_rtc_hw_t *hw)
{
	return hw->ctrl.tick_int_sts;
}

//write 1 to clear interrupt
static inline void aon_rtc_ll_clear_tick_int_status(aon_rtc_hw_t *hw)
{
	hw->ctrl.tick_int_sts = 1;
}

static inline void aon_rtc_ll_set_upper_val(aon_rtc_hw_t *hw, uint32_t up_val)
{
	hw->upper_val = up_val;
}

static inline void aon_rtc_ll_set_upper_val_hi(aon_rtc_hw_t *hw, uint32_t up_val)
{
	hw->upper_val_hi = up_val;
}

static inline uint32_t aon_rtc_ll_get_upper_val(aon_rtc_hw_t *hw)
{
	return hw->upper_val;
}

static inline uint32_t aon_rtc_ll_get_upper_val_hi(aon_rtc_hw_t *hw)
{
	return hw->upper_val_hi;
}

static inline void aon_rtc_ll_enable_upper_int(aon_rtc_hw_t *hw)
{
	hw->ctrl.up_int_en = 1;
}

static inline void aon_rtc_ll_disable_upper_int(aon_rtc_hw_t *hw)
{
	hw->ctrl.up_int_en = 0;
}

static inline bool aon_rtc_ll_is_upper_int_enable(aon_rtc_hw_t *hw)
{
	return hw->ctrl.up_int_en;
}

static inline bool aon_rtc_ll_get_upper_int_status(aon_rtc_hw_t *hw)
{
	return hw->ctrl.up_int_sts;
}

//write 1 to clear interrupt
static inline void aon_rtc_ll_clear_upper_int_status(aon_rtc_hw_t *hw)
{
	hw->ctrl.up_int_sts = 1;
}

static inline uint32_t aon_rtc_ll_get_counter_val(aon_rtc_hw_t *hw)
{
	return hw->counter_val;
}

static inline uint32_t aon_rtc_ll_get_counter_val_hi(aon_rtc_hw_t *hw)
{
	return hw->counter_val_hi;
}

static inline uint32_t aon_rtc_ll_get_upper_val_lpo(aon_rtc_hw_t *hw)
{
	return hw->up_val_lpo;
}

static inline uint32_t aon_rtc_ll_get_upper_val_lpo_hi(aon_rtc_hw_t *hw)
{
	return hw->up_val_lpo_hi;
}

static inline uint32_t aon_rtc_ll_get_tick_val_lpo(aon_rtc_hw_t *hw)
{
	return hw->tick_val_lpo;
}

static inline uint32_t aon_rtc_ll_get_tick_val_lpo_hi(aon_rtc_hw_t *hw)
{
	return hw->tick_val_lpo_hi;
}

#ifdef __cplusplus
}
#endif



