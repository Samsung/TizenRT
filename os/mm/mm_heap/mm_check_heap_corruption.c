/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
 * mm/mm_heap/mm_check_heap_corruption.c
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
#include <tinyara/arch.h>
#include <tinyara/mm/mm.h>
#if defined(CONFIG_DEBUG_MM_HEAPINFO)  && (CONFIG_TASK_NAME_SIZE > 0)
#include <sys/prctl.h>
#endif
#include <debug.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define MM_PREV_NODE_SIZE(x)		((x)->preceding & ~MM_ALLOC_BIT)
#define IS_ALLOCATED_NODE(x)		(((x)->preceding & MM_ALLOC_BIT) != 0)
#define IS_FREE_NODE(x)			(!IS_ALLOCATED_NODE(x))

#define MM_LAST_CORRUPT_SIZE		(1024)	/* Size of last node to dump in corruption scenario */
/****************************************************************************
 * Public data
 ****************************************************************************/
/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void dump_node(struct mm_allocnode_s *node, char *node_type)
{
#if defined(CONFIG_DEBUG_MM_HEAPINFO)  && (CONFIG_TASK_NAME_SIZE > 0)
	char myname[CONFIG_TASK_NAME_SIZE + 1];
	char is_stack[9] = "'s stack";
#endif

	if (!node) {
		mfdbg("Unable to dump NULL node\n");
		return;
	}

	mfdbg("%s: addr = 0x%08x, type = %c, size = %u, preceding size = %u\n", node_type, node, IS_ALLOCATED_NODE(node) ? 'A' : 'F', node->size, MM_PREV_NODE_SIZE(node));

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	pid_t pid = node->pid;
	if (pid < 0) {
		/* For stack allocated node, pid is negative value.
		 * To use the pid, change it to original positive value.
		 */
		pid = (-1) * pid;
	} else {
		is_stack[0] = '\0';
	}
#if CONFIG_TASK_NAME_SIZE > 0
	if (prctl(PR_GET_NAME_BYPID, myname, pid) == OK) {
		mfdbg("%s owner pid = %d (%s%s), allocated by code at addr = 0x%08x\n", node_type, node->pid, myname, is_stack, node->alloc_call_addr);
	} else {
		mfdbg("%s owner pid = %d (Exited Task%s), allocated by code at addr = 0x%08x\n", node_type, node->pid, is_stack, node->alloc_call_addr);
	}
#else
	mfdbg("%s owner pid = %d%s, allocated by code at addr = 0x%08x\n", node_type, node->pid, is_stack, node->alloc_call_addr);
#endif
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_check_heap_corruption
 *
 * Description:
 *   This function walk through heap and prints information about corrupt node.
 ****************************************************************************/
int mm_check_heap_corruption(struct mm_heap_s *heap)
{
	struct mm_allocnode_s *node;
	struct mm_allocnode_s *prev;
#if CONFIG_KMM_REGIONS > 1
	int region;
#else
#define region 0
#endif
	uint32_t start_corrupt = 0;
	uint32_t end_corrupt = 0;
	bool iscorrupt_f = false;
	bool iscorrupt_r = false;

	DEBUGASSERT(heap);

	/* Visit each region */

#if CONFIG_KMM_REGIONS > 1
	for (region = 0; region < heap->mm_nregions; region++)
#endif
	{
		/* Visit each node in the region
		 * Retake the semaphore for each region to reduce latencies
		 */

#if defined(CONFIG_BUILD_FLAT) || defined(__KERNEL__)
		if (!abort_mode && !up_interrupt_context()) 
#endif
		{
			mm_takesemaphore(heap);
		}

		mfdbg("Heap start = 0x%08x end = 0x%08x\n", heap->mm_heapstart[region], heap->mm_heapend[region]);

		if (((struct mm_allocnode_s *)(heap->mm_heapstart[region]))->size != SIZEOF_MM_ALLOCNODE) {
			mfdbg("#########################################################################################\n");
			mfdbg("ERROR: Heap corruption detected in mm_heapstart node.\n");
			mfdbg("ERROR: Check for overflow during backward traversal of heap\n");
			mfdbg("=========================================================================================\n");
			dump_node(heap->mm_heapstart[region], "HEAP START NODE");
			iscorrupt_f = true;
			start_corrupt = (uint32_t)(heap->mm_heapstart[region]);
			mfdbg("=========================================================================================\n");
		} else {
			/* Forward traversal of heap */
			for (prev = heap->mm_heapstart[region], node = (struct mm_allocnode_s *)((char *)prev + prev->size);
					node <= heap->mm_heapend[region];
					prev = node, node = (struct mm_allocnode_s *)((char *)node + node->size)) {
				if ((prev->size != MM_PREV_NODE_SIZE(node)) || (node->size < SIZEOF_MM_ALLOCNODE)) {
					mfdbg("#########################################################################################\n");
					mfdbg("ERROR: Heap corruption detected. Check below nodes for possible corruption.\n");
					mfdbg("ERROR: Forward traversal of heap\n");
					mfdbg("=========================================================================================\n");
					dump_node(prev, "PREV NODE");
					dump_node(node, "CORRUPT NODE");
					iscorrupt_f = true;
					if (prev->size <= MM_LAST_CORRUPT_SIZE) {
						start_corrupt = (uint32_t)prev;
					} else {
						start_corrupt = (uint32_t)prev + prev->size - MM_LAST_CORRUPT_SIZE;
					}
					mfdbg("=========================================================================================\n");
					break;
				}
			}

			if (!iscorrupt_f && prev != heap->mm_heapend[region]) {
				mfdbg("#########################################################################################\n");
				mfdbg("ERROR: Heap corruption detected. Check below nodes for possible corruption.\n");
				mfdbg("ERROR: Forward traversal of heap without reaching heapend\n");
				mfdbg("=========================================================================================\n");
				dump_node((struct mm_allocnode_s *)((char *)prev - MM_PREV_NODE_SIZE(prev)), "PREV NODE");
				dump_node(prev, "CORRUPT NODE");
				iscorrupt_f = true;
				start_corrupt = (uint32_t)prev - (MM_PREV_NODE_SIZE(prev) <= MM_LAST_CORRUPT_SIZE ? MM_PREV_NODE_SIZE(prev) : MM_LAST_CORRUPT_SIZE);
				mfdbg("=========================================================================================\n");
			}
		}

		if (((struct mm_allocnode_s *)(heap->mm_heapend[region]))->size != SIZEOF_MM_ALLOCNODE) {
			mfdbg("ERROR: Heap corruption detected in mm_heapend node.\n");
			mfdbg("ERROR: Check for overflow from PREV node in forward traversal of heap\n");
			mfdbg("=========================================================================================\n");
			dump_node(heap->mm_heapend[region], "HEAP END NODE");
			iscorrupt_r = true;
			end_corrupt = (uint32_t)(heap->mm_heapend[region]) + SIZEOF_MM_ALLOCNODE;
			mfdbg("#########################################################################################\n");
		} else {
			/* Backward traversal of heap */
			for (prev = heap->mm_heapend[region], node = (struct mm_allocnode_s *)((char *)prev - MM_PREV_NODE_SIZE(prev));
					prev > heap->mm_heapstart[region];
					prev = node, node = (struct mm_allocnode_s *)((char *)node - MM_PREV_NODE_SIZE(node))) {
				if ((MM_PREV_NODE_SIZE(prev) != node->size) || 
						((MM_PREV_NODE_SIZE(node) < SIZEOF_MM_ALLOCNODE) &&(node != heap->mm_heapstart[region]))) {
					mfdbg("ERROR: Backward traversal of heap\n");
					mfdbg("=========================================================================================\n");
					dump_node(node, "CORRUPT NODE");
					dump_node(prev, "PREV NODE");
					iscorrupt_r = true;
					end_corrupt = (uint32_t)prev + (prev->size <= MM_LAST_CORRUPT_SIZE ? prev->size : MM_LAST_CORRUPT_SIZE);
					mfdbg("#########################################################################################\n");
					break;
				}
			}

			if (!iscorrupt_r && prev != heap->mm_heapstart[region]) {
				mfdbg("ERROR: Backward traversal of heap without reaching heapstart\n");
				mfdbg("=========================================================================================\n");
				dump_node(prev, "CORRUPT NODE");
				dump_node((struct mm_allocnode_s *)((char *)prev + prev->size), "PREV NODE");
				iscorrupt_r = true;
				node = (struct mm_allocnode_s *)((char *)prev + prev->size);
				end_corrupt = (uint32_t)node + (node->size <= MM_LAST_CORRUPT_SIZE ? node->size : MM_LAST_CORRUPT_SIZE);
				mfdbg("#########################################################################################\n");
			}
		}

#if defined(CONFIG_BUILD_FLAT) || defined(__KERNEL__)
		if (!abort_mode && !up_interrupt_context()) 
#endif
		{
			mm_givesemaphore(heap);
		}

		if (iscorrupt_f || iscorrupt_r) {
#if defined(CONFIG_MM_DUMP_CORRPUTED_HEAP)
			mfdbg("CONFIG_MM_DUMP_CORRPUTED_HEAP enabled. Dumping full heap!!\n");
			mm_dump_heap_region(heap->mm_heapstart[region], (uint32_t)(heap->mm_heapend[region]) + SIZEOF_MM_ALLOCNODE);
#else
			mfdbg("Dumping the corrupted heap area\n");
			mm_dump_heap_region(start_corrupt, end_corrupt);
#endif
			return -1;
		}
	}

	return 0;
}
