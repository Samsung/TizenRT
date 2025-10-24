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

#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_BAUD_RATE_50KHZ      50000
#define I2C_BAUD_RATE_100KHZ     100000
#define I2C_BAUD_RATE_200KHZ     200000
#define I2C_BAUD_RATE_400KHZ     400000
#define I2C_DEFAULT_BAUD_RATE    I2C_BAUD_RATE_100KHZ

typedef uint8_t i2c_unit_t; /**< i2c uint id */

typedef enum {
	I2C_ID_0 = 0, /**< i2c id 0 */
#if (SOC_I2C_UNIT_NUM > 1)
	I2C_ID_1,     /**< i2c id 1 */
#endif
#if (SOC_I2C_UNIT_NUM > 2)
	I2C_ID_2,     /**< i2c id 2 */
#endif
	I2C_ID_MAX    /**< i2c id max */
} i2c_id_t;

typedef enum {
	I2C_ADDR_MODE_7BIT = 0, /**< i2c address mode 7bit */
	I2C_ADDR_MODE_10BIT,    /**< i2c address mode 10bit */
} i2c_addr_mode_t;

typedef enum {
	I2C_FIFO_INT_LEVEL_1 = 0, /**< i2c fifo int level 1 */
	I2C_FIFO_INT_LEVEL_4,     /**< i2c fifo int level 4 */
	I2C_FIFO_INT_LEVEL_8,     /**< i2c fifo int level 8 */
	I2C_FIFO_INT_LEVEL_12,    /**< i2c fifo int level 12 */
} i2c_fifo_int_level_t;

typedef enum {
	I2C_MEM_ADDR_SIZE_8BIT = 0, /**< i2c memory address size 8bit */
	I2C_MEM_ADDR_SIZE_16BIT,    /**< i2c memory address size 16bit */
} i2c_mem_addr_size_t;

typedef struct {
	uint32_t baud_rate;        /**< i2c clock rate */
	i2c_addr_mode_t addr_mode; /**< i2c address mode */
	uint16_t slave_addr;       /**< i2c address for slave mode */
} i2c_config_t;

#ifdef __cplusplus
}
#endif

