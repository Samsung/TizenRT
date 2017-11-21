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
 * arch/arm/src/armv7-r/arm_dataabort.c
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
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

/* Output debug info if stack dump is selected -- even if debug is not
 * selected.
 */

#ifdef CONFIG_ARCH_STACKDUMP
#undef  CONFIG_DEBUG
#undef  CONFIG_DEBUG_ERROR
#undef  CONFIG_DEBUG_WARN
#undef  CONFIG_DEBUG_VERBOSE
#undef  CONFIG_LOGM
#define CONFIG_DEBUG 1
#define CONFIG_DEBUG_ERROR 1
#define CONFIG_DEBUG_WARN 1
#define CONFIG_DEBUG_VERBOSE 1
#endif

#include <stdint.h>
#include <stdio.h>
#include <debug.h>
#include <tinyara/irq.h>
#include "sched/sched.h"
#include "up_internal.h"
#include "mpu.h"

#define STATUS_1_MASK	(0x0F)
#define STATUS_2_MASK	(0x01)
#define STATUS_1(x)	((x >> 0) & STATUS_1_MASK)
#define STATUS_2(x)	((x >> 10) & STATUS_2_MASK)

#define FSR_STATUS(x)	((STATUS_2(x) << 4) | STATUS_1(x) << 0)
#define ALIGNMENT_FLT		(0x01)
#define BACKGRND_FLT		(0x00)
#define PERMISSION_FLT		(0x0D)
#define PRECISE_ABT_FLT		(0x08)
#define IMPRECISE_ABT_FLT	(0x16)
#define PRECISE_PAR_FLT		(0x19)
#define IMPRECISE_PAR_FLT	(0x18)
#define DEBUG_EVENT_FLT		(0x02)

#define MIN_LOG2_SIZE 8

#if defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_UTASK_MEMORY_PROTECTION)
uint8_t get_log2size(size_t size)
{
	uint8_t l2size;

	/* The minimum permitted region size is 32 bytes (log2(32) = 5. */

	for (l2size = 5; l2size < 32 && size > (1 << l2size); l2size++) ;
	return l2size;
}

int arm_find_addrmatch(uint32_t *regs, FAR sq_queue_t *pq, uint32_t dfar)
{
	int32_t ret = -1;
	int base;
	int size;
	int subregions;
	sq_entry_t *entry;
	sq_entry_t *head;
	struct mm_allocnode_s *node;

	if (sq_empty(pq)) {
		return ret;
	}

	entry = sq_peek(pq);

	if (!entry) {
		return ret;
	}
	head = entry;

	do {
	    node = (struct mm_allocnode_s *)entry;
		if (dfar >= (uint32_t)node && dfar <= (uint32_t)((uint8_t *)node + node->size)) {
			ret = 1;
			break;
		}
		entry = sq_next(entry);
	} while (entry != head);

	size_t hsize = node->size;

	/* If the address range match exist, grant permissions for the memory */
	if (ret > 0) {
		size = get_log2size(hsize);
		if (size < MIN_LOG2_SIZE) {
			size = MIN_LOG2_SIZE;
		}
		base = ((uint32_t)node) & ~(size - 1);
		regs[REG_RBASE] = ((uint32_t)base  & MPU_RBAR_ADDR_MASK);
		subregions = mpu_subregion(base, hsize, size);
		regs[REG_RSIZE] = ((uint32_t)subregions << MPU_RASR_SRD_SHIFT) |
					MPU_RASR_RSIZE_LOG2(size)  | MPU_RASR_ENABLE;
		regs[REG_RATTR] = MPU_RACR_B | MPU_RACR_TEX(5) | MPU_RACR_AP_RWRW | MPU_RACR_XN;
	}
	return ret;
}

int32_t arm_exempt_streamlist(uint32_t dfar, uint32_t *regs, FAR struct tcb_s *current)
{
	int32_t ret = -1;
	int32_t	tg_size = 0;
	int size;
	int base;
	int subregions;
	int stream_index;
	FAR struct task_group_s *group;
	FAR struct streamlist *tg_streamlist;
	FAR FILE *stream;
	unsigned char *buf_start;

	size = base = subregions = 0;
	if (current->pid < 0) {
	    return ret;
	}
	group = current->group;
	if (!group) {
	    return ret;
	}

	tg_streamlist = group->tg_streamlist;
	tg_size = sizeof(struct task_group_s);

#if CONFIG_STDIO_BUFFER_SIZE > 0
	for (stream_index = 0; stream_index < CONFIG_NFILE_STREAMS; stream_index++) {
		stream = &(tg_streamlist->sl_streams[stream_index]);
		buf_start = stream->fs_bufstart;
		if (dfar >= (int32_t)buf_start && dfar <= (int32_t)(buf_start + CONFIG_STDIO_BUFFER_SIZE)) {
			size = get_log2size(CONFIG_STDIO_BUFFER_SIZE);
			if (size < MIN_LOG2_SIZE) {
				size = MIN_LOG2_SIZE;
			}
			base = ((uint32_t)buf_start) & ~(size - 1);
			regs[REG_RBASE1] = ((uint32_t)base  & MPU_RBAR_ADDR_MASK);
			subregions = mpu_subregion(base, CONFIG_STDIO_BUFFER_SIZE, size);
			regs[REG_RSIZE1] = ((uint32_t)subregions << MPU_RASR_SRD_SHIFT) |
				MPU_RASR_RSIZE_LOG2(size)  | MPU_RASR_ENABLE;
			regs[REG_RATTR1] = MPU_RACR_B | MPU_RACR_TEX(5) | MPU_RACR_AP_RWRW | MPU_RACR_XN;
			ret = 1;
			return ret;
		}
	}
#endif
	if (dfar >= (int32_t)tg_streamlist && dfar <= (int32_t)((int32_t)tg_streamlist + tg_size)) {
		size = get_log2size(tg_size);
		if (size < MIN_LOG2_SIZE) {
			size = MIN_LOG2_SIZE;
		}
		base = ((uint32_t)tg_streamlist) & ~(size - 1);
		regs[REG_RBASE1] = ((uint32_t)base  & MPU_RBAR_ADDR_MASK);
		subregions = mpu_subregion(base, tg_size, size);
		regs[REG_RSIZE1] = ((uint32_t)subregions << MPU_RASR_SRD_SHIFT) |
				MPU_RASR_RSIZE_LOG2(size)  | MPU_RASR_ENABLE;
		regs[REG_RATTR1] = MPU_RACR_B | MPU_RACR_TEX(5) | MPU_RACR_AP_RWRW | MPU_RACR_XN;
		ret = 1;
	}
	return ret;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_dataabort
 *
 * Input parameters:
 *   regs - The standard, ARM register save array.
 *   dfar - Fault address register.
 *   dfsr - Fault status register.
 *
 * Description:
 *   This is the data abort exception handler. The ARM data abort exception
 *   occurs when a memory fault is detected during a data transfer.
 *
 ****************************************************************************/

uint32_t *arm_dataabort(uint32_t *regs, uint32_t dfar, uint32_t dfsr)
{
#if defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_UTASK_MEMORY_PROTECTION)
	FAR struct tcb_s *current;
	FAR sq_queue_t *pqueue;
	FAR struct mm_heap_s *heap = (void *)USERSPACE->us_heap;
	pid_t pid;
	int32_t size;
	int32_t base;
	int32_t subregions;
#ifdef HAVE_GROUP_MEMBERS
	FAR struct task_group_s *group;
	int32_t i;
#endif
	extern uint32_t __usram_segment_start__[];
	uint32_t *user_data = (uint32_t *)__usram_segment_start__;
#endif
	volatile int32_t status = -1;

	/* Save the saved processor context in current_regs where it can be accessed
	 * for register dumps and possibly context switching.
	 */
	current_regs = regs;

#if defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_UTASK_MEMORY_PROTECTION)
	if (FSR_STATUS(dfsr) == PERMISSION_FLT) {
		current = sched_self();
		pid = current->pid;

		status = arm_exempt_streamlist(dfar, regs, current);

		/* Handle only user threads and tasks */
		if (status < 0 && !is_tasktype_kernel(current->flags)) {
			if (heap->alloc_state[pid] == MM_IN_MALLOC || heap->alloc_state[pid] == MM_IN_CALLOC
					|| heap->alloc_state[pid] == MM_IN_REALLOC || heap->alloc_state[pid] == MM_IN_FREE) {
				uintptr_t ubase = (uintptr_t)USERSPACE->us_bssend;
				size_t heap_size = CONFIG_RAM_END - ubase;
				size = get_log2size(heap_size);
				if (size < MIN_LOG2_SIZE) {
					size = MIN_LOG2_SIZE;
				}
				base = ((uint32_t)ubase) & ~(size - 1);
				regs[REG_RBASE] = ((uint32_t)base  & MPU_RBAR_ADDR_MASK);
				subregions = mpu_subregion(base, heap_size, size);
				regs[REG_RSIZE] = ((uint32_t)subregions << MPU_RASR_SRD_SHIFT) |
					MPU_RASR_RSIZE_LOG2(size)  | MPU_RASR_ENABLE;
				regs[REG_RATTR] = MPU_RACR_B | MPU_RACR_TEX(5) |
					MPU_RACR_AP_RWRW | MPU_RACR_XN;
				status = 1;
			} else if (heap->alloc_state[pid] == MM_FREE_DONE || heap->alloc_state[pid] == MM_DEFAULT) {
				regs[REG_RBASE] = 0x0;
				regs[REG_RSIZE] = 0x0;
				regs[REG_RATTR] = 0x0;
				regs[REG_PC] += 4;
				status = 1;
			} else if (dfar >= (uint32_t)user_data && dfar <= CONFIG_RAM_END) {
				pqueue = (FAR sq_queue_t *)mm_get_mmlist_bypid(heap, pid);
				/* If the PID list exist, run through the list for address match else */
				if (pqueue) {
					status = arm_find_addrmatch(regs, pqueue, dfar);
				}
#ifdef HAVE_GROUP_MEMBERS
				/* If no address match in the current pid, traverse all the pids in the group */
				if (status < 0) {
					group = current->group;
					for (i = 0; i < group->tg_nmembers && status < 0; i++) {
						pqueue = (FAR sq_queue_t *)mm_get_mmlist_bypid(heap, group->tg_members[i]);
						status = arm_find_addrmatch(regs, pqueue, dfar);
					}
				}
#endif
			}
			heap->alloc_state[pid] = MM_RESET;
		}
	}
#endif

	if (status < 0) {
		/* Crash -- possibly showing diagnostic debug information. */
		lldbg("\nData abort. PC: %08x DFAR: %08x DFSR: %08x\n", regs[REG_PC], dfar, dfsr);
		PANIC();
	}

	current_regs = NULL;
	return regs;				/* To keep the compiler happy */
}
