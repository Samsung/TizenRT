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
extern int armino_hal_gcm_decrypt(hal_data *enc_data, hal_gcm_param *gcm_param, uint32_t key_idx, hal_data *dec_data);
extern int armino_hal_gcm_encrypt(hal_data *dec_data, hal_gcm_param *gcm_param, uint32_t key_idx, hal_data *enc_data);

uint8_t nist_gcm_dec_verify_callback(nist_handle_t *nist_handle)
{
    uint32_t key_idx = 32;

    uint16_t klen_len = 0;
    uint8_t *klen_data = get_param_by_index(0, &klen_len);
    uint32_t klen = (klen_data) ? *(uint32_t *)klen_data : 0;

    uint16_t ivlen_len = 0;
    uint8_t *ivlen_data = get_param_by_index(1, &ivlen_len);
    uint32_t ivlen = (ivlen_data) ? *(uint32_t *)ivlen_data : 0;

    uint16_t ptlen_len = 0;
    uint8_t *ptlen_data = get_param_by_index(2, &ptlen_len);
    uint32_t ptlen = (ptlen_data) ? *(uint32_t *)ptlen_data : 0;

    uint16_t aadlen_len = 0;
    uint8_t *aadlen_data = get_param_by_index(3, &aadlen_len);
    uint32_t aadlen = (aadlen_data) ? *(uint32_t *)aadlen_data : 0;

    uint16_t tag_len = 0;
    uint8_t *tag_data = get_param_by_index(4, &tag_len);
    uint32_t taglen = (tag_data) ? *(uint32_t *)tag_data : 0;

    uint16_t count_len = 0;
    uint8_t *count_data = get_param_by_index(5, &count_len);
    uint32_t count = (count_data) ? *(uint32_t *)count_data : 0;

    uint16_t key_len = 0;
    uint8_t *key = get_param_by_index(6, &key_len);

    uint16_t iv_len = 0;
    uint8_t *iv = get_param_by_index(7, &iv_len);

    uint16_t ct_len = 0;
    uint8_t *ct = get_param_by_index(8, &ct_len);

    uint16_t aad_len = 0;
    uint8_t *aad = get_param_by_index(9, &aad_len);

    uint8_t *tag = get_param_by_index(10, &tag_len);

    uint16_t pt_len = 0;
    uint8_t *pt = get_param_by_index(11, &pt_len);

    hal_gcm_param gcm_param = {0};
    hal_data data, key_data, output_data;
    uint8_t *result_data = NULL;
    int ret, status = NIST_OK;
    hal_key_type mode;

    gcm_param.cipher = HAL_GCM_AES;
    gcm_param.iv_len = ivlen / 8;
    gcm_param.iv = iv;
    gcm_param.aad_len = aadlen / 8;
    gcm_param.aad = aad;
    gcm_param.tag_len = taglen / 8;
    gcm_param.tag = os_malloc(gcm_param.tag_len);
    if (gcm_param.tag == NULL) {
        BK_LOGI(NULL, "malloc failed\n");
        status = NIST_FAIL;
        goto exit;
    }
    memcpy(gcm_param.tag, tag, gcm_param.tag_len);

    hal_nist_init_buffer(&data);
    hal_nist_init_buffer(&key_data);
    hal_nist_init_buffer(&output_data);

    key_data.data_len = klen / 8;
    key_data.data = key;

    switch (klen) {
        case 128:
            mode = HAL_KEY_AES_128;
            break;
        case 192:
            mode = HAL_KEY_AES_192;
            break;
        case 256:
            mode = HAL_KEY_AES_256;
            break;
        default:
            BK_LOGI(NULL, "INVALID KEY LENGTH!\n");
            status = NIST_FAIL;
            goto exit;
    }

    ret = armino_hal_set_key(mode, key_idx, &key_data, NULL);
    if (ret != 0) {
        BK_LOGI(NULL, "ARMINO SET KEY FAIL!\n");
        status = NIST_FAIL;
        goto exit;
    }

    data.data_len = ct_len;
    data.data = ct;
    result_data = tag;

    output_data.data_len = pt_len;
    ret = hal_nist_malloc_buffer(&output_data, output_data.data_len);
    if (ret != 0 && output_data.data_len != 0) {
        BK_LOGI(NULL, "OUTPUT DATA ALLOC FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }
    memcpy(output_data.data, pt, output_data.data_len);

    ret = armino_hal_gcm_decrypt(&data, &gcm_param, key_idx, &output_data);
    if (ret != 0) {
        BK_LOGI(NULL, "ARMINO GCM DECRYPT FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    print_hex(gcm_param.tag, gcm_param.tag_len);
    print_hex(result_data, gcm_param.tag_len);

    if (os_memcmp(gcm_param.tag, result_data, gcm_param.tag_len) != 0) {
        if (nist_handle->param_num == 12) {
            BK_LOGI(NULL, "THE AES RESULT IS FAULT!\n");
            status = NIST_FAIL;
            goto key_exit;
        }
    } else {
        if(nist_handle->param_num == 11) {
            BK_LOGI(NULL, "THE AES RESULT IS CORRECT BUT TAG IS FAULT!\n");
            status = NIST_FAIL;
            goto key_exit;
        }
    }

key_exit:
    ret = armino_hal_remove_key(mode, key_idx);
    if (ret != 0) {
        BK_LOGI(NULL, "ARMINO REMOVE KEY FAIL!\n");
        status = NIST_FAIL;
    }
exit:
    os_free(gcm_param.tag);
    hal_nist_free_buffer(&output_data);

    return status;
}

uint8_t nist_gcm_enc_verify_callback(nist_handle_t *nist_handle)
{
    uint32_t key_idx = 32;

    uint16_t klen_len = 0;
    uint8_t *klen_data = get_param_by_index(0, &klen_len);
    uint32_t klen = (klen_data) ? *(uint32_t *)klen_data : 0;

    uint16_t ivlen_len = 0;
    uint8_t *ivlen_data = get_param_by_index(1, &ivlen_len);
    uint32_t ivlen = (ivlen_data) ? *(uint32_t *)ivlen_data : 0;

    uint16_t ptlen_len = 0;
    uint8_t *ptlen_data = get_param_by_index(2, &ptlen_len);
    uint32_t ptlen = (ptlen_data) ? *(uint32_t *)ptlen_data : 0;

    uint16_t aadlen_len = 0;
    uint8_t *aadlen_data = get_param_by_index(3, &aadlen_len);
    uint32_t aadlen = (aadlen_data) ? *(uint32_t *)aadlen_data : 0;

    uint16_t tag_len = 0;
    uint8_t *tag_data = get_param_by_index(4, &tag_len);
    uint32_t taglen = (tag_data) ? *(uint32_t *)tag_data : 0;

    uint16_t count_len = 0;
    uint8_t *count_data = get_param_by_index(5, &count_len);
    uint32_t count = (count_data) ? *(uint32_t *)count_data : 0;

    uint16_t key_len = 0;
    uint8_t *key = get_param_by_index(6, &key_len);

    uint16_t iv_len = 0;
    uint8_t *iv = get_param_by_index(7, &iv_len);

    uint16_t pt_len = 0;
    uint8_t *pt = get_param_by_index(8, &pt_len);

    uint16_t aad_len = 0;
    uint8_t *aad = get_param_by_index(9, &aad_len);

    uint16_t ct_len = 0;
    uint8_t *ct = get_param_by_index(10, &ct_len);

    uint8_t *tag = get_param_by_index(11, &tag_len);


    hal_gcm_param gcm_param = {0};
    hal_data data, key_data, output_data;
    uint8_t *result_data = NULL;
    int ret, status = NIST_OK;
    hal_key_type mode;

    gcm_param.cipher = HAL_GCM_AES;
    gcm_param.iv_len = ivlen / 8;
    gcm_param.iv = iv;
    gcm_param.aad_len = aadlen / 8;
    gcm_param.aad = aad;
    gcm_param.tag_len = taglen / 8;
    gcm_param.tag = os_malloc(gcm_param.tag_len);
    if (gcm_param.tag == NULL) {
        BK_LOGI(NULL, "malloc failed\n");
        status = NIST_FAIL;
        goto exit;
    }
    memcpy(gcm_param.tag, tag, gcm_param.tag_len);

    hal_nist_init_buffer(&data);
    hal_nist_init_buffer(&key_data);
    hal_nist_init_buffer(&output_data);

    key_data.data_len = klen / 8;
    key_data.data = key;

    switch (klen) {
        case 128:
            mode = HAL_KEY_AES_128;
            break;
        case 192:
            mode = HAL_KEY_AES_192;
            break;
        case 256:
            mode = HAL_KEY_AES_256;
            break;
        default:
            BK_LOGI(NULL, "INVALID KEY LENGTH!\n");
            status = NIST_FAIL;
            goto exit;
    }

    ret = armino_hal_set_key(mode, key_idx, &key_data, NULL);
    if (ret != 0) {
        BK_LOGI(NULL, "ARMINO SET KEY FAIL!\n");
        status = NIST_FAIL;
        goto exit;
    }

    data.data_len = pt_len;
    data.data = pt;
    result_data = tag;

    output_data.data_len = ct_len;
    ret = hal_nist_malloc_buffer(&output_data, output_data.data_len);
    if (ret != 0 && output_data.data_len != 0) {
        BK_LOGI(NULL, "OUTPUT DATA ALLOC FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }
    memcpy(output_data.data, ct, output_data.data_len);

    ret = armino_hal_gcm_encrypt(&data, &gcm_param, key_idx, &output_data);
    if (ret != 0) {
        BK_LOGI(NULL, "ARMINO GCM ENCRYPT FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    print_hex(gcm_param.tag, taglen / 8);
    print_hex(result_data, taglen / 8);

    if (os_memcmp(gcm_param.tag, result_data, taglen / 8) != 0) {
            BK_LOGI(NULL, "THE AES RESULT IS FAULT!\n");
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
    os_free(gcm_param.tag);
    hal_nist_free_buffer(&output_data);

    return status;
}

