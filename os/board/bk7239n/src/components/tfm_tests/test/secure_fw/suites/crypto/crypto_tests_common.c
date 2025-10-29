/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#if DOMAIN_NS == 1
#include <string.h>
#else
#include "tfm_memory_utils.h"
#endif

#include <stdbool.h>
#include "crypto_tests_common.h"

void psa_key_interface_test(const psa_key_type_t key_type,
                            struct test_result_t *ret)
{
    psa_status_t status = PSA_SUCCESS;
    uint32_t i = 0;
    psa_key_handle_t key_handle = 0x0u;
    const uint8_t data[] = "THIS IS MY KEY1";
    uint8_t exported_data[sizeof(data)] = {0};
    size_t exported_data_size = 0;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_attributes_t retrieved_attributes = psa_key_attributes_init();

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_EXPORT);
    psa_set_key_type(&key_attributes, key_type);

    status = psa_import_key(&key_attributes, data, sizeof(data),
                            &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    status = psa_get_key_attributes(key_handle, &retrieved_attributes);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error getting key metadata");
        return;
    }

    if (psa_get_key_bits(&retrieved_attributes) != BIT_SIZE_TEST_KEY) {
        TEST_FAIL("The number of key bits is different from expected");
        return;
    }

    if (psa_get_key_type(&retrieved_attributes) != key_type) {
        TEST_FAIL("The type of the key is different from expected");
        return;
    }

    psa_reset_key_attributes(&retrieved_attributes);

    status = psa_export_key(key_handle,
                            exported_data,
                            sizeof(data),
                            &exported_data_size);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error exporting a key");
        return;
    }

    if (exported_data_size != BYTE_SIZE_TEST_KEY) {
        TEST_FAIL("Number of bytes of exported key different from expected");
        return;
    }

    /* Check that the exported key is the same as the imported one */
    for (i=0; i<exported_data_size; i++) {
        if (exported_data[i] != data[i]) {
            TEST_FAIL("Exported key doesn't match the imported key");
            return;
        }
    }

    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying the key");
        return;
    }

    status = psa_get_key_attributes(key_handle, &retrieved_attributes);
    if (status != PSA_ERROR_INVALID_HANDLE) {
        TEST_FAIL("Key handle should be invalid now");
        return;
    }

    psa_reset_key_attributes(&retrieved_attributes);

    ret->val = TEST_PASSED;
}

static inline int compare_buffers(const uint8_t *p1,
                                  const uint8_t *p2,
                                  size_t comp_size)
{
#if DOMAIN_NS == 1U
    /* Check that the decrypted data is the same as the original data */
    return memcmp(p1, p2, comp_size);
#else
    return tfm_memcmp(p1, p2, comp_size);
#endif
}

#define CIPHER_TEST_KEY_ID (0x1)

void psa_cipher_padded_modes_test(const psa_key_type_t key_type,
                                  const psa_algorithm_t alg,
                                  uint8_t len,
                                  struct test_result_t *ret)
{
    psa_cipher_operation_t handle = psa_cipher_operation_init();
    psa_cipher_operation_t handle_dec = psa_cipher_operation_init();
    psa_status_t status = PSA_SUCCESS;
    psa_key_handle_t key_handle;
    const uint8_t data[] = "THIS IS MY KEY1";
    const size_t iv_length = PSA_BLOCK_CIPHER_BLOCK_LENGTH(key_type);
    const uint8_t iv[] = "012345678901234";
    const uint8_t plain_text[PLAIN_DATA_SIZE_PAD_TEST] =
        "Little text, full!!";
    uint8_t decrypted_data[ENC_DEC_BUFFER_SIZE_PAD_TEST] = {0};
    size_t output_length = 0, total_output_length = 0;
    uint8_t encrypted_data[ENC_DEC_BUFFER_SIZE_PAD_TEST] = {0};
    uint32_t comp_result;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_usage_t usage = (PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    bool bAbortDecryption = false;

    if (iv_length != sizeof(iv)) {
        /* Whenever this condition is hit, it's likely the test requires
         * refactoring to remove any hardcoded behaviour
         */
        TEST_FAIL("Hardcoded IV does not match cipher block length");
        return;
    }

    if (len > sizeof(plain_text)) {
        TEST_FAIL("Requested input length is greater than supported");
        return;
    }

    ret->val = TEST_PASSED;

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, key_type);
    psa_set_key_id(&key_attributes, CIPHER_TEST_KEY_ID);

    /* Import a key */
    status = psa_import_key(&key_attributes, data, sizeof(data), &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    status = psa_get_key_attributes(key_handle, &key_attributes);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error getting key metadata");
        goto destroy_key;
    }

    if (psa_get_key_bits(&key_attributes) != BIT_SIZE_TEST_KEY) {
        TEST_FAIL("The number of key bits is different from expected");
        goto destroy_key;
    }

    if (psa_get_key_type(&key_attributes) != key_type) {
        TEST_FAIL("The type of the key is different from expected");
        goto destroy_key;
    }

    /* Setup the encryption object */
    status = psa_cipher_encrypt_setup(&handle, key_handle, alg);
    if (status != PSA_SUCCESS) {
        if (status == PSA_ERROR_NOT_SUPPORTED) {
            TEST_FAIL("Algorithm NOT SUPPORTED by the implementation");
        } else {
            TEST_FAIL("Error setting up cipher operation object");
        }
        goto destroy_key;
    }

    /* Set the IV */
    status = psa_cipher_set_iv(&handle, iv, iv_length);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting the IV on the cipher operation object");
        goto abort;
    }

    /* Encrypt one chunk of information */
    if (len < BYTE_SIZE_CHUNK) {
        status = psa_cipher_update(&handle, plain_text,
                                   len,
                                   encrypted_data,
                                   sizeof(encrypted_data),
                                   &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error encrypting one chunk of information");
            goto abort;
        }

        /* When encrypting less than a block, the output is produced only
         * when performing the following finish operation
         */
        if (output_length != 0) {
            TEST_FAIL("Expected encrypted length is different from expected");
            goto abort;
        }

        status = psa_cipher_finish(&handle, encrypted_data,
                                   sizeof(encrypted_data),
                                   &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error finalising the cipher operation");
            goto abort;
        }

    } else if (len < 2 * BYTE_SIZE_CHUNK) {
        status = psa_cipher_update(&handle, plain_text,
                                   BYTE_SIZE_CHUNK,
                                   encrypted_data,
                                   sizeof(encrypted_data),
                                   &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error encrypting one chunk of information");
            goto abort;
        }

        /* When encrypting one block, the output is produced right away */
        if (output_length != BYTE_SIZE_CHUNK) {
            TEST_FAIL("Expected encrypted length is different from expected");
            goto abort;
        }

        total_output_length += output_length;
        status = psa_cipher_update(&handle, &plain_text[BYTE_SIZE_CHUNK],
                                   len % BYTE_SIZE_CHUNK,
                                   &encrypted_data[total_output_length],
                                   sizeof(encrypted_data) - total_output_length,
                                   &output_length);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error encrypting one chunk of information");
            goto abort;
        }

        /* When encrypting less than a block, the output is zero */
        if (output_length != 0) {
            TEST_FAIL("Expected encrypted length is different from expected");
            goto abort;
        }

        /* The output is then produced only when calling finish if the previous
         * update did not produce any output - We need to take padding into
         * account
         */
        total_output_length += output_length;
        status = psa_cipher_finish(&handle, &encrypted_data[total_output_length],
                                   sizeof(encrypted_data) - total_output_length,
                                   &output_length);

        total_output_length += output_length;
    }

    /* Setup the decryption object */
    status = psa_cipher_decrypt_setup(&handle_dec, key_handle, alg);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting up cipher operation object");
        goto destroy_key;
    }

    /* From now on, in case of failure we want to abort the decryption op */
    bAbortDecryption = true;

    /* Set the IV for decryption */
    status = psa_cipher_set_iv(&handle_dec, iv, iv_length);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting the IV for decryption");
        goto abort;
    }

    /* Reset total output length */
    total_output_length = 0;
    if (len < BYTE_SIZE_CHUNK) {
        status = psa_cipher_update(&handle_dec,
                                   encrypted_data,
                                   BYTE_SIZE_CHUNK,
                                   decrypted_data,
                                   sizeof(decrypted_data),
                                   &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error decrypting one chunk of information");
            goto abort;
        }

        /* Doesn't produce output on the first cipher update */
        if (output_length != 0) {
            TEST_FAIL("Expected decrypted length is different from expected");
            goto abort;
        }

        status = psa_cipher_finish(&handle_dec, decrypted_data,
                                   sizeof(decrypted_data),
                                   &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error finalising the cipher operation");
            goto abort;
        }

        if (output_length != len) {
            TEST_FAIL("Expected decrypted length is different from expected");
            goto destroy_key;
        }

    } else if (len < 2*BYTE_SIZE_CHUNK) {
        status = psa_cipher_update(&handle_dec, encrypted_data,
                                   BYTE_SIZE_CHUNK,
                                   decrypted_data,
                                   sizeof(decrypted_data),
                                   &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error encrypting one chunk of information");
            goto abort;
        }

        /* Doesn't produce output on the first cipher update */
        if (output_length != 0) {
            TEST_FAIL("Expected decrypted length is different from expected");
            goto abort;
        }

        total_output_length += output_length;
        status = psa_cipher_update(&handle_dec,
                                   &encrypted_data[BYTE_SIZE_CHUNK],
                                   BYTE_SIZE_CHUNK,
                                   &decrypted_data[total_output_length],
                                   sizeof(decrypted_data) - total_output_length,
                                   &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error decrypting one chunk of information");
            goto abort;
        }

        /* We now get the output corresponding to the previous block */
        if (output_length != BYTE_SIZE_CHUNK) {
            TEST_FAIL("Expected decrypted length is different from expected");
            goto abort;
        }

        total_output_length += output_length;
        status = psa_cipher_finish(&handle_dec,
                                   &decrypted_data[total_output_length],
                                   sizeof(decrypted_data) - total_output_length,
                                   &output_length);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error finalising the cipher operation");
            goto abort;
        }

        total_output_length += output_length;
        if (total_output_length != len) {
            TEST_FAIL("Expected decrypted length is different from expected");
            goto destroy_key;
        }
    }

    /* Check that the plain text matches the decrypted data */
    comp_result = compare_buffers(plain_text, decrypted_data, len);
    if (comp_result != 0) {
        TEST_FAIL("Decrypted data doesn't match with plain text");
        goto destroy_key;
    }

    /* Go directly to destroy key from here */
    goto destroy_key;

abort:
    /* Abort the operation */
    status = bAbortDecryption ? psa_cipher_abort(&handle_dec) :
                                psa_cipher_abort(&handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error aborting the operation");
    }
destroy_key:
    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying a key");
    }
}

#ifdef TFM_CRYPTO_TEST_CHACHA20
/* Chacha20 test vectors are taken directly from RFC7539 */
static const uint8_t chacha20_testKey[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

static const uint8_t chacha20_testNonce[] = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x4a,
  0x00, 0x00, 0x00, 0x00
};

/* The initial counter of the Chacha20 RFC7539 test vectors is 1, while the PSA
 * APIs assume it to be zero. This means that this expected ciphertext is not
 * the same as the one presented in the RFC
 */
static const uint8_t chacha20_testCiphertext_expected[] = {
  0xe3, 0x64, 0x7a, 0x29, 0xde, 0xd3, 0x15, 0x28, 0xef, 0x56, 0xba, 0xc7,
  0x0f, 0x7a, 0x7a, 0xc3, 0xb7, 0x35, 0xc7, 0x44, 0x4d, 0xa4, 0x2d, 0x99,
  0x82, 0x3e, 0xf9, 0x93, 0x8c, 0x8e, 0xbf, 0xdc, 0xf0, 0x5b, 0xb7, 0x1a,
  0x82, 0x2c, 0x62, 0x98, 0x1a, 0xa1, 0xea, 0x60, 0x8f, 0x47, 0x93, 0x3f,
  0x2e, 0xd7, 0x55, 0xb6, 0x2d, 0x93, 0x12, 0xae, 0x72, 0x03, 0x76, 0x74,
  0xf3, 0xe9, 0x3e, 0x24, 0x4c, 0x23, 0x28, 0xd3, 0x2f, 0x75, 0xbc, 0xc1,
  0x5b, 0xb7, 0x57, 0x4f, 0xde, 0x0c, 0x6f, 0xcd, 0xf8, 0x7b, 0x7a, 0xa2,
  0x5b, 0x59, 0x72, 0x97, 0x0c, 0x2a, 0xe6, 0xcc, 0xed, 0x86, 0xa1, 0x0b,
  0xe9, 0x49, 0x6f, 0xc6, 0x1c, 0x40, 0x7d, 0xfd, 0xc0, 0x15, 0x10, 0xed,
  0x8f, 0x4e, 0xb3, 0x5d, 0x0d, 0x62
};
#endif /* TFM_CRYPTO_TEST_CHACHA20 */

#if defined(TFM_CRYPTO_TEST_CHACHA20) ||      \
    defined(TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305)
/* The plaintext of the vectors is the same for both Chacha20 and
 * Chacha20-Poly1305
 */
static const uint8_t chacha20_testPlaintext[] = {
  0x4c, 0x61, 0x64, 0x69, 0x65, 0x73, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x47,
  0x65, 0x6e, 0x74, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x20, 0x6f, 0x66, 0x20,
  0x74, 0x68, 0x65, 0x20, 0x63, 0x6c, 0x61, 0x73, 0x73, 0x20, 0x6f, 0x66,
  0x20, 0x27, 0x39, 0x39, 0x3a, 0x20, 0x49, 0x66, 0x20, 0x49, 0x20, 0x63,
  0x6f, 0x75, 0x6c, 0x64, 0x20, 0x6f, 0x66, 0x66, 0x65, 0x72, 0x20, 0x79,
  0x6f, 0x75, 0x20, 0x6f, 0x6e, 0x6c, 0x79, 0x20, 0x6f, 0x6e, 0x65, 0x20,
  0x74, 0x69, 0x70, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x74, 0x68, 0x65, 0x20,
  0x66, 0x75, 0x74, 0x75, 0x72, 0x65, 0x2c, 0x20, 0x73, 0x75, 0x6e, 0x73,
  0x63, 0x72, 0x65, 0x65, 0x6e, 0x20, 0x77, 0x6f, 0x75, 0x6c, 0x64, 0x20,
  0x62, 0x65, 0x20, 0x69, 0x74, 0x2e
};
/* To hold intermediate results in both Chacha20 and Chacha20-Poly1305 */
static uint8_t chacha20_testCiphertext[sizeof(chacha20_testPlaintext)] = {0};
static uint8_t chacha20_testDecryptedtext[sizeof(chacha20_testPlaintext)] = {0};
#endif /* TFM_CRYPTO_TEST_CHACHA20 || TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305 */

#ifdef TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305
/* Chacha20-Poly1305 test vectors are taken directly from RFC7539 */
static const uint8_t chacha20poly1305_testKey[] = {
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
  0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f
};

static const uint8_t chacha20poly1305_testNonce[] = {
  0x07, 0x00, 0x00, 0x00, /* constant */
  0x40, 0x41, 0x42, 0x43, /* IV[0] */
  0x44, 0x45, 0x46, 0x47  /* IV[1] */
};

static const uint8_t chacha20poly1305_testAad[] = {
  0x50, 0x51, 0x52, 0x53, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7
};

static const uint8_t chacha20poly1305_testCiphertext_expected[] = {
  0xd3, 0x1a, 0x8d, 0x34, 0x64, 0x8e, 0x60, 0xdb, 0x7b, 0x86, 0xaf, 0xbc,
  0x53, 0xef, 0x7e, 0xc2, 0xa4, 0xad, 0xed, 0x51, 0x29, 0x6e, 0x08, 0xfe,
  0xa9, 0xe2, 0xb5, 0xa7, 0x36, 0xee, 0x62, 0xd6, 0x3d, 0xbe, 0xa4, 0x5e,
  0x8c, 0xa9, 0x67, 0x12, 0x82, 0xfa, 0xfb, 0x69, 0xda, 0x92, 0x72, 0x8b,
  0x1a, 0x71, 0xde, 0x0a, 0x9e, 0x06, 0x0b, 0x29, 0x05, 0xd6, 0xa5, 0xb6,
  0x7e, 0xcd, 0x3b, 0x36, 0x92, 0xdd, 0xbd, 0x7f, 0x2d, 0x77, 0x8b, 0x8c,
  0x98, 0x03, 0xae, 0xe3, 0x28, 0x09, 0x1b, 0x58, 0xfa, 0xb3, 0x24, 0xe4,
  0xfa, 0xd6, 0x75, 0x94, 0x55, 0x85, 0x80, 0x8b, 0x48, 0x31, 0xd7, 0xbc,
  0x3f, 0xf4, 0xde, 0xf0, 0x8e, 0x4b, 0x7a, 0x9d, 0xe5, 0x76, 0xd2, 0x65,
  0x86, 0xce, 0xc6, 0x4b, 0x61, 0x16
};

static const uint8_t chacha20poly1305_testTag_expected[] = {
  0x1a, 0xe1, 0x0b, 0x59, 0x4f, 0x09, 0xe2, 0x6a,
  0x7e, 0x90, 0x2e, 0xcb, 0xd0, 0x60, 0x06, 0x91
};
#endif /* TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305 */

#define LOG_INF(...) printf(__VA_ARGS__)
#define LOG_HEXDUMP_INF(a,b,c)      \
    do {                            \
       LOG_INF(c);            \
       for (int i=0; i<b; i++) {    \
           LOG_INF("0x%x, ", a[i]); \
       }                            \
       printf("\r\n");              \
    }                               \
    while (0)

#ifdef TFM_CRYPTO_TEST_CHACHA20
void psa_cipher_rfc7539_test(struct test_result_t *ret)
{
    psa_cipher_operation_t handle = psa_cipher_operation_init();
    psa_cipher_operation_t handle_dec = psa_cipher_operation_init();
    psa_status_t status = PSA_SUCCESS;
    psa_key_handle_t key_handle;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_usage_t usage = (PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    const psa_key_type_t key_type = PSA_KEY_TYPE_CHACHA20;
    const psa_algorithm_t alg = PSA_ALG_STREAM_CIPHER;
    bool bAbortDecryption = false;
    /* Variables required during multipart update */
    size_t data_left = sizeof(chacha20_testPlaintext);
    size_t lengths[] = {42, 24, 48};
    size_t start_idx = 0;
    size_t output_length = 0; size_t total_output_length = 0;
    int comp_result;

    ret->val = TEST_PASSED;

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, key_type);
    psa_set_key_id(&key_attributes, CIPHER_TEST_KEY_ID);
    status = psa_import_key(&key_attributes, chacha20_testKey,
                            sizeof(chacha20_testKey), &key_handle);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    /* Setup the encryption object */
    status = psa_cipher_encrypt_setup(&handle, key_handle, alg);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Encryption setup shouldn't fail");
        goto destroy_key;
    }

    /* Set the IV */
    status = psa_cipher_set_iv(&handle,
                               chacha20_testNonce, sizeof(chacha20_testNonce));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting the IV on the cipher operation object");
        goto abort;
    }

    for (int i=0; i<sizeof(lengths)/sizeof(size_t); i++) {
        /* Encrypt one chunk of information */
        status = psa_cipher_update(
                        &handle,
                        &chacha20_testPlaintext[start_idx],
                        lengths[i],
                        &chacha20_testCiphertext[total_output_length],
                        sizeof(chacha20_testCiphertext) - total_output_length,
                        &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error encrypting one chunk of information");
            goto abort;
        }

        if (output_length != lengths[i]) {
            TEST_FAIL("Expected encrypted length is different from expected");
            goto abort;
        }

        data_left -= lengths[i];
        total_output_length += output_length;

        start_idx += lengths[i];
    }

    /* Finalise the cipher operation */
    status = psa_cipher_finish(
                    &handle,
                    &chacha20_testCiphertext[total_output_length],
                    sizeof(chacha20_testCiphertext) - total_output_length,
                    &output_length);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error finalising the cipher operation");
        goto abort;
    }

    if (output_length != 0) {
        TEST_FAIL("Un-padded mode final output length unexpected");
        goto abort;
    }

    /* Add the last output produced, it might be encrypted padding */
    total_output_length += output_length;

    /* Compare encrypted data produced with single-shot and multipart APIs */
    comp_result = compare_buffers(chacha20_testCiphertext_expected,
                                  chacha20_testCiphertext,
                                  total_output_length);
    if (comp_result != 0) {
        TEST_FAIL("Single-shot crypt doesn't match with multipart crypt");
        goto destroy_key;
    }

    /* Setup the decryption object */
    status = psa_cipher_decrypt_setup(&handle_dec, key_handle, alg);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting up cipher operation object");
        goto destroy_key;
    }

    /* From now on, in case of failure we want to abort the decryption op */
    bAbortDecryption = true;

    /* Set the IV for decryption */
    status = psa_cipher_set_iv(&handle_dec,
                               chacha20_testNonce, sizeof(chacha20_testNonce));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting the IV for decryption");
        goto abort;
    }

    /* Decrypt - total_output_length considers encrypted padding */
    data_left = total_output_length;
    /* Update in different chunks of plainText */
    lengths[0] = 14; lengths[1] = 70; lengths[2] = 30;
    start_idx = 0;
    output_length = 0; total_output_length = 0;
    for (int i=0; i<sizeof(lengths)/sizeof(size_t); i++) {
        status = psa_cipher_update(
                    &handle_dec,
                    &chacha20_testCiphertext[start_idx],
                    lengths[i],
                    &chacha20_testDecryptedtext[total_output_length],
                    sizeof(chacha20_testDecryptedtext) - total_output_length,
                    &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error decrypting one chunk of information");
            goto abort;
        }

        if (output_length != lengths[i]) {
            TEST_FAIL("Expected encrypted length is different from expected");
            goto abort;
        }

        data_left -= lengths[i];
        total_output_length += output_length;

        start_idx += lengths[i];
    }

    /* Finalise the cipher operation for decryption */
    status = psa_cipher_finish(
                    &handle_dec,
                    &chacha20_testDecryptedtext[total_output_length],
                    sizeof(chacha20_testDecryptedtext) - total_output_length,
                    &output_length);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error finalising the cipher operation");
        goto abort;
    }

    /* Finalize the count of output which has been produced */
    total_output_length += output_length;

    /* Check that the decrypted length is equal to the original length */
    if (total_output_length != sizeof(chacha20_testPlaintext)) {
        TEST_FAIL("After finalising, unexpected decrypted length");
        goto destroy_key;
    }

    /* Check that the plain text matches the decrypted data */
    comp_result = compare_buffers(chacha20_testPlaintext,
                                  chacha20_testDecryptedtext,
                                  sizeof(chacha20_testPlaintext));
    if (comp_result != 0) {
        TEST_FAIL("Decrypted data doesn't match with plain text");
    }

    /* Go directly to the destroy_key label at this point */
    goto destroy_key;

abort:
    /* Abort the operation */
    status = bAbortDecryption ? psa_cipher_abort(&handle_dec) :
                                psa_cipher_abort(&handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error aborting the operation");
    }
destroy_key:
    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying a key");
    }

    return;
}
#endif /* TFM_CRYPTO_TEST_CHACHA20 */

#ifdef TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305
void psa_aead_rfc7539_test(struct test_result_t *ret)
{
    psa_aead_operation_t handle = psa_aead_operation_init();
    psa_aead_operation_t handle_dec = psa_aead_operation_init();
    psa_status_t status = PSA_SUCCESS;
    psa_key_handle_t key_handle;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_usage_t usage = (PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    const psa_key_type_t key_type = PSA_KEY_TYPE_CHACHA20;
    const psa_algorithm_t alg = PSA_ALG_CHACHA20_POLY1305;
    uint8_t tag[16] = {0}; /* tag in chacha20-poly1305 is 16 bytes */
    size_t tag_length = 0;
    bool bAbortDecryption = false;
    /* Variables related to multipart update */
    size_t data_left = sizeof(chacha20_testPlaintext);
    size_t lengths[] = {42, 24, 48};
    size_t start_idx = 0;
    size_t output_length = 0; size_t total_output_length = 0;
    int comp_result;

    ret->val = TEST_PASSED;

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, key_type);
    status = psa_import_key(&key_attributes, chacha20poly1305_testKey,
                            sizeof(chacha20poly1305_testKey), &key_handle);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    /* Setup the encryption object */
    status = psa_aead_encrypt_setup(&handle, key_handle, alg);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Encryption setup shouldn't fail");
        goto destroy_key;
    }

    /* Set the IV */
    status = psa_aead_set_nonce(&handle,
                                chacha20poly1305_testNonce,
                                sizeof(chacha20poly1305_testNonce));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting the nonce on the aead operation object");
        goto abort;
    }

    /* Set lengths */
    status = psa_aead_set_lengths(&handle,
                                  sizeof(chacha20poly1305_testAad),
                                  sizeof(chacha20_testPlaintext));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting the lengths on the aead operation object");
        goto abort;
    }

    /* Update AD in one go */
    status = psa_aead_update_ad(&handle,
                                chacha20poly1305_testAad,
                                sizeof(chacha20poly1305_testAad));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error updating AD");
        goto abort;
    }

    for (int i=0; i<sizeof(lengths)/sizeof(size_t); i++) {
        /* Encrypt one chunk of information */
        status = psa_aead_update(
                        &handle,
                        &chacha20_testPlaintext[start_idx],
                        lengths[i],
                        &chacha20_testCiphertext[total_output_length],
                        sizeof(chacha20_testCiphertext) - total_output_length,
                        &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error encrypting one chunk of information");
            goto abort;
        }

        if (output_length != lengths[i]) {
            TEST_FAIL("Expected encrypted length is different from expected");
            goto abort;
        }

        data_left -= lengths[i];
        total_output_length += output_length;

        start_idx += lengths[i];
    }

    /* Finalise the cipher operation */
    status = psa_aead_finish(
                    &handle,
                    &chacha20_testCiphertext[total_output_length],
                    sizeof(chacha20_testCiphertext) - total_output_length,
                    &output_length,
                    tag,
                    sizeof(tag),
                    &tag_length);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error finalising the cipher operation");
        goto abort;
    }

    if (output_length != 0) {
        TEST_FAIL("Un-padded mode final output length unexpected");
        goto abort;
    }

    if (tag_length != 16) {
        TEST_FAIL("Unexpected tag length different than 16");
        goto abort;
    }

    /* Add the last output produced, it might be encrypted padding */
    total_output_length += output_length;

    /* Compare encrypted data produced with single-shot and multipart APIs */
    comp_result = compare_buffers(chacha20poly1305_testCiphertext_expected,
                                  chacha20_testCiphertext,
                                  total_output_length);
    if (comp_result != 0) {
        TEST_FAIL("Encrypted data does not match reference data");
        goto destroy_key;
    }

    comp_result = compare_buffers(chacha20poly1305_testTag_expected,
                                  tag, tag_length);
    if (comp_result != 0) {
        TEST_FAIL("Computed tag does not match reference data");
        goto destroy_key;
    }

    /* Setup the decryption object */
    status = psa_aead_decrypt_setup(&handle_dec, key_handle, alg);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting up aead operation object");
        goto destroy_key;
    }

    /* From now on, in case of failure we want to abort the decryption op */
    bAbortDecryption = true;

    /* Set the IV for decryption */
    status = psa_aead_set_nonce(&handle_dec,
                                chacha20poly1305_testNonce,
                                sizeof(chacha20poly1305_testNonce));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting the nonce for decryption");
        goto abort;
    }

    /* Set lengths */
    status = psa_aead_set_lengths(&handle_dec,
                                  sizeof(chacha20poly1305_testAad),
                                  sizeof(chacha20_testPlaintext));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting the lengths on the aead operation object");
        goto abort;
    }

    /* Update AD in one go */
    status = psa_aead_update_ad(&handle_dec,
                                chacha20poly1305_testAad,
                                sizeof(chacha20poly1305_testAad));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error updating AD");
        goto abort;
    }

    /* Decrypt - total_output_length considers encrypted padding */
    data_left = total_output_length;
    /* Update in different chunks of plainText */
    lengths[0] = 14; lengths[1] = 70; lengths[2] = 30;
    start_idx = 0;
    output_length = 0; total_output_length = 0;
    for (int i=0; i<sizeof(lengths)/sizeof(size_t); i++) {
        status = psa_aead_update(
                    &handle_dec,
                    &chacha20_testCiphertext[start_idx],
                    lengths[i],
                    &chacha20_testDecryptedtext[total_output_length],
                    sizeof(chacha20_testDecryptedtext) - total_output_length,
                    &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error decrypting one chunk of information");
            goto abort;
        }

        if (output_length != lengths[i]) {
            TEST_FAIL("Expected encrypted length is different from expected");
            goto abort;
        }

        data_left -= lengths[i];
        total_output_length += output_length;

        start_idx += lengths[i];
    }

    /* Finalise the cipher operation for decryption (destroys decrypted data) */
    status = psa_aead_verify(
                    &handle_dec,
                    &chacha20_testDecryptedtext[total_output_length],
                    sizeof(chacha20_testDecryptedtext) - total_output_length,
                    &output_length,
                    tag,
                    tag_length);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error verifying the aead operation");
        goto abort;
    }

    /* Finalize the count of output which has been produced */
    total_output_length += output_length;

    /* Check that the decrypted length is equal to the original length */
    if (total_output_length != sizeof(chacha20_testPlaintext)) {
        TEST_FAIL("After finalising, unexpected decrypted length");
        goto destroy_key;
    }

    /* Check that the plain text matches the decrypted data */
    comp_result = compare_buffers(chacha20_testPlaintext,
                                  chacha20_testDecryptedtext,
                                  sizeof(chacha20_testPlaintext));
    if (comp_result != 0) {
        TEST_FAIL("Decrypted data doesn't match with plain text");
    }

    /* Go directly to the destroy_key label at this point */
    goto destroy_key;

abort:
    /* Abort the operation */
    status = bAbortDecryption ? psa_aead_abort(&handle_dec) :
                                psa_aead_abort(&handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error aborting the operation");
    }
destroy_key:
    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying a key");
    }

    return;
}
#endif /* TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305 */

void psa_cipher_test(const psa_key_type_t key_type,
                     const psa_algorithm_t alg,
                     const uint8_t *key,
                     size_t key_bits,
                     struct test_result_t *ret)
{
    psa_cipher_operation_t handle = psa_cipher_operation_init();
    psa_cipher_operation_t handle_dec = psa_cipher_operation_init();
    psa_status_t status = PSA_SUCCESS;
    psa_key_handle_t key_handle;
    size_t iv_length = PSA_CIPHER_IV_LENGTH(key_type, alg);
    uint8_t iv[16] = {0};
    const uint8_t plain_text[PLAIN_DATA_SIZE] =
        "This is my plaintext to encrypt, 48 bytes long!";
    uint8_t encrypted_data_single_shot[ENC_DEC_BUFFER_SIZE];
    uint8_t decrypted_data[ENC_DEC_BUFFER_SIZE] = {0};
    size_t output_length = 0, total_output_length = 0;
    union {
        uint8_t encrypted_data[ENC_DEC_BUFFER_SIZE];
        uint8_t encrypted_data_pad[ENC_DEC_BUFFER_SIZE_PAD_MODES];
    } input = {0};
    uint32_t comp_result;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_usage_t usage = (PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    bool bAbortDecryption = false;

    if (iv_length > 16) {
        TEST_FAIL("Unexpected IV length greater than 16 for this alg/key type");
        return;
    }

    ret->val = TEST_PASSED;

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, key_type);
    psa_set_key_id(&key_attributes, CIPHER_TEST_KEY_ID);

    /* Import a key */
    status = psa_import_key(&key_attributes, key, PSA_BITS_TO_BYTES(key_bits),
                            &key_handle);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    status = psa_get_key_attributes(key_handle, &key_attributes);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error getting key metadata");
        goto destroy_key;
    }

    if (psa_get_key_bits(&key_attributes) != key_bits) {
        TEST_FAIL("The number of key bits is different from expected");
        goto destroy_key;
    }

    if (psa_get_key_type(&key_attributes) != key_type) {
        TEST_FAIL("The type of the key is different from expected");
        goto destroy_key;
    }

    psa_reset_key_attributes(&key_attributes);

    /* Encrypt single part functions */
    status = psa_cipher_encrypt(CIPHER_TEST_KEY_ID, alg, plain_text,
                                sizeof(plain_text),
                                input.encrypted_data_pad,
                                sizeof(input.encrypted_data_pad),
                                &output_length);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error encrypting with the single-shot API");
        goto destroy_key;
    }

    /* Store a copy of the encrypted data for later checking it against
     * multipart results
     */
#if DOMAIN_NS == 1U
    memcpy(encrypted_data_single_shot, &input.encrypted_data_pad[iv_length],
           output_length-iv_length);
#else
    tfm_memcpy(encrypted_data_single_shot, &input.encrypted_data_pad[iv_length],
               output_length-iv_length);
#endif

    /* Make sure to use the randomly generated IV for the multipart flow */
    for (int i=0; i<iv_length; i++) {
        iv[i] = input.encrypted_data_pad[i];
    }

    status = psa_cipher_decrypt(CIPHER_TEST_KEY_ID, alg,
                                input.encrypted_data_pad,
                                output_length,
                                decrypted_data, ENC_DEC_BUFFER_SIZE,
                                &output_length);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error decrypting with the single shot API");
        goto destroy_key;
    }

    if (sizeof(plain_text) != output_length) {
        TEST_FAIL("Unexpected output length");
        goto destroy_key;
    }

    /* Check that the plain text matches the decrypted data */
    comp_result = compare_buffers(plain_text, decrypted_data,
                                  sizeof(plain_text));
    if (comp_result != 0) {
        TEST_FAIL("Decrypted data doesn't match with plain text");
        goto destroy_key;
    }

    /* Clear inputs/outputs before moving to multipart tests */
#if DOMAIN_NS == 1U
    /* Clear intermediate buffers for additional single-shot API tests */
    memset(input.encrypted_data_pad, 0, sizeof(input.encrypted_data_pad));
    memset(decrypted_data, 0, sizeof(decrypted_data));
#else
    tfm_memset(input.encrypted_data_pad, 0, sizeof(input.encrypted_data_pad));
    tfm_memset(decrypted_data, 0, sizeof(decrypted_data));
#endif
    /* Replicate the same test as above, but now using the multipart APIs */

    /* Setup the encryption object */
    status = psa_cipher_encrypt_setup(&handle, key_handle, alg);
    if (status != PSA_SUCCESS) {
        if (status == PSA_ERROR_NOT_SUPPORTED) {
            TEST_FAIL("Algorithm NOT SUPPORTED by the implementation");
        } else {
            TEST_FAIL("Error setting up cipher operation object");
        }
        goto destroy_key;
    }

    /* Set the IV */
    if (alg != PSA_ALG_ECB_NO_PADDING) {
        status = psa_cipher_set_iv(&handle, iv, iv_length);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error setting the IV on the cipher operation object");
            goto abort;
        }
    }

    size_t data_left = sizeof(plain_text);
    while (data_left) {
        /* Encrypt one chunk of information */
        status = psa_cipher_update(&handle, &plain_text[total_output_length],
                                   BYTE_SIZE_CHUNK,
                                   &input.encrypted_data[total_output_length],
                                   ENC_DEC_BUFFER_SIZE - total_output_length,
                                   &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error encrypting one chunk of information");
            goto abort;
        }

        if (output_length != BYTE_SIZE_CHUNK) {
            TEST_FAIL("Expected encrypted length is different from expected");
            goto abort;
        }

        data_left -= BYTE_SIZE_CHUNK;
        total_output_length += output_length;
    }

    /* Finalise the cipher operation */
    status = psa_cipher_finish(&handle,
                               &input.encrypted_data[total_output_length],
                               ENC_DEC_BUFFER_SIZE - total_output_length,
                               &output_length);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error finalising the cipher operation");
        goto abort;
    }

    if (alg == PSA_ALG_CBC_PKCS7) {
        /* Finalisation produces an output for padded modes, which is the
         * encryption of the padded data added
         */
        if (output_length != BYTE_SIZE_CHUNK) {
            TEST_FAIL("Padded mode final output length unexpected");
            goto abort;
        }
    } else {
        if (output_length != 0) {
            TEST_FAIL("Un-padded mode final output length unexpected");
            goto abort;
        }
    }

    /* Add the last output produced, it might be encrypted padding */
    total_output_length += output_length;

    /* Compare encrypted data produced with single-shot and multipart APIs */
    comp_result = compare_buffers(encrypted_data_single_shot,
                                  input.encrypted_data,
                                  total_output_length);
    if (comp_result != 0) {
        TEST_FAIL("Single-shot crypt doesn't match with multipart crypt");
        goto destroy_key;
    }

    /* Setup the decryption object */
    status = psa_cipher_decrypt_setup(&handle_dec, key_handle, alg);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting up cipher operation object");
        goto destroy_key;
    }

    /* From now on, in case of failure we want to abort the decryption op */
    bAbortDecryption = true;

    /* Set the IV for decryption */
    if (alg != PSA_ALG_ECB_NO_PADDING) {
        status = psa_cipher_set_iv(&handle_dec, iv, iv_length);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error setting the IV for decryption");
            goto abort;
        }
    }

    /* Padded mode output is produced one block later */
    bool bIsLagging = false;
    if (alg == PSA_ALG_CBC_PKCS7) {
        bIsLagging = true; /* Padded modes lag by 1 block */
    }

    /* Decrypt - total_output_length considers encrypted padding */
    data_left = total_output_length;
    total_output_length = 0;
    size_t message_start = 0;
    while (data_left) {
        status = psa_cipher_update(&handle_dec,
                                   &input.encrypted_data[message_start],
                                   BYTE_SIZE_CHUNK,
                                   &decrypted_data[total_output_length],
                                   (ENC_DEC_BUFFER_SIZE - total_output_length),
                                   &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error decrypting one chunk of information");
            goto abort;
        }

        if (!bIsLagging && output_length != BYTE_SIZE_CHUNK) {
            TEST_FAIL("Expected encrypted length is different from expected");
            goto abort;
        }

        message_start += BYTE_SIZE_CHUNK;
        data_left -= BYTE_SIZE_CHUNK;
        total_output_length += output_length;
        bIsLagging = false;
    }

    /* Finalise the cipher operation for decryption (destroys decrypted data) */
    status = psa_cipher_finish(&handle_dec, &decrypted_data[total_output_length],
                               BYTE_SIZE_CHUNK,
                               &output_length);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error finalising the cipher operation");
        goto abort;
    }

    /* Finalize the count of output which has been produced */
    total_output_length += output_length;

    /* Check that the decrypted length is equal to the original length */
    if (total_output_length != 3*BYTE_SIZE_CHUNK) {
        TEST_FAIL("After finalising, unexpected decrypted length");
        goto destroy_key;
    }

    /* Check that the plain text matches the decrypted data */
    comp_result = compare_buffers(plain_text, decrypted_data,
                                  sizeof(plain_text));
    if (comp_result != 0) {
        TEST_FAIL("Decrypted data doesn't match with plain text");
    }

    /* Go directly to the destroy_key label at this point */
    goto destroy_key;

abort:
    /* Abort the operation */
    status = bAbortDecryption ? psa_cipher_abort(&handle_dec) :
                                psa_cipher_abort(&handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error aborting the operation");
    }
destroy_key:
    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying a key");
    }
}

void psa_invalid_cipher_test(const psa_key_type_t key_type,
                             const psa_algorithm_t alg,
                             const size_t key_size,
                             struct test_result_t *ret)
{
    psa_status_t status;
    psa_cipher_operation_t handle = psa_cipher_operation_init();
    psa_key_handle_t key_handle;
    uint8_t data[TEST_MAX_KEY_LENGTH];
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_usage_t usage = (PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, key_type);

#if DOMAIN_NS == 1U
    /* Fill the key data */
    (void)memset(data, 'A', key_size);
#else
    (void)tfm_memset(data, 'A', key_size);
#endif

    /* Import a key */
    status = psa_import_key(&key_attributes, data, key_size, &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    /* Setup the encryption object */
    status = psa_cipher_encrypt_setup(&handle, key_handle, alg);
    if (status == PSA_SUCCESS) {
        TEST_FAIL("Should not successfully setup an invalid cipher");
        (void)psa_destroy_key(key_handle);
        return;
    }

    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying a key");
        return;
    }

    ret->val = TEST_PASSED;
}

void psa_unsupported_hash_test(const psa_algorithm_t alg,
                               struct test_result_t *ret)
{
    psa_status_t status;
    psa_hash_operation_t handle = PSA_HASH_OPERATION_INIT;

    /* Setup the hash object for the unsupported hash algorithm */
    status = psa_hash_setup(&handle, alg);
    if (status != PSA_ERROR_NOT_SUPPORTED) {
        TEST_FAIL("Should not successfully setup an unsupported hash alg");
        return;
    }

    ret->val = TEST_PASSED;
}

/*
 * \brief This is the list of algorithms supported by the current
 *        configuration of the crypto engine used by the crypto
 *        service. In case the crypto engine default capabilities
 *        is changed, this list needs to be updated accordingly
 */
static const psa_algorithm_t hash_alg[] = {
    PSA_ALG_SHA_224,
    PSA_ALG_SHA_256,
    PSA_ALG_SHA_384,
    PSA_ALG_SHA_512,
};

static const uint8_t hash_val[][PSA_HASH_LENGTH(PSA_ALG_SHA_512)] = {
    {0x00, 0xD2, 0x90, 0xE2, 0x0E, 0x4E, 0xC1, 0x7E, /*!< SHA-224 */
     0x7A, 0x95, 0xF5, 0x10, 0x5C, 0x76, 0x74, 0x04,
     0x6E, 0xB5, 0x56, 0x5E, 0xE5, 0xE7, 0xBA, 0x15,
     0x6C, 0x23, 0x47, 0xF3},
    {0x6B, 0x22, 0x09, 0x2A, 0x37, 0x1E, 0xF5, 0x14, /*!< SHA-256 */
     0xF7, 0x39, 0x4D, 0xCF, 0xAD, 0x4D, 0x17, 0x46,
     0x66, 0xCB, 0x33, 0xA0, 0x39, 0xD8, 0x41, 0x4E,
     0xF1, 0x2A, 0xD3, 0x4D, 0x69, 0xC3, 0xB5, 0x3E},
    {0x64, 0x79, 0x11, 0xBB, 0x47, 0x4E, 0x47, 0x59, /*!< SHA-384 */
     0x3E, 0x4D, 0xBC, 0x60, 0xA5, 0xF9, 0xBF, 0x9C,
     0xC0, 0xBA, 0x55, 0x0F, 0x93, 0xCA, 0x72, 0xDF,
     0x57, 0x1E, 0x50, 0x56, 0xF9, 0x4A, 0x01, 0xD6,
     0xA5, 0x6F, 0xF7, 0x62, 0x34, 0x4F, 0x48, 0xFD,
     0x9D, 0x15, 0x07, 0x42, 0xB7, 0x72, 0x94, 0xB8},
    {0xB4, 0x1C, 0xA3, 0x6C, 0xA9, 0x67, 0x1D, 0xAD, /*!< SHA-512 */
     0x34, 0x1F, 0xBE, 0x1B, 0x83, 0xC4, 0x40, 0x2A,
     0x47, 0x42, 0x79, 0xBB, 0x21, 0xCA, 0xF0, 0x60,
     0xE4, 0xD2, 0x6E, 0x9B, 0x70, 0x12, 0x34, 0x3F,
     0x55, 0x2C, 0x09, 0x31, 0x0A, 0x5B, 0x40, 0x21,
     0x01, 0xA8, 0x3B, 0x58, 0xE7, 0x48, 0x13, 0x1A,
     0x7E, 0xCD, 0xE1, 0xD2, 0x46, 0x10, 0x58, 0x34,
     0x49, 0x14, 0x4B, 0xAA, 0x89, 0xA9, 0xF5, 0xB1},
};

void psa_hash_test(const psa_algorithm_t alg,
                   struct test_result_t *ret)
{
    const char *msg =
        "This is my test message, please generate a hash for this.";
    /* Length of each chunk in the multipart API */
    const size_t msg_size[] = {25, 32};
    const uint32_t msg_num = sizeof(msg_size)/sizeof(msg_size[0]);
    uint32_t idx, start_idx = 0;

    psa_status_t status;
    psa_hash_operation_t handle = psa_hash_operation_init();

    /* Setup the hash object for the desired hash*/
    status = psa_hash_setup(&handle, alg);

    if (status != PSA_SUCCESS) {
        if (status == PSA_ERROR_NOT_SUPPORTED) {
            TEST_FAIL("Algorithm NOT SUPPORTED by the implementation");
            return;
        }

        TEST_FAIL("Error setting up hash operation object");
        return;
    }

    /* Update object with all the chunks of message */
    for (idx=0; idx<msg_num; idx++) {
        status = psa_hash_update(&handle,
                                 (const uint8_t *)&msg[start_idx],
                                 msg_size[idx]);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error updating the hash operation object");
            return;
        }
        start_idx += msg_size[idx];
    }

    /* Cycle until idx points to the correct index in the algorithm table */
    for (idx=0; hash_alg[idx] != alg; idx++);

    /* Finalise and verify that the hash is as expected */
    status = psa_hash_verify(&handle, hash_val[idx], PSA_HASH_LENGTH(alg));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error verifying the hash operation object");
        return;
    }

    /* Do the same as above with the single shot APIs */
    status = psa_hash_compare(alg,
                              (const uint8_t *)msg, strlen(msg),
                              hash_val[idx], PSA_HASH_LENGTH(alg));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error using the single shot API");
        return;
    }

    ret->val = TEST_PASSED;
}

void psa_unsupported_mac_test(const psa_key_type_t key_type,
                              const psa_algorithm_t alg,
                              struct test_result_t *ret)
{
    psa_status_t status;
    psa_key_handle_t key_handle;
    psa_mac_operation_t handle = PSA_MAC_OPERATION_INIT;
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
    const uint8_t data[] = "THIS IS MY KEY1";

    ret->val = TEST_PASSED;

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_VERIFY_HASH);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, key_type);

    /* Import key */
    status = psa_import_key(&key_attributes, data, sizeof(data), &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    /* Setup the mac object for the unsupported mac algorithm */
    status = psa_mac_verify_setup(&handle, key_handle, alg);
    if (status != PSA_ERROR_NOT_SUPPORTED) {
        TEST_FAIL("Should not successfully setup an unsupported MAC alg");
        /* Do not return, to ensure key is destroyed */
    }

    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying the key");
    }
}

static const uint8_t hmac_val[][PSA_HASH_LENGTH(PSA_ALG_SHA_512)] = {
    {0xc1, 0x9f, 0x19, 0xac, 0x05, 0x65, 0x5f, 0x02, /*!< SHA-224 */
     0x1b, 0x64, 0x32, 0xd9, 0xb1, 0x49, 0xba, 0x75,
     0x05, 0x60, 0x52, 0x4e, 0x78, 0xfa, 0x61, 0xc9,
     0x37, 0x5d, 0x7f, 0x58},
    {0x94, 0x37, 0xbe, 0xb5, 0x7f, 0x7c, 0x5c, 0xb0, /*!< SHA-256 */
     0x0a, 0x92, 0x4d, 0xd3, 0xba, 0x7e, 0xb1, 0x1a,
     0xdb, 0xa2, 0x25, 0xb2, 0x82, 0x8e, 0xdf, 0xbb,
     0x61, 0xbf, 0x91, 0x1d, 0x28, 0x23, 0x4a, 0x04},
    {0x94, 0x21, 0x9b, 0xc3, 0xd5, 0xed, 0xe6, 0xee, /*!< SHA-384 */
     0x42, 0x10, 0x5a, 0x58, 0xa4, 0x4d, 0x67, 0x87,
     0x16, 0xa2, 0xa7, 0x6c, 0x2e, 0xc5, 0x85, 0xb7,
     0x6a, 0x4c, 0x90, 0xb2, 0x73, 0xee, 0x58, 0x3c,
     0x59, 0x16, 0x67, 0xf3, 0x6f, 0x30, 0x99, 0x1c,
     0x2a, 0xf7, 0xb1, 0x5f, 0x45, 0x83, 0xf5, 0x9f},
    {0x8f, 0x76, 0xef, 0x12, 0x0b, 0x92, 0xc2, 0x06, /*!< SHA-512 */
     0xce, 0x01, 0x18, 0x75, 0x84, 0x96, 0xd9, 0x6f,
     0x23, 0x88, 0xd4, 0xf8, 0xcf, 0x79, 0xf8, 0xcf,
     0x27, 0x12, 0x9f, 0xa6, 0x7e, 0x87, 0x9a, 0x68,
     0xee, 0xe2, 0xe7, 0x1d, 0x4b, 0xf2, 0x87, 0xc0,
     0x05, 0x6a, 0xbd, 0x7f, 0x9d, 0xff, 0xaa, 0xf3,
     0x9a, 0x1c, 0xb7, 0xb7, 0xbd, 0x03, 0x61, 0xa3,
     0xa9, 0x6a, 0x5d, 0xb2, 0x81, 0xe1, 0x6f, 0x1f},
};

static const uint8_t long_key_hmac_val[PSA_HASH_LENGTH(PSA_ALG_SHA_224)] = {
    0x47, 0xa3, 0x42, 0xb1, 0x2f, 0x52, 0xd3, 0x8f, /*!< SHA-224 */
    0x1e, 0x02, 0x4a, 0x46, 0x73, 0x0b, 0x77, 0xc1,
    0x5e, 0x93, 0x31, 0xa9, 0x3e, 0xc2, 0x81, 0xb5,
    0x3d, 0x07, 0x6f, 0x31
};

#define MAC_TEST_KEY_ID (0x1)

void psa_mac_test(const psa_algorithm_t alg,
                  const uint8_t *key,
                  size_t key_bits,
                  struct test_result_t *ret)
{
    const char *msg =
        "This is my test message, please generate a hmac for this.";
    /* Length of each chunk in the multipart API */
    const size_t msg_size[] = {25, 32};
    const uint32_t msg_num = sizeof(msg_size)/sizeof(msg_size[0]);
    uint32_t idx, start_idx = 0;
    uint8_t *hmac_res;

    psa_key_handle_t key_handle;
    psa_key_type_t key_type = PSA_KEY_TYPE_HMAC;
    psa_status_t status;
    psa_mac_operation_t handle = psa_mac_operation_init();
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_attributes_t retrieved_attributes = psa_key_attributes_init();
    psa_key_usage_t usage = PSA_KEY_USAGE_VERIFY_HASH;

    ret->val = TEST_PASSED;

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, key_type);
    psa_set_key_id(&key_attributes, MAC_TEST_KEY_ID);

    /* Import key */
    status = psa_import_key(&key_attributes, key, PSA_BITS_TO_BYTES(key_bits),
                            &key_handle);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    status = psa_get_key_attributes(key_handle, &retrieved_attributes);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error getting key metadata");
        goto destroy_key_mac;
    }

    if (psa_get_key_bits(&retrieved_attributes) != key_bits) {
        TEST_FAIL("The number of key bits is different from expected");
        goto destroy_key_mac;
    }

    if (psa_get_key_type(&retrieved_attributes) != key_type) {
        TEST_FAIL("The type of the key is different from expected");
        goto destroy_key_mac;
    }

    psa_reset_key_attributes(&retrieved_attributes);

    /* Setup the mac object for hmac */
    status = psa_mac_verify_setup(&handle, key_handle, alg);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting up mac operation object");
        goto destroy_key_mac;
    }

    /* Update object with all the chunks of message */
    for (idx=0; idx<msg_num; idx++) {
        status = psa_mac_update(&handle,
                                (const uint8_t *)&msg[start_idx],
                                msg_size[idx]);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error during mac operation");
            goto destroy_key_mac;
        }
        start_idx += msg_size[idx];
    }

    /* Cycle until idx points to the correct index in the algorithm table */
    for (idx=0; hash_alg[idx] != PSA_ALG_HMAC_GET_HASH(alg); idx++);

    if (key_bits == BIT_SIZE_TEST_LONG_KEY) {
        hmac_res = (uint8_t *)long_key_hmac_val;
    } else {
        hmac_res = (uint8_t *)hmac_val[idx];
    }

    /* Finalise and verify the mac value */
    status = psa_mac_verify_finish(&handle, hmac_res,
                                   PSA_HASH_LENGTH(PSA_ALG_HMAC_GET_HASH(alg)));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error during finalising the mac operation");
        goto destroy_key_mac;
    }

    /* Do the same as above with the single shot APIs */
    status = psa_mac_verify(MAC_TEST_KEY_ID, alg,
                            (const uint8_t *)msg, strlen(msg),
                            hmac_res,
                            PSA_HASH_LENGTH(PSA_ALG_HMAC_GET_HASH(alg)));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error using the single shot API");
    }

destroy_key_mac:
    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying the key");
    }
}

static const uint8_t chacha20_poly1305_ref_encrypted[] = {
0xae, 0x42, 0xf0, 0xd7, 0x3f, 0x7b, 0xe4, 0xaa,
0xb7, 0x50, 0xe0, 0xd6, 0x66, 0x12, 0xe8, 0x5f,
0x27, 0x51, 0x7d, 0xcb, 0x4f, 0x09, 0xd6, 0x98,
0x83, 0x08, 0xda, 0x16, 0xb7, 0xf4, 0xb7, 0xb0,
0xda, 0x88, 0xa9, 0xe8, 0xc0, 0x02, 0x62, 0xea,
0xa6, 0xcd, 0xc2, 0x10, 0x05, 0x17, 0x56, 0x77,
0xd7, 0xd7, 0x4e, 0xca, 0x7d, 0x96, 0xc1, 0xd1,
0xd9, 0x46, 0xd8, 0xcd, 0x95, 0xf3, 0x47, 0xd1,
0x55, 0xb7, 0xbf, 0x7e, 0x5d, 0xfe, 0x52, 0x57,
0x4a, 0x1a, 0xe1, 0xf5, 0xc8, 0x2a, 0x5b, 0xf8,
0xdd, 0xc6, 0x71, 0x70
};

void psa_aead_test(const psa_key_type_t key_type,
                   const psa_algorithm_t alg,
                   const uint8_t *key,
                   size_t key_bits,
                   struct test_result_t *ret)
{
    psa_aead_operation_t encop = psa_aead_operation_init();
    psa_aead_operation_t decop = psa_aead_operation_init();
    psa_status_t status = PSA_SUCCESS;
    psa_key_handle_t key_handle;
    const size_t nonce_length = 12;
    const uint8_t nonce[] = "01234567890";
    const uint8_t plain_text[MAX_PLAIN_DATA_SIZE_AEAD] =
        "This is my plaintext message and it's made of 68 characters...!1234";
    const uint8_t associated_data[] =
        "This is my associated data to authenticate";
    uint8_t decrypted_data[MAX_PLAIN_DATA_SIZE_AEAD] = {0};
    uint8_t encrypted_data[ENC_DEC_BUFFER_SIZE_AEAD] = {0};
    size_t encrypted_data_length = 0, decrypted_data_length = 0;
    size_t total_output_length = 0, total_encrypted_length = 0;
    uint32_t comp_result;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_attributes_t retrieved_attributes = psa_key_attributes_init();
    psa_key_usage_t usage = (PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);

    /* Variables required for multipart operations */
    uint8_t *tag = &encrypted_data[MAX_PLAIN_DATA_SIZE_AEAD];
    size_t tag_size = PSA_AEAD_TAG_LENGTH(key_type,
                                          psa_get_key_bits(&key_attributes),
                                          alg);
    size_t tag_length = 0;

    ret->val = TEST_PASSED;

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, key_type);

    /* Import a key */
    status = psa_import_key(&key_attributes, key, PSA_BITS_TO_BYTES(key_bits),
                            &key_handle);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    status = psa_get_key_attributes(key_handle, &retrieved_attributes);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error getting key metadata");
        goto destroy_key_aead;
    }

    if (psa_get_key_bits(&retrieved_attributes) != key_bits) {
        TEST_FAIL("The number of key bits is different from expected");
        goto destroy_key_aead;
    }

    if (psa_get_key_type(&retrieved_attributes) != key_type) {
        TEST_FAIL("The type of the key is different from expected");
        goto destroy_key_aead;
    }

    psa_reset_key_attributes(&retrieved_attributes);

    /* Perform AEAD encryption */
    status = psa_aead_encrypt(key_handle, alg, nonce, nonce_length,
                              associated_data,
                              sizeof(associated_data),
                              plain_text,
                              sizeof(plain_text),
                              encrypted_data,
                              sizeof(encrypted_data),
                              &encrypted_data_length);

    if (status != PSA_SUCCESS) {
        if (status == PSA_ERROR_NOT_SUPPORTED) {
            TEST_FAIL("Algorithm NOT SUPPORTED by the implementation");
            goto destroy_key_aead;
        }

        TEST_FAIL("Error performing AEAD encryption");
        goto destroy_key_aead;
    }

    if (encrypted_data_length
        != PSA_AEAD_ENCRYPT_OUTPUT_SIZE(key_type, alg, sizeof(plain_text))) {
        TEST_FAIL("Encrypted data length is different than expected");
        goto destroy_key_aead;
    }

    if (key_type == PSA_KEY_TYPE_CHACHA20) {
        /* Check that the decrypted data is the same as the original data */
        comp_result = compare_buffers(encrypted_data,
                                      chacha20_poly1305_ref_encrypted,
                                      sizeof(encrypted_data));
        if (comp_result != 0) {
            TEST_FAIL("Encrypted data does not match reference data");
            goto destroy_key_aead;
        }
    }

    /* Perform AEAD decryption */
    status = psa_aead_decrypt(key_handle, alg, nonce, nonce_length,
                              associated_data,
                              sizeof(associated_data),
                              encrypted_data,
                              encrypted_data_length,
                              decrypted_data,
                              sizeof(decrypted_data),
                              &decrypted_data_length);

    if (status != PSA_SUCCESS) {
        if (status == PSA_ERROR_NOT_SUPPORTED) {
            TEST_FAIL("Algorithm NOT SUPPORTED by the implementation");
        } else {
            TEST_FAIL("Error performing AEAD decryption");
        }

        goto destroy_key_aead;
    }

    if (sizeof(plain_text) != decrypted_data_length) {
        TEST_FAIL("Decrypted data length is different from plain text");
        goto destroy_key_aead;
    }

    /* Check that the decrypted data is the same as the original data */
    comp_result = compare_buffers(plain_text, decrypted_data,
                                  sizeof(plain_text));
    if (comp_result != 0) {
        TEST_FAIL("Decrypted data doesn't match with plain text");
        goto destroy_key_aead;
    }

    /* Setup the encryption object */
    status = psa_aead_encrypt_setup(&encop, key_handle, alg);
    if (status != PSA_SUCCESS) {
        /* Implementations using the PSA Crypto Driver APIs, that don't
         * support multipart API flows, will return PSA_ERROR_NOT_SUPPORTED
         * when calling psa_aead_encrypt_setup(). In this case, it's fine
         * just to skip the multipart APIs test flow from this point on
         */
        if (status == PSA_ERROR_NOT_SUPPORTED) {
            TEST_LOG("psa_aead_encrypt_setup(): Algorithm NOT SUPPORTED by"\
                     " the implementation - skip multipart API flow\r\n");
        } else {
            TEST_FAIL("Error setting up encryption object");
        }
        goto destroy_key_aead;
    }

    /* Set lengths */
    status = psa_aead_set_lengths(&encop,
                                  sizeof(associated_data),
                                  sizeof(plain_text));
    if (status != PSA_SUCCESS) {
        /* Implementations using the mbed TLS _ALT APIs, that don't support
         * multipart API flows in CCM mode, will return PSA_ERROR_NOT_SUPPORTED
         * when calling psa_aead_set_lengths(). In this case, it's fine just
         * to skip the multipart APIs test flow from this point on
         */
        if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg) == PSA_ALG_CCM
            && status == PSA_ERROR_NOT_SUPPORTED) {
            TEST_LOG("psa_aead_set_lengths(): Algorithm NOT SUPPORTED by the"\
                     "implementation - skip multipart API flow\r\n");
        } else {
            TEST_FAIL("Error setting lengths");
        }
        status = psa_aead_abort(&encop);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key_aead;
    }

    /* Set nonce */
    status = psa_aead_set_nonce(&encop, nonce, nonce_length);
    if (status != PSA_SUCCESS) {
        /* Implementations using the mbed TLS _ALT APIs, that don't support
         * multipart API flows in GCM, will return PSA_ERROR_NOT_SUPPORTED when
         * calling psa_aead_set_nonce(). In this case, it's fine just to skip
         * the multipart APIs test flow from this point on
         */
        if (PSA_ALG_AEAD_WITH_DEFAULT_LENGTH_TAG(alg) == PSA_ALG_GCM
            && status == PSA_ERROR_NOT_SUPPORTED) {
            TEST_LOG("psa_aead_set_nonce(): Algorithm NOT SUPPORTED by the "\
                     "implementation - skip multipart API flow\r\n");
        } else {
            TEST_FAIL("Error setting nonce");
        }
        status = psa_aead_abort(&encop);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key_aead;
    }

    /* Update additional data */
    status = psa_aead_update_ad(&encop,
                                associated_data,
                                sizeof(associated_data));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error updating additional data");
        status = psa_aead_abort(&encop);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key_aead;
    }

    /* Encrypt one chunk of information at a time */
    for (size_t i = 0; i <= sizeof(plain_text)/BYTE_SIZE_CHUNK; i++) {

        size_t size_to_encrypt =
            (sizeof(plain_text) - i*BYTE_SIZE_CHUNK) > BYTE_SIZE_CHUNK ?
            BYTE_SIZE_CHUNK : (sizeof(plain_text) - i*BYTE_SIZE_CHUNK);

        status = psa_aead_update(&encop,
                                 plain_text + i*BYTE_SIZE_CHUNK,
                                 size_to_encrypt,
                                 encrypted_data + total_encrypted_length,
                                 sizeof(encrypted_data) -
                                                      total_encrypted_length,
                                 &encrypted_data_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error encrypting one chunk of information");
            status = psa_aead_abort(&encop);
            if (status != PSA_SUCCESS) {
                TEST_FAIL("Error aborting the operation");
            }
            goto destroy_key_aead;
        }
        total_encrypted_length += encrypted_data_length;
    }

    /* Finish the aead operation */
    status = psa_aead_finish(&encop,
                             encrypted_data + total_encrypted_length,
                             sizeof(encrypted_data) - total_encrypted_length,
                             &encrypted_data_length,
                             tag,
                             tag_size,
                             &tag_length);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error finalising the AEAD operation");
        status = psa_aead_abort(&encop);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key_aead;
    }
    total_encrypted_length += encrypted_data_length;

    /* Setup up the decryption object */
    status = psa_aead_decrypt_setup(&decop, key_handle, alg);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting uup AEAD object");
        goto destroy_key_aead;
    }

    /* Set lengths */
    status = psa_aead_set_lengths(&decop,
                                  sizeof(associated_data),
                                  sizeof(plain_text));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting lengths");
        status = psa_aead_abort(&decop);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key_aead;
    }

    /* Set nonce */
    status = psa_aead_set_nonce(&decop, nonce, nonce_length);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting nonce");
        status = psa_aead_abort(&decop);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key_aead;
    }

    /* Update additional data */
    status = psa_aead_update_ad(&decop,
                                associated_data,
                                sizeof(associated_data));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error updating additional data");
        status = psa_aead_abort(&decop);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key_aead;
    }

    /* Decrypt */
    for (size_t i = 0; i <= total_encrypted_length/BYTE_SIZE_CHUNK; i++) {

        size_t size_to_decrypt =
            (total_encrypted_length - i*BYTE_SIZE_CHUNK) > BYTE_SIZE_CHUNK ?
            BYTE_SIZE_CHUNK : (total_encrypted_length - i*BYTE_SIZE_CHUNK);

        status = psa_aead_update(&decop,
                                 encrypted_data + i*BYTE_SIZE_CHUNK,
                                 size_to_decrypt,
                                 decrypted_data + total_output_length,
                                 sizeof(decrypted_data)
                                                       - total_output_length,
                                 &decrypted_data_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error during decryption");
            status = psa_aead_abort(&decop);
            if (status != PSA_SUCCESS) {
                TEST_FAIL("Error aborting the operation");
            }
            goto destroy_key_aead;
        }
        total_output_length += decrypted_data_length;
    }

    /* Verify the AEAD operation */
    status = psa_aead_verify(&decop,
                             decrypted_data + total_output_length,
                             sizeof(decrypted_data) - total_output_length,
                             &decrypted_data_length,
                             tag,
                             tag_size);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error verifying the AEAD operation");
        status = psa_aead_abort(&decop);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key_aead;
    }
    total_output_length += decrypted_data_length;

    if (total_output_length != sizeof(plain_text)) {
        TEST_FAIL("Total decrypted length does not match size of plain text");
        status = psa_aead_abort(&decop);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key_aead;
    }

    /* Check that the decrypted data is the same as the original data */
    comp_result = compare_buffers(plain_text, decrypted_data,
                                  sizeof(plain_text));
    if (comp_result != 0) {
        TEST_FAIL("Decrypted data doesn't match with plain text");
    }

destroy_key_aead:
    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying a key");
    }
}

/*
 * The list of available AES cipher/AEAD mode for test.
 * Not all the modes can be available in some use cases and configurations.
 */
static const psa_algorithm_t test_aes_mode_array[] = {
#ifdef TFM_CRYPTO_TEST_ALG_CBC
    PSA_ALG_CBC_NO_PADDING,
#endif
#ifdef TFM_CRYPTO_TEST_ALG_CCM
    PSA_ALG_CCM,
#endif
#ifdef TFM_CRYPTO_TEST_ALG_CFB
    PSA_ALG_CFB,
#endif
#ifdef TFM_CRYPTO_TEST_ALG_ECB
    PSA_ALG_ECB_NO_PADDING,
#endif
#ifdef TFM_CRYPTO_TEST_ALG_CTR
    PSA_ALG_CTR,
#endif
#ifdef TFM_CRYPTO_TEST_ALG_OFB
    PSA_ALG_OFB,
#endif
#ifdef TFM_CRYPTO_TEST_ALG_GCM
    PSA_ALG_GCM,
#endif
    /* In case no AES algorithm is available */
    PSA_ALG_VENDOR_FLAG,
};

/* Number of available AES cipher modes */
#define NR_TEST_AES_MODE        (sizeof(test_aes_mode_array) / \
                                 sizeof(test_aes_mode_array[0]) - 1)

void psa_invalid_key_length_test(struct test_result_t *ret)
{
    psa_status_t status;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_handle_t key_handle;
    const uint8_t data[19] = {0};

    if (NR_TEST_AES_MODE < 1) {
        TEST_FAIL("A cipher mode in AES is required in current test case");
        return;
    }

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_ENCRYPT);
    psa_set_key_algorithm(&key_attributes, test_aes_mode_array[0]);
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);

    /* AES does not support 152-bit keys */
    status = psa_import_key(&key_attributes, data, sizeof(data), &key_handle);
    if (status != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Should not successfully import with an invalid key length");
        return;
    }

    ret->val = TEST_PASSED;
}

void psa_policy_key_interface_test(struct test_result_t *ret)
{
    psa_algorithm_t alg = test_aes_mode_array[0];
    psa_algorithm_t alg_out;
    psa_key_lifetime_t lifetime = PSA_KEY_LIFETIME_VOLATILE;
    psa_key_lifetime_t lifetime_out;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_usage_t usage = PSA_KEY_USAGE_EXPORT;
    psa_key_usage_t usage_out;

    if (NR_TEST_AES_MODE < 1) {
        TEST_FAIL("A cipher mode in AES is required in current test case");
        return;
    }

    /* Verify that initialised policy forbids all usage */
    usage_out = psa_get_key_usage_flags(&key_attributes);
    if (usage_out != 0) {
        TEST_FAIL("Unexpected usage value");
        return;
    }

    alg_out = psa_get_key_algorithm(&key_attributes);
    if (alg_out != 0) {
        TEST_FAIL("Unexpected algorithm value");
        return;
    }

    /* Set the key policy values */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);

    /* Check that the key policy has the correct usage */
    usage_out = psa_get_key_usage_flags(&key_attributes);
    if (usage_out != usage) {
        TEST_FAIL("Unexpected usage value");
        return;
    }

    /* Check that the key policy has the correct algorithm */
    alg_out = psa_get_key_algorithm(&key_attributes);
    if (alg_out != alg) {
        TEST_FAIL("Unexpected algorithm value");
        return;
    }

    /* Check the key handle has the correct key lifetime */
    lifetime_out = psa_get_key_lifetime(&key_attributes);

    if (lifetime_out != lifetime) {
        TEST_FAIL("Unexpected key lifetime value");
        return;
    }

    ret->val = TEST_PASSED;
}

void psa_policy_invalid_policy_usage_test(struct test_result_t *ret)
{
    psa_status_t status;
    psa_algorithm_t alg, not_permit_alg;
    psa_cipher_operation_t handle = psa_cipher_operation_init();
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_handle_t key_handle;
    psa_key_usage_t usage = (PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    size_t data_len;
    const uint8_t data[] = "THIS IS MY KEY1";
    uint8_t data_out[sizeof(data)];
    uint8_t i, j;

    ret->val = TEST_PASSED;

    if (NR_TEST_AES_MODE < 2) {
        TEST_LOG("Two cipher modes are required. Skip this test case\r\n");
        return;
    }

    /*
     * Search for two modes for test. Both modes should be Cipher algorithms.
     * Otherwise, cipher setup may fail before policy permission check.
     */
    for (i = 0; i < NR_TEST_AES_MODE - 1; i++) {
        if (PSA_ALG_IS_CIPHER(test_aes_mode_array[i])) {
            alg = test_aes_mode_array[i];
            break;
        }
    }

    for (j = i + 1; j < NR_TEST_AES_MODE; j++) {
        if (PSA_ALG_IS_CIPHER(test_aes_mode_array[j])) {
            not_permit_alg = test_aes_mode_array[j];
            break;
        }
    }

    if (j == NR_TEST_AES_MODE) {
        TEST_LOG("Unable to find two Cipher algs. Skip this test case.\r\n");
        return;
    }

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);

    /* Import a key to the key handle for which policy has been set */
    status = psa_import_key(&key_attributes, data, sizeof(data), &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to import a key");
        return;
    }

    /* Setup a cipher permitted by the key policy */
    status = psa_cipher_encrypt_setup(&handle, key_handle, alg);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to setup cipher operation");
        goto destroy_key;
    }

    status = psa_cipher_abort(&handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to abort cipher operation");
        goto destroy_key;
    }

    /* Attempt to setup a cipher with an alg not permitted by the policy */
    status = psa_cipher_encrypt_setup(&handle, key_handle, not_permit_alg);
    if (status != PSA_ERROR_NOT_PERMITTED) {
        TEST_FAIL("Was able to setup cipher operation with wrong alg");
        goto destroy_key;
    }

    /* Attempt to export the key, which is forbidden by the key policy */
    status = psa_export_key(key_handle, data_out, sizeof(data_out), &data_len);
    if (status != PSA_ERROR_NOT_PERMITTED) {
        TEST_FAIL("Should not be able to export key without correct usage");
        goto destroy_key;
    }

destroy_key:
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to destroy key");
    }
}

void psa_persistent_key_test(psa_key_id_t key_id, struct test_result_t *ret)
{
    psa_status_t status;
    int comp_result;
    psa_key_handle_t key_handle;
    psa_algorithm_t alg = test_aes_mode_array[0];
    psa_key_usage_t usage = PSA_KEY_USAGE_EXPORT;
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
    size_t data_len;
    const uint8_t data[] = "THIS IS MY KEY1";
    uint8_t data_out[sizeof(data)] = {0};

    if (NR_TEST_AES_MODE < 1) {
        TEST_FAIL("A cipher mode in AES is required in current test case");
        return;
    }

    /* Setup the key attributes with a key ID to create a persistent key */
    psa_set_key_id(&key_attributes, key_id);
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);

    /* Import key data to create the persistent key */
    status = psa_import_key(&key_attributes, data, sizeof(data), &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to import a key");
        return;
    }

    /* Close the persistent key handle */
    status = psa_close_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to close a persistent key handle");
        return;
    }

    /* Open the previsously-created persistent key */
    status = psa_open_key(key_id, &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to open a persistent key");
        return;
    }

    /* Export the persistent key */
    status = psa_export_key(key_handle, data_out, sizeof(data_out), &data_len);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to export a persistent key");
        return;
    }

    if (data_len != sizeof(data)) {
        TEST_FAIL("Number of bytes of exported key different from expected");
        return;
    }

    /* Check that the exported key is the same as the imported one */
    comp_result = compare_buffers(data_out, data, sizeof(data));
    if (comp_result != 0) {
        TEST_FAIL("Exported key does not match the imported key");
        return;
    }

    /* Destroy the persistent key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to destroy a persistent key");
        return;
    }

    ret->val = TEST_PASSED;
}

#define KEY_DERIV_OUTPUT_LEN       32
#define KEY_DERIV_SECRET_LEN       16
#define KEY_DERIV_PEER_LEN         16
#define KEY_DERIV_LABEL_INFO_LEN   8
#define KEY_DERIV_SEED_SALT_LEN    8
#define KEY_DERIV_RAW_MAX_PEER_LEN 100
#define KEY_DERIV_RAW_OUTPUT_LEN   48

/* An example of a 32 bytes / 256 bits ECDSA private key */
static const uint8_t ecdsa_private_key[] = {
    0x11, 0xb5, 0x73, 0x7c, 0xf9, 0xd9, 0x3f, 0x17,
    0xc0, 0xcb, 0x1a, 0x84, 0x65, 0x5d, 0x39, 0x95,
    0xa0, 0x28, 0x24, 0x09, 0x7e, 0xff, 0xa5, 0xed,
    0xd8, 0xee, 0x26, 0x38, 0x1e, 0xb5, 0xd6, 0xc3};
/* Buffer to hold the peer key of the key agreement process */
static uint8_t raw_agreement_peer_key[KEY_DERIV_RAW_MAX_PEER_LEN] = {0};

static uint8_t key_deriv_secret[KEY_DERIV_SECRET_LEN];
static uint8_t key_deriv_label_info[KEY_DERIV_LABEL_INFO_LEN];
static uint8_t key_deriv_seed_salt[KEY_DERIV_SEED_SALT_LEN];

#define RAW_AGREEMENT_TEST_KEY_ID (0x1)

void psa_key_agreement_test(psa_algorithm_t deriv_alg,
                            struct test_result_t *ret)
{
    psa_status_t status;
    psa_key_type_t key_type;
    psa_key_handle_t input_handle = 0;
    psa_key_attributes_t input_key_attr = PSA_KEY_ATTRIBUTES_INIT;
    uint8_t raw_agreement_output_buffer[KEY_DERIV_RAW_OUTPUT_LEN] = {0};
    size_t raw_agreement_output_size = 0;
    size_t public_key_length = 0;

    if (!PSA_ALG_IS_RAW_KEY_AGREEMENT(deriv_alg)) {
        TEST_FAIL("Unsupported key agreement algorithm");
        return;
    }

    psa_set_key_usage_flags(&input_key_attr, PSA_KEY_USAGE_DERIVE);
    psa_set_key_algorithm(&input_key_attr, deriv_alg);
    key_type = PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1);
    psa_set_key_type(&input_key_attr, key_type);
    psa_set_key_id(&input_key_attr, RAW_AGREEMENT_TEST_KEY_ID);
    status = psa_import_key(&input_key_attr, ecdsa_private_key,
                            sizeof(ecdsa_private_key), &input_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing the private key");
        return;
    }

    /* For simplicity, as the peer key use the public part of private key */
    status = psa_export_public_key(RAW_AGREEMENT_TEST_KEY_ID,
                                   raw_agreement_peer_key,
                                   KEY_DERIV_RAW_MAX_PEER_LEN,
                                   &public_key_length);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error extracting the public key as peer key");
        goto destroy_key;
    }

    status = psa_raw_key_agreement(deriv_alg,
                                   RAW_AGREEMENT_TEST_KEY_ID,
                                   raw_agreement_peer_key,
                                   public_key_length,
                                   raw_agreement_output_buffer,
                                   KEY_DERIV_RAW_OUTPUT_LEN,
                                   &raw_agreement_output_size);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error performing single step raw key agreement");
        goto destroy_key;
    }

    if (raw_agreement_output_size != sizeof(ecdsa_private_key)) {
        TEST_FAIL("Agreed key size is different than expected!");
        goto destroy_key;
    }

    ret->val = TEST_PASSED;

destroy_key:
    psa_destroy_key(input_handle);

    return;
}

void psa_key_derivation_test(psa_algorithm_t deriv_alg,
                             struct test_result_t *ret)
{
    psa_key_handle_t input_handle = 0, output_handle = 0;
    psa_key_attributes_t input_key_attr = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_attributes_t output_key_attr = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_derivation_operation_t deriv_ops;
    psa_status_t status;
    uint8_t counter = 0xA5;
    psa_key_type_t key_type;

    /* Prepare the parameters */
#if DOMAIN_NS == 1U
    memset(key_deriv_secret, counter, KEY_DERIV_SECRET_LEN);
    memset(key_deriv_label_info, counter++, KEY_DERIV_LABEL_INFO_LEN);
    memset(key_deriv_seed_salt, counter++, KEY_DERIV_SEED_SALT_LEN);
#else
    tfm_memset(key_deriv_secret, counter, KEY_DERIV_SECRET_LEN);
    tfm_memset(key_deriv_label_info, counter++, KEY_DERIV_LABEL_INFO_LEN);
    tfm_memset(key_deriv_seed_salt, counter++, KEY_DERIV_SEED_SALT_LEN);
#endif

    deriv_ops = psa_key_derivation_operation_init();

    psa_set_key_usage_flags(&input_key_attr, PSA_KEY_USAGE_DERIVE);
    psa_set_key_algorithm(&input_key_attr, deriv_alg);
    key_type = PSA_KEY_TYPE_DERIVE;
    psa_set_key_type(&input_key_attr, key_type);
    status = psa_import_key(&input_key_attr, key_deriv_secret,
                            KEY_DERIV_SECRET_LEN, &input_handle);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to import secret");
        return;
    }

    status = psa_key_derivation_setup(&deriv_ops, deriv_alg);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to setup derivation operation");
        goto destroy_key;
    }

    if (PSA_ALG_IS_TLS12_PRF(deriv_alg) ||
        PSA_ALG_IS_TLS12_PSK_TO_MS(deriv_alg)) {
        status = psa_key_derivation_input_bytes(&deriv_ops,
                                                PSA_KEY_DERIVATION_INPUT_SEED,
                                                key_deriv_seed_salt,
                                                KEY_DERIV_SEED_SALT_LEN);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Failed to input seed");
            goto deriv_abort;
        }

        status = psa_key_derivation_input_key(&deriv_ops,
                                              PSA_KEY_DERIVATION_INPUT_SECRET,
                                              input_handle);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Failed to input key");
            goto deriv_abort;
        }

        status = psa_key_derivation_input_bytes(&deriv_ops,
                                                PSA_KEY_DERIVATION_INPUT_LABEL,
                                                key_deriv_label_info,
                                                KEY_DERIV_LABEL_INFO_LEN);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Failed to input label");
            goto deriv_abort;
        }
    } else if (PSA_ALG_IS_HKDF(deriv_alg)) {
        status = psa_key_derivation_input_bytes(&deriv_ops,
                                                PSA_KEY_DERIVATION_INPUT_SALT,
                                                key_deriv_seed_salt,
                                                KEY_DERIV_SEED_SALT_LEN);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Failed to input salt");
            goto deriv_abort;
        }

        status = psa_key_derivation_input_key(&deriv_ops,
                                              PSA_KEY_DERIVATION_INPUT_SECRET,
                                              input_handle);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Failed to input key");
            goto deriv_abort;
        }

        status = psa_key_derivation_input_bytes(&deriv_ops,
                                                PSA_KEY_DERIVATION_INPUT_INFO,
                                                key_deriv_label_info,
                                                KEY_DERIV_LABEL_INFO_LEN);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Failed to input info");
            goto deriv_abort;
        }
    } else {
        TEST_FAIL("Unsupported derivation algorithm");
        goto deriv_abort;
    }

    if (NR_TEST_AES_MODE < 1) {
        TEST_LOG("No AES algorithm to verify. Output raw data instead\r\n");
        psa_set_key_type(&output_key_attr, PSA_KEY_TYPE_RAW_DATA);
    } else {
        psa_set_key_usage_flags(&output_key_attr, PSA_KEY_USAGE_ENCRYPT);
        psa_set_key_algorithm(&output_key_attr, test_aes_mode_array[0]);
        psa_set_key_type(&output_key_attr, PSA_KEY_TYPE_AES);
    }
    psa_set_key_bits(&output_key_attr,
                     PSA_BYTES_TO_BITS(KEY_DERIV_OUTPUT_LEN));

    status = psa_key_derivation_output_key(&output_key_attr, &deriv_ops,
                                           &output_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to output key");
        goto deriv_abort;
    }

    ret->val = TEST_PASSED;

deriv_abort:
    psa_key_derivation_abort(&deriv_ops);
destroy_key:
    psa_destroy_key(input_handle);
    if (output_handle) {
        psa_destroy_key(output_handle);
    }

    return;
}

/* Key is generated using psa_generate_key then psa_export_key, using the
 * key attributes given in the test function */
static const uint8_t rsa_key_pair[] = {
0x30, 0x82, 0x02, 0x5d, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xd1, 0xd9,
0xa2, 0x11, 0x6f, 0x2c, 0x56, 0x3b, 0x10, 0x01, 0xb8, 0x96, 0xa1, 0x8c, 0x79,
0xfa, 0x3c, 0x9a, 0x45, 0xe8, 0x03, 0xdb, 0x45, 0x66, 0xd1, 0x75, 0xf3, 0xd8,
0x3a, 0x10, 0xb3, 0x0f, 0x6c, 0x61, 0x78, 0x05, 0x21, 0xd6, 0x9a, 0x32, 0x25,
0xf0, 0x04, 0xd8, 0x1b, 0xf7, 0xa4, 0xd7, 0x0d, 0xd6, 0x00, 0xa4, 0x95, 0x92,
0xeb, 0x24, 0x1e, 0x37, 0xb5, 0x22, 0x9e, 0x7d, 0x22, 0x31, 0x6d, 0xd8, 0xf3,
0x2e, 0x4f, 0x8a, 0x94, 0x6f, 0xe6, 0x4e, 0x60, 0xa9, 0xed, 0x85, 0x70, 0x5b,
0xa3, 0x32, 0xbd, 0x8c, 0x1b, 0x11, 0xe4, 0x99, 0xce, 0x1f, 0xb9, 0x1f, 0xe1,
0xa8, 0x3a, 0x45, 0x55, 0xdd, 0x63, 0x94, 0x01, 0x99, 0xb7, 0xf7, 0x70, 0x7a,
0xf6, 0x49, 0xf3, 0x2e, 0xd3, 0xd7, 0x61, 0xfe, 0x81, 0x59, 0xd2, 0xe1, 0x45,
0x57, 0xfd, 0x1e, 0xfe, 0x87, 0x97, 0xbf, 0x40, 0x09, 0x02, 0x03, 0x01, 0x00,
0x01, 0x02, 0x81, 0x80, 0x03, 0x8d, 0x79, 0x2a, 0x6d, 0x64, 0xe5, 0x42, 0xd3,
0xb7, 0x0b, 0xbe, 0x75, 0x16, 0xb1, 0x3b, 0xf4, 0xc9, 0xb1, 0xd4, 0x47, 0x38,
0x6f, 0x98, 0xd9, 0x83, 0xf3, 0x30, 0x5e, 0x6f, 0x48, 0xf0, 0xc2, 0x67, 0x76,
0x06, 0x34, 0x37, 0xf3, 0x5d, 0x54, 0xfa, 0x16, 0xc2, 0xe7, 0xda, 0x4d, 0xee,
0x9c, 0x1b, 0xda, 0xdf, 0xee, 0x6e, 0x51, 0xcf, 0xc7, 0x39, 0x2f, 0x36, 0x5a,
0x53, 0x89, 0x00, 0x20, 0x5d, 0x0c, 0x14, 0x2c, 0xb1, 0x0b, 0xa1, 0x7f, 0xcf,
0xee, 0x48, 0x34, 0x01, 0x93, 0xb2, 0x67, 0x0b, 0x6b, 0xaa, 0xd8, 0xc6, 0x03,
0x67, 0xe3, 0xda, 0x64, 0xb4, 0xd1, 0x89, 0x9e, 0x46, 0xc5, 0xdd, 0x6c, 0x8a,
0xb6, 0x92, 0x0a, 0x24, 0xb7, 0x2d, 0x4a, 0x36, 0x7b, 0x5e, 0x3b, 0x98, 0x42,
0x5d, 0xa8, 0x58, 0xfe, 0x02, 0xd3, 0x5d, 0x1a, 0xc8, 0x02, 0xef, 0xc3, 0x4b,
0xe7, 0x59, 0x02, 0x41, 0x00, 0xf5, 0x8d, 0x58, 0xb0, 0x5e, 0x82, 0x17, 0x8b,
0xa7, 0x31, 0xed, 0x0b, 0xb2, 0x1c, 0x38, 0x7c, 0x07, 0x31, 0x15, 0xe3, 0x74,
0xc4, 0x7d, 0xd8, 0xd7, 0x29, 0xfe, 0x84, 0xa0, 0x8d, 0x81, 0xdd, 0x6c, 0xb4,
0x27, 0x41, 0xe3, 0x82, 0xa1, 0x6c, 0xa9, 0x3e, 0x93, 0xe8, 0xfd, 0x3c, 0xa4,
0xd1, 0x06, 0xb9, 0x9e, 0x07, 0x38, 0x51, 0xa4, 0x45, 0x3d, 0xff, 0x6d, 0x62,
0x39, 0xa3, 0x11, 0x5d, 0x02, 0x41, 0x00, 0xda, 0xc7, 0x67, 0xe8, 0xf0, 0xa2,
0xe3, 0xe7, 0x7d, 0x6c, 0xf4, 0xcc, 0x6d, 0x87, 0xf9, 0x76, 0xea, 0x61, 0xb4,
0xfa, 0xce, 0x05, 0x5e, 0xee, 0x5c, 0x12, 0x53, 0x88, 0xda, 0xc9, 0xc6, 0x81,
0x7d, 0x5c, 0xd3, 0x89, 0x1f, 0x2f, 0x7f, 0x1f, 0x11, 0x11, 0xd6, 0xd6, 0x45,
0x44, 0xe9, 0x1d, 0x35, 0x55, 0x1b, 0x02, 0xaf, 0xd4, 0xa5, 0xd1, 0xc2, 0xe7,
0x55, 0x35, 0x00, 0xf8, 0x62, 0x9d, 0x02, 0x41, 0x00, 0xbc, 0xe8, 0x90, 0x29,
0xa7, 0x9b, 0xa7, 0xe7, 0x9d, 0xaa, 0x50, 0x36, 0xa6, 0x41, 0x05, 0xc7, 0x8d,
0x74, 0xda, 0xe5, 0x11, 0x69, 0x35, 0x74, 0x44, 0x1c, 0x1f, 0x9e, 0x03, 0x32,
0xba, 0x8d, 0x11, 0xdb, 0x0b, 0x34, 0xaa, 0x86, 0x4e, 0x10, 0x1d, 0xa8, 0x71,
0xfc, 0x56, 0x0e, 0x78, 0xb2, 0x02, 0xdd, 0x7c, 0x51, 0x0b, 0xa7, 0xeb, 0x9c,
0x05, 0x95, 0x63, 0x9e, 0xa4, 0xbe, 0xea, 0x55, 0x02, 0x41, 0x00, 0x94, 0x8a,
0xc9, 0x79, 0x76, 0x51, 0x12, 0xae, 0x6d, 0x11, 0x9a, 0x50, 0x66, 0x99, 0xe8,
0xfe, 0x1d, 0x7b, 0x43, 0x96, 0xfa, 0x64, 0xd9, 0x24, 0xbb, 0xac, 0xd1, 0xbc,
0xdc, 0xd8, 0x1d, 0x08, 0x74, 0x66, 0x9f, 0x55, 0xbd, 0xaf, 0xd0, 0xfe, 0xf5,
0xe7, 0x07, 0xd8, 0x29, 0xe5, 0xf4, 0xe5, 0x18, 0xfd, 0xf4, 0xbd, 0xe9, 0x46,
0x57, 0x63, 0xc9, 0x92, 0xa9, 0xde, 0xb8, 0x0e, 0xed, 0x5d, 0x02, 0x40, 0x39,
0x30, 0x79, 0xb5, 0xe1, 0x22, 0xa7, 0x0e, 0xff, 0x96, 0x56, 0x7e, 0x16, 0xf3,
0x4c, 0xbd, 0x81, 0x94, 0x14, 0x53, 0xd4, 0x9b, 0xb6, 0xfa, 0xf9, 0x18, 0xf5,
0x4b, 0xe3, 0x5f, 0xb1, 0x87, 0x54, 0x67, 0x61, 0xc4, 0x05, 0x05, 0x01, 0x26,
0x89, 0xb0, 0xa3, 0x8b, 0xde, 0x18, 0xdb, 0x3f, 0x37, 0x09, 0x35, 0x65, 0x17,
0x9c, 0x37, 0xbe, 0x40, 0xed, 0xc6, 0x5c, 0xf8, 0x7e, 0x4b, 0x04};

#define PLAIN_TEXT_SIZE 16
#define RSA_KEY_SIZE 128

void psa_asymmetric_encryption_test(psa_algorithm_t alg,
                                    struct test_result_t *ret)
{
    psa_status_t status = PSA_SUCCESS;
    psa_key_handle_t key_handle = 0x0u;
    const uint8_t plain_text[] = "This is a test.";
    uint8_t encrypted_data[PSA_ASYMMETRIC_ENCRYPT_OUTPUT_MAX_SIZE] = {0};
    size_t encrypted_size;
    size_t encrypted_length = 0;
    uint8_t decrypted_data[PLAIN_TEXT_SIZE] = {0};
    size_t decrypted_size = PLAIN_TEXT_SIZE;
    size_t decrypted_length = 0;
    const uint8_t * key_pair;
    size_t key_size;
    uint32_t comp_result;

    psa_key_attributes_t key_attributes = psa_key_attributes_init();

    /* Setup key attributes */
    psa_set_key_usage_flags(&key_attributes,
                            PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&key_attributes, alg);

    if (PSA_ALG_IS_RSA_OAEP(alg) || alg == PSA_ALG_RSA_PKCS1V15_CRYPT) {
        psa_set_key_type(&key_attributes, PSA_KEY_TYPE_RSA_KEY_PAIR);
        encrypted_size = RSA_KEY_SIZE;
        key_pair = rsa_key_pair;
        key_size = sizeof(rsa_key_pair);
    } else {
        TEST_FAIL("Unsupported asymmetric encryption algorithm");
        return;
    }

    status = psa_import_key(&key_attributes, key_pair, key_size, &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    status = psa_asymmetric_encrypt(key_handle, alg, plain_text,
                                    sizeof(plain_text), NULL, 0, encrypted_data,
                                    encrypted_size, &encrypted_length);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error encrypting the plaintext");
        goto destroy_key;
    }

    if (encrypted_size != encrypted_length) {
        TEST_FAIL("Unexpected encrypted length");
        goto destroy_key;
    }

    status = psa_asymmetric_decrypt(key_handle, alg, encrypted_data,
                                    encrypted_size, NULL, 0, decrypted_data,
                                    decrypted_size, &decrypted_length);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error during asymmetric decryption");
        goto destroy_key;
    }

    if (decrypted_size != decrypted_length) {
        TEST_FAIL("Unexpected decrypted length");
        goto destroy_key;
    }

    /* Check that the plain text matches the decrypted data */
    comp_result = compare_buffers(plain_text, decrypted_data,
                                  sizeof(plain_text));
    if (comp_result != 0) {
        TEST_FAIL("Decrypted data doesn't match with plain text");
        goto destroy_key;
    }

    ret->val = TEST_PASSED;
destroy_key:
    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying a key");
    }
}

#define SIGNATURE_BUFFER_SIZE \
    (PSA_ECDSA_SIGNATURE_SIZE(PSA_VENDOR_ECC_MAX_CURVE_BITS))
#define SIGNING_TEST_KEY_ID (0x1)

void psa_sign_verify_message_test(psa_algorithm_t alg,
                                  struct test_result_t *ret)
{
    psa_status_t status = PSA_SUCCESS;
    psa_key_handle_t key_handle = 0;
    psa_key_type_t key_type;
    psa_key_attributes_t input_key_attr = PSA_KEY_ATTRIBUTES_INIT;
    const uint8_t message[] =
        "This is the message that I would like to sign";
    uint8_t signature[SIGNATURE_BUFFER_SIZE] = {0};
    size_t signature_length = 0;

    /* Set attributes and import key */
    psa_set_key_usage_flags(&input_key_attr,
        PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
    psa_set_key_algorithm(&input_key_attr, alg);
    key_type = PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1);
    psa_set_key_type(&input_key_attr, key_type);
    psa_set_key_id(&input_key_attr, SIGNING_TEST_KEY_ID);
    status = psa_import_key(&input_key_attr, ecdsa_private_key,
                            sizeof(ecdsa_private_key), &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing the private key");
        return;
    }

    status = psa_sign_message(SIGNING_TEST_KEY_ID, alg,
                              message, sizeof(message) - 1,
                              signature, SIGNATURE_BUFFER_SIZE,
                              &signature_length);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Message signing failed!");
        goto destroy_key;
    }

    if (signature_length != PSA_ECDSA_SIGNATURE_SIZE(
                                PSA_BYTES_TO_BITS(
                                    sizeof(ecdsa_private_key)))) {
        TEST_FAIL("Unexpected signature length");
        goto destroy_key;
    }

    status = psa_verify_message(SIGNING_TEST_KEY_ID, alg,
                                message, sizeof(message) - 1,
                                signature, signature_length);
    if (status != PSA_SUCCESS) {
        TEST_FAIL(("Signature verification failed!"));
        goto destroy_key;
    }

    ret->val = TEST_PASSED;

destroy_key:
    psa_destroy_key(key_handle);

    return;
}
