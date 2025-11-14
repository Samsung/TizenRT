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

#include "flash_layout.h"
#include "armino_config.h"

#define BL2_HEAP_SIZE           (0x0004000)
#define BL2_MSP_STACK_SIZE      (0x0004800)

#define S_HEAP_SIZE             CONFIG_TFM_HEAP_SIZE
#define S_MSP_STACK_SIZE_INIT   (0x0000400)
#define S_MSP_STACK_SIZE        (0x0000800)
#define S_PSP_STACK_SIZE        (0x0000800)

#define NS_HEAP_SIZE            (0x0001000)
#define NS_MSP_STACK_SIZE       (0x0000800)
#define NS_PSP_STACK_SIZE       (0x0004000)

/* This size of buffer is big enough to store an attestation
 * token produced by initial attestation service
 */
#define PSA_INITIAL_ATTEST_TOKEN_MAX_SIZE   (0x250)

#define S_IMAGE_PRIMARY_PARTITION_OFFSET   CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET
#define S_IMAGE_SECONDARY_PARTITION_OFFSET CONFIG_SECONDARY_TFM_S_PHY_PARTITION_OFFSET
#define NS_IMAGE_PRIMARY_PARTITION_OFFSET  CONFIG_PRIMARY_TFM_NS_PHY_PARTITION_OFFSET

/* Alias definitions for secure and non-secure areas*/
#define S_ROM_ALIAS(x)  (S_ROM_ALIAS_BASE + (x))
#define NS_ROM_ALIAS(x) (NS_ROM_ALIAS_BASE + (x))

#define S_RAM_ALIAS(x)  (S_RAM_ALIAS_BASE + (x))
#define NS_RAM_ALIAS(x) (NS_RAM_ALIAS_BASE + (x))

/* Secure regions */
#define S_CODE_START    S_ROM_ALIAS(CONFIG_PRIMARY_TFM_S_VIRTUAL_CODE_START)
#define S_CODE_SIZE     CONFIG_PRIMARY_TFM_S_VIRTUAL_CODE_SIZE
#define S_CODE_LIMIT    (S_CODE_START + S_CODE_SIZE - 1)

/* Size of vector table: 139 interrupt handlers + 4 bytes MPS initial value */
#define S_CODE_VECTOR_TABLE_SIZE    (0x230)

#define S_DATA_START    (S_RAM_ALIAS(0x0))

#if CONFIG_TFM_S_JUMP_TO_CPU0_APP
#define S_DATA_SIZE     CONFIG_TFM_RAM_SIZE
#else
#define S_DATA_SIZE     0x40000
#endif

#define S_DATA_LIMIT    (S_DATA_START + S_DATA_SIZE - 1)

/* Non-secure regions */
#if CONFIG_TFM_S_JUMP_TO_TFM_NS
#define NS_CODE_START   NS_ROM_ALIAS(CONFIG_PRIMARY_TFM_NS_VIRTUAL_CODE_START)
#define NS_CODE_SIZE    CONFIG_PRIMARY_TFM_NS_VIRTUAL_CODE_SIZE
#else
#define NS_CODE_START   NS_ROM_ALIAS(CONFIG_PRIMARY_CPU0_APP_VIRTUAL_CODE_START)
#define NS_CODE_SIZE    CONFIG_PRIMARY_CPU0_APP_VIRTUAL_CODE_SIZE
#endif
#define NS_CODE_LIMIT   (NS_CODE_START + NS_CODE_SIZE - 1)

#define NS_DATA_START   (NS_RAM_ALIAS(TOTAL_RAM_SIZE / 2))
#if defined(PSA_API_TEST_NS) && !defined(PSA_API_TEST_IPC)
#define DEV_APIS_TEST_NVMEM_REGION_SIZE  0x400
#define NS_DATA_SIZE    (TOTAL_RAM_SIZE / 2 - DEV_APIS_TEST_NVMEM_REGION_SIZE)
#else
#define NS_DATA_SIZE    (TOTAL_RAM_SIZE / 2) //TODO peter
#endif
#define NS_DATA_LIMIT   (NS_DATA_START + NS_DATA_SIZE - 1)

/* NS partition information is used for MPC and SAU configuration */
#define NS_PARTITION_START NS_CODE_START
#define NS_PARTITION_SIZE (FLASH_NS_PARTITION_SIZE)

/* Secondary partition for new images in case of firmware upgrade */
#define SECONDARY_PARTITION_START \
            (NS_ROM_ALIAS(S_IMAGE_SECONDARY_PARTITION_OFFSET))
#define SECONDARY_PARTITION_SIZE (FLASH_S_PARTITION_SIZE + \
                                  FLASH_NS_PARTITION_SIZE)

#ifndef CONFIG_BL2_B_VIRTUAL_CODE_START
#define CONFIG_BL2_B_VIRTUAL_CODE_START (0)
#endif

#ifdef BL2
/* Bootloader regions */
#define BL2_CODE_START    (0x02000000 + CONFIG_BL2_VIRTUAL_CODE_START)
#define BL2_B_CODE_START  (0x02000000 + CONFIG_BL2_B_VIRTUAL_CODE_START)
#define BL2_CODE_SIZE     CONFIG_BL2_VIRTUAL_CODE_SIZE
#define BL2_CODE_LIMIT    (BL2_CODE_START + BL2_CODE_SIZE - 1)

#define BL2_DATA_START    (S_RAM_ALIAS(0x0))
#define BL2_DATA_SIZE     (TOTAL_RAM_SIZE)
#define BL2_DATA_LIMIT    (BL2_DATA_START + BL2_DATA_SIZE - 1)
#endif /* BL2 */



/* Shared symbol area between bootloader and runtime firmware. Global variables
 * in the shared code can be placed here.
 */
#ifdef CODE_SHARING
#define SHARED_SYMBOL_AREA_BASE S_RAM_ALIAS_BASE
#define SHARED_SYMBOL_AREA_SIZE 0x20
#else
#define SHARED_SYMBOL_AREA_BASE S_RAM_ALIAS_BASE
#define SHARED_SYMBOL_AREA_SIZE 0x0
#endif /* CODE_SHARING */

/* Shared data area between bootloader and runtime firmware.
 * These areas are allocated at the beginning of the RAM, it is overlapping
 * with TF-M Secure code's MSP stack
 */
#define BOOT_TFM_SHARED_DATA_BASE (SHARED_SYMBOL_AREA_BASE + \
                                   SHARED_SYMBOL_AREA_SIZE)
#define BOOT_TFM_SHARED_DATA_SIZE (0x400)
#define BOOT_TFM_SHARED_DATA_LIMIT (BOOT_TFM_SHARED_DATA_BASE + \
                                    BOOT_TFM_SHARED_DATA_SIZE - 1)
