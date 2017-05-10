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
 * arch/arm/src/s5j/chip/s5j_memorymap.h
 *
 *   Copyright (C) 2016 Gregory Nutt. All rights reserved.
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

#ifndef __ARCH_ARM_SRC_S5J_CHIP_S5J_MEMORYMAP_H
#define __ARCH_ARM_SRC_S5J_CHIP_S5J_MEMORYMAP_H

/* S5J Physical Memory Map */
#define S5J_IROM_PADDR			0x00000000	/* 0x00000000-0x0000FFFF iROM */
#define S5J_IRAM_PADDR			0x02020000	/* 0x02020000-0x0215FFFF iRAM */
#define S5J_IRAM_SHARED_PADDR	0x02300000	/* 0x02300000-0x0231FFFF iRAM shared */
#define S5J_FLASH_PADDR			0x04000000	/* 0x04000000-0x04FFFFFF NOR flash */
#define S5J_FLASH_MIRROR_PADDR	0x60000000	/* 0x60000000-0x60FFFFFF NOR flash (mirror of 0x04000000-0x04FFFFFF) */
#define S5J_PERIPHERAL_PADDR	0x80000000	/* 0x80000000-0x8FFFFFFF SFR region */
#define S5J_IRAM_MIRROR_PADDR	0xFFFF0000	/* 0xFFFF0000-0xFFFFFFFF iRAM (mirror of 0x02020000-0x0202FFFF) */

/* Size of memory regions in bytes */
#define S5J_IROM_SIZE			(64 * 1024)
#define S5J_IRAM_SIZE			(1280 * 1024)
#define S5J_IRAM_SHARED_SIZE	(128 * 1024)
#define S5J_FLASH_SIZE			(16 * 1024 * 1024)
#define S5J_FLASH_MIRROR_SIZE	S5J_FLASH_SIZE
#define S5J_PERIPHERAL_SIZE		(256 * 1024 * 1024)
#define S5J_IRAM_MIRROR_SIZE	(64 * 1024)

#define VECTOR_BASE				0xFFFF0000

/* Base Address of SFR at CR4 side */
#define S5J_CHIPID_BASE			0x80000000
#define S5J_MCT_BASE			0x80010000
#define S5J_TICK_COUNTER_BASE	0x80020000
#define S5J_WDT_BASE			0x80030000
#define S5J_GPIO_BASE			0x80040000
#define S5J_MAILBOX_WIFI_BASE	0x80050000
#define S5J_MAILBOX_M0_BASE		0x80060000
#define S5J_CMU_SSS_BASE		0x80070000
#define S5J_CMU_BASE			0x80080000
#define S5J_PMU_BASE			0x80090000
#define S5J_SYSREG_BASE			0x800A0000
#define S5J_PDMA_BASE			0x800B0000
#define S5J_SSS_SS_BASE			0x800C0000
#define S5J_SSS_KM_BASE			0x800D0000
#define S5J_SSS_MB_BASE			0x800E0000
#define S5J_RTC_BASE			0x80100000
#define S5J_PUF_BASE			0x80110000
#define S5J_UART4_BASE			0x80180000
#define S5J_CSSYS_BASE			0x801D0000
#define S5J_CSSYS_CR4_BASE		0x801E0000
#define S5J_CSSYS_PMU_BASE		0x801F0000
#define S5J_UART0_BASE			0x80200000
#define S5J_UART1_BASE			0x80210000
#define S5J_UART2_BASE			0x80220000
#define S5J_UART3_BASE			0x80230000
#define S5J_SPI0_BASE			0x80240000
#define S5J_SPI1_BASE			0x80250000
#define S5J_SPI2_BASE			0x80260000
#define S5J_SPI3_BASE			0x80270000
#define S5J_HSI2C0_BASE			0x80280000
#define S5J_HSI2C1_BASE			0x80290000
#define S5J_HSI2C2_BASE			0x802A0000
#define S5J_HSI2C3_BASE			0x802B0000
#define S5J_I2S_BASE			0x802C0000
#define S5J_ADC_BASE			0x802D0000
#define S5J_PWM0_BASE			0x802E0000
#define S5J_PWM1_BASE			0x802F0000
#define S5J_SDIO_BASE			0x80300000
#define S5J_SFLASH_BASE			0x80310000
#define S5J_GIC_BASE			0x80400000

#endif /* __ARCH_ARM_SRC_S5J_CHIP_S5J_MEMORYMAP_H */
