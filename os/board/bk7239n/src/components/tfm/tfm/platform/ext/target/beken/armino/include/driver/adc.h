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

#include <common/bk_include.h>
#include <driver/adc_types.h>
#include <driver/hal/hal_adc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief ADC API
 * @defgroup bk_api_adc ADC API group
 * @{
 */

/**
 * @brief     Init the ADC driver
 *
 * This API init the resoure common to all ADC  id:
 *   - Init ADC driver control memory
 *
 * @attention 1. This API should be called before any other ADC APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_adc_driver_init(void);

/**
 * @brief     lock the runing ADC channel
 *
 * This API lock ADC channel which is runing.
 *
 * @param id ADC id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_adc_acquire(void);

/**
 * @brief     release the runing ADC channel
 *
 * This API release ADC channel which is runing.
 *
 * @param id ADC id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_adc_release();

/**
 * @brief     Deinit the ADC driver
 *
 * This API free all resource related to ADC and power down all ADC.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_adc_driver_deinit(void);

/**
 * @brief     Init the ADC
 *
 * This API init the ADC:
 *  - Power up the ADC
 *  - Map the ADC id to dedicated GPIO port
 *  - Set adc work channel
 *
 * @param id ADC channel
 * @param size ADC recieve data size
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_ADC_NOT_INIT: ADC driver not init
 *    - BK_ERR_ADC_INVALID_ID: ADC id is invalid
 *    - others: other errors.
 */
bk_err_t bk_adc_init(adc_chan_t adc_id);

/**
 * @brief     Deinit the ADC id
 *
 * This API deinit the ADC id:
 *   - Stop the ADC
 *   - Disable the ADC id interrupt
 *   - Power down the ADC
 *   - Unmap gpio id
 *
 * @param id ADC id
 *
 * @attention 1. This API should be called before use adc.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_ADC_NOT_INIT: ADC driver not init
 *    - BK_ERR_ADC_INVALID_ID: ADC id is invalid
 *    - others: other errors.
 */
bk_err_t bk_adc_deinit(adc_chan_t id);

/**
 * @brief     Start the ADC
 *
 * This API start adc:
 *   - enable the ADC id interrupt
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_ADC_BUSY: ADC is busy
 *    - others: other errors.
 */
bk_err_t bk_adc_start(void);

/**
 * @brief     Stop the ADC
 *
 * This API stop adc:
 *   - disable the ADC en bit
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_ADC_BUSY: ADC is busy
 *    - others: other errors.
 */
bk_err_t bk_adc_stop(void);

/**
 * @brief     Start read the raw ADC data
 *
 * @attention 1. The read_buf is in unit of uint16_t, the application needs to
 *               malloc (size * 2) bytes for read_buf.
 * @attention 2. The maximum value of size is CONFIG_ADC_BUF_SIZE, if the size
 *               exceeds CONFIG_ADC_BUF_SIZE, the API returns BK_ERR_ADC_SIZE_TOO_BIG.
 *
 * @param read_buf application malloc buffer which save the current adc value
 * @param size the size of read_buf, the unit of size is uint16_t
 * @param timeout adc read semaphore timeout
 *
 * @return
 *    - BK_ERR_ADC_BUSY: ADC is busy
 *    - BK_ERR_ADC_INVALID_MODE: ADC mode is invalid
 *    - BK_ERR_ADC_SIZE_TOO_BIG: size is too big
 *    - others: other errors.
 */
bk_err_t bk_adc_read_raw(uint16_t* read_buf, uint32_t size, uint32_t timeout);

/**
 * @brief     Start read the average value of sampling data
 *
 * @param data store the average value of all sample values in ADC software buffers
 * @param timeout adc read semaphore timeout
 *
 * @return

 *    - BK_ERR_ADC_BUSY: ADC is busy
 *    - BK_ERR_ADC_INVALID_MODE: ADC mode is invalid
 *    - others: other errors.
 */
bk_err_t bk_adc_read(uint16_t* data, uint32_t timeout);

/**
 * @brief     Register the adc interrupt service routine
 *
 * @param isr ADC intterrupt callback
 * @param param ADC sample data size which depend on user
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_ADC_NOT_INIT: adc not init
 *    - others: other errors.
 */
bk_err_t bk_adc_register_isr(adc_isr_t isr, uint32_t param);

/**
 * @brief     set ADC source clock and work frequence
 *
 * This API set adc clock source and frequence
 *
 * @param src_clk adc source clock
 * @param clk adc work clock :adc clk
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_ADC_INVALID_SCLK_MODE: ADC source clock is invalid
 *    - others: other errors.
 */
bk_err_t bk_adc_set_clk(adc_src_clk_t src_clk, uint32_t clk);

/**
 * @brief     set ADC work channel
 *
 * This API set adc work channel
 *
 * @param adc_id adc channel
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_ADC_INVALID_ID: ADC invalid id
 *    - others: other errors.
 */
bk_err_t bk_adc_set_channel(adc_chan_t adc_id);

/**
 * @brief     set ADC sample rate
 *
 * This API set adc work sample rate, period = (16 + sample_rate)*adc_clk
 * mcu output 16 adc_clks every period

 *
 * @param sample_rate adc sample rate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_adc_set_sample_rate(uint32_t sample_rate);

/**
 * @brief     set ADC filter
 *
 * This API set adc filter, the value default < 64,
 * data output rate = period/(filter + 1)
 *
 * @param filter adc filter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_adc_set_filter(uint32_t filter);

/**
 * @brief     set ADC steady time
 *
 * This API set adc filter, adc wait (steady_ctrl +1)*8 adc_clks to send valid
 *
 * @param steady_ctrl adc steady time
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_adc_set_steady_time(uint32_t steady_ctrl);

/**
 * @brief     set ADC sample cnt
 *
 * This API sets ADC sample cnt per-interrupt, the ADC raises the interrupt
 * when every time it samples number of cnt times. The maximum value of
 * cnt is 32.
 *
 * @param cnt per-interrupt sample cnt
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_adc_set_sample_cnt(uint32_t cnt);


/**
 * @brief     set adc saturate
 *
 * This API set  adc saturate, auto process adc fifo data in cpu ,just get the data from reg ADC_DATA_16
 *
 * @param mode adc saturate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_adc_set_saturate_mode(adc_saturate_mode_t mode);

/**
 * @brief     get adc saturate
 *
 * This API get  adc saturate
 *
 * @return
 *    - adc_saturate_mode_t: adc saturate mode
 */
bk_err_t bk_adc_get_saturate(adc_saturate_mode_t *mode);

/**
 * @brief     enable ADC bypass calibration
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_adc_enable_bypass_clalibration(void);

/**
 * @brief     disable ADC bypass calibration
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_adc_disable_bypass_clalibration(void);


/**
 * @brief     adc set config
 *
 * This API config adc params
 *
 * @param config adc parameter settings

 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM : param is null
 *    - BK_ERR_ADC_INVALID_MODE:adc invalid mode
 *    - BK_ERR_ADC_INVALID_SCLK_MODE: ADC source clock is invalid
 *    - BK_ERR_ADC_INVALID_ID: ADC id is invalid
 *    - others: other errors.
 */
bk_err_t bk_adc_set_config(adc_config_t *config);

/**
 * @brief     adc set mode
 *
 * This API config adc mode
 *
 * @param mode : adc mode:
 *    - 0: sleep mode: adc is power down in this mode
 *    - 1: single step mode: adc finish sample data convesion once after enable adc
 *  and wait mcu to read sample data. The mode will turn to sleep mode after mcu read this sample date
 *  It must be enabled again when you want to sample next data.
 *    - 2: software control mode: adc gengerate interrupt after sample data convesion
 *  wait mcu to read sample data. adc will convert sample data after mcu reading this sample date from data register
 *  and software cleaning int flag.
 *    - 3: continous mode:adc wait adc_setting time to sample data, and generate int flag after
 *  finish sample data convesion, then always repeat last sample way. This mode don't depend on muc reading data register.
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_ADC_INVALID_MODE:adc invalid mode
 *    - others: other errors.
 */
bk_err_t bk_adc_set_mode(adc_mode_t adc_mode);

/**
 * @brief     get adc mode
 *
 * This API get  adc work mode
 *
 *
 * @return
 *    - adc_mode_t: adc_mode
 */
adc_mode_t bk_adc_get_mode(void);

/**
 * @brief     only use for saradc
 */
#if SARADC_AUTOTEST
int saradc_hal_is_fifo_empty(void);
void saradc_hal_start_enable(void);
bk_err_t bk_saradc_start_int_disable(void);
bk_err_t bk_saradc_set_config(adc_config_t *config, uint32_t div);
uint32_t bk_saradc_read_raw_data(uint32_t timeout);
#endif
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

