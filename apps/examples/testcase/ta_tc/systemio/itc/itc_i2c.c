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
#include <errno.h>
#include "itc_internal.h"
#include <iotbus_i2c.h>
#include <iotbus_error.h>

int g_bus = 0;

/**
* @testcase         itc_systemio_i2c_init_stop_p
* @brief            initializes and closes i2c_context
* @scenario         initializes and closes i2c_context
* @apicovered       iotbus_i2c_init, iotbus_i2c_stop
* @precondition     none
* @postcondition    none
*/
void itc_systemio_i2c_init_stop_p(void)
{
	int ret = IOTBUS_ERROR_NONE;
	iotbus_i2c_context_h h_i2c = iotbus_i2c_init(g_bus);
	TC_ASSERT_NEQ("iotbus_i2c_init", h_i2c, NULL);

	ret = iotbus_i2c_stop(h_i2c);
	TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_i2c_set_frequency_p
* @brief            sets the i2c frequency
* @scenario         sets the i2c frequency
* @apicovered       iotbus_i2c_set_frequency
* @precondition     initializes i2c_context
* @postcondition    closes i2c_context
*/
void itc_systemio_i2c_set_frequency_p(void)
{
	int ret = IOTBUS_ERROR_NONE;
	int freq_mode[] = { IOTBUS_I2C_STD, IOTBUS_I2C_FAST, IOTBUS_I2C_HIGH };
	int num_modes = sizeof(freq_mode) / sizeof(int);
	int index = 0;
	bool check = true;
	iotbus_i2c_context_h h_i2c = iotbus_i2c_init(g_bus);
	TC_ASSERT_NEQ("iotbus_i2c_init", h_i2c, NULL);

	for (index = 0; index < num_modes; index++) {
		ret = iotbus_i2c_set_frequency(h_i2c, freq_mode[index]);
		if (ret != IOTBUS_ERROR_NONE) {
			check = false;
			SYSIO_ITC_PRINT("iotbus_i2c_set_frequency failed for frequency : %d for index : %d\n", freq_mode[index], index);
		}
	}
	TC_ASSERT_EQ_CLEANUP("itc_systemio_i2c_set_frequency_p", check, true, iotbus_i2c_stop(h_i2c));
	ret = iotbus_i2c_stop(h_i2c);
	TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_i2c_set_address_p
* @brief            sets the i2c slave address
* @scenario         sets the i2c slave address
* @apicovered       iotbus_i2c_set_address
* @precondition     initializes i2c_context
* @postcondition    closes i2c_context
*/
void itc_systemio_i2c_set_address_p(void)
{
	int ret = IOTBUS_ERROR_NONE;
	uint8_t address = 0x8;
	iotbus_i2c_context_h h_i2c = iotbus_i2c_init(g_bus);
	TC_ASSERT_NEQ("iotbus_i2c_init", h_i2c, NULL);

	ret = iotbus_i2c_set_address(h_i2c, address);
	TC_ASSERT_EQ_CLEANUP("iotbus_i2c_set_address", ret, IOTBUS_ERROR_NONE, iotbus_i2c_stop(h_i2c));

	ret = iotbus_i2c_stop(h_i2c);
	TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_i2c_write_read_p
* @brief            writes to i2c device and reads from i2c device
* @scenario         writes to i2c device and reads from i2c device
* @apicovered       iotbus_i2c_write, iotbus_i2c_read
* @precondition     initializes i2c_context
* @postcondition    closes i2c_context
*/
void itc_systemio_i2c_write_read_p(void)
{
	int ret = IOTBUS_ERROR_NONE;
	uint8_t write_cmd[2] = { 0x1, 0x2 };
	uint8_t address = 0x23;
	uint8_t read_buf;

	iotbus_i2c_context_h h_i2c = iotbus_i2c_init(g_bus);
	TC_ASSERT_NEQ("iotbus_i2c_init", h_i2c, NULL);

	ret = iotbus_i2c_set_address(h_i2c, address);
	TC_ASSERT_EQ_CLEANUP("iotbus_i2c_set_address", ret, IOTBUS_ERROR_NONE, iotbus_i2c_stop(h_i2c));

	ret = iotbus_i2c_write(h_i2c, &write_cmd[0], 1);
	TC_ASSERT_GEQ_CLEANUP("iotbus_i2c_write", ret, IOTBUS_ERROR_NONE, iotbus_i2c_stop(h_i2c));

	ret = iotbus_i2c_read(h_i2c, &read_buf, 1);
	TC_ASSERT_GEQ_CLEANUP("iotbus_i2c_read", ret, IOTBUS_ERROR_NONE, iotbus_i2c_stop(h_i2c));

	ret = iotbus_i2c_write(h_i2c, &write_cmd[1], 1);
	TC_ASSERT_GEQ_CLEANUP("iotbus_i2c_write", ret, IOTBUS_ERROR_NONE, iotbus_i2c_stop(h_i2c));

	ret = iotbus_i2c_read(h_i2c, &read_buf, 1);
	TC_ASSERT_GEQ_CLEANUP("iotbus_i2c_read", ret, IOTBUS_ERROR_NONE, iotbus_i2c_stop(h_i2c));

	ret = iotbus_i2c_stop(h_i2c);
	TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_i2c_set_frequency_address_p
* @brief            sets the i2c frequency and address
* @scenario         sets the i2c frequency and address
* @apicovered       iotbus_i2c_set_frequency, iotbus_i2c_set_address
* @precondition     initializes i2c_context
* @postcondition    closes i2c_context
*/
void itc_systemio_i2c_set_frequency_address_p(void)
{
	int ret = IOTBUS_ERROR_NONE;
	uint8_t address = 0x8;
	int freq_mode[] = { IOTBUS_I2C_STD, IOTBUS_I2C_FAST, IOTBUS_I2C_HIGH };
	int num_modes = sizeof(freq_mode) / sizeof(freq_mode[0]);
	int index = 0;
	bool check = true;
	iotbus_i2c_context_h h_i2c = iotbus_i2c_init(g_bus);
	TC_ASSERT_NEQ("iotbus_i2c_init", h_i2c, NULL);

	for (index = 0; index < num_modes; index++) {
		ret = iotbus_i2c_set_frequency(h_i2c, freq_mode[index]);
		if (ret != IOTBUS_ERROR_NONE) {
			check = false;
			SYSIO_ITC_PRINT("\nitc_systemio_i2c_set_frequency_address_p: iotbus_i2c_set_frequency FAIL for frequency : %d\n", freq_mode[index]);
			continue;
		}

		ret = iotbus_i2c_set_address(h_i2c, address);
		if (ret != IOTBUS_ERROR_NONE) {
			check = false;
			SYSIO_ITC_PRINT("\nitc_systemio_i2c_set_frequency_address_p: iotbus_i2c_set_address FAIL for frequency : %d\n", freq_mode[index]);
		}
	}

	TC_ASSERT_EQ_CLEANUP("itc_systemio_i2c_set_frequency_address_p", check, true, iotbus_i2c_stop(h_i2c));
	ret = iotbus_i2c_stop(h_i2c);
	TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_i2c_write_read_p_all_freq
* @brief            writes to i2c device and reads from i2c device
* @scenario         writes to i2c device and reads from i2c device
* @apicovered       iotbus_i2c_write, iotbus_i2c_read
* @precondition     initializes i2c_context
* @postcondition    closes i2c_context
*/
void itc_systemio_i2c_write_read_p_all_freq(void)
{
	int ret = IOTBUS_ERROR_NONE;
	uint8_t write_cmd[2] = { 0x1, 0x2 };
	uint8_t read_buf[2];
	uint8_t address = 0x23;
	int freq_mode[] = { IOTBUS_I2C_STD, IOTBUS_I2C_FAST, IOTBUS_I2C_HIGH };
	int num_modes = sizeof(freq_mode) / sizeof(freq_mode[0]);
	int index = 0;
	bool check = true;
	iotbus_i2c_context_h h_i2c;

	for (index = 0; index < num_modes; index++) {
		h_i2c = iotbus_i2c_init(g_bus);
		if (h_i2c == NULL) {
			check = false;
			SYSIO_ITC_PRINT("\nitc_systemio_i2c_write_read_p_all_freq: iotbus_i2c_init FAIL for init for index : %d\n", index);
			continue;
		}
		TC_ASSERT_NEQ("iotbus_i2c_init", h_i2c, NULL);

		ret = iotbus_i2c_set_frequency(h_i2c, freq_mode[index]);
		if (ret != IOTBUS_ERROR_NONE) {
			check = false;
			SYSIO_ITC_PRINT("\nitc_systemio_i2c_write_read_p_all_freq: iotbus_i2c_set_frequency FAIL for frequency : %d for index : %d\n", freq_mode[index], index);
			ret = iotbus_i2c_stop(h_i2c);
			TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);
			continue;
		}

		ret = iotbus_i2c_set_address(h_i2c, address);
		if (ret != IOTBUS_ERROR_NONE) {
			check = false;
			SYSIO_ITC_PRINT("\nitc_systemio_i2c_write_read_p_all_freq: iotbus_i2c_set_address FAIL for frequency : %d for index : %d\n", freq_mode[index], index);
			ret = iotbus_i2c_stop(h_i2c);
			TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);
			continue;
		}

		ret = iotbus_i2c_write(h_i2c, write_cmd, 2);
		if (ret < IOTBUS_ERROR_NONE) {
			check = false;
			SYSIO_ITC_PRINT("\nitc_systemio_i2c_write_read_p_all_freq: iotbus_i2c_write FAIL for frequency : %d for index : %d\n", freq_mode[index], index);
			ret = iotbus_i2c_stop(h_i2c);
			TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);
			continue;
		}

		ret = iotbus_i2c_read(h_i2c, read_buf, 2);
		if (ret < IOTBUS_ERROR_NONE) {
			check = false;
			SYSIO_ITC_PRINT("\nitc_systemio_i2c_write_read_p_all_freq: iotbus_i2c_read FAIL for frequency : %d for index : %d\n", freq_mode[index], index);
			ret = iotbus_i2c_stop(h_i2c);
			TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);
			continue;
		}
		ret = iotbus_i2c_stop(h_i2c);
		TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);
	}
	TC_ASSERT_EQ("itc_systemio_i2c_write_read_p_all_freq", check, true);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_i2c_init_n
* @brief            initializes i2c_context
* @scenario         initializes i2c_context with invalid parameter
* @apicovered       iotbus_i2c_init
* @precondition     none
* @postcondition    none
*/
void itc_systemio_i2c_init_n(void)
{
	iotbus_i2c_context_h h_i2c = iotbus_i2c_init(-1);//invalid bus
	TC_ASSERT_EQ("iotbus_i2c_init", h_i2c, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_i2c_stop_n
* @brief            stop i2c_context
* @scenario         stop i2c_context with invalid parameter
* @apicovered       iotbus_i2c_stop
* @precondition     none
* @postcondition    none
*/
void itc_systemio_i2c_stop_n(void)
{
	int ret = iotbus_i2c_stop(NULL);//invalid handle
	TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_i2c_set_frequency_n
* @brief            sets the i2c frequency
* @scenario         sets the i2c frequency with invalid handle
* @apicovered       iotbus_i2c_set_frequency
* @precondition     initializes i2c_context
* @postcondition    closes i2c_context
*/
void itc_systemio_i2c_set_frequency_n(void)
{
	int ret = IOTBUS_ERROR_NONE;
	int freq_mode = IOTBUS_I2C_STD;

	ret = iotbus_i2c_set_frequency(NULL, freq_mode);//invalid handle
	TC_ASSERT_EQ("iotbus_i2c_set_frequency", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_i2c_set_address_n
* @brief            sets the i2c slave address
* @scenario         sets the i2c slave address with invalid parameter
* @apicovered       iotbus_i2c_set_address
* @precondition     initializes i2c_context
* @postcondition    closes i2c_context
*/
void itc_systemio_i2c_set_address_n(void)
{
	int ret = IOTBUS_ERROR_NONE;
	uint8_t address = 0x3;

	ret = iotbus_i2c_set_address(NULL, address);
	TC_ASSERT_EQ("iotbus_i2c_set_address", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_i2c_set_address_n_after_stop
* @brief            sets the i2c slave address
* @scenario         sets the i2c slave address
* @apicovered       iotbus_i2c_set_address
* @precondition     initializes i2c_context
* @postcondition    closes i2c_context
*/
void itc_systemio_i2c_set_address_n_after_stop(void)
{
	int ret = IOTBUS_ERROR_NONE;
	uint8_t address = 0x8;
	iotbus_i2c_context_h h_i2c = iotbus_i2c_init(g_bus);
	TC_ASSERT_NEQ("iotbus_i2c_init", h_i2c, NULL);

	ret = iotbus_i2c_stop(h_i2c);
	TC_ASSERT_EQ("iotbus_i2c_stop", ret, IOTBUS_ERROR_NONE);

	ret = iotbus_i2c_set_address(h_i2c, address);
	TC_ASSERT_EQ("iotbus_i2c_set_address", ret, IOTBUS_ERROR_NOT_SUPPORTED);

	TC_SUCCESS_RESULT();
}

int itc_i2c_main(void)
{
	itc_systemio_i2c_init_stop_p();
	itc_systemio_i2c_set_frequency_p();
	itc_systemio_i2c_set_address_p();
#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
	itc_systemio_i2c_write_read_p(); // Require hardware to perform write/read operation
	itc_systemio_i2c_write_read_p_all_freq();//TC FAIL
#endif
	itc_systemio_i2c_set_frequency_address_p();
	itc_systemio_i2c_init_n();
	itc_systemio_i2c_stop_n();
	itc_systemio_i2c_set_frequency_n();
	itc_systemio_i2c_set_address_n();
	//itc_systemio_i2c_set_address_n_after_stop();//System hangs on execution of this TC

	return 0;
}
