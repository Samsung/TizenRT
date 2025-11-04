/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#include <stdint.h>
#include <string.h>
#include "include/armino_adapter_common.h"

#define AES_BLOCK_SIZE       (16)
#define AES_BLOCK_SIZE_MASK  (15)

//PKSC7 padding
static int pkcs7_pad(const uint8_t *data, size_t data_len, uint8_t *padded_data, size_t *padded_len)
{
    //printf("pkcs7_pad, data_len :%d, padded_len :%d \r\n", data_len, *padded_len);
    size_t block_size = AES_BLOCK_SIZE;
    size_t pad_len = block_size - (data_len & AES_BLOCK_SIZE_MASK);

    if((data == NULL) ||(padded_data == NULL))
    {
        return -1;
    }

    if (padded_data && data)
    {
        os_memcpy(padded_data, data, data_len);
        os_memset(padded_data + data_len, pad_len, pad_len);
    }

    *padded_len = data_len + pad_len;

    return 0;
}

// ISO9797-M1 padding
static int iso9797_m1_pad(const uint8_t *data, size_t data_len, uint8_t *padded_data, size_t *padded_len)
{
    size_t block_size = AES_BLOCK_SIZE;
    size_t pad_len = block_size - (data_len & AES_BLOCK_SIZE_MASK);

    if (pad_len == block_size)
    {
        pad_len = 0;
    }

    if((data == NULL) ||(padded_data == NULL))
    {
        return -1;
    }
    if (padded_data && data) {
        os_memcpy(padded_data, data, data_len);
        if (pad_len > 0)
        {
            os_memset(padded_data + data_len, 0x00, pad_len);
        }
    }

    *padded_len = data_len + pad_len;
    return 0;
}

// ISO9797-M2 padding
static int iso9797_m2_pad(const uint8_t *data, size_t data_len, uint8_t *padded_data, size_t *padded_len)
{
    size_t block_size = AES_BLOCK_SIZE;
    size_t pad_len;

    if ((data_len & AES_BLOCK_SIZE_MASK) == 0)
    {
        pad_len = block_size;
    }
    else
    {
        pad_len = block_size - (data_len & AES_BLOCK_SIZE_MASK);
    }

    if((data == NULL) ||(padded_data == NULL))
    {
        return -1;
    }
    if (padded_data && data)
    {
        os_memcpy(padded_data, data, data_len);
        padded_data[data_len] = 0x80;
        if (pad_len > 1)
        {
            os_memset(padded_data + data_len + 1, 0x00, pad_len - 1);
        }
    }

    *padded_len = data_len + pad_len;
    return 0;
}

static int pkcs7_unpad(const uint8_t *padded_data, size_t padded_len, uint8_t *data, size_t *data_len)
{
    if (padded_len == 0 || (padded_len & AES_BLOCK_SIZE_MASK) != 0) 
    {
        return -1;
    }

    uint8_t pad_len = padded_data[padded_len - 1];
    if (pad_len == 0 || pad_len > AES_BLOCK_SIZE)
    {
        return -1;
    }

    for (size_t i = padded_len - pad_len; i < padded_len; i++)
    {
        if (padded_data[i] != pad_len)
        {
            return -1;
        }
    }

    *data_len = padded_len - pad_len;
    if (data)
    {
        os_memcpy(data, padded_data, *data_len);
    }

    return 0;
}

static int iso9797_m1_unpad(const uint8_t *padded_data, size_t padded_len, uint8_t *data, size_t *data_len)
{
    if (padded_len == 0 || (padded_len & AES_BLOCK_SIZE_MASK) != 0)
    {
        return -1;
    }

    size_t data_end = padded_len;
    for (size_t i = padded_len - 1; i > 0; i--)
    {
        if (padded_data[i] != 0x00)
        {
            data_end = i + 1;
            break;
        }
    }

    if (data_end == 0)
    {
        *data_len = 0;
        return 0;
    }

    *data_len = data_end;
    if (data)
    {
        os_memcpy(data, padded_data, data_end);
    }

    return 0;
}

static int iso9797_m2_unpad(const uint8_t *padded_data, size_t padded_len, uint8_t *data, size_t *data_len) 
{
    if (padded_len == 0 || padded_len % AES_BLOCK_SIZE != 0) 
    {
        return -1;
    }

    int found = 0;
    size_t data_end = padded_len;
    for (size_t i = padded_len - 1; i > 0; i--)
    {
        if (padded_data[i] == 0x80)
        {
            data_end = i;
            found = 1;
            break;
        }
        if (padded_data[i] != 0x00)
        {
            return -1;
        }
    }

    if (!found) {
        if (padded_data[padded_len - 1] == 0x80)
        {
            data_end = padded_len - 1;
            found = 1;
        }
        else
        {
            return -1;
        }
    }

    *data_len = data_end;
    if (data) {
        os_memcpy(data, padded_data, data_end);
    }

    return 0;
}

uint32_t aes_get_padded_len(size_t data_len)
{
    size_t block_size = AES_BLOCK_SIZE;
    size_t pad_len = 0;

    if ((data_len & AES_BLOCK_SIZE_MASK) == 0)
    {
        pad_len = block_size;
    }
    else
    {
        pad_len = block_size - (data_len & AES_BLOCK_SIZE_MASK);
    }

    return data_len + pad_len;
}

int aes_padding_adapter(hal_aes_algo mode, const uint8_t *input,  size_t input_len,  uint8_t *output, size_t *output_len, int operation)  // 0: pad, 1: unpad
{
    if((input_len == 0) ||(input == NULL) || (output == NULL))
    {
        return -1;
    }

    switch (mode)
    {
        case HAL_AES_ECB_NOPAD:
        case HAL_AES_CBC_NOPAD:
        {
            printf("ecb&cbc no padding \r\n");
            if (operation == 0)
            {
                if((input_len  & AES_BLOCK_SIZE_MASK) != 0)
                {
                    printf("line :%d, input_len %d is not multiple of AES_BLOCK_SIZE \n", __LINE__, input_len);
                    return -1;
                }
                if (output && input)
                {
                    os_memcpy(output, input, input_len);
                }
                *output_len = input_len;
                return 0;
            }
            else
            {
                if((input_len  & AES_BLOCK_SIZE_MASK) != 0)
                {
                    return -1;
                }
                if (output && input) {
                    memcpy(output, input, input_len);
                }
                *output_len = input_len;
                return 0;
            }
        }
        case HAL_AES_ECB_PKCS5:
        case HAL_AES_ECB_PKCS7:
        case HAL_AES_CBC_PKCS5:
        case HAL_AES_CBC_PKCS7:
        {
            printf("add pkcs5&pkcs7 padding mode :%d \r\n",mode);
            if (operation == 0)
            {
                return pkcs7_pad(input, input_len, output, output_len);
            }
            else
            {
                return pkcs7_unpad(input, input_len, output, output_len);
            }
        }
        case HAL_AES_ECB_ISO9797_M1:
        case HAL_AES_CBC_ISO9797_M1:
        {
            printf("add M1 padding \r\n");
            if (operation == 0)
            {
                return iso9797_m1_pad(input, input_len, output, output_len);
            }
            else
            {
                return iso9797_m1_unpad(input, input_len, output, output_len);
            }
        }
        case HAL_AES_ECB_ISO9797_M2:
        case HAL_AES_CBC_ISO9797_M2:
        {
            printf("add M2 padding \r\n");
            if (operation == 0)
            {
                return iso9797_m2_pad(input, input_len, output, output_len);
            }
            else
            {
                return iso9797_m2_unpad(input, input_len, output, output_len);
            }
        }
        case HAL_AES_CTR:
        case HAL_AES_CFB128:
        {
            printf("ctr&cfb128 no padding \r\n");
            if (output && input)
            {
                memcpy(output, input, input_len);
            }
            *output_len = input_len;
            return 0;
        }
        default:
            return -1;
    }
}
