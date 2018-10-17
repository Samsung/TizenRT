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

#ifndef __ARCH_ARM_SRC_SIDK_S5JT200_INCLUDE_S5JT200_PARTITIONS_H
#define __ARCH_ARM_SRC_SIDK_S5JT200_INCLUDE_S5JT200_PARTITIONS_H

/******************************************************************************
 * Included Files
 *****************************************************************************/

#include <tinyara/config.h>

/******************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

#define MTD_SFLASH_ADDR         (0x04000000)	// SFLASH PHY ADDR
#define MTD_SFLASH_SIZE         (8 * 1024 * 1024)	// 8MB Fixed Size
#define MTD_1K_SIZE             (1024)

#define MTD_SFLASH_ERASE_SIZE   (64 * 1024)	// 64K

/* Align flash partition size to erase size(multiple of erase size) */
#define MTD_FLASH_ALIGN_SIZE(size, align)  ((((size) + (align - 1)) / align) * align)

/* Note : All partition size must be aligned to MTD_SFLASH_ERASE_SIZE */
/* execept below partition's from BL1 -> OTA1 which are fixed for flashing binaries */

/* FLASH Partition Layout */
/*
  +---------------+
  | BL1           |
  | (16 KB)       |
  +---------------+
  | OTA0 (OS)     |
  | (2MB + 960KB) |
  +---------------+
  | LED F/W       |
  | (32KB)        |
  +---------------+
  | SSS F/W       |
  | (32 KB)       |
  +---------------+
  | WLAN F/W      |
  | (752 KB)      |
  +---------------+
  | SSS KEY       |
  | (256KB)       |
  +---------------+
  | OTA1(NEW OS)  |
  | (2MB)         |
  +---------------+
  | ROMFS         |
  | (512 KB)      |
  +---------------+
  | SMARTFS       |
  | (1MB + 320KB) |
  +---------------+
  | RAMDUMP       |
  | (64KB)        |
  +---------------+
  | COREDUMP      |
  | (64KB)        |
  +---------------+
  | NV            |
  | (32KB)        |
  +---------------+
  | BOOTPARAM     |
  | (32KB)        |
  +---------------+
*/

/********************* Partition Size's ***************************************/
#define MTD_BLK_MASTER_SIZE     MTD_SFLASH_SIZE
#define MTD_BLK_BOOTLDR_SIZE    (16 * MTD_1K_SIZE)
#define MTD_BLK_OTA0_SIZE       (3008 * MTD_1K_SIZE)
#define MTD_BLK_LEDFW_SIZE      (32 * MTD_1K_SIZE)
#define MTD_BLK_SSSFW_SIZE      (32 * MTD_1K_SIZE)
#define MTD_BLK_WLANFW_SIZE     (752 * MTD_1K_SIZE)
#define MTD_BLK_SSSKEY_SIZE     (256 * MTD_1K_SIZE)
#define MTD_BLK_OTA1_SIZE       (2608 * MTD_1K_SIZE)

/* Align above total partition size to multiple of erase size */
#define TOTAL_ALIGNED_SIZE      MTD_FLASH_ALIGN_SIZE((MTD_BLK_BOOTLDR_SIZE + \
									MTD_BLK_OTA0_SIZE + MTD_BLK_LEDFW_SIZE + \
									MTD_BLK_SSSFW_SIZE + MTD_BLK_WLANFW_SIZE + \
									MTD_BLK_SSSKEY_SIZE + MTD_BLK_OTA1_SIZE), \
									MTD_SFLASH_ERASE_SIZE)

#if defined(CONFIG_FS_ROMFS)
#define MTD_BLK_ROMFS_SIZE      MTD_FLASH_ALIGN_SIZE((512 * MTD_1K_SIZE), \
													 MTD_SFLASH_ERASE_SIZE)
#else
#define MTD_BLK_ROMFS_SIZE      MTD_FLASH_ALIGN_SIZE((0 * MTD_1K_SIZE), \
													 MTD_SFLASH_ERASE_SIZE)
#endif

#define MTD_BLK_RAMDUMP_SIZE    MTD_FLASH_ALIGN_SIZE((0 * MTD_1K_SIZE), \
													 MTD_SFLASH_ERASE_SIZE)
#if defined(CONFIG_BOARD_COREDUMP_FLASH)
#define MTD_BLK_COREDUMP_SIZE   MTD_FLASH_ALIGN_SIZE((64 * MTD_1K_SIZE), \
													 MTD_SFLASH_ERASE_SIZE)
#else
#define MTD_BLK_COREDUMP_SIZE   MTD_FLASH_ALIGN_SIZE((0 * MTD_1K_SIZE), \
													 MTD_SFLASH_ERASE_SIZE)
#endif
#define MTD_BLK_NV_SIZE         MTD_FLASH_ALIGN_SIZE((32 * MTD_1K_SIZE), \
													 (32 * MTD_1K_SIZE))
#define MTD_BLK_BOOTPARAM_SIZE  MTD_FLASH_ALIGN_SIZE((32 * MTD_1K_SIZE), \
													 (32 * MTD_1K_SIZE))

/* Note: Both NV and BOOTPARAM share the last 64KB partition */
/* Align both partition to 32K address */

/* For smartfs, use remainging size */
#define MTD_BLK_SMARTFS_SIZE    (MTD_SFLASH_SIZE - (TOTAL_ALIGNED_SIZE + \
									MTD_BLK_ROMFS_SIZE + MTD_BLK_RAMDUMP_SIZE + \
									MTD_BLK_COREDUMP_SIZE + MTD_BLK_NV_SIZE + \
									MTD_BLK_BOOTPARAM_SIZE))

/********************* End of Partition Size's  ***********************************/

/********************* Partition Start Offset's ***********************************/

/* Partition[0] : MASTER,       SIZE = 8MB */
#define MTD_BLK_MASTER_START    (0x0)

/* Partition[1] : BOOTLOADER,   SIZE = 16KB */
#define MTD_BLK_BOOTLDR_START   (0x0)

/* Partition[2] : OTA0 (OS),    SIZE = 2MB + 960KB*/
#define MTD_BLK_OTA0_START      (MTD_BLK_BOOTLDR_START + MTD_BLK_BOOTLDR_SIZE)

/* Partition[3] : LED FW,       SIZE = 32KB */
#define MTD_BLK_LEDFW_START     (MTD_BLK_OTA0_START + MTD_BLK_OTA0_SIZE)

/* Partition[4] : SSS FW,       SIZE = 32KB */
#define MTD_BLK_SSSFW_START     (MTD_BLK_LEDFW_START + MTD_BLK_LEDFW_SIZE)

/* Partition[5] : WLAN FW,      SIZE = 752KB */
#define MTD_BLK_WLANFW_START    (MTD_BLK_SSSFW_START + MTD_BLK_SSSFW_SIZE)

/* Partition[6] : SSS KEY,      SIZE = 256KB */
#define MTD_BLK_SSSKEY_START    (MTD_BLK_WLANFW_START + MTD_BLK_WLANFW_SIZE)

/* Partition[7] : OTA1 (OS),    SIZE = 2MB */
#define MTD_BLK_OTA1_START      (MTD_BLK_SSSKEY_START + MTD_BLK_SSSKEY_SIZE)

/* Need ERASE_SIZE aligned partitions from here */
/* Partition[8] : RESOURCE,     SIZE = 0K */
#define MTD_BLK_ROMFS_START     (MTD_BLK_BOOTLDR_START + TOTAL_ALIGNED_SIZE)

/* Partition[9] : SMAERFS,      SIZE = 1MB + 320K */
#define MTD_BLK_SMARTFS_START   (MTD_BLK_ROMFS_START + MTD_BLK_ROMFS_SIZE)

/* Partition[10] : RAMDUMP,     SIZE = 0K */
#define MTD_BLK_RAMDUMP_START   (MTD_BLK_SMARTFS_START + MTD_BLK_SMARTFS_SIZE)

/* Partition[11] : COREDUMP,    SIZE = 0K */
#define MTD_BLK_COREDUMP_START  (MTD_BLK_RAMDUMP_START + MTD_BLK_RAMDUMP_SIZE)

/* Partition[12] : NV,          SIZE = 32K */
#define MTD_BLK_NV_START        (MTD_BLK_COREDUMP_START + MTD_BLK_COREDUMP_SIZE)

/* Partition[13] : BOOTPARAM,   SIZE = 32K */
#define MTD_BLK_BOOTPARAM_START (MTD_BLK_NV_START + MTD_BLK_NV_SIZE)

/********************* End of Partition Start Offset's ****************************/

#endif							/* __ARCH_ARM_SRC_SIDK_S5JT200_INCLUDE_S5JT200_PARTITIONS_H */
