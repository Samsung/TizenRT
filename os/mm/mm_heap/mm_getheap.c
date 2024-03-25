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
	bool is_active;
} app_heap_s;

static dq_queue_t app_heap_q;

#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(__KERNEL__)
void mm_initialize_app_heap_q()
{
	dq_init(&app_heap_q);
}

void mm_add_app_heap_list(struct mm_heap_s *heap, char *app_name)
{
	app_heap_s *node = (app_heap_s *)dq_peek(&app_heap_q);

	/* Search the heap node in the list */
	while (node) {
		if (node->heap == heap) {
			/* Remove and free the matching node */
			node->is_active = true;
			return;
		}

		node = dq_next(node);
	}

	node = (app_heap_s *)kmm_malloc(sizeof(app_heap_s));
	if (!node) {
		mdbg("Error allocating heap node\n");
		return;
	}

	node->is_active = true;
	node->heap = heap;

	strncpy(node->app_name, app_name, BIN_NAME_MAX - 1);
	node->app_name[BIN_NAME_MAX - 1] = '\0';

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

void mm_disable_app_heap_list(struct mm_heap_s *heap)
{
	app_heap_s *node = (app_heap_s *)dq_peek(&app_heap_q);

	/* Search the heap node in the list */
	while (node) {
		if (node->heap == heap) {
			/* Disable the matching node */
			node->is_active = false;
			return;
		}

		node = dq_next(node);
	}
}

static app_heap_s *mm_get_app_heap_node(void *address)
{
	/* First, search the address in list of app heaps */
	app_heap_s *node = (app_heap_s *)dq_peek(&app_heap_q);

	while (node) {
		if ((address > (void *)node->heap->mm_heapstart[0]) && (address < (void *)node->heap->mm_heapend[0])) {
			return node;
		}
		node = dq_next(node);
	}

	return NULL;
}

struct mm_heap_s *mm_get_app_heap_with_name(char *app_name)
{
	app_heap_s *node = (app_heap_s *)dq_peek(&app_heap_q);

	/* Search the heap */
	while (node && node->is_active) {
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

	while (node && node->is_active) {
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
#if defined(CONFIG_APP_BINARY_SEPARATION)
#if defined(__KERNEL__)
	/* Try to find the app heap from kernel. */
	app_heap_s *node =  mm_get_app_heap_node(address);
	if (node) {
		if (node->is_active) {
			return node->heap;
		} else {
			return NULL;
		}
	}
#else
	/* User can only have single heap. */
	return BASE_HEAP;
#endif
#endif

#ifdef CONFIG_MM_KERNEL_HEAP
	int heap_idx;
	int region_idx;
	struct mm_heap_s *heap = kmm_get_baseheap();

	for (heap_idx = 0; heap_idx < CONFIG_KMM_NHEAPS; heap_idx++) {
		for (region_idx = 0; region_idx < CONFIG_KMM_REGIONS; region_idx++) {
			if (address >= (FAR void *)heap[heap_idx].mm_heapstart[region_idx] && address < (FAR void *)heap[heap_idx].mm_heapend[region_idx]) {
				return &heap[heap_idx];
			}
		}
	}
#endif
	return NULL;
}

/****************************************************************************
 * Name: mm_get_heap_with_index
 ****************************************************************************/
struct mm_heap_s *mm_get_heap_with_index(int index)
{
	if (index >= CONFIG_KMM_NHEAPS) {
		mdbg("heap index is out of range.\n");
		return NULL;
	}
	return &BASE_HEAP[index];
}

/****************************************************************************
 * Name: mm_get_index_of_heap
 ****************************************************************************/
int mm_get_index_of_heap(void *mem)
{
	int heap_idx;
	if (mem == NULL) {
		return 0;
	}
	if (BASE_HEAP == NULL) {
		lldbg("BASE_HEAP NULL\n");
		return 0;
	}

	for (heap_idx = 0; heap_idx < CONFIG_KMM_NHEAPS; heap_idx++) {
		int region = 0;
#if CONFIG_KMM_REGIONS > 1
		for (; region < BASE_HEAP[heap_idx].mm_nregions; region++)
#endif
		{
			/* A valid address from the user heap for this region would have to lie
			 * between the region's two guard nodes.
			 */
			if ((mem > (void *)BASE_HEAP[heap_idx].mm_heapstart[region]) && (mem < (void *)BASE_HEAP[heap_idx].mm_heapend[region])) {
				return heap_idx;
			}
		}
	}
	/* The address does not lie in the user heaps */
	return INVALID_HEAP_IDX;
}
