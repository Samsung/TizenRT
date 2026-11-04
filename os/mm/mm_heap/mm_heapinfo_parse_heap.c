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
#ifdef CONFIG_WATCHDOG
#include <tinyara/watchdog.h>
#include <fcntl.h>
#include <errno.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define HEAPINFO_INT INT16_MAX
#define HEAPINFO_NONSCHED (INT16_MAX - 1)

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#ifdef CONFIG_WATCHDOG
static inline int check_watchdog_state(int wd_fd, bool *is_watchdog_running)
{
	struct watchdog_status_s wd_status;
	if (wd_fd < 0) {
		mfdbg("Fail to open %s, errno %d\n", CONFIG_WATCHDOG_DEVPATH, get_errno());
		return ERROR;
	}

	if (ioctl(wd_fd, WDIOC_GETSTATUS, (unsigned long)&wd_status) != OK) {
		mfdbg("Fail to get watchdog state, errno %d\n", get_errno());
		return ERROR;
	}

	*is_watchdog_running = wd_status.flags & WDFLAGS_ACTIVE;

	return OK;
}
#endif
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

#ifdef CONFIG_WATCHDOG
	bool is_watchdog_running;
	int wd_fd = open(CONFIG_WATCHDOG_DEVPATH, O_RDONLY);

	if (check_watchdog_state(wd_fd, &is_watchdog_running) != OK) {
		close(wd_fd);
#ifdef CONFIG_MM_ASSERT_ON_FAIL
		/* Anyway, it will reboot after printing the heap usage log.
		 * Print as many logs as possible even if fail watchdog keepalive. */
		is_watchdog_running = false;
		mfdbg("WARNING: It might reboot by watchdog during the printing heap usage log.\n");
#else
		mfdbg("It doesn't print the heap usage dump to prevent watchdog reboot.\n");
		return;
#endif
	}
#endif

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
						heap_dbg("0x%x | %8u |   %c    | 0x%8x | %3d   | bt: 0x%x 0x%x\n", node, node->size, 'A', (mmaddress_t)node->backtrace[0], node->pid, node->backtrace[1], node->backtrace[2]);
					} else {
						heap_dbg("0x%x | %8u |   %c    | 0x%8x | %3d(S)| bt: 0x%x 0x%x\n", node, node->size, 'A', (mmaddress_t)node->backtrace[0], -(node->pid), node->backtrace[1], node->backtrace[2]);
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
#ifdef CONFIG_WATCHDOG
			/* Keep watchdog alive */
			if (is_watchdog_running) {
#ifdef CONFIG_MM_ASSERT_ON_FAIL
				/* Anyway, it will reboot after printing the heap usage log.
		 		 * Print as many logs as possible even if fail watchdog keepalive. */
				(void)ioctl(wd_fd, WDIOC_KEEPALIVE, 0);
#else
				if (ioctl(wd_fd, WDIOC_KEEPALIVE, 0) != OK) {
					mfdbg("Fail to watchdog keepalive, errno %d\n", get_errno());
					mfdbg("It stops printing heap usage, to prevent watchdog reboot.\n");
					mm_givesemaphore(heap);
					close(wd_fd);
					return;
				}
#endif
			}
#endif
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
#ifdef CONFIG_WATCHDOG
	if (is_watchdog_running) {
		close(wd_fd);
	}
#endif
	return;
}

/****************************************************************************
 * Name: heapinfo_capture_reset
 *
 * Description:
 *   Clear any stale MM_MEMORY_STATE_CAPTURED tags in the heap. Called when a
 *   capture window is (re)started so that tags left over from an aborted
 *   session do not pollute the next report.
 ****************************************************************************/
void heapinfo_capture_reset(FAR struct mm_heap_s *heap)
{
	struct mm_allocnode_s *node;
#if CONFIG_KMM_REGIONS > 1
	int region;
#else
#define region 0
#endif

#if CONFIG_KMM_REGIONS > 1
	for (region = 0; region < heap->mm_nregions; region++)
#endif
	{
		DEBUGASSERT(mm_takesemaphore(heap));
		for (node = heap->mm_heapstart[region]; node < heap->mm_heapend[region]; node = (struct mm_allocnode_s *)((char *)node + node->size)) {
			if ((node->preceding & MM_ALLOC_BIT) != 0 && node->memory_state == MM_MEMORY_STATE_CAPTURED) {
				node->memory_state = MM_MEMORY_STATE_UNUSED;
			}
		}
		mm_givesemaphore(heap);
	}
#undef region
}

/****************************************************************************
 * Name: heapinfo_capture_report
 *
 * Description:
 *   Print the entries that make up the window delta, in three sections :
 *     (1) NEW ALLOCATIONS  : allocated in the window and still alive (+size)
 *     (2) REALLOCATIONS    : pre window blocks resized in the window, with the
 *                            old and new size (delta contribution = new - old)
 *     (3) FREED            : pre window blocks freed in the window (-size)
 *   Recorded on the heapinfo counter update path, no heap walk. By
 *   construction : sum(1 sizes) + sum(2 new - old) - sum(3 sizes) == the
 *   heapinfo counter delta (CURR_HEAP stop - start).
 ****************************************************************************/
void heapinfo_capture_report(FAR struct mm_heap_s *heap, pid_t pid)
{
	struct heapinfo_capture_entry_s *table;
	int alloc_sum = 0;
	int realloc_delta = 0;
	int freed_sum = 0;
	int alloc_cnt = 0;
	int realloc_cnt = 0;
	int freed_cnt = 0;
	int count;
	int lost;
	int start_size;
	int stop_size = 0;
	pid_t hash_pid;
	int i;

	/* Detach the table and snapshot the counters under the semaphore */
	DEBUGASSERT(mm_takesemaphore(heap));
	table = heap->mm_capture_table;
	heap->mm_capture_table = NULL;
	count = heap->mm_capture_count;
	lost = heap->mm_capture_lost;
	start_size = heap->mm_capture_start_size;
	if (pid == HEAPINFO_PID_ALL) {
		stop_size = (int)heap->total_alloc_size;
	} else {
		hash_pid = PIDHASH(pid);
		if (heap->alloc_list[hash_pid].pid == pid) {
			stop_size = heap->alloc_list[hash_pid].curr_alloc_size;
		}
	}
	mm_givesemaphore(heap);

	if (!table) {
		heap_dbg(" Heap Capture : no data (window not started or start failed)\n");
		return;
	}

	/* (1) new allocations still alive */
	heap_dbg("****************************************************************\n");
	heap_dbg(" [1] NEW ALLOCATIONS (allocated in window, not freed)\n");
	heap_dbg("  MemAddr |   Size   |    Owner   |  Pid  | bt: level1 level2\n");
	heap_dbg("----------|----------|------------|-------|------------------------\n");
	for (i = 0; i < count; i++) {
		if (table[i].type == HEAPINFO_CAPTURE_ALLOC) {
			heap_dbg("0x%x | %8u | 0x%8x | %3d   | bt: 0x%x 0x%x\n", table[i].addr, table[i].size, table[i].caller, table[i].pid, table[i].backtrace[0], table[i].backtrace[1]);
			alloc_sum += (int)table[i].size;
			alloc_cnt++;
		}
	}
	heap_dbg(" [1] total : %d blocks, %d bytes\n", alloc_cnt, alloc_sum);

	/* (2) reallocations of pre window blocks */
	heap_dbg("\n [2] REALLOCATIONS (pre window block resized in window)\n");
	heap_dbg("  MemAddr | Old Size | New Size |  Diff   |    Owner   |  Pid  | bt: level1 level2\n");
	heap_dbg("----------|----------|----------|---------|------------|-------|------------------------\n");
	for (i = 0; i < count; i++) {
		if (table[i].type == HEAPINFO_CAPTURE_REALLOC) {
			heap_dbg("0x%x | %8u | %8u | %7d | 0x%8x | %3d   | bt: 0x%x 0x%x\n", table[i].addr, table[i].old_size, table[i].size, (int)table[i].size - (int)table[i].old_size, table[i].caller, table[i].pid, table[i].backtrace[0], table[i].backtrace[1]);
			realloc_delta += (int)table[i].size - (int)table[i].old_size;
			realloc_cnt++;
		}
	}
	heap_dbg(" [2] total : %d blocks, diff %d bytes\n", realloc_cnt, realloc_delta);

	/* (3) pre window blocks freed in the window */
	heap_dbg("\n [3] FREED (pre window block freed in window)\n");
	heap_dbg("  MemAddr |   Size   |    Owner   |  Pid  | bt: level1 level2\n");
	heap_dbg("----------|----------|------------|-------|------------------------\n");
	for (i = 0; i < count; i++) {
		if (table[i].type == HEAPINFO_CAPTURE_FREED) {
			heap_dbg("0x%x | %8u | 0x%8x | %3d   | bt: 0x%x 0x%x\n", table[i].addr, table[i].size, table[i].caller, table[i].pid, table[i].backtrace[0], table[i].backtrace[1]);
			freed_sum += (int)table[i].size;
			freed_cnt++;
		}
	}
	heap_dbg(" [3] total : %d blocks, %d bytes\n", freed_cnt, freed_sum);

	/* reconciliation */
	heap_dbg("\n****************************************************************\n");
	if (lost) {
		heap_dbg(" WARNING : table full, %d events were not recorded\n", lost);
	}
	heap_dbg(" [1] + [2] - [3] = %d + %d - %d = %d bytes\n", alloc_sum, realloc_delta, freed_sum, alloc_sum + realloc_delta - freed_sum);
	heap_dbg(" heapinfo CURR_HEAP delta (stop - start) = %d - %d = %d bytes\n", stop_size, start_size, stop_size - start_size);

	mm_free(heap, table);
}
#endif
