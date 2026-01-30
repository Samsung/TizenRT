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

#include <soc/soc.h>
#include <components/log.h>
#include "sys_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void aon_pmu_drv_init(void);

void aon_pmu_drv_clear_wakeup_source(wakeup_source_t value);

void aon_pmu_drv_set_wakeup_source(wakeup_source_t value);

uint32_t aon_pmu_drv_get_adc_cal(void);

void aon_pmu_drv_touch_select(uint32_t value);

uint32_t aon_pmu_drv_get_cap_cal(void);

uint32_t aon_pmu_drv_get_touch_state(void);

void aon_pmu_drv_touch_int_enable(uint32_t enable);

void aon_pmu_drv_touch_int_disable(uint32_t disable);

uint32_t aon_pmu_drv_get_touch_int_status(void);

void aon_pmu_drv_clear_touch_int(uint32_t value);
void aon_pmu_drv_reg_set(pmu_reg_e reg, uint32_t value);
uint32_t aon_pmu_drv_reg_get(pmu_reg_e reg);
void aon_pmu_drv_wdt_rst_dev_enable(void);
void aon_pmu_drv_wdt_change_not_rosc_clk(void);
void aon_pmu_drv_lpo_src_extern32k_enable(void);
void aon_pmu_drv_lpo_src_set(uint32_t lpo_src);
uint32_t aon_pmu_drv_lpo_src_get(void);
uint32_t aon_pmu_drv_bias_cal_get(void);
uint32_t aon_pmu_drv_band_cal_get(void);
void aon_pmu_drv_gpio_clksel_set(uint32_t value);
uint32_t aon_pmu_drv_gpio_clksel_get(void);
#if defined(CONFIG_AON_PMU_REG0_REFACTOR_DEV)
void aon_pmu_drv_gpio_state_lock(bool immediately);
void aon_pmu_drv_gpio_state_unlock(bool immediately);
void aon_pmu_drv_r0_latch_to_r7b(void);
#endif

#if defined(CONFIG_AON_PMU_POR_TIMING_SUPPORT)
void aon_pmu_driver_save_time(bool is_startup);
void aon_pmu_driver_dump_time(void);
uint32_t aon_pmu_driver_get_por_timing(void);
#endif

#ifdef __cplusplus
}
#endif
