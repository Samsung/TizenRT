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
#include <common/bk_include.h>
#include <driver/fft_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief FFT API
 * @defgroup bk_api_fft FFT API group
 * @{
 */


/**
 * @brief     Get the fft module working status
 *
 * @return
 *    - TRUE: fft is busying
 *    - FALSE: fft is not busying
 */
bool bk_fft_is_busy(void);

/**
 * @brief     Enable fft module to process fft/ifft function
 *
 * This API process fft/ifft function :
 *  - Set fft work mode: fft/ifft
 *  - Configure the fft/ifft parameters
 *  - start trigger
 *  - enable interrupts
 *
 * Usage example:
 *
 *     //init fft driver
 *     bk_fft_driver_init();
 *
 *     fft_conf.inbuf = os_malloc(4*1024);
 *     os_memset(fft_conf.inbuf, 0, 4*1024);
 *
 *     fft_conf.mode = FFT_WORK_MODE_FFT;
 *     fft_conf.size = 1024;
 *
 *     //start fft
 *     bk_fft_enable(&fft_conf);
 *
 *     //wait fft complete
 *     while(fft_busy_flag)
 *         bk_fft_is_busy(&fft_busy_flag);
 *     CLI_LOGI("fft complete\r\n");
 *     //read output data
 *     bk_fft_output_read(data_proc_i, data_proc_q, 2 * 1024);
 *
 * @param fft_conf fft/ifft parameters
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_fft_enable(fft_input_t *fft_conf);

/**
 * @brief     Init fft module driver
 *
 * This API init fft driver :
 *  - power on
 *  - enable interrupts
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_fft_driver_init(void);

/**
 * @brief     Deinit fft module driver
 *
 * This API deinit fft driver :
 *  - Power down clock
 *  - disable interrupts
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_fft_driver_deinit(void);

/**
 * @brief     Read fft data
 *
 * This API read fft data after processing complete
 *

 * @param i_output save fft data
 * @param q_output save fft data
 * @param size read size if i_output
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_fft_output_read(int16_t *i_output, int16_t *q_output, uint32_t size);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
