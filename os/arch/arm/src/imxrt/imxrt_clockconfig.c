/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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
 * os/arch/arm/src/imxrt/imxrt_clockconfig.c
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author:  Janne Rosberg <janne@offcode.fi>
 *   Modified: Ivan Ucherdzhiev <ivanucherdjiev@gmail.com>
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include "up_arch.h"
#include <arch/board/board.h>
#include "chip/imxrt_ccm.h"
#include "chip/imxrt_dcdc.h"
#include "imxrt_clockconfig.h"
#include "chip/imxrt_memorymap.h"
#include "imxrt_clock.h"
#include "imxrt_iomuxc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Variables for BOARD_BootClockRUN configuration
 ******************************************************************************/
const clock_arm_pll_config_t armPllConfig_BOARD_BootClockRUN = {
    .loopDivider = 100, /* PLL loop divider, Fout = Fin * 50 */
    .src = 0,           /* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};
const clock_sys_pll_config_t sysPllConfig_BOARD_BootClockRUN = {
    .loopDivider = 1, /* PLL loop divider, Fout = Fin * ( 20 + loopDivider*2 + numerator / denominator ) */
    .numerator = 0,   /* 30 bit numerator of fractional loop divider */
    .denominator = 1, /* 30 bit denominator of fractional loop divider */
    .src = 0,         /* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};
const clock_usb_pll_config_t usb1PllConfig_BOARD_BootClockRUN = {
    .loopDivider = 0, /* PLL loop divider, Fout = Fin * 20 */
    .src = 0,         /* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: imxrt_clockconfig
 *
 * Description:
 *   Called to initialize the i.MXRT.  This does whatever setup is needed to
 *   put the SoC in a usable state.  This includes the initialization of
 *   clocking using the settings in board.h.
 *
 ****************************************************************************/

void imxrt_clockconfig(void)
{
	/* Don't change the current basic clock configuration if we are running
	 * from SDRAM.  In this case, some bootloader logic has already configured
	 * clocking and SDRAM.  We are pretty much committed to using things the
	 * way that the bootloader has left them.
	 */

#ifndef CONFIG_IMXRT_BOOT_SDRAM
	/* Init RTC OSC clock frequency. */
    imxrt_clock_setrtcxtalfreq(32768U);
    /* Enable 1MHz clock output. */
    XTALOSC24M->OSC_CONFIG2 |= XTALOSC24M_OSC_CONFIG2_ENABLE_1M_MASK;
    /* Use free 1MHz clock output. */
    XTALOSC24M->OSC_CONFIG2 &= ~XTALOSC24M_OSC_CONFIG2_MUX_1M_MASK;
    /* Set XTAL 24MHz clock frequency. */
    imxrt_clock_setxtalfreq(24000000U);
    /* Enable XTAL 24MHz clock source. */
    imxrt_clock_initexternalclk(0);
    /* Enable internal RC. */
    imxrt_clock_initrcosc24m();
    /* Switch clock source to external OSC. */
    imxrt_clock_switchosc(kCLOCK_XtalOsc);
    /* Set Oscillator ready counter value. */
    CCM->CCR = (CCM->CCR & (~CCM_CCR_OSCNT_MASK)) | CCM_CCR_OSCNT(127);
    /* Setting PeriphClk2Mux and PeriphMux to provide stable clock before PLLs are initialed */
    imxrt_clock_setmux(kCLOCK_PeriphClk2Mux, 1); /* Set PERIPH_CLK2 MUX to OSC */
    imxrt_clock_setmux(kCLOCK_PeriphMux, 1);     /* Set PERIPH_CLK MUX to PERIPH_CLK2 */
    /* Setting the VDD_SOC to 1.275V. It is necessary to config AHB to 600Mhz. */
    DCDC->REG3 = (DCDC->REG3 & (~DCDC_REG3_TRG_MASK)) | DCDC_REG3_TRG(0x13);
    /* Waiting for DCDC_STS_DC_OK bit is asserted */
    while (DCDC_REG0_STS_DC_OK_MASK != (DCDC_REG0_STS_DC_OK_MASK & DCDC->REG0)) {
    }
    /* Set AHB_PODF. */
    imxrt_clock_setdiv(kCLOCK_AhbDiv, 0);
    /* Disable IPG clock gate. */
    imxrt_clock_disableclock(kCLOCK_Adc1);
    imxrt_clock_disableclock(kCLOCK_Adc2);
    imxrt_clock_disableclock(kCLOCK_Xbar1);
    imxrt_clock_disableclock(kCLOCK_Xbar2);
    imxrt_clock_disableclock(kCLOCK_Xbar3);
    /* Set IPG_PODF. */
    imxrt_clock_setdiv(kCLOCK_IpgDiv, 3);
    /* Set ARM_PODF. */
    imxrt_clock_setdiv(kCLOCK_ArmDiv, 1);
    /* Disable PERCLK clock gate. */
    imxrt_clock_disableclock(kCLOCK_Gpt1);
    imxrt_clock_disableclock(kCLOCK_Gpt1S);
    imxrt_clock_disableclock(kCLOCK_Gpt2);
    imxrt_clock_disableclock(kCLOCK_Gpt2S);
    imxrt_clock_disableclock(kCLOCK_Pit);
    /* Set PERCLK_PODF. */
    imxrt_clock_setdiv(kCLOCK_PerclkDiv, 1);
    /* Set per clock source. */
    imxrt_clock_setmux(kCLOCK_PerclkMux, 0);
    /* Disable USDHC1 clock gate. */
    imxrt_clock_disableclock(kCLOCK_Usdhc1);
    /* Set USDHC1_PODF. */
    imxrt_clock_setdiv(kCLOCK_Usdhc1Div, 1);
    /* Set Usdhc1 clock source. */
    imxrt_clock_setmux(kCLOCK_Usdhc1Mux, 0);
    /* Disable USDHC2 clock gate. */
    imxrt_clock_disableclock(kCLOCK_Usdhc2);
    /* Set USDHC2_PODF. */
    imxrt_clock_setdiv(kCLOCK_Usdhc2Div, 1);
    /* Set Usdhc2 clock source. */
    imxrt_clock_setmux(kCLOCK_Usdhc2Mux, 0);
#ifndef SKIP_SYSCLK_INIT
    /* Disable Semc clock gate. */
    imxrt_clock_disableclock(kCLOCK_Semc);
    /* Set SEMC_PODF. */
    imxrt_clock_setdiv(kCLOCK_SemcDiv, 7);
    /* Set Semc alt clock source. */
    imxrt_clock_setmux(kCLOCK_SemcAltMux, 0);
    /* Set Semc clock source. */
    imxrt_clock_setmux(kCLOCK_SemcMux, 0);
#endif
    /* Disable CSI clock gate. */
    imxrt_clock_disableclock(kCLOCK_Csi);
    /* Set CSI_PODF. */
    imxrt_clock_setdiv(kCLOCK_CsiDiv, 1);
    /* Set Csi clock source. */
    imxrt_clock_setmux(kCLOCK_CsiMux, 0);
    /* Disable LPSPI clock gate. */
    imxrt_clock_disableclock(kCLOCK_Lpspi1);
    imxrt_clock_disableclock(kCLOCK_Lpspi2);
    imxrt_clock_disableclock(kCLOCK_Lpspi3);
    imxrt_clock_disableclock(kCLOCK_Lpspi4);
    /* Set LPSPI_PODF. */
    imxrt_clock_setdiv(kCLOCK_LpspiDiv, 4);
    /* Set Lpspi clock source. */
    imxrt_clock_setmux(kCLOCK_LpspiMux, 2);
    /* Disable TRACE clock gate. */
    imxrt_clock_disableclock(kCLOCK_Trace);
    /* Set TRACE_PODF. */
    imxrt_clock_setdiv(kCLOCK_TraceDiv, 2);
    /* Set Trace clock source. */
    imxrt_clock_setmux(kCLOCK_TraceMux, 2);
    /* Disable SAI1 clock gate. */
    imxrt_clock_disableclock(kCLOCK_Sai1);
    /* Set SAI1_CLK_PRED. */
    imxrt_clock_setdiv(kCLOCK_Sai1PreDiv, 3);
    /* Set SAI1_CLK_PODF. */
    imxrt_clock_setdiv(kCLOCK_Sai1Div, 1);
    /* Set Sai1 clock source. */
    imxrt_clock_setmux(kCLOCK_Sai1Mux, 0);
    /* Disable SAI2 clock gate. */
    imxrt_clock_disableclock(kCLOCK_Sai2);
    /* Set SAI2_CLK_PRED. */
    imxrt_clock_setdiv(kCLOCK_Sai2PreDiv, 3);
    /* Set SAI2_CLK_PODF. */
    imxrt_clock_setdiv(kCLOCK_Sai2Div, 1);
    /* Set Sai2 clock source. */
    imxrt_clock_setmux(kCLOCK_Sai2Mux, 0);
    /* Disable SAI3 clock gate. */
    imxrt_clock_disableclock(kCLOCK_Sai3);
    /* Set SAI3_CLK_PRED. */
    imxrt_clock_setdiv(kCLOCK_Sai3PreDiv, 3);
    /* Set SAI3_CLK_PODF. */
    imxrt_clock_setdiv(kCLOCK_Sai3Div, 1);
    /* Set Sai3 clock source. */
    imxrt_clock_setmux(kCLOCK_Sai3Mux, 0);
    /* Disable Lpi2c clock gate. */
    imxrt_clock_disableclock(kCLOCK_Lpi2c1);
    imxrt_clock_disableclock(kCLOCK_Lpi2c2);
    imxrt_clock_disableclock(kCLOCK_Lpi2c3);
    /* Set LPI2C_CLK_PODF. */
    imxrt_clock_setdiv(kCLOCK_Lpi2cDiv, 0);
    /* Set Lpi2c clock source. */
    imxrt_clock_setmux(kCLOCK_Lpi2cMux, 0);
    /* Disable CAN clock gate. */
    imxrt_clock_disableclock(kCLOCK_Can1);
    imxrt_clock_disableclock(kCLOCK_Can2);
    imxrt_clock_disableclock(kCLOCK_Can1S);
    imxrt_clock_disableclock(kCLOCK_Can2S);
    /* Set CAN_CLK_PODF. */
    imxrt_clock_setdiv(kCLOCK_CanDiv, 1);
    /* Set Can clock source. */
    imxrt_clock_setmux(kCLOCK_CanMux, 2);
    /* Disable UART clock gate. */
    imxrt_clock_disableclock(kCLOCK_Lpuart1);
    imxrt_clock_disableclock(kCLOCK_Lpuart2);
    imxrt_clock_disableclock(kCLOCK_Lpuart3);
    imxrt_clock_disableclock(kCLOCK_Lpuart4);
    imxrt_clock_disableclock(kCLOCK_Lpuart5);
    imxrt_clock_disableclock(kCLOCK_Lpuart6);
    imxrt_clock_disableclock(kCLOCK_Lpuart7);
    imxrt_clock_disableclock(kCLOCK_Lpuart8);
    /* Set UART_CLK_PODF. */
    imxrt_clock_setdiv(kCLOCK_UartDiv, 0);
    /* Set Uart clock source. */
    imxrt_clock_setmux(kCLOCK_UartMux, 0);
    /* Disable LCDIF clock gate. */
    imxrt_clock_disableclock(kCLOCK_LcdPixel);
    /* Set LCDIF_PRED. */
    imxrt_clock_setdiv(kCLOCK_LcdifPreDiv, 1);
    /* Set LCDIF_CLK_PODF. */
    imxrt_clock_setdiv(kCLOCK_LcdifDiv, 3);
    /* Set Lcdif pre clock source. */
    imxrt_clock_setmux(kCLOCK_LcdifPreMux, 5);
    /* Disable SPDIF clock gate. */
    imxrt_clock_disableclock(kCLOCK_Spdif);
    /* Set SPDIF0_CLK_PRED. */
    imxrt_clock_setdiv(kCLOCK_Spdif0PreDiv, 1);
    /* Set SPDIF0_CLK_PODF. */
    imxrt_clock_setdiv(kCLOCK_Spdif0Div, 7);
    /* Set Spdif clock source. */
    imxrt_clock_setmux(kCLOCK_SpdifMux, 3);
    /* Disable Flexio1 clock gate. */
    imxrt_clock_disableclock(kCLOCK_Flexio1);
    /* Set FLEXIO1_CLK_PRED. */
    imxrt_clock_setdiv(kCLOCK_Flexio1PreDiv, 1);
    /* Set FLEXIO1_CLK_PODF. */
    imxrt_clock_setdiv(kCLOCK_Flexio1Div, 7);
    /* Set Flexio1 clock source. */
    imxrt_clock_setmux(kCLOCK_Flexio1Mux, 3);
    /* Disable Flexio2 clock gate. */
    imxrt_clock_disableclock(kCLOCK_Flexio2);
    /* Set FLEXIO2_CLK_PRED. */
    imxrt_clock_setdiv(kCLOCK_Flexio2PreDiv, 1);
    /* Set FLEXIO2_CLK_PODF. */
    imxrt_clock_setdiv(kCLOCK_Flexio2Div, 7);
    /* Set Flexio2 clock source. */
    imxrt_clock_setmux(kCLOCK_Flexio2Mux, 3);
    /* Set Pll3 sw clock source. */
    imxrt_clock_setmux(kCLOCK_Pll3SwMux, 0);
    /* Set lvds1 clock source. */
    CCM_ANALOG->MISC1 =
        (CCM_ANALOG->MISC1 & (~CCM_ANALOG_MISC1_LVDS1_CLK_SEL_MASK)) | CCM_ANALOG_MISC1_LVDS1_CLK_SEL(0);
    /* Set clock out1 divider. */
    CCM->CCOSR = (CCM->CCOSR & (~CCM_CCOSR_CLKO1_DIV_MASK)) | CCM_CCOSR_CLKO1_DIV(0);
    /* Set clock out1 source. */
    CCM->CCOSR = (CCM->CCOSR & (~CCM_CCOSR_CLKO1_SEL_MASK)) | CCM_CCOSR_CLKO1_SEL(1);
    /* Set clock out2 divider. */
    CCM->CCOSR = (CCM->CCOSR & (~CCM_CCOSR_CLKO2_DIV_MASK)) | CCM_CCOSR_CLKO2_DIV(0);
    /* Set clock out2 source. */
    CCM->CCOSR = (CCM->CCOSR & (~CCM_CCOSR_CLKO2_SEL_MASK)) | CCM_CCOSR_CLKO2_SEL(18);
    /* Set clock out1 drives clock out1. */
    CCM->CCOSR &= ~CCM_CCOSR_CLK_OUT_SEL_MASK;
    /* Disable clock out1. */
    CCM->CCOSR &= ~CCM_CCOSR_CLKO1_EN_MASK;
    /* Disable clock out2. */
    CCM->CCOSR &= ~CCM_CCOSR_CLKO2_EN_MASK;
    /* Set SAI1 MCLK1 clock source. */
    imxrt_iomuxc_setsaimclkclocksource(IOMUXC_GPR, kIOMUXC_GPR_SAI1MClk1Sel, 0);
    /* Set SAI1 MCLK2 clock source. */
    imxrt_iomuxc_setsaimclkclocksource(IOMUXC_GPR, kIOMUXC_GPR_SAI1MClk2Sel, 0);
    /* Set SAI1 MCLK3 clock source. */
    imxrt_iomuxc_setsaimclkclocksource(IOMUXC_GPR, kIOMUXC_GPR_SAI1MClk3Sel, 0);
    /* Set SAI2 MCLK3 clock source. */
    imxrt_iomuxc_setsaimclkclocksource(IOMUXC_GPR, kIOMUXC_GPR_SAI2MClk3Sel, 0);
    /* Set SAI3 MCLK3 clock source. */
    imxrt_iomuxc_setsaimclkclocksource(IOMUXC_GPR, kIOMUXC_GPR_SAI3MClk3Sel, 0);
    /* Set MQS configuration. */
    imxrt_iomuxc_mqsconfig(IOMUXC_GPR, kIOMUXC_MqsPwmOverSampleRate32, 0);
    /* Set ENET Tx clock source. */
    imxrt_iomuxc_enablemode(IOMUXC_GPR, kIOMUXC_GPR_ENET1RefClkMode, false);
    /* Set GPT1 High frequency reference clock source. */
    IOMUXC_GPR->GPR5 &= ~IOMUXC_GPR_GPR5_VREF_1M_CLK_GPT1_MASK;
    /* Set GPT2 High frequency reference clock source. */
    IOMUXC_GPR->GPR5 &= ~IOMUXC_GPR_GPR5_VREF_1M_CLK_GPT2_MASK;
    /* Init ARM PLL. */
    imxrt_clock_initarmpll(&armPllConfig_BOARD_BootClockRUN);
/* In SDK projects, SDRAM (configured by SEMC) will be initialized in either debug script or dcd.
 * With this macro SKIP_SYSCLK_INIT, system pll (selected to be SEMC source clock in SDK projects) will be left
 * unchanged.
 * Note: If another clock source is selected for SEMC, user may want to avoid changing that clock as well.*/
#ifndef SKIP_SYSCLK_INIT
    /* Init System PLL. */
    imxrt_clock_initsyspll(&sysPllConfig_BOARD_BootClockRUN);
    /* Init System pfd0. */
    imxrt_clock_initsyspfd(kCLOCK_Pfd0, 27);
    /* Init System pfd1. */
    imxrt_clock_initsyspfd(kCLOCK_Pfd1, 16);
    /* Init System pfd2. */
    imxrt_clock_initsyspfd(kCLOCK_Pfd2, 24);
    /* Init System pfd3. */
    imxrt_clock_initsyspfd(kCLOCK_Pfd3, 16);
    /* Disable pfd offset. */
    CCM_ANALOG->PLL_SYS &= ~CCM_ANALOG_PLL_SYS_PFD_OFFSET_EN_MASK;
#endif
    /* DeInit Audio PLL. */
    imxrt_clock_deinitaudiopll();
    /* Bypass Audio PLL. */
    imxrt_clock_setpllbypass(CCM_ANALOG, kCLOCK_PllAudio, 1);
    /* Set divider for Audio PLL. */
    CCM_ANALOG->MISC2 &= ~CCM_ANALOG_MISC2_AUDIO_DIV_LSB_MASK;
    CCM_ANALOG->MISC2 &= ~CCM_ANALOG_MISC2_AUDIO_DIV_MSB_MASK;
    /* Enable Audio PLL output. */
    CCM_ANALOG->PLL_AUDIO |= CCM_ANALOG_PLL_AUDIO_ENABLE_MASK;
    /* DeInit Video PLL. */
    imxrt_clock_deinitvideopll();
    /* Bypass Video PLL. */
    CCM_ANALOG->PLL_VIDEO |= CCM_ANALOG_PLL_VIDEO_BYPASS_MASK;
    /* Set divider for Video PLL. */
    CCM_ANALOG->MISC2 = (CCM_ANALOG->MISC2 & (~CCM_ANALOG_MISC2_VIDEO_DIV_MASK)) | CCM_ANALOG_MISC2_VIDEO_DIV(0);
    /* Enable Video PLL output. */
    CCM_ANALOG->PLL_VIDEO |= CCM_ANALOG_PLL_VIDEO_ENABLE_MASK;
    /* DeInit Enet PLL. */
    imxrt_clock_deinitenetpll();
    /* Bypass Enet PLL. */
    imxrt_clock_setpllbypass(CCM_ANALOG, kCLOCK_PllEnet, 1);
    /* Set Enet output divider. */
    CCM_ANALOG->PLL_ENET =
        (CCM_ANALOG->PLL_ENET & (~CCM_ANALOG_PLL_ENET_DIV_SELECT_MASK)) | CCM_ANALOG_PLL_ENET_DIV_SELECT(1);
    /* Enable Enet output. */
    CCM_ANALOG->PLL_ENET |= CCM_ANALOG_PLL_ENET_ENABLE_MASK;
    /* Enable Enet25M output. */
    CCM_ANALOG->PLL_ENET |= CCM_ANALOG_PLL_ENET_ENET_25M_REF_EN_MASK;
    /* DeInit Usb2 PLL. */
    imxrt_clock_deinitusb2pll();
    /* Bypass Usb2 PLL. */
    imxrt_clock_setpllbypass(CCM_ANALOG, kCLOCK_PllUsb2, 1);
    /* Enable Usb2 PLL output. */
    CCM_ANALOG->PLL_USB2 |= CCM_ANALOG_PLL_USB2_ENABLE_MASK;
    /* Set preperiph clock source. */
    imxrt_clock_setmux(kCLOCK_PrePeriphMux, 3);
    /* Set periph clock source. */
    imxrt_clock_setmux(kCLOCK_PeriphMux, 0);
    /* Set PERIPH_CLK2_PODF. */
    imxrt_clock_setdiv(kCLOCK_PeriphClk2Div, 0);
    /* Set periph clock2 clock source. */
    imxrt_clock_setmux(kCLOCK_PeriphClk2Mux, 0);
#else
	uint32_t reg;

	/* Set clock mux and dividers */

	/* Set PERIPH_CLK2 MUX to OSC */

	reg = getreg32(IMXRT_CCM_CBCMR);
	reg &= ~CCM_CBCMR_PERIPH_CLK2_SEL_MASK;
	reg |= CCM_CBCMR_PERIPH_CLK2_SEL_OSC_CLK;
	putreg32(reg, IMXRT_CCM_CBCMR);

	/* Set PERIPH_CLK MUX to PERIPH_CLK2 */

	reg = getreg32(IMXRT_CCM_CBCDR);
	reg &= ~CCM_CBCDR_PERIPH_CLK_SEL_MASK;
	reg |= CCM_CBCDR_PERIPH_CLK_SEL(CCM_CBCDR_PERIPH_CLK_SEL_PERIPH_CLK2);
	putreg32(reg, IMXRT_CCM_CBCDR);

	/* Wait handshake */

	while ((getreg32(IMXRT_CCM_CDHIPR) & CCM_CDHIPR_PERIPH2_CLK_SEL_BUSY(1U)) != 0) {
	}

	/* Set Soc VDD */

	reg = getreg32(IMXRT_DCDC_REG3);
	reg &= ~(DCDC_REG3_TRG_MASK);
	reg |= DCDC_REG3_TRG(IMXRT_VDD_SOC);
	putreg32(reg, IMXRT_DCDC_REG3);

	/* Init Arm PLL1 */

	reg = CCM_ANALOG_PLL_ARM_DIV_SELECT(IMXRT_ARM_PLL_DIV_SELECT) | CCM_ANALOG_PLL_ARM_ENABLE(1U);
	putreg32(reg, IMXRT_CCM_ANALOG_PLL_ARM);
	while ((getreg32(IMXRT_CCM_ANALOG_PLL_ARM) & CCM_ANALOG_PLL_ARM_LOCK((1U))) == 0) {
	}

	/* Init Sys PLL2 */

	reg = CCM_ANALOG_PLL_SYS_DIV_SELECT(IMXRT_SYS_PLL_SELECT) | CCM_ANALOG_PLL_SYS_ENABLE(1U);
	putreg32(reg, IMXRT_CCM_ANALOG_PLL_SYS);
	while ((getreg32(IMXRT_CCM_ANALOG_PLL_SYS) & CCM_ANALOG_PLL_SYS_LOCK(1U)) == 0) {
	}

	/* TODO: other pll configs */

	/* Set Dividers */

	reg = getreg32(IMXRT_CCM_CACRR);
	reg &= ~CCM_CACRR_ARM_PODF_MASK;
	reg |= CCM_CACRR_ARM_PODF(CCM_PODF_FROM_DIVISOR(IMXRT_ARM_PODF_DIVIDER));
	putreg32(reg, IMXRT_CCM_CACRR);

	reg = getreg32(IMXRT_CCM_CBCDR);
	reg &= ~CCM_CBCDR_AHB_PODF_MASK;
	reg |= CCM_CBCDR_AHB_PODF(CCM_PODF_FROM_DIVISOR(IMXRT_AHB_PODF_DIVIDER));
	putreg32(reg, IMXRT_CCM_CBCDR);

	reg = getreg32(IMXRT_CCM_CBCDR);
	reg &= ~CCM_CBCDR_IPG_PODF_MASK;
	reg |= CCM_CBCDR_IPG_PODF(CCM_PODF_FROM_DIVISOR(IMXRT_IPG_PODF_DIVIDER));
	putreg32(reg, IMXRT_CCM_CBCDR);

	reg = getreg32(IMXRT_CCM_CSCMR1);
	reg &= ~CCM_CSCMR1_PERCLK_PODF_MASK;
	reg |= CCM_CSCMR1_PERCLK_PODF(CCM_PODF_FROM_DIVISOR(IMXRT_PERCLK_PODF_DIVIDER));
	putreg32(reg, IMXRT_CCM_CSCMR1);

	reg = getreg32(IMXRT_CCM_CBCDR);
	reg &= ~CCM_CBCDR_SEMC_PODF_MASK;
	reg |= CCM_CBCDR_SEMC_PODF(CCM_PODF_FROM_DIVISOR(IMXRT_SEMC_PODF_DIVIDER));
	putreg32(reg, IMXRT_CCM_CBCDR);

  /* Set PRE_PERIPH_CLK to Board Selection */

	reg = getreg32(IMXRT_CCM_CBCMR);
	reg &= ~CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK;
	reg |= CCM_CBCMR_PRE_PERIPH_CLK_SEL(IMXRT_PRE_PERIPH_CLK_SEL);
	putreg32(reg, IMXRT_CCM_CBCMR);

  /* Set PERIPH_CLK MUX to Board Selection */

	reg = getreg32(IMXRT_CCM_CBCDR);
	reg &= ~CCM_CBCDR_PERIPH_CLK_SEL_MASK;
	reg |= CCM_CBCDR_PERIPH_CLK_SEL(IMXRT_PERIPH_CLK_SEL);
	putreg32(reg, IMXRT_CCM_CBCDR);

	/* Wait handshake */

	while ((getreg32(IMXRT_CCM_CDHIPR) & CCM_CDHIPR_PERIPH2_CLK_SEL_BUSY(1U)) != 0) {
	}

	/* Set PERCLK_CLK_SEL to Board Selection */

	reg = getreg32(IMXRT_CCM_CSCMR1);
	reg &= ~CCM_CSCMR1_PERCLK_CLK_SEL_MASK;
	reg |= CCM_CSCMR1_PERCLK_CLK_SEL(IMXRT_PERCLK_CLK_SEL);
	putreg32(reg, IMXRT_CCM_CSCMR1);

	/* Set UART source to PLL3 80M */

	reg = getreg32(IMXRT_CCM_CSCDR1);
	reg &= ~CCM_CSCDR1_UART_CLK_SEL(1U);
	reg |= CCM_CSCDR1_UART_CLK_SEL_PLL3_80;
	putreg32(reg, IMXRT_CCM_CSCDR1);

	/* Set UART divider to 1 */

	reg = getreg32(IMXRT_CCM_CSCDR1);
	reg &= ~CCM_CSCDR1_UART_CLK_PODF_MASK;
	reg |= CCM_CSCDR1_UART_CLK_PODF(0);
	putreg32(reg, IMXRT_CCM_CSCDR1);

#ifdef CONFIG_IMXRT_LPI2C
  /* Set LPI2C source to PLL3 60M */

	reg = getreg32(IMXRT_CCM_CSCDR2);
	reg &= ~CCM_CSCDR2_LPI2C_CLK_SEL(1U);
	reg |= CCM_CSCDR2_LPI2C_CLK_SEL_PLL3_60M;
	putreg32(reg, IMXRT_CCM_CSCDR2);

	while ((getreg32(IMXRT_CCM_CDHIPR) & CCM_CDHIPR_PERIPH_CLK_SEL_BUSY(1U)) != 0) {
	}

  /* Set LPI2C divider to 5  for 12 Mhz */

	reg = getreg32(IMXRT_CCM_CSCDR2);
	reg &= ~CCM_CSCDR2_LPI2C_CLK_PODF_MASK;
	reg |= CCM_CSCDR2_LPI2C_CLK_PODF(5);
	putreg32(reg, IMXRT_CCM_CSCDR2);

	while ((getreg32(IMXRT_CCM_CDHIPR) & CCM_CDHIPR_PERIPH_CLK_SEL_BUSY(1U)) != 0) {
	}
#endif

#ifdef CONFIG_IMXRT_LPSPI
  /* Set LPSPI clock source to PLL3 PFD0 */

  reg  = getreg32(IMXRT_CCM_CBCMR);
  reg &= ~CCM_CBCMR_LPSPI_CLK_SEL_MASK;
  reg |= CCM_CBCMR_LPSPI_CLK_SEL_PLL3_PFD0;
  putreg32(reg, IMXRT_CCM_CBCMR);

  /* Set LPSPI divider to IMXRT_LSPI_PODF_DIVIDER */

  reg  = getreg32(IMXRT_CCM_CBCMR);
  reg &= ~CCM_CBCMR_LPSPI_PODF_MASK;
  reg |= CCM_CBCMR_LPSPI_PODF(CCM_PODF_FROM_DIVISOR(IMXRT_LSPI_PODF_DIVIDER));
  putreg32(reg, IMXRT_CCM_CBCMR);
#endif

#ifdef CONFIG_IMXRT_USDHC
  /* Optionally set USDHC1 & 2 to generate clocks from IMXRT_USDHC1_CLK_SELECT */

  reg  = getreg32(IMXRT_CCM_CSCMR1);
  reg &= ~(CCM_CSCMR1_USDHC1_CLK_SEL | CCM_CSCMR1_USDHC2_CLK_SEL);
#if defined(IMXRT_USDHC1_CLK_SELECT)
  reg |= IMXRT_USDHC1_CLK_SELECT;
#endif
#if defined(IMXRT_USDHC2_CLK_SELECT)
  reg |= IMXRT_USDHC2_CLK_SELECT;
#endif
  putreg32(reg, IMXRT_CCM_CSCMR1);

  /* Now divide down clocks by IMXRT_USDHC[1|2]_PODF_DIVIDER */

  reg  = getreg32(IMXRT_CCM_CSCDR1);
  reg &= ~(CCM_CSCDR1_USDHC1_PODF_MASK | CCM_CSCDR1_USDHC2_PODF_MASK);
#if defined(IMXRT_USDHC1_PODF_DIVIDER)
  reg |= CCM_CSCDR1_USDHC1_PODF(CCM_PODF_FROM_DIVISOR(IMXRT_USDHC1_PODF_DIVIDER));
#endif
#if defined(IMXRT_USDHC2_PODF_DIVIDER)
  reg |= CCM_CSCDR1_USDHC2_PODF(CCM_PODF_FROM_DIVISOR(IMXRT_USDHC2_PODF_DIVIDER));
#endif
  putreg32(reg, IMXRT_CCM_CSCDR1);
#endif

#endif
}
