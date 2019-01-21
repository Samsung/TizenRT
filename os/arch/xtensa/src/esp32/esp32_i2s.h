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

#ifndef __ARCH_XTENSA_SRC_ESP32_I2S_H
#define __ARCH_XTENSA_SRC_ESP32_I2S_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>
#include <tinyara/audio/i2s.h>

#include <arch/chip/irq.h>

#include <arch/chip/i2s_struct.h>
#include "chip/esp32_i2s_reg.h"

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
#ifndef GPIO_PIN_COUNT
#define GPIO_PIN_COUNT          40
#endif
#ifndef GPIO_PIN_OUT_MAX
#define GPIO_PIN_OUT_MAX        34
#endif

/************************************************************************************
 * Public Types
 ************************************************************************************/

/**
* @brief I2S bit width per sample.
*
*/
typedef enum {
	I2S_BITS_PER_SAMPLE_8BIT = 8,	/*!< I2S bits per sample: 8-bits */
	I2S_BITS_PER_SAMPLE_16BIT = 16,	/*!< I2S bits per sample: 16-bits */
	I2S_BITS_PER_SAMPLE_24BIT = 24,	/*!< I2S bits per sample: 24-bits */
	I2S_BITS_PER_SAMPLE_32BIT = 32,	/*!< I2S bits per sample: 32-bits */
} i2s_bits_per_sample_t;

/**
* @brief I2S channel.
*
*/
typedef enum {
	I2S_CHANNEL_MONO = 1,		/*!< I2S 1 channel (mono) */
	I2S_CHANNEL_STEREO = 2		/*!< I2S 2 channel (stereo) */
} i2s_channel_t;

/**
* @brief I2S communication standard format
*
*/
typedef enum {
	I2S_COMM_FORMAT_I2S = 0x01,	/*!< I2S communication format I2S */
	I2S_COMM_FORMAT_I2S_MSB = 0x02,	/*!< I2S format MSB */
	I2S_COMM_FORMAT_I2S_LSB = 0x04,	/*!< I2S format LSB */
	I2S_COMM_FORMAT_PCM = 0x08,	/*!< I2S communication format PCM */
	I2S_COMM_FORMAT_PCM_SHORT = 0x10,	/*!< PCM Short */
	I2S_COMM_FORMAT_PCM_LONG = 0x20,	/*!< PCM Long */
} i2s_comm_format_t;

/**
* @brief I2S channel format type
*/
typedef enum {
	I2S_CHANNEL_FMT_RIGHT_LEFT = 0x00,
	I2S_CHANNEL_FMT_ALL_RIGHT,
	I2S_CHANNEL_FMT_ALL_LEFT,
	I2S_CHANNEL_FMT_ONLY_RIGHT,
	I2S_CHANNEL_FMT_ONLY_LEFT,
} i2s_channel_fmt_t;

/**
* @brief I2S Peripheral, 0 & 1.
*
*/
typedef enum {
	I2S_NUM_0 = 0x0,			/*!< I2S 0 */
	I2S_NUM_1 = 0x1,			/*!< I2S 1 */
	I2S_NUM_MAX,
} i2s_port_t;

/**
* @brief I2S Mode, defaut is I2S_MODE_MASTER | I2S_MODE_TX
*
* @note PDM and built-in DAC functions are only supported on I2S0 for current ESP32 chip.
*
*/
typedef enum {
	I2S_MODE_MASTER = 1,
	I2S_MODE_SLAVE = 2,
	I2S_MODE_TX = 4,
	I2S_MODE_RX = 8,
	I2S_MODE_DAC_BUILT_IN = 16,	/*!< Output I2S data to built-in DAC, no matter the data format is 16bit or 32 bit, the DAC module will only take the 8bits from MSB */
	I2S_MODE_ADC_BUILT_IN = 32,	/*!< Input I2S data from built-in ADC, each data can be 12-bit width at most */
	I2S_MODE_PDM = 64,
} i2s_mode_t;

/************************************************************************************
 * Inline Functions
 ************************************************************************************/

/************************************************************************************
 * Public Data
 ************************************************************************************/

/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

/****************************************************************************
 * Name: esp32_i2s_initialize
 *
 * Description:
 *   Initialize the selected I2S port.
 *
 * Input Parameter:
 *   Port number (for hardware that has mutiple I2S interfaces)
 *
 * Returned Value:
 *   Valid I2S device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/

FAR struct i2s_dev_s *esp32_i2s_initialize(uint16_t port);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __ASSEMBLY__ */
#endif							/* __ARCH_XTENSA_SRC_ESP32_I2S_H */
