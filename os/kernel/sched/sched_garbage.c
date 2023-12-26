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
 * kernel/sched/sched_garbage.c
 *
 *   Copyright (C) 2009, 2011, 2013 Gregory Nutt. All rights reserved.
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
#include <tinyara/kmalloc.h>

#include "sched/sched.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sched_kucleanup
 *
 * Description:
 *   Clean-up deferred de-allocations of user memory
 *
 * Input parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static inline void sched_kucleanup(void)
{
	irqstate_t flags;
	FAR void *address;
	struct mm_heap_s *heap;

	/* Test if the delayed deallocation queue is empty.  No special protection
	 * is needed because this is an atomic test.
	 */

	while (g_delayed_kufree.head) {
		/* Remove the first delayed deallocation.  This is not atomic and so
		 * we must disable interrupts around the queue operation.
		 */

		address = sq_peek(&g_delayed_kufree);
		if (!address) {
			return;
		}
		heap = mm_get_heap(address);
		if (heap && heap->mm_semaphore.semcount <= 0) {
			continue;
		}
		flags = enter_critical_section();
		address = (FAR void *)sq_remfirst((FAR sq_queue_t *)&g_delayed_kufree);
		leave_critical_section(flags);

		/* The address should always be non-NULL since that was checked in the
		 * 'while' condition above.
		 */

		if (heap && address) {
			/* Return the memory to the user heap */

			mm_free(heap, address);
		}
	}
}

/****************************************************************************
 * Name: sched_kcleanup
 *
 * Description:
 *   Clean-up deferred de-allocations of kernel memory
 *
 * Input parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#if (defined(CONFIG_BUILD_PROTECTED) || defined(CONFIG_BUILD_KERNEL)) && \
	 defined(CONFIG_MM_KERNEL_HEAP)
static inline void sched_kcleanup(void)
{
	irqstate_t flags;
	FAR void *address;

	/* Test if the delayed deallocation queue is empty.  No special protection
	 * is needed because this is an atomic test.
	 */

	while (g_delayed_kfree.head) {
		/* Remove the first delayed deallocation.  This is not atomic and so
		 * we must disable interrupts around the queue operation.
		 */

		flags = enter_critical_section();
		address = (FAR void *)sq_remfirst((FAR sq_queue_t *)&g_delayed_kfree);
		leave_critical_section(flags);

		/* The address should always be non-NULL since that was checked in the
		 * 'while' condition above.
		 */

		if (address) {
			/* Return the memory to the kernel heap */

			kmm_free(address);
		}
	}
}
#else
#define sched_kcleanup()
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: sched_garbagecollection
 *
 * Description:
 *   Clean-up memory de-allocations that we queued because they could not
 *   be freed in that execution context (for example, if the memory was freed
 *   from an interrupt handler).
 *
 *   This logic may be called from the worker thread (see work_thread.c).
 *   If, however, CONFIG_SCHED_WORKQUEUE is not defined, then this logic will
 *   be called from the IDLE thread.  It is less optimal for the garbage
 *   collection to be called from the IDLE thread because it runs at a very
 *   low priority and could cause false memory out conditions.
 *
 * Input parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void sched_garbagecollection(void)
{
	/* Handle deferred deallocations for the kernel heap */

	sched_kcleanup();

	/* Handle deferred dealloctions for the user heap */

	sched_kucleanup();
}
