/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/chip/s5j200_system.h
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

#ifndef __ARCH_ARM_SRC_S5J_CHIP_S5JT200_SYSTEM_H
#define __ARCH_ARM_SRC_S5J_CHIP_S5JT200_SYSTEM_H

/****************************************************************************
 * Include
 ****************************************************************************/
#include "s5j_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* SYSTEM Register Address **************************************************/
#define S5J_SYSREG_LDO_CTRL				(S5J_SYSREG_BASE + 0x0008)
#define S5J_SYSREG_SFR_APB				(S5J_SYSREG_BASE + 0x0100)
#define S5J_SYSREG_COMPONENT_DRCG_EN	(S5J_SYSREG_BASE + 0x0200)
#define S5J_SYSREG_CR4_RF1_HS			(S5J_SYSREG_BASE + 0x0300)
#define S5J_SYSREG_CR4_RA1_HD			(S5J_SYSREG_BASE + 0x0304)
#define S5J_SYSREG_BLK_WIFI_RF1_HS		(S5J_SYSREG_BASE + 0x0308)
#define S5J_SYSREG_BLK_WIFI_RA1_HD		(S5J_SYSREG_BASE + 0x030C)
#define S5J_SYSREG_BLK_WIFI_RF2_UHD		(S5J_SYSREG_BASE + 0x0310)
#define S5J_SYSREG_BLK_WIFI_VROM_HD		(S5J_SYSREG_BASE + 0x0314)
#define S5J_SYSREG_SSS_RF1_HS			(S5J_SYSREG_BASE + 0x0318)
#define S5J_SYSREG_SSS_RA1_HD			(S5J_SYSREG_BASE + 0x031C)
#define S5J_SYSREG_SSS_RF2_UHD			(S5J_SYSREG_BASE + 0x0320)
#define S5J_SYSREG_SSS_VROM_HD			(S5J_SYSREG_BASE + 0x0324)
#define S5J_SYSREG_IntMEM_RA1_HD		(S5J_SYSREG_BASE + 0x0328)
#define S5J_SYSREG_IntMEM_VROM_HD		(S5J_SYSREG_BASE + 0x032C)
#define S5J_SYSREG_SDIO_RA2_HD			(S5J_SYSREG_BASE + 0x0330)
#define S5J_SYSREG_IRAM_SEC_CONTROL		(S5J_SYSREG_BASE + 0x0400)
#define S5J_SYSREG_WIFI2AP_ADDR			(S5J_SYSREG_BASE + 0x0404)
#define S5J_SYSREG_WIFI2AP_PERI			(S5J_SYSREG_BASE + 0x0430)
#define S5J_SYSREG_WIFI_IRAM			(S5J_SYSREG_BASE + 0x0434)
#define S5J_SYSREG_WIFI_MEM_BA0			(S5J_SYSREG_BASE + 0x0438)
#define S5J_SYSREG_WIFI_MEM_BA1			(S5J_SYSREG_BASE + 0x043C)
#define S5J_SYSREG_WIFI_MEM_SIZE		(S5J_SYSREG_BASE + 0x0440)
#define S5J_SYSREG_CR4_INT				(S5J_SYSREG_BASE + 0x0444)
#define S5J_SYSREG_PDMA_ADDR			(S5J_SYSREG_BASE + 0x0448)
#define S5J_SYSREG_PWM_INT				(S5J_SYSREG_BASE + 0x044C)
#define S5J_SYSREG_RCG_EN				(S5J_SYSREG_BASE + 0x0450)
#define S5J_SYSREG_UART_CONF			(S5J_SYSREG_BASE + 0x0454)
#define S5J_SYSREG_CM0P2AP_PERI			(S5J_SYSREG_BASE + 0x0530)
#define S5J_SYSREG_CM0P_IRAM			(S5J_SYSREG_BASE + 0x0534)
#define S5J_SYSREG_CM0P_MEM_BA0			(S5J_SYSREG_BASE + 0x0538)
#define S5J_SYSREG_CM0P_MEM_BA1			(S5J_SYSREG_BASE + 0x053C)
#define S5J_SYSREG_CM0P_MEM_BA0_SIZE	(S5J_SYSREG_BASE + 0x0504)
#define S5J_SYSREG_CM0P_MEM_BA1_SIZE	(S5J_SYSREG_BASE + 0x0540)
#define S5J_SYSREG_MCU_CFG				(S5J_SYSREG_BASE + 0x0544)
#define S5J_SYSREG_GPIO_INT_MASK		(S5J_SYSREG_BASE + 0x0548)
#define S5J_SYSREG_CM0P_PWRCTL			(S5J_SYSREG_BASE + 0x054C)
#define S5J_SYSREG_CR4_MON				(S5J_SYSREG_BASE + 0x0550)
#define S5J_SYSREG_OSC_CON				(S5J_SYSREG_BASE + 0x0554)

/* Register Bitfield Definitions ********************************************/
/* UART_CONF ****************************************************************/
#define SYSREG_UART_CONF_BIT_OFFSET(c)			((c) == 0 ? 0 : ((c) * 4))

#define SYSREG_UART_CONF_FILT_SHIFT(c)			SYSREG_UART_CONF_BIT_OFFSET(c)
#define SYSREG_UART_CONF_FILT_MASK(c)			(0x1 << SYSREG_UART_CONF_FILT_SHIFT(c))
#define SYSREG_UART_CONF_FILT_OFF(c)			(0x0 << SYSREG_UART_CONF_FILT_SHIFT(c))
#define SYSREG_UART_CONF_FILT_ON(c)				(0x1 << SYSREG_UART_CONF_FILT_SHIFT(c))

#define SYSREG_UART_CONF_FILT_CYCLE_SHIFT(c)	(SYSREG_UART_CONF_BIT_OFFSET(c) + 1)
#define SYSREG_UART_CONF_FILT_CYCLE_MASK(c)		(0x7 << SYSREG_UART_CONF_FILT_CYCLE_SHIFT(c))
#define SYSREG_UART_CONF_FILT_CYCLE_MAX(c)		(0x4 << SYSREG_UART_CONF_FILT_CYCLE_SHIFT(c))

#endif /* __ARCH_ARM_SRC_S5J_CHIP_S5JT200_SYSTEM_H */
