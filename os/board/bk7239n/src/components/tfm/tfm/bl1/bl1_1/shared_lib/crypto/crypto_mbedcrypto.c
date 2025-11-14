/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "crypto.h"

#include <string.h>
#include "mbedtls/sha256.h"
#include "mbedtls/aes.h"
#include "mbedtls/memory_buffer_alloc.h"
#include "otp.h"

static int mbedtls_is_initialised = 0;
static uint8_t mbedtls_memory_buf[256];

static void mbedtls_init(uint8_t mbedtls_memory_buf[], size_t size)
{
    mbedtls_memory_buffer_alloc_init(mbedtls_memory_buf,
                                     size);
}

int32_t bl1_sha256_compute(const uint8_t *data,
                           size_t data_length,
                           uint8_t *hash)
{
    int rc = 0;
    fih_int fih_rc;
    mbedtls_sha256_context ctx;

    if (!mbedtls_is_initialised) {
        mbedtls_init(mbedtls_memory_buf, sizeof(mbedtls_memory_buf));
        mbedtls_is_initialised = 1;
    }

    mbedtls_sha256_init(&ctx);

    rc = mbedtls_sha256_starts(&ctx, 0);
    fih_rc = fih_int_encode_zero_equality(rc);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        goto out;
    }

    rc = mbedtls_sha256_update(&ctx, data, data_length);
    fih_rc = fih_int_encode_zero_equality(rc);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        goto out;
    }

    rc = mbedtls_sha256_finish(&ctx, hash);
    fih_rc = fih_int_encode_zero_equality(rc);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        goto out;
    }

out:
    mbedtls_sha256_free(&ctx);
    FIH_RET(fih_rc);
}

int32_t bl1_aes_256_ctr_decrypt(enum tfm_bl1_key_id_t key_id,
                                uint8_t *counter,
                                const uint8_t *ciphertext,
                                size_t ciphertext_length,
                                uint8_t *plaintext)
{
    int rc = 0;
    uint8_t stream_block[16];
    uint8_t key_buf[32];
    mbedtls_aes_context ctx;
    size_t nc_off = 0;

    if (ciphertext_length == 0) {
        return 0;
    }

    if (ciphertext == NULL || plaintext == NULL || counter == NULL) {
        return -2;
    }

    if (!mbedtls_is_initialised) {
        mbedtls_init(mbedtls_memory_buf, sizeof(mbedtls_memory_buf));
        mbedtls_is_initialised = 1;
    }

    mbedtls_aes_init(&ctx);
    rc = otp_read_key(key_id, key_buf);
    if (rc) {
        goto out;
    }

    rc = mbedtls_aes_setkey_enc(&ctx, key_buf, 256);
    if (rc) {
        goto out;
    }

    rc = mbedtls_aes_crypt_ctr(&ctx, ciphertext_length, &nc_off, counter,
                               stream_block, ciphertext, plaintext);
    if (rc) {
        goto out;
    }

out:
    mbedtls_aes_free(&ctx);

    memset(key_buf, 0, 32);
    memset(stream_block, 0, 16);

    return rc;
}
