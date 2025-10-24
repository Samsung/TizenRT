// Copyright 2020-2021 Beken
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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief JPEG_SW_ENC APIs Version 1.0
 * @{
 */

typedef struct _jpeg_sw_encoder
{
    void *codec;
    uint8_t *input;
    uint8_t *output;
    uint16_t width;
    uint16_t height;
    uint16_t header_len;
    int enc_size;
    int (*open)(void **codec, uint16_t width, uint16_t height, uint8_t *input, uint8_t *output, uint16_t *header_len);
    int (*reset)(void *codec);
    int (*enc)(void *codec, uint8_t* output, int remaining_size, int *enc_size);                                       
    int (*deinit)(void **codec);    
}jpeg_sw_encoder_t;

extern jpeg_sw_encoder_t jpeg_sw_encoder;

/**
 * @brief     Initialize the JPEG softeware encoder
 *
 * @attention 1. only for software jpeg encode
 *
 * @return
 */

void jpeg_sw_encoder_init(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

