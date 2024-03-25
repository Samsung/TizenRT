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
 * mm/mm_heap/mm_memalign.c
 *
 *   Copyright (C) 2007, 2009, 2013-2014  Gregory Nutt. All rights reserved.
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

#include <debug.h>
#ifdef CONFIG_MM_ASSERT_ON_FAIL
#include <assert.h>
#ifdef CONFIG_SYSTEM_REBOOT_REASON
#include <tinyara/reboot_reason.h>
#endif
#endif
#include <tinyara/mm/mm.h>

#ifdef CONFIG_DEBUG_MM_HEAPINFO
#include  <tinyara/sched.h>
#endif
#include "mm_node.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Type Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: mm_memalign
 *
 * Description:
 *   memalign requests more than enough space from malloc, finds a region
 *   within that chunk that meets the alignment request and then frees any
 *   leading or trailing space.
 *
 *   The alignment argument must be a power of two (not checked).  8-byte
 *   alignment is guaranteed by normal malloc calls.
 *
 ****************************************************************************/
#ifdef CONFIG_DEBUG_MM_HEAPINFO
FAR void *mm_memalign(FAR struct mm_heap_s *heap, size_t alignment, size_t size, mmaddress_t caller_retaddr)
#else
FAR void *mm_memalign(FAR struct mm_heap_s *heap, size_t alignment, size_t size)
#endif
{
	FAR struct mm_freenode_s *node;
	void *ret = NULL;
	int ndx;
	size_t newsize;
	FAR struct mm_allocnode_s *alignchunk = NULL;
	bool found_align = false;
	size_t mask = (size_t)(alignment - 1);

	/* If this requested alinement's less than or equal to the natural alignment
	 * of malloc, then just let malloc do the work.
	 */

	if (alignment <= MM_MIN_CHUNK) {
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		return mm_malloc(heap, size, caller_retaddr);
#else
		return mm_malloc(heap, size);
#endif
	}

	if (size > MM_ALIGN_DOWN(MMSIZE_MAX) - SIZEOF_MM_ALLOCNODE) {
		mdbg("Because of mm_allocnode, %u cannot be allocated. The maximum \
			 allocable size is (MM_ALIGN_DOWN(MMSIZE_MAX) - SIZEOF_MM_ALLOCNODE) \
			 : %u\n.", size, (MM_ALIGN_DOWN(MMSIZE_MAX) - SIZEOF_MM_ALLOCNODE));
		return NULL;
	}

	/* Adjust the size to account for (1) the size of the allocated node and
	 * (2) to make sure that it is an even multiple of our granule size.
	 */

	newsize = MM_ALIGN_UP(size + SIZEOF_MM_ALLOCNODE);

	/* We need to hold the MM semaphore while we muck with the nodelist. */

	DEBUGASSERT(mm_takesemaphore(heap));

	/* Get the location in the node list to start the search
	 * by converting the request size into a nodelist index.
	 */

	ndx = mm_size2ndx(newsize);

	/* Search for a large enough chunk in the list of nodes.
	 * mm_nodelist is an array of lists. The array is arranged in ascending order of size.
	 * Each list is ordered by size in a descending order.
	 * If this list does not have free nodes whose size is large enough
	 * to accommodate the requested size, it will fail due to no more space.
	 */
	for (; ndx < MM_NNODES; ndx++) {
		node = heap->mm_nodelist[ndx].flink;
		if (!(node && node->size >= newsize)) {
			/* If the list at this index is empty or if the size of first node
			 * in the list is less than the required size, then go to next index.
			 */
			continue;
		}

		FAR struct mm_freenode_s *prev = &heap->mm_nodelist[ndx];

		/* Traverse the list until the end or until the node size is less than required size */
		for ( ; node && node->size >= newsize; prev = node, node = node->flink);

		/* If you hit the end of list, then set the node to prev */
		if (!node) {
			node = prev;
		}

		/* Now, traverse the list in reverse direction, towards bigger size nodes */
		for ( ; node; node = node->blink) {
			/* Search the suitable aligned address in the same node. */
			for (alignchunk = (FAR struct mm_allocnode_s *)(((size_t)node + SIZEOF_MM_ALLOCNODE + mask) & ~mask);
				(uintptr_t)(alignchunk + alignment) < (uintptr_t)(node + node->size);
				alignchunk = alignchunk + alignment) {

				size_t alignsize = (size_t)alignchunk - (size_t)node + size;
				size_t remainsize = (size_t)alignchunk - SIZEOF_MM_ALLOCNODE - (size_t)node;

				/* We found a suitable node if node size is more than required size after alignment and
				 * if the remaining bytes before the alignment point is either zero or bigger than freenode.
				 */
				if (node->size >= alignsize && (remainsize == 0 || remainsize >= SIZEOF_MM_FREENODE)) {
					found_align = true;
					break;
				}
			}

			if (found_align) {
				break;
			}
		}

		if (found_align) {
			break;
		}
	}

	if (found_align) {
		FAR struct mm_allocnode_s *newnode = (FAR struct mm_allocnode_s *)((size_t)alignchunk - SIZEOF_MM_ALLOCNODE);
		/* Get the next node after the allocation. */
		FAR struct mm_allocnode_s *next = (FAR struct mm_allocnode_s *)((char *)node + node->size);

		/* Remove the node.  There must be a predecessor, but there may not be
		 * a successor node.
		 */

		REMOVE_NODE_FROM_LIST(node);

		/* Check if there is free space at the beginning of the aligned chunk */
		if ((size_t)newnode - (size_t)node >= SIZEOF_MM_FREENODE) {
			size_t precedingsize;

			/* Preceding size is full size of the new 'node,' including
			 * SIZEOF_MM_ALLOCNODE
			 */
			precedingsize = (size_t)newnode - (size_t)node;

			/* Set up the size of the new node */
			newnode->size = (size_t)next - (size_t)newnode;
			newnode->preceding = precedingsize | MM_ALLOC_BIT;

			/* Reduce the size of the original chunk and mark it not allocated, */
			node->size = precedingsize;
			node->preceding &= ~MM_ALLOC_BIT;

			/* Fix the preceding size of the next node */
			next->preceding = newnode->size | (next->preceding & MM_ALLOC_BIT);

			/* Add the original, newly freed node to the free nodelist */
			mm_addfreechunk(heap, (FAR struct mm_freenode_s *)node);

			/* Replace the original node with the newly reallocated,
			 * aligned node
			 */
			node = (FAR struct mm_freenode_s *)newnode;
		} else {
			node->size = (size_t)next - (size_t)node;
			node->preceding |= MM_ALLOC_BIT;
		}

		/* Check if there is free space at the end of the aligned chunk */

		if (node->size > size + SIZEOF_MM_ALLOCNODE) {
			/* Shrink the chunk by that much -- remember, mm_shrinkchunk wants
			 * internal chunk sizes that include SIZEOF_MM_ALLOCNODE, and not the
			 * malloc-compatible sizes that we have.
			 */
			mm_shrinkchunk(heap, (FAR struct mm_allocnode_s *)node, size + SIZEOF_MM_ALLOCNODE);
		}

#ifdef CONFIG_DEBUG_MM_HEAPINFO
		heapinfo_update_node((struct mm_allocnode_s *)node, caller_retaddr);
		heapinfo_add_size(heap, ((struct mm_allocnode_s *)node)->pid, node->size);
		heapinfo_update_total_size(heap, node->size, ((struct mm_allocnode_s *)node)->pid);
#endif

		ret = (void *)alignchunk;
	}

	mm_givesemaphore(heap);

	/* If CONFIG_DEBUG_MM is defined, then output the result of the allocation
	 * to the SYSLOG.
	 */

	if (ret) {
		mvdbg("Allocated %p, size %u\n", ret, size);
	}

	return ret;
}
