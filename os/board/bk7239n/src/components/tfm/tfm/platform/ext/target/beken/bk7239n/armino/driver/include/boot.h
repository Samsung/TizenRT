// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <common/sys_config.h>

#define BOOT_COLOR_UNUSED                 0x5A5A5A5A      //Pattern to fill UNUSED stack
#define BOOT_COLOR_SVC                    0x5A5A5A5A      //Pattern to fill SVC stack
#define BOOT_COLOR_IRQ                    0x5A5A5A5A      //Pattern to fill IRQ stack
#define BOOT_COLOR_FIQ                    0x5A5A5A5A      //Pattern to fill FIQ stack
#define BOOT_COLOR_SYS                    0x5A5A5A5A      //Pattern to fill SYS stack

/* Backup [R8~R14], CPSR and SPSR, totally 9 registers */
#define MCU_REG_BACKUP_NUM           9

/* From bottom to top, the registers are stored as:
 * bottom => CPSR, SPSR, R8, R9, R10, R11, R12, R13, R14 => top
 * SP(R13) is in offset 7
 * */
#define MCU_REG_BACKUP_SP_OFFSET     (7 << 2)
#define MCU_REG_BACKUP_ADDR_BASE     0x400020
#define MCU_REG_BACKUP_STACK_LEN     (MCU_REG_BACKUP_NUM << 2)

#define MCU_REG_BACKUP_BOTTOM_SYS    MCU_REG_BACKUP_ADDR_BASE
#define MCU_REG_BACKUP_TOP_SYS       (MCU_REG_BACKUP_BOTTOM_SYS + MCU_REG_BACKUP_STACK_LEN)
#define MCU_REG_BACKUP_SP_SYS        (MCU_REG_BACKUP_BOTTOM_SYS + MCU_REG_BACKUP_SP_OFFSET)

#define MCU_REG_BACKUP_BOTTOM_IRQ    MCU_REG_BACKUP_TOP_SYS
#define MCU_REG_BACKUP_TOP_IRQ       (MCU_REG_BACKUP_BOTTOM_IRQ + MCU_REG_BACKUP_STACK_LEN)
#define MCU_REG_BACKUP_SP_IRQ        (MCU_REG_BACKUP_BOTTOM_IRQ + MCU_REG_BACKUP_SP_OFFSET)

#define MCU_REG_BACKUP_BOTTOM_FIQ    MCU_REG_BACKUP_TOP_IRQ
#define MCU_REG_BACKUP_TOP_FIQ       (MCU_REG_BACKUP_BOTTOM_FIQ + MCU_REG_BACKUP_STACK_LEN)
#define MCU_REG_BACKUP_SP_FIQ        (MCU_REG_BACKUP_BOTTOM_FIQ + MCU_REG_BACKUP_SP_OFFSET)

#define MCU_REG_BACKUP_BOTTOM_ABT    MCU_REG_BACKUP_TOP_FIQ
#define MCU_REG_BACKUP_TOP_ABT       (MCU_REG_BACKUP_BOTTOM_ABT + MCU_REG_BACKUP_STACK_LEN)
#define MCU_REG_BACKUP_SP_ABT        (MCU_REG_BACKUP_BOTTOM_ABT + MCU_REG_BACKUP_SP_OFFSET)

#define MCU_REG_BACKUP_BOTTOM_UND    MCU_REG_BACKUP_TOP_ABT
#define MCU_REG_BACKUP_TOP_UND       (MCU_REG_BACKUP_BOTTOM_UND + MCU_REG_BACKUP_STACK_LEN)
#define MCU_REG_BACKUP_SP_UND        (MCU_REG_BACKUP_BOTTOM_UND + MCU_REG_BACKUP_SP_OFFSET)

#define MCU_REG_BACKUP_BOTTOM_SVC    MCU_REG_BACKUP_TOP_UND
#define MCU_REG_BACKUP_TOP_SVC       (MCU_REG_BACKUP_BOTTOM_SVC + MCU_REG_BACKUP_STACK_LEN)
#define MCU_REG_BACKUP_SP_SVC        (MCU_REG_BACKUP_BOTTOM_SVC + MCU_REG_BACKUP_SP_OFFSET)

#define FIQ_STACK_SIZE               0xFF0
#define IRQ_STACK_SIZE               0xFF0
#define SVC_STACK_SIZE               0x3F0
#define SYS_STACK_SIZE               0x100
#define UND_STACK_SIZE               0x280
#define ABT_STACK_SIZE               0x280

#ifdef __cplusplus
}
#endif
