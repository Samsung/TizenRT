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

/// @file itc_spi.c

/// @brief Test Case Iotbus APIs for SPI

#include <tinyara/config.h>
#include "itc_internal.h"
#include <iotbus_spi.h>
#include <iotbus_error.h>

static unsigned int g_bus = 0;

static struct iotbus_spi_config_s g_st_config = {
	(char)8,
	0,
	12000000,
	IOTBUS_SPI_MODE0,
};

/**
* @testcase         itc_systemio_spi_open_close_p
* @brief            initializes and closes the spi_context
* @scenario         initializes and closes the spi_context
* @apicovered       iotbus_spi_open, iotbus_spi_close
* @precondition     none
* @postcondition    none
*/
void itc_systemio_spi_open_close_p(void)
{
	int ret = IOTBUS_ERROR_NONE;
	iotbus_spi_context_h h_spi = iotbus_spi_open(g_bus, &g_st_config);
	TC_ASSERT_NEQ("iotbus_spi_open", h_spi, NULL);

	ret = iotbus_spi_close(h_spi);
	TC_ASSERT_EQ("iotbus_spi_close", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_spi_open_close_p_multi_handler
* @brief            initializes and closes the spi_context with multi handler
* @scenario         initializes and closes the spi_context with multi handler
* @apicovered       iotbus_spi_open, iotbus_spi_close
* @precondition     none
* @postcondition    none
*/
static void itc_systemio_spi_open_close_p_multi_handler(void)
{
	int ret = IOTBUS_ERROR_NONE;
	iotbus_spi_context_h h_spi_1 = iotbus_spi_open(g_bus, &g_st_config);
	TC_ASSERT_NEQ("iotbus_spi_open", h_spi_1, NULL);

	iotbus_spi_context_h h_spi_2 = iotbus_spi_open(g_bus, &g_st_config);
	TC_ASSERT_NEQ_CLEANUP("iotbus_spi_open", h_spi_2, NULL, iotbus_spi_close(h_spi_1));

	ret = iotbus_spi_close(h_spi_2);
	TC_ASSERT_EQ_CLEANUP("iotbus_spi_close", ret, IOTBUS_ERROR_NONE, iotbus_spi_close(h_spi_1));

	ret = iotbus_spi_close(h_spi_1);
	TC_ASSERT_EQ("iotbus_spi_close", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_spi_open_close_p_modes
* @brief            initializes and closes the spi_context for modes
* @scenario         initializes and closes the spi_context for modes
* @apicovered       iotbus_spi_open, iotbus_spi_close
* @precondition     none
* @postcondition    none
*/
static void itc_systemio_spi_open_close_p_modes(void)
{
	int ret = IOTBUS_ERROR_NONE;
	int max_index = 4;
	int check_open = 0;
	int check_close = 0;
	int index = 0;
	struct iotbus_spi_config_s st_config = {
		(char)8,
		0,
		12000000,
		IOTBUS_SPI_MODE0,
	};

	int modes[4] = { IOTBUS_SPI_MODE0, IOTBUS_SPI_MODE1, IOTBUS_SPI_MODE2, IOTBUS_SPI_MODE2 };
	char *str_modes[4] = { "IOTBUS_SPI_MODE0", "IOTBUS_SPI_MODE1", "IOTBUS_SPI_MODE2", "IOTBUS_SPI_MODE3" };

	for (index = 0; index < max_index; ++index) {
		st_config.mode = modes[index];

		iotbus_spi_context_h h_spi = iotbus_spi_open(g_bus, &st_config);
		if (h_spi == NULL) {
			SYSIO_ITC_PRINT("iotbus_spi_open failed, mode :: %s \n", str_modes[index]);
			check_open++;
			continue;
		}

		ret = iotbus_spi_close(h_spi);
		if (h_spi == NULL) {
			SYSIO_ITC_PRINT("iotbus_spi_close failed, mode :: %s \n", str_modes[index]);
			check_close++;
			continue;
		}

		SYSIO_ITC_PRINT("spi open/close  :: %s \n", str_modes[index]);
	}

	TC_ASSERT_EQ("iotbus_spi_open failed", check_open, 0);
	TC_ASSERT_EQ("iotbus_spi_close failed", check_close, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_spi_open_close_n_invalid_bus
* @brief            initializes and closes the spi_context
* @scenario         initializes and closes the spi_context
* @apicovered       iotbus_spi_open, iotbus_spi_close
* @precondition     none
* @postcondition    none
*/
static void itc_systemio_spi_open_close_n_invalid_bus(void)
{
	int ret = IOTBUS_ERROR_NONE;
	int invalid_bus = -1;
	iotbus_spi_context_h h_spi = iotbus_spi_open(invalid_bus, &g_st_config);
	TC_ASSERT_EQ("iotbus_spi_open", h_spi, NULL);

	ret = iotbus_spi_close(h_spi);
	TC_ASSERT_EQ("iotbus_spi_close", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_spi_open_close_n_invalid_bpw
* @brief            initializes and closes the spi_context
* @scenario         initializes and closes the spi_context
* @apicovered       iotbus_spi_open, iotbus_spi_close
* @precondition     none
* @postcondition    none
*/
static void itc_systemio_spi_open_close_n_invalid_bpw(void)
{
	int ret = IOTBUS_ERROR_NONE;

	struct iotbus_spi_config_s st_config = {
		(char)1,
		0,
		12000000,
		IOTBUS_SPI_MODE0,
	};

	iotbus_spi_context_h h_spi = iotbus_spi_open(g_bus, &st_config);
	TC_ASSERT_EQ("iotbus_spi_open", h_spi, NULL);

	ret = iotbus_spi_close(h_spi);
	TC_ASSERT_EQ("iotbus_spi_close", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_spi_open_close_n_invalid_frequecy
* @brief            initializes and closes the spi_context
* @scenario         initializes and closes the spi_context
* @apicovered       iotbus_spi_open, iotbus_spi_close
* @precondition     none
* @postcondition    none
*/
static void itc_systemio_spi_open_close_n_invalid_frequecy(void)
{
	int ret = IOTBUS_ERROR_NONE;

	struct iotbus_spi_config_s st_config = {
		(char)8,
		0,
		13000000, //13Mhz Invalid frequency
		IOTBUS_SPI_MODE0,
	};

	iotbus_spi_context_h h_spi = iotbus_spi_open(g_bus, &st_config);
	TC_ASSERT_EQ("iotbus_spi_open", h_spi, NULL);

	ret = iotbus_spi_close(h_spi);
	TC_ASSERT_EQ("iotbus_spi_close", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_spi_write_recv_trasfer_p
* @brief            reads and writes data over spi bus, and transfers rx and tx data over spi bus
* @scenario         writes data over spi bus, reads data over spi bus and transfers rx and tx data over spi bus
* @apicovered       iotbus_spi_write, iotbus_spi_recv, iotbus_spi_transfer_buf
* @precondition     none
* @postcondition    none
*/
void itc_systemio_spi_write_recv_trasfer_p(void)
{
	unsigned char sz_txbuf[64] = { 0, };
	unsigned char sz_rxbuf[64] = { 0, };
	int ret = IOTBUS_ERROR_NONE;
	iotbus_spi_context_h h_spi = iotbus_spi_open(g_bus, &g_st_config);
	TC_ASSERT_NEQ("iotbus_spi_open", h_spi, NULL);

	ret = iotbus_spi_write(h_spi, sz_txbuf, 8);
	TC_ASSERT_EQ_CLEANUP("iotbus_spi_write", ret, IOTBUS_ERROR_NONE, iotbus_spi_close(h_spi));

	ret = iotbus_spi_recv(h_spi, sz_rxbuf, 8);
	TC_ASSERT_EQ_CLEANUP("iotbus_spi_recv", ret, IOTBUS_ERROR_NONE, iotbus_spi_close(h_spi));

#ifdef CONFIG_SPI_EXCHANGE
	ret = iotbus_spi_transfer_buf(h_spi, sz_txbuf, sz_rxbuf, 16);
	TC_ASSERT_EQ_CLEANUP("iotbus_spi_transfer_buf", ret, IOTBUS_ERROR_NONE, iotbus_spi_close(h_spi));
#endif

	ret = iotbus_spi_close(h_spi);
	TC_ASSERT_EQ("iotbus_spi_close", ret, IOTBUS_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

int itc_spi_main(void)
{
	itc_systemio_spi_open_close_p();
	itc_systemio_spi_open_close_p_multi_handler();
	itc_systemio_spi_open_close_p_modes();
	itc_systemio_spi_open_close_n_invalid_bus();
	itc_systemio_spi_open_close_n_invalid_bpw();
	itc_systemio_spi_open_close_n_invalid_frequecy();
	itc_systemio_spi_write_recv_trasfer_p();
	return 0;
}
