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
 * PKCS #5 (Password-based Encryption)
 * Copyright (c) 2009, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef PKCS5_H
#define PKCS5_H

u8 * pkcs5_decrypt(const u8 *enc_alg, size_t enc_alg_len,
		   const u8 *enc_data, size_t enc_data_len,
		   const char *passwd, size_t *data_len);

#endif /* PKCS5_H */
