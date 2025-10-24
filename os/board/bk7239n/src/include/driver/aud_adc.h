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
#include <driver/aud_common.h>

#include <driver/aud_adc_types.h>

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
 * @brief     Init the adc module of audio
 *
 * This API init the adc module:
 *  - Init audio driver
 *  - Set audio adc sample rate, work mode...
 *  - Configure mic
 *
 * @param adc_config audio adc config
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: config is NULL
 *    - BK_ERR_AUD_DRV_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_init(aud_adc_config_t *adc_config);

/**
 * @brief     Deinit adc module
 *
 * This API deinit the adc module of audio:
 *   - Disable adc and mic
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_deinit(void);

/**
 * @brief     Set the sample rate in adc work mode
 *
 * @param samp_rate adc sample rate of adc work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_set_samp_rate(uint32_t samp_rate);

/**
 * @brief     Set the adc gain
 *
 * @param value the gain value(0x0 ~ 0x3f)
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_set_gain(uint32_t value);

/**
 * @brief     Set the adc channel
 *
 * @param chl the mic channel
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_set_chl(aud_adc_chl_t chl);

/**
 * @brief     Set the mic external interface mode
 *
 * @param mic_id the mic id
 * @param intf_mode single end or difference
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_set_mic_mode(aud_mic_id_t mic_id, aud_adc_mode_t mode);

/**
 * @brief     Get the adc fifo address
 *
 * @param adc_fifo_addr adc fifo address
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_get_fifo_addr(uint32_t *adc_fifo_addr);

/**
 * @brief     Get the audio adc fifo status information
 *
 * This API get the audio adc status:
 *   - Get fifo status
 *
 * @param adc_status adc fifo status and agc status
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_get_status(uint32_t *adc_status);

/**
 * @brief     Enable adc interrupt
 *
 * This API enable adc interrupt:
 *   - Enable audio adc interrupt
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_enable_int(void);

/**
 * @brief     Disable adc interrupt
 *
 * This API disable adc interrupt:
 *   - Disable adc interrupt if work mode is adc work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_disable_int(void);

/**
 * @brief     Start adc
 *
 * This API start adc:
 *   - Enable adc if work mode is adc work mode
 *   - Enable dtmf if work mode is dtmf work mode
 *
 * Usage example:
 *
 *     //init audio adc module
 *     aud_adc_config_t adc_config = DEFAULT_AUD_ADC_CONFIG();
 *     bk_aud_adc_init(&adc_config);
 *     CLI_LOGI("init adc successful\n");
 *
 *     //start adc and dac
 *     bk_aud_start_adc();
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_start(void);

/**
 * @brief     Stop audio adc
 *
 * This API stop adc:
 *   - Disable audio adc
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_stop(void);

/**
 * @brief     Get adc data
 *
 * This API get adc fifo data
 *
 * @param adc_data save audio adc fifo data
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_get_fifo_data(uint32_t *adc_data);

/**
 * @brief     Set audio adc data write threshold
 *
 * This API set audio adc data write threshold:
 *          - audio adc interrupt will trigger when the amount of data in FIFO reaches the threshold
 *
 * @param value the value of write threshold
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_set_adcl_wr_threshold(uint32_t value);

/**
 * @brief     Register audio adc isr
 *
 * @param isr audio isr callback
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_register_isr(aud_isr_t isr);

/**
 * @brief     Config audio adc HPF(High Pass Filter)
 *
 * @param config the audio adc HPF parameters
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_hpf_config(aud_adc_hpf_config_t *config);

/**
 * @brief     Config audio adc AGC(Automatic Gain Control)
 *
 * @param config the audio adc AGC parameters
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_agc_config(aud_adc_agc_config_t *config);

/**
 * @brief     Enable audio adc loop test
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_start_loop_test(void);

/**
 * @brief     Disable audio adc loop test
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_NOT_INIT: audio adc is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_stop_loop_test(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
