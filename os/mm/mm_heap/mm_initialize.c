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
 * mm/mm_heap/mm_initialize.c
 *
 *   Copyright (C) 2007, 2009, 2011, 2013 Gregory Nutt. All rights reserved.
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

#include <string.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/sched.h>
#include <tinyara/mm/mm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_addregion
 *
 * Description:
 *   This function adds a region of contiguous memory to the selected heap.
 *
 * Parameters:
 *   heap      - The selected heap
 *   heapstart - Start of the heap region
 *   heapsize  - Size of the heap region
 *
 * Return Value:
 *   OK on success, negative errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

int mm_addregion(FAR struct mm_heap_s *heap, FAR void *heapstart, size_t heapsize)
{
	FAR struct mm_freenode_s *node;
	uintptr_t heapbase;
	uintptr_t heapend;

	if (!heap || !heapstart || !heapsize) {
		mdbg("ERROR : Invalid parameter, heap : %x, heapstart : %x, heapsize : %u\n", heap, heapstart, heapsize);
		return -EINVAL;
	}

#if (CONFIG_KMM_REGIONS > 1) || (defined(CONFIG_MM_KERNEL_HEAP) && (CONFIG_KMM_REGIONS > 1))
	int IDX = heap->mm_nregions;
#else
#define IDX 0
#endif

	/* If the MCU handles wide addresses but the memory manager is configured
	 * for a small heap, then verify that the caller is  not doing something
	 * crazy.
	 */

#if defined(CONFIG_MM_SMALL) && !defined(CONFIG_SMALL_MEMORY)
	DEBUGASSERT(heapsize <= MMSIZE_MAX + 1);
#endif

	/* Adjust the provide heap start and size so that they are both aligned
	 * with the MM_MIN_CHUNK size.
	 */

	heapbase = MM_ALIGN_UP((uintptr_t)heapstart);
	heapend  = MM_ALIGN_DOWN((uintptr_t)heapstart + (uintptr_t)heapsize);
	if ((heapbase < (uintptr_t)heapstart)) {
		/* heapbase cannot be smaller than heapstart.
		 * If this happens, heapbase can be overflowed from alignment.
		 */
		mdbg("ERROR : heapbase(%u) is smaller than heapstart(%u).\n", heapbase, (uintptr_t)heapstart);
		return -EINVAL;
	}

	if (heapbase >= (uintptr_t)heapend) {
		/* heapbase is aligned up from heapstart,
		 * and heapend is the summation of heapstart and heapsize.
		 * If heapsize is very small, align-up address can be equal to or greater than heapend.
		 */
		mdbg("ERROR : heapbase(%u) is greater than equal to heapend(%u).\n", heapbase, heapend);
		return -EINVAL;
	}
	heapsize = heapend - heapbase;

	mlldbg("Region %d: base=%p size=%u\n", IDX + 1, heapstart, heapsize);

	/* Add the size of this region to the total size of the heap */

	heap->mm_heapsize += heapsize;

	/* Create two "allocated" guard nodes at the beginning and end of
	 * the heap.  These only serve to keep us from allocating outside
	 * of the heap.
	 *
	 * And create one free node between the guard nodes that contains
	 * all available memory.
	 */

	heap->mm_heapstart[IDX]            = (FAR struct mm_allocnode_s *)heapbase;
	heap->mm_heapstart[IDX]->size      = SIZEOF_MM_ALLOCNODE;
	heap->mm_heapstart[IDX]->preceding = MM_ALLOC_BIT;
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	/* fill magic number 0xDEAD as malloc info for head node */
	heapinfo_update_node((FAR struct mm_allocnode_s *)heap->mm_heapstart[IDX], 0xDEAD);
#endif

	node            = (FAR struct mm_freenode_s *)(heapbase + SIZEOF_MM_ALLOCNODE);
	node->size      = heapsize - 2 * SIZEOF_MM_ALLOCNODE;
	node->preceding = SIZEOF_MM_ALLOCNODE;
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	heapinfo_update_node((FAR struct mm_allocnode_s *)node, 0xDEADDEAD);
#endif

	heap->mm_heapend[IDX]            = (FAR struct mm_allocnode_s *)(heapend - SIZEOF_MM_ALLOCNODE);
	heap->mm_heapend[IDX]->size      = SIZEOF_MM_ALLOCNODE;
	heap->mm_heapend[IDX]->preceding = node->size | MM_ALLOC_BIT;
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	/* Fill magic number 0xDEADDEAD as malloc info for tail node */
	heapinfo_update_node((FAR struct mm_allocnode_s *)heap->mm_heapend[IDX], 0xDEADDEAD);
#endif

#undef IDX

#if (CONFIG_KMM_REGIONS > 1) || (defined(CONFIG_MM_KERNEL_HEAP) && (CONFIG_KMM_REGIONS > 1))
	heap->mm_nregions++;
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	/* add guard nodes size to the newly added region as they are considered as allocated*/
	heap->total_alloc_size += 2 * SIZEOF_MM_ALLOCNODE;
	if (heap->peak_alloc_size < heap->total_alloc_size) {
		heap->peak_alloc_size = heap->total_alloc_size;
	}
#endif

	/* Add the single, large free node to the nodelist */

	mm_addfreechunk(heap, node);
	return OK;
}

/****************************************************************************
 * Name: mm_initialize
 *
 * Description:
 *   Initialize the selected heap data structures, providing the initial
 *   heap region.
 *
 * Parameters:
 *   heap      - The selected heap
 *   heapstart - Start of the initial heap region
 *   heapsize  - Size of the initial heap region
 *
 * Return Value:
 *   OK on success, negative errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

int mm_initialize(FAR struct mm_heap_s *heap, FAR void *heapstart, size_t heapsize)
{
	int ret;
	int i;

	mlldbg("Heap: start=%p size=%u\n", heapstart, heapsize);

	/* The following two lines have cause problems for some older ZiLog
	 * compilers in the past (but not the more recent).  Life is easier if we
	 * just the suppress them altogther for those tools.
	 */

#ifndef __ZILOG__
	CHECK_ALLOCNODE_SIZE;
	CHECK_FREENODE_SIZE;
#endif

	/* Set up global variables */

	heap->mm_heapsize = 0;

#if (CONFIG_KMM_REGIONS > 1) || (defined(CONFIG_MM_KERNEL_HEAP) && (CONFIG_KMM_REGIONS > 1))
	heap->mm_nregions = 0;
#endif

	/* Initialize the node array */

	memset(heap->mm_nodelist, 0, sizeof(struct mm_freenode_s) * (MM_NNODES + 1));

	/* Initialize delay list to NULL for all cpus */

	for (i = 0; i < CONFIG_SMP_NCPUS; i++) {
		heap->mm_delaylist[i] = NULL;
	}

	/* Initialize the malloc semaphore to one (to support one-at-
	 * a-time access to private data sets).
	 */

	mm_seminitialize(heap);

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	heap->total_alloc_size = heap->peak_alloc_size = 0;
#endif

	/* Add the initial region of memory to the heap */

	ret = mm_addregion(heap, heapstart, heapsize);
	if (ret != OK) {
		return ret;
	}
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	for (i = 0; i < CONFIG_MAX_TASKS; i++) {
		heap->alloc_list[i].pid = HEAPINFO_INIT_INFO;
	}
#ifdef CONFIG_HEAPINFO_USER_GROUP
	heapinfo_update_group_info(INVALID_PROCESS_ID, HEAPINFO_INVALID_GROUPID, HEAPINFO_INIT_INFO);
#endif
#endif
	return OK;
}
