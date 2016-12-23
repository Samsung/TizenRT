/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef __ARCH_ARM_SRC_S5J_SOC_PWRCAL_S5JT200_S5JT200_VCLK_H
#define __ARCH_ARM_SRC_S5J_SOC_PWRCAL_S5JT200_S5JT200_VCLK_H

enum {
	gate_uhd_efuse_writer = 0x0A000000,
	gate_mct0,
	gate_tick_counter,
	gate_wdt,
	gate_gpiocon,
	gate_mailbox_wifi,
	gate_mailbox_m0,
	gate_sysreg,
	gate_pdma,
	gate_iso_sss,
	gate_top_rtc,
	gate_puf_system,
	gate_uart0,
	gate_uart1,
	gate_uart2,
	gate_uart3,
	gate_uart_debug,
	gate_cssys,
	gate_spi0,
	gate_spi1,
	gate_spi2,
	gate_spi3,
	gate_hsi2c0,
	gate_hsi2c1,
	gate_hsi2c2,
	gate_hsi2c3,
	gate_i2s,
	gate_adc_if,
	gate_pwm0,
	gate_pwm1,
	gate_intmem,
	gate_intmem_shared,
	gate_sdio_device,
	gate_serialflash,
	gate_gic400,
	vclk_group_grpgate_end,
	num_of_grpgate = vclk_group_grpgate_end - 0x0A000000,

	sclk_dummy = 0x0A010000,
	vclk_group_m1d1g1_end,
	num_of_m1d1g1 = vclk_group_m1d1g1_end - 0x0A010000,

	p1_wpll = 0x0A020000,
	vclk_group_p1_end,
	num_of_p1 = vclk_group_p1_end - 0x0A020000,

	m1_wpll = 0x0A030000,
	m1_clkcmu_is2b,
	m1_clkcmu_uart,
	vclk_group_m1_end,
	num_of_m1 = vclk_group_m1_end - 0x0A030000,

	d1_spi0 = 0x0A040000,
	d1_spi1,
	d1_spi2,
	d1_spi3,
	d1_serialflash,
	vclk_group_d1_end,
	num_of_d1 = vclk_group_d1_end - 0x0A040000,

	pxmxdx_top = 0x0A050000,
	vclk_group_pxmxdx_end,
	num_of_pxmxdx = vclk_group_pxmxdx_end - 0x0A050000,

	umux_dummy = 0x0A060000,
	vclk_group_umux_end,
	num_of_umux = vclk_group_umux_end - 0x0A060000,

	dvfs_dummy = 0x0A070000,
	vclk_group_dfs_end,
	num_of_dfs = vclk_group_dfs_end - 0x0A070000,
};

#endif
