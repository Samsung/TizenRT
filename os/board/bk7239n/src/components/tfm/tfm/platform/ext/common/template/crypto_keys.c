/*
 * Copyright (c) 2017-2021 Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stddef.h>
#include <string.h>

#include "psa/crypto_types.h"
#include "tfm_plat_crypto_keys.h"
#include "tfm_plat_otp.h"
#include "mbedtls/hkdf.h"

#ifdef CRYPTO_HW_ACCELERATOR
#include "crypto_hw.h"
#endif /* CRYPTO_HW_ACCELERATOR */

enum tfm_plat_err_t tfm_plat_get_huk_derived_key(const uint8_t *label,
                                                 size_t label_size,
                                                 const uint8_t *context,
                                                 size_t context_size,
                                                 uint8_t *key,
                                                 size_t key_size)
{
#ifdef CRYPTO_HW_ACCELERATOR
    return crypto_hw_accelerator_huk_derive_key(label, label_size, context,
                                                context_size, key, key_size);
#else
    uint8_t huk_buf[32];
    enum tfm_plat_err_t err;
    int mbedtls_err;

    if (key == NULL) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    if (label == NULL && label_size != 0) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    if (context == NULL && context_size != 0) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    err = tfm_plat_otp_read(PLAT_OTP_ID_HUK, sizeof(huk_buf), huk_buf);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        goto out;
    }

    mbedtls_err = mbedtls_hkdf(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
                               label, label_size, huk_buf, sizeof(huk_buf),
                               context, context_size, key, key_size);
    if (mbedtls_err) {
        err = TFM_PLAT_ERR_SYSTEM_ERR;
        goto out;
    }

out:
    memset(huk_buf, 0, sizeof(huk_buf));

    return err;
#endif /* CRYPTO_HW_ACCELERATOR */
}

#ifdef SYMMETRIC_INITIAL_ATTESTATION
enum tfm_plat_err_t tfm_plat_get_symmetric_iak(uint8_t *key_buf,
                                               size_t buf_len,
                                               size_t *key_len,
                                               psa_algorithm_t *key_alg)
{
    enum tfm_plat_err_t err;

    if (key_buf == NULL || key_len == NULL) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    err = tfm_plat_otp_read(PLAT_OTP_ID_IAK, buf_len, key_buf);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = tfm_plat_otp_read(PLAT_OTP_ID_IAK_LEN, sizeof(size_t),
                            (uint8_t*)key_len);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = tfm_plat_otp_read(PLAT_OTP_ID_IAK_TYPE,
                            sizeof(psa_algorithm_t), (uint8_t*)key_alg);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    return TFM_PLAT_ERR_SUCCESS;

}

enum tfm_plat_err_t tfm_plat_get_symmetric_iak_id(void *kid_buf,
                                                  size_t buf_len,
                                                  size_t *kid_len)
{
    enum tfm_plat_err_t err;
    size_t otp_size;

    if (kid_buf == NULL || kid_len == NULL) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    err = tfm_plat_otp_read(PLAT_OTP_ID_IAK_ID, buf_len, kid_buf);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err =  tfm_plat_otp_get_size(PLAT_OTP_ID_IAK_ID, &otp_size);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    *kid_len = strlen(kid_buf) <= otp_size ? strlen(kid_buf) : otp_size;

    return TFM_PLAT_ERR_SUCCESS;
}
#else /* SYMMETRIC_INITIAL_ATTESTATION */
enum tfm_plat_err_t
tfm_plat_get_initial_attest_key(uint8_t          *key_buf,
                                uint32_t          size,
                                struct ecc_key_t *ecc_key,
                                psa_ecc_family_t *curve_type)
{
    uint32_t key_size;
    enum tfm_plat_err_t err;

    if (key_buf == NULL || ecc_key == NULL || curve_type == NULL) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    err = tfm_plat_otp_read(PLAT_OTP_ID_IAK, size, key_buf);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err =  tfm_plat_otp_read(PLAT_OTP_ID_IAK_LEN, sizeof(key_size),
                             (uint8_t*)&key_size);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    /* Set the EC curve type which the key belongs to */
    err = tfm_plat_otp_read(PLAT_OTP_ID_IAK_TYPE,
                            sizeof(psa_ecc_family_t), curve_type);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    /* Copy the private key to the buffer, it MUST be present */
    ecc_key->priv_key = key_buf;
    ecc_key->priv_key_size = key_size;

    ecc_key->pubx_key = NULL;
    ecc_key->pubx_key_size = 0;
    ecc_key->puby_key = NULL;
    ecc_key->puby_key_size = 0;

    return TFM_PLAT_ERR_SUCCESS;
}
#endif /* SYMMETRIC_INITIAL_ATTESTATION */
