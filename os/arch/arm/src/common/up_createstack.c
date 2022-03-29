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
/****************************************************************************
 * arch/arm/src/common/up_createstack.c
 *
 *   Copyright (C) 2007-2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <sched.h>
#include <debug.h>
#include <errno.h>

#ifdef CONFIG_DEBUG_MM_HEAPINFO
#include <tinyara/mm/mm.h>
#endif
#include <tinyara/kmalloc.h>
#include <tinyara/arch.h>
#include <tinyara/sched.h>
#include <arch/board/board.h>

#include "up_arch.h"
#include "up_internal.h"

#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION
#include <tinyara/mpu.h>
#endif

/****************************************************************************
 * Pre-processor Macros
 ****************************************************************************/
/* Configuration */

#undef HAVE_KERNEL_HEAP
#if (defined(CONFIG_BUILD_PROTECTED) || defined(CONFIG_BUILD_KERNEL)) && \
	 defined(CONFIG_MM_KERNEL_HEAP)
#define HAVE_KERNEL_HEAP 1
#endif

/* ARM requires at least a 4-byte stack alignment.  For use with EABI and
 * floating point, the stack must be aligned to 8-byte addresses.
 */

#ifndef CONFIG_STACK_ALIGNMENT

/* The symbol  __ARM_EABI__ is defined by GCC if EABI is being used.  If you
 * are not using GCC, make sure that CONFIG_STACK_ALIGNMENT is set correctly!
 */

#ifdef __ARM_EABI__
#define CONFIG_STACK_ALIGNMENT 8
#else
#define CONFIG_STACK_ALIGNMENT 4
#endif
#endif

/* Stack alignment macros */

#define STACK_ALIGN_MASK    (CONFIG_STACK_ALIGNMENT-1)
#define STACK_ALIGN_DOWN(a) ((a) & ~STACK_ALIGN_MASK)
#define STACK_ALIGN_UP(a)   (((a) + STACK_ALIGN_MASK) & ~STACK_ALIGN_MASK)

/* Alignment value for stack allocation when mpu stack overflow protection is enabled */
#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION
#ifdef CONFIG_ARMV8M_MPU
#define STACK_PROTECTION_MPU_ALIGNMENT (32)	/* Fixed alignment size of 32 bytes as per ARMv8-M spec*/
#else /* CONFIG_ARMV7M_MPU */
#define STACK_PROTECTION_MPU_ALIGNMENT CONFIG_MPU_STACK_GUARD_SIZE /* In ARMv7-M, MPU requires alignment to region size */
#endif
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Global Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_create_stack
 *
 * Description:
 *   Allocate a stack for a new thread and setup up stack-related information
 *   in the TCB.
 *
 *   The following TCB fields must be initialized by this function:
 *
 *   - adj_stack_size: Stack size after adjustment for hardware, processor,
 *     etc.  This value is retained only for debug purposes.
 *   - stack_alloc_ptr: Pointer to allocated stack
 *   - adj_stack_ptr: Adjusted stack_alloc_ptr for HW.  The initial value of
 *     the stack pointer.
 *
 * Inputs:
 *   - tcb: The TCB of new task
 *   - stack_size:  The requested stack size.  At least this much
 *     must be allocated.
 *   - ttype:  The thread type.  This may be one of following (defined in
 *     include/tinyara/sched.h):
 *
 *       TCB_FLAG_TTYPE_TASK     Normal user task
 *       TCB_FLAG_TTYPE_PTHREAD  User pthread
 *       TCB_FLAG_TTYPE_KERNEL   Kernel thread
 *
 *     This thread type is normally available in the flags field of the TCB,
 *     however, there are certain contexts where the TCB may not be fully
 *     initialized when up_create_stack is called.
 *
 *     If either CONFIG_BUILD_PROTECTED or CONFIG_BUILD_KERNEL are defined,
 *     then this thread type may affect how the stack is allocated.  For
 *     example, kernel thread stacks should be allocated from protected
 *     kernel memory.  Stacks for user tasks and threads must come from
 *     memory that is accessible to user code.
 *
 ****************************************************************************/

int up_create_stack(FAR struct tcb_s *tcb, size_t stack_size, uint8_t ttype)
{
	size_t stack_alloc_size = stack_size;

	/* new thread does not have the uheap value yet, It's added in the task_
	 * schedsetup function.
	 * Parent and child threads must have the same uheap value, so we are
	 * using the parent thread uheap
	 */
#if defined(HAVE_KERNEL_HEAP) || defined(CONFIG_MPU_STACK_OVERFLOW_PROTECTION)
	uint32_t uheap = sched_self()->uheap;	/* User heap pointer */
#endif

	/* Is there already a stack allocated of a different size?  Because of
	 * alignment issues, stack_size might erroneously appear to be of a
	 * different size.  Fortunately, this is not a critical operation.
	 */

	if (tcb->stack_alloc_ptr && tcb->adj_stack_size != stack_size) {
		/* Yes.. Release the old stack */

		up_release_stack(tcb, ttype);
	}

	/* Do we need to allocate a new stack? */

	if (!tcb->stack_alloc_ptr) {
		/* Allocate the stack.  If DEBUG is enabled (but not stack debug),
		 * then create a zeroed stack to make stack dumps easier to trace.
		 */

#ifdef HAVE_KERNEL_HEAP
		/* Use the kernel allocator if this is a task / pthread being created
		 * to run only on kernel side. We verify this by checking the uheap
		 * object in the tcb of current running task. uheap is always non-null
		 * in user threads and null for kernel threads.
		 */
		if (!uheap || ttype == TCB_FLAG_TTYPE_KERNEL) {
#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION
			stack_alloc_size = stack_alloc_size + CONFIG_MPU_STACK_GUARD_SIZE;
			tcb->stack_alloc_ptr = (uint32_t *)kmm_memalign(STACK_PROTECTION_MPU_ALIGNMENT, stack_alloc_size);
#else
			tcb->stack_alloc_ptr = (uint32_t *)kmm_malloc(stack_alloc_size);
#endif
		} else
#endif
		{
			/* Use the user-space allocator if this is a task or pthread */
#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION
			stack_alloc_size = stack_alloc_size + CONFIG_MPU_STACK_GUARD_SIZE;
			tcb->stack_alloc_ptr = (uint32_t *)kumm_memalign(STACK_PROTECTION_MPU_ALIGNMENT, stack_alloc_size);
#else
			tcb->stack_alloc_ptr = (uint32_t *)kumm_malloc(stack_size);
#endif
		}

#ifdef CONFIG_DEBUG
		/* Was the allocation successful? */

		if (!tcb->stack_alloc_ptr) {
			sdbg("ERROR: Failed to allocate stack, size %d\n", stack_size);
		}
#endif
	}

	/* Did we successfully allocate a stack? */

	if (tcb->stack_alloc_ptr) {
		size_t top_of_stack;
		size_t size_of_stack;

		/* The ARM uses a push-down stack:  the stack grows toward lower
		 * addresses in memory.  The stack pointer register, points to
		 * the lowest, valid work address (the "top" of the stack).  Items
		 * on the stack are referenced as positive word offsets from sp.
		 */

		top_of_stack = (uint32_t)tcb->stack_alloc_ptr + stack_alloc_size - 4;


		/* The ARM stack must be aligned; 4 byte alignment for OABI and
		 * 8-byte alignment for EABI. If necessary top_of_stack must be
		 * rounded down to the next boundary
		 */

		top_of_stack = STACK_ALIGN_DOWN(top_of_stack);

		/* The size of the stack in bytes is then the difference between
		 * the top and the bottom of the stack (+4 because if the top
		 * is the same as the bottom, then the size is one 32-bit element).
		 * The size need not be aligned.
		 */

		size_of_stack = top_of_stack - (uint32_t)tcb->stack_alloc_ptr + 4;

#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION

		/* Adjust stack size after guard_size calculation */
		size_of_stack = size_of_stack - CONFIG_MPU_STACK_GUARD_SIZE;
#endif
		/* Save the adjusted stack values in the struct tcb_s */

		tcb->adj_stack_ptr = (uint32_t *)top_of_stack;
		tcb->adj_stack_size = size_of_stack;

		/* If stack debug is enabled, then fill the stack with a
		 * recognizable value that we can use later to test for high
		 * water marks.
		 */

#ifdef CONFIG_STACK_COLORATION
#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION
		/* If CONFIG_MPU_STACK_OVERFLOW_PROTECTION is enabled, stack is created with requested size + CONFIG_MPU_STACK_GUARD_SIZE.
		 * In this case, we should make coloration from stack_alloc_ptr to adj_stack_ptr.
		 * So we should re-calculate the coloration size with CONFIG_MPU_STACK_GUARD_SIZE.
		 */
		up_stack_color(tcb->stack_alloc_ptr, tcb->adj_stack_size + CONFIG_MPU_STACK_GUARD_SIZE);
#else
		/* If CONFIG_MPU_STACK_OVERFLOW_PROTECTION is not enabled, kernel/user thread
		 * does not have a stack guard at front of stack so that the size of coloration
		 * should be the tcb->adj_stack_size.
		 */
		up_stack_color(tcb->stack_alloc_ptr, tcb->adj_stack_size);
#endif
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		heapinfo_exclude_stacksize(tcb->stack_alloc_ptr);
#endif
		board_led_on(LED_STACKCREATED);
#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION
		uint8_t nregion = mpu_get_nregion_info(MPU_REGION_STACKOVF);
		/* The smallest size that can be programmed for an MPU region is 32 bytes */
		mpu_get_register_config_value(&tcb->stack_mpu_regs[0], nregion - 1,
			(uint32_t)tcb->stack_alloc_ptr, CONFIG_MPU_STACK_GUARD_SIZE, true, false);
#endif

		return OK;
	}

	return -ENOMEM;
}

/****************************************************************************
 * Name: up_stack_color
 *
 * Description:
 *   Write a well know value into the stack
 *
 ****************************************************************************/

#ifdef CONFIG_STACK_COLORATION
void up_stack_color(FAR void *stackbase, size_t nbytes)
{
	/* Take extra care that we do not write outsize the stack boundaries */

	uint32_t *stkptr = (uint32_t *)(((uintptr_t)stackbase + 3) & ~3);
	uintptr_t stkend = (((uintptr_t)stackbase + nbytes) & ~3);
	size_t nwords = (stkend - (uintptr_t)stackbase) >> 2;

	/* Set the entire stack to the coloration value */

	while (nwords-- > 0) {
		*stkptr++ = STACK_COLOR;
	}
}
#endif
