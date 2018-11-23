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
#include "itc_internal.h"
#include <iotbus_gpio.h>
#include <iotbus_error.h>

#define GPIO_PIN 41
#define WAIT_SECONDS 2
#define LOOP_COUNT 10

static iotbus_gpio_context_h g_gpio_h;
static iotbus_gpio_context_h g_gpio_h2;

static int gpio_flag_callback = 0;
static int gpio_callback_data = -1;

/**
* @testcase         gpio_event_callback
* @brief            callback called from itc_systemio_gpio_register_unregister_callback_p
* @scenario         successfully invoke callback and iotbus_gpio_read
* @apicovered       none
* @precondition     none
* @postcondition    nona
*/
static void gpio_event_callback(void *user_data)
{
	SYSIO_ITC_PRINT("\n inside the function %s \n", __func__);
	iotbus_gpio_context_h hnd = (iotbus_gpio_context_h) user_data;
	gpio_callback_data = iotbus_gpio_read(hnd);
	gpio_flag_callback = 1;
}

/**
* @testcase         itc_systemio_gpio_open_close_p
* @brief            gpio_open returns handle of gpio context and gpio_close closes the context
* @scenario         initializes gpio_context based on gpio pin and then closes the gpio_context.
* @apicovered       iotbus_gpio_open, iotbus_gpio_close
* @precondition     none
* @postcondition    none
*/
static void itc_systemio_gpio_open_close_p(void)
{
	int ret;
	g_gpio_h = NULL;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_set_get_direction_p
* @brief            sets and gets gpio direction
* @scenario         Sets gpio direction and then gets the same gpio direction
* @apicovered       iotbus_gpio_set_direction, iotbus_gpio_get_direction
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_set_get_direction_p(void)
{
	int ret;
	g_gpio_h = NULL;
	int index;
	int ncount;
	iotbus_gpio_direction_e get_direction;
	iotbus_gpio_direction_e set_direction[] = { IOTBUS_GPIO_DIRECTION_NONE, IOTBUS_GPIO_DIRECTION_OUT, IOTBUS_GPIO_DIRECTION_IN };
	char *ptr_dir[] = { "IOTBUS_GPIO_DIRECTION_NONE", "IOTBUS_GPIO_DIRECTION_OUT", "IOTBUS_GPIO_DIRECTION_IN" };
	ncount = sizeof(set_direction) / sizeof(set_direction[0]);
	bool check = true;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	for (index = 0; index < ncount; index++) {
		ret = iotbus_gpio_set_direction(g_gpio_h, set_direction[index]);

		if (ret != 0) {
			SYSIO_ITC_PRINT("\nitc_systemio_gpio_set_get_direction: iotbus_gpio_set_direction FAIL for direction : %s\n", ptr_dir[index]);
			check = false;
			continue;
		}

		ret = iotbus_gpio_get_direction(g_gpio_h, &get_direction);
		if (ret != 0) {
			SYSIO_ITC_PRINT("\nitc_systemio_gpio_set_get_direction: iotbus_gpio_get_direction FAIL for direction : %s\n", ptr_dir[index]);
			check = false;
			continue;
		}

		if (get_direction != set_direction[index]) {
			SYSIO_ITC_PRINT("\nitc_systemio_gpio_set_get_direction: FAIL, set and get values not same for direction : %s\n", ptr_dir[index]);
			check = false;
		}
	}

	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_set_get_direction", check, true, iotbus_gpio_close(g_gpio_h));
	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_set_get_edge_mode_p
* @brief            sets and gets gpio edge mode
* @scenario         Sets gpio edge mode and then gets the same gpio edge mode
* @apicovered       iotbus_gpio_set_edge_mode, iotbus_gpio_get_edge_mode
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_set_get_edge_mode_p(void)
{
	int ret;
	g_gpio_h = NULL;
	int index;
	int ncount;
	iotbus_gpio_edge_e get_edge;
	iotbus_gpio_edge_e set_edge[] = { IOTBUS_GPIO_EDGE_NONE, IOTBUS_GPIO_EDGE_BOTH, IOTBUS_GPIO_EDGE_RISING, IOTBUS_GPIO_EDGE_FALLING };
	char *ptr_edge[] = { "IOTBUS_GPIO_EDGE_NONE", "IOTBUS_GPIO_EDGE_BOTH", "IOTBUS_GPIO_EDGE_RISING", "IOTBUS_GPIO_EDGE_FALLING" };
	ncount = sizeof(set_edge) / sizeof(set_edge[0]);
	bool check = true;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	for (index = 0; index < ncount; index++) {
		ret = iotbus_gpio_set_edge_mode(g_gpio_h, set_edge[index]);
		if (ret != 0) {
			SYSIO_ITC_PRINT("\nitc_systemio_gpio_set_get_edge_mode: iotbus_gpio_set_edge_mode FAIL for edge mode : %s\n", ptr_edge[index]);
			check = false;
			continue;
		}
		ret = iotbus_gpio_get_edge_mode(g_gpio_h, &get_edge);
		if (ret != 0) {
			SYSIO_ITC_PRINT("\nitc_systemio_gpio_set_get_edge_mode: iotbus_gpio_get_edge_mode FAIL for edge mode : %s\n", ptr_edge[index]);
			check = false;
			continue;
		}
		if (get_edge != set_edge[index]) {
			SYSIO_ITC_PRINT("\nitc_systemio_gpio_set_get_edge_mode: FAIL, set and get values not same for edge mode : %s\n", ptr_edge[index]);
			check = false;
		}
	}

	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_set_get_edge", check, true, iotbus_gpio_close(g_gpio_h));
	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_set_get_drive_mode_p
* @brief            sets and gets gpio drive mode
* @scenario         Sets gpio drive mode and then gets the same gpio drive mode
* @apicovered       iotbus_gpio_set_drive_mode, iotbus_gpio_get_drive_mode
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_set_get_drive_mode_p(void)
{
	int ret;
	g_gpio_h = NULL;
	int index;
	int ncount;
	iotbus_gpio_drive_e get_drive;
	iotbus_gpio_drive_e set_drive[] = { IOTBUS_GPIO_DRIVE_PULLUP, IOTBUS_GPIO_DRIVE_PULLDOWN, IOTBUS_GPIO_DRIVE_FLOAT };
	char *ptr_drive[] = { "IOTBUS_GPIO_DRIVE_PULLUP", "IOTBUS_GPIO_DRIVE_PULLDOWN", "IOTBUS_GPIO_DRIVE_FLOAT" };
	ncount = sizeof(set_drive) / sizeof(set_drive[0]);
	bool check = true;

	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	for (index = 0; index < ncount; index++) {
		ret = iotbus_gpio_set_drive_mode(g_gpio_h, set_drive[index]);
		if (ret != 0) {
			check = false;
			SYSIO_ITC_PRINT("\nitc_systemio_gpio_set_get_drive_mode: iotbus_gpio_set_drive_mode FAIL for drive : %s\n", ptr_drive[index]);
			continue;
		}
		ret = iotbus_gpio_get_drive_mode(g_gpio_h, &get_drive);
		if (ret != 0) {
			SYSIO_ITC_PRINT("\nitc_systemio_gpio_set_get_drive_mode: iotbus_gpio_get_drive_mode FAIL for drive : %s\n", ptr_drive[index]);
			check = false;
			continue;
		}
		if (get_drive != set_drive[index]) {
			SYSIO_ITC_PRINT("\nitc_systemio_gpio_set_get_drive_mode: FAIL, set and get values not same for drive : %s\n", ptr_drive[index]);
			check = false;
		}
	}

	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_set_get_drive_mode", check, true, iotbus_gpio_close(g_gpio_h));
	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_get_pin_p
* @brief            gets a pin number of the gpio.
* @scenario         gets a pin number of the gpio.
* @apicovered       iotbus_gpio_get_pin
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_get_pin_p(void)
{
	int ret;
	int gpio_set_pin = GPIO_PIN;
	int gpio_get_pin;
	iotbus_gpio_context_h m_gpio = iotbus_gpio_open(gpio_set_pin);
	TC_ASSERT_NEQ("iotbus_gpio_open", m_gpio, NULL);

	gpio_get_pin = iotbus_gpio_get_pin(m_gpio);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_get_pin", gpio_get_pin, gpio_set_pin, iotbus_gpio_close(m_gpio));

	ret = iotbus_gpio_close(m_gpio);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_read_write_p
* @brief            reads and writes the gpio value
* @scenario         reads and writes the gpio value
* @apicovered       iotbus_gpio_read, iotbus_gpio_write
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_read_write_p(void)
{
	int ret;
	g_gpio_h = NULL;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_read(g_gpio_h);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_read", (ret < 0), false, iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_write(g_gpio_h, 1);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_write", (ret < 0), false, iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_register_unregister_callback_p
* @brief            register and unregister event handler callback for interrupt
* @scenario         register and unregister event handler callback for interrupt
* @apicovered       iotbus_gpio_register_cb, iotbus_gpio_unregister_cb
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_register_unregister_callback_p(void)
{
	gpio_flag_callback = 0;
	int ret;
	g_gpio_h = NULL;
	g_gpio_h2 = NULL;
	int data = 0;
	int gpio_pin2 = 57;
	int gpio_pin1 = GPIO_PIN;

	g_gpio_h = iotbus_gpio_open(gpio_pin1);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	g_gpio_h2 = iotbus_gpio_open(gpio_pin2);
	TC_ASSERT_NEQ_CLEANUP("iotbus_gpio_open", g_gpio_h2, NULL, iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_set_direction(g_gpio_h, IOTBUS_GPIO_DIRECTION_OUT);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_set_direction", ret, 0, iotbus_gpio_close(g_gpio_h2); iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_set_direction(g_gpio_h2, IOTBUS_GPIO_DIRECTION_IN);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_set_direction", ret, 0, iotbus_gpio_close(g_gpio_h2); iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_register_cb(g_gpio_h2, IOTBUS_GPIO_EDGE_RISING, gpio_event_callback, (void *)g_gpio_h2);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_register_cb", ret, 0, iotbus_gpio_close(g_gpio_h2); iotbus_gpio_close(g_gpio_h));

	// To trigger event for callback
	ret = iotbus_gpio_write(g_gpio_h, data);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_write", ret, 0, iotbus_gpio_close(g_gpio_h2); iotbus_gpio_close(g_gpio_h));
	sleep(WAIT_SECONDS);

	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_register_cb", gpio_flag_callback, 0, iotbus_gpio_unregister_cb(g_gpio_h2); iotbus_gpio_close(g_gpio_h2); iotbus_gpio_close(g_gpio_h));

	data = 1;
	ret = iotbus_gpio_write(g_gpio_h, data);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_write", ret, 0, iotbus_gpio_close(g_gpio_h2); iotbus_gpio_close(g_gpio_h));
	sleep(WAIT_SECONDS);

	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_register_cb", gpio_flag_callback, 1, iotbus_gpio_unregister_cb(g_gpio_h2); iotbus_gpio_close(g_gpio_h2); iotbus_gpio_close(g_gpio_h));
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_register_cb", gpio_callback_data, data, iotbus_gpio_unregister_cb(g_gpio_h2); iotbus_gpio_close(g_gpio_h2); iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_unregister_cb(g_gpio_h2);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_unregister_cb", ret, 0, iotbus_gpio_close(g_gpio_h2); iotbus_gpio_close(g_gpio_h));

	gpio_flag_callback = 0;
	data = 1;

	ret = iotbus_gpio_write(g_gpio_h, data);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_register_cb", ret, 0, iotbus_gpio_unregister_cb(g_gpio_h2); iotbus_gpio_close(g_gpio_h2); iotbus_gpio_close(g_gpio_h));
	sleep(WAIT_SECONDS);

	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_register_cb", gpio_flag_callback, 0, iotbus_gpio_unregister_cb(g_gpio_h2); iotbus_gpio_close(g_gpio_h2); iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_close(g_gpio_h2);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_close", ret, 0, iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ(iotbus_gpio_close, ret, 0)

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_set_direction_read_write_p_all_directions
* @brief            sets and gets gpio direction and read write
* @scenario         Sets gpio direction and then gets the same gpio direction. reads and writes the gpio value
* @apicovered       iotbus_gpio_set_direction, iotbus_gpio_get_direction, iotbus_gpio_read, iotbus_gpio_write
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_set_direction_read_write_p_all_directions(void)
{
	int ret;
	g_gpio_h = NULL;
	int count_dir;
	int i;

	iotbus_gpio_direction_e get_direction;
	iotbus_gpio_direction_e set_direction[] = { IOTBUS_GPIO_DIRECTION_NONE, IOTBUS_GPIO_DIRECTION_OUT, IOTBUS_GPIO_DIRECTION_IN };
	char *ptr_dir[] = { "IOTBUS_GPIO_DIRECTION_NONE", "IOTBUS_GPIO_DIRECTION_OUT", "IOTBUS_GPIO_DIRECTION_IN" };

	count_dir = sizeof(set_direction) / sizeof(set_direction[0]);
	bool check = true;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	for (i = 0; i < count_dir; i++) {
		ret = iotbus_gpio_set_direction(g_gpio_h, set_direction[i]);
		if (ret != 0) {
			SYSIO_ITC_PRINT("itc_systemio_gpio_all_direction_read_write_p: iotbus_gpio_set_direction FAIL for direction %s\n", ptr_dir[i]);
			check = false;
			continue;
		}
		ret = iotbus_gpio_get_direction(g_gpio_h, &get_direction);
		if (ret != 0) {
			SYSIO_ITC_PRINT("itc_systemio_gpio_all_direction_read_write_p: iotbus_gpio_get_direction FAIL for direction %s\n", ptr_dir[i]);
			check = false;
			continue;
		}
		if (get_direction != set_direction[i]) {
			SYSIO_ITC_PRINT("itc_systemio_gpio_all_direction_read_write_p: FAIL, set and get values not same for direction %s\n", ptr_dir[i]);
			check = false;
		}

		ret = iotbus_gpio_read(g_gpio_h);
		TC_ASSERT_EQ_CLEANUP("iotbus_gpio_read", (ret < 0), false, iotbus_gpio_close(g_gpio_h));

		ret = iotbus_gpio_write(g_gpio_h, 1);
		TC_ASSERT_EQ_CLEANUP("iotbus_gpio_write", (ret < 0), false, iotbus_gpio_close(g_gpio_h));
	}
	TC_ASSERT_EQ_CLEANUP("itc_systemio_gpio_all_direction_read_write_p", check, true, iotbus_gpio_close(g_gpio_h));
	iotbus_gpio_close(g_gpio_h);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_set_edge_read_write_p_all_edges
* @brief            sets and gets gpio edge mode and gpio read and write
* @scenario         Sets gpio edge mode and then gets the same gpio edge mode. reads and writes the gpio value
* @apicovered       iotbus_gpio_set_edge_mode, iotbus_gpio_get_edge_mode, iotbus_gpio_read, iotbus_gpio_write
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_set_edge_read_write_p_all_edges(void)
{
	int ret;
	g_gpio_h = NULL;
	int count_edge;
	int i;

	iotbus_gpio_edge_e get_edge;
	iotbus_gpio_edge_e set_edge[] = { IOTBUS_GPIO_EDGE_NONE, IOTBUS_GPIO_EDGE_BOTH, IOTBUS_GPIO_EDGE_RISING, IOTBUS_GPIO_EDGE_FALLING };
	char *ptr_edge[] = { "IOTBUS_GPIO_EDGE_NONE", "IOTBUS_GPIO_EDGE_BOTH", "IOTBUS_GPIO_EDGE_RISING", "IOTBUS_GPIO_EDGE_FALLING" };

	count_edge = sizeof(set_edge) / sizeof(set_edge[0]);
	bool check = true;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	for (i = 0; i < count_edge; i++) {
		ret = iotbus_gpio_set_edge_mode(g_gpio_h, set_edge[i]);
		if (ret != 0) {
			SYSIO_ITC_PRINT("itc_systemio_gpio_all_edge_read_write_p: iotbus_gpio_set_edge_mode FAIL for edge %s\n", ptr_edge[i]);
			check = false;
			continue;
		}
		ret = iotbus_gpio_get_edge_mode(g_gpio_h, &get_edge);
		if (ret != 0) {
			SYSIO_ITC_PRINT("itc_systemio_gpio_all_edge_read_write_p: iotbus_gpio_get_edge_mode FAIL for edge %s\n", ptr_edge[i]);
			check = false;
			continue;
		}
		if (get_edge != set_edge[i]) {
			SYSIO_ITC_PRINT("itc_systemio_gpio_all_edge_read_write_p: FAIL, set and get values not same for edge %s\n", ptr_edge[i]);
			check = false;
		}

		ret = iotbus_gpio_read(g_gpio_h);
		TC_ASSERT_EQ_CLEANUP("iotbus_gpio_read", (ret < 0), false, iotbus_gpio_close(g_gpio_h));

		ret = iotbus_gpio_write(g_gpio_h, 1);
		TC_ASSERT_EQ_CLEANUP("iotbus_gpio_write", (ret < 0), false, iotbus_gpio_close(g_gpio_h));
	}
	TC_ASSERT_EQ_CLEANUP("itc_systemio_gpio_all_edge_read_write_p", check, true, iotbus_gpio_close(g_gpio_h));
	iotbus_gpio_close(g_gpio_h);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_set_drive_read_write_p_all_drive_mode
* @brief            sets and gets gpio drive mode and gpio read and write
* @scenario         Sets gpio drive mode and then gets the same gpio drive mode. reads and writes the gpio value
* @apicovered       iotbus_gpio_set_drive_mode, iotbus_gpio_get_drive_mode, iotbus_gpio_read, iotbus_gpio_write
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_set_drive_read_write_p_all_drive_mode(void)
{
	int ret;
	g_gpio_h = NULL;
	int count_drive;
	int i;

	iotbus_gpio_drive_e get_drive;
	iotbus_gpio_drive_e set_drive[] = { IOTBUS_GPIO_DRIVE_PULLUP, IOTBUS_GPIO_DRIVE_PULLDOWN, IOTBUS_GPIO_DRIVE_FLOAT };
	char *ptr_drive[] = { "IOTBUS_GPIO_DRIVE_PULLUP", "IOTBUS_GPIO_DRIVE_PULLDOWN", "IOTBUS_GPIO_DRIVE_FLOAT" };

	count_drive = sizeof(set_drive) / sizeof(set_drive[0]);
	bool check = true;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	for (i = 0; i < count_drive; i++) {
		ret = iotbus_gpio_set_drive_mode(g_gpio_h, set_drive[i]);
		if (ret != 0) {
			SYSIO_ITC_PRINT("itc_systemio_gpio_all_drive_read_write_p: iotbus_gpio_set_drive_mode FAIL for drive : %s\n", ptr_drive[i]);
			check = false;
			continue;
		}
		ret = iotbus_gpio_get_drive_mode(g_gpio_h, &get_drive);
		if (ret != 0) {
			SYSIO_ITC_PRINT("itc_systemio_gpio_all_drive_read_write_p: iotbus_gpio_get_drive_mode FAIL for drive : %s\n", ptr_drive[i]);
			check = false;
			continue;
		}
		if (get_drive != set_drive[i]) {
			SYSIO_ITC_PRINT("itc_systemio_gpio_all_drive_read_write_p: FAIL, set and get values not same for drive : %s\n", ptr_drive[i]);
			check = false;
		}

		ret = iotbus_gpio_read(g_gpio_h);
		TC_ASSERT_EQ_CLEANUP("iotbus_gpio_read", (ret < 0), false, iotbus_gpio_close(g_gpio_h));

		ret = iotbus_gpio_write(g_gpio_h, 1);
		TC_ASSERT_EQ_CLEANUP("iotbus_gpio_write", (ret < 0), false, iotbus_gpio_close(g_gpio_h));
	}
	TC_ASSERT_EQ_CLEANUP("itc_systemio_gpio_all_drive_read_write_p", check, true, iotbus_gpio_close(g_gpio_h));
	iotbus_gpio_close(g_gpio_h);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_set_get_direction_edge_drive_mode_p
* @brief            sets and gets gpio direction and edge mode
* @scenario         Sets gpio direction, edge mode and drive mode and then gets the same gpio direction, edge mode and drive mode
* @apicovered       iotbus_gpio_set_direction, iotbus_gpio_get_direction, iotbus_gpio_set_edge_mode, iotbus_gpio_get_edge_mode, iotbus_gpio_set_drive_mode, iotbus_gpio_get_drive_mode
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_set_get_direction_edge_drive_mode_p(void)
{
	int ret;
	g_gpio_h = NULL;
	int count_dir;
	int count_mode;
	int count_drive;
	int h;
	int i;
	int j;
	int k;
	iotbus_gpio_direction_e get_direction;
	iotbus_gpio_edge_e get_edge;
	iotbus_gpio_drive_e get_drive;
	iotbus_gpio_direction_e set_direction[] = { IOTBUS_GPIO_DIRECTION_NONE, IOTBUS_GPIO_DIRECTION_OUT, IOTBUS_GPIO_DIRECTION_IN };
	iotbus_gpio_edge_e set_edge[] = { IOTBUS_GPIO_EDGE_NONE, IOTBUS_GPIO_EDGE_BOTH, IOTBUS_GPIO_EDGE_RISING, IOTBUS_GPIO_EDGE_FALLING };
	iotbus_gpio_drive_e set_drive[] = { IOTBUS_GPIO_DRIVE_PULLUP, IOTBUS_GPIO_DRIVE_PULLDOWN, IOTBUS_GPIO_DRIVE_FLOAT };
	char *ptr_dir[] = { "IOTBUS_GPIO_DIRECTION_NONE", "IOTBUS_GPIO_DIRECTION_OUT", "IOTBUS_GPIO_DIRECTION_IN" };
	char *ptr_edge[] = { "IOTBUS_GPIO_EDGE_NONE", "IOTBUS_GPIO_EDGE_BOTH", "IOTBUS_GPIO_EDGE_RISING", "IOTBUS_GPIO_EDGE_FALLING" };
	char *ptr_drive[] = { "IOTBUS_GPIO_DRIVE_PULLUP", "IOTBUS_GPIO_DRIVE_PULLDOWN", "IOTBUS_GPIO_DRIVE_FLOAT" };

	count_dir = sizeof(set_direction) / sizeof(set_direction[0]);
	count_mode = sizeof(set_edge) / sizeof(set_edge[0]);
	count_drive = sizeof(set_drive) / sizeof(set_drive[0]);
	bool check = true;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	for (h = 0; h < LOOP_COUNT; h++) {
		for (i = 0; i < count_dir; i++) {
			for (j = 0; j < count_mode; j++) {
				for (k = 0; k < count_drive; k++) {

					ret = iotbus_gpio_set_direction(g_gpio_h, set_direction[i]);
					if (ret != 0) {
						SYSIO_ITC_PRINT("itc_systemio_gpio_set_get_direction_edge_drive_mode_p: iotbus_gpio_set_direction FAIL for direction %s\n", ptr_dir[i]);
						check = false;
						continue;
					}
					ret = iotbus_gpio_get_direction(g_gpio_h, &get_direction);
					if (ret != 0) {
						SYSIO_ITC_PRINT("itc_systemio_gpio_set_get_direction_edge_drive_mode_p: iotbus_gpio_get_direction FAIL for direction %s\n", ptr_dir[i]);
						check = false;
						continue;
					}
					if (get_direction != set_direction[i]) {
						SYSIO_ITC_PRINT("itc_systemio_gpio_set_get_direction_edge_drive_mode_p: FAIL, set and get values not same for direction %s\n", ptr_dir[i]);
						check = false;
					}

					ret = iotbus_gpio_set_edge_mode(g_gpio_h, set_edge[j]);
					if (ret != 0) {
						SYSIO_ITC_PRINT("itc_systemio_gpio_set_get_direction_edge_drive_mode_p: iotbus_gpio_set_edge_mode FAIL for edge mode : %s\n", ptr_edge[j]);
						check = false;
						continue;
					}
					ret = iotbus_gpio_get_edge_mode(g_gpio_h, &get_edge);
					if (ret != 0) {
						SYSIO_ITC_PRINT("itc_systemio_gpio_set_get_direction_edge_drive_mode_p: iotbus_gpio_get_edge_mode FAIL for edge mode : %s\n", ptr_edge[j]);
						check = false;
						continue;
					}
					if (get_edge != set_edge[j]) {
						SYSIO_ITC_PRINT("itc_systemio_gpio_set_get_direction_edge_drive_mode_p: FAIL, set and get values not same for edge mode : %s\n", ptr_edge[j]);
						check = false;
					}

					ret = iotbus_gpio_set_drive_mode(g_gpio_h, set_drive[k]);
					if (ret != 0) {
						SYSIO_ITC_PRINT("itc_systemio_gpio_set_get_direction_edge_drive_mode_p: iotbus_gpio_set_drive_mode FAIL for drive : %s\n", ptr_drive[k]);
						check = false;
						continue;
					}
					ret = iotbus_gpio_get_drive_mode(g_gpio_h, &get_drive);
					if (ret != 0) {
						SYSIO_ITC_PRINT("itc_systemio_gpio_set_get_direction_edge_drive_mode_p: iotbus_gpio_get_drive_mode FAIL for drive : %s\n", ptr_drive[k]);
						check = false;
						continue;
					}
					if (get_drive != set_drive[k]) {
						SYSIO_ITC_PRINT("\nitc_systemio_gpio_set_get_drive_mode: FAIL, set and get values not same for drive : %s\n", ptr_drive[k]);
						check = false;
					}
				}
			}
		}
	}
	TC_ASSERT_EQ_CLEANUP("itc_systemio_gpio_set_get_direction_edge_drive_mode_p", check, true, iotbus_gpio_close(g_gpio_h));
	iotbus_gpio_close(g_gpio_h);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_open_close_p_different_pin
* @brief            gpio_open returns handle of gpio context and gpio_close closes the context
* @scenario         initializes gpio_context based on gpio pin and then closes the gpio_context for all the pins.
* @apicovered       iotbus_gpio_open, iotbus_gpio_close
* @precondition     none
* @postcondition    none
*/
static void itc_systemio_gpio_open_close_p_different_pin(void)
{
	int gpio_pin[] = { 30, 31, 32, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 57, 58, 59 };
	int ret;
	int count;
	int index;
	count = sizeof(gpio_pin) / sizeof(gpio_pin[0]);
	for (index = 0; index < count; index++) {
		g_gpio_h = NULL;
		g_gpio_h = iotbus_gpio_open(gpio_pin[index]);
		TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

		ret = iotbus_gpio_close(g_gpio_h);
		TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_open_close_p_multi_handle
* @brief            gpio_open returns handle of gpio context and gpio_close closes the context
* @scenario         initializes gpio_context based on gpio pin and then reopens gpio with same pin and then closes the gpio_context.
* @apicovered       iotbus_gpio_open, iotbus_gpio_close
* @precondition     none
* @postcondition    none
*/
static void itc_systemio_gpio_open_close_p_multi_handle(void)
{
	int ret;
	g_gpio_h = NULL;
	g_gpio_h2 = NULL;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	g_gpio_h2 = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ_CLEANUP("iotbus_gpio_open", g_gpio_h2, NULL, iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_close", ret, 0, iotbus_gpio_close(g_gpio_h2));

	ret = iotbus_gpio_close(g_gpio_h2);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	TC_SUCCESS_RESULT();
}

#if 0
/**
* @testcase         itc_systemio_gpio_open_close_p_reclose
* @brief            gpio_open returns handle of gpio context and gpio_close closes the context
* @scenario         initializes gpio_context based on gpio pin and then closes the gpio_context and then again recloses same gpio context.
* @apicovered       iotbus_gpio_open, iotbus_gpio_close
* @precondition     none
* @postcondition    none
*/
static void itc_systemio_gpio_open_close_p_reclose(void)
{
	int ret;
	g_gpio_h = NULL;

	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_LT("iotbus_gpio_close", ret, 0);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @testcase         itc_systemio_gpio_open_n
* @brief            gpio_open returns handle of gpio context
* @scenario         initializes gpio_context based on invalid pin
* @apicovered       iotbus_gpio_open
* @precondition     none
* @postcondition    none
*/
static void itc_systemio_gpio_open_n(void)
{
	g_gpio_h = NULL;
	g_gpio_h = iotbus_gpio_open(1);
	TC_ASSERT_EQ("iotbus_gpio_open", g_gpio_h, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_close_n
* @brief            gpio_open returns handle of gpio context and gpio_close closes the context
* @scenario         initializes gpio_context based on gpio pin and then closes the gpio_context and then again recloses same gpio context.
* @apicovered       iotbus_gpio_close
* @precondition     none
* @postcondition    none
*/
static void itc_systemio_gpio_close_n(void)
{
	iotbus_gpio_context_h gpio_context = NULL;
	int ret;

	ret = iotbus_gpio_close(gpio_context);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_get_pin_n
* @brief            gets a pin number of the gpio.
* @scenario         gets a pin number of the gpio with invalid parameter.
* @apicovered       iotbus_gpio_get_pin
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_get_pin_n(void)
{
	int ret;
	g_gpio_h = NULL;
	ret = iotbus_gpio_get_pin(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_get_pin", ret, IOTBUS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_get_pin_n_after_close
* @brief            gpio_open returns handle of gpio context and gpio_close closes the context
* @scenario         initializes gpio_context based on gpio pin and then closes the gpio_context, then get gpio pin
* @apicovered       iotbus_gpio_get_pin
* @precondition     none
* @postcondition    none
*/
static void itc_systemio_gpio_get_pin_n_after_close(void)
{
	int ret;
	g_gpio_h = NULL;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	ret = iotbus_gpio_get_pin(g_gpio_h);
	TC_ASSERT_LT("iotbus_gpio_get_pin", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_get_pin_n_after_invalid_open
* @brief            gpio_open returns handle of gpio context and gpio_close closes the context
* @scenario         initializes gpio_context based on gpio pin and then closes the gpio_context, then get gpio pin
* @apicovered       iotbus_gpio_get_pin
* @precondition     none
* @postcondition    none
*/
static void itc_systemio_gpio_get_pin_n_after_invalid_open(void)
{
	int ret;
	int gpio_invalid_pin = 1;
	g_gpio_h = NULL;
	g_gpio_h = iotbus_gpio_open(gpio_invalid_pin);
	TC_ASSERT_EQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_get_pin(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_get_pin", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_read_n_after_close
* @brief            reads the gpio value
* @scenario         reads the gpio value after gpio close
* @apicovered       iotbus_gpio_read
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_read_n_after_close(void)
{
	int ret;
	g_gpio_h = NULL;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	ret = iotbus_gpio_read(g_gpio_h);
	TC_ASSERT_LT("iotbus_gpio_read", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_write_n_after_close
* @brief            writes the gpio value
* @scenario         writes the gpio value after gpio close
* @apicovered       iotbus_gpio_write
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_write_n_after_close(void)
{
	int ret;
	g_gpio_h = NULL;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	ret = iotbus_gpio_write(g_gpio_h, 1);
	TC_ASSERT_LT("iotbus_gpio_write", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_set_direction_n
* @brief            sets the gpio direction
* @scenario         sets the gpio direction with invalid parameter
* @apicovered       iotbus_gpio_set_direction
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_set_direction_n(void)
{
	int ret;
	g_gpio_h = NULL;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_set_direction(g_gpio_h, -1);
	TC_ASSERT_LT_CLEANUP("iotbus_gpio_set_direction", ret, 0, iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_get_direction_n
* @brief            gets the gpio direction
* @scenario         gets the gpio direction with invalid parameter
* @apicovered       iotbus_gpio_get_direction
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_get_direction_n(void)
{
	int ret;
	g_gpio_h = NULL;
	iotbus_gpio_direction_e get_direction;

	ret = iotbus_gpio_get_direction(g_gpio_h, &get_direction);
	TC_ASSERT_EQ("iotbus_gpio_get_direction", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_set_direction_n_after_close
* @brief            sets the gpio direction
* @scenario         sets the gpio direction after gpio context is closed
* @apicovered       iotbus_gpio_set_direction
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_set_direction_n_after_close(void)
{
	int ret;
	g_gpio_h = NULL;
	iotbus_gpio_direction_e set_direction = IOTBUS_GPIO_DIRECTION_NONE;
	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	ret = iotbus_gpio_set_direction(g_gpio_h, set_direction);
	TC_ASSERT_LT("iotbus_gpio_set_direction", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_get_direction_n_after_close
* @brief            gets the gpio direction
* @scenario         gets the gpio direction after gpio context is closed
* @apicovered       iotbus_gpio_get_direction
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_get_direction_n_after_close(void)
{
	int ret;
	g_gpio_h = NULL;
	iotbus_gpio_direction_e set_direction = IOTBUS_GPIO_DIRECTION_NONE;
	iotbus_gpio_direction_e get_direction;

	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_set_direction(g_gpio_h, set_direction);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_set_direction", ret, 0, iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	ret = iotbus_gpio_get_direction(g_gpio_h, &get_direction);
	TC_ASSERT_LT("iotbus_gpio_get_direction", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_set_edge_mode_n
* @brief            sets the gpio edge mode
* @scenario         sets the gpio edge mode with invalid parameter
* @apicovered       iotbus_gpio_set_edge_mode
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_set_edge_mode_n(void)
{
	int ret;
	g_gpio_h = NULL;

	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_set_edge_mode(g_gpio_h, -1);
	TC_ASSERT_LT_CLEANUP("iotbus_gpio_set_edge_mode", ret, 0, iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_get_edge_mode_n
* @brief            gets the gpio edge mode
* @scenario         gets the gpio edge mode with invalid parameter
* @apicovered       iotbus_gpio_get_edge_mode
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_get_edge_mode_n(void)
{
	int ret;
	g_gpio_h = NULL;
	iotbus_gpio_edge_e get_edge;

	ret = iotbus_gpio_get_edge_mode(g_gpio_h, &get_edge);
	TC_ASSERT_EQ("iotbus_gpio_get_edge_mode", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_set_edge_mode_n_after_close
* @brief            sets the gpio edge mode
* @scenario         sets the gpio edge mode after gpio context is closed
* @apicovered       iotbus_gpio_set_edge_mode
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_set_edge_mode_n_after_close(void)
{
	int ret;
	g_gpio_h = NULL;
	iotbus_gpio_edge_e set_edge = IOTBUS_GPIO_EDGE_NONE;

	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	ret = iotbus_gpio_set_edge_mode(g_gpio_h, set_edge);
	TC_ASSERT_LT("iotbus_gpio_set_edge_mode", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_get_edge_mode_n_after_close
* @brief            gets the gpio edge mode
* @scenario         gets the gpio edge mode after gpio context is closed
* @apicovered       iotbus_gpio_get_edge_mode
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_get_edge_mode_n_after_close(void)
{
	int ret;
	g_gpio_h = NULL;
	iotbus_gpio_edge_e set_edge = IOTBUS_GPIO_EDGE_NONE;
	iotbus_gpio_edge_e get_edge;

	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_set_edge_mode(g_gpio_h, set_edge);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_set_edge_mode", ret, 0, iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	ret = iotbus_gpio_get_edge_mode(g_gpio_h, &get_edge);
	TC_ASSERT_LT("iotbus_gpio_get_edge_mode", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_set_drive_mode_n
* @brief            sets the gpio drive mode
* @scenario         sets the gpio drive mode with invalid parameter
* @apicovered       iotbus_gpio_set_drive_mode
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_set_drive_mode_n(void)
{
	int ret;
	g_gpio_h = NULL;

	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_set_drive_mode(g_gpio_h, -1);
	TC_ASSERT_LT_CLEANUP("iotbus_gpio_set_drive_mode", ret, 0, iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_get_drive_mode_n
* @brief            gets the gpio drive mode
* @scenario         gets the gpio drive mode with invalid parameter
* @apicovered       iotbus_gpio_get_drive_mode
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_get_drive_mode_n(void)
{
	int ret;
	g_gpio_h = NULL;
	iotbus_gpio_drive_e get_drive;

	ret = iotbus_gpio_get_drive_mode(NULL, &get_drive);
	TC_ASSERT_EQ("iotbus_gpio_get_drive_mode", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_set_drive_mode_n_after_close
* @brief            sets the gpio drive mode
* @scenario         sets the gpio drive mode after gpio context close
* @apicovered       iotbus_gpio_set_drive_mode
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_set_drive_mode_n_after_close(void)
{
	int ret;
	g_gpio_h = NULL;
	iotbus_gpio_drive_e set_drive = IOTBUS_GPIO_DRIVE_PULLUP;

	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	ret = iotbus_gpio_set_drive_mode(g_gpio_h, set_drive);
	TC_ASSERT_LT("iotbus_gpio_set_drive_mode", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_gpio_get_drive_mode_n_after_close
* @brief            gets the gpio drive mode
* @scenario         gets the gpio drive mode after gpio context close
* @apicovered       iotbus_gpio_get_drive_mode
* @precondition     open gpio context
* @postcondition    close gpio context
*/
static void itc_systemio_gpio_get_drive_mode_n_after_close(void)
{
	int ret;
	g_gpio_h = NULL;
	iotbus_gpio_drive_e set_drive = IOTBUS_GPIO_DRIVE_PULLUP;
	iotbus_gpio_drive_e get_drive;

	g_gpio_h = iotbus_gpio_open(GPIO_PIN);
	TC_ASSERT_NEQ("iotbus_gpio_open", g_gpio_h, NULL);

	ret = iotbus_gpio_set_drive_mode(g_gpio_h, set_drive);
	TC_ASSERT_EQ_CLEANUP("iotbus_gpio_set_drive_mode", ret, 0, iotbus_gpio_close(g_gpio_h));

	ret = iotbus_gpio_close(g_gpio_h);
	TC_ASSERT_EQ("iotbus_gpio_close", ret, 0);

	ret = iotbus_gpio_get_drive_mode(g_gpio_h, &get_drive);
	TC_ASSERT_LT("iotbus_gpio_get_drive_mode", ret, 0);

	TC_SUCCESS_RESULT();
}

int itc_gpio_main(void)
{
	iotapi_initialize();

	itc_systemio_gpio_open_close_p();
	itc_systemio_gpio_get_pin_p();
	itc_systemio_gpio_set_get_direction_p();
	itc_systemio_gpio_set_get_edge_mode_p();
	itc_systemio_gpio_set_get_drive_mode_p();
	itc_systemio_gpio_read_write_p();
	itc_systemio_gpio_register_unregister_callback_p();//TC FAIL, callback is not invoked
	itc_systemio_gpio_set_direction_read_write_p_all_directions();
	itc_systemio_gpio_set_edge_read_write_p_all_edges();
	itc_systemio_gpio_set_drive_read_write_p_all_drive_mode();
	itc_systemio_gpio_set_get_direction_edge_drive_mode_p();
	itc_systemio_gpio_open_close_p_different_pin();
	itc_systemio_gpio_open_close_p_multi_handle();
	//itc_systemio_gpio_open_close_p_reclose();//TC FAIL, it should not return 0 on re-closing it. Also remaining TCs donot work after its execution
	itc_systemio_gpio_open_n();
	itc_systemio_gpio_close_n();
	itc_systemio_gpio_get_pin_n();
	itc_systemio_gpio_get_pin_n_after_close();//TC FAIL, it should not return opened pin after it has already been close
	itc_systemio_gpio_get_pin_n_after_invalid_open();
	itc_systemio_gpio_read_n_after_close();
	itc_systemio_gpio_write_n_after_close();
	itc_systemio_gpio_set_direction_n();
	itc_systemio_gpio_get_direction_n();
	itc_systemio_gpio_set_direction_n_after_close();
	itc_systemio_gpio_get_direction_n_after_close();//TC FAIL, ret value should be less than 0
	itc_systemio_gpio_set_edge_mode_n();
	itc_systemio_gpio_get_edge_mode_n();
	itc_systemio_gpio_set_edge_mode_n_after_close();
	itc_systemio_gpio_get_edge_mode_n_after_close();//TC FAIL, ret value should be less than 0
	itc_systemio_gpio_set_drive_mode_n();
	itc_systemio_gpio_get_drive_mode_n();
	itc_systemio_gpio_set_drive_mode_n_after_close();
	itc_systemio_gpio_get_drive_mode_n_after_close();//TC FAIL, ret value should be less than 0

	return 0;
}
