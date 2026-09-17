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
 * mem_leak_checker.c
 *
 * Description:
 *   Improved memory leak checker using mark-and-sweep from true roots.
 *
 *   Unlike the original mem_leak_checker.c which treats ALL heap blocks
 *   as roots (causing reference cycle false negatives), this implementation
 *   only scans BSS, DATA, and thread stacks as roots. Heap blocks are
 *   scanned transitively only from blocks already marked as reachable.
 *
 *   This breaks reference cycles: if A->B->A but neither is reachable
 *   from a true root, both are correctly identified as leaks.
 *
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdlib.h>
#include <debug.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <queue.h>

#include <sys/types.h>
#include <tinyara/mm/mm.h>
#include <tinyara/mm/heap_regioninfo.h>
#include <tinyara/binfmt/elf.h>
#include <tinyara/sched.h>
#include <tinyara/wqueue.h>
#include <arch/chip/memory_region.h>

#include "binary_manager/binary_manager_internal.h"



/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define CMN_BIN_IDX 0

#define MEM_ACCESS_UNIT    0x04
#define MAX_BLOCKS         CONFIG_MEM_LEAK_CHECKER_MAX_BLOCKS
#define HASH_SIZE          CONFIG_MEM_LEAK_CHECKER_HASH_SIZE
#define WORKLIST_SIZE      CONFIG_MEM_LEAK_CHECKER_WORKLIST_SIZE
#define MEM_DUMP_MAX_BYTES 16

#ifdef CONFIG_MEM_LEAK_CHECKER_DEBUG
#define LEAK_CHECKER_DEBUG(fmt, ...)  printf("[LEAKDBG] " fmt, ##__VA_ARGS__)
#else
#define LEAK_CHECKER_DEBUG(fmt, ...)  do {} while (0)
#endif


#define MM_PREV_NODE_SIZE(x)  ((x)->preceding & ~MM_ALLOC_BIT)

/* Reachability states */
#define BLOCK_UNREACHABLE  0
#define BLOCK_REACHABLE     1

/****************************************************************************
 * Data Structures
 ****************************************************************************/

/* Information about a single allocated block */
struct leak_block_info_s {
	volatile struct mm_allocnode_s *node;  /* Pointer to the heap node */
	void *addr;                             /* User-visible address (node + SIZEOF_MM_ALLOCNODE) */
	size_t size;                            /* Usable size */
	void *owner_addr;                        /* Allocation site */
	pid_t pid;                               /* Allocating task's PID */
	uint8_t reachability;                    /* BLOCK_REACHABLE or BLOCK_UNREACHABLE */
	uint8_t in_worklist;                     /* Already added to worklist */
};

/* Hash table entry for fast block lookup by address */
struct leak_hash_entry_s {
	int block_index;                        /* Index into g_blocks array */
	struct leak_hash_entry_s *next;           /* Chain for collision resolution */
};

/* Worklist entry for transitive marking */
struct leak_worklist_entry_s {
	int block_index;                        /* Index of block to scan */
};

/****************************************************************************
 * Globals
 ****************************************************************************/

static struct leak_block_info_s *g_leak_blocks;
static int g_leak_block_count;
static struct leak_hash_entry_s *g_leak_hash_table;
static struct leak_hash_entry_s *g_leak_hash_entries;
static struct leak_worklist_entry_s *g_leak_worklist;
static int g_leak_worklist_head;
static int g_leak_worklist_tail;

/* Node addresses of leak checker's own heap allocations — these must NOT be
 * collected or scanned transitively, since they contain pointers to
 * all collected blocks (which would cause false "reachable" marking).
 */
static void *g_leak_own_nodes[8];
static int g_leak_own_node_count;

/* Check if a node address belongs to leak checker's own allocations */
static bool leak_is_own_node(void *node_addr)
{
	int i;
	for (i = 0; i < g_leak_own_node_count; i++) {
		if (g_leak_own_nodes[i] == node_addr) {
			return true;
		}
	}
	return false;
}



/****************************************************************************
 * Hash Table Operations
 ****************************************************************************/

static int leak_hash_init(void)
{
	int i;

	g_leak_hash_table = (struct leak_hash_entry_s *)malloc(sizeof(struct leak_hash_entry_s) * HASH_SIZE);
	if (!g_leak_hash_table) {
		return ERROR;
	}

	g_leak_hash_entries = (struct leak_hash_entry_s *)malloc(sizeof(struct leak_hash_entry_s) * MAX_BLOCKS);
	if (!g_leak_hash_entries) {
		free(g_leak_hash_table);
		g_leak_hash_table = NULL;
		return ERROR;
	}

	for (i = 0; i < HASH_SIZE; i++) {
		g_leak_hash_table[i].block_index = -1;
		g_leak_hash_table[i].next = NULL;
	}

	return OK;
}

static void leak_hash_deinit(void)
{
	free(g_leak_hash_table);
	free(g_leak_hash_entries);
	g_leak_hash_table = NULL;
	g_leak_hash_entries = NULL;
}

static void leak_hash_insert(int index)
{
	unsigned long key;
	struct leak_hash_entry_s *entry;

	/* Hash on the node address, ignoring the lowest 4 bits.
	 * This ensures that addresses within 16 bytes of each other
	 * (e.g., a node and an interior pointer 8 bytes away due to
	 * C++ array cookies) hash to the same bucket.
	 */
	key = ((unsigned long)g_leak_blocks[index].node >> 4) % HASH_SIZE;

	entry = &g_leak_hash_entries[index];
	entry->block_index = index;
	entry->next = g_leak_hash_table[key].next;
	g_leak_hash_table[key].next = entry;
}

/* Look up a node address in the hash table.
 * Returns block index if found, -1 if not found.
 * If mark_reachable is true, sets the block's reachability to REACHABLE.
 *
 * Uses a hybrid matching strategy:
 * 1. Fast path: exact node address match via hash table (most real pointers)
 * 2. Slow path: range match for interior pointers within the same bucket
 *    (handles C++ array cookies where new T[n] returns a pointer past
 *    the cookie, and other interior pointer cases)
 *
 * The hash key ignores the lowest 4 bits (>> 4), so addresses within
 * 16 bytes of each other land in the same bucket. This allows interior
 * pointers (e.g., C++ array cookies offset by 8 bytes) to be found in
 * the same bucket as the actual node — no O(n) linear scan needed.
 */
static int leak_hash_lookup(void *node_addr, bool mark_reachable)
{
	unsigned long key;
	struct leak_hash_entry_s *entry;
	unsigned long value = (unsigned long)node_addr;

	/* Hash key: ignore lowest 4 bits so nearby addresses share a bucket */
	key = (value >> 4) % HASH_SIZE;

	entry = g_leak_hash_table[key].next;
	while (entry) {
		unsigned long node_start = (unsigned long)g_leak_blocks[entry->block_index].node;
		unsigned long node_end = node_start + (unsigned long)g_leak_blocks[entry->block_index].size;

		/* Fast path: exact node address match */
		if (value == node_start) {
			if (mark_reachable) {
				g_leak_blocks[entry->block_index].reachability = BLOCK_REACHABLE;
			}
			return entry->block_index;
		}

		/* Slow path: interior pointer within allocation range.
		 * Only check if value is aligned (real pointers are aligned).
		 * This handles C++ array cookies where new T[n] returns
		 * a pointer past the cookie (node + 16 + cookie_size).
		 */
		if ((value % sizeof(void *) == 0) &&
			(value > node_start && value < node_end)) {
			if (mark_reachable) {
				g_leak_blocks[entry->block_index].reachability = BLOCK_REACHABLE;
			}
			return entry->block_index;
		}
		entry = entry->next;
	}

	return -1;
}

/****************************************************************************
 * Worklist Operations
 ****************************************************************************/

static int leak_worklist_init(void)
{
	g_leak_worklist = (struct leak_worklist_entry_s *)malloc(sizeof(struct leak_worklist_entry_s) * WORKLIST_SIZE);
	if (!g_leak_worklist) {
		return ERROR;
	}
	g_leak_worklist_head = 0;
	g_leak_worklist_tail = 0;
	return OK;
}

static void leak_worklist_deinit(void)
{
	free(g_leak_worklist);
	g_leak_worklist = NULL;
}

static bool leak_worklist_is_empty(void)
{
	return g_leak_worklist_head == g_leak_worklist_tail;
}

static bool leak_worklist_is_full(void)
{
	return ((g_leak_worklist_tail + 1) % WORKLIST_SIZE) == g_leak_worklist_head;
}

static int leak_worklist_push(int block_index)
{
	if (leak_worklist_is_full()) {
		return ERROR;
	}
	g_leak_worklist[g_leak_worklist_tail].block_index = block_index;
	g_leak_worklist_tail = (g_leak_worklist_tail + 1) % WORKLIST_SIZE;
	return OK;
}

static int leak_worklist_pop(void)
{
	int block_index;
	if (leak_worklist_is_empty()) {
		return -1;
	}
	block_index = g_leak_worklist[g_leak_worklist_head].block_index;
	g_leak_worklist_head = (g_leak_worklist_head + 1) % WORKLIST_SIZE;
	return block_index;
}

/* Add block to worklist if not already added and not already reachable */
static void leak_mark_block(int block_index)
{
	if (block_index < 0 || block_index >= g_leak_block_count) {
		return;
	}
	if (g_leak_blocks[block_index].reachability == BLOCK_REACHABLE &&
		g_leak_blocks[block_index].in_worklist) {
		return;
	}
	g_leak_blocks[block_index].reachability = BLOCK_REACHABLE;
	if (!g_leak_blocks[block_index].in_worklist) {
		g_leak_blocks[block_index].in_worklist = 1;
		leak_worklist_push(block_index);
	}
}

/****************************************************************************
 * Phase 1: Collect all allocated blocks from heap
 ****************************************************************************/

static int leak_get_node_cnt(struct mm_heap_s *heap)
{
	volatile struct mm_allocnode_s *node;
	mmsize_t node_size;
	int ret = 0;

#if CONFIG_KMM_REGIONS > 1
	int region;
#else
#define region 0
#endif

#if CONFIG_KMM_REGIONS > 1
	for (region = 0; region < heap->mm_nregions; region++)
#endif
	{
		node_size = SIZEOF_MM_ALLOCNODE;
		for (node = heap->mm_heapstart[region]; node < heap->mm_heapend[region]; node = (struct mm_allocnode_s *)((char *)node + node->size)) {
			ASSERT(node->size);
			if (node == heap->mm_heapstart[region]) {
				continue;
			}
			if (node_size != MM_PREV_NODE_SIZE(node)) {
				continue;
			}
			node_size = node->size;
			if ((node->preceding & MM_ALLOC_BIT) != 0) {
				ret++;
			}
		}
	}

	return ret;
}

static void leak_collect_blocks(struct mm_heap_s *heap, int *broken_cnt)
{
	volatile struct mm_allocnode_s *node;
	mmsize_t node_size;
	int idx = 0;

#if CONFIG_KMM_REGIONS > 1
	int region;
#else
#define region 0
#endif

	mm_takesemaphore(heap);

#if CONFIG_KMM_REGIONS > 1
	for (region = 0; region < heap->mm_nregions; region++)
#endif
	{
		node_size = SIZEOF_MM_ALLOCNODE;
		for (node = heap->mm_heapstart[region]; node < heap->mm_heapend[region]; node = (struct mm_allocnode_s *)((char *)node + node->size)) {
			ASSERT(node->size);
			if (node == heap->mm_heapstart[region]) {
				continue;
			}
			if (node_size != MM_PREV_NODE_SIZE(node)) {
				(*broken_cnt)++;
				continue;
			}
			node_size = node->size;

			/* Skip our own allocations — use node address comparison */
			if (leak_is_own_node((void *)node)) {
				continue;
			}


			if ((node->preceding & MM_ALLOC_BIT) != 0) {
				if (idx >= MAX_BLOCKS) {
					LEAK_CHECKER_DEBUG("idx value exceeded, skipping = %p\n", node);
					break;
				}
				g_leak_blocks[idx].node = node;
				g_leak_blocks[idx].addr = (void *)((char *)node + SIZEOF_MM_ALLOCNODE);
				g_leak_blocks[idx].size = node->size - SIZEOF_MM_ALLOCNODE;
				g_leak_blocks[idx].owner_addr = (void *)node->alloc_call_addr;
				g_leak_blocks[idx].pid = node->pid;
				g_leak_blocks[idx].reachability = BLOCK_UNREACHABLE;
				g_leak_blocks[idx].in_worklist = 0;
				leak_hash_insert(idx);
				idx++;
			}
		}
	}

	g_leak_block_count = idx;
	mm_givesemaphore(heap);
}

/****************************************************************************
 * Phase 2: Mark roots and transitive marking
 ****************************************************************************/

/* Scan a memory region at 4-byte alignment for pointers to allocated blocks.
 * Any block found is marked as REACHABLE and added to the worklist.
 */
static const char *g_leak_scan_label = "UNKNOWN";
static int g_leak_scan_pid = -1;


static void leak_scan_region(void *start_addr, void *end_addr)
{
	unsigned long *ptr;
	unsigned long *end;
	unsigned long node_addr;

	if (!start_addr || !end_addr || start_addr >= end_addr) {
		return;
	}

	LEAK_CHECKER_DEBUG("Scanning %-12s: %p - %p (%d bytes)\n",
		g_leak_scan_label, start_addr, end_addr, (int)((char *)end_addr - (char *)start_addr));

	/* Align start to 4-byte boundary */
	ptr = (unsigned long *)(((unsigned long)start_addr + 3) & ~3UL);
	end = (unsigned long *)end_addr;

	while (ptr < end) {
		/* The value in memory might be a pointer to a user block.
		 * The user address is node + SIZEOF_MM_ALLOCNODE.
		 * So the node address would be value - SIZEOF_MM_ALLOCNODE.
		 */
		node_addr = *ptr - (unsigned long)SIZEOF_MM_ALLOCNODE;
		if (node_addr > 0) {
			int idx = leak_hash_lookup((void *)node_addr, true);
			if (idx >= 0) {
				LEAK_CHECKER_DEBUG("  Found ptr at %p (PID %d): value=0x%lx -> block %d (addr=%p, pid=%d) MARKED\n",
					ptr, g_leak_scan_pid, (unsigned long)*ptr, idx, g_leak_blocks[idx].addr, g_leak_blocks[idx].pid);
				leak_mark_block(idx);
			}

		}
		ptr++;
	}
}


/* Scan all thread stacks for root pointers */
static void leak_scan_thread_stacks(int checker_pid)
{
	struct tcb_s *tcb;
	int i;

	/* Iterate over all TCBs */

	for (i = 0; i < CONFIG_MAX_TASKS; i++) {
		tcb = (struct tcb_s *)sched_gettcb(i);
		if (!tcb) {
			continue;
		}

		/* Skip the checker's own stack */
		if (i == checker_pid) {
			continue;
		}

		/* Scan the stack from top to bottom */
		if (tcb->adj_stack_ptr && tcb->adj_stack_size > 0) {
			void *stack_top = tcb->adj_stack_ptr;
			void *stack_bottom = tcb->adj_stack_ptr - tcb->adj_stack_size;
			LEAK_CHECKER_DEBUG("Scanning stack of PID %d (name: %s)\n", i, tcb->name);
			g_leak_scan_label = "STACK";
			g_leak_scan_pid = i;
			leak_scan_region(stack_bottom, stack_top);
			g_leak_scan_label = "UNKNOWN";
			g_leak_scan_pid = -1;

		}

	}
}

/* Scan kernel heap for pointers to app heap blocks.
 * This handles IPC cases where kernel structures (MQ, etc.) hold
 * pointers to app-allocated buffers.
 */
static void leak_scan_kernel_heap_for_app_blocks(struct mm_heap_s *app_heap)
{
	struct mm_heap_s *kheap;
	volatile struct mm_allocnode_s *node;
	mmsize_t node_size;

#if CONFIG_KMM_REGIONS > 1
	int region;
#else
#define region 0
#endif

	kheap = kmm_get_baseheap();
	if (!kheap) {
		return;
	}

	mm_takesemaphore(kheap);

#if CONFIG_KMM_REGIONS > 1
	for (region = 0; region < kheap->mm_nregions; region++)
#endif
	{
		node_size = SIZEOF_MM_ALLOCNODE;
		for (node = kheap->mm_heapstart[region]; node < kheap->mm_heapend[region]; node = (struct mm_allocnode_s *)((char *)node + node->size)) {
			if (!node->size) {
				break;
			}
			if (node == kheap->mm_heapstart[region]) {
				continue;
			}
			if (node_size != MM_PREV_NODE_SIZE(node)) {
				continue;
			}
			node_size = node->size;

			/* Only scan ALLOCATED kernel heap blocks (not freed ones!) */
			if ((node->preceding & MM_ALLOC_BIT) != 0) {
				void *start = (void *)((char *)node + SIZEOF_MM_ALLOCNODE);
				void *end = (void *)((char *)node + node->size);
				leak_scan_region(start, end);
			}
		}
	}

	mm_givesemaphore(kheap);
}

/* Scan all app heaps for pointers to kernel heap blocks.
 * This handles IPC cases where app-allocated structures hold
 * pointers to kernel-allocated buffers (e.g., file descriptors,
 * socket handles, etc.).
 *
 * Called when checking the KERNEL heap to prevent false positives
 * for kernel blocks referenced from app heaps.
 */
static void leak_scan_app_heaps_for_kernel_blocks(void)
{
#ifdef CONFIG_APP_BINARY_SEPARATION
	bin_addr_info_t *bin_addr_info = (bin_addr_info_t *)get_bin_addr_list();
	int bin_idx;

	for (bin_idx = 1; bin_idx <= CONFIG_NUM_APPS; bin_idx++) {
		if (bin_addr_info[bin_idx].text_addr != 0) {
			struct mm_heap_s *app_heap = mm_get_app_heap_with_name(BIN_NAME(bin_idx));
			if (!app_heap) {
				continue;
			}

			volatile struct mm_allocnode_s *node;
			mmsize_t node_size;

#if CONFIG_KMM_REGIONS > 1
			int region;
#else
#define region 0
#endif

			mm_takesemaphore(app_heap);

			g_leak_scan_label = "APP_HEAP";

#if CONFIG_KMM_REGIONS > 1
			for (region = 0; region < app_heap->mm_nregions; region++)
#endif
			{
				node_size = SIZEOF_MM_ALLOCNODE;
				for (node = app_heap->mm_heapstart[region]; node < app_heap->mm_heapend[region]; node = (struct mm_allocnode_s *)((char *)node + node->size)) {
					if (!node->size) {
						break;
					}
					if (node == app_heap->mm_heapstart[region]) {
						continue;
					}
					if (node_size != MM_PREV_NODE_SIZE(node)) {
						continue;
					}
					node_size = node->size;

					/* Only scan ALLOCATED app heap blocks */
					if ((node->preceding & MM_ALLOC_BIT) != 0) {
						void *start = (void *)((char *)node + SIZEOF_MM_ALLOCNODE);
						void *end = (void *)((char *)node + node->size);
						leak_scan_region(start, end);
					}
				}
			}

			mm_givesemaphore(app_heap);
			g_leak_scan_label = "UNKNOWN";
		}
	}
#endif /* CONFIG_APP_BINARY_SEPARATION */
}

/* Transitive marking: scan contents of each reachable block for more pointers */

static void leak_transitive_mark(void)
{
	int block_index;

	while (!leak_worklist_is_empty()) {
		block_index = leak_worklist_pop();
		if (block_index < 0 || block_index >= g_leak_block_count) {
			continue;
		}

		/* Scan the block's contents for pointers to other blocks */
		void *start = g_leak_blocks[block_index].addr;
		void *end = (void *)((char *)start + g_leak_blocks[block_index].size);
		g_leak_scan_label = "HEAP_TRANS";
		g_leak_scan_pid = g_leak_blocks[block_index].pid;
		leak_scan_region(start, end);
		g_leak_scan_label = "UNKNOWN";
		g_leak_scan_pid = -1;

	}
}

/****************************************************************************
 * Reporting
 ****************************************************************************/

static void leak_print_hex_dump(void *addr, size_t alloc_size)
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

static void leak_print_report(struct mm_heap_s *heap, int broken_cnt, int filter_pid)
{
	int leak_cnt = 0;
	int reachable_cnt = 0;
	int i;
	int show_details = (filter_pid != 0) ? 1 : 0;


	/* Per-PID leak tracking for summary.
	 * Use a fixed reasonable size — number of distinct PIDs with leaks
	 * is typically small (< 50).
	 * Declared static to avoid stack overflow (1024-byte task stack). */
#define MAX_PID_SUMMARY 64
	static struct {
		pid_t pid;
		int count;
		size_t total_bytes;
	} pid_summary[MAX_PID_SUMMARY];
	int pid_summary_count = 0;
	int total_leak_bytes = 0;

	/* Reset static pid_summary from previous calls */
	memset(pid_summary, 0, sizeof(pid_summary));

	/* Count results and track per-PID statistics */
	for (i = 0; i < g_leak_block_count; i++) {
		if (g_leak_blocks[i].reachability == BLOCK_UNREACHABLE) {
			leak_cnt++;
			pid_t pid = g_leak_blocks[i].pid;
			if (pid < 0) {
				pid = (-1) * pid;
			}
			total_leak_bytes += g_leak_blocks[i].size;
			int found = 0;
			int j;
			for (j = 0; j < pid_summary_count; j++) {
				if (pid_summary[j].pid == pid) {
					pid_summary[j].count++;
					pid_summary[j].total_bytes += g_leak_blocks[i].size;
					found = 1;
					break;
				}
			}
			if (!found && pid_summary_count < MAX_PID_SUMMARY) {
				pid_summary[pid_summary_count].pid = pid;
				pid_summary[pid_summary_count].count = 1;
				pid_summary[pid_summary_count].total_bytes = g_leak_blocks[i].size;
				pid_summary_count++;
			}
		} else {
			reachable_cnt++;
		}
	}

	if (leak_cnt > 0 || broken_cnt > 0) {
		/* Print detailed leak information only if filter_pid is set */
		/* filter_pid == 0: summary only (no details) */
		/* filter_pid == -1: show details for all PIDs */
		/* filter_pid > 0: show details for specific PID only */
		if (show_details) {
			printf("Type   |    Addr    | Size(byte) |    Owner   | PID  | Status\n");
			printf("----------------------------------------------------------------\n");
			fflush(stdout);

			mm_takesemaphore(heap);

			int detail_count = 0;
			for (i = 0; i < g_leak_block_count; i++) {
				if (g_leak_blocks[i].reachability == BLOCK_UNREACHABLE) {
					pid_t pid = g_leak_blocks[i].pid;
					if (pid < 0) {
						pid = (-1) * pid;
					}
					/* Filter: -1 means all, >0 means specific PID */
					if (filter_pid == -1 || pid == filter_pid) {
						printf("LEAK   | %10p |  %8d  | %10p | %4d | UNREACHABLE\n",
							g_leak_blocks[i].addr,
							g_leak_blocks[i].size,
							g_leak_blocks[i].owner_addr,
							pid);
						leak_print_hex_dump(g_leak_blocks[i].addr, g_leak_blocks[i].size);
						detail_count++;
						/* Flush every 10 entries to prevent UART buffer overflow */
						if (detail_count % 10 == 0) {
							fflush(stdout);
							usleep(1000);
						}
					}
				}
			}

			mm_givesemaphore(heap);
			fflush(stdout);
		}



		printf("*** %d LEAKS (UNREACHABLE), %d reachable, %d broken.\n",
			leak_cnt, reachable_cnt, broken_cnt);

		/* Print summary by PID with task names */
		if (leak_cnt > 0) {
			printf("\n=== LEAK SUMMARY BY TASK ===\n");
			printf("PID  | Task Name      | Leak Count | Total Bytes\n");
			printf("------------------------------------------------\n");
			for (i = 0; i < pid_summary_count; i++) {
				struct tcb_s *tcb = sched_gettcb(pid_summary[i].pid);
				const char *name = (tcb && tcb->name[0]) ? tcb->name : "<exited>";
				printf("%4d | %14s | %10d | %11u\n",
					pid_summary[i].pid,
					name,
					pid_summary[i].count,
					(unsigned int)pid_summary[i].total_bytes);
			}
			printf("------------------------------------------------\n");
			printf("TOTAL: %d LEAKS, %u bytes\n", leak_cnt, (unsigned int)total_leak_bytes);
		}
		fflush(stdout);
	} else {
		printf("*** NO MEMORY LEAK. (%d blocks reachable, %d broken)\n",
			reachable_cnt, broken_cnt);
		fflush(stdout);
	}
}



/****************************************************************************
 * Main entry point
 ****************************************************************************/

static int run_mem_leak_checker_leak_single(int checker_pid, char *bin_name, int filter_pid)
{

	int broken_cnt = 0;
	int node_cnt;
	struct mm_heap_s *heap = NULL;
	int mem_region_idx;

	/* Get the heap to check */
	if (strncmp(bin_name, "kernel", strlen("kernel") + 1) == 0) {
		heap = kmm_get_baseheap();
	}
#ifdef CONFIG_APP_BINARY_SEPARATION
	else {
		heap = mm_get_app_heap_with_name(bin_name);
	}
#endif

	if (!heap) {
		printf("Can't find heap, bin name: %s\n", bin_name);
		return ERROR;
	}

	/* Check if we have enough capacity */
	node_cnt = leak_get_node_cnt(heap);
	if (MAX_BLOCKS < node_cnt) {
		printf("Available buffer size (%d) is too small.\n"
			"Please increase CONFIG_MEM_LEAK_CHECKER_MAX_BLOCKS to more than %d.\n",
			MAX_BLOCKS, node_cnt);
		return ERROR;
	}

	/* Allocate our data structures */
	g_leak_blocks = (struct leak_block_info_s *)malloc(sizeof(struct leak_block_info_s) * MAX_BLOCKS);
	if (!g_leak_blocks) {
		printf("Failed to allocate block info array.\n");
		return ERROR;
	}

	if (leak_hash_init() != OK) {
		printf("Failed to allocate hash table.\n");
		free(g_leak_blocks);
		g_leak_blocks = NULL;
		return ERROR;
	}

	if (leak_worklist_init() != OK) {
		printf("Failed to allocate worklist.\n");
		leak_hash_deinit();
		free(g_leak_blocks);
		g_leak_blocks = NULL;
		return ERROR;
	}

	g_leak_block_count = 0;
	g_leak_own_node_count = 0;

	/* Record node addresses of our own allocations so we can skip them.
	 * The user data pointer (e.g. g_leak_blocks) points to node + SIZEOF_MM_ALLOCNODE,
	 * so the node address is user_ptr - SIZEOF_MM_ALLOCNODE.
	 */
	g_leak_own_nodes[g_leak_own_node_count++] = (void *)((char *)g_leak_blocks - SIZEOF_MM_ALLOCNODE);
	g_leak_own_nodes[g_leak_own_node_count++] = (void *)((char *)g_leak_hash_table - SIZEOF_MM_ALLOCNODE);
	g_leak_own_nodes[g_leak_own_node_count++] = (void *)((char *)g_leak_hash_entries - SIZEOF_MM_ALLOCNODE);
	g_leak_own_nodes[g_leak_own_node_count++] = (void *)((char *)g_leak_worklist - SIZEOF_MM_ALLOCNODE);

	LEAK_CHECKER_DEBUG("Skipping %d own allocations:\n", g_leak_own_node_count);
	for (int dbg_i = 0; dbg_i < g_leak_own_node_count; dbg_i++) {
		LEAK_CHECKER_DEBUG("  own node %d: %p (user data: %p)\n", dbg_i,
			g_leak_own_nodes[dbg_i],
			(void *)((char *)g_leak_own_nodes[dbg_i] + SIZEOF_MM_ALLOCNODE));
	}


	/* Phase 1: Collect all allocated blocks */
	leak_collect_blocks(heap, &broken_cnt);


	LEAK_CHECKER_DEBUG("Collected %d blocks from heap (broken: %d)\n", g_leak_block_count, broken_cnt);
#ifdef CONFIG_MEM_LEAK_CHECKER_DEBUG
	for (int dbg_i = 0; dbg_i < g_leak_block_count; dbg_i++) {
		LEAK_CHECKER_DEBUG("  Block %d: addr=%p, size=%d, pid=%d\n",
			dbg_i, g_leak_blocks[dbg_i].addr, g_leak_blocks[dbg_i].size, g_leak_blocks[dbg_i].pid);
	}
#endif

	/* Phase 2: Mark roots */
	/* 2a: Scan BSS/DATA regions */
	for (mem_region_idx = 0; mem_region_idx < MEM_VAR_REGION_COUNT; mem_region_idx++) {
		g_leak_scan_label = (mem_region_idx == 0) ? "BSS" : "DATA";
		leak_scan_region(variable_region_start_addr[mem_region_idx],
			variable_region_end_addr[mem_region_idx]);
	}
	g_leak_scan_label = "UNKNOWN";


#ifdef CONFIG_APP_BINARY_SEPARATION
	/* 2b: Scan app binary BSS/DATA */
	if (strncmp(bin_name, "kernel", strlen("kernel") + 1) != 0) {
		bin_addr_info_t *info;
		int bin_idx;

		info = (bin_addr_info_t *)get_bin_addr_list();
		for (bin_idx = 0; bin_idx <= CONFIG_NUM_APPS; bin_idx++) {
			if (strncmp(BIN_NAME(bin_idx), bin_name, strlen(bin_name)) == 0) {
				break;
			}
		}

#ifdef CONFIG_SUPPORT_COMMON_BINARY
		leak_scan_region((void *)info[CMN_BIN_IDX].data_addr,
			(void *)(info[CMN_BIN_IDX].data_addr + info[CMN_BIN_IDX].data_size));
		leak_scan_region((void *)info[CMN_BIN_IDX].bss_addr,
			(void *)(info[CMN_BIN_IDX].bss_addr + info[CMN_BIN_IDX].bss_size));
#endif
		leak_scan_region((void *)info[bin_idx].data_addr,
			(void *)(info[bin_idx].data_addr + info[bin_idx].data_size));
		leak_scan_region((void *)info[bin_idx].bss_addr,
			(void *)(info[bin_idx].bss_addr + info[bin_idx].bss_size));

		/* 2c: Scan kernel heap for pointers to app blocks (IPC case) */
		leak_scan_kernel_heap_for_app_blocks(heap);
	} else {
		/* 2c-kernel: Scan all app heaps for pointers to kernel blocks.
		 * This fixes false positives where kernel blocks are referenced
		 * from app heap structures (e.g., IPC handles, fd tables). */
		leak_scan_app_heaps_for_kernel_blocks();
	}
#endif

	/* 2d: Scan all thread stacks (except checker's own) */

	leak_scan_thread_stacks(checker_pid);

	/* Phase 2: Transitive marking from reachable blocks */
	leak_transitive_mark();

	/* Print report */
	leak_print_report(heap, broken_cnt, filter_pid);


	/* Cleanup */
	leak_worklist_deinit();
	leak_hash_deinit();
	free(g_leak_blocks);
	g_leak_blocks = NULL;
	g_leak_block_count = 0;

	return OK;
}

int run_all_mem_leak_checker(int checker_pid, int filter_pid, const char *heap_name)
{
	int ret;

	/* If heap_name is specified, check only that heap.
	 * If heap_name is NULL, check all heaps (default behavior).
	 */
	if (heap_name != NULL) {
		/* Single heap mode */
		printf("\n%s :\n", heap_name);

		/* Print text address info for app heaps */
#ifdef CONFIG_APP_BINARY_SEPARATION
		if (strncmp(heap_name, "kernel", strlen("kernel") + 1) != 0) {
			bin_addr_info_t *bin_addr_info = (bin_addr_info_t *)get_bin_addr_list();
			int bin_idx;
			for (bin_idx = 0; bin_idx <= CONFIG_NUM_APPS; bin_idx++) {
				if (bin_addr_info[bin_idx].text_addr != 0 &&
					strncmp(BIN_NAME(bin_idx), heap_name, strlen(heap_name)) == 0) {
					printf("[%s] Text Addr : %p, Text Size : %u\n",
						BIN_NAME(bin_idx),
						bin_addr_info[bin_idx].text_addr,
						bin_addr_info[bin_idx].text_size);
					break;
				}
			}
			printf("\n");
		}
#endif

		ret = run_mem_leak_checker_leak_single(checker_pid, (char *)heap_name, filter_pid);
		fflush(stdout);
		return ret;
	}

	/* Default: check all heaps */
	printf("\nKernel :\n");
	ret = run_mem_leak_checker_leak_single(checker_pid, "kernel", filter_pid);
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
			printf("[%s] Text Addr : %p, Text Size : %u\n",
				BIN_NAME(bin_idx),
				bin_addr_info[bin_idx].text_addr,
				bin_addr_info[bin_idx].text_size);
		}
	}
	printf("\n");

	for (bin_idx = 1; bin_idx <= CONFIG_NUM_APPS; bin_idx++) {
		if (bin_addr_info[bin_idx].text_addr != 0) {
			printf("%s :\n", BIN_NAME(bin_idx));
			ret = run_mem_leak_checker_leak_single(checker_pid, BIN_NAME(bin_idx), filter_pid);

			if (ret != OK) {
				return ERROR;
			}
		}
	}
#endif

	fflush(stdout);
	return OK;
}


