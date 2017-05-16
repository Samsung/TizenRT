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
/****************************************************************************
 * arch/arm/src/s5j/s5j_vclk.h
 *
 *   Copyright (C) 2009-2010, 2014-2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
#ifndef __ARCH_ARM_SRC_S5J_S5J_VCLK_H__
#define __ARCH_ARM_SRC_S5J_S5J_VCLK_H__

enum {
	gate_uhd_efuse_writer = 0x0A000000,
	gate_top_rtc,
	gate_hsi2c0,
	gate_hsi2c1,
	gate_hsi2c2,
	gate_hsi2c3,
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

#define S5J_DEFAULT_I2C_CLOCK	(160 * 1000 * 1000)
#define S5J_DEFAULT_UART_CLOCK	(26 * 1000 * 1000)

#endif /* __ARCH_ARM_SRC_S5J_S5J_VCLK_H__ */
