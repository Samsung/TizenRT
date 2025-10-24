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
#include <driver/sdmadc_types.h>
#include <driver/hal/hal_sdmadc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the SDMADC driver
 *
 * This API init the resoure common to all SDMADC  id:
 *   - Init SDMADC driver control memory
 *
 * @attention 1. This API should be called before any other ADC APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NO_MEM: lack of memeory.
 *    - others: other errors.
 */
bk_err_t bk_sdmadc_driver_init(void);
/**
 * @brief     Deinit the SDMADC driver
 *
 * This API free all resource related to ADC and power down all ADC.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */

bk_err_t bk_sdmadc_driver_deinit(void);
/**
 * @brief     Init the SDMADC
 *
 * This API init the SDMADC:
 *  - Set Interrupt
 *  - Set analog register
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_FAIL: sdmadc deiver has not inited.
 */

bk_err_t bk_sdmadc_init(void);
/**
 * @brief     Deinit the SDMADC
 *
 * This API deinit the SDMADC:
 *   - Disable the SDMADC interrupt
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */

bk_err_t bk_sdmadc_deinit(void);
/**
 * @brief     Set channel of SDMADC
 *
 * This API set the SDMADC channel
 *
 * @param SDMADC channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sdmadc_set_channel(uint32_t channel);
/**
 * @brief     Set mode of SDMADC
 *
 * This API set the SDMADC mode:
 *   - Single Mode or Continsous Mode
 *
 * @param SDMADC mode
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sdmadc_set_sample_mode(sdmadc_mode_t mode);
/**
 * @brief     Set config of SDMADC
 *
 * This API set the SDMADC config
 *
 * @param SDMADC config
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sdmadc_set_cfg(sdmadc_config_t *config);
/**
 * @brief     read average value of sampling data
 *
 * This API read the result of SMDADC
 *
 * @param data store the average value
 * @param size of sample data
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sdmadc_read_average(int16_t* average, uint32_t size);
/**
 * @brief     read average value of sampling data
 *
 * This API read the result of SMDADC by default single mode config
 *
 * @param data store the average value
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sdmadc_single_read(int16_t* average, uint16_t channel);
/**
 * @brief     read average value of sampling data
 *
 * This API read the result of SMDADC by default single mode config
 *
 * @param data store the average value
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sdmadc_continuous_read(int16_t* average, uint32_t size, uint16_t channel);
/**
 * @brief     read raw value of sampling data
 *
 * This API read the raw data of sdmadc
 *
 * @param data store the raw value
 * @param size of sample data
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sdmadc_read_raw(int16_t* buf, uint32_t size);

/**
 * @brief     calculate voltage according to adc value
 *
 * This API calculate voltage by sdmadc sample value
 *
 * @param adc value
 *
 * @return
 *    - practic_voltage: calculate result
 */
float bk_sdmadc_calculate_voltage(int16_t adc_val);

#ifdef __cplusplus
}
#endif

