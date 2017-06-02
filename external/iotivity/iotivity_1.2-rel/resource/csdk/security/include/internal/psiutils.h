//******************************************************************
//
// Copyright 2017 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef IOTVT_SRM_PSIUTILS_H
#define IOTVT_SRM_PSIUTILS_H

#define AES_KEY_SIZE 32

/**
 * @brief Generic Encryption function to encrypt data buffer in plaintext
 * and update in ciphertext and len in ct_len. (AES-CBC-HMAC)
 * @param[in] plaintext plaintext to be encrypted
 * @param[in] pt_len length of plaintext
 * @param[out] ciphertext ciphered text
 * @param[out] ct_len is length of the ciphered text.
 *
 * @return ::0 for Success.
 */
int psiEncrypt(const unsigned char *plaintext, size_t pt_len,
                unsigned char **ciphertext, size_t *ct_len);

/**
 * @brief Generic Decryption function to decrypt data buffer in ciphertext
 * and update in plaintext and len in pt_len. (AES-CBC-HMAC)
 * @param[in] ciphertext ciphered to be decrypted
 * @param[in] ct_len length of cipher text
 * @param[out] plaintext plaintext text
 * @param[out] pt_len is length of the plaintext text.
 *
 * @return ::0 for Success.
 */
int psiDecrypt(const unsigned char *ciphertext, size_t ct_len,
                unsigned char **plaintext, size_t *pt_len);

/**
 * @brief API to set key to psi
 * @param[in] key key used for encryption
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult psiSetKey(const unsigned char* key);

/**
 * @brief API to get key from psi
 * @param[out] key key used for encryption
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult psiGetKey(unsigned char* key);

/**
 * @brief It provides state to set key for encryption
 * @return ::true for setting key.
 */
bool psiIsKeySet();

#endif //IOTVT_SRM_PSIUTILS_H
