/******************************************************************
*
* Copyright 2016 Microsoft Corporation
*
*
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************/

#include "logger.h"
#include "mbedtls/pkcs5.h"
#include "mbedtls/md.h"

#define TAG "OIC_SEC_PBDKF2"

int DeriveCryptoKeyFromPassword(const unsigned char *passwd, size_t pLen,
    const uint8_t *salt, const size_t saltLen,
    const size_t iterations,
    const size_t keyLen, uint8_t *derivedKey)
{
    mbedtls_md_context_t sha_ctx;
    const mbedtls_md_info_t *info_sha;
    int ret = -1;

    /* Setup the hash/HMAC function, for the PBKDF2 function. */
    mbedtls_md_init(&sha_ctx);

    info_sha = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    if (info_sha == NULL)
    {
        OIC_LOG(ERROR, TAG, "Failed to get hash information");
        return ret;
    }

    ret = mbedtls_md_setup(&sha_ctx, info_sha, 1);
    if (ret != 0)
    {
        OIC_LOG(ERROR, TAG, "Failed to setup hash function");
        return ret;
    }

    ret = mbedtls_pkcs5_pbkdf2_hmac(&sha_ctx, passwd, pLen, salt, saltLen, iterations, keyLen, derivedKey);
    if (ret != 0)
    {
        OIC_LOG(ERROR, TAG, "Call to mbedtls PBKDF2 function failed");
    }

    mbedtls_md_free(&sha_ctx);
    return ret;
}
