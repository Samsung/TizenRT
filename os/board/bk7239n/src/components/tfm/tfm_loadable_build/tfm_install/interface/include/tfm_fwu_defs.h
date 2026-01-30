/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef TFM_FWU_BOOTLOADER_DEFS_H
#define TFM_FWU_BOOTLOADER_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Image type identities. When porting a specific bootloader to FWU partition,
 * the bootloader specific image types can be defined here.
 */
#define FWU_IMAGE_TYPE_NONSECURE        0x01U
#define FWU_IMAGE_TYPE_SECURE           0x02U
#define FWU_IMAGE_TYPE_FULL             0x03U

// Images for BL2 upgrade
#define FWU_IMAGE_TYPE_PRIMARY_MANIFEST    0x4U
#define FWU_IMAGE_TYPE_SECONDARY_MANIFEST  0x5U
#define FWU_IMAGE_TYPE_PRIMARY_BL2         0x6U
#define FWU_IMAGE_TYPE_SECONDARY_BL2       0x7U
#define FWU_IMAGE_TYPE_BL1_CONTROL         0x8U
#define FWU_IMAGE_TYPE_BOOT_FLAG           0x9U
#define FWU_IMAGE_TYPE_PARTITION_FLAG      0xAU
#define FWU_IMAGE_TYPE_PARTITION_B_FLAG    0xBU

#define FWU_IMAGE_TYPE_INVALID             0xFFU

#ifdef __cplusplus
}
#endif
#endif /* TFM_FWU_BOOTLOADER_DEFS_H */
