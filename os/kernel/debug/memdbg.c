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

/***************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdint.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/mm/mm.h>

/***************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

extern const uint32_t g_idle_topstack;

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: display_memory_information
 *
 * Description:
 *   This function prints the memory information for debug such as
 *   the RAM configuration, Idle stack configuration, Heap settings.
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/
void display_memory_information(void)
{
	int region_idx;
	int heap_idx;
	struct mm_heap_s *kheap = kmm_get_baseheap();

	mllwdbg("--------------------RAM Configuration---------------------\n");
	/* Print RAM configuration */

	for (region_idx = 0; region_idx < CONFIG_KMM_REGIONS; region_idx++) {
		mllwdbg("Kernel   RAM  [%d] : start addr = 0x%x size = %u\n", region_idx, kregionx_start[region_idx], kregionx_size[region_idx]);
	}

	/* Print Idle Stack configuration */

	mllwdbg("Idle stack     : base addr = 0x%x size = %u\n", g_idle_topstack, CONFIG_IDLETHREAD_STACKSIZE);
	mllwdbg("-----------------------------------------------------------\n");

	mllwdbg("------------------------------ Heap Settings ------------------------------\n");

	/* Print HEAP configuration */

	for (heap_idx = 0; heap_idx < CONFIG_KMM_NHEAPS; heap_idx++) {
		mllwdbg("Heap[%d] : size = %u, number of regions = %d\n", heap_idx, kheap[heap_idx].mm_heapsize, kheap[heap_idx].mm_nregions);
		region_idx = 0;
#if CONFIG_KMM_REGIONS > 1
		for (; region_idx < kheap[heap_idx].mm_nregions; region_idx++)
#endif
		{
			mllwdbg(" Region[%d] : start addr = %8p, end addr = %8p, size = %u\n", region_idx, kheap[heap_idx].mm_heapstart[region_idx], kheap[heap_idx].mm_heapend[region_idx], ((uint32_t)kheap[heap_idx].mm_heapend[region_idx] - (uint32_t)kheap[heap_idx].mm_heapstart[region_idx]));

		}
	}
	mllwdbg("---------------------------------------------------------------------------\n");
}
