/*
 * WPA Supplicant / wrapper functions for libalicrypto
 *
 */

#include "includes.h"

#include "common.h"
#include "crypto.h"

#if 0
#define CRYPTO_DBG WPA_LOGI
#else
#define CRYPTO_DBG(...)
#endif

#if CONFIG_ALIOS
#include "ali_crypto.h"

int des_encrypt(const u8 *clear, const u8 *key, u8 *cypher)
{
	size_t size;
    void *context;

    ali_des_get_ctx_size(DES_ECB, &size);
    context = os_malloc(size);
    if (context == NULL)
        return -1;
    ali_des_init(DES_ECB, true, key, 64,
                 NULL, context);
    ali_des_process(clear, cypher, 8, context);
    os_free(context);

	return 0;
}


int md5_vector(size_t num_elem, const u8 *addr[], const size_t *len, u8 *mac)
{
	size_t size;
    void *context;
    int i;

    ali_hash_get_ctx_size(MD5, &size);

    context = os_malloc(size);
    if (context == NULL) {
        return -1;
    }
    os_memset(context, 0, size);

    ali_hash_init(MD5, context);
    for (i = 0; i < num_elem; i++) {
	    ali_hash_update(addr[i], len[i], context);
    }
    ali_hash_final(mac, context);
    
    os_free(context);
	return 0;
}


int sha1_vector(size_t num_elem, const u8 *addr[], const size_t *len, u8 *mac)
{
	size_t size;
    void *context;
    int i;

    ali_hash_get_ctx_size(SHA1, &size);

    context = os_malloc(size);
    if (context == NULL) {
        return -1;
    }
    os_memset(context, 0, size);

    ali_hash_init(SHA1, context);
    for (i = 0; i < num_elem; i++) {
	    ali_hash_update(addr[i], len[i], context);
    }
    ali_hash_final(mac, context);
    
    os_free(context);
	return 0;
}

int hmac_sha1_vector(const u8 *key, size_t key_len, size_t num_elem,
		     const u8 *addr[], const size_t *len, u8 *mac)
{
	size_t size;
    void *context;
    int i;
    
    ali_hmac_get_ctx_size(SHA1, &size);
    context = os_malloc(size);
    if (context == NULL) {
        return -1;
    }
    os_memset(context, 0, size);

    ali_hmac_init(SHA1, key, key_len, context);
    for (i = 0; i < num_elem; i++) {
	    ali_hmac_update(addr[i], len[i], context);
    }
    ali_hmac_final(mac, context);
    
    os_free(context);

    return 0;
}


int hmac_sha1(const u8 *key, size_t key_len, const u8 *data, size_t data_len,
	       u8 *mac)
{
	return hmac_sha1_vector(key, key_len, 1, &data, &data_len, mac);
}

int hmac_md5_vector(const u8 *key, size_t key_len, size_t num_elem,
		     const u8 *addr[], const size_t *len, u8 *mac)
{
	size_t size;
    void *context;
    int i;

    ali_hmac_get_ctx_size(MD5, &size);

    context = os_malloc(size);
    if (context == NULL) {
        return -1;
    }
    os_memset(context, 0, size);

    ali_hmac_init(MD5, key, key_len, context);
    for (i = 0; i < num_elem; i++) {
	    ali_hmac_update(addr[i], len[i], context);
    }
    ali_hmac_final(mac, context);
    
    os_free(context);
    return 0;
}


int hmac_md5(const u8 *key, size_t key_len, const u8 *data, size_t data_len,
	       u8 *mac)
{
	return hmac_md5_vector(key, key_len, 1, &data, &data_len, mac);
}

void * aes_encrypt_init(const u8 *key, size_t len)
{
    size_t size;
    void *context;
    int ret;

    ali_aes_get_ctx_size(AES_ECB, &size);

    context = os_malloc(size);
    if (context == NULL) {
        return NULL;
    }
    os_memset(context, 0, size);
    
	ret = ali_aes_init(AES_ECB, true, key, NULL,
                 len, NULL, context);

    if (ret != 0) {
        os_free(context);
        return NULL;
    }
	return context;
}

int aes_encrypt(void *ctx, const u8 *plain, u8 *crypt)
{
	ali_aes_process(plain, crypt, 16, ctx);

	return 0;
}

void aes_encrypt_deinit(void *ctx)
{
	os_free(ctx);
}

void * aes_decrypt_init(const u8 *key, size_t len)
{
	size_t size;
    void *context;
    int ret;

    ali_aes_get_ctx_size(AES_ECB, &size);

    context = os_malloc(size);
    if (context == NULL) {
        return NULL;
    }
    os_memset(context, 0, size);
    
	ret = ali_aes_init(AES_ECB, false, key, NULL,
                 len, NULL, context);

    if (ret != 0) {
        os_free(context);
        return NULL;
    }
	return context;
}

int aes_decrypt(void *ctx, const u8 *crypt, u8 *plain)
{
	ali_aes_process(crypt, plain, 16, ctx);

	return 0;
}

void aes_decrypt_deinit(void *ctx)
{
	os_free(ctx);
}
#elif CONFIG_USE_MBEDTLS
#include "mbedtls/platform.h"
#include "mbedtls/des.h"
#include "mbedtls/md5.h"
#include "mbedtls/sha1.h"
#include "mbedtls/md.h"
#include "mbedtls/aes.h"

int des_encrypt(const u8 *clear, const u8 *key, u8 *cypher)
{
    mbedtls_des_context des;

    mbedtls_des_init(&des);
    mbedtls_des_setkey_enc(&des, key);
    mbedtls_des_crypt_cbc(&des, MBEDTLS_DES_ENCRYPT, 8, key, clear, cypher);
    mbedtls_des_free(&des);

	return 0;
}

int md5_vector(size_t num_elem, const u8 *addr[], const size_t *len, u8 *mac)
{
    mbedtls_md5_context MD;
    int i;

    mbedtls_md5_init(&MD); /* initialize MD5 */
    mbedtls_md5_starts(&MD);

    for (i = 0; i < num_elem; i++) {
	    mbedtls_md5_update(&MD, addr[i], len[i]);
    }
    mbedtls_md5_finish(&MD, mac); /* tell MD5 we're done */

    mbedtls_md5_free(&MD);
	return 0;
}

int sha1_vector(size_t num_elem, const u8 *addr[], const size_t *len, u8 *mac)
{
    mbedtls_sha1_context SH;
    int i;

    mbedtls_sha1_init(&SH); /* initialize SHA */
    mbedtls_sha1_starts(&SH);

    for (i = 0; i < num_elem; i++) {
	    mbedtls_sha1_update(&SH, addr[i], len[i]);
    }
    mbedtls_sha1_finish(&SH, mac); /* tell SHA we're done */

    mbedtls_sha1_free(&SH);
	return 0;
}

int hmac_sha1_vector(const u8 *key, size_t key_len, size_t num_elem,
		     const u8 *addr[], const size_t *len, u8 *mac)
{
    const mbedtls_md_info_t *md_info;
    mbedtls_md_context_t ctx;
    int i;
    
    md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA1);

    mbedtls_md_init(&ctx);
    if(mbedtls_md_setup(&ctx, md_info, 1) != 0) {
        CRYPTO_DBG("%s:%d mbedtls_md_setup failed\n", __FUNCTION__, __LINE__);
        return -1;
    }

    if (mbedtls_md_hmac_starts(&ctx, key, key_len) != 0) {
        CRYPTO_DBG("%s:%d mbedtls_md_hmac_starts failed\n", __FUNCTION__, __LINE__);
        goto free_md;
    }
    for (i = 0; i < num_elem; i++) {
	    mbedtls_md_hmac_update(&ctx, addr[i], len[i]);
    }
    mbedtls_md_hmac_finish(&ctx, mac);

    mbedtls_md_free(&ctx);

    return 0;

free_md:
    mbedtls_md_free(&ctx);
    return -1;
}


int hmac_sha1(const u8 *key, size_t key_len, const u8 *data, size_t data_len,
	       u8 *mac)
{
	return hmac_sha1_vector(key, key_len, 1, &data, &data_len, mac);
}

int hmac_md5_vector(const u8 *key, size_t key_len, size_t num_elem,
		     const u8 *addr[], const size_t *len, u8 *mac)
{
    const mbedtls_md_info_t *md_info;
    mbedtls_md_context_t ctx;
    int i;

    md_info = mbedtls_md_info_from_type(MBEDTLS_MD_MD5);

    mbedtls_md_init(&ctx);
    if(mbedtls_md_setup(&ctx, md_info, 1) != 0) {
        CRYPTO_DBG("%s:%d mbedtls_md_setup failed\n", __FUNCTION__, __LINE__);
        return -1;
    }

    if (mbedtls_md_hmac_starts(&ctx, key, key_len) != 0) {
        CRYPTO_DBG("%s:%d mbedtls_md_hmac_starts failed\n", __FUNCTION__, __LINE__);
        goto free_md;
    }
    for (i = 0; i < num_elem; i++) {
        mbedtls_md_hmac_update(&ctx, addr[i], len[i]);
    }
    mbedtls_md_hmac_finish(&ctx, mac);

    mbedtls_md_free(&ctx);

    return 0;

free_md:
    mbedtls_md_free(&ctx);
    return -1;
}

int hmac_md5(const u8 *key, size_t key_len, const u8 *data, size_t data_len,
	       u8 *mac)
{
	return hmac_md5_vector(key, key_len, 1, &data, &data_len, mac);
}

void *aes_encrypt_init(const u8 *key, size_t len)
{
    int ret;
    mbedtls_aes_context *aes;

    aes = os_malloc(sizeof(mbedtls_aes_context));
    if (aes == NULL) {
        CRYPTO_DBG("%s:%d os_malloc failed\n", __FUNCTION__, __LINE__);
        return NULL;
    }
    mbedtls_aes_init(aes);

    ret = mbedtls_aes_setkey_enc(aes, key, len * 8);
    if (ret != 0) {
        CRYPTO_DBG("%s:%d mbedtls_aes_setkey_enc failed\n", __FUNCTION__, __LINE__);
        os_free(aes);
        return NULL;
    }

	return aes;
}

int aes_encrypt(void *ctx, const u8 *plain, u8 *crypt)
{
    mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, plain, crypt);

	return 0;
}

void aes_encrypt_deinit(void *ctx)
{
	os_free(ctx);
}

void * aes_decrypt_init(const u8 *key, size_t len)
{
    int ret;
    mbedtls_aes_context *aes;

    aes = os_malloc(sizeof(mbedtls_aes_context));
    if (aes == NULL) {
        CRYPTO_DBG("%s:%d os_malloc failed\n", __FUNCTION__, __LINE__);
        return NULL;
    }
    mbedtls_aes_init(aes);

    ret = mbedtls_aes_setkey_dec(aes, key, len * 8);
    if (ret != 0) {
        CRYPTO_DBG("%s:%d mbedtls_aes_setkey_dec failed\n", __FUNCTION__, __LINE__);
        os_free(aes);
        return NULL;
    }

	return aes;
}

int aes_decrypt(void *ctx, const u8 *crypt, u8 *plain)
{
    mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_DECRYPT, plain, crypt);

	return 0;
}

void aes_decrypt_deinit(void *ctx)
{
	os_free(ctx);
}
#endif
