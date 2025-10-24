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

#include <common/bk_include.h>
#include <driver/hal/hal_jpeg_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief JPEGENC APIs Version 1.0
 * @{
 */

#define Y_PIXEL_272     (34)  /**< image resolution for hight: Y * 8 = 272  */
#define Y_PIXEL_320     (40)  /**< image resolution for hight: Y * 8 = 320  */
#define X_PIXEL_480     (60)  /**< image resolution for width: X * 8 = 480  */
#define Y_PIXEL_480     (60)  /**< image resolution for hight: Y * 8 = 480  */
#define X_PIXEL_640     (80)  /**< image resolution for hight: X * 8 = 640  */
#define Y_PIXEL_240     (30)  /**< image resolution for hight: Y * 8 = 240  */
#define X_PIXEL_320     (40)  /**< image resolution for hight: X * 8 = 320  */
#define Y_PIXEL_600     (75)  /**< image resolution for hight: Y * 8 = 600  */
#define X_PIXEL_800     (100) /**< image resolution for hight: X * 8 = 800  */
#define Y_PIXEL_720     (90)  /**< image resolution for hight: Y * 8 = 720  */
#define X_PIXEL_1280    (160) /**< image resolution for hight: X * 8 = 1280 */
#define X_PIXEL_1600    (200) /**< image resolution for hight: X * 8 = 1600 */
#define Y_PIXEL_1200    (150) /**< image resolution for hight: Y * 8 = 1200 */

/**
 * @brief jpeg isr callback relay jpeg driver
 *
 * @param id: only 0 useful, set 0 default
 * @param param: NULL default
 */
typedef void (*jpeg_isr_t)(jpeg_unit_t id, void *param);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

