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
#include <iotbus/iotbus_i2c.h>
#include <iotbus/iotbus_error.h>
#include "utc_internal.h"

iotbus_i2c_context_h i2c;

static void utc_systemio_i2c_init_p(void)
{
	iotbus_i2c_context_h m_i2c = iotbus_i2c_init(0);
	TC_ASSERT_NEQ("iotbus_i2c_init", m_i2c, NULL);
	i2c = m_i2c;
	TC_SUCCESS_RESULT();
}

static void utc_systemio_i2c_init_n(void)
{
	TC_ASSERT_EQ("iotbus_i2c_init", iotbus_i2c_init(-1), NULL);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_i2c_set_frequency_p_IOTBUS_I2C_FAST(void)
{
	TC_ASSERT_EQ("iotbus_i2c_set_frequency", iotbus_i2c_set_frequency(i2c, IOTBUS_I2C_FAST), IOTBUS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_i2c_set_frequency_p_IOTBUS_I2C_HIGH(void)
{
	TC_ASSERT_EQ("iotbus_i2c_set_frequency", iotbus_i2c_set_frequency(i2c, IOTBUS_I2C_HIGH), IOTBUS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_i2c_set_frequency_p_IOTBUS_I2C_STD(void)
{
	TC_ASSERT_EQ("iotbus_i2c_set_frequency", iotbus_i2c_set_frequency(i2c, IOTBUS_I2C_STD), IOTBUS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_i2c_set_frequency_n(void)
{
	TC_ASSERT_EQ("iotbus_i2c_set_frequency", iotbus_i2c_set_frequency(i2c, 5), IOTBUS_ERROR_NOT_SUPPORTED);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_i2c_set_address_p(void)
{
	TC_ASSERT_EQ("iotbus_i2c_set_address", iotbus_i2c_set_address(i2c, 0x23), IOTBUS_ERROR_NONE);	
	TC_SUCCESS_RESULT();
}

static void utc_systemio_i2c_set_address_n(void)
{
	TC_ASSERT_EQ("iotbus_i2c_set_address", iotbus_i2c_set_address(i2c, 0xff), IOTBUS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
static void utc_systemio_i2c_write_p(void)
{
	uint8_t cmd[1] = { 0x01};
	TC_ASSERT_GT("iotbus_i2c_write", iotbus_i2c_write(i2c, cmd, 1), IOTBUS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}
#endif

static void utc_systemio_i2c_write_n(void)
{
	TC_ASSERT_EQ("iotbus_i2c_write", iotbus_i2c_write(i2c, NULL, 1), IOTBUS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
static void utc_systemio_i2c_read_p(void)
{
	uint8_t buf[1];
	TC_ASSERT_GT("iotbus_i2c_read", iotbus_i2c_read(i2c, buf, 1), IOTBUS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}
#endif

static void utc_systemio_i2c_read_n(void)
{
	TC_ASSERT_EQ("iotbus_i2c_read", iotbus_i2c_read(i2c, NULL, 1), IOTBUS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_i2c_stop_p(void)
{
	TC_ASSERT_EQ("iotbus_i2c_stop", iotbus_i2c_stop(NULL), IOTBUS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_i2c_stop_n(void)
{
	TC_ASSERT_EQ("iotbus_i2c_stop", iotbus_i2c_stop(i2c), IOTBUS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

int utc_i2c_main(void)
{
	utc_systemio_i2c_init_p();
	utc_systemio_i2c_init_n();
	utc_systemio_i2c_set_frequency_n();
	utc_systemio_i2c_set_frequency_p_IOTBUS_I2C_FAST();
	utc_systemio_i2c_set_frequency_p_IOTBUS_I2C_HIGH();
	utc_systemio_i2c_set_frequency_p_IOTBUS_I2C_STD();
	utc_systemio_i2c_set_address_p();
	utc_systemio_i2c_set_address_n();
#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
	utc_systemio_i2c_write_p(); //(Manual TCs) Require hardware to perform write/read operation
	utc_systemio_i2c_read_p();
#endif
	utc_systemio_i2c_write_n();
	utc_systemio_i2c_read_n();
	utc_systemio_i2c_stop_n();
	utc_systemio_i2c_stop_p();

	return 0;
}
