/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * @defgroup I2C I2C
 * @brief Provides APIs for I2C (Inter Integrated Circuit)
 * @ingroup IOTBUS
 * @{
 */

/**
 * @file iotbus_i2c.h
 * @brief Iotbus APIs for I2C
 */

#include <stdint.h>

#ifndef IOTBUS_I2C_H_
#define IOTBUS_I2C_H_

struct _iotbus_i2c_s;

/**
 * @brief Pointer definition to the internal struct _iotbus_i2c_s
 */
typedef struct _iotbus_i2c_s *iotbus_i2c_context_h;

/**
 * @brief Enumeration of I2C frequency mode
 * @details
 * Enumeration Details:\n
 * IOTBUS_I2C_STD = 0, < up to 100Khz\n
 * IOTBUS_I2C_FAST = 1, < up to 400Khz\n
 * IOTBUS_I2C_HIGH = 2, < up to 3.4Mhz\n
 */
typedef enum {
	IOTBUS_I2C_STD = 0, /**< up to 100Khz */
	IOTBUS_I2C_FAST = 1, /**< up to 400Khz */
	IOTBUS_I2C_HIGH = 2 /**< up to 3.4Mhz */
} iotbus_i2c_mode_e;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief initializes i2c_context.
 *
 * @param[in] bus i2c bus number
 * @return On success, handle of i2c_context is returned. On failure, NULL is returned.
 * @since Tizen RT v1.0
 */
iotbus_i2c_context_h iotbus_i2c_init(int bus);

/**
 * @brief closes i2c_context.
 *
 * @param[in] hnd handle of i2c_context
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since Tizen RT v1.0
 */
int iotbus_i2c_stop(iotbus_i2c_context_h hnd);

/**
 * @brief sets the i2c frequency.
 *
 * @param[in] hnd handle of i2c_context
 * @param[in] mode i2c frequency mode
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since Tizen RT v1.0
 */
int iotbus_i2c_set_frequency(iotbus_i2c_context_h hnd, iotbus_i2c_mode_e mode);

/**
 * @brief sets the i2c slave address.
 *
 * @param[in] hnd handle of i2c_context
 * @param[in] address i2c address
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since Tizen RT v1.0
 */
int iotbus_i2c_set_address(iotbus_i2c_context_h hnd, uint8_t address);

/**
 * @brief reads from i2c device.
 *
 * @param[in] hnd handle of i2c_context
 * @param[in] data the pointer of data buffer
 * @param[in] length size to read
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since Tizen RT v1.0
 */
int iotbus_i2c_read(iotbus_i2c_context_h hnd, uint8_t *data, size_t length);

/**
 * @brief writes to i2c device.
 *
 * @param[in] hnd handle of i2c_context
 * @param[in] data the pointer of data buffer
 * @param[in] length size to write
 * @return On success, size is returned. On failure, a negative value is returned.
 * @since Tizen RT v1.0
 */
int iotbus_i2c_write(iotbus_i2c_context_h hnd, const uint8_t *data, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* IOTBUS_I2C_H_ */

/** @} */ // end of I2C group
