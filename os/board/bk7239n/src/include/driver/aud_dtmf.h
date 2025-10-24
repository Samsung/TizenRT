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
#include <driver/aud_dtmf_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief AUD API
 * @defgroup bk_api_aud AUD API group
 * @{
 */


/**
 * @brief     Init the dtmf module of audio
 *
 * This API init the dtmf module:
 *  - Configure the dtmf parameters to enable dtmf function.
 *
 * @param
 *    - dtmf_config: dtmf parameters configure
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: config is NULL
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dtmf_init(aud_dtmf_config_t *dtmf_config);

/**
 * @brief     Deinit dtmf module of audio
 *
 * This API deinit the dtmf module:
 *   - Configure the dtmf parameters to default value.
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DTMF_NOT_INIT: audio dtmf is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dtmf_deinit(void);

/**
 * @brief     Get the audio dtmf fifo address
 *
 * @param
 *    - dtmf_fifo_addr: dtmf fifo address
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_dtmf_get_fifo_addr(uint32_t *dtmf_fifo_addr);

/**
 * @brief     Get dtmf data
 *
 * This API get dtmf fifo data
 *
 * @param dtmf_data save audio dtmf fifo data
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DTMF_NOT_INIT: audio dtmf is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dtmf_get_fifo_data(uint32_t *dtmf_data);

/**
 * @brief   Get the dtmf status information
 *
 * This API get the dtmf fifo status.
 *
 * @param
 *    - dtmf_status: dtmf fifo status
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DTMF_NOT_INIT: audio dtmf is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dtmf_get_status(uint32_t *dtmf_status);

/**
 * @brief     Enable dtmf interrupt
 *
 * This API enable dtmf interrupt:
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DTMF_NOT_INIT: audio dtmf is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dtmf_enable_int(void);

/**
 * @brief     Disable dtmf interrupt
 *
 * This API disable dtmf interrupt:
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DTMF_NOT_INIT: audio dtmf is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dtmf_disable_int(void);

/**
 * @brief     Start dtmf
 *
 * This API start dtmf function.
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DTMF_NOT_INIT: audio dtmf is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dtmf_start(void);

/**
 * @brief     Stop dtmf
 *
 * This API stop dtmf function.
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DTMF_NOT_INIT: audio dtmf is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dtmf_stop(void);

/**
 * @brief     Set audio dtmf data write threshold
 *
 * This API set audio dtmf data write threshold:
 *          - audio dtmf interrupt will trigger when the amount of data in FIFO reaches the threshold
 *
 * @param value the value of write threshold
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DTMF_NOT_INIT: audio dtmf is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dtmf_set_dtmf_wr_threshold(uint32_t value);

/**
 * @brief     Register audio dtmf isr
 *
 * @param isr audio isr callback
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DTMF_NOT_INIT: audio dtmf is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dtmf_register_isr(aud_isr_t isr);

/**
 * @brief     Enable audio dtmf loop test
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DTMF_NOT_INIT: audio dtmf is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dtmf_start_loop_test(void);

/**
 * @brief     Disable audio dtmf loop test
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DTMF_NOT_INIT: audio dtmf is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dtmf_stop_loop_test(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
