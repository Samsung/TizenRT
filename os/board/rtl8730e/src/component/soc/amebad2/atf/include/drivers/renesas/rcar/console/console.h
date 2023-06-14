/*
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RCAR_PRINTF_H
#define RCAR_PRINTF_H

#ifndef __ASSEMBLER__

#include <stdint.h>

/*
 * Initialize a new rcar console instance and register it with the console
 * framework. The |console| pointer must point to storage that will be valid
 * for the lifetime of the console, such as a global or static local variable.
 * Its contents will be reinitialized from scratch.
 */
int console_rcar_register(uintptr_t baseaddr, uint32_t clock, uint32_t baud,
			  console_t *console);

#endif /*__ASSEMBLER__*/

#endif /* RCAR_PRINTF_H */
