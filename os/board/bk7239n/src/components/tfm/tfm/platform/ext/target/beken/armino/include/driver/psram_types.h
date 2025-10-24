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


typedef enum {
	PSRAM_240M = 0,
	PSRAM_160M,
	PSRAM_120M,
	PSRAM_80M
}psram_clk_t;

typedef enum {
	PSRAM_OUT_3_2V = 0,
	PSRAM_OUT_3V,
	PSRAM_OUT_2V,
	PSRAM_OUT_1_8V
}psram_voltage_t;

typedef enum {
	PSRAM_INCRASE_TRANS = 0,
	PSRAM_32_WORD_WRAP,
	PSRAM_64_WORD_WRAP,
	PSRAM_128_WORD_WRAP,
	PSRAM_256_WORD_WRAP,
	PSRAM_512_WORD_WRAP,
	PSRAM_1024_WORD_WRAP,
	PSRAM_2048_WORD_WRAP
}psram_tansfer_mode_t;





/*
 * @}
 */

#ifdef __cplusplus
}
#endif


