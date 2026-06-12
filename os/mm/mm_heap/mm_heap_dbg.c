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
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <tinyara/sched.h>
#include <tinyara/mm/mm.h>
#include <tinyara/security_level.h>

/************************************************************************
 * Name: heap_dbg
 *
 * Description: Dump heap info debug logs without adding function name to start of log.
 ************************************************************************/
int heap_dbg(const char *fmt, ...)
{
	va_list ap;
	int ret;

	if (IS_SECURE_STATE()) {
		return 0;
	}

	va_start(ap, fmt);
#if defined(CONFIG_BUILD_FLAT) || defined(__KERNEL__)
	extern bool abort_mode;

	if (abort_mode) {
		ret = lowvsyslog(LOG_ERR, fmt, ap);
	} else {
		ret = vsyslog(LOG_ERR, fmt, ap);
	}
#else
	ret = vsyslog(LOG_ERR, fmt, ap);
#endif
	va_end(ap);

	return ret;
}

/************************************************************************
 * Name: mm_dump_heap_region
 *
 * Description: Print the hex value contents of heap from start to end address.
 ************************************************************************/
void mm_dump_heap_region(uint32_t start, uint32_t end)
{
	heap_dbg("#########################################################################################\n");
	heap_dbg("Dump heap: 0x%08x - 0x%08x\n", start, end);
	heap_dbg("#########################################################################################\n");
	for (; start < end; start += 32) {
		uint32_t *ptr = (uint32_t *)start;
		heap_dbg("%08x: %08x %08x %08x %08x %08x %08x %08x %08x\n",
			   start, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
	}
	heap_dbg("#########################################################################################\n");
}

/************************************************************************
 * Name: mm_dump_heap_free_node_list
 *
 * Description: Print the hex value contents of heap free nodes.
 ************************************************************************/
void mm_dump_heap_free_node_list(struct mm_heap_s *heap)
{
	struct mm_freenode_s *node;
	heap_dbg("#########################################################################################\n");
	heap_dbg("Dump heap free node list\n");
	heap_dbg("[ndx], [HEAD]: [FREE NODES(SIZE)]\n");
	heap_dbg("#########################################################################################\n");
	for (uint8_t ndx = 0; ndx < MM_NNODES; ndx++) {
		heap_dbg("%3d, %08x:", ndx, &heap->mm_nodelist[ndx]);
		for (node = heap->mm_nodelist[ndx].flink; node; node = node->flink) {
			heap_dbg(" %08x(%d)", node, node->size);
		}
		heap_dbg("\n");

		if (heap->mm_nodelist[ndx].size != 0) {
			mfdbg("    Corrupted HEAD of free node link %08x\n", &heap->mm_nodelist[ndx]);
		}
	}
	heap_dbg("#########################################################################################\n");
}

/************************************************************************
 * Name: mm_dump_node
 *
 * Description: Print the information of heap node.
 ************************************************************************/
void mm_dump_node(struct mm_allocnode_s *node, char *node_type)
{
#if defined(CONFIG_DEBUG_MM_HEAPINFO)  && (CONFIG_TASK_NAME_SIZE > 0)
	char myname[CONFIG_TASK_NAME_SIZE + 1];
	char is_stack[9] = "'s stack";
#endif

	if (!node) {
		mfdbg("Unable to dump NULL node\n");
		return;
	}

	mfdbg("%s: addr = 0x%08x, type = %c, size = %u, preceding size = %u\n", node_type, node, node->preceding & MM_ALLOC_BIT ? 'A' : 'F', node->size, node->preceding & ~MM_ALLOC_BIT);

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

/************************************************************************
 * Name: mm_dump_node_containing
 *
 * Description: Given an arbitrary address, find and print the owner of
 *              the heap node that contains this address. This is useful
 *              when a corrupted SP points into the heap and you want to
 *              know which allocation owns that memory.
 ************************************************************************/
void mm_dump_node_containing(void *address)
{
	struct mm_allocnode_s *node;
	struct mm_heap_s *heap;
	int region;
	uint32_t target = (uint32_t)address;

	if (!address) {
		mfdbg("Cannot find node for NULL address\n");
		return;
	}

	/* Find which heap this address belongs to */
	heap = mm_get_heap(address);
	if (!heap) {
		mfdbg("Address 0x%08x is not in any heap\n", target);
		return;
	}

	mfdbg("===========================================================\n");
	mfdbg("Finding heap node containing address 0x%08x\n", target);
	mfdbg("===========================================================\n");

	/* Walk each region of the heap */
#if CONFIG_KMM_REGIONS > 1
	for (region = 0; region < heap->mm_nregions; region++)
#else
	region = 0;
#endif
	{
		/* Walk nodes forward from heap start */
		for (node = heap->mm_heapstart[region];
		     node <= heap->mm_heapend[region];
		     node = (struct mm_allocnode_s *)((char *)node + node->size)) {

			/* Check if the target address falls within this node's data area.
			 * The data area starts at (node + SIZEOF_MM_ALLOCNODE) and
			 * extends to (node + node->size).
			 */
			if (target >= (uint32_t)node + SIZEOF_MM_ALLOCNODE &&
			    target < (uint32_t)node + node->size) {

				mfdbg("Address 0x%08x is inside heap node:\n", target);
				mm_dump_node(node, "CONTAINING NODE");

				/* Also dump adjacent nodes for context */
				/*if (node->preceding & MM_ALLOC_BIT) {
					mmsize_t prev_size = node->preceding & ~MM_ALLOC_BIT;
					if (prev_size >= SIZEOF_MM_ALLOCNODE) {
						struct mm_allocnode_s *prev_node =
							(struct mm_allocnode_s *)((char *)node - prev_size);
						if ((void *)prev_node >= (void *)heap->mm_heapstart[region] &&
						    (void *)prev_node < (void *)heap->mm_heapend[region]) {
							mm_dump_node(prev_node, "PREV ADJACENT NODE");
						}
					}
				}

				struct mm_allocnode_s *next_node =
					(struct mm_allocnode_s *)((char *)node + node->size);
				if ((void *)next_node >= (void *)heap->mm_heapstart[region] &&
				    (void *)next_node <= (void *)heap->mm_heapend[region]) {
					mm_dump_node(next_node, "NEXT ADJACENT NODE");
				}*/

				return;  /* Found it, done */
			}

			/* Safety: if node->size is 0 or corrupted, break to avoid infinite loop */
			if (node->size < SIZEOF_MM_ALLOCNODE) {
				mfdbg("Corrupted node at 0x%08x (size=%u), stopping search\n",
				       node, node->size);
				break;
			}
		}
	}

	mfdbg("Address 0x%08x not found in any heap node\n", target);
}
