/**
 ******************************************************************************
 * @file    quick_uart.h
 * @brief   This file provides all the headers of UART operation functions for 
            quick_track debug.
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

#include <os/os.h>
#include <driver/hal/hal_uart_types.h>

bk_err_t quicktrack_uart_init(uart_id_t uart);
bk_err_t quicktrack_uart_deinit(uart_id_t uart);
bk_err_t quicktrack_uart_send(uart_id_t uart, const void *data, uint32_t size);
bk_err_t quicktrack_uart_recv(uart_id_t uart, const void *data, uint32_t size, uint32_t timeout);

#endif




