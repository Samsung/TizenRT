// Copyright 2020-2023 Beken
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

#include <common/bk_err.h>

#define PSRAM_CHIP_ID    "psram_id"

#define BK_ERR_PSRAM_SERVER_NOT_INIT   (BK_ERR_PSRAM_BASE - 1) /**< psram server not init */
#define BK_ERR_PSRAM_ADDR_ALIGN        (BK_ERR_PSRAM_BASE - 2)
#define BK_ERR_PSRAM_ADDR_OUT_OF_RANGE (BK_ERR_PSRAM_BASE - 3)
#define BK_ERR_PSRAM_ADDR_RELATION     (BK_ERR_PSRAM_BASE - 4)
#define BK_ERR_PSRAM_AREA              (BK_ERR_PSRAM_BASE - 5)

typedef enum {
	PSRAM_240M = 0,
	PSRAM_160M,
	PSRAM_120M,
	PSRAM_80M
} psram_clk_t;

typedef enum {
	PSRAM_OUT_3_20V = 0,
	PSRAM_OUT_3_0V,
	PSRAM_OUT_2_0V,
	PSRAM_OUT_1_95V,
	PSRAM_OUT_1_90V,
	PSRAM_OUT_1_85V,
	PSRAM_OUT_1_80V,
} psram_voltage_t;

typedef enum {
	PSRAM_INCRASE_TRANS = 0,
	PSRAM_32_WORD_WRAP,
	PSRAM_64_WORD_WRAP,
	PSRAM_128_WORD_WRAP,
	PSRAM_256_WORD_WRAP,
	PSRAM_512_WORD_WRAP,
	PSRAM_1024_WORD_WRAP,
	PSRAM_2048_WORD_WRAP
} psram_tansfer_mode_t;

typedef enum {
	PSRAM_WRITE_THROUGH_AREA_0 = 0,
	PSRAM_WRITE_THROUGH_AREA_1,
	PSRAM_WRITE_THROUGH_AREA_2,
	PSRAM_WRITE_THROUGH_AREA_3,
	PSRAM_WRITE_THROUGH_AREA_COUNT
} psram_write_through_area_t;

typedef enum
{
	PSRAM_HEAP_USER = 0,
	PSRAM_HEAP_AUDIO,
	PSRAM_HEAP_ENCODE,
	PSRAM_HEAP_YUV,
	PSRAM_HEAP_MAX,
} psram_heap_type_t;

typedef enum
{
	FB_INDEX_DISPLAY,
	FB_INDEX_JPEG,
	FB_INDEX_H264,
	FB_INDEX_H265,
	FB_INDEX_MAX,
} fb_type_t;

typedef enum
{
	FB_MEM_SHARED,
	FB_MEM_ALONE,
} fb_mem_mode_t;

/*
 * @}
 */

#ifdef __cplusplus
}
#endif


