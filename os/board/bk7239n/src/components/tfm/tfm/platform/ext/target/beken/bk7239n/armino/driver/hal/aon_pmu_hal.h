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
void aon_pmu_hal_usbplug_int_en(uint32_t value);
void aon_pmu_hal_touch_int_en(uint32_t value);
void aon_pmu_hal_touch_int_disable(uint32_t value);

uint32_t aon_pmu_hal_get_adc_cal(void);
void aon_pmu_hal_touch_select(uint32_t value);
uint32_t aon_pmu_hal_get_cap_cal(void);
uint32_t aon_pmu_hal_get_touch_state(void);
uint32_t aon_pmu_hal_get_touch_int_status(void);
void aon_pmu_hal_clear_touch_int(uint32_t value);
void aon_pmu_hal_wdt_rst_dev_enable(void);
void aon_pmu_hal_lpo_src_set(uint32_t lpo_src);
uint32_t aon_pmu_hal_lpo_src_get();
uint32_t aon_pmu_hal_bias_cal_get();
void aon_pmu_hal_psram_iodrv_set(uint32_t io_drv);

void aon_pmu_struct_dump(uint32_t start, uint32_t end);

#ifdef __cplusplus
}
#endif

