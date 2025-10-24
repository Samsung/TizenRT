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

#include "hal_config.h"
#include "aon_rtc_hw.h"
#include "aon_rtc_ll.h"
#include "aon_rtc_hal.h"

#if CFG_HAL_DEBUG_AON_RTC
void aon_rtc_struct_dump(void)
{
	aon_rtc_hw_t *hw = (aon_rtc_hw_t *)AON_RTC_LL_REG_BASE(0);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("  ctrl=0x%x value=0x%x\r\n", &hw->ctrl, hw->ctrl.ctrl_v);
	SOC_LOGI("  en:       0x%x\r\n", aon_rtc_ll_is_enable(hw));
	SOC_LOGI("  reset:    0x%x\r\n", aon_rtc_ll_is_counter_reset(hw));
	SOC_LOGI("  stop:     0x%x\r\n", aon_rtc_ll_is_counter_stop(hw));
	SOC_LOGI("  tick int en:   0x%x\r\n", aon_rtc_ll_is_tick_int_enable(hw));
	SOC_LOGI("  upper int en:  0x%x\r\n", aon_rtc_ll_is_upper_int_enable(hw));
	SOC_LOGI("  tick int sts:  0x%x\r\n", aon_rtc_ll_get_tick_int_status(hw));
	SOC_LOGI("  upper int sts: 0x%x\r\n", aon_rtc_ll_get_upper_int_status(hw));

	SOC_LOGI("  counter val:  0x%x\r\n", aon_rtc_ll_get_counter_val(hw));
	SOC_LOGI("  counter val hi:  0x%x\r\n", aon_rtc_ll_get_counter_val_hi(hw));
	SOC_LOGI("  tick val:     0x%x\r\n", aon_rtc_ll_get_tick_val(hw));
	SOC_LOGI("  tick val hi:     0x%x\r\n", aon_rtc_ll_get_tick_val_hi(hw));
	SOC_LOGI("  upper val:    0x%x\r\n", aon_rtc_ll_get_upper_val(hw));
	SOC_LOGI("  upper val hi:    0x%x\r\n", aon_rtc_ll_get_upper_val_hi(hw));
	SOC_LOGI("  counter lpo:  0x%x\r\n", aon_rtc_ll_get_tick_val_lpo(hw));
	SOC_LOGI("  counter lpo hi:  0x%x\r\n", aon_rtc_ll_get_tick_val_lpo_hi(hw));
	SOC_LOGI("  upper lpo:    0x%x\r\n", aon_rtc_ll_get_upper_val_lpo(hw));
	SOC_LOGI("  upper lpo hi:    0x%x\r\n", aon_rtc_ll_get_upper_val_lpo_hi(hw));
}
#endif

