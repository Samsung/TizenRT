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
#include <assert.h>
#include <debug.h>

#include <tinyara/mm/mm.h>

#include "mm_node.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Upper bound on the hex dump printed when a corrupted chunk is found. A
 * chunk smaller than this is dumped in full so that the bytes on either side
 * of the corruption stay visible; a larger one is truncated. Without the
 * bound, a corrupted top of heap chunk dumps the whole remaining heap, one
 * line per 32 bytes, with the memory manager semaphore held.
 */

#define MM_UAF_DUMP_MAX		256

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_uaf_area
 *
 * Description:
 *   Locate the part of the free chunk 'node' which carries the poison
 *   pattern. It is the data area of the chunk, excluding the free node
 *   bookkeeping fields (flink/blink and the free debug info) which the
 *   allocator itself keeps using while the chunk sits in the free list.
 *
 *   Those fields end at SIZEOF_MM_FREENODE, which is a multiple of four in
 *   every supported configuration, so rounding that up to a whole word is all
 *   that is needed to keep the pattern reachable with aligned 32-bit accesses.
 *   Rounding up to the allocator granule instead, as MM_ALIGN_UP() does,
 *   pushes the window a further eight bytes into the chunk on the common
 *   HEAPINFO configuration and leaves a 32 byte chunk with no coverage at
 *   all.
 *
 *   The size is clamped to CONFIG_DEBUG_MM_UAF_POISON_SIZE and truncated to
 *   whole words, so nothing is ever written past the end of the chunk.
 *
 * Returned Value:
 *   The number of poisoned words, 0 if the chunk is too small to hold any.
 *   On a non-zero return, *poison points to the first poisoned word.
 *
 ****************************************************************************/

static size_t mm_uaf_area(struct mm_freenode_s *node, uint32_t **poison)
{
	size_t offset = MM_UAF_ALIGN_UP(SIZEOF_MM_FREENODE);
	size_t nbytes;

	*poison = (uint32_t *)((char *)node + offset);

	if (node->size <= offset) {
		/* The chunk is all bookkeeping, there is no room for the pattern */

		return 0;
	}

	nbytes = node->size - offset;
	if (nbytes > CONFIG_DEBUG_MM_UAF_POISON_SIZE) {
		nbytes = CONFIG_DEBUG_MM_UAF_POISON_SIZE;
	}

	return nbytes / sizeof(uint32_t);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_uaf_poison_range
 *
 * Description:
 *   Fill an explicit range with the poison pattern.
 *
 *   The range is passed in rather than derived from the node, because how
 *   much of a chunk may carry the pattern depends on why the chunk is not in
 *   use. One on the free list has to leave flink, blink and the free debug
 *   info alone; one held out of the free list altogether can carry the
 *   pattern across its whole user data area.
 *
 *   'nbytes' is truncated to whole words, so nothing outside the range is
 *   written.
 *
 *   NOTES:
 *     (1) the caller must hold the MM semaphore.
 *
 ****************************************************************************/

void mm_uaf_poison_range(FAR void *start, size_t nbytes)
{
	FAR uint32_t *poison = (FAR uint32_t *)start;
	size_t nwords = nbytes / sizeof(uint32_t);
	size_t i;

	for (i = 0; i < nwords; i++) {
		poison[i] = MM_UAF_PATTERN;
	}
}

/****************************************************************************
 * Name: mm_uaf_verify_range
 *
 * Description:
 *   Check that the pattern written by mm_uaf_poison_range() is still intact
 *   over an explicit range. Any difference means the memory was written while
 *   it was not in use, which is a use-after-free by whoever owned it before.
 *
 *   'node' is only used to describe the chunk in the report; the range does
 *   not have to start at the node.
 *
 *   NOTES:
 *     (1) the caller must hold the MM semaphore.
 *
 ****************************************************************************/

void mm_uaf_verify_range(FAR struct mm_allocnode_s *node, FAR void *start, size_t nbytes)
{
	FAR uint32_t *poison = (FAR uint32_t *)start;
	size_t nwords = nbytes / sizeof(uint32_t);
	size_t dumpsize;
	size_t i;

	for (i = 0; i < nwords; i++) {
		if (poison[i] != MM_UAF_PATTERN) {
			break;
		}
	}

	if (i == nwords) {
		/* Every word still holds the pattern, so nothing wrote into the chunk
		 * while it was free. An empty range lands here too.
		 */

		return;
	}

	/* Show the checked range whatever happens, plus as much of the rest of
	 * the chunk as the bound allows.
	 */

	dumpsize = (size_t)((FAR char *)start - (FAR char *)node) + nwords * sizeof(uint32_t);
	if (dumpsize < MM_UAF_DUMP_MAX) {
		dumpsize = MM_UAF_DUMP_MAX;
	}

	if (dumpsize > node->size) {
		dumpsize = node->size;
	}

	heap_dbg("#########################################################################################\n");
	mfdbg("ERROR: Use after free detected.\n");
	mm_dump_node(node, "FREE NODE");
	mm_dump_heap_region((uint32_t)node, (uint32_t)node + dumpsize);

#ifdef CONFIG_DEBUG_MM_UAF_PANIC
	PANIC();
#else
	mm_uaf_poison_range(start, nbytes);
#endif
}

/****************************************************************************
 * Name: mm_uaf_poison
 *
 * Description:
 *   Fill the data area of a chunk which is about to enter the free list with
 *   the poison pattern. Called for every chunk which becomes free, so that
 *   mm_uaf_verify() has a known content to compare against.
 *
 *   NOTES:
 *     (1) node->size must already be set to its final value.
 *     (2) the caller must hold the MM semaphore.
 *
 ****************************************************************************/

void mm_uaf_poison(FAR struct mm_freenode_s *node)
{
	uint32_t *poison;
	size_t nwords;

	nwords = mm_uaf_area(node, &poison);
	mm_uaf_poison_range(poison, nwords * sizeof(uint32_t));
}

/****************************************************************************
 * Name: mm_uaf_verify
 *
 * Description:
 *   Check that the poison pattern written by mm_uaf_poison() is still intact
 *   in a chunk which is about to be consumed. Any difference means that the
 *   chunk was written while it was free, which is a use-after-free by whoever
 *   owned it before.
 *
 *   NOTES:
 *     (1) must be called before the chunk is split, merged or resized, so
 *         that the checked area is still the one which was poisoned when the
 *         chunk was freed.
 *     (2) the caller must hold the MM semaphore.
 *
 ****************************************************************************/

void mm_uaf_verify(FAR struct mm_freenode_s *node)
{
	uint32_t *poison;
	size_t nwords;

	nwords = mm_uaf_area(node, &poison);
	mm_uaf_verify_range((FAR struct mm_allocnode_s *)node, poison, nwords * sizeof(uint32_t));
}
