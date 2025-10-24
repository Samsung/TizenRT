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
#include <driver/slcd_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief SLCD API
 * @defgroup slcd.h SLCD API group
 * @{
 */

 /**
  * @brief     Init the slcd driver
  *
  * This API init the slcd driver:
  *   - Enable the slcd clock
  *   - Config the analog register of slcd
  *   - Init the gpio pin function of slcd
  *   - Configure the parameter of slcd
  *
  *
  * @param
  *    - slcd_config: com_num, slcd_bias and slcd_rate. See the enum in slcd_types.h
  *
  * @return
  *    - None
  */
void bk_slcd_driver_init(slcd_config_t slcd_config);

/**
 * @brief     Deinit the slcd driver
 *
 * This API deinit the slcd driver:
 *   - Disable the slcd clock
 *   - Disable the slcd display
 *
 *
 * @param
 *    - None
 *
 * @return
 *    - None
 */
void bk_slcd_driver_deinit(void);

/**
 * @brief     Set the value of slcd seg
 *
 * This API set the value of slcd seg
 *
 *
 * @param
 *    - seg_id: seg00 - seg31 / seg00 - seg27
 *    - value: seg value
 *
 * @return
 *    - None
 */
void bk_slcd_set_seg_value(slcd_seg_id_t seg_id, uint8_t value);

/**
 * @brief     Enable the slcd com port
 *
 * This API enable the slcd com port
 *
 *
 * @param
 *    - com_enable: Each bit corresponds to a com enable.
 *
 * @return
 *    - None
 */
void bk_slcd_set_com_port_enable(uint8_t com_enable);

/**
 * @brief     Enable the slcd seg port
 *
 * This API enable the slcd seg port
 *
 *
 * @param
 *    - seg_enable: Each bit corresponds to a seg enable.
 *
 * @return
 *    - None
 */
void bk_slcd_set_seg_port_enable(uint32_t seg_enable);

/**
 * @brief     Set the slcd seg value from seg00 to seg03
 *
 * This API set the slcd seg value from seg00 to seg03
 *
 *
 * @param
 *    - value: Each byte corresponds to a seg id and each bit corresponds to a com id.
 *
 * @return
 *    - None
 */
void bk_slcd_set_seg00_03_value(uint32_t value);

/**
 * @brief     Set the slcd seg value from seg04 to seg07
 *
 * This API set the slcd seg value from seg04 to seg07
 *
 *
 * @param
 *    - value: Each byte corresponds to a seg id and each bit corresponds to a com id.
 *
 * @return
 *    - None
 */
void bk_slcd_set_seg04_07_value(uint32_t value);

/**
 * @brief     Set the slcd seg value from seg08 to seg11
 *
 * This API set the slcd seg value from seg08 to seg11
 *
 *
 * @param
 *    - value: Each byte corresponds to a seg id and each bit corresponds to a com id.
 *
 * @return
 *    - None
 */
void bk_slcd_set_seg08_11_value(uint32_t value);

/**
 * @brief     Set the slcd seg value from seg12 to seg15
 *
 * This API set the slcd seg value from seg12 to seg15
 *
 *
 * @param
 *    - value: Each byte corresponds to a seg id and each bit corresponds to a com id.
 *
 * @return
 *    - None
 */
void bk_slcd_set_seg12_15_value(uint32_t value);

/**
 * @brief     Set the slcd seg value from seg16 to seg19
 *
 * This API set the slcd seg value from seg16 to seg19
 *
 *
 * @param
 *    - value: Each byte corresponds to a seg id and each bit corresponds to a com id.
 *
 * @return
 *    - None
 */
void bk_slcd_set_seg16_19_value(uint32_t value);

/**
 * @brief     Set the slcd seg value from seg20 to seg23
 *
 * This API set the slcd seg value from seg20 to seg23
 *
 *
 * @param
 *    - value: Each byte corresponds to a seg id and each bit corresponds to a com id.
 *
 * @return
 *    - None
 */
void bk_slcd_set_seg20_23_value(uint32_t value);

/**
 * @brief     Set the slcd seg value from seg24 to seg27
 *
 * This API set the slcd seg value from seg24 to seg27
 *
 *
 * @param
 *    - value: Each byte corresponds to a seg id and each bit corresponds to a com id.
 *
 * @return
 *    - None
 */
void bk_slcd_set_seg24_27_value(uint32_t value);

/**
 * @brief     Set the slcd seg value from seg28 to seg31
 *
 * This API set the slcd seg value from seg28 to seg31
 *
 *
 * @param
 *    - value: Each byte corresponds to a seg id and each bit corresponds to a com id.
 *
 * @return
 *    - None
 */
void bk_slcd_set_seg28_31_value(uint32_t value);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif


