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
 * os/arch/arm/src/imxrt/chip/imxrt_lpi2c.h
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author: Ivan Ucherdzhiev <ivanucherdjiev@gmail.com>
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

#ifndef __ARCH_ARM_SRC_IMXRT_CHIP_IMXRT_LPI2C_H
#define __ARCH_ARM_SRC_IMXRT_CHIP_IMXRT_LPI2C_H

/********************************************************************************************
 * Included Files
 ********************************************************************************************/

#include <tinyara/config.h>

/********************************************************************************************
 * Pre-processor Definitions
 ********************************************************************************************/

/* Register offsets *************************************************************************/

#define IMXRT_LPI2C_VERID_OFFSET             0x0000	/* Version ID Register offset */
#define IMXRT_LPI2C_PARAM_OFFSET             0x0004	/* Parameter Register offset */
#define IMXRT_LPI2C_MCR_OFFSET               0x0010	/* Master Control Register offset */
#define IMXRT_LPI2C_MSR_OFFSET               0x0014	/* Master Status Register offset */
#define IMXRT_LPI2C_MIER_OFFSET              0x0018	/* Master Interrupt Enable Register offset */
#define IMXRT_LPI2C_MDER_OFFSET              0x001c	/* Master DMA Enable Register offset */
#define IMXRT_LPI2C_MCFGR0_OFFSET            0x0020	/* Master Config Register 0 offset */
#define IMXRT_LPI2C_MCFGR1_OFFSET            0x0024	/* Master Config Register 1 offset */
#define IMXRT_LPI2C_MCFGR2_OFFSET            0x0028	/* Master Config Register 2 offset */
#define IMXRT_LPI2C_MCFGR3_OFFSET            0x002c	/* Master Config Register 3 offset */
#define IMXRT_LPI2C_MDMR_OFFSET              0x0040	/* Master Data Match Register offset */
#define IMXRT_LPI2C_MCCR0_OFFSET             0x0048	/* Master Clock Configuration Register 0 offset */
#define IMXRT_LPI2C_MCCR1_OFFSET             0x0050	/* Master Clock Configuration Register 1 offset */
#define IMXRT_LPI2C_MFCR_OFFSET              0x0058	/* Master FIFO Control Register offset */
#define IMXRT_LPI2C_MFSR_OFFSET              0x005C	/* Master FIFO Status Register offset */
#define IMXRT_LPI2C_MTDR_OFFSET              0x0060	/* Master Transmit Data Register offset */
#define IMXRT_LPI2C_MRDR_OFFSET              0x0070	/* Master Receive Data Register offset */
#define IMXRT_LPI2C_SCR_OFFSET               0x0110	/* Slave Control Register offset */
#define IMXRT_LPI2C_SSR_OFFSET               0x0114	/* Slave Status Register offset */
#define IMXRT_LPI2C_SIER_OFFSET              0x0118	/* Slave Interrupt Enable Register offset */
#define IMXRT_LPI2C_SDER_OFFSET              0x011c	/* Slave DMA Enable Register offset */
#define IMXRT_LPI2C_SCFGR1_OFFSET            0x0124	/* Slave Config Register 1 offset */
#define IMXRT_LPI2C_SCFGR2_OFFSET            0x0128	/* Slave Config Register 2 offset */
#define IMXRT_LPI2C_SAMR_OFFSET              0x0140	/* Slave Address Match Register offset */
#define IMXRT_LPI2C_SASR_OFFSET              0x0150	/* Slave Address Status Register offset */
#define IMXRT_LPI2C_STAR_OFFSET              0x0154	/* Slave Transmit ACK Register offset */
#define IMXRT_LPI2C_STDR_OFFSET              0x0160	/* Slave Transmit Data Register offset */
#define IMXRT_LPI2C_SRDR_OFFSET              0x0170	/* Slave Receive Data Register offset */

/* Register addresses ***********************************************************************/

/* LPI2C1 Registers */

#define IMXRT_LPI2C1_VERID                  (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_VERID_OFFSET)	/* Version ID Register */
#define IMXRT_LPI2C1_PARAM                  (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_PARAM_OFFSET)	/* Parameter Register  */
#define IMXRT_LPI2C1_MCR                    (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MCR_OFFSET)	/* Master Control Register  */
#define IMXRT_LPI2C1_MSR                    (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MSR_OFFSET)	/* Master Status Register  */
#define IMXRT_LPI2C1_MIER                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MIER_OFFSET)	/* Master Interrupt Enable Register  */
#define IMXRT_LPI2C1_MDER                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MDER_OFFSET)	/* Master DMA Enable Register  */
#define IMXRT_LPI2C1_MCFGR0                 (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MCFGR0_OFFSET)	/* Master Config Register 0  */
#define IMXRT_LPI2C1_MCFGR1                 (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MCFGR1_OFFSET)	/* Master Config Register 1  */
#define IMXRT_LPI2C1_MCFGR2                 (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MCFGR2_OFFSET)	/* Master Config Register 2  */
#define IMXRT_LPI2C1_MCFGR3                 (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MCFGR3_OFFSET)	/* Master Config Register 3  */
#define IMXRT_LPI2C1_MDMR                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MDMR_OFFSET)	/* Master Data Match Register  */
#define IMXRT_LPI2C1_MCCR0                  (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MCCR0_OFFSET)	/* Master Clock Configuration Register 0  */
#define IMXRT_LPI2C1_MCCR1                  (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MCCR1_OFFSET)	/* Master Clock Configuration Register 1  */
#define IMXRT_LPI2C1_MFCR                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MFCR_OFFSET)	/* Master FIFO Control Register  */
#define IMXRT_LPI2C1_MFSR                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MFSR_OFFSET)	/* Master FIFO Status Register  */
#define IMXRT_LPI2C1_MTDR                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MTDR_OFFSET)	/* Master Transmit Data Register  */
#define IMXRT_LPI2C1_MRDR                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_MRDR_OFFSET)	/* Master Receive Data Register  */
#define IMXRT_LPI2C1_SCR                    (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_SCR_OFFSET)	/* Slave Control Register  */
#define IMXRT_LPI2C1_SSR                    (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_SSR_OFFSET)	/* Slave Status Register  */
#define IMXRT_LPI2C1_SIER                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_SIER_OFFSET)	/* Slave Interrupt Enable Register  */
#define IMXRT_LPI2C1_SDER                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_SDER_OFFSET)	/* Slave DMA Enable Register  */
#define IMXRT_LPI2C1_SCFGR1                 (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_SCFGR1_OFFSET)	/* Slave Config Register 1  */
#define IMXRT_LPI2C1_SCFGR2                 (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_SCFGR2_OFFSET)	/* Slave Config Register 2  */
#define IMXRT_LPI2C1_SAMR                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_SAMR_OFFSET)	/* Slave Address Match Register  */
#define IMXRT_LPI2C1_SASR                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_SASR_OFFSET)	/* Slave Address Status Register  */
#define IMXRT_LPI2C1_STAR                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_STAR_OFFSET)	/* Slave Transmit ACK Register  */
#define IMXRT_LPI2C1_STDR                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_STDR_OFFSET)	/* Slave Transmit Data Register  */
#define IMXRT_LPI2C1_SRDR                   (IMXRT_LPI2C1_BASE + IMXRT_LPI2C_SRDR_OFFSET)	/* Slave Receive Data Register  */

/* LPI2C2 Registers */

#define IMXRT_LPI2C2_VERID                  (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_VERID_OFFSET)	/* Version ID Register */
#define IMXRT_LPI2C2_PARAM                  (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_PARAM_OFFSET)	/* Parameter Register  */
#define IMXRT_LPI2C2_MCR                    (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MCR_OFFSET)	/* Master Control Register  */
#define IMXRT_LPI2C2_MSR                    (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MSR_OFFSET)	/* Master Status Register  */
#define IMXRT_LPI2C2_MIER                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MIER_OFFSET)	/* Master Interrupt Enable Register  */
#define IMXRT_LPI2C2_MDER                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MDER_OFFSET)	/* Master DMA Enable Register  */
#define IMXRT_LPI2C2_MCFGR0                 (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MCFGR0_OFFSET)	/* Master Config Register 0  */
#define IMXRT_LPI2C2_MCFGR1                 (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MCFGR1_OFFSET)	/* Master Config Register 1  */
#define IMXRT_LPI2C2_MCFGR2                 (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MCFGR2_OFFSET)	/* Master Config Register 2  */
#define IMXRT_LPI2C2_MCFGR3                 (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MCFGR3_OFFSET)	/* Master Config Register 3  */
#define IMXRT_LPI2C2_MDMR                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MDMR_OFFSET)	/* Master Data Match Register  */
#define IMXRT_LPI2C2_MCCR0                  (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MCCR0_OFFSET)	/* Master Clock Configuration Register 0  */
#define IMXRT_LPI2C2_MCCR1                  (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MCCR1_OFFSET)	/* Master Clock Configuration Register 1  */
#define IMXRT_LPI2C2_MFCR                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MFCR_OFFSET)	/* Master FIFO Control Register  */
#define IMXRT_LPI2C2_MFSR                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MFSR_OFFSET)	/* Master FIFO Status Register  */
#define IMXRT_LPI2C2_MTDR                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MTDR_OFFSET)	/* Master Transmit Data Register  */
#define IMXRT_LPI2C2_MRDR                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_MRDR_OFFSET)	/* Master Receive Data Register  */
#define IMXRT_LPI2C2_SCR                    (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_SCR_OFFSET)	/* Slave Control Register  */
#define IMXRT_LPI2C2_SSR                    (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_SSR_OFFSET)	/* Slave Status Register  */
#define IMXRT_LPI2C2_SIER                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_SIER_OFFSET)	/* Slave Interrupt Enable Register  */
#define IMXRT_LPI2C2_SDER                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_SDER_OFFSET)	/* Slave DMA Enable Register  */
#define IMXRT_LPI2C2_SCFGR1                 (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_SCFGR1_OFFSET)	/* Slave Config Register 1  */
#define IMXRT_LPI2C2_SCFGR2                 (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_SCFGR2_OFFSET)	/* Slave Config Register 2  */
#define IMXRT_LPI2C2_SAMR                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_SAMR_OFFSET)	/* Slave Address Match Register  */
#define IMXRT_LPI2C2_SASR                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_SASR_OFFSET)	/* Slave Address Status Register  */
#define IMXRT_LPI2C2_STAR                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_STAR_OFFSET)	/* Slave Transmit ACK Register  */
#define IMXRT_LPI2C2_STDR                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_STDR_OFFSET)	/* Slave Transmit Data Register  */
#define IMXRT_LPI2C2_SRDR                   (IMXRT_LPI2C2_BASE + IMXRT_LPI2C_SRDR_OFFSET)	/* Slave Receive Data Register  */

/* LPI2C3 Registers */

#define IMXRT_LPI2C3_VERID                  (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_VERID_OFFSET)	/* Version ID Register */
#define IMXRT_LPI2C3_PARAM                  (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_PARAM_OFFSET)	/* Parameter Register  */
#define IMXRT_LPI2C3_MCR                    (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MCR_OFFSET)	/* Master Control Register  */
#define IMXRT_LPI2C3_MSR                    (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MSR_OFFSET)	/* Master Status Register  */
#define IMXRT_LPI2C3_MIER                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MIER_OFFSET)	/* Master Interrupt Enable Register  */
#define IMXRT_LPI2C3_MDER                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MDER_OFFSET)	/* Master DMA Enable Register  */
#define IMXRT_LPI2C3_MCFGR0                 (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MCFGR0_OFFSET)	/* Master Config Register 0  */
#define IMXRT_LPI2C3_MCFGR1                 (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MCFGR1_OFFSET)	/* Master Config Register 1  */
#define IMXRT_LPI2C3_MCFGR2                 (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MCFGR2_OFFSET)	/* Master Config Register 2  */
#define IMXRT_LPI2C3_MCFGR3                 (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MCFGR3_OFFSET)	/* Master Config Register 3  */
#define IMXRT_LPI2C3_MDMR                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MDMR_OFFSET)	/* Master Data Match Register  */
#define IMXRT_LPI2C3_MCCR0                  (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MCCR0_OFFSET)	/* Master Clock Configuration Register 0  */
#define IMXRT_LPI2C3_MCCR1                  (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MCCR1_OFFSET)	/* Master Clock Configuration Register 1  */
#define IMXRT_LPI2C3_MFCR                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MFCR_OFFSET)	/* Master FIFO Control Register  */
#define IMXRT_LPI2C3_MFSR                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MFSR_OFFSET)	/* Master FIFO Status Register  */
#define IMXRT_LPI2C3_MTDR                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MTDR_OFFSET)	/* Master Transmit Data Register  */
#define IMXRT_LPI2C3_MRDR                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_MRDR_OFFSET)	/* Master Receive Data Register  */
#define IMXRT_LPI2C3_SCR                    (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_SCR_OFFSET)	/* Slave Control Register  */
#define IMXRT_LPI2C3_SSR                    (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_SSR_OFFSET)	/* Slave Status Register  */
#define IMXRT_LPI2C3_SIER                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_SIER_OFFSET)	/* Slave Interrupt Enable Register  */
#define IMXRT_LPI2C3_SDER                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_SDER_OFFSET)	/* Slave DMA Enable Register  */
#define IMXRT_LPI2C3_SCFGR1                 (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_SCFGR1_OFFSET)	/* Slave Config Register 1  */
#define IMXRT_LPI2C3_SCFGR2                 (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_SCFGR2_OFFSET)	/* Slave Config Register 2  */
#define IMXRT_LPI2C3_SAMR                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_SAMR_OFFSET)	/* Slave Address Match Register  */
#define IMXRT_LPI2C3_SASR                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_SASR_OFFSET)	/* Slave Address Status Register  */
#define IMXRT_LPI2C3_STAR                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_STAR_OFFSET)	/* Slave Transmit ACK Register  */
#define IMXRT_LPI2C3_STDR                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_STDR_OFFSET)	/* Slave Transmit Data Register  */
#define IMXRT_LPI2C3_SRDR                   (IMXRT_LPI2C3_BASE + IMXRT_LPI2C_SRDR_OFFSET)	/* Slave Receive Data Register  */

/* LPI2C4 Registers */

#define IMXRT_LPI2C4_VERID                  (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_VERID_OFFSET)	/* Version ID Register */
#define IMXRT_LPI2C4_PARAM                  (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_PARAM_OFFSET)	/* Parameter Register  */
#define IMXRT_LPI2C4_MCR                    (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MCR_OFFSET)	/* Master Control Register  */
#define IMXRT_LPI2C4_MSR                    (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MSR_OFFSET)	/* Master Status Register  */
#define IMXRT_LPI2C4_MIER                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MIER_OFFSET)	/* Master Interrupt Enable Register  */
#define IMXRT_LPI2C4_MDER                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MDER_OFFSET)	/* Master DMA Enable Register  */
#define IMXRT_LPI2C4_MCFGR0                 (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MCFGR0_OFFSET)	/* Master Config Register 0  */
#define IMXRT_LPI2C4_MCFGR1                 (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MCFGR1_OFFSET)	/* Master Config Register 1  */
#define IMXRT_LPI2C4_MCFGR2                 (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MCFGR2_OFFSET)	/* Master Config Register 2  */
#define IMXRT_LPI2C4_MCFGR3                 (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MCFGR3_OFFSET)	/* Master Config Register 3  */
#define IMXRT_LPI2C4_MDMR                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MDMR_OFFSET)	/* Master Data Match Register  */
#define IMXRT_LPI2C4_MCCR0                  (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MCCR0_OFFSET)	/* Master Clock Configuration Register 0  */
#define IMXRT_LPI2C4_MCCR1                  (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MCCR1_OFFSET)	/* Master Clock Configuration Register 1  */
#define IMXRT_LPI2C4_MFCR                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MFCR_OFFSET)	/* Master FIFO Control Register  */
#define IMXRT_LPI2C4_MFSR                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MFSR_OFFSET)	/* Master FIFO Status Register  */
#define IMXRT_LPI2C4_MTDR                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MTDR_OFFSET)	/* Master Transmit Data Register  */
#define IMXRT_LPI2C4_MRDR                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_MRDR_OFFSET)	/* Master Receive Data Register  */
#define IMXRT_LPI2C4_SCR                    (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_SCR_OFFSET)	/* Slave Control Register  */
#define IMXRT_LPI2C4_SSR                    (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_SSR_OFFSET)	/* Slave Status Register  */
#define IMXRT_LPI2C4_SIER                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_SIER_OFFSET)	/* Slave Interrupt Enable Register  */
#define IMXRT_LPI2C4_SDER                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_SDER_OFFSET)	/* Slave DMA Enable Register  */
#define IMXRT_LPI2C4_SCFGR1                 (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_SCFGR1_OFFSET)	/* Slave Config Register 1  */
#define IMXRT_LPI2C4_SCFGR2                 (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_SCFGR2_OFFSET)	/* Slave Config Register 2  */
#define IMXRT_LPI2C4_SAMR                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_SAMR_OFFSET)	/* Slave Address Match Register  */
#define IMXRT_LPI2C4_SASR                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_SASR_OFFSET)	/* Slave Address Status Register  */
#define IMXRT_LPI2C4_STAR                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_STAR_OFFSET)	/* Slave Transmit ACK Register  */
#define IMXRT_LPI2C4_STDR                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_STDR_OFFSET)	/* Slave Transmit Data Register  */
#define IMXRT_LPI2C4_SRDR                   (IMXRT_LPI2C4_BASE + IMXRT_LPI2C_SRDR_OFFSET)	/* Slave Receive Data Register  */


#endif							/* __ARCH_ARM_SRC_IMXRT_CHIP_IMXRT_LPI2C_H */
