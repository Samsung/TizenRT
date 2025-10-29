/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
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

fih_int bl1_read_bl1_2_image(uint8_t *image)
{
    uint32_t flash_offset;
    fih_int fih_rc;

    fih_rc = fih_int_encode(FLASH_DEV_NAME.ReadData(BL1_2_IMAGE_FLASH_OFFSET,
                                                    image,
                                                    BL1_2_CODE_SIZE);

    FIH_RET(fih_rc);
}
