/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef BL1_1_CRYPTO_H
#define BL1_1_CRYPTO_H

#include <stddef.h>
#include <stdint.h>

#define CTR_IV_LEN 16

#include "crypto_key_defs.h"
#include "fih.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Calculates a SHA-256 hash of the input data */
fih_int bl1_sha256_compute(const uint8_t *data,
                           size_t data_length,
                           uint8_t *hash);

/* Performs AES-256-CTR decryption */
int32_t bl1_aes_256_ctr_decrypt(enum tfm_bl1_key_id_t key_id,
                                uint8_t *counter,
                                const uint8_t *ciphertext,
                                size_t ciphertext_length,
                                uint8_t *plaintext);

#ifdef __cplusplus
}
#endif

#endif /* BL1_1_CRYPTO_H */
