/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_plat_crypto_nv_seed.h"
#include "tfm_plat_otp.h"
#include "psa/internal_trusted_storage.h"

#ifndef TFM_PSA_API
#include "mbedtls/entropy.h"

/* This implementation stores the seed in ITS. Under library model, calling ITS
 * from the crypto_init function is forbidden and results in a crash. Therefore,
 * if library model is enabled then a dummy seed will be returned every time,
 * avoiding ITS calls, which is not secure. A runtime warning about this will be
 * emitted.
 */

static unsigned char seed_buf[MBEDTLS_ENTROPY_BLOCK_SIZE] = {0};

/*
 \brief Copy the seed to the destination buffer

 \param[out]  p_dst  Pointer to buffer where to store the seed
 \param[in]   p_src  Pointer to the seed
 \param[in]   size   Length of the seed
*/
static inline void copy_seed(uint8_t *p_dst, const uint8_t *p_src, size_t size)
{
    uint32_t i;

    for (i = size; i > 0; i--) {
        *p_dst = *p_src;
        p_src++;
        p_dst++;
    }
}
#endif /* !TFM_PSA_API */

int tfm_plat_crypto_provision_entropy_seed(void)
{
#ifdef TFM_PSA_API
    enum tfm_plat_err_t plat_err;
    int err;
    uint8_t buf[64];

    /* If the seed is already provisioned, then return successfully */
    err = tfm_plat_crypto_nv_seed_read(buf, sizeof(buf));
    if (err == TFM_CRYPTO_NV_SEED_SUCCESS) {
        return err;
    }

    plat_err = tfm_plat_otp_read(PLAT_OTP_ID_ENTROPY_SEED, sizeof(buf), buf);
    if(plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_CRYPTO_NV_SEED_FAILED;
    }

    err = tfm_plat_crypto_nv_seed_write(buf, sizeof(buf));
    if (err != TFM_CRYPTO_NV_SEED_SUCCESS) {
        return err;
    }
#endif /* TFM_PSA_API */

    return TFM_CRYPTO_NV_SEED_SUCCESS;
}

int tfm_plat_crypto_nv_seed_read(unsigned char *buf, size_t buf_len)
{
#ifdef TFM_PSA_API
    psa_storage_uid_t uid = NV_SEED_FILE_ID;
    psa_status_t status;
    size_t data_length = 0;

    status = psa_its_get(uid, 0, buf_len, buf, &data_length);

    if (status == PSA_SUCCESS && data_length == buf_len) {
        return TFM_CRYPTO_NV_SEED_SUCCESS;
    } else {
        return TFM_CRYPTO_NV_SEED_FAILED;
    }
#else
    if (buf_len != MBEDTLS_ENTROPY_BLOCK_SIZE) {
        return TFM_CRYPTO_NV_SEED_FAILED;
    } else {
        copy_seed(buf, seed_buf, buf_len);
        return TFM_CRYPTO_NV_SEED_SUCCESS;
    }
#endif
}

int tfm_plat_crypto_nv_seed_write(const unsigned char *buf, size_t buf_len)
{
#ifdef TFM_PSA_API
    psa_storage_uid_t uid = NV_SEED_FILE_ID;
    psa_status_t status;

    status = psa_its_set(uid, buf_len, buf, 0);

    if (status == PSA_SUCCESS) {
        return TFM_CRYPTO_NV_SEED_SUCCESS;
    } else {
        return TFM_CRYPTO_NV_SEED_FAILED;
    }
#else
    if (buf_len != MBEDTLS_ENTROPY_BLOCK_SIZE) {
        return TFM_CRYPTO_NV_SEED_FAILED;
    } else {
        copy_seed(seed_buf, buf, buf_len);
        return TFM_CRYPTO_NV_SEED_SUCCESS;
    }
#endif
}
