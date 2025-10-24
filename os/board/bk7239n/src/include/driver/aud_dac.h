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
#include <driver/aud_dac_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */


/**
 * @brief     Init the dac module of audio
 *
 * This API init the dac module:
 *  - Configure the dac parameters to enable dac function.
 *
 * @param
 *    - dac_config: dac parameters configure
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: config is NULL
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_init(aud_dac_config_t *dac_config);

/**
 * @brief     Deinit dac module of audio
 *
 * This API deinit the dac module:
 *   - Configure the dac parameters to default value.
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_deinit(void);

/**
 * @brief     Set the dac sample rate
 *
 * This API set the dac sample rate value.
 *
 * @param
 *    - samp_rate: dac sample rate
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_set_samp_rate(uint32_t samp_rate);

/**
 * @brief     Set the dac gain
 *
 * @param value the gain value, range:0x00 ~ 0x3f
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_set_gain(uint32_t value);

/**
 * @brief     Set the dac analog gain
 *
 * @param value the gain value, range:0x00 ~ 0x0f
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_set_ana_gain(uint8_t value);

/**
 * @brief     Get the dac analog gain
 *
 * @param value the gain value
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_get_ana_gain(uint32_t *gain);

/**
 * @brief     Mute audio dac
 *
 * @param
 *    - none
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_mute(void);

/**
 * @brief     Unmute audio dac
 *
 * @param
 *    - none
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_unmute(void);

/**
 * @brief     Set the dac channel
 *
 * @param dac_chl the channel value
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_set_chl(aud_dac_chl_t dac_chl);

/**
 * @brief     Enable dac interrupt
 *
 * This API enable dac interrupt:
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_enable_int(void);

/**
 * @brief     Disable dac interrupt
 *
 * This API disable dac interrupt:
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_disable_int(void);

/**
 * @brief     Get the audio dac fifo address
 *
 * @param
 *    - dac_fifo_addr: dac fifo address
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_get_fifo_addr(uint32_t *dac_fifo_addr);

/**
* @brief   Get the dac status information
*
* This API get the dac fifo status.
*
* @param
*    - dac_status: dac fifo status
*
* @return
*    - BK_OK: succeed
*    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
*    - others: other errors.
*/
bk_err_t bk_aud_dac_get_status(uint32_t *dac_status);

/**
 * @brief     Start dac
 *
 * This API start dac function.
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_start(void);

/**
 * @brief     Stop dac
 *
 * This API stop dac function.
 *
 * @param
 *    - None
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_stop(void);

/**
 * @brief     Write data to dac
 *
 * This API write pcm data to audio dac fifo.
 *
 * @param
 *    - pcm_value: audio data
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_write(uint32_t pcm_value);

/**
 * @brief     Init the eq module of audio
 *
 * This API init the eq module:
 *  - Configure the eq parameters to enable the eq function.
 *
 * @param
 *    - eq_config: eq parameter configure
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: config is NULL
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_eq_config(aud_dac_eq_config_t *config);

/**
 * @brief     Deinit the eq module of audio
 *
 * This API deinit the eq module:
 *  - Configure the eq parameters to default value.
 *
 * @param
 *    - none
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_eq_deconfig(void);

/**
 * @brief     Register audio dac isr
 *
 * @param isr_id AUD_ISR_DACR or AUD_ISR_DACL
 * @param isr audio isr callback
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_register_isr(aud_isr_id_t isr_id, aud_isr_t isr);

/**
 * @brief     Set bypass audio dac DWA(Device Wire Adapter)
 *
 * This API set bypass audio dac DWA:
 *  - 1: bypass audio dac DWA, audio dac analog not work.
 *
 * @param
 *    - value: 0: audio dac analog not work
 *             1: audio dac analog work
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_DAC_NOT_INIT: audio dac is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_set_dwa_bypass(uint8_t value);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
