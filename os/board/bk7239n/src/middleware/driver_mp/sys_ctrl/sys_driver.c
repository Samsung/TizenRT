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

#include "sys_hal.h"
#include "sys_driver.h"
#include "sys_driver_common.h"
#include <soc/bk7239n/soc.h>

/**  Platform Start **/
//Platform

/** Platform Misc Start **/
void sys_drv_init()
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_amp_res_init();
	sys_hal_init();

	sys_drv_exit_critical(int_level);
}

/** Platform Misc End **/

uint32 sys_drv_get_chip_id(void)
{
	uint32 reg = 0;
	uint32_t int_level = sys_drv_enter_critical();

	reg = sys_hal_get_chip_id();

	sys_drv_exit_critical(int_level);

	return reg;
}

// Replace sddev_control(DD_DEV_TYPE_SCTRL,CMD_GET_DEVICE_ID, NULL)
uint32 sys_drv_get_device_id(void)
{
	uint32 reg = 0;
	uint32_t int_level = sys_drv_enter_critical();

	reg = sys_hal_get_device_id();

	sys_drv_exit_critical(int_level);

	return reg;
}

int32 sys_drv_set_jtag_mode(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_set_jtag_mode(param);
	sys_drv_exit_critical(int_level);

	return ret;
}

uint32 sys_drv_get_jtag_mode(void)
{
	int32 ret = 0;
	uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_get_jtag_mode();
	sys_drv_exit_critical(int_level);

	return ret;
}

void sys_drv_en_tempdet(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_en_tempdet(value);
    sys_drv_exit_critical(int_level);
}

void sys_drv_set_temp_mode(bool high_volt_mode)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_temp_mode(high_volt_mode);
    sys_drv_exit_critical(int_level);
}
/**  Platform End **/

uint32_t sys_drv_get_cpu_storage_connect_op_select_flash_sel(void)
{
	return 	sys_hal_get_cpu_storage_connect_op_select_flash_sel();
}

void sys_drv_set_cpu_storage_connect_op_select_flash_sel(uint32_t value)
{
	sys_hal_set_cpu_storage_connect_op_select_flash_sel(value);
}

/**  Misc Start **/
//Misc
/**  Misc End **/

#if 1
void system_driver_set_bts_wakeup_platform_en(bool value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	if(value)
		sys_hal_set_bts_wakeup_platform_en(1);
	else
		sys_hal_set_bts_wakeup_platform_en(0);

	if(!ret)
		ret = sys_amp_res_release();

}
uint32_t system_driver_get_bts_wakeup_platform_en(void)
{
	return sys_hal_get_bts_wakeup_platform_en();
}

void system_driver_set_bts_sleep_exit_req(bool value)
{
	if(value)
		sys_hal_set_bts_sleep_exit_req(1);
	else
		sys_hal_set_bts_sleep_exit_req(0);
}
uint32_t system_driver_get_bts_sleep_exit_req(void)
{
	return sys_hal_get_bts_sleep_exit_req();
}
#endif

void sys_drv_set_ana_trxt_tst_enable(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_trxt_tst_enable(value);
    sys_drv_exit_critical(int_level);
}
void sys_drv_set_ana_scal_en(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_scal_en(value);
    sys_drv_exit_critical(int_level);
}
void sys_drv_set_ana_gadc_buf_ictrl(uint32_t value)
{
   uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_gadc_buf_ictrl(value);
    sys_drv_exit_critical(int_level);
}
void sys_drv_set_ana_gadc_cmp_ictrl(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_gadc_cmp_ictrl(value);
    sys_drv_exit_critical(int_level);
}

void sys_drv_set_ana_pwd_gadc_buf(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_pwd_gadc_buf(value);
    sys_drv_exit_critical(int_level);
}

void sys_drv_set_ana_hres_sel0v9(void)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_hres_sel0v9(0x0);
    sys_hal_set_ana_hres_sel0v9(0x1);
    sys_drv_exit_critical(int_level);
}

void sys_drv_set_ana_vref_sel(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_vref_sel(value);
    sys_drv_exit_critical(int_level);
}
void sys_drv_set_ana_cb_cal_manu(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_cb_cal_manu(value);
    sys_drv_exit_critical(int_level);
}
void sys_drv_set_ana_cb_cal_trig(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_cb_cal_trig(value);
    sys_drv_exit_critical(int_level);
}

UINT32 sys_drv_get_ana_cb_cal_manu_val(void)
{
    uint32_t cal_manu_val;
    uint32_t int_level = sys_drv_enter_critical();

    cal_manu_val = sys_hal_get_ana_cb_cal_manu_val();
    sys_drv_exit_critical(int_level);
    return cal_manu_val;
}

void sys_drv_set_ana_cb_cal_manu_val(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_cb_cal_manu_val(value);
    sys_drv_exit_critical(int_level);
}

__IRAM_SEC void sys_drv_set_ana_reg11_apfms(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_reg11_apfms(value);
    sys_drv_exit_critical(int_level);
}

__IRAM_SEC void sys_drv_set_ana_reg12_dpfms(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_reg12_dpfms(value);
    sys_drv_exit_critical(int_level);
}

void sys_drv_set_ana_vlsel_ldodig(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();
    sys_hal_set_ana_vlsel_ldodig(value);
    sys_drv_exit_critical(int_level);
}
void sys_drv_set_ana_vhsel_ldodig(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();
    sys_hal_set_ana_vhsel_ldodig(value);
    sys_drv_exit_critical(int_level);
}

void sys_drv_set_ana_vctrl_sysldo(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();
    sys_hal_set_ana_vctrl_sysldo(value);
    sys_drv_exit_critical(int_level);
}

void sys_drv_set_ana_vtempsel(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();
    sys_hal_set_ana_vtempsel(value);
    sys_drv_exit_critical(int_level);
}

void sys_drv_set_ana_ioldo_lp(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();
    sys_hal_set_ioldo_lp(value);
    sys_drv_exit_critical(int_level);
}

void sys_drv_set_ana_com_port_enable(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_slcd_com_enable(value);

    sys_drv_exit_critical(int_level);
}

void sys_drv_set_ana_seg_port_enable(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_slcd_seg_enable(value);

    sys_drv_exit_critical(int_level);
}

uint32_t sys_drv_get_ana_seg_port_enable_status(void)
{
    uint32_t int_level = sys_drv_enter_critical();

    uint32_t status = sys_hal_get_slcd_seg_enable_status();

	sys_drv_exit_critical(int_level);

	return status;
}

void sys_drv_set_ana_crb(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_slcd_crb(value);

    sys_drv_exit_critical(int_level);
}

void sys_drv_set_ana_sw_bias(uint32_t value)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_slcd_sw_bias(value);

    sys_drv_exit_critical(int_level);
}

void sys_drv_set_ana_lcd_enable(void)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_slcd_enable(0x1);

    sys_drv_exit_critical(int_level);
}

void sys_drv_set_ana_lcd_disable(void)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_slcd_enable(0x0);

    sys_drv_exit_critical(int_level);
}

__FLASH_BOOT_CODE void sys_drv_early_init(void)
{
	sys_hal_early_init();
}

__IRAM_SEC void sys_drv_set_sys2flsh_2wire(uint32_t value)
{
    sys_hal_set_sys2flsh_2wire(value);
}


void sys_drv_set_cpu0_rxevt_sel(uint32 value) {
#if defined(CONFIG_SOC_BK7236XX)
    sys_hal_set_cpu0_rxevt_sel(value);
#endif
}

void sys_drv_set_cpu1_rxevt_sel(uint32 value) {
#if defined(CONFIG_SOC_BK7236XX)
    sys_hal_set_cpu1_rxevt_sel(value);
#endif
}

void sys_drv_set_cpu2_rxevt_sel(uint32 value) {
#if defined(CONFIG_SOC_BK7236XX)
    sys_hal_set_cpu2_rxevt_sel(value);
#endif 
}