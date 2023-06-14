/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Ameba_Mbed_API
  * @{
  */

/** @addtogroup MBED_UART
 *  @brief      MBED_UART driver modules.
 *  @{
 */

/** @addtogroup MBED_UART_Exported_Types MBED_UART Exported Types
  * @{
  */
typedef enum {
	UART_0 = (int)UART0_DEV,
	UART_1 = (int)UART1_DEV,
	UART_2 = (int)UART2_DEV,
	UART_3 = (int)UART3_DEV,
} UARTName;
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup MBED_ADC
 *  @brief      MBED_ADC driver modules.
 *  @{
 */

/** @addtogroup MBED_ADC_Exported_Types MBED_ADC Exported Types
  * @{
  */
typedef enum {
	ADC0_0 = 0,
	ADC0_1,
	ADC0_2,
	ADC0_3
} ADCName;
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup MBED_SPI
 *  @brief    MBED_SPI driver modules.
 *  @{
 */

/** @addtogroup MBED_SPI_Exported_Types MBED_SPI Exported Types
  * @{
  */
typedef enum {
	SPI_0 = (int)SPI0_DEV,
	SPI_1 = (int)SPI1_DEV,
} SPIName;
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup MBED_I2C
 *  @brief    MBED_I2C driver modules.
 *  @{
 */

/** @addtogroup MBED_I2C_Exported_Types MBED_I2C Exported Types
  * @{
  */
typedef enum {
	I2C_0 = (int)I2C0_DEV,
} I2CName;
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup MBED_PWM
 *  @brief    MBED_PWM driver modules.
 *  @{
 */

/** @addtogroup MBED_PWM_Exported_Types MBED_PWM Exported Types
 * @{
 */
typedef enum {
	PWM_0 = 1,
	PWM_1,
	PWM_2,
	PWM_3,
	PWM_4,
	PWM_5
} PWMName;

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
