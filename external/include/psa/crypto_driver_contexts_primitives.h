/**
 * \file crypto_driver_contexts_primitives.h
 *
 * \brief Declaration of context structures for use with the PSA driver wrapper
 * interface. This file contains the context structures for 'primitive'
 * operations, i.e. those operations which do not rely on other contexts.
 *
 * \warning This file will be auto-generated in the future.
 *
 * \note This file may not be included directly. Applications must
 * include psa/crypto.h.
 *
 * \note This header and its content are not part of the Mbed TLS API and
 * applications must not depend on it. Its main purpose is to define the
 * multi-part state objects of the PSA drivers included in the cryptographic
 * library. The definitions of these objects are then used by crypto_struct.h
 * to define the implementation-defined types of PSA multi-part state objects.
 */
/*  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

#ifndef PSA_CRYPTO_DRIVER_CONTEXTS_PRIMITIVES_H
#define PSA_CRYPTO_DRIVER_CONTEXTS_PRIMITIVES_H

#include "psa/crypto_driver_common.h"

/* Include the context structure definitions for the Mbed TLS software drivers */
#include "mbedtls/private/crypto_builtin_primitives.h"

/* TizenRT Samsung SE transparent cipher driver context.
 * Fields use only standard C types to avoid pulling in platform headers.
 * The actual driver (tizenrt_psa_cipher_driver.c) casts this struct to
 * match its internal view which uses sl_ctx / hal_key_type directly.     */
#if defined(TIZENRT_PSA_DRIVER_PRESENT)
#include <stdint.h>
#include <stddef.h>
typedef struct {
    void       *shnd;           /* sl_ctx handle (struct _seclink_s_ *)  */
    int         key_idx;        /* hardware AES key slot index           */
    uint32_t    key_type;       /* hal_key_type (HAL_KEY_AES_128/192/256)*/
    uint32_t    alg;            /* psa_algorithm_t                       */
    int         is_encrypt;
    uint8_t     iv[16];
    size_t      iv_length;
    int         iv_set;
    uint8_t     buffer[16];     /* partial-block accumulation buffer     */
    size_t      buffer_length;
    int         is_active;
} tizenrt_transparent_cipher_operation_t;
#endif /* TIZENRT_PSA_DRIVER_PRESENT */

/* Include the context structure definitions for those drivers that were
 * declared during the autogeneration process. */

#if defined(MBEDTLS_TEST_LIBTESTDRIVER1)
#if defined(TF_PSA_CRYPTO_TEST_LIBTESTDRIVER1)
#include "mbedtls/private/libtestdriver1-crypto_builtin_primitives.h"
#else
#include <libtestdriver1/tf-psa-crypto/include/psa/crypto.h>
#endif
#endif

#if defined(PSA_CRYPTO_DRIVER_TEST)

#if defined(MBEDTLS_TEST_LIBTESTDRIVER1) && \
    defined(LIBTESTDRIVER1_MBEDTLS_PSA_BUILTIN_CIPHER)
typedef libtestdriver1_mbedtls_psa_cipher_operation_t
    mbedtls_transparent_test_driver_cipher_operation_t;

#define MBEDTLS_TRANSPARENT_TEST_DRIVER_CIPHER_OPERATION_INIT \
    LIBTESTDRIVER1_MBEDTLS_PSA_CIPHER_OPERATION_INIT
#else
typedef mbedtls_psa_cipher_operation_t
    mbedtls_transparent_test_driver_cipher_operation_t;

#define MBEDTLS_TRANSPARENT_TEST_DRIVER_CIPHER_OPERATION_INIT \
    MBEDTLS_PSA_CIPHER_OPERATION_INIT
#endif /* MBEDTLS_TEST_LIBTESTDRIVER1 &&
          LIBTESTDRIVER1_MBEDTLS_PSA_BUILTIN_CIPHER */

#if defined(MBEDTLS_TEST_LIBTESTDRIVER1) && \
    defined(LIBTESTDRIVER1_MBEDTLS_PSA_BUILTIN_HASH)
typedef libtestdriver1_mbedtls_psa_hash_operation_t
    mbedtls_transparent_test_driver_hash_operation_t;

#define MBEDTLS_TRANSPARENT_TEST_DRIVER_HASH_OPERATION_INIT \
    LIBTESTDRIVER1_MBEDTLS_PSA_HASH_OPERATION_INIT
#else
typedef mbedtls_psa_hash_operation_t
    mbedtls_transparent_test_driver_hash_operation_t;

#define MBEDTLS_TRANSPARENT_TEST_DRIVER_HASH_OPERATION_INIT \
    MBEDTLS_PSA_HASH_OPERATION_INIT
#endif /* MBEDTLS_TEST_LIBTESTDRIVER1 &&
          LIBTESTDRIVER1_MBEDTLS_PSA_BUILTIN_HASH */

#if defined(MBEDTLS_TEST_LIBTESTDRIVER1) && \
    defined(LIBTESTDRIVER1_MBEDTLS_PSA_BUILTIN_XOF)
typedef libtestdriver1_mbedtls_psa_xof_operation_t
    mbedtls_transparent_test_driver_xof_operation_t;

#define MBEDTLS_TRANSPARENT_TEST_DRIVER_XOF_OPERATION_INIT \
    LIBTESTDRIVER1_MBEDTLS_PSA_XOF_OPERATION_INIT
#else
typedef mbedtls_psa_xof_operation_t
    mbedtls_transparent_test_driver_xof_operation_t;

#define MBEDTLS_TRANSPARENT_TEST_DRIVER_XOF_OPERATION_INIT \
    MBEDTLS_PSA_XOF_OPERATION_INIT
#endif /* MBEDTLS_TEST_LIBTESTDRIVER1 &&
          LIBTESTDRIVER1_MBEDTLS_PSA_BUILTIN_XOF */

typedef struct {
    unsigned int initialised : 1;
    mbedtls_transparent_test_driver_cipher_operation_t ctx;
} mbedtls_opaque_test_driver_cipher_operation_t;

#define MBEDTLS_OPAQUE_TEST_DRIVER_CIPHER_OPERATION_INIT \
    { 0, MBEDTLS_TRANSPARENT_TEST_DRIVER_CIPHER_OPERATION_INIT }

#endif /* PSA_CRYPTO_DRIVER_TEST */

/* Define the context to be used for an operation that is executed through the
 * PSA Driver wrapper layer as the union of all possible driver's contexts.
 *
 * The union members are the driver's context structures, and the member names
 * are formatted as `'drivername'_ctx`. This allows for procedural generation
 * of both this file and the content of psa_crypto_driver_wrappers.h */

typedef union {
    unsigned dummy; /* Make sure this union is always non-empty */
    mbedtls_psa_hash_operation_t mbedtls_ctx;
#if defined(PSA_CRYPTO_DRIVER_TEST)
    mbedtls_transparent_test_driver_hash_operation_t test_driver_ctx;
#endif
} psa_driver_hash_context_t;

typedef union {
    unsigned dummy; /* Make sure this union is always non-empty */
    mbedtls_psa_xof_operation_t mbedtls_ctx;
#if defined(PSA_CRYPTO_DRIVER_TEST)
    mbedtls_transparent_test_driver_xof_operation_t test_driver_ctx;
#endif
} psa_driver_xof_context_t;

typedef union {
    unsigned dummy; /* Make sure this union is always non-empty */
    mbedtls_psa_cipher_operation_t mbedtls_ctx;
#if defined(PSA_CRYPTO_DRIVER_TEST)
    mbedtls_transparent_test_driver_cipher_operation_t transparent_test_driver_ctx;
    mbedtls_opaque_test_driver_cipher_operation_t opaque_test_driver_ctx;
#endif
#if defined(TIZENRT_PSA_DRIVER_PRESENT)
    tizenrt_transparent_cipher_operation_t tizenrt_ctx;
#endif
} psa_driver_cipher_context_t;

#endif /* PSA_CRYPTO_DRIVER_CONTEXTS_PRIMITIVES_H */
/* End of automatically generated file. */
