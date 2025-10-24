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
//
#pragma once

#include <driver/int_types.h>
#include <common/bk_include.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef void (*fft_isr_t)(void *param);


/**
 * @brief FFT defines
 * @defgroup bk_api_fft_defs macos
 * @ingroup bk_api_fft
 * @{
 */

#define BK_ERR_FFT_NOT_INIT			(BK_ERR_FFT_BASE - 1) /**< FFT driver not init */
#define BK_ERR_FFT_PARAM			(BK_ERR_FFT_BASE - 2) /**< FFT parameter invalid */

/**
 * @}
 */

/**
 * @brief FFT enum defines
 * @defgroup bk_api_aud_enum FFT enums
 * @ingroup bk_api_fft
 * @{
 */

typedef enum {
	FFT_WORK_MODE_FFT = 0, /**< FFT fft work mode */
	FFT_WORK_MODE_IFFT,    /**< FFT ifft work mode */
	FFT_WORK_MODE_MAX,
} fft_work_mode_t;

typedef enum {
	FFT_MODE_NORMAL = 0, /**< FFT normal mode */
	FFT_MODE_BK5130,     /**< FFT bk5130 mode */
	FFT_MODE_MAX,
} fft_mode_t;

typedef enum {
	FFT_IFFT_DISABLE = 0, /**< IFFT disable */
	FFT_IFFT_ENABLE,      /**< IFFT enable */
	FFT_IFFT_MAX,
} fft_ifft_en_t;

typedef enum {
	FFT_INT_DISABLE = 0, /**< FFT interrupt disable */
	FFT_INT_ENABLE,      /**< FFT interrupt enable */
	FFT_INT_MAX,
} fft_int_en_t;

typedef enum {
	FFT_DISABLE = 0, /**< FFT disable */
	FFT_ENABLE,      /**< FFT enable */
	FFT_ENABLE_MAX,
} fft_enable_t;

typedef enum {
	FFT_GAT_ON = 0, /**< FFT gat on */
	FFT_GAT_OFF,    /**< FFT gat off */
	FFT_GAT_MAX,
} fft_gat_t;

typedef enum {
	FFT_FIR_MODE_SIGNAL = 0, /**< FIR signal mode */
	FFT_FIR_MODE_DUAL,       /**< FIR dual mode */
	FFT_FIR_MODE_MAX,
} fft_fir_mode_t;

typedef enum {
	FFT_FIR_INT_DISABLE = 0, /**< FIR interrupt disable */
	FFT_FIR_INT_ENABLE,      /**< FIR interrupt enable */
	FFT_FIR_INT_MAX,
} fft_fir_int_en_t;

typedef enum {
	FFT_FIR_DISABLE = 0, /**< FIR disable */
	FFT_FIR_ENABLE,      /**< FIR enable */
	FFT_FIR_ENABLE_MAX,
} fft_fir_en_t;

typedef enum {
	FFT_SELF_PROC_MODE0 = 0, /**< Self process mode1 */
	FFT_SELF_PROC_MODE1,     /**< Self process mode2 */
	FFT_SELF_PROC_MODE_MAX,
} fft_self_proc_mode_t;

typedef enum {
	FFT_SELECT_MEMORY0 = 0, /**< FFT select Memroy0 */
	FFT_SELECT_MEMORY1,     /**< FFT select Memroy1 */
	FFT_SELECT_MEMORY_MAX,
} fft_memory_config_t;


/**
 * @}
 */

/**
 * @brief FFT struct defines
 * @defgroup bk_api_aud_structs structs in FFT
 * @ingroup bk_api_fft
 * @{
 */

typedef struct {
	icu_int_src_t int_src;
	int_group_isr_t isr;
} fft_int_config_t;

typedef struct {
	int16 *i_out;
	int16 *q_out;
	uint32_t busy_flag;
	uint32_t size;
} fft_driver_t;

typedef struct {
	/* fifo status */
	bool fft_done;       /**< FFT fft process done */
	bool fir_done;       /**< FFT fir process done */
	bool self_proc_done; /**< FFT self process done */
	bool start_trigger;  /**< FFT start trigger flag */
	uint32_t bit_ext;    /**< FFT fft bit extend */
} fft_status_t;

typedef struct {
	bool fft_int_enable;
	bool fir_int_enable;
} fft_int_status;


typedef struct
{
	fft_work_mode_t mode;
	uint32_t *inbuf;
	uint32_t size;
} fft_input_t;

typedef struct
{
	uint32_t mode;
	uint32_t fir_len;
	int16 *coef_c0;
	int16 *coef_c1;
	int16 *input_d0;
	int16 *input_d1;
} fft_fir_input_t;

/**
 * @}
 */


#ifdef __cplusplus
}
#endif
