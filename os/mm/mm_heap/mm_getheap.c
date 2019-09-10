/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <string.h>
#include <tinyara/mm/mm.h>
#ifdef CONFIG_MM_KERNEL_HEAP
#include <tinyara/sched.h>
#endif
#ifdef CONFIG_APP_BINARY_SEPARATION
#include <tinyara/binary_manager.h>
#endif

#if defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__)

#include <tinyara/userspace.h>

#ifdef CONFIG_APP_BINARY_SEPARATION
#include <tinyara/sched.h>
#define USR_HEAP_TCB ((struct mm_heap_s *)((struct tcb_s*)sched_self())->ram_start)
#define USR_HEAP_CFG ((struct mm_heap_s *)(*(uint32_t *)(CONFIG_TINYARA_USERSPACE + sizeof(struct userspace_s))))
#define USR_HEAP (USR_HEAP_TCB == NULL ? USR_HEAP_CFG : USR_HEAP_TCB)
#else
#define USR_HEAP ((struct mm_heap_s *)(*(uint32_t *)(CONFIG_TINYARA_USERSPACE + sizeof(struct userspace_s))))
#endif

#elif defined(CONFIG_BUILD_PROTECTED) && !defined(__KERNEL__)
extern uint32_t _stext;
#define USR_HEAP ((struct mm_heap_s *)_stext)

#else
extern struct mm_heap_s g_mmheap[CONFIG_MM_NHEAPS];
#define USR_HEAP       g_mmheap
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/
#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(__KERNEL__)
#include <queue.h>

typedef struct app_heap_s {
	struct app_heap_s *flink;
	struct app_heap_s *blink;
	char app_name[BIN_NAME_MAX];
	struct mm_heap_s *heap;
} app_heap_s;

static dq_queue_t app_heap_q;

#ifdef CONFIG_MM_PARTITION_HEAP
extern struct mm_heap_s g_pheap;
#endif

#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(__KERNEL__)
void mm_initialize_app_heap()
{
	dq_init(&app_heap_q);
}

void mm_add_app_heap_list(struct mm_heap_s *heap, char *app_name)
{
	app_heap_s *node = (app_heap_s *)kmm_malloc(sizeof(app_heap_s));
	if (!node) {
		mdbg("Error allocating heap node\n");
		return;
	}

	node->heap = heap;
	strncpy(node->app_name, app_name, BIN_NAME_MAX);

	/* Add the new heap node to the head of the list*/
	dq_addfirst((dq_entry_t *)node, &app_heap_q);
}

void mm_remove_app_heap_list(struct mm_heap_s *heap)
{
	app_heap_s *node = (app_heap_s *)dq_peek(&app_heap_q);

	/* Search the heap node in the list */
	while (node) {
		if (node->heap == heap) {
			/* Remove and free the matching node */
			dq_rem((dq_entry_t *)node, &app_heap_q);
			kmm_free(node);
			return;
		}

		node = dq_next(node);
	}
}

static struct mm_heap_s *mm_get_app_heap(void *address)
{
	/* First, search the address in list of app heaps */
	app_heap_s *node = (app_heap_s *)dq_peek(&app_heap_q);

	while (node) {
		if ((address > (void *)node->heap->mm_heapstart[0]) && (address < (void *)node->heap->mm_heapend[0])) {
			return node->heap;
		}
		node = dq_next(node);
	}

#ifdef CONFIG_MM_PARTITION_HEAP
	/* If address was not found in the app heaps, then it might be in the partition heap */
	if ((address > (void *)g_pheap.mm_heapstart[0]) && (address < (void *)g_pheap.mm_heapend[0])) {
		return &g_pheap;
	}
#endif

	mdbg("address 0x%x is not in any app heap region.\n", address);
	return NULL;
}

struct mm_heap_s *mm_get_app_heap_with_name(char *app_name)
{
	app_heap_s *node = (app_heap_s *)dq_peek(&app_heap_q);

	/* Search the heap */
	while (node) {
		if (strncmp(node->app_name, app_name, BIN_NAME_MAX) == 0) {
			return node->heap;
		}

		node = dq_next(node);
	}

	/* There is no app which matched with app_name. */
	return NULL;
}

char *mm_get_app_heap_name(void *address)
{
	/* First, search the address in list of app heaps */
	app_heap_s *node = (app_heap_s *)dq_peek(&app_heap_q);

	while (node) {
		if ((address > (void *)node->heap->mm_heapstart[0]) && (address < (void *)node->heap->mm_heapend[0])) {
			return node->app_name;
		}
		node = dq_next(node);
	}

	mdbg("address 0x%x is not in any app heap region.\n", address);
	return NULL;
}
#endif

/****************************************************************************
 * Name: mm_get_heap
 *
 * Description:
 *   returns a heap which type is matched with ttype
 *
 ****************************************************************************/
struct mm_heap_s *mm_get_heap(void *address)
{
#ifdef CONFIG_MM_KERNEL_HEAP
	struct mm_heap_s *heap = kmm_get_heap();
	if (address >= (FAR void *)heap->mm_heapstart[0] && address < (FAR void *)heap->mm_heapend[0]) {
		return heap;
	}
#endif
	int heap_idx;
	heap_idx = mm_get_heapindex(address);
	if (heap_idx == INVALID_HEAP_IDX) {
#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(__KERNEL__)
		/* If address was not found in kernel or user heap, search for it in app heaps */
		return mm_get_app_heap(address);
#else
		mdbg("address 0x%x is not in heap region.\n", address);
		return NULL;
#endif
	}

	return &USR_HEAP[heap_idx];
}


/****************************************************************************
 * Name: mm_get_heap_with_index
 ****************************************************************************/
struct mm_heap_s *mm_get_heap_with_index(int index)
{
	if (index >= CONFIG_MM_NHEAPS) {
		mdbg("heap index is out of range.\n");
		return NULL;
	}
	return &USR_HEAP[index];
}

/****************************************************************************
 * Name: mm_get_heapindex
 ****************************************************************************/
int mm_get_heapindex(void *mem)
{
	int heap_idx;
	if (mem == NULL) {
		return 0;
	}
	for (heap_idx = 0; heap_idx < CONFIG_MM_NHEAPS; heap_idx++) {
		int region = 0;
#if CONFIG_MM_REGIONS > 1
		for (; region < g_mmheap[heap_idx].mm_nregions; region++)
#endif
		{
			/* A valid address from the user heap for this region would have to lie
			 * between the region's two guard nodes.
			 */
			if ((mem > (void *)USR_HEAP[heap_idx].mm_heapstart[region]) && (mem < (void *)USR_HEAP[heap_idx].mm_heapend[region])) {
				return heap_idx;
			}
		}
	}
	/* The address does not lie in the user heaps */
	return INVALID_HEAP_IDX;
}
