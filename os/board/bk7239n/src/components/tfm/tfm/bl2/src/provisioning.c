/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_plat_provisioning.h"

#include "cmsis_compiler.h"
#include "tfm_plat_otp.h"
#include "tfm_attest_hal.h"
#include "psa/crypto.h"
#include "bootutil/bootutil_log.h"

#include <string.h>

#define ASSEMBLY_AND_TEST_PROV_DATA_MAGIC 0xC0DEFEED

__PACKED_STRUCT bl2_assembly_and_test_provisioning_data_t {
    uint32_t magic;
    uint8_t bl2_rotpk_0[32];
    uint8_t bl2_rotpk_1[32];
    uint8_t bl2_rotpk_2[32];

#ifdef PLATFORM_PSA_ADAC_SECURE_DEBUG
    uint8_t secure_debug_pk[32];
#endif /* PLATFORM_PSA_ADAC_SECURE_DEBUG */
};

#ifdef TFM_DUMMY_PROVISIONING
static const struct bl2_assembly_and_test_provisioning_data_t bl2_assembly_and_test_prov_data = {
    ASSEMBLY_AND_TEST_PROV_DATA_MAGIC,
#if (MCUBOOT_SIGN_RSA_LEN == 2048)
    /* bl2 rotpk 0 */
    {
        0xfc, 0x57, 0x01, 0xdc, 0x61, 0x35, 0xe1, 0x32,
        0x38, 0x47, 0xbd, 0xc4, 0x0f, 0x04, 0xd2, 0xe5,
        0xbe, 0xe5, 0x83, 0x3b, 0x23, 0xc2, 0x9f, 0x93,
        0x59, 0x3d, 0x00, 0x01, 0x8c, 0xfa, 0x99, 0x94,
    },
    /* bl2 rotpk 1 */
    {
        0xe1, 0x80, 0x15, 0x99, 0x3d, 0x6d, 0x27, 0x60,
        0xb4, 0x99, 0x27, 0x4b, 0xae, 0xf2, 0x64, 0xb8,
        0x3a, 0xf2, 0x29, 0xe9, 0xa7, 0x85, 0xf3, 0xd5,
        0xbf, 0x00, 0xb9, 0xd3, 0x2c, 0x1f, 0x03, 0x96,
    },
    /* bl2 rotpk 2 */
    {
        0xe1, 0x80, 0x15, 0x99, 0x3d, 0x6d, 0x27, 0x60,
        0xb4, 0x99, 0x27, 0x4b, 0xae, 0xf2, 0x64, 0xb8,
        0x3a, 0xf2, 0x29, 0xe9, 0xa7, 0x85, 0xf3, 0xd5,
        0xbf, 0x00, 0xb9, 0xd3, 0x2c, 0x1f, 0x03, 0x96,
    },
#elif (MCUBOOT_SIGN_RSA_LEN == 3072)
    /* bl2 rotpk 0 */
    {
        0xbf, 0xe6, 0xd8, 0x6f, 0x88, 0x26, 0xf4, 0xff,
        0x97, 0xfb, 0x96, 0xc4, 0xe6, 0xfb, 0xc4, 0x99,
        0x3e, 0x46, 0x19, 0xfc, 0x56, 0x5d, 0xa2, 0x6a,
        0xdf, 0x34, 0xc3, 0x29, 0x48, 0x9a, 0xdc, 0x38,
    },
    /* bl2 rotpk 1 */
    {
        0xb3, 0x60, 0xca, 0xf5, 0xc9, 0x8c, 0x6b, 0x94,
        0x2a, 0x48, 0x82, 0xfa, 0x9d, 0x48, 0x23, 0xef,
        0xb1, 0x66, 0xa9, 0xef, 0x6a, 0x6e, 0x4a, 0xa3,
        0x7c, 0x19, 0x19, 0xed, 0x1f, 0xcc, 0xc0, 0x49,
    },
    /* bl2 rotpk 2 */
    {
        0xb3, 0x60, 0xca, 0xf5, 0xc9, 0x8c, 0x6b, 0x94,
        0x2a, 0x48, 0x82, 0xfa, 0x9d, 0x48, 0x23, 0xef,
        0xb1, 0x66, 0xa9, 0xef, 0x6a, 0x6e, 0x4a, 0xa3,
        0x7c, 0x19, 0x19, 0xed, 0x1f, 0xcc, 0xc0, 0x49,
    },
#else
#error "No public key available for given signing algorithm."
#endif /* MCUBOOT_SIGN_RSA_LEN */

#ifdef PLATFORM_PSA_ADAC_SECURE_DEBUG
    {
        0xf4, 0x0c, 0x8f, 0xbf, 0x12, 0xdb, 0x78, 0x2a,
        0xfd, 0xf4, 0x75, 0x96, 0x6a, 0x06, 0x82, 0x36,
        0xe0, 0x32, 0xab, 0x80, 0xd1, 0xb7, 0xf1, 0xbc,
        0x9f, 0xe7, 0xd8, 0x7a, 0x88, 0xcb, 0x26, 0xd0,
    },
#endif /* PLATFORM_PSA_ADAC_SECURE_DEBUG */
};
#else
static const struct bl2_assembly_and_test_provisioning_data_t bl2_assembly_and_test_prov_data;
#endif /* TFM_DUMMY_PROVISIONING */

void tfm_plat_provisioning_check_for_dummy_keys(void)
{
    uint64_t iak_start;

    tfm_plat_otp_read(PLAT_OTP_ID_IAK, sizeof(iak_start), (uint8_t*)&iak_start);

    if(iak_start == 0xA4906F6DB254B4A9) {
        BOOT_LOG_WRN("%s%s%s%s",
                     "\033[1;31m",
                     "This device was provisioned with dummy keys. ",
                     "This device is \033[1;1mNOT SECURE",
                     "\033[0m");
    }

    memset(&iak_start, 0, sizeof(iak_start));
}

int tfm_plat_provisioning_is_required(void)
{
    enum tfm_plat_err_t err;
    enum plat_otp_lcs_t lcs;

    err = tfm_plat_otp_read(PLAT_OTP_ID_LCS, sizeof(lcs), (uint8_t*)&lcs);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    return lcs == PLAT_OTP_LCS_ASSEMBLY_AND_TEST
        || lcs == PLAT_OTP_LCS_PSA_ROT_PROVISIONING;
}

enum tfm_plat_err_t provision_assembly_and_test(void)
{
    enum tfm_plat_err_t err;

    err = tfm_plat_otp_write(PLAT_OTP_ID_BL2_ROTPK_0,
                             sizeof(bl2_assembly_and_test_prov_data.bl2_rotpk_0),
                             bl2_assembly_and_test_prov_data.bl2_rotpk_0);
    if (err != TFM_PLAT_ERR_SUCCESS && err != TFM_PLAT_ERR_UNSUPPORTED) {
        return err;
    }
    err = tfm_plat_otp_write(PLAT_OTP_ID_BL2_ROTPK_1,
                             sizeof(bl2_assembly_and_test_prov_data.bl2_rotpk_1),
                             bl2_assembly_and_test_prov_data.bl2_rotpk_1);
    if (err != TFM_PLAT_ERR_SUCCESS && err != TFM_PLAT_ERR_UNSUPPORTED) {
        return err;
    }

    err = tfm_plat_otp_write(PLAT_OTP_ID_BL2_ROTPK_2,
                             sizeof(bl2_assembly_and_test_prov_data.bl2_rotpk_2),
                             bl2_assembly_and_test_prov_data.bl2_rotpk_2);
    if (err != TFM_PLAT_ERR_SUCCESS && err != TFM_PLAT_ERR_UNSUPPORTED) {
        return err;
    }

#ifdef PLATFORM_PSA_ADAC_SECURE_DEBUG
    err = tfm_plat_otp_write(PLAT_OTP_ID_SECURE_DEBUG_PK,
                             sizeof(bl2_assembly_and_test_prov_data.secure_debug_pk),
                             bl2_assembly_and_test_prov_data.secure_debug_pk);
    if (err != TFM_PLAT_ERR_SUCCESS && err != TFM_PLAT_ERR_UNSUPPORTED) {
        return err;
    }
#endif /* PLATFORM_PSA_ADAC_SECURE_DEBUG */

    return err;
}

enum tfm_plat_err_t tfm_plat_provisioning_perform(void)
{
    enum tfm_plat_err_t err;
    enum plat_otp_lcs_t lcs;

    err = tfm_plat_otp_read(PLAT_OTP_ID_LCS, sizeof(lcs), (uint8_t*)&lcs);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    BOOT_LOG_INF("Beginning BL2 provisioning");

#ifdef TFM_DUMMY_PROVISIONING
    BOOT_LOG_WRN("%s%s%s%s",
                 "\033[1;31m",
                 "TFM_DUMMY_PROVISIONING is not suitable for production! ",
                 "This device is \033[1;1mNOT SECURE",
                 "\033[0m");
#endif /* TFM_DUMMY_PROVISIONING */

    if (lcs == PLAT_OTP_LCS_ASSEMBLY_AND_TEST) {
        if (bl2_assembly_and_test_prov_data.magic != ASSEMBLY_AND_TEST_PROV_DATA_MAGIC) {
            BOOT_LOG_ERR("No valid ASSEMBLY_AND_TEST provisioning data found");
            return TFM_PLAT_ERR_INVALID_INPUT;
        }

        err = provision_assembly_and_test();
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }
    }

    return TFM_PLAT_ERR_SUCCESS;
}
