/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
 * mm/mm_heap/mm_heapinfo_utils.c
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
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <tinyara/sched.h>
#include <tinyara/arch.h>
#include <tinyara/mm/mm.h>

/****************************************************************************
 * Name: heapinfo_capture_insert / heapinfo_capture_remove
 *
 * Description:
 * Table helpers for the capture window. Called only from the counter update
 * path with the heap semaphore held, so every table change mirrors a counter
 * change one to one.
 ****************************************************************************/
static void heapinfo_capture_insert(struct mm_heap_s *heap, FAR struct mm_allocnode_s *node)
{
	struct heapinfo_capture_entry_s *entry;

	if (heap->mm_capture_count < HEAPINFO_CAPTURE_MAX_ENTRIES) {
		entry = &heap->mm_capture_table[heap->mm_capture_count++];
		entry->addr = (void *)node;
		entry->size = node->size;
		entry->old_size = 0;
		entry->caller = node->alloc_call_addr;
		memcpy(entry->backtrace, node->alloc_caller_backtrace, sizeof(entry->backtrace));
		entry->pid = node->pid;
		entry->type = HEAPINFO_CAPTURE_ALLOC;
	} else {
		heap->mm_capture_lost++;
	}
}

/* Record a pre window block that was freed inside the window. These count
 * negative in the delta, so the printed entries sum up to it exactly.
 */
static void heapinfo_capture_insert_freed(struct mm_heap_s *heap, FAR struct mm_allocnode_s *node, pid_t pid, mmsize_t size)
{
	struct heapinfo_capture_entry_s *entry;

	if (heap->mm_capture_count < HEAPINFO_CAPTURE_MAX_ENTRIES) {
		entry = &heap->mm_capture_table[heap->mm_capture_count++];
		entry->addr = (void *)node;
		entry->size = size;
		entry->old_size = 0;
		entry->caller = node->alloc_call_addr;
		memcpy(entry->backtrace, node->alloc_caller_backtrace, sizeof(entry->backtrace));
		entry->pid = pid;
		entry->type = HEAPINFO_CAPTURE_FREED;
	} else {
		heap->mm_capture_lost++;
	}
}

static bool heapinfo_capture_remove(struct mm_heap_s *heap, FAR struct mm_allocnode_s *node)
{
	int i;

	/* Newest first, so address reuse matches the living entry. Freed entries
	 * are history and never match.
	 */
	for (i = heap->mm_capture_count - 1; i >= 0; i--) {
		if (heap->mm_capture_table[i].addr == (void *)node && heap->mm_capture_table[i].type != HEAPINFO_CAPTURE_FREED) {
			if (heap->mm_capture_table[i].type == HEAPINFO_CAPTURE_ALLOC) {
				/* replace with the last entry, order does not matter */
				heap->mm_capture_table[i] = heap->mm_capture_table[--heap->mm_capture_count];
			} else {
				/* a realloced pre window block died : keep the loss of its
				 * original size in the books
				 */
				heap->mm_capture_table[i].size = heap->mm_capture_table[i].old_size;
				heap->mm_capture_table[i].old_size = 0;
				heap->mm_capture_table[i].type = HEAPINFO_CAPTURE_FREED;
			}
			return true;
		}
	}
	return false;
}

/****************************************************************************
 * Name: heapinfo_capture_start
 *
 * Description:
 * Arm a heap capture window on the given heap. While active, every update of
 * the per pid heapinfo counter also updates the capture table, so at stop the
 * table holds exactly the blocks allocated in the window and not yet freed,
 * accounted the same way heapinfo accounts them. pid == HEAPINFO_PID_ALL
 * captures every task. The state lives in the heap struct (shared kernel/user
 * memory) so a kernel ioctl can arm a window the user side allocator observes.
 ****************************************************************************/
void heapinfo_capture_start(struct mm_heap_s *heap, pid_t pid)
{
	pid_t hash_pid;
	struct heapinfo_capture_entry_s *table;
	struct heapinfo_capture_entry_s *old_table;

	/* Drop any table left over from a previous session */
	DEBUGASSERT(mm_takesemaphore(heap));
	old_table = heap->mm_capture_table;
	heap->mm_capture_table = NULL;
	heap->mm_capture_active = false;
	mm_givesemaphore(heap);
	if (old_table) {
		mm_free(heap, old_table);
	}

	/* Allocate the table before arming, so it is never tracked itself */
	table = (struct heapinfo_capture_entry_s *)mm_malloc(heap, HEAPINFO_CAPTURE_MAX_ENTRIES * sizeof(struct heapinfo_capture_entry_s), NULL);
	if (!table) {
		mdbg("capture start failed : no memory for %d entries\n", HEAPINFO_CAPTURE_MAX_ENTRIES);
		return;
	}

	DEBUGASSERT(mm_takesemaphore(heap));
	heap->mm_capture_pid = pid;
	heap->mm_capture_table = table;
	heap->mm_capture_count = 0;
	heap->mm_capture_lost = 0;
	heap->mm_capture_prewindow_freed = 0;
	/* Snapshot the same counter that the heapinfo pid row shows */
	if (pid == HEAPINFO_PID_ALL) {
		heap->mm_capture_start_size = (int)heap->total_alloc_size;
	} else {
		hash_pid = PIDHASH(pid);
		if (heap->alloc_list[hash_pid].pid == pid) {
			heap->mm_capture_start_size = heap->alloc_list[hash_pid].curr_alloc_size;
		} else {
			heap->mm_capture_start_size = 0;
		}
	}
	heap->mm_capture_active = true;
	mm_givesemaphore(heap);
}

/****************************************************************************
 * Name: heapinfo_capture_stop
 *
 * Description:
 * Disarm the heap capture window. Tagged nodes remain marked until reported.
 ****************************************************************************/
void heapinfo_capture_stop(struct mm_heap_s *heap)
{
	DEBUGASSERT(mm_takesemaphore(heap));
	heap->mm_capture_active = false;
	mm_givesemaphore(heap);
}

/****************************************************************************
 * Name: heapinfo_capture_note_realloc
 *
 * Description:
 * Called by mm_realloc after it resized a block. If the old block was from
 * before the window, the table holds a FREED entry for it and an ALLOC entry
 * for the new block : merge the two into one REALLOC entry that shows the old
 * and new size. Old blocks from inside the window already collapsed into a
 * single ALLOC entry, so nothing to do then. Value neutral for the delta.
 ****************************************************************************/
void heapinfo_capture_note_realloc(struct mm_heap_s *heap, FAR struct mm_allocnode_s *new_node, FAR void *old_node_addr)
{
	int i;
	int freed_idx = -1;
	int alloc_idx = -1;

	if (!heap->mm_capture_table) {
		return;
	}
	DEBUGASSERT(mm_takesemaphore(heap));
	if (heap->mm_capture_table) {
		/* newest first, to match the events this realloc just made */
		for (i = heap->mm_capture_count - 1; i >= 0; i--) {
			if (freed_idx < 0 && heap->mm_capture_table[i].type == HEAPINFO_CAPTURE_FREED && heap->mm_capture_table[i].addr == old_node_addr) {
				freed_idx = i;
			}
			if (alloc_idx < 0 && heap->mm_capture_table[i].type == HEAPINFO_CAPTURE_ALLOC && heap->mm_capture_table[i].addr == (void *)new_node) {
				alloc_idx = i;
			}
			if (freed_idx >= 0 && alloc_idx >= 0) {
				break;
			}
		}
		if (freed_idx >= 0 && alloc_idx >= 0) {
			/* the freed record keeps the true original size, even over
			 * a chain of reallocs on the same block
			 */
			heap->mm_capture_table[alloc_idx].type = HEAPINFO_CAPTURE_REALLOC;
			heap->mm_capture_table[alloc_idx].old_size = heap->mm_capture_table[freed_idx].size;
			heap->mm_capture_table[freed_idx] = heap->mm_capture_table[--heap->mm_capture_count];
		}
	}
	mm_givesemaphore(heap);
}

/****************************************************************************
 * Name: heapinfo_add_size
 *
 * Description:
 * Add the allocated size in tcb
 ****************************************************************************/
void heapinfo_add_size(struct mm_heap_s *heap, pid_t pid, mmsize_t size, FAR struct mm_allocnode_s *node)
{
	pid_t hash_pid;

	hash_pid = PIDHASH(pid);
	if (heap->alloc_list[hash_pid].pid == HEAPINFO_INIT_INFO || heap->alloc_list[hash_pid].pid == pid) {
		heap->alloc_list[hash_pid].pid = pid;
		heap->alloc_list[hash_pid].curr_alloc_size += size;
		if (heap->alloc_list[hash_pid].curr_alloc_size > heap->alloc_list[hash_pid].peak_alloc_size) {
			heap->alloc_list[hash_pid].peak_alloc_size = heap->alloc_list[hash_pid].curr_alloc_size;
		}
		heap->alloc_list[hash_pid].num_alloc_free++;
		/* Capture window : record the allocation exactly when the counter
		 * counted it, so the table follows the heapinfo accounting path.
		 */
		if (heap->mm_capture_active && heap->mm_capture_table && node && (heap->mm_capture_pid == HEAPINFO_PID_ALL || pid == heap->mm_capture_pid)) {
			heapinfo_capture_insert(heap, node);
		}
	}
}

/****************************************************************************
 * Name: heapinfo_subtract_size
 *
 * Description:
 * Subtract the allocated size in tcb
 ****************************************************************************/
void heapinfo_subtract_size(struct mm_heap_s *heap, pid_t pid, mmsize_t size, FAR struct mm_allocnode_s *node)
{
	pid_t hash_pid;

	hash_pid = PIDHASH(pid);
	if (heap->alloc_list[hash_pid].pid == pid) {
			heap->alloc_list[hash_pid].curr_alloc_size -= size;
			heap->alloc_list[hash_pid].num_alloc_free--;
			/* Capture window : a tracked block freed drops out of the table;
			 * an untracked one was allocated before the window started.
			 */
			if (heap->mm_capture_table && node) {
				if (!heapinfo_capture_remove(heap, node) && heap->mm_capture_active && (heap->mm_capture_pid == HEAPINFO_PID_ALL || pid == heap->mm_capture_pid)) {
					heap->mm_capture_prewindow_freed += size;
					heapinfo_capture_insert_freed(heap, node, pid, size);
				}
			}
	}
}

/****************************************************************************
 * Name: heapinfo_update_total_size
 *
 * Description:
 * Calculate the total allocated size and update the peak allocated size for heap
 ****************************************************************************/
void heapinfo_update_total_size(struct mm_heap_s *heap, mmsize_t size, pid_t pid)
{
	DEBUGASSERT(heap);
	heap->total_alloc_size += size;
	if (heap->peak_alloc_size < heap->total_alloc_size) {
		heap->peak_alloc_size = heap->total_alloc_size;
	}
#ifdef CONFIG_HEAPINFO_USER_GROUP
	heapinfo_update_group(size, pid);
#endif
}
/****************************************************************************
 * Name: heapinfo_update_node
 *
 * Description:
 * Adds pid and malloc caller return address to mem chunk
 ****************************************************************************/
void heapinfo_update_node(FAR struct mm_heap_s *heap, FAR struct mm_allocnode_s *node, mmaddress_t caller_retaddr)
{
	int i;
	DEBUGASSERT(node);
	node->alloc_call_addr = caller_retaddr;
	/* Default the deeper backtrace levels to NULL for every allocation (user
	 * and kernel). User-space allocations get them filled in mm_malloc(). */
	for (i = 0; i < HEAPINFO_BACKTRACE_DEPTH - 1; i++) {
		node->alloc_caller_backtrace[i] = NULL;
	}
	node->pid = getpid();
	/* Tag this allocation if a capture window is active on this heap and the pid
	 * matches. Blocks freed before the window is stopped are coalesced (they
	 * lose MM_ALLOC_BIT), so only blocks still allocated at stop time are
	 * reported. This is always called with the heap semaphore held.
	 */
	if (heap->mm_capture_active && (heap->mm_capture_pid == HEAPINFO_PID_ALL || node->pid == heap->mm_capture_pid)) {
		node->memory_state = MM_MEMORY_STATE_CAPTURED;
	} else {
		node->memory_state = MM_MEMORY_STATE_UNUSED;
	}
}

/****************************************************************************
 * Name: heapinfo_set_pid
 *
 * Description:
 * Set PID of the task to mem chunk
 * This is useful when kernel modules allocate memory on behalf of other tasks
 * and want to attribute the allocation to themselves.
 * This function also updates per-PID and group accounting to maintain consistency.
 ****************************************************************************/
void heapinfo_set_pid(void *address, pid_t pid)
{
	struct mm_allocnode_s *node;
	struct mm_heap_s *heap;
	pid_t old_pid;

	heap = mm_get_heap(address);
	if (heap) {
		node = (struct mm_allocnode_s *)((char *)address - SIZEOF_MM_ALLOCNODE);
		DEBUGASSERT(mm_takesemaphore(heap));
		
		old_pid = node->pid;
		if (old_pid != pid) {
			/* Subtract from old PID's accounting */
			heapinfo_subtract_size(heap, old_pid, node->size, node);
			heapinfo_update_total_size(heap, (-1) * node->size, old_pid);
			
			/* Update node PID */
			node->pid = pid;
			
			/* Add to new PID's accounting */
			heapinfo_add_size(heap, pid, node->size, node);
			heapinfo_update_total_size(heap, node->size, pid);
		}
		
		mm_givesemaphore(heap);
	} else {
		mdbg("Failed to set pid, heap not found. addr:%x\n", address);
	}
}

/****************************************************************************
 * Name: heapinfo_set_caller_addr
 *
 * Description:
 * Set caller address of malloc API to mem chunk
 * It is only called in DEBUG_SET_CALLER_ADDR macro
 ****************************************************************************/
void heapinfo_set_caller_addr(void *address, mmaddress_t caller_retaddr)
{
	struct mm_allocnode_s *node;
	struct mm_heap_s *heap;

	heap = mm_get_heap(address);
	if (heap) {
		node = (struct mm_allocnode_s *)((char *)address - SIZEOF_MM_ALLOCNODE);
		DEBUGASSERT(mm_takesemaphore(heap));
		heapinfo_update_node(heap, node, caller_retaddr);
		mm_givesemaphore(heap);
	} else {
		mdbg("Failed to set caller address, heap not found. addr:%x\n", address);
	}

}

/****************************************************************************
 * Name: heapinfo_set_stack_node
 *
 * Description:
 * This function sets a heap node as a stack node by setting negative value in the pid value.
 * This function requires allocated stack pointer and assigned pid value.
 ****************************************************************************/
void heapinfo_set_stack_node(void *stack_ptr, pid_t pid)
{
	struct mm_allocnode_s *node;

	node = (struct mm_allocnode_s *)(stack_ptr - SIZEOF_MM_ALLOCNODE);

	DEBUGASSERT(node && pid > 0);
	node->pid = (-1) * (pid);
}

/****************************************************************************
 * Name: heapinfo_exclude_stacksize
 *
 * Description:
 * when create a stack, subtract the stacksize from parent
 ****************************************************************************/
void heapinfo_exclude_stacksize(void *stack_ptr)
{
	struct mm_allocnode_s *node;
	pid_t hash_pid;
	struct mm_heap_s *heap = mm_get_heap(stack_ptr);
	if (heap == NULL) {
		return;
	}
	node = (struct mm_allocnode_s *)(stack_ptr - SIZEOF_MM_ALLOCNODE);

	DEBUGASSERT(node);
	hash_pid = PIDHASH(node->pid);
	heap->alloc_list[hash_pid].curr_alloc_size -= node->size;
	/* Capture window : the stack chunk was recorded at malloc time but the
	 * counter excludes stacks here, so drop it from the table as well. This
	 * function runs without the heap semaphore, take it for the table only.
	 */
	if (heap->mm_capture_table) {
		DEBUGASSERT(mm_takesemaphore(heap));
		heapinfo_capture_remove(heap, node);
		mm_givesemaphore(heap);
	}
#ifdef CONFIG_HEAPINFO_USER_GROUP
	int check_idx;
	int group_num;

	for (check_idx = 0; check_idx <= heapinfo_max_group; check_idx++) {
		if (node->pid == group_info[check_idx].pid) {
			group_num = group_info[check_idx].group;
			heapinfo_group[group_num].curr_size -= node->size;
			break;
		}
	}
#endif
}

/****************************************************************************
 * Name: heapinfo_peak_init
 *
 * Description:
 * initialize the peak allocation size in heap
 ****************************************************************************/
void heapinfo_peak_init(struct mm_heap_s *heap)
{
	int tcb_idx;
	int heap_idx;

	DEBUGASSERT(heap);
	for (heap_idx = 0; heap_idx < CONFIG_KMM_NHEAPS; heap_idx++) {
		for (tcb_idx = 0; tcb_idx < CONFIG_MAX_TASKS; tcb_idx++) {
			heap[heap_idx].alloc_list[tcb_idx].peak_alloc_size = 0;
		}
	}
}

/************************************************************************
 * Name: heapinfo_dealloc_tcbinfo
 *
 * Description:  Free the tcb info from heapinfo for a requested tcb.
 ************************************************************************/
void heapinfo_dealloc_tcbinfo(void *address, pid_t pid)
{
	pid_t hash_pid;
	struct mm_heap_s *heap = mm_get_heap(address);
	hash_pid = PIDHASH(pid);
	if (heap && heap->alloc_list[hash_pid].pid == pid) {
		heap->alloc_list[hash_pid].pid = HEAPINFO_INIT_INFO;
		heap->alloc_list[hash_pid].curr_alloc_size = 0;
		heap->alloc_list[hash_pid].peak_alloc_size = 0;
		heap->alloc_list[hash_pid].num_alloc_free = 0;
	}
}

/************************************************************************
 * Name: heapinfo_dump_heap
 *
 * Description: Print the hex value contents of heap.
 ************************************************************************/
void heapinfo_dump_heap(struct mm_heap_s *heap)
{
#if CONFIG_KMM_REGIONS > 1
	int region;
#else
#define region 0
#endif
#if CONFIG_KMM_REGIONS > 1
	for (region = 0; region < heap->mm_nregions; region++)
#endif
	{
#if defined(CONFIG_BUILD_FLAT) || defined(__KERNEL__)
		if (!abort_mode && !up_interrupt_context())
#endif
		{
			DEBUGASSERT(mm_takesemaphore(heap));
		}

		mm_dump_heap_region((uint32_t)(heap->mm_heapstart[region]), (uint32_t)(heap->mm_heapend[region]) + SIZEOF_MM_ALLOCNODE);
#if defined(CONFIG_BUILD_FLAT) || defined(__KERNEL__)
		if (!abort_mode && !up_interrupt_context())
#endif
		{
			mm_givesemaphore(heap);
		}
	}
}
