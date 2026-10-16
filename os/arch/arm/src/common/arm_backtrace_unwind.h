/****************************************************************************
 * os/arch/arm/src/common/arm_backtrace_unwind.h
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

#ifndef __ARCH_ARM_SRC_COMMON_ARM_BACKTRACE_UNWIND_H
#define __ARCH_ARM_SRC_COMMON_ARM_BACKTRACE_UNWIND_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdint.h>

#if defined(CONFIG_UNWINDER_ARM)

/* Backtrace capture method */
#define BT_METHOD_EHABI    0
#define BT_METHOD_STACK    1
#define BT_METHOD_UNKNOWN  2

/* ARM register indices */
enum regs {
#ifdef CONFIG_ARM_THUMB
	FP = 7,
#else
	FP = 11,
#endif
	SP = 13,
	LR = 14,
	PC = 15
};

/* Unwinding frame state */
struct unwind_frame_s {
	unsigned long fp;
	unsigned long sp;
	unsigned long lr;
	unsigned long pc;
	unsigned long *lr_addr;
	unsigned long stack_base;
	unsigned long stack_top;
};

/* Unwinding control state */
struct unwind_ctrl_s {
	unsigned long vrs[16];		/* Virtual register set */
	const unsigned long *insn;	/* Pointer to current unwind instruction */
	unsigned long stack_top;	/* Stack upper bound for validation */
	unsigned long *lr_addr;		/* Address where LR was stored on stack */
	int entries;				/* Number of remaining unwind instruction words */
	int byte;					/* Current byte index within instruction word */
	int check_each_pop;			/* Validate stack bounds on each pop */
};


/****************************************************************************
 * Name: up_get_binary_region
 *
 * Description:
 *   Determine which binary region a PC address belongs to based on exidx tables.
 *
 * Input Parameters:
 *   pc - Program counter address
 *
 * Returned Value:
 *   Pointer to region name string ("kernel", "common", "app1", etc.)
 *
 ****************************************************************************/

const char *up_get_binary_region(unsigned long pc);


/****************************************************************************
 * Name: up_register_exidx
 *
 * Description:
 *   Register exidx unwind tables for loadable binaries.
 *   Called from binfmt when loading ELF binaries with .ARM.exidx sections.
 *
 * Input Parameters:
 *   exidx_start - Start address of exidx section
 *   exidx_size  - Size of exidx section in bytes
 *   text_start  - Start address of text section
 *   text_end    - End address of text section
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_APP_BINARY_SEPARATION
void up_register_exidx(unsigned long exidx_start, unsigned long exidx_size, unsigned long text_start, unsigned long text_end);
#endif

/****************************************************************************
 * Name: up_backtrace_current
 *
 * Description:
 *   Capture a backtrace of the currently executing task.
 *
 * Input Parameters:
 *   buffer - Pointer to array to store backtrace addresses
 *   size   - Maximum number of entries buffer can hold
 *   skip   - Number of frames to skip from the top
 *
 * Returned Value:
 *   Number of frames captured, or 0 on error
 *
 ****************************************************************************/

int up_backtrace_current(void **buffer, int size, int skip);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_UNWINDER_ARM */

#endif							/* __ARCH_ARM_SRC_COMMON_ARM_BACKTRACE_UNWIND_H */
