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
 * mm/mm_heap/mm_extend.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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

#include <assert.h>

#include <tinyara/mm/mm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MIN_EXTEND (2 * SIZEOF_MM_ALLOCNODE)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_extend
 *
 * Description:
 *   Extend a heap region by add a block of (virtually) contiguous memory
 *   to the end of the heap.
 *
 ****************************************************************************/

void mm_extend(FAR struct mm_heap_s *heap, FAR void *mem, size_t size, int region)
{
	struct mm_allocnode_s *oldnode;
	struct mm_allocnode_s *newnode;
	uintptr_t blockstart;
	uintptr_t blockend;

	/* Make sure that we were passed valid parameters */

	ASSERT(heap && mem);
#if CONFIG_KMM_REGIONS > 1
	ASSERT(size >= MIN_EXTEND && (unsigned)region < heap->mm_nregions);
#else
	ASSERT(size >= MIN_EXTEND && (unsigned)region == 0);
#endif

	/* Make sure that the memory region are properly aligned */

	blockstart = (uintptr_t)mem;
	blockend   = blockstart + size;

	ASSERT(MM_ALIGN_UP(blockstart) == blockstart);
	ASSERT(MM_ALIGN_DOWN(blockend) == blockend);

	/* Take the memory manager semaphore */

	DEBUGASSERT(mm_takesemaphore(heap));

	/* Get the terminal node in the old heap.  The block to extend must
	 * immediately follow this node.
	 */

	oldnode = heap->mm_heapend[region];
	ASSERT((uintptr_t)oldnode + SIZEOF_MM_ALLOCNODE == (uintptr_t)mem);

	/* The size of the old node now extends to the new terminal node.
	 * This is the old size (SIZEOF_MM_ALLOCNODE) plus the size of
	 * the block (size) minus the size of the new terminal node
	 * (SIZEOF_MM_ALLOCNODE) or simply:
	 */

	oldnode->size = size;

	/* The old node should already be marked as allocated */

	ASSERT((oldnode->preceding & MM_ALLOC_BIT) != 0);

	/* Get and initialize the new terminal node in the heap */

	newnode            = (FAR struct mm_allocnode_s *)(blockend - SIZEOF_MM_ALLOCNODE);
	newnode->size      = SIZEOF_MM_ALLOCNODE;
	newnode->preceding = oldnode->size | MM_ALLOC_BIT;

	heap->mm_heapend[region] = newnode;
	mm_givesemaphore(heap);

	/* Finally "free" the new block of memory where the old terminal node was
	 * located.
	 */

	mm_free(heap, (FAR void *)mem);
}
