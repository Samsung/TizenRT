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

#include "hal_config.h"
#include "system_hw.h"

#if 0
#if CFG_HAL_DEBUG_SYS_CLOCK

void sys_clock_struct_dump(void)
{
	sys_hw_t *hw = (sys_hw_t *)SOC_SYSTEM_REG_BASE;
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("  reg_0x8 addr=%x value=%x\n", &hw->reg0x8, hw->reg0x8.v);
	SOC_LOGI("    clkdiv_core:   %x\n", hw->reg0x8.clkdiv_core);
	SOC_LOGI("    cksel_core:    %x\n", hw->reg0x8.cksel_core);
	SOC_LOGI("    clkdiv_bus:    %x\n", hw->reg0x8.clkdiv_bus);
	SOC_LOGI("    clkdiv_uart0:  %x\n", hw->reg0x8.clkdiv_uart0);
	SOC_LOGI("    clksel_uart0:  %x\n", hw->reg0x8.clksel_uart0);
	SOC_LOGI("    clkdiv_uart1:  %x\n", hw->reg0x8.clkdiv_uart1);
	SOC_LOGI("    cksel_uart1:   %x\n", hw->reg0x8.cksel_uart1);
	SOC_LOGI("    clkdiv_uart2:  %x\n", hw->reg0x8.clkdiv_uart2);
	SOC_LOGI("    cksel_uart2:   %x\n", hw->reg0x8.cksel_uart2);
	SOC_LOGI("    cksel_sadc:    %x\n", hw->reg0x8.cksel_sadc);
	SOC_LOGI("    cksel_pwm0:    %x\n", hw->reg0x8.cksel_pwm0);
	SOC_LOGI("    cksel_pwm1:    %x\n", hw->reg0x8.cksel_pwm1);
	SOC_LOGI("    cksel_tim0:    %x\n", hw->reg0x8.cksel_tim0);
	SOC_LOGI("    cksel_tim1:    %x\n", hw->reg0x8.cksel_tim1);
	SOC_LOGI("    cksel_tim2:    %x\n", hw->reg0x8.cksel_tim2);
	SOC_LOGI("    reserved0:     %x\n", hw->reg0x8.reserved0);
	SOC_LOGI("    cksel_i2s:     %x\n", hw->reg0x8.cksel_i2s);
	SOC_LOGI("    cksel_aud:     %x\n", hw->reg0x8.cksel_aud);
	SOC_LOGI("    clkdiv_jpeg:   %x\n", hw->reg0x8.clkdiv_jpeg);
	SOC_LOGI("    cksel_jpeg:    %x\n", hw->reg0x8.cksel_jpeg);
	SOC_LOGI("    clkdiv_disp_l: %x\n", hw->reg0x8.clkdiv_disp_l);
	SOC_LOGI("\r\n");

	SOC_LOGI("  reg_0x9 addr=%x value=%x\n", &hw->reg0x9, hw->reg0x9.v);
	SOC_LOGI("    clkdiv_disp_h: %x\n", hw->reg0x9.clkdiv_disp_h);
	SOC_LOGI("    cksel_disp:    %x\n", hw->reg0x9.cksel_disp);
	SOC_LOGI("    ckdiv_psram:   %x\n", hw->reg0x9.ckdiv_psram);
	SOC_LOGI("    cksel_psram:   %x\n", hw->reg0x9.cksel_psram);
	SOC_LOGI("    ckdiv_qspi0:   %x\n", hw->reg0x9.ckdiv_qspi0);
	SOC_LOGI("    reserved0:     %x\n", hw->reg0x9.reserved0);
	SOC_LOGI("    ckdiv_sdio:    %x\n", hw->reg0x9.ckdiv_sdio);
	SOC_LOGI("    cksel_sdio:    %x\n", hw->reg0x9.cksel_sdio);
	SOC_LOGI("    ckdiv_auxs:    %x\n", hw->reg0x9.ckdiv_auxs);
	SOC_LOGI("    cksel_auxs:    %x\n", hw->reg0x9.cksel_auxs);
	SOC_LOGI("    cksel_flash:   %x\n", hw->reg0x9.cksel_flash);
	SOC_LOGI("    ckdiv_flash:   %x\n", hw->reg0x9.ckdiv_flash);
	SOC_LOGI("    ckdiv_i2s0:    %x\n", hw->reg0x9.ckdiv_i2s0);
	SOC_LOGI("    reserved1:     %x\n", hw->reg0x9.reserved1);
	SOC_LOGI("\r\n");

	SOC_LOGI("  reg_0xa addr=%x value=%x\n", &hw->reg0xa, hw->reg0xa.v);
	SOC_LOGI("    ckdiv_26m:   %x\n", hw->reg0xa.ckdiv_26m);
	SOC_LOGI("    ckdiv_wdt:   %x\n", hw->reg0xa.ckdiv_wdt);
	SOC_LOGI("    clksel_spi0: %x\n", hw->reg0xa.clksel_spi0);
	SOC_LOGI("    clksel_spi1: %x\n", hw->reg0xa.clksel_spi1);
	SOC_LOGI("    reserved0:   %x\n", hw->reg0xa.reserved0);
	SOC_LOGI("\r\n");

	SOC_LOGI("  reg_0xc addr=%x value=%x\n", &hw->reg0xc, hw->reg0xc.v);
	SOC_LOGI("    i2c0_cken:  %x\n", hw->reg0xc.i2c0_cken);
	SOC_LOGI("    spi0_cken:  %x\n", hw->reg0xc.spi0_cken);
	SOC_LOGI("    uart0_cken: %x\n", hw->reg0xc.uart0_cken);
	SOC_LOGI("    pwm0_cken:  %x\n", hw->reg0xc.pwm0_cken);
	SOC_LOGI("    tim0_cken:  %x\n", hw->reg0xc.tim0_cken);
	SOC_LOGI("    sadc_cken:  %x\n", hw->reg0xc.sadc_cken);
	SOC_LOGI("    irda_cken:  %x\n", hw->reg0xc.irda_cken);
	SOC_LOGI("    efuse_cken: %x\n", hw->reg0xc.efuse_cken);
	SOC_LOGI("    i2c1_cken:  %x\n", hw->reg0xc.i2c1_cken);
	SOC_LOGI("    spi1_cken:  %x\n", hw->reg0xc.spi1_cken);
	SOC_LOGI("    uart1_cken: %x\n", hw->reg0xc.uart1_cken);
	SOC_LOGI("    uart2_cken: %x\n", hw->reg0xc.uart2_cken);
	SOC_LOGI("    pwm1_cken:  %x\n", hw->reg0xc.pwm1_cken);
	SOC_LOGI("    tim1_cken:  %x\n", hw->reg0xc.tim1_cken);
	SOC_LOGI("    tim2_cken:  %x\n", hw->reg0xc.tim2_cken);
	SOC_LOGI("    otp_cken:   %x\n", hw->reg0xc.otp_cken);
	SOC_LOGI("    i2s_cken:   %x\n", hw->reg0xc.i2s_cken);
	SOC_LOGI("    usb_cken:   %x\n", hw->reg0xc.usb_cken);
	SOC_LOGI("    can_cken:   %x\n", hw->reg0xc.can_cken);
	SOC_LOGI("    psram_cken: %x\n", hw->reg0xc.psram_cken);
	SOC_LOGI("    qspi0_cken: %x\n", hw->reg0xc.qspi0_cken);
	SOC_LOGI("    qspi1_cken: %x\n", hw->reg0xc.qspi1_cken);
	SOC_LOGI("    sdio_cken:  %x\n", hw->reg0xc.sdio_cken);
	SOC_LOGI("    auxs_cken:  %x\n", hw->reg0xc.auxs_cken);
	SOC_LOGI("    btdm_cken:  %x\n", hw->reg0xc.btdm_cken);
	SOC_LOGI("    xvr_cken:   %x\n", hw->reg0xc.xvr_cken);
	SOC_LOGI("    mac_cken:   %x\n", hw->reg0xc.mac_cken);
	SOC_LOGI("    phy_cken:   %x\n", hw->reg0xc.phy_cken);
	SOC_LOGI("    jpeg_cken:  %x\n", hw->reg0xc.jpeg_cken);
	SOC_LOGI("    disp_cken:  %x\n", hw->reg0xc.disp_cken);
	SOC_LOGI("    aud_cken:   %x\n", hw->reg0xc.aud_cken);
	SOC_LOGI("    wdt_cken:   %x\n", hw->reg0xc.wdt_cken);
	SOC_LOGI("\r\n");

	SOC_LOGI("  reg_0xd addr=%x value=%x\n", &hw->reg0xd, hw->reg0xd.v);

	SOC_LOGI("  reg_0x20 addr=%x value=%x\n", &hw->reg0x20, hw->reg0x20.v);
	SOC_LOGI("  reg_0x21 addr=%x value=%x\n", &hw->reg0x21, hw->reg0x21.v);
}

#endif
#endif

