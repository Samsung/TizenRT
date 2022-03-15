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
 * mm/kmm_heap/kmm_mallinfo.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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

#include <stdlib.h>

#include <tinyara/mm/mm.h>

#ifdef CONFIG_MM_KERNEL_HEAP

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: kmm_mallinfo
 *
 * Description:
 *   kmm_mallinfo returns a copy of updated current heap information for the
 *   kernel heap
 *
 ****************************************************************************/

#ifdef CONFIG_CAN_PASS_STRUCTS

struct mallinfo kmm_mallinfo(void)
{
	struct mallinfo info;
	int kheap_idx;
	struct mm_heap_s *kheap = kmm_get_baseheap();
#if CONFIG_KMM_NHEAPS > 1
	info.arena = 0;
	info.fordblks = 0;
	info.mxordblk = 0;
	info.ordblks = 0;
	info.uordblks = 0;
#endif
	for (kheap_idx = 0; kheap_idx < CONFIG_KMM_NHEAPS; kheap_idx++) {
		mm_mallinfo(&kheap[kheap_idx], &info);
	}
	return info;
}

#else

int kmm_mallinfo(struct mallinfo *info)
{
	int kheap_idx;
	struct mm_heap_s *kheap = kmm_get_baseheap();
#if CONFIG_KMM_NHEAPS > 1
	if (!info) {
		mdbg("info is NULL\n");
		return ERROR;
	}

	info->arena = 0;
	info->fordblks = 0;
	info->mxordblk = 0;
	info->ordblks = 0;
	info->uordblks = 0;
#endif
	for (kheap_idx = 0; kheap_idx < CONFIG_KMM_NHEAPS; kheap_idx++) {
		return mm_mallinfo(&kheap[kheap_idx], info);
	}
}

#endif							/* CONFIG_CAN_PASS_STRUCTS */
#endif							/* CONFIG_MM_KERNEL_HEAP */
