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
/*clock power control start*/
void sys_drv_dev_clk_pwr_up(dev_clk_pwr_id_t dev, dev_clk_pwr_ctrl_t power_up)
{
	sys_hal_clk_pwr_ctrl(dev, power_up);
}

void sys_drv_set_clk_select(dev_clk_select_id_t dev, dev_clk_select_t clk_sel)
{
	sys_hal_set_clk_select(dev, clk_sel);
}

dev_clk_select_t sys_drv_get_clk_select(dev_clk_select_id_t dev)
{
	dev_clk_select_t clk_sel;
	clk_sel = sys_hal_get_clk_select(dev);
	return clk_sel;
}

void sys_drv_set_clk_div(dev_clk_select_id_t dev, uint32_t clk_div)
{
	sys_hal_set_clk_div(dev, clk_div);
}

//DCO divider is valid for all of the peri-devices.
void sys_drv_set_dco_div(dev_clk_dco_div_t div)
{
	sys_hal_set_dco_div(div);
}

//DCO divider is valid for all of the peri-devices.
dev_clk_dco_div_t sys_drv_get_dco_div(void)
{
	dev_clk_dco_div_t dco_div;
	dco_div = sys_hal_get_dco_div();
	return dco_div;
}
/*clock power control end*/
void sys_drv_sadc_pwr_up(void)
{
    sys_hal_sadc_pwr_up();
}
void sys_drv_sadc_pwr_down(void)
{
    sys_hal_sadc_pwr_down();
}
/* Platform UART Start **/
void sys_drv_uart_select_clock(uart_id_t id, uart_src_clk_t mode)
{
	sys_hal_uart_select_clock(id, mode);
}

/* Platform UART End **/
/* Platform PWM Start **/
void sys_drv_pwm_set_clock(uint32_t mode, uint32_t param)
{
	sys_hal_pwm_set_clock(mode, param);
}

void sys_drv_pwm_select_clock(sys_sel_pwm_t num, pwm_src_clk_t mode)
{
	sys_hal_pwm_select_clock(num, mode);
}

/* Platform PWM End **/
void sys_drv_timer_select_clock(sys_sel_timer_t num, timer_src_clk_t mode)
{
	sys_hal_timer_select_clock(num, mode);
}
#if 0
void sys_drv_usb_clock_ctrl(bool ctrl, void *arg)
{
	sys_hal_usb_enable_clk(ctrl);
}
#endif
//sys_ctrl CMD: CMD_SCTRL_SET_FLASH_DCO
void sys_drv_flash_set_dco(void)
{
	sys_hal_flash_set_dco();
}

//sys_ctrl CMD: CMD_SCTRL_SET_FLASH_DPLL
void sys_drv_flash_set_dpll(void)
{
	sys_hal_flash_set_dpll();
}

void sys_drv_flash_cksel(uint32_t value)
{
	sys_hal_flash_set_clk(value);
}

void sys_drv_flash_set_clk_div(uint32_t value)
{
	sys_hal_flash_set_clk_div(value);
}

uint32_t sys_drv_flash_get_clk_sel(void)
{
	return sys_hal_flash_get_clk_sel();
}

uint32_t sys_drv_flash_get_clk_div(void)
{
	return sys_hal_flash_get_clk_div();
}

//sys_ctrl CMD: CMD_QSPI_CLK_SEL
void sys_drv_qspi_clk_sel(uint32_t param)
{
	sys_hal_qspi_clk_sel(param);
}

void sys_drv_qspi_set_src_clk_div(uint32_t value)
{
	sys_hal_qspi_set_src_clk_div(value);
}

uint32_t sys_drv_psram_clk_sel(uint32_t value)
{
	sys_hal_psram_clk_sel(value);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_psram_set_clkdiv(uint32_t value)
{
	sys_hal_psram_set_clkdiv(value);
	return SYS_DRV_SUCCESS;
}
uint32_t sys_drv_i2s_select_clock(uint32_t value)
{
	sys_hal_i2s_select_clock(value);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_i2s_clock_en(uint32_t value)
{
#if (CONFIG_SOC_BK7256)
	sys_hal_aud_clock_en(value);
#else
	sys_hal_i2s_clock_en(value);
#endif
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_i2s1_clock_en(uint32_t value)
{
	sys_hal_i2s1_clock_en(value);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_i2s2_clock_en(uint32_t value)
{
	sys_hal_i2s2_clock_en(value);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_fft_disckg_set(uint32_t value)
{
	sys_hal_fft_disckg_set(value);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_i2s_disckg_set(uint32_t value)
{
	sys_hal_i2s_disckg_set(value);
	return SYS_DRV_SUCCESS;
}
void sys_drv_nmi_wdt_set_clk_div(uint32_t value)
{
	sys_hal_nmi_wdt_set_clk_div(value);
}
uint32_t sys_drv_nmi_wdt_get_clk_div(void)
{
	return sys_hal_nmi_wdt_get_clk_div();
}
void sys_drv_trng_disckg_set(uint32_t value)
{
	sys_hal_trng_disckg_set(value);
}
void sys_drv_yuv_buf_pwr_up(void)
{
	sys_hal_set_yuv_buf_clock_en(1);
}

void sys_drv_yuv_buf_pwr_down(void)
{
	sys_hal_set_yuv_buf_clock_en(0);
}

void sys_drv_h264_pwr_up(void)
{
	sys_hal_set_h264_clock_en(1);
}

void sys_drv_h264_pwr_down(void)
{
	sys_hal_set_h264_clock_en(0);
}

