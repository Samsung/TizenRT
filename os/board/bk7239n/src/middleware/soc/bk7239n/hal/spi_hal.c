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

#define CONFIG_SPI_MAX_BAUD_RATE                80000000

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
	spi_ll_set_rx_sample_edge(hal->hw, 0);
	spi_ll_set_miso_o_fbsel(hal->hw);

	if (config->role == SPI_ROLE_SLAVE) {
		spi_ll_set_role_slave(hal->hw);
	} else {
		spi_ll_set_role_master(hal->hw);
		/* If two beken evb boards are tested against each other, for instance bk7239n/36n spi master read with bk7239n/36n spi slave write
		 * Mode 3 and Mode 2 require follow_up testing to supplement,baud_rate will be improved
		 */
		uint32_t spi_version = spi_ll_get_version_id(hal->hw);
		if(spi_version == 0x00010003){
			if((config->polarity == 0) && (config->phase == 0)){
				if(config->baud_rate > 20000000){
					spi_ll_set_rx_sample_edge(hal->hw, 1);
				}else{
					spi_ll_set_rx_sample_edge(hal->hw, 0);
				}
			}else if((config->polarity == 1) && (config->phase == 1)){
				if(config->baud_rate > 300000){
					spi_ll_set_rx_sample_edge(hal->hw, 1);
				}else{
					spi_ll_set_rx_sample_edge(hal->hw, 0);
				}
			}
		}
		/* If two beken evb boards are tested against each other, for instance bk7239n/36n spi master with bk7239n/36n spi slave
		 * rx_sample_edge will be equal to 2 using maximum spi baudrate(80MHz)
		 */
		else if(config->baud_rate > 10000000){
			spi_ll_set_rx_sample_edge(hal->hw, 1);
		}
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

bk_err_t spi_select_clock_src(uint32_t spi_clk, uint32_t *src_clk_array, uint32_t items_cnt, uint32_t *src_id)
{
        uint32_t i, id = 0xff;
        uint32_t min_id, multi_val;

        /*exactly equal, div = 1, and configurable div val = 0*/
        for(i = 0; i < items_cnt; i ++){
                multi_val = 1;
                if(src_clk_array[i] == (spi_clk * multi_val)){
                        id = i;
                        break;
                }
        }
        if(0xff != id){
                goto select_exit;
        }

        /*exact division, div = n, and configurable div val = n*/
        for(i = 0; i < items_cnt; i ++){
                multi_val = 2;
                if(0 == (src_clk_array[i] % (spi_clk * multi_val))){
                        id = i;
                        break;
                }
        }
        if(0xff != id){
                goto select_exit;
        }

        /*none can be exactly divided, check the smallest remainder*/
        min_id = items_cnt - 1;
        for(i = 0; i < items_cnt - 1; i ++){
                multi_val = 2;

                if((spi_clk * multi_val) > src_clk_array[i]){
                        continue;
                }

                if((src_clk_array[i] % (spi_clk * multi_val)) < (src_clk_array[min_id] % (spi_clk * multi_val))){
                        min_id = i;
                }
        }

        id = min_id;

select_exit:
        *src_id = id;

        return BK_OK;
}

bk_err_t spi_hal_set_baud_rate(spi_hal_t *hal, uint32_t baud_rate)
{
        uint32_t src_clk_array[] = {CONFIG_XTAL_FREQ, 60*1000000, 80*1000000};
	uint32_t src_clk, spi_clk = baud_rate;
	uint32_t clk_div, src_clk_id;
        uint32_t src_clk_cnt,multi_val = 1;

	if (baud_rate > CONFIG_SPI_MAX_BAUD_RATE) {
		HAL_LOGW("baud_rate(%d) not support, spi support max baud_rate:%d\r\n", baud_rate, CONFIG_SPI_MAX_BAUD_RATE);
		spi_clk = CONFIG_SPI_MAX_BAUD_RATE;
	}

        src_clk_cnt = sizeof(src_clk_array)/sizeof(src_clk_array[0]);
        spi_select_clock_src(spi_clk, src_clk_array, src_clk_cnt, &src_clk_id);
        src_clk = src_clk_array[src_clk_id];

        if(src_clk == spi_clk){
                clk_div = 0;
        }else{
                multi_val = 2;
                clk_div = (src_clk / (multi_val * spi_clk)) & SPI_F_CLK_DIV_M;
        }

	spi_ll_set_clk_div(hal->hw, clk_div);
	sys_hal_spi_select_clock(hal->id, src_clk_id);
	HAL_LOGD("baud_rate = %d, src_clk = %d, clk_div = 0x%x.\r\n", baud_rate, src_clk, clk_div);

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
