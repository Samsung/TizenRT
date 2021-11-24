/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
#pragma once

#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>

/**
 * \brief The AES context-type definition.
 *
 * \note           buf is able to hold 32 extra bytes, which can be used:
 *                 - for alignment purposes if VIA padlock is used, and/or
 *                 - to simplify key expansion in the 256-bit case by
 *                 generating an extra round key
 */
typedef struct
{
	sl_ctx shnd;
	uint32_t enc_key_idx;
	hal_key_type enc_key_type;
	uint32_t dec_key_idx;
	hal_key_type dec_key_type;
	int nr;			  /*!< The number of rounds. */
	uint32_t *rk;	  /*!< AES round keys. */
	uint32_t buf[68]; /*!< Unaligned data buffer. This buffer can
                                     hold 32 extra Bytes, which can be used for
                                     one of the following purposes:
                                     <ul><li>Alignment if VIA padlock is
                                             used.</li>
                                     <li>Simplifying key expansion in the 256-bit
                                         case by generating an extra round key.
                                         </li></ul> */
} mbedtls_aes_context;

void mbedtls_aes_init(mbedtls_aes_context *ctx);
int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key,
						   unsigned int keybits);
int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key,
						   unsigned int keybits);
int mbedtls_aes_crypt_ecb(mbedtls_aes_context *ctx,
						  int mode,
						  const unsigned char input[16],
						  unsigned char output[16]);
int mbedtls_aes_crypt_cbc(mbedtls_aes_context *ctx,
						  int mode,
						  size_t length,
						  unsigned char iv[16],
						  const unsigned char *input,
						  unsigned char *output);
int mbedtls_aes_crypt_cfb128(mbedtls_aes_context *ctx,
							 int mode,
							 size_t length,
							 size_t *iv_off,
							 unsigned char iv[16],
							 const unsigned char *input,
							 unsigned char *output);
int mbedtls_aes_crypt_ctr(mbedtls_aes_context *ctx,
						  size_t length,
						  size_t *nc_off,
						  unsigned char nonce_counter[16],
						  unsigned char stream_block[16],
						  const unsigned char *input,
						  unsigned char *output);
void mbedtls_aes_free(mbedtls_aes_context *ctx);
