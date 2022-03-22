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
/************************************************************************
 * mm/kmm_heap/kmm_kernel.c
 *
 *   Copyright (C) 2013-2014 Gregory Nutt. All rights reserved.
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
 ************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>

#include <assert.h>

#include <tinyara/kmalloc.h>
#include <tinyara/mm/mm.h>

#if ((defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__)) || \
	 defined(CONFIG_BUILD_KERNEL)) && defined(CONFIG_MM_KERNEL_HEAP)

/************************************************************************
 * Pre-processor definition
 ************************************************************************/

/************************************************************************
 * Private Types
 ************************************************************************/

/************************************************************************
 * Public Data
 ************************************************************************/

/************************************************************************
 * Private Functions
 ************************************************************************/

/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name: kmm_heapmember
 *
 * Description:
 *   Check if an address lies in the kernel heap.
 *
 * Parameters:
 *   mem - The address to check
 *
 * Return Value:
 *   true if the address is a member of the kernel heap.  false if not
 *   not.  If the address is not a member of the kernel heap, then it
 *   must be a member of the user-space heap (unchecked)
 *
 ************************************************************************/

bool kmm_heapmember(FAR void *mem)
{
	int kheap_idx;
	int region_idx;
	struct mm_heap_s *kheap = kmm_get_baseheap();

	/* A valid address from the kernel heap for this region would have to lie
	 * between the region's two guard nodes.
	 */

	for (kheap_idx = 0; kheap_idx < CONFIG_KMM_NHEAPS; kheap_idx++) {
		for (region_idx = 0; region_idx < CONFIG_KMM_REGIONS; region_idx++) {
			if (mem >= (FAR void *)kheap[kheap_idx].mm_heapstart[region_idx] && mem < (FAR void *)kheap[kheap_idx].mm_heapend[region_idx]) {
				return true;
			}
		}
	}

	/* The address does not like any any region assigned to kernel heap */

	return false;
}

#endif							/* ((CONFIG_BUILD_PROTECTED && __KERNEL__) || CONFIG_BUILD_KERNEL)  && CONFIG_MM_KERNEL_HEAP */
