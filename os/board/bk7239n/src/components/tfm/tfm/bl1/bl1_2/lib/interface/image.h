/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef BL1_2_IMAGE_H
#define BL1_2_IMAGE_H

#include <stddef.h>
#include <stdint.h>
#include "crypto.h"
#include "region_defs.h"
#include "cmsis_compiler.h"
#include "fih.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BL1_2_IMAGE_DECRYPT_MAGIC_EXPECTED 0xDEADBEEF
#define PAD_SIZE (BL1_HEADER_SIZE - CTR_IV_LEN - 1292 - \
                  sizeof(struct tfm_bl1_image_version_t) - 2 * sizeof(uint32_t))

__PACKED_STRUCT tfm_bl1_image_version_t {
    uint8_t  major;
    uint8_t  minor;
    uint16_t revision;
    uint32_t build_num;
};

__PACKED_STRUCT bl1_2_image_t {
    __PACKED_STRUCT  {
        uint8_t ctr_iv[CTR_IV_LEN];
        uint8_t sig[1292];
    } header;
    __PACKED_STRUCT {
        struct tfm_bl1_image_version_t version;
        uint32_t security_counter;

        __PACKED_STRUCT {
            uint32_t decrypt_magic;
            uint8_t pad[PAD_SIZE];
            uint8_t data[IMAGE_BL2_CODE_SIZE];
        } encrypted_data;
    } protected_values;
};

int32_t bl1_image_get_flash_offset(uint32_t image_id);

fih_int bl1_image_copy_to_sram(uint32_t image_id, uint8_t *out);

#ifdef __cplusplus
}
#endif

#endif /* BL1_2_IMAGE_H */
