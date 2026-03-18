// Copyright 2022-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#include <soc/soc.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AES_GCM_BLOCK_SIZE      (16)
#define AES_GCM_KEY_LENGTH      (32)
#define AES_GCM_IV_LENGTH       (12)

#define AES_GCM_MALLOC_FAIL     (-2)
#define AES_GCM_PARAMETER_FAIL  (-3)
#define AES_GCM_LENGTH_NO_16_ALIGH  (-4)
typedef struct
{
	uint8_t item_id;            //represents the otp2_id (_otp.h).
	unsigned int out_length;    //represents the data length after aes_gcm operation.
}aes_gcm_context_t;

typedef int32_t psa_status_t;

/**
 * @brief aes gcm decryption
 *
 *
 * @attention
 * - This API is used to do aes_gcm decryption with the input key.
 *
 * @param
 * -ex_ctx[in] :  aes_gcm context include otp2's item_id¡¢ and output_length .
 * -input[in]  : the ciphertext required for the aes_gcm decryption process
 * -in_len[in] : length of the ciphertext required for aes_gcm decryption
 * -output[out]: used to store decrypted data after decryption.
 * @return
 *  - 0: succeed
 *  - others: other errors.
 */

psa_status_t psa_aes_gcm_decrypt(aes_gcm_context_t *ex_ctx, unsigned char*input,size_t in_len,unsigned char*output);

/**
 * @brief aes gcm encryption
 *
 *
 * @attention
 * - This API is used to do aes_gcm encryption with the input key.
 *
 * @param
 * -ex_ctx[in]  :  aes_gcm context include otp's item_id ¡¢utput_length .
 * -input[in]   : the plaintext required for the aes_gcm encryption process
 * -in_len[in]  : length of the plaintext required for aes_gcm encryption
 * -output[out] : used to store encrypted data after encryption.
 * @return
 *  - 0: succeed
 *  - others: other errors.
 */
psa_status_t psa_aes_gcm_encrypt(aes_gcm_context_t *ex_ctx, unsigned char*input, size_t in_len, unsigned char*output);

psa_status_t psa_register_dump_callback(uint32_t cb_address, uint32_t cb_arg);

#ifdef __cplusplus
}
#endif
