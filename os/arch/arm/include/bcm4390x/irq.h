/****************************************************************************
 *
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
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
#ifndef __ARCH_ARM_INCLUDE_BCM4390X_IRQ_H
#define __ARCH_ARM_INCLUDE_BCM4390X_IRQ_H

/******************************************************************************
 * Included Files
 *****************************************************************************/

#include <tinyara/config.h>
#include <arch/bcm4390x/chip.h>

/******************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

/* ARMCR4 Core Interrupts (vectors 0 - 31) */

#define BCM4390X_IRQ_CHIPCOMMON        0
#define BCM4390X_IRQ_ACPU_TIMER        1
#define BCM4390X_IRQ_CRYPTO            2
#define BCM4390X_IRQ_D11               3
#define BCM4390X_IRQ_DMA1              4
#define BCM4390X_IRQ_GMAC              5
#define BCM4390X_IRQ_I2S0              6
#define BCM4390X_IRQ_I2S1              7
#define BCM4390X_IRQ_SDIOD1            8
#define BCM4390X_IRQ_SDIOH             9
#define BCM4390X_IRQ_USB20D            10
#define BCM4390X_IRQ_UBS20H            11
#define BCM4390X_IRQ_DMA2              12
#define BCM4390X_IRQ_WCPU              13
#define BCM4390X_IRQ_SDIOD2            15
#define BCM4390X_IRQ_SWI0              16
#define BCM4390X_IRQ_SWI1              17
#define BCM4390X_IRQ_CLOCK_STABLE      29
#define BCM4390X_IRQ_PERF              30
#define BCM4390X_IRQ_BUS_ERROR         31

#define NR_VECTORS 32

/* ChipCommon Interrupts (vector 32 - 44) */

#define BCM4390X_IRQ_CHIPCOMMON_GPIO          (NR_VECTORS + 0)
#define BCM4390X_IRQ_CHIPCOMMON_EXT           (NR_VECTORS + 1)
#define BCM4390X_IRQ_CHIPCOMMON_ECIGCI        (NR_VECTORS + 4)
#define BCM4390X_IRQ_CHIPCOMMON_PMU           (NR_VECTORS + 5)
#define BCM4390X_IRQ_CHIPCOMMON_UART          (NR_VECTORS + 6)
#define BCM4390X_IRQ_CHIPCOMMON_SECIGCI       (NR_VECTORS + 7)
#define BCM4390X_IRQ_CHIPCOMMON_SPM           (NR_VECTORS + 8)
#define BCM4390X_IRQ_CHIPCOMMON_ASCU_RX       (NR_VECTORS + 9)
#define BCM4390X_IRQ_CHIPCOMMON_ASCU_TX       (NR_VECTORS + 10)
#define BCM4390X_IRQ_CHIPCOMMON_ASCU_ASTP     (NR_VECTORS + 11)
#define BCM4390X_IRQ_CHIPCOMMON_WATCHDOG      (NR_VECTORS + 12)

#define NR_IRQS (NR_VECTORS + 13)

/******************************************************************************
 * Public Types
 *****************************************************************************/

#ifndef __ASSEMBLY__

/******************************************************************************
 * Public Data
 *****************************************************************************/

#define EXTERN extern
#ifdef __cplusplus
#undef EXTERN
#define EXTERN extern "C"
extern "C" {
#endif

/******************************************************************************
 * Public Function Prototypes
 *****************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif
#endif							/* __ARCH_ARM_INCLUDE_BCM4390X_IRQ_H */
