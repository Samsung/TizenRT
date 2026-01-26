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
#include "i2c_hw.h"
#include <driver/hal/hal_i2c_types.h>
#include "icu_hw.h"
#include "icu_ll.h"
#include "power_ll.h"

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_LL_REG_BASE(_i2c_unit_id)    (i2c_ll_get_reg_base(_i2c_unit_id))

#define I2C0_LL_SCL_PIN    GPIO_20
#define I2C0_LL_SDA_PIN    GPIO_21

#define I2C1_LL_SCL_PIN    GPIO_0
#define I2C1_LL_SDA_PIN    GPIO_1

static inline uint32_t i2c_ll_get_reg_base(i2c_id_t id)
{
	switch (id) {
	case I2C_ID_0:
		return SOC_I2C0_REG_BASE;
	case I2C_ID_1:
		return SOC_I2C1_REG_BASE;
	default:
		return BK_ERR_I2C_BASE;
	}
}

static inline void i2c_ll_soft_reset(i2c_hw_t *hw, i2c_id_t id)
{
	uint32_t reg_addr = I2C_LL_REG_BASE(id) + (0x2 << 2);
	uint32_t reg_val = REG_READ(reg_addr);

	reg_val &= ~BIT(0);
	REG_WRITE(reg_addr, reg_val);
	for(volatile uint32_t i = 0; i < 1000; i++);
	reg_val = REG_READ(reg_addr);
	reg_val |= BIT(0);
	REG_WRITE(reg_addr, reg_val);
}

static inline uint32_t i2c_ll_get_device_id(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		return hw->i2c0_hw->dev_id;
	} else {
		return hw->i2c1_hw->dev_id;
	}
}

static inline uint32_t i2c_ll_get_version_id(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		return hw->i2c0_hw->dev_version;
	} else {
		return hw->i2c1_hw->dev_version;
	}
}

static inline uint32_t i2c_ll_get_dev_status(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		return hw->i2c0_hw->dev_status;
	} else {
		return hw->i2c1_hw->dev_status;
	}
}

static inline void i2c_ll_init(i2c_hw_t *hw)
{
	hw->i2c0_hw = (i2c_typedef_t *)I2C_LL_REG_BASE(0);
	hw->i2c1_hw = (i2c_typedef_t *)I2C_LL_REG_BASE(1);

	i2c_ll_soft_reset(hw, 0);
	i2c_ll_soft_reset(hw, 1);
	hw->i2c0_hw->sm_bus_cfg.v = 0;
	hw->i2c0_hw->sm_bus_status.v = 0;
	hw->i2c1_hw->sm_bus_cfg.v = 0;
	hw->i2c1_hw->sm_bus_status.v = 0;
}

static inline void i2c_ll_enable(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		/* need open clock bypass, otherwise cannot read ack(BIT8) in i2c_isr */
		hw->i2c0_hw->global_ctrl.clk_gate_bypass = 1;
		hw->i2c0_hw->sm_bus_cfg.en = 1;
	} else {
		hw->i2c1_hw->global_ctrl.clk_gate_bypass = 1;
		hw->i2c1_hw->sm_bus_cfg.en = 1;
	}
}

static inline void i2c_ll_disable(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.en = 0;
	} else {
		hw->i2c1_hw->sm_bus_cfg.en = 0;
	}
}

static inline void i2c_ll_set_pin(i2c_hw_t *hw, i2c_id_t id)
{

}

static inline void i2c_ll_set_idle_detect_threshold(i2c_hw_t *hw, i2c_id_t id, uint32_t threshold)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.idle_cr = threshold;
	} else {
		hw->i2c1_hw->sm_bus_cfg.idle_cr = threshold;
	}
}

static inline void i2c_ll_set_scl_timeout_threshold(i2c_hw_t *hw, i2c_id_t id, uint32_t threshold)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.scl_cr = threshold;
	} else {
		hw->i2c1_hw->sm_bus_cfg.scl_cr = threshold;
	}
}

static inline void i2c_ll_set_clk_src(i2c_hw_t *hw, i2c_id_t id, uint32_t clk_src)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.clk_src = clk_src;
	} else {
		hw->i2c1_hw->sm_bus_cfg.clk_src = clk_src;
	}
}

static inline void i2c_ll_enable_scl_timeout(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.timeout_en = 1;
	} else {
		hw->i2c1_hw->sm_bus_cfg.timeout_en = 1;
	}
}

static inline void i2c_ll_disable_scl_timeout(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.timeout_en = 0;
	} else {
		hw->i2c1_hw->sm_bus_cfg.timeout_en = 0;
	}
}

static inline void i2c_ll_enable_idle_det(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.idle_det_en = 1;
	} else {
		hw->i2c1_hw->sm_bus_cfg.idle_det_en = 1;
	}
}

static inline void i2c_ll_disable_idle_det(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.idle_det_en = 0;
	} else {
		hw->i2c1_hw->sm_bus_cfg.idle_det_en = 0;
	}
}

static inline void i2c_ll_enable_slave(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.inh = 0;
	} else {
		hw->i2c1_hw->sm_bus_cfg.inh = 0;
	}
}

static inline void i2c_ll_disable_slave(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.inh = 1;
	} else {
		hw->i2c1_hw->sm_bus_cfg.inh = 1;
	}
}

static inline void i2c_ll_set_freq_div(i2c_hw_t *hw, i2c_id_t id, uint32_t freq_div)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.freq_div = freq_div & I2C_F_FREQ_DIV_M;
	} else {
		hw->i2c1_hw->sm_bus_cfg.freq_div = freq_div & I2C_F_FREQ_DIV_M;
	}
}

static inline void i2c_ll_set_slave_addr(i2c_hw_t *hw, i2c_id_t id, uint32_t slave_addr)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.slave_addr = slave_addr & I2C_F_SLAVE_ADDR_M;
	} else {
		hw->i2c1_hw->sm_bus_cfg.slave_addr = slave_addr & I2C_F_SLAVE_ADDR_M;
	}
}

static inline void i2c_ll_reset_config_to_default(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.v = 0;
	} else {
		hw->i2c1_hw->sm_bus_cfg.v = 0;
	}
}

static inline void i2c_ll_enable_start(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_status.start = 1;
	} else {
		hw->i2c1_hw->sm_bus_status.start = 1;
	}
}

static inline void i2c_ll_disable_start(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_status.start = 0;
	} else {
		hw->i2c1_hw->sm_bus_status.start = 0;
	}
}

static inline bool i2c_ll_is_start(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		return !!(hw->i2c0_hw->sm_bus_status.start);
	} else {
		return !!(hw->i2c1_hw->sm_bus_status.start);
	}
}

static inline bool i2c_ll_is_start_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	return int_status & BIT(10);
}

static inline void i2c_ll_enable_stop(i2c_hw_t *hw, i2c_id_t id)
{
		/* sm_int and stop (bit[0] and bit[9]) must set together,
		 * otherwize i2c stop will not work.
		 * e.g.
		 * case 1: i2c stop not work
		 * hw->i2c1_hw->int_status.stop = 1;
		 * or REG_SET_BIT(I2C1_R_INT_STAUS, BIT(9));
		 *
		 * case 2: i2c stop not work
		 * hw->i2c1_hw->int_status.stop = 1;
		 * hw->i2c1_hw->int_status.sm_int = 0;
		 *
		 * case 3: i2c stop not work
		 * hw->i2c1_hw->int_status.sm_int = 0;
		 * hw->i2c1_hw->int_status.stop = 1;
 		 */
}

static inline void i2c_ll_disable_stop(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_status.stop = 0;
	} else {
		hw->i2c1_hw->sm_bus_status.stop = 0;
	}
}

static inline bool i2c_ll_is_stop_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	return int_status & BIT(9);
}

static inline void i2c_ll_set_write_int_mode(i2c_hw_t *hw, i2c_id_t id, i2c_fifo_int_level_t int_mode)
{
	if (id == I2C_ID_0) {
		switch (int_mode) {
		case I2C_FIFO_INT_LEVEL_1:
			hw->i2c0_hw->sm_bus_status.int_mode = 0;
			break;
		case I2C_FIFO_INT_LEVEL_4:
			hw->i2c0_hw->sm_bus_status.int_mode = 1;
			break;
		case I2C_FIFO_INT_LEVEL_8:
			hw->i2c0_hw->sm_bus_status.int_mode = 2;
			break;
		case I2C_FIFO_INT_LEVEL_12:
			hw->i2c0_hw->sm_bus_status.int_mode = 3;
			break;
		default:
			break;
		}
	} else {
		switch (int_mode) {
		case I2C_FIFO_INT_LEVEL_1:
			hw->i2c1_hw->sm_bus_status.int_mode = 0;
			break;
		case I2C_FIFO_INT_LEVEL_4:
			hw->i2c1_hw->sm_bus_status.int_mode = 1;
			break;
		case I2C_FIFO_INT_LEVEL_8:
			hw->i2c1_hw->sm_bus_status.int_mode = 2;
			break;
		case I2C_FIFO_INT_LEVEL_12:
			hw->i2c1_hw->sm_bus_status.int_mode = 3;
			break;
		default:
			break;
		}
	}
}

static inline void i2c_ll_set_read_int_mode(i2c_hw_t *hw, i2c_id_t id, i2c_fifo_int_level_t int_mode)
{
	if (id == I2C_ID_0) {
		switch (int_mode) {
		case I2C_FIFO_INT_LEVEL_1:
			hw->i2c0_hw->sm_bus_status.int_mode = 3;
			break;
		case I2C_FIFO_INT_LEVEL_4:
			hw->i2c0_hw->sm_bus_status.int_mode = 2;
			break;
		case I2C_FIFO_INT_LEVEL_8:
			hw->i2c0_hw->sm_bus_status.int_mode = 1;
			break;
		case I2C_FIFO_INT_LEVEL_12:
			hw->i2c0_hw->sm_bus_status.int_mode = 0;
			break;
		default:
			break;
		}
	} else {
		switch (int_mode) {
		case I2C_FIFO_INT_LEVEL_1:
			hw->i2c1_hw->sm_bus_status.int_mode = 3;
			break;
		case I2C_FIFO_INT_LEVEL_4:
			hw->i2c1_hw->sm_bus_status.int_mode = 2;
			break;
		case I2C_FIFO_INT_LEVEL_8:
			hw->i2c1_hw->sm_bus_status.int_mode = 1;
			break;
		case I2C_FIFO_INT_LEVEL_12:
			hw->i2c1_hw->sm_bus_status.int_mode = 0;
			break;
		default:
			break;
		}
	}
}

static inline uint32_t i2c_ll_get_write_empty_fifo_num(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		switch (hw->i2c0_hw->sm_bus_status.int_mode) {
		case 0x00:
			return 16;
		case 0x01:
			return 12;
		case 0x02:
			return 8;
		case 0x03:
			return 4;
		default:
			return 0;
		}
	} else {
		switch (hw->i2c1_hw->sm_bus_status.int_mode) {
		case 0x00:
			return 16;
		case 0x01:
			return 12;
		case 0x02:
			return 8;
		case 0x03:
			return 4;
		default:
			return 0;
		}
	}
}

static inline uint32_t i2c_ll_get_read_fifo_num(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		switch (hw->i2c0_hw->sm_bus_status.int_mode)
		{
		case 0x00:
			return 12;
		case 0x01:
			return 8;
		case 0x02:
			return 4;
		case 0x03:
			return 1;
		default:
			return 0;
		}
	} else {
		switch (hw->i2c1_hw->sm_bus_status.int_mode)
		{
		case 0x00:
			return 12;
		case 0x01:
			return 8;
		case 0x02:
			return 4;
		case 0x03:
			return 1;
		default:
			return 0;
		}
	}
}

static inline bool i2c_ll_is_busy(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		return !!(hw->i2c0_hw->sm_bus_status.busy);
	} else {
		return !!(hw->i2c1_hw->sm_bus_status.busy);
	}
}

static inline bool i2c_ll_is_addr_matched(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_1) {
		return !!(hw->i2c1_hw->sm_bus_status.addr_match);
	} else {
		return !!(hw->i2c0_hw->sm_bus_status.addr_match);
	}
}

static inline bool i2c_ll_is_tx_mode(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_1) {
		return !!(hw->i2c1_hw->sm_bus_status.tx_mode);
	} else {
		return !!(hw->i2c0_hw->sm_bus_status.tx_mode);
	}
}

static inline bool i2c_ll_is_rx_mode(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_1) {
		return !(hw->i2c1_hw->sm_bus_status.tx_mode);
	} else {
		return !(hw->i2c0_hw->sm_bus_status.tx_mode);
	}
}

static inline uint32_t i2c_ll_get_interrupt_status(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		return hw->i2c0_hw->sm_bus_status.v;
	} else {
		return hw->i2c1_hw->sm_bus_status.v;
	}
}

static inline void i2c_ll_clear_interrupt_status(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	if (id == I2C_ID_0) {
		if (int_status & I2C0_F_SM_INT) {
			int_status &= ~I2C0_F_SM_INT; /* clear sm_bus_int */
		}
		REG_WRITE(I2C0_R_INT_STAUS, int_status);
	} else {
		if (int_status & I2C1_F_SM_INT) {
			int_status &= ~I2C1_F_SM_INT; /* clear sm_bus_int */
		}
		REG_WRITE(I2C1_R_INT_STAUS, int_status);
	}
}

static inline bool i2c_ll_is_sm_int_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	return !!(int_status & BIT(0));
}

static inline bool i2c_ll_is_scl_timeout_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	return !!(int_status & BIT(1));
}

static inline bool i2c_ll_is_arb_lost_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	return !!(int_status & BIT(3));
}

static inline bool i2c_ll_is_rx_ack_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	return !!(int_status & BIT(8));
}

static inline bool i2c_ll_is_tx_ack_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	return !!(int_status & BIT(8));
}

static inline bool i2c_ll_is_receiver_ack_req(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	return !!(int_status & BIT(12));
}

static inline bool i2c_ll_is_transmitter_mode(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	return !!(int_status & BIT(13));
}

static inline bool i2c_ll_is_master_mode(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	return !!(int_status & BIT(14));
}

static inline void i2c_ll_tx_ack(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_status.ack = 1;
	} else {
		hw->i2c1_hw->sm_bus_status.ack = 1;
	}
}

static inline void i2c_ll_tx_non_ack(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_status.ack = 0;
	} else {
		hw->i2c1_hw->sm_bus_status.ack = 0;
	}
}

static inline void i2c_ll_set_tx_mode(i2c_hw_t *hw, i2c_id_t id)
{

}

static inline void i2c_ll_set_rx_mode(i2c_hw_t *hw, i2c_id_t id)
{

}

static inline void i2c_ll_write_byte(i2c_hw_t *hw, i2c_id_t id, uint32_t data)
{
	if (id == I2C_ID_0) {
		REG_WRITE(I2C0_R_DATA, data & I2C_F_DATA_M);
	} else {
		REG_WRITE(I2C1_R_DATA, data & I2C_F_DATA_M);
	}
}

static inline uint8_t i2c_ll_read_byte(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		return hw->i2c0_hw->sm_bus_data.data;
	} else {
		return hw->i2c1_hw->sm_bus_data.data;
	}
}

static inline void i2c_ll_enable_addr_output(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_ex_cfg.addr_h_outen = 1;
	} else {
		hw->i2c1_hw->sm_bus_ex_cfg.addr_h_outen = 1;
	}
}

static inline void i2c_ll_disable_addr_output(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_ex_cfg.addr_h_outen = 0;
	} else {
		hw->i2c1_hw->sm_bus_ex_cfg.addr_h_outen = 0;
	}
}

static inline void i2c_ll_enable_data_output(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_ex_cfg.data_h_outen = 1;
	} else {
		hw->i2c1_hw->sm_bus_ex_cfg.data_h_outen = 1;
	}
}

static inline void i2c_ll_disable_data_output(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_ex_cfg.data_h_outen = 0;
	} else {
		hw->i2c1_hw->sm_bus_ex_cfg.data_h_outen = 0;
	}
}

static inline void i2c_ll_set_byte_interval(i2c_hw_t *hw, i2c_id_t id, uint32_t interval)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_ex_cfg.byte_interval = interval;
	} else {
		hw->i2c1_hw->sm_bus_ex_cfg.byte_interval = interval;
	}
}

#if (defined(CONFIG_I2C_PM_CB_SUPPORT))
#define I2C_PM_BACKUP_REG_NUM    3

static inline void i2c_ll_backup(i2c_hw_t *hw, i2c_id_t id, uint32_t *pm_backup)
{
	if (id == I2C_ID_0) {
		pm_backup[0] = hw->i2c0_hw->global_ctrl.v;
		pm_backup[1] = hw->i2c0_hw->sm_bus_cfg.v;
		pm_backup[2] = hw->i2c0_hw->sm_bus_status.v;
	} else {
		pm_backup[0] = hw->i2c1_hw->global_ctrl.v;
		pm_backup[1] = hw->i2c1_hw->sm_bus_cfg.v;
		pm_backup[2] = hw->i2c1_hw->sm_bus_status.v;
	}
}

static inline void i2c_ll_restore(i2c_hw_t *hw, i2c_id_t id, uint32_t *pm_backup)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->global_ctrl.v = pm_backup[0];
		hw->i2c0_hw->sm_bus_cfg.v = pm_backup[1];
		hw->i2c0_hw->sm_bus_status.v = pm_backup[2];
	} else {
		hw->i2c1_hw->global_ctrl.v = pm_backup[0];
		hw->i2c1_hw->sm_bus_cfg.v = pm_backup[1];
		hw->i2c1_hw->sm_bus_status.v = pm_backup[2];
	}
}
#endif

#ifdef __cplusplus
}
#endif

