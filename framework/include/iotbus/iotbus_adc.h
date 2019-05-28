/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/**
 * @defgroup ADC ADC
 * @brief Provides APIs for ADC (Analog to Digital Converters)
 * @ingroup IOTBUS
 * @{
 */

/**
 * @file iotbus/iotbus_adc.h
 * @brief Iotbus APIs for ADC
 */

#ifndef IOTBUS_ADC_H_
#define IOTBUS_ADC_H_

#include <stdint.h>
#include <sys/types.h>

struct _iotbus_adc_s;

/**
 * @brief Pointer definition to the internal struct _iotbus_adc_wrapper_s
 */
typedef struct _iotbus_adc_wrapper_s *iotbus_adc_context_h;

/**
 * @brief Enumeration of ADC state
 * @details
 * Enumeration Details:
 * IOTBUS_ADC_RDY = 0, adc device is ready
 * IOTBUS_ADC_BUSY = 1, adc device is busy
 */
typedef enum {
	IOTBUS_ADC_NONE = 0,
	IOTBUS_ADC_RDY, /** adc device is ready to use */
	IOTBUS_ADC_BUSY, /** adc device is busy */
	IOTBUS_ADC_STOP, /** adc device is stopped */
} iotbus_adc_state_e;

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*adc_read_cb)(int channel, uint32_t data);

/**
 * @brief initializes adc_context.
 *
 * @details @b #include <iotbus/iotbus_adc.h>
 * @param[in] bus ADC bus number
 * @param[in] channel The 8-bit ADC Channel
 * @return On success, handle of adc_context is returned. On failure, NULL is returned.
 * @since TizenRT v2.0
 */
iotbus_adc_context_h iotbus_adc_init(int bus, uint8_t channel);

/**
 * @brief deinitializes adc_context.
 *
 * @details @b #include <iotbus/iotbus_adc.h>
 * @param[in] hnd handle of i2c_context
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v2.0
 */
int iotbus_adc_deinit(iotbus_adc_context_h hnd);

/**
 * @brief set the ADC channel.
 *
 * @details @b #include <iotbus/iotbus_adc.h>
 * @param[in] hnd handle of adc_context
 * @param[in] channel The 8-bit ADC Channel
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v2.0
 */
int iotbus_adc_set_channel(iotbus_adc_context_h hnd, uint8_t channel);

/**
 * @brief get the ADC channel.
 *
 * @details @b #include <iotbus/iotbus_adc.h>
 * @param[in] hnd handle of adc_context
 * @return On success, the ADC channel is returned. On failure, a negative value is returned.
 * @since TizenRT v2.0
 */
int iotbus_adc_get_channel(iotbus_adc_context_h hnd);

/**
 * @brief get the ADC state.
 *
 * @details @b #include <iotbus/iotbus_adc.h>
 * @param[in] hnd handle of adc_context
 * @return On success, IOTBUS_ADC_RDY or IOTBUS_ADC_BUSY is returned. On failure, a negative value is returned.
 * @since TizenRT v2.0
 */
int iotbus_adc_get_state(iotbus_adc_context_h hnd);

/**
 * @brief start adc asynchronous read.
 *
 * @details @b #include <iotbus/iotbus_adc.h>
 * @param[in] hnd handle of adc_context
 * @param[in] read_cb callback function called adc read is done.
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v2.0
 */
int iotbus_adc_start(iotbus_adc_context_h hnd, const adc_read_cb read_cb);

/**
 * @brief stop adc asynchronous read.
 *
 * @details @b #include <iotbus/iotbus_adc.h>
 * @param[in] hnd handle of adc_context
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v2.0
 */
int iotbus_adc_stop(iotbus_adc_context_h hnd);

/**
 * @brief adc synchronous read.
 *
 * @details @b #include <iotbus/iotbus_adc.h>
 * @param[in] hnd handle of adc_context
 * @param[in] timeout timeout value (ms).
 * @return On success, ADC sample value is returned. On failure, a negative value is returned.
 * @since TizenRT v2.0
 */
uint32_t iotbus_adc_get_sample(iotbus_adc_context_h hnd, int timeout);

#ifdef __cplusplus
}
#endif

#endif /* IOTBUS_ADC_H_ */

/** @} */ // end of ADC group
