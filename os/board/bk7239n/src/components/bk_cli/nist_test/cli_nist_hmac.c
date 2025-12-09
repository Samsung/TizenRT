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

#include <stdint.h>
#include <string.h>
#include <os/os.h>
#include "cli.h"
#include "cli_nist.h"
#include "cli_nist_common.h"
#include <tinyara/config.h>
#include <tinyara/security_hal.h>

extern int armino_hal_get_hmac(hal_hash_type mode, hal_data *input, uint32_t key_idx, hal_data *hmac);
extern int armino_hal_set_key(hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey);
extern int armino_hal_remove_key(hal_key_type mode, uint32_t key_idx);
extern uint8_t *get_param_by_index(uint32_t index, uint16_t *len);

uint8_t nist_hmac_verify_callback(nist_handle_t *nist_handle)
{
    uint32_t key_idx = 32;
    uint8_t sub_type = nist_handle->sub_type;

    uint16_t count_len = 0;
    uint8_t *count_data = get_param_by_index(0, &count_len);
    uint32_t count = (count_data) ? *(uint32_t *)count_data : 0;

    uint16_t klen_len = 0;
    uint8_t *klen_data = get_param_by_index(1, &klen_len);
    uint32_t klen = (klen_data) ? *(uint32_t *)klen_data : 0;

    uint16_t tlen_len = 0;
    uint8_t *tlen_data = get_param_by_index(2, &tlen_len);
    uint32_t tlen = (tlen_data) ? *(uint32_t *)tlen_data : 0;

    uint16_t key_len = 0;
    uint8_t *key = get_param_by_index(3, &key_len);

    uint16_t msg_len = 0;
    uint8_t *msg = get_param_by_index(4, &msg_len);

    uint16_t mac_len = 0;
    uint8_t *mac = get_param_by_index(5, &mac_len);

    hal_data data, key_data, output_data;
    uint8_t *result_data = NULL;

    hal_key_type mode;
    int ret, status = NIST_OK;

    BK_LOGI(NULL, "sub_type = %u\n", sub_type);
    BK_LOGI(NULL, "count = %u\n", count);

    hal_nist_init_buffer(&data);
    hal_nist_init_buffer(&key_data);
    hal_nist_init_buffer(&output_data);

    key_data.data_len = klen;
    key_data.data = key;

    switch (sub_type) {
        case 1:
            mode = HAL_KEY_HMAC_SHA1;
            break;
        case 2:
            mode = HAL_KEY_HMAC_SHA224;
            break;
        case 3:
            mode = HAL_KEY_HMAC_SHA256;
            break;
        case 4:
            mode = HAL_KEY_HMAC_SHA384;
            break;
        case 5:
            mode = HAL_KEY_HMAC_SHA512;
            break;
        default:
            BK_LOGI(NULL, "INVALID SUB TYPE!\n");
            status = NIST_FAIL;
            goto exit;
    }

    ret = armino_hal_set_key(mode, key_idx, &key_data, NULL);
    if (ret != 0) {
        BK_LOGI(NULL, "ARMINO SET KEY FAIL!\n");
        status = NIST_FAIL;
        goto exit;
    }

    data.data_len = msg_len;
    data.data = msg;
    result_data = mac;

    output_data.data_len = tlen * 2;
    ret = hal_nist_malloc_buffer(&output_data, output_data.data_len);
    if (ret != 0) {
        BK_LOGI(NULL, "OUTPUT DATA ALLOC FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }


    ret = armino_hal_get_hmac(sub_type, &data, key_idx, &output_data);
    if (ret != 0) {
        BK_LOGI(NULL, "ARMINO GET HMAC FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    if (os_memcmp(output_data.data, result_data, tlen) != 0) {
        BK_LOGI(NULL, "THE HMAC RESULT IS FAULT!\n");
        print_hex(output_data.data, tlen);
        print_hex(result_data, tlen);
        status = NIST_FAIL;
        goto key_exit;
    }

    key_exit:
    ret = armino_hal_remove_key(mode, key_idx);
    if (ret != 0) {
        BK_LOGI(NULL, "ARMINO REMOVE KEY FAIL!\n");
        status = NIST_FAIL;
    }

    exit:
    hal_nist_free_buffer(&output_data);

    return status;
}

