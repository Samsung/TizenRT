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

/// @file clock.c

/// @brief Test Case Example for Clock API

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <tinyara/time.h>
#include <sys/time.h>
#include "itc_internal.h"
#include <iotbus_i2c.h>
#include <iotbus_error.h>

int g_bus = 1;

/**
* @testcase         itc_iotbus_i2c_init_stop_p
* @brief            initializes and closes i2c_context
* @scenario         initializes and closes i2c_context
* @apicovered       iotbus_i2c_init, iotbus_i2c_stop
* @precondition     none
* @postcondition    none
*/
void itc_iotbus_i2c_init_stop_p(void)
{
	int ret = IOTBUS_ERROR_NONE;
	iotbus_i2c_context_h h_i2c = iotbus_i2c_init(g_bus);
	TC_ASSERT_NEQ("iotbus_i2c_init", h_i2c, NULL);

	ret = iotbus_i2c_stop(h_i2c);
	TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_iotbus_i2c_set_frequency_p
* @brief            sets the i2c frequency
* @scenario         sets the i2c frequency
* @apicovered       iotbus_i2c_set_frequency
* @precondition     initializes i2c_context
* @postcondition    closes i2c_context
*/
void itc_iotbus_i2c_set_frequency_p(void)
{
	int ret = IOTBUS_ERROR_NONE;
	int szFreqMode[] = { IOTBUS_I2C_STD, IOTBUS_I2C_FAST, IOTBUS_I2C_HIGH };
	int numModes = sizeof(szFreqMode) / sizeof(int);
	int index = 0;
	iotbus_i2c_context_h h_i2c = iotbus_i2c_init(g_bus);
	TC_ASSERT_NEQ("iotbus_i2c_init", h_i2c, NULL);

	for (index = 0; index < numModes; index++) {
		ret = iotbus_i2c_set_frequency(h_i2c, szFreqMode[index]);
		TC_ASSERT_EQ_CLEANUP("iotbus_i2c_set_frequency", ret, IOTBUS_ERROR_NONE, iotbus_i2c_stop(h_i2c));
	}

	ret = iotbus_i2c_stop(h_i2c);
	TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_iotbus_i2c_set_address_p
* @brief            sets the i2c slave address
* @scenario         sets the i2c slave address
* @apicovered       iotbus_i2c_set_address
* @precondition     initializes i2c_context
* @postcondition    closes i2c_context
*/
void itc_iotbus_i2c_set_address_p(void)
{
	int ret = IOTBUS_ERROR_NONE;
	uint8_t address = 0x08;
	iotbus_i2c_context_h h_i2c = iotbus_i2c_init(g_bus);
	TC_ASSERT_NEQ("iotbus_i2c_init", h_i2c, NULL);

	ret = iotbus_i2c_set_address(h_i2c, address);
	TC_ASSERT_EQ_CLEANUP("iotbus_i2c_set_address", ret, IOTBUS_ERROR_NONE, iotbus_i2c_stop(h_i2c));

	ret = iotbus_i2c_stop(h_i2c);
	TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_iotbus_i2c_write_read_p
* @brief            writes to i2c device and reads from i2c device
* @scenario         writes to i2c device and reads from i2c device
* @apicovered       iotbus_i2c_write, iotbus_i2c_read
* @precondition     initializes i2c_context
* @postcondition    closes i2c_context
*/
void itc_iotbus_i2c_write_read_p(void)
{
	int ret = IOTBUS_ERROR_NONE;
	uint8_t szCmd[2] = { 0x01, 0x02 };
	uint8_t szReadBuf[10];
	iotbus_i2c_context_h h_i2c = iotbus_i2c_init(g_bus);
	TC_ASSERT_NEQ("iotbus_i2c_init", h_i2c, NULL);

	ret = iotbus_i2c_write(h_i2c, szCmd, 1);
	TC_ASSERT_EQ_CLEANUP("iotbus_i2c_write", (ret < 0), false, iotbus_i2c_stop(h_i2c));

	ret = iotbus_i2c_read(h_i2c, szReadBuf, 10);
	TC_ASSERT_GT_CLEANUP("iotbus_i2c_read", ret, IOTBUS_ERROR_NONE, iotbus_i2c_stop(h_i2c));

	ret = iotbus_i2c_stop(h_i2c);
	TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

int itc_i2c_main(void)
{
	itc_iotbus_i2c_init_stop_p();
	itc_iotbus_i2c_set_frequency_p();
	itc_iotbus_i2c_set_address_p();
	itc_iotbus_i2c_write_read_p();
	return 0;
}
