/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/armv7-a/arm_allocateheap.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <tinyara/userspace.h>

#include <arch/board/board.h>

#include <tinyara/mm/mm.h>
#include <tinyara/kmalloc.h>
#include <tinyara/mm/heap_regioninfo.h>

#if defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_MM_KERNEL_HEAP)
#include <string.h>
#endif

#include "up_arch.h"
#include "arm_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

/* Terminology.  In the flat build (CONFIG_BUILD_FLAT=y), there is only a
 * single heap access with the standard allocations (malloc/free).  This
 * heap is referred to as the user heap.  In the protected build
 * (CONFIG_BUILD_PROTECTED=y) where an MPU is used to protect a region of
 * otherwise flat memory, there will be two allocators:  One that allocates
 * protected (kernel) memory and one that allocates unprotected (user)
 * memory.  These are referred to as the kernel and user heaps,
 * respectively.
 *
 * The ARMv7 has no MPU but does have an MMU.  With this MMU, it can support
 * the kernel build (CONFIG_BUILD_KERNEL=y).  In this configuration, there
 * is one kernel heap but multiple user heaps:  One per task group.  However,
 * in this case, we need only be concerned about initializing the single
 * kernel heap here.
 */

extern const uint32_t g_idle_topstack;
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_allocate_kheap
 *
 * Description:
 *   For the kernel build (CONFIG_BUILD_PROTECTED/KERNEL=y) with both kernel-
 *   and user-space heaps (CONFIG_MM_KERNEL_HEAP=y), this function allocates
 *   the kernel-space heap.  A custom version of this function is needed if
 *   memory protection of the kernel heap is required.
 *
 *
 ****************************************************************************/

void up_allocate_kheap(FAR void **heap_start, size_t *heap_size)
{
	/* Return the kernel heap settings (i.e., the part of the heap region
	 * that was not dedicated to the user heap).
	 */
	void *stack_end = (void *)(g_idle_topstack & ~(0x7));

	if ((void *)&_sdata <= (void *)KREGION_END && stack_end >= (void *)KREGION_END) {
		lldbg("ERROR: Failed to allocate kheap for ram region configuration\n");
		lldbg("Region start = 0x%x region end = 0x%x\n_sdata = 0x%x end of stack = 0x%x\n",
				KREGION_START, KREGION_END, &_sdata, stack_end);
		PANIC();
	} else if (stack_end >= (void *)KREGION_START && stack_end < (void *)KREGION_END) {
		*heap_start = stack_end;
	} else {
		*heap_start = (void *)KREGION_START;
	}

	*heap_size = (void *)KREGION_END - *heap_start;

	lldbg("start = 0x%x size = %d\n", *heap_start, *heap_size);
}

/****************************************************************************
 * Name: up_add_kregion
 ****************************************************************************/
#if defined(CONFIG_MM_KERNEL_HEAP) && (CONFIG_KMM_REGIONS > 1)
void up_add_kregion(void)
{
	int region_cnt;
	struct mm_heap_s *kheap;
	void *heap_start = NULL;
	size_t heap_size;
	void *stack_end = (void *)(g_idle_topstack & ~(0x7));
	kheap = kmm_get_baseheap();
	for (region_cnt = 1; region_cnt < CONFIG_KMM_REGIONS; region_cnt++) {
		if (kheap[kregionx_heap_idx[region_cnt]].mm_heapsize == 0) {
			lldbg("Heap idx = %u start = 0x%x size = %d\n", kregionx_heap_idx[region_cnt], kregionx_start[region_cnt], kregionx_size[region_cnt]);
			if (mm_initialize(&kheap[kregionx_heap_idx[region_cnt]], kregionx_start[region_cnt], kregionx_size[region_cnt]) != OK) {
				return;
			}
			continue;
		}

		void *kregionx_end = (void *)(((uint32_t)kregionx_start[region_cnt]) + kregionx_size[region_cnt]);
		if ((void *)&_sdata <= (void *)kregionx_end && stack_end >= (void *)kregionx_end) {
			lldbg("ERROR: Failed to allocate kheap for ram region configuration\n");
			lldbg("Region start = 0x%x region end = 0x%x\n_sdata = 0x%x end of stack = 0x%x\n",
					kregionx_start[region_cnt], kregionx_end, &_sdata, stack_end);
			PANIC();
		} else if (stack_end >= (void *)kregionx_start[region_cnt] && stack_end < (void *)kregionx_end) {
			heap_start = stack_end;
		} else {
			heap_start = (void *)kregionx_start[region_cnt];
		}

		heap_size = kregionx_end - heap_start;

		lldbg("start = 0x%x size = %d\n", heap_start, heap_size);
		mm_addregion(&kheap[kregionx_heap_idx[region_cnt]], heap_start, heap_size);
	}
}
#endif
