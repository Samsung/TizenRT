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

#include <common/bk_include.h>
#include "aon_pmu_hal.h"
#include "aon_pmu_ll.h"
#include "system_hw.h"

bk_err_t aon_pmu_hal_init(void)
{
	return BK_OK;
}

int aon_pmu_hal_set_sleep_parameters(uint32_t value)
{
	aon_pmu_ll_set_r40(value);
	return 0;
}

void aon_pmu_hal_set_wakeup_source_reg(uint32_t value)
{
	//TODO fix it
	aon_pmu_ll_set_r41(value);
}

uint32_t aon_pmu_hal_get_wakeup_source_reg(void)
{
	return aon_pmu_ll_get_r41();
}

uint32_t aon_pmu_hal_get_chipid(void)
{
	return aon_pmu_ll_get_r7c_id();
}

void aon_pmu_hal_usbplug_int_en(uint32_t value)
{
	uint32_t int_state = 0;
	int_state = aon_pmu_ll_get_r1_usbplug_int_en();
	int_state |= value;
	aon_pmu_ll_set_r1_usbplug_int_en(int_state);
}

void aon_pmu_hal_touch_int_en(uint32_t value)
{
	uint32_t int_state = 0;
	int_state = aon_pmu_ll_get_r1_touch_int_en();
	int_state |= value;
	aon_pmu_ll_set_r1_touch_int_en(int_state);
}

uint32_t aon_pmu_hal_get_touch_int_status(void)
{
	return aon_pmu_ll_get_reg73_mul_touch_int();
}

uint32_t aon_pmu_hal_get_cap_cal(void)
{
	return aon_pmu_ll_get_reg73_cap_cal();
}

uint32_t aon_pmu_hal_get_touch_state(void)
{
	return aon_pmu_ll_get_reg73_mul_touch_int();
}

uint32_t aon_pmu_hal_get_adc_cal()
{
	return aon_pmu_ll_get_r7d_adc_cal();
}

void aon_pmu_hal_wdt_rst_dev_enable()
{
	uint32_t aon_pmu_r2 = 0;
	aon_pmu_r2 = aon_pmu_ll_get_r2();
	aon_pmu_r2 |= 0x1ff;

	aon_pmu_ll_set_r2(aon_pmu_r2);
}

void aon_pmu_hal_lpo_src_set(uint32_t lpo_src)
{
	aon_pmu_ll_set_r41_lpo_config(lpo_src);
}

uint32_t aon_pmu_hal_lpo_src_get()
{
	return aon_pmu_ll_get_r41_lpo_config();
}

uint32_t aon_pmu_hal_bias_cal_get()
{
	return aon_pmu_ll_get_r7e_cbcal();
}

uint32_t aon_pmu_hal_get_reset_reason()
{
	uint32_t value = aon_pmu_ll_get_r7a();
	return ( value >> 24 & 0x7F );
}


uint32_t aon_pmu_hal_get_dpll_band()
{
	return aon_pmu_ll_get_r7d_dpll_band();
}