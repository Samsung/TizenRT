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
#include "tfm_spm_log.h"
#include "tfm_memory_utils.h"

#include <string.h>

#define ASSEMBLY_AND_TEST_PROV_DATA_MAGIC 0xC0DEFEED
#define PSA_ROT_PROV_DATA_MAGIC           0xBEEFFEED

__PACKED_STRUCT tfm_assembly_and_test_provisioning_data_t {
    uint32_t magic;
    uint8_t huk[32];
};

__PACKED_STRUCT tfm_psa_rot_provisioning_data_t {
    uint32_t magic;
    uint8_t iak[32];
    uint32_t iak_len;
    uint32_t iak_type;
    uint8_t iak_id[32];

    uint8_t boot_seed[32];
    uint8_t implementation_id[32];
    uint8_t hw_version[32];
    uint8_t verification_service_url[32];
    uint8_t profile_definition[32];

    uint8_t entropy_seed[64];
};

#ifdef TFM_DUMMY_PROVISIONING
static const struct tfm_assembly_and_test_provisioning_data_t assembly_and_test_prov_data = {
    ASSEMBLY_AND_TEST_PROV_DATA_MAGIC,
    /* HUK */
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    },
};

static const struct tfm_psa_rot_provisioning_data_t psa_rot_prov_data = {
    PSA_ROT_PROV_DATA_MAGIC,
    /* IAK */
    {
        0xA9, 0xB4, 0x54, 0xB2, 0x6D, 0x6F, 0x90, 0xA4,
        0xEA, 0x31, 0x19, 0x35, 0x64, 0xCB, 0xA9, 0x1F,
        0xEC, 0x6F, 0x9A, 0x00, 0x2A, 0x7D, 0xC0, 0x50,
        0x4B, 0x92, 0xA1, 0x93, 0x71, 0x34, 0x58, 0x5F
    },
    /* IAK len */
    32,
#ifdef SYMMETRIC_INITIAL_ATTESTATION
    /* IAK type */
    PSA_ALG_HMAC(PSA_ALG_SHA_256),
#else
    /* IAK type */
    PSA_ECC_FAMILY_SECP_R1,
#endif /* SYMMETRIC_INITIAL_ATTESTATION */
    /* IAK id */
    "kid@trustedfirmware.example",
    /* boot seed */
    {
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
        0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
        0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
        0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    },
    /* implementation id */
    {
        0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
        0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB,
        0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
        0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
    },
    /* hw version */
    "0604565272829100",
    /* verification_service_url */
    "www.trustedfirmware.org",
    /* attestation_profile_definition */
    "PSA_IOT_PROFILE_1",
    /* Entropy seed */
    {
        0x12, 0x13, 0x23, 0x34, 0x0a, 0x05, 0x89, 0x78,
        0xa3, 0x66, 0x8c, 0x0d, 0x97, 0x55, 0x53, 0xca,
        0xb5, 0x76, 0x18, 0x62, 0x29, 0xc6, 0xb6, 0x79,
        0x75, 0xc8, 0x5a, 0x8d, 0x9e, 0x11, 0x8f, 0x85,
        0xde, 0xc4, 0x5f, 0x66, 0x21, 0x52, 0xf9, 0x39,
        0xd9, 0x77, 0x93, 0x28, 0xb0, 0x5e, 0x02, 0xfa,
        0x58, 0xb4, 0x16, 0xc8, 0x0f, 0x38, 0x91, 0xbb,
        0x28, 0x17, 0xcd, 0x8a, 0xc9, 0x53, 0x72, 0x66,
    },
};
#else
static struct tfm_assembly_and_test_provisioning_data_t assembly_and_test_prov_data;
static struct tfm_psa_rot_provisioning_data_t psa_rot_prov_data;
#endif /* TFM_DUMMY_PROVISIONING */

void tfm_plat_provisioning_check_for_dummy_keys(void)
{
    uint64_t iak_start;

    tfm_plat_otp_read(PLAT_OTP_ID_IAK, sizeof(iak_start), (uint8_t*)&iak_start);

    if(iak_start == 0xA4906F6DB254B4A9) {
        SPMLOG_ERRMSG("[WRN]\033[1;31m ");
        SPMLOG_ERRMSG("This device was provisioned with dummy keys. ");
        SPMLOG_ERRMSG("This device is \033[1;1mNOT SECURE");
        SPMLOG_ERRMSG("\033[0m\r\n");
    }

    tfm_memset(&iak_start, 0, sizeof(iak_start));
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
    uint32_t new_lcs;

    err = tfm_plat_otp_write(PLAT_OTP_ID_HUK, sizeof(assembly_and_test_prov_data.huk),
                             assembly_and_test_prov_data.huk);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    new_lcs = PLAT_OTP_LCS_PSA_ROT_PROVISIONING;
    err = tfm_plat_otp_write(PLAT_OTP_ID_LCS, sizeof(new_lcs),
                             (uint8_t*)&new_lcs);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    return err;
}

enum tfm_plat_err_t provision_psa_rot(void)
{
    enum tfm_plat_err_t err;
    uint32_t new_lcs;

    err = tfm_plat_otp_write(PLAT_OTP_ID_IAK,
                             sizeof(psa_rot_prov_data.iak),
                             psa_rot_prov_data.iak);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }
    err = tfm_plat_otp_write(PLAT_OTP_ID_IAK_LEN,
                             sizeof(psa_rot_prov_data.iak_len),
                             (uint8_t*)&psa_rot_prov_data.iak_len);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }
    err = tfm_plat_otp_write(PLAT_OTP_ID_IAK_TYPE,
                             sizeof(psa_rot_prov_data.iak_type),
                             (uint8_t*)&psa_rot_prov_data.iak_type);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

#ifdef ATTEST_INCLUDE_COSE_KEY_ID
    err = tfm_plat_otp_write(PLAT_OTP_ID_IAK_ID,
                             sizeof(psa_rot_prov_data.iak_id),
                             psa_rot_prov_data.iak_id);
    if (err != TFM_PLAT_ERR_SUCCESS && err != TFM_PLAT_ERR_UNSUPPORTED) {
        return err;
    }
#endif /* ATTEST_INCLUDE_COSE_KEY_ID */

    err = tfm_plat_otp_write(PLAT_OTP_ID_BOOT_SEED,
                             sizeof(psa_rot_prov_data.boot_seed),
                             psa_rot_prov_data.boot_seed);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }
    err = tfm_plat_otp_write(PLAT_OTP_ID_IMPLEMENTATION_ID,
                             sizeof(psa_rot_prov_data.implementation_id),
                             psa_rot_prov_data.implementation_id);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }
    err = tfm_plat_otp_write(PLAT_OTP_ID_HW_VERSION,
                             sizeof(psa_rot_prov_data.hw_version),
                             psa_rot_prov_data.hw_version);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }
    err = tfm_plat_otp_write(PLAT_OTP_ID_VERIFICATION_SERVICE_URL,
                             sizeof(psa_rot_prov_data.verification_service_url),
                             psa_rot_prov_data.verification_service_url);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }
    err = tfm_plat_otp_write(PLAT_OTP_ID_PROFILE_DEFINITION,
                             sizeof(psa_rot_prov_data.profile_definition),
                             psa_rot_prov_data.profile_definition);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = tfm_plat_otp_write(PLAT_OTP_ID_ENTROPY_SEED,
                             sizeof(psa_rot_prov_data.entropy_seed),
                             psa_rot_prov_data.entropy_seed);
    if (err != TFM_PLAT_ERR_SUCCESS && err != TFM_PLAT_ERR_UNSUPPORTED) {
        return err;
    }

    new_lcs = PLAT_OTP_LCS_SECURED;
    err = tfm_plat_otp_write(PLAT_OTP_ID_LCS,
                             sizeof(new_lcs),
                             (uint8_t*)&new_lcs);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

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

    SPMLOG_INFMSG("[INF] Beginning TF-M provisioning\r\n");


#ifdef TFM_DUMMY_PROVISIONING
    SPMLOG_ERRMSG("[WRN]\033[1;31m ");
    SPMLOG_ERRMSG("TFM_DUMMY_PROVISIONING is not suitable for production! ");
    SPMLOG_ERRMSG("This device is \033[1;1mNOT SECURE");
    SPMLOG_ERRMSG("\033[0m\r\n");
#endif /* TFM_DUMMY_PROVISIONING */

    if (lcs == PLAT_OTP_LCS_ASSEMBLY_AND_TEST) {
        if (assembly_and_test_prov_data.magic != ASSEMBLY_AND_TEST_PROV_DATA_MAGIC) {
            SPMLOG_ERRMSG("No valid ASSEMBLY_AND_TEST provisioning data found\r\n");
            return TFM_PLAT_ERR_INVALID_INPUT;
        }

        err = provision_assembly_and_test();
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }
    }

    err = tfm_plat_otp_read(PLAT_OTP_ID_LCS, sizeof(lcs), (uint8_t*)&lcs);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }
    if (lcs == PLAT_OTP_LCS_PSA_ROT_PROVISIONING) {
        if (psa_rot_prov_data.magic != PSA_ROT_PROV_DATA_MAGIC) {
            SPMLOG_ERRMSG("No valid PSA_ROT provisioning data found\r\n");
            return TFM_PLAT_ERR_INVALID_INPUT;
        }

        err = provision_psa_rot();
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return err;
        }
    }

    return TFM_PLAT_ERR_SUCCESS;
}
