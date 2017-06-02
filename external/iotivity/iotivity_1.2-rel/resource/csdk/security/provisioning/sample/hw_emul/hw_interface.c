/* *****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * *****************************************************************/

#include <stdio.h>
#include <string.h>

#include "ocstack.h"
#include "oic_malloc.h"
#include "logger.h"
#include "mbedtls/pk_internal.h"
#include "hw_interface.h"


#define TAG "OIC_HWIF"

#define HWIF_KEY_DEFAULT_ALIAS "HW_KEY_DEFAULT"
#define HWIF_KEY_CONTEXT HWIF_KEY_DEFAULT_ALIAS
#define HWIF_RSA_ALIAS HWIF_KEY_DEFAULT_ALIAS


/********************************************
 * RSA alternative functions
 */

static int RsaDecryptAlt(void *ctx, int mode, size_t *olen,
                                    const unsigned char *input, unsigned char *output,
                                    size_t output_max_len )
{
    (void)ctx;
    (void)mode;
    (void)olen;
    (void)input;
    (void)output;
    (void)output_max_len;

    OIC_LOG_V(INFO, TAG, "In %s", __func__);
    OIC_LOG_V(INFO, TAG, "Out %s", __func__);
    return 0;
}

static int RsaSignAlt(void *ctx,
                                    int (*f_rng)(void *, unsigned char *, size_t), void *p_rng,
                                    int mode, mbedtls_md_type_t md_alg, unsigned int hashlen,
                                    const unsigned char *hash, unsigned char *sig )
{
    OIC_LOG_V(INFO, TAG, "In %s", __func__);

    if(!ctx || 0 != strncmp((const char*)(ctx), HWIF_RSA_ALIAS, strlen(HWIF_RSA_ALIAS)))
    {
        OIC_LOG(ERROR, TAG, "Invalid paramters, ctx must has same key id");
        OIC_LOG_V(INFO, TAG, "Out %s", __func__);
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    if(mode != MBEDTLS_RSA_PRIVATE)
    {
        OIC_LOG(ERROR, TAG, "Invalid mode requested");
        OIC_LOG_V(INFO, TAG, "Out %s", __func__);
        return MBEDTLS_ERR_PK_BAD_INPUT_DATA;
    }

    // RSA encryption with hw stored private key
    int ret = SSemulRsaSign(ctx, f_rng, p_rng, mode, md_alg, hashlen, hash, sig);
    if(0 != ret )
    {
        OIC_LOG_V(ERROR, TAG, "Fail to RSA sign [0x%x]", ret);
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "Success to RSA sign");

exit:
    OIC_LOG_V(INFO, TAG, "Out %s", __func__);
    return ret;
}

static size_t RsaKeyLen( void *ctx )
{
    OIC_LOG_V(INFO, TAG, "In %s", __func__);

    if(!ctx || 0 != strncmp((const char*)(ctx), HWIF_RSA_ALIAS, strlen(HWIF_RSA_ALIAS)))
    {
        OIC_LOG(ERROR, TAG, "Invalid paramters, ctx must has same key id");
        OIC_LOG_V(INFO, TAG, "Out %s", __func__);
        return 0;
    }

    // Load private key length from HW Secure Storage
    int ret = SSemulGetKeylen(ctx);
    if (0 >= ret)
    {
        OIC_LOG_V(ERROR, TAG, "Fail to load key length [0x%x]", ret);
        goto exit;
    }

    OIC_LOG_V(INFO, TAG, "key length : %d", ret);

exit:
    OIC_LOG_V(INFO, TAG, "Out %s", __func__);
    return (size_t)ret;
}


/********************************************
 * ECDSA alternative functions
 */

static int EcdsaSignAlt( void *ctx, mbedtls_md_type_t md_alg,
                   const unsigned char *hash, size_t hash_len,
                   unsigned char *sig, size_t *sig_len,
                   int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    (void)ctx;
    OIC_LOG_V(INFO, TAG, "In %s", __func__);

    OIC_LOG_V(ERROR, TAG, "hash_len : %zu md_alg : %d", hash_len, md_alg);

    // ECDSA signing with hw stored private key
    int ret = SSemulEcdsaSign(ctx, md_alg, hash, hash_len, sig, sig_len, f_rng, p_rng);
    if(0 != ret )
    {
        OIC_LOG_V(ERROR, TAG, "Fail to ECDSA sign [0x%x]", ret);
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "Success to ECDSA sign");

    OIC_LOG_V(DEBUG, TAG, "# [hash - %zu bytes] #", hash_len);
    OIC_LOG_BUFFER(DEBUG, TAG, hash, hash_len);
    OIC_LOG_V(DEBUG, TAG, "# [signature - %zu bytes] #", *sig_len);
    OIC_LOG_BUFFER(DEBUG, TAG, sig, *sig_len);

exit:
    OIC_LOG_V(INFO, TAG, "Out %s", __func__);
    return ret;
}


/********************************************
 * mbedtls pk setup fucntions
 */

int SetupRSAContext(mbedtls_pk_context* ctx, void* key_context)
{
    OIC_LOG_V(INFO, TAG, "In %s", __func__);
    int ret = HWIF_SUCCESS;

    if(NULL == ctx || NULL == key_context)
    {
        OIC_LOG(ERROR, TAG, "Invalid input parameters.");
        ret = HWIF_ERR_INVALID_PARAM;
        goto exit;
    }

    ret = mbedtls_pk_setup_rsa_alt( ctx, HWIF_RSA_ALIAS,
                            RsaDecryptAlt, RsaSignAlt, RsaKeyLen);
    if(0 != ret)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to setup rsa alt [%d]", ret);
        ret = HWIF_ERROR;
        goto exit;
    }

    OIC_LOG(INFO, TAG, "Success to setup RSA alt");

exit:
    OIC_LOG_V(INFO, TAG, "Out %s", __func__);
    return ret;
}

int SetupECDSAContext(mbedtls_pk_context* ctx, void* key_context)
{
    OIC_LOG_V(INFO, TAG, "In %s", __func__);
    int ret = HWIF_SUCCESS;

    if(NULL == ctx || NULL == key_context)
    {
        OIC_LOG(ERROR, TAG, "Invalid input parameters.");
        ret = HWIF_ERR_INVALID_PARAM;
        goto exit;
    }

    // get mbedtls ecdsa structure
    const mbedtls_pk_info_t *mbedtls_ec_info;
    static mbedtls_pk_info_t ec_info;

    mbedtls_ec_info = mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY);
    if (NULL == mbedtls_ec_info)
    {
        OIC_LOG(ERROR, TAG, "mbedtls_pk_info_from_type error");
        ret = HWIF_ERROR;
        goto exit;
    }

    // set hw sign function
    ec_info = *mbedtls_ec_info;
    ec_info.sign_func = EcdsaSignAlt;
    if (0 != mbedtls_pk_setup(ctx, &ec_info))
    {
        OIC_LOG(ERROR, TAG, "mbedtls_pk_setup error");
        ret = HWIF_ERROR;
        goto exit;
    }

    OIC_LOG(INFO, TAG, "Success to setup ECDSA alt");

exit:
    OIC_LOG_V(INFO, TAG, "Out %s", __func__);
    return ret;
}

/********************************************
 * HW interface callback functions
 */

void* HWGetKeyContext(const char* service, const char* usage, const char* keytype)
{
    (void)keytype;

    OIC_LOG_V(INFO, TAG, "In %s", __func__);
    if(NULL == service || NULL == usage)
    {
        OIC_LOG(ERROR, TAG, "Invalid input parameters. service and usage should be not NULL.");
        return NULL;
    }

    // Implementation should return allocated key context depend on hw library
    // key context is related to service, usage and keytype(optional), which is same with alias name.
    // As for now, Iotivity stack is defining following the service and the usage as a default.
    // Refer to pkix_interface.h file
      // #define HWKEY_SVC_IOTIVITY "iotivity"
      // #define HWKEY_USAGE_PRIMARY "primary"

    OIC_LOG_V(INFO, TAG, "Out %s", __func__);
    return (void*)HWIF_KEY_CONTEXT;
}

int HWFreeKeyContext(void* keyContext)
{
    OIC_LOG_V(INFO, TAG, "In %s", __func__);
    if(NULL == keyContext)
    {
        OIC_LOG(ERROR, TAG, "key context value is NULL.");
        return HWIF_ERR_INVALID_PARAM;
    }

    // Implementation should free key context memory if allocated

    OIC_LOG_V(INFO, TAG, "Out %s", __func__);
    return HWIF_SUCCESS;
}

int HWGetOwnCertificateChain(const void* keyContext,
                    uint8_t** cert_chain, size_t* cert_chain_len)
{
    OIC_LOG_V(INFO, TAG, "In %s", __func__);
    int ret = HWIF_SUCCESS;

    if(NULL == keyContext || NULL == cert_chain || NULL == cert_chain_len)
    {
        OIC_LOG(ERROR, TAG, "Invalid input parameters.");
        ret = HWIF_ERR_INVALID_PARAM;
        goto exit;
    }

    // Load own certificate from HW Secure Storage
    ret = SSemulLoadOwncert(keyContext, cert_chain, cert_chain_len);
    if (0 != ret)
    {
        OIC_LOG(ERROR, TAG, "Faile to load the own certificate from Secure Storage");
        ret = HWIF_ERROR;
        goto exit;
    }
    OIC_LOG_V(DEBUG, TAG, "cert chain size : %zu", *cert_chain_len);

exit:
    OIC_LOG_V(INFO, TAG, "Out %s", __func__);
    return ret;
}

int HWSetupPkContext(mbedtls_pk_context* ctx, void* key_context)
{
    OIC_LOG_V(INFO, TAG, "In %s", __func__);
    int ret = HWIF_SUCCESS;

    if(NULL == ctx || NULL == key_context)
    {
        OIC_LOG(ERROR, TAG, "Invalid input parameters.");
        ret = HWIF_ERR_INVALID_PARAM;
        goto exit;
    }

    int keytype = SSemulGetKeytype(key_context);
    if (KEYTYPE_RSA == keytype)
    {
        ret = SetupRSAContext(ctx, key_context);
    }
    else if (KEYTYPE_ECC == keytype)
    {
        ret = SetupECDSAContext(ctx, key_context);
    }

exit:
    OIC_LOG_V(INFO, TAG, "Out %s", __func__);
    return ret;
}



