/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
 * mm/mm_heap/mm_heapinfo_parse_heap.c
 *
 *   Copyright (C) 2007, 2009, 2013-2014 Gregory Nutt. All rights reserved.
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
#include <tinyara/sched.h>
#include <tinyara/mm/mm.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <syslog.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define HEAPINFO_INT INT16_MAX
#define HEAPINFO_NONSCHED (INT16_MAX - 1)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_DEBUG_MM_HEAPINFO

/****************************************************************************
 * Name: heapinfo_parse
 *
 * Description:
 *   This function walk through heap and displays alloc info.
 ****************************************************************************/
void heapinfo_parse_heap(FAR struct mm_heap_s *heap, int mode, pid_t pid)
{
	struct mm_allocnode_s *node;
	size_t mxordblk = 0;
	int    ordblks  = 0;		/* Number of non-inuse chunks */
	size_t fordblks = 0;		/* Total non-inuse space */
	size_t heap_resource;
	size_t stack_resource;
	size_t nonsched_resource;
	int nonsched_idx;
	struct sched_param sched_data;
	size_t heap_size;

	/* This nonsched can be 3 types : group resources, freed when child task finished, leak */
	pid_t nonsched_list[CONFIG_MAX_TASKS];
	size_t nonsched_size[CONFIG_MAX_TASKS];

#if CONFIG_KMM_REGIONS > 1
	int region;
#else
#define region 0
#endif

#ifdef CONFIG_DEBUG_CHECK_FRAGMENTATION
	int ndx;
	int nodelist_cnt[MM_NNODES] = {0, };
	size_t nodelist_size[MM_NNODES] = {0, };
	FAR struct mm_freenode_s *fnode;
#endif

	ASSERT(mm_check_heap_corruption(heap) == OK);

	/* initialize the heap, stack and nonsched resource */
	nonsched_resource = 0;
	heap_resource = 0;
	stack_resource = 0;
	for (nonsched_idx = 0; nonsched_idx < CONFIG_MAX_TASKS; nonsched_idx++) {
		nonsched_list[nonsched_idx] = HEAPINFO_NONSCHED;
		nonsched_size[nonsched_idx] = 0;
	}

	/* Visit each region */

#if CONFIG_KMM_REGIONS > 1
	for (region = 0; region < heap->mm_nregions; region++)
#endif
	{
		/* Visit each node in the region
		 * Retake the semaphore for each region to reduce latencies
		 */
		DEBUGASSERT(mm_takesemaphore(heap));

		if (mode != HEAPINFO_SIMPLE) {
			heap_dbg("****************************************************************\n");
			heap_dbg("REGION #%d Start=0x%p, End=0x%p, Size=%d\n",
				region,
				heap->mm_heapstart[region],
				heap->mm_heapend[region],
				(int)heap->mm_heapend[region] - (int)heap->mm_heapstart[region] + SIZEOF_MM_ALLOCNODE);
			heap_dbg("****************************************************************\n");
			heap_dbg("  MemAddr |   Size   | Status |    Owner   |  Pid  |\n");
			heap_dbg("----------|----------|--------|------------|-------|\n");
		}

		for (node = heap->mm_heapstart[region]; node < heap->mm_heapend[region]; node = (struct mm_allocnode_s *)((char *)node + node->size)) {
			ASSERT(node->size);

			/* Check if the node corresponds to an allocated memory chunk */
			if ((pid == HEAPINFO_PID_ALL || node->pid == pid) && (node->preceding & MM_ALLOC_BIT) != 0) {
				if (mode == HEAPINFO_DETAIL_ALL || mode == HEAPINFO_DETAIL_PID || mode == HEAPINFO_DETAIL_SPECIFIC_HEAP) {
					if (node->pid >= 0) {
						heap_dbg("0x%x | %8u |   %c    | 0x%8x | %3d   |\n", node, node->size, 'A', node->alloc_call_addr, node->pid);
					} else {
						heap_dbg("0x%x | %8u |   %c    | 0x%8x | %3d(S)|\n", node, node->size, 'A', node->alloc_call_addr, -(node->pid));
					}
				}

#if CONFIG_TASK_NAME_SIZE > 0
				if (node->pid == HEAPINFO_INT && mode != HEAPINFO_SIMPLE) {
					heap_dbg("INT Context\n");
				} else if (node->pid < 0 && sched_getparam((-1) * (node->pid), &sched_data) != ERROR) {
					stack_resource += node->size;
				} else if (sched_getparam(node->pid, &sched_data) == ERROR) {
					nonsched_list[PIDHASH(node->pid)] = node->pid;
					nonsched_size[PIDHASH(node->pid)] += node->size;
					nonsched_resource += node->size;
				} else {
					heap_resource += node->size;
				}
#else
				if (mode != HEAPINFO_SIMPLE) {
					heap_dbg("\n");
				}
#endif
			} else {
				ordblks++;
				fordblks += node->size;
				if (node->size > mxordblk) {
					mxordblk = node->size;
				}
				if (mode == HEAPINFO_DETAIL_ALL || mode == HEAPINFO_DETAIL_FREE || mode == HEAPINFO_DETAIL_SPECIFIC_HEAP) {
					heap_dbg("0x%x | %8d |   %c    |            |       |\n", node, node->size, 'F');
				}
			}
		}

		if (mode != HEAPINFO_SIMPLE) {
			heap_dbg("** PID(S) in Pid column means that mem is used for stack of PID\n\n");
		}
		mm_givesemaphore(heap);
	}
#undef region

	heap_dbg("\n****************************************************************\n");
	heap_dbg("     Summary of Heap Usages (Size in Bytes)\n");
	heap_dbg("****************************************************************\n");
	heap_size = heap->mm_heapsize;

	heap_dbg("Total                           : %u (100%%)\n", heap_size);
	heap_dbg("  - Allocated (Current / Peak)  : %u (%d%%) / %u (%d%%)\n",\
		heap->total_alloc_size, (size_t)((uint64_t)(heap->total_alloc_size) * 100 / heap_size),\
		heap->peak_alloc_size,  (size_t)((uint64_t)(heap->peak_alloc_size) * 100 / heap_size));
	heap_dbg("  - Free (Current)              : %u (%d%%)\n", fordblks, (size_t)((uint64_t)fordblks * 100 / heap_size));
	heap_dbg("  - Reserved                    : %u\n", SIZEOF_MM_ALLOCNODE * 2);

	heap_dbg("\n****************************************************************\n");
	heap_dbg("     Details of Heap Usages (Size in Bytes)\n");
	heap_dbg("****************************************************************\n");
	heap_dbg("< Free >\n");
	heap_dbg("  - Number of Free Node               : %d\n", ordblks);
	heap_dbg("  - Largest Free Node Size            : %u\n", mxordblk);
	heap_dbg("\n< Allocation >\n");
	heap_dbg("  - Current Size (Alive Allocation) = (1) + (2) + (3)\n");
	heap_dbg("     . by Dead Threads (*) (1)        : %u\n", nonsched_resource);
	heap_dbg("     . by Alive Threads\n");
	heap_dbg("        - Sum of \"STACK\"(**) (2)      : %u\n", stack_resource);
	heap_dbg("        - Sum of \"CURR_HEAP\" (3)      : %u\n", heap_resource - SIZEOF_MM_ALLOCNODE);	// Because of above for loop (node < heap->mm_heapend[region];),
													// one of SIZEOF_MM_ALLOCNODE is subtracted.
	heap_dbg("** NOTE **\n");
	heap_dbg("(*)  Alive allocation by dead threads might be used by others or might be a leakage.\n");
	heap_dbg("(**) Only Idle task has a separate stack region,\n");
	heap_dbg("  rest are all allocated on the heap region.\n");

#ifdef CONFIG_DEBUG_CHECK_FRAGMENTATION
	heap_dbg("\nAvailable fragmented memory segments in heap memory\n");

	DEBUGVERIFY(mm_takesemaphore(heap));

	for (ndx = 0; ndx < MM_NNODES; ++ndx) {
		for (fnode = heap->mm_nodelist[ndx].flink; fnode && fnode->size; fnode = fnode->flink) {
			++nodelist_cnt[ndx];
			nodelist_size[ndx] += fnode->size;
		}
	}

	mm_givesemaphore(heap);

	for (ndx = 0; ndx < MM_NNODES; ++ndx) {
		heap_dbg("Nodelist[%d] ranging [%u, %u] : num %d, size %u [Bytes]\n", ndx, ((ndx > 0 ? (1 << (ndx + MM_MIN_SHIFT)) : 0) + 1), 1 << (ndx + MM_MIN_SHIFT + 1), nodelist_cnt[ndx], nodelist_size[ndx]);
	}
#endif

	if (mode != HEAPINFO_SIMPLE) {
		heap_dbg("\n< by Dead Threads >\n");
		heap_dbg(" Pid | Size \n");
		heap_dbg("-----|------\n");
		for (nonsched_idx = 0; nonsched_idx < CONFIG_MAX_TASKS; nonsched_idx++) {
			if (nonsched_list[nonsched_idx] != HEAPINFO_NONSCHED) {
				heap_dbg("%4d | %5u\n", nonsched_list[nonsched_idx], nonsched_size[nonsched_idx]);
			}
		}
	}

	return;
}
#endif
