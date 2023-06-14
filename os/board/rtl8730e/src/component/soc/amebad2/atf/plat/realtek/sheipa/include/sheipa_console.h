/*
 * Copyright (c) 2019, Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SHEIPA_UART_H
#define SHEIPA_UART_H

#include <drivers/console.h>

#define CONSOLE_T_UART_BASE	CONSOLE_T_DRVDATA

#ifndef __ASSEMBLER__

typedef struct {
	console_t console;
	uintptr_t base;
} console_uart_t;

int console_core_register(uintptr_t baseaddr, uint32_t clock, uint32_t baud,
			   console_uart_t *console);
#endif /*__ASSEMBLER__*/

#endif  /* SHEIPA_UART_H */
