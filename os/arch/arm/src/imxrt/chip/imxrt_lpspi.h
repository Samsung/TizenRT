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
/********************************************************************************************
 * os/arch/arm/src/imxrt/chip/imxrt_lpspi.h
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author:  Pavlina Koleva <pavlinaikoleva19@gmail.com>
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
 ********************************************************************************************/

#ifndef __ARCH_ARM_SRC_IMXRT_CHIP_IMXRT_LPSPI_H
#define __ARCH_ARM_SRC_IMXRT_CHIP_IMXRT_LPSPI_H

/********************************************************************************************
 * Included Files
 ********************************************************************************************/

#include <tinyara/config.h>
#include "chip/imxrt_memorymap.h"

/********************************************************************************************
 * Pre-processor Definitions
 ********************************************************************************************/

/* Register offsets *************************************************************************/

#define IMXRT_LPSPI_VERID_OFFSET       0x0000  /* Version ID Register offset */
#define IMXRT_LPSPI_PARAM_OFFSET       0x0004  /* Parameter Register offset */
#define IMXRT_LPSPI_CR_OFFSET          0x0010  /* Control Register offset */
#define IMXRT_LPSPI_SR_OFFSET          0x0014  /* Status Register offset */
#define IMXRT_LPSPI_IER_OFFSET         0x0018  /* Interrupt Enable Register offset */
#define IMXRT_LPSPI_DER_OFFSET         0x001C  /* DMA Enable Register offset */
#define IMXRT_LPSPI_CFGR0_OFFSET       0x0020  /* Configuration Register 0 offset */
#define IMXRT_LPSPI_CFGR1_OFFSET       0x0024  /* Configuration Register 1 offset */
#define IMXRT_LPSPI_DMR0_OFFSET        0x0030  /* Data Match Register 0 offset */
#define IMXRT_LPSPI_DMR1_OFFSET        0x0034  /* Data Match Register 1 offset */
#define IMXRT_LPSPI_CCR_OFFSET         0x0040  /* Clock Configuration Register offset */
#define IMXRT_LPSPI_FCR_OFFSET         0x0058  /* FIFO Control Register offset */
#define IMXRT_LPSPI_FSR_OFFSET         0x005C  /* FIFO Status Register offset */
#define IMXRT_LPSPI_TCR_OFFSET         0x0060  /* Transmit Command Register offset */
#define IMXRT_LPSPI_TDR_OFFSET         0x0064  /* Transmit Data Register offset */
#define IMXRT_LPSPI_RSR_OFFSET         0x0070  /* Receive Status Register offset */
#define IMXRT_LPSPI_RDR_OFFSET         0x0074  /* Receive Data Register offset */

/* Register addresses ***********************************************************************/

#define IMXRT_LPSPI1_VERID             (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_VERID_OFFSET)
#define IMXRT_LPSPI1_PARAM             (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_PARAM_OFFSET)
#define IMXRT_LPSPI1_CR                (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_CR_OFFSET)
#define IMXRT_LPSPI1_SR                (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_SR_OFFSET)
#define IMXRT_LPSPI1_IER               (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_IER_OFFSET)
#define IMXRT_LPSPI1_DER               (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_DER_OFFSET)
#define IMXRT_LPSPI1_CFGR0             (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_CFGR0_OFFSET)
#define IMXRT_LPSPI1_CFGR1             (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_CFGR1_OFFSET)
#define IMXRT_LPSPI1_DMR0              (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_DMR0_OFFSET)
#define IMXRT_LPSPI1_DMR1              (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_DMR1_OFFSET)
#define IMXRT_LPSPI1_CCR               (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_CCR_OFFSET)
#define IMXRT_LPSPI1_FCR               (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_FCR_OFFSET)
#define IMXRT_LPSPI1_FSR               (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_FSR_OFFSET)
#define IMXRT_LPSPI1_TCR               (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_TCR_OFFSET)
#define IMXRT_LPSPI1_TDR               (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_TDR_OFFSET)
#define IMXRT_LPSPI1_RSR               (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_RSR_OFFSET)
#define IMXRT_LPSPI1_RDR               (IMXRT_LPSPI1_BASE + IMXRT_LPSPI_RDR_OFFSET)

#define IMXRT_LPSPI2_VERID             (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_VERID_OFFSET)
#define IMXRT_LPSPI2_PARAM             (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_PARAM_OFFSET)
#define IMXRT_LPSPI2_CR                (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_CR_OFFSET)
#define IMXRT_LPSPI2_SR                (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_SR_OFFSET)
#define IMXRT_LPSPI2_IER               (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_IER_OFFSET)
#define IMXRT_LPSPI2_DER               (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_DER_OFFSET)
#define IMXRT_LPSPI2_CFGR0             (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_CFGR0_OFFSET)
#define IMXRT_LPSPI2_CFGR1             (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_CFGR1_OFFSET)
#define IMXRT_LPSPI2_DMR0              (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_DMR0_OFFSET)
#define IMXRT_LPSPI2_DMR1              (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_DMR1_OFFSET)
#define IMXRT_LPSPI2_CCR               (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_CCR_OFFSET)
#define IMXRT_LPSPI2_FCR               (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_FCR_OFFSET)
#define IMXRT_LPSPI2_FSR               (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_FSR_OFFSET)
#define IMXRT_LPSPI2_TCR               (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_TCR_OFFSET)
#define IMXRT_LPSPI2_TDR               (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_TDR_OFFSET)
#define IMXRT_LPSPI2_RSR               (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_RSR_OFFSET)
#define IMXRT_LPSPI2_RDR               (IMXRT_LPSPI2_BASE + IMXRT_LPSPI_RDR_OFFSET)

#define IMXRT_LPSPI3_VERID             (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_VERID_OFFSET)
#define IMXRT_LPSPI3_PARAM             (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_PARAM_OFFSET)
#define IMXRT_LPSPI3_CR                (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_CR_OFFSET)
#define IMXRT_LPSPI3_SR                (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_SR_OFFSET)
#define IMXRT_LPSPI3_IER               (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_IER_OFFSET)
#define IMXRT_LPSPI3_DER               (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_DER_OFFSET)
#define IMXRT_LPSPI3_CFGR0             (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_CFGR0_OFFSET)
#define IMXRT_LPSPI3_CFGR1             (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_CFGR1_OFFSET)
#define IMXRT_LPSPI3_DMR0              (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_DMR0_OFFSET)
#define IMXRT_LPSPI3_DMR1              (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_DMR1_OFFSET)
#define IMXRT_LPSPI3_CCR               (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_CCR_OFFSET)
#define IMXRT_LPSPI3_FCR               (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_FCR_OFFSET)
#define IMXRT_LPSPI3_FSR               (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_FSR_OFFSET)
#define IMXRT_LPSPI3_TCR               (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_TCR_OFFSET)
#define IMXRT_LPSPI3_TDR               (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_TDR_OFFSET)
#define IMXRT_LPSPI3_RSR               (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_RSR_OFFSET)
#define IMXRT_LPSPI3_RDR               (IMXRT_LPSPI3_BASE + IMXRT_LPSPI_RDR_OFFSET)

#define IMXRT_LPSPI4_VERID             (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_VERID_OFFSET)
#define IMXRT_LPSPI4_PARAM             (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_PARAM_OFFSET)
#define IMXRT_LPSPI4_CR                (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_CR_OFFSET)
#define IMXRT_LPSPI4_SR                (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_SR_OFFSET)
#define IMXRT_LPSPI4_IER               (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_IER_OFFSET)
#define IMXRT_LPSPI4_DER               (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_DER_OFFSET)
#define IMXRT_LPSPI4_CFGR0             (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_CFGR0_OFFSET)
#define IMXRT_LPSPI4_CFGR1             (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_CFGR1_OFFSET)
#define IMXRT_LPSPI4_DMR0              (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_DMR0_OFFSET)
#define IMXRT_LPSPI4_DMR1              (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_DMR1_OFFSET)
#define IMXRT_LPSPI4_CCR               (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_CCR_OFFSET)
#define IMXRT_LPSPI4_FCR               (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_FCR_OFFSET)
#define IMXRT_LPSPI4_FSR               (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_FSR_OFFSET)
#define IMXRT_LPSPI4_TCR               (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_TCR_OFFSET)
#define IMXRT_LPSPI4_TDR               (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_TDR_OFFSET)
#define IMXRT_LPSPI4_RSR               (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_RSR_OFFSET)
#define IMXRT_LPSPI4_RDR               (IMXRT_LPSPI4_BASE + IMXRT_LPSPI_RDR_OFFSET)

#endif /* __ARCH_ARM_SRC_IMXRT_CHIP_IMXRT_LPSPI_H */
