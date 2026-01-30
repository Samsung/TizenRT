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
#include "aon_pmu_ll.h"
#include "sys_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	aon_pmu_hw_t *hw;
} aon_pmu_hal_t;

bk_err_t aon_pmu_hal_init(void);
uint32_t aon_pmu_hal_get_wakeup_source_reg(void);
uint32_t aon_pmu_hal_get_chipid(void);
bool aon_pmu_hal_is_chipid_later_than_version_C(void);
void aon_pmu_hal_set_wakeup_source_reg(uint32_t value);
int aon_pmu_hal_set_sleep_parameters(uint32_t value);
void aon_pmu_hal_clear_wakeup_source(wakeup_source_t value);
void aon_pmu_hal_set_wakeup_source(wakeup_source_t value);
void aon_pmu_hal_usbplug_int_en(uint32_t value);
void aon_pmu_hal_touch_int_en(uint32_t value);
void aon_pmu_hal_touch_int_disable(uint32_t value);

uint32_t aon_pmu_hal_get_adc_cal(void);
uint32_t aon_pmu_hal_get_dpll_unlock(uint32_t *unlockL, uint32_t *unlockH);
uint32_t aon_pmu_hal_get_dpll_band(void);
void aon_pmu_hal_touch_select(uint32_t value);
uint32_t aon_pmu_hal_get_cap_cal(void);
uint32_t aon_pmu_hal_get_touch_state(void);
uint32_t aon_pmu_hal_get_touch_int_status(void);
void aon_pmu_hal_clear_touch_int(uint32_t value);
void aon_pmu_hal_reg_set(pmu_reg_e reg, uint32_t value);
uint32_t aon_pmu_hal_reg_get(pmu_reg_e reg);
void aon_pmu_hal_wdt_rst_dev_enable(void);
void aon_pmu_hal_lpo_src_extern32k_enable(void);
void aon_pmu_hal_lpo_src_set(uint32_t lpo_src);
uint32_t aon_pmu_hal_lpo_src_get(void);
uint32_t aon_pmu_hal_bias_cal_get(void);
uint32_t aon_pmu_hal_band_cal_get();
void aon_pmu_hal_psram_iodrv_set(uint32_t io_drv);
void aon_pmu_hal_r0_latch_to_r7b(void);
uint32_t aon_pmu_hal_get_reset_reason(void);
#if defined(CONFIG_AON_PMU_REG0_REFACTOR_DEV)
void aon_pmu_hal_set_reset_reason(uint32_t value, bool write_immediately);
uint32_t aon_pmu_hal_get_gpio_sleep(void);
void aon_pmu_hal_set_gpio_sleep(uint32_t value, bool write_immediately);
uint32_t aon_pmu_hal_get_gpio_retention_bitmap(bool read_history);
void aon_pmu_hal_set_gpio_retention_bitmap(uint32_t value);
#else
void aon_pmu_hal_set_reset_reason(uint32_t value);
uint32_t aon_pmu_hal_gpio_retention_bitmap_get();
void aon_pmu_hal_gpio_retention_bitmap_set(uint32_t bitmap);
#endif
uint32_t aon_pmu_hal_rtc_tick_h_get(void);
uint32_t aon_pmu_hal_rtc_tick_l_get(void);
void aon_pmu_hal_rtc_samp_sel(uint32_t value);
uint32_t aon_pmu_hal_get_wakeup_source(void);
void aon_pmu_hal_gpio_clksel_set(uint32_t value);
uint32_t aon_pmu_hal_gpio_clksel_get(void);
#if defined(CONFIG_DEEP_LV)
uint32_t aon_pmu_hal_get_dlv_startup(void);
uint32_t aon_pmu_hal_get_dlv_startup_iram(void);
void aon_pmu_hal_set_dlv_startup(uint32_t value);
void aon_pmu_hal_set_halt_cpu(uint32_t value);
void aon_pmu_hal_backup(void);
void aon_pmu_hal_restore(void);
#endif
void aon_pmu_hal_save_time(bool is_startup);
uint32_t aon_pmu_hal_get_por_timing(void);
uint32_t aon_pmu_hal_get_ana_gpio_status(void);
uint32_t aon_pmu_hal_set_r0_fast_boot(uint32_t value);
void aon_pmu_struct_dump(uint32_t start, uint32_t end);

#ifdef __cplusplus
}
#endif

