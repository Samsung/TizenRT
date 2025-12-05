/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "image.h"

#include "region_defs.h"
#include "tfm_plat_otp.h"

fih_int bl1_read_bl1_2_image(uint8_t *image)
{
    fih_int fih_rc;
    enum tfm_plat_err_t plat_err;

    plat_err = tfm_plat_otp_read(PLAT_OTP_ID_BL1_2_IMAGE, BL1_2_CODE_SIZE,
                                 image);
    fih_rc = fih_int_encode_zero_equality(plat_err);

    FIH_RET(fih_rc);
}
