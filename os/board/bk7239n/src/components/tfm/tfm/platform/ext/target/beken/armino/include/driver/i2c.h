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

#include <stdbool.h>
#include <driver/i2c_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the I2C driver
 *
 * This API init the resoure common:
 *   - Init I2C driver control memory
 *
 * @attention 1. This API should be called before any other I2C APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_i2c_driver_init(void);

/**
 * @brief     Deinit the I2C driver
 *
 * This API free all resource related to I2C and disable I2C.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_i2c_driver_deinit(void);

/**
 * @brief     Init the I2C id
 *
 * This API init the I2C id:
 *  - Power up the I2C id
 *  - Configure the I2C id clock
 *  - Map the I2C id to dedicated GPIO port
 *  - Set the I2C parameters
 *  - Start the I2C id
 *
 * @param id I2C id
 * @param config I2C parameter settings

 * @attention Multifunction GPIO initialization affects other functions

 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: I2C config paramter is NULL
 *    - BK_ERR_I2C_NOT_INIT: I2C driver not init
 *    - others: other errors.
 */
bk_err_t bk_i2c_init(i2c_id_t id, const i2c_config_t *cfg);

/**
 * @brief     Deinit the I2C id
 *
 * This API deinit the I2C id:
 *   - Stop the I2C id
 *   - Disable the I2C id interrupt
 *   - Power down the I2C id
 *
 * @param id I2C id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_i2c_deinit(i2c_id_t id);

/**
 * @brief     Write data to the I2C port from a given buffer and length,
 *            It shall only be called in I2C master mode.
 *
 * @param id I2C id
 * @param dev_addr slave device address
 * @param data pointer to the buffer
 * @param size data length to write
 * @param timeout_ms timeout ms
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_I2C_NOT_INIT: I2C driver not init
 *    - BK_ERR_I2C_INVALID_ID: I2C id number is invalid
 *    - BK_ERR_I2C_ID_NOT_INIT: I2C id not init
 *    - others: other errors.
 */
bk_err_t bk_i2c_master_write(i2c_id_t id, uint32_t dev_addr, const uint8_t *data, uint32_t size, uint32_t timeout_ms);

/**
 * @brief     I2C read data from I2C buffer,
 *            It shall only be called in I2C master mode.
 *
 * @param id I2C id
 * @param dev_addr slave device address
 * @param data pointer to the buffer
 * @param size data length to read
 * @param timeout_ms timeout ms
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_I2C_NOT_INIT: I2C driver not init
 *    - BK_ERR_I2C_INVALID_ID: I2C id number is invalid
 *    - BK_ERR_I2C_ID_NOT_INIT: I2C id not init
 *    - others: other errors.
 */
bk_err_t bk_i2c_master_read(i2c_id_t id, uint32_t dev_addr, uint8_t *data, uint32_t size, uint32_t timeout_ms);

/**
 * @brief     Write data to the I2C port from a given buffer and length,
 *            It shall only be called in I2C slave mode.
 *
 * @param id I2C id
 * @param data pointer to the buffer
 * @param size data length to write
 * @param timeout_ms timeout ms
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_I2C_NOT_INIT: I2C driver not init
 *    - BK_ERR_I2C_INVALID_ID: I2C id number is invalid
 *    - BK_ERR_I2C_ID_NOT_INIT: I2C id not init
 *    - others: other errors.
 */
bk_err_t bk_i2c_slave_write(i2c_id_t id, const uint8_t *data, uint32_t size, uint32_t timeout_ms);

/**
 * @brief     I2C read data from I2C buffer,
 *            It shall only be called in I2C slave mode.
 *
 * @param id I2C id
 * @param dev_addr slave device address
 * @param data pointer to the buffer
 * @param size data length to read
 * @param timeout_ms timeout ms
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_I2C_NOT_INIT: I2C driver not init
 *    - BK_ERR_I2C_INVALID_ID: I2C id number is invalid
 *    - BK_ERR_I2C_ID_NOT_INIT: I2C id not init
 *    - others: other errors.
 */
bk_err_t bk_i2c_slave_read(i2c_id_t id, uint8_t *data, uint32_t size, uint32_t timeout_ms);

/**
 * @brief     Write data to the specific memory address from a given buffer and length,
 *            It shall only be called in I2C master mode.
 *
 * @param id I2C id
 * @param mem_param memory parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: I2C mem_param is NULL
 *    - BK_ERR_I2C_NOT_INIT: I2C driver not init
 *    - BK_ERR_I2C_INVALID_ID: I2C id number is invalid
 *    - BK_ERR_I2C_ID_NOT_INIT: I2C id not init
 *    - others: other errors.
 */
bk_err_t bk_i2c_memory_write(i2c_id_t id, const i2c_mem_param_t *mem_param);

/**
 * @brief     I2C read data from I2C specific memory address,
 *            It shall only be called in I2C master mode.
 *
 * @param id I2C id
 * @param mem_param memory parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: I2C mem_param is NULL
 *    - BK_ERR_I2C_NOT_INIT: I2C driver not init
 *    - BK_ERR_I2C_INVALID_ID: I2C id number is invalid
 *    - BK_ERR_I2C_ID_NOT_INIT: I2C id not init
 *    - others: other errors.
 */
bk_err_t bk_i2c_memory_read(i2c_id_t id, const i2c_mem_param_t *mem_param);

/**
 * @brief     I2C set baud rate
 *
 * @param id I2C id
 * @param baud_rate baud rate
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_I2C_NOT_INIT: I2C driver not init
 *    - BK_ERR_I2C_INVALID_ID: I2C id number is invalid
 *    - BK_ERR_I2C_ID_NOT_INIT: I2C id not init
 *    - others: other errors.
 */
bk_err_t bk_i2c_set_baud_rate(i2c_id_t id, uint32_t baud_rate);

/**
 * @brief     Set slave address when current role is i2c slave
 *
 * @param id I2C id
 * @param slave_addr slave address
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_I2C_NOT_INIT: I2C driver not init
 *    - others: other errors.
 */
bk_err_t bk_i2c_set_slave_address(i2c_id_t id, uint16_t slave_addr);

/**
 * @brief    Enable I2C interrupt
 *
 * @param id I2C id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_i2c_enable_interrupt(i2c_id_t id);

/**
 * @brief    Disable I2C interrupt
 *
 * @param id I2C id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_i2c_disable_interrupt(i2c_id_t id);

/**
 * @brief     Check if I2C is busy
 *
 * @param id I2C id
 *
 * @return true: busy, false: not busy
 */
bool bk_i2c_is_bus_busy(i2c_id_t id);

/**
 * @brief     Get i2c current action, such as start,stop,send write_addr
 *
 * @param id I2C id
 *
 * @return i2c action
 */
uint32_t bk_i2c_get_cur_action(i2c_id_t id);

/**
 * @brief     bk_i2c_timer_callback
 *
 * This API set timer call back
 *
 * @return
*/
void bk_i2c_timer_callback(int id, void* myTimer);

/**
 * @brief	  bk_i2c_get_busstate
 *
 * This API get bus status idle or busy
 *
 * @return
 *	  - 1: idle
 *	  - 0: busy.
 */
uint8_t bk_i2c_get_busstate ( int id );

/**
 * @brief	  bk_i2c_get_transstate
 *
 * This API get bus status idle or busy
 *
 * @return
 *	  - 1: ok
 *	  - 0: fail.
 */
uint8_t bk_i2c_get_transstate ( int id );

#ifdef __cplusplus
}
#endif

