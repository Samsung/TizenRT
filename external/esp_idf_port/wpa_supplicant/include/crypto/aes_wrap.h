/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/*
 * AES-based functions
 *
 * - AES Key Wrap Algorithm (128-bit KEK) (RFC3394)
 * - One-Key CBC MAC (OMAC1) hash with AES-128
 * - AES-128 CTR mode encryption
 * - AES-128 EAX mode encryption/decryption
 * - AES-128 CBC
 *
 * Copyright (c) 2003-2007, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef AES_WRAP_H
#define AES_WRAP_H

int __must_check aes_wrap(const u8 *kek, int n, const u8 *plain, u8 *cipher);
int __must_check aes_unwrap(const u8 *kek, int n, const u8 *cipher, u8 *plain);
int __must_check omac1_aes_128_vector(const u8 *key, size_t num_elem,
				      const u8 *addr[], const size_t *len,
				      u8 *mac);
int __must_check omac1_aes_128(const u8 *key, const u8 *data, size_t data_len,
			       u8 *mac);
int __must_check aes_128_encrypt_block(const u8 *key, const u8 *in, u8 *out);
int __must_check aes_128_ctr_encrypt(const u8 *key, const u8 *nonce,
				     u8 *data, size_t data_len);
int __must_check aes_128_eax_encrypt(const u8 *key,
				     const u8 *nonce, size_t nonce_len,
				     const u8 *hdr, size_t hdr_len,
				     u8 *data, size_t data_len, u8 *tag);
int __must_check aes_128_eax_decrypt(const u8 *key,
				     const u8 *nonce, size_t nonce_len,
				     const u8 *hdr, size_t hdr_len,
				     u8 *data, size_t data_len, const u8 *tag);
int __must_check aes_128_cbc_encrypt(const u8 *key, const u8 *iv, u8 *data,
				     size_t data_len);
int __must_check aes_128_cbc_decrypt(const u8 *key, const u8 *iv, u8 *data,
				     size_t data_len);
int __must_check fast_aes_wrap(const uint8_t *kek, int n, const uint8_t *plain, uint8_t *cipher);
int __must_check fast_aes_unwrap(const uint8_t *kek, int n, const uint8_t *cipher, uint8_t *plain);
int __must_check fast_aes_128_cbc_encrypt(const uint8_t *key, const uint8_t *iv, uint8_t *data,
				          size_t data_len);
int __must_check fast_aes_128_cbc_decrypt(const uint8_t *key, const uint8_t *iv, uint8_t *data,
				          size_t data_len);
#endif /* AES_WRAP_H */
