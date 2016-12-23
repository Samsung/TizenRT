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

#ifndef _UART_H
#define _UART_H

#include <stdint.h>

enum uart_baudrate {
	UART_38400,
	UART_57600,
	UART_115200,
	UART_230400,
	UART_460800,
	UART_614400,
	UART_921600,
};

void uart_init(uint8_t uart, uint8_t interrupts);
void uart_putchar_wait(uint8_t uart, int c);
int uart_putchar_nb(uint8_t uart, int c);
int uart_getchar_nb(uint8_t uart, uint8_t *ch);
int uart_tx_busy(uint8_t uart);
int uart_baudrate(uint8_t uart, enum uart_baudrate bdrt);

enum uart_irq {
	UART_IRQ_TX_EMPTY,
	UART_IRQ_RX_CHAR,
};

void uart_irq_enable(uint8_t uart, enum uart_irq irq, int on);

void uart_poll(uint8_t uart);

#endif							/* _UART_H */
