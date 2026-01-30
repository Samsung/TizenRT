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
#include "hal_port.h"
#include <driver/hal/hal_gpio_types.h>
#include "spi_hw.h"
#include <driver/hal/hal_spi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_LL_REG_BASE(_spi_unit_id)    (SOC_SPI_REG_BASE + (0x1010000 * _spi_unit_id))

#define SPI0_LL_CSN_PIN     GPIO_15
#define SPI0_LL_SCK_PIN     GPIO_14
#define SPI0_LL_MOSI_PIN    GPIO_16
#define SPI0_LL_MISO_PIN    GPIO_17

#if 0
#define SPI0_LL_CSN_PIN     GPIO_45
#define SPI0_LL_SCK_PIN     GPIO_44
#define SPI0_LL_MOSI_PIN    GPIO_46
#define SPI0_LL_MISO_PIN    GPIO_47
#endif

#define SPI1_LL_SCK_PIN     GPIO_2
#define SPI1_LL_CSN_PIN     GPIO_3
#define SPI1_LL_MOSI_PIN    GPIO_4
#define SPI1_LL_MISO_PIN    GPIO_5

static inline void spi_ll_soft_reset(spi_hw_t *hw)
{
	hw->global_ctrl.soft_reset = 1;
}

static inline void spi_ll_reset_config_to_default(spi_hw_t *hw)
{
	hw->ctrl.enable = 0;
	hw->ctrl.master_en = 0;
	hw->ctrl.wire3_en = 0;
	hw->ctrl.clk_rate = 0;
}

static inline uint32_t spi_ll_get_device_id(spi_hw_t *hw)
{
	return hw->dev_id;
}

static inline uint32_t spi_ll_get_version_id(spi_hw_t *hw)
{
	return hw->dev_version;
}

static inline uint32_t spi_ll_get_dev_status(spi_hw_t *hw)
{
	return hw->dev_status;
}

static inline void spi_ll_set_byte_interval(spi_hw_t *hw, uint32_t val)
{
	hw->ctrl.byte_interval = val;
}

static inline uint32_t spi_ll_get_byte_interval(spi_hw_t *hw)
{
	return hw->ctrl.byte_interval;
}

//Rx sample edge:  0x0: 1th posedge;  0x1: 1th negedge;  0x2:  2th posedge;  0x3: 2th negedge;
static inline void spi_ll_set_rx_sample_edge(spi_hw_t *hw, uint32_t val)
{
	hw->ctrl.reserved = val;
}

static inline uint32_t spi_ll_get_rx_sample_edge(spi_hw_t *hw)
{
	return hw->ctrl.reserved;
}

//TODO init more
static inline void spi_ll_init(spi_hw_t *hw)
{
	spi_ll_soft_reset(hw);

	//notice bk7236 v5 need set byte_interval more than 0, to prevent spi clock issue when reading
	spi_ll_set_byte_interval(hw, 0);
	hw->ctrl.tx_fifo_int_level = 0;
	hw->ctrl.rx_fifo_int_level = 0;
}

static inline void spi_ll_enable(spi_hw_t *hw)
{
	hw->ctrl.enable = 1;
}

static inline void spi_ll_disable(spi_hw_t *hw)
{
	hw->ctrl.enable = 0;
}

static inline void spi_ll_set_tx_fifo_int_level(spi_hw_t *hw, spi_fifo_int_level level)
{
	hw->ctrl.tx_fifo_int_level = level;
}

static inline void spi_ll_set_rx_fifo_int_level(spi_hw_t *hw, spi_fifo_int_level level)
{
	hw->ctrl.rx_fifo_int_level = level;
}

static inline uint32_t spi_ll_get_tx_fifo_int_level(spi_hw_t *hw)
{
	switch (hw->ctrl.tx_fifo_int_level) {
	case SPI_FIFO_INT_LEVEL_1:
		return 1;
	case SPI_FIFO_INT_LEVEL_16:
		return 16;
	case SPI_FIFO_INT_LEVEL_32:
		return 32;
	case SPI_FIFO_INT_LEVEL_48:
	default:
		return 48;
	}
}

static inline uint32_t spi_ll_get_rx_fifo_int_level(spi_hw_t *hw)
{
	switch (hw->ctrl.rx_fifo_int_level) {
	case SPI_FIFO_INT_LEVEL_1:
		return 1;
	case SPI_FIFO_INT_LEVEL_16:
		return 16;
	case SPI_FIFO_INT_LEVEL_32:
		return 32;
	case SPI_FIFO_INT_LEVEL_48:
	default:
		return 48;
	}
}

static inline void spi_ll_enable_tx_underflow_int(spi_hw_t *hw)
{
	hw->ctrl.tx_udf_int_en = 1;
}

static inline void spi_ll_disable_tx_underflow_int(spi_hw_t *hw)
{
	hw->ctrl.tx_udf_int_en = 0;
}

static inline void spi_ll_enable_rx_overflow_int(spi_hw_t *hw)
{
	hw->ctrl.rx_ovf_int_en = 1;
}

static inline void spi_ll_disable_rx_overflow_int(spi_hw_t *hw)
{
	hw->ctrl.rx_ovf_int_en = 0;
}

static inline void spi_ll_enable_tx_fifo_int(spi_hw_t *hw)
{
	hw->ctrl.tx_fifo_int_en = 1;
}

static inline void spi_ll_disable_tx_fifo_int(spi_hw_t *hw)
{
	hw->ctrl.tx_fifo_int_en = 0;
}

static inline void spi_ll_enable_rx_fifo_int(spi_hw_t *hw)
{
	hw->ctrl.rx_fifo_int_en = 1;
}

static inline void spi_ll_disable_rx_fifo_int(spi_hw_t *hw)
{
	hw->ctrl.rx_fifo_int_en = 0;
}

static inline void spi_ll_set_clk_div(spi_hw_t *hw, uint32_t clk_div)
{
	hw->ctrl.clk_rate = clk_div & SPI_F_CLK_DIV_M;
}

static inline void spi_ll_enable_slave_release_int(spi_hw_t *hw)
{
	hw->ctrl.slave_release_int_en = 1;
}

static inline void spi_ll_disable_slave_release_int(spi_hw_t *hw)
{
	hw->ctrl.slave_release_int_en = 0;
}

static inline void spi_ll_set_wire_mode(spi_hw_t *hw, spi_wire_mode_t wire_mode)
{
	hw->ctrl.wire3_en = wire_mode;
}

static inline void spi_ll_set_bit_width(spi_hw_t *hw, spi_bit_width_t bit_width)
{
	hw->ctrl.bit_width = bit_width;
}

static inline void spi_ll_set_first_bit(spi_hw_t *hw, spi_bit_order_t first_bit)
{
	hw->ctrl.lsb_first_en = first_bit;
}

static inline void spi_ll_set_cpol(spi_hw_t *hw, spi_polarity_t cpol)
{
	hw->ctrl.cpol = cpol;
}

static inline void spi_ll_set_cpha(spi_hw_t *hw, spi_phase_t cpha)
{
	hw->ctrl.cpha = cpha;
}

static inline void spi_ll_set_role(spi_hw_t *hw, spi_role_t role)
{
	hw->ctrl.master_en = role;
}

static inline void spi_ll_set_role_master(spi_hw_t *hw)
{
	hw->ctrl.master_en = 1;
}

static inline void spi_ll_set_role_slave(spi_hw_t *hw)
{
	hw->ctrl.master_en = 0;
}

static inline bool spi_ll_is_role_master(spi_hw_t *hw)
{
	return !!(hw->ctrl.master_en);
}

static inline bool spi_ll_is_role_slave(spi_hw_t *hw)
{
	return !(hw->ctrl.master_en);
}

static inline void spi_ll_enable_tx(spi_hw_t *hw)
{
	hw->cfg.tx_en = 1;
}

static inline void spi_ll_disable_tx(spi_hw_t *hw)
{
	hw->cfg.tx_en = 0;
}

static inline void spi_ll_enable_rx(spi_hw_t *hw)
{
	hw->cfg.rx_en = 1;
}

static inline void spi_ll_disable_rx(spi_hw_t *hw)
{
	hw->cfg.rx_en = 0;
}

static inline void spi_ll_enable_tx_finish_int(spi_hw_t *hw)
{
	hw->cfg.tx_finish_int_en = 1;
}

static inline void spi_ll_disable_tx_finish_int(spi_hw_t *hw)
{
	hw->cfg.tx_finish_int_en = 0;
}

static inline void spi_ll_enable_rx_finish_int(spi_hw_t *hw)
{
	hw->cfg.rx_finish_int_en = 1;
}

static inline void spi_ll_disable_rx_finish_int(spi_hw_t *hw)
{
	hw->cfg.rx_finish_int_en = 0;
}

static inline void spi_ll_set_tx_trans_len(spi_hw_t *hw, uint32_t len)
{
	if (hw->ctrl.bit_width == SPI_BIT_WIDTH_16BITS) {
		len = len / 2;
	}
	hw->cfg.tx_trans_len = len & SPI_F_TX_TRANS_LEN_M;
}

static inline void spi_ll_set_rx_trans_len(spi_hw_t *hw, uint32_t len)
{
	if (hw->ctrl.bit_width == SPI_BIT_WIDTH_16BITS) {
		len = len / 2;
	}
	hw->cfg.rx_trans_len = len & SPI_F_RX_TRANS_LEN_M;
}

static inline void spi_ll_set_miso_o_fbsel(spi_hw_t *hw)
{
	hw->cfg.miso_o_fbsel = 1;
}

static inline void spi_ll_enable_tx_rx(spi_hw_t *hw)
{
	hw->cfg.v |= 0b11;
}

static inline void spi_ll_clear_int_status(spi_hw_t *hw)
{
	hw->int_status.tx_fifo_int = 1;
	hw->int_status.rx_fifo_int = 1;
	hw->int_status.slave_release_int = 1;
	hw->int_status.tx_underflow_int = 1;
	hw->int_status.rx_overflow_int = 1;
}

static inline void spi_ll_clear_slave_release_int_status(spi_hw_t *hw)
{
	hw->int_status.slave_release_int = 1;
}

static inline void spi_ll_reset_slave_release_int_status_to_default(spi_hw_t *hw)
{
	hw->int_status.slave_release_int = 0;
}

static inline void spi_ll_clear_tx_underflow_int_status(spi_hw_t *hw)
{
	hw->int_status.tx_underflow_int = 1;
}

static inline void spi_ll_clear_rx_overflow_int_status(spi_hw_t *hw)
{
	hw->int_status.rx_overflow_int = 1;
}

static inline uint32_t spi_ll_get_interrupt_status(spi_hw_t *hw)
{
	return hw->int_status.v;
}

static inline void spi_ll_clear_interrupt_status(spi_hw_t *hw, uint32_t status)
{
	hw->int_status.v = status;
}

static inline bool spi_ll_is_tx_fifo_wr_ready(spi_hw_t *hw)
{
	return !!(hw->int_status.tx_fifo_wr_ready);
}

static inline bool spi_ll_is_rx_fifo_rd_ready(spi_hw_t *hw)
{
	return !!(hw->int_status.rx_fifo_rd_ready);
}

static inline bool spi_ll_is_tx_fifo_int_triggered(spi_hw_t *hw)
{
	return !!(hw->int_status.tx_fifo_int);
}

static inline bool spi_ll_is_rx_fifo_int_triggered(spi_hw_t *hw)
{
	return !!(hw->int_status.rx_fifo_int);
}

static inline bool spi_ll_is_tx_fifo_int_triggered_with_status(spi_hw_t *hw, uint32_t status)
{
	return !!(status & BIT(8));
}

static inline bool spi_ll_is_rx_fifo_int_triggered_with_status(spi_hw_t *hw, uint32_t status)
{
	return !!(status & BIT(9));
}

static inline bool spi_ll_is_slave_release_int_triggered(spi_hw_t *hw, uint32_t status)
{
	return !!(status & BIT(10));
}

static inline bool spi_ll_is_tx_finish_int_triggered(spi_hw_t *hw, uint32_t status)
{
	return !!(status & BIT(13));
}

static inline bool spi_ll_is_rx_finish_int_triggered(spi_hw_t *hw, uint32_t status)
{
	return !!(status & BIT(14));
}

static inline bool spi_ll_is_rx_overflow_int_triggered(spi_hw_t *hw, uint32_t status)
{
	return !!(status & BIT(12));
}

static inline bool spi_ll_is_tx_underflow_int_triggered(spi_hw_t *hw, uint32_t status)
{
	return !!(status & BIT(11));
}

static inline void spi_ll_clear_tx_finish_int_status(spi_hw_t *hw)
{
	hw->int_status.tx_finish_int = 1;
}

static inline void spi_ll_clear_rx_finish_int_status(spi_hw_t *hw)
{
	hw->int_status.rx_finish_int = 1;
}

static inline void spi_ll_clear_tx_fifo_int_status(spi_hw_t *hw)
{
	hw->int_status.tx_fifo_int = 1;
}

static inline void spi_ll_clear_rx_fifo_int_status(spi_hw_t *hw)
{
	hw->int_status.rx_fifo_int = 1;
}

static inline void spi_ll_clear_tx_fifo(spi_hw_t *hw)
{
	hw->int_status.tx_fifo_clr = 1;
}

static inline void spi_ll_clear_rx_fifo(spi_hw_t *hw)
{
	hw->int_status.rx_fifo_clr = 1;
}

static inline void spi_ll_write_byte(spi_hw_t *hw, uint32_t data)
{
	hw->data.fifo_data = data;
}

static inline uint32_t spi_ll_read_byte(spi_hw_t *hw)
{
	return hw->data.fifo_data;
}

#if (defined(CONFIG_SPI_PM_CB_SUPPORT))
#define SPI_PM_BACKUP_REG_NUM    3

static inline void spi_ll_backup(spi_hw_t *hw, uint32_t *pm_backup)
{
	pm_backup[0] = hw->global_ctrl.v;
	pm_backup[1] = hw->ctrl.v;
	pm_backup[2] = hw->cfg.v;
}

static inline void spi_ll_restore(spi_hw_t *hw, uint32_t *pm_backup)
{
	hw->global_ctrl.v = pm_backup[0];
	hw->ctrl.v = pm_backup[1];
	hw->cfg.v = pm_backup[2];
}
#endif

#ifdef __cplusplus
}
#endif

