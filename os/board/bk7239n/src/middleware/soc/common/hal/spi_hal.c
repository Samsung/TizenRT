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

#include "icu_hal.h"
#include "clock_driver.h"
#include "power_hal.h"
#include "spi_hal.h"
#include "spi_ll.h"
#include <driver/hal/hal_spi_types.h>
#include "sys_hal.h"
#include "bk_misc.h"

#define CONFIG_SPI_MAX_BAUD_RATE                49000000     //49M

#if (!defined(CONFIG_SYSTEM_CTRL))
/* spi_clk(more than 1M) support list when source_clk = XTAL_26M
 * sort the numbers from largest to smallest, otherwize binary_search will not work
 */
static const uint32_t s_spi_clk_list[] = {
	26000000, /**< DIV1 */
	13000000, /**< DIV2 */
	6500000,  /**< DIV4 */
	5200000,  /**< DIV5 */
	3250000,  /**< DIV8 */
	2600000,  /**< DIV10 */
	2000000,  /**< DIV13 */
	1625000,  /**< DIV16 */
	1300000,  /**< DIV20 */
	1040000,  /**< DIV25 */
	1000000,  /**< DIV26 */
};

static int binary_search(const uint32_t *array, uint32_t size, uint32_t target)
{
	uint32_t left = 0;
	uint32_t right = size - 1;
	uint32_t mid = (left + right) / 2;

	while (left <= right && mid < size) {
		mid = (left + right) / 2;
		if (array[mid] == target) {
			return mid;
		}
		if (target > array[mid]) {
			right = mid - 1;
		} else {
			left = mid + 1;
		}
	}
	return BK_FAIL;
}
#endif

bk_err_t spi_hal_init(spi_hal_t *hal)
{
	spi_ll_init(hal->hw);
	return BK_OK;
}

bk_err_t spi_hal_reset_config_to_default(spi_hal_t *hal)
{
	spi_ll_reset_config_to_default(hal->hw);
	return BK_OK;
}

/* 1. bit_width, clk_rate, cpol, cpha, master/slave, 4line, int_level=0, rxovf_en=1, txudf_en=1
 * 2. if 4 wire mode, enable sla_release_int_en, clear slv_release_int
 * 3. set rx_callback, tx_need_write_callback, tx_finish_callback
 * 4. enable spi, bit[23] spi_en
 * 5. if need clk_rate more than 26M, need close i2s to avoid conflict about APLL 98M clock
 */
bk_err_t spi_hal_configure(spi_hal_t *hal, const spi_config_t *config)
{
	spi_ll_enable_tx_underflow_int(hal->hw);
	spi_ll_enable_rx_overflow_int(hal->hw);
	spi_ll_disable_tx_fifo_int(hal->hw);
	spi_ll_enable_rx_fifo_int(hal->hw);

	spi_hal_set_baud_rate(hal, config->baud_rate);
#if (defined(CONFIG_SPI_BYTE_INTERVAL))
	spi_hal_set_byte_interval(hal, config->byte_interval);
#endif

	spi_ll_set_bit_width(hal->hw, config->bit_width);
	spi_ll_set_first_bit(hal->hw, config->bit_order);
	spi_ll_set_cpol(hal->hw, config->polarity);
	spi_ll_set_cpha(hal->hw, config->phase);
	if (config->role == SPI_ROLE_SLAVE) {
		spi_ll_set_role_slave(hal->hw);
	} else {
		spi_ll_set_role_master(hal->hw);
	}
	spi_ll_set_wire_mode(hal->hw, config->wire_mode);
	if (config->wire_mode == SPI_4WIRE_MODE) {
		spi_ll_enable_slave_release_int(hal->hw);
	} else {
		spi_ll_disable_slave_release_int(hal->hw);
	}
	spi_ll_enable_tx_finish_int(hal->hw);
	spi_ll_enable_rx_finish_int(hal->hw);
	spi_ll_clear_int_status(hal->hw);

	return BK_OK;
}

bk_err_t spi_hal_start_common(spi_hal_t *hal)
{
	spi_ll_enable(hal->hw);
	return BK_OK;
}

bk_err_t spi_hal_stop_common(spi_hal_t *hal)
{
	spi_ll_disable(hal->hw);
	return BK_OK;
}

/* 1) src_clk=dco, div = ((dco_clk/2) / spi_clk) - 1
 * 2) src_clk=xtal_26M, div = xtal_26m / spi_clk - 1
 */
bk_err_t spi_hal_set_baud_rate(spi_hal_t *hal, uint32_t baud_rate)
{
	uint32_t src_clk = 0;
	uint32_t spi_clk = baud_rate;
	uint32_t clk_div = 0;

	if (baud_rate > CONFIG_SPI_MAX_BAUD_RATE) {
		HAL_LOGW("baud_rate(%d) not support, spi support max baud_rate:%d\r\n", baud_rate, CONFIG_SPI_MAX_BAUD_RATE);
		spi_clk = CONFIG_SPI_MAX_BAUD_RATE;
	}

#if (defined(CONFIG_SYSTEM_CTRL))
	if (baud_rate <= CONFIG_XTAL_FREQ) {
#else
	if (binary_search(s_spi_clk_list, ARRAY_SIZE(s_spi_clk_list), baud_rate) >= 0) {
#endif
		HAL_LOGI("spi select src_clk xtal\r\n");
		src_clk = CONFIG_XTAL_FREQ;
#if (defined(CONFIG_SYSTEM_CTRL))
		sys_hal_spi_select_clock(hal->id, SPI_CLK_XTAL);
		clk_div = (src_clk / (2 * spi_clk)) & SPI_F_CLK_DIV_M;
#else
		clk_set_spi_clk_26m(hal->id);
		clk_div = (src_clk / spi_clk - 1) & SPI_F_CLK_DIV_M;
#endif
	} else {
#if (defined(CONFIG_SYSTEM_CTRL))
		HAL_LOGI("spi select src_clk apll\r\n");
#if defined(CONFIG_SOC_BK7256XX)
		//enable apll clock and set clock config
		sys_hal_apll_en(true);
		sys_hal_cb_manu_val_set(SPI_EN_APLL_MANU_VAL);
		sys_hal_ana_reg11_vsel_set(SPI_EN_APLL_VSEL_VAL);
		src_clk = SPI_APLL_98M;
#else
		sys_hal_apll_en(1);
		//set apll clock config
		sys_hal_apll_cal_val_set(0x8973CA6F);
		sys_hal_apll_config_set(0xC2A0AE86);
		sys_hal_apll_spi_trigger_set(1);
		delay_us(10);
		sys_hal_apll_spi_trigger_set(0);
		src_clk = SPI_APLL_98M;
#endif
		sys_hal_spi_select_clock(hal->id, SPI_CLK_APLL);
		clk_div = (src_clk / (2 * spi_clk)) & SPI_F_CLK_DIV_M;
#else
		HAL_LOGI("spi select src_clk dco\r\n");
		src_clk = CONFIG_DCO_FREQ >> 1; /* The spi clock is derived from the dco clock divided by 2 */
		clk_set_spi_clk_dco(hal->id);
		clk_div = (src_clk / spi_clk) & SPI_F_CLK_DIV_M;
#endif
	}
	HAL_LOGD("baud_rate = %d, src_clk = %d, clk_div = 0x%x.\r\n", baud_rate, src_clk, clk_div);
	spi_ll_set_clk_div(hal->hw, clk_div);

	return BK_OK;
}

bk_err_t spi_hal_read_byte(spi_hal_t *hal, uint16_t *data)
{
	if (spi_ll_is_rx_fifo_rd_ready(hal->hw)) {
		*data = spi_ll_read_byte(hal->hw);
		return BK_OK;
	}
	return BK_ERR_SPI_FIFO_RD_NOT_READY;
}

bk_err_t spi_hal_enable_tx_rx(spi_hal_t *hal)
{
	spi_ll_enable_tx_rx(hal->hw);
	return BK_OK;
}

bk_err_t spi_hal_duplex_config(spi_hal_t *hal)
{
	spi_ll_disable_tx_underflow_int(hal->hw);
	spi_ll_disable_rx_overflow_int(hal->hw);
	spi_ll_disable_tx_fifo_int(hal->hw);
	spi_ll_disable_rx_fifo_int(hal->hw);
	spi_ll_enable_slave_release_int(hal->hw);
	spi_ll_disable_tx(hal->hw);
	spi_ll_disable_rx(hal->hw);
	spi_ll_enable_tx_finish_int(hal->hw);
	spi_ll_enable_rx_finish_int(hal->hw);
	spi_ll_clear_int_status(hal->hw);
	return BK_OK;
}

bk_err_t spi_hal_duplex_release(spi_hal_t *hal)
{
	spi_ll_enable_tx_underflow_int(hal->hw);
	spi_ll_enable_rx_overflow_int(hal->hw);
	spi_ll_disable_tx_fifo_int(hal->hw);
	spi_ll_enable_rx_fifo_int(hal->hw);
	spi_ll_disable_tx(hal->hw);
	return BK_OK;
}

#if (defined(CONFIG_SPI_PM_CB_SUPPORT))
void spi_hal_backup(spi_hal_t *hal, uint32_t *pm_backup)
{
	spi_ll_backup(hal->hw, pm_backup);
}
void spi_hal_restore(spi_hal_t *hal, uint32_t *pm_backup)
{
	spi_ll_restore(hal->hw, pm_backup);
}
#endif
