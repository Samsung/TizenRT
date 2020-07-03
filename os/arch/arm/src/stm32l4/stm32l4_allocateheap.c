/****************************************************************************
 * arch/arm/src/stm32l4/stm32l4_allocateheap.c
 *
 *   Copyright (C) 2011-2013, 2015 Gregory Nutt. All rights reserved.
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

#include <sys/types.h>
#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <tinyara/kmalloc.h>

#include <arch/board/board.h>

#include "chip.h"
#include "mpu.h"
#include "up_arch.h"
#include "up_internal.h"

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
 * Name: up_heap_color
 *
 * Description:
 *   Set heap memory to a known, non-zero state to checking heap usage.
 *
 ****************************************************************************/

#ifdef CONFIG_HEAP_COLORATION
static inline void up_heap_color(FAR void *start, size_t size)
{
	memset(start, HEAP_COLOR, size);
}
#else
#define up_heap_color(start, size)
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_allocate_kheap
 *
 * Description:
 *   For the kernel build (CONFIG_BUILD_PROTECTED=y) with both kernel- and
 *   user-space heaps (CONFIG_MM_KERNEL_HEAP=y), this function allocates
 *   (and protects) the kernel-space heap.
 *
 ****************************************************************************/

void up_allocate_kheap(FAR void **heap_start, size_t *heap_size)
{
	*heap_start = (FAR void *)(g_idle_topstack & ~(0x7));

	/* There may be a special scenario where we might configure a different region
	 * for heap. In such case, if end of bss falls outside of the region address range,
	 * then we use the whole region for heap.
	 */
	if (*heap_start < (void *)KREGION_START || *heap_start > (void *)KREGION_END) {
		*heap_start = (void *)KREGION_START;
	}

	*heap_size = (void *)KREGION_END - *heap_start;

	dbg("start = 0x%x size = %d\n", *heap_start, *heap_size);
	up_heap_color((FAR void *)*heap_start, *heap_size);
}

/****************************************************************************
 * Name: up_add_kregion
 ****************************************************************************/
#if defined(CONFIG_MM_KERNEL_HEAP) && (CONFIG_KMM_REGIONS > 1)
void up_add_kregion(void)
{
	int region_cnt;
	struct mm_heap_s *kheap;
	kheap = kmm_get_heap();
	for (region_cnt = 1; region_cnt < CONFIG_KMM_REGIONS; region_cnt++) {
		if (kheap[regionx_kheap_idx[region_cnt]].mm_heapsize == 0) {
			mm_initialize(&kheap[regionx_kheap_idx[region_cnt]], kregionx_start[region_cnt], kregionx_size[region_cnt]);
			continue;
		}
		mm_addregion(&kheap[regionx_kheap_idx[region_cnt]], kregionx_start[region_cnt], kregionx_size[region_cnt]);
	}
}
#endif
