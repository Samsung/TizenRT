/*
 *  PSA FFDH layer on top of Mbed TLS crypto
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "common.h"

#if defined(MBEDTLS_PSA_CRYPTO_C)
#include <psa/crypto.h>
#include "psa_crypto_core.h"
#include "psa_crypto_ffdh.h"
// #include "psa_crypto_random_impl.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/platform.h"
#include "mbedtls/error.h"
#include <mbedtls/psa_util.h>

unsigned char *P_TEST;
unsigned char *G_TEST;
size_t P_TEST_size = 0;
size_t G_TEST_size = 0;

extern mbedtls_ctr_drbg_context *const mbedtls_psa_random_state;
#define MBEDTLS_PSA_RANDOM_STATE mbedtls_psa_random_state

#if defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_EXPORT) ||   \
    defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_GENERATE) ||   \
    defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_PUBLIC_KEY) || \
    defined(MBEDTLS_PSA_BUILTIN_ALG_FFDH)

static psa_status_t mbedtls_psa_ffdh_set_prime_generator(size_t key_size,
                                                         mbedtls_mpi *P,
                                                         mbedtls_mpi *G)
{
    const unsigned char *dhm_P = NULL;
    const unsigned char *dhm_G = NULL;
    size_t dhm_size_P = 0;
    size_t dhm_size_G = 0;
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    if (P == NULL && G == NULL)
        return PSA_ERROR_INVALID_ARGUMENT;

    if (P_TEST == NULL || G_TEST == NULL)
        return PSA_ERROR_INVALID_ARGUMENT;

    dhm_size_P = P_TEST_size;
    dhm_size_G = G_TEST_size;

    dhm_P = malloc(P_TEST_size);
    if (dhm_P == NULL)
        return PSA_ERROR_INVALID_ARGUMENT;

    dhm_G = malloc(G_TEST_size);
    if (dhm_G == NULL) {
        free(dhm_P);
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    memcpy(dhm_P, P_TEST, P_TEST_size);
    memcpy(dhm_G, G_TEST, G_TEST_size);

#if 0
    static const unsigned char dhm_P_2048[] =
        MBEDTLS_DHM_RFC7919_FFDHE2048_P_BIN;
    static const unsigned char dhm_P_3072[] =
        MBEDTLS_DHM_RFC7919_FFDHE3072_P_BIN;
    static const unsigned char dhm_P_4096[] =
        MBEDTLS_DHM_RFC7919_FFDHE4096_P_BIN;
    static const unsigned char dhm_P_6144[] =
        MBEDTLS_DHM_RFC7919_FFDHE6144_P_BIN;
    static const unsigned char dhm_P_8192[] =
        MBEDTLS_DHM_RFC7919_FFDHE8192_P_BIN;
    static const unsigned char dhm_G_2048[] =
        MBEDTLS_DHM_RFC7919_FFDHE2048_G_BIN;
    static const unsigned char dhm_G_3072[] =
        MBEDTLS_DHM_RFC7919_FFDHE3072_G_BIN;
    static const unsigned char dhm_G_4096[] =
        MBEDTLS_DHM_RFC7919_FFDHE4096_G_BIN;
    static const unsigned char dhm_G_6144[] =
        MBEDTLS_DHM_RFC7919_FFDHE6144_G_BIN;
    static const unsigned char dhm_G_8192[] =
        MBEDTLS_DHM_RFC7919_FFDHE8192_G_BIN;

    switch (key_size) {
        case sizeof(dhm_P_2048):
            dhm_P = dhm_P_2048;
            dhm_G = dhm_G_2048;
            dhm_size_P = sizeof(dhm_P_2048);
            dhm_size_G = sizeof(dhm_G_2048);
            break;
        case sizeof(dhm_P_3072):
            dhm_P = dhm_P_3072;
            dhm_G = dhm_G_3072;
            dhm_size_P = sizeof(dhm_P_3072);
            dhm_size_G = sizeof(dhm_G_3072);
            break;
        case sizeof(dhm_P_4096):
            dhm_P = dhm_P_4096;
            dhm_G = dhm_G_4096;
            dhm_size_P = sizeof(dhm_P_4096);
            dhm_size_G = sizeof(dhm_G_4096);
            break;
        case sizeof(dhm_P_6144):
            dhm_P = dhm_P_6144;
            dhm_G = dhm_G_6144;
            dhm_size_P = sizeof(dhm_P_6144);
            dhm_size_G = sizeof(dhm_G_6144);
            break;
        case sizeof(dhm_P_8192):
            dhm_P = dhm_P_8192;
            dhm_G = dhm_G_8192;
            dhm_size_P = sizeof(dhm_P_8192);
            dhm_size_G = sizeof(dhm_G_8192);
            break;
        default:
            return PSA_ERROR_INVALID_ARGUMENT;
    }
#endif

    if (P != NULL) {
        MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(P, dhm_P,
                                                dhm_size_P));
    }
    if (G != NULL) {
        MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(G, dhm_G,
                                                dhm_size_G));
    }

    free(dhm_P);
    free(dhm_G);

cleanup:
    if (ret != 0) {
        return mbedtls_to_psa_error(ret);
    }

    return PSA_SUCCESS;
}

#endif /* MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_EXPORT ||
          MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_GENERATE ||
          MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_PUBLIC_KEY ||
          MBEDTLS_PSA_BUILTIN_ALG_FFDH */

#if defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_EXPORT) || \
    defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_PUBLIC_KEY)
psa_status_t mbedtls_psa_ffdh_export_public_key(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer,
    size_t key_buffer_size,
    uint8_t *data,
    size_t data_size,
    size_t *data_length)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    mbedtls_mpi GX, G, X, P;
    psa_key_type_t type = attributes->core.type;

    if (PSA_KEY_TYPE_IS_PUBLIC_KEY(type)) {
        if (key_buffer_size > data_size) {
            return PSA_ERROR_BUFFER_TOO_SMALL;
        }
        memcpy(data, key_buffer, key_buffer_size);
        memset(data + key_buffer_size, 0,
               data_size - key_buffer_size);
        *data_length = key_buffer_size;
        return PSA_SUCCESS;
    }

    mbedtls_mpi_init(&GX); mbedtls_mpi_init(&G);
    mbedtls_mpi_init(&X); mbedtls_mpi_init(&P);

    size_t key_len = PSA_BITS_TO_BYTES(attributes->core.bits);

    status = mbedtls_psa_ffdh_set_prime_generator(key_len, &P, &G);

    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&X, key_buffer,
                                            key_buffer_size));

    MBEDTLS_MPI_CHK(mbedtls_mpi_exp_mod(&GX, &G, &X, &P, NULL));
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&GX, data, key_len));

    *data_length = key_len;

    ret = 0;
cleanup:
    mbedtls_mpi_free(&P); mbedtls_mpi_free(&G);
    mbedtls_mpi_free(&X); mbedtls_mpi_free(&GX);

    if (status == PSA_SUCCESS && ret != 0) {
        status = mbedtls_to_psa_error(ret);
    }

    return status;
}
#endif /* MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_EXPORT ||
          MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_PUBLIC_KEY */

#if defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_GENERATE)
psa_status_t mbedtls_psa_ffdh_generate_key(
    const psa_key_attributes_t *attributes,
    uint8_t *key_buffer, size_t key_buffer_size, size_t *key_buffer_length)
{
    mbedtls_mpi X, P;
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    mbedtls_mpi_init(&P); mbedtls_mpi_init(&X);
    (void) attributes;

    status = mbedtls_psa_ffdh_set_prime_generator(key_buffer_size, &P, NULL);

    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    /* RFC7919: Traditional finite field Diffie-Hellman has each peer choose their
        secret exponent from the range [2, P-2].
        Select random value in range [3, P-1] and decrease it by 1. */
    MBEDTLS_MPI_CHK(mbedtls_mpi_random(&X, 3, &P, mbedtls_ctr_drbg_random,
                                       MBEDTLS_PSA_RANDOM_STATE));
    MBEDTLS_MPI_CHK(mbedtls_mpi_sub_int(&X, &X, 1));
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&X, key_buffer, key_buffer_size));

    *key_buffer_length = key_buffer_size;

cleanup:
    mbedtls_mpi_free(&P); mbedtls_mpi_free(&X);
    if (status == PSA_SUCCESS && ret != 0) {
        return mbedtls_to_psa_error(ret);
    }

    return status;
}
#endif /* MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_GENERATE */

#if defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_IMPORT)
psa_status_t mbedtls_psa_ffdh_import_key(
    const psa_key_attributes_t *attributes,
    const uint8_t *data, size_t data_length,
    uint8_t *key_buffer, size_t key_buffer_size,
    size_t *key_buffer_length, size_t *bits)
{
    (void) attributes;

    if (key_buffer_size < data_length) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }
    memcpy(key_buffer, data, data_length);
    *key_buffer_length = data_length;
    *bits = PSA_BYTES_TO_BITS(data_length);

    return PSA_SUCCESS;
}
#endif /* MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_IMPORT */

#if defined(MBEDTLS_PSA_BUILTIN_ALG_FFDH)
psa_status_t mbedtls_psa_ffdh_key_agreement(
    const psa_core_key_attributes_t *attributes,
    const uint8_t *peer_key,
    size_t peer_key_length,
    const uint8_t *key_buffer,
    size_t key_buffer_size,
    uint8_t *shared_secret,
    size_t shared_secret_size,
    size_t *shared_secret_length)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    mbedtls_mpi P, G, X, GY, K;
    const size_t calculated_shared_secret_size = peer_key_length;

    if (peer_key_length != key_buffer_size ||
        calculated_shared_secret_size > shared_secret_size) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (!PSA_KEY_TYPE_IS_DH_KEY_PAIR(attributes->type)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    mbedtls_mpi_init(&P); mbedtls_mpi_init(&G);
    mbedtls_mpi_init(&X); mbedtls_mpi_init(&GY);
    mbedtls_mpi_init(&K);

    status = mbedtls_psa_ffdh_set_prime_generator(
        PSA_BITS_TO_BYTES(attributes->bits), &P, &G);

    if (status != PSA_SUCCESS) {
        goto cleanup;
    }

    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&X, key_buffer,
                                            key_buffer_size));

    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&GY, peer_key,
                                            peer_key_length));

    /* Calculate shared secret public key: K = G^(XY) mod P = GY^X mod P */
    MBEDTLS_MPI_CHK(mbedtls_mpi_exp_mod(&K, &GY, &X, &P, NULL));

    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&K, shared_secret,
                                             calculated_shared_secret_size));

    *shared_secret_length = calculated_shared_secret_size;

    ret = 0;

cleanup:
    mbedtls_mpi_free(&P); mbedtls_mpi_free(&G);
    mbedtls_mpi_free(&X); mbedtls_mpi_free(&GY);
    mbedtls_mpi_free(&K);

    if (status == PSA_SUCCESS && ret != 0)
        status = mbedtls_to_psa_error(ret);

    return status;
}
#endif /* MBEDTLS_PSA_BUILTIN_ALG_FFDH */

#endif /* MBEDTLS_PSA_CRYPTO_C */
