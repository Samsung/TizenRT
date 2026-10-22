/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * os/mm/mm_heap/mm_heapinfo_backtrace.c
 *
 * Prints, one line per live allocation, the allocation call-stack that was
 * captured at malloc time (sched_backtrace() when CONFIG_SCHED_BACKTRACE is
 * set, otherwise the arch up_backtrace()).  Unlike a heuristic stack scan this
 * only reads the backtrace already stored in each allocated node, so the
 * reported call path is exactly the one recorded by the allocator.
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include <tinyara/sched.h>
#include <tinyara/mm/mm.h>

#if defined(CONFIG_DEBUG_MM_HEAPINFO) && (CONFIG_MM_BACKTRACE > 0)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* pid value used to mark an allocation made from interrupt context */

#define HEAPINFO_INT      INT16_MAX

/* Max length of a single "bt: a <- b <- c" line: header plus each pointer
 * ("0x........" = 10 chars) and its " <- " separator.
 */

#define HEAPINFO_BT_LINELEN (80 + (CONFIG_MM_BACKTRACE * 16))

#if CONFIG_KMM_REGIONS > 1
#define HEAPINFO_NREGIONS (heap->mm_nregions)
#else
#define HEAPINFO_NREGIONS 1
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: heapinfo_print_node_backtrace
 *
 * Description:
 *   Format and print a single allocated node as:
 *     PID=<pid>(<name>) ptr=<mem> size=<size> seq=<seqno> bt: a <- b <- c
 *
 * Returned Value:
 *   1 if at least one backtrace frame was recorded for this node, else 0.
 *
 ****************************************************************************/

static int heapinfo_print_node_backtrace(FAR struct mm_allocnode_s *node)
{
	int i;
	int len = 0;
	char buf[HEAPINFO_BT_LINELEN];
	FAR void *mem = (FAR void *)((char *)node + SIZEOF_MM_ALLOCNODE);
	pid_t pid = (node->pid >= 0) ? node->pid : -(node->pid);
#if CONFIG_TASK_NAME_SIZE > 0
	FAR struct tcb_s *tcb;
	FAR const char *name = "unknown";

	if (node->pid == HEAPINFO_INT) {
		name = "INT";
	} else if (node->pid < 0) {
		/* A negative pid marks memory used as the stack of that pid. */
		name = "stack";
	} else {
		tcb = sched_gettcb(node->pid);
		if (tcb != NULL) {
			name = tcb->name;
		}
	}

	len += snprintf(buf + len, sizeof(buf) - len, "PID=%d(%s) ptr=%p size=%u", pid, name, mem, (unsigned int)node->size);
#else
	len += snprintf(buf + len, sizeof(buf) - len, "PID=%d ptr=%p size=%u", pid, mem, (unsigned int)node->size);
#endif

#ifdef CONFIG_MM_BACKTRACE_SEQNO
	len += snprintf(buf + len, sizeof(buf) - len, " seq=%lu", node->seqno);
#endif

	len += snprintf(buf + len, sizeof(buf) - len, " bt: ");

	for (i = 0; i < CONFIG_MM_BACKTRACE && node->backtrace[i] != NULL; i++) {
		if (len >= (int)sizeof(buf) - 16) {
			break;
		}
		if (i > 0) {
			len += snprintf(buf + len, sizeof(buf) - len, " <- ");
		}
		len += snprintf(buf + len, sizeof(buf) - len, "%p", node->backtrace[i]);
	}

	if (i == 0) {
		/* Kernel-context allocations record no backtrace (sequence number
		 * only).  Fall back to the 1-level malloc call address so the origin
		 * is still identifiable against the kernel ELF.
		 */
		len += snprintf(buf + len, sizeof(buf) - len, "(none) call=%p", (mmaddress_t)node->backtrace[0]);
	}

	heap_dbg("%s\n", buf);

	return (i > 0) ? 1 : 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: heapinfo_dump_backtrace
 *
 * Description:
 *   Walk every region of the heap and print the allocation backtrace of each
 *   live (in-use) chunk.  When pid == HEAPINFO_PID_ALL all chunks are printed,
 *   otherwise only those owned by the given pid.
 *
 ****************************************************************************/

void heapinfo_dump_backtrace(FAR struct mm_heap_s *heap, pid_t pid)
{
	FAR struct mm_allocnode_s *node;
	int region;
	int count = 0;
	int bt_count = 0;
	size_t total = 0;

	ASSERT(mm_check_heap_corruption(heap) == OK);

	heap_dbg("PID(name) | Alloc Addr | Size | Backtrace (malloc call path)\n");
	heap_dbg("-------------------------------------------------------------\n");

	for (region = 0; region < HEAPINFO_NREGIONS; region++) {
		/* Retake the semaphore for each region to reduce latencies. */

		DEBUGASSERT(mm_takesemaphore(heap));

		for (node = heap->mm_heapstart[region]; node < heap->mm_heapend[region]; node = (FAR struct mm_allocnode_s *)((char *)node + node->size)) {
			ASSERT(node->size);

			/* Skip the region head sentinel node.  It is marked allocated
			 * (MM_ALLOC_BIT) but is never handed out by mm_malloc, so its
			 * seqno/backtrace fields hold the heap fill pattern.
			 */
			if (node == heap->mm_heapstart[region]) {
				continue;
			}

			/* Only in-use chunks carry a valid allocation backtrace. */

			if ((node->preceding & MM_ALLOC_BIT) == 0) {
				continue;
			}

			if (pid != HEAPINFO_PID_ALL && node->pid != pid) {
				continue;
			}

			bt_count += heapinfo_print_node_backtrace(node);
			count++;
			total += node->size;
		}

		mm_givesemaphore(heap);
	}

	heap_dbg("-------------------------------------------------------------\n");
	if (pid == HEAPINFO_PID_ALL) {
		heap_dbg("Total live allocations : %d (%u bytes)\n", count, (unsigned int)total);
	} else {
		heap_dbg("PID %d live allocations : %d (%u bytes)\n", pid, count, (unsigned int)total);
	}
	heap_dbg("  with backtrace : %d  (without : %d - kernel-side/early allocs)\n", bt_count, count - bt_count);
}

#endif /* defined(CONFIG_DEBUG_MM_HEAPINFO) && (CONFIG_MM_BACKTRACE > 0) */

