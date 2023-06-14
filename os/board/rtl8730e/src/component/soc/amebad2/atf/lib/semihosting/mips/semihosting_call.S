/*
 * Copyright (c) 2016, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2017, Realtek. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <asm_macros.S>

	.globl	semihosting_call

func semihosting_call
	jr	$ra
endfunc semihosting_call
