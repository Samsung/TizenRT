/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_aes.h
 *
 * This file contains the declaration of the internal functions to
 * perform symmetric encryption and decryption using the AES algorithm
 *
 */

#ifndef CC3XX_INTERNAL_AES_H
#define CC3XX_INTERNAL_AES_H

#include "psa/crypto.h"
#include "aes_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initialize the specified AES context.
 */
void cc3xx_aes_init(AesContext_t *ctx);

/**
 * \brief Release and clear the specified AES context.
 */
void cc3xx_aes_free(AesContext_t *ctx);

/**
 * \brief Set the encryption key.
 */
psa_status_t cc3xx_aes_setkey_enc(
        AesContext_t *ctx,
        const uint8_t *key,
        size_t key_bits);

/**
 * \brief Set the decryption key.
 */
psa_status_t cc3xx_aes_setkey_dec(
        AesContext_t *ctx,
        const uint8_t *key,
        size_t key_bits);

/**
 * \brief AES block encryption/decryption.
 */
psa_status_t cc3xx_aes_crypt(
        AesContext_t *ctx,
        aesMode_t mode,
        size_t length,
        uint8_t iv[AES_IV_SIZE],
        const uint8_t *input,
        uint8_t *output);

#ifdef __cplusplus
}
#endif
#endif /* CC3XX_INTERNAL_AES_H */
