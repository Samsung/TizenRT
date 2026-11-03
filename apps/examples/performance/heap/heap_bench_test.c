/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/// @file heap_bench_test.c

/// @brief Heap bench test: allocate blocks, free a configurable percentage,
///        and optionally invoke the memory leak checker to verify detection.
///        Also supports stress test modes (fill, perf, frag) for performance
///        measurement. All modes work independently of CONFIG_MEM_LEAK_CHECKER;
///        the leak checker is only invoked when explicitly requested in basic
///        mode (run_checker=1) or fill mode, and only if CONFIG_MEM_LEAK_CHECKER
///        is enabled. Perf and frag modes never call the leak checker.

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>
#include <sys/time.h>

#ifdef CONFIG_MEM_LEAK_CHECKER
#include <sys/prctl.h>
#include <sys/types.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define DEFAULT_NUM_ALLOC       50
#define DEFAULT_FREE_PCT        50
#define MAX_BLOCKS              256
#define HEAP_BENCH_STACK_SIZE   16384
#define NUM_SIZE_CLASSES        10

/* Stress test parameters.
 * CONFIG_HEAP_STRESS_MAX_BLOCKS is independent of the memory leak checker.
 * It controls the maximum number of blocks the stress test will track.
 */
#ifndef CONFIG_EXAMPLES_HEAP_STRESS_MAX_BLOCKS
#define CONFIG_EXAMPLES_HEAP_STRESS_MAX_BLOCKS 3000
#endif
#define MAX_STRESS_BLOCKS       CONFIG_EXAMPLES_HEAP_STRESS_MAX_BLOCKS
#define DEFAULT_FILL_PCT        75
#define MAX_FILL_PCT            75

/* Performance mode parameters */
#define PERF_FILL_PCT              75      /* Total heap fill % across all size classes */
#define DEFAULT_PERF_ITERATIONS    5
#define MAX_PERF_ITERATIONS        20

/* Fragmentation mode parameters */
#define FRAG_FILL_PCT              85
#define DEFAULT_FRAG_FREE_PCT      50

/* Fixed size classes (powers of 2) — no ranges, exact sizes.
 * This makes allocation deterministic and faster (no rand() per block
 * for size calculation; distribution is pre-computed once via
 * build_class_sequence()).
 */
#define SIZE_CLASS_0            4
#define SIZE_CLASS_1            8
#define SIZE_CLASS_2            16
#define SIZE_CLASS_3            32
#define SIZE_CLASS_4            64
#define SIZE_CLASS_5            128
#define SIZE_CLASS_6            256
#define SIZE_CLASS_7            512
#define SIZE_CLASS_8            1024
#define SIZE_CLASS_9            2048

/* Distribution profiles */
#define NUM_DIST_PROFILES       4
#define DIST_CURRENT            0
#define DIST_UNIFORM            1
#define DIST_SMALL              2
#define DIST_LARGE              3
#define DEFAULT_DIST            DIST_CURRENT


/****************************************************************************
 * Private Data Structures
 ****************************************************************************/

struct alloc_record {
	void *ptr;       /* Pointer to allocated block (NULL = freed/not allocated) */
	uint16_t size;   /* Block size in bytes (max 2048) */
};

/* Timing information for stress tests */
struct stress_timing {
	struct timeval alloc_start;
	struct timeval alloc_end;
	struct timeval leak_start;
	struct timeval leak_end;
	long alloc_time_ms;
	long leak_check_time_ms;
};

/* Statistics for stress tests */
struct stress_stats {
	int total_allocated;
	int total_bytes;
	int size_class_counts[NUM_SIZE_CLASSES];
	int size_class_bytes[NUM_SIZE_CLASSES];
	int peak_blocks;
	int peak_bytes;
};

/* Performance stats per size class (times in microseconds) */
struct perf_size_stats {
	size_t size_bytes;
	int block_count;
	long alloc_times_us[MAX_PERF_ITERATIONS];
	long free_times_us[MAX_PERF_ITERATIONS];
	int iterations;
	long avg_alloc_us, min_alloc_us, max_alloc_us;
	long avg_free_us, min_free_us, max_free_us;
};

/* Fragmentation stats per distribution */
struct frag_dist_stats {
	const char *dist_name;
	int block_count;
	size_t heap_size;
	size_t used_before;
	size_t free_before;
	size_t largest_before;
	size_t used_after;
	size_t free_after;
	size_t largest_after;
	size_t external_frag_pct;
	int frag_ratio_pct;  /* largest_free * 100 / total_free (100 = no frag) */
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const char *g_size_class_names[NUM_SIZE_CLASSES] = {
	"4B", "8B", "16B", "32B", "64B", "128B", "256B", "512B", "1K", "2K"
};

static const int g_size_class_sizes[NUM_SIZE_CLASSES] = {
	SIZE_CLASS_0, SIZE_CLASS_1, SIZE_CLASS_2, SIZE_CLASS_3, SIZE_CLASS_4,
	SIZE_CLASS_5, SIZE_CLASS_6, SIZE_CLASS_7, SIZE_CLASS_8, SIZE_CLASS_9
};

/* Distribution profile names */
static const char *g_dist_names[NUM_DIST_PROFILES] = {
	"current", "uniform", "small", "large"
};

/* Distribution weights (percent, must sum to 100 per row) */
static const int g_dist_weights[NUM_DIST_PROFILES][NUM_SIZE_CLASSES] = {
	/* current: bias toward medium sizes (128-512) */
	{ 0,  0, 10, 15, 25, 25, 15,  7,  2,  1 },
	/* uniform: equal across all 10 classes */
	{10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
	/* small: 80% in smallest 4 classes (4-32 bytes) */
	{30, 25, 15, 10, 10,  5,  3,  1,  1,  0 },
	/* large: 70% in largest 4 classes (256B-2K) */
	{ 0,  0,  2,  3,  5,  8, 12, 20, 25, 25 },
};

/* We use a heap-allocated array for pointer tracking.  Before running the
 * leak checker, we free the array, making the pointers unreachable.
 * This ensures that unfreed blocks are true leaks (unreachable from any
 * thread stack, BSS, DATA, or reachable heap block).
 */
static struct alloc_record *g_allocs;
static int g_num_allocs;
static int g_max_allocs;

/* Stress test data */
static struct stress_timing g_timing;
static struct stress_stats g_stats;
static int g_distribution = DEFAULT_DIST;

/* Pre-computed deterministic size class sequence.
 * Instead of calling rand() per block (which gives approximate
 * distribution percentages), we pre-compute an exact sequence:
 *   1. Calculate exact count per class from weights
 *   2. Fill array with class indices
 *   3. Fisher-Yates shuffle (seeded with srand(42))
 * This guarantees identical class counts every run.
 */
static int g_class_sequence[MAX_STRESS_BLOCKS];
static int g_sequence_len;
static int g_sequence_idx;

/****************************************************************************
 * Private Functions — Common Helpers
 ****************************************************************************/

/****************************************************************************
 * Private Functions — Common Helpers
 ****************************************************************************/

static void print_usage(const char *progname)
{
	printf("Usage: %s [num_allocs] [free_pct] [run_checker]\n", progname);
	printf("       %s --stress <mode> [args...]\n", progname);
	printf("\n");
	printf("Basic mode:\n");
	printf("  num_allocs   : Number of allocations (default %d, max %d)\n", DEFAULT_NUM_ALLOC, MAX_BLOCKS);
	printf("  free_pct     : Percentage of blocks to free, 0-100 (default %d)\n", DEFAULT_FREE_PCT);
	printf("  run_checker  : 1=run leak checker after test, 0=skip (default 0)\n");
	printf("\n");
	printf("Stress mode (--stress):\n");
	printf("  fill <pct> [blocks <count>] [checker <0|1>] : Fill heap to <pct> percent (1-%d)\n", MAX_FILL_PCT);
	printf("                        checker 1: run leak checker to detect intentional leaks\n");
	printf("                        checker 0: skip leak checker (default)\n");
	printf("                        Max fill: %d%% (higher values may cause OOM)\n", MAX_FILL_PCT);
	printf("                        Optional 'blocks <count>' caps block count\n");
	printf("                        Default: %d%%, Max blocks: %d\n", DEFAULT_FILL_PCT, MAX_STRESS_BLOCKS);
	printf("  perf [iterations]           : Performance test (default: %d iterations)\n", DEFAULT_PERF_ITERATIONS);
	printf("                        Tests all size classes with 100%% free\n");
	printf("                        Fills %d%% of heap, divided equally among size classes\n", PERF_FILL_PCT);
	printf("  frag [free_pct]             : Fragmentation test (default: %d%% free)\n", DEFAULT_FRAG_FREE_PCT);
	printf("                        Allocates to 85%% fill with each distribution\n");
	printf("                        Frees randomly, reports fragmentation metrics\n");
	printf("  --dist <profile>    : Size distribution (optional, fill mode only)\n");
	printf("                        current (default), uniform, small, large\n");
	printf("  Size classes: 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048 bytes\n");
	printf("\n");
	printf("Examples:\n");
	printf("  %s [num_allocs] [free_pct] [run_checker]   # Basic mode (e.g. 100 0 1)\n", progname);
	printf("  %s --stress fill [pct] [blocks <count>] [checker <0|1>] [--dist <profile>]\n", progname);
	printf("  %s --stress perf [iterations]\n", progname);
	printf("  %s --stress frag [free_pct]\n", progname);
	printf("\n");
	printf("Configuration (from .config):\n");
	printf("  CONFIG_EXAMPLES_HEAP_STRESS_MAX_BLOCKS: %d\n", MAX_STRESS_BLOCKS);
}

static long timeval_diff_ms(struct timeval *start, struct timeval *end)
{
	return (end->tv_sec - start->tv_sec) * 1000 +
	       (end->tv_usec - start->tv_usec) / 1000;
}

static size_t get_heap_size(void)
{
	struct mallinfo info;
#ifdef CONFIG_CAN_PASS_STRUCTS
	info = mallinfo();
#else
	mallinfo(&info);
#endif
	return info.arena;
}

static size_t get_heap_used(void)
{
	struct mallinfo info;
#ifdef CONFIG_CAN_PASS_STRUCTS
	info = mallinfo();
#else
	mallinfo(&info);
#endif
	return info.uordblks;
}

static size_t get_largest_free_block(void)
{
	struct mallinfo info;
#ifdef CONFIG_CAN_PASS_STRUCTS
	info = mallinfo();
#else
	mallinfo(&info);
#endif
	return info.mxordblk;
}

static long timeval_diff_us(struct timeval *start, struct timeval *end)
{
	return (end->tv_sec - start->tv_sec) * 1000000 +
	       (end->tv_usec - start->tv_usec);
}

static void print_heap_info(const char *phase)
{
	struct mallinfo info;
#ifdef CONFIG_CAN_PASS_STRUCTS
	info = mallinfo();
#else
	mallinfo(&info);
#endif
	printf("  [%s] Heap: used=%d KB, free=%d KB, total=%d KB, largest=%d KB\n",
	       phase,
	       info.uordblks / 1024,
	       info.fordblks / 1024,
	       info.arena / 1024,
	       info.mxordblk / 1024);
}

static void print_size_distribution(void)
{
	int i;
	printf("  Block distribution:");
	for (i = 0; i < NUM_SIZE_CLASSES; i++) {
		printf("    %-8s: %6d blocks, %8d bytes (%d KB)\n",
		       g_size_class_names[i],
		       g_stats.size_class_counts[i],
		       g_stats.size_class_bytes[i],
		       g_stats.size_class_bytes[i] / 1024);
	}
}

/* Build a pre-computed size class sequence with exact distribution counts.
 * After calling this, get_next_size_class() returns class indices in a
 * deterministic shuffled order — same sequence every run.
 */
static void build_class_sequence(int num_blocks)
{
	int counts[NUM_SIZE_CLASSES];
	int i, j, idx;

	if (num_blocks > MAX_STRESS_BLOCKS) {
		num_blocks = MAX_STRESS_BLOCKS;
	}

	/* Calculate exact count per class from weight percentages */
	for (i = 0; i < NUM_SIZE_CLASSES; i++) {
		counts[i] = (num_blocks * g_dist_weights[g_distribution][i]) / 100;
	}

	/* Fill the sequence array with class indices */
	idx = 0;
	for (i = 0; i < NUM_SIZE_CLASSES; i++) {
		for (j = 0; j < counts[i] && idx < num_blocks; j++) {
			g_class_sequence[idx++] = i;
		}
	}

	/* If weights don't sum to 100 (rounding), fill remainder with
	 * the largest-weight class to use up remaining slots.
	 */
	if (idx < num_blocks) {
		int best = 0;
		for (i = 1; i < NUM_SIZE_CLASSES; i++) {
			if (g_dist_weights[g_distribution][i] >
			    g_dist_weights[g_distribution][best]) {
				best = i;
			}
		}
		while (idx < num_blocks) {
			g_class_sequence[idx++] = best;
		}
	}

	g_sequence_len = num_blocks;

	/* Fisher-Yates shuffle for randomized order within determinism */
	srand(42);
	for (i = g_sequence_len - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int tmp = g_class_sequence[i];
		g_class_sequence[i] = g_class_sequence[j];
		g_class_sequence[j] = tmp;
	}

	g_sequence_idx = 0;
}

/* Return the next size class from the pre-computed sequence.
 * Wraps around if we exhaust the sequence (e.g., fill mode where
 * exact block count is unknown ahead of time).
 */
static int get_next_size_class(void)
{
	int cls = g_class_sequence[g_sequence_idx % g_sequence_len];
	g_sequence_idx++;
	return cls;
}

#ifdef CONFIG_MEM_LEAK_CHECKER
static void run_leak_checker(pid_t pid)
{
	printf("  (leak checker output follows)\n");
	prctl(PR_MEM_LEAK_CHECKER, pid, -1, NULL);
	printf("  (leak checker output ends)\n");
}
#endif

/****************************************************************************
 * Private Functions — Basic Leak Test
 ****************************************************************************/

static int do_allocations(int num_alloc)
{
	int i;
	int cls;
	size_t sz;

	if (num_alloc > MAX_BLOCKS) {
		printf("ERROR: Requested %d blocks exceeds max %d\n", num_alloc, MAX_BLOCKS);
		return -1;
	}

	for (i = 0; i < num_alloc; i++) {
		cls = i % NUM_SIZE_CLASSES;
		sz = g_size_class_sizes[cls];

		g_allocs[i].ptr = malloc(sz);
		if (g_allocs[i].ptr == NULL) {
			printf("ERROR: malloc failed at block %d\n", i);
			g_num_allocs = i;
			return -1;
		}
		g_allocs[i].size = sz;
		memset(g_allocs[i].ptr, 0xAA, sz);
	}

	g_num_allocs = num_alloc;
	return 0;
}

static int do_frees(int free_pct)
{
	int i;
	int freed = 0;
	int to_free = (g_num_allocs * free_pct) / 100;

	for (i = 0; i < g_num_allocs && freed < to_free; i++) {
		if (g_allocs[i].ptr != NULL) {
			free(g_allocs[i].ptr);
			g_allocs[i].ptr = NULL;
			freed++;
		}
	}

	return freed;
}

/****************************************************************************
 * Private Functions — Stress Test Helpers
 ****************************************************************************/

static int stress_do_allocations(int num_alloc)
{
	int i;
	size_t total_bytes = 0;

	if (num_alloc > g_max_allocs) {
		printf("ERROR: Requested %d blocks exceeds max %d\n", num_alloc, g_max_allocs);
		return -1;
	}

	build_class_sequence(num_alloc);

	for (i = 0; i < num_alloc; i++) {
		int cls = get_next_size_class();
		size_t sz = g_size_class_sizes[cls];

		g_allocs[i].ptr = malloc(sz);
		if (g_allocs[i].ptr == NULL) {
			printf("  Malloc failed at block %d (allocated %d blocks so far)\n",
			       i, i);
			g_num_allocs = i;
			return i;
		}
		g_allocs[i].size = sz;
		memset(g_allocs[i].ptr, 0xAA, sz);
		g_stats.size_class_counts[cls]++;
		g_stats.size_class_bytes[cls] += sz;
		total_bytes += sz;

		if (i + 1 > g_stats.peak_blocks) {
			g_stats.peak_blocks = i + 1;
		}
		if (total_bytes > g_stats.peak_bytes) {
			g_stats.peak_bytes = total_bytes;
		}
	}

	g_num_allocs = num_alloc;
	g_stats.total_allocated = num_alloc;
	g_stats.total_bytes = total_bytes;

	return num_alloc;
}

static int stress_do_frees(size_t target_bytes)
{
	int i;
	int freed = 0;
	size_t freed_bytes = 0;
	int pct;

	/* Free blocks randomly to simulate real-world fragmentation.
	 * First pass: probabilistic freeing.  Second pass: if we
	 * haven't freed enough bytes, free remaining blocks.
	 */
	if (g_stats.total_bytes > 0) {
		pct = (int)((target_bytes * 100) / g_stats.total_bytes);
		if (pct < 1) pct = 1;
		if (pct > 100) pct = 100;
	} else {
		pct = 100;
	}

	for (i = 0; i < g_num_allocs && freed_bytes < target_bytes; i++) {
		if (g_allocs[i].ptr != NULL && (rand() % 100) < pct) {
			free(g_allocs[i].ptr);
			g_allocs[i].ptr = NULL;
			freed_bytes += g_allocs[i].size;
			freed++;
		}
	}
	/* Second pass: free remaining blocks if not enough freed */
	for (i = 0; i < g_num_allocs && freed_bytes < target_bytes; i++) {
		if (g_allocs[i].ptr != NULL) {
			free(g_allocs[i].ptr);
			g_allocs[i].ptr = NULL;
			freed_bytes += g_allocs[i].size;
			freed++;
		}
	}

	return freed;
}

/****************************************************************************
 * Private Functions — Stress Test Modes
 ****************************************************************************/

static int heap_stress_fill_mode(int fill_pct, int max_blocks, int run_checker)
{
	size_t heap_size, target_bytes;
	size_t current_used;
	int alloc_count = 0;
	int i;

	printf("\n=== Heap Stress Test: Fill Mode ===\n");
	printf("Target: %d%% of heap", fill_pct);
	if (max_blocks > 0) {
		printf(", max %d blocks", max_blocks);
	}
	printf(", leak checker: %s", run_checker ? "on" : "off");
	printf("\n\n");

	printf("Phase 1: Detecting heap size...\n");
	heap_size = get_heap_size();
	current_used = get_heap_used();
	printf("  Current heap usage: %zu KB (%zu bytes)\n",
	       current_used / 1024, current_used);
	printf("  Total heap size: %zu KB (%zu bytes)\n",
	       heap_size / 1024, heap_size);

	target_bytes = (heap_size * fill_pct) / 100;
	printf("  Target allocation: %zu KB (%zu bytes, %d%%)\n\n",
	       target_bytes / 1024, target_bytes, fill_pct);

	g_max_allocs = (target_bytes / SIZE_CLASS_0) + 1000;
	if (g_max_allocs > MAX_STRESS_BLOCKS) {
		g_max_allocs = MAX_STRESS_BLOCKS;
	}
	/* If user specified a block limit, cap to that */
	if (max_blocks > 0 && g_max_allocs > max_blocks) {
		g_max_allocs = max_blocks;
		printf("  Block limit: %d (user-specified)\n", max_blocks);
	}
	g_allocs = (struct alloc_record *)malloc(sizeof(struct alloc_record) * g_max_allocs);
	if (!g_allocs) {
		printf("ERROR: Failed to allocate pointer array (need %d bytes)\n",
		       (int)(sizeof(struct alloc_record) * g_max_allocs));
		return -1;
	}
	memset(g_allocs, 0, sizeof(struct alloc_record) * g_max_allocs);
	printf("  Tracking array: %d KB (%d bytes) for %d max blocks\n",
	       (int)(sizeof(struct alloc_record) * g_max_allocs) / 1024,
	       (int)(sizeof(struct alloc_record) * g_max_allocs),
	       g_max_allocs);
	g_num_allocs = 0;
	memset(&g_stats, 0, sizeof(g_stats));

	printf("Phase 2: Allocating blocks...\n");
	build_class_sequence(g_max_allocs);
	gettimeofday(&g_timing.alloc_start, NULL);

	i = 0;
	while (1) {
		int cls = get_next_size_class();
		size_t sz = g_size_class_sizes[cls];

		if (i >= g_max_allocs) {
			printf("  Reached max block count (%d), stopping\n", g_max_allocs);
			break;
		}

		g_allocs[i].ptr = malloc(sz);
		if (g_allocs[i].ptr == NULL) {
			printf("  Malloc failed at block %d (heap full)\n", i);
			break;
		}

		g_allocs[i].size = sz;
		memset(g_allocs[i].ptr, 0xAA, sz);
		g_stats.size_class_counts[cls]++;
		g_stats.size_class_bytes[cls] += sz;
		g_stats.total_bytes += sz;
		i++;

		/* Check actual heap usage (includes allocnode headers and
		 * alignment padding) rather than just the user-requested
		 * bytes.  This prevents overfilling the heap.
		 */
		if (fill_pct < 100) {
			size_t actual_used = get_heap_used();
			if (actual_used >= target_bytes) {
				printf("  Reached target (%d%%, actual heap: %zu KB)\n",
				       fill_pct, actual_used / 1024);
				break;
			}
		}

		if (i % 1000 == 0) {
			printf("  ... %d blocks (%d KB)\n", i, g_stats.total_bytes / 1024);
		}
	}

	alloc_count = i;
	g_num_allocs = alloc_count;
	g_stats.total_allocated = alloc_count;
	g_stats.peak_blocks = alloc_count;
	g_stats.peak_bytes = g_stats.total_bytes;

	gettimeofday(&g_timing.alloc_end, NULL);
	g_timing.alloc_time_ms = timeval_diff_ms(&g_timing.alloc_start, &g_timing.alloc_end);

	printf("\n  Allocated %d blocks in %ld ms\n", alloc_count, g_timing.alloc_time_ms);
	if (g_timing.alloc_time_ms > 0) {
		printf("  Allocation rate: %ld blocks/sec, %ld KB/sec\n",
		       (alloc_count * 1000) / g_timing.alloc_time_ms,
		       (g_stats.total_bytes / 1024 * 1000) / g_timing.alloc_time_ms);
	}
	print_size_distribution();
	print_heap_info("After alloc");

	/* Phase 3: Run leak checker.
	 *
	 * The leak checker's internal data structures (hash table, block info
	 * array, worklist) are allocated from the heap.  With the optimized
	 * leak_block_info_s (4 bytes per record), the leak checker needs
	 * ~1.4 MB for 90K blocks.  At 75% fill, ~25% of the heap is free
	 * (~1.3 MB on a 5 MB heap).  Additionally, g_allocs (the tracking
	 * array, ~703 KB) is freed below before the leak checker runs,
	 * giving a total of ~2.0 MB available — sufficient for the leak
	 * checker with ~600 KB margin for fragmentation.
	 *
	 * No test blocks need to be freed; all allocated blocks become
	 * intentional leaks for the leak checker to detect.
	 */
	gettimeofday(&g_timing.leak_start, NULL);

	/* Free the tracking array so leaked blocks become unreachable.
	 * The leak checker scans heap; if pointers remain in g_allocs,
	 * blocks would be marked as reachable (not leaks).
	 */
	free(g_allocs);
	g_allocs = NULL;

	if (run_checker) {
		printf("\nPhase 3: Running leak checker...\n");
#ifdef CONFIG_MEM_LEAK_CHECKER
		run_leak_checker(getpid());
#else
		printf("  WARNING: CONFIG_MEM_LEAK_CHECKER is not enabled.\n");
#endif
	} else {
		printf("\nPhase 3: Skipping leak checker (not requested)\n");
	}

	gettimeofday(&g_timing.leak_end, NULL);
	g_timing.leak_check_time_ms = timeval_diff_ms(&g_timing.leak_start, &g_timing.leak_end);

	printf("\n  Leak checker: %ld ms total\n", g_timing.leak_check_time_ms);
	if (g_timing.leak_check_time_ms > 0) {
		printf("  Check rate: %ld blocks/ms\n",
		       alloc_count / g_timing.leak_check_time_ms);
	}

	printf("\n=== Performance Summary ===\n");
	printf("  Heap fill: %d%% (%d / %zu KB)\n",
	       fill_pct, g_stats.peak_bytes / 1024, heap_size / 1024);
	printf("  Block count: %d\n", alloc_count);
	printf("  Allocation time: %ld ms\n", g_timing.alloc_time_ms);
	printf("  Leak checker time: %ld ms\n", g_timing.leak_check_time_ms);
	printf("  Total test time: %ld ms\n",
	       g_timing.alloc_time_ms + g_timing.leak_check_time_ms);

	return 0;
}

static void print_perf_results(struct perf_size_stats *stats, int num_classes,
			      int iterations, size_t heap_size)
{
	int i;

	printf("\n=== Performance Test Results ===\n");
	printf("Iterations: %d\n", iterations);
	printf("Target fill: %d%% (divided equally: %d%% per size class)\n",
	       PERF_FILL_PCT, PERF_FILL_PCT / NUM_SIZE_CLASSES);
	printf("Heap size: %zu KB\n\n", heap_size / 1024);
	printf("Size  | Blocks | Batch Time (ms) avg/min/max | Free Batch (ms) avg/min/max | Alloc Rate  | Free Rate\n");
	printf("------|--------|-----------------------------|---------------------------|-------------|----------\n");

	for (i = 0; i < num_classes; i++) {
		long alloc_ms = stats[i].avg_alloc_us / 1000;
		long alloc_min = stats[i].min_alloc_us / 1000;
		long alloc_max = stats[i].max_alloc_us / 1000;
		long free_ms = stats[i].avg_free_us / 1000;
		long free_min = stats[i].min_free_us / 1000;
		long free_max = stats[i].max_free_us / 1000;

		printf("%-5s | %6d | %6ld / %6ld / %6ld | %6ld / %6ld / %6ld | ",
		       g_size_class_names[i], stats[i].block_count,
		       alloc_ms, alloc_min, alloc_max,
		       free_ms, free_min, free_max);

		if (stats[i].avg_alloc_us > 0) {
			long long rate = (long long)stats[i].block_count * 1000000 / stats[i].avg_alloc_us;
			if (rate >= 1000000) {
				printf("%lld.%lldM blk/s | ", rate / 1000000, (rate / 100000) % 10);
			} else {
				printf("%lldK blk/s | ", rate / 1000);
			}
		} else {
			printf("N/A         | ");
		}

		if (stats[i].avg_free_us > 0) {
			long long rate = (long long)stats[i].block_count * 1000000 / stats[i].avg_free_us;
			if (rate >= 1000000) {
				printf("%lld.%lldM blk/s\n", rate / 1000000, (rate / 100000) % 10);
			} else {
				printf("%lldK blk/s\n", rate / 1000);
			}
		} else {
			printf("N/A\n");
		}
	}
}

static int heap_stress_perf_mode(int iterations)
{
	int cls, iter, i;
	struct perf_size_stats stats[NUM_SIZE_CLASSES];
	size_t heap_size;
	size_t bytes_per_class;

	printf("\n=== Heap Stress Test: Performance Mode ===\n");
	printf("Iterations: %d\n", iterations);

	heap_size = get_heap_size();
	bytes_per_class = (heap_size * PERF_FILL_PCT) / (100 * NUM_SIZE_CLASSES);
	printf("Heap size: %zu KB, target fill: %d%% (%zu KB per class)\n\n",
	       heap_size / 1024, PERF_FILL_PCT, bytes_per_class / 1024);

	for (cls = 0; cls < NUM_SIZE_CLASSES; cls++) {
		size_t sz = g_size_class_sizes[cls];
		int blocks = (int)(bytes_per_class / sz);

		stats[cls].size_bytes = sz;
		stats[cls].block_count = blocks;
		stats[cls].iterations = iterations;

		for (iter = 0; iter < iterations; iter++) {
			struct timeval t1, t2;
			void **ptrs;
			int actual_blocks = blocks;

			ptrs = (void **)malloc(sizeof(void *) * blocks);
			if (!ptrs) {
				printf("ERROR: Failed to allocate pointer array for %d blocks\n", blocks);
				return -1;
			}

			/* Time allocation */
			gettimeofday(&t1, NULL);
			for (i = 0; i < blocks; i++) {
				ptrs[i] = malloc(sz);
				if (ptrs[i] == NULL) {
					printf("  malloc failed at block %d (size %zu)\n", i, sz);
					actual_blocks = i;
					break;
				}
				memset(ptrs[i], 0xAA, sz);
			}
			gettimeofday(&t2, NULL);
			stats[cls].alloc_times_us[iter] = timeval_diff_us(&t1, &t2);

			/* Time free */
			gettimeofday(&t1, NULL);
			for (i = 0; i < actual_blocks; i++) {
				if (ptrs[i]) {
					free(ptrs[i]);
				}
			}
			gettimeofday(&t2, NULL);
			stats[cls].free_times_us[iter] = timeval_diff_us(&t1, &t2);

			free(ptrs);

			printf("  %-5s iter %d: alloc %ld us, free %ld us (%d blocks)\n",
			       g_size_class_names[cls], iter + 1,
			       stats[cls].alloc_times_us[iter],
			       stats[cls].free_times_us[iter], actual_blocks);

			if (actual_blocks < blocks) {
				stats[cls].block_count = actual_blocks;
				break;
			}
		}

		/* Compute avg/min/max */
		{
			long sum_a = 0, sum_f = 0;
			stats[cls].min_alloc_us = stats[cls].alloc_times_us[0];
			stats[cls].max_alloc_us = stats[cls].alloc_times_us[0];
			stats[cls].min_free_us = stats[cls].free_times_us[0];
			stats[cls].max_free_us = stats[cls].free_times_us[0];
			for (iter = 0; iter < iterations; iter++) {
				sum_a += stats[cls].alloc_times_us[iter];
				sum_f += stats[cls].free_times_us[iter];
				if (stats[cls].alloc_times_us[iter] < stats[cls].min_alloc_us)
					stats[cls].min_alloc_us = stats[cls].alloc_times_us[iter];
				if (stats[cls].alloc_times_us[iter] > stats[cls].max_alloc_us)
					stats[cls].max_alloc_us = stats[cls].alloc_times_us[iter];
				if (stats[cls].free_times_us[iter] < stats[cls].min_free_us)
					stats[cls].min_free_us = stats[cls].free_times_us[iter];
				if (stats[cls].free_times_us[iter] > stats[cls].max_free_us)
					stats[cls].max_free_us = stats[cls].free_times_us[iter];
			}
			stats[cls].avg_alloc_us = sum_a / iterations;
			stats[cls].avg_free_us = sum_f / iterations;
		}
	}

	print_perf_results(stats, NUM_SIZE_CLASSES, iterations, heap_size);
	return 0;
}

static void print_frag_results(struct frag_dist_stats *stats, int num_dists,
			       int free_pct)
{
	int i;

	printf("\n=== Fragmentation Test Results ===\n");
	printf("Target fill: %d%%, Free percentage: %d%%\n\n", FRAG_FILL_PCT, free_pct);
	printf("Distribution | Blocks | Before Free       | After Free        | Ext Frag%% | Largest Free | Contiguity Ratio\n");
	printf("-------------|--------|-------------------|-------------------|-----------|--------------|-------------------\n");

	for (i = 0; i < num_dists; i++) {
		int before_pct = (stats[i].heap_size > 0) ?
			(int)(stats[i].used_before * 100 / stats[i].heap_size) : 0;
		int after_pct = (stats[i].heap_size > 0) ?
			(int)(stats[i].used_after * 100 / stats[i].heap_size) : 0;

		printf("%-12s | %6d | %3d%% (%4dKB used) | %3d%% (%4dKB used) | %9d%% | %8d KB | %d.%02d\n",
		       stats[i].dist_name, stats[i].block_count,
		       before_pct, (int)(stats[i].used_before / 1024),
		       after_pct, (int)(stats[i].used_after / 1024),
		       (int)stats[i].external_frag_pct,
		       (int)(stats[i].largest_after / 1024),
		       stats[i].frag_ratio_pct / 100,
		       stats[i].frag_ratio_pct % 100);
	}

	printf("\nNote: External fragmentation %% = ((total_free - largest_free) / total_free) x 100\n");
	printf("      Contiguity ratio = largest_free x 10000 / total_free (stored as X.XX × 100)\n");
	printf("      Display: 100.00 = fully contiguous (no fragmentation), 0.00 = highly fragmented\n");
	printf("      Lower external frag%% and higher ratio = better (less fragmented)\n");
}

static int heap_stress_frag_mode(int free_pct)
{
	int dist;
	struct frag_dist_stats results[NUM_DIST_PROFILES];
	size_t heap_size;

	printf("\n=== Heap Stress Test: Fragmentation Mode ===\n");
	printf("Target fill: %d%%, Free percentage: %d%%\n", FRAG_FILL_PCT, free_pct);

	heap_size = get_heap_size();
	printf("  Heap size: %zu KB\n\n", heap_size / 1024);

	for (dist = 0; dist < NUM_DIST_PROFILES; dist++) {
		size_t target_bytes;
		int alloc_count = 0;
		int i;
		size_t target_free_bytes;
		int freed_count;

		printf("--- Distribution: %s ---\n", g_dist_names[dist]);

		g_distribution = dist;

		/* Calculate target bytes for 75% fill */
		target_bytes = (heap_size * FRAG_FILL_PCT) / 100;

		/* Estimate max allocs (worst case: all 4-byte blocks) */
		g_max_allocs = (target_bytes / SIZE_CLASS_0) + 1000;
		if (g_max_allocs > MAX_STRESS_BLOCKS) {
			g_max_allocs = MAX_STRESS_BLOCKS;
		}

		/* Allocate tracking array */
		g_allocs = (struct alloc_record *)malloc(sizeof(struct alloc_record) * g_max_allocs);
		if (!g_allocs) {
			printf("ERROR: Failed to allocate pointer array\n");
			return -1;
		}
		memset(g_allocs, 0, sizeof(struct alloc_record) * g_max_allocs);
		g_num_allocs = 0;
		memset(&g_stats, 0, sizeof(g_stats));

		/* Allocate blocks until 75% full */
		build_class_sequence(g_max_allocs);

		for (i = 0; i < g_max_allocs; i++) {
			int cls = get_next_size_class();
			size_t sz = g_size_class_sizes[cls];
			size_t actual_used;

			g_allocs[i].ptr = malloc(sz);
			if (g_allocs[i].ptr == NULL) {
				printf("  malloc failed at block %d\n", i);
				break;
			}
			g_allocs[i].size = sz;
			memset(g_allocs[i].ptr, 0xAA, sz);
			g_stats.total_bytes += sz;

			/* Check actual heap usage */
			actual_used = get_heap_used();
			if (actual_used >= target_bytes) {
				alloc_count = i + 1;
				break;
			}
		}

		if (alloc_count == 0) {
			alloc_count = i;
		}
		g_num_allocs = alloc_count;

		printf("  Allocated %d blocks (%d KB)\n", alloc_count,
		       g_stats.total_bytes / 1024);

		/* Record before-free stats */
		results[dist].dist_name = g_dist_names[dist];
		results[dist].block_count = alloc_count;
		results[dist].heap_size = heap_size;
		results[dist].used_before = get_heap_used();
		results[dist].free_before = heap_size - results[dist].used_before;
		results[dist].largest_before = get_largest_free_block();

		printf("  Before free: %d%% used (%d KB), largest free %d KB\n",
		       (int)(results[dist].used_before * 100 / heap_size),
		       (int)(results[dist].used_before / 1024),
		       (int)(results[dist].largest_before / 1024));

		/* Free free_pct% randomly.
		 * Reseed with a distribution-specific seed so each distribution
		 * gets a different free pattern (build_class_sequence reseeds
		 * with srand(42), which would otherwise leave the RNG in the
		 * same state for every distribution). */
		target_free_bytes = (g_stats.total_bytes * free_pct) / 100;
		printf("  Freeing %d%% (%zu KB)...\n", free_pct,
		       target_free_bytes / 1024);
		srand(42 + dist * 1000 + free_pct);
		freed_count = stress_do_frees(target_free_bytes);
		printf("  Freed %d blocks\n", freed_count);

		/* Record after-free stats */
		results[dist].used_after = get_heap_used();
		results[dist].free_after = heap_size - results[dist].used_after;
		results[dist].largest_after = get_largest_free_block();

		printf("  After free: %d%% used (%d KB), largest free %d KB\n",
		       (int)(results[dist].used_after * 100 / heap_size),
		       (int)(results[dist].used_after / 1024),
		       (int)(results[dist].largest_after / 1024));

		/* Calculate fragmentation metrics */
		if (results[dist].free_after > 0) {
			results[dist].external_frag_pct =
				((results[dist].free_after - results[dist].largest_after) * 100)
				/ results[dist].free_after;
			results[dist].frag_ratio_pct =
				(int)(results[dist].largest_after * 10000 / results[dist].free_after);
		} else {
			results[dist].external_frag_pct = 0;
			results[dist].frag_ratio_pct = 0;
		}

		printf("  External frag: %d%%, ratio: %d:%02d\n\n",
		       (int)results[dist].external_frag_pct,
		       results[dist].frag_ratio_pct / 100,
		       results[dist].frag_ratio_pct % 100);

		/* Free all remaining blocks */
		for (i = 0; i < alloc_count; i++) {
			if (g_allocs[i].ptr != NULL) {
				free(g_allocs[i].ptr);
				g_allocs[i].ptr = NULL;
			}
		}

		free(g_allocs);
		g_allocs = NULL;
	}

	print_frag_results(results, NUM_DIST_PROFILES, free_pct);
	return 0;
}

/****************************************************************************
 * Private Functions — Stress Test Dispatcher
 ****************************************************************************/

static int heap_stress_run(int argc, char *argv[])
{
	/* In child task: argv[0]=progname, argv[1]=task name,
	 * argv[2]="--stress", argv[3]=mode, argv[4..]=mode args
	 */
	const char *mode = argv[3];
	int i;

	/* Parse --dist <profile> from any position in args */
	g_distribution = DEFAULT_DIST;
	for (i = 4; i < argc - 1; i++) {
		if (strcmp(argv[i], "--dist") == 0) {
			int j;
			for (j = 0; j < NUM_DIST_PROFILES; j++) {
				if (strcmp(argv[i + 1], g_dist_names[j]) == 0) {
					g_distribution = j;
					break;
				}
			}
			if (j == NUM_DIST_PROFILES) {
				printf("ERROR: Invalid distribution: %s\n", argv[i + 1]);
				printf("  Valid: current, uniform, small, large\n");
				return -1;
			}
			break;
		}
	}

	printf("  Max blocks (CONFIG_HEAP_STRESS_MAX_BLOCKS): %d\n", MAX_STRESS_BLOCKS);
	printf("  Distribution: %s\n\n", g_dist_names[g_distribution]);

	if (mode == NULL) {
		printf("ERROR: Missing stress mode\n");
		print_usage(argv[1]);
		return -1;
	}

	if (strcmp(mode, "fill") == 0) {
		int fill_pct = (argc >= 5) ? atoi(argv[4]) : DEFAULT_FILL_PCT;
		int max_blocks = 0;
		int run_checker = 0;
		int j;

		if (fill_pct < 1 || fill_pct > MAX_FILL_PCT) {
			printf("Invalid fill percentage: %d (valid: 1-%d)\n", fill_pct, MAX_FILL_PCT);
			return -1;
		}

		/* Check for optional 'blocks <count>' and 'checker <0|1>'
		 * after fill percentage
		 */
		for (j = 5; j < argc - 1; j++) {
			if (strcmp(argv[j], "blocks") == 0) {
				max_blocks = atoi(argv[j + 1]);
				if (max_blocks < 1 || max_blocks > MAX_STRESS_BLOCKS) {
					printf("Invalid block count: %d (valid: 1-%d)\n", max_blocks, MAX_STRESS_BLOCKS);
					return -1;
				}
			} else if (strcmp(argv[j], "checker") == 0) {
				run_checker = atoi(argv[j + 1]);
				if (run_checker < 0 || run_checker > 1) {
					printf("Invalid checker flag: %d (valid: 0-1)\n", run_checker);
					return -1;
				}
			}
		}
		return heap_stress_fill_mode(fill_pct, max_blocks, run_checker);

	} else if (strcmp(mode, "perf") == 0) {
		int iterations = (argc >= 5) ? atoi(argv[4]) : DEFAULT_PERF_ITERATIONS;
		if (iterations < 1 || iterations > MAX_PERF_ITERATIONS) {
			printf("Invalid iterations: %d (valid: 1-%d)\n", iterations, MAX_PERF_ITERATIONS);
			return -1;
		}
		return heap_stress_perf_mode(iterations);

	} else if (strcmp(mode, "frag") == 0) {
		int free_pct = (argc >= 5) ? atoi(argv[4]) : DEFAULT_FRAG_FREE_PCT;
		if (free_pct < 0 || free_pct > 100) {
			printf("Invalid free_pct: %d (valid: 0-100)\n", free_pct);
			return -1;
		}
		return heap_stress_frag_mode(free_pct);

	} else {
		printf("Unknown stress mode: %s\n", mode);
		print_usage(argv[1]);
		return -1;
	}
}

/****************************************************************************
 * Private Functions — Test Runner
 ****************************************************************************/

static int heap_bench_test_run(int argc, char *argv[])
{
	int num_alloc = DEFAULT_NUM_ALLOC;
	int free_pct = DEFAULT_FREE_PCT;
	int run_checker = 0;
	int freed_count;
	int leak_count;

	/* In child task: argv[0]=progname, argv[1]=task name,
	 * argv[2]=first user arg, argv[3]=second user arg, etc.
	 */

	/* Check for --help */
	if (argc >= 2 && (strcmp(argv[2], "--help") == 0 || strcmp(argv[2], "-h") == 0)) {
		print_usage(argv[1]);
		return 0;
	}

	/* Check for --stress mode */
	if (argc >= 2 && strcmp(argv[2], "--stress") == 0) {
		printf("\n=== Heap Stress Test ===\n");
		return heap_stress_run(argc, argv);
	}

	/* Basic leak test mode */
	g_allocs = (struct alloc_record *)malloc(sizeof(struct alloc_record) * MAX_BLOCKS);
	if (!g_allocs) {
		printf("ERROR: Failed to allocate pointer array\n");
		return -1;
	}
	memset(g_allocs, 0, sizeof(struct alloc_record) * MAX_BLOCKS);
	g_num_allocs = 0;

	if (argc >= 2) {
		num_alloc = atoi(argv[2]);
		if (num_alloc <= 0 || num_alloc > MAX_BLOCKS) {
			printf("Invalid num_allocs: %d (valid: 1-%d)\n", num_alloc, MAX_BLOCKS);
			return -1;
		}
	}
	if (argc >= 3) {
		free_pct = atoi(argv[3]);
		if (free_pct < 0 || free_pct > 100) {
			printf("Invalid free_pct: %d (valid: 0-100)\n", free_pct);
			return -1;
		}
	}
	if (argc >= 4) {
		run_checker = atoi(argv[4]);
	}

	printf("\n=== Heap Leak Test ===\n");
	printf("Parameters: num_allocs=%d, free_pct=%d%%, run_checker=%d\n",
	       num_alloc, free_pct, run_checker);

	printf("\nPhase 1: Allocating %d blocks...\n", num_alloc);
	if (do_allocations(num_alloc) != 0) {
		return -1;
	}

	printf("\nPhase 2: Freeing %d%% of blocks...\n", free_pct);
	freed_count = do_frees(free_pct);
	leak_count = g_num_allocs - freed_count;

	printf("\nPhase 3: Freeing pointer array (making leaks unreachable)...\n");
	free(g_allocs);
	g_allocs = NULL;

	if (run_checker) {
		printf("\nPhase 4: Running leak checker...\n");
#ifdef CONFIG_MEM_LEAK_CHECKER
		run_leak_checker(getpid());
#else
		printf("  WARNING: CONFIG_MEM_LEAK_CHECKER is not enabled.\n");
		printf("  Enable it in menuconfig to use the leak checker.\n");
#endif
	}

	printf("\nPhase 5: Cleanup...\n");
	printf("  Note: Blocks freed automatically when task exits (pointer array was freed)\n");

	printf("\n=== Test Summary ===\n");
	printf("  Allocated: %d blocks\n", g_num_allocs);
	printf("  Freed:     %d blocks\n", freed_count);
	printf("  Leaked:    %d blocks [intentional]\n", leak_count);
	if (run_checker) {
		printf("  Expected leak checker to detect: %d leaks\n", leak_count);
	}
	printf("=== Test Complete ===\n");

	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int heapbench_main(int argc, char *argv[])
#endif
{
	printf("Heap Bench Test!!\n");
	task_create("Heap bench test", 100, HEAP_BENCH_STACK_SIZE,
	            heap_bench_test_run, argv);

	sleep(1);

	return 0;
}
