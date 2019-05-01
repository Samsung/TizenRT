/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef __ARCH_XTENSA_SRC_ESP32_I2C_H
#define __ARCH_XTENSA_SRC_ESP32_I2C_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/i2c.h>

#define I2C_APB_CLK_FREQ  APB_CLK_FREQ	/*!< I2C source clock is APB clock, 80MHz */
#define I2C_FIFO_LEN   (32)		/*!< I2C hardware fifo length */
typedef enum {
	I2C_MODE_SLAVE = 0,			/*!< I2C slave mode */
	I2C_MODE_MASTER,			/*!< I2C master mode */
	I2C_MODE_MAX,
} i2c_mode_t;

typedef enum {
	I2C_MASTER_WRITE = 0,		/*!< I2C write data */
	I2C_MASTER_READ,			/*!< I2C read data */
} i2c_rw_t;

typedef enum {
	I2C_DATA_MODE_MSB_FIRST = 0,	/*!< I2C data msb first */
	I2C_DATA_MODE_LSB_FIRST = 1,	/*!< I2C data lsb first */
	I2C_DATA_MODE_MAX
} i2c_trans_mode_t;

typedef enum {
	I2C_CMD_RESTART = 0,		/*!<I2C restart command */
	I2C_CMD_WRITE,				/*!<I2C write command */
	I2C_CMD_READ,				/*!<I2C read command */
	I2C_CMD_STOP,				/*!<I2C stop command */
	I2C_CMD_END					/*!<I2C end command */
} i2c_opmode_t;

typedef enum {
	I2C_NUM_0 = 0,				/*!< I2C port 0 */
	I2C_NUM_1,					/*!< I2C port 1 */
	I2C_NUM_MAX
} i2c_port_t;

typedef enum {
	I2C_ADDR_BIT_7 = 0,			/*!< I2C 7bit address for slave mode */
	I2C_ADDR_BIT_10,			/*!< I2C 10bit address for slave mode */
	I2C_ADDR_BIT_MAX,
} i2c_addr_mode_t;

typedef enum {
	I2C_MASTER_ACK = 0x0,		/*!< I2C ack for each byte read */
	I2C_MASTER_NACK = 0x1,		/*!< I2C nack for each byte read */
	I2C_MASTER_LAST_NACK = 0x2,	/*!< I2C nack for the last byte */
	I2C_MASTER_ACK_MAX,
} i2c_ack_type_t;

/**
 * @brief I2C initialization parameters
 */
typedef struct {
	uint8_t periph;				/* I2C peripheral ID */

	i2c_mode_t mode;			/*!< I2C mode */

	uint8_t irq_num;

	uint8_t sda_pin;			/*!< GPIO number for I2C sda signal */
	uint8_t sda_pullup_en;		/*!< Internal GPIO pull mode for I2C sda signal */
	uint8_t sda_out_sig;
	uint8_t sda_in_sig;

	uint8_t scl_pin;			/*!< GPIO number for I2C scl signal */
	uint8_t scl_pullup_en;		/*!< Internal GPIO pull mode for I2C scl signal */
	uint8_t scl_out_sig;
	uint8_t scl_in_sig;
} i2c_config_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
void esp32_i2c_register(int bus);

#endif							/* __ARCH_XTENSA_SRC_ESP32_I2C_H */
