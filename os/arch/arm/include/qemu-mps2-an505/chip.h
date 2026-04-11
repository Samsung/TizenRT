/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_INCLUDE_QEMU_MPS2_AN505_CHIP_H
#define __ARCH_ARM_INCLUDE_QEMU_MPS2_AN505_CHIP_H

#include <tinyara/config.h>

/* ARM AN505 exposes eight programmable priority levels on the Cortex-M33
 * NVIC, so only the upper three bits of each 8-bit priority field are used.
 */

#define __NVIC_PRIO_BITS             3

#define NVIC_SYSH_PRIORITY_MIN       0xe0
#define NVIC_SYSH_PRIORITY_DEFAULT   0x80
#define NVIC_SYSH_PRIORITY_MAX       0x00
#define NVIC_SYSH_PRIORITY_STEP      0x20

#define ARMV8M_PERIPHERAL_INTERRUPTS 124

#if defined(CONFIG_ARCH_HIPRI_INTERRUPT) && defined(CONFIG_ARCH_INT_DISABLEALL)
#define NVIC_SYSH_MAXNORMAL_PRIORITY  (NVIC_SYSH_PRIORITY_MAX + 2 * NVIC_SYSH_PRIORITY_STEP)
#define NVIC_SYSH_HIGH_PRIORITY       (NVIC_SYSH_PRIORITY_MAX + NVIC_SYSH_PRIORITY_STEP)
#define NVIC_SYSH_DISABLE_PRIORITY    NVIC_SYSH_HIGH_PRIORITY
#define NVIC_SYSH_SVCALL_PRIORITY     NVIC_SYSH_PRIORITY_MAX
#else
#define NVIC_SYSH_MAXNORMAL_PRIORITY  (NVIC_SYSH_PRIORITY_MAX + 2 * NVIC_SYSH_PRIORITY_STEP)
#define NVIC_SYSH_HIGH_PRIORITY       NVIC_SYSH_PRIORITY_MAX
#define NVIC_SYSH_DISABLE_PRIORITY    NVIC_SYSH_MAXNORMAL_PRIORITY
#define NVIC_SYSH_SVCALL_PRIORITY     NVIC_SYSH_PRIORITY_MAX
#endif

/* Peripheral aliases below use the non-secure AN505 view. The values come
 * from Arm Application Note AN505, Table 3-7 and Table 7-1.
 */

#define QEMU_MPS2_AN505_UART0_BASE   0x50200000
#define QEMU_MPS2_AN505_UART1_BASE   0x50201000
#define QEMU_MPS2_AN505_UART2_BASE   0x50202000
#define QEMU_MPS2_AN505_UART3_BASE   0x50203000
#define QEMU_MPS2_AN505_UART4_BASE   0x50204000
#define QEMU_MPS2_AN505_SCC_BASE     0x50300000
#define QEMU_MPS2_AN505_FPGAIO_BASE  0x50302000
#define QEMU_MPS2_AN505_FLASH_EXEC_BASE   0x10000000
#define QEMU_MPS2_AN505_FLASH_IMAGE_BASE  CONFIG_FLASH_START_ADDR

#define QEMU_MPS2_AN505_CPU_CLOCK    CONFIG_QEMU_MPS2_AN505_CPU_CLOCK
#define QEMU_MPS2_AN505_SYSTICK_CLK  CONFIG_QEMU_MPS2_AN505_CPU_CLOCK
#define QEMU_MPS2_AN505_UART_CLK     CONFIG_QEMU_MPS2_AN505_UART_CLOCK

#endif /* __ARCH_ARM_INCLUDE_QEMU_MPS2_AN505_CHIP_H */
