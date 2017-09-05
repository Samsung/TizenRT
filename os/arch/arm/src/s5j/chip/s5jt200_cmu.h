/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/chip/s5j200_clock.h
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

#ifndef _ARCH_ARM_SRC_S5J_CHIP_S5JT200_CLOCK_H
#define _ARCH_ARM_SRC_S5J_CHIP_S5JT200_CLOCK_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Address *********************************************************/

/* PLL control register (24 entries) */
#define S5J_CMU_WPLL_CON0					0x80080000
#define S5J_CMU_WPLL_CON1					0x80080004
#define S5J_CMU_WPLL_CON2					0x80080008
#define S5J_CMU_WPLL_CON3					0x8008000C
#define S5J_CMU_WPLL_CON4					0x80080010
#define S5J_CMU_WPLL_CON5					0x80080014
#define S5J_CMU_WPLL_STAT					0x80080018
#define S5J_CMU_PLL_CON0					0x80080180
#define S5J_CMU_PLL_CON1					0x80080184
#define S5J_CMU_PLL_CON2					0x80080188

/* Miscellaneous functions */
#define S5J_CMU_CMU_CON						0x80080800
#define S5J_CMU_CLKOUT_CON					0x80080810
#define S5J_CMU_MCU_SPARE0					0x80080a00
#define S5J_CMU_MCU_SPARE1					0x80080a04

/* MUX controls such as MUX selection, enable and status */
#define S5J_CMU_MUX_I2SB					0x80081000
#define S5J_CMU_MUX_UART					0x8008100c

/* Clock divider controls such as divider ratio and status */
#define S5J_CMU_DIV_SFLASH					0x80081800
#define S5J_CMU_DIV_SPI0					0x80081804
#define S5J_CMU_DIV_SPI1					0x80081808
#define S5J_CMU_DIV_SPI2					0x8008180C
#define S5J_CMU_DIV_SPI3					0x80081810
#define S5J_CMU_DIV_WPLL_DIV12				0x80081814
#define S5J_CMU_DIV_WPLL_DIV3				0x80081818
#define S5J_CMU_DIV_WPLL_DIV6				0x8008181C

/* Clock gating controls such as clock gating of IPs and function blocks */
#define S5J_CMU_GAT_WPLL_DIV12				0x80082000
#define S5J_CMU_GAT_ADC						0x80082004
#define S5J_CMU_GAT_CM0P					0x80082008
#define S5J_CMU_GAT_GPIOCON_CLK				0x8008200c
#define S5J_CMU_GAT_I2S_I2SBCLKI			0x80082010
#define S5J_CMU_GAT_MCT0					0x80082014
#define S5J_CMU_GAT_CMU_PCLK				0x80082018
#define S5J_CMU_GAT_PWM0					0x8008201C
#define S5J_CMU_GAT_PWM1					0x80082020
#define S5J_CMU_GAT_SYSREG_CLK				0x80082028
#define S5J_CMU_GAT_CHIPID					0x80082034
#define S5J_CMU_GAT_WDT						0x80082038
#define S5J_CMU_GAT_ADC_PCLK				0x8008203C
#define S5J_CMU_GAT_AHB2AXI_CM0P_ACLK		0x80082040
#define S5J_CMU_GAT_AHB2AXI_SDIO_ACLK		0x80082048
#define S5J_CMU_GAT_APB_CSSYS_PCLKM			0x80082048
#define S5J_CMU_GAT_APB_CSSYS_PCLKS			0x8008204C
#define S5J_CMU_GAT_APB_PDMA_PCLKM			0x80082050
#define S5J_CMU_GAT_APB_PDMA_PCLKS			0x80082054
#define S5J_CMU_GAT_AXI2AHB_FLASH_ACLK		0x80082058
#define S5J_CMU_GAT_AXI2AHB_SDIO_PCLK		0x8008205C
#define S5J_CMU_GAT_PERIC_ACLK				0x80082060
#define S5J_CMU_GAT_PERIS0_ACLK				0x80082064
#define S5J_CMU_GAT_PERIS1_ACLK				0x80082068
#define S5J_CMU_GAT_CM0P_DCLK				0x8008206C
#define S5J_CMU_GAT_CM0P_HCLK				0x80082070
#define S5J_CMU_GAT_CM0P_SCLK				0x80082074
#define S5J_CMU_GAT_CSSYS_PCLKDBG			0x80082078
#define S5J_CMU_GAT_DS_64TO32_SDIO_ACLK		0x8008207C
#define S5J_CMU_GAT_DS_64TO32_DP_ACLK		0x80082080
#define S5J_CMU_GAT_DS_64TO32_FLASH_ACLK	0x80082084
#define S5J_CMU_GAT_GIC400_INPUT			0x80082088
#define S5J_CMU_GAT_GIC400					0x8008208C
#define S5J_CMU_GAT_HSI2C0					0x80082090
#define S5J_CMU_GAT_HSI2C1					0x80082094
#define S5J_CMU_GAT_HSI2C2					0x80082098
#define S5J_CMU_GAT_HSI2C3					0x8008209C
#define S5J_CMU_GAT_I2S_PCLK				0x800820a0
#define S5J_CMU_GAT_INTMEM_ACLK				0x800820a4
#define S5J_CMU_GAT_INTMEM_SHARED_ACLK		0x800820a8
#define S5J_CMU_GAT_CR4_CLKIN				0x800820ac
#define S5J_CMU_GAT_CR4_FREECLKIN			0x800820b0
#define S5J_CMU_GAT_CR4_PCLKDBG				0x800820b4
#define S5J_CMU_GAT_LHM_AXI_D_WIFI_ICLK		0x800820b8
#define S5J_CMU_GAT_LHM_DP_ICLK				0x800820bc
#define S5J_CMU_GAT_LHS_DP_ICLK				0x800820c0
#define S5J_CMU_GAT_MAILBOX_M0				0x800820c4
#define S5J_CMU_GAT_MAILBOX_WIFI			0x800820c8
#define S5J_CMU_GAT_MCT0_PCLK				0x800820cc
#define S5J_CMU_GAT_PDMA_ACLK				0x800820d0
#define S5J_CMU_GAT_PMU_PCLK				0x800820d4
#define S5J_CMU_GAT_PMU_PCLK_CSSYS			0x800820d8
#define S5J_CMU_GAT_PUF_SYSTEM_ICLK			0x800820dc
#define S5J_CMU_GAT_PWM0_PCLK				0x800820e0
#define S5J_CMU_GAT_PWM1_PCLK				0x800820e4
#define S5J_CMU_GAT_SDIO_DEVICE_CLK_AHB		0x800820f4
#define S5J_CMU_GAT_SFLASH_HCLK				0x800820f8
#define S5J_CMU_GAT_SFLASH_SFCLK			0x800820fc
#define S5J_CMU_GAT_SFR_APBIF_GPIO_PCLK		0x80082100
#define S5J_CMU_GAT_SPI0_PCLK				0x80082104
#define S5J_CMU_GAT_SPI0_EXTCLK				0x80082108
#define S5J_CMU_GAT_SPI1_PCLK				0x8008210C
#define S5J_CMU_GAT_SPI1_EXTCLK				0x80082110
#define S5J_CMU_GAT_SPI2_PCLK				0x80082114
#define S5J_CMU_GAT_SPI2_EXTCLK				0x80082118
#define S5J_CMU_GAT_SPI3_PCLK				0x8008211c
#define S5J_CMU_GAT_SPI3_EXTCLK				0x80082120
#define S5J_CMU_GAT_SWEEPER_WIFI_ACLK		0x80082124
#define S5J_CMU_GAT_SYSREG_PCLK				0x80082128
#define S5J_CMU_GAT_TICKCNT_PCLK			0x8008212C
#define S5J_CMU_GAT_TOPRTC_PCLK				0x80082130
#define S5J_CMU_GAT_UART0_EXTCLK			0x80082134
#define S5J_CMU_GAT_UART0_PCLK				0x80082138
#define S5J_CMU_GAT_UART1_EXTCLK			0x8008213C
#define S5J_CMU_GAT_UART1_PCLK				0x80082140
#define S5J_CMU_GAT_UART2_EXTCLK			0x80082144
#define S5J_CMU_GAT_UART2_PCLK				0x80082148
#define S5J_CMU_GAT_UART3_EXTCLK			0x8008214C
#define S5J_CMU_GAT_UART3_PCLK				0x80082150
#define S5J_CMU_GAT_UARTDBG_EXTCLK			0x80082154
#define S5J_CMU_GAT_UARTDBG_PCLK			0x80082158
#define S5J_CMU_GAT_CHIPID_PCLK				0x8008215C
#define S5J_CMU_GAT_US_32TO64_CM0P_ACLK		0x80082160
#define S5J_CMU_GAT_US_32TO64_PDMA_ACLK		0x80082164
#define S5J_CMU_GAT_US_32TO64_SDIO_ACLK		0x80082168
#define S5J_CMU_GAT_WDT_PCLK				0x8008216C
#define S5J_CMU_GAT_XIU_D_T20_ACLK			0x80082174

/* Q-Channel controls such as Q-channel enable and counter */
#define S5J_CMU_DMYQCH_CON_CR4				0x80083014
#define S5J_CMU_DMYQCH_CON_XIU				0x80083020
#define S5J_CMU_QCH_CON_LHM_AXI				0x80083024
#define S5J_CMU_QCH_CON_LHM_DP				0x80083028
#define S5J_CMU_QCH_CON_LHS_DP				0x8008302C
#define S5J_CMU_QCH_CON_MCU					0x80083030

/* Q-state controls */
#define S5J_CMU_QUEUE_CTRL					0x80083C00

/* Register Bitfield Definitions ********************************************/

/* WPLL_CON0 ****************************************************************/
#define CMU_WPLL_CON0_FREF_SEL				(1 << 28)
#define CMU_WPLL_CON0_EN					(1 << 22)
#define CMU_WPLL_CON0_FREF_EN				(1 << 21)
#define CMU_WPLL_CON0_PFD_EN				(1 << 20)
#define CMU_WPLL_CON0_CP_EN					(1 << 19)
#define CMU_WPLL_CON0_VCO_EN				(1 << 18)
#define CMU_WPLL_CON0_ABC_EN				(1 << 17)
#define CMU_WPLL_CON0_BGR_EN				(1 << 16)
#define CMU_WPLL_CON0_PFD_TRI				(1 << 2)
#define CMU_WPLL_CON0_CP0					(1 << 1)
#define CMU_WPLL_CON0_PFD_TRI_EXT_SEL		(1 << 0)

/* WPLL_CON1 ****************************************************************/
#define CMU_WPLL_CON1_VCO_ED				(1 << 30)
#define CMU_WPLL_CON1_VCO_REF_EN			(1 << 29)
#define CMU_WPLL_CON1_REF_EXT_SEL			(1 << 28)

#define CMU_WPLL_CON1_VCO_REF_CTRL_SHIFT	27
#define CMU_WPLL_CON1_VCO_REF_CTRL_MASK		(0x3 << CMU_WPLL_CON1_VCO_REF_CTRL_SHIFT)

#define CMU_WPLL_CON1_ABC_START				(1 << 26)

#define CMU_WPLL_CON1_ABC_DIV_SHIFT			8
#define CMU_WPLL_CON1_ABC_DIV_MASK			(0xff << CMU_WPLL_CON1_ABC_DIV_SHIFT)

#define CMU_WPLL_CON1_ABC_FBIT_SHIFT		4
#define CMU_WPLL_CON1_ABC_FBIT_MASK			(0xf << CMU_WPLL_CON1_ABC_FBIT_SHIFT)

#define CMU_WPLL_CON1_ABC_FBIT_SEL			(1 << 0)

/* WPLL_CON2 ****************************************************************/
#define CMU_WPLL_CON2_BGR_SEL_SHIFT			28
#define CMU_WPLL_CON2_BGR_SEL_MASK			(0x3 << CMU_WPLL_CON2_BGR_SEL_SHIFT)

#define CMU_WPLL_CON2_BGR_CNT_SHIFT			24
#define CMU_WPLL_CON2_BGR_CNT_MASK			(0xf << CMU_WPLL_CON2_BGR_CNT_SHIFT)

#define CMU_WPLL_CON2_MMD_DIV_SHIFT			16
#define CMU_WPLL_CON2_MMD_DIV_MASK			(0x3f << CMU_WPLL_CON2_MMD_DIV_SHIFT)

#define CMU_WPLL_CON2_MMD_LOAD_SEL			(1 << 8)

#define CMU_WPLL_CON2_LF_SHIFT				0
#define CMU_WPLL_CON2_LF_MASK				(0x1f << CMU_WPLL_CON2_LF_SHIFT)

/* WPLL_CON3 ****************************************************************/
#define CMU_WPLL_CON3_DSM_EN_SEL			(1 << 29)
#define CMU_WPLL_CON3_DSM_EN				(1 << 28)

#define CMU_WPLL_CON3_DSM_F_SHIFT			0
#define CMU_WPLL_CON3_DSM_F_MASK			(0x7ffffff << CMU_WPLL_CON3_DSM_F_SHIFT)

/* WPLL_CON4 ****************************************************************/
#define CMU_WPLL_CON4_DSM_K_SHIFT			0
#define CMU_WPLL_CON4_DSM_K_MASK			(0x7ffffff << CMU_WPLL_CON4_DSM_K_SHIFT)

/* WPLL_CON5 ****************************************************************/
#define CMU_WPLL_CON5_CLK_ADC_SEL_SHIFT		0
#define CMU_WPLL_CON5_CLK_ADC_SEL_MASK		(0x7 << CMU_WPLL_CON5_CLK_ADC_SEL_SHIFT)

#define CMU_WPLL_CON5_CLK320M_SEL			(1 << 4)
#define CMU_WPLL_CON5_CLK20M_SEL			(1 << 5)

#define CMU_WPLL_CON5_LOCK_TRY_SHIFT		8
#define CMU_WPLL_CON5_LOCK_TRY_MASK			(0xf << CMU_WPLL_CON5_LOCK_TRY_SHIFT)

#define CMU_WPLL_CON5_LOCK_CNT_SHIFT		12
#define CMU_WPLL_CON5_LOCK_CNT_MASK			(0xf << CMU_WPLL_CON5_LOCK_CNT_SHIFT)

#define CMU_WPLL_CON5_EN_CLK_ADC			(1 << 16)
#define CMU_WPLL_CON5_EN_CLK20M				(1 << 17)
#define CMU_WPLL_CON5_EN_CLK80M				(1 << 18)
#define CMU_WPLL_CON5_EN_CLK160M			(1 << 19)
#define CMU_WPLL_CON5_EN_CLK240M			(1 << 20)
#define CMU_WPLL_CON5_EN_CLK480M			(1 << 21)
#define CMU_WPLL_CON5_EN_CLK960M			(1 << 22)
#define CMU_WPLL_CON5_IGNORE_SYSREQ			(1 << 24)

#define CMU_WPLL_CON5_LOCK_MASK_SHIFT		25
#define CMU_WPLL_CON5_LOCK_MASK_MASK		(0x7 << CMU_WPLL_CON5_LOCK_MASK_SHIFT)
#define CMU_WPLL_CON5_LOCK_MASK_FBIT_STATUS	(0x0 << CMU_WPLL_CON5_LOCK_MASK_SHIFT)
#define CMU_WPLL_CON5_LOCK_MASK_ABC_DONE	(0x1 << CMU_WPLL_CON5_LOCK_MASK_SHIFT)
#define CMU_WPLL_CON5_LOCK_MASK_LOCK_CNT	(0x2 << CMU_WPLL_CON5_LOCK_MASK_SHIFT)

#define CMU_WPLL_CON5_EN_CLK_DIV			(1 << 28)

/* WPLL_STAT ****************************************************************/
#define CMU_WPLL_STAT_LOCK					(1 << 28)

#define CMU_WPLL_STAT_HW_STATUS_SHIFT		25
#define CMU_WPLL_STAT_HW_STATUS_MASK		(0x3 << CMU_WPLL_STAT_HW_STATUS_SHIFT)

#define CMU_WPLL_STAT_HW_OVERRIDE			(1 << 24)

#define CMU_WPLL_STAT_FBIT_STATUS_SHIFT		16
#define CMU_WPLL_STAT_FBIT_STATUS_MASK		(0xf << CMU_WPLL_STAT_FBIT_STATUS_SHIFT)

#define CMU_WPLL_STAT_TEST_STATUS			(1 << 8)
#define CMU_WPLL_STAT_ABC_DONE				(1 << 0)

/* CON0_MUX *****************************************************************/
#define CMU_CON0_MUX_BUSY					(1 << 7)

#define CMU_CON0_MUX_SEL_SHIFT				4
#define CMU_CON0_MUX_SEL_MASK				(1 << CMU_CON0_MUX_SEL_SHIFT)
#define CMU_CON0_MUX_SEL_OSCCLK				(0 << CMU_CON0_MUX_SEL_SHIFT)
#define CMU_CON0_MUX_SEL_CLK960M			(1 << CMU_CON0_MUX_SEL_SHIFT)

/* CON1_MUX *****************************************************************/
#define CMU_CON1_MUX_IGNORE_REQ_SYSCLK		(1 << 5)

/* CON2_MUX *****************************************************************/
#define CMU_CON2_MUX_OVERRIDE_BY_HCH		(1 << 30)
#define CMU_CON2_MUX_AUTO_CLKGATING			(1 << 28)

/* CMU_CON ******************************************************************/
#define CMU_CMU_CON_ENABLE_POWER_MANAGEMENT	(1 << 29)
#define CMU_CMU_CON_AUTO_CLKGATING			(1 << 28)

/* CLKOUT_CON ***************************************************************/
#define CMU_CLKOUT_CON_ENABLE				(1 << 29)
#define CMU_CLKOUT_CON_AUTO_CLKGATING		(1 << 28)
#define CMU_CLKOUT_CON_VALIDATE_CLK_REQ		(1 << 20)
#define CMU_CLKOUT_CON_BUSY					(1 << 16)

#define CMU_CLKOUT_CON_SEL_SHIFT			8
#define CMU_CLKOUT_CON_SEL_MASK				(0x1f << CMU_CLKOUT_CON_SEL_SHIFT)
#define CMU_CLKOUT_CON_SEL_OSCCLK			(0x0 << CMU_CLKOUT_CON_SEL_SHIFT)
#define CMU_CLKOUT_CON_SEL_XIU_D_T20_ACLK	(0x1 << CMU_CLKOUT_CON_SEL_SHIFT)
#define CMU_CLKOUT_CON_SEL_XIU_P_T20_ACLK	(0x2 << CMU_CLKOUT_CON_SEL_SHIFT)
#define CMU_CLKOUT_CON_SEL_UART0_EXTCLK		(0x3 << CMU_CLKOUT_CON_SEL_SHIFT)
#define CMU_CLKOUT_CON_SEL_UART1_EXTCLK		(0x4 << CMU_CLKOUT_CON_SEL_SHIFT)
#define CMU_CLKOUT_CON_SEL_UART2_EXTCLK		(0x5 << CMU_CLKOUT_CON_SEL_SHIFT)
#define CMU_CLKOUT_CON_SEL_UART3_EXTCLK		(0x6 << CMU_CLKOUT_CON_SEL_SHIFT)
#define CMU_CLKOUT_CON_SEL_UARTDBG_EXTCLK	(0x7 << CMU_CLKOUT_CON_SEL_SHIFT)

#define CMU_CLKOUT_CON_DIV_RATIO_SHIFT		0
#define CMU_CLKOUT_CON_DIV_RATIO_MASK		(0xf << CMU_CLKOUT_CON_DIV_RATIO_SHIFT)

/* MUX_I2SB *****************************************************************/
#define CMU_MUX_I2SB_OVERRIDE_BY_HCH		(1 << 30)
#define CMU_MUX_I2SB_AUTO_CLKGATING			(1 << 28)
#define CMU_MUX_I2SB_BUSY					(1 << 16)

#define CMU_MUX_I2SB_SELECT_SHIFT			0
#define CMU_MUX_I2SB_SELECT_MASK			(1 << CMU_MUX_I2SB_SELECT_SHIFT)
#define CMU_MUX_I2SB_SELECT_OSCCLK			(0 << CMU_MUX_I2SB_SELECT_SHIFT)
#define CMU_MUX_I2SB_SELECT_PADCLK_I2S_BCLK	(1 << CMU_MUX_I2SB_SELECT_SHIFT)

/* MUX_UART *****************************************************************/
#define CMU_MUX_UART_OVERRIDE_BY_HCH		(1 << 30)
#define CMU_MUX_UART_AUTO_CLKGATING			(1 << 28)
#define CMU_MUX_UART_BUSY					(1 << 16)

#define CMU_MUX_UART_SELECT_SHIFT			0
#define CMU_MUX_UART_SELECT_MASK			(1 << CMU_MUX_UART_SELECT_SHIFT)
#define CMU_MUX_UART_SELECT_OSCCLK			(0 << CMU_MUX_UART_SELECT_SHIFT)
#define CMU_MUX_UART_SELECT_WPLL_DIV12		(1 << CMU_MUX_UART_SELECT_SHIFT)

/* DIV_SFLASH ***************************************************************/
#define CMU_DIV_SFLASH_OVERRIDE_BY_HCH		(1 << 30)
#define CMU_DIV_SFLASH_AUTO_CLKGATING		(1 << 28)
#define CMU_DIV_SFLASH_BUSY					(1 << 16)

#define CMU_DIV_SFLASH_DIV_RATIO_SHIFT		0
#define CMU_DIV_SFLASH_DIV_RATIO_MASK		(0xf << CMU_DIV_SFLASH_DIV_RATIO_SHIFT)

/* DIV_SPI0 *****************************************************************/
#define CMU_DIV_SPI0_OVERRIDE_BY_HCH		(1 << 30)
#define CMU_DIV_SPI0_AUTO_CLKGATING			(1 << 28)
#define CMU_DIV_SPI0_BUSY					(1 << 16)

#define CMU_DIV_SPI0_DIV_RATIO_SHIFT		0
#define CMU_DIV_SPI0_DIV_RATIO_MASK			(0x7ff << CMU_DIV_SPI0_DIV_RATIO_SHIFT)

/* DIV_SPI1 *****************************************************************/
#define CMU_DIV_SPI1_OVERRIDE_BY_HCH		(1 << 30)
#define CMU_DIV_SPI1_AUTO_CLKGATING			(1 << 28)
#define CMU_DIV_SPI1_BUSY					(1 << 16)

#define CMU_DIV_SPI1_DIV_RATIO_SHIFT		0
#define CMU_DIV_SPI1_DIV_RATIO_MASK			(0x7ff << CMU_DIV_SPI1_DIV_RATIO_SHIFT)

/* DIV_SPI2 *****************************************************************/
#define CMU_DIV_SPI2_OVERRIDE_BY_HCH		(1 << 30)
#define CMU_DIV_SPI2_AUTO_CLKGATING			(1 << 28)
#define CMU_DIV_SPI2_BUSY					(1 << 16)

#define CMU_DIV_SPI2_DIV_RATIO_SHIFT		0
#define CMU_DIV_SPI2_DIV_RATIO_MASK			(0x7ff << CMU_DIV_SPI2_DIV_RATIO_SHIFT)

/* DIV_SPI3 *****************************************************************/
#define CMU_DIV_SPI3_OVERRIDE_BY_HCH		(1 << 30)
#define CMU_DIV_SPI3_AUTO_CLKGATING			(1 << 28)
#define CMU_DIV_SPI3_BUSY					(1 << 16)

#define CMU_DIV_SPI3_DIV_RATIO_SHIFT		0
#define CMU_DIV_SPI3_DIV_RATIO_MASK			(0x7ff << CMU_DIV_SPI3_DIV_RATIO_SHIFT)

/* DIV_WPLL_DIV12 ***********************************************************/
#define CMU_DIV_WPLL_DIV12_OVERRIDE_BY_HCH	(1 << 30)
#define CMU_DIV_WPLL_DIV12_AUTO_CLKGATING	(1 << 28)
#define CMU_DIV_WPLL_DIV12_BUSY				(1 << 16)

#define CMU_DIV_WPLL_DIV12_RATIO_SHIFT		0
#define CMU_DIV_WPLL_DIV12_RATIO_MASK		(0x7ff << CMU_DIV_WPLL_DIV12_RATIO_SHIFT)

/* DIV_WPLL_DIV3 ************************************************************/
#define CMU_DIV_WPLL_DIV3_OVERRIDE_BY_HCH	(1 << 30)
#define CMU_DIV_WPLL_DIV3_AUTO_CLKGATING	(1 << 28)
#define CMU_DIV_WPLL_DIV3_BUSY				(1 << 16)

#define CMU_DIV_WPLL_DIV3_RATIO_SHIFT		0
#define CMU_DIV_WPLL_DIV3_RATIO_MASK		(0xf << CMU_DIV_WPLL_DIV3_RATIO_SHIFT)

/* DIV_WPLL_DIV6 ************************************************************/
#define CMU_DIV_WPLL_DIV6_OVERRIDE_BY_HCH	(1 << 30)
#define CMU_DIV_WPLL_DIV6_AUTO_CLKGATING	(1 << 28)
#define CMU_DIV_WPLL_DIV6_BUSY				(1 << 16)

#define CMU_DIV_WPLL_DIV6_RATIO_SHIFT		0
#define CMU_DIV_WPLL_DIV6_RATIO_MASK		(0x1 << CMU_DIV_WPLL_DIV6_RATIO_SHIFT)

/* Clock Gating Control/Status for Clock Nodes ******************************/
#define CMU_GAT_MODE_SHIFT					20
#define CMU_GAT_MODE_MASK					(1 << CMU_GAT_MODE_SHIFT)
#define CMU_GAT_MODE_AUTO					(0 << CMU_GAT_MODE_SHIFT)
#define CMU_GAT_MODE_MANUAL					(1 << CMU_GAT_MODE_SHIFT)

#define CMU_GAT_CG_SHIFT					21
#define CMU_GAT_CG_MASK						(1 << CMU_GAT_CG_SHIFT)
#define CMU_GAT_CG_DISABLE					(0 << CMU_GAT_CG_SHIFT)
#define CMU_GAT_CG_ENABLE					(1 << CMU_GAT_CG_SHIFT)

/* PM Protocol Control/Status for ISO_CR4/QCH *******************************/
#define CMU_DMYQCH_CON_CR4_CLOCK_REQ		(1 << 1)
#define CMU_DMYQCH_CON_CR4_ENABLE			(1 << 0)

/* PM Protocol Control/Status for XIU_D_T20/QCH *****************************/
#define CMU_DMYQCH_CON_XIU_CLOCK_REQ		(1 << 1)
#define CMU_DMYQCH_CON_XIU_ENABLE			(1 << 0)

/* PM Protocol Control/Status for LHM_AXI_D_WIFI/QCH ************************/
#define CMU_QCH_CON_LHM_AXI_CLOCK_REQ		(1 << 1)
#define CMU_QCH_CON_LHM_AXI_ENABLE			(1 << 0)

/* PM Protocol Control/Status for LHM_DP/QCH ********************************/
#define CMU_QCH_CON_LHM_DP_CLOCK_REQ		(1 << 1)
#define CMU_QCH_CON_LHM_DP_ENABLE			(1 << 0)

/* PM Protocol Control/Status for LHS_DP/QCH ********************************/
#define CMU_QCH_CON_LHS_DP_CLOCK_REQ		(1 << 1)
#define CMU_QCH_CON_LHS_DP_ENABLE			(1 << 0)

/* PM Protocol Control/Status for MCU/QCH ***********************************/
#define CMU_QCH_CON_MCU_CLOCK_REQ			(1 << 1)
#define CMU_QCH_CON_MCU_ENABLE				(1 << 0)

#endif /* _ARCH_ARM_SRC_S5J_CHIP_S5JT200_CLOCK_H */
