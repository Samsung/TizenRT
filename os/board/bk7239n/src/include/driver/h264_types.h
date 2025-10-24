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
#include <driver/hal/hal_h264_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_H264_DRIVER_NOT_INIT             (BK_ERR_H264_BASE - 1)
#define BK_ERR_H264_ISR_INVALID_ID				(BK_ERR_H264_BASE - 2)
#define BK_ERR_H264_INVALID_RESOLUTION_TYPE		(BK_ERR_H264_BASE - 3)
#define BK_ERR_H264_INVALID_PFRAME_NUMBER		(BK_ERR_H264_BASE - 4)
#define BK_ERR_H264_INVALID_QP					(BK_ERR_H264_BASE - 5)
#define BK_ERR_H264_INVALID_IMB_BITS			(BK_ERR_H264_BASE - 6)
#define BK_ERR_H264_INVALID_PMB_BITS			(BK_ERR_H264_BASE - 7)
#define BK_ERR_H264_INVALID_CONFIG_PARAM		(BK_ERR_H264_BASE - 8)
#define BK_ERR_H264_INVALID_PIXEL_HEIGHT		(BK_ERR_H264_BASE - 9)

#define INT_SKIP_FRAME      0x1
#define INT_FINAL_OUT       0x2
#define INT_LINE_DONE       0x4

#define	H264_CLK_SEL_480M		1
#define H264_AHB_CLK_DIV		3
#define H264_AHB_CLK_SEL		3
#define H264_INT_ENABLE			0x7
#define H264_INT_DISABLE		0
#define H264_CPU_INT_ENABLE		1
#define H264_CPU_INT_DISABLE	0

#define MAX_QP					51
#define MIN_QP					0
#define MAX_PFRAME				5
#define MIN_PFRAME				0
#define MAX_IFRAME_BITS			3412
#define MAX_PFRAME_BITS			4095
#define MIN_FRAME_BITS			0
#define DEFAULT_SKIP_MODE		1

typedef enum {
	H264_SKIP_FRAME = 0,
	H264_FINAL_OUT,
	H264_LINE_DONE,
	H264_ISR_MAX,
} h264_isr_type_t;

typedef struct
{
	uint8_t h264_state;
	uint8_t p_frame_cnt;
	uint8_t profile_id;
	uint8_t qp;
	uint16_t num_imb_bits;
	uint16_t num_pmb_bits;
	uint16_t width;
	uint16_t height;
} h264_base_config_t;

#ifdef __cplusplus
}
#endif