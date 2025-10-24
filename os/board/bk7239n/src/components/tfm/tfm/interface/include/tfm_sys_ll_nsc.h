// Copyright 2022-2023 Beken
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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#include <soc/soc.h>
#include "hal_port.h"
#include "system_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYS_LL_REG_BASE   SOC_SYS_REG_BASE

//This way of setting ana_reg_bit value is only for sys_ctrl, other driver please implement by yourself!!

#if CONFIG_ANA_REG_WRITE_POLL_REG_B
#define SYS_ANALOG_REG_SPI_STATE_REG (SYS_CPU_ANASPI_FREQ_ADDR)
#define SYS_ANALOG_REG_SPI_STATE_POS(idx) (idx + 8)
#else
#define SYS_ANALOG_REG_SPI_STATE_REG (SOC_SYS_REG_BASE + (0x3a << 2))
#define SYS_ANALOG_REG_SPI_STATE_POS(idx) (idx)
#endif

#define GET_SYS_ANALOG_REG_IDX(addr) ((addr - SYS_ANA_REG0_ADDR) >> 2)

uint32_t sys_ll_get_analog_reg_value(uint32_t addr);

void sys_ll_set_analog_reg_value(uint32_t addr, uint32_t value);

void sys_set_ana_reg_bit(uint32_t reg_addr, uint32_t pos, uint32_t mask, uint32_t value);

void sys_ll_set_device_id_value(uint32_t v);

uint32_t sys_ll_get_device_id_value(void);

uint32_t sys_ll_get_device_id_deviceid(void);

void sys_ll_set_version_id_value(uint32_t v);

uint32_t sys_ll_get_version_id_value(void);

uint32_t sys_ll_get_version_id_versionid(void);

//reg cpu_storage_connect_op_select:

void sys_ll_set_cpu_storage_connect_op_select_value(uint32_t v);

uint32_t sys_ll_get_cpu_storage_connect_op_select_value(void);

void sys_ll_set_cpu_storage_connect_op_select_boot_mode(uint32_t v);

uint32_t sys_ll_get_cpu_storage_connect_op_select_boot_mode(void);

void sys_ll_set_cpu_storage_connect_op_select_jtag_core_sel(uint32_t v);

uint32_t sys_ll_get_cpu_storage_connect_op_select_jtag_core_sel(void);

void sys_ll_set_cpu_storage_connect_op_select_flash_sel(uint32_t v);

uint32_t sys_ll_get_cpu_storage_connect_op_select_flash_sel(void);

//reg cpu_current_run_status:

void sys_ll_set_cpu_current_run_status_value(uint32_t v);

uint32_t sys_ll_get_cpu_current_run_status_value(void);

uint32_t sys_ll_get_cpu_current_run_status_core0_halted(void);

uint32_t sys_ll_get_cpu_current_run_status_core1_halted(void);

uint32_t sys_ll_get_cpu_current_run_status_cpu0_sw_reset(void);

uint32_t sys_ll_get_cpu_current_run_status_cpu1_sw_reset(void);

uint32_t sys_ll_get_cpu_current_run_status_cpu0_pwr_dw_state(void);

uint32_t sys_ll_get_cpu_current_run_status_cpu1_pwr_dw_state(void);

//reg cpu0_int_halt_clk_op:

void sys_ll_set_cpu0_int_halt_clk_op_value(uint32_t v);

uint32_t sys_ll_get_cpu0_int_halt_clk_op_value(void);

void sys_ll_set_cpu0_int_halt_clk_op_cpu0_sw_rst(uint32_t v);

uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_sw_rst(void);

void sys_ll_set_cpu0_int_halt_clk_op_cpu0_pwr_dw(uint32_t v);

uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_pwr_dw(void);

void sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask(uint32_t v);

uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_int_mask(void);

void sys_ll_set_cpu0_int_halt_clk_op_cpu0_halt(uint32_t v);

uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_halt(void);

void sys_ll_set_cpu0_int_halt_clk_op_reserved_4_7(uint32_t v);

uint32_t sys_ll_get_cpu0_int_halt_clk_op_reserved_4_7(void);

uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_offset(void);

//reg cpu1_int_halt_clk_op:

void sys_ll_set_cpu1_int_halt_clk_op_value(uint32_t v);

uint32_t sys_ll_get_cpu1_int_halt_clk_op_value(void);

void sys_ll_set_cpu1_int_halt_clk_op_cpu1_sw_rst(uint32_t v);

uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_sw_rst(void);

void sys_ll_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(uint32_t v);

uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_pwr_dw(void);

void sys_ll_set_cpu1_int_halt_clk_op_cpu1_int_mask(uint32_t v);

uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_int_mask(void);

void sys_ll_set_cpu1_int_halt_clk_op_cpu1_halt(uint32_t v);

uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_halt(void);

void sys_ll_set_cpu1_int_halt_clk_op_cpu0_bus_clk_2div(uint32_t v);

uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu0_bus_clk_2div(void);

void sys_ll_set_cpu1_int_halt_clk_op_reserved_5_7(uint32_t v);

uint32_t sys_ll_get_cpu1_int_halt_clk_op_reserved_5_7(void);

void sys_ll_set_cpu1_int_halt_clk_op_cpu1_offset(uint32_t v);

uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_offset(void);

//reg reserved_reg0x6:

void sys_ll_set_reserved_reg0x6_value(uint32_t v);

uint32_t sys_ll_get_reserved_reg0x6_value(void);

void sys_ll_set_reserved_reg0x6_reserved_0_31(uint32_t v);

uint32_t sys_ll_get_reserved_reg0x6_reserved_0_31(void);

//reg cpu_clk_div_mode1:

void sys_ll_set_cpu_clk_div_mode1_value(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_value(void);

void sys_ll_set_cpu_clk_div_mode1_clkdiv_core(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_core(void);

void sys_ll_set_cpu_clk_div_mode1_cksel_core(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_core(void);

void sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_bus(void);

void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart0(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart0(void);

void sys_ll_set_cpu_clk_div_mode1_clksel_uart0(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_clksel_uart0(void);

void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart1(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart1(void);

void sys_ll_set_cpu_clk_div_mode1_cksel_uart1(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart1(void);

void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart2(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart2(void);

void sys_ll_set_cpu_clk_div_mode1_cksel_uart2(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart2(void);

void sys_ll_set_cpu_clk_div_mode1_cksel_sadc(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_sadc(void);

void sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_pwm0(void);

void sys_ll_set_cpu_clk_div_mode1_cksel_pwm1(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_pwm1(void);

void sys_ll_set_cpu_clk_div_mode1_cksel_tim0(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_tim0(void);

void sys_ll_set_cpu_clk_div_mode1_cksel_tim1(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_tim1(void);

void sys_ll_set_cpu_clk_div_mode1_cksel_tim2(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_tim2(void);

void sys_ll_set_cpu_clk_div_mode1_cksel_i2s(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_i2s(void);

void sys_ll_set_cpu_clk_div_mode1_cksel_aud(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_aud(void);

void sys_ll_set_cpu_clk_div_mode1_clkdiv_jpeg(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_jpeg(void);

void sys_ll_set_cpu_clk_div_mode1_cksel_jpeg(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_jpeg(void);

void sys_ll_set_cpu_clk_div_mode1_clkdiv_disp_l(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_disp_l(void);

//reg cpu_clk_div_mode2:

void sys_ll_set_cpu_clk_div_mode2_value(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_value(void);

void sys_ll_set_cpu_clk_div_mode2_clkdiv_disp_h(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_clkdiv_disp_h(void);

void sys_ll_set_cpu_clk_div_mode2_cksel_disp(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_disp(void);

void sys_ll_set_cpu_clk_div_mode2_ckdiv_psram(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_psram(void);

void sys_ll_set_cpu_clk_div_mode2_cksel_psram(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_psram(void);

void sys_ll_set_cpu_clk_div_mode2_ckdiv_qspi0(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_qspi0(void);

void sys_ll_set_cpu_clk_div_mode2_cksel_qspi0(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_qspi0(void);

void sys_ll_set_cpu_clk_div_mode2_reserved_11_13(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_reserved_11_13(void);

void sys_ll_set_cpu_clk_div_mode2_ckdiv_sdio(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_sdio(void);

void sys_ll_set_cpu_clk_div_mode2_cksel_sdio(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_sdio(void);

void sys_ll_set_cpu_clk_div_mode2_ckdiv_auxs(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_auxs(void);

void sys_ll_set_cpu_clk_div_mode2_cksel_auxs(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_auxs(void);

void sys_ll_set_cpu_clk_div_mode2_cksel_flash(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_flash(void);

void sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_flash(void);

void sys_ll_set_cpu_clk_div_mode2_ckdiv_i2s0(uint32_t v);

uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_i2s0(void);

//reg cpu_26m_wdt_clk_div:

void sys_ll_set_cpu_26m_wdt_clk_div_value(uint32_t v);

uint32_t sys_ll_get_cpu_26m_wdt_clk_div_value(void);

void sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_26m(uint32_t v);

uint32_t sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_26m(void);

void sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_wdt(uint32_t v);

uint32_t sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_wdt(void);

void sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(uint32_t v);

uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_spi0(void);

void sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(uint32_t v);

uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_spi1(void);

void sys_ll_set_cpu_26m_wdt_clk_div_clksel_qspi1(uint32_t v);

uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_qspi1(void);

void sys_ll_set_cpu_26m_wdt_clk_div_clkdiv_qspi1(uint32_t v);

uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clkdiv_qspi1(void);

//reg cpu_anaspi_freq:

void sys_ll_set_cpu_anaspi_freq_value(uint32_t v);

uint32_t sys_ll_get_cpu_anaspi_freq_value(void);

void sys_ll_set_cpu_anaspi_freq_anaspi_freq(uint32_t v);

uint32_t sys_ll_get_cpu_anaspi_freq_anaspi_freq(void);

//reg cpu_device_clk_enable:

void sys_ll_set_cpu_device_clk_enable_value(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_value(void);

void sys_ll_set_cpu_device_clk_enable_i2c0_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_i2c0_cken(void);

void sys_ll_set_cpu_device_clk_enable_spi0_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_spi0_cken(void);

void sys_ll_set_cpu_device_clk_enable_uart0_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_uart0_cken(void);

void sys_ll_set_cpu_device_clk_enable_pwm0_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_pwm0_cken(void);

void sys_ll_set_cpu_device_clk_enable_tim0_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_tim0_cken(void);

void sys_ll_set_cpu_device_clk_enable_sadc_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_sadc_cken(void);

void sys_ll_set_cpu_device_clk_enable_irda_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_irda_cken(void);

void sys_ll_set_cpu_device_clk_enable_efuse_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_efuse_cken(void);

void sys_ll_set_cpu_device_clk_enable_i2c1_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_i2c1_cken(void);

void sys_ll_set_cpu_device_clk_enable_spi1_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_spi1_cken(void);

void sys_ll_set_cpu_device_clk_enable_uart1_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_uart1_cken(void);

void sys_ll_set_cpu_device_clk_enable_uart2_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_uart2_cken(void);

void sys_ll_set_cpu_device_clk_enable_pwm1_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_pwm1_cken(void);

void sys_ll_set_cpu_device_clk_enable_tim1_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_tim1_cken(void);

void sys_ll_set_cpu_device_clk_enable_tim2_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_tim2_cken(void);

void sys_ll_set_cpu_device_clk_enable_otp_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_otp_cken(void);

void sys_ll_set_cpu_device_clk_enable_i2s_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_i2s_cken(void);

void sys_ll_set_cpu_device_clk_enable_usb_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_usb_cken(void);

void sys_ll_set_cpu_device_clk_enable_can_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_can_cken(void);

void sys_ll_set_cpu_device_clk_enable_psram_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_psram_cken(void);

void sys_ll_set_cpu_device_clk_enable_qspi0_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_qspi0_cken(void);

void sys_ll_set_cpu_device_clk_enable_qspi1_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_qspi1_cken(void);

void sys_ll_set_cpu_device_clk_enable_sdio_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_sdio_cken(void);

void sys_ll_set_cpu_device_clk_enable_auxs_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_auxs_cken(void);

void sys_ll_set_cpu_device_clk_enable_btdm_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_btdm_cken(void);

void sys_ll_set_cpu_device_clk_enable_xvr_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_xvr_cken(void);

void sys_ll_set_cpu_device_clk_enable_mac_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_mac_cken(void);

void sys_ll_set_cpu_device_clk_enable_phy_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_phy_cken(void);

void sys_ll_set_cpu_device_clk_enable_jpeg_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_jpeg_cken(void);

void sys_ll_set_cpu_device_clk_enable_disp_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_disp_cken(void);

void sys_ll_set_cpu_device_clk_enable_aud_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_aud_cken(void);

void sys_ll_set_cpu_device_clk_enable_wdt_cken(uint32_t v);

uint32_t sys_ll_get_cpu_device_clk_enable_wdt_cken(void);

//reg reserver_reg0xd:

void sys_ll_set_reserver_reg0xd_value(uint32_t v);

uint32_t sys_ll_get_reserver_reg0xd_value(void);

void sys_ll_set_reserver_reg0xd_h264_cken(uint32_t v);

uint32_t sys_ll_get_reserver_reg0xd_h264_cken(void);

void sys_ll_set_reserver_reg0xd_i2s1_cken(uint32_t v);

uint32_t sys_ll_get_reserver_reg0xd_i2s1_cken(void);

void sys_ll_set_reserver_reg0xd_i2s2_cken(uint32_t v);

uint32_t sys_ll_get_reserver_reg0xd_i2s2_cken(void);

void sys_ll_set_reserver_reg0xd_yuv_cken(uint32_t v);

uint32_t sys_ll_get_reserver_reg0xd_yuv_cken(void);

void sys_ll_set_reserver_reg0xd_slcd_cken(uint32_t v);

uint32_t sys_ll_get_reserver_reg0xd_slcd_cken(void);

void sys_ll_set_reserver_reg0xd_reserved_5_31(uint32_t v);

uint32_t sys_ll_get_reserver_reg0xd_reserved_5_31(void);

//reg cpu_device_mem_ctrl1:

void sys_ll_set_cpu_device_mem_ctrl1_value(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_value(void);

void sys_ll_set_cpu_device_mem_ctrl1_uart1_mem_sd(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_uart1_mem_sd(void);

void sys_ll_set_cpu_device_mem_ctrl1_uart2(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_uart2(void);

void sys_ll_set_cpu_device_mem_ctrl1_spi1(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_spi1(void);

void sys_ll_set_cpu_device_mem_ctrl1_sdio(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_sdio(void);

void sys_ll_set_cpu_device_mem_ctrl1_usb(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_usb(void);

void sys_ll_set_cpu_device_mem_ctrl1_can(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_can(void);

void sys_ll_set_cpu_device_mem_ctrl1_qspi0(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_qspi0(void);

void sys_ll_set_cpu_device_mem_ctrl1_qspi1(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_qspi1(void);

void sys_ll_set_cpu_device_mem_ctrl1_pram(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_pram(void);

void sys_ll_set_cpu_device_mem_ctrl1_fft(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_fft(void);

void sys_ll_set_cpu_device_mem_ctrl1_abc(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_abc(void);

void sys_ll_set_cpu_device_mem_ctrl1_aud(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_aud(void);

void sys_ll_set_cpu_device_mem_ctrl1_i2s0(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_i2s0(void);

void sys_ll_set_cpu_device_mem_ctrl1_i2s1(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_i2s1(void);

void sys_ll_set_cpu_device_mem_ctrl1_i2s2(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_i2s2(void);

void sys_ll_set_cpu_device_mem_ctrl1_jpge(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_jpge(void);

void sys_ll_set_cpu_device_mem_ctrl1_yuv(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_yuv(void);

void sys_ll_set_cpu_device_mem_ctrl1_jpgd(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_jpgd(void);

void sys_ll_set_cpu_device_mem_ctrl1_disp(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_disp(void);

void sys_ll_set_cpu_device_mem_ctrl1_dmad(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_dmad(void);

void sys_ll_set_cpu_device_mem_ctrl1_h26f(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_h26f(void);

void sys_ll_set_cpu_device_mem_ctrl1_mac(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_mac(void);

void sys_ll_set_cpu_device_mem_ctrl1_phy(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_phy(void);

void sys_ll_set_cpu_device_mem_ctrl1_xvr(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_xvr(void);

void sys_ll_set_cpu_device_mem_ctrl1_irda(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_irda(void);

void sys_ll_set_cpu_device_mem_ctrl1_la(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_la(void);

void sys_ll_set_cpu_device_mem_ctrl1_flsh(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_flsh(void);

void sys_ll_set_cpu_device_mem_ctrl1_uart(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_uart(void);

void sys_ll_set_cpu_device_mem_ctrl1_spi0(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_spi0(void);

void sys_ll_set_cpu_device_mem_ctrl1_enc(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_enc(void);

void sys_ll_set_cpu_device_mem_ctrl1_dma0(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_dma0(void);

void sys_ll_set_cpu_device_mem_ctrl1_dma1(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl1_dma1(void);

//reg cpu_device_mem_ctrl2:

void sys_ll_set_cpu_device_mem_ctrl2_value(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_value(void);

void sys_ll_set_cpu_device_mem_ctrl2_uart1_mem_ds(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_uart1_mem_ds(void);

void sys_ll_set_cpu_device_mem_ctrl2_uart2(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_uart2(void);

void sys_ll_set_cpu_device_mem_ctrl2_spi1(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_spi1(void);

void sys_ll_set_cpu_device_mem_ctrl2_sdio(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_sdio(void);

void sys_ll_set_cpu_device_mem_ctrl2_usb(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_usb(void);

void sys_ll_set_cpu_device_mem_ctrl2_can(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_can(void);

void sys_ll_set_cpu_device_mem_ctrl2_qspi0(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_qspi0(void);

void sys_ll_set_cpu_device_mem_ctrl2_qspi1(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_qspi1(void);

void sys_ll_set_cpu_device_mem_ctrl2_pram(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_pram(void);

void sys_ll_set_cpu_device_mem_ctrl2_fft(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_fft(void);

void sys_ll_set_cpu_device_mem_ctrl2_abc(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_abc(void);

void sys_ll_set_cpu_device_mem_ctrl2_aud(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_aud(void);

void sys_ll_set_cpu_device_mem_ctrl2_i2s0(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_i2s0(void);

void sys_ll_set_cpu_device_mem_ctrl2_i2s1(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_i2s1(void);

void sys_ll_set_cpu_device_mem_ctrl2_i2s2(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_i2s2(void);

void sys_ll_set_cpu_device_mem_ctrl2_jpge(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_jpge(void);

void sys_ll_set_cpu_device_mem_ctrl2_yuv(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_yuv(void);

void sys_ll_set_cpu_device_mem_ctrl2_jpgd(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_jpgd(void);

void sys_ll_set_cpu_device_mem_ctrl2_disp(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_disp(void);

void sys_ll_set_cpu_device_mem_ctrl2_dmad(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_dmad(void);

void sys_ll_set_cpu_device_mem_ctrl2_h26f(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_h26f(void);

void sys_ll_set_cpu_device_mem_ctrl2_mac(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_mac(void);

void sys_ll_set_cpu_device_mem_ctrl2_phy(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_phy(void);

void sys_ll_set_cpu_device_mem_ctrl2_xvr(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_xvr(void);

void sys_ll_set_cpu_device_mem_ctrl2_irda(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_irda(void);

void sys_ll_set_cpu_device_mem_ctrl2_la(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_la(void);

void sys_ll_set_cpu_device_mem_ctrl2_flsh(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_flsh(void);

void sys_ll_set_cpu_device_mem_ctrl2_uart(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_uart(void);

void sys_ll_set_cpu_device_mem_ctrl2_spi0(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_spi0(void);

void sys_ll_set_cpu_device_mem_ctrl2_enc(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_enc(void);

void sys_ll_set_cpu_device_mem_ctrl2_dma0(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_dma0(void);

void sys_ll_set_cpu_device_mem_ctrl2_dma1(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl2_dma1(void);

//reg cpu_power_sleep_wakeup:

void sys_ll_set_cpu_power_sleep_wakeup_value(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_value(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem1(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem1(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem2(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem2(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem3(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem3(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_encp(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_encp(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_bakp(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_bakp(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_ahbp(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_ahbp(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_audp(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_audp(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_vidp(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_vidp(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_btsp(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_btsp(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_mac(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_mac(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_phy(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_phy(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem0(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem0(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem4(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem4(void);

void sys_ll_set_cpu_power_sleep_wakeup_pwd_ofdm(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_ofdm(void);

void sys_ll_set_cpu_power_sleep_wakeup_tcm0_pgen(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_tcm0_pgen(void);

void sys_ll_set_cpu_power_sleep_wakeup_rom_pgen(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_rom_pgen(void);

void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(void);

void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(void);

void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(void);

void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_global(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_global(void);

void sys_ll_set_cpu_power_sleep_wakeup_sleep_bus_idle_bypass(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_bus_idle_bypass(void);

void sys_ll_set_cpu_power_sleep_wakeup_bts_wakeup_platform_en(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_bts_wakeup_platform_en(void);

void sys_ll_set_cpu_power_sleep_wakeup_bts_soft_wakeup_req(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_bts_soft_wakeup_req(void);

void sys_ll_set_cpu_power_sleep_wakeup_rom_rd_disable(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_rom_rd_disable(void);

void sys_ll_set_cpu_power_sleep_wakeup_otp_rd_disable(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_otp_rd_disable(void);

void sys_ll_set_cpu_power_sleep_wakeup_tcm1_pgen(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_tcm1_pgen(void);

void sys_ll_set_cpu_power_sleep_wakeup_cpu0_subpwdm_en(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu0_subpwdm_en(void);

void sys_ll_set_cpu_power_sleep_wakeup_cpu1_subpwdm_en(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu1_subpwdm_en(void);

void sys_ll_set_cpu_power_sleep_wakeup_share_mem_clkgating_disable(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_share_mem_clkgating_disable(void);

void sys_ll_set_cpu_power_sleep_wakeup_cpu0_ticktimer_32k_enable(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu0_ticktimer_32k_enable(void);

void sys_ll_set_cpu_power_sleep_wakeup_cpu1_ticktimer_32k_enable(uint32_t v);

uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu1_ticktimer_32k_enable(void);

//reg reserver_reg0x11:

void sys_ll_set_reserver_reg0x11_value(uint32_t v);

uint32_t sys_ll_get_reserver_reg0x11_value(void);

void sys_ll_set_reserver_reg0x11_reserved_0_31(uint32_t v);

uint32_t sys_ll_get_reserver_reg0x11_reserved_0_31(void);

//reg cpu_device_mem_ctrl3:

void sys_ll_set_cpu_device_mem_ctrl3_value(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_value(void);

void sys_ll_set_cpu_device_mem_ctrl3_ram0_mem_ds_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram0_mem_ds_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_ram1_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram1_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_ram2_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram2_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_ram3_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram3_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_ram4_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram4_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu0_icache_itag_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_icache_itag_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu0_dcache_dtag_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_dcache_dtag_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu1_icache_itag_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_icache_itag_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu1_dcache_dtag_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_dcache_dtag_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu0_itcm_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_itcm_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu0_dtcm_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_dtcm_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu1_itcm_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_itcm_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu1_dtcm_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_dtcm_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_rott_deep_sleep(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_rott_deep_sleep(void);

void sys_ll_set_cpu_device_mem_ctrl3_reserved0(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved0(void);

void sys_ll_set_cpu_device_mem_ctrl3_reserved1(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved1(void);

void sys_ll_set_cpu_device_mem_ctrl3_ram0_mem_sd_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram0_mem_sd_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_ram1_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram1_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_ram2_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram2_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_ram3_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram3_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_ram4_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram4_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu0_icache_itag_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_icache_itag_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu0_dcache_dtag_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_dcache_dtag_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu1_icache_itag_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_icache_itag_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu1_dcache_dtag_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_dcache_dtag_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu0_itcm_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_itcm_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu0_dtcm_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_dtcm_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu1_itcm_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_itcm_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_cpu1_dtcm_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_dtcm_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_rott_shutdown(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_rott_shutdown(void);

void sys_ll_set_cpu_device_mem_ctrl3_reserved2(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved2(void);

void sys_ll_set_cpu_device_mem_ctrl3_reserved3(uint32_t v);

uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved3(void);

//reg cpu0_int_0_31_en:

void sys_ll_set_cpu0_int_0_31_en_value(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_value(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_dma0_nsec_intr_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_dma0_nsec_intr_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_encp_sec_intr_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_encp_sec_intr_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_encp_nsec_intr_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_encp_nsec_intr_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_timer0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_timer0_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_uart_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_pwm0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_pwm0_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_i2c0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2c0_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_spi0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_spi0_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_sadc_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sadc_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_irda_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_irda_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_sdio_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sdio_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_gdma_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_gdma_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_la_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_la_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_timer1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_timer1_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_i2c1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2c1_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_uart1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart1_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_uart2_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart2_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_spi1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_spi1_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_can_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_can_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_usb_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_usb_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_qspi0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_qspi0_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_fft_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_fft_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_sbc_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sbc_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_aud_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_aud_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_i2s0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2s0_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_jpegenc_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_jpegenc_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_jpegdec_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_jpegdec_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_lcd_display_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_lcd_display_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_dma2d_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_dma2d_int_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_mpb_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_int_phy_mpb_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_riu_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_int_phy_riu_en(void);

void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(void);

//reg cpu0_int_32_63_en:

void sys_ll_set_cpu0_int_32_63_en_value(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_value(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_rx_misc_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_rx_misc_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_prot_trigger_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_prot_trigger_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_dm_irq_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dm_irq_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_ble_irq_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_ble_irq_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_bt_irq_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_bt_irq_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_qspi1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_qspi1_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_pwm1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_pwm1_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_i2s1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_i2s1_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_i2s2_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_i2s2_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_h264_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_h264_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_sdmadc_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_sdmadc_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_mbox0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mbox0_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_mbox1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mbox1_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_bmc64_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_bmc64_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_dpll_unlock_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dpll_unlock_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_touched_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_touched_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_usbplug_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_usbplug_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_rtc_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_rtc_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_gpio_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_gpio_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_dma1_sec_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dma1_sec_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_dma1_nsec_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dma1_nsec_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_yuvb_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_yuvb_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_cpu0_rott_int_en(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_rott_int_en(void);

void sys_ll_set_cpu0_int_32_63_en_reserved0(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_en_reserved0(void);

//reg cpu1_int_0_31_en:

void sys_ll_set_cpu1_int_0_31_en_value(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_value(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_dma0_nsec_intr_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_dma0_nsec_intr_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_encp_sec_intr_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_encp_sec_intr_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_encp_nsec_intr_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_encp_nsec_intr_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_timer0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_timer0_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_uart_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_pwm0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_pwm0_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_i2c0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2c0_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_spi0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_spi0_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_sadc_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sadc_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_irda_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_irda_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_sdio_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sdio_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_gdma_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_gdma_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_la_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_la_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_timer1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_timer1_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_i2c1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2c1_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_uart1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart1_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_uart2_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart2_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_spi1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_spi1_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_can_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_can_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_usb_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_usb_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_qspi0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_qspi0_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_fft_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_fft_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_sbc_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sbc_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_aud_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_aud_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_i2s0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2s0_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_jpegenc_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_jpegenc_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_jpegdec_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_jpegdec_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_lcd_display_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_lcd_display_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_dma2d_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_dma2d_int_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_int_phy_mpb_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_int_phy_mpb_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_int_phy_riu_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_int_phy_riu_en(void);

void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(void);

//reg cpu1_int_32_63_en:

void sys_ll_set_cpu1_int_32_63_en_value(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_value(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_rx_misc_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_rx_misc_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_prot_trigger_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_prot_trigger_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_dm_irq_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dm_irq_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_ble_irq_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_ble_irq_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_bt_irq_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_bt_irq_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_qspi1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_qspi1_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_pwm1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_pwm1_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_i2s1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_i2s1_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_i2s2_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_i2s2_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_h264_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_h264_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_sdmadc_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_sdmadc_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_mbox0_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_mbox0_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_mbox1_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_mbox1_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_bmc64_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_bmc64_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_dpll_unlock_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dpll_unlock_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_touched_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_touched_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_usbplug_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_usbplug_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_rtc_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_rtc_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_gpio_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_gpio_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_dma1_sec_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dma1_sec_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_dma1_nsec_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dma1_nsec_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_yuvb_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_yuvb_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_cpu1_rott_int_en(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_rott_int_en(void);

void sys_ll_set_cpu1_int_32_63_en_reserved0(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_en_reserved0(void);

//reg cpu0_int_0_31_status:

void sys_ll_set_cpu0_int_0_31_status_value(uint32_t v);

uint32_t sys_ll_get_cpu0_int_0_31_status_value(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_dma0_nsec_intr_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_encp_sec_intr_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_encp_nsec_intr_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_timer0_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_pwm0_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2c0_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_spi0_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sadc_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_irda_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sdio_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_gdma_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_la_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_timer1_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2c1_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart1_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart2_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_spi1_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_can_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_usb_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_qspi0_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_fft_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sbc_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_aud_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2s0_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_jpegenc_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_jpegdec_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_lcd_display_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_dma2d_int_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_int_phy_mpb_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_int_phy_riu_st(void);

uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_mac_int_tx_rx_timer_st(void);

//reg cpu0_int_32_63_status:

void sys_ll_set_cpu0_int_32_63_status_value(uint32_t v);

uint32_t sys_ll_get_cpu0_int_32_63_status_value(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_tx_rx_misc_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_rx_trigger_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_tx_trigger_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_prot_trigger_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_gen_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_hsu_irq_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_int_mac_wakeup_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dm_irq_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_ble_irq_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_bt_irq_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_qspi1_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_pwm1_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_i2s1_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_i2s2_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_h264_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_sdmadc_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mbox0_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mbox1_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_bmc64_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dpll_unlock_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_touched_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_usbplug_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_rtc_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_gpio_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dma1_sec_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dma1_nsec_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_yuvb_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_rott_int_st(void);

uint32_t sys_ll_get_cpu0_int_32_63_status_reserved0(void);

//reg cpu1_int_0_31_status:

void sys_ll_set_cpu1_int_0_31_status_value(uint32_t v);

uint32_t sys_ll_get_cpu1_int_0_31_status_value(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_dma0_nsec_intr_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_encp_sec_intr_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_encp_nsec_intr_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_timer0_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_pwm0_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2c0_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_spi0_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sadc_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_irda_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sdio_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_gdma_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_la_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_timer1_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2c1_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart1_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart2_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_spi1_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_can_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_usb_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_qspi0_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_fft_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sbc_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_aud_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2s0_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_jpegenc_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_jpegdec_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_lcd_display_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_dma2d_int_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_int_phy_mpb_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_int_phy_riu_st(void);

uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_mac_int_tx_rx_timer_st(void);

//reg cpu1_int_32_63_status:

void sys_ll_set_cpu1_int_32_63_status_value(uint32_t v);

uint32_t sys_ll_get_cpu1_int_32_63_status_value(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_tx_rx_misc_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_rx_trigger_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_tx_trigger_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_prot_trigger_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_gen_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_hsu_irq_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_int_mac_wakeup_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dm_irq_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_ble_irq_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_bt_irq_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_qspi1_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_pwm1_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_i2s1_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_i2s2_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_h264_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_sdmadc_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_mbox0_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_mbox1_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_bmc64_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dpll_unlock_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_touched_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_usbplug_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_rtc_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_gpio_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dma1_sec_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dma1_nsec_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_yuvb_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_rott_int_st(void);

uint32_t sys_ll_get_cpu1_int_32_63_status_reserved0(void);

//reg gpio_config0:

void sys_ll_set_gpio_config0_value(uint32_t v);

uint32_t sys_ll_get_gpio_config0_value(void);

void sys_ll_set_gpio_config0_gpio_config0(uint32_t v);

uint32_t sys_ll_get_gpio_config0_gpio_config0(void);

//reg gpio_config1:

void sys_ll_set_gpio_config1_value(uint32_t v);

uint32_t sys_ll_get_gpio_config1_value(void);

void sys_ll_set_gpio_config1_gpio_config1(uint32_t v);

uint32_t sys_ll_get_gpio_config1_gpio_config1(void);

//reg gpio_config2:

void sys_ll_set_gpio_config2_value(uint32_t v);

uint32_t sys_ll_get_gpio_config2_value(void);

void sys_ll_set_gpio_config2_gpio_config2(uint32_t v);

uint32_t sys_ll_get_gpio_config2_gpio_config2(void);

//reg gpio_config3:

void sys_ll_set_gpio_config3_value(uint32_t v);

uint32_t sys_ll_get_gpio_config3_value(void);

void sys_ll_set_gpio_config3_gpio_config3(uint32_t v);

uint32_t sys_ll_get_gpio_config3_gpio_config3(void);

//reg gpio_config4:

void sys_ll_set_gpio_config4_value(uint32_t v);

uint32_t sys_ll_get_gpio_config4_value(void);

void sys_ll_set_gpio_config4_gpio_config4(uint32_t v);

uint32_t sys_ll_get_gpio_config4_gpio_config4(void);

//reg gpio_config5:

void sys_ll_set_gpio_config5_value(uint32_t v);

uint32_t sys_ll_get_gpio_config5_value(void);

void sys_ll_set_gpio_config5_gpio_config5(uint32_t v);

uint32_t sys_ll_get_gpio_config5_gpio_config5(void);

//reg sys_debug_config0:

void sys_ll_set_sys_debug_config0_value(uint32_t v);

uint32_t sys_ll_get_sys_debug_config0_value(void);

void sys_ll_set_sys_debug_config0_dbug_config0(uint32_t v);

uint32_t sys_ll_get_sys_debug_config0_dbug_config0(void);

//reg sys_debug_config1:

void sys_ll_set_sys_debug_config1_value(uint32_t v);

uint32_t sys_ll_get_sys_debug_config1_value(void);

void sys_ll_set_sys_debug_config1_dbug_config1(uint32_t v);

uint32_t sys_ll_get_sys_debug_config1_dbug_config1(void);

//reg ana_reg0:

void sys_ll_set_ana_reg0_value(uint32_t v);

uint32_t sys_ll_get_ana_reg0_value(void);

void sys_ll_set_ana_reg0_dpll_tsten(uint32_t v);

uint32_t sys_ll_get_ana_reg0_dpll_tsten(void);

void sys_ll_set_ana_reg0_cp(uint32_t v);

uint32_t sys_ll_get_ana_reg0_cp(void);

void sys_ll_set_ana_reg0_spideten(uint32_t v);

uint32_t sys_ll_get_ana_reg0_spideten(void);

void sys_ll_set_ana_reg0_hvref(uint32_t v);

uint32_t sys_ll_get_ana_reg0_hvref(void);

void sys_ll_set_ana_reg0_lvref(uint32_t v);

uint32_t sys_ll_get_ana_reg0_lvref(void);

void sys_ll_set_ana_reg0_rzctrl26m(uint32_t v);

uint32_t sys_ll_get_ana_reg0_rzctrl26m(void);

void sys_ll_set_ana_reg0_looprzctrl(uint32_t v);

uint32_t sys_ll_get_ana_reg0_looprzctrl(void);

void sys_ll_set_ana_reg0_rpc(uint32_t v);

uint32_t sys_ll_get_ana_reg0_rpc(void);

void sys_ll_set_ana_reg0_openloop_en(uint32_t v);

uint32_t sys_ll_get_ana_reg0_openloop_en(void);

void sys_ll_set_ana_reg0_cksel(uint32_t v);

uint32_t sys_ll_get_ana_reg0_cksel(void);

void sys_ll_set_ana_reg0_spitrig(uint32_t v);

uint32_t sys_ll_get_ana_reg0_spitrig(void);

void sys_ll_set_ana_reg0_band0(uint32_t v);

uint32_t sys_ll_get_ana_reg0_band0(void);

void sys_ll_set_ana_reg0_band1(uint32_t v);

uint32_t sys_ll_get_ana_reg0_band1(void);

void sys_ll_set_ana_reg0_band(uint32_t v);

uint32_t sys_ll_get_ana_reg0_band(void);

void sys_ll_set_ana_reg0_bandmanual(uint32_t v);

uint32_t sys_ll_get_ana_reg0_bandmanual(void);

void sys_ll_set_ana_reg0_dsptrig(uint32_t v);

uint32_t sys_ll_get_ana_reg0_dsptrig(void);

void sys_ll_set_ana_reg0_lpen_dpll(uint32_t v);

uint32_t sys_ll_get_ana_reg0_lpen_dpll(void);

void sys_ll_set_ana_reg0_nc_28_30(uint32_t v);

uint32_t sys_ll_get_ana_reg0_nc_28_30(void);

void sys_ll_set_ana_reg0_vctrl_dpllldo(uint32_t v);

uint32_t sys_ll_get_ana_reg0_vctrl_dpllldo(void);

//reg ana_reg1:

void sys_ll_set_ana_reg1_value(uint32_t v);

uint32_t sys_ll_get_ana_reg1_value(void);

void sys_ll_set_ana_reg1_nc3(uint32_t v);

void sys_ll_set_ana_reg1_nc2(uint32_t v);

void sys_ll_set_ana_reg1_msw(uint32_t v);

void sys_ll_set_ana_reg1_ictrl(uint32_t v);

void sys_ll_set_ana_reg1_osc_trig(uint32_t v);

void sys_ll_set_ana_reg1_osccal_trig(uint32_t v);

void sys_ll_set_ana_reg1_cnti(uint32_t v);

void sys_ll_set_ana_reg1_spi_rst(uint32_t v);

void sys_ll_set_ana_reg1_amsel(uint32_t v);

void sys_ll_set_ana_reg1_divctrl(uint32_t v);

void sys_ll_set_ana_reg1_nc1(uint32_t v);

void sys_ll_set_ana_reg1_nc0(uint32_t v);

//reg ana_reg2:

void sys_ll_set_ana_reg2_value(uint32_t v);

uint32_t sys_ll_get_ana_reg2_value(void);

void sys_ll_set_ana_reg2_xtalh_ctune(uint32_t v);

uint32_t sys_ll_get_ana_reg2_xtalh_ctune(void);

void sys_ll_set_ana_reg2_force_26mpll(uint32_t v);

uint32_t sys_ll_get_ana_reg2_force_26mpll(void);

void sys_ll_set_ana_reg2_gadc_cmp_ictrl(uint32_t v);

uint32_t sys_ll_get_ana_reg2_gadc_cmp_ictrl(void);

void sys_ll_set_ana_reg2_gadc_inbuf_ictrl(uint32_t v);

uint32_t sys_ll_get_ana_reg2_gadc_inbuf_ictrl(void);

void sys_ll_set_ana_reg2_gadc_refbuf_ictrl(uint32_t v);

uint32_t sys_ll_get_ana_reg2_gadc_refbuf_ictrl(void);

void sys_ll_set_ana_reg2_gadc_nobuf_enable(uint32_t v);

uint32_t sys_ll_get_ana_reg2_gadc_nobuf_enable(void);

void sys_ll_set_ana_reg2_vref_scale(uint32_t v);

uint32_t sys_ll_get_ana_reg2_vref_scale(void);

void sys_ll_set_ana_reg2_scal_en(uint32_t v);

uint32_t sys_ll_get_ana_reg2_scal_en(void);

void sys_ll_set_ana_reg2_gadc_capcal_en(uint32_t v);

uint32_t sys_ll_get_ana_reg2_gadc_capcal_en(void);

void sys_ll_set_ana_reg2_gadc_capcal(uint32_t v);

uint32_t sys_ll_get_ana_reg2_gadc_capcal(void);

void sys_ll_set_ana_reg2_sp_nt_ctrl(uint32_t v);

uint32_t sys_ll_get_ana_reg2_sp_nt_ctrl(void);

//reg ana_reg3:

void sys_ll_set_ana_reg3_value(uint32_t v);

uint32_t sys_ll_get_ana_reg3_value(void);

void sys_ll_set_ana_reg3_usbpen(uint32_t v);

void sys_ll_set_ana_reg3_usbnen(uint32_t v);

void sys_ll_set_ana_reg3_hpssren(uint32_t v);

uint32_t sys_ll_get_ana_reg3_hpssren(void);

void sys_ll_set_ana_reg3_ck_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg3_ck_sel(void);

void sys_ll_set_ana_reg3_anabuf_sel_rx(uint32_t v);

uint32_t sys_ll_get_ana_reg3_anabuf_sel_rx(void);

void sys_ll_set_ana_reg3_pwd_xtalldo(uint32_t v);

uint32_t sys_ll_get_ana_reg3_pwd_xtalldo(void);

void sys_ll_set_ana_reg3_iamp(uint32_t v);

uint32_t sys_ll_get_ana_reg3_iamp(void);

void sys_ll_set_ana_reg3_vddren(uint32_t v);

uint32_t sys_ll_get_ana_reg3_vddren(void);

void sys_ll_set_ana_reg3_xamp(uint32_t v);

uint32_t sys_ll_get_ana_reg3_xamp(void);

void sys_ll_set_ana_reg3_vosel(uint32_t v);

uint32_t sys_ll_get_ana_reg3_vosel(void);

void sys_ll_set_ana_reg3_en_xtalh_sleep(uint32_t v);

uint32_t sys_ll_get_ana_reg3_en_xtalh_sleep(void);

void sys_ll_set_ana_reg3_xtal40_en(uint32_t v);

uint32_t sys_ll_get_ana_reg3_xtal40_en(void);

void sys_ll_set_ana_reg3_bufictrl(uint32_t v);

uint32_t sys_ll_get_ana_reg3_bufictrl(void);

void sys_ll_set_ana_reg3_ibias_ctrl(uint32_t v);

uint32_t sys_ll_get_ana_reg3_ibias_ctrl(void);

void sys_ll_set_ana_reg3_icore_ctrl(uint32_t v);

uint32_t sys_ll_get_ana_reg3_icore_ctrl(void);

//reg ana_reg4:

void sys_ll_set_ana_reg4_value(uint32_t v);

uint32_t sys_ll_get_ana_reg4_value(void);

void sys_ll_set_ana_reg4_anabuf_sel_tx(uint32_t v);

uint32_t sys_ll_get_ana_reg4_anabuf_sel_tx(void);

void sys_ll_set_ana_reg4_trng_tsten(uint32_t v);

uint32_t sys_ll_get_ana_reg4_trng_tsten(void);

void sys_ll_set_ana_reg4_itune_ref(uint32_t v);

uint32_t sys_ll_get_ana_reg4_itune_ref(void);

void sys_ll_set_ana_reg4_itune_opa(uint32_t v);

uint32_t sys_ll_get_ana_reg4_itune_opa(void);

void sys_ll_set_ana_reg4_itune_cmp(uint32_t v);

uint32_t sys_ll_get_ana_reg4_itune_cmp(void);

void sys_ll_set_ana_reg4_rnooise_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg4_rnooise_sel(void);

void sys_ll_set_ana_reg4_fslow_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg4_fslow_sel(void);

void sys_ll_set_ana_reg4_ffast_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg4_ffast_sel(void);

void sys_ll_set_ana_reg4_trng_tstck_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg4_trng_tstck_sel(void);

void sys_ll_set_ana_reg4_cktst_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg4_cktst_sel(void);

void sys_ll_set_ana_reg4_ck_tst_enbale(uint32_t v);

uint32_t sys_ll_get_ana_reg4_ck_tst_enbale(void);

void sys_ll_set_ana_reg4_sw_bias(uint32_t v);

uint32_t sys_ll_get_ana_reg4_sw_bias(void);

void sys_ll_set_ana_reg4_crb(uint32_t v);

uint32_t sys_ll_get_ana_reg4_crb(void);

void sys_ll_set_ana_reg4_port_enablel(uint32_t v);

uint32_t sys_ll_get_ana_reg4_port_enablel(void);

//reg ana_reg5:

void sys_ll_set_ana_reg5_value(uint32_t v);

uint32_t sys_ll_get_ana_reg5_value(void);

void sys_ll_set_ana_reg5_en_usb(uint32_t v);

uint32_t sys_ll_get_ana_reg5_en_usb(void);

void sys_ll_set_ana_reg5_en_xtall(uint32_t v);

uint32_t sys_ll_get_ana_reg5_en_xtall(void);

void sys_ll_set_ana_reg5_en_dco(uint32_t v);

uint32_t sys_ll_get_ana_reg5_en_dco(void);

void sys_ll_set_ana_reg5_en_ram(uint32_t v);

uint32_t sys_ll_get_ana_reg5_en_ram(void);

void sys_ll_set_ana_reg5_en_temp(uint32_t v);

uint32_t sys_ll_get_ana_reg5_en_temp(void);

void sys_ll_set_ana_reg5_en_dpll(uint32_t v);

uint32_t sys_ll_get_ana_reg5_en_dpll(void);

void sys_ll_set_ana_reg5_en_cb(uint32_t v);

uint32_t sys_ll_get_ana_reg5_en_cb(void);

void sys_ll_set_ana_reg5_en_lcd(uint32_t v);

uint32_t sys_ll_get_ana_reg5_en_lcd(void);

void sys_ll_set_ana_reg5_trxspi_ctrl(uint32_t v);

uint32_t sys_ll_get_ana_reg5_trxspi_ctrl(void);

void sys_ll_set_ana_reg5_adc_div(uint32_t v);

uint32_t sys_ll_get_ana_reg5_adc_div(void);

void sys_ll_set_ana_reg5_usb_speed(uint32_t v);

uint32_t sys_ll_get_ana_reg5_usb_speed(void);

void sys_ll_set_ana_reg5_spideepsleep(uint32_t v);

uint32_t sys_ll_get_ana_reg5_spideepsleep(void);

void sys_ll_set_ana_reg5_vsel(uint32_t v);

uint32_t sys_ll_get_ana_reg5_vsel(void);

void sys_ll_set_ana_reg5_swb(uint32_t v);

uint32_t sys_ll_get_ana_reg5_swb(void);

void sys_ll_set_ana_reg5_itune_xtall(uint32_t v);

uint32_t sys_ll_get_ana_reg5_itune_xtall(void);

void sys_ll_set_ana_reg5_xtall_ten(uint32_t v);

uint32_t sys_ll_get_ana_reg5_xtall_ten(void);

void sys_ll_set_ana_reg5_rosc_tsten(uint32_t v);

uint32_t sys_ll_get_ana_reg5_rosc_tsten(void);

void sys_ll_set_ana_reg5_bcal_start(uint32_t v);

uint32_t sys_ll_get_ana_reg5_bcal_start(void);

void sys_ll_set_ana_reg5_bcal_en(uint32_t v);

uint32_t sys_ll_get_ana_reg5_bcal_en(void);

void sys_ll_set_ana_reg5_bcal_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg5_bcal_sel(void);

void sys_ll_set_ana_reg5_vbias(uint32_t v);

uint32_t sys_ll_get_ana_reg5_vbias(void);

//reg ana_reg6:

void sys_ll_set_ana_reg6_value(uint32_t v);

uint32_t sys_ll_get_ana_reg6_value(void);

void sys_ll_set_ana_reg6_calib_interval(uint32_t v);

uint32_t sys_ll_get_ana_reg6_calib_interval(void);

void sys_ll_set_ana_reg6_modify_interval(uint32_t v);

uint32_t sys_ll_get_ana_reg6_modify_interval(void);

void sys_ll_set_ana_reg6_xtal_wakeup_time(uint32_t v);

uint32_t sys_ll_get_ana_reg6_xtal_wakeup_time(void);

void sys_ll_set_ana_reg6_spi_trig(uint32_t v);

uint32_t sys_ll_get_ana_reg6_spi_trig(void);

void sys_ll_set_ana_reg6_modifi_auto(uint32_t v);

uint32_t sys_ll_get_ana_reg6_modifi_auto(void);

void sys_ll_set_ana_reg6_calib_auto(uint32_t v);

uint32_t sys_ll_get_ana_reg6_calib_auto(void);

void sys_ll_set_ana_reg6_cal_mode(uint32_t v);

uint32_t sys_ll_get_ana_reg6_cal_mode(void);

void sys_ll_set_ana_reg6_manu_ena(uint32_t v);

uint32_t sys_ll_get_ana_reg6_manu_ena(void);

void sys_ll_set_ana_reg6_manu_cin(uint32_t v);

uint32_t sys_ll_get_ana_reg6_manu_cin(void);

//reg ana_reg7:

void sys_ll_set_ana_reg7_value(uint32_t v);

uint32_t sys_ll_get_ana_reg7_value(void);

void sys_ll_set_ana_reg7_port_enablel(uint32_t v);

uint32_t sys_ll_get_ana_reg7_port_enablel(void);

//reg ana_reg8:

void sys_ll_set_ana_reg8_value(uint32_t v);

uint32_t sys_ll_get_ana_reg8_value(void);

void sys_ll_set_ana_reg8_ioldo_lp(uint32_t v);

uint32_t sys_ll_get_ana_reg8_ioldo_lp(void);

void sys_ll_set_ana_reg8_coreldo_hp(uint32_t v);

uint32_t sys_ll_get_ana_reg8_coreldo_hp(void);

void sys_ll_set_ana_reg8_dldohp(uint32_t v);

uint32_t sys_ll_get_ana_reg8_dldohp(void);

void sys_ll_set_ana_reg8_t_vanaldosel(uint32_t v);

uint32_t sys_ll_get_ana_reg8_t_vanaldosel(void);

void sys_ll_set_ana_reg8_r_vanaldosel(uint32_t v);

uint32_t sys_ll_get_ana_reg8_r_vanaldosel(void);

void sys_ll_set_ana_reg8_en_trsw(uint32_t v);

uint32_t sys_ll_get_ana_reg8_en_trsw(void);

void sys_ll_set_ana_reg8_aldohp(uint32_t v);

uint32_t sys_ll_get_ana_reg8_aldohp(void);

void sys_ll_set_ana_reg8_anacurlim(uint32_t v);

uint32_t sys_ll_get_ana_reg8_anacurlim(void);

void sys_ll_set_ana_reg8_violdosel(uint32_t v);

uint32_t sys_ll_get_ana_reg8_violdosel(void);

void sys_ll_set_ana_reg8_iocurlim(uint32_t v);

uint32_t sys_ll_get_ana_reg8_iocurlim(void);

void sys_ll_set_ana_reg8_valoldosel(uint32_t v);

uint32_t sys_ll_get_ana_reg8_valoldosel(void);

void sys_ll_set_ana_reg8_alopowsel(uint32_t v);

uint32_t sys_ll_get_ana_reg8_alopowsel(void);

void sys_ll_set_ana_reg8_en_fast_aloldo(uint32_t v);

uint32_t sys_ll_get_ana_reg8_en_fast_aloldo(void);

void sys_ll_set_ana_reg8_aloldohp(uint32_t v);

uint32_t sys_ll_get_ana_reg8_aloldohp(void);

void sys_ll_set_ana_reg8_bgcal(uint32_t v);

uint32_t sys_ll_get_ana_reg8_bgcal(void);

void sys_ll_set_ana_reg8_vbgcalmode(uint32_t v);

uint32_t sys_ll_get_ana_reg8_vbgcalmode(void);

void sys_ll_set_ana_reg8_vbgcalstart(uint32_t v);

uint32_t sys_ll_get_ana_reg8_vbgcalstart(void);

void sys_ll_set_ana_reg8_pwd_bgcal(uint32_t v);

uint32_t sys_ll_get_ana_reg8_pwd_bgcal(void);

void sys_ll_set_ana_reg8_spi_envbg(uint32_t v);

uint32_t sys_ll_get_ana_reg8_spi_envbg(void);

//reg ana_reg9:

void sys_ll_set_ana_reg9_value(uint32_t v);

uint32_t sys_ll_get_ana_reg9_value(void);

void sys_ll_set_ana_reg9_wkgpiosel1(uint32_t v);

uint32_t sys_ll_get_ana_reg9_wkgpiosel1(void);

void sys_ll_set_ana_reg9_rst_wks1v(uint32_t v);

uint32_t sys_ll_get_ana_reg9_rst_wks1v(void);

void sys_ll_set_ana_reg9_wkgpiosel2(uint32_t v);

uint32_t sys_ll_get_ana_reg9_wkgpiosel2(void);

void sys_ll_set_ana_reg9_spi_latch1v(uint32_t v);

uint32_t sys_ll_get_ana_reg9_spi_latch1v(void);

void sys_ll_set_ana_reg9_digcurlim(uint32_t v);

uint32_t sys_ll_get_ana_reg9_digcurlim(void);

void sys_ll_set_ana_reg9_pupres_enb1v(uint32_t v);

uint32_t sys_ll_get_ana_reg9_pupres_enb1v(void);

void sys_ll_set_ana_reg9_pdnres_en1v(uint32_t v);

uint32_t sys_ll_get_ana_reg9_pdnres_en1v(void);

void sys_ll_set_ana_reg9_d_veasel1v(uint32_t v);

uint32_t sys_ll_get_ana_reg9_d_veasel1v(void);

void sys_ll_set_ana_reg9_ensfsdd(uint32_t v);

uint32_t sys_ll_get_ana_reg9_ensfsdd(void);

void sys_ll_set_ana_reg9_vcorehsel(uint32_t v);

uint32_t sys_ll_get_ana_reg9_vcorehsel(void);

void sys_ll_set_ana_reg9_vcorelsel(uint32_t v);

uint32_t sys_ll_get_ana_reg9_vcorelsel(void);

void sys_ll_set_ana_reg9_vlden(uint32_t v);

uint32_t sys_ll_get_ana_reg9_vlden(void);

void sys_ll_set_ana_reg9_en_fast_coreldo(uint32_t v);

uint32_t sys_ll_get_ana_reg9_en_fast_coreldo(void);

void sys_ll_set_ana_reg9_pwdcoreldo(uint32_t v);

uint32_t sys_ll_get_ana_reg9_pwdcoreldo(void);

void sys_ll_set_ana_reg9_vdighsel(uint32_t v);

uint32_t sys_ll_get_ana_reg9_vdighsel(void);

void sys_ll_set_ana_reg9_vdigsel(uint32_t v);

uint32_t sys_ll_get_ana_reg9_vdigsel(void);

void sys_ll_set_ana_reg9_vdd12lden(uint32_t v);

uint32_t sys_ll_get_ana_reg9_vdd12lden(void);

//reg ana_reg10:

void sys_ll_set_ana_reg10_value(uint32_t v);

uint32_t sys_ll_get_ana_reg10_value(void);

void sys_ll_set_ana_reg10_pasoft_st(uint32_t v);

uint32_t sys_ll_get_ana_reg10_pasoft_st(void);

void sys_ll_set_ana_reg10_azcdcnt_manu(uint32_t v);

uint32_t sys_ll_get_ana_reg10_azcdcnt_manu(void);

void sys_ll_set_ana_reg10_timer_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg10_timer_sel(void);

void sys_ll_set_ana_reg10_vpabucksel(uint32_t v);

uint32_t sys_ll_get_ana_reg10_vpabucksel(void);

void sys_ll_set_ana_reg10_spi_timerwken(uint32_t v);

uint32_t sys_ll_get_ana_reg10_spi_timerwken(void);

void sys_ll_set_ana_reg10_nc_16_16(uint32_t v);

uint32_t sys_ll_get_ana_reg10_nc_16_16(void);

void sys_ll_set_ana_reg10_sd(uint32_t v);

uint32_t sys_ll_get_ana_reg10_sd(void);

void sys_ll_set_ana_reg10_ioldosel(uint32_t v);

uint32_t sys_ll_get_ana_reg10_ioldosel(void);

void sys_ll_set_ana_reg10_iobyapssen(uint32_t v);

uint32_t sys_ll_get_ana_reg10_iobyapssen(void);

void sys_ll_set_ana_reg10_ckfs(uint32_t v);

uint32_t sys_ll_get_ana_reg10_ckfs(void);

void sys_ll_set_ana_reg10_ckintsel(uint32_t v);

uint32_t sys_ll_get_ana_reg10_ckintsel(void);

void sys_ll_set_ana_reg10_osccaltrig(uint32_t v);

uint32_t sys_ll_get_ana_reg10_osccaltrig(void);

void sys_ll_set_ana_reg10_mroscsel(uint32_t v);

uint32_t sys_ll_get_ana_reg10_mroscsel(void);

void sys_ll_set_ana_reg10_mrosci_cal(uint32_t v);

uint32_t sys_ll_get_ana_reg10_mrosci_cal(void);

void sys_ll_set_ana_reg10_mrosccap_cal(uint32_t v);

uint32_t sys_ll_get_ana_reg10_mrosccap_cal(void);

//reg ana_reg11:

void sys_ll_set_ana_reg11_value(uint32_t v);

uint32_t sys_ll_get_ana_reg11_value(void);

void sys_ll_set_ana_reg11_sfsr(uint32_t v);

uint32_t sys_ll_get_ana_reg11_sfsr(void);

void sys_ll_set_ana_reg11_ensfsaa(uint32_t v);

uint32_t sys_ll_get_ana_reg11_ensfsaa(void);

void sys_ll_set_ana_reg11_apfms(uint32_t v);

uint32_t sys_ll_get_ana_reg11_apfms(void);

void sys_ll_set_ana_reg11_atmpo_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg11_atmpo_sel(void);

void sys_ll_set_ana_reg11_ampoen(uint32_t v);

uint32_t sys_ll_get_ana_reg11_ampoen(void);

void sys_ll_set_ana_reg11_enpowa(uint32_t v);

uint32_t sys_ll_get_ana_reg11_enpowa(void);

void sys_ll_set_ana_reg11_avea_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg11_avea_sel(void);

void sys_ll_set_ana_reg11_aforcepfm(uint32_t v);

uint32_t sys_ll_get_ana_reg11_aforcepfm(void);

void sys_ll_set_ana_reg11_acls(uint32_t v);

uint32_t sys_ll_get_ana_reg11_acls(void);

void sys_ll_set_ana_reg11_aswrsten(uint32_t v);

uint32_t sys_ll_get_ana_reg11_aswrsten(void);

void sys_ll_set_ana_reg11_aripc(uint32_t v);

uint32_t sys_ll_get_ana_reg11_aripc(void);

void sys_ll_set_ana_reg11_arampc(uint32_t v);

uint32_t sys_ll_get_ana_reg11_arampc(void);

void sys_ll_set_ana_reg11_arampcen(uint32_t v);

uint32_t sys_ll_get_ana_reg11_arampcen(void);

void sys_ll_set_ana_reg11_aenburst(uint32_t v);

uint32_t sys_ll_get_ana_reg11_aenburst(void);

void sys_ll_set_ana_reg11_apfmen(uint32_t v);

uint32_t sys_ll_get_ana_reg11_apfmen(void);

void sys_ll_set_ana_reg11_aldosel(uint32_t v);

uint32_t sys_ll_get_ana_reg11_aldosel(void);

//reg ana_reg12:

void sys_ll_set_ana_reg12_value(uint32_t v);

uint32_t sys_ll_get_ana_reg12_value(void);

void sys_ll_set_ana_reg12_buckd_softst(uint32_t v);

uint32_t sys_ll_get_ana_reg12_buckd_softst(void);

void sys_ll_set_ana_reg12_dzcdcnt_manu(uint32_t v);

uint32_t sys_ll_get_ana_reg12_dzcdcnt_manu(void);

void sys_ll_set_ana_reg12_clk_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg12_clk_sel(void);

void sys_ll_set_ana_reg12_dpfms(uint32_t v);

uint32_t sys_ll_get_ana_reg12_dpfms(void);

void sys_ll_set_ana_reg12_dtmpo_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg12_dtmpo_sel(void);

void sys_ll_set_ana_reg12_dmpoen(uint32_t v);

uint32_t sys_ll_get_ana_reg12_dmpoen(void);

void sys_ll_set_ana_reg12_dforcepfm(uint32_t v);

uint32_t sys_ll_get_ana_reg12_dforcepfm(void);

void sys_ll_set_ana_reg12_dcls(uint32_t v);

uint32_t sys_ll_get_ana_reg12_dcls(void);

void sys_ll_set_ana_reg12_dswrsten(uint32_t v);

uint32_t sys_ll_get_ana_reg12_dswrsten(void);

void sys_ll_set_ana_reg12_dripc(uint32_t v);

uint32_t sys_ll_get_ana_reg12_dripc(void);

void sys_ll_set_ana_reg12_drampc(uint32_t v);

uint32_t sys_ll_get_ana_reg12_drampc(void);

void sys_ll_set_ana_reg12_drampcen(uint32_t v);

uint32_t sys_ll_get_ana_reg12_drampcen(void);

void sys_ll_set_ana_reg12_denburst(uint32_t v);

uint32_t sys_ll_get_ana_reg12_denburst(void);

void sys_ll_set_ana_reg12_dpfmen(uint32_t v);

uint32_t sys_ll_get_ana_reg12_dpfmen(void);

void sys_ll_set_ana_reg12_dldosel(uint32_t v);

uint32_t sys_ll_get_ana_reg12_dldosel(void);

//reg ana_reg13:

void sys_ll_set_ana_reg13_value(uint32_t v);

uint32_t sys_ll_get_ana_reg13_value(void);

void sys_ll_set_ana_reg13_pwdovp1v(uint32_t v);

uint32_t sys_ll_get_ana_reg13_pwdovp1v(void);

void sys_ll_set_ana_reg13_asoft_stc(uint32_t v);

uint32_t sys_ll_get_ana_reg13_asoft_stc(void);

void sys_ll_set_ana_reg13_volen(uint32_t v);

uint32_t sys_ll_get_ana_reg13_volen(void);

void sys_ll_set_ana_reg13_dpfms(uint32_t v);

uint32_t sys_ll_get_ana_reg13_dpfms(void);

void sys_ll_set_ana_reg13_dtmpo_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg13_dtmpo_sel(void);

void sys_ll_set_ana_reg13_dmpoen(uint32_t v);

uint32_t sys_ll_get_ana_reg13_dmpoen(void);

void sys_ll_set_ana_reg13_pavea_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg13_pavea_sel(void);

void sys_ll_set_ana_reg13_dforcepfm(uint32_t v);

uint32_t sys_ll_get_ana_reg13_dforcepfm(void);

void sys_ll_set_ana_reg13_dcls(uint32_t v);

uint32_t sys_ll_get_ana_reg13_dcls(void);

void sys_ll_set_ana_reg13_dswrsten(uint32_t v);

uint32_t sys_ll_get_ana_reg13_dswrsten(void);

void sys_ll_set_ana_reg13_dripc(uint32_t v);

uint32_t sys_ll_get_ana_reg13_dripc(void);

void sys_ll_set_ana_reg13_drampc(uint32_t v);

uint32_t sys_ll_get_ana_reg13_drampc(void);

void sys_ll_set_ana_reg13_drampcen(uint32_t v);

uint32_t sys_ll_get_ana_reg13_drampcen(void);

void sys_ll_set_ana_reg13_paenburst(uint32_t v);

uint32_t sys_ll_get_ana_reg13_paenburst(void);

void sys_ll_set_ana_reg13_papfmen(uint32_t v);

uint32_t sys_ll_get_ana_reg13_papfmen(void);

void sys_ll_set_ana_reg13_enbuckpa(uint32_t v);

uint32_t sys_ll_get_ana_reg13_enbuckpa(void);

//reg ana_reg14:

void sys_ll_set_ana_reg14_value(uint32_t v);

uint32_t sys_ll_get_ana_reg14_value(void);

void sys_ll_set_ana_reg14_chs(uint32_t v);

void sys_ll_set_ana_reg14_en_lpmod(uint32_t v);

void sys_ll_set_ana_reg14_cal_vth(uint32_t v);

void sys_ll_set_ana_reg14_crg(uint32_t v);

void sys_ll_set_ana_reg14_vrefs(uint32_t v);

void sys_ll_set_ana_reg14_gain_s(uint32_t v);

void sys_ll_set_ana_reg14_td_latch1v(uint32_t v);

void sys_ll_set_ana_reg14_pwd_td(uint32_t v);

void sys_ll_set_ana_reg14_en_seri_cap(uint32_t v);

void sys_ll_set_ana_reg14_sel_seri_cap(uint32_t v);

//reg ana_reg15:

void sys_ll_set_ana_reg15_value(uint32_t v);

uint32_t sys_ll_get_ana_reg15_value(void);

void sys_ll_set_ana_reg15_cal_number1v(uint32_t v);

void sys_ll_set_ana_reg15_cal_period1v(uint32_t v);

void sys_ll_set_ana_reg15_test_number1v(uint32_t v);

void sys_ll_set_ana_reg15_test_period1v(uint32_t v);

void sys_ll_set_ana_reg15_chs(uint32_t v);

void sys_ll_set_ana_reg15_nc_21_21(uint32_t v);

void sys_ll_set_ana_reg15_chs_sel_cal1v(uint32_t v);

void sys_ll_set_ana_reg15_cal_done_clr1v(uint32_t v);

void sys_ll_set_ana_reg15_en_cal_force1v(uint32_t v);

void sys_ll_set_ana_reg15_en_cal_auto1v(uint32_t v);

void sys_ll_set_ana_reg15_en_scm(uint32_t v);

void sys_ll_set_ana_reg15_en_adcmod(uint32_t v);

void sys_ll_set_ana_reg15_enfsr1v(uint32_t v);

//reg ana_reg16:

void sys_ll_set_ana_reg16_value(uint32_t v);

uint32_t sys_ll_get_ana_reg16_value(void);

void sys_ll_set_ana_reg16_int_en(uint32_t v);

void sys_ll_set_ana_reg16_int_en16(uint32_t v);

void sys_ll_set_ana_reg16_nc_11_15(uint32_t v);

void sys_ll_set_ana_reg16_ckadc_sel(uint32_t v);

void sys_ll_set_ana_reg16_int_clr_sel1v(uint32_t v);

void sys_ll_set_ana_reg16_ctrl_ck2d(uint32_t v);

void sys_ll_set_ana_reg16_ctrl_seri_cap(uint32_t v);

void sys_ll_set_ana_reg16_en_testcmp1v(uint32_t v);

void sys_ll_set_ana_reg16_en_man_wr1v(uint32_t v);

void sys_ll_set_ana_reg16_en_manmod1v(uint32_t v);

void sys_ll_set_ana_reg16_cap_calspi1v(uint32_t v);

void sys_ll_set_ana_reg16_cal_period1v(uint32_t v);

void sys_ll_set_ana_reg16_cal_number1v(uint32_t v);

//reg ana_reg17:

void sys_ll_set_ana_reg17_value(uint32_t v);

uint32_t sys_ll_get_ana_reg17_value(void);

void sys_ll_set_ana_reg17_int_clr(uint32_t v);

void sys_ll_set_ana_reg17_int_clr16(uint32_t v);

void sys_ll_set_ana_reg17_nc_11_11(uint32_t v);

void sys_ll_set_ana_reg17_int_clr_cal(uint32_t v);

void sys_ll_set_ana_reg17_int_en_cal(uint32_t v);

//reg ana_reg18:

void sys_ll_set_ana_reg18_value(uint32_t v);

uint32_t sys_ll_get_ana_reg18_value(void);

void sys_ll_set_ana_reg18_iselaud(uint32_t v);

uint32_t sys_ll_get_ana_reg18_iselaud(void);

void sys_ll_set_ana_reg18_audck_rlcen1v(uint32_t v);

uint32_t sys_ll_get_ana_reg18_audck_rlcen1v(void);

void sys_ll_set_ana_reg18_lchckinven1v(uint32_t v);

uint32_t sys_ll_get_ana_reg18_lchckinven1v(void);

void sys_ll_set_ana_reg18_enaudbias(uint32_t v);

uint32_t sys_ll_get_ana_reg18_enaudbias(void);

void sys_ll_set_ana_reg18_enadcbias(uint32_t v);

uint32_t sys_ll_get_ana_reg18_enadcbias(void);

void sys_ll_set_ana_reg18_enmicbias(uint32_t v);

uint32_t sys_ll_get_ana_reg18_enmicbias(void);

void sys_ll_set_ana_reg18_adcckinven1v(uint32_t v);

uint32_t sys_ll_get_ana_reg18_adcckinven1v(void);

void sys_ll_set_ana_reg18_dacfb2st0v9(uint32_t v);

uint32_t sys_ll_get_ana_reg18_dacfb2st0v9(void);

void sys_ll_set_ana_reg18_nc1(uint32_t v);

uint32_t sys_ll_get_ana_reg18_nc1(void);

void sys_ll_set_ana_reg18_micbias_trm(uint32_t v);

uint32_t sys_ll_get_ana_reg18_micbias_trm(void);

void sys_ll_set_ana_reg18_micbias_voc(uint32_t v);

uint32_t sys_ll_get_ana_reg18_micbias_voc(void);

void sys_ll_set_ana_reg18_vrefsel1v(uint32_t v);

uint32_t sys_ll_get_ana_reg18_vrefsel1v(void);

void sys_ll_set_ana_reg18_capswspi(uint32_t v);

uint32_t sys_ll_get_ana_reg18_capswspi(void);

void sys_ll_set_ana_reg18_adref_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg18_adref_sel(void);

void sys_ll_set_ana_reg18_nc0(uint32_t v);

uint32_t sys_ll_get_ana_reg18_nc0(void);

void sys_ll_set_ana_reg18_spi_dacckpssel(uint32_t v);

//reg ana_reg19:

void sys_ll_set_ana_reg19_value(uint32_t v);

uint32_t sys_ll_get_ana_reg19_value(void);

void sys_ll_set_ana_reg19_isel(uint32_t v);

uint32_t sys_ll_get_ana_reg19_isel(void);

void sys_ll_set_ana_reg19_micirsel1(uint32_t v);

uint32_t sys_ll_get_ana_reg19_micirsel1(void);

void sys_ll_set_ana_reg19_micdacit(uint32_t v);

uint32_t sys_ll_get_ana_reg19_micdacit(void);

void sys_ll_set_ana_reg19_micdacih(uint32_t v);

uint32_t sys_ll_get_ana_reg19_micdacih(void);

void sys_ll_set_ana_reg19_micsingleen(uint32_t v);

uint32_t sys_ll_get_ana_reg19_micsingleen(void);

void sys_ll_set_ana_reg19_dccompen(uint32_t v);

uint32_t sys_ll_get_ana_reg19_dccompen(void);

void sys_ll_set_ana_reg19_micgain(uint32_t v);

uint32_t sys_ll_get_ana_reg19_micgain(void);

void sys_ll_set_ana_reg19_micdacen(uint32_t v);

uint32_t sys_ll_get_ana_reg19_micdacen(void);

void sys_ll_set_ana_reg19_stg2lsen1v(uint32_t v);

uint32_t sys_ll_get_ana_reg19_stg2lsen1v(void);

void sys_ll_set_ana_reg19_openloopcal1v(uint32_t v);

uint32_t sys_ll_get_ana_reg19_openloopcal1v(void);

void sys_ll_set_ana_reg19_callatch(uint32_t v);

uint32_t sys_ll_get_ana_reg19_callatch(void);

void sys_ll_set_ana_reg19_vcmsel(uint32_t v);

uint32_t sys_ll_get_ana_reg19_vcmsel(void);

void sys_ll_set_ana_reg19_dwamode(uint32_t v);

uint32_t sys_ll_get_ana_reg19_dwamode(void);

void sys_ll_set_ana_reg19_r2ren(uint32_t v);

uint32_t sys_ll_get_ana_reg19_r2ren(void);

void sys_ll_set_ana_reg19_nc_26_27(uint32_t v);

uint32_t sys_ll_get_ana_reg19_nc_26_27(void);

void sys_ll_set_ana_reg19_micen(uint32_t v);

uint32_t sys_ll_get_ana_reg19_micen(void);

void sys_ll_set_ana_reg19_rst(uint32_t v);

uint32_t sys_ll_get_ana_reg19_rst(void);

void sys_ll_set_ana_reg19_bpdwa1v(uint32_t v);

uint32_t sys_ll_get_ana_reg19_bpdwa1v(void);

void sys_ll_set_ana_reg19_hcen1stg(uint32_t v);

uint32_t sys_ll_get_ana_reg19_hcen1stg(void);

//reg ana_reg20:

void sys_ll_set_ana_reg20_value(uint32_t v);

uint32_t sys_ll_get_ana_reg20_value(void);

void sys_ll_set_ana_reg20_hpdac(uint32_t v);

uint32_t sys_ll_get_ana_reg20_hpdac(void);

void sys_ll_set_ana_reg20_calcon_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg20_calcon_sel(void);

void sys_ll_set_ana_reg20_oscdac(uint32_t v);

uint32_t sys_ll_get_ana_reg20_oscdac(void);

void sys_ll_set_ana_reg20_ocendac(uint32_t v);

uint32_t sys_ll_get_ana_reg20_ocendac(void);

void sys_ll_set_ana_reg20_vcmsel(uint32_t v);

uint32_t sys_ll_get_ana_reg20_vcmsel(void);

void sys_ll_set_ana_reg20_adjdacref(uint32_t v);

uint32_t sys_ll_get_ana_reg20_adjdacref(void);

void sys_ll_set_ana_reg20_dcochg(uint32_t v);

uint32_t sys_ll_get_ana_reg20_dcochg(void);

void sys_ll_set_ana_reg20_diffen(uint32_t v);

uint32_t sys_ll_get_ana_reg20_diffen(void);

void sys_ll_set_ana_reg20_endaccal(uint32_t v);

uint32_t sys_ll_get_ana_reg20_endaccal(void);

void sys_ll_set_ana_reg20_nc2(uint32_t v);

uint32_t sys_ll_get_ana_reg20_nc2(void);

void sys_ll_set_ana_reg20_lendcoc(uint32_t v);

uint32_t sys_ll_get_ana_reg20_lendcoc(void);

void sys_ll_set_ana_reg20_nc1(uint32_t v);

uint32_t sys_ll_get_ana_reg20_nc1(void);

void sys_ll_set_ana_reg20_lenvcmd(uint32_t v);

uint32_t sys_ll_get_ana_reg20_lenvcmd(void);

void sys_ll_set_ana_reg20_dacdrven(uint32_t v);

uint32_t sys_ll_get_ana_reg20_dacdrven(void);

void sys_ll_set_ana_reg20_nc0(uint32_t v);

uint32_t sys_ll_get_ana_reg20_nc0(void);

void sys_ll_set_ana_reg20_daclen(uint32_t v);

uint32_t sys_ll_get_ana_reg20_daclen(void);

void sys_ll_set_ana_reg20_dacg(uint32_t v);

uint32_t sys_ll_get_ana_reg20_dacg(void);

void sys_ll_set_ana_reg20_dacmute(uint32_t v);

uint32_t sys_ll_get_ana_reg20_dacmute(void);

void sys_ll_set_ana_reg20_dacdwamode_sel(uint32_t v);

uint32_t sys_ll_get_ana_reg20_dacdwamode_sel(void);

void sys_ll_set_ana_reg20_dacsel(uint32_t v);

uint32_t sys_ll_get_ana_reg20_dacsel(void);

//reg ana_reg21:

void sys_ll_set_ana_reg21_value(uint32_t v);

uint32_t sys_ll_get_ana_reg21_value(void);

void sys_ll_set_ana_reg21_lmdcin(uint32_t v);

uint32_t sys_ll_get_ana_reg21_lmdcin(void);

void sys_ll_set_ana_reg21_nc1(uint32_t v);

uint32_t sys_ll_get_ana_reg21_nc1(void);

void sys_ll_set_ana_reg21_spirst_ovc(uint32_t v);

uint32_t sys_ll_get_ana_reg21_spirst_ovc(void);

void sys_ll_set_ana_reg21_nc0(uint32_t v);

uint32_t sys_ll_get_ana_reg21_nc0(void);

void sys_ll_set_ana_reg21_enidacl(uint32_t v);

uint32_t sys_ll_get_ana_reg21_enidacl(void);

void sys_ll_set_ana_reg21_dac3rdhc0v9(uint32_t v);

uint32_t sys_ll_get_ana_reg21_dac3rdhc0v9(void);

void sys_ll_set_ana_reg21_hc2s(uint32_t v);

uint32_t sys_ll_get_ana_reg21_hc2s(void);

void sys_ll_set_ana_reg21_rfb_ctrl(uint32_t v);

uint32_t sys_ll_get_ana_reg21_rfb_ctrl(void);

void sys_ll_set_ana_reg21_vcmsel(uint32_t v);

uint32_t sys_ll_get_ana_reg21_vcmsel(void);

void sys_ll_set_ana_reg21_enbs(uint32_t v);

uint32_t sys_ll_get_ana_reg21_enbs(void);

void sys_ll_set_ana_reg21_calck_sel0v9(uint32_t v);

uint32_t sys_ll_get_ana_reg21_calck_sel0v9(void);

void sys_ll_set_ana_reg21_bpdwa0v9(uint32_t v);

uint32_t sys_ll_get_ana_reg21_bpdwa0v9(void);

void sys_ll_set_ana_reg21_looprst0v9(uint32_t v);

uint32_t sys_ll_get_ana_reg21_looprst0v9(void);

void sys_ll_set_ana_reg21_oct0v9(uint32_t v);

uint32_t sys_ll_get_ana_reg21_oct0v9(void);

void sys_ll_set_ana_reg21_sout0v9(uint32_t v);

uint32_t sys_ll_get_ana_reg21_sout0v9(void);

void sys_ll_set_ana_reg21_hc0v9(uint32_t v);

uint32_t sys_ll_get_ana_reg21_hc0v9(void);

//reg ana_reg22:

void sys_ll_set_ana_reg22_value(uint32_t v);

uint32_t sys_ll_get_ana_reg22_value(void);

void sys_ll_set_ana_reg22_ictrl_dsppll(uint32_t v);

uint32_t sys_ll_get_ana_reg22_ictrl_dsppll(void);

void sys_ll_set_ana_reg22_nc_4_18(uint32_t v);

uint32_t sys_ll_get_ana_reg22_nc_4_18(void);

void sys_ll_set_ana_reg22_mode(uint32_t v);

uint32_t sys_ll_get_ana_reg22_mode(void);

void sys_ll_set_ana_reg22_iamsel(uint32_t v);

uint32_t sys_ll_get_ana_reg22_iamsel(void);

void sys_ll_set_ana_reg22_hvref(uint32_t v);

uint32_t sys_ll_get_ana_reg22_hvref(void);

void sys_ll_set_ana_reg22_lvref(uint32_t v);

uint32_t sys_ll_get_ana_reg22_lvref(void);

void sys_ll_set_ana_reg22_nc_25_31(uint32_t v);

uint32_t sys_ll_get_ana_reg22_nc_25_31(void);

//reg ana_reg23:

void sys_ll_set_ana_reg23_value(uint32_t v);

uint32_t sys_ll_get_ana_reg23_value(void);

void sys_ll_set_ana_reg23_camsel(uint32_t v);

uint32_t sys_ll_get_ana_reg23_camsel(void);

void sys_ll_set_ana_reg23_msw(uint32_t v);

uint32_t sys_ll_get_ana_reg23_msw(void);

void sys_ll_set_ana_reg23_tstcken_dpll(uint32_t v);

uint32_t sys_ll_get_ana_reg23_tstcken_dpll(void);

void sys_ll_set_ana_reg23_osccal_trig(uint32_t v);

uint32_t sys_ll_get_ana_reg23_osccal_trig(void);

void sys_ll_set_ana_reg23_cnti(uint32_t v);

uint32_t sys_ll_get_ana_reg23_cnti(void);

void sys_ll_set_ana_reg23_nc_21_21(uint32_t v);

uint32_t sys_ll_get_ana_reg23_nc_21_21(void);

void sys_ll_set_ana_reg23_spi_rst(uint32_t v);

uint32_t sys_ll_get_ana_reg23_spi_rst(void);

void sys_ll_set_ana_reg23_closeloop_en(uint32_t v);

uint32_t sys_ll_get_ana_reg23_closeloop_en(void);

void sys_ll_set_ana_reg23_caltime(uint32_t v);

uint32_t sys_ll_get_ana_reg23_caltime(void);

void sys_ll_set_ana_reg23_lpfrz(uint32_t v);

uint32_t sys_ll_get_ana_reg23_lpfrz(void);

void sys_ll_set_ana_reg23_icp(uint32_t v);

uint32_t sys_ll_get_ana_reg23_icp(void);

void sys_ll_set_ana_reg23_cp2ctrl(uint32_t v);

uint32_t sys_ll_get_ana_reg23_cp2ctrl(void);

#ifdef __cplusplus
}
#endif