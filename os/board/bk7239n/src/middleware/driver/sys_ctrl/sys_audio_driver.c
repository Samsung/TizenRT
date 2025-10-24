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

#include "sys_hal.h"
#include "sys_driver.h"
#include "sys_driver_common.h"

/**  Audio Start  **/

uint32_t  sys_drv_aud_select_clock(uint32_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level = sys_drv_enter_critical();
	sys_hal_aud_select_clock(value);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t  sys_drv_aud_clock_en(uint32_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level = sys_drv_enter_critical();
	sys_hal_aud_clock_en(value);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t sys_drv_aud_vdd1v_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_vdd1v_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_vdd1v5_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_vdd1v5_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic1_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_mic1_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic2_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_mic2_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_audpll_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_audpll_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_aud_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_aud_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dac_drv_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_dac_drv_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dac_bias_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_dac_bias_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dacr_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_dacr_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dacl_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_dacl_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_diffen_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_diffen_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_rvcmd_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_rvcmd_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_lvcmd_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_lvcmd_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_micbias1v_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_micbias1v_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_micbias_trim_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_micbias_trim_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic_rst_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_mic_rst_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic1_gain_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_mic1_gain_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic2_gain_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_mic2_gain_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic1_single_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_mic1_single_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic2_single_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_mic2_single_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dacg_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_dacg_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dacg_get(void)
{
	uint32_t ret = 0;
	uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_aud_dacg_get();
	sys_drv_exit_critical(int_level);
	return ret;
}

uint32_t sys_drv_aud_int_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_int_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_sbc_int_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_sbc_int_en(value);

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_power_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_aud_power_en(value);

	if(!ret)
		ret = sys_amp_res_release();

	sys_drv_exit_critical(int_level);
	return ret;
}

uint32_t sys_drv_aud_dac_dcoc_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_dcoc_en(value);

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dac_idac_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_idac_en(value);

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_audbias_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_audbias_en(value);

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_adcbias_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_adcbias_en(value);

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_micbias_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_micbias_en(value);

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_dmic_clk_div_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_dmic_clk_div_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dac_bypass_dwa_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_dac_bypass_dwa_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dac_dacmute_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_aud_dac_dacmute_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

/**  Audio End  **/

/**  I2S Start  **/

uint32_t sys_drv_apll_en(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_apll_en(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_cb_manu_val_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_cb_manu_val_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_ana_reg11_vsel_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_ana_reg11_vsel_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_apll_cal_val_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_apll_cal_val_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_apll_spi_trigger_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_apll_spi_trigger_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_i2s0_ckdiv_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_i2s0_ckdiv_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_apll_config_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_apll_config_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

/**  I2S End  **/


