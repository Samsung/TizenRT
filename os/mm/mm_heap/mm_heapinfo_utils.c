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
#include <sys/types.h>
#include <tinyara/sched.h>
#include <tinyara/arch.h>
#include <tinyara/mm/mm.h>

/****************************************************************************
 * Name: heapinfo_add_size
 *
 * Description:
 * Add the allocated size in tcb
 ****************************************************************************/
void heapinfo_add_size(struct mm_heap_s *heap, pid_t pid, mmsize_t size)
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
	}
}

/****************************************************************************
 * Name: heapinfo_subtract_size
 *
 * Description:
 * Subtract the allocated size in tcb
 ****************************************************************************/
void heapinfo_subtract_size(struct mm_heap_s *heap, pid_t pid, mmsize_t size)
{
	pid_t hash_pid;

	hash_pid = PIDHASH(pid);
	if (heap->alloc_list[hash_pid].pid == pid) {
			heap->alloc_list[hash_pid].curr_alloc_size -= size;
			heap->alloc_list[hash_pid].num_alloc_free--;
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
void heapinfo_update_node(FAR struct mm_allocnode_s *node, mmaddress_t caller_retaddr)
{
	DEBUGASSERT(node);
	node->alloc_call_addr = caller_retaddr;
	node->reserved = 0;
	node->pid = getpid();
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
		mm_takesemaphore(heap);
		heapinfo_update_node(node, caller_retaddr);
		mm_givesemaphore(heap);
	} else {
		mdbg("Failed to set caller address, heap not found. addr:%x\n", address);
	}

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
			mm_takesemaphore(heap);
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
