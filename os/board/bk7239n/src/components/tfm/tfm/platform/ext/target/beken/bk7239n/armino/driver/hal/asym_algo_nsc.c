// Copyright 2022-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <stdio.h>
#include <string.h>
#include <os/mem.h>
#include "tfm_secure_api.h"
#include "mbedtls/ecp.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/sha256.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "tfm_secure_storage.h"
#include "tfm_asym_algo.h"

#define ECDSA_TAG "ECDSA"

#define ECDSA_LOGI(...) BK_LOGI(ECDSA_TAG, ##__VA_ARGS__)
#define ECDSA_LOGW(...) BK_LOGW(ECDSA_TAG, ##__VA_ARGS__)
#define ECDSA_LOGE(...) BK_LOGE(ECDSA_TAG, ##__VA_ARGS__)
#define ECDSA_LOGD(...) BK_LOGD(ECDSA_TAG, ##__VA_ARGS__)

typedef enum {
    MBEDTLS_ECDSA_SUCCESS = 0,
    MBEDTLS_ECDSA_ERROR_INVALID_INPUT = -1,
    MBEDTLS_ECDSA_ERROR_INIT_FAILED = -2,
    MBEDTLS_ECDSA_ERROR_KEY_PARSE = -3,
    MBEDTLS_ECDSA_ERROR_SIGN_FAILED = -4,
    MBEDTLS_ECDSA_ERROR_VERIFY_FAILED = -5,
    MBEDTLS_ECDSA_ERROR_MEMORY = -6,
    MBEDTLS_ECDSA_ERROR_RANDOM = -7
} mbedtls_ecdsa_result_t;

#define ECDSA_PRIVATE_KEY_SIZE  (32)
#define ECDSA_PUBLIC_KEY_SIZE   (65)

/* Calling this API in tfm_s.bin to trigger the compiler to generate NSC symbols in tfm_s_veneers.o.
 **/
void psa_asym_algo_nsc_stub(void)
{
	return;
}

/**
 * @brief use ECDSA P-256 to sign message
 * 
 * @param[in] private_key_raw 32 bytes, d value of P-256 private key
 * @param[in] message
 * @param[in] message_len
 * @param[out] signature
 * @param[in] signature_buf_size
 * @param[out] signature_len
 * @return mbedtls_ecdsa_result_t
 */
static mbedtls_ecdsa_result_t ecdsa_sign_p256_raw(
    const uint8_t *private_key_raw,
    const uint8_t *message,
    size_t message_len,
    uint8_t *signature,
    size_t *signature_len
) {

    int ret = 0;
    mbedtls_ecdsa_context ctx;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    uint8_t hash[32];
    const char *pers = "ecdsa_sign";
    char error_buf[100];
    mbedtls_mpi r, s;

    mbedtls_ecdsa_init(&ctx);
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);

    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                                (const uint8_t *) pers, strlen(pers));
    if (ret != 0) {
        mbedtls_strerror(ret, error_buf, sizeof(error_buf));
        ECDSA_LOGE("CTR_DRBG seed failed: %d: %s\n", ret, error_buf);
        ret = MBEDTLS_ECDSA_ERROR_RANDOM;
        goto cleanup;
    }

    ret = mbedtls_ecp_group_load(&ctx.grp, MBEDTLS_ECP_DP_SECP256R1);
    if (ret != 0) {
        mbedtls_strerror(ret, error_buf, sizeof(error_buf));
        ECDSA_LOGE("ECP group load failed: %d: %s\n", ret, error_buf);
        ret = MBEDTLS_ECDSA_ERROR_INIT_FAILED;
        goto cleanup;
    }

    ret = mbedtls_mpi_read_binary(&ctx.d, private_key_raw, ECDSA_PRIVATE_KEY_SIZE);
    if (ret != 0) {
        mbedtls_strerror(ret, error_buf, sizeof(error_buf));
        ECDSA_LOGE("MPI read binary (private key) failed: %d: %s\n", ret, error_buf);
        ret = MBEDTLS_ECDSA_ERROR_KEY_PARSE;
        goto cleanup;
    }

    ret = mbedtls_sha256(message, message_len, hash, 0); // 0 for SHA-256
    if (ret != 0) {
        mbedtls_strerror(ret, error_buf, sizeof(error_buf));
        ECDSA_LOGE("SHA-256 hash failed: %d: %s\n", ret, error_buf);
        ret = MBEDTLS_ECDSA_ERROR_SIGN_FAILED;
        goto cleanup;
    }

    ret = mbedtls_ecdsa_sign(&ctx.grp, &r, &s, &ctx.d, hash, 32, mbedtls_ctr_drbg_random, &ctr_drbg);
    if (ret != 0) {
        mbedtls_strerror(ret, error_buf, sizeof(error_buf));
        ECDSA_LOGE("ECDSA sign failed: %d: %s\n", ret, error_buf);
        ret = MBEDTLS_ECDSA_ERROR_SIGN_FAILED;
        goto cleanup;
    }

    // Convert the generated r and s values to 32-byte raw format
    ret = mbedtls_mpi_write_binary(&r, signature, 32);
    if (ret != 0) {
        ECDSA_LOGE("Failed to write r to signature buffer: -0x%04x\n", -ret);
        ret = MBEDTLS_ECDSA_ERROR_SIGN_FAILED;
        goto cleanup;
    }
    ret = mbedtls_mpi_write_binary(&s, signature + 32, 32);
    if (ret != 0) {
        ECDSA_LOGE("Failed to write s to signature buffer: -0x%04x\n", -ret);
        ret = MBEDTLS_ECDSA_ERROR_SIGN_FAILED;
        goto cleanup;
    }
    *signature_len = 64;
    ret = MBEDTLS_ECDSA_SUCCESS; // Success

cleanup:
    mbedtls_mpi_free(&s);
    mbedtls_mpi_free(&r);
    mbedtls_ecdsa_free(&ctx);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    return (mbedtls_ecdsa_result_t)ret;
}

/**
 * @brief use ECDSA P-256 verify signature
 * 
 * @param[in] public_key_raw (65 bytes, 0x4 || X || Y)
 * @param[in] message
 * @param[in] message_len
 * @param[in] signature
 * @param[in] signature_len
 * @return mbedtls_ecdsa_result_t 
 */
static mbedtls_ecdsa_result_t ecdsa_verify_p256_raw(
    const uint8_t *public_key_raw,
    const uint8_t *message,
    size_t message_len,
    const uint8_t *signature
) {
    int ret = 0;
    mbedtls_ecdsa_context ctx;
    uint8_t hash[32];
    char error_buf[100];
    mbedtls_mpi r, s;

    mbedtls_ecdsa_init(&ctx);
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);

    ret = mbedtls_ecp_group_load(&ctx.grp, MBEDTLS_ECP_DP_SECP256R1);
    if (ret != 0) {
        mbedtls_strerror(ret, error_buf, sizeof(error_buf));
        ECDSA_LOGE("ECP group load failed: %d: %s\n", ret, error_buf);
        ret = MBEDTLS_ECDSA_ERROR_INIT_FAILED;
        goto cleanup;
    }

    ret = mbedtls_ecp_point_read_binary(&ctx.grp, &ctx.Q, public_key_raw, ECDSA_PUBLIC_KEY_SIZE);
    if (ret != 0) {
        mbedtls_strerror(ret, error_buf, sizeof(error_buf));
        ECDSA_LOGE("ECP point read binary (public key) failed: %d: %s\n", ret, error_buf);
        ret = MBEDTLS_ECDSA_ERROR_KEY_PARSE;
        goto cleanup;
    }

    ret = mbedtls_sha256(message, message_len, hash, 0); // 0 for SHA-256
    if (ret != 0) {
        mbedtls_strerror(ret, error_buf, sizeof(error_buf));
        ECDSA_LOGE("SHA-256 hash failed: %d: %s\n", ret, error_buf);
        ret = MBEDTLS_ECDSA_ERROR_VERIFY_FAILED;
        goto cleanup;
    }

    ret = mbedtls_mpi_read_binary(&r, signature, 32);
    if (ret != 0) {
        mbedtls_strerror(ret, error_buf, sizeof(error_buf));
        ECDSA_LOGE("MPI read binary (r) failed: %d: %s\n", ret, error_buf);
        ret = MBEDTLS_ECDSA_ERROR_VERIFY_FAILED;
        goto cleanup;
    }
    ret = mbedtls_mpi_read_binary(&s, signature + 32, 32);
    if (ret != 0) {
        mbedtls_strerror(ret, error_buf, sizeof(error_buf));
        ECDSA_LOGE("MPI read binary (s) failed: %d: %s\n", ret, error_buf);
        ret = MBEDTLS_ECDSA_ERROR_VERIFY_FAILED;
        goto cleanup;
    }

    ret = mbedtls_ecdsa_verify(&ctx.grp, hash, 32, &ctx.Q, &r, &s);
    if (ret == 0) {
        ret = MBEDTLS_ECDSA_SUCCESS;
    } else {
        mbedtls_strerror(ret, error_buf, sizeof(error_buf));
        ECDSA_LOGE("ECDSA verify failed: %d: %s\n", ret, error_buf);
        ret = MBEDTLS_ECDSA_ERROR_VERIFY_FAILED;
    }

cleanup:
    mbedtls_ecdsa_free(&ctx);
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);

    return (mbedtls_ecdsa_result_t)ret;
}

__tfm_psa_secure_gateway_no_naked_attributes__ int psa_ecdsa_sign_p256(hal_data_type_t *message, uint32_t key_idx, hal_data_type_t *sign) {

    int32_t ret;
    uint8_t private_key_raw[ECDSA_PRIVATE_KEY_SIZE] = {0x0};
    uint32_t read_len;

    if (!message || !message->data || !sign || !sign->data) {
        return SS_INVALID_ARGS;
    }

    if (key_idx >= SS_RAM_INDEX_START) {
        return SS_INVALID_SLOT_RANGE;
    }

    ret = ss_read_key(key_idx, private_key_raw, &read_len);
    if(ret != SS_SUCCESS) {
        return ret;
    }
    if(read_len != ECDSA_PRIVATE_KEY_SIZE) {
        memset(private_key_raw, 0x0, ECDSA_PRIVATE_KEY_SIZE);
        return SS_BAD_KEY_TYPE;
    }

    ret =  ecdsa_sign_p256_raw(private_key_raw, message->data, message->data_len, sign->data, (size_t *)&sign->data_len);
    if(ret != MBEDTLS_ECDSA_SUCCESS) {
        ECDSA_LOGE("ECDSA sign failed: %d line %d\r\n", ret, __LINE__);
        memset(private_key_raw, 0x0, ECDSA_PRIVATE_KEY_SIZE);
        return SS_FAIL;
    }

    memset(private_key_raw, 0x0, ECDSA_PRIVATE_KEY_SIZE);
    return SS_SUCCESS;
}

__tfm_psa_secure_gateway_no_naked_attributes__ int psa_ecdsa_verify_p256( hal_data_type_t *message, uint32_t key_idx, hal_data_type_t *sign) {

    int32_t ret;
    uint8_t public_key_raw[ECDSA_PUBLIC_KEY_SIZE] = {0x0};
    uint32_t read_len;

    if (!message || !message->data || !sign || !sign->data) {
        return SS_INVALID_ARGS;
    }

    if (key_idx >= SS_RAM_INDEX_START) {
        return SS_INVALID_SLOT_RANGE;
    }

    public_key_raw[0] = (uint8_t)0x04;
    ret = ss_read_key(key_idx, &public_key_raw[1], &read_len);
    if(ret != SS_SUCCESS) {
        return ret;
    }
    if(read_len != ECDSA_PUBLIC_KEY_SIZE - 1) {
        memset(public_key_raw, 0x0, ECDSA_PUBLIC_KEY_SIZE);
        return SS_BAD_KEY_TYPE;
    }

    ret = ecdsa_verify_p256_raw(public_key_raw, message->data, message->data_len, sign->data);
    if(ret != MBEDTLS_ECDSA_SUCCESS) {
        ECDSA_LOGE("ECDSA verify failed: %d line %d\r\n", ret, __LINE__);
        memset(public_key_raw, 0x0, ECDSA_PUBLIC_KEY_SIZE);
        return SS_FAIL;
    }

    memset(public_key_raw, 0x0, ECDSA_PUBLIC_KEY_SIZE);
    return SS_SUCCESS;
}