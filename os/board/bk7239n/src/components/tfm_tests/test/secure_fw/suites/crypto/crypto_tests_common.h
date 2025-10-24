/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CRYPTO_TESTS_COMMON_H__
#define __CRYPTO_TESTS_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "psa/crypto.h"
#include "test_framework_helpers.h"

/**
 * \brief Size of the key to use in tests in bits
 *
 */
#define BIT_SIZE_TEST_KEY (128)

/**
 * \brief Size of the long key to use in tests in bits
 *
 */
#define BIT_SIZE_TEST_LONG_KEY (256)

/**
 * \brief Macro to extract the size of the key in bytes
 *
 */
#define BYTE_SIZE_TEST_KEY (BIT_SIZE_TEST_KEY / 8)

/**
 * \brief Size in bytes of a chunk of data to process
 *
 */
#define BYTE_SIZE_CHUNK (16)

/**
 * \brief Size in bytes of the plain data processed in cipher tests
 *
 */
#define PLAIN_DATA_SIZE (3 * BYTE_SIZE_CHUNK)

/**
 * \brief Size in bytes of the encryption/decryption buffers
 *
 */
#define ENC_DEC_BUFFER_SIZE (4 * BYTE_SIZE_CHUNK)

/**
 * \brief When the input algorithm has padding, the required encrypted
 *        data needs one block more due to the fact that it has to
 *        produce the encrypted padding
 */
#define ENC_DEC_BUFFER_SIZE_PAD_MODES (ENC_DEC_BUFFER_SIZE + BYTE_SIZE_CHUNK)

/**
 * \brief Size in bytes of the small input plain data that is used for
 *        the dedicated tests for padded modes which process smaller
 *        chunks of data
 */
#define PLAIN_DATA_SIZE_PAD_TEST (BYTE_SIZE_CHUNK + BYTE_SIZE_CHUNK / 2)
/**
 * \brief Size in bytes of the encrypted/decrypted buffers for the dedicated
 *        tests for padded modes which process smaller chunks of data
 */
#define ENC_DEC_BUFFER_SIZE_PAD_TEST (2 * BYTE_SIZE_CHUNK)

/**
 * \brief Max size in bytes of the data to be encrypted in AEAD
 *        tests.
 */
#define MAX_PLAIN_DATA_SIZE_AEAD (68)

/**
 * \brief Size in bytes of the encryption/decryption buffers used
 *        in AEAD tests
 *
 */
#define ENC_DEC_BUFFER_SIZE_AEAD \
    (PSA_AEAD_TAG_MAX_SIZE + MAX_PLAIN_DATA_SIZE_AEAD)

/**
 * \brief The maximum allowed key length in bytes used in the
 *        tests
 */
#define TEST_MAX_KEY_LENGTH (64)

/**
 * \brief The length of truncated authentication tag for AEAD algorithm
 */
#define TRUNCATED_AUTH_TAG_LEN (8)

/**
 * \brief 128-bit test key
 */
static const uint8_t test_key_128[] = "THIS IS MY KEY1";

/**
 * \brief 256-bit test key
 */
static const uint8_t test_key_256[] = "THIS IS MY UNCOMMONLY LONG KEY1";

/**
 * \brief Tests the key interfaces with different key types
 *
 * \param[in]  key_type PSA key type
 * \param[out] ret      Test result
 *
 */
void psa_key_interface_test(psa_key_type_t key_type,
                            struct test_result_t *ret);
/**
 * \brief Run block ciphering tests with padded modes only
 *
 * \param[in]  key_type PSA key type
 * \param[in]  alg      PSA algorithm
 * \param[in]  len      Input length
 * \param[out] ret      Test result
 *
 */
void psa_cipher_padded_modes_test(psa_key_type_t key_type,
                                  psa_algorithm_t alg,
                                  uint8_t len,
                                  struct test_result_t *ret);
/**
 * \brief Run block ciphering tests with different algorithms and key types
 *
 * \param[in]  key_type PSA key type
 * \param[in]  alg      PSA algorithm
 * \param[in]  key      Encryption key
 * \param[in]  key_bits Encryption key size in bits
 * \param[out] ret      Test result
 *
 */
void psa_cipher_test(const psa_key_type_t key_type,
                     const psa_algorithm_t alg,
                     const uint8_t *key,
                     size_t key_bits,
                     struct test_result_t *ret);
/**
 * \brief Tests invalid key type and algorithm combinations for block ciphers
 *
 * \param[in]  key_type  PSA key type
 * \param[in]  alg       PSA algorithm
 * \param[in]  key_size  Key size
 * \param[out] ret       Test result
 *
 */
void psa_invalid_cipher_test(psa_key_type_t key_type,
                             psa_algorithm_t alg,
                             size_t key_size,
                             struct test_result_t *ret);
/**
 * \brief Tests setup of an unsupported hash algorithm
 *
 * \param[in]  alg PSA algorithm
 * \param[out] ret Test result
 *
 */
void psa_unsupported_hash_test(psa_algorithm_t alg,
                               struct test_result_t *ret);
/**
 * \brief Tests different hashing algorithms
 *
 * \param[in]  alg PSA algorithm
 * \param[out] ret Test result
 *
 */
void psa_hash_test(psa_algorithm_t alg,
                   struct test_result_t *ret);
/**
 * \brief Tests setup of an unsupported MAC algorithm
 *
 * \param[in]  key_type PSA key type
 * \param[in]  alg      PSA algorithm
 * \param[out] ret      Test result
 *
 */
void psa_unsupported_mac_test(psa_key_type_t key_type,
                              psa_algorithm_t alg,
                              struct test_result_t *ret);
/**
 * \brief Tests different MAC algorithms
 *
 * \param[in]  alg          PSA algorithm
 * \param[in]  key          Encryption key
 * \param[in]  key_bits     Encryption key size in bits
 * \param[out] ret          Test result
 *
 */
void psa_mac_test(const psa_algorithm_t alg,
                  const uint8_t *key,
                  size_t key_bits,
                  struct test_result_t *ret);
/**
 * \brief Run AEAD tests with different algorithms and key types
 *
 * \param[in]  key_type PSA key type
 * \param[in]  alg      PSA algorithm
 * \param[in]  key      Encryption key
 * \param[in]  key_bits Encryption key size in bits
 * \param[out] ret      Test result
 *
 */
void psa_aead_test(const psa_key_type_t key_type,
                   const psa_algorithm_t alg,
                   const uint8_t *key,
                   size_t key_bits,
                   struct test_result_t *ret);
/**
 * \brief Tests invalid key length
 *
 * \param[out] ret Test result
 *
 */
void psa_invalid_key_length_test(struct test_result_t *ret);

/**
 * \brief Tests the policy key interface
 *
 * \param[out] ret Test result
 *
 */
void psa_policy_key_interface_test(struct test_result_t *ret);

/**
 * \brief Tests invalid policy usage
 *
 * \param[out] ret Test result
 *
 */
void psa_policy_invalid_policy_usage_test(struct test_result_t *ret);

/**
 * \brief Tests persistent keys
 *
 * \param[out] ret Test result
 *
 */
void psa_persistent_key_test(psa_key_id_t key_id, struct test_result_t *ret);

/**
 * \brief Key derivation test
 *
 * \param[in] deriv_alg Key derivation algorithm
 * \param[out] ret      Test result
 */
void psa_key_derivation_test(psa_algorithm_t deriv_alg,
                             struct test_result_t *ret);

/**
 * \brief Key agreement test
 *
 * \param[in] deriv_alg A key derivation algorithm which acts as raw key
 *                      agreement
 * \param[out] ret      Test result
 */
void psa_key_agreement_test(psa_algorithm_t deriv_alg,
                            struct test_result_t *ret);

/**
 * \brief Asymmetric encryption/decryption test
 *
 *  \param[in] alg  Encryption algorithm
 *  \param[out] ret Test result
 */
void psa_asymmetric_encryption_test(psa_algorithm_t alg,
                                    struct test_result_t *ret);

/**
 * \brief Message sign/verify test
 *
 *  \param[in] alg  Signing algorithm
 *  \param[out] ret Test result
 */
void psa_sign_verify_message_test(psa_algorithm_t alg,
                                  struct test_result_t *ret);

#ifdef TFM_CRYPTO_TEST_CHACHA20
/**
 * \brief Verification of Chacha20 using RFC7539 test vectors
 *
 * \param[out] ret Test result
 */
void psa_cipher_rfc7539_test(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_CHACHA20 */

#ifdef TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305
/**
 * \brief Verification of Chacha20-Poly1305 using RFC7539 test vectors
 *
 * \param[out] ret Test result
 */
void psa_aead_rfc7539_test(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305 */

#ifdef __cplusplus
}
#endif

#endif /* __CRYPTO_TESTS_COMMON__ */
