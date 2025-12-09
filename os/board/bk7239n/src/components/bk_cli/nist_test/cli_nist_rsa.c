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

#include "cli_nist_common.h"

extern int armino_hal_set_key(hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey);
extern int armino_hal_remove_key(hal_key_type mode, uint32_t key_idx);
extern int armino_hal_rsa_verify_md(hal_rsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx);
extern int armino_hal_rsa_sign_md(hal_rsa_mode mode, hal_data *hash, uint32_t key_idx, hal_data *sign);

#define SET_RSA_KEY_MODE(sub_type_var, key_mode_var) do { \
    switch (sub_type_var) { \
    case 0: \
        key_mode_var = HAL_KEY_RSA_1024; \
        break; \
    case 1: \
        BK_LOGI(NULL, "unsupport sub type RSA1536 %u\n\r", sub_type_var); \
        return NIST_OK; \
    case 2: \
        key_mode_var = HAL_KEY_RSA_2048; \
        break; \
    case 3: \
        key_mode_var = HAL_KEY_RSA_3072; \
        break; \
    case 4: \
        key_mode_var = HAL_KEY_RSA_4096; \
        break; \
    default: \
        BK_LOGE(NULL, "unknown sub type %u\n\r", sub_type_var); \
        return NIST_FAIL; \
    } \
} while (0)

static hal_hash_type hal_hash_type_from_string(const char *hash_str, size_t len)
{
    if (hash_str == NULL) {
        return HAL_HASH_UNKNOWN;
    }

    if (strcasecmp(hash_str, "MD5") == 0) {
        return HAL_HASH_MD5;
    } else if (strncmp(hash_str, "SHA1", len) == 0) {
        return HAL_HASH_SHA1;
    } else if (strncmp(hash_str, "SHA224", len) == 0) {
        return HAL_HASH_SHA224;
    } else if (strncmp(hash_str, "SHA256", len) == 0) {
        return HAL_HASH_SHA256;
    } else if (strncmp(hash_str, "SHA384", len) == 0) {
        return HAL_HASH_SHA384;
    } else if (strncmp(hash_str, "SHA512", len) == 0) {
        return HAL_HASH_SHA512;
    } else {
        return HAL_HASH_UNKNOWN;
    }
}

int nist_rsa_key_pair_verify_callback(nist_handle_t *nist_handle)
{
    // uint8_t sub_type = nist_handle->sub_type;

    // hal_data e = {0};
    // hal_data p = {0};
    // hal_data q = {0};
    // hal_data n = {0};
    // hal_data d = {0};

    // e.data = get_param_by_index(0, (uint16_t *)&e.data_len);
    // p.data = get_param_by_index(1, (uint16_t *)&p.data_len);
    // q.data = get_param_by_index(2, (uint16_t *)&q.data_len);
    // n.data = get_param_by_index(3, (uint16_t *)&n.data_len);
    // d.data = get_param_by_index(4, (uint16_t *)&d.data_len);



    // //for debug
    // print_hex(e.data, e.data_len);
    // print_hex(p.data, p.data_len);
    // print_hex(q.data, q.data_len);
    // print_hex(n.data, n.data_len);
    // print_hex(d.data, d.data_len);

    return NIST_OK;

}

int nist_rsa_pub_key_verify_callback(nist_handle_t *nist_handle)
{
    return NIST_OK;
}

#define RSA_MAX_KEY_SIZE 3072
uint8_t nist_rsa_sign_verify_callback(nist_handle_t *nist_handle)
{
    uint8_t sub_type = nist_handle->sub_type;
    char *hash_algo = NULL;
    uint16_t hash_algo_len = 0;
    hal_data Msg = {0};
    hal_data S = {0};
    hal_data n = {0};
    hal_data e = {0};

    hal_rsa_mode mode = {0};
    uint32_t key_idx = 32;
    hal_key_type key_mode = HAL_KEY_RSA_1024;
    int result;
    
    n.data    = get_param_by_index(0, (uint16_t *)&n.data_len);
    e.data    = get_param_by_index(1, (uint16_t *)&e.data_len);
    hash_algo = (char *)get_param_by_index(2, (uint16_t *)&hash_algo_len);
    Msg.data  = get_param_by_index(3, (uint16_t *)&Msg.data_len);
    S.data    = get_param_by_index(4, (uint16_t *)&S.data_len);

    SET_RSA_KEY_MODE(sub_type, key_mode);

    hal_hash_type hash = hal_hash_type_from_string(hash_algo, hash_algo_len);
    if (hash == HAL_HASH_UNKNOWN) {
        hash_algo[hash_algo_len-1] = '\0';
        BK_LOGE(NULL, "unknown hash algo %s size %d\n", hash_algo, hash_algo_len);
        return NIST_FAIL;
    }

    if(nist_handle->case_type == NIST_RSA_SIGN_V15) {
        mode.rsa_a = HAL_RSASSA_PKCS1_V1_5;
    }
    else if(nist_handle->case_type == NIST_RSA_SIGN_PSS) {
        mode.rsa_a = HAL_RSASSA_PKCS1_PSS_MGF1;
    }
    mode.hash_t = hash;
    mode.mgf = hash;
    mode.salt_byte_len = 20;

    result = armino_hal_set_key(key_mode, key_idx, &e, NULL);
    if (result != HAL_SUCCESS) {
        BK_LOGE(NULL, "set key failed\n");
        return NIST_FAIL;
    }

    result = armino_hal_rsa_verify_md(mode, &Msg, &S, key_idx);
    if (result != HAL_SUCCESS) {
        armino_hal_remove_key(key_mode, key_idx);
        BK_LOGE(NULL, "verify failed\n");
        return NIST_FAIL;
    }

    armino_hal_remove_key(key_mode, key_idx);
    return NIST_OK;
}

#define RSA_MAX_SIGNATURE_BYTE_LEN 512
uint8_t nist_rsa_verify_verify_callback(nist_handle_t *nist_handle)
{
    uint8_t sub_type = nist_handle->sub_type;
    char *hash_algo = NULL;
    uint16_t hash_algo_len = 0;
    hal_data n = {0};
    hal_data p = {0};
    hal_data q = {0};
    hal_data e = {0};
    hal_data d = {0};
    hal_data Msg = {0};
    hal_data S = {0};

    hal_rsa_mode mode = {0};
    uint32_t key_idx = 32;
    hal_key_type key_mode = HAL_KEY_RSA_1024;
    hal_data signature = {0};
    int result;

    n.data    = get_param_by_index(0, (uint16_t *)&n.data_len);
    p.data    = get_param_by_index(1, (uint16_t *)&p.data_len);
    q.data    = get_param_by_index(2, (uint16_t *)&q.data_len);
    hash_algo = (char *)get_param_by_index(3, (uint16_t *)&hash_algo_len);
    e.data    = get_param_by_index(4, (uint16_t *)&e.data_len);
    d.data    = get_param_by_index(5, (uint16_t *)&d.data_len);
    Msg.data  = get_param_by_index(6, (uint16_t *)&Msg.data_len);
    S.data    = get_param_by_index(7, (uint16_t *)&S.data_len);

    uint16_t expect_result_len = 0;
    uint8_t *expect_result_ptr = get_param_by_index(8, &expect_result_len);
    uint32_t expect_result = *(uint32_t *)expect_result_ptr;

    SET_RSA_KEY_MODE(sub_type, key_mode);

    hal_hash_type hash = hal_hash_type_from_string(hash_algo, hash_algo_len);
    if (hash == HAL_HASH_UNKNOWN) {
        hash_algo[hash_algo_len-1] = '\0';
        BK_LOGE(NULL, "unknown hash algo %s size %d\n", hash_algo, hash_algo_len);
        return NIST_FAIL;
    }

    if(nist_handle->case_type == NIST_RSA_VERIFY_V15) {
        mode.rsa_a = HAL_RSASSA_PKCS1_V1_5;
    }
    else if(nist_handle->case_type == NIST_RSA_VERIFY_PSS) {
        mode.rsa_a = HAL_RSASSA_PKCS1_PSS_MGF1;
    }
    mode.hash_t = hash;
    mode.mgf = hash;
    mode.salt_byte_len = 20;

    result = armino_hal_set_key(key_mode, key_idx, NULL, &d);
    if (result != HAL_SUCCESS) {
        BK_LOGI(NULL, "set key failed\n");
        return NIST_FAIL;
    }

    signature.data = os_malloc(RSA_MAX_SIGNATURE_BYTE_LEN);
    signature.data_len = RSA_MAX_SIGNATURE_BYTE_LEN;
    result = armino_hal_rsa_sign_md(mode, &Msg, key_idx, &signature);
    if (result != HAL_SUCCESS) {
        BK_LOGE(NULL, "sign failed\n");
        result = NIST_FAIL;
    }else{
        if(signature.data_len == S.data_len) {
            if(memcmp(signature.data, S.data, signature.data_len) == 0) {
                BK_LOGI(NULL, "sign match\n");
                result = NIST_OK;
            } else {
                print_hex(signature.data, signature.data_len);
                BK_LOGE(NULL, "sign not match\n");
                result = NIST_FAIL;
            }
        }
        else{
            BK_LOGE(NULL, "sign len not match, expect %d, got %d\n", S.data_len, signature.data_len);
            result = NIST_FAIL;
        }
    }
    if ((!expect_result) != (!result)) {
        BK_LOGI(NULL, "check failed, expect_result = %u, result = %u\n", expect_result, result);
        hal_nist_free_buffer(&signature);
        armino_hal_remove_key(key_mode, key_idx);
        return NIST_FAIL;
    }
    hal_nist_free_buffer(&signature);
    armino_hal_remove_key(key_mode, key_idx);

    return NIST_OK;
}

