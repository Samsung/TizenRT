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

/// @file iotbus_spi.h

/// @brief Test Case Iotbus APIs for SPI

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <tinyara/time.h>
#include <sys/time.h>
#include "itc_internal.h"
#include <iotbus_spi.h>
#include <iotbus_error.h>

iotbus_spi_context_h spi;

unsigned int gBus = 0;

struct iotbus_spi_config_s _g_st_config = {
    (char)8,
    0,
    12000000,
    IOTBUS_SPI_MODE0,
};

/**
* @testcase         itc_iotbus_spi_open_close_p
* @brief            initializes and closes the spi_context
* @scenario         initializes and closes the spi_context
* @apicovered       iotbus_spi_open, iotbus_spi_close
* @precondition     none
* @postcondition    none
*/
void itc_iotbus_spi_open_close_p(void)
{
    int ret = IOTBUS_ERROR_NONE;
    iotbus_spi_context_h h_spi = iotbus_spi_open(gBus, &_g_st_config);
    TC_ASSERT_NEQ("iotbus_spi_open", h_spi, NULL);

    ret = iotbus_spi_close(h_spi);
    TC_ASSERT_EQ("iotbus_spi_close", ret, IOTBUS_ERROR_NONE);

    TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_iotbus_spi_write_recv_trasfer_p
* @brief            reads and writes data over spi bus, and transfers rx and tx data over spi bus
* @scenario         writes data over spi bus, reads data over spi bus and transfers rx and tx data over spi bus
* @apicovered       iotbus_spi_write, iotbus_spi_recv, iotbus_spi_transfer_buf
* @precondition     none
* @postcondition    none
*/
void itc_iotbus_spi_write_recv_trasfer_p(void)
{
    unsigned char sz_txbuf[64] = { 0, };
    unsigned char sz_rxbuf[64] = { 0, };
    int ret = IOTBUS_ERROR_NONE;
    iotbus_spi_context_h h_spi = iotbus_spi_open(gBus, &_g_st_config);
    TC_ASSERT_NEQ("iotbus_spi_open", h_spi, NULL);

	ret = iotbus_spi_write(h_spi, sz_txbuf, 8);
	TC_ASSERT_EQ_CLEANUP("iotbus_spi_write", ret, IOTBUS_ERROR_NONE, iotbus_spi_close(h_spi));

	ret = iotbus_spi_recv(h_spi, sz_rxbuf, 8);
	TC_ASSERT_EQ_CLEANUP("iotbus_spi_recv", ret, IOTBUS_ERROR_NONE, iotbus_spi_close(h_spi));

    ret = iotbus_spi_transfer_buf(h_spi, sz_txbuf, sz_rxbuf, 16);
#ifdef CONFIG_SPI_EXCHANGE
	TC_ASSERT_EQ_CLEANUP("iotbus_spi_transfer_buf", ret, IOTBUS_ERROR_NONE, iotbus_spi_close(h_spi));
#else
	TC_ASSERT_EQ_CLEANUP("iotbus_spi_transfer_buf", ret, IOTBUS_ERROR_NOT_SUPPORTED, iotbus_spi_close(h_spi));
#endif

    ret = iotbus_spi_close(h_spi);
    TC_ASSERT_EQ("iotbus_spi_close", ret, IOTBUS_ERROR_NONE);

    TC_SUCCESS_RESULT();
}

int itc_spi_main(void)
{
    itc_iotbus_spi_open_close_p();
    itc_iotbus_spi_write_recv_trasfer_p();
    return 0;
}
