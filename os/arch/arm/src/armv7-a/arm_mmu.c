/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/armv7-a/arm_mmu.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>

#include "cp15_cacheops.h"
#include "mmu.h"

#ifdef CONFIG_APP_BINARY_SEPARATION
#include <tinyara/mm/mm.h>
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/
static int l2_idx = 0;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void mmu_set_flags(uint32_t *val, bool ro, bool exec, uint8_t isL1)
{
	if (isL1) {
		if (ro && exec) {
			*val |= MMU_APP_L1_ROX;
		} else if (ro) {
			*val |= MMU_APP_L1_RO;
		} else if (exec) {
			*val |= MMU_APP_L1_RWX;
		} else {
			*val |= MMU_APP_L1_RW;
		}
	} else {
		if (ro && exec) {
			*val |= MMU_APP_L2_ROX;
		} else if (ro) {
			*val |= MMU_APP_L2_RO;
		} else if (exec) {
			*val |= MMU_APP_L2_RWX;
		} else {
			*val |= MMU_APP_L2_RW;
		}
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mmu_l1_setentry
 *
 * Description:
 *   Set a one level 1 translation table entry.  Only a single L1 page table
 *   is supported.
 *
 * Input Parameters:
 *   paddr - The physical address to be mapped.  Must be aligned to a 1MB
 *     address boundary
 *   vaddr - The virtual address to be mapped.  Must be aligned to a 1MB
 *     address boundary
 *   mmuflags - The MMU flags to use in the mapping.
 *
 ****************************************************************************/

#ifndef CONFIG_ARCH_ROMPGTABLE
void mmu_l1_setentry(uint32_t paddr, uint32_t vaddr, uint32_t mmuflags)
{
  uint32_t *l1table = mmu_l1_pgtable();
  uint32_t  index   = vaddr >> 20;

  /* Save the page table entry */

  l1table[index] = (paddr | mmuflags);

  /* Flush the data cache entry.  Make sure that the modified contents
   * of the page table are flushed into physical memory.
   */

  cp15_clean_dcache_bymva((uint32_t)&l1table[index]);

  /* Invalidate the TLB cache associated with virtual address range */

  mmu_invalidate_region(vaddr, SECTION_SIZE);
}
#endif

/****************************************************************************
 * Name: mmu_l1_restore
 *
 * Description:
 *   Restore one L1 table entry previously returned by mmu_l1_getentry() (or
 *   any other encoded L1 page table value).
 *
 * Input Parameters:
 *   vaddr - A virtual address to be mapped
 *   l1entry - The value to write into the page table entry
 *
 ****************************************************************************/

#if !defined(CONFIG_ARCH_ROMPGTABLE) && defined(CONFIG_ARCH_ADDRENV)
void mmu_l1_restore(uintptr_t vaddr, uint32_t l1entry)
{
  uint32_t *l1table = mmu_l1_pgtable();
  uint32_t  index   = vaddr >> 20;

  /* Set the encoded page table entry */

  l1table[index] = l1entry;

  /* Flush the data cache entry.  Make sure that the modified contents
   * of the page table are flushed into physical memory.
   */

  cp15_clean_dcache_bymva((uint32_t)&l1table[index]);

  /* Invalidate the TLB cache associated with virtual address range */

  mmu_invalidate_region(vaddr & PMD_PTE_PADDR_MASK, SECTION_SIZE);
}
#endif

/****************************************************************************
 * Name: mmu_l2_setentry
 *
 * Description:
 *   Set one small (4096B) entry in a level2 translation table.
 *
 * Input Parameters:
 *   l2vaddr - the virtual address of the beginning of the L2 translation
 *     table.
 *   paddr - The physical address to be mapped.  Must be aligned to a 4KB
 *     address boundary
 *   vaddr - The virtual address to be mapped.  Must be aligned to a 4KB
 *     address boundary
 *   mmuflags - The MMU flags to use in the mapping.
 *
 ****************************************************************************/

#ifndef CONFIG_ARCH_ROMPGTABLE
void mmu_l2_setentry(uint32_t l2vaddr, uint32_t paddr, uint32_t vaddr,
                     uint32_t mmuflags)
{
  uint32_t *l2table  = (uint32_t *)l2vaddr;
  uint32_t  index;

  /* The table divides a 1Mb address space up into 256 entries, each
   * corresponding to 4Kb of address space.  The page table index is
   * related to the offset from the beginning of 1Mb region.
   */

  index = (vaddr & 0x000ff000) >> 12;

  /* Save the table entry */

  l2table[index] = (paddr | mmuflags);

  /* Flush the data cache entry.  Make sure that the modified contents
   * of the page table are flushed into physical memory.
   */

  cp15_clean_dcache_bymva((uint32_t)&l2table[index]);

  /* Invalidate the TLB cache associated with virtual address range */

  cp15_invalidate_tlb_bymva(vaddr);
}
#endif

/****************************************************************************
 * Name: mmu_l1_map_region
 *
 * Description:
 *   Set multiple level 1 translation table entries in order to map a
 *   region of memory.
 *
 * Input Parameters:
 *   mapping - Describes the mapping to be performed.
 *
 ****************************************************************************/

#ifndef CONFIG_ARCH_ROMPGTABLE
void mmu_l1_map_region(const struct section_mapping_s *mapping)
{
  uint32_t paddr    = mapping->physbase;
  uint32_t vaddr    = mapping->virtbase;
  uint32_t mmuflags = mapping->mmuflags;
  int i;

  /* Loop, writing each mapping into the L1 page table */

  for (i = 0; i < mapping->nsections; i++)
    {
      mmu_l1_setentry(paddr, vaddr, mmuflags);
      paddr += SECTION_SIZE;
      vaddr += SECTION_SIZE;
    }
}
#endif

/****************************************************************************
 * Name: mmu_l1_map_regions
 *
 * Description:
 *   Set multiple level 1 translation table entries in order to map a region
 *   array of memory.
 *
 * Input Parameters:
 *   mappings - Describes the array of mappings to be performed.
 *   count    - The number of mappings to be performed.
 *
 ****************************************************************************/

#ifndef CONFIG_ARCH_ROMPGTABLE
void mmu_l1_map_regions(const struct section_mapping_s *mappings,
                        size_t count)
{
  size_t i;

  for (i = 0; i < count; i++)
    {
      mmu_l1_map_region(&mappings[i]);
    }
}
#endif

/****************************************************************************
 * Name: mmu_invalidate_region
 *
 * Description:
 *   Invalidate TLBs for a range of addresses (all 4KB aligned).
 *
 * Input Parameters:
 *   vaddr - The beginning of the region to invalidate.
 *   size  - The size of the region in bytes to be invalidated.
 *
 ****************************************************************************/

#ifndef CONFIG_ARCH_ROMPGTABLE
void mmu_invalidate_region(uint32_t vstart, size_t size)
{
  uint32_t vaddr = vstart & 0xfffff000;
  uint32_t vend  = vstart + size;

  /* Loop, invalidating regions */

  while (vaddr < vend)
    {
      cp15_invalidate_tlb_bymva(vaddr);
      vaddr += 4096;
    }
}
#endif

#ifdef CONFIG_APP_BINARY_SEPARATION
/****************************************************************************
 * Name: mmu_get_os_l1_pgtbl
 *
 * Description:
 *   Returns the virtual address of the kernel L1 page table.
 *
 * Input Parameters:
 *
 * Returned Value:
 * Page table address
 ****************************************************************************/
uint32_t *mmu_get_os_l1_pgtbl(void)
{
	return (uint32_t *)PGTABLE_BASE_VADDR;
}

/****************************************************************************
 * Name: mmu_allocate_app_l1_pgtbl
 *
 * Description:
 *   Allocate space for L1 page table of application, in accordance with
 *   the requirements of the arch specific mmu.
 *
 * Input Parameters:
 *
 * Returned Value:
 * L1 Page table address
 ****************************************************************************/
uint32_t *mmu_allocate_app_l1_pgtbl(int app_id)
{
	uint32_t *addr = (uint32_t *)(PGTABLE_BASE_VADDR + (app_id * L1_PGTBL_SIZE));
	l2_idx = 0;
	return addr;
}

/****************************************************************************
 * Name: mmu_allocate_app_l2_pgtbl
 *
 * Description:
 *   Allocate space for L2 page table of application, in accordance with
 *   the requirements of the arch specific mmu.
 *
 * Input Parameters:
 *
 * Returned Value:
 * L2 Page table address
 ****************************************************************************/
uint32_t *mmu_allocate_app_l2_pgtbl(int app_id, int l2idx)
{
	/* Page table structure as follows:
	 *
	 * | Kernel L1 | App1 L1 | App2 L1 | Common L2 1 ... CONFIG_NUM_L2_PER_APP | App1 L2 1 ... CONFIG_NUM_L2_PER_APP | App2 L2 1 ... CONFIG_NUM_L2_PER_APP |
	 *
	 * In the below calculation, we take (CONFIG_NUM_APPS + 1) because the first
	 * page table is for kernel. Also, we reserve CONFIG_NUM_L2_PER_APP L2 page tables for each app.
	 */
	uint32_t *addr = (uint32_t *)(PGTABLE_BASE_VADDR + ((CONFIG_NUM_APPS + 1) * L1_PGTBL_SIZE) + 
			(app_id * CONFIG_NUM_L2_PER_APP * L2_PGTBL_SIZE) + (l2idx * L2_PGTBL_SIZE));
	return addr;
}

/****************************************************************************
 * Name: mmu_update_app_l1_pgtbl_ospgtbl
 *
 * Description:
 * Loop through the L1 page table.
 * Copy kernel L1 page table to app page table.
 * If the entry is pointing to a L2 page table
 * Allocate L2 page table for app.
 * Copy entries from kernel to app L2 table.
 * Update the L2 page table address in L1 table.
 * 
 * Input Parameters:
 * app_pgtbl: Pointer to L1 page table of app
 *
 ****************************************************************************/
void mmu_update_app_l1_pgtbl_ospgtbl(uint32_t *app_l1_pgtbl)
{
	uint32_t *os_l1_pgtbl = (uint32_t *)PGTABLE_BASE_VADDR;
	memcpy((void *)app_l1_pgtbl, (void *)os_l1_pgtbl, L1_PGTBL_SIZE);
}

/****************************************************************************
 * Name: mmu_clear_app_pgtbl
 *
 * Description:
 * Clear the app page tables. If required reset the entries in L1 page table.
 *
 * Input Parameters:
 * app_id : Application id for clearing page tables.
 *
 * Returned Value:
 *
 ****************************************************************************/
void mmu_clear_app_pgtbl(uint32_t app_id)
{
	uint32_t *addr;

	// Clear L2 page tables
	addr = (uint32_t *)(PGTABLE_BASE_VADDR + ((CONFIG_NUM_APPS + 1) * L1_PGTBL_SIZE) +
                       (app_id * CONFIG_NUM_L2_PER_APP * L2_PGTBL_SIZE));
	memset(addr, 0, CONFIG_NUM_L2_PER_APP * L2_PGTBL_SIZE);
	
	if (app_id == 0) {
		// Reset the L2 page entries in L1 page table
		addr = (uint32_t *)PGTABLE_BASE_VADDR;
		for (int i = 0; i < L1_PGTBL_NENTRIES; i++) {
			if((addr[i] & PMD_TYPE_MASK) == PMD_TYPE_PTE) {
				addr[i] = addr[i - 1] & PMD_SECT_PADDR_MASK;
				addr[i] += SECTION_SIZE;
				addr[i] |= MMU_MEMFLAGS;
			}
		}
	} else {
		// Clear L1 page table
		addr = (uint32_t *)(PGTABLE_BASE_VADDR + (app_id * L1_PGTBL_SIZE));
		memset(addr, 0, L1_PGTBL_SIZE);
	}

	cp15_wrttb((uint32_t)(mmu_get_os_l1_pgtbl()) | TTBR0_RGN_WBWA | TTBR0_IRGN0);
	cp15_invalidate_tlbs();
}

/****************************************************************************
 * Name: mmu_map_app_region
 *
 * Description:
 * The api will create page tables for the given section of app and update them with the
 * access permissions as mentioned in the input parameters. While updating the page table,
 * following is done:
 * - Check if the given region can exactly fit into a 1MB section.
 * - In this case, update the corresponding section in the l1 page table.
 * - Otherwise, create an L2 page table and update it with default entries.
 * - Later, update the access permissions in the L2 page table for the given memory region.
 * - Update the L2 page table address in L1 page table. 
 *
 * Input Parameters:
 * app_id :	Id of the app being updated.
 * l1_pgtbl:	Pointer to the L1 page table allocated for the app.
 * start:	Start address of memory region to be mapped.
 * size:	Size of the memory region to be mapped.
 * ro:		Whether this region is read-only?
 * exec:	Whether this region has execute permission?
 *
 * Returned Value:
 ****************************************************************************/
void mmu_map_app_region(int app_id, uint32_t *l1_pgtbl, uint32_t start, uint32_t size, bool ro, bool exec)
{
	uint32_t idx;
	uint32_t val;
	uint32_t end = start + size;

	// Run a loop until the entire region is mapped.
	while (start < end) {
		// Check if this address can be mapped to a section.
		if (!(start & SECTION_MASK) && !(size & SECTION_MASK)) {
			// Yes. Update the section entry in the the L1 page table.
			idx = start >> 20;
			val = start & PMD_PTE_PADDR_MASK;
			mmu_set_flags(&val, ro, exec, true);
			l1_pgtbl[idx] = val;

			// Advance the memory region address.
			start += SECTION_SIZE;
		} else {	// Check if this address can be mapped to a small page.

			// Check if L2 page table is not created.
			idx = (start & 0xfff00000) >> 20;
			uint32_t *l2_pgtbl = (uint32_t *)(l1_pgtbl[idx] & PMD_PTE_PADDR_MASK);
			
			if ((l1_pgtbl[idx] & PMD_TYPE_MASK) != PMD_TYPE_PTE) {
				// Yes. Allocate L2 page table for app.
				l2_pgtbl = mmu_allocate_app_l2_pgtbl(app_id, l2_idx++);

				// Fill the newly allocated l2 page table with default kernel page entries
				uint32_t l2_start = start & PMD_SECT_PADDR_MASK;
                                for (int i = 0; i < L2_PGTBL_NENTRIES; i++) {
                                        l2_pgtbl[i] = l2_start | MMU_L2_MEMFLAGS;
                                        l2_start += SMALL_PAGE_SIZE;
                                }

				// Update L2 page table address in L1 page table.
				val = (uint32_t)l2_pgtbl & PMD_PTE_PADDR_MASK;
				val |= MMU_L1_PGTABFLAGS;
				l1_pgtbl[idx] = val;
			}

			// Update the L2 page table entry.
  			idx = (start & 0x000ff000) >> 12;
			val = start & PTE_SMALL_PADDR_MASK;
			mmu_set_flags(&val, ro, exec, false);
			l2_pgtbl[idx] = val;

			// Advance the memory region address.
			start += SMALL_PAGE_SIZE;
		}
	}
	cp15_invalidate_tlbs();
}

/****************************************************************************
 * Name: mmu_dump_app_pgtbl
 *
 * Description:
 * Print the L1 and L2 page table entries corresponding to the application.
 *
 * Input Parameters:
 *
 * Returned Value:
 ****************************************************************************/
void mmu_dump_app_pgtbl(void)
{
	struct tcb_s *rtcb = sched_self();
	uint32_t *l1tbl = mmu_l1_pgtable();

	lldbg_noarg("L1 page table base addr = 0x%08x appid = %d\n", l1tbl, rtcb->app_id);

	if (rtcb->app_id < 1) {
		return;
	}

	lldbg_noarg("=====================================================================\n");
	lldbg_noarg("ENTRY      TYPE    OUT             ACCESS\n");
	lldbg_noarg("ADDR               ADDR                  \n");
	lldbg_noarg("=====================================================================\n");
	for (int i = 0; i < L1_PGTBL_NENTRIES; i++) {
		if ((l1tbl[i] & PMD_TYPE_MASK) == PMD_TYPE_SECT && 
				(l1tbl[i] & PMD_SECT_AP1)) {  	// Only print user areas.
			lldbg_noarg("0x%08x SECT    0x%08x      %s-%s\n", 
					&l1tbl[i], 
					l1tbl[i] & PMD_SECT_PADDR_MASK, 
					(l1tbl[i] & PMD_SECT_AP2) ? "RO" : "RW", 
					(l1tbl[i] & PMD_SECT_XN) ? "XN" : "X");
		} else if((l1tbl[i] & PMD_TYPE_MASK) == PMD_TYPE_PTE) {
			lldbg_noarg("0x%08x L1PTE   0x%08x\n", &l1tbl[i], l1tbl[i] & PMD_PTE_PADDR_MASK); 
			uint32_t *l2tbl = (uint32_t *)(l1tbl[i] & PMD_PTE_PADDR_MASK);
			for (int j = 0; j < L2_PGTBL_NENTRIES; j++) {
				if ((l2tbl[j] & PTE_AP1) && 	// Only print user areas.
						((l2tbl[j] & PTE_TYPE_MASK) != PTE_TYPE_FAULT)) {
					lldbg_noarg("0x%08x PAGE    0x%08x      %s-%s\n", 
						&l2tbl[j], 
						l2tbl[j] & PTE_SMALL_PADDR_MASK, 
						(l2tbl[j] & PTE_AP2) ? "RO" : "RW",
						(l2tbl[j] & PTE_SMALL_XN) ? "XN" : "X");
				}
			}
		}
	}
	lldbg_noarg("=====================================================================\n");
}
#endif // CONFIG_APP_BINARY_SEPARATION
