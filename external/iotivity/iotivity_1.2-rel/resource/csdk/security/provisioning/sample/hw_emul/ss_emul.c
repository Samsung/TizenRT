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

#include "oic_malloc.h"
#include "logger.h"
#include "ss_emul.h"


#define TAG "OIC_SSEMUL"

#define HWIF_MAXPATH (4096)
static char HWIF_OWNCERT_FILE_NAME[HWIF_MAXPATH] = {0,};
static char HWIF_KEY_FILE_NAME[HWIF_MAXPATH] = {0,};
static char HWIF_KEY_PASS[HWIF_MAXPATH] = {0,};


/********************************************
 * utility functions
 */

int LoadCertFile(const char* filepath, uint8_t** cert_chain, size_t* cert_chain_len)
{
    if (NULL == filepath || NULL == cert_chain || NULL == cert_chain_len)
    {
        OIC_LOG(ERROR, TAG, "Invalid parameters");
        return -1;
    }

    uint8_t *certchain;
    size_t certLen;

    FILE *fp = fopen(filepath, "rb");
    if (fp)
    {
        char buffer[1024];
        size_t fsize = 0;
        size_t bytesRead = 0;
        do
        {
            bytesRead = fread(buffer, 1, sizeof(buffer), fp);
            fsize += bytesRead;
        } while (bytesRead);

        if (!fsize)
        {
            OIC_LOG(ERROR,TAG,"File is empty");
            fclose(fp);
            return -1;
        }

        certchain = (uint8_t*)OICCalloc(1, fsize);
        certLen = fsize;
        if (NULL == certchain)
        {
            OIC_LOG(ERROR,TAG,"Failed to allocate memory");
            fclose(fp);
            return -1;
        }

        rewind(fp);
        if (fsize != fread(certchain, 1, fsize, fp))
        {
            OIC_LOG(ERROR, TAG, "Certiface was not read completely");
        }
        fclose(fp);
    }
    else
    {
        OIC_LOG_V(ERROR,TAG,"Failed to open cert file : %s", filepath);
        return -1;
    }

    *cert_chain = certchain;
    *cert_chain_len = certLen;

    OIC_LOG_V(INFO, TAG, "Loading cert success [%s]", filepath);
    OIC_LOG_BUFFER(DEBUG, TAG, certchain, certLen);
    return 0;
}


int LoadKeyFile(mbedtls_pk_context *pkey, const char* filepath, const char* password)
{
    if(!filepath)
    {
        OIC_LOG(ERROR, TAG, "Invalid key file path" );
        return -1;
    }

    int ret = mbedtls_pk_parse_keyfile(pkey, filepath, password);
    if( ret != 0 )
    {
        OIC_LOG_V(ERROR, TAG, "Faile to parse key file [0x%x]", ret );
        return ret;
    }

    OIC_LOG_V(INFO, TAG, "Loading key success [%s]", filepath);
    return ret;
}


/********************************************
 * HW Secure Storage emulation functions
 */
int SSemulSetCertkeyFilepath(const char* cert_filepath,
                                            const char* key_filepath, const char* pwd)
{
    if (NULL == cert_filepath || NULL == key_filepath)
    {
        OIC_LOG(ERROR, TAG, "Invalid parameters");
        return -1;
    }

    if (HWIF_MAXPATH <= strlen(cert_filepath) || !strlen(cert_filepath)
        || HWIF_MAXPATH <= strlen(key_filepath) || !strlen(key_filepath)
        || (pwd && (HWIF_MAXPATH <= strlen(pwd))))
    {
        OIC_LOG(ERROR, TAG, "Invalid large path length");
        return -1;
    }

    memset(HWIF_OWNCERT_FILE_NAME, 0, HWIF_MAXPATH);
    memset(HWIF_KEY_FILE_NAME, 0, HWIF_MAXPATH);
    memset(HWIF_KEY_PASS, 0, HWIF_MAXPATH);

    strncpy(HWIF_OWNCERT_FILE_NAME, cert_filepath, strlen(cert_filepath));
    strncpy(HWIF_KEY_FILE_NAME, key_filepath, strlen(key_filepath));
    if (pwd)
    {
        strncpy(HWIF_KEY_PASS, pwd, strlen(pwd) + 1);
    }

    OIC_LOG(INFO, TAG, "[Configure Secure Storage Emulation files]");
    OIC_LOG_V(INFO, TAG, "OwnCert file[%zu]: %s", strlen(cert_filepath), cert_filepath);
    OIC_LOG_V(INFO, TAG, "Key file[%zu]: %s", strlen(key_filepath), key_filepath);
    if (pwd)
    {
        OIC_LOG_V(INFO, TAG, "Password[%zu]: %s", strlen(pwd), pwd);
    }
    return 0;
}

int SSemulGetKeytype(const void* keyContext)
{
    (void)keyContext;
    if (0 == strlen(HWIF_KEY_FILE_NAME))
    {
        OIC_LOG(ERROR, TAG, "Need to set key file name");
        return KEYTYPE_NONE;
    }

    int ret = KEYTYPE_NONE;
    mbedtls_pk_context pk;
    mbedtls_pk_init(&pk);

    // 1. load key
    const char* key_file = HWIF_KEY_FILE_NAME;
    const char* key_pass = !strlen(HWIF_KEY_PASS) ? NULL : HWIF_KEY_PASS;
    if (0 > LoadKeyFile(&pk, key_file, key_pass))
    {
        OIC_LOG(ERROR, TAG, "Fail to load key file");
        goto exit;
    }

    // 2. get key type
    mbedtls_pk_type_t keytype = mbedtls_pk_get_type(&pk);
    if(MBEDTLS_PK_RSA == keytype)
    {
        ret = KEYTYPE_RSA;
    }
    else if(MBEDTLS_PK_ECKEY == keytype ||
               MBEDTLS_PK_ECKEY_DH == keytype)
    {
        ret = KEYTYPE_ECC;
    }

exit:
    mbedtls_pk_free(&pk);
    return ret;
}

int SSemulLoadOwncert(const void* keyContext,
                                           uint8_t** cert_chain, size_t* cert_chain_len)
{
    (void)keyContext;
    if (0 == strlen(HWIF_OWNCERT_FILE_NAME))
    {
        OIC_LOG(ERROR, TAG, "Need to set owncert file name");
        return -1;
    }

    if (0 != LoadCertFile(HWIF_OWNCERT_FILE_NAME, cert_chain, cert_chain_len))
    {
        OIC_LOG(ERROR, TAG, "Faile to load the own certificate file");
        return -1;
    }

    return 0;
}

int SSemulGetKeylen(const void* keyContext)
{
    (void)keyContext;
    if (0 == strlen(HWIF_KEY_FILE_NAME))
    {
        OIC_LOG(ERROR, TAG, "Need to set private key file name");
        return -1;
    }

    int ret = -1;
    size_t key_len = 0;
    mbedtls_pk_context pk;
    mbedtls_pk_init(&pk);

    // 1. load key
    const char* key_file = HWIF_KEY_FILE_NAME;
    const char* key_pass = !strlen(HWIF_KEY_PASS) ? NULL : HWIF_KEY_PASS;
    ret = LoadKeyFile(&pk, key_file, key_pass);
    if (0 > ret)
    {
        OIC_LOG_V(ERROR, TAG, "Fail to load key file [0x%x]", ret);
        goto exit;
    }

    key_len = ( 8 * ((const mbedtls_rsa_context *) pk.pk_ctx)->len );
    if(0 >= key_len)
    {
        OIC_LOG(ERROR, TAG, "Invalid key length");
        goto exit;
    }

exit:
    mbedtls_pk_free(&pk);
    return key_len;
}

int SSemulRsaSign(const void *keyContext,
                            int (*f_rng)(void *, unsigned char *, size_t), void *p_rng,
                            int mode, mbedtls_md_type_t md_alg, unsigned int hashlen,
                            const unsigned char *hash, unsigned char *sig )
{
    if (0 == strlen(HWIF_KEY_FILE_NAME))
    {
        OIC_LOG(ERROR, TAG, "Need to set private key file name");
        return -1;
    }

    // workaround for test under sw - 1. load key, 2. sign

    int ret;
    size_t key_len;
    mbedtls_pk_context pk;
    mbedtls_pk_init(&pk);

    // 1. load key
    const char* key_file = HWIF_KEY_FILE_NAME;
    const char* key_pass = !strlen(HWIF_KEY_PASS) ? NULL : HWIF_KEY_PASS;
    ret = LoadKeyFile(&pk, key_file, key_pass);
    if (0 > ret)
    {
        OIC_LOG_V(ERROR, TAG, "Fail to load key file [0x%x]", ret);
        goto exit;
    }

    key_len = ( 8 * ((const mbedtls_rsa_context *) pk.pk_ctx)->len );
    if(0 >= key_len)
    {
        OIC_LOG(ERROR, TAG, "Invalid key length");
        ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "key name : %s, mode : %d hashlen : %u md_alg : %d keylen : %zu",
            (const char*)(keyContext), mode, hashlen, md_alg, key_len);

    // 2. sign
    ret = mbedtls_rsa_pkcs1_sign( (mbedtls_rsa_context *)pk.pk_ctx, f_rng, p_rng,
                        MBEDTLS_RSA_PRIVATE, md_alg, (unsigned int) hashlen, hash, sig);
    if(0 != ret )
    {
        OIC_LOG_V(ERROR, TAG, "Fail to sign [0x%x]", ret);
        goto exit;
    }

exit:
    mbedtls_pk_free(&pk);
    return ret;
}

int SSemulEcdsaSign( void *ctx, mbedtls_md_type_t md_alg,
                   const unsigned char *hash, size_t hash_len,
                   unsigned char *sig, size_t *sig_len,
                   int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    (void)ctx;
    if (0 == strlen(HWIF_KEY_FILE_NAME))
    {
        OIC_LOG(ERROR, TAG, "Need to set private key file name");
        return -1;
    }

    // workaround for test under sw  - 1. load key, 2. set temp ctx, 3.sign

    int ret;
    mbedtls_pk_context pk;
    mbedtls_ecdsa_context ecdsa;
    mbedtls_ecp_keypair *eckey;
    mbedtls_pk_init(&pk);
    mbedtls_ecdsa_init( &ecdsa );

    // 1. load key
    const char* key_file = HWIF_KEY_FILE_NAME;
    const char* key_pass = !strlen(HWIF_KEY_PASS) ? NULL : HWIF_KEY_PASS;
    ret = LoadKeyFile(&pk, key_file, key_pass);
    if (0 > ret)
    {
        OIC_LOG_V(ERROR, TAG, "Fail to load key file [0x%x]", ret);
        goto exit;
    }

    // 2. set temp ctx
    eckey = (mbedtls_ecp_keypair*)pk.pk_ctx;
    ret = mbedtls_ecdsa_from_keypair(&ecdsa, eckey);
    if(0 != ret )
    {
        OIC_LOG_V(ERROR, TAG, "Fail to copy key pair [0x%x]", ret);
        goto exit;
    }

    // 3. sign
    ret = mbedtls_ecdsa_write_signature( (mbedtls_ecdsa_context *) &ecdsa,
                md_alg, hash, hash_len, sig, sig_len, f_rng, p_rng );
    if(0 != ret )
    {
        OIC_LOG_V(ERROR, TAG, "Fail to sign [0x%x]", ret);
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "Success to sign");

exit:
    mbedtls_ecdsa_free( &ecdsa );
    mbedtls_pk_free(&pk);
    OIC_LOG_V(INFO, TAG, "Out %s", __func__);
    return ret;
}


