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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <debug.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <queue.h>
#include <sys/types.h>
#include <tinyara/mm/mm.h>
#include <tinyara/mm/heap_regioninfo.h>
#include <arch/chip/memory_region.h>
#include <tinyara/binfmt/elf.h>

#include "binary_manager/binary_manager_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define CMN_BIN_IDX 0

/* Size of a single memory access while scanning for a reference. A reference
 * is always a pointer sized value, so the scan reads one pointer at a time.
 */
#define MEM_PTR_SIZE       ((size_t)sizeof(uintptr_t))

#define MAX_ALLOC_COUNT    CONFIG_MEM_LEAK_CHECKER_MAX_ALLOC_COUNT
#define MEM_DUMP_MAX_BYTES 32

#define MM_PREV_NODE_SIZE(x)            ((x)->preceding & ~MM_ALLOC_BIT)

#if CONFIG_KMM_REGIONS > 1
#define HEAP_NREGIONS(h)   ((h)->mm_nregions)
#else
#define HEAP_NREGIONS(h)   (1)
#endif

struct alloc_node_info_s {
	volatile struct mm_allocnode_s *node;
};

/* Every allocated chunk of the checked heap, sorted by ascending address
 * within each heap region. The heap is walked from low to high address, so
 * the array is built already sorted and can be searched by bisection.
 */
static struct alloc_node_info_s *g_node_info;
static int g_node_total;
static bool g_node_overflow;

/* Address bounds of each region of the checked heap and the slice of
 * g_node_info[] which belongs to it.
 */
static uintptr_t g_region_start[CONFIG_KMM_REGIONS];
static uintptr_t g_region_end[CONFIG_KMM_REGIONS];
static int g_region_first[CONFIG_KMM_REGIONS];
static int g_region_count[CONFIG_KMM_REGIONS];
static int g_region_total;

static int node_info_init(void)
{
	g_node_info = (struct alloc_node_info_s *)malloc(sizeof(struct alloc_node_info_s) * MAX_ALLOC_COUNT);
	if (!g_node_info) {
		return ERROR;
	}

	g_node_total = 0;
	g_region_total = 0;
	g_node_overflow = false;

	return OK;
}

static void node_info_deinit(void)
{
	free(g_node_info);
	g_node_info = NULL;
	g_node_total = 0;
	g_region_total = 0;
}

/****************************************************************************
 * Name: mark_if_referenced
 *
 * Description:
 *   Check whether 'addr' refers to any allocated chunk of the checked heap
 *   and mark that chunk as referenced.
 *
 *   Both a base pointer, which is the address returned by malloc, and an
 *   interior pointer, which is the address of a member of an allocated
 *   structure, are accepted. Accepting an interior pointer is required
 *   because a structure is very often kept alive only through a member, for
 *   example when an embedded sq_entry_t is queued into a list and the base
 *   pointer itself is dropped. Matching the base pointer alone reports such
 *   a structure as a leak although it is perfectly reachable.
 *
 *   The chunk header must stay outside of the accepted range. g_node_info[]
 *   holds the header address of every allocated chunk, so a range starting
 *   at the header would let the book keeping of the checker itself look like
 *   a reference to every chunk of the heap.
 *
 *   Returns true only when the chunk changes from unreferenced to
 *   referenced, so that a chunk is subtracted from the leak count once.
 *
 ****************************************************************************/

static bool mark_if_referenced(uintptr_t addr)
{
	int reg;
	int lo;
	int hi;
	int mid;
	volatile struct mm_allocnode_s *node;
	uintptr_t payload;
	uintptr_t tail;

	/* Reject any value which is not an address of the checked heap. Nearly
	 * every scanned value ends here, so this keeps the scan cheap.
	 */

	for (reg = 0; reg < g_region_total; reg++) {
		if (addr >= g_region_start[reg] && addr < g_region_end[reg]) {
			break;
		}
	}

	if (reg == g_region_total || g_region_count[reg] == 0) {
		return false;
	}

	/* Find the allocated chunk with the greatest header address which is not
	 * above 'addr'. It is the only chunk which can contain 'addr'.
	 */

	lo = g_region_first[reg];
	hi = lo + g_region_count[reg] - 1;

	if (addr < (uintptr_t)g_node_info[lo].node) {
		return false;
	}

	while (lo < hi) {
		mid = lo + ((hi - lo + 1) >> 1);
		if ((uintptr_t)g_node_info[mid].node <= addr) {
			lo = mid;
		} else {
			hi = mid - 1;
		}
	}

	node = g_node_info[lo].node;
	payload = (uintptr_t)node + (uintptr_t)SIZEOF_MM_ALLOCNODE;
	tail = (uintptr_t)node + (uintptr_t)node->size;

	if (addr < payload || addr >= tail) {
		/* 'addr' points into the chunk header, or into a free chunk which
		 * follows this allocated chunk. Neither is a reference.
		 */
		return false;
	}

	if (node->memory_state == MM_MEMORY_STATE_USED) {
		return false;
	}

	node->memory_state = MM_MEMORY_STATE_USED;
	return true;
}

static int get_node_cnt(struct mm_heap_s *heap)
{
	volatile struct mm_allocnode_s *node;
	mmsize_t node_size;
	int nregions = HEAP_NREGIONS(heap);
	int reg;
	int ret = 0;

	mm_takesemaphore(heap);

	/* Visit each region */

	for (reg = 0; reg < nregions; reg++) {
		node_size = SIZEOF_MM_ALLOCNODE;
		for (node = heap->mm_heapstart[reg]; node < heap->mm_heapend[reg]; node = (struct mm_allocnode_s *)((char *)node + node->size)) {
			ASSERT(node->size);
			/* Ignore the heap start checking, because there is a guard node in heap start */
			if (node == heap->mm_heapstart[reg]) {
				continue;
			}
			/* Check broken link */
			if (node_size != MM_PREV_NODE_SIZE(node)) {
				continue;
			}
			node_size = node->size;
			/* Check if the node corresponds to an allocated memory chunk */
			if ((node->preceding & MM_ALLOC_BIT) != 0) {
				ret++;
			}
		}
	}

	mm_givesemaphore(heap);

	return ret;
}

static void fill_node_info(struct mm_heap_s *heap, int *leak_cnt, int *broken_cnt)
{
	volatile struct mm_allocnode_s *node;
	mmsize_t node_size;
	int nregions = HEAP_NREGIONS(heap);
	int reg;

	mm_takesemaphore(heap);

	g_node_total = 0;
	g_region_total = nregions;

	/* Visit each region */

	for (reg = 0; reg < nregions; reg++) {
		g_region_start[reg] = (uintptr_t)heap->mm_heapstart[reg];
		g_region_end[reg] = (uintptr_t)heap->mm_heapend[reg];
		g_region_first[reg] = g_node_total;
		g_region_count[reg] = 0;

		node_size = SIZEOF_MM_ALLOCNODE;
		for (node = heap->mm_heapstart[reg]; node < heap->mm_heapend[reg]; node = (struct mm_allocnode_s *)((char *)node + node->size)) {
			ASSERT(node->size);
			/* Ignore the heap start checking, because there is a guard node in heap start */
			if (node == heap->mm_heapstart[reg]) {
				continue;
			}

			/* Check broken link */
			if (node_size != MM_PREV_NODE_SIZE(node)) {
				node->memory_state = MM_MEMORY_STATE_BROKEN;
				(*broken_cnt)++;
				continue;
			}
			node_size = node->size;
			/* The buffer of the checker itself is never a leak */
			if ((uintptr_t)node + (uintptr_t)SIZEOF_MM_ALLOCNODE == (uintptr_t)g_node_info) {
				continue;
			}
			/* Check if the node corresponds to an allocated memory chunk */
			if ((node->preceding & MM_ALLOC_BIT) == 0) {
				continue;
			}
			if (g_node_total >= MAX_ALLOC_COUNT) {
				/* The heap grew after get_node_cnt() counted it. Stop here
				 * instead of writing past the end of g_node_info[].
				 */
				g_node_overflow = true;
				break;
			}
			g_node_info[g_node_total].node = node;
			node->memory_state = MM_MEMORY_STATE_LEAK;
			g_node_total++;
			g_region_count[reg]++;
			(*leak_cnt)++;
		}

		if (g_node_overflow) {
			break;
		}
	}

	mm_givesemaphore(heap);
}

static void search_addr(void *start_addr, void *end_addr, int *leak_cnt)
{
	/* This function traverses the memory from start_addr to end_addr and
	 * checks every position for a reference to an allocated chunk.
	 */
	char *leak_chk;
	char *last;

	if ((uintptr_t)end_addr < (uintptr_t)start_addr + MEM_PTR_SIZE) {
		return;
	}

	/* The last position which can be read without leaving the region. It has
	 * to be visited too, otherwise a reference kept in the last word of the
	 * region is never seen and its chunk is reported as a leak.
	 */
	last = (char *)end_addr - MEM_PTR_SIZE;

	for (leak_chk = (char *)start_addr; leak_chk <= last; leak_chk++) {
		if (mark_if_referenced(*(volatile uintptr_t *)leak_chk)) {
			(*leak_cnt)--;
		}
	}
}

static void heap_check(struct mm_heap_s *heap, int checker_pid, int *leak_cnt)
{
	char *leak_chk;
	char *last;
	struct mm_allocnode_s *visit_node;
	void *exclude_top;
	void *exclude_bottom;
	int nregions = HEAP_NREGIONS(heap);
	int reg;

	struct tcb_s *ctcb = sched_gettcb(checker_pid);
	ASSERT(ctcb != NULL);
	exclude_top = ctcb->adj_stack_ptr;
	exclude_bottom = ctcb->adj_stack_ptr - ctcb->adj_stack_size;

	/* Visit each region */

	for (reg = 0; reg < nregions; reg++) {
		for (visit_node = heap->mm_heapstart[reg]; visit_node < heap->mm_heapend[reg]; visit_node = (struct mm_allocnode_s *)((char *)visit_node + visit_node->size)) {
			ASSERT(visit_node->size);
			if ((visit_node->preceding & MM_ALLOC_BIT) == 0) {
				continue;
			}
			if ((void *)((char *)visit_node + SIZEOF_MM_ALLOCNODE) == (void *)g_node_info) {
				continue;
			}

			/* Only the payload of a chunk can hold a reference. Skipping the
			 * chunk header also keeps its size, pid and caller fields from
			 * matching an allocated chunk by chance.
			 */
			leak_chk = (char *)visit_node + SIZEOF_MM_ALLOCNODE;

			/* Stop at the last position which is still inside this chunk, so
			 * that the scan never reads into the next chunk header or past
			 * the end of the region.
			 */
			last = (char *)visit_node + visit_node->size - MEM_PTR_SIZE;

			for (; leak_chk <= last; leak_chk++) {
				if ((void *)leak_chk >= exclude_bottom && (void *)leak_chk <= exclude_top) {
					continue;
				}
				if (mark_if_referenced(*(volatile uintptr_t *)leak_chk)) {
					(*leak_cnt)--;
				}
			}
		}
	}
}

static struct mm_heap_s * init_mem_leak_checker(int checker_pid, char *bin_name);

static void ram_check(struct mm_heap_s *heap, int checker_pid, char *bin_name, int *leak_cnt)
{
	
#ifdef CONFIG_APP_BINARY_SEPARATION
	bin_addr_info_t *info;
	struct mm_heap_s *kheap;
	int bin_idx;

	info = (bin_addr_info_t *)get_bin_addr_list();
	for (bin_idx = 0; bin_idx <= CONFIG_NUM_APPS; bin_idx++) {
		if (strncmp(BIN_NAME(bin_idx), bin_name, strlen(bin_name)) == 0) {
			break;
		}
	}
#endif
	/* Visit all the data regions
	 */
	int mem_region_idx;
	for (mem_region_idx = 0; mem_region_idx < MEM_VAR_REGION_COUNT; mem_region_idx++) {
		search_addr(variable_region_start_addr[mem_region_idx], variable_region_end_addr[mem_region_idx], leak_cnt);
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	if (strncmp(bin_name, "kernel", strlen("kernel") + 1) == 0) {
		/* do nothing */
	} else {
#ifdef CONFIG_SUPPORT_COMMON_BINARY
		search_addr((void *)info[CMN_BIN_IDX].data_addr, (void *)(info[CMN_BIN_IDX].data_addr + info[CMN_BIN_IDX].data_size), leak_cnt);
		search_addr((void *)info[CMN_BIN_IDX].bss_addr, (void *)(info[CMN_BIN_IDX].bss_addr + info[CMN_BIN_IDX].bss_size), leak_cnt);
#endif
		/* search the bss and data region of the loadable app */
		search_addr((void *)info[bin_idx].data_addr, (void *)(info[bin_idx].data_addr + info[bin_idx].data_size), leak_cnt);
		search_addr((void *)info[bin_idx].bss_addr, (void *)(info[bin_idx].bss_addr + info[bin_idx].bss_size), leak_cnt);
		/* search the kernel heap first */
		kheap = kmm_get_baseheap();
		heap_check(kheap, checker_pid, leak_cnt);
	}
#endif

	/* Visit heap region */
	heap_check(heap, checker_pid, leak_cnt);
}

static void print_mem_hex_dump(void *addr, size_t alloc_size)
{
	unsigned char *ptr = (unsigned char *)addr;
	size_t dump_size = (alloc_size < MEM_DUMP_MAX_BYTES) ? alloc_size : MEM_DUMP_MAX_BYTES;
	size_t i;

	printf("[DATA] ");
	for (i = 0; i < dump_size; i++) {
		printf("%02x ", ptr[i]);
		if ((i + 1) % 16 == 0 && (i + 1) < dump_size) {
			printf("\n       ");
		}
	}
	printf("\n");
}

static void print_info(struct mm_heap_s *heap, int leak_cnt, int broken_cnt)
{
	volatile struct mm_allocnode_s *node;
	uint32_t owner_addr;	

	if (leak_cnt > 0 || broken_cnt > 0) {
		printf("Type   |    Addr    | Size(byte) |    Owner   | PID \n");
		printf("---------------------------------------------------\n");

		mm_takesemaphore(heap);

#if CONFIG_KMM_REGIONS > 1
		int region;
#else
#define region 0
#endif

		/* Visit each region */

#if CONFIG_KMM_REGIONS > 1
		for (region = 0; region < heap->mm_nregions; region++)
#endif
		{
			for (node = heap->mm_heapstart[region]; node <  heap->mm_heapend[region]; node = (struct mm_allocnode_s *)((char *)node + node->size)) {
				ASSERT(node->size);
				if (node->memory_state == MM_MEMORY_STATE_LEAK) {
					/* alloc_call_addr can be from kernel, app or common binary.
					* based on the text addresses printed, user needs to check the
					* corresponding binaries accordingly
					*/
					owner_addr = (uint32_t)node->alloc_call_addr;
					pid_t pid = node->pid;
					if (pid < 0) {
						/* For stack allocated node, pid is negative value.
						* To use the pid, change it to original positive value.
						*/
						pid = (-1) * pid;
					}
					printf("LEAK   | %10p |  %8d  | %10p | %d\n", (void *)((char *)node + SIZEOF_MM_ALLOCNODE), node->size - SIZEOF_MM_ALLOCNODE, owner_addr, pid);
					print_mem_hex_dump((void *)((char *)node + SIZEOF_MM_ALLOCNODE), node->size - SIZEOF_MM_ALLOCNODE);
				} else if (node->memory_state == MM_MEMORY_STATE_BROKEN) {
					printf("BROKEN | %p\n", node);
				}
			}
		}

		mm_givesemaphore(heap);

		printf("*** %d LEAKS, %d BROKENS.\n", leak_cnt, broken_cnt);
	} else {
		printf("*** NO MEMORY LEAK.\n");
	}
}

int run_mem_leak_checker(int checker_pid, char *bin_name)
{
	int leak_cnt = 0;
	int node_cnt = 0;
	int broken_cnt = 0;
	struct mm_heap_s *heap = NULL;

	if (strncmp(bin_name, "kernel", strlen("kernel") + 1) == 0) {
		heap = kmm_get_baseheap();
	} 
#ifdef CONFIG_APP_BINARY_SEPARATION
	else {
		heap = mm_get_app_heap_with_name(bin_name);
	}
#endif

	if (!heap) {
		printf("Can't found heap, bin name: %s", bin_name);
		return ERROR;
	}

	node_cnt = get_node_cnt(heap);
	if (MAX_ALLOC_COUNT < node_cnt) {
		printf("Available buffer size (%d) is small.\nPlease increase CONFIG_MEM_LEAK_CHECKER_MAX_ALLOC_COUNT value more than %d.\n", MAX_ALLOC_COUNT, node_cnt);
		return ERROR;
	}

	if (g_node_info) {
		printf("mem_leak_checker is already running.\n");
		return ERROR;
	}

	if (node_info_init() != OK) {
		printf("node info memory alloc is failed.\n");
		return ERROR;
	}

	fill_node_info(heap, &leak_cnt, &broken_cnt);

	if (g_node_overflow) {
		printf("Available buffer size (%d) is small, the result is incomplete.\nPlease increase CONFIG_MEM_LEAK_CHECKER_MAX_ALLOC_COUNT value.\n", MAX_ALLOC_COUNT);
	}

	/* Visit RAM region */
	ram_check(heap, checker_pid, bin_name, &leak_cnt);

	print_info(heap, leak_cnt, broken_cnt);

	node_info_deinit();
	return OK;
}

int run_all_mem_leak_checker(int checker_pid)
{
	int ret;
	printf("\nKernel :\n");
	ret = run_mem_leak_checker(checker_pid, "kernel");

	if (ret != OK) {
		return ERROR;
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	printf("\nBelow are text addresses of loadable apps (and common binary if enabled) :\n");
	printf("The pc value of the allocation can be obtained by subtracting the text start address of the appropriate binary\n\n");
	bin_addr_info_t *bin_addr_info = (bin_addr_info_t *)get_bin_addr_list();
	int bin_idx;
	for (bin_idx = 0; bin_idx <= CONFIG_NUM_APPS; bin_idx++) {
		if (bin_addr_info[bin_idx].text_addr != 0) {
			printf("[%s] Text Addr : %p, Text Size : %u\n", BIN_NAME(bin_idx), bin_addr_info[bin_idx].text_addr, bin_addr_info[bin_idx].text_size);
		}
	}
	printf("\n");
	/* bin_idx value zero is always reserved for common binary, so
	 * skip checking common binary and start checking from index one
	 */
	for (bin_idx = 1; bin_idx <= CONFIG_NUM_APPS; bin_idx++) {
		if (bin_addr_info[bin_idx].text_addr != 0) {
			printf("%s :\n", BIN_NAME(bin_idx));
			ret = run_mem_leak_checker(checker_pid, BIN_NAME(bin_idx));
			if (ret != OK) {
				return ERROR;
			}
		}
	}
#endif
	return OK;
}
