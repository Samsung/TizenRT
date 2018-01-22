/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include <sched/sched.h>
#include <tls/see_api.h>

#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/
#ifndef CONFIG_ARCH_STACK_SMASH_PROTECT_NONE
uintptr_t __stack_chk_guard = 0xe2dee396;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
****************************************************************************/
void __stack_chk_region(struct tcb_s *ptcb)
{
#ifdef CONFIG_ARCH_STACK_OVF_DETECTION
	FAR uint32_t *ebp;

	/* Idle Task */

	if (!ptcb->pid) {
		return;
	}

	/* Get aligned addresses and adjusted sizes */

	ebp = (FAR uint32_t *)((uintptr_t)ptcb->stack_alloc_ptr & ~3);

	/* The ARM uses a grow downward stack */

	if (*ebp != STACK_COLOR) {
		lldbg("*** stack overflow detected ***\n");
		lldbg("%s(%d): Kernel stack is corrupted in: %p(%x)\n",
				ptcb->name, ptcb->pid, ptcb->adj_stack_ptr, *ebp);
		lldbg("%s(%d): stack ptr: %p, size: %x\n",
				ptcb->name, ptcb->pid,
				ptcb->stack_alloc_ptr, ptcb->adj_stack_size);

		assert(0);
	}
#endif
}

#ifndef CONFIG_ARCH_STACK_SMASH_PROTECT_NONE
void __stack_chk_fail(void)
{
	lldbg("*** stack smashing detected ***\n");
	lldbg("Kernel stack is corrupted in: %p\n",
			((struct tcb_s *) g_readytorun.head)->adj_stack_ptr);

	assert(0);
}

#endif
