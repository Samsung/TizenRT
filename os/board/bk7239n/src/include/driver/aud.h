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
#include <driver/aud_types.h>

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
 * @brief     Init the AUD driver
 *
 * This API init the resoure common:
 *   - Init AUD driver control memory
 *   - Configure clock and power
 *   - Configure mic enable
 *   - Register AUD isr interrupt
 *
 * This API should be called before any other AUD APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_driver_init(void);

/**
 * @brief     Deinit the AUD driver
 *
 * This API free all resource related to AUD, power down AUD and mic.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_driver_deinit(void);

/**
 * @brief     Init the adc module of audio
 *
 * This API init the adc module:
 *  - Set adc work mode: adc/dtmf
 *  - Configure the adc/dtmf parameters
 *  - disable adc/dtmf
 *  - disable adc/dtmf interrupts
 *
 * @param adc_work_mode adc work mode adc/dtmf
 * @param adc_config adc configure of adc work mode
 * @param dtmf_config dtmf configure of dtmf work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_MODE: adc work mode is error
 *    - BK_ERR_NULL_PARAM: config is NULL
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_adc_init(aud_adc_work_mode_t adc_work_mode, const aud_adc_config_t *adc_config, const aud_dtmf_config_t *dtmf_config);

/**
 * @brief     Deinit adc module
 *
 * This API deinit the adc module of audio:
 *   - Disable adc and dtmf
 *
 * @return
 *    - BK_OK: succeed
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
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_set_adc_samp_rate(aud_adc_samp_rate_t samp_rate);

/**
 * @brief     Set the adc gain in adc work mode
 *
 * @param value the gain value of adc work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_set_adc_gain(uint32_t value);

/**
 * @brief     Set the adc gain in adc work mode
 *
 * @param mic_chl the mic channel value of adc work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_set_mic_chl(aud_mic_enable_t mic_chl);

/**
 * @brief     Set the mic external interface mode in adc work mode
 *
 * @param mic_id the mic id
 * @param intf_mode signal end or difference
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_set_mic_intf_mode(aud_mic_id_t mic_id, aud_adc_intf_mode_t intf_mode);

/**
 * @brief     Get the adc fifo address in adc work mode
 *
 * @param adc_fifo_addr adc fifo address of adc work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_get_adc_fifo_addr(uint32_t *adc_fifo_addr);

/**
 * @brief     Get the dtmf fifo address in adc work mode
 *
 * @param dtmf_fifo_addr dtmf fifo address of dtmf work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_get_dtmf_fifo_addr(uint32_t *dtmf_fifo_addr);

/**
 * @brief     Get the dac fifo address
 *
 * @param
 *    - dac_fifo_addr: dac fifo address
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_get_dac_fifo_addr(uint32_t *dac_fifo_addr);

/**
 * @brief     Get the adc status information in adc work mode
 *
 * This API get the adc status of adc work mode:
 *   - Get fifo status
 *
 * @param adc_status adc fifo status and agc status
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_get_adc_status(uint32_t *adc_status);

/**
 * @brief     Get the dtmf status information in dtmf work mode
 *
 * This API get the adc status of dtmf work mode:
 *   - Get fifo status
 *
 * @param dtmf_status dtmf fifo status
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_get_dtmf_status(uint32_t *dtmf_status);

/**
 * @brief     Enable adc interrupt
 *
 * This API enable adc interrupt:
 *   - Enable adc interrupt if work mode is adc work mode
 *   - Enable dtmf interrupt if work mode is dtmf work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_enable_adc_int(void);

/**
 * @brief     Disable adc interrupt
 *
 * This API disable adc interrupt:
 *   - Disable adc interrupt if work mode is adc work mode
 *   - Disable dtmf interrupt if work mode is dtmf work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_disable_adc_int(void);

/**
 * @brief     Start adc
 *
 * This API start adc:
 *   - Enable adc if work mode is adc work mode
 *   - Enable dtmf if work mode is dtmf work mode
 *
 * Usage example:
 *
 *     //init audio driver
 *     bk_aud_driver_init();
 *
 *     //init adc configuration
 *     aud_adc_config_t adc_config;
 *     adc_config.samp_rate = AUD_ADC_SAMP_RATE_8K;
 *     adc_config.adc_enable = AUD_ADC_DISABLE;
 *     adc_config.line_enable = AUD_ADC_LINE_DISABLE;
 *     adc_config.dtmf_enable = AUD_DTMF_DISABLE;
 *     adc_config.adc_hpf2_coef_B2 = 0;
 *     adc_config.adc_hpf2_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
 *     adc_config.adc_hpf1_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
 *     adc_config.adc_set_gain = 0x2d;
 *     adc_config.adc_samp_edge = AUD_ADC_SAMP_EDGE_RISING;
 *     adc_config.adc_hpf2_coef_B0 = 0;
 *     adc_config.adc_hpf2_coef_B1 = 0;
 *     adc_config.adc_hpf2_coef_A0 = 0;
 *     adc_config.adc_hpf2_coef_A1 = 0;
 *     adc_config.dtmf_wr_threshold = 8;
 *     adc_config.adcl_wr_threshold = 8;
 *     adc_config.dtmf_int_enable = AUD_DTMF_INT_DISABLE;
 *     adc_config.adcl_int_enable = AUD_ADCL_INT_DISABLE;
 *     adc_config.loop_adc2dac = AUD_LOOP_ADC2DAC_DISABLE;
 *     adc_config.agc_noise_thrd = 101;
 *     adc_config.agc_noise_high = 101;
 *     adc_config.agc_noise_low = 160;
 *     adc_config.agc_noise_min = 1;
 *     adc_config.agc_noise_tout = 0;
 *     adc_config.agc_high_dur = 3;
 *     adc_config.agc_low_dur = 3;
 *     adc_config.agc_min = 1;
 *     adc_config.agc_max = 4;
 *     adc_config.agc_ng_method = AUD_AGC_NG_METHOD_MUTE;
 *     adc_config.agc_ng_enable = AUD_AGC_NG_DISABLE;
 *     adc_config.agc_decay_time = AUD_AGC_DECAY_TIME_128;
 *     adc_config.agc_attack_time = AUD_AGC_ATTACK_TIME_128;
 *     adc_config.agc_high_thrd = 18;
 *     adc_config.agc_low_thrd = 0;
 *     adc_config.agc_iir_coef = AUD_AGC_IIR_COEF_1_1024;
 *     adc_config.agc_enable = AUD_AGC_DISABLE;
 *     adc_config.manual_pga_value = 0;
 *     adc_config.manual_pga_enable = AUD_GAC_MANUAL_PGA_DISABLE;
 *     bk_aud_adc_init(AUD_ADC_WORK_MODE_ADC, &adc_config, NULL);
 *     CLI_LOGI("init adc successful\n");
 *
 *     //start adc and dac
 *     bk_aud_start_adc();
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_MODE: adc work mode is NULL
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_start_adc(void);

/**
 * @brief     Stop adc
 *
 * This API stop adc:
 *   - Disable adc if work mode is adc work mode
 *   - Disable dtmf if work mode is dtmf work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_MODE: adc work mode is NULL
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_stop_adc(void);

/**
 * @brief     Get adc data
 *
 * This API get adc fifo data
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_MODE: adc work mode is NULL
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_get_adc_fifo_data(uint32_t *adc_data);

/**
 * @brief     Get dtmf data
 *
 * This API get dtmf fifo data
 *
 * @param
 *    - dtmf_data: save dtmf data
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_MODE: dtmf work mode is NULL
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_get_dtmf_fifo_data(uint32_t *dtmf_data);

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
 */
bk_err_t bk_aud_dac_init(const aud_dac_config_t *dac_config);

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
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 */
bk_err_t bk_aud_set_dac_samp_rate(aud_dac_samp_rate_t samp_rate);

/**
 * @brief     Set the dac gain
 *
 * @param value the gain value, range:0x00 ~ 0x3f
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_set_dac_gain(uint32_t value);

/**
 * @brief     Set the dac channel
 *
 * @param dac_chl the channel value
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_set_dac_chl(aud_dac_chl_enable_t dac_chl);

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
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 */
bk_err_t bk_aud_enable_dac_int(void);

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
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 */
 bk_err_t bk_aud_disable_dac_int(void);

 /**
  * @brief	   Get the dac status information
  *
  * This API get the dac fifo status.
  *
  * @param
  *    - dac_status: dac fifo status
  *
  * @return
  *    - BK_OK: succeed
  *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
  *
  * Usage example:
  *
  *    uint32_t dac_fifo_status = 0;
  *	   bk_aud_get_dac_status(&dac_fifo_status);
  *    if (dac_fifo_status & AUD_DACL_NEAR_EMPTY_MASK) {
  *    }
  *
  */
 bk_err_t bk_aud_get_dac_status(uint32_t *dac_status);

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
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 */
bk_err_t bk_aud_start_dac(void);

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
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 */
bk_err_t bk_aud_stop_dac(void);

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
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 */
bk_err_t bk_aud_eq_init(const aud_eq_config_t *eq_config);

/**
 * @brief     Deinit the eq module of audio
 *
 * This API deinit the eq module:
 *  - Configure the eq parameters to default value.
 *
 * @param
 *    - eq_config: eq parameter configure
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: config is NULL
 */
bk_err_t bk_aud_eq_deinit(aud_eq_config_t *eq_config);

/**
 * @brief     Register audio isr
 *
 * This API register audio isr:
 *   - Disable adc if work mode is adc work mode
 *   - Disable dtmf if work mode is dtmf work mode
 *
 * Usage example:
 *
 *     void cli_aud_adcl_isr(void *param)
 *     {
 *         uint32_t adc_data;
 *         uint32_t adc_status;
 *
 *         bk_aud_get_adc_status(&adc_status);
 *         if (adc_status & AUD_ADCL_NEAR_FULL_MASK) {
 *             bk_aud_get_adc_fifo_data(&adc_data);
 *             bk_aud_dac_write(adc_data);
 *         }
 *     }
 *
 *     //register isr
 *     ret = bk_aud_register_aud_isr(AUD_ISR_ADCL, cli_aud_adcl_isr, NULL);
 *     CLI_LOGI("register adc isr successful\n");
 *
 *     //enable audio interrupt
 *     bk_aud_enable_adc_int();
 *     CLI_LOGI("enable adc interrupt successful\n");
 *
 * @param isr_id adc work mode adc/dtmf
 * @param isr audio isr callback
 * @param param audio isr callback parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_ADC_MODE: adc work mode is NULL
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_register_aud_isr(aud_isr_id_t isr_id, aud_isr_t isr);


bk_err_t bk_aud_dmic_init(uint32_t samp_rate);

void bk_aud_dmic_start_loop_test(void);

void bk_aud_dmic_stop_loop_test(void);

bk_err_t bk_aud_clk_config(aud_clk_t clk);

bk_err_t bk_aud_clk_deconfig(aud_clk_t clk);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
