/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/*
 *  PSA hashing layer on top of Mbed TLS software crypto
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

#include "mbedtls/common.h"

#if defined(MBEDTLS_PSA_CRYPTO_C)

#include "mbedtls/psa/crypto.h"

#include "psa_crypto_core.h"
#include <mbedtls/psa_util.h>
#include <mbedtls/error.h>
#include <mbedtls/lms.h>
#include <mbedtls/ssl.h>
#include <mbedtls/rsa.h>

/* PSA_SUCCESS is kept at the top of each error table since
 * it's the most common status when everything functions properly. */
#if !defined(MBEDTLS_MD_C) || !defined(MBEDTLS_MD5_C) || defined(MBEDTLS_USE_PSA_CRYPTO)
const mbedtls_error_pair_t psa_to_md_errors[] =
{
    { PSA_SUCCESS,                     0 },
    { PSA_ERROR_NOT_SUPPORTED,         MBEDTLS_ERR_MD_FEATURE_UNAVAILABLE },
    { PSA_ERROR_INVALID_ARGUMENT,      MBEDTLS_ERR_MD_BAD_INPUT_DATA },
    { PSA_ERROR_INSUFFICIENT_MEMORY,   MBEDTLS_ERR_MD_ALLOC_FAILED }
};
#endif
#if defined(MBEDTLS_LMS_C)
const mbedtls_error_pair_t psa_to_lms_errors[] =
{
    { PSA_SUCCESS,                     0 },
    { PSA_ERROR_BUFFER_TOO_SMALL,      MBEDTLS_ERR_LMS_BUFFER_TOO_SMALL },
    { PSA_ERROR_INVALID_ARGUMENT,      MBEDTLS_ERR_LMS_BAD_INPUT_DATA }
};
#endif
#if defined(MBEDTLS_USE_PSA_CRYPTO) || defined(MBEDTLS_SSL_PROTO_TLS1_3)
const mbedtls_error_pair_t psa_to_ssl_errors[] =
{
    { PSA_SUCCESS,                     0 },
    { PSA_ERROR_INSUFFICIENT_MEMORY,   MBEDTLS_ERR_SSL_ALLOC_FAILED },
    { PSA_ERROR_NOT_SUPPORTED,         MBEDTLS_ERR_SSL_FEATURE_UNAVAILABLE },
    { PSA_ERROR_INVALID_SIGNATURE,     MBEDTLS_ERR_SSL_INVALID_MAC },
    { PSA_ERROR_INVALID_ARGUMENT,      MBEDTLS_ERR_SSL_BAD_INPUT_DATA },
    { PSA_ERROR_BAD_STATE,             MBEDTLS_ERR_SSL_INTERNAL_ERROR },
    { PSA_ERROR_BUFFER_TOO_SMALL,      MBEDTLS_ERR_SSL_BUFFER_TOO_SMALL }
};
#endif

#if defined(PSA_WANT_KEY_TYPE_RSA_PUBLIC_KEY) ||    \
    defined(PSA_WANT_KEY_TYPE_RSA_KEY_PAIR)
const mbedtls_error_pair_t psa_to_pk_rsa_errors[] =
{
    { PSA_SUCCESS,                     0 },
    { PSA_ERROR_NOT_PERMITTED,         MBEDTLS_ERR_RSA_BAD_INPUT_DATA },
    { PSA_ERROR_INVALID_ARGUMENT,      MBEDTLS_ERR_RSA_BAD_INPUT_DATA },
    { PSA_ERROR_INVALID_HANDLE,        MBEDTLS_ERR_RSA_BAD_INPUT_DATA },
    { PSA_ERROR_BUFFER_TOO_SMALL,      MBEDTLS_ERR_RSA_OUTPUT_TOO_LARGE },
    { PSA_ERROR_INSUFFICIENT_ENTROPY,  MBEDTLS_ERR_RSA_RNG_FAILED },
    { PSA_ERROR_INVALID_SIGNATURE,     MBEDTLS_ERR_RSA_VERIFY_FAILED },
    { PSA_ERROR_INVALID_PADDING,       MBEDTLS_ERR_RSA_INVALID_PADDING }
};
#endif

#if defined(MBEDTLS_USE_PSA_CRYPTO) && \
    defined(PSA_WANT_KEY_TYPE_ECC_PUBLIC_KEY)
const mbedtls_error_pair_t psa_to_pk_ecdsa_errors[] =
{
    { PSA_SUCCESS,                     0 },
    { PSA_ERROR_NOT_PERMITTED,         MBEDTLS_ERR_ECP_BAD_INPUT_DATA },
    { PSA_ERROR_INVALID_ARGUMENT,      MBEDTLS_ERR_ECP_BAD_INPUT_DATA },
    { PSA_ERROR_INVALID_HANDLE,        MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE },
    { PSA_ERROR_BUFFER_TOO_SMALL,      MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL },
    { PSA_ERROR_INSUFFICIENT_ENTROPY,  MBEDTLS_ERR_ECP_RANDOM_FAILED },
    { PSA_ERROR_INVALID_SIGNATURE,     MBEDTLS_ERR_ECP_VERIFY_FAILED }
};
#endif

int psa_generic_status_to_mbedtls(psa_status_t status)
{
    switch (status) {
        case PSA_SUCCESS:
            return 0;
        case PSA_ERROR_NOT_SUPPORTED:
            return MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;
        case PSA_ERROR_CORRUPTION_DETECTED:
            return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
        case PSA_ERROR_COMMUNICATION_FAILURE:
        case PSA_ERROR_HARDWARE_FAILURE:
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        case PSA_ERROR_NOT_PERMITTED:
        default:
            return MBEDTLS_ERR_ERROR_GENERIC_ERROR;
    }
}

int psa_status_to_mbedtls(psa_status_t status,
                          const mbedtls_error_pair_t *local_translations,
                          size_t local_errors_num,
                          int (*fallback_f)(psa_status_t))
{
    for (size_t i = 0; i < local_errors_num; i++) {
        if (status == local_translations[i].psa_status) {
            return local_translations[i].mbedtls_error;
        }
    }
    return fallback_f(status);
}

int psa_pk_status_to_mbedtls(psa_status_t status)
{
    switch (status) {
        case PSA_ERROR_INVALID_HANDLE:
            return MBEDTLS_ERR_PK_KEY_INVALID_FORMAT;
        case PSA_ERROR_BUFFER_TOO_SMALL:
            return MBEDTLS_ERR_PK_BUFFER_TOO_SMALL;
        case PSA_ERROR_NOT_SUPPORTED:
            return MBEDTLS_ERR_PK_FEATURE_UNAVAILABLE;
        case PSA_ERROR_INVALID_ARGUMENT:
            return MBEDTLS_ERR_PK_INVALID_ALG;
        case PSA_ERROR_INSUFFICIENT_MEMORY:
            return MBEDTLS_ERR_PK_ALLOC_FAILED;
        case PSA_ERROR_BAD_STATE:
            return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
        case PSA_ERROR_DATA_CORRUPT:
        case PSA_ERROR_DATA_INVALID:
        case PSA_ERROR_STORAGE_FAILURE:
            return MBEDTLS_ERR_PK_FILE_IO_ERROR;
        default:
            return psa_generic_status_to_mbedtls(status);
    }
}
#endif /* MBEDTLS_PSA_CRYPTO_C */
