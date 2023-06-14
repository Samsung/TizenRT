/*
 * Copyright (c) 2018, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef STM32_CONSOLE_H
#define STM32_CONSOLE_H

#include <drivers/console.h>

#ifndef __ASSEMBLER__

#include <stdint.h>

/*
 * Initialize a new STM32 console instance and register it with the console
 * framework. The |console| pointer must point to storage that will be valid
 * for the lifetime of the console, such as a global or static local variable.
 * Its contents will be reinitialized from scratch.
 */
int console_stm32_register(uintptr_t baseaddr, uint32_t clock, uint32_t baud,
			   console_t *console);

#endif /*__ASSEMBLER__*/

#endif /* STM32_CONSOLE_H */
