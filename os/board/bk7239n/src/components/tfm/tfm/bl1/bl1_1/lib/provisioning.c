/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_plat_provisioning.h"

#include "cmsis_compiler.h"
#include "tfm_plat_otp.h"
#include "tfm_attest_hal.h"
#include "psa/crypto.h"
#include "region_defs.h"
#include "log.h"

#include <string.h>

#define ASSEMBLY_AND_TEST_PROV_DATA_MAGIC 0xC0DEFEED

__PACKED_STRUCT bl1_assembly_and_test_provisioning_data_t {
    uint32_t magic;
    uint8_t bl2_encryption_key[32];
    uint8_t bl1_2_image_hash[32];
    uint8_t bl2_image_hash[32];
    uint8_t bl1_2_image[BL1_2_CODE_SIZE];
};

static const struct bl1_assembly_and_test_provisioning_data_t *bl1_assembly_and_test_prov_data =
                    (struct bl1_assembly_and_test_provisioning_data_t *)PROVISIONING_DATA_START;

void tfm_plat_provisioning_check_for_dummy_keys(void)
{
}

int tfm_plat_provisioning_is_required(void)
{
    enum tfm_plat_err_t err;
    enum plat_otp_lcs_t lcs;

    err = tfm_plat_otp_read(PLAT_OTP_ID_LCS, sizeof(lcs), (uint8_t *)&lcs);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    return lcs == PLAT_OTP_LCS_ASSEMBLY_AND_TEST
        || lcs == PLAT_OTP_LCS_PSA_ROT_PROVISIONING;
}

enum tfm_plat_err_t provision_assembly_and_test(void)
{
    enum tfm_plat_err_t err;

    err = tfm_plat_otp_write(PLAT_OTP_ID_KEY_BL2_ENCRYPTION,
                             sizeof(bl1_assembly_and_test_prov_data->bl2_encryption_key),
                             bl1_assembly_and_test_prov_data->bl2_encryption_key);
    if (err != TFM_PLAT_ERR_SUCCESS && err != TFM_PLAT_ERR_UNSUPPORTED) {
        return err;
    }

    err = tfm_plat_otp_write(PLAT_OTP_ID_BL1_2_IMAGE_HASH,
                             sizeof(bl1_assembly_and_test_prov_data->bl1_2_image_hash),
                             bl1_assembly_and_test_prov_data->bl1_2_image_hash);
    if (err != TFM_PLAT_ERR_SUCCESS && err != TFM_PLAT_ERR_UNSUPPORTED) {
        return err;
    }

    err = tfm_plat_otp_write(PLAT_OTP_ID_BL2_IMAGE_HASH,
                             sizeof(bl1_assembly_and_test_prov_data->bl2_image_hash),
                             bl1_assembly_and_test_prov_data->bl2_image_hash);
    if (err != TFM_PLAT_ERR_SUCCESS && err != TFM_PLAT_ERR_UNSUPPORTED) {
        return err;
    }

    err = tfm_plat_otp_write(PLAT_OTP_ID_BL1_2_IMAGE,
                             sizeof(bl1_assembly_and_test_prov_data->bl1_2_image),
                             bl1_assembly_and_test_prov_data->bl1_2_image);
    if (err != TFM_PLAT_ERR_SUCCESS && err != TFM_PLAT_ERR_UNSUPPORTED) {
        return err;
    }

    return err;
}

enum tfm_plat_err_t tfm_plat_provisioning_perform(void)
{
    enum tfm_plat_err_t err;
    enum plat_otp_lcs_t lcs;

    err = tfm_plat_otp_read(PLAT_OTP_ID_LCS, sizeof(lcs), (uint8_t *)&lcs);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    BL1_LOG("[INF] Beginning BL1 provisioning\r\n");

#ifdef TFM_DUMMY_PROVISIONING
    BL1_LOG("\033[1;31m[WRN] ");
    BL1_LOG("TFM_DUMMY_PROVISIONING is not suitable for production! ");
    BL1_LOG("This device is \033[1;1mNOT SECURE");
    BL1_LOG("\033[0m\r\n");
#endif /* TFM_DUMMY_PROVISIONING */

    if (lcs == PLAT_OTP_LCS_ASSEMBLY_AND_TEST) {
        if (bl1_assembly_and_test_prov_data->magic != ASSEMBLY_AND_TEST_PROV_DATA_MAGIC) {
            BL1_LOG("[ERR] No valid ASSEMBLY_AND_TEST provisioning data found\r\n");
            return TFM_PLAT_ERR_INVALID_INPUT;
        }

        err = provision_assembly_and_test();
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }
    }

    return TFM_PLAT_ERR_SUCCESS;
}
