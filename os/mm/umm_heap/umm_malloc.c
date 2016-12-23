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
 * mm/umm_heap/umm_malloc.c
 *
 *   Copyright (C) 2007, 2009, 2013-2014  Gregory Nutt. All rights reserved.
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
#include <unistd.h>

#include <tinyara/mm/mm.h>

#if !defined(CONFIG_BUILD_PROTECTED) || !defined(__KERNEL__)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
/* In the kernel build, there a multiple user heaps; one for each task
 * group.  In this build configuration, the user heap structure lies
 * in a reserved region at the beginning of the .bss/.data address
 * space (CONFIG_ARCH_DATA_VBASE).  The size of that region is given by
 * ARCH_DATA_RESERVE_SIZE
 */

#include <tinyara/addrenv.h>
#define USR_HEAP (&ARCH_DATA_RESERVE->ar_usrheap)

#else
/* Otherwise, the user heap data structures are in common .bss */

#define USR_HEAP &g_mmheap
#endif

/****************************************************************************
 * Type Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/************************************************************************
 * Name: malloc
 *
 * Description:
 *   Allocate memory from the user heap.
 *
 * Parameters:
 *   size - Size (in bytes) of the memory region to be allocated.
 *
 * Return Value:
 *   The address of the allocated memory (NULL on failure to allocate)
 *
 ************************************************************************/

FAR void *malloc(size_t size)
{
#ifdef CONFIG_BUILD_KERNEL
	FAR void *brkaddr;
	FAR void *mem;

	/* Loop until we successfully allocate the memory or until an error
	 * occurs. If we fail to allocate memory on the first pass, then call
	 * sbrk to extend the heap by one page.  This may require several
	 * passes if more the size of the allocation is more than one page.
	 *
	 * An alternative would be to increase the size of the heap by the
	 * full requested allocation in sbrk().  Then the loop should never
	 * execute more than twice (but more memory than we need may be
	 * allocated).
	 */

	do {
		mem = mm_malloc(USR_HEAP, size);
		if (!mem) {
			brkaddr = sbrk(size);
			if (brkaddr == (FAR void *)-1) {
				return NULL;
			}
		}
	} while (mem == NULL);

	return mem;
#else
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	ARCH_GET_RET_ADDRESS
	return mm_malloc(USR_HEAP, size, retaddr);
#else
	return mm_malloc(USR_HEAP, size);
#endif
#endif
}

#endif							/* !CONFIG_BUILD_PROTECTED || !__KERNEL__ */
