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
#include <iotbus/iotbus_spi.h>
#include <iotbus/iotbus_error.h>
#include "utc_internal.h"

iotbus_spi_context_h spi;

unsigned int bus = 1;

unsigned char txbuf[64] = { 0, };
unsigned char rxbuf[64] = { 0, };;

struct iotbus_spi_config_s config = {
	(char)8,
	0,
	12000000,
	0,
};

static void utc_systemio_spi_open_p(void)
{
	iotbus_spi_context_h m_spi = iotbus_spi_open(bus, &config);
	TC_ASSERT_NEQ("iotbus_spi_open", m_spi, NULL);
	spi = m_spi;
	TC_SUCCESS_RESULT();
}

static void utc_systemio_spi_open_n(void)
{
	iotbus_spi_context_h m_spi = iotbus_spi_open(bus, NULL);
	TC_ASSERT_EQ("iotbus_spi_open", m_spi, NULL);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_spi_write_p(void)
{
	int ret = iotbus_spi_write(spi, txbuf, 8);
	TC_ASSERT_EQ("iotbus_spi_write", ret, IOTBUS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_spi_write_n(void)
{
	int ret = iotbus_spi_write(spi, NULL, 0);
	TC_ASSERT_EQ("iotbus_spi_write", ret, IOTBUS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_spi_recv_p(void)
{
	int ret = iotbus_spi_recv(spi, rxbuf, 8);
	TC_ASSERT_EQ("iotbus_spi_recv", ret, IOTBUS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_spi_recv_n(void)
{
	int ret = iotbus_spi_recv(spi, NULL, -1);
	TC_ASSERT_EQ("iotbus_spi_recv", ret, IOTBUS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_spi_transfer_p(void)
{
	//TO DO
}

static void utc_systemio_spi_transfer_n(void)
{
	//TO DO
}

static void utc_systemio_spi_close_p(void)
{
	int ret = iotbus_spi_close(spi);
	TC_ASSERT_EQ("iotbus_spi_close", ret, IOTBUS_ERROR_NONE);
	TC_SUCCESS_RESULT();

}

static void utc_systemio_spi_close_n(void)
{
	int ret = iotbus_spi_close(NULL);
	TC_ASSERT_EQ("iotbus_spi_close", ret, IOTBUS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

int utc_spi_main(void)
{
	SYSIO_UTC_I2C_PRINT("## SPI SYSIO Test##\n");
	utc_systemio_spi_open_p();
	utc_systemio_spi_open_n();
	utc_systemio_spi_write_p();
	utc_systemio_spi_write_n();
	utc_systemio_spi_recv_p();
	utc_systemio_spi_recv_n();
	utc_systemio_spi_transfer_p();
	utc_systemio_spi_transfer_n();
	utc_systemio_spi_close_n();
	utc_systemio_spi_close_p();	

	return 0;
}
