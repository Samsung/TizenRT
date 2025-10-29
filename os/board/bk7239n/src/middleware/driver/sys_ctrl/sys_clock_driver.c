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

bk_err_t bk_pm_clock_ctrl(pm_dev_clk_e module, pm_dev_clk_pwr_e clock_state);

/*clock power control start*/
__IRAM_SEC void sys_drv_dev_clk_pwr_up(dev_clk_pwr_id_t dev, dev_clk_pwr_ctrl_t power_up)
{

#if defined(CONFIG_SYS_CPU1)
	bk_pm_clock_ctrl(dev, power_up);
#else
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_clk_pwr_ctrl(dev, power_up);
	sys_drv_exit_critical(int_level);
#endif

}

void sys_drv_set_clk_select(dev_clk_select_id_t dev, dev_clk_select_t clk_sel)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_set_clk_select(dev, clk_sel);

	sys_drv_exit_critical(int_level);
}

dev_clk_select_t sys_drv_get_clk_select(dev_clk_select_id_t dev)
{
	dev_clk_select_t clk_sel;

	uint32_t int_level = sys_drv_enter_critical();

	clk_sel = sys_hal_get_clk_select(dev);

	sys_drv_exit_critical(int_level);

	return clk_sel;
}

//DCO divider is valid for all of the peri-devices.
void sys_drv_set_dco_div(dev_clk_dco_div_t div)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_set_dco_div(div);

	sys_drv_exit_critical(int_level);
}

//DCO divider is valid for all of the peri-devices.
dev_clk_dco_div_t sys_drv_get_dco_div(void)
{
	dev_clk_dco_div_t dco_div;

	uint32_t int_level = sys_drv_enter_critical();

	dco_div = sys_hal_get_dco_div();

	sys_drv_exit_critical(int_level);

	return dco_div;
}

/*clock power control end*/

void sys_drv_sadc_pwr_up(void)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_sadc_pwr_up();
    sys_drv_exit_critical(int_level);
}

void sys_drv_sadc_pwr_down(void)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_sadc_pwr_down();
    sys_drv_exit_critical(int_level);
}

#if defined(CONFIG_SDIO_V2P0)
void sys_driver_set_sdio_clk_en(uint32_t value)
{
	if (value) {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SDIO, CLK_PWR_CTRL_PWR_UP);
	} else {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SDIO, CLK_PWR_CTRL_PWR_DOWN);
	}
}

void sys_driver_set_sdio_clk_div(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_set_sdio_clk_div(value);

	sys_drv_exit_critical(int_level);
}

uint32_t sys_driver_get_sdio_clk_div()
{
	uint32_t reg_v;
	uint32_t int_level = sys_drv_enter_critical();

	reg_v = sys_hal_get_sdio_clk_div();
	sys_drv_exit_critical(int_level);

	return reg_v;
}

void sys_driver_set_sdio_clk_sel(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_set_sdio_clk_sel(value);

	sys_drv_exit_critical(int_level);
}

uint32_t sys_driver_get_sdio_clk_sel()
{
	uint32_t reg_v;
	uint32_t int_level = sys_drv_enter_critical();

	reg_v = sys_hal_get_sdio_clk_sel();
	sys_drv_exit_critical(int_level);

	return reg_v;
}
#endif

/* Platform UART Start **/
void sys_drv_uart_select_clock(uart_id_t id, uart_src_clk_t mode)
{
	uint32_t int_level = 0;
	uint32_t ret = SYS_DRV_FAILURE;

	ret = sys_amp_res_acquire();
	int_level = sys_drv_enter_critical();

	sys_hal_uart_select_clock(id, mode);

	sys_drv_exit_critical(int_level);
	if(!ret)
		ret = sys_amp_res_release();
}
/* Platform UART End **/

/* Platform I2C Start **/
void sys_drv_i2c_select_clock(i2c_id_t id, i2c_src_clk_t mode)
{
	uint32_t int_level = 0;
	uint32_t ret = SYS_DRV_FAILURE;

	ret = sys_amp_res_acquire();
	int_level = sys_drv_enter_critical();

	sys_hal_i2c_select_clock(id, mode);

	sys_drv_exit_critical(int_level);
	if(!ret)
		ret = sys_amp_res_release();
}
/* Platform I2C End **/

/* Platform PWM Start **/
void sys_drv_pwm_set_clock(uint32_t mode, uint32_t param)
{
	uint32_t int_level = 0;
	uint32_t ret = SYS_DRV_FAILURE;

	ret = sys_amp_res_acquire();
	int_level = sys_drv_enter_critical();

	sys_hal_pwm_set_clock(mode, param);

	sys_drv_exit_critical(int_level);
	if(!ret)
		ret = sys_amp_res_release();
}

void sys_drv_pwm_select_clock(sys_sel_pwm_t num, pwm_src_clk_t mode)
{
	uint32_t int_level = 0;
	uint32_t ret = SYS_DRV_FAILURE;

	ret = sys_amp_res_acquire();
	int_level = sys_drv_enter_critical();

	sys_hal_pwm_select_clock(num, mode);

	sys_drv_exit_critical(int_level);
	if(!ret)
		ret = sys_amp_res_release();
}
/* Platform PWM End **/

void sys_drv_timer_select_clock(sys_sel_timer_t num, timer_src_clk_t mode)
{
//	uint32_t int_level = sys_drv_enter_critical();
//	uint32_t ret = SYS_DRV_FAILURE;
//	ret = sys_amp_res_acquire();

	sys_hal_timer_select_clock(num, mode);

//	if(!ret)
//		ret = sys_amp_res_release();

//	sys_drv_exit_critical(int_level);
}

void sys_drv_usb_clock_ctrl(bool ctrl, void *arg)
{
	#if 0//TODO
	if (ctrl) {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_USB_1, CLK_PWR_CTRL_PWR_UP);
	} else {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_USB_1, CLK_PWR_CTRL_PWR_DOWN);
	}
	#endif
}

//sys_ctrl CMD: CMD_SCTRL_SET_FLASH_DCO
void sys_drv_flash_set_dco(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_flash_set_dco();

	sys_drv_exit_critical(int_level);
}

//sys_ctrl CMD: CMD_SCTRL_SET_FLASH_DPLL
void sys_drv_flash_set_dpll(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_flash_set_dpll();

	sys_drv_exit_critical(int_level);
}

void sys_drv_flash_cksel(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_flash_set_clk(value);

	sys_drv_exit_critical(int_level);
}

void sys_drv_flash_set_clk_div(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_flash_set_clk_div(value);

	sys_drv_exit_critical(int_level);
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
void sys_drv_qspi_clk_sel(uint32_t id, uint32_t param)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_qspi_clk_sel(id, param);

	sys_drv_exit_critical(int_level);
}

void sys_drv_qspi_set_src_clk_div(uint32_t id, uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_qspi_set_src_clk_div(id, value);

	sys_drv_exit_critical(int_level);
}

uint32_t sys_drv_psram_clk_sel(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_psram_clk_sel(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_psram_set_clkdiv(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_psram_set_clkdiv(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_i2s_select_clock(uint32_t value)
{
	uint32_t int_level = 0;
	uint32_t ret = SYS_DRV_FAILURE;

	ret = sys_amp_res_acquire();
	int_level = sys_drv_enter_critical();

	sys_hal_i2s_select_clock(value);

	sys_drv_exit_critical(int_level);
	if(!ret)
		ret = sys_amp_res_release();
	return ret;
}

uint32_t sys_drv_i2s_clock_en(uint32_t value)
{
	if (value) {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_I2S_1, CLK_PWR_CTRL_PWR_UP);
	} else {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_I2S_1, CLK_PWR_CTRL_PWR_DOWN);
	}
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_i2s1_clock_en(uint32_t value)
{
	#if 0 //TODO
	if (value) {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_I2S2, CLK_PWR_CTRL_PWR_UP);
	} else {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_I2S2, CLK_PWR_CTRL_PWR_DOWN);
	}
	#endif
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_i2s2_clock_en(uint32_t value)
{
	#if 0//TODO
	if (value) {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_I2S3, CLK_PWR_CTRL_PWR_UP);
	} else {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_I2S3, CLK_PWR_CTRL_PWR_DOWN);
	}
	#endif
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_fft_disckg_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_fft_disckg_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_i2s_disckg_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_i2s_disckg_set(value);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

void sys_drv_nmi_wdt_set_clk_div(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_nmi_wdt_set_clk_div(value);

	sys_drv_exit_critical(int_level);
}

uint32_t sys_drv_nmi_wdt_get_clk_div(void)
{
	return sys_hal_nmi_wdt_get_clk_div();
}

void sys_drv_trng_disckg_set(uint32_t value)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_trng_disckg_set(value);
	sys_drv_exit_critical(int_level);
}

void sys_drv_yuv_buf_pwr_up(void)
{
	#if 0//TODO
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_YUV, CLK_PWR_CTRL_PWR_UP);
	#endif
}

void sys_drv_yuv_buf_pwr_down(void)
{
	#if 0//TODO
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_YUV, CLK_PWR_CTRL_PWR_DOWN);
	#endif
}

void sys_drv_h264_pwr_up(void)
{
	#if 0//TODO
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_H264, CLK_PWR_CTRL_PWR_UP);
	#endif
}

void sys_drv_h264_pwr_down(void)
{
	#if 0 //TODO
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_H264, CLK_PWR_CTRL_PWR_DOWN);
	#endif
}

void sys_drv_slcd_clock_enable(void)
{
	#if 0//TODO
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SLCD, CLK_PWR_CTRL_PWR_UP);
	#endif
}

void sys_drv_slcd_clock_disable(void)
{
	#if 0//TODO
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SLCD, CLK_PWR_CTRL_PWR_DOWN);
	#endif
}


