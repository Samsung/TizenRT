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
#include <driver/aud_dmic_types.h>

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
 * @brief     Init the dmic module of audio
 *
 * This API init the adc module:
 *  - Init audio driver
 *  - Set audio adc sample rate, work mode...
 *  - Configure dmic
 *
 * @param dmic_config audio dmic config
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: config is NULL
 *    - BK_ERR_AUD_DRV_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_init(aud_dmic_config_t *dmic_config);

/**
 * @brief     Deinit dmic module of audio
 *
 * This API deinit the dmic module:
 *   - Configure the dmic parameters to default value.
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DMIC_NOT_INIT: audio dmic is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_deinit(void);

/**
 * @brief     Set the dmic sample rate
 *
 * This API set the dmic sample rate value.
 *
 * @param
 *    - samp_rate: dmic sample rate
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DMIC_NOT_INIT: audio dmic is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_set_samp_rate(uint32_t samp_rate);

/**
 * @brief     Get the audio dmic fifo address
 *
 * @param
 *    - dmic_fifo_addr: dmic fifo address
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_get_fifo_addr(uint32_t *dmic_fifo_addr);

/**
 * @brief     Get dmic data
 *
 * This API get dmic fifo data
 *
 * @param dmic_data save audio dmic fifo data
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DMIC_NOT_INIT: audio dmic is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_get_fifo_data(uint32_t *dmic_data);

/**
 * @brief   Get the dmic status information
 *
 * This API get the dmic fifo status.
 *
 * @param
 *    - dmic_status: dmic fifo status
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DMIC_NOT_INIT: audio dmic is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_get_status(uint32_t *dmic_status);

/**
 * @brief     Enable audio dmic loop test
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DMIC_NOT_INIT: audio dmic is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_start_loop_test(void);

/**
 * @brief     Disable audio dmic loop test
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DMIC_NOT_INIT: audio dmic is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_stop_loop_test(void);

/**
 * @brief     Enable dmic interrupt
 *
 * This API enable dmic interrupt:
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DMIC_NOT_INIT: audio dmic is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_enable_int(void);

/**
 * @brief     Disable dmic interrupt
 *
 * This API disable dmic interrupt:
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DMIC_NOT_INIT: audio dmic is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_disable_int(void);

/**
 * @brief     Start dmic
 *
 * This API start dmic function.
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DMIC_NOT_INIT: audio dmic is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_start(void);

/**
 * @brief     Stop dmic
 *
 * This API stop dmic function.
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DMIC_NOT_INIT: audio dmic is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_stop(void);

/**
 * @brief     Set audio dmic data write threshold
 *
 * This API set audio dmic data write threshold:
 *          - audio dmic interrupt will trigger when the amount of data in FIFO reaches the threshold
 *
 * @param value the value of write threshold
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DMIC_NOT_INIT: audio dmic is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_set_dmic_wr_threshold(uint32_t value);

/**
 * @brief     Register audio dmic isr
 *
 * @param isr audio isr callback
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DMIC_NOT_INIT: audio dmic is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dmic_register_isr(aud_isr_t isr);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif
