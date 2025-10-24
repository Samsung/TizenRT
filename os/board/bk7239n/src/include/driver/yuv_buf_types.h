// Copyright 2022-2023 Beken
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
#include <driver/hal/hal_yuv_buf_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_YUV_BUF_DRIVER_NOT_INIT             (BK_ERR_YUV_BUF_BASE - 1) /**< YUV_BUF driver not init */

typedef uint8_t yuv_buf_unit_t; /**< yuv_buf uint id */

typedef enum {
	YUV_BUF_VSYNC_NEGEDGE = 0, /**< when detect vsync negedge will trigger this isr */
	YUV_BUF_YUV_ARV,
	YUV_BUF_SM0_WR,
	YUV_BUF_SM1_WR,
	YUV_BUF_FULL,
	YUV_BUF_ENC_LINE,
	YUV_BUF_SEN_RESL,
	YUV_BUF_H264_ERR,
	YUV_BUF_ENC_SLOW,
	YUV_BUF_ISR_MAX,
} yuv_buf_isr_type_t;

typedef void (*yuv_buf_isr_t)(yuv_buf_unit_t id, void *param);

#ifdef __cplusplus
}
#endif

