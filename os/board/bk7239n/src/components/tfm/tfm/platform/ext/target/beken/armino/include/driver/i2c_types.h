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

#include <driver/hal/hal_i2c_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_I2C_NOT_INIT              (BK_ERR_I2C_BASE - 1) /**< I2C driver not init */
#define BK_ERR_I2C_ID_NOT_INIT           (BK_ERR_I2C_BASE - 2) /**< I2C id not init */
#define BK_ERR_I2C_SM_BUS_BUSY           (BK_ERR_I2C_BASE - 3) /**< I2C bus busy */
#define BK_ERR_I2C_ACK_TIMEOUT           (BK_ERR_I2C_BASE - 4) /**< I2C receive ack timeout */
#define BK_ERR_I2C_CHECK_DEFCONFIG       (BK_ERR_I2C_BASE - 5) /**< I2C please check defconfig */
#define BK_ERR_I2C_SCL_TIMEOUT           (BK_ERR_I2C_BASE - 6) /**< I2C SCL timeout */

typedef enum {
	I2C_MASTER_WRITE = 0, /**< I2C master write */
	I2C_MASTER_READ,      /**< I2C master read */
	I2C_SLAVE_WRITE,      /**< I2C slave write */
	I2C_SLAVE_READ,       /**< I2C slave read */
} i2c_work_mode_t;

typedef void (*i2c_isr_t)(i2c_id_t id, void *param); /**< i2c interrupt service routine */

typedef struct {
	uint32_t dev_addr;                 /**< I2C slave device address */
	uint32_t mem_addr;                 /**< I2C slave device memerory address */
	i2c_mem_addr_size_t mem_addr_size; /**< I2C slave device memerory address size */
	uint8_t *data;                     /**< I2C data pointer */
	uint32_t data_size;                /**< I2C data size */
	uint32_t timeout_ms;               /**< I2C timeout ms */
} i2c_mem_param_t;

#ifdef __cplusplus
}
#endif

