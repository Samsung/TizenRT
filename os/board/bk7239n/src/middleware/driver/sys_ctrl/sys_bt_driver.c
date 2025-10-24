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

/**  BT Start **/
//BT

//CMD_SCTRL_MCLK_MUX_GET
uint32_t sys_drv_mclk_mux_get(void)
{
	uint32_t ret = 0;
	uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_mclk_mux_get();
	sys_drv_exit_critical(int_level);

	return ret;
}

//CMD_SCTRL_MCLK_DIV_GET
uint32_t sys_drv_mclk_div_get(void)
{
	uint32_t ret = 0;
	uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_mclk_div_get();
	sys_drv_exit_critical(int_level);

	return ret;
}

// CMD_SCTRL_MCLK_SELECT
void sys_drv_mclk_select(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_mclk_select(value);

	sys_drv_exit_critical(int_level);
}

//CMD_SCTRL_MCLK_DIVISION
void sys_drv_mclk_div_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_mclk_div_set(value);

	sys_drv_exit_critical(int_level);
}

//CMD_SCTRL_BLE_POWERUP/CMD_SCTRL_BLE_POWERDOWN/CMD_SCTRL_BT_POWERDOWN/CMD_SCTRL_BT_POWERUP
void sys_drv_bt_power_ctrl(bool power_up)
{
	uint32_t int_level = sys_drv_enter_critical();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_bt_power_ctrl(power_up);

	if(!ret)
		ret = sys_amp_res_release();

	sys_drv_exit_critical(int_level);
}

//CMD_TL410_CLK_PWR_UP/CMD_TL410_CLK_PWR_DOWN
void sys_drv_bt_clock_ctrl(bool en)
{
	uint32_t int_level = sys_drv_enter_critical();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_bt_clock_ctrl(en);

	if(!ret)
		ret = sys_amp_res_release();

	sys_drv_exit_critical(int_level);
}

//bk7256 add
void sys_drv_xvr_clock_ctrl(bool en)
{
 	uint32_t int_level = sys_drv_enter_critical();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_xvr_clock_ctrl(en);

	if(!ret)
		ret = sys_amp_res_release();

	sys_drv_exit_critical(int_level);
}

// CMD_GET_INTR_STATUS
uint32_t sys_drv_interrupt_status_get(void)
{
	uint32_t ret = 0;
	uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_interrupt_status_get();
	sys_drv_exit_critical(int_level);

	return ret;
}

// CMD_CLR_INTR_STATUS
void sys_drv_interrupt_status_set(uint32_t value)
{
 	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_interrupt_status_set(value);

	sys_drv_exit_critical(int_level);
}

// CMD_ICU_INT_ENABLE
void sys_drv_btdm_interrupt_ctrl(bool en)
{
  	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_btdm_interrupt_ctrl(en);

	sys_drv_exit_critical(int_level);
}

// CMD_ICU_INT_ENABLE
void sys_drv_ble_interrupt_ctrl(bool en)
{
  	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_ble_interrupt_ctrl(en);

	sys_drv_exit_critical(int_level);
}

// CMD_ICU_INT_ENABLE
void sys_drv_bt_interrupt_ctrl(bool en)
{
   	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_bt_interrupt_ctrl(en);

	sys_drv_exit_critical(int_level);
}

// CMD_BLE_RF_BIT_SET/CMD_BLE_RF_BIT_CLR
void sys_drv_bt_rf_ctrl(bool en)
{
  	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_bt_rf_ctrl(en);

	sys_drv_exit_critical(int_level);
}

// CMD_BLE_RF_BIT_GET
uint32_t sys_drv_bt_rf_status_get(void)
{
	uint32_t ret = 0;
	uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_bt_rf_status_get();
	sys_drv_exit_critical(int_level);

	return ret;
}

void sys_drv_bt_sleep_exit_ctrl(bool en)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_bt_sleep_exit_ctrl(en);

	sys_drv_exit_critical(int_level);
}

/**  BT End **/

