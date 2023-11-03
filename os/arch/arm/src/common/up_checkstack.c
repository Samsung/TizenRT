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
 * arch/arm/src/common/up_checkstack.c
 *
 *   Copyright (C) 2011, 2013 Gregory Nutt. All rights reserved.
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

#include <tinyara/arch.h>

#include "sched/sched.h"
#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_STACK_COLORATION

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static size_t do_stackcheck(uintptr_t alloc, size_t size);

/****************************************************************************
 * Name: do_stackcheck
 *
 * Description:
 *   Determine (approximately) how much stack has been used be searching the
 *   stack memory for a high water mark.  That is, the deepest level of the
 *   stack that clobbered some recognizable marker in the stack memory.
 *
 * Input Parameters:
 *   alloc - Allocation base address of the stack
 *   size - The size of the stack in bytes
 *
 * Returned value:
 *   The estimated amount of stack space used.
 *
 ****************************************************************************/

static size_t do_stackcheck(uintptr_t start, uintptr_t end)
{
	start = STACK_ALIGN_UP(start);
	end = STACK_ALIGN_DOWN(end);

	size_t size = end - start;
	FAR uint32_t *ptr;
	size_t mark;

	/* The ARM uses a push-down stack:  the stack grows toward lower addresses
	 * in memory.  We need to start at the lowest address in the stack memory
	 * allocation and search to higher addresses.  The first word we encounter
	 * that does not have the magic value is the high water mark.
	 */

	for (ptr = (FAR uint32_t *)start, mark = (size >> 2); *ptr == STACK_COLOR && mark > 0; ptr++, mark--) ;

	/* If the stack is completely used, then this might mean that the stack
	 * overflowed from above (meaning that the stack is too small), or may
	 * have been overwritten from below meaning that some other stack or data
	 * structure overflowed.
	 *
	 * If you see returned values saying that the entire stack is being used
	 * then enable the following logic to see it there are unused areas in the
	 * middle of the stack.
	 */

#if 0
	if (mark + 16 > nwords) {
		int i, j;

		ptr = (FAR uint32_t *)start;
		for (i = 0; i < size; i += 4 * 64) {
			for (j = 0; j < 64; j++) {
				int ch;
				if (*ptr++ == STACK_COLOR) {
					ch = '.';
				} else {
					ch = 'X';
				}

				up_putc(ch);
			}

			up_putc('\n');
		}
	}
#endif

	/* Return our guess about how much stack space was used */

	return mark << 2;
}

/****************************************************************************
 * Global Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_check_stack and friends
 *
 * Description:
 *   Determine (approximately) how much stack has been used be searching the
 *   stack memory for a high water mark.  That is, the deepest level of the
 *   stack that clobbered some recognizable marker in the stack memory.
 *
 * Input Parameters:
 *   None
 *
 * Returned value:
 *   The estimated amount of stack space used.
 *
 ****************************************************************************/

size_t up_check_tcbstack(FAR struct tcb_s *tcb)
{
	return do_stackcheck((uintptr_t)tcb->stack_alloc_ptr, (uintptr_t)tcb->adj_stack_ptr);
}

ssize_t up_check_tcbstack_remain(FAR struct tcb_s *tcb)
{
	return (ssize_t)tcb->adj_stack_size - (ssize_t)up_check_tcbstack(tcb);
}

size_t up_check_stack(void)
{
	return up_check_tcbstack(this_task());
}

size_t up_check_assertstack(uintptr_t start, uintptr_t end)
{
	return do_stackcheck(start, end);
}

ssize_t up_check_stack_remain(void)
{
	return up_check_tcbstack_remain(this_task());
}

#ifdef CONFIG_ARCH_NESTED_IRQ_STACK_SIZE
size_t up_check_nestirqstack(void)
{
	return do_stackcheck((uintptr_t)&g_nestedirqstkalloc, (uintptr_t)&g_nestedirqstkbase);
}

size_t up_check_nestirqstack_remain(void)
{
	return (STACK_ALIGN_DOWN(CONFIG_ARCH_INTERRUPTSTACK) - up_check_nestirqstack());
}
#endif

#if CONFIG_ARCH_INTERRUPTSTACK > 3
size_t up_check_intstack(void)
{
#ifdef CONFIG_SMP
	return do_stackcheck((uintptr_t)arm_intstack_alloc(), (uintptr_t)arm_intstack_top());
#else
	return do_stackcheck((uintptr_t)&g_intstackalloc, (uintptr_t)&g_intstackbase);
#endif
}

size_t up_check_intstack_remain(void)
{
	return STACK_ALIGN_DOWN(CONFIG_ARCH_INTERRUPTSTACK) - up_check_intstack();
}
#endif

#endif							/* CONFIG_STACK_COLORATION */
