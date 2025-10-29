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

#include "partitions.h"

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_retarget.h to access flash related defines. To resolve this
 * some of the values are redefined here with different names, these are marked
 * with comment.
 */

/* Size of a Secure and of a Non-secure image */
#define FLASH_S_PARTITION_SIZE          CONFIG_PRIMARY_TFM_S_PHY_PARTITION_SIZE
#define FLASH_NS_PARTITION_SIZE         CONFIG_PRIMARY_TFM_NS_PHY_PARTITION_SIZE
#define FLASH_MAX_PARTITION_SIZE        CONFIG_PRIMARY_ALL_PHY_PARTITION_SIZE

/* Sector size of the flash hardware; same as FLASH0_SECTOR_SIZE */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x1000)     /* 4 KB */

/* Same as FLASH0_SIZE */
#define FLASH_TOTAL_SIZE                (0x00400000) /* 4 MB TODO peter*/

/* Flash layout info for BL2 bootloader */
#define FLASH_BASE_ADDRESS              (0x02000000)

#define FLASH_IMAGE_ALL_ID                 0

/* Per BL2 image define principles, each image upgraded by BL2 only have
 * one image ID, but two slots: primary and secondary.
 *
 * For manifest/bl2 images, they are not upgraded by BL2, but by BL1,
 * we need to define an image ID for each slot because the BL1 upgrade
 * is similar as A/B partition upgrade
 */
#define FLASH_IMAGE_PRIMARY_MANIFEST_ID    0x11
#define FLASH_IMAGE_SECONDARY_MANIFEST_ID  0x12
#define FLASH_IMAGE_PRIMARY_BL2_ID         0x13
#define FLASH_IMAGE_SECONDARY_BL2_ID       0x14

#define FLASH_IMAGE_PRIMARY_PARTITION_ID   0x15
#define FLASH_IMAGE_SECONDARY_PARTITON_ID  0x16

#define FLASH_AREA_PRIMARY_ALL_ID          (0)
 
#define FLASH_AREA_SECONDARY_ALL_ID        (1)

/* Secure image primary slot */
#define FLASH_AREA_0_ID                    (0)
 
/* Non-secure image primary slot */
#define FLASH_AREA_1_ID                    (FLASH_AREA_0_ID + 1)

/* Secure image secondary slot */
#define FLASH_AREA_2_ID                    (FLASH_AREA_1_ID + 1)
 
/* Non-secure image secondary slot */
#define FLASH_AREA_3_ID                    (FLASH_AREA_2_ID + 1)

/* Scratch area */
#define FLASH_AREA_SCRATCH_ID              (FLASH_AREA_SECONDARY_ALL_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET          (FLASH_AREA_3_OFFSET + FLASH_AREA_3_SIZE)
#define FLASH_AREA_SCRATCH_SIZE            (FLASH_MAX_PARTITION_SIZE)

/*ota area start*/
#define FLASH_AREA_PRIMARY_MANIFEST_ID     (FLASH_AREA_SCRATCH_ID + 1)
#define FLASH_AREA_SECONDARY_MANIFEST_ID   (FLASH_AREA_PRIMARY_MANIFEST_ID + 1)
#define FLASH_AREA_PRIMARY_BL2_ID          (FLASH_AREA_SECONDARY_MANIFEST_ID + 1)
#define FLASH_AREA_SECONDARY_BL2_ID        (FLASH_AREA_PRIMARY_BL2_ID + 1)
#define FLASH_AREA_PRIMARY_PARTITION_ID    (FLASH_AREA_SECONDARY_BL2_ID + 1)
#define FLASH_AREA_SECONDARY_PARTITION_ID  (FLASH_AREA_PRIMARY_PARTITION_ID + 1)
/*ota area end*/

/* The maximum number of status entries supported by the bootloader. */
#define MCUBOOT_STATUS_MAX_ENTRIES (FLASH_MAX_PARTITION_SIZE / FLASH_AREA_SCRATCH_SIZE)

/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS    (FLASH_MAX_PARTITION_SIZE / FLASH_AREA_IMAGE_SECTOR_SIZE)

/* Protected Storage (PS) Service definitions */
#define FLASH_PS_AREA_SIZE              CONFIG_SYS_PS_PHY_PARTITION_SIZE //TODO fix me

/* Internal Trusted Storage (ITS) Service definitions */
#define FLASH_ITS_AREA_SIZE             CONFIG_SYS_ITS_PHY_PARTITION_SIZE //TODO fix me

/* Offset and size definition in flash area used by assemble.py */
#define SECURE_IMAGE_OFFSET             0
#define SECURE_IMAGE_MAX_SIZE           FLASH_S_PARTITION_SIZE

#define NON_SECURE_IMAGE_OFFSET         (SECURE_IMAGE_OFFSET + SECURE_IMAGE_MAX_SIZE)
#define NON_SECURE_IMAGE_MAX_SIZE       FLASH_NS_PARTITION_SIZE

/* Flash device name used by BL2
 * Name is defined in flash driver file: Driver_Flash.c
 */
#define FLASH_DEV_NAME Driver_FLASH0
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_FLASH_PROGRAM_UNIT       (0x1)

/* Protected Storage (PS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M PS Integration Guide.
 */
#define TFM_HAL_PS_FLASH_DRIVER Driver_FLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
/* Size of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_SIZE    FLASH_PS_AREA_SIZE
#define PS_RAM_FS_SIZE                TFM_HAL_PS_FLASH_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_PS_SECTORS_PER_BLOCK  (1)
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_PS_PROGRAM_UNIT       (0x1)

/* Internal Trusted Storage (ITS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M ITS Integration Guide. The ITS should be in the internal flash, but is
 * allocated in the external flash just for development platforms that don't
 * have internal flash available.
 */
#define TFM_HAL_ITS_FLASH_DRIVER Driver_FLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
/* Size of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_SIZE    FLASH_ITS_AREA_SIZE
#define ITS_RAM_FS_SIZE                TFM_HAL_ITS_FLASH_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_ITS_SECTORS_PER_BLOCK  (1)
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_ITS_PROGRAM_UNIT       (0x1)

/* OTP / NV counter definitions */
#define TFM_OTP_NV_COUNTERS_SECTOR_SIZE FLASH_AREA_IMAGE_SECTOR_SIZE

//TODO peter prefer reg_base.h
/* Use SRAM1 memory to store Code data */
#define S_ROM_ALIAS_BASE  (0x02000000)
#define NS_ROM_ALIAS_BASE (0x12000000)

#define S_RAM_ALIAS_BASE  (0x28000000)
#define NS_RAM_ALIAS_BASE (0x38000000)

#define TOTAL_ROM_SIZE FLASH_TOTAL_SIZE
#define TOTAL_RAM_SIZE    (KB(512))

#define CONFIG_BL2_RAM_LOAD_ADDR               0x28040000

#define FLASH_AREA_IMAGE_PRIMARY(x)     (((x) == FLASH_IMAGE_ALL_ID) ? FLASH_AREA_PRIMARY_ALL_ID : 255 )


#define FLASH_AREA_IMAGE_SECONDARY(x)   (((x) == FLASH_IMAGE_ALL_ID) ? FLASH_AREA_SECONDARY_ALL_ID : \
					 (((x) == FLASH_IMAGE_PRIMARY_MANIFEST_ID) ? FLASH_AREA_PRIMARY_MANIFEST_ID : \
					 (((x) == FLASH_IMAGE_SECONDARY_MANIFEST_ID) ? FLASH_AREA_SECONDARY_MANIFEST_ID : \
					 (((x) == FLASH_IMAGE_PRIMARY_BL2_ID) ? FLASH_AREA_PRIMARY_BL2_ID : \
					 (((x) == FLASH_IMAGE_SECONDARY_BL2_ID) ? FLASH_AREA_SECONDARY_BL2_ID : \
					 (((x) == FLASH_IMAGE_PRIMARY_PARTITION_ID) ? FLASH_AREA_PRIMARY_PARTITION_ID : \
					 (((x) == FLASH_IMAGE_SECONDARY_PARTITON_ID) ? FLASH_AREA_SECONDARY_PARTITION_ID : 255 \
					  )))))))

#define FLASH_AREA_IMAGE_SCRATCH        FLASH_AREA_SCRATCH_ID


