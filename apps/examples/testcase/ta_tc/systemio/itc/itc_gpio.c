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

/// @file itc_gpio.c

/// @brief Test Case Example for gpio API

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <tinyara/time.h>
#include <sys/time.h>
#include "itc_internal.h"
#include <iotbus_gpio.h>
#include <iotbus_error.h>
#ifdef CONFIG_ARCH_CHIP_STM32
#include "../../os/arch/arm/src/mikroequail/src/mikroequail_gpio.h"
#endif

iotbus_gpio_context_h g_gpio;
iotbus_gpio_context_h g_gpio2;

int gpio_flag_callback = 0;

/**
* @testcase         gpio_event_callback
* @brief            callback called from itc_gpio_register_unregister_callback_p
* @scenario         successfully invoke callback and iotbus_gpio_read
* @apicovered       none
* @precondition     none
* @postcondition    nona
*/
void gpio_event_callback(void *user_data)
{
	SYSIO_ITC_PRINT("\n inside the function %s \n", __func__);
	iotbus_gpio_context_h hnd = (iotbus_gpio_context_h) user_data;
	int value = iotbus_gpio_read(hnd);
	if (value < 0) {
		gpio_flag_callback = 0;
		return;
	}
	gpio_flag_callback = 1;

	return;
}

/**
* @testcase         itc_gpio_open_close_p
* @brief            gpio_open returns handle of gpio context and gpio_close closes the context
* @scenario         initializes gpio_context based on gpio pin and then closes the gpio_context.
* @apicovered       iotbus_gpio_open, iotbus_gpio_close
* @precondition     none
* @postcondition    none
*/
void itc_gpio_open_close_p(void)
{
	int gpiopin = 41;
	int ret;
	g_gpio = iotbus_gpio_open(gpiopin);
	TC_ASSERT_NEQ("iotbus_gpio_open" , g_gpio, NULL);

	gpiopin = 14;
	g_gpio2 = iotbus_gpio_open(gpiopin);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_open", g_gpio2 , NULL , iotbus_gpio_close(g_gpio));

	ret = iotbus_gpio_close(g_gpio);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_close", ret, 0, iotbus_gpio_close(g_gpio2));

	ret = iotbus_gpio_close(g_gpio2);
	TC_ASSERT_LT("iotbus_gpio_close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_gpio_set_get_direction_p
* @brief            sets and gets gpio direction
* @scenario         Sets gpio direction and then gets the same gpio direction
* @apicovered       iotbus_gpio_set_direction, iotbus_gpio_get_direction
* @precondition     open gpio context
* @postcondition    close gpio context
*/
void itc_gpio_set_get_direction_p(void)
{
	int ret, index, ncount, gpiopin;
	iotbus_gpio_direction_e getDirection;
	iotbus_gpio_direction_e setDirection[] = { IOTBUS_GPIO_DIRECTION_NONE, IOTBUS_GPIO_DIRECTION_OUT, IOTBUS_GPIO_DIRECTION_IN };
	ncount = sizeof(setDirection) / sizeof(setDirection[0]);
	bool check = true;
	gpiopin = 41;
	g_gpio = iotbus_gpio_open(gpiopin);
	TC_ASSERT_NEQ("iotbus_gpio_open" , g_gpio, NULL);

	for (index = 0; index < ncount; index++) {
		ret = iotbus_gpio_set_direction(g_gpio, setDirection[index]);

		if (ret != 0) {
			SYSIO_ITC_PRINT("\nitc_gpio_set_get_direction: iotbus_gpio_set_direction FAIL for array index : %d\n", index);
			check = false;
			continue;
		}

		ret = iotbus_gpio_get_direction(g_gpio, &getDirection);
		if (ret != 0) {
			SYSIO_ITC_PRINT("\nitc_gpio_set_get_direction: iotbus_gpio_get_direction FAIL for array index : %d\n", index);
			check = false;
			continue;
		}

		if (getDirection != setDirection[index]) {
			SYSIO_ITC_PRINT("\nitc_gpio_set_get_direction: FAIL, set and get values not same for array index : %d\n", index);
			check = false;
		}
	}

	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_set_get_direction", check , true , iotbus_gpio_close(g_gpio));
	iotbus_gpio_close(g_gpio);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_gpio_set_get_edge_mode_p
* @brief            sets and gets gpio edge mode
* @scenario         Sets gpio edge mode and then gets the same gpio edge mode
* @apicovered       iotbus_gpio_set_edge_mode, iotbus_gpio_get_edge_mode
* @precondition     open gpio context
* @postcondition    close gpio context
*/
void itc_gpio_set_get_edge_mode_p(void)
{
	int ret, index, ncount, gpiopin;
	iotbus_gpio_edge_e getEdge;
	iotbus_gpio_edge_e setEdge[] = { IOTBUS_GPIO_EDGE_NONE, IOTBUS_GPIO_EDGE_BOTH, IOTBUS_GPIO_EDGE_RISING, IOTBUS_GPIO_EDGE_FALLING };
	ncount = sizeof(setEdge) / sizeof(setEdge[0]);
	bool check = true;
	gpiopin = 41;
	g_gpio = iotbus_gpio_open(gpiopin);
	TC_ASSERT_NEQ("iotbus_gpio_open" , g_gpio, NULL);

	for (index = 0; index < ncount; index++) {
		ret = iotbus_gpio_set_edge_mode(g_gpio, setEdge[index]);
		if (ret != 0) {
			SYSIO_ITC_PRINT("\nitc_gpio_set_get_edge_mode: iotbus_gpio_set_edge_mode FAIL for array index : %d\n", index);
			check = false;
			continue;
		}
		ret = iotbus_gpio_get_edge_mode(g_gpio, &getEdge);
		if (ret != 0) {
			SYSIO_ITC_PRINT("\nitc_gpio_set_get_edge_mode: iotbus_gpio_get_edge_mode FAIL for array index : %d\n", index);
			check = false;
			continue;
		}
		if (getEdge != setEdge[index]) {
			SYSIO_ITC_PRINT("\nitc_gpio_set_get_edge_mode: FAIL, set and get values not same for array index : %d\n", index);
			check = false;
		}
	}

	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_set_get_edge", check , true , iotbus_gpio_close(g_gpio));
	iotbus_gpio_close(g_gpio);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_gpio_set_get_drive_mode_p
* @brief            sets and gets gpio drive mode
* @scenario         Sets gpio drive mode and then gets the same gpio drive mode
* @apicovered       iotbus_gpio_set_drive_mode, iotbus_gpio_get_drive_mode
* @precondition     open gpio context
* @postcondition    close gpio context
*/
void itc_gpio_set_get_drive_mode_p(void)
{
	int gpiopin = 41;
	int ret, index, ncount;
	iotbus_gpio_drive_e getDrive;
	iotbus_gpio_drive_e setDrive[] = { IOTBUS_GPIO_DRIVE_NONE, IOTBUS_GPIO_DRIVE_PULLUP, IOTBUS_GPIO_DRIVE_PULLDOWN, IOTBUS_GPIO_DRIVE_FLOAT, IOTBUS_GPIO_DRIVE_PUSHPULL };
	ncount = sizeof(setDrive) / sizeof(setDrive[0]);
	bool check = true;

	g_gpio = iotbus_gpio_open(gpiopin);
	TC_ASSERT_NEQ("iotbus_gpio_open" , g_gpio, NULL);

	for (index = 0; index < ncount; index++) {
		ret = iotbus_gpio_set_drive_mode(g_gpio, setDrive[index]);
		if (ret != 0) {
			SYSIO_ITC_PRINT("\nitc_gpio_set_get_drive_mode: iotbus_gpio_set_drive_mode FAIL for array index : %d\n", index);
			check = false;
			continue;
		}
		ret = iotbus_gpio_get_drive_mode(g_gpio, &getDrive);
		if (ret != 0) {
			SYSIO_ITC_PRINT("\nitc_gpio_set_get_drive_mode: iotbus_gpio_get_drive_mode FAIL for array index : %d\n", index);
			check = false;
			continue;
		}
		if (getDrive != setDrive[index]) {
			SYSIO_ITC_PRINT("\nitc_gpio_set_get_drive_mode: FAIL, set and get values not same for array index : %d\n", index);
			check = false;
		}
	}

	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_set_get_drive_mode", check , true , iotbus_gpio_close(g_gpio));
	iotbus_gpio_close(g_gpio);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_gpio_get_pin_p
* @brief            gets a pin number of the gpio.
* @scenario         gets a pin number of the gpio.
* @apicovered       iotbus_gpio_get_pin
* @precondition     open gpio context
* @postcondition    close gpio context
*/
void itc_gpio_get_pin_p(void)
{
	int gpioSetpin = 41;
	int gpioGetpin;
	iotbus_gpio_context_h m_gpio = iotbus_gpio_open(gpioSetpin);
	TC_ASSERT_NEQ("iotbus_gpio_open" , m_gpio, NULL);

	gpioGetpin = iotbus_gpio_get_pin(m_gpio);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_get_pin", gpioGetpin, gpioSetpin, iotbus_gpio_close(m_gpio));

	iotbus_gpio_close(m_gpio);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_gpio_read_write_p
* @brief            reads and writes the gpio value
* @scenario         reads and writes the gpio value
* @apicovered       iotbus_gpio_read, iotbus_gpio_write
* @precondition     open gpio context
* @postcondition    close gpio context
*/
void itc_gpio_read_write_p(void)
{
	int ret, gpiopin = 41;
	g_gpio = iotbus_gpio_open(gpiopin);
	TC_ASSERT_NEQ("iotbus_gpio_open" , g_gpio, NULL);

	ret = iotbus_gpio_read(g_gpio);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_read", (ret < 0), false, iotbus_gpio_close(g_gpio));

	ret = iotbus_gpio_write(g_gpio, 1);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_write", (ret < 0), false, iotbus_gpio_close(g_gpio));

	iotbus_gpio_close(g_gpio);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_gpio_register_unregister_callback_p
* @brief            register and unregister event handler callback for interrupt
* @scenario         register and unregister event handler callback for interrupt
* @apicovered       iotbus_gpio_register_cb, iotbus_gpio_unregister_cb
* @precondition     open gpio context
* @postcondition    close gpio context
*/
void itc_gpio_register_unregister_callback_p(void)
{
	gpio_flag_callback = 0;
	int ret;
	int data = 0, gpio_pin2 = 57, gpio_pin1 = 41;

	g_gpio = iotbus_gpio_open(gpio_pin1);
	TC_ASSERT_NEQ("iotbus_gpio_open" , g_gpio, NULL);

	g_gpio2 = iotbus_gpio_open(gpio_pin2);
	TC_ASSERT_NEQ_CLEANUP("iotbus_gpio_open", g_gpio2 , NULL , iotbus_gpio_close(g_gpio));

	iotbus_gpio_set_direction(g_gpio, IOTBUS_GPIO_DIRECTION_OUT);
	iotbus_gpio_set_direction(g_gpio2, IOTBUS_GPIO_DIRECTION_IN);

	ret = iotbus_gpio_register_cb(g_gpio2, IOTBUS_GPIO_EDGE_RISING, gpio_event_callback, (void *)g_gpio2);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_register_cb", ret , 0, iotbus_gpio_close(g_gpio2); iotbus_gpio_close(g_gpio));

	// To trigger event for callback
	iotbus_gpio_write(g_gpio, data);
	sleep(1);
	data = 1;
	iotbus_gpio_write(g_gpio, data);
	sleep(2);

	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_register_cb", gpio_flag_callback, 1, iotbus_gpio_unregister_cb(g_gpio); iotbus_gpio_close(g_gpio2); iotbus_gpio_close(g_gpio));

	ret = iotbus_gpio_unregister_cb(g_gpio);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_unregister_cb", ret, 0, iotbus_gpio_close(g_gpio2); iotbus_gpio_close(g_gpio));

	ret = iotbus_gpio_close(g_gpio2);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_close", ret, 0 , iotbus_gpio_close(g_gpio));

	ret = iotbus_gpio_close(g_gpio);
	TC_ASSERT_EQ(iotbus_gpio_close, ret, 0)

	TC_SUCCESS_RESULT();
}

int itc_gpio_main(void)
{
	iotapi_initialize();

	itc_gpio_open_close_p();
	itc_gpio_get_pin_p();
	itc_gpio_set_get_direction_p();
	itc_gpio_set_get_edge_mode_p();
	itc_gpio_set_get_drive_mode_p();
	itc_gpio_read_write_p();
	itc_gpio_register_unregister_callback_p();

	return 0;
}
