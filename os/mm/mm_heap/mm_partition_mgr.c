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
#define EXTRA_PADDING		100
/****************************************************************************
 * Public Variables
 ****************************************************************************/
extern uint32_t __usram_segment_start__[];
extern uint32_t __usram_segment_size__[];

/****************************************************************************
 * Private variables
 ****************************************************************************/
uint32_t g_default_size;
uint32_t g_ram_start;
uint32_t g_ram_size;

#ifdef CONFIG_MM_PARTITION_HEAP
struct mm_heap_s g_pheap;
#else
struct mm_part_s {
	uint32_t start;
	uint32_t size;
};

struct mm_part_s g_mm_parts[CONFIG_NUM_APPS];
#endif

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
	 * allocation node and also a 100 byte buffer (EXTRA_PADDING) for future change in mm.
	 */
#ifdef CONFIG_MM_PARTITION_HEAP
	uint32_t overhead = CONFIG_NUM_APPS * (SIZEOF_MM_ALLOCNODE + sizeof(struct mm_heap_s) + EXTRA_PADDING);
#else
	uint32_t overhead = 0;
#endif

#ifdef CONFIG_IMXRT_SEMC_SDRAM
	g_ram_start = CONFIG_IMXRT_SDRAM_START + CONFIG_MM_KERNEL_HEAPSIZE;
	g_ram_size = CONFIG_IMXRT_SDRAM_SIZE - CONFIG_MM_KERNEL_HEAPSIZE;
#else
	g_ram_start = (uint32_t)__usram_segment_start__ + (uint32_t)__usram_segment_size__;
	g_ram_size = (uint32_t)(REGION_END - g_ram_start);
#endif

	g_default_size = (g_ram_size - overhead) / CONFIG_NUM_APPS;
	DEBUGASSERT(g_ram_start != 0);
	DEBUGASSERT(g_ram_size != 0);
	DEBUGASSERT(g_default_size > 0);

#ifdef CONFIG_MM_PARTITION_HEAP
	mm_initialize(&g_pheap, (void *)g_ram_start, g_ram_size);
#else
	g_mm_parts[0].start = g_ram_start;
	g_mm_parts[0].size = 0;
	g_mm_parts[1].start = g_ram_start + g_ram_size;
	g_mm_parts[1].size = 0;
#endif

	mvdbg("Default RAM partition size set to %u\n", g_default_size);
	mvdbg("Initialized partition heap start = 0x%x size = %u\n", g_ram_start, g_ram_size);

	mm_initialize_app_heap();
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

int8_t mm_allocate_ram_partition(uint32_t **start_addr, uint32_t *size, char *name)
{
	if (*size == 0) {
		*size = g_default_size;
	}

#ifdef CONFIG_MM_PARTITION_HEAP

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	ARCH_GET_RET_ADDRESS
	*start_addr = mm_malloc(&g_pheap, *size + sizeof(struct mm_heap_s), retaddr);
#else
	*start_addr = mm_malloc(&g_pheap, *size + sizeof(struct mm_heap_s));
#endif

#else	/* CONFIG_MM_PARTITION_HEAP */
	uint32_t overhead = sizeof(struct mm_heap_s);
	if (g_mm_parts[0].size == 0 && g_mm_parts[1].start > g_ram_start + *size + overhead) {
		*start_addr = (uint32_t *)g_ram_start;
		g_mm_parts[0].size = *size + overhead;
	} else if (g_mm_parts[1].size == 0 && g_ram_start + g_ram_size - *size - overhead > g_ram_start + g_mm_parts[0].size) {
		*start_addr = (uint32_t *)(g_ram_start + g_ram_size - *size - overhead);
		g_mm_parts[1].start = *start_addr;
		g_mm_parts[1].size = *size + overhead;
	} else {
		mvdbg("Part 1 Start = 0x%x, size = %u\n", g_mm_parts[0].start, g_mm_parts[0].size);
		mvdbg("Part 2 Start = 0x%x, size = %u\n", g_mm_parts[1].start, g_mm_parts[1].size);
	}
#endif	/* CONFIG_MM_PARTITION_HEAP */

	if (*start_addr == NULL) {
		mdbg("Failed to allocate RAM partition of size %u\n", *size);
		return -ENOMEM;
	}

	/* struct mm_heap_s will be situated at start of partition and heap will be initialized after this */
	mm_initialize((struct mm_heap_s *)*start_addr, (uint8_t *)*start_addr + sizeof(struct mm_heap_s), *size);
	mm_add_app_heap_list((struct mm_heap_s *)*start_addr, name);

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
	mm_remove_app_heap_list((struct mm_heap_s *)address);
#ifdef CONFIG_MM_PARTITION_HEAP
	mm_free(&g_pheap, (void *)address);
#else
	if (g_mm_parts[0].start == address) {
		g_mm_parts[0].size = 0;
	} else if (g_mm_parts[1].start == address) {
		g_mm_parts[1].size = 0;
		g_mm_parts[1].start = g_ram_start + g_ram_size;
	} else {
		mdbg("Incorrect partition address 0x%x\n", address);
	}
#endif
	mvdbg("Freed RAM partition at 0x%x\n", address);
}

#endif /* defined(CONFIG_APP_BINARY_SEPARATION) && defined(__KERNEL__) */
