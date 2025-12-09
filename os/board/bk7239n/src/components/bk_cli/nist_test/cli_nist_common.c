// Copyright 2020-2025 Beken
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

#include <tinyara/config.h>
#include <tinyara/security_hal.h>

#include <stdlib.h>
#include "cli_nist_common.h"
#include "cli_nist.h"


void hal_nist_free_buffer(hal_data *data)
{
    if (data == NULL) {
        return;
    }

    if (data->data) {
        free(data->data);
        data->data = NULL;
    }
    if (data->priv) {
        free(data->priv);
        data->priv = NULL;
    }
    data->data_len = 0;
    data->priv_len = 0;
}


int hal_nist_malloc_buffer(hal_data *data, int buf_len)
{
    data->data = (unsigned char *)os_malloc(buf_len);
    if (!data->data) {
        return -1;
    }
    return 0;
}

void hal_nist_init_buffer(hal_data *data)
{
    if (data) {
        data->data = NULL;
        data->data_len = 0;
        data->priv = NULL;
        data->priv_len = 0;
    }
}

void hal_nist_xor_bit_arrays(const uint8_t *array1, const uint8_t *array2, uint8_t *result, uint32_t len)
{
    if (array1 == NULL || array2 == NULL || result == NULL) {
        return;
    }

    for (int i = 0; i < len; i++) {
        result[i] = array1[i] ^ array2[i];
    }
}

#define HEX_PRINT_BYTE_MAX 128
static char s_hex_buff[HEX_PRINT_BYTE_MAX * 2 + 1];
void print_hex(uint8_t *data, uint32_t len)
{
    if (len > HEX_PRINT_BYTE_MAX) {
        len = HEX_PRINT_BYTE_MAX;
    }
    for (size_t i = 0; i < len; i++) {
        sprintf(s_hex_buff + (2 * i), "%02x", data[i]);
    }
    s_hex_buff[len * 2] = 0;
    BK_LOGI(NULL, "%s\n", s_hex_buff);
}

typedef struct nist_algo_verify_callback {
    uint8_t id;
    algo_verify_callback_t callback;
} nist_algo_verify_t;

// HASH callbacks
uint8_t nist_hash_verify_callback(nist_handle_t *nist_handle);
uint8_t nist_hash_monte_verify_callback(nist_handle_t *nist_handle);

// AES callbacks
uint8_t nist_aes_ecb_verify_callback(nist_handle_t *nist_handle);
uint8_t nist_aes_cbc_verify_callback(nist_handle_t *nist_handle);
uint8_t nist_aes_cfb_verify_callback(nist_handle_t *nist_handle);
uint8_t nist_aes_mct_ecb_verify_callback(nist_handle_t *nist_handle);
uint8_t nist_aes_mct_cbc_verify_callback(nist_handle_t *nist_handle);
uint8_t nist_aes_mct_cfb_verify_callback(nist_handle_t *nist_handle);

// HMAC callbacks
uint8_t nist_hmac_verify_callback(nist_handle_t *nist_handle);

// ECDSA callbacks
uint8_t nist_ecdsa_key_pair_verify_callback(nist_handle_t *nist_handle);
uint8_t nist_ecdsa_pub_key_verify_callback(nist_handle_t *nist_handle);
uint8_t nist_ecdsa_sign_verify_callback(nist_handle_t *nist_handle);
uint8_t nist_ecdsa_verify_verify_callback(nist_handle_t *nist_handle);

// RSA callbacks
uint8_t nist_rsa_key_pair_verify_callback(nist_handle_t *nist_handle);
uint8_t nist_rsa_pub_key_verify_callback(nist_handle_t *nist_handle);
uint8_t nist_rsa_sign_verify_callback(nist_handle_t *nist_handle);
uint8_t nist_rsa_verify_verify_callback(nist_handle_t *nist_handle);

// ECDH callbacks
uint8_t nist_ecdh_verify_callback(nist_handle_t *nist_handle);

// RNG callbacks
uint8_t nist_rng_verify_callback(nist_handle_t *nist_handle);

// GCM callbacks
uint8_t nist_gcm_dec_verify_callback(nist_handle_t *nist_handle);
uint8_t nist_gcm_enc_verify_callback(nist_handle_t *nist_handle);


const nist_algo_verify_t s_nist_algo_verify[] = {
    {NIST_HASH,           nist_hash_verify_callback},
    {NIST_HASH_MONTE,     nist_hash_monte_verify_callback},
    {NIST_AES_ECB,        nist_aes_ecb_verify_callback},
    {NIST_AES_CBC,        nist_aes_cbc_verify_callback},
    {NIST_AES_CFB,        nist_aes_cfb_verify_callback},
    {NIST_AES_MCT_ECB,    nist_aes_mct_ecb_verify_callback},
    {NIST_AES_MCT_CBC,    nist_aes_mct_cbc_verify_callback},
    {NIST_AES_MCT_CFB,    nist_aes_mct_cfb_verify_callback},
    {NIST_HMAC,           nist_hmac_verify_callback},
    {NIST_ECDSA_KEY_PAIR, nist_ecdsa_key_pair_verify_callback},
    {NIST_ECDSA_PUB_KEY,  nist_ecdsa_pub_key_verify_callback},
    {NIST_ECDSA_SIGN,     nist_ecdsa_sign_verify_callback},
    {NIST_ECDSA_VERIFY,   nist_ecdsa_verify_verify_callback},
    {NIST_RSA_KEY_PAIR,   nist_rsa_key_pair_verify_callback},
    {NIST_RSA_PUB_KEY,    nist_rsa_pub_key_verify_callback},
    {NIST_RSA_SIGN_V15,   nist_rsa_sign_verify_callback},
    {NIST_RSA_SIGN_PSS,   nist_rsa_sign_verify_callback},
    {NIST_RSA_VERIFY_V15, nist_rsa_verify_verify_callback},
    {NIST_RSA_VERIFY_PSS, nist_rsa_verify_verify_callback},
    // {NIST_RSA_ENC,     nist_rsa_enc_verify_callback},
    // {NIST_RSA_DEC,     nist_rsa_dec_verify_callback},
    //{NIST_DH,             nist_dh_verify_callback},
    {NIST_ECDH,           nist_ecdh_verify_callback},
    {NIST_RNG,            nist_rng_verify_callback},
    {NIST_GCM_DEC,            nist_gcm_dec_verify_callback},
    {NIST_GCM_ENC,         nist_gcm_enc_verify_callback},
};

uint32_t get_algo_verify_callback_size(void)
{
    return sizeof(s_nist_algo_verify) / sizeof(nist_algo_verify_t);
}

#define NIST_ALGO_VERIFY_CALLBACK_SIZE (sizeof(s_nist_algo_verify) / sizeof(nist_algo_verify_t))
algo_verify_callback_t get_algo_verify_callback(uint8_t case_type)
{
    for (int i = 0; i < NIST_ALGO_VERIFY_CALLBACK_SIZE; i++) {
        if (s_nist_algo_verify[i].id == case_type) {
            return s_nist_algo_verify[i].callback;
        }
    }
    return NULL;
}
