/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * kernel/debug/stackinfo_save_terminated.c
 *
 *   Copyright (C) 2007, 2009 Gregory Nutt. All rights reserved.
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
#include <time.h>
#include <sys/types.h>

#include <tinyara/clock.h>
#include <tinyara/sched.h>
#ifdef CONFIG_DEBUG_MM_HEAPINFO
#include <tinyara/mm/mm.h>
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/
static struct stkmon_save_s terminated_thread_stkinfo[CONFIG_MAX_TASKS * 2];
static int stkmon_chk_idx;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: stkmon_copy_log
 *
 * Description:
 *   This function copies the terminated task/pthread's information to
 *  user buffer.
 *
 ****************************************************************************/
void stkmon_copy_log(struct stkmon_save_s *dest_arr)
{
	int thread_idx;

	for (thread_idx = 0; thread_idx < STKMON_MAX_LOGS; thread_idx++) {
		if (terminated_thread_stkinfo[thread_idx].timestamp != 0) {
			memcpy(&dest_arr[thread_idx], &terminated_thread_stkinfo[thread_idx], sizeof(struct stkmon_save_s));
		}
	}
}

/****************************************************************************
 * Name: stackinfo_save_terminated
 *
 * Description:
 *   This function saves the terminated task/pthread's information for
 *  stack monitor.
 *
 ****************************************************************************/
void stackinfo_save_terminated(struct tcb_s *tcb)
{
#if (CONFIG_TASK_NAME_SIZE > 0)
	int name_idx;
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	struct mm_heap_s *heap;
#endif
	terminated_thread_stkinfo[stkmon_chk_idx % STKMON_MAX_LOGS].timestamp = clock();
	terminated_thread_stkinfo[stkmon_chk_idx % STKMON_MAX_LOGS].chk_pid = tcb->pid;
	terminated_thread_stkinfo[stkmon_chk_idx % STKMON_MAX_LOGS].chk_stksize = tcb->adj_stack_size;
#ifdef CONFIG_STACK_COLORATION
	terminated_thread_stkinfo[stkmon_chk_idx % STKMON_MAX_LOGS].chk_peaksize = up_check_tcbstack(tcb);
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	heap = mm_get_heap(tcb->stack_alloc_ptr);
	if (heap == NULL) {
		return;
	}
	terminated_thread_stkinfo[stkmon_chk_idx % STKMON_MAX_LOGS].chk_peakheap = heap->alloc_list[PIDHASH(tcb->pid)].peak_alloc_size;
#endif
#if (CONFIG_TASK_NAME_SIZE > 0)
	for (name_idx = 0; name_idx < CONFIG_TASK_NAME_SIZE + 1; name_idx++) {
		terminated_thread_stkinfo[stkmon_chk_idx % STKMON_MAX_LOGS].chk_name[name_idx] = tcb->name[name_idx];
	}
#endif
	stkmon_chk_idx %= STKMON_MAX_LOGS;
	stkmon_chk_idx++;
}

