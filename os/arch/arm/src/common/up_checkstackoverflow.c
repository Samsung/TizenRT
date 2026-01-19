/****************************************************************************
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <tinyara/sched.h>
#include <tinyara/irq.h>

#include <assert.h>
#include <debug.h>
#include <stdint.h>

#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_checkstackoverflow
 *
 * Description:
 *   This function checks stack overflow condition for a thread.
 *
 * Inputs:
 *   rtcb - Points to the TCB that is ready-to-run
 *
 ****************************************************************************/

void up_checkstackoverflow(FAR struct tcb_s *rtcb)
{
	if (*(uint32_t *)rtcb->stack_base_ptr != STACK_COLOR) {
		irqstate_t flags = enter_critical_section();
		//We have seen that the base pointer can get corrupted because of some
		//code, therefore, we will be checking the stack with adjusted stack
		//pointer and adjusted stack size also.
		uintptr_t ptr = STACK_ALIGN_UP(((uintptr_t)rtcb->adj_stack_ptr - (uintptr_t)rtcb->adj_stack_size));

		if (*(FAR uint32_t *)ptr != STACK_COLOR) {

			lldbg_noarg("\n###############    STACK OVERFLOW at pid %d ", rtcb->pid);
#if CONFIG_TASK_NAME_SIZE > 0
			lldbg_noarg("(%s) ", rtcb->name);
#endif
			lldbg_noarg("###################\n");
		} else {
			//Pointer we got from adjusted stack pointer and adjusted stack size
			//didn't reveal corrupted stack, therefore, we will treat this as memory
			//corruption and print logs accordingly.
			lldbg_noarg("\n###############    MEMORY CORRUPTION IN TCB at pid %d ", rtcb->pid);
#if CONFIG_TASK_NAME_SIZE > 0
            lldbg_noarg("(%s) ", rtcb->name);
#endif
            lldbg_noarg("###################\n");
			lldbg_noarg("Stack base pointer (corrupted value) : %p", (void *)(rtcb->stack_base_ptr));
		}
		PANIC();
		leave_critical_section(flags);
	}
}
