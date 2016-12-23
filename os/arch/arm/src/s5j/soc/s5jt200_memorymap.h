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
 * arch/arm/src/s5j/soc/s5jt200_memorymap.h
 *
 *   Copyright (C) 2016 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Reference:
 *   "i.MX 6Dual/6Quad ApplicationsProcessor Reference Manual," Document Number
 *   IMX6DQRM, Rev. 3, 07/2015, FreeScale.
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

#ifndef __ARCH_ARM_SRC_S5J_SOC_S5JT200_MEMORYMAP_H
#define __ARCH_ARM_SRC_S5J_SOC_S5JT200_MEMORYMAP_H

/* S5JT200 SFR BASE ADDRESS */
#define S5J_VECTOR_BASE   0xFFFF0000

#define EFUSE_WRITER      0x80000000
#define CHIPID_BASE       EFUSE_WRITER
#define MCT0_BASE         0x80010000
#define S5J_TICK_COUNTER  0x80020000
#define S5J_WDT_BASE      0x80030000
#define GPIO_CON_BASE     0x80040000

#define UARTDBG_BASE      0x80180000
#define UART0_BASE        0x80200000
#define UART1_BASE        0x80210000
#define UART2_BASE        0x80220000
#define UART3_BASE        0x80230000
#define OTP_CON_TOP_BASE  0x100D0000
#define SDIO_BASE         0x80300000
#define SFI_BASE          0x80310000

#define S5J_GIC_BASE      0x80400000
#define PMU_BASE          0x80090000
#define PMU_ALIVE_BASE    PMU_BASE	/*Temp */
#define PWM0_BASE         0x802E0000
#define PWM1_BASE         0x802F0000
#define S5J_SPI0_BASE     0x80240000
#define S5J_SPI1_BASE     0x80250000
#define S5J_SPI2_BASE     0x80260000
#define S5J_SPI3_BASE     0x80270000
#define S5J_TOP_RTC_BASE  0x80100000
#define S5J_RTC_BASE      S5J_TOP_RTC_BASE
#define S5J_PUF_BASE      0x80110000
#define S5J_CSSYS__BASE   0x801D0000
#define S5J_CSSYS_CR4_BASE    0x801E0000
#define S5J_CSSYS_PMU_BASE    0x801F0000
#define WDT_CPUCL_BASE        S5J_WDT_BASE	/*Temp */
#define S5J_MAILBOX_WIFI_BASE 0x80050000
#define S5J_MAILBOX_M0_BASE   0x80060000
#define S5J_CMU_SSS_BASE      0x80070000
#define S5J_CMU_BASE          0x80080000

#define S5J_SYSREG_BASE   0x800A0000
#define S5J_PDMA_BASE     0x800B0000
#define S5J_SSS_SS_BASE   0x800C0000
#define S5J_SSS_KM_BASE   0x800D0000
#define S5J_SSS_MB_BASE   0x800E0000
/* #define S5J_TEST_MEMORY  0x50000000 */
#define S5J_HSI2C0_BASE   0x80280000
#define S5J_HSI2C1_BASE   0x80290000
#define S5J_HSI2C2_BASE   0x802A0000
#define S5J_HSI2C3_BASE   0x802B0000
#define S5J_I2S_BASE      0x802C0000
#define S5J_ADC_BASE      0x802D0000

#define S5J_UART0_BASE    UART0_BASE
#define S5J_UART1_BASE    UART1_BASE
#define S5J_UART2_BASE    UART2_BASE
#define S5J_UART3_BASE    UART3_BASE

/* Physical address */
#if !defined(CONFIG_ARCH_ARMV8)
#define S5J_SECTION_MASK    0xFFF00000
#else
#define S5J_SECTION_MASK    0xFFE00000
#endif

#define S5J_MCT0_PSECTION       (MCT0_BASE & S5J_SECTION_MASK)
#define S5J_UARTDBG_PSECTION    (UARTDBG_BASE & S5J_SECTION_MASK)
#define S5J_UART0_PSECTION      (UART0_BASE & S5J_SECTION_MASK)
#define S5J_GPIO_CON_PSECTION   (GPIO_CON_BASE & S5J_SECTION_MASK)
#define S5J_GIC_PSECTION        (S5J_GIC_BASE & S5J_SECTION_MASK)
#define S5J_PMU_ALIVE_PSECTION  (PMU_ALIVE_BASE & S5J_SECTION_MASK)
#define S5J_PWM0_PSECTION       (PWM0_BASE & S5J_SECTION_MASK)
#define S5J_SPI0_PSECTION       (S5J_SPI0_BASE & S5J_SECTION_MASK)
#define S5J_SPI1_PSECTION       (S5J_SPI1_BASE & S5J_SECTION_MASK)
#define S5J_RTC_PSECTION        (S5J_RTC_BASE & S5J_SECTION_MASK)
#define S5J_TOP_RTC_PSECTION    (S5J_TOP_RTC_BASE & S5J_SECTION_MASK)
#define S5J_WDT_CPUCL_PSECTION  (WDT_CPUCL_BASE & S5J_SECTION_MASK)
/*#define S5J_MAILBOX_WLBT_PSECTION (MAILBOX_WLBT_BASE & S5J_SECTION_MASK)  */
/*#define S5J_CMU_MIF_PSECTION    (CMU_MIF_BASE & S5J_SECTION_MASK)  */
/*#define S5J_CMU_FSYS_PSECTION   (CMU_FSYS_BASE & S5J_SECTION_MASK) */
#define S5J_TEST_MEMORY_PSECTION (S5J_TEST_MEMORY & S5J_SECTION_MASK)
#define S5J_HSI2C0_PSECTION     (S5J_HSI2C0_BASE & S5J_SECTION_MASK)
#define S5J_HSI2C1_PSECTION     (S5J_HSI2C1_BASE & S5J_SECTION_MASK)

/* Virtual address */
#define S5J_MCT0_VSECTION       (MCT0_BASE & S5J_SECTION_MASK)
#define S5J_UARTDBG_VSECTION    (UARTDBG_BASE & S5J_SECTION_MASK)
#define S5J_UART0_VSECTION      (UART0_BASE & S5J_SECTION_MASK)
#define S5J_GPIO_CON_VSECTION   (GPIO_CON_BASE & S5J_SECTION_MASK)
#define S5J_GIC_VSECTION        (S5J_GIC_BASE & S5J_SECTION_MASK)
#define S5J_PMU_ALIVE_VSECTION  (PMU_ALIVE_BASE & S5J_SECTION_MASK)
#define S5J_PWM0_VSECTION       (PWM0_BASE & S5J_SECTION_MASK)
#define S5J_SPI0_VSECTION       (S5J_SPI0_BASE & S5J_SECTION_MASK)
#define S5J_SPI1_VSECTION       (S5J_SPI1_BASE & S5J_SECTION_MASK)
#define S5J_RTC_VSECTION        (S5J_RTC_BASE & S5J_SECTION_MASK)
#define S5J_TOP_RTC_VSECTION    (S5J_TOP_RTC_BASE & S5J_SECTION_MASK)
#define S5J_WDT_CPUCL_VSECTION  (WDT_CPUCL_BASE & S5J_SECTION_MASK)
/*#define S5J_MAILBOX_WLBT_VSECTION (MAILBOX_WLBT_BASE & S5J_SECTION_MASK) */
/*#define S5J_CMU_MIF_VSECTION    (CMU_MIF_BASE & S5J_SECTION_MASK) */
/*#define S5J_CMU_FSYS_VSECTION   (CMU_FSYS_BASE & S5J_SECTION_MASK) */
#define S5J_TEST_MEMORY_VSECTION (S5J_TEST_MEMORY & S5J_SECTION_MASK)
#define S5J_HSI2C0_VSECTION     (S5J_HSI2C0_BASE & S5J_SECTION_MASK)
#define S5J_HSI2C1_VSECTION     (S5J_HSI2C1_BASE & S5J_SECTION_MASK)

#define PGTABLE_BASE_PADDR 0x40000000
#define PGTABLE_BASE_VADDR 0x40000000

#define NUTTX_TEXT_VADDR       (CONFIG_RAM_VSTART & 0xfff00000)
#define NUTTX_TEXT_PADDR       (CONFIG_RAM_START & 0xfff00000)
#define NUTTX_TEXT_PEND        ((CONFIG_RAM_END + 0x000fffff) & 0xfff00000)
#define NUTTX_TEXT_SIZE        (NUTTX_TEXT_PEND - NUTTX_TEXT_PADDR)

#define NUTTX_RAM_SIZE    CONFIG_RAM_SIZE

#define NUTTX_RAM_PADDR   0x40000000
#define NUTTX_RAM_VADDR   0x40000000

#endif
