/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "image.h"

#include "Driver_Flash.h"
#include "flash_layout.h"
#include "region_defs.h"
#include "fih.h"

extern ARM_DRIVER_FLASH FLASH_DEV_NAME;

int32_t __WEAK bl1_image_get_flash_offset(uint32_t image_id)
{
    switch (image_id) {
    case 0:
        return FLASH_AREA_0_OFFSET;
    case 1:
        return FLASH_AREA_1_OFFSET;
    default:
        FIH_PANIC;
    }
}

fih_int bl1_image_copy_to_sram(uint32_t image_id, uint8_t *out)
{
    uint32_t flash_offset;
    fih_int fih_rc;

    flash_offset = bl1_image_get_flash_offset(image_id);
    fih_rc = fih_int_encode(FLASH_DEV_NAME.ReadData(flash_offset, out,
                                                    sizeof(struct bl1_2_image_t)));

    FIH_RET(fih_rc);
}
