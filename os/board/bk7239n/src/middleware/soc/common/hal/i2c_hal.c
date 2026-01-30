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

#include "i2c_hal.h"
#include "i2c_ll.h"
#include <driver/hal/hal_i2c_types.h>

#define I2C_DEFAULT_CLK_SRC    CONFIG_XTAL_FREQ
#define NUM_ROUND_UP(a,b)      ((a) / (b) + (((a) % (b)) ? 1 : 0))
#define I2C_CLK_DIVID(rate)    (NUM_ROUND_UP(NUM_ROUND_UP(I2C_DEFAULT_CLK_SRC, rate) - 6, 3) - 1)

bk_err_t i2c_hal_soft_reset(i2c_hal_t *hal)
{
	i2c_ll_soft_reset(&hal->hw, hal->id);
	return BK_OK;
}

bk_err_t i2c_hal_init(i2c_hal_t *hal)
{
	i2c_ll_init(&hal->hw);
	return BK_OK;
}

bk_err_t i2c_hal_set_baud_rate(i2c_hal_t *hal, uint32_t baud_rate)
{
	uint32_t freq_div = I2C_CLK_DIVID(baud_rate);
	i2c_ll_set_freq_div(&hal->hw, hal->id, freq_div);
	return BK_OK;
}

bk_err_t i2c_hal_configure(i2c_hal_t *hal, const i2c_config_t *cfg)
{
	uint32_t baud_rate = cfg->baud_rate;
#if defined(CONFIG_BK7239N_MP) && (CONFIG_BK7239N_MP == 1)
	uint32_t freq_div  = 0;
#endif
	if (!baud_rate) {
		baud_rate = I2C_DEFAULT_BAUD_RATE;
	}
#if defined(CONFIG_BK7239N_MP) && (CONFIG_BK7239N_MP == 1)
	if (cfg->src_clk == I2C_SCLK_80M)
		freq_div = I2C_CLK_DIVID(baud_rate)/2;  // 80M / 40M = 2
	else
		freq_div = I2C_CLK_DIVID(baud_rate);
#else
    uint32_t freq_div = I2C_CLK_DIVID(baud_rate);
#endif
	i2c_ll_set_freq_div(&hal->hw, hal->id, freq_div);
	i2c_ll_set_slave_addr(&hal->hw, hal->id, cfg->slave_addr);
	i2c_ll_set_idle_detect_threshold(&hal->hw, hal->id, 0x3);
	i2c_ll_set_scl_timeout_threshold(&hal->hw, hal->id, 0x4);
	i2c_ll_set_clk_src(&hal->hw, hal->id, 0x3);
	i2c_ll_enable_scl_timeout(&hal->hw, hal->id);
	i2c_ll_enable_idle_det(&hal->hw, hal->id);
	i2c_ll_enable_slave(&hal->hw, hal->id);
	i2c_ll_disable_start(&hal->hw, hal->id);

//i2c add address/data output mode config in ASIC, since bk7236v5, 2023.09
#if (defined(CONFIG_I2C_DIRECT_DRIVE_MODE))
	i2c_ll_enable_addr_output(&hal->hw, hal->id);
	i2c_ll_enable_data_output(&hal->hw, hal->id);
#endif

	return BK_OK;
}

bk_err_t i2c_hal_start_common(i2c_hal_t *hal)
{
	i2c_ll_enable(&(hal)->hw, (hal)->id);
	return BK_OK;
}

bk_err_t i2c_hal_stop_common(i2c_hal_t *hal)
{
	i2c_ll_disable(&(hal)->hw, (hal)->id);
	return BK_OK;
}

bk_err_t i2c_hal_set_write_int_mode(i2c_hal_t *hal, uint32_t data_size)
{
	if (data_size < 4) {
		i2c_ll_set_write_int_mode(&hal->hw, hal->id, I2C_FIFO_INT_LEVEL_1);
	} else {
		i2c_ll_set_write_int_mode(&hal->hw, hal->id, I2C_FIFO_INT_LEVEL_4);
	}

	return BK_OK;
}

bk_err_t i2c_hal_set_read_int_mode(i2c_hal_t *hal, uint32_t data_size)
{
	uint32_t int_mode;
	if (data_size > 12) {
		int_mode = I2C_FIFO_INT_LEVEL_12;
	} else if (data_size > 8) {
		int_mode = I2C_FIFO_INT_LEVEL_8;
	} else if (data_size > 4) {
		int_mode = I2C_FIFO_INT_LEVEL_4;
	} else {
		int_mode = I2C_FIFO_INT_LEVEL_1;
	}
	i2c_ll_set_read_int_mode(&hal->hw, hal->id, int_mode);

	return BK_OK;
}

#if (defined(CONFIG_I2C_PM_CB_SUPPORT))
void i2c_hal_backup(i2c_hal_t *hal, uint32_t *pm_backup)
{
	i2c_ll_backup(&hal->hw, hal->id, pm_backup);
}

void i2c_hal_restore(i2c_hal_t *hal, uint32_t *pm_backup)
{
	i2c_ll_restore(&hal->hw, hal->id, pm_backup);
}
#endif