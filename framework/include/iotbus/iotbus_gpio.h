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
 * @defgroup GPIO GPIO
 * @brief Provides APIs for GPIO (General Purpose Input/Output)
 * @ingroup IOTBUS
 * @{
 */

/**
 * @file iotbus/iotbus_gpio.h
 * @brief Iotbus APIs for GPIO
 */

#ifndef IOTBUS_GPIO_H_
#define IOTBUS_GPIO_H_

#include <tinyara/config.h>
/**
 * @brief Enumeration of Gpio signal value
 * @details
 * Enumeration Details:\n
 * IOTBUS_GPIO_LOW = 0, Low value on Gpio
 * IOTBUS_GPIO_HIGH = 1, High value on Gpio
 */
typedef enum {
	IOTBUS_GPIO_LOW    = 0, /* Low value on Gpio */
	IOTBUS_GPIO_HIGH    = 1, /* High value on Gpio */
} iotbus_gpio_val_e;

/**
 * @brief Enumeration of Gpio output mode
 * @details
 * Enumeration Details:\n
 * IOTBUS_GPIO_DRIVE_PULLUP = 0, Resistive High\n
 * IOTBUS_GPIO_DRIVE_PULLDOWN = 1, Resistive Low\n
 * IOTBUS_GPIO_DRIVE_FLOAT = 2, Float\n
 */
typedef enum {
	IOTBUS_GPIO_DRIVE_PULLUP   = 0, /* Resistive High */
	IOTBUS_GPIO_DRIVE_PULLDOWN = 1, /* Resistive Low */
	IOTBUS_GPIO_DRIVE_FLOAT    = 2,
} iotbus_gpio_drive_e;

/**
 * @brief Enumeration of Gpio direction options
 * @details
 * Enumeration Details:\n
 * IOTBUS_GPIO_DIRECTION_NONE = 0, No set any state\n
 * IOTBUS_GPIO_DIRECTION_OUT = 1, Output Direction\n
 * IOTBUS_GPIO_DIRECTION_IN = 2, Input Direction\n
 */
typedef enum {
	IOTBUS_GPIO_DIRECTION_NONE = 0,
	IOTBUS_GPIO_DIRECTION_OUT  = 1,  /* Output. A Mode can also be set */
	IOTBUS_GPIO_DIRECTION_IN   = 2,  /* Input */
} iotbus_gpio_direction_e;

/**
 * @brief Enumeration of Gpio edge type for interrupt
 * @details
 * Enumeration Details:\n
 * IOTBUS_GPIO_EDGE_NONE = 0, No interrupt on Gpio\n
 * IOTBUS_GPIO_EDGE_BOTH = 1, Interrupt on rising and falling edge\n
 * IOTBUS_GPIO_EDGE_RISING = 2, Interrupt on rising edge only\n
 * IOTBUS_GPIO_EDGE_FALLING = 3, Interrupt on falling edge only\n
 */
typedef enum {
	IOTBUS_GPIO_EDGE_NONE    = 0, /* No interrupt on Gpio */
	IOTBUS_GPIO_EDGE_BOTH    = 1, /* Interrupt on rising & falling */
	IOTBUS_GPIO_EDGE_RISING  = 2, /* Interrupt on rising only */
	IOTBUS_GPIO_EDGE_FALLING = 3  /* Interrupt on falling only */
} iotbus_gpio_edge_e;

struct _iotbus_gpio_s;

/**
 * @brief Pointer definition to the internal struct _iotbus_gpio_wrapper_s
 */
typedef struct _iotbus_gpio_wrapper_s *iotbus_gpio_context_h;

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*gpio_isr_cb)(void *user_data);

/**
 * @brief initializes gpio_context based on gpio pin.
 *
 * @details @b #include <iotbus/iotbus_gpio.h>
 * @param[in] gpiopin gpio pin number
 * @return On success, handle of gpio_context is returned. On failure, NULL is returned.
 * @since TizenRT v1.0
 */
iotbus_gpio_context_h iotbus_gpio_open(int gpiopin);

/**
 * @brief closes the gpio_context.
 *
 * @details @b #include <iotbus/iotbus_gpio.h>
 * @param[in] dev handle of gpio_context
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_gpio_close(iotbus_gpio_context_h dev);

/**
 * @brief sets gpio direction.
 *
 * @details @b #include <iotbus/iotbus_gpio.h>
 * @param[in] dev handle of gpio_context
 * @param[in] dir gpio direction type
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_gpio_set_direction(iotbus_gpio_context_h dev, iotbus_gpio_direction_e dir);

/**
 * @brief sets the edge mode on the gpio.
 *
 * @details @b #include <iotbus/iotbus_gpio.h>
 * @param[in] dev handle of gpio_context
 * @param[in] edge gpio edge type
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_gpio_set_edge_mode(iotbus_gpio_context_h dev, iotbus_gpio_edge_e edge);

/**
 * @brief sets gpio output mode.
 *
 * @details @b #include <iotbus/iotbus_gpio.h>
 * @param[in] dev handle of gpio_context
 * @param[in] drive gpio drive type
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_gpio_set_drive_mode(iotbus_gpio_context_h dev, iotbus_gpio_drive_e drive);

/**
 * @brief registers event handler callback for interrupt.
 *
 * @details @b #include <iotbus/iotbus_gpio.h>\n
 * isr_cb is called when following situation occured.\n
 * the gpio value is changed from 0 to 1 on IOTBUS_GPIO_EDGE_RISING mode.\n
 * the gpio value is changed from 1 to 0 on IOTBUS_GPIO_EDGE_FALLING mode.
 *
 * @param[in] dev handle of gpio_context
 * @param[in] edge gpio edge type
 * @param[in] isr_cb the pointer of isr callback function
 * @param[in] user_data isr function parameter
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_gpio_register_cb(iotbus_gpio_context_h dev, iotbus_gpio_edge_e edge, gpio_isr_cb isr_cb, void *user_data);

/**
 * @brief unregisters event handler callback for interrupt.
 *
 * @details @b #include <iotbus/iotbus_gpio.h>
 * @param[in] dev handle of gpio_context
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_gpio_unregister_cb(iotbus_gpio_context_h dev);

/**
 * @brief reads the gpio value.
 *
 * @details @b #include <iotbus/iotbus_gpio.h>
 * @param[in] dev handle of gpio_context
 * @return On success, 0 or 1 is returned. (0: signal low, 1: signal high)
 *             On failure, a nagative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_gpio_read(iotbus_gpio_context_h dev);

/**
 * @brief writes to the gpio value.
 *
 * @details @b #include <iotbus/iotbus_gpio.h>
 * @param[in] dev handle of gpio_context
 * @param[in] value Gpio signal value
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_gpio_write(iotbus_gpio_context_h dev, int value);

/**
 * @brief gets a direction of the gpio.
 *
 * @details @b #include <iotbus/iotbus_gpio.h>
 * @param[in] dev handle of gpio_context
 * @param[out] dir current gpio direction
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_gpio_get_direction(iotbus_gpio_context_h dev, iotbus_gpio_direction_e *dir);

/**
 * @brief gets a pin number of the gpio.
 *
 * @details @b #include <iotbus/iotbus_gpio.h>
 * @param[in] dev handle of gpio_context
 * @return On success, gpio pin number is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_gpio_get_pin(iotbus_gpio_context_h dev);

/**
 * @brief gets a edge mode of the gpio.
 *
 * @details @b #include <iotbus/iotbus_gpio.h>
 * @param[in] dev handle of gpio_context
 * @param[out] edge current gpio edge type
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_gpio_get_edge_mode(iotbus_gpio_context_h dev, iotbus_gpio_edge_e *edge);

/**
 * @brief gets a drive mode of the gpio.
 *
 * @details @b #include <iotbus/iotbus_gpio.h>
 * @param[in] dev handle of gpio_context
 * @param[out] drive current drive type
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_gpio_get_drive_mode(iotbus_gpio_context_h dev, iotbus_gpio_drive_e *drive);

/**
 * @brief registers a signal for current on rising or falling edge of gpio
 *
 * @details @b #include <iotbus/iotbus_gpio.h>
 * @param[in] dev handle of gpio_context
 * @param[in] edge gpio edge type
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v2.0
 */
#ifndef CONFIG_DISABLE_SIGNALS
int iotbus_gpio_register_signal(iotbus_gpio_context_h dev, iotbus_gpio_edge_e edge);
#endif

#ifdef __cplusplus
}
#endif

#endif /* IOTBUS_GPIO_H_ */

/** @} */ // end of GPIO group
