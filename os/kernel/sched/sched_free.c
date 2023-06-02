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
/************************************************************************
 * kernel/sched/sched_free.c
 *
 *   Copyright (C) 2007, 2009, 2012-2013 Gregory Nutt. All rights reserved.
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
 ************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>

#include <queue.h>
#include <assert.h>

#include <tinyara/kmalloc.h>
#include <tinyara/arch.h>
#include <tinyara/wqueue.h>
#include "sched/sched.h"

/************************************************************************
 * Definitions
 ************************************************************************/

/************************************************************************
 * Private Type Declarations
 ************************************************************************/

/************************************************************************
 * Global Variables
 ************************************************************************/

/************************************************************************
 * Private Variables
 ************************************************************************/

/************************************************************************
 * Private Function Prototypes
 ************************************************************************/

/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name: sched_ufree and sched_kfree
 *
 * Description:
 *   These function performs deallocations that the operating system may
 *   need to make.  This special interface to free is used in handling
 *   corner cases where the operating system may have to perform
 *   deallocations from within an interrupt handler.
 *
 ************************************************************************/

void sched_ufree(FAR void *address)
{
	irqstate_t flags;

	/* Check if this is an attempt to deallocate memory from an exception
	 * handler.  If this function is called from the IDLE task, then we
	 * must have exclusive access to the memory manager to do this.
	 */

	if (up_interrupt_context() || kumm_trysemaphore(address) != 0) {
		/* Yes.. Make sure that this is not a attempt to free kernel memory
		 * using the user deallocator.
		 */

		flags = enter_critical_section();
#if (defined(CONFIG_BUILD_PROTECTED) || defined(CONFIG_BUILD_KERNEL)) && \
	 defined(CONFIG_MM_KERNEL_HEAP)
		DEBUGASSERT(!kmm_heapmember(address));
#endif

		/* Delay the deallocation until a more appropriate time. */

		sq_addlast((FAR sq_entry_t *)address, (sq_queue_t *)&g_delayed_kufree);

		/* Signal the worker thread that is has some clean up to do */

#ifdef CONFIG_SCHED_WORKQUEUE
		work_signal(LPWORK);
#endif
		leave_critical_section(flags);
	} else {
		/* No.. just deallocate the memory now. */

		kumm_free(address);
		kumm_givesemaphore(address);
	}
}

#ifdef CONFIG_MM_KERNEL_HEAP
void sched_kfree(FAR void *address)
{
	irqstate_t flags;

	if (!kmm_heapmember(address)) {
		sched_ufree(address);
		return;
	}

	/* Check if this is an attempt to deallocate memory from an exception
	 * handler.  If this function is called from the IDLE task, then we
	 * must have exclusive access to the memory manager to do this.
	 */

	if (up_interrupt_context() || kmm_trysemaphore(address) != 0) {
		/* Yes.. Make sure that this is not a attempt to free user memory
		 * using the kernel deallocator.
		 */

		flags = enter_critical_section();
		DEBUGASSERT(kmm_heapmember(address));

		/* Delay the deallocation until a more appropriate time. */

		sq_addlast((FAR sq_entry_t *)address, (sq_queue_t *)&g_delayed_kfree);

		/* Signal the worker thread that is has some clean up to do */

#ifdef CONFIG_SCHED_WORKQUEUE
		work_signal(LPWORK);
#endif
		leave_critical_section(flags);
	} else {
		/* No.. just deallocate the memory now. */

		kmm_free(address);
		kmm_givesemaphore(address);
	}
}
#endif
