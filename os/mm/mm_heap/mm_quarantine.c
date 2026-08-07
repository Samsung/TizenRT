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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/mm/mm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define QUARANTINE_SLOTS CONFIG_DEBUG_MM_QUARANTINE_CHUNKS

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: quarantine_area
 *
 * Description:
 *   Work out which part of a held chunk carries the poison pattern.
 *
 *   A held chunk keeps MM_ALLOC_BIT set and stays out of the free list, but
 *   when it is released from quarantine it will be converted to a free node
 *   and added to the free list. At that point, flink and blink pointers will
 *   be written into the space immediately following the alloc header.
 *
 *   Therefore, the poison pattern must start after SIZEOF_MM_FREENODE to
 *   avoid corrupting the flink/blink pointers that will be stored there.
 *   This ensures that when the chunk is released from quarantine and merged
 *   with neighbors, the free list manipulation macros can safely access
 *   these pointers.
 *
 *   The length is bounded by CONFIG_DEBUG_MM_QUARANTINE_MAX_SIZE, which
 *   decides whether a chunk is worth holding at all.
 *
 ****************************************************************************/

static void quarantine_area(FAR struct mm_allocnode_s *node, FAR void **start, FAR size_t *nbytes)
{
	/* Skip the entire free node header area to preserve space for flink/blink
	 * pointers that will be written when this chunk is released from quarantine
	 * and added to the free list.
	 */
	size_t offset = MM_UAF_ALIGN_UP(SIZEOF_MM_FREENODE);

	*start = (FAR void *)((FAR char *)node + offset);
	*nbytes = (node->size > offset) ? node->size - offset : 0;
}

/****************************************************************************
 * Name: quarantine_release_oldest
 *
 * Description:
 *   Take the oldest chunk out of the quarantine and give it back to the
 *   allocator. The poison pattern is checked on the way out: this is the
 *   point at which a write through a pointer freed long ago is found,
 *   because until now nothing else could have written to the chunk.
 *
 *   NOTES:
 *     (1) the quarantine must not be empty.
 *     (2) the caller must hold the MM semaphore.
 *
 ****************************************************************************/

static void quarantine_release_oldest(FAR struct mm_heap_s *heap)
{
	FAR struct mm_allocnode_s *node;
	FAR void *start;
	size_t nbytes;

	DEBUGASSERT(heap->mm_qcount > 0);

	node = heap->mm_quarantine[heap->mm_qhead];
	heap->mm_quarantine[heap->mm_qhead] = NULL;
	heap->mm_qhead = (heap->mm_qhead + 1) % QUARANTINE_SLOTS;
	heap->mm_qcount--;
	heap->mm_qbytes -= node->size;

	/* Check the chunk before anything else can touch it. Clear the allocated
	 * bit first so that a report describes it as a free node rather than an
	 * allocated one.
	 */

	node->preceding &= ~MM_ALLOC_BIT;

	quarantine_area(node, &start, &nbytes);
	mm_uaf_verify_range(node, start, nbytes);

	/* Now it may be merged with its neighbours and become reusable */

	mm_free_coalesce(heap, (FAR struct mm_freenode_s *)node);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_quarantine_init
 *
 * Description:
 *   Reset the quarantine of a heap. Called from mm_initialize().
 *
 ****************************************************************************/

void mm_quarantine_init(FAR struct mm_heap_s *heap)
{
	int i;

	for (i = 0; i < QUARANTINE_SLOTS; i++) {
		heap->mm_quarantine[i] = NULL;
	}

	heap->mm_qhead = 0;
	heap->mm_qcount = 0;
	heap->mm_qbytes = 0;
}

/****************************************************************************
 * Name: mm_quarantine_contains
 *
 * Description:
 *   Test whether a chunk is currently held in the quarantine.
 *
 *   A held chunk keeps MM_ALLOC_BIT set, so it is indistinguishable from a
 *   live allocation by its header alone and the usual double free test in
 *   mm_free() cannot see it. This scan is what makes that case detectable,
 *   and it is also what stops a second free from entering the same chunk
 *   into the ring twice. The ring is small and bounded, so the scan is a
 *   short walk over an array of pointers.
 *
 *   NOTES:
 *     (1) the caller must hold the MM semaphore.
 *
 ****************************************************************************/

bool mm_quarantine_contains(FAR struct mm_heap_s *heap, FAR struct mm_allocnode_s *node)
{
	uint16_t i;
	uint16_t slot;

	for (i = 0; i < heap->mm_qcount; i++) {
		slot = (heap->mm_qhead + i) % QUARANTINE_SLOTS;
		if (heap->mm_quarantine[slot] == node) {
			return true;
		}
	}

	return false;
}

/****************************************************************************
 * Name: mm_quarantine_add
 *
 * Description:
 *   Offer a chunk which has just been freed to the quarantine.
 *
 *   Returns true when the chunk has been taken, in which case the caller
 *   must not touch it any further: it keeps MM_ALLOC_BIT set and stays out
 *   of the free list, so its address cannot be handed to anybody else until
 *   it ages out. Returns false when the chunk should be freed normally.
 *
 *   A chunk larger than CONFIG_DEBUG_MM_QUARANTINE_MAX_SIZE is never taken.
 *   Holding a large buffer costs more of a small heap than the extra
 *   detection is worth.
 *
 *   NOTES:
 *     (1) node->size must already be set to its final value.
 *     (2) MM_ALLOC_BIT must still be set in node->preceding.
 *     (3) the caller must hold the MM semaphore.
 *
 ****************************************************************************/

bool mm_quarantine_add(FAR struct mm_heap_s *heap, FAR struct mm_allocnode_s *node)
{
	FAR void *start;
	size_t nbytes;
	uint16_t slot;

	if (node->size > CONFIG_DEBUG_MM_QUARANTINE_MAX_SIZE) {
		return false;
	}

	/* Make room. Releasing the oldest entries here is safe even though the
	 * caller's chunk may be adjacent to one of them: the caller's chunk still
	 * has MM_ALLOC_BIT set, so a released neighbour cannot merge into it.
	 */

	while (heap->mm_qcount >= QUARANTINE_SLOTS ||
		   heap->mm_qbytes + node->size > CONFIG_DEBUG_MM_QUARANTINE_BYTES) {
		if (heap->mm_qcount == 0) {
			/* The chunk on its own does not fit under the byte budget */

			return false;
		}

		quarantine_release_oldest(heap);
	}

	/* Lay down the pattern while nothing owns the chunk. The poison starts
	 * after SIZEOF_MM_FREENODE to avoid corrupting flink/blink pointers that
	 * will be stored when this chunk is released from quarantine and added
	 * to the free list.
	 */

	quarantine_area(node, &start, &nbytes);
	mm_uaf_poison_range(start, nbytes);

	slot = (heap->mm_qhead + heap->mm_qcount) % QUARANTINE_SLOTS;
	heap->mm_quarantine[slot] = node;
	heap->mm_qcount++;
	heap->mm_qbytes += node->size;

	return true;
}

/****************************************************************************
 * Name: mm_quarantine_flush
 *
 * Description:
 *   Release everything the quarantine is holding and return the number of
 *   bytes handed back to the allocator.
 *
 *   This is what keeps the quarantine from turning into spurious allocation
 *   failures: malloc calls it before giving up, so memory which is only held
 *   back for debugging is always available to a real request.
 *
 *   NOTES:
 *     (1) the caller must hold the MM semaphore.
 *
 ****************************************************************************/

size_t mm_quarantine_flush(FAR struct mm_heap_s *heap)
{
	size_t released = heap->mm_qbytes;

	while (heap->mm_qcount > 0) {
		quarantine_release_oldest(heap);
	}

	return released;
}
