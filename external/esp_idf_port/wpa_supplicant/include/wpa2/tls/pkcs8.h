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
 * PKCS #8 (Private-key information syntax)
 * Copyright (c) 2006-2009, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef PKCS8_H
#define PKCS8_H

struct crypto_private_key * pkcs8_key_import(const u8 *buf, size_t len);
struct crypto_private_key *
pkcs8_enc_key_import(const u8 *buf, size_t len, const char *passwd);

#endif /* PKCS8_H */
