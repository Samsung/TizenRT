/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "tfm_plat_otp.h"

#ifdef BL2

static enum tfm_plat_err_t get_rotpk_hash(enum tfm_otp_element_id_t id,
                                          uint8_t* rotpk_hash,
                                          uint32_t* rotpk_hash_size)
{
    enum tfm_plat_err_t err;
    size_t otp_size;

    err = tfm_plat_otp_read(id, *rotpk_hash_size, rotpk_hash);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = tfm_plat_otp_get_size(id, &otp_size);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    *rotpk_hash_size = otp_size;

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t
tfm_plat_get_rotpk_hash(uint8_t image_id,
                        uint8_t *rotpk_hash,
                        uint32_t *rotpk_hash_size)
{
    switch(image_id) {
        case 0:
            return get_rotpk_hash(PLAT_OTP_ID_BL2_ROTPK_0, rotpk_hash,
                                  rotpk_hash_size);
        case 1:
            return get_rotpk_hash(PLAT_OTP_ID_BL2_ROTPK_1, rotpk_hash,
                                  rotpk_hash_size);
        case 2:
            return get_rotpk_hash(PLAT_OTP_ID_BL2_ROTPK_2, rotpk_hash,
                                  rotpk_hash_size);
        default:
            return TFM_PLAT_ERR_INVALID_INPUT;
    }

    return TFM_PLAT_ERR_SYSTEM_ERR;
}

#endif /* BL2 */
