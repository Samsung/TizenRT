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
#include <tinyara/mm/mm.h>
#include <stdint.h>
#include <sys/types.h>
#include <debug.h>
#include <errno.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* If multiple regions are supported or if multi-heap is supported in the project,
then we dont know how to configure the partitions for user applications. Hence we
will throw an error here
*/

#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(__KERNEL__)

#if CONFIG_MM_REGIONS > 1
#error "Unable to handle APP RAM PARTITIONS for CONFIG_MM_REGIONS > 1"
#endif

#if CONFIG_MM_NHEAPS > 1
#error "Unable to handle APP RAM PARTITIONS for CONFIG_MM_NHEAPS > 1"
#endif

#define ALIGN_UP_MASK		3
#define ALIGN_UP(a)		((a) + ALIGN_UP_MASK) & ~ALIGN_UP_MASK
/****************************************************************************
 * Public Variables
 ****************************************************************************/
extern uint32_t __usram_segment_start__[];
extern uint32_t __usram_segment_size__[];

/****************************************************************************
 * Private variables
 ****************************************************************************/
uint32_t g_default_size;
struct mm_heap_s g_pheap;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: mm_initialize_ram_partitions
 *
 * Description:
 * This function is called during os boot to initialize the memory partitions.
 *
 * Parameters:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

void mm_initialize_ram_partitions(void)
{
	/* We will calculate a overhead value for default partition size.
	 * The overhead value will include the size of mm_heap_s and the
	 * allocation node and also a 100 byte buffer for future change in mm.
	 */
	uint32_t overhead = CONFIG_NUM_APPS * (SIZEOF_MM_ALLOCNODE + sizeof(struct mm_heap_s) + 100); 
#ifdef CONFIG_IMXRT_SEMC_SDRAM
	uint32_t start = CONFIG_IMXRT_SDRAM_START + CONFIG_MM_KERNEL_HEAPSIZE;
	uint32_t size = CONFIG_IMXRT_SDRAM_SIZE - CONFIG_MM_KERNEL_HEAPSIZE;
	g_default_size = (size - overhead) / CONFIG_NUM_APPS;

	DEBUGASSERT(start != 0);
	DEBUGASSERT(size != 0);
	DEBUGASSERT(g_default_size > 0);

	mm_initialize(&g_pheap, start, size);
	mvdbg("Default RAM partition size set to %u\n", g_default_size);
	mvdbg("Initialized partition heap start = 0x%x size = %u\n", start, size);
#else
	uint32_t start = (uint32_t)__usram_segment_start__ + (uint32_t)__usram_segment_size__;

	g_default_size = (REGION_END - start - overhead) / CONFIG_NUM_APPS;
	mvdbg("Default RAM partition size set to %u\n", g_default_size);

	mm_initialize(&g_pheap, start, (uint32_t)(REGION_END - start));
	mvdbg("Initialized partition heap start = 0x%x size = %u\n", start, (uint32_t)(REGION_END - start));
#endif
}

/****************************************************************************
 * Name: mm_allocate_ram_partition
 *
 * Description:
 * This function is called to allocate a user app partition in which the app
 * will be loaded by the loader.
 *
 * Parameters:
 *   start_addr : OUT param to hold start address of allocated partition
 *   size	: IN/OUT  param to hold size of allocated partition
 *
 * Return Value:
 *   success: Zero value if partition was successfuly allocated.
 *
 * Assumptions:
 *
 ****************************************************************************/

int8_t mm_allocate_ram_partition(uint32_t **start_addr, uint32_t *size)
{
	if (*size == 0) {
		*size = g_default_size;
	}

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	ARCH_GET_RET_ADDRESS
	*start_addr = mm_malloc(&g_pheap, *size + sizeof(struct mm_heap_s), retaddr);
#else
	*start_addr = mm_malloc(&g_pheap, *size + sizeof(struct mm_heap_s));
#endif

	if (*start_addr == NULL) {
		mdbg("Failed to allocate RAM partition of size %u\n", *size);
		return -ENOMEM;
	}

	/* struct mm_heap_s will be situated at start of partition and heap will be initialized after this */
	mm_initialize((struct mm_heap_s *)*start_addr, (uint8_t *)*start_addr + sizeof(struct mm_heap_s), *size);

	mvdbg("Allocated RAM partition with start = 0x%x size = %u\n", *start_addr, *size);
	return OK;
}

/****************************************************************************
 * Name: mm_free_ram_partition
 *
 * Description:
 * This function is called to free a partition at the time of exiting or
 * killing an application.
 *
 * Parameters:
 *   address: The start address of the partition to free
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

void mm_free_ram_partition(uint32_t address)
{
	mm_free(&g_pheap, address);
	mvdbg("Freed RAM partition at 0x%x\n", address);
}

#endif /* defined(CONFIG_APP_BINARY_SEPARATION) && defined(__KERNEL__) */
