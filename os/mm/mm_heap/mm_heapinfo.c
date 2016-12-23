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
 * mm/mm_heap/mm_heapinfo.c
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
#include <tinyara/arch.h>
#include <unistd.h>
#include <stdio.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_DEBUG_MM_HEAPINFO

/****************************************************************************
 * Name: mm_heapinfo_parse
 *
 * Description:
 *   This function walk through heap and displays alloc info.
 ****************************************************************************/
void heapinfo_parse(FAR struct mm_heap_s *heap)
{
	struct mm_allocnode_s *node;
	size_t mxordblk = 0;
	int    ordblks  = 0;		/* Number of non-inuse chunks */
	size_t fordblks = 0;		/* Total non-inuse space */
	FAR struct tcb_s *tcb;
#if CONFIG_MM_REGIONS > 1
	int region;
#else
#define region 0
#endif
	printf("****************************************************************\n");
	printf("Heap Walker Output for Heap =0x%p\n", heap);
	printf("****************************************************************\n");

	/* Visit each region */

#if CONFIG_MM_REGIONS > 1
	for (region = 0; region < heap->mm_nregions; region++)
#endif
	{
		/* Visit each node in the region
		 * Retake the semaphore for each region to reduce latencies
		 */
		mm_takesemaphore(heap);

		printf("HeapReg#%d Heap StartAddr=0x%p, EndAddr=0x%p\n\n", region, heap->mm_heapstart[region], heap->mm_heapend[region]);
		printf("****************************************************************\n");
		printf("Heap Alloation Info- (Size in Bytes)\n");
		printf("****************************************************************\n");

		for (node = heap->mm_heapstart[region]; node < heap->mm_heapend[region]; node = (struct mm_allocnode_s *)((char *)node + node->size)) {

			/* Check if the node corresponds to an allocated memory chunk */

			if ((node->preceding & MM_ALLOC_BIT) != 0) {
				printf("MemAddr=0x%x Size=%5d (%c) Owner=0x%x Pid=%3d ", node, node->size, 'A', node->alloc_call_addr, node->pid);

#if CONFIG_TASK_NAME_SIZE > 0
				if (node->pid == -1) {
					printf("INT Context\n");
				} else {
					tcb = sched_gettcb(node->pid);
					if (tcb != NULL) {
						printf("%s\n", tcb->name);
					} else {
						printf("Unknown Context\n");
					}
				}
#else
				printf("\n");
#endif
			} else {
				ordblks++;
				fordblks += node->size;
				if (node->size > mxordblk) {
					mxordblk = node->size;
				}
				printf("MemAddr=0x%x Size=%5d (%c)\n", node, node->size, 'F');
			}
		}

		mm_givesemaphore(heap);
		printf("HeapReg#=%d End node=0x%p Size=%5d (%c)\n", region, node, node->size, 'A');
	}
#undef region
	printf("\nHeap Alloation Summary(Size in Bytes)\n");
	printf("Heap Size                      : %d\n", heap->mm_heapsize);
	printf("Current Allocated Node Size    : %d\n", heap->total_alloc_size + SIZEOF_MM_ALLOCNODE * 2);
	printf("Peak Allocated Node Size       : %d\n", heap->peak_alloc_size);
	printf("Free Size                      : %d\n", fordblks);
	printf("Largest Free Chunk Size        : %d\n", mxordblk);
	printf("Allocated not used(free)Chunks : %d\n", ordblks);
	return;
}

/****************************************************************************
 * Name: heapinfo_add_size
 *
 * Description:
 * Add the allocated size in tcb
 ****************************************************************************/
void heapinfo_add_size(size_t size)
{
	struct tcb_s *rtcb = sched_self();
	if (rtcb) {
		rtcb->curr_alloc_size += size;
		rtcb->num_alloc_free++;
		if (rtcb->curr_alloc_size > rtcb->peak_alloc_size) {
			rtcb->peak_alloc_size = rtcb->curr_alloc_size;
		}
	}
}

/****************************************************************************
 * Name: heapinfo_subtract_size
 *
 * Description:
 * Subtract the allocated size in tcb
 ****************************************************************************/
void heapinfo_subtract_size(struct mm_allocnode_s *node, size_t size)
{
	struct tcb_s *rtcb = sched_gettcb(node->pid);

	if (rtcb) {
		rtcb->curr_alloc_size -= size;
		rtcb->num_alloc_free--;
	}
}

/****************************************************************************
 * Name: heapinfo_update_total_size
 *
 * Description:
 * Calculate the total allocated size and update the peak allocated size for heap
 ****************************************************************************/
void heapinfo_update_total_size(struct mm_heap_s *heap, int size)
{
	heap->total_alloc_size += size;
	if (heap->total_alloc_size > heap->peak_alloc_size) {
		heap->peak_alloc_size = heap->total_alloc_size;
	}
}

/****************************************************************************
 * Name: heapinfo_update_node
 *
 * Description:
 * Adds pid and malloc caller return address to mem chunk
 ****************************************************************************/
void heapinfo_update_node(FAR struct mm_allocnode_s *node, mmaddress_t caller_retaddr)
{
	node->alloc_call_addr = caller_retaddr;
	node->reserved = 0;
	if (up_interrupt_context() == true) {
		/* update pid as -1 if allocation is from INT context */
		node->pid = -1;
	} else {
		node->pid = getpid();
	}
	return;
}
#endif
