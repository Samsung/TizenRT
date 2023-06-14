/** mbed Microcontroller Library
  ******************************************************************************
  * @file    port_api.c
  * @author
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides mbed API for GPIO PORT.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "objects.h"
#include "port_api.h"
#include "pinmap.h"
#include "PinNames.h"
//#include "mbed_error.h"

#define GPIO_PORT_NUM 3

/** @addtogroup Ameba_Mbed_API
  * @{
  */

/** @addtogroup MBED_GPIO
 *  @brief    MBED_GPIO driver modules.
 *  @{
 */

/** @addtogroup MBED_GPIO_Exported_Functions MBED_GPIO Exported Functions
  * @{
  */

/**
  * @brief  Get GPIO port pin name.
  * @param  port: PortName according to pinmux spec, this parameter can be one of the following values:
  *		@arg PortA: Port number is A, which has 32 pins.
  *		@arg PortB: Port number is B, which has 32 pins.
  *		@arg PortC: Port number is C, which has 7 pins.
  * @param  pin_n: Pin number.
  * @return Port pin name.
  * @attention  Pin_n must be in the 0~31 range for PortA/B and 0~6 range for PortC.
  */
PinName port_pin(PortName port, int pin_n)
{
	return (PinName)(port << 5 | pin_n);
}

/**
  * @brief  Initialize the GPIO device port, including data direction registers.
  * @param  obj: GPIO port object defined in application software.
  * @param  port: PortName according to pinmux spec, this parameter can be one of the following values:
  *		@arg PortA: Port A, which has 32 pins.
  *		@arg PortB: Port B, which has 32 pins.
  *		@arg PortC: Port C, which has 7 pins.
  * @param  mask: Select one or multiple pins of the specified port.
  * @param  dir: GPIO port direction, this parameter can be one of the following values:
  *		@arg PIN_INPUT: Set selected port pins as input.
  *		@arg PIN_OUTPUT: Set selected port pins as output.
  * @retval none
  * @note One mask bit for one GPIO pin.
  */
void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
	assert_param(port < GPIO_PORT_NUM);

	obj->port      = port;
	obj->mask      = mask;

	port_dir(obj, dir);
}

/**
  * @brief  Configure data direction of GPIO port pins.
  * @param  obj: GPIO port object defined in application software.
  * @param  dir: This parameter can be one of the following values:
  *		@arg PIN_INPUT: Set selected port pins as input.
  *		@arg PIN_OUTPUT: Set selected port pins as input.
  * @retval none
  */
void port_dir(port_t *obj, PinDirection dir)
{
	if (dir == PIN_OUTPUT) {
		GPIO_PortDirection(obj->port, obj->mask, GPIO_Mode_OUT);
	} else {
		GPIO_PortDirection(obj->port, obj->mask, GPIO_Mode_IN);
	}
}

/**
  * @brief  Configure pull mode of GPIO port pins.
  * @param  obj: GPIO port object defined in application software.
  * @param  mode: This parameter can be one of the following values:
  *		@arg PullNone: Set selected port pins as HighZ.
  *		@arg PullDown: Pull down selected port pins.
  *		@arg PullUp: Pull up selected port pins.
  * @retval none
  */
void port_mode(port_t *obj, PinMode mode)
{
	uint32_t pin_idx;
	uint32_t max_num = 32;

	if (obj->port == PortC) {
		max_num = 7;
	}

	for (pin_idx = 0; pin_idx < max_num; pin_idx++) {
		if (obj->mask & BIT(pin_idx)) {
			/* PinName = (obj->port << 5 | pin_idx) */
			pin_mode((obj->port << 5 | pin_idx), mode);
		}
	}
}

/**
  * @brief  Set output value of the selected GPIO port pins.
  * @param  obj: GPIO port object defined in application software.
  * @param  value: Set value for one or multiple pins of the specified port.
  * @retval none
  * @note One bit for one GPIO pin: If corresponding bit is 1, pin state will be set to high and vice versa.
  */
void port_write(port_t *obj, int value)
{
	GPIO_PortWrite(obj->port, obj->mask, value);
}

/**
  * @brief  Read value of the specified GPIO port pins.
  * @param  obj: GPIO port object defined in application software.
  * @return State of the specified GPIO port pins.
  * @note One bit for one GPIO pin: If corresponding bit is 1, pin state is high and vice versa.
  */
int port_read(port_t *obj)
{
	u32 value = GPIO_PortRead(obj->port, obj->mask);

	return value;
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
