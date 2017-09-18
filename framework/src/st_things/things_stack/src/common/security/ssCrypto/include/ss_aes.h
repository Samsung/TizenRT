/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef _SS_AES_H_
#define _SS_AES_H_

#include <stdint.h>

// #define the macros below to 1/0 to enable/disable the mode of operation.
//
// CBC enables AES128 encryption in CBC-mode of operation and handles 0-padding.
// ECB enables the basic ECB 16-byte block algorithm. Both can be enabled simultaneously.

// The #ifndef-guard allows it to be configured before #include'ing or at compile time.
#ifndef SS_CBC
#define SS_CBC 1
#endif

#ifndef SS_ECB
#define SS_ECB 1
#endif

#if defined(SS_ECB) && SS_ECB

void ss_aes128_ecb_encrypt(const uint8_t *input, const uint8_t *key, uint8_t *output);
void ss_aes128_ecb_decrypt(const uint8_t *input, const uint8_t *key, uint8_t *output);

#endif							// #if defined(SS_ECB) && SS_ECB

#if defined(SS_CBC) && SS_CBC

void ss_aes128_cbc_encrypt_buffer(uint8_t *output, uint8_t *input, uint32_t length, const uint8_t *key, const uint8_t *iv);
void ss_aes128_cbc_decrypt_buffer(uint8_t *output, uint8_t *input, uint32_t length, const uint8_t *key, const uint8_t *iv);

#endif							// #if defined(SS_CBC) && SS_CBC

#endif							//_SS_AES_H_
