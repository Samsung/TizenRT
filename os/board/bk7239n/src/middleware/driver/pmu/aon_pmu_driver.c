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
#include <driver/aon_rtc.h>
#include <modules/pm.h>

void aon_pmu_drv_init(void)
{
    aon_pmu_hal_init();
}
__IRAM_SEC void aon_pmu_drv_clear_wakeup_source(wakeup_source_t value)
{
    aon_pmu_hal_clear_wakeup_source(value);
}
void aon_pmu_drv_set_wakeup_source(wakeup_source_t value)
{
    aon_pmu_hal_set_wakeup_source(value);
}

uint32_t aon_pmu_drv_get_adc_cal(void)
{
	return aon_pmu_hal_get_adc_cal();
}

void aon_pmu_drv_touch_select(uint32_t value)
{
	aon_pmu_hal_touch_select(value);
}

uint32_t aon_pmu_drv_get_cap_cal(void)
{
	return aon_pmu_hal_get_cap_cal();
}

uint32_t aon_pmu_drv_get_touch_state(void)
{
	return aon_pmu_hal_get_touch_state();
}

void aon_pmu_drv_touch_int_enable(uint32_t enable)
{
	aon_pmu_hal_touch_int_en(enable);
}

void aon_pmu_drv_touch_int_disable(uint32_t disable)
{
	aon_pmu_hal_touch_int_disable(disable);
}

uint32_t aon_pmu_drv_get_touch_int_status(void)
{
	return aon_pmu_hal_get_touch_int_status();
}

void aon_pmu_drv_clear_touch_int(uint32_t value)
{
	aon_pmu_hal_clear_touch_int(value);
}
__IRAM_SEC void aon_pmu_drv_reg_set(pmu_reg_e reg, uint32_t value)
{
    aon_pmu_hal_reg_set(reg,value);
}
__IRAM_SEC uint32_t aon_pmu_drv_reg_get(pmu_reg_e reg)
{
	return aon_pmu_hal_reg_get(reg);
}

void aon_pmu_drv_lpo_src_extern32k_enable(void)
{
	aon_pmu_hal_lpo_src_extern32k_enable();
}
void aon_pmu_drv_lpo_src_set(uint32_t lpo_src)
{
	if(lpo_src == PM_LPO_SRC_X32K)
	{
		bk_rtc_set_clock_freq(AON_RTC_EXTERN_32K_CLOCK_FREQ);
	} else {
		bk_rtc_set_clock_freq(AON_RTC_DEFAULT_CLOCK_FREQ);
	}

#if defined(CONFIG_ROSC_COMPENSATION)
	// get current tick to sync tick value for rosc_rtc_tick_compensation
	bk_aon_rtc_get_current_tick_with_compensation(AON_RTC_ID_1);
#endif
	aon_pmu_hal_lpo_src_set(lpo_src);
}
__IRAM_SEC uint32_t aon_pmu_drv_lpo_src_get(void)
{
	return aon_pmu_hal_lpo_src_get();
}

#if (defined(CONFIG_SYSTEM_CTRL))
void aon_pmu_drv_wdt_rst_dev_enable(void)
{
	aon_pmu_hal_wdt_rst_dev_enable();
}

void aon_pmu_drv_wdt_change_not_rosc_clk(void)
{
	uint32_t param =0;
	param = aon_pmu_drv_reg_get(PMU_REG0x41);
	param &= ~0x3; //select clk_DIVD as lpo_src
	aon_pmu_drv_reg_set(PMU_REG0x41,param);
}

#endif

uint32_t aon_pmu_drv_bias_cal_get(void)
{
	return aon_pmu_hal_bias_cal_get();
}

uint32_t aon_pmu_drv_band_cal_get(void)
{
	return aon_pmu_hal_band_cal_get();
}

void aon_pmu_drv_gpio_clksel_set(uint32_t value)
{
	aon_pmu_hal_gpio_clksel_set(!!value);
}

uint32_t aon_pmu_drv_gpio_clksel_get(void)
{
	return aon_pmu_hal_gpio_clksel_get();
}

#if defined(CONFIG_AON_PMU_REG0_REFACTOR_DEV)
void aon_pmu_drv_gpio_state_lock(bool immediately)
{
	if (immediately)
		aon_pmu_hal_set_gpio_sleep(1, true);
	else
		aon_pmu_hal_set_gpio_sleep(1, false);
}

void aon_pmu_drv_gpio_state_unlock(bool immediately)
{
	if (immediately)
		aon_pmu_hal_set_gpio_sleep(0, true);
	else
		aon_pmu_hal_set_gpio_sleep(0, false);
}

void aon_pmu_drv_r0_latch_to_r7b(void)
{
	return aon_pmu_hal_r0_latch_to_r7b();
}
#endif

#if defined(CONFIG_AON_PMU_POR_TIMING_SUPPORT)
static uint32_t s_por_timing_ms = 0;
void aon_pmu_driver_save_time(bool is_startup)
{
	aon_pmu_hal_save_time(is_startup);
}

// this api only works before sys_tick start running
void aon_pmu_driver_dump_time(void)
{
	uint32_t por_timing = aon_pmu_hal_get_por_timing();
	s_por_timing_ms = por_timing * 100;
	os_printf("Por Timing: %dms RTC: %dms\r\n", s_por_timing_ms, aon_pmu_hal_rtc_tick_l_get()/32);
}

uint32_t aon_pmu_driver_get_por_timing(void)
{
	return s_por_timing_ms;
}
#endif
