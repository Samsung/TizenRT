/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_crypto_primitives.h
 *
 * This file contains the declaration of public types associated
 * to the CC3XX driver
 *
 */

#ifndef CC3XX_CRYPTO_PRIMITIVES_H
#define CC3XX_CRYPTO_PRIMITIVES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Make the private structure names visible to the code that includes
 * this public interface header
 */
struct cc3xx_hash_operation_s;
struct cc3xx_cipher_operation_s;
struct cc3xx_mac_operation_s;
struct cc3xx_aead_operation_s;

/*! \typedef cc3xx_hash_operation_t
 *
 * \brief A concise way to refer to \ref cc3xx_hash_operation_s
 *
 */
typedef struct cc3xx_hash_operation_s cc3xx_hash_operation_t;

/*! \typedef cc3xx_cipher_operation_t
 *
 * \brief A concise way to refer to \ref cc3xx_cipher_operation_s
 *
 */
typedef struct cc3xx_cipher_operation_s cc3xx_cipher_operation_t;

/*! \typedef cc3xx_mac_operation_t
 *
 * \brief A concise way to refer to \ref cc3xx_mac_operation_s
 *
 */
typedef struct cc3xx_mac_operation_s cc3xx_mac_operation_t;

/*! \typedef cc3xx_aead_operation_t
 *
 * \brief A concise way to refer to \ref cc3xx_aead_operation_t
 *
 */
typedef struct cc3xx_aead_operation_s cc3xx_aead_operation_t;

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_CRYPTO_PRIMITIVES_H */
