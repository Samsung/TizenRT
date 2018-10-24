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
 * arch/arm/src/common/up_allocateheap.c
 *
 *   Copyright (C) 2007, 2008, 2014 Gregory Nutt. All rights reserved.
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
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/userspace.h>

#include <arch/board/board.h>

#include <tinyara/mm/mm.h>
#include <tinyara/kmalloc.h>
#include <tinyara/mm/heap_regioninfo.h>

bool heapx_is_init[CONFIG_MM_NHEAPS];

#if defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_MM_KERNEL_HEAP)
#include <string.h>
#endif

#include "up_arch.h"
#include "up_internal.h"

/****************************************************************************
 * Private Definitions
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

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_allocate_heap/up_allocate_kheap
 *
 * Description:
 *   This function will be called to dynamically set aside the heap region.
 *
 *   - For the normal "flat" build, this function returns the size of the
 *     single heap.
 *   - For the protected build (CONFIG_BUILD_PROTECTED=y) with both kernel-
 *     and user-space heaps (CONFIG_MM_KERNEL_HEAP=y), this function
 *     provides the size of the unprotected, user-space heap.
 *   - For the kernel build (CONFIG_BUILD_KERNEL=y), this function provides
 *     the size of the protected, kernel-space heap.
 *
 *   If a protected kernel-space heap is provided, the kernel heap must be
 *   allocated by an analogous up_allocate_kheap(). A custom version of this
 *   file is needed if memory protection of the kernel heap is required.
 *
 *   The following memory map is assumed for the flat build:
 *
 *     .data region.  Size determined at link time.
 *     .bss  region  Size determined at link time.
 *     IDLE thread stack.  Size determined by CONFIG_IDLETHREAD_STACKSIZE.
 *     Heap.  Extends to the end of SRAM.
 *
 *   The following memory map is assumed for the kernel build:
 *
 *     Kernel .data region.  Size determined at link time.
 *     Kernel .bss  region  Size determined at link time.
 *     Kernel IDLE thread stack.  Size determined by CONFIG_IDLETHREAD_STACKSIZE.
 *     Padding for alignment
 *     User .data region.  Size determined at link time.
 *     User .bss region  Size determined at link time.
 *     Kernel heap.  Size determined by CONFIG_MM_KERNEL_HEAPSIZE.
 *     User heap.  Extends to the end of SRAM.
 *
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
void up_allocate_kheap(FAR void **heap_start, size_t *heap_size)
#else
void up_allocate_heap(FAR void **heap_start, size_t *heap_size)
#endif
{
#if defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_MM_KERNEL_HEAP)
	/* Get the unaligned size and position of the user-space heap.
	 * This heap begins after the user-space .bss section at an offset
	 * of CONFIG_MM_KERNEL_HEAPSIZE (subject to alignment).
	 */

	uintptr_t ubss_start = (uintptr_t)USERSPACE->us_bssstart;
	uintptr_t ubase = (uintptr_t)USERSPACE->us_bssend;
	size_t usize = REGION_END - ubase;

	DEBUGASSERT(ubase < (uintptr_t)(REGION_END);

	/* zero initialize the user space bss section */
	memset((void *)ubss_start, 0, (ubase - ubss_start));
	/* Return the user-space heap settings */

	board_led_on(LED_HEAPALLOCATE);
	*heap_start = (FAR void *)ubase;
	*heap_size = usize;
#else

	/* Return the heap settings */

	board_led_on(LED_HEAPALLOCATE);
	*heap_start = (FAR void *)(g_idle_topstack & ~(0x7));
	*heap_size =  REGION_END - (uint32_t)(*heap_start);

#endif
}

/****************************************************************************
 * Name: up_allocate_kheap
 *
 * Description:
 *   For the kernel build (CONFIG_BUILD_PROTECTED/KERNEL=y) with both kernel-
 *   and user-space heaps (CONFIG_MM_KERNEL_HEAP=y), this function allocates
 *   the kernel-space heap.  A custom version of this function is needed if
 *   memory protection of the kernel heap is required.
 *
 ****************************************************************************/

#if defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_MM_KERNEL_HEAP)
void up_allocate_kheap(FAR void **heap_start, size_t *heap_size)
{
	/* Return the kernel heap settings (i.e., the part of the heap region
	 * that was not dedicated to the user heap).
	 */

	*heap_start = (FAR void *)(g_idle_topstack & ~(0x7));
	*heap_size = (uint32_t)((uintptr_t)__usram_segment_start__) - (uint32_t)(*heap_start);
}
#endif

/****************************************************************************
 * Name: up_addregion
 ****************************************************************************/
#if CONFIG_MM_REGIONS > 1
void up_addregion(void)
{
	int region_cnt;

	for (region_cnt = 1; region_cnt < CONFIG_MM_REGIONS; region_cnt++) {
		if (heapx_is_init[regionx_heap_idx[region_cnt]] != true) {
			mm_initialize(&g_mmheap[regionx_heap_idx[region_cnt]], regionx_start[region_cnt], regionx_size[region_cnt]);
			heapx_is_init[regionx_heap_idx[region_cnt]] = true;
			continue;
		}
		mm_addregion(&g_mmheap[regionx_heap_idx[region_cnt]], regionx_start[region_cnt], regionx_size[region_cnt]);
	}
}
#endif
