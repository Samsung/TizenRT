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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <debug.h>
#include <errno.h>

#include <tinyara/mm/mm.h>
#include <tinyara/kmalloc.h>
#include <tinyara/binfmt/binfmt.h>
#include <tinyara/mmu.h>

#include "binfmt.h"

#if defined(CONFIG_BINFMT_ENABLE) && defined(CONFIG_ARCH_USE_MMU)

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binfmt_setup_app_pgtable
 *
 * Description:
 *   
 *
 * Returned Value:
 *   Zero (OK) is returned on success; Otherwise a negated errno value.
 *
 ****************************************************************************/

void binfmt_setup_app_pgtable(struct binary_s *binp)
{
	uint32_t *l1tbl = 0;

	// Check if this is common binary.
#ifdef CONFIG_SUPPORT_COMMON_BINARY
	if (binp->islibrary) {
		// Yes. We will update the kernel page tables for common binary.
		l1tbl = mmu_get_os_l1_pgtbl();
	} else
#endif
	{
		// No. We will create separate page tables for the app.
	
		// Allocate L1 page table for app.
		l1tbl = mmu_allocate_app_l1_pgtbl(binp->binary_idx);

		// Loop through the L1 page table.
		// Copy kernel L1 page table to app page table.
		// If the entry is pointing to a L2 page table
		// Allocate L2 page table for app.
		// Copy entries from kernel to app L2 table.
		// Update the L2 page table address in L2 table.
		mmu_update_app_l1_pgtbl_ospgtbl(l1tbl);

		// Set the pointer of the L1 pgtbl into the tcb so that it is
		// passed on to all the tasks in the app.
		sched_self()->pgtbl = l1tbl;
	}


	// We will create page table entries such that we minimize the total 
	// number of entries by allocating the largest possible memory region
	// for each entry. 

	// Get the start and end address of the memory region.
	// Map the region to the page tables.
#ifdef CONFIG_ELF

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	/* Configure text section as RO and executable region */
	mmu_map_app_region(binp->binary_idx, l1tbl, binp->sections[BIN_TEXT], binp->sizes[BIN_TEXT], true, true);
	/* Configure ro section as RO and non-executable region */
	mmu_map_app_region(binp->binary_idx, l1tbl, binp->sections[BIN_RO], binp->sizes[BIN_RO], true, false);
	/* Complete RAM partition will be configured as RW region */
	mmu_map_app_region(binp->binary_idx, l1tbl, binp->sections[BIN_DATA], binp->ramsize, false, false);
#else
	/* Complete RAM partition will be configured as RW region */
	mmu_map_app_region(binp->binary_idx, l1tbl, binp->ramstart, binp->ramsize, false, true);
#endif

#else // ELF

	/* map entire flash region of app as rw */
	//mmu_map_app_region(binp->binary_idx, l1tbl, binp->flash_region_start, binp->flash_region_end - binp->flash_region_start, true, true);
	
	/* map entire RAM region of app as rx */
	mmu_map_app_region(binp->binary_idx, l1tbl, binp->ram_region_start, binp->ram_region_end - binp->ram_region_start, false, true);
#endif // ELF

}

#endif 	//defined(CONFIG_BINFMT_ENABLE) && defined(CONFIG_ARCH_USE_MMU)
