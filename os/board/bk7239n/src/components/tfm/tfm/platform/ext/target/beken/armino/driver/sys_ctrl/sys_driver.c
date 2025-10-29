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
#include "cmsis_gcc.h"
/**  Platform Start **/
//Platform

/** Platform Misc Start **/
void sys_drv_init()
{
	sys_hal_init();
}

/** Platform Misc End **/

uint32 sys_drv_get_chip_id(void)
{
	uint32 reg = 0;
	reg = sys_hal_get_chip_id();

	return reg;
}

// Replace sddev_control(DD_DEV_TYPE_SCTRL,CMD_GET_DEVICE_ID, NULL)
uint32 sys_drv_get_device_id(void)
{
	uint32 reg = 0;

	reg = sys_hal_get_device_id();

	return reg;
}

int32 sys_drv_set_jtag_mode(uint32 param)
{
	int32 ret = 0;

	ret = sys_hal_set_jtag_mode(param);

	return ret;
}

uint32 sys_drv_get_jtag_mode(void)
{
	int32 ret = 0;

	ret = sys_hal_get_jtag_mode();

	return ret;
}

void sys_drv_en_tempdet(uint32_t value)
{

    sys_hal_en_tempdet(value);
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

	if(value)
		sys_hal_set_bts_wakeup_platform_en(1);
	else
		sys_hal_set_bts_wakeup_platform_en(0);

}
uint32_t system_driver_get_bts_wakeup_platform_en()
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
uint32_t system_driver_get_bts_sleep_exit_req()
{
	return sys_hal_get_bts_sleep_exit_req();
}
#endif

void sys_drv_set_ana_trxt_tst_enable(uint32_t value)
{

    sys_hal_set_ana_trxt_tst_enable(value);
}
void sys_drv_set_ana_scal_en(uint32_t value)
{
    sys_hal_set_ana_scal_en(value);
}
void sys_drv_set_ana_gadc_buf_ictrl(uint32_t value)
{
    sys_hal_set_ana_gadc_buf_ictrl(value);
}
void sys_drv_set_ana_gadc_cmp_ictrl(uint32_t value)
{
    sys_hal_set_ana_gadc_cmp_ictrl(value);
}

void sys_drv_set_ana_pwd_gadc_buf(uint32_t value)
{
    sys_hal_set_ana_pwd_gadc_buf(value);
}
void sys_drv_set_ana_vref_sel(uint32_t value)
{
    sys_hal_set_ana_vref_sel(value);
}
void sys_drv_set_ana_cb_cal_manu(uint32_t value)
{
    sys_hal_set_ana_cb_cal_manu(value);
}
void sys_drv_set_ana_cb_cal_trig(uint32_t value)
{
    sys_hal_set_ana_cb_cal_trig(value);
}

UINT32 sys_drv_get_ana_cb_cal_manu_val(void)
{
    uint32_t cal_manu_val;
    cal_manu_val = sys_hal_get_ana_cb_cal_manu_val();
    return cal_manu_val;
}

void sys_drv_set_ana_cb_cal_manu_val(uint32_t value)
{
    sys_hal_set_ana_cb_cal_manu_val(value);
}

void sys_drv_set_ana_vlsel_ldodig(uint32_t value)
{
    sys_hal_set_ana_vlsel_ldodig(value);
}
void sys_drv_set_ana_vhsel_ldodig(uint32_t value)
{
    sys_hal_set_ana_vhsel_ldodig(value);
}

void sys_drv_set_ana_vctrl_sysldo(uint32_t value)
{
    sys_hal_set_ana_vctrl_sysldo(value);
}

void sys_drv_set_ana_vtempsel(uint32_t value)
{
    sys_hal_set_ana_vtempsel(value);
}

void sys_drv_early_init(void)
{
	sys_hal_early_init();
}

void sys_drv_set_sys2flsh_2wire(uint32_t value)
{
    sys_hal_set_sys2flsh_2wire(value);
}

void sys_drv_enable_int(sys_lock_ctx_t *ctx)
{
	sys_hal_enable_int(ctx->int0, ctx->int1);
}

void sys_drv_disable_int(const sys_lock_ctx_t *ctx)
{
	sys_hal_disable_int((uint32_t*)&ctx->int0, (uint32_t*)&ctx->int1);
}

void sys_drv_set_base_addr(uint32_t addr)
{
	sys_hal_set_base_addr(addr);
}

uint32_t port_disable_interrupts_flag(void)
{
    uint32_t primask_val = __get_PRIMASK();
    __disable_irq();
    return primask_val;
}

void port_enable_interrupts_flag(int val)
{
    __set_PRIMASK(val);
}

void sys_switch_cpu_flash_freq()
{
	sys_hal_flash_set_clk_div(FLASH_CLK_DIV_MAX - 1);
	sys_hal_flash_set_clk(FLASH_CLK_80M);
	sys_hal_flash_set_clk_div(FLASH_CLK_DIV_1);
	sys_hal_switch_cpu_bus_freq(CPU_FRQ_120M);
}

#include "Driver_Flash.h"

void bk_flash_lock(uint32_t *ppro_flash_ns_flag, uint32_t *int_status_val) {
	do {\
			/* *int_status_val = port_disable_interrupts_flag();*/ \
			uint32_t reg;\
			reg = *((volatile uint32_t *)(0x41040000 + PPRO_FLASH_REG * 4));\
			*ppro_flash_ns_flag = reg & PPRO_FLASH_BITS;\
			*((volatile uint32_t *)(0x41040000 + PPRO_FLASH_REG * 4)) = reg & (~PPRO_FLASH_BITS);\
	} while(0);
}


void bk_flash_unlock(uint32_t ppro_flash_ns_flag, uint32_t int_status_val) {
	do {\
			uint32_t reg;\
			reg = *((volatile uint32_t *)(0x41040000 + PPRO_FLASH_REG * 4));\
			if(ppro_flash_ns_flag){\
					*((volatile uint32_t *)(0x41040000 + PPRO_FLASH_REG * 4)) = reg | PPRO_FLASH_BITS;\
			}\
			/*port_enable_interrupts_flag(int_status_val);*/ \
	} while(0);
}