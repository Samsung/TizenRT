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
 * RSA
 * Copyright (c) 2006, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef RSA_H
#define RSA_H

struct crypto_rsa_key;

struct crypto_rsa_key *
crypto_rsa_import_public_key(const u8 *buf, size_t len);
struct crypto_rsa_key *
crypto_rsa_import_private_key(const u8 *buf, size_t len);
size_t crypto_rsa_get_modulus_len(struct crypto_rsa_key *key);
int crypto_rsa_exptmod(const u8 *in, size_t inlen, u8 *out, size_t *outlen,
		       struct crypto_rsa_key *key, int use_private);
void crypto_rsa_free(struct crypto_rsa_key *key);

#endif /* RSA_H */
