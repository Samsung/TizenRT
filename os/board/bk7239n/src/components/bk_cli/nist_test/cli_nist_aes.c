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

extern int armino_hal_set_key(hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey);
extern int armino_hal_remove_key(hal_key_type mode, uint32_t key_idx);
extern int armino_hal_aes_encrypt(hal_data *dec_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *enc_data);
extern int armino_hal_aes_decrypt(hal_data *enc_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *dec_data);

uint8_t nist_aes_ecb_verify_callback(nist_handle_t *nist_handle)
{
    // TODO just print data.
    uint32_t key_idx = 32;
    uint8_t sub_type = nist_handle->sub_type;
    uint16_t count_len = 0;
    uint8_t *count_data = get_param_by_index(0, &count_len);
    uint32_t count = (count_data) ? *(uint32_t *)count_data : 0;
    uint16_t key_len = 0;
    uint8_t *key = get_param_by_index(1, &key_len);
    uint16_t text1_len = 0;
    uint8_t *text1 = get_param_by_index(2, &text1_len);
    uint16_t text2_len = 0;
    uint8_t *text2 = get_param_by_index(3, &text2_len);
    uint8_t *result_data = NULL;
    int ret, status = NIST_OK;
    hal_data data, key_data, output_data;
    hal_aes_param aes_param;
    hal_key_type mode;

    aes_param.mode = HAL_AES_ECB_NOPAD;
    aes_param.iv_len = 0;
    aes_param.iv_offset = NULL;
    aes_param.iv = NULL;
    aes_param.nc_off = NULL;
    aes_param.nonce_counter = NULL;
    aes_param.stream_block = NULL;

    BK_LOGI(NULL, "sub_type = %u\n", sub_type);
    BK_LOGI(NULL, "count = %u\n", count);

    hal_nist_init_buffer(&data);
    hal_nist_init_buffer(&key_data);
    hal_nist_init_buffer(&output_data);

    key_data.data_len = key_len;
    key_data.data = key;

    switch (key_len) {
        case 16:
            mode = HAL_KEY_AES_128;
            break;
        case 24:
            mode = HAL_KEY_AES_192;
            break;
        case 32:
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

    data.data_len = text1_len;
    data.data = text1;
    result_data = text2;

    output_data.data_len = text1_len;
    ret = hal_nist_malloc_buffer(&output_data, output_data.data_len);
    if (ret != 0) {
        BK_LOGI(NULL, "OUTPUT DATA ALLOC FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    if (!sub_type) {
        ret = armino_hal_aes_encrypt(&data, &aes_param, key_idx, &output_data);
    } else {
        ret = armino_hal_aes_decrypt(&data, &aes_param, key_idx, &output_data);
    }

    if (ret != 0) {
        BK_LOGI(NULL, "ARMINO AES ENCRYPT/DECRYPT FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    if (os_memcmp(output_data.data, result_data, output_data.data_len) != 0) {
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
    hal_nist_free_buffer(&output_data);

    return status;
}

uint8_t nist_aes_cbc_verify_callback(nist_handle_t *nist_handle)
{
    // TODO just print data.
    uint32_t key_idx = 32;
    uint8_t sub_type = nist_handle->sub_type;
    uint16_t count_len = 0;
    uint8_t *count_data = get_param_by_index(0, &count_len);
    uint32_t count = (count_data) ? *(uint32_t *)count_data : 0;
    uint16_t key_len = 0;
    uint8_t *key = get_param_by_index(1, &key_len);
    uint16_t iv_len = 0;
    uint8_t *iv = get_param_by_index(2, &iv_len);
    uint16_t text1_len = 0;
    uint8_t *text1 = get_param_by_index(3, &text1_len);
    uint16_t text2_len = 0;
    uint8_t *text2 = get_param_by_index(4, &text2_len);
    uint8_t *result_data = NULL;
    int ret, status = NIST_OK;
    hal_data data, key_data, output_data;
    hal_aes_param aes_param;
    hal_key_type mode;

    aes_param.mode = HAL_AES_CBC_NOPAD;
    aes_param.iv_len = iv_len;
    aes_param.iv_offset = NULL;
    aes_param.iv = iv;
    aes_param.nc_off = NULL;
    aes_param.nonce_counter = NULL;
    aes_param.stream_block = NULL;

    BK_LOGI(NULL, "sub_type = %u\n", sub_type);
    BK_LOGI(NULL, "count = %u\n", count);

    hal_nist_init_buffer(&data);
    hal_nist_init_buffer(&key_data);
    hal_nist_init_buffer(&output_data);

    key_data.data_len = key_len;
    key_data.data = key;

    switch (key_len) {
        case 16:
            mode = HAL_KEY_AES_128;
            break;
        case 24:
            mode = HAL_KEY_AES_192;
            break;
        case 32:
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

    data.data_len = text1_len;
    data.data = text1;
    result_data = text2;

    output_data.data_len = text1_len;
    ret = hal_nist_malloc_buffer(&output_data, output_data.data_len);
    if (ret != 0) {
        BK_LOGI(NULL, "OUTPUT DATA ALLOC FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    if (!sub_type) {
        ret = armino_hal_aes_encrypt(&data, &aes_param, key_idx, &output_data);
    } else {
        ret = armino_hal_aes_decrypt(&data, &aes_param, key_idx, &output_data);
    }

    if (ret != 0) {
        BK_LOGI(NULL, "ARMINO AES ENCRYPT/DECRYPT FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    if (os_memcmp(output_data.data, result_data, output_data.data_len) != 0) {
        BK_LOGI(NULL, "THE AES RESULT IS FAULT!\n");

        print_hex(output_data.data, output_data.data_len);
        print_hex(result_data, output_data.data_len);

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

uint8_t nist_aes_cfb_verify_callback(nist_handle_t *nist_handle)
{
    // TODO just print data.
    uint32_t key_idx = 32, iv_offset = 0;
    uint8_t sub_type = nist_handle->sub_type;
    uint16_t count_len = 0;
    uint8_t *count_data = get_param_by_index(0, &count_len);
    uint32_t count = (count_data) ? *(uint32_t *)count_data : 0;
    uint16_t key_len = 0;
    uint8_t *key = get_param_by_index(1, &key_len);
    uint16_t iv_len = 0;
    uint8_t *iv = get_param_by_index(2, &iv_len);
    uint16_t text1_len = 0;
    uint8_t *text1 = get_param_by_index(3, &text1_len);
    uint16_t text2_len = 0;
    uint8_t *text2 = get_param_by_index(4, &text2_len);
    uint8_t *result_data = NULL;
    int ret, status = NIST_OK;
    hal_data data, key_data, output_data;
    hal_aes_param aes_param;
    hal_key_type mode;

    aes_param.mode = HAL_AES_CFB128;
    aes_param.iv_len = iv_len;
    aes_param.iv_offset = NULL;
    aes_param.iv = iv;
    aes_param.nc_off = NULL;
    aes_param.nonce_counter = NULL;
    aes_param.stream_block = NULL;

    aes_param.iv_offset = os_malloc(4);
    if(aes_param.iv_offset !=NULL)
    {
        os_memcpy( aes_param.iv_offset,  &iv_offset, 4);
    } else {
        BK_LOGI(NULL, "os_malloc failed!\n");
        return NIST_FAIL;
    }

    BK_LOGI(NULL, "sub_type = %u\n", sub_type);
    BK_LOGI(NULL, "count = %u\n", count);

    hal_nist_init_buffer(&data);
    hal_nist_init_buffer(&key_data);
    hal_nist_init_buffer(&output_data);

    key_data.data_len = key_len;
    key_data.data = key;

    switch (key_len) {
        case 16:
            mode = HAL_KEY_AES_128;
            break;
        case 24:
            mode = HAL_KEY_AES_192;
            break;
        case 32:
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
        os_free(aes_param.iv_offset);
        hal_nist_free_buffer(&data);
        hal_nist_free_buffer(&key_data);
        hal_nist_free_buffer(&output_data);
        return NIST_FAIL;
    }

    data.data_len = text1_len;
    data.data = text1;
    result_data = text2;

    output_data.data_len = text1_len;
    ret = hal_nist_malloc_buffer(&output_data, output_data.data_len);
    if (ret != 0) {
        BK_LOGI(NULL, "OUTPUT DATA ALLOC FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    if (!sub_type) {
        ret = armino_hal_aes_encrypt(&data, &aes_param, key_idx, &output_data);
    } else {
        ret = armino_hal_aes_decrypt(&data, &aes_param, key_idx, &output_data);
    }

    if (ret != 0) {
        BK_LOGI(NULL, "ARMINO AES ENCRYPT/DECRYPT FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    if (os_memcmp(output_data.data, result_data, output_data.data_len) != 0) {
        BK_LOGI(NULL, "THE AES RESULT IS FAULT!\n");

        print_hex(output_data.data, output_data.data_len);
        print_hex(result_data, output_data.data_len);

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
    os_free(aes_param.iv_offset);
    hal_nist_free_buffer(&output_data);

    return status;
}

uint8_t nist_aes_mct_ecb_verify_callback(nist_handle_t *nist_handle)
{
    // TODO just print data.
    uint32_t key_idx = 32;
    uint8_t sub_type = nist_handle->sub_type;
    uint16_t count_len = 0;
    uint8_t *count_data = get_param_by_index(0, &count_len);
    uint32_t count = (count_data) ? *(uint32_t *)count_data : 0;
    uint16_t key_len = 0;
    uint8_t *key = get_param_by_index(1, &key_len);
    uint16_t text1_len = 0;
    uint8_t *text1 = get_param_by_index(2, &text1_len);
    uint16_t text2_len = 0;
    uint8_t *text2 = get_param_by_index(3, &text2_len);
    uint8_t *result_data = NULL;
    int ret, status = NIST_OK;
    hal_data data, key_data, output_data;
    hal_aes_param aes_param;
    hal_key_type mode;

    aes_param.mode = HAL_AES_ECB_NOPAD;
    aes_param.iv_len = 0;
    aes_param.iv_offset = NULL;
    aes_param.iv = NULL;
    aes_param.nc_off = NULL;
    aes_param.nonce_counter = NULL;
    aes_param.stream_block = NULL;

    BK_LOGI(NULL, "sub_type = %u\n", sub_type);
    BK_LOGI(NULL, "count = %u\n", count);

    hal_nist_init_buffer(&data);
    hal_nist_init_buffer(&key_data);
    hal_nist_init_buffer(&output_data);

    key_data.data_len = key_len;
    key_data.data = key;

    switch (key_len) {
        case 16:
            mode = HAL_KEY_AES_128;
            break;
        case 24:
            mode = HAL_KEY_AES_192;
            break;
        case 32:
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

    data.data_len = text1_len;
    data.data = text1;
    result_data = text2;

    output_data.data_len = text1_len;
    ret = hal_nist_malloc_buffer(&output_data, output_data.data_len);
    if (ret != 0) {
        BK_LOGI(NULL, "OUTPUT DATA ALLOC FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    for (int i = 0; i < 1000; i++) {
        if (!sub_type) {
            ret = armino_hal_aes_encrypt(&data, &aes_param, key_idx, &output_data);
        } else {
            ret = armino_hal_aes_decrypt(&data, &aes_param, key_idx, &output_data);
        }

        if (ret != 0) {
            BK_LOGI(NULL, "ARMINO AES ENCRYPT/DECRYPT FAIL!\n");
            status = NIST_FAIL;
            goto key_exit;
        } else {
            memcpy(data.data, output_data.data, output_data.data_len);
        }
    }

    if (os_memcmp(output_data.data, result_data, output_data.data_len) != 0) {
        BK_LOGI(NULL, "THE AES RESULT IS FAULT!\n");

        print_hex(output_data.data, output_data.data_len);
        print_hex(result_data, output_data.data_len);
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

uint8_t nist_aes_mct_cbc_verify_callback(nist_handle_t *nist_handle)
{
    // TODO just print data.
    uint32_t key_idx = 32;
    uint8_t sub_type = nist_handle->sub_type;
    uint16_t count_len = 0;
    uint8_t *count_data = get_param_by_index(0, &count_len);
    uint32_t count = (count_data) ? *(uint32_t *)count_data : 0;
    uint16_t key_len = 0;
    uint8_t *key = get_param_by_index(1, &key_len);
    uint16_t iv_len = 0;
    uint8_t *iv = get_param_by_index(2, &iv_len);
    uint16_t text1_len = 0;
    uint8_t *text1 = get_param_by_index(3, &text1_len);
    uint16_t text2_len = 0;
    uint8_t *text2 = get_param_by_index(4, &text2_len);
    uint8_t *result_data = NULL;
    uint8_t *cj_data = NULL;
    int ret, status = NIST_OK;
    hal_data data, key_data, output_data;
    hal_aes_param aes_param;
    hal_key_type mode;

    aes_param.mode = HAL_AES_CBC_NOPAD;
    aes_param.iv_len = iv_len;
    aes_param.iv_offset = NULL;
    aes_param.iv = iv;
    aes_param.nc_off = NULL;
    aes_param.nonce_counter = NULL;
    aes_param.stream_block = NULL;

    BK_LOGI(NULL, "sub_type = %u\n", sub_type);
    BK_LOGI(NULL, "count = %u\n", count);

    hal_nist_init_buffer(&data);
    hal_nist_init_buffer(&key_data);
    hal_nist_init_buffer(&output_data);

    key_data.data_len = key_len;
    key_data.data = key;

    switch (key_len) {
        case 16:
            mode = HAL_KEY_AES_128;
            break;
        case 24:
            mode = HAL_KEY_AES_192;
            break;
        case 32:
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

    data.data_len = text1_len;
    data.data = text1;
    result_data = text2;

    output_data.data_len = text1_len;
    ret = hal_nist_malloc_buffer(&output_data, output_data.data_len);
    if (ret != 0) {
        BK_LOGI(NULL, "OUTPUT DATA ALLOC FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    cj_data = os_malloc(output_data.data_len);
    if(cj_data == NULL) {
        BK_LOGI(NULL, "os_malloc failed!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    for (int i = 0; i < 1000; i++) {
        if (!sub_type) {
            ret = armino_hal_aes_encrypt(&data, &aes_param, key_idx, &output_data);
        } else {
            ret = armino_hal_aes_decrypt(&data, &aes_param, key_idx, &output_data);
        }

        if (ret != 0) {
            BK_LOGI(NULL, "ARMINO AES ENCRYPT/DECRYPT FAIL!\n");
            if (i != 0)
                os_free(aes_param.iv);
            status = NIST_FAIL;
            goto cj_exit;
        } else {

            if (i != 0)
                os_free(aes_param.iv);

            aes_param.iv = os_malloc(output_data.data_len);
            if(aes_param.iv == NULL) {
                BK_LOGI(NULL, "os_malloc failed!\n");
                status = NIST_FAIL;
                goto cj_exit;
            }

            if (!sub_type) {
                memcpy(aes_param.iv, output_data.data, output_data.data_len);
            } else {
                memcpy(aes_param.iv, data.data, output_data.data_len);
            }

            if (i == 0) {
                memcpy(data.data, iv, data.data_len);
            } else {
                memcpy(data.data, cj_data, data.data_len);
            }

            memcpy(cj_data, output_data.data, output_data.data_len);
        }
    }

    if (os_memcmp(output_data.data, result_data, output_data.data_len) != 0) {
        BK_LOGI(NULL, "THE AES RESULT IS FAULT!\n");

        print_hex(output_data.data, output_data.data_len);
        print_hex(result_data, output_data.data_len);

        status = NIST_FAIL;
        goto iv_exit;
    }

iv_exit:
    os_free(aes_param.iv);
cj_exit:
    os_free(cj_data);

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

uint8_t nist_aes_mct_cfb_verify_callback(nist_handle_t *nist_handle)
{
    // TODO just print data.
    uint32_t key_idx = 32, iv_offset = 0;
    uint8_t sub_type = nist_handle->sub_type;
    uint16_t count_len = 0;
    uint8_t *count_data = get_param_by_index(0, &count_len);
    uint32_t count = (count_data) ? *(uint32_t *)count_data : 0;
    uint16_t key_len = 0;
    uint8_t *key = get_param_by_index(1, &key_len);
    uint16_t iv_len = 0;
    uint8_t *iv = get_param_by_index(2, &iv_len);
    uint16_t text1_len = 0;
    uint8_t *text1 = get_param_by_index(3, &text1_len);
    uint16_t text2_len = 0;
    uint8_t *text2 = get_param_by_index(4, &text2_len);
    uint8_t *result_data = NULL;
    uint8_t *cj_data = NULL;
    int ret, status = NIST_OK;
    hal_data data, key_data, output_data;
    hal_aes_param aes_param;
    hal_key_type mode;

    aes_param.mode = HAL_AES_CFB128;
    aes_param.iv_len = iv_len;
    aes_param.iv_offset = NULL;
    aes_param.iv = iv;
    aes_param.nc_off = NULL;
    aes_param.nonce_counter = NULL;
    aes_param.stream_block = NULL;

    aes_param.iv_offset = os_malloc(4);
    if(aes_param.iv_offset !=NULL)
    {
        os_memcpy( aes_param.iv_offset,  &iv_offset, 4);
    } else {
        BK_LOGI(NULL, "os_malloc failed!\n");
        return NIST_FAIL;
    }

    BK_LOGI(NULL, "sub_type = %u\n", sub_type);
    BK_LOGI(NULL, "count = %u\n", count);

    hal_nist_init_buffer(&data);
    hal_nist_init_buffer(&key_data);
    hal_nist_init_buffer(&output_data);

    key_data.data_len = key_len;
    key_data.data = key;

    switch (key_len) {
        case 16:
            mode = HAL_KEY_AES_128;
            break;
        case 24:
            mode = HAL_KEY_AES_192;
            break;
        case 32:
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

    data.data_len = text1_len;
    data.data = text1;
    result_data = text2;

    output_data.data_len = text1_len;
    ret = hal_nist_malloc_buffer(&output_data, output_data.data_len);
    if (ret != 0) {
        BK_LOGI(NULL, "OUTPUT DATA ALLOC FAIL!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    cj_data = os_malloc(output_data.data_len);
    if(cj_data == NULL) {
        BK_LOGI(NULL, "os_malloc failed!\n");
        status = NIST_FAIL;
        goto key_exit;
    }

    for (int i = 0; i < 1000; i++) {
        if (!sub_type) {
            ret = armino_hal_aes_encrypt(&data, &aes_param, key_idx, &output_data);
        } else {
            ret = armino_hal_aes_decrypt(&data, &aes_param, key_idx, &output_data);
        }

        if (ret != 0) {
            BK_LOGI(NULL, "ARMINO AES ENCRYPT/DECRYPT FAIL!\n");
            if (i != 0)
                os_free(aes_param.iv);
            status = NIST_FAIL;
            goto cj_exit;

        } else {

            if (i != 0)
                os_free(aes_param.iv);

            aes_param.iv = os_malloc(output_data.data_len);
            if(aes_param.iv == NULL) {
                BK_LOGI(NULL, "os_malloc failed!\n");
                status = NIST_FAIL;
                goto cj_exit;
            }

            if (!sub_type) {
                memcpy(aes_param.iv, output_data.data, output_data.data_len);
            } else {
                memcpy(aes_param.iv, data.data, output_data.data_len);
            }

            if (i == 0) {
                memcpy(data.data, iv, data.data_len);
            } else {
                memcpy(data.data, cj_data, data.data_len);
            }

            memcpy(cj_data, output_data.data, output_data.data_len);
        }
    }

    if (os_memcmp(output_data.data, result_data, output_data.data_len) != 0) {
        BK_LOGI(NULL, "THE AES RESULT IS FAULT!\n");

        print_hex(output_data.data, output_data.data_len);
        print_hex(result_data, output_data.data_len);

        status = NIST_FAIL;
        goto iv_exit;
    }

iv_exit:
    os_free(aes_param.iv);

cj_exit:
    os_free(cj_data);

key_exit:
    ret = armino_hal_remove_key(mode, key_idx);
    if (ret != 0) {
        BK_LOGI(NULL, "ARMINO REMOVE KEY FAIL!\n");
        status = NIST_FAIL;
    }

exit:
    os_free(aes_param.iv_offset);
    hal_nist_free_buffer(&output_data);

    return status;
}

