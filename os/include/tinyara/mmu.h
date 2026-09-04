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

///@file tinyara/mmu.h

#ifndef __INCLUDE_TINYARA_MMU_H
#define __INCLUDE_TINYARA_MMU_H

/********************************************************************************
 * Included Files
 ********************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>

/********************************************************************************
 * Pre-processor Definitions
 ********************************************************************************/

#define SMALL_PAGE_SZ		(4096)	/* 4KB small page */
#define MMU_ALIGNMENT_BYTES	SMALL_PAGE_SZ
#define MMU_ALIGN_UP(a)		(((a) + MMU_ALIGNMENT_BYTES - 1) & ~(MMU_ALIGNMENT_BYTES - 1))

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
/* Separate three MPU regions (text, ro and rw) to optimize reloading time */
#define NUM_APP_REGIONS     3
#else
/* Just a MPU region for all of section data */
#define NUM_APP_REGIONS     1
#endif


/********************************************************************************
 * Public Function Prototypes
 ********************************************************************************/
#ifdef CONFIG_APP_BINARY_SEPARATION
uint32_t *mmu_get_os_l1_pgtbl(void);
uint32_t *mmu_allocate_app_l1_pgtbl(int app_id);
uint32_t *mmu_allocate_app_l2_pgtbl(int app_id, int l2_idx);
void mmu_update_app_l1_pgtbl_ospgtbl(uint32_t *app_l1_pgtbl);
void mmu_map_app_region(int app_id, uint32_t *l1_pgtbl, uint32_t start, uint32_t size, bool ro, bool exec);
void mmu_dump_app_pgtbl(void);
void mmu_clear_app_pgtbl(uint32_t app_id);
#endif // CONFIG_APP_BINARY_SEPARATION

/********************************************************************************
 * MMU Page Protection Functions for Use-After-Free Detection
 * ********************************************************************************/

/****************************************************************************
 * Name: mmu_split_section_to_l2
 *
 * Description:
 *   Split a 1MB L1 section mapping into 256 × 4KB L2 small page mappings.
 *   This is necessary before using mmu_set_page_ro() or mmu_set_page_no_access()
 *   on a specific 4KB page within a 1MB section, otherwise the entire 1MB
 *   section would be affected.
 *
 * Input Parameters:
 *   vaddr - A virtual address within the 1MB section to split.
 *
 ****************************************************************************/

void mmu_split_section_to_l2(uint32_t vaddr);

/****************************************************************************
 * Name: mmu_save_page_pte

 *
 * Description:
 *   Save the current page table entry (L2 PTE or L1 section entry) for a
 *   given virtual address so it can be restored later.
 *
 * Input Parameters:
 *   vaddr - A virtual address within the page whose PTE should be saved.
 *
 ****************************************************************************/

uint32_t mmu_save_page_pte(uint32_t vaddr);

/****************************************************************************
 * Name: mmu_set_page_no_access
 *
 * Description:
 *   Set a 4KB page (or 1MB section) to No-Access at any privilege level.
 *   Any subsequent access triggers a Data Abort.  Used for use-after-free
 *   detection on page-aligned allocations.
 *
 * Input Parameters:
 *   vaddr - A virtual address within the page to protect.
 *
 ****************************************************************************/

void mmu_set_page_no_access(uint32_t vaddr);

/****************************************************************************
 * Name: mmu_set_page_ro
 *
 * Description:
 *   Set a 4KB page (or 1MB section) to Read-Only at privileged level.
 *   Any write triggers a Data Abort.  Used to protect read-only data
 *   structures from accidental corruption.
 *
 * Input Parameters:
 *   vaddr - A virtual address within the page to protect.
 *
 ****************************************************************************/

void mmu_set_page_ro(uint32_t vaddr);

/****************************************************************************
 * Name: mmu_restore_page_pte
 *
 * Description:
 *   Restore a previously saved PTE for a given virtual address, reverting
 *   the page to its original access permissions.
 *
 * Input Parameters:
 *   vaddr     - A virtual address within the page to restore.
 *   saved_pte - The PTE value previously returned by mmu_save_page_pte().
 *
 ****************************************************************************/

void mmu_restore_page_pte(uint32_t vaddr, uint32_t saved_pte);

#endif
