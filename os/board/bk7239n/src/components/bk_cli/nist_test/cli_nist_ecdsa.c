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

extern int armino_hal_generate_key(hal_key_type mode, uint32_t key_idx);
extern int armino_hal_get_key(hal_key_type mode, uint32_t key_idx, hal_data *key);
extern int armino_hal_set_key(hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey);
extern int armino_hal_remove_key(hal_key_type mode, uint32_t key_idx);
extern int armino_hal_ecdsa_sign_md(hal_ecdsa_mode mode, hal_data *hash, uint32_t key_idx, hal_data *sign);
extern int armino_hal_ecdsa_verify_md(hal_ecdsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx);
extern int armino_hal_der_ecdsa_private_key(hal_data *key, hal_data *prikey);
extern int armino_hal_der_ecdsa_public_key(hal_data *key, hal_data *pubkey);
extern int armino_hal_be_der_ecdsa_public_key(hal_key_type mode, uint8_t *key, hal_data *pubkey, uint32_t key_len);

#define HAL_MAX_ECP_KEY_SIZE  133
#define HAL_MAX_ECP_DER_KEY   288
uint8_t nist_ecdsa_key_pair_verify_callback(nist_handle_t *nist_handle)
{
    uint8_t key_data[132];
    BK_LOGI(NULL, "ecdsa_key_pair start\n");
    hal_key_type mode;
    uint8_t sub_type = nist_handle->sub_type;
    BK_LOGI(NULL, "sub_type = %u\n", sub_type);
    uint16_t data_len = 0;
    uint8_t *data = get_param_by_index(0, &data_len);
    uint32_t key_num = *(uint32_t *)data;

    uint8_t key_len[] = {24, 28, 32, 48, 66};
    mode = HAL_KEY_ECC_SEC_P192R1 + sub_type;

    hal_data key = {0};
    key.data = key_data;
    key.priv = key_data + key_len[sub_type];
    int result;
    uint8_t ret = nist_set_response_total_len(key_len[sub_type] * 2 * key_num);
    RETURN_IF_FAIL(ret);
    for (int i = 0; i < key_num; i++) {
        result = armino_hal_generate_key(mode, 32);
        if (result != HAL_SUCCESS) {
            BK_LOGI(NULL, "generate key failed\n");
            return NIST_FAIL;
        }
        result = armino_hal_get_key(mode, 32, &key);
        if (result != HAL_SUCCESS) {
            BK_LOGI(NULL, "get key failed\n");
            return NIST_FAIL;
        }
        // BK_LOGI(NULL, "key.data_len = %u\n", key.data_len);
        // BK_LOGI(NULL, "key.priv_len = %u\n", key.priv_len);
        ret = nist_add_response_data(key.data, key.data_len * 2);
        RETURN_IF_FAIL(ret);
        result = armino_hal_remove_key(mode, 32);
        if (result != HAL_SUCCESS) {
            BK_LOGI(NULL, "remove key failed\n");
            return NIST_FAIL;
        }
    }
    return NIST_OK;
}

uint8_t nist_ecdsa_pub_key_verify_callback(nist_handle_t *nist_handle)
{
    uint8_t sub_type = nist_handle->sub_type;
    BK_LOGI(NULL,"sub_type = %u\n", sub_type);
    hal_key_type mode;
    mode = HAL_KEY_ECC_SEC_P192R1 + sub_type;
    uint16_t data_len = 0;
    uint16_t qx_len = 0;
    uint16_t qy_len = 0;
    uint8_t *qx = get_param_by_index(0, &qx_len);
    uint8_t *qy = get_param_by_index(1, &qy_len);

    uint8_t *data = get_param_by_index(2, &data_len);
    uint32_t check_result = *(uint32_t *)data;

    hal_data key = {0};
    key.data = qx;
    key.data_len = qx_len;
    key.priv = qy;
    key.priv_len = qy_len;


    hal_data pubkey = {0};
    pubkey.data = (unsigned char *)kmm_malloc(HAL_MAX_ECP_DER_KEY);
    if(pubkey.data == NULL){
        return HAL_NOT_ENOUGH_MEMORY;
    }

    uint32_t pub_key_len = key.data_len + key.priv_len;
    /* Allocate buffer for 0x04 || qx || qy, so need pub_key_len + 1 bytes */
    uint8_t *pub_key_buffer = (uint8_t *)kmm_malloc(pub_key_len + 1);
    if(pub_key_buffer == NULL){
        kmm_free(pubkey.data);
        return HAL_NOT_ENOUGH_MEMORY;
    }
    pub_key_buffer[0] = 0x04;  /* Uncompressed point format */
    memcpy(pub_key_buffer + 1, qx, qx_len);
    memcpy(pub_key_buffer + 1 + qx_len, qy, qy_len);

    int result = armino_hal_be_der_ecdsa_public_key(mode, pub_key_buffer, &pubkey, pub_key_len);
    kmm_free(pub_key_buffer);
    if (result != NIST_OK) {
        kmm_free(pubkey.data);
        return NIST_FAIL;
    }

    // print_hex(key.data, key.data_len);
    result = armino_hal_set_key(mode, 32, &pubkey, NULL);
    if ((!check_result) != (!result)) {
        BK_LOGI(NULL, "check failed, check_result = %u, result = %u\n", check_result, result);
        kmm_free(pubkey.data);
        return NIST_FAIL;
    }

    if (result == 0) {
        armino_hal_remove_key(mode, 32);
        kmm_free(pubkey.data);
    }

    return NIST_OK;
}

uint8_t nist_ecdsa_sign_verify_callback(nist_handle_t *nist_handle)
{
    uint8_t sub_type = nist_handle->sub_type;
    BK_LOGI(NULL, "sub_type = %u\n", sub_type);
    hal_ecdsa_mode mode;
    uint16_t msg_len = 0;
    uint8_t curve_id = sub_type / 5;
    hal_ecdsa_curve curve = HAL_ECDSA_SEC_P224R1 + curve_id;
    uint8_t hash_id = sub_type % 5;
    hal_hash_type hash = HAL_HASH_SHA1 + hash_id;
    mode.curve = curve;
    mode.hash_t = hash;
    uint8_t *msg = get_param_by_index(0, &msg_len);
    hal_key_type key_mode = HAL_KEY_ECC_SEC_P224R1 + curve_id;
    int result = armino_hal_generate_key(key_mode, 32);
    if (result != HAL_SUCCESS) {
        BK_LOGI(NULL, "generate key failed\n");
        return NIST_FAIL;
    }
    uint8_t key_data[132];
    hal_data key = {0};
    uint16_t key_len[] = {28, 32, 48, 66};
    key.data = key_data;
    key.priv = key_data + key_len[curve_id];
    result = armino_hal_get_key(key_mode, 32, &key);
    if (result != HAL_SUCCESS) {
        BK_LOGI(NULL, "get key failed\n");
        return NIST_FAIL;
    }
    // sign
    hal_data sign = {0};
    uint8_t sign_data[136];
    sign.data = sign_data;
    hal_data msg_hash = {0};
    msg_hash.data = msg;
    msg_hash.data_len = msg_len;
    result = armino_hal_ecdsa_sign_md(mode, &msg_hash, 32, &sign);
    if (result != HAL_SUCCESS) {
        BK_LOGI(NULL, "sign failed\n");
        return NIST_FAIL;
    }
    result = armino_hal_remove_key(key_mode, 32);
    if (result != HAL_SUCCESS) {
        BK_LOGI(NULL, "remove key failed\n");
        return NIST_FAIL;
    }
    uint16_t total_len = msg_len + key.data_len + key.priv_len + sign.data_len;

    uint8_t ret = nist_set_response_total_len(total_len);
    RETURN_IF_FAIL(ret);
    ret = nist_add_response_data(msg, msg_len);
    RETURN_IF_FAIL(ret);
    ret = nist_add_response_data(key.data, key.data_len);
    RETURN_IF_FAIL(ret);
    ret = nist_add_response_data(key.priv, key.priv_len);
    RETURN_IF_FAIL(ret);
    ret = nist_add_response_data(sign.data, sign.data_len);
    RETURN_IF_FAIL(ret);
    return NIST_OK;
}

uint8_t nist_ecdsa_verify_verify_callback(nist_handle_t *nist_handle)
{
    uint8_t sub_type = nist_handle->sub_type;
    BK_LOGI(NULL, "sub_type = %u\n", sub_type);
    hal_ecdsa_mode mode;
    uint8_t curve_id = sub_type / 5;
    hal_ecdsa_curve curve = HAL_ECDSA_SEC_P192R1 + curve_id;
    uint8_t hash_id = sub_type % 5;
    hal_hash_type hash = HAL_HASH_SHA1 + hash_id;
    mode.curve = curve;
    mode.hash_t = hash;
    uint16_t msg_len = 0;
    uint8_t *msg = get_param_by_index(0, &msg_len);

    uint16_t qx_len = 0;
    uint16_t qy_len = 0;
    uint8_t *qx = get_param_by_index(1, &qx_len);
    uint8_t *qy = get_param_by_index(2, &qy_len);

    uint16_t sign_r_len = 0;
    uint16_t sign_s_len = 0;
    uint8_t *sign_r = get_param_by_index(3, &sign_r_len);
    uint8_t *sign_s = get_param_by_index(4, &sign_s_len);
    uint16_t sign_len = sign_r_len + sign_s_len;

    uint16_t expect_result_len = 0;
    uint8_t *expect_result_ptr = get_param_by_index(5, &expect_result_len);
    uint32_t expect_result = *(uint32_t *)expect_result_ptr;

    hal_data key = {0};
    key.data = qx;
    key.data_len = qx_len;
    key.priv = qy;
    key.priv_len = qy_len;
    hal_key_type key_mode = HAL_KEY_ECC_SEC_P192R1 + curve_id;

    hal_data pubkey = {0};
    pubkey.data = (unsigned char *)kmm_malloc(HAL_MAX_ECP_DER_KEY);
    if(pubkey.data == NULL){
        return HAL_NOT_ENOUGH_MEMORY;
    }

    uint32_t pub_key_len = key.data_len + key.priv_len;
    /* Allocate buffer for 0x04 || qx || qy, so need pub_key_len + 1 bytes */
    uint8_t *pub_key_buffer = (uint8_t *)kmm_malloc(pub_key_len + 1);
    if(pub_key_buffer == NULL){
        kmm_free(pubkey.data);
        return HAL_NOT_ENOUGH_MEMORY;
    }
    pub_key_buffer[0] = 0x04;  /* Uncompressed point format */
    memcpy(pub_key_buffer + 1, qx, qx_len);
    memcpy(pub_key_buffer + 1 + qx_len, qy, qy_len);

    int result = armino_hal_be_der_ecdsa_public_key(key_mode, pub_key_buffer, &pubkey, pub_key_len);
    kmm_free(pub_key_buffer);
    if (result != NIST_OK) {
        kmm_free(pubkey.data);
        return NIST_FAIL;
    }

    result = armino_hal_set_key(key_mode, 32, &pubkey, NULL);
    if (result != HAL_SUCCESS) {
        BK_LOGI(NULL, "set key failed\n");
        kmm_free(pubkey.data);
        return NIST_FAIL;
    }
    hal_data sign_data = {0};
    sign_data.data = sign_r;
    sign_data.data_len = sign_len;
    hal_data msg_hash = {0};
    msg_hash.data = msg;
    msg_hash.data_len = msg_len;
    result = armino_hal_ecdsa_verify_md(mode, &msg_hash, &sign_data, 32);
    armino_hal_remove_key(key_mode, 32);
    if ((!expect_result) != (!result)) {
        BK_LOGI(NULL, "check failed, expect_result = %u, result = %u\n", expect_result, result);
        kmm_free(pubkey.data);
        return NIST_FAIL;
    }
    kmm_free(pubkey.data);
    return NIST_OK;
}

