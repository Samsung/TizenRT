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

#define ALIGN_EVEN_MASK		2
#define ALIGN_EVEN(a)		((a) + ALIGN_EVEN_MASK) & ~ALIGN_EVEN_MASK
/****************************************************************************
 * Public Variables
 ****************************************************************************/
extern uint32_t __ksram_segment_start__[];
extern uint32_t __ksram_segment_size__[];

/****************************************************************************
 * Private variables
 ****************************************************************************/
static struct mm_ram_partition_s g_mempartitions[CONFIG_NUM_APPS];
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
	int i = 0;
	uint32_t start;
	uint32_t size;

	start = (uint32_t)__ksram_segment_start__ + (uint32_t)__ksram_segment_size__;
	size = (REGION_SIZE - (uint32_t)__ksram_segment_size__) / CONFIG_NUM_APPS;
	for (i = 0; i < CONFIG_NUM_APPS; i++) {
		g_mempartitions[i].start = ALIGN_EVEN(start);
		g_mempartitions[i].size = size;
		g_mempartitions[i].status = MM_PART_FREE;
		start += size;
	}
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
 *   size	: OUT param to hold size of allocated partition
 *
 * Return Value:
 *   success: Zero value if partition was successfuly allocated.
 *
 * Assumptions:
 *
 ****************************************************************************/

int8_t mm_allocate_ram_partition(uint32_t **start_addr, uint32_t *size)
{
	int i;

	for (i = 0; i < CONFIG_NUM_APPS; i++) {
		if (g_mempartitions[i].status == MM_PART_FREE) {
			g_mempartitions[i].status = MM_PART_USED;
			*start_addr = (uint32_t *)g_mempartitions[i].start;
			*size = g_mempartitions[i].size;
			mvdbg("Allocated partition Start = 0x%x Size = %dbytes\n", *start_addr, *size);
			return OK;
		}
	}

	return ERROR;
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
	int i;
	for (i = 0; i < CONFIG_NUM_APPS; i++) {
		if (g_mempartitions[i].start == address) {
			g_mempartitions[i].status = MM_PART_FREE;
			return;
		}
	}

	mdbg("ERROR: Could not free partition for address 0x%x\n", address);
}

#endif /* defined(CONFIG_APP_BINARY_SEPARATION) && defined(__KERNEL__) */
