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

#ifdef CONFIG_SCHED_BACKTRACE

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
  unsigned long vrs[16];       /* Virtual register set */
  const unsigned long *insn;   /* Pointer to current unwind instruction */
  unsigned long stack_top;     /* Stack upper bound for validation */
  unsigned long *lr_addr;      /* Address where LR was stored on stack */
  int entries;                 /* Number of remaining unwind instruction words */
  int byte;                    /* Current byte index within instruction word */
  int check_each_pop;          /* Validate stack bounds on each pop */
};

/* ARM EHABI unwind table entry structure */
struct __EIT_entry {
  unsigned long fnoffset;      /* prel31 offset to function start */
  unsigned long content;       /* Unwind instructions or offset */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: up_has_exidx_entry
 *
 * Description:
 *   Check if a PC address has a valid exidx unwind entry.
 *   Used to determine if EHABI unwinding is available for a given address.
 *
 * Input Parameters:
 *   pc - Program counter address to check
 *
 * Returned Value:
 *   true if exidx entry exists, false otherwise
 *
 ****************************************************************************/

bool up_has_exidx_entry(unsigned long pc);

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
 * Name: unwind_find_entry
 *
 * Description:
 *   Find the EHABI unwind table entry for a given PC address.
 *   Searches the kernel exidx table and registered app exidx tables
 *   for the entry that covers the specified PC address.
 *
 * Input Parameters:
 *   addr - Address to find entry for
 *
 * Returned Value:
 *   Pointer to the EIT entry, or NULL if not found
 *
 ****************************************************************************/

const struct __EIT_entry *unwind_find_entry(unsigned long addr);

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
void up_register_exidx(unsigned long exidx_start, unsigned long exidx_size,
			unsigned long text_start, unsigned long text_end);
#endif

/****************************************************************************
 * Name: unwind_frame
 *
 * Description:
 *   Unwind a single stack frame using ARM EHABI.
 *   Updates the frame structure with the previous frame's PC, SP, and LR.
 *
 * Input Parameters:
 *   frame - Frame structure to update
 *
 * Returned Value:
 *   0 on success, negative error code on failure
 *
 ****************************************************************************/

int unwind_frame(struct unwind_frame_s *frame);

/****************************************************************************
 * Name: backtrace_unwind
 *
 * Description:
 *   Main EHABI backtrace engine. Unwinds the stack using ARM EHABI
 *   unwind tables and collects return addresses.
 *
 * Input Parameters:
 *   frame  - Initial frame (PC, SP, LR, stack bounds)
 *   buffer - Buffer to store return addresses
 *   size   - Maximum number of frames to capture
 *   skip   - Number of frames to skip (input/output)
 *
 * Returned Value:
 *   Number of frames captured
 *
 ****************************************************************************/

int backtrace_unwind(struct unwind_frame_s *frame, void **buffer,
		     int size, int *skip, unsigned char *methods);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* CONFIG_SCHED_BACKTRACE */

#endif /* __ARCH_ARM_SRC_COMMON_ARM_BACKTRACE_UNWIND_H */
