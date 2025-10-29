/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "crypto.h"
#include "otp.h"
#include "tfm_plat_provisioning.h"
#include "boot_hal.h"
#include "region_defs.h"
#include "log.h"
#include "util.h"
#include "image.h"
#include "fih.h"

fih_int validate_image_at_addr(uint8_t *image)
{
    uint8_t computed_bl1_2_hash[BL1_2_HASH_SIZE];
    uint8_t stored_bl1_2_hash[BL1_2_HASH_SIZE];
    fih_int fih_rc = FIH_FAILURE;

    FIH_CALL(bl1_sha256_compute, fih_rc, image, BL1_2_CODE_SIZE,
                                     computed_bl1_2_hash);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(FIH_FAILURE);
    }

    FIH_CALL(bl1_otp_read_bl1_2_image_hash, fih_rc, stored_bl1_2_hash);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(FIH_FAILURE);
    }

    FIH_CALL(bl_secure_memeql, fih_rc, computed_bl1_2_hash,
                                       stored_bl1_2_hash, BL1_2_HASH_SIZE);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(FIH_FAILURE);
    }

    FIH_RET(FIH_SUCCESS);
}

int main(void)
{
    fih_int fih_rc = FIH_FAILURE;

    fih_rc = fih_int_encode_zero_equality(boot_platform_init());
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_PANIC;
    }
    BL1_LOG("[INF] Starting TF-M BL1_1\r\n");

    fih_rc = bl1_otp_init();

    if (tfm_plat_provisioning_is_required()) {
        if (tfm_plat_provisioning_perform()) {
            BL1_LOG("[ERR] BL1_2 provisioning failed\r\n");
            FIH_PANIC;
        }
    }

    tfm_plat_provisioning_check_for_dummy_keys();

    fih_rc = fih_int_encode_zero_equality(boot_platform_post_init());
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_PANIC;
    }

#ifdef TEST_BL1_1
    run_bl1_1_testsuite();
#endif /* TEST_BL1_1 */

    /* Copy BL1_2 from OTP into SRAM*/
    FIH_CALL(bl1_read_bl1_2_image, fih_rc, (uint8_t *)BL1_2_CODE_START);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_PANIC;
    }

    FIH_CALL(validate_image_at_addr, fih_rc, (uint8_t *)BL1_2_CODE_START);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        BL1_LOG("[ERR] BL1_2 image failed to validate\r\n");
        FIH_PANIC;
    }

    BL1_LOG("[INF] Jumping to BL1_2\r\n");
    /* Jump to BL1_2 */
    boot_platform_quit((struct boot_arm_vector_table *)BL1_2_CODE_START);

    /* This should never happen */
    FIH_PANIC;
}
