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

#include "aon_rtc_ll.h"
#include <driver/hal/hal_aon_rtc_types.h>
#include "aon_rtc_hal.h"

void aon_rtc_hal_enable(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_enable(hal->hw);
}

void aon_rtc_hal_disable(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_disable(hal->hw);
}

bool aon_rtc_hal_is_enable(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	return aon_rtc_ll_is_enable(hal->hw);
}

void aon_rtc_hal_stop_counter(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_stop_counter(hal->hw);
}

void aon_rtc_hal_start_counter(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_start_counter(hal->hw);
}

bool aon_rtc_hal_is_counter_stop(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	return aon_rtc_ll_is_counter_stop(hal->hw);
}

void aon_rtc_hal_reset_counter(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_reset_counter(hal->hw);
}

void aon_rtc_hal_clear_reset_counter(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_clear_reset_counter(hal->hw);
}

bool aon_rtc_hal_is_counter_reset(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	return aon_rtc_ll_is_counter_reset(hal->hw);
}

void aon_rtc_hal_set_tick_val(aon_rtc_hal_t *hal, uint32_t val)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_set_tick_val(hal->hw, val);
}

uint32_t aon_rtc_hal_get_tick_val(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	return aon_rtc_ll_get_tick_val(hal->hw);
}

void aon_rtc_hal_enable_tick_int(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_enable_tick_int(hal->hw);
}

void aon_rtc_hal_disable_tick_int(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_disable_tick_int(hal->hw);
}

bool aon_rtc_hal_is_tick_int_enable(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	return aon_rtc_ll_is_tick_int_enable(hal->hw);
}

bool aon_rtc_hal_get_tick_int_status(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	return aon_rtc_ll_get_tick_int_status(hal->hw);
}

void aon_rtc_hal_clear_tick_int_status(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_clear_tick_int_status(hal->hw);
}

void aon_rtc_hal_set_upper_val(aon_rtc_hal_t *hal, uint32_t up_val)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_set_upper_val(hal->hw, up_val);
}

uint32_t aon_rtc_hal_get_upper_val(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	return aon_rtc_ll_get_upper_val(hal->hw);
}

void aon_rtc_hal_enable_upper_int(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_enable_upper_int(hal->hw);
}

void aon_rtc_hal_disable_upper_int(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_disable_upper_int(hal->hw);
}

bool aon_rtc_hal_is_upper_int_enable(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	return aon_rtc_ll_is_upper_int_enable(hal->hw);
}

bool aon_rtc_hal_get_upper_int_status(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	return aon_rtc_ll_get_upper_int_status(hal->hw);
}

void aon_rtc_hal_clear_upper_int_status(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	aon_rtc_ll_clear_upper_int_status(hal->hw);
}

uint32_t aon_rtc_hal_get_counter_val(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	volatile uint32_t val = aon_rtc_ll_get_counter_val(hal->hw);

	while (aon_rtc_ll_get_counter_val(hal->hw) != val) {
		val = aon_rtc_ll_get_counter_val(hal->hw);
	}

	return val;
}

uint32_t aon_rtc_hal_get_upper_val_lpo(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	return aon_rtc_ll_get_upper_val_lpo(hal->hw);
}

uint32_t aon_rtc_hal_get_tick_val_lpo(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);
	return aon_rtc_ll_get_tick_val_lpo(hal->hw);
}

bk_err_t aon_rtc_hal_init(aon_rtc_hal_t *hal)
{
	extern void delay_us(uint32_t us);
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);

	//enter deepsleep/reboot,before init,the tick is still on-going
	aon_rtc_ll_reset_counter(hal->hw);
	delay_us(125);
	aon_rtc_ll_clear_reset_counter(hal->hw);

	aon_rtc_ll_clear_ctrl(hal->hw);
	aon_rtc_ll_set_tick_val(hal->hw, 0);
	aon_rtc_ll_set_upper_val(hal->hw, 0);
	aon_rtc_ll_enable(hal->hw);

	return BK_OK;
}

bk_err_t aon_rtc_hal_deinit(aon_rtc_hal_t *hal)
{
	hal->hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(hal->id);

	aon_rtc_ll_reset_counter(hal->hw);
	aon_rtc_ll_clear_tick_int_status(hal->hw);
	aon_rtc_ll_clear_upper_int_status(hal->hw);
	aon_rtc_ll_set_tick_val(hal->hw, 0);
	aon_rtc_ll_set_upper_val(hal->hw, 0);
	aon_rtc_ll_clear_ctrl(hal->hw);
	aon_rtc_ll_disable(hal->hw);

	return BK_OK;
}
bk_err_t aon_rtc_hal_tick_init()
{
    aon_rtc_ll_tick_init();

	return BK_OK;
}
bk_err_t aon_rtc_hal_open_rtc_wakeup(uint32_t period)
{
    aon_rtc_ll_open_rtc_wakeup(period);

	return BK_OK;
}
uint32_t aon_rtc_hal_get_current_tick()
{
	return aon_rtc_ll_get_current_tick();
}

