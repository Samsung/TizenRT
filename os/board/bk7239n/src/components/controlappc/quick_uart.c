/**
 ******************************************************************************
 * @file    quick_uart.c
 * @brief   This file provides all the headers of UART operation functions.
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2017 BEKEN Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 */
#ifndef __QUICKTRACKUART_H__
#define __QUICKTRACKUART_H__

#include "quick_uart.h"
#include <driver/uart.h>
#include "uart_statis.h"
#include "bk_uart.h"
#include <components/log.h>
#include <os/mem.h>

#define UART_TAG "QuickTrack_Uart"
#define UART_LOGI(...) BK_LOGI(UART_TAG, ##__VA_ARGS__)
#define UART_LOGW(...) BK_LOGW(UART_TAG, ##__VA_ARGS__)
#define UART_LOGE(...) BK_LOGE(UART_TAG, ##__VA_ARGS__)
#define UART_LOGD(...) BK_LOGD(UART_TAG, ##__VA_ARGS__)

#define QuickT_UART_GET_PORT_NUMBER(port_id) ((port_id) & 0xFFFF)

/**
 * @brief quicktrack uart init
 * 
 */
bk_err_t quicktrack_uart_init(uart_id_t uart_id)
{
	int port_num = QuickT_UART_GET_PORT_NUMBER(uart_id);
	uart_id_t port;

	const uart_config_t quickcfg = {
		.baud_rate = UART_BAUD_RATE,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_NONE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_FLOWCTRL_DISABLE,
		.src_clk = UART_SCLK_XTAL_26M
		};

	UART_LOGI("QuickTrack_UART_Init: port_num(%d).\n", port_num);

	if(CONFIG_UART_PRINT_PORT == port_num)
	{
		UART_LOGI("QuickTrack_UART_Init: print port already init.\n");
		return BK_FAIL;
	}else if (1 == port_num)
	{
		port = UART_ID_1;
	}else if (2 == port_num)
	{
		port = UART_ID_2;
	}else
	{
		return BK_FAIL;
	}

	bk_uart_init(port, &quickcfg);
	bk_uart_deinit(port);
	bk_uart_init(port, &quickcfg);

	return BK_OK;
	
}

/**
 * @brief quicktrack uart deinit
 * 
 */
 bk_err_t quicktrack_uart_deinit(uart_id_t uart_id)
{
	int port_num = QuickT_UART_GET_PORT_NUMBER(uart_id);
	uart_id_t port;

	if(CONFIG_UART_PRINT_PORT == port_num)
	{
		UART_LOGI("QuickTrack_UART_Init: print port already init.\n");
		return kGeneralErr;
	}else if(1 == port_num)
	{
		port = UART_ID_1;
	}else if(2 == port_num)
	{
		port = UART_ID_2;
	}else
	{
		return BK_FAIL;
	}

	bk_uart_deinit(port);
	
	return BK_OK;

}

 /**
  * @brief quicktrack uart write data
  * 
  */
bk_err_t  quicktrack_uart_send(uart_id_t uart_id, const void *data, uint32_t size)
{
	uart_id_t port;
	const uint8_t *b = data;
	
	if (1 == QuickT_UART_GET_PORT_NUMBER(uart_id))
	{
		port = UART_ID_1;
	}else if(2 == QuickT_UART_GET_PORT_NUMBER(uart_id))
	{
		port = UART_ID_2;
	}else
	{
		return BK_FAIL;
	}

	for (int i =0; i < size; i++){
		uart_write_byte(port, b[i]);
	}

	/* Keep ACK and response as separate UART bursts for agent.py. */
	rtos_delay_milliseconds(200);
	return BK_OK;
}

/**
 * @brief quicktrack uart read data
 * 
 */
bk_err_t quicktrack_uart_recv(uart_id_t uart_id, void *data, uint32_t size, uint32_t timeout)
{
	uart_id_t port;
	if(1 == QuickT_UART_GET_PORT_NUMBER(uart_id))
	{
		port = UART_ID_1;
	}else if(2 == QuickT_UART_GET_PORT_NUMBER(uart_id))
	{
		port = UART_ID_2;
	}else
	{
		return BK_FAIL;
	}

	uint32_t length = bk_uart_read_bytes(port, data, size, timeout);
	
	return length;
}

#endif

