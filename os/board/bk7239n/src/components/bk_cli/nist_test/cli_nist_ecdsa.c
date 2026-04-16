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
#include "psa/crypto.h"

extern int armino_hal_generate_key(hal_key_type mode, uint32_t key_idx);
extern int armino_hal_get_key(hal_key_type mode, uint32_t key_idx, hal_data *key);
extern int armino_hal_set_key(hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey);
extern int armino_hal_remove_key(hal_key_type mode, uint32_t key_idx);
extern int armino_hal_ecdsa_sign_md(hal_ecdsa_mode mode, hal_data *hash, uint32_t key_idx, hal_data *sign);
extern int armino_hal_ecdsa_verify_md(hal_ecdsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx);
extern int armino_hal_der_ecdsa_private_key(hal_data *key, hal_data *prikey);
extern int armino_hal_der_ecdsa_public_key(hal_data *key, hal_data *pubkey);
extern int armino_hal_be_der_ecdsa_public_key(hal_key_type mode, uint8_t *key, hal_data *pubkey, uint32_t key_len);

#define ECDSA_HASH_OUTPUT_MAX_SIZE 64

static psa_algorithm_t hal_hash_type_to_psa_alg(hal_hash_type hash_t)
{
    switch (hash_t) {
    case HAL_HASH_MD5:    return PSA_ALG_MD5;
    case HAL_HASH_SHA1:   return PSA_ALG_SHA_1;
    case HAL_HASH_SHA224: return PSA_ALG_SHA_224;
    case HAL_HASH_SHA256: return PSA_ALG_SHA_256;
    case HAL_HASH_SHA384: return PSA_ALG_SHA_384;
    case HAL_HASH_SHA512: return PSA_ALG_SHA_512;
    default:              return PSA_ALG_NONE;
    }
}

static int compute_msg_hash(hal_hash_type hash_t, const uint8_t *msg, size_t msg_len,
                            uint8_t *hash_buf, size_t hash_buf_size, size_t *hash_len)
{
    psa_algorithm_t alg = hal_hash_type_to_psa_alg(hash_t);
    if (alg == PSA_ALG_NONE) {
        BK_LOGE(NULL, "unsupported hash type %d\n", hash_t);
        return NIST_FAIL;
    }
    psa_status_t status = psa_hash_compute(alg, msg, msg_len, hash_buf, hash_buf_size, hash_len);
    if (status != PSA_SUCCESS) {
        BK_LOGE(NULL, "psa_hash_compute failed: %d\n", status);
        return NIST_FAIL;
    }
    return NIST_OK;
}

static size_t der_read_length(const uint8_t *data, size_t data_len, size_t *value)
{
    if (data_len == 0) return 0;
    if ((data[0] & 0x80) == 0) {
        *value = data[0];
        return 1;
    }
    size_t num_bytes = data[0] & 0x7F;
    if (num_bytes == 0 || num_bytes > 2 || data_len < 1 + num_bytes) return 0;
    size_t v = 0;
    for (size_t i = 0; i < num_bytes; i++) {
        v = (v << 8) | data[1 + i];
    }
    *value = v;
    return 1 + num_bytes;
}

static size_t der_write_length(uint8_t *buf, size_t value)
{
    if (value < 0x80) {
        buf[0] = (uint8_t)value;
        return 1;
    } else if (value <= 0xFF) {
        buf[0] = 0x81;
        buf[1] = (uint8_t)value;
        return 2;
    } else {
        buf[0] = 0x82;
        buf[1] = (uint8_t)(value >> 8);
        buf[2] = (uint8_t)(value & 0xFF);
        return 3;
    }
}

static int ecdsa_der_to_raw(const uint8_t *der, size_t der_len,
                            uint8_t *raw, size_t component_size, size_t *raw_len)
{
    if (der_len < 6 || der[0] != 0x30) {
        return NIST_FAIL;
    }
    size_t seq_len = 0;
    size_t pos = 1;
    size_t len_bytes = der_read_length(der + pos, der_len - pos, &seq_len);
    if (len_bytes == 0) return NIST_FAIL;
    pos += len_bytes;

    for (int comp = 0; comp < 2; comp++) {
        if (pos >= der_len || der[pos] != 0x02) {
            return NIST_FAIL;
        }
        pos++;
        size_t clen = 0;
        len_bytes = der_read_length(der + pos, der_len - pos, &clen);
        if (len_bytes == 0) return NIST_FAIL;
        pos += len_bytes;

        const uint8_t *cdata = &der[pos];
        pos += clen;
        if (pos > der_len) return NIST_FAIL;

        if (clen > 0 && cdata[0] == 0x00) {
            cdata++;
            clen--;
        }
        if (clen > component_size) {
            return NIST_FAIL;
        }
        size_t pad = component_size - clen;
        memset(raw + comp * component_size, 0, pad);
        memcpy(raw + comp * component_size + pad, cdata, clen);
    }
    *raw_len = component_size * 2;
    return NIST_OK;
}

static int ecdsa_raw_to_der(const uint8_t *raw, size_t component_size,
                            uint8_t *der, size_t der_buf_size, size_t *der_len)
{
    const uint8_t *r_in = raw;
    const uint8_t *s_in = raw + component_size;
    size_t r_size = component_size;
    size_t s_size = component_size;

    while (r_size > 1 && r_in[0] == 0) { r_in++; r_size--; }
    while (s_size > 1 && s_in[0] == 0) { s_in++; s_size--; }

    size_t r_pad = (r_in[0] & 0x80) ? 1 : 0;
    size_t s_pad = (s_in[0] & 0x80) ? 1 : 0;

    size_t r_total = r_size + r_pad;
    size_t s_total = s_size + s_pad;
    size_t seq_len = 2 + r_total + 2 + s_total;

    uint8_t seq_len_buf[3];
    size_t seq_len_bytes = der_write_length(seq_len_buf, seq_len);
    size_t total = 1 + seq_len_bytes + seq_len;

    if (total > der_buf_size) {
        return NIST_FAIL;
    }

    size_t pos = 0;
    der[pos++] = 0x30;
    memcpy(&der[pos], seq_len_buf, seq_len_bytes);
    pos += seq_len_bytes;
    der[pos++] = 0x02;
    der[pos++] = (uint8_t)r_total;
    if (r_pad) der[pos++] = 0x00;
    memcpy(&der[pos], r_in, r_size);
    pos += r_size;
    der[pos++] = 0x02;
    der[pos++] = (uint8_t)s_total;
    if (s_pad) der[pos++] = 0x00;
    memcpy(&der[pos], s_in, s_size);
    pos += s_size;

    *der_len = pos;
    return NIST_OK;
}

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
    // hash the message
    uint8_t hash_buf[ECDSA_HASH_OUTPUT_MAX_SIZE];
    size_t hash_len = 0;
    if (compute_msg_hash(hash, msg, msg_len, hash_buf, sizeof(hash_buf), &hash_len) != NIST_OK) {
        armino_hal_remove_key(key_mode, 32);
        return NIST_FAIL;
    }
    hal_data msg_digest = {hash_buf, hash_len, NULL, 0};

    // sign → DER output
    hal_data sign = {0};
    uint8_t der_sign_buf[144];
    sign.data = der_sign_buf;
    sign.data_len = sizeof(der_sign_buf);
    result = armino_hal_ecdsa_sign_md(mode, &msg_digest, 32, &sign);
    if (result != HAL_SUCCESS) {
        BK_LOGI(NULL, "sign failed\n");
        armino_hal_remove_key(key_mode, 32);
        return NIST_FAIL;
    }

    // convert DER → raw R || S
    uint8_t raw_sign_buf[132];
    size_t raw_sign_len = 0;
    result = ecdsa_der_to_raw(sign.data, sign.data_len,
                              raw_sign_buf, key_len[curve_id], &raw_sign_len);
    if (result != NIST_OK) {
        BK_LOGE(NULL, "DER->RAW failed\n");
        armino_hal_remove_key(key_mode, 32);
        return NIST_FAIL;
    }

    result = armino_hal_remove_key(key_mode, 32);
    if (result != HAL_SUCCESS) {
        BK_LOGI(NULL, "remove key failed\n");
        return NIST_FAIL;
    }
    uint16_t total_len = msg_len + key.data_len + key.priv_len + raw_sign_len;

    uint8_t ret = nist_set_response_total_len(total_len);
    RETURN_IF_FAIL(ret);
    ret = nist_add_response_data(msg, msg_len);
    RETURN_IF_FAIL(ret);
    ret = nist_add_response_data(key.data, key.data_len);
    RETURN_IF_FAIL(ret);
    ret = nist_add_response_data(key.priv, key.priv_len);
    RETURN_IF_FAIL(ret);
    ret = nist_add_response_data(raw_sign_buf, raw_sign_len);
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

    // hash the message
    uint8_t hash_buf[ECDSA_HASH_OUTPUT_MAX_SIZE];
    size_t hash_len = 0;
    if (compute_msg_hash(hash, msg, msg_len, hash_buf, sizeof(hash_buf), &hash_len) != NIST_OK) {
        kmm_free(pubkey.data);
        return NIST_FAIL;
    }
    hal_data msg_digest = {hash_buf, hash_len, NULL, 0};

    // convert raw R || S → DER for verify_md
    uint8_t raw_cat[132];
    memcpy(raw_cat, sign_r, sign_r_len);
    memcpy(raw_cat + sign_r_len, sign_s, sign_s_len);
    uint8_t der_sign_buf[144];
    size_t der_sign_len = 0;
    int conv_ret = ecdsa_raw_to_der(raw_cat, sign_r_len, der_sign_buf, sizeof(der_sign_buf), &der_sign_len);
    if (conv_ret != NIST_OK) {
        BK_LOGE(NULL, "RAW->DER failed\n");
        kmm_free(pubkey.data);
        return NIST_FAIL;
    }

    result = armino_hal_set_key(key_mode, 32, &pubkey, NULL);
    if (result != HAL_SUCCESS) {
        BK_LOGI(NULL, "set key failed\n");
        kmm_free(pubkey.data);
        return NIST_FAIL;
    }
    hal_data sign_data = {der_sign_buf, der_sign_len, NULL, 0};
    result = armino_hal_ecdsa_verify_md(mode, &msg_digest, &sign_data, 32);
    armino_hal_remove_key(key_mode, 32);
    if ((!expect_result) != (!result)) {
        BK_LOGI(NULL, "check failed, expect_result = %u, result = %u\n", expect_result, result);
        kmm_free(pubkey.data);
        return NIST_FAIL;
    }
    kmm_free(pubkey.data);
    return NIST_OK;
}

