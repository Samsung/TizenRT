// Copyright 2021-2025 Beken
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
#include <modules/pm.h>
#include <driver/gpio.h>
#include "gpio_map.h"
#include <components/sensor.h>
#include <driver/aon_rtc.h>
#include "sys_driver.h"

#include "pm_debug.h"


static uint32_t s_pm_mcu_pm_state                       = 0;
static uint32_t s_pm_cp1_auto_power_down_flag           = PM_CP1_AUTO_POWER_DOWN_CTRL;
static uint32_t s_pm_lowvol_consume_time_exit_wfi       = 0;


static pm_mem_auto_ctrl_e s_pm_mem_auto_power_down_flag = PM_MEM_AUTO_CTRL_ENABLE;

uint32_t bk_pm_mcu_pm_state_get()
{
	return s_pm_mcu_pm_state;
}
bk_err_t bk_pm_mcu_pm_ctrl(uint32_t power_state)
{
	s_pm_mcu_pm_state = power_state;
	return BK_OK;
}

uint32_t bk_pm_wakeup_from_lowvol_consume_time_get()
{
	return ((s_pm_lowvol_consume_time_exit_wfi * 1000) / bk_rtc_get_ms_tick_count()); // unit: us
}

uint32_t bk_pm_cp1_boot_flag_get()
{
	return 0; // s_pm_cp1_boot_ready;
}

uint32_t bk_pm_cp1_auto_power_down_state_get()
{
	return s_pm_cp1_auto_power_down_flag;
}

bk_err_t bk_pm_cp1_auto_power_down_state_set(uint32_t value)
{
	s_pm_cp1_auto_power_down_flag = value;
	return BK_OK;
}

pm_mem_auto_ctrl_e bk_pm_mem_auto_power_down_state_get()
{
	return s_pm_mem_auto_power_down_flag;
}

bk_err_t bk_pm_mem_auto_power_down_state_set(pm_mem_auto_ctrl_e value)
{
	s_pm_mem_auto_power_down_flag = value;
	return BK_OK;
}

/*=========================EXTERNAL LDO CTRL START========================*/
bk_err_t bk_pm_module_vote_ctrl_external_ldo(gpio_ctrl_ldo_module_e module,gpio_id_t gpio_id,gpio_output_state_e value)
{
	bk_gpio_ctrl_external_ldo(module,gpio_id,value);
	return BK_OK;
}

bk_err_t bk_pm_external_ldo_ctrl(uint32_t value)
{
	uint32_t i = 0;
	uint32_t gpio_ctrl_ldo_output_high_map[] = GPIO_CTRL_LDO_OUTPUT_HIGH_MAP;
	uint32_t gpio_ctrl_ldo_output_low_map[] = GPIO_CTRL_LDO_OUTPUT_LOW_MAP;

	if (value == 0x1) // output higt
	{
		for (i = 0; i < sizeof(gpio_ctrl_ldo_output_high_map) / sizeof(uint32_t); i++)
		{
			bk_gpio_enable_output(gpio_ctrl_ldo_output_high_map[i]);
			bk_gpio_set_output_high(gpio_ctrl_ldo_output_high_map[i]);
		}
	}
	else if (value == 0x0) // output low
	{
		for (i = 0; i < sizeof(gpio_ctrl_ldo_output_low_map) / sizeof(uint32_t); i++)
		{
			bk_gpio_enable_output(gpio_ctrl_ldo_output_high_map[i]);
			bk_gpio_set_output_low(gpio_ctrl_ldo_output_low_map[i]);
		}
	}
	else
	{
	}

	return BK_OK;
}
/*=========================EXTERNAL LDO CTRL END========================*/

/*=========================POWER/VOLTAGE CTRL START========================*/
bk_err_t bk_pm_cpu_freq_dump(void)
{
	sys_hal_cpu_freq_dump();
	return BK_OK;
}
// TODO: for debug use?
uint32_t bk_pm_lp_vol_get(void)
{
	return sys_drv_lp_vol_get();
}

int bk_pm_lp_vol_set(uint32_t value)
{
	sys_drv_lp_vol_set(value);
	return BK_OK;
}

uint32_t bk_pm_rf_tx_vol_get()
{
	return sys_drv_rf_tx_vol_get();
}

int bk_pm_rf_tx_vol_set(uint32_t value)
{
	sys_drv_rf_tx_vol_set(value);
	return BK_OK;
}

uint32_t bk_pm_rf_rx_vol_get()
{
	return sys_drv_rf_rx_vol_get();
}

int bk_pm_rf_rx_vol_set(uint32_t value)
{
	sys_drv_rf_rx_vol_set(value);
	return BK_OK;
}
/*=========================POWER/VOLTAGE CTRL END========================*/

/*=========================DEBUG/TEST CTRL START========================*/
const char *pm_sleep_mode_to_string(pm_sleep_mode_e sleep_mode)
{
    static const char* sleep_mode_strings[] = {
        "NORMAL_SLEEP",
        "LOW_VOLTAGE",
        "DEEP_SLEEP",
        "SUPER_DEEP_SLEEP",
        "DEFAULT",
        "UNKNOWN"
    };

    if (sleep_mode > PM_MODE_DEFAULT) {
        return "UNKNOWN";
    }

    return sleep_mode_strings[sleep_mode];
}

void pm_printf_current_temperature(void)
{
	return;
#if CONFIG_TEMP_DETECT
	float temp;
	bk_sensor_get_current_temperature(&temp);
	BK_LOGD(NULL, "current chip temperature about %.2f\r\n",temp);
#endif
}
uint32_t pm_disable_int(void)
{
	uint32_t primask = 0;

	/* Return the current value of primask register and set
	 * bit 0 of the primask register to disable interrupts
	 */
	__asm__ __volatile__
	(
		"\tmrs    %0, primask\n"
		"\tcpsid  i\n"
		: "=r"(primask)
		:
		: "memory"
	);

	__asm volatile ( "dsb" );
	__asm volatile ( "isb" );
	return primask;
}

uint32_t pm_get_int_status(void)
{
	uint32_t primask = 0;

	/* Read the current value of primask register without modifying it */
	__asm__ __volatile__
	(
		"\tmrs    %0, primask\n"
		: "=r"(primask)
		:
		: "memory"
	);

	return primask;
}
void pm_enable_int(uint32_t int_level)
{
	/* If bit 0 of the primask is 0, then we need to restore
	 * interrupts.
	 */
	__asm__ __volatile__
	(
		"\ttst    %0, #1\n"
		"\tbne.n  1f\n"
		"\tcpsie  i\n"
		"1:\n"
		:
		: "r"(int_level)
		: "memory"
	);

	__asm volatile ( "dsb" );
	__asm volatile ( "isb" );
}