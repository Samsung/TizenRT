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

#include <components/log.h>
#include <driver/fft_types.h>

#define FFT_TAG "fft"
#define FFT_LOGI(...) BK_LOGI(FFT_TAG, ##__VA_ARGS__)
#define FFT_LOGW(...) BK_LOGW(FFT_TAG, ##__VA_ARGS__)
#define FFT_LOGE(...) BK_LOGE(FFT_TAG, ##__VA_ARGS__)
#define FFT_LOGD(...) BK_LOGD(FFT_TAG, ##__VA_ARGS__)

/**
 * @brief     Enable fft module to process fir function
 *
 * This API process fir function :
 *  - Set fir work mode: signal/dual
 *  - Configure the fir parameters
 *  - start trigger
 *  - enable interrupts
 * Usage example:
 *
 *     //init fft driver
 *     ret = bk_fft_driver_init();
 *
 *     fir_conf.coef_c0 = os_malloc(2*FFT_TEST_DATA_SIZE);
 *     os_memset(fir_conf.coef_c0, 0, 2*FFT_TEST_DATA_SIZE);
 *     fir_conf.coef_c1 = NULL;
 *     fir_conf.input_d0 = os_malloc(2*FFT_TEST_DATA_SIZE);
 *     os_memset(fir_conf.input_d0, 0, 2*FFT_TEST_DATA_SIZE);
 *     fir_conf.input_d1 = NULL;
 *     fir_conf.mode = FFT_FIR_MODE_SIGNAL;
 *     fir_conf.fir_len = FFT_TEST_DATA_SIZE;
 *
 *     //start fir
 *     bk_fft_fir_single_enable(&fir_conf);
 *     CLI_LOGI("start fir process \r\n");
 *
 *     //wait fir complete
 *     while(fft_busy_flag)
 *         bk_fft_is_busy(&fft_busy_flag);
 *     CLI_LOGI("fft complete\r\n");
 *
 *     //read output data
 *     bk_fft_output_read(data_proc_i, data_proc_q, 2 * FFT_TEST_DATA_SIZE);
 *
 * @param fir_conf fir parameters
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_fft_fir_single_enable(fft_fir_input_t *fir_conf);

