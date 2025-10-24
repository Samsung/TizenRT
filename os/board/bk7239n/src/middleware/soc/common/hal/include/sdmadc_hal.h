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

#include "sdmadc_hw.h"
#include "sdmadc_ll.h"
#include <driver/hal/hal_sdmadc_types.h>
#include "sdmadc_map.h"
#include <common/bk_err.h>
#include "hal_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	sdmadc_hw_t *hw;
} sdmadc_hal_t;

#define sdmadc_hal_enable_soft_rst()        sdmadc_ll_set_REG0x2_soft_rst(0)
#define sdmadc_hal_disable_soft_rst()       sdmadc_ll_set_REG0x2_soft_rst(1)
#define sdmadc_hal_enable_int()             sys_drv_int_group2_enable(SDMADC_INTERRUPT_CTRL_BIT)
#define sdmadc_hal_disable_int()            sys_drv_int_group2_disable(SDMADC_INTERRUPT_CTRL_BIT)
#define sdmadc_hal_enable_sample()          sdmadc_ll_set_REG0x4_sample_enable(1)
#define sdmadc_hal_disable_sample()         sdmadc_ll_set_REG0x4_sample_enable(0)
#define sdmadc_hal_enable_efuse_cken()      sys_drv_dev_clk_pwr_up(CLK_PWR_ID_EFUSE, CLK_PWR_CTRL_PWR_UP)
#define sdmadc_hal_enable_no_buffer()       sys_ll_set_ana_reg2_gadc_nobuf_enable(1);

#define sdmadc_hal_set_sample_channel(id)	sdmadc_ll_set_REG0x5_sample_chsel(id)
#define sdmadc_hal_set_cic2_bypass(id) 		sdmadc_ll_set_REG0x5_cic2_bypass(id)
#define sdmadc_hal_set_cic2_gains(id)		sdmadc_ll_set_REG0x5_cic2_gains(id)
#define sdmadc_hal_set_comp_bypass(id)		sdmadc_ll_set_REG0x5_comp_bypass(id)
#define sdmadc_hal_set_intr_enable(id)		sdmadc_ll_set_REG0x5_intr_enable(id)
#define sdmadc_hal_set_cali_offset(id)		sdmadc_ll_set_REG0x6_cali_offset(id)
#define sdmadc_hal_set_cali_gain(id)		sdmadc_ll_set_REG0x6_cali_gain(id)
#define sdmadc_hal_get_sample_status()		sdmadc_ll_get_REG0x7_sadc_status()
#define sdmadc_hal_get_sample_data()		sdmadc_ll_get_REG0x8_fifo_data()

#define sdmadc_hal_is_analog_channel(id)      sdmadc_ll_is_analog_channel((id))
#define sdmadc_hal_is_valid_channel(id)       (sdmadc_ll_is_analog_channel((id)) ||\
				sdmadc_ll_is_digital_channel((id)))

bk_err_t sdmadc_hal_set_sample_mode(sdmadc_mode_t mode);
bk_err_t sdmadc_hal_set_sample_numb(sdmadc_sample_numb_t numb);
bk_err_t sdmadc_hal_set_sample_status(uint32_t stat);
bk_err_t sdmadc_hal_clear_interrupt_status(uint32 stat);
bool smdadc_hal_is_fifo_empty_int_triggered();

#if CFG_HAL_DEBUG_SDMADC
void sdmadc_struct_dump(uint32_t, uint32_t);
#else
#define sdmadc_struct_dump(start, end)
#endif

#ifdef __cplusplus
}
#endif