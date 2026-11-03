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
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/amebasmart/amebasmart_allocateheap.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>

#include <tinyara/arch.h>

#include "arm_internal.h"
#include "mmu.h"
#include "chip.h"

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Start of the free PSRAM after the image (see the linker script) */

extern uint8_t _psram_heap_start[];

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_allocate_heap
 *
 * Description:
 *   Return the heap of the FLAT build.  The common arm_allocateheap.c of
 *   NuttX starts the heap at g_idle_topstack, but on AmebaSmart the IDLE
 *   stack and the page tables are at the end of the PSRAM (see
 *   hardware/amebasmart_memorymap.h).  The heap is the PSRAM between the
 *   end of the image and the IDLE stack.
 *
 ****************************************************************************/

void up_allocate_heap(void **heap_start, size_t *heap_size)
{
	uintptr_t base = (uintptr_t)_psram_heap_start;
	uintptr_t end = IDLE_STACK_VBASE;

	*heap_start = (void *)base;
	*heap_size = end - base;
}
