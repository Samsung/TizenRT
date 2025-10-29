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

#ifdef __cplusplus
extern "C" {
#endif

#define SS_TYPE_IS_KEY  		      (1)
#define SCA_AES_KEY_MAX_LENGTH        (32)
#define TOTAL_KEY_STORAGE_INDEX_NUM   (64)
#define SCA_AES_BLOCK_SIZE            (16)
#define SCA_AES_IV_LEN                (16)
#define SCA_AES_NONCE_COUNTER_LEN     (16)
#define SCA_AES_STREAM_BLOCK_LEN      (16)

#define AES_GCM_BLOCK_SIZE      (16)
#define AES_GCM_KEY_LENGTH      (32)
#define AES_GCM_IV_LENGTH       (12)
#define AES_GCM_KEY_SHIFT_BITS   (3)

#define AES_GCM_MALLOC_FAIL     (-2)
#define AES_GCM_LENGTH_NO_16_ALIGH  (-4)

typedef int32_t psa_status_t;

typedef enum {
	/*  AES */
	AES_KEY_128, // 128 bits aes algorithm
	AES_KEY_192, // 192 bits aes algorithm
	AES_KEY_256, // 256 bits aes algorithm
	KEY_UNKNOWN,
} aes_key_type;

typedef enum{
    AES_KEY_128_BITS = 128,
    AES_KEY_192_BITS = 192,
    AES_KEY_256_BITS = 256,
}sca_aes_key_bits_t;

typedef enum{
	SCA_AES_ECB_DECRYPT_MODE = 0,
	SCA_AES_ECB_ENCRYPT_MODE = 1,
	SCA_AES_CBC_DECRYPT_MODE = 2,
	SCA_AES_CBC_ENCRYPT_MODE = 3,
	SCA_AES_CTR_DECRYPT_MODE = 4,
	SCA_AES_CTR_ENCRYPT_MODE = 5,
	SCA_AES_CFB_DECRYPT_MODE = 6,
	SCA_AES_CFB_ENCRYPT_MODE = 7,
	SCA_AES_MODE_MAX         = 8,
}sca_aes_mode_t;

typedef enum {
	SCA_AES_SUCCESS,
	SCA_AES_NOT_INITIALIZED,
	SCA_AES_INVALID_ARGS,
	SCA_AES_LENGTH_NO_ALIGN,
	SCA_AES_MALLOC_FAIL,
	SCA_AES_PARAMETER_FAIL,
	SCA_AES_FAIL,
} sca_aes_result_e;

typedef struct {
	uint8_t key_data[SCA_AES_KEY_MAX_LENGTH];
	uint32_t key_idx;
	sca_aes_key_bits_t key_bits;
}record_sca_aes_key_info_t;

typedef struct _key_data {
	void *data;
	uint32_t data_len;
	void *priv;
	uint32_t priv_len;
} key_data_t;

typedef struct
{
    uint8_t *input_data;
    uint32_t input_len;
    uint8_t *output_data;
    uint32_t output_len;
	uint8_t *iv;
	uint32_t iv_len;
	uint32_t *iv_offset;
	uint32_t *nc_off;
    uint8_t *nonce_counter;
	uint8_t *stream_block;
	uint32_t key_idx;           // key index
	sca_aes_mode_t  mode;       //aes key mode
	uint32_t out_length;    //represents the data length after aes_gcm operation.
}sca_aes_context_t;

typedef struct {
	uint8_t key_id;
	unsigned char *iv;
	unsigned int iv_len;
	unsigned char *aad;
	unsigned int aad_len;
	unsigned char *tag;
	unsigned int tag_len;
	unsigned char *dec_data;
	unsigned int dec_data_len;
	unsigned char *enc_data;
	unsigned int enc_data_len;
	unsigned int alg;
} gcm_type_t;

/**
 * @brief aes gcm decryption
 *
 *
 * @attention
 * - This API is used to do aes_gcm decryption with the input key.
 *
 * @param
 * -ex_ctx[in] :  aes_gcm context include otp2's item_id�� and output_length .
 * -input[in]  : the ciphertext required for the aes_gcm decryption process
 * -in_len[in] : length of the ciphertext required for aes_gcm decryption
 * -output[out]: used to store decrypted data after decryption.
 * @return
 *  - 0: succeed
 *  - others: other errors.
 */

psa_status_t psa_sca_aes_generate_key(aes_key_type key_type, uint32_t key_index);

psa_status_t psa_sca_aes_set_key(aes_key_type key_type, uint32_t key_idx, key_data_t *key);

psa_status_t psa_sca_aes_remove_key(aes_key_type key_type, uint32_t key_idx);

psa_status_t psa_sca_aes_ecb_crypt(sca_aes_context_t *ex_ctx, uint8_t*input, size_t in_len, uint8_t*output);

psa_status_t psa_sca_aes_cbc_crypt(sca_aes_context_t *sca_aes_ctx, uint8_t*input, size_t in_len, uint8_t*output);

psa_status_t psa_sca_aes_ctr_crypt(sca_aes_context_t *sca_aes_ctx, uint8_t*input, size_t in_len, uint8_t*output);

psa_status_t psa_sca_aes_cfb_crypt(sca_aes_context_t *sca_aes_ctx, uint8_t*input, size_t in_len, uint8_t*output);

psa_status_t armino_hal_psa_gcm_encrypt(gcm_type_t *gcm_param);

psa_status_t armino_hal_psa_gcm_decrypt(gcm_type_t *gcm_param);

#ifdef __cplusplus
}
#endif
