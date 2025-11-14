/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "otp.h"

#include <stdint.h>

#include "region_defs.h"
#include "tfm_plat_otp.h"
#include "tfm_plat_nv_counters.h"
#include "util.h"

#ifdef TEST_BL1_1
extern uint8_t tfm_bl1_key_test_1_buf[];
extern uint8_t tfm_bl1_key_test_2_buf[];
#endif /* TEST_BL1_1 */

fih_int bl1_otp_read(uint8_t *dst, uint8_t *src, size_t size);
fih_int bl1_otp_write(uint8_t *dst, uint8_t *src, size_t size);

fih_int bl1_otp_init(void)
{
    fih_int fih_rc;
    enum tfm_plat_err_t plat_err;

    plat_err = tfm_plat_otp_init();
    fih_rc = fih_int_encode_zero_equality(plat_err);

    plat_err = tfm_plat_init_nv_counter();
    fih_rc = fih_int_encode_zero_equality(plat_err);

    return fih_rc;
}

fih_int bl1_otp_read_bl1_2_image_hash(uint8_t *hash)
{
    fih_int fih_rc;
    enum tfm_plat_err_t plat_err;

    plat_err = tfm_plat_otp_read(PLAT_OTP_ID_BL1_2_IMAGE_HASH, BL1_2_HASH_SIZE,
                                 hash);
    fih_rc = fih_int_encode_zero_equality(plat_err);

    FIH_RET(fih_rc);
}

fih_int bl1_otp_read_bl2_image_hash(uint8_t *hash)
{
    fih_int fih_rc;
    enum tfm_plat_err_t plat_err;

    plat_err = tfm_plat_otp_read(PLAT_OTP_ID_BL2_IMAGE_HASH, BL2_HASH_SIZE,
                                 hash);
    fih_rc = fih_int_encode_zero_equality(plat_err);

    FIH_RET(fih_rc);
}

fih_int bl1_otp_read_nv_counter(enum tfm_bl1_nv_counter_id_t counter_id,
                                uint32_t *count)
{
    fih_int fih_rc;
    enum tfm_plat_err_t plat_err;

    plat_err = tfm_plat_read_nv_counter(PLAT_NV_COUNTER_BL1_0,
                                        sizeof(uint32_t), (uint8_t *)count);
    fih_rc = fih_int_encode_zero_equality(plat_err);

    FIH_RET(fih_rc);
}

fih_int bl1_otp_write_nv_counter(enum tfm_bl1_nv_counter_id_t counter_id,
                                 uint32_t count)
{
    fih_int fih_rc;
    enum tfm_plat_err_t plat_err;

    plat_err = tfm_plat_set_nv_counter(PLAT_NV_COUNTER_BL1_0, count);
    fih_rc = fih_int_encode_zero_equality(plat_err);

    FIH_RET(fih_rc);
}

fih_int bl1_otp_read_key(enum tfm_bl1_key_id_t key_id, uint8_t *key_buf)
{
    fih_int fih_rc;
    enum tfm_plat_err_t plat_err;

    switch (key_id) {
    case TFM_BL1_KEY_HUK:
        plat_err = tfm_plat_otp_read(PLAT_OTP_ID_HUK, 32, key_buf);
        fih_rc = fih_int_encode_zero_equality(plat_err);
        break;
    case TFM_BL1_KEY_BL2_ENCRYPTION:
        plat_err = tfm_plat_otp_read(PLAT_OTP_ID_KEY_BL2_ENCRYPTION, 32,
                                     key_buf);
        fih_rc = fih_int_encode_zero_equality(plat_err);
        break;
#ifdef TEST_BL1_1
    case TFM_BL1_KEY_TEST_1:
        memcpy(key_buf, tfm_bl1_key_test_1_buf, 32);
        fih_rc = FIH_SUCCESS;
        break;
    case TFM_BL1_KEY_TEST_2:
        memcpy(key_buf, tfm_bl1_key_test_2_buf, 32);
        fih_rc = FIH_SUCCESS;
        break;
#endif /* TEST_BL1_1 */
    default:
        FIH_RET(FIH_FAILURE);
    }

    FIH_RET(fih_rc);
}
