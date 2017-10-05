/******************************************************************
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
******************************************************************/

#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "ocstack.h"
#include "octypes.h"
#include "psiutils.h"
#include "psinterface.h"
#include "oic_malloc.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/aes.h"
#include "mbedtls/md.h"

#define TAG "OIC_SEC_PSIUTIL"

/*Internal to this File only*/
#define BLOCK_LEN 16
#define KEY_LEN 256
#define IV_LEN 16
#define DIGEST_LEN 32

#define VERIFY_NON_NULL(tag, arg, logLevel) do{ if (NULL == (arg)) \
    { OIC_LOG((logLevel), tag, #arg " is NULL"); goto exit; } }while(0)

static unsigned char aesKey[AES_KEY_SIZE];
bool isKeySet = false;

OCStackResult psiSetKey(const unsigned char* key)
{
    if (NULL == key)
    {
        return OC_STACK_INVALID_PARAM;
    }
    memcpy(aesKey,key,AES_KEY_SIZE);
    isKeySet = true;
    return OC_STACK_OK;
}

OCStackResult psiGetKey(unsigned char* key)
{
    if(false == isKeySet)
    {
        return OC_STACK_ERROR;
    }
    memcpy(key, aesKey, AES_KEY_SIZE);
    return OC_STACK_OK;
}

bool psiIsKeySet()
{
    return isKeySet;
}

static int GenerateIV(unsigned char *IV)
{
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    char *pers = "generate psi iv vector";
    int ret;

    mbedtls_entropy_init( &entropy );
    mbedtls_ctr_drbg_init(&ctr_drbg);

    if((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                    (unsigned char *) pers, strlen(pers))) != 0 )
    {
        OIC_LOG(ERROR, TAG, "mbedtls_ctr_drbg_init() Failed!!");
        return 1;
    }

    if((ret = mbedtls_ctr_drbg_random(&ctr_drbg, IV, 16)) != 0 )
    {
        OIC_LOG(ERROR, TAG, "mbedtls_ctr_drbg_random Failed!!");
        return 1;
    }
    return 0;
}

/*
 * User need to OICFree() *ciphertext after using it.
 */
int psiEncrypt(const unsigned char *plaintext, size_t pt_len,
        unsigned char **ciphertext, size_t *ct_len)
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);

    unsigned char IV[IV_LEN];
    unsigned char digest[DIGEST_LEN] , key[AES_KEY_SIZE];
    size_t padLen = BLOCK_LEN - (pt_len % BLOCK_LEN);
    int ret = 0;

    if (!plaintext || !pt_len || !ciphertext || *ciphertext || !ct_len)
    {
        OIC_LOG_V(ERROR, TAG, "%s: %s is NULL",
                __func__, !plaintext ? "plaintext" : !pt_len ? "pt_len"
                : !ciphertext || *ciphertext ? "ciphertext" : "ct_len");
        ret = 1;
        goto exit;
    }

    /*
       -----------------------------------------
       |  16 Bytes |  Plain/cipher    |32 Bytes|
       |   IV      |  text + PadLen   | (H)MAC |
       -----------------------------------------
    */
    int tempBufSize = (IV_LEN + pt_len + padLen + DIGEST_LEN), mod;
    unsigned char *tempBuf = (unsigned char*)OICCalloc(1,
            sizeof(unsigned char) * tempBufSize);
    VERIFY_NON_NULL(TAG, tempBuf, ERROR);

    if (OC_STACK_OK != psiGetKey(key))
    {
        OIC_LOG(ERROR, TAG, "Failed to get Key");
        ret = 1;
        goto exit;
    }

    /* first IV_LEN bytes for Init. vector*/
    memcpy(tempBuf + IV_LEN , plaintext, pt_len);
    unsigned int i;
    for (i = 0; i < padLen; ++i)
    {
        tempBuf[IV_LEN + pt_len + i] = padLen;
    }

    if (GenerateIV(IV))
    {
        OIC_LOG(ERROR, TAG, "IV Generation Failed!!");
        ret = 1;
        goto exit;
    }

    //last 4 bits of IV will store pt_len%BLOCK_LEN
    //we will use these to recover padLen in decryption.
    mod = (int)(pt_len & 0x0F);
    IV[IV_LEN-1] = (unsigned char)((IV[IV_LEN -1] & 0xF0) | mod);

    //store IV
    memcpy(tempBuf, IV, IV_LEN);

    mbedtls_aes_context aes_ctx;
    mbedtls_aes_init(&aes_ctx);

    mbedtls_aes_setkey_enc(&aes_ctx, key, KEY_LEN);

    /*plain data start at an offset of IV_LEN*/
    mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_ENCRYPT, (pt_len + padLen),
            IV, (tempBuf+IV_LEN), (tempBuf+IV_LEN));

    mbedtls_md_context_t sha_ctx;
    mbedtls_md_init(&sha_ctx);

    if(0 != mbedtls_md_setup(&sha_ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1))
    {
        OIC_LOG(ERROR, TAG, "mbedtls_md_setup() Failed!!");
        ret = 1;
        goto exit;
    }

    /* setup the AES and HMAC context*/
    memset(digest, 0, DIGEST_LEN);

    mbedtls_md_hmac_starts(&sha_ctx, key, DIGEST_LEN);
    mbedtls_md_hmac_update(&sha_ctx, tempBuf + IV_LEN, (pt_len+padLen));
    mbedtls_md_hmac_finish(&sha_ctx, digest);
    mbedtls_md_free(&sha_ctx);

    //copy last 32 bytes for MAC
    memcpy((tempBuf + IV_LEN + pt_len + padLen), digest, DIGEST_LEN);

    memset(key, 0, AES_KEY_SIZE); //destroy key

    *ciphertext = tempBuf;
    *ct_len = tempBufSize;
exit:
    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return ret;
}

/*
 * plaintext will be free by App
 */
int psiDecrypt(const unsigned char *ciphertext, size_t ct_len,
        unsigned char **plaintext, size_t *pt_len)
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);

    int mod, len;
    unsigned char IV[IV_LEN], digest[DIGEST_LEN];
    unsigned char key[AES_KEY_SIZE];
    unsigned char *pt;
    OCStackResult ret = 0;

    if (!ciphertext || !ct_len || !plaintext || *plaintext || !pt_len)
    {
        OIC_LOG_V(ERROR, TAG, "%s: %s is NULL",
                __func__, !ciphertext ? "ciphertext" : !ct_len ? "ct_len"
                : !plaintext || *plaintext ? "plaintext" : "pt_len");
        ret = 1;
        goto exit;
    }


    if (OC_STACK_OK != psiGetKey(key))
    {
        OIC_LOG(ERROR, TAG, "Failed to get Key");
        ret = 1;
        goto exit;
    }

    memcpy(IV, ciphertext, IV_LEN);
    //restore mod
    mod = IV[IV_LEN-1] & 0x0F;

    //Length of original data was..
    //Avoid writing Padding, IV & HMAC into the SVR DB File
    len = ct_len - (BLOCK_LEN - mod) - (IV_LEN + DIGEST_LEN);
    pt = (unsigned char*)OICCalloc(1, ct_len *sizeof(unsigned char));
    VERIFY_NON_NULL(TAG, pt, ERROR);

    //decrypt
    mbedtls_aes_context aes_ctx;
    mbedtls_aes_init(&aes_ctx);

    mbedtls_aes_setkey_dec(&aes_ctx, key, KEY_LEN);
    mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_DECRYPT, ct_len - IV_LEN,
            IV, (ciphertext+IV_LEN), pt);

    //hash check
    mbedtls_md_context_t sha_ctx;
    mbedtls_md_init(&sha_ctx);

    if (0 != mbedtls_md_setup(&sha_ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1))
    {
        OIC_LOG(ERROR, TAG, "mbedtls_md_setup() Failed!!");
        ret = 1;
        goto exit;
    }

    memset(digest, 0, DIGEST_LEN);
    mbedtls_md_hmac_starts(&sha_ctx, key, DIGEST_LEN);
    mbedtls_md_hmac_update(&sha_ctx, ciphertext + IV_LEN, (ct_len - DIGEST_LEN - IV_LEN));
    mbedtls_md_hmac_finish(&sha_ctx, digest);
    mbedtls_md_free(&sha_ctx);
    memset(key, 0, AES_KEY_SIZE); //destroy key

    int diff = 0;
    for (int i = 0; i < DIGEST_LEN; i++)
    {
        diff |= digest[i] ^ ciphertext[ct_len - DIGEST_LEN + i];
    }
    if (diff != 0)
    {
        OIC_LOG(ERROR, TAG, "HMAC check failed: wrong key, "
                "or file corrupted.");
        ret = 1;
        goto exit;
    }
    *plaintext = pt;
    *pt_len = len;
exit:
    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return ret;
}
