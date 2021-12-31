/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>
#include <mbedtls/pk.h>
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_AES_C)
#include <string.h>
#include "mbedtls/aes.h"
#if defined(MBEDTLS_PADLOCK_C)
#include "mbedtls/padlock.h"
#endif
#if defined(MBEDTLS_AESNI_C)
#include "mbedtls/aesni.h"
#endif
#include "mbedtls/alt/common.h"
#if defined(MBEDTLS_SELF_TEST)
#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */
#endif /* MBEDTLS_SELF_TEST */

#if defined(MBEDTLS_AES_ALT)

static hal_key_type _mbedtls_get_aes_keytype(int keybits)
{
	hal_key_type key_type;
	switch (keybits) {
	case 128:
		key_type = HAL_KEY_AES_128;
		break;
	case 192:
		key_type = HAL_KEY_AES_192;
		break;
	case 256:
		key_type = HAL_KEY_AES_256;
		break;
	default:
		key_type = HAL_KEY_UNKNOWN;
		break;
	}
	return key_type;
}

void mbedtls_aes_init(mbedtls_aes_context *ctx)
{
	int ret = 0;
	memset(ctx, 0x0, sizeof(mbedtls_aes_context));
	if ((ret = sl_init(&ctx->shnd)) != SECLINK_OK) {
		assert(0);
		return;
	}
	ctx->enc_key_idx = KEY_INDEX_INVALID;
	ctx->dec_key_idx = KEY_INDEX_INVALID;
}

void mbedtls_aes_free(mbedtls_aes_context *ctx)
{
	if (ctx->enc_key_idx != KEY_INDEX_INVALID) {
		sl_remove_key(ctx->shnd, ctx->enc_key_type, ctx->enc_key_idx);
	}
	if (ctx->dec_key_idx != KEY_INDEX_INVALID) {
		sl_remove_key(ctx->shnd, ctx->dec_key_type, ctx->dec_key_idx);
	}
	sl_deinit(ctx->shnd);
}

int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key,
							 unsigned int keybits)
{
	hal_key_type key_type = _mbedtls_get_aes_keytype(keybits);
	if (key_type == HAL_KEY_UNKNOWN) {
		return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
	}

	if (ctx->enc_key_idx != KEY_INDEX_INVALID) {
		sl_remove_key(ctx->shnd, ctx->enc_key_type, ctx->enc_key_idx);
	}
	hal_data aeskey = HAL_DATA_INITIALIZER;
	aeskey.data = (void *)key;
	aeskey.data_len = keybits >> 3;

	int key_idx = alt_set_key(ctx->shnd, key_type, &aeskey, NULL, 32);
	if (key_idx == -1) {
		return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
	}
	ctx->enc_key_idx = key_idx;
	ctx->enc_key_type = key_type;

	return 0;
}

int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key,
							 unsigned int keybits)
{
	hal_key_type key_type = _mbedtls_get_aes_keytype(keybits);
	if (key_type == HAL_KEY_UNKNOWN) {
		return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
	}

	if (ctx->dec_key_idx != KEY_INDEX_INVALID) {
		sl_remove_key(ctx->shnd, ctx->dec_key_type, ctx->dec_key_idx);
	}
	hal_data aeskey = HAL_DATA_INITIALIZER;
	aeskey.data = (void *)key;
	aeskey.data_len = keybits >> 3;

	int key_idx = alt_set_key(ctx->shnd, key_type, &aeskey, NULL, 32);
	if (key_idx == -1) {
		return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
	}
	ctx->dec_key_idx = key_idx;
	ctx->dec_key_type = key_type;

	return 0;
}

int mbedtls_aes_crypt_ecb(mbedtls_aes_context *ctx,
							int mode,
							const unsigned char input[16],
							unsigned char output[16])
{
	hal_data aes_in = {(void *)input, 16, NULL, 0};
	hal_data aes_out = {(void *)output, 16, NULL, 0};
	hal_aes_param aes_param = {HAL_AES_ECB_NOPAD, NULL, 0, 0, NULL, NULL, NULL};
	int ret = 0;

	if (mode == MBEDTLS_AES_ENCRYPT) {
		ret = sl_aes_encrypt(ctx->shnd, &aes_in, &aes_param, ctx->enc_key_idx, &aes_out);
	} else if (mode == MBEDTLS_AES_DECRYPT) {
		ret = sl_aes_decrypt(ctx->shnd, &aes_in, &aes_param, ctx->dec_key_idx, &aes_out);
	}

	if (ret != SECLINK_OK) {
		return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
	}

	return 0;
}

int mbedtls_aes_crypt_cbc(mbedtls_aes_context *ctx,
							int mode,
							size_t length,
							unsigned char iv[16],
							const unsigned char *input,
							unsigned char *output)
{
	hal_data aes_in = {(void *)input, 16, NULL, 0};
	hal_data aes_out = {(void *)output, 16, NULL, 0};
	hal_aes_param aes_param = {HAL_AES_CBC_NOPAD, iv, 16, 0, NULL, NULL, NULL};
	int ret = 0;

	if (mode == MBEDTLS_AES_ENCRYPT) {
		ret = sl_aes_encrypt(ctx->shnd, &aes_in, &aes_param, ctx->enc_key_idx, &aes_out);
	} else if (mode == MBEDTLS_AES_DECRYPT) {
		ret = sl_aes_decrypt(ctx->shnd, &aes_in, &aes_param, ctx->dec_key_idx, &aes_out);
	}

	if (ret != SECLINK_OK) {
		return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
	}

	return 0;
}

/* note: input buffer and output buffer can have same space */
int mbedtls_aes_crypt_cfb128(mbedtls_aes_context *ctx,
							 int mode,
							 size_t length,
							 size_t *iv_off,
							 unsigned char iv[16],
							 const unsigned char *input,
							 unsigned char *output)
{
	hal_data aes_in = {(void *)input, length, NULL, 0};
	hal_data aes_out = {(void *)output, length, NULL, 0};
	hal_aes_param aes_param = {HAL_AES_CFB128, iv, 16, iv_off, NULL, NULL, NULL};
	int ret = 0;

	if (mode == MBEDTLS_AES_ENCRYPT) {
		ret = sl_aes_encrypt(ctx->shnd, &aes_in, &aes_param, ctx->enc_key_idx, &aes_out);
	} else if (mode == MBEDTLS_AES_DECRYPT) {
		ret = sl_aes_decrypt(ctx->shnd, &aes_in, &aes_param, ctx->enc_key_idx, &aes_out);
	}
	if (ret != SECLINK_OK) {
		return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
	}

	return 0;
}

int mbedtls_aes_crypt_ctr(mbedtls_aes_context *ctx,
							size_t length,
							size_t *nc_off,
							unsigned char nonce_counter[16],
							unsigned char stream_block[16],
							const unsigned char *input,
							unsigned char *output)
{
	hal_data aes_in = {(void *)input, length, NULL, 0};
	hal_data aes_out = {(void *)output, length, NULL, 0};
	hal_aes_param aes_param = {HAL_AES_CTR, NULL, 0, NULL, nc_off, nonce_counter, stream_block};
	int ret = 0;

	ret = sl_aes_encrypt(ctx->shnd, &aes_in, &aes_param, ctx->enc_key_idx, &aes_out);
	if (ret != SECLINK_OK) {
		return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
	}

	return 0;
}

#endif /* MBEDTLS_AES_ALT */

#endif /* MBEDTLS_AES_C */
