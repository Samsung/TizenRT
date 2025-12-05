// Copyright 2022-2025 Beken
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

#include "platform_irq.h"            /* IRQ numbers */
#include "soc/bk7239n/reg_base.h"

#define FLASH0_BASE_S                  SOC_GET_S_ADDR(SOC_FLASH_DATA_BASE)
#define FLASH0_BASE_NS                 SOC_GET_NS_ADDR(SOC_FLASH_DATA_BASE)
#define FLASH0_SIZE                    0x01000000  /* Max flash size 16 MB, the actual flash size is initialized in Flash_Initialize() */
#define FLASH0_SECTOR_SIZE             0x00001000  /* 4 kB */
#define FLASH0_PAGE_SIZE               0x00001000  /* 4 kB */
#define FLASH0_PROGRAM_UNIT            0x1         /* Minimum write size */

#define UART0_BASE_S                   SOC_GET_S_ADDR(SOC_UART0_REG_BASE)
#define UART0_BASE_NS                  SOC_GET_NS_ADDR(SOC_UART0_REG_BASE)
#define UART1_BASE_S                   SOC_GET_S_ADDR(SOC_UART1_REG_BASE)
#define UART1_BASE_NS                  SOC_GET_NS_ADDR(SOC_UART1_REG_BASE)
#define TIMER0_BASE_S                  SOC_GET_S_ADDR(SOC_TIMER0_REG_BASE)
#define TIMER0_BASE_NS                 SOC_GET_NS_ADDR(SOC_TIMER0_REG_BASE)

