/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2017-2020 Linaro LTD
 * Copyright (c) 2017-2019 JUUL Labs
 * Copyright (c) 2019-2021 Arm Limited
 *
 * Original license:
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef H_BOOT_PARAM_H_
#define H_BOOT_PARAM_H_

#include <string.h>
#include "sysflash/sysflash.h"
#include <flash_map_backend/flash_map_backend.h>
#include "bootutil/bootutil.h"
#include "bootutil/image.h"
#include "bootutil/fault_injection_hardening.h"
#include "mcuboot_config/mcuboot_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Result values of returned from recover type. */
typedef enum {
	BP_DO_RECOVERY         = 1,
    BP_DO_RECOVERY_SUCCESS = 2,
	BP_DO_RECOVERY_FAIL    = 3,
}bp_recover_type_e;

typedef enum
{
    KERNEL_A_IMGAE_VERSION_IS_HIGN =0,
    KERNEL_B_IMGAE_VERSION_IS_HIGN,
    KERNEL_IMGAE_VERSION_IS_SAME,
}kernel_image_ver_e;

typedef enum
{
    INUSE_BP_IDX0 = 0,
    INUSE_BP_IDX1,
}bp_slot_e;

typedef enum
{
    ACTIVE_KERNEL_A =0,
    ACTIVE_KERNEL_B,
}active_kernel_e;

#define APP_NAME_LEN                (16)
#ifndef CONFIG_NUM_APPS
#define CONFIG_NUM_APPS             (1)
#endif
/* Bootparam information */
#define BOOT_PARAM_COUNT            (2)                          /* The number of boot parameters */
#define BOOT_PARAM_SIZE             (4096)                       /* The size of boot parameter : 4K */
#define BOOT_PARAM_PARTSIZE         (BOOT_PARAM_SIZE * 2)         /* The size of partition for dual boot parameters : 8K */
#define CHECKSUM_SIZE               (4)
#define NEED_DO_REBOOT_RETRY        (3)
#define NEED_DO_RECORVERY           (4)

/****************************************************************************
 * Public Types
 ****************************************************************************/
/* User bootparam data */
struct boot_userbp_s {
    char name[APP_NAME_LEN];
    uint8_t useidx;
};
typedef struct boot_userbp_s boot_userbp_t;

/* Boot parameter data structure */
struct boot_bpdata_s {
    uint32_t crc_hash;
    uint32_t version;
    uint32_t format_ver;
    uint8_t active_slot;
    uint32_t address[BOOT_PARAM_COUNT];
    uint8_t app_count;
    boot_userbp_t app_data[CONFIG_NUM_APPS + 1];
    uint8_t resource_active_idx;
} __attribute__((__packed__));
typedef struct boot_bpdata_s boot_bpdata_t;

/* Boot parameter info structure */
struct boot_bpinfo_s {
    int inuse_bp; //boot param slot index
    int part_num;
    uint8_t recover_flag;
    boot_bpdata_t bp_data[BOOT_PARAM_COUNT];
};
typedef struct boot_bpinfo_s boot_bpinfo_t;

/* kernel head info structure */
struct kernel_header_s {
	uint32_t crc_hash;
	uint16_t header_size;
	uint32_t version;
	uint32_t binary_size;
	uint16_t secure_header_size;
} __attribute__((__packed__));
typedef struct kernel_header_s kernel_header_t;

int boot_get_recovery_active_slot(boot_bpinfo_t *bp_info);
int boot_get_the_highest_bp_slot(boot_bpinfo_t *bp_info);
int boot_get_the_valid_active_slot(boot_bpinfo_t *bp_info);
int boot_update_the_active_slot(boot_bpinfo_t *bp_info);
int boot_update_bp_param_partition(boot_bpinfo_t *bp_info);

#ifdef __cplusplus
}
#endif

#endif
